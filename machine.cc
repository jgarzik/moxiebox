
#include <algorithm>
#include "sandbox.h"

void *machine::physaddr(uint32_t addr, size_t objLen, bool wantWrite)
{
	for (unsigned int i = 0; i < memmap.size(); i++) {
		addressRange* mr = memmap[i];
		if (mr->inRange(addr, objLen)) {
			if (wantWrite && mr->readOnly)
				return NULL;
			return mr->physaddr(addr);
		}
	}

	return NULL;
}

bool machine::read8(uint32_t addr, uint32_t& val_out)
{
	uint8_t *paddr = (uint8_t *) physaddr(addr, 1);
	if (!paddr)
		return false;

	val_out = *paddr;
	return true;
}

bool machine::read16(uint32_t addr, uint32_t& val_out)
{
	uint16_t *paddr = (uint16_t *) physaddr(addr, 2);
	if (!paddr)
		return false;

	val_out = *paddr;
	return true;
}

bool machine::read32(uint32_t addr, uint32_t& val_out)
{
	uint32_t *paddr = (uint32_t *) physaddr(addr, 4);
	if (!paddr)
		return false;

	val_out = *paddr;
	return true;
}

bool machine::write8(uint32_t addr, uint32_t val)
{
	uint8_t *paddr = (uint8_t *) physaddr(addr, 1, true);
	if (!paddr)
		return false;

	*paddr = (uint8_t) val;
	return true;
}

bool machine::write16(uint32_t addr, uint32_t val)
{
	uint16_t *paddr = (uint16_t *) physaddr(addr, 2, true);
	if (!paddr)
		return false;

	*paddr = (uint16_t) val;
	return true;
}

bool machine::write32(uint32_t addr, uint32_t val)
{
	uint32_t *paddr = (uint32_t *) physaddr(addr, 4, true);
	if (!paddr)
		return false;

	*paddr = val;
	return true;
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

