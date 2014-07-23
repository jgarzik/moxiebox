
#include <stddef.h>
#include "sandboxrt.h"

int main (int argc, char *argv[])
{
	void *p = mmap(NULL, 0, 0, 0, 0, 0);
	_exit(0);
	return 0;

	(void) p;
}

