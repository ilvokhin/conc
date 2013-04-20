#include "helpers.hpp"
#include "index.hpp"

#include <functional>
#include <algorithm>
#include <exception> // TODO: write my own exeption-class
#include <fstream>
#include <iostream> // TODO
#include <tr1/memory>
#include <utility>
#include <cstdio> // for "remove" function
#include <queue>
#include <map>

namespace conc
{

std::string BSBI_Index::build(const std::vector<std::string>& source_files)
{
	files = source_files;
	for(std::vector<std::string>::const_iterator it = files.begin(); it != files.end(); it++)
		std::cout << *it << std::endl;
	std::vector<std::string> tmp_indexes = make_temp_indexes();
	return merge(tmp_indexes);
}

std::vector<std::string> BSBI_Index::make_temp_indexes()
{
	int file_num = 0, word_num = 0;
	std::vector<std::string> indexes;
	std::vector<Term> buf;
	buf.reserve(BUF_SIZE);
	for(std::vector<std::string>::const_iterator it = files.begin(); it != files.end(); it++)
	{
		std::cout << "before crash" << std::endl;
		std::cout << "working with file: " << *it << std::endl;
		std::map<std::string, int>::iterator mit;
		if( (mit = file2num.find(*it)) != file2num.end() )
			continue; 
		else
			file2num[*it] = file_num++;
		std::ifstream in( it->c_str() );
		if( !in ) throw std::exception(); // FIXME: Write my own exeption-class, ask-user what should to do
		std::string tok;
		while( in >> tok )
		{
			if( (int) tok.size() == 1 ) continue;
			tok = normalize(tok);
			int cur_file_num = file_num - 1, cur_word_num;
			if( (mit = word2num.find(tok)) != word2num.end() )
				cur_word_num = mit->second;
			else
			{
				cur_word_num = word_num++;
				word2num[tok] = cur_word_num;
			}
			buf.push_back(Term(cur_word_num, cur_file_num, in.tellg()));
			if( (int) buf.size() == BUF_SIZE )
			{
				std::cout << "Buffer is full" << std::endl;
				indexes.push_back(save(buf));
				std::cout << "file saved" << std::endl;
			}
		}
		std::cout << "out of while" << std::endl;
	}
	return indexes;
}

void BSBI_Index::clear_temp_indexes(const std::vector<std::string>& tmp_idx)
{
	for(std::vector<std::string>::const_iterator it = tmp_idx.begin(); it != tmp_idx.end(); it++)
		if( std::remove( it->c_str() ) ) std::cout << "Can't remove file" << std::endl;
}

std::string BSBI_Index::merge(const std::vector<std::string>& tmp_idx) // TODO: finish
{
	std::priority_queue<std::pair<Term, int>, std::vector<std::pair<Term, int> >, std::greater<std::pair<Term, int> > > pq;
	std::vector<std::tr1::shared_ptr<std::ifstream> > open_files;
	for(int i = 0; i < (int) tmp_idx.size(); i++)
	{
		open_files.push_back(std::tr1::shared_ptr<std::ifstream>(new std::ifstream(tmp_idx[i].c_str())));
		Term term;
		*(open_files.back()) >> term;
		pq.push( std::make_pair(term, i) ); 
	}
	{
		std::vector<bool> read_all(open_files.size());
		std::ofstream output(index.c_str());
		while( !pq.empty() )
		{
			output << pq.top().first << std:: endl;
			int cur_idx = pq.top().second;
			pq.pop();
			Term term;
			read_all[cur_idx] = !(*(open_files[cur_idx]) >> term);
			if( !read_all[cur_idx] )
				pq.push( std::make_pair(term, cur_idx) );
		}
		for(std::vector<bool>::iterator it = read_all.begin(); it != read_all.end(); it++) // TODO: check
			if( ! (*it) ) std::cout << "Error: exist not empty files" << std::endl;
	}
	clear_temp_indexes(tmp_idx);
	return index;
}

}