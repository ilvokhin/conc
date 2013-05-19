#include "search.h"
#include "helpers.h"
#include "output_format.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <algorithm>
#include <iterator>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstdio>
#include <cstdlib>
#include <utility>
#include <exception>
#include <tr1/memory>

using namespace std;
using namespace conc;

void usage(const char* name)
{
        printf("USAGE: %s [flags: -w | -s | -p NUM] <index path>\n", name);
}

pair<string, pair<int, int> >  parse(int argc, char* argv[])
{
	if( argc < 2 || argc > 4 ) { err("Wrong arguments format"); usage(argv[0]); exit(1); }
	string path, opt1, opt2;
	if( argc == 2 )
		path = argv[1];
	else if( argc == 4 )
		opt1 = argv[1], opt2 = argv[2], path = argv[3];
	int output_type = 0, offset = 0;
	if( !opt1.empty() )
	{
		stringstream ss(opt2);
		ss >> offset;
		switch(opt1[1])
		{
			case 's': output_type = 1; break;
			case 'p': output_type = 2; break;
		}
	}
	return make_pair(path, make_pair(output_type, offset));
}

tr1::shared_ptr<Output> choose_output(pair<int, int>& param, vector<string>& files)
{
	int output_type = param.first, offset = param.second;
	switch(output_type)
	{
		case 0: return tr1::shared_ptr<Output> ( new OutputWord(files, offset) );
		case 1: return tr1::shared_ptr<Output> ( new OutputSent(files, offset) );
		case 2: return tr1::shared_ptr<Output> ( new OutputPar(files, offset) );
	}
	throw std::exception();
} 

int main(int argc, char * argv[])
{
	pair<string, pair<int, int> > param = parse(argc, argv);
	if( param.first[param.first.size() - 1] != '/') param.first.push_back('/');
	struct stat st;
	if( stat(param.first.c_str(), &st) != 0 ) err("Directory doesn't not exist"), exit(2);
	BSBI_Search s(param.first + "main");
	tr1::shared_ptr<Output> out = choose_output(param.second, s.get_files());
	string str;
	int count = 0;
	while( getline(cin, str) )
	{
		stringstream ss(str);
		vector<string> query;
		string word;
		while( ss >> word ) query.push_back( normalize(word) );
		vector<pair<Term, Term> > f = s.find(query);
		cout << "Query " << ++count << ": ";
		copy(query.begin(), query.end(), ostream_iterator<string>(cout, " "));
		cout << endl;
		if( f.empty() ) cout << "Not found" << endl;
		out->get_result( f );
	}
	return 0;
}
