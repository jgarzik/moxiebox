
#include <string>
#include <stdio.h>
#include "sandbox.h"

uint32_t sim_core_read_aligned_1(machine& mach,
				     address_word& cia,
				     enum map_direction mdir,
				     uint32_t addr)
{
	return 0; //FIXME
}

uint32_t sim_core_read_aligned_2(machine& mach,
				     address_word& cia,
				     enum map_direction mdir,
				     uint32_t addr)
{
	return 0; //FIXME
}

uint32_t sim_core_read_aligned_4(machine& mach,
				     address_word& cia,
				     enum map_direction mdir,
				     uint32_t addr)
{
	return 0; //FIXME
}


void sim_core_write_aligned_1(machine& mach,
				     address_word& cia,
				     enum map_direction mdir,
				     uint32_t addr,
				     uint32_t val)
{
	// FIXME
}

void sim_core_write_aligned_2(machine& mach,
				     address_word& cia,
				     enum map_direction mdir,
				     uint32_t addr,
				     uint32_t val)
{
	// FIXME
}

void sim_core_write_aligned_4(machine& mach,
				     address_word& cia,
				     enum map_direction mdir,
				     uint32_t addr,
				     uint32_t val)
{
	// FIXME
}


int main (int argc, char *argv[])
{
	machine mach;
	sim_resume(mach);
	return 0;
}
