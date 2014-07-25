#ifndef __SANDBOXRT_H__
#define __SANDBOXRT_H__

#include <stddef.h>

struct moxie_memory_map_ent {
	void *addr;
	size_t length;
	char tags[32 - 4 - 4];
};

enum {
	MOXIE_PAGE_SIZE = 4096U,
	MOXIE_MEMMAP_ADDR = 0x400000U + MOXIE_PAGE_SIZE,
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

extern struct moxie_memory_map_ent *moxie_memmap;
extern void _exit(int status);
extern void *mmap(void *addr, size_t length, int prot, int flags,
		  int fd, /*off_t*/ int offset);

#endif // __SANDBOXRT_H__
