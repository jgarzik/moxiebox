
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

static void fini(void)
{
	exit(0);
}

static void test_string_func(void)
{
	const char *teststr = "We are Motorhead";
	const char *teststr2 = "We are Motorhead";
	char buf[32];
	unsigned int i;

	// strlen
	size_t s_len = strlen(teststr);
	assert(s_len == 16);

	// memchr
	char *p = memchr(teststr, ' ', s_len);
	assert(p == (teststr + 2));

	// memcmp
	int rc = memcmp(teststr, teststr2, s_len + 1);
	assert(rc == 0);

	// memcpy
	memcpy(buf, teststr, s_len + 1);
	assert(strlen(buf) == s_len);
	// FIXME assert(strcmp(buf, teststr) == 0);

	// memset
	memset(buf, ' ', sizeof(buf));
	for (i = 0; i < sizeof(buf); i++)
		assert(buf[i] == ' ');

	// strchr
	p = strchr(teststr, ' ');
	assert(p == (teststr + 2));

	// strcmp
	assert(strcmp(teststr, teststr2) == 0);

	// strcpy
	strcpy(buf, teststr);
	assert(strlen(buf) == strlen(teststr));
	// FIXME assert(strcmp(buf, teststr) == 0);

	// strncpy
	strncpy(buf, teststr, 3);
	buf[3] = 0;
	assert(strlen(buf) == 3);
	// FIXME assert(memcmp(buf, teststr, 3) == 0);

	// strstr
	const char *needle = "are";
	p = strstr(teststr, needle);
	assert(p == (teststr + 3));
}

int main (int argc, char *argv[])
{
	test_string_func();
	do_setup();
	fini();
	return 0;

	(void) p;
}

