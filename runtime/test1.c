
#include <stddef.h>
#include "sandboxrt.h"

int prot = MOXIE_PROT_READ | MOXIE_PROT_WRITE | MOXIE_PROT_EXEC;
int flags = MOXIE_MAP_PRIVATE | MOXIE_MAP_ANONYMOUS;
static void *p;
static const int MAP_SIZE = 0x10000;

static void do_setup(void)
{
	p = mmap(NULL, MAP_SIZE, prot, flags, 0, 0);
}

static void do_accesses(void)
{
	unsigned char *c = p;

	c[10] = 30;
	c[20] = 60;
	c[100] = 99;

	setreturn(&c[100], 1);
}

static int do_memmap_accesses(void)
{
	int count = 0;
	struct moxie_memory_map_ent *ent = moxie_memmap;
	while (ent->addr) {
		count++;
		ent++;
	}

	return count;
}

static void fini(void)
{
	_exit(0);
}

int main (int argc, char *argv[])
{
	do_setup();
	do_accesses();
	do_memmap_accesses();
	fini();
	return 0;

	(void) p;
}

