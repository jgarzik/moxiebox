
#include <sys/mman.h>
#include <string>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <fcntl.h>
#include <errno.h>
#include "sandbox.h"

using namespace std;

class mdir {
public:
	DIR *d;
	std::string pathname;

	mdir(const std::string& pathname_ = "") {
		d = NULL;
		pathname = pathname_;
	}
	~mdir() {
		if (d)
			closedir(d);
	}

	bool open() {
		d = opendir(pathname.c_str());
		return (d != NULL);
	}
	struct dirent *read() { return readdir(d); }
};

const signed char p_util_hexdigit[256] =
{ -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
  -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
  -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
  0,1,2,3,4,5,6,7,8,9,-1,-1,-1,-1,-1,-1,
  -1,0xa,0xb,0xc,0xd,0xe,0xf,-1,-1,-1,-1,-1,-1,-1,-1,-1,
  -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
  -1,0xa,0xb,0xc,0xd,0xe,0xf,-1,-1,-1,-1,-1,-1,-1,-1,-1,
  -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
  -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
  -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
  -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
  -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
  -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
  -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
  -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
  -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1, };

signed char HexDigit(char c)
{
    return p_util_hexdigit[(unsigned char)c];
}

bool IsHex(const string& str)
{
    for (unsigned int i = 0; i < str.size(); i++)
    {
	char c = str[i];
        if (HexDigit(c) < 0)
            return false;
    }
    return (str.size() > 0) && (str.size()%2 == 0);
}

vector<unsigned char> ParseHex(const char* psz)
{
    // convert hex dump to vector
    vector<unsigned char> vch;
    while (true)
    {
        while (isspace(*psz))
            psz++;
        signed char c = HexDigit(*psz++);
        if (c == (signed char)-1)
            break;
        unsigned char n = (c << 4);
        c = HexDigit(*psz++);
        if (c == (signed char)-1)
            break;
        n |= c;
        vch.push_back(n);
    }
    return vch;
}

vector<unsigned char> ParseHex(const string& str)
{
    return ParseHex(str.c_str());
}

bool mfile::open(int flags, mode_t mode, bool map)
{
	fd = ::open(pathname.c_str(), flags, mode);
	if (fd < 0)
		return false;

	if (!map)
		return true;

	if (fstat(fd, &st) < 0)
		return false;

	// sanity check
	if (st.st_size > (1 * 1024 * 1024 * 1024))
		return false;

	int mmap_prot;
	if (flags & O_RDWR)
		mmap_prot = PROT_READ | PROT_WRITE;
	else if (flags & O_WRONLY)
		mmap_prot = PROT_WRITE;
	else
		mmap_prot = PROT_READ;

	data = mmap(NULL, st.st_size, mmap_prot, MAP_SHARED, fd, 0);
	if (data == (void *)-1)
		return false;
	
	return true;
}

bool ReadDir(const std::string& pathname, std::vector<std::string>& dirNames)
{
	mdir d(pathname);
	if (!d.open())
		return false;

	dirNames.clear();

	errno = 0;
	struct dirent *de;
	while ((de = d.read()) != NULL) {
		if (!strcmp(de->d_name, ".") ||
		    !strcmp(de->d_name, ".."))
			continue;

		dirNames.push_back(de->d_name);
	}

	if (errno) {
		dirNames.clear();
		return false;
	}

	return true;
}

