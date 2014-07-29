
#include "sandboxrt.h"
#include "sha256.h"
#include "strstr.c"

static struct moxie_memory_map_ent *data;

static void find_data(void)
{
	data = moxie_memmap;
	while (data->addr) {
		if (strstr(data->tags, "data0,"))
			return;

		data++;
	}

	_exit(1);
}

static SHA256_CTX ctx;

static void hash_data(void)
{
	sha256_init(&ctx);
	sha256_update(&ctx, data->addr, data->length);
}

static void output_result(void)
{
	void *result = mmap(NULL, MACH_PAGE_SIZE,
			    MOXIE_PROT_EXEC|MOXIE_PROT_READ|MOXIE_PROT_WRITE,
			    MOXIE_MAP_PRIVATE|MOXIE_MAP_ANONYMOUS, 0, 0);
	if (result == (void *)-1)
		_exit(1);
	
	sha256_final(&ctx, result);

	setreturn(result, SHA256_BLOCK_SIZE);
}

int main (int argc, char *argv[])
{
	find_data();
	hash_data();
	output_result();
	return 0;
}

