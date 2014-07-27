#ifndef __SANDBOX_MOXIE_H__
#define __SANDBOX_MOXIE_H__

#include <stdint.h>

typedef int32_t word;
typedef uint32_t uword;

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

#endif // __SANBOX_MOXIE_H__
