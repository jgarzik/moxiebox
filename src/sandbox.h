#ifndef __SANDBOX_H__
#define __SANDBOX_H__

#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <vector>
#include <string>
#include <string.h>
#include <stdint.h>
#include <openssl/sha.h>
#include <unordered_map>
#include "moxie.h"

typedef std::unordered_map<uint32_t, uint32_t> gprof_bb_map_t;
typedef std::unordered_map<uint64_t, uint32_t> gprof_cg_map_t;

enum {
	MACH_PAGE_SIZE = 4096,
	MACH_PAGE_MASK = (MACH_PAGE_SIZE-1),
};

static inline bool eqVec(const std::vector<unsigned char>& a,
			 const std::vector<unsigned char>& b)
{
	if (a.size() != b.size())
		return false;
	
	return (memcmp(&a[0], &b[0], a.size()) == 0);
}

class sha256hash {
private:
	SHA256_CTX ctx;

public:
	sha256hash(const void *p = NULL, size_t len = 0) {
		clear();
		if (p && len)
			update(p, len);
	}

	void clear() { SHA256_Init(&ctx); }
	void update(const void *p, size_t len) { SHA256_Update(&ctx, p, len); }
	void final(std::vector<unsigned char>& digest) {
		digest.resize(SHA256_DIGEST_LENGTH);
		SHA256_Final(&digest[0], &ctx);
	}
};

class mfile {
public:
	int fd;
	void *data;
	struct stat st;
	std::string pathname;

	mfile(const std::string& pathname_ = "") {
		fd = -1;
		data = NULL;
		pathname = pathname_;
	}
	~mfile() {
		if (data)
			munmap(data, st.st_size);
		if (fd >= 0)
			close(fd);
	}

	bool open(int flags, mode_t mode = 0, bool map = true);
};

struct mach_memmap_ent {
	uint32_t vaddr;
	uint32_t length;
	char tags[32 - 4 - 4];
};

class cpuState {
public:
	struct moxie_regset asregs;

	cpuState() {
		memset(&asregs, 0, sizeof(asregs));
	}
};

class addressRange {
public:
	std::string name;
	uint32_t start;
	uint32_t end;
	uint32_t length;
	void *root;
	bool readOnly;
	std::string buf;

	addressRange(std::string name_, size_t sz) {
		name = name_;
		start = 0;
		end = 0;
		length = sz;
		root = NULL;
		readOnly = true;
	}

	void *physaddr(uint32_t addr) {
		uint32_t offset = addr - start;
		return (char *)root + offset;
	}

	bool inRange(uint32_t addr, uint32_t len) {
		return ((addr >= start) &&
			((addr + len) <= end));		// warn: overflow
	}

	void updateRoot() { root = &buf[0]; }
};

class machine {
public:
	std::vector<addressRange*> memmap;
	cpuState cpu;

	uint32_t startAddr;
	bool tracing;
	bool profiling;
	uint32_t heapAvail;

	gprof_bb_map_t gprof_bb_data;
	gprof_cg_map_t gprof_cg_data;

	machine() {
		startAddr = 0;
		tracing = false;
		profiling = false;
		heapAvail = 0xfffffffU;
	}

	bool read8(uint32_t addr, uint32_t& val_out);
	bool read16(uint32_t addr, uint32_t& val_out);
	bool read32(uint32_t addr, uint32_t& val_out);

	bool write8(uint32_t addr, uint32_t val);
	bool write16(uint32_t addr, uint32_t val);
	bool write32(uint32_t addr, uint32_t val);

	void *physaddr(uint32_t addr, size_t objLen, bool wantWrite = false);
	void sortMemMap();
	bool mapInsert(addressRange *ar);
	void fillDescriptors(std::vector<struct mach_memmap_ent>& desc);
};

extern void sim_resume (machine& mach, unsigned long long cpu_budget = 0);
extern bool loadElfProgram(machine& mach, const std::string& filename);
extern bool loadElfHash(machine& mach, const std::string& hash,
			const std::vector<std::string>& pathExec);

extern signed char HexDigit(char c);
extern bool IsHex(const std::string& str);
extern std::vector<unsigned char> ParseHex(const char* psz);
extern std::vector<unsigned char> ParseHex(const std::string& str);
extern bool ReadDir(const std::string& pathname,
		    std::vector<std::string>& dirNames);

#endif // __SANDBOX_H__
