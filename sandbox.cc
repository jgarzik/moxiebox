
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

	rwDataRange *rdr = new rwDataRange(st.st_size);

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
		fprintf(stdout, "%s %08x-%08x\n",
			ar->name.c_str(), ar->start, ar->end);
	}
}

static void addStackMem(machine& mach)
{
	rwDataRange *rdr = new rwDataRange(STACK_SIZE);

	rdr->buf.resize(STACK_SIZE);
	rdr->updateRoot();

	rdr->end = 0x400000;
	rdr->length = STACK_SIZE;
	rdr->start = rdr->end - rdr->length;

	mach.memmap.push_back(rdr);
	mach.sortMemMap();
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

	printMemMap(mach);

	mach.cpu.asregs.regs[PC_REGNO] = mach.startAddr;
	sim_resume(mach);

	if (mach.cpu.asregs.exception)
		fprintf(stderr, "Sim exception %d (%s)\n",
			mach.cpu.asregs.exception,
			strsignal(mach.cpu.asregs.exception));

	return 0;
}

