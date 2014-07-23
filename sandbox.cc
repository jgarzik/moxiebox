
#include <string>
#include <vector>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "sandbox.h"

using namespace std;

static const uint32_t RAM_SIZE = 16 * 1024 * 1024;

uint32_t sim_core_read_aligned_1(machine& mach,
				     uint32_t addr)
{
	uint32_t ret;
	if (!mach.read8(addr, ret))
		return 0xffffffffU;
	return ret;
}

uint32_t sim_core_read_aligned_2(machine& mach,
				     uint32_t addr)
{
	uint32_t ret;
	if (!mach.read16(addr, ret))
		return 0xffffffffU;
	return ret;
}

uint32_t sim_core_read_aligned_4(machine& mach,
				     uint32_t addr)
{
	uint32_t ret;
	if (!mach.read32(addr, ret))
		return 0xffffffffU;
	return ret;
}


void sim_core_write_aligned_1(machine& mach,
				     uint32_t addr,
				     uint32_t val)
{
	mach.write8(addr, val);
	// FIXME handle failure
}

void sim_core_write_aligned_2(machine& mach,
				     uint32_t addr,
				     uint32_t val)
{
	mach.write16(addr, val);
	// FIXME handle failure
}

void sim_core_write_aligned_4(machine& mach,
				     uint32_t addr,
				     uint32_t val)
{
	mach.write32(addr, val);
	// FIXME handle failure
}

bool machine::read8(uint32_t addr, uint32_t& val_out)
{
	for (unsigned int i = 0; i < memmap.size(); i++) {
		addressRange& mr = memmap[i];
		if (mr.inRange(addr, 1))
			return mr.read8(addr - mr.start, val_out);
	}

	return false;
}

bool machine::read16(uint32_t addr, uint32_t& val_out)
{
	for (unsigned int i = 0; i < memmap.size(); i++) {
		addressRange& mr = memmap[i];
		if (mr.inRange(addr, 1))
			return mr.read16(addr - mr.start, val_out);
	}

	return false;
}

bool machine::read32(uint32_t addr, uint32_t& val_out)
{
	for (unsigned int i = 0; i < memmap.size(); i++) {
		addressRange& mr = memmap[i];
		if (mr.inRange(addr, 1))
			return mr.read32(addr - mr.start, val_out);
	}

	return false;
}

bool machine::write8(uint32_t addr, uint32_t val)
{
	for (unsigned int i = 0; i < memmap.size(); i++) {
		addressRange& mr = memmap[i];
		if (mr.inRange(addr, 1))
			return mr.write8(addr - mr.start, val);
	}

	return false;
}

bool machine::write16(uint32_t addr, uint32_t val)
{
	for (unsigned int i = 0; i < memmap.size(); i++) {
		addressRange& mr = memmap[i];
		if (mr.inRange(addr, 1))
			return mr.write16(addr - mr.start, val);
	}

	return false;
}

bool machine::write32(uint32_t addr, uint32_t val)
{
	for (unsigned int i = 0; i < memmap.size(); i++) {
		addressRange& mr = memmap[i];
		if (mr.inRange(addr, 1))
			return mr.write32(addr - mr.start, val);
	}

	return false;
}

bool err_write(uint32_t addr, uint32_t val)
{
	return false;
}

bool zp_read8(uint32_t addr, uint32_t& val_out)
{
	val_out = 0;
	return true;
}

void addZeroPage(machine& mach)
{
	addressRange zp;

	zp.start = 0x0U;
	zp.length = MACH_PAGE_SIZE;
	zp.end = zp.start + zp.length;

	zp.read8 = zp_read8;
	zp.read16 = zp_read8;
	zp.read32 = zp_read8;
	zp.write8 = err_write;
	zp.write16 = err_write;
	zp.write32 = err_write;

	mach.memmap.push_back(zp);
}

vector<string> inputFiles;

bool file_read32(uint32_t addr, uint32_t& val_out)
{
	string& data = inputFiles[0];	// FIXME

	if ((addr + sizeof(uint32_t)) > data.size())
		return false;

	memcpy(&val_out, &data[addr], sizeof(uint32_t));
	return true;
}

bool loadRawData(machine& mach, const char *filename)
{
	string fileData;

	FILE *f = fopen(filename, "r");
	if (!f) {
		perror(filename);
		return false;
	}
	
	char line[512];
	while (fgets(line, sizeof(line), f) != NULL) {
		fileData += line;
	}

	if (ferror(f)) {
		perror(filename);
		return false;
	}

	fclose(f);

	uint32_t last_end = mach.memmap.back().end;

	addressRange fr;

	fr.start = (last_end + (MACH_PAGE_SIZE * 2)) & ~(MACH_PAGE_SIZE-1);
	fr.length = fileData.size();
	fr.end = fr.start + fr.length;

	fr.read8 = file_read32;
	fr.read16 = file_read32;
	fr.read32 = file_read32;
	fr.write8 = err_write;
	fr.write16 = err_write;
	fr.write32 = err_write;

	mach.memmap.push_back(fr);

	return true;
}

static void usage(const char *progname)
{
	fprintf(stderr,
		"Usage: %s [options]\n"
		"\n"
		"options:\n"
		"-e <moxie executable>\tLoad executable into address space\n"
		"-d <file>\t\tLoad data into address space\n"
		,
		progname);
}

int main (int argc, char *argv[])
{
	machine mach;

	addZeroPage(mach);

	int opt;
	while ((opt = getopt(argc, argv, "e:d:")) != -1) {
		switch(opt) {
		case 'e':
			if (!loadElfProgram(mach, optarg)) {
				fprintf(stderr, "ELF load failed for %s\n",
					optarg);
				exit(EXIT_FAILURE);
			}
			break;
		case 'd':
			if (!loadRawData(mach, optarg)) {
				fprintf(stderr, "Data load failed for %s\n",
					optarg);
				exit(EXIT_FAILURE);
			}
			break;
		default:
			usage(argv[0]);
			exit(EXIT_FAILURE);
		}
	}

	sim_resume(mach);
	return 0;
}

