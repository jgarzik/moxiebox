
#include <stddef.h>
#include "sandboxrt.h"

int prot = 0x11;
int flags = 0;

int main (int argc, char *argv[])
{
	void *p = mmap(NULL, 0, prot, flags, 0, 0);
	_exit(0);
	return 0;

	(void) p;
}

