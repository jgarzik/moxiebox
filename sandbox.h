#ifndef __DEMO_H__
#define __DEMO_H__

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

class machine {
public:
	int dummy;

	machine() { dummy = 0; }
};

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

#endif // __DEMO_H__
