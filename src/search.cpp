#include "search.h"

#include <algorithm>
#include <exception> //TODO
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

void BSBI_Search::find(std::vector<std::string>& q)
{ }

}
