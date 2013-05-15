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
	// can be improve
	if( !file_lst ) throw std::exception();
	std::copy(std::istream_iterator<std::string>(file_lst), std::istream_iterator<std::string>(), std::back_inserter(files));
	
}

std::vector<std::pair<Term, Term> > BSBI_Search::find(std::vector<std::string>& q) // simplest way can be improved
{ 
	std::transform(q.begin(), q.end(), q.begin(), normalize);
	std::vector<std::vector<Term> > v = collect(q);
	std::vector<std::pair<Term, Term> > res;
	std::vector<Term>::iterator it;
	if( !v.empty() ) it = v[0].begin();
	else return res;
	std::vector<Term>::iterator f = v[0].end();
	for( ; it != v[0].end(); it++)
	{
		bool found = true;
		int pos = it->pos;
		for(int i = 1; i < (int) v.size(); i++)
		{
			pos += (q[i].size() + 1);
			f = std::lower_bound(v[i].begin(), v[i].end(), Term(v[i].begin()->word, it->file, pos));
			if(f == v[i].end() ||
				(*f != Term(v[i].begin()->word, it->file, pos + 1 ) && *f != Term(v[i].begin()->word, it->file, pos)))
			{ 
				found = false;  
				break; 
			}
			else if( *f == Term(v[i].begin()->word, it->file, pos + 1) ) pos++;
		}
		if( found )
		{
			Term begin(it->word, it->file, it->pos - q[0].size());
			Term end = (f == v[0].end()? Term(it->word, it->file, it->pos) : *f); // + q[0].size()
			res.push_back(std::make_pair(begin, end));
		}
	}
	/*
	// TODO: remove
	std::cout << "Result:" << std::endl;
	if( !res.empty() )
	{
		for(std::vector<std::pair<Term, Term> >::iterator it = res.begin(); it != res.end(); it++)
			std::cout << "Begin: " << it->first << "\nEnd: " << it->second << std::endl;
	}
	*/
	return res;
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
			v[i].push_back( Term(input[0], input[1], input[2]) );
			idx.read(reinterpret_cast<char*>(input), sizeof(input) );
		}
	}
	return v;
}
std::vector<std::string>& BSBI_Search::get_files()
{
	return files;
}

}
