#ifndef __SANDBOX_H__
#define __SANDBOX_H__

#include <vector>
#include <stdint.h>

enum map_direction {
	read_map,
	write_map,
};

class address_word {
public:
	int dummy;

	address_word() { dummy = 0; }
};

class addressRange {
public:
	uint32_t start;
	uint32_t end;
	uint32_t length;

	bool (*read8)(uint32_t addr, uint32_t& val_out);
	bool (*read16)(uint32_t addr, uint32_t& val_out);
	bool (*read32)(uint32_t addr, uint32_t& val_out);

	bool (*write8)(uint32_t addr, uint32_t val);
	bool (*write16)(uint32_t addr, uint32_t val);
	bool (*write32)(uint32_t addr, uint32_t val);

	bool inRange(uint32_t addr, uint32_t len) {
		return ((addr >= start) &&
			((addr + len) <= end));		// warn: overflow
	}
};

class machine {
public:
	std::vector<addressRange> memmap;

	bool read8(uint32_t addr, uint32_t& val_out);
	bool read16(uint32_t addr, uint32_t& val_out);
	bool read32(uint32_t addr, uint32_t& val_out);

	bool write8(uint32_t addr, uint32_t val);
	bool write16(uint32_t addr, uint32_t val);
	bool write32(uint32_t addr, uint32_t val);
};

extern void set_initial_gprs();
extern void sim_resume (machine& mach);

extern uint32_t sim_core_read_aligned_1(machine& mach,
				     address_word& cia,
				     enum map_direction mdir,
				     uint32_t addr);
extern uint32_t sim_core_read_aligned_2(machine& mach,
				     address_word& cia,
				     enum map_direction mdir,
				     uint32_t addr);
extern uint32_t sim_core_read_aligned_4(machine& mach,
				     address_word& cia,
				     enum map_direction mdir,
				     uint32_t addr);

extern void sim_core_write_aligned_1(machine& mach,
				     address_word& cia,
				     enum map_direction mdir,
				     uint32_t addr,
				     uint32_t val);
extern void sim_core_write_aligned_2(machine& mach,
				     address_word& cia,
				     enum map_direction mdir,
				     uint32_t addr,
				     uint32_t val);
extern void sim_core_write_aligned_4(machine& mach,
				     address_word& cia,
				     enum map_direction mdir,
				     uint32_t addr,
				     uint32_t val);

#endif // __SANDBOX_H__
