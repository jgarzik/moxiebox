#ifndef __SANDBOX_H__
#define __SANDBOX_H__

#include <vector>
#include <string>
#include <string.h>
#include <stdint.h>

typedef int32_t word;
typedef uint32_t uword;

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

/* The machine state.

   This state is maintained in host byte order.  The fetch/store
   register functions must translate between host byte order and the
   target processor byte order.  Keeping this data in target byte
   order simplifies the register read/write functions.  Keeping this
   data in native order improves the performance of the simulator.
   Simulation speed is deemed more important.  */

enum {
	NUM_MOXIE_REGS = 17, /* Including PC */
	NUM_MOXIE_SREGS = 256, /* The special registers */
	PC_REGNO     = 16,
};

/* The ordering of the moxie_regset structure is matched in the
   gdb/config/moxie/tm-moxie.h file in the REGISTER_NAMES macro.  */
struct moxie_regset
{
  word		  regs[NUM_MOXIE_REGS + 1]; /* primary registers */
  word		  sregs[256];             /* special registers */
  word            cc;                   /* the condition code reg */
  int		  exception;
  unsigned long long insts;                /* instruction counter */
};

enum {
	CC_GT  = (1<<0),
	CC_LT  = (1<<1),
	CC_EQ  = (1<<2),
	CC_GTU = (1<<3),
	CC_LTU = (1<<4),
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
};

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
