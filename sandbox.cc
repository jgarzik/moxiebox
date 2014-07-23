
#include <string>
#include <vector>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "sandbox.h"

using namespace std;

static const uint32_t RAM_SIZE = 16 * 1024 * 1024;

uint32_t sim_core_read_aligned_1(machine& mach,
				     address_word& cia,
				     enum map_direction mdir,
				     uint32_t addr)
{
	uint32_t ret;
	if (!mach.read8(addr, ret))
		return 0xffffffffU;
	return ret;
}

uint32_t sim_core_read_aligned_2(machine& mach,
				     address_word& cia,
				     enum map_direction mdir,
				     uint32_t addr)
{
	uint32_t ret;
	if (!mach.read16(addr, ret))
		return 0xffffffffU;
	return ret;
}

uint32_t sim_core_read_aligned_4(machine& mach,
				     address_word& cia,
				     enum map_direction mdir,
				     uint32_t addr)
{
	uint32_t ret;
	if (!mach.read32(addr, ret))
		return 0xffffffffU;
	return ret;
}


void sim_core_write_aligned_1(machine& mach,
				     address_word& cia,
				     enum map_direction mdir,
				     uint32_t addr,
				     uint32_t val)
{
	mach.write8(addr, val);
	// FIXME handle failure
}

void sim_core_write_aligned_2(machine& mach,
				     address_word& cia,
				     enum map_direction mdir,
				     uint32_t addr,
				     uint32_t val)
{
	mach.write16(addr, val);
	// FIXME handle failure
}

void sim_core_write_aligned_4(machine& mach,
				     address_word& cia,
				     enum map_direction mdir,
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
	zp.length = 4096;
	zp.end = zp.start + zp.length;

	zp.read8 = zp_read8;
	zp.read16 = zp_read8;
	zp.read32 = zp_read8;
	zp.write8 = err_write;
	zp.write16 = err_write;
	zp.write32 = err_write;

	mach.memmap.push_back(zp);
}

static unsigned char ramBuffer[RAM_SIZE];

bool ram_read32(uint32_t addr, uint32_t& val_out)
{
	if ((addr + sizeof(uint32_t)) > RAM_SIZE)
		return false;

	memcpy(&val_out, ramBuffer + addr, sizeof(uint32_t));
	return true;
}

bool ram_write32(uint32_t addr, uint32_t val)
{
	if ((addr + sizeof(uint32_t)) > RAM_SIZE)
		return false;

	memcpy(ramBuffer + addr, &val, sizeof(uint32_t));
	return true;
}

void addRam(machine& mach)
{
        addressRange ram;

	ram.start = 16 * 1024 * 1024;
	ram.length = RAM_SIZE;
	ram.end = ram.start + ram.length;

	ram.read8 = ram_read32;
	ram.read16 = ram_read32;
	ram.read32 = ram_read32;
	ram.write8 = ram_write32;
	ram.write16 = ram_write32;
	ram.write32 = ram_write32;

	mach.memmap.push_back(ram);
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

void loadFile(machine& mach, const char *filename)
{
	string fileData;

	FILE *f = fopen(filename, "r");
	if (!f) {
		perror(filename);
		exit(EXIT_FAILURE);
	}
	
	char line[512];
	while (fgets(line, sizeof(line), f) != NULL) {
		fileData += line;
	}

	if (ferror(f)) {
		perror(filename);
		exit(EXIT_FAILURE);
	}

	fclose(f);

	uint32_t last_end = mach.memmap.back().end;

	addressRange fr;

	fr.start = (last_end + (4096 * 2)) & ~(4096-1);
	fr.length = fileData.size();
	fr.end = fr.start + fr.length;

	fr.read8 = file_read32;
	fr.read16 = file_read32;
	fr.read32 = file_read32;
	fr.write8 = err_write;
	fr.write16 = err_write;
	fr.write32 = err_write;

	mach.memmap.push_back(fr);
}

int main (int argc, char *argv[])
{
	machine mach;

	addZeroPage(mach);
	addRam(mach);

	for (int i = 1; i < argc; i++) {
		loadFile(mach, argv[i]);
	}

	sim_resume(mach);
	return 0;
}

