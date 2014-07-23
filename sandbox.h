#ifndef __SANDBOX_H__
#define __SANDBOX_H__

#include <vector>
#include <string>
#include <string.h>
#include <stdint.h>

enum {
	MACH_PAGE_SIZE = 4096,
};

// TODO: check w/ ABI
enum moxie_mmap_prot_flags {
	MOXIE_PROT_EXEC = (1U << 0),
	MOXIE_PROT_READ = (1U << 1),
	MOXIE_PROT_WRITE = (1U << 2),
	MOXIE_PROT_NONE = 0U,
};

// TODO: check w/ ABI
enum moxie_mmap_flags {
	MOXIE_MAP_SHARED = (1U << 0),
	MOXIE_MAP_PRIVATE = (1U << 1),
	MOXIE_MAP_ANONYMOUS = (1U << 2),
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

	bool read8(uint32_t addr, uint32_t& val_out) {
		val_out = 0xffffffffU;
		return false;
	}
	bool read16(uint32_t addr, uint32_t& val_out) {
		val_out = 0xffffffffU;
		return false;
	}
	bool read32(uint32_t addr, uint32_t& val_out) {
		val_out = 0xffffffffU;
		return false;
	}

	bool write8(uint32_t addr, uint32_t val) { return false; }
	bool write16(uint32_t addr, uint32_t val) { return false; }
	bool write32(uint32_t addr, uint32_t val) { return false; }

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
		return read32(addr, val_out);
	}
	bool read16(uint32_t addr, uint32_t& val_out) {
		return read32(addr, val_out);
	}
	bool read32(uint32_t addr, uint32_t& val_out) {
		memcpy(&val_out, &buf[addr], sizeof(val_out));
		return true;
	}
};

class machine {
public:
	std::vector<addressRange*> memmap;

	bool read8(uint32_t addr, uint32_t& val_out);
	bool read16(uint32_t addr, uint32_t& val_out);
	bool read32(uint32_t addr, uint32_t& val_out);

	bool write8(uint32_t addr, uint32_t val);
	bool write16(uint32_t addr, uint32_t val);
	bool write32(uint32_t addr, uint32_t val);
};

extern void set_initial_gprs();
extern void sim_resume (machine& mach, unsigned long long cpu_budget = 0);
extern bool loadElfProgram(machine& mach, const char *filename);

extern uint32_t sim_core_read_aligned_1(machine& mach,
				     uint32_t addr);
extern uint32_t sim_core_read_aligned_2(machine& mach,
				     uint32_t addr);
extern uint32_t sim_core_read_aligned_4(machine& mach,
				     uint32_t addr);

extern void sim_core_write_aligned_1(machine& mach,
				     uint32_t addr,
				     uint32_t val);
extern void sim_core_write_aligned_2(machine& mach,
				     uint32_t addr,
				     uint32_t val);
extern void sim_core_write_aligned_4(machine& mach,
				     uint32_t addr,
				     uint32_t val);

#endif // __SANDBOX_H__
