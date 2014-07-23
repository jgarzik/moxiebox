
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
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
		addressRange* mr = memmap[i];
		if (mr->inRange(addr, 1))
			return mr->read8(addr - mr->start, val_out);
	}

	return false;
}

bool machine::read16(uint32_t addr, uint32_t& val_out)
{
	for (unsigned int i = 0; i < memmap.size(); i++) {
		addressRange* mr = memmap[i];
		if (mr->inRange(addr, 1))
			return mr->read16(addr - mr->start, val_out);
	}

	return false;
}

bool machine::read32(uint32_t addr, uint32_t& val_out)
{
	for (unsigned int i = 0; i < memmap.size(); i++) {
		addressRange* mr = memmap[i];
		if (mr->inRange(addr, 1))
			return mr->read32(addr - mr->start, val_out);
	}

	return false;
}

bool machine::write8(uint32_t addr, uint32_t val)
{
	for (unsigned int i = 0; i < memmap.size(); i++) {
		addressRange* mr = memmap[i];
		if (mr->inRange(addr, 1))
			return mr->write8(addr - mr->start, val);
	}

	return false;
}

bool machine::write16(uint32_t addr, uint32_t val)
{
	for (unsigned int i = 0; i < memmap.size(); i++) {
		addressRange* mr = memmap[i];
		if (mr->inRange(addr, 1))
			return mr->write16(addr - mr->start, val);
	}

	return false;
}

bool machine::write32(uint32_t addr, uint32_t val)
{
	for (unsigned int i = 0; i < memmap.size(); i++) {
		addressRange* mr = memmap[i];
		if (mr->inRange(addr, 1))
			return mr->write32(addr - mr->start, val);
	}

	return false;
}

class zeroPageRange : public addressRange {
public:
	zeroPageRange() {
		start = 0x0U;
		length = MACH_PAGE_SIZE;
		end = start + length;
	}

	bool read8(uint32_t addr, uint32_t& val_out) {
		return read32(addr, val_out);
	}
	bool read16(uint32_t addr, uint32_t& val_out) {
		return read32(addr, val_out);
	}
	bool read32(uint32_t addr, uint32_t& val_out) {
		val_out = 0;
		return true;
	}
};

void addZeroPage(machine& mach)
{
	zeroPageRange *zpr = new zeroPageRange();

	mach.memmap.push_back(zpr);
}

class rwDataRange : public roDataRange {
public:
	rwDataRange(size_t sz) : roDataRange(sz) {
		start = 0;
		end = 0;
		length = sz;
	}

	bool write8(uint32_t addr, uint32_t val_in) {
		uint8_t val = val_in;
		memcpy(&buf[addr], &val, sizeof(val));
		return true;
	}
	bool write16(uint32_t addr, uint32_t val_in) {
		uint16_t val = val_in;
		memcpy(&buf[addr], &val, sizeof(val));
		return true;
	}
	bool write32(uint32_t addr, uint32_t val_in) {
		memcpy(&buf[addr], &val_in, sizeof(val_in));
		return true;
	}
};

bool loadRawData(machine& mach, const char *filename)
{
	int fd;
	if (( fd = open ( filename, O_RDONLY , 0)) < 0)
		return false;

	struct stat st;
	if (fstat(fd, &st) < 0) {
		close(fd);
		return false;
	}

	void *p;
	p = mmap(NULL, st.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
	if (p == (void *)-1) {
		close(fd);
		return false;
	}

	rwDataRange *rdr = new rwDataRange(st.st_size);

	rdr->buf.assign((char *) p, (size_t) st.st_size);

	munmap(p, st.st_size);
	close(fd);

	addressRange *ar = mach.memmap.back();
	rdr->start = ar->end + (MACH_PAGE_SIZE * 2);
	rdr->end = rdr->start + rdr->length;

	mach.memmap.push_back(rdr);

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

