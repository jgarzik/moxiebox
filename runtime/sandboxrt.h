#ifndef __SANDBOXRT_H__
#define __SANDBOXRT_H__

extern void _exit(int status);
extern void *mmap(void *addr, size_t length, int prot, int flags,
		  int fd, /*off_t*/ int offset);

#endif // __SANDBOXRT_H__
