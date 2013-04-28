#include "search.h"

#include <algorithm>
#include <exception> //TODO
#include <iostream>
#include <iterator>
#include <utility>
#include <fstream>
#include <map>

namespace conc
{

BSBI_Search::BSBI_Search(const std::string& main)
{
	std::ifstream conf( main.c_str() );
	if( !conf ) throw std::exception(); // TODO: change it
	std::string files_filename;
	conf >> index >> word2index_pos_filename >> files_filename;
	std::ifstream w2ip( word2index_pos_filename.c_str() );
	if( !w2ip ) throw std::exception();
	std::string s;
	int pos;
	while( w2ip >> s >> pos )
		word2index_pos.insert(word2index_pos.end(), make_pair(s, pos));
	std::ifstream file_lst( files_filename.c_str() );
	if( !file_lst ) throw std::exception();
	std::copy(std::istream_iterator<std::string>(file_lst), std::istream_iterator<std::string>(), std::back_inserter(files));
	
}

void BSBI_Search::find(const std::vector<std::string>& q)
{ 
	std::vector<std::vector<Term> > v = collect(q);
	std::vector<Term>::iterator it = v[0].begin();
	std::vector<Term> res;
	// TODO: write this
	std::cout << "Result:" << std::endl;
	std::copy(res.begin(), res.end(), std::ostream_iterator<Term>(std::cout, "\n"));
}

std::vector<std::vector<Term> > BSBI_Search::collect(const std::vector<std::string>& q)
{
	std::vector<std::vector<Term> > v( q.size() );
	std::ifstream idx( index.c_str() );
	for(int i = 0; i < (int) q.size(); i++)
	{
		std::map<std::string, int>::iterator f = word2index_pos.find(q[i]);
		if( f == word2index_pos.end() ) return std::vector<std::vector<Term> >();
		idx.seekg(f->second);
		int input[3];
		idx.read(reinterpret_cast<char*>(input), sizeof(input) );
		int word_num = input[0];
		while( word_num == input[0] )
		{
			idx.read(reinterpret_cast<char*>(input), sizeof(input) );
			v[i].push_back( Term(input[0], input[1], input[2]) );
		}
	}
	return v;
}

}
