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

class cpuState {
public:
	struct moxie_regset asregs;

	cpuState() {
		memset(&asregs, 0, sizeof(asregs));
	}
};

class addressRange {
public:
	std::string name;
	uint32_t start;
	uint32_t end;
	uint32_t length;

	addressRange() {
		name = "ar";
		start = 0;
		end = 0;
		length = 0;
	}

	virtual bool read8(uint32_t addr, uint32_t& val_out) {
		val_out = 0xffffffffU;
		return false;
	}
	virtual bool read16(uint32_t addr, uint32_t& val_out) {
		val_out = 0xffffffffU;
		return false;
	}
	virtual bool read32(uint32_t addr, uint32_t& val_out) {
		val_out = 0xffffffffU;
		return false;
	}

	virtual bool write8(uint32_t addr, uint32_t val) { return false; }
	virtual bool write16(uint32_t addr, uint32_t val) { return false; }
	virtual bool write32(uint32_t addr, uint32_t val) { return false; }

	bool inRange(uint32_t addr, uint32_t len) {
		return ((addr >= start) &&
			((addr + len) <= end));		// warn: overflow
	}
};

class roDataRange : public addressRange {
public:
	std::string buf;

	roDataRange(size_t sz) {
		name = "ro";
		start = 0;
		end = 0;
		length = sz;
	}

	bool read8(uint32_t addr, uint32_t& val_out) {
		uint8_t val8;
		memcpy(&val8, &buf[addr], sizeof(val8));
		val_out = val8;
		return true;
	}
	bool read16(uint32_t addr, uint32_t& val_out) {
		uint16_t val16;
		memcpy(&val16, &buf[addr], sizeof(val16));
		val_out = val16;
		return true;
	}
	bool read32(uint32_t addr, uint32_t& val_out) {
		memcpy(&val_out, &buf[addr], sizeof(val_out));
		return true;
	}
};

class machine {
public:
	uint32_t startAddr;
	std::vector<addressRange*> memmap;
	cpuState cpu;
	bool tracing;

	machine() {
		startAddr = 0;
		tracing = false;
	}

	bool read8(uint32_t addr, uint32_t& val_out);
	bool read16(uint32_t addr, uint32_t& val_out);
	bool read32(uint32_t addr, uint32_t& val_out);

	bool write8(uint32_t addr, uint32_t val);
	bool write16(uint32_t addr, uint32_t val);
	bool write32(uint32_t addr, uint32_t val);

	void sortMemMap();
};

extern void sim_resume (machine& mach, unsigned long long cpu_budget = 0);
extern bool loadElfProgram(machine& mach, const char *filename);

#endif // __SANDBOX_H__
