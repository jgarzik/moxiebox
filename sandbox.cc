
#include <string>
#include <stdio.h>
#include "sandbox.h"

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

void machine::loadFile(const char *filename)
{
}

int main (int argc, char *argv[])
{
	machine mach;

	for (int i = 1; i < argc; i++) {
		mach.loadFile(argv[i]);
	}

	sim_resume(mach);
	return 0;
}
