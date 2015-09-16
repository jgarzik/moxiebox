#ifndef __SANDBOXRT_H__
#define __SANDBOXRT_H__

#include <stddef.h>

struct moxie_memory_map_ent {
	void *addr;
	size_t length;
	char tags[32 - 4 - 4];
};

enum {
	MACH_PAGE_SIZE = 4096U,
	MACH_MEMMAP_ADDR = 0x3d0000U,
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

// moxie-specific environment
extern struct moxie_memory_map_ent *moxie_memmap;
extern void setreturn(void *addr, size_t length);
extern void _exit(int status);
extern void *mmap(void *addr, size_t length, int prot, int flags,
		  int fd, /*off_t*/ int offset);

// ISO C assert.h
#ifndef assert
#  ifdef NDEBUG
#    define assert(expr) {}
#  else
#    define assert(expr) { if (!(expr)) abort(); }
#  endif
#endif

// ISO C stdlib.h
static inline void abort(void) { _exit(1); }
static inline void exit(int status) { _exit(status); }

// ISO C string.h
extern void *memchr(const void *s, int c, size_t n);
extern int memcmp(const void *s1, const void *s2, size_t n);
extern void *memcpy(void *dest, const void *src, size_t n);
extern void *memset(void *s, int c, size_t n);

extern char *strchr(const char *s, int c);
extern int strcmp(const char *s1, const char *s2);
extern char *strcpy(char *dest, const char *src);
extern size_t strlen(const char *s);
extern char *strncpy(char *dest, const char *src, size_t n);
extern char *strcpy(char *dest, const char *src);
extern char *strstr(const char *haystack, const char *needle);

#endif // __SANDBOXRT_H__
