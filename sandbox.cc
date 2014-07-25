
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <string>
#include <vector>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "sandbox.h"

using namespace std;

static const uint32_t STACK_SIZE = 64 * 1024;
static const uint32_t STACK_VADDR = 0x400000;


bool loadRawData(machine& mach, const char *filename)
{
	int fd;
	if (( fd = open ( filename, O_RDONLY , 0)) < 0)
		return false;

	struct stat st;
	if (fstat(fd, &st) < 0) {
		close(fd);
		return false;
	}

	void *p;
	p = mmap(NULL, st.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
	if (p == (void *)-1) {
		close(fd);
		return false;
	}

	static unsigned int dataCount = 0;
	char tmpstr[32];

	sprintf(tmpstr, "data%u", dataCount++);
	addressRange *rdr = new addressRange(tmpstr, st.st_size);

	rdr->buf.assign((char *) p, (size_t) st.st_size);
	rdr->updateRoot();

	munmap(p, st.st_size);
	close(fd);

	return mach.mapInsert(rdr);
}

static void usage(const char *progname)
{
	fprintf(stderr,
		"Usage: %s [options]\n"
		"\n"
		"options:\n"
		"-e <moxie executable>\tLoad executable into address space\n"
		"-d <file>\t\tLoad data into address space\n"
		"-t\t\tEnabling simulator tracing\n"
		,
		progname);
}

static void printMemMap(machine &mach)
{
	for (unsigned int i = 0; i < mach.memmap.size(); i++) {
		addressRange *ar = mach.memmap[i];
		fprintf(stderr, "%s %08x-%08x %s\n",
			ar->readOnly ? "ro" : "rw", ar->start, ar->end,
			ar->name.c_str());
	}
}

static void addStackMem(machine& mach)
{
	addressRange *rdr = new addressRange("stack", STACK_SIZE);

	rdr->buf.resize(STACK_SIZE);
	rdr->updateRoot();
	rdr->readOnly = false;

	rdr->end = STACK_VADDR;
	rdr->length = STACK_SIZE;
	rdr->start = rdr->end - rdr->length;

	mach.memmap.push_back(rdr);
	mach.sortMemMap();
}

static void addMapDescriptor(machine& mach)
{
	// fill list from existing memory map
	vector<struct mach_memmap_ent> desc;
	mach.fillDescriptors(desc);

	// add entry for the mapdesc range to be added to memory map
	struct mach_memmap_ent mme_self;
	memset(&mme_self, 0, sizeof(mme_self));
	desc.push_back(mme_self);

	// add blank entry for list terminator
	struct mach_memmap_ent mme_end;
	memset(&mme_end, 0, sizeof(mme_end));
	desc.push_back(mme_end);

	// calc total region size
	size_t sz = sizeof(mme_end) * desc.size();

	// manually fill in mapdesc range descriptor
	mme_self.length = sz;
	strcpy(mme_self.tags, "ro,mapdesc,");

	// build entry for global memory map
	addressRange *ar = new addressRange("mapdesc", sz);

	// allocate space for descriptor array
	ar->buf.resize(sz);
	ar->updateRoot();

	// copy 'desc' array into allocated memory space
	unsigned int i = 0;
	for (vector<struct mach_memmap_ent>::iterator it = desc.begin();
	     it != desc.end(); it++, i++) {
		struct mach_memmap_ent& mme = (*it);
		memcpy(&ar->buf[i * sizeof(mme)], &mme, sizeof(mme));
	}


	// add memory range to global memory map
	mach.mapInsert(ar);

	// set SR #6 to now-initialized mapdesc start vaddr
	mach.cpu.asregs.sregs[6] = ar->start;
}

int main (int argc, char *argv[])
{
	machine mach;

	int opt;
	while ((opt = getopt(argc, argv, "e:d:t")) != -1) {
		switch(opt) {
		case 'e':
			if (!loadElfProgram(mach, optarg)) {
				fprintf(stderr, "ELF load failed for %s\n",
					optarg);
				exit(EXIT_FAILURE);
			}
			break;
		case 'd':
			if (!loadRawData(mach, optarg)) {
				fprintf(stderr, "Data load failed for %s\n",
					optarg);
				exit(EXIT_FAILURE);
			}
			break;

		case 't':
			mach.tracing = true;
			break;

		default:
			usage(argv[0]);
			exit(EXIT_FAILURE);
		}
	}

	addStackMem(mach);
	addMapDescriptor(mach);

	printMemMap(mach);

	mach.cpu.asregs.regs[PC_REGNO] = mach.startAddr;
	sim_resume(mach);

	if (mach.cpu.asregs.exception)
		fprintf(stderr, "Sim exception %d (%s)\n",
			mach.cpu.asregs.exception,
			strsignal(mach.cpu.asregs.exception));

	return 0;
}

