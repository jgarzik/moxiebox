#ifndef __SANDBOX_H__
#define __SANDBOX_H__

#include <vector>
#include <string>
#include <string.h>
#include <stdint.h>
#include "moxie.h"

enum {
	MACH_PAGE_SIZE = 4096,
};

struct mach_memmap_ent {
	uint32_t vaddr;
	uint32_t length;
	char tags[32 - 4 - 4];
};

class cpuState {
public:
	struct moxie_regset asregs;

	cpuState() {
		memset(&asregs, 0, sizeof(asregs));
	}
};

class addressRange {
public:
	uint32_t start;
	uint32_t end;
	uint32_t length;
	void *root;
	bool readOnly;
	std::string buf;

	addressRange(size_t sz) {
		start = 0;
		end = 0;
		length = sz;
		root = NULL;
		readOnly = true;
	}

	void *physaddr(uint32_t addr) {
		uint32_t offset = addr - start;
		return (char *)root + offset;
	}

	bool inRange(uint32_t addr, uint32_t len) {
		return ((addr >= start) &&
			((addr + len) <= end));		// warn: overflow
	}

	void updateRoot() { root = &buf[0]; }
};

class machine {
public:
	std::vector<addressRange*> memmap;
	cpuState cpu;

	uint32_t startAddr;
	bool tracing;
	uint32_t heapAvail;

	machine() {
		startAddr = 0;
		tracing = false;
		heapAvail = 0xfffffffU;
	}

	bool read8(uint32_t addr, uint32_t& val_out);
	bool read16(uint32_t addr, uint32_t& val_out);
	bool read32(uint32_t addr, uint32_t& val_out);

	bool write8(uint32_t addr, uint32_t val);
	bool write16(uint32_t addr, uint32_t val);
	bool write32(uint32_t addr, uint32_t val);

	void *physaddr(uint32_t addr, size_t objLen, bool wantWrite = false);
	void sortMemMap();
	bool mapInsert(addressRange *ar);
	void fillDescriptors(std::vector<struct mach_memmap_ent>& desc);
};

extern void sim_resume (machine& mach, unsigned long long cpu_budget = 0);
extern bool loadElfProgram(machine& mach, const char *filename);

#endif // __SANDBOX_H__
