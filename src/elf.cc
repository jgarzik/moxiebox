
#include <string>
#include <vector>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <libelf.h>
#include <gelf.h>
#include <sys/mman.h>
#include <stdio.h>
#include "sandbox.h"

#ifndef EM_MOXIE
#define EM_MOXIE                0xFEED  /* Moxie */
#endif // EM_MOXIE

using namespace std;

bool loadElfProgSection(machine& mach, Elf *e, GElf_Phdr *phdr, void *p)
{
	bool writable = (phdr->p_flags & PF_W);
	size_t sz = phdr->p_memsz;
	static unsigned int elfCount = 0;
	char tmpstr[32];

	sprintf(tmpstr, "elf%u", elfCount++);
	addressRange *rdr = new addressRange(tmpstr, sz);

	rdr->start = phdr->p_vaddr;
	rdr->length = sz;
	rdr->end = rdr->start + rdr->length;
	rdr->readOnly = (writable ? false : true);

	char *cp = (char *) p;
	rdr->buf.assign(cp + phdr->p_offset, phdr->p_filesz);
	rdr->buf.resize(phdr->p_memsz);
	rdr->updateRoot();

	mach.memmap.push_back(rdr);
	mach.sortMemMap();

	return true;
}

static bool loadElfFile(machine& mach, mfile& pf)
{
	if ( elf_version ( EV_CURRENT ) == EV_NONE )
		return false;

	Elf *e;
	if (( e = elf_memory((char *)pf.data, pf.st.st_size)) == NULL )
		return false;

	if ( elf_kind ( e ) != ELF_K_ELF )
		goto err_out_elf;

	GElf_Ehdr ehdr;
	if (gelf_getehdr(e, &ehdr) != &ehdr)
		goto err_out_elf;

	if ((ehdr.e_ident[EI_CLASS] != ELFCLASS32) ||
	    (ehdr.e_ident[EI_DATA] != ELFDATA2LSB) ||
	    (ehdr.e_machine != EM_MOXIE)) {
		fprintf(stderr, "unsupported ELF binary type\n");
		goto err_out_elf;
	}

	mach.startAddr = ehdr.e_entry;
	fprintf(stderr, "ep %08lx\n", ehdr.e_entry);

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

		if (!loadElfProgSection(mach, e, &phdr, pf.data))
			goto err_out_elf;
	}

	elf_end(e);
	return true;

err_out_elf:
	elf_end(e);
	return false;
}

bool loadElfProgram(machine& mach, const string& filename)
{
	mfile pf(filename);
	if (!pf.open(O_RDONLY))
		return false;

	return loadElfFile(mach, pf);
}

bool loadElfHash(machine& mach, const string& hash,
		 const std::vector<std::string>& pathExec)
{
	vector<unsigned char> digest = ParseHex(hash);

	for (unsigned int i = 0; i < pathExec.size(); i++) {
		const std::string& path = pathExec[i];

		vector<string> dirNames;
		if (!ReadDir(path, dirNames)) {
			perror(path.c_str());
			continue;
		}

		for (vector<string>::iterator it = dirNames.begin();
		     it != dirNames.end(); it++) {
			string filename = path + "/" + (*it);

			mfile pf(filename);
			if (!pf.open(O_RDONLY)) {
				perror(filename.c_str());
				continue;
			}

			sha256hash hash(pf.data, pf.st.st_size);

			vector<unsigned char> tmpHash;
			hash.final(tmpHash);

			if (eqVec(digest, tmpHash))
				return loadElfFile(mach, pf);
		}
	}

	return false;
}

