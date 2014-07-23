
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <libelf.h>
#include <gelf.h>
#include <sys/mman.h>
#include <stdio.h>
#include "sandbox.h"

bool loadElfProgram(machine& mach, const char *filename)
{
	if ( elf_version ( EV_CURRENT ) == EV_NONE )
		return false;

	int fd;
	if (( fd = open ( filename, O_RDONLY , 0)) < 0)
		return false;

	struct stat st;
	if (fstat(fd, &st) < 0)
		goto err_out;

	void *p;
	p = mmap(NULL, st.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
	if (p == (void *)-1)
		goto err_out;

	Elf *e;
	if (( e = elf_memory((char *)p, st.st_size)) == NULL )
		goto err_out_map;

	if ( elf_kind ( e ) != ELF_K_ELF )
		goto err_out_elf;

	size_t n;
	if ( elf_getphdrnum (e , & n ) != 0)
		goto err_out_elf;

	unsigned int i;
	GElf_Phdr phdr;
	for (i = 0; i < n; i++) {
		if ( gelf_getphdr (e, i, &phdr) != &phdr )
			goto err_out_elf;

		if (phdr.p_type != PT_LOAD) {
			fprintf(stderr, "ignoring unknown p_type %lu\n",
				(unsigned long) phdr.p_type);
			continue;
		}

		size_t sz = phdr.p_memsz;
		roDataRange *rdr = new roDataRange(sz);
		rdr->start = phdr.p_vaddr;
		rdr->length = sz;
		rdr->end = rdr->start + rdr->length;

		char *cp = (char *) p;
		rdr->buf.assign(cp + phdr.p_offset, phdr.p_filesz);
		rdr->buf.resize(phdr.p_memsz);

		mach.memmap.push_back(rdr);
	}

	elf_end(e);
	munmap(p, st.st_size);
	close(fd);
	return true;

err_out_elf:
	elf_end(e);
err_out_map:
	munmap(p, st.st_size);
err_out:
	close(fd);
	return false;
}

