#include "index.h"
#include "helpers.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <iostream>
#include <vector>
#include <string>
#include <cstdio>

using namespace conc;
using namespace std;

void usage(const char* name)
{
	printf("USAGE: %s <index path> <file for indexing 1> <file for indexing 2> ...\n", name);
}

int main(int argc, char* argv[])
{
	if( argc < 3 ) { err("You should use at least three arguments"); usage(argv[0]); return 1; }
	string path(argv[1]);
	vector<string> files;
	for(int i = 2; i < argc; i++)
		files.push_back(string(argv[i]));
	struct stat st;
	if( stat(path.c_str(), &st) != 0 ) // path does not exist
		mkdir(path.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH); 
		// read, write, search for owner and group, read and search for others
	if( path[path.size() - 1] != '/' ) path.push_back('/');
	BSBI_Index idx(path);
	idx.build(files);
	return 0;
}

