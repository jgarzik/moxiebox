
#include <algorithm>
#include "sandbox.h"

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

static bool memmapCmp(addressRange *a, addressRange *b)
{
	return (a->start < b->start);
}

void machine::sortMemMap()
{
	std::sort(memmap.begin(), memmap.end(), memmapCmp);
}

bool machine::mapInsert(addressRange *rdr)
{
	addressRange *ar = memmap.back();
	rdr->start = (ar->end + (MACH_PAGE_SIZE * 2)) & ~(MACH_PAGE_SIZE-1);
	rdr->end = rdr->start + rdr->length;
	memmap.push_back(rdr);

	return true;
}

