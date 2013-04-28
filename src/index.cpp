#include "helpers.h"
#include "index.h"

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
#include <iterator>

/*
TODO:
1. Delete debugs cout's;
2. Error handling;
3. Error messages;
4. SPIMI_Index impl
*/

namespace conc
{

const std::string Index::main = "main";

void BSBI_Index::build(const std::vector<std::string>& source_files)
{
	files = source_files;
	for(std::vector<std::string>::const_iterator it = files.begin(); it != files.end(); it++)
		std::cout << *it << std::endl;
	std::vector<std::string> tmp_indexes = make_temp_indexes();
	merge(tmp_indexes);
	bind();
	save_files();
	clear(tmp_indexes);
}

std::vector<std::string> BSBI_Index::make_temp_indexes()
{
	int file_num = 0, word_num = 0;
	std::vector<std::string> indexes;
	std::vector<Term> buf;
	buf.reserve(BUF_SIZE);
	std::ofstream lex( lexicon.c_str() );
	std::map<std::string, int> word2num;
	for(std::vector<std::string>::const_iterator it = files.begin(); it != files.end(); it++)
	{
		std::cout << "before crash" << std::endl;
		std::cout << "working with file: " << *it << std::endl;
		std::map<std::string, int>::iterator mit;
		file_num++;
		std::ifstream in( it->c_str() );
		if( !in ) throw std::exception(); // FIXME: Write my own exeption-class, ask-user what should to do
		std::string tok;
		while( in >> tok )
		{
			if( (int) tok.size() == 1 ) continue;
			tok = normalize(tok);
			int cur_file_num = file_num - 1, cur_word_num;
			if( (mit = word2num.find(tok)) != word2num.end() )
			{
				cur_word_num = mit->second;
			}
			else
			{
				cur_word_num = word_num++;
				word2num[tok] = cur_word_num;
				lex << tok << std::endl;
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
		std::cout << "save ramains" << std::endl;
		if( ! buf.empty() ) indexes.push_back(save(buf));
	}
	return indexes;
}

void BSBI_Index::clear(const std::vector<std::string>& tmp_idx)
{
	for(std::vector<std::string>::const_iterator it = tmp_idx.begin(); it != tmp_idx.end(); it++)
		if( std::remove( it->c_str() ) ) std::cout << "Can't remove file" << std::endl;
	if( std::remove( lexicon.c_str() ) ) std::cout << "Can't remove " << lexicon << "file" << std::endl;
}

void BSBI_Index::merge(const std::vector<std::string>& tmp_idx) // TODO: finish
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
		std::ofstream output(index.c_str(), std::ios::binary); //FIXME: add ios::binary
		while( !pq.empty() )
		{
			//output << pq.top().first << std:: endl;
			output.write(reinterpret_cast<const char*>(&(pq.top().first)), sizeof(Term));
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
}
void BSBI_Index::bind(void)
{
	std::cout << "start bind" << std::endl;
	std::ifstream lex( lexicon.c_str() ), idx ( index.c_str(), std::ios::in | std::ios::binary );
	std::string word;
	idx.seekg(0, idx.end);
	int end = idx.tellg();
	idx.seekg(0, idx.beg);
	std::cout << idx.end << " " << end << " " << end / 12 << std::endl;
	//int n;
	//std::cin >> n;
	while( lex >> word )
	{
		std::cout << "work with " << word << std::endl;
		word2_idx_pos[word] = idx.tellg();
		int input[3];
		idx.read( (char*)input, sizeof(input) );
		int word_num = input[0];
		std::cout << "start move" << std::endl;
		while( input[0] == word_num && idx.tellg() != end )
		{
			idx.read(reinterpret_cast<char*>(input), sizeof(input) );
		}
		std::cout << "next word_num " << input[0] << std::endl;
		std::cout << "end move" << std::endl;
		idx.seekg(-1*sizeof(input), idx.cur);
		std::cout << "cur pos " << idx.tellg() << std::endl;
	}
	std::cout << "end bind" << std::endl;
}
void BSBI_Index::save_files(void) // stupidest map save method
{
	std::ofstream w2ip( word2index_pos_filename.c_str() );
	for(std::map<std::string, int>::iterator it = word2_idx_pos.begin(); it != word2_idx_pos.end(); it++)
		w2ip << it->first << " " << it->second << " " << std::endl;
	std::ofstream f( files_filename.c_str() );
	//f << files.size() << std::endl;
	std::copy(files.begin(), files.end(), std::ostream_iterator<std::string>(f, "\n"));
	std::ofstream main_file ( main.c_str() );
	main_file << index << " " << word2index_pos_filename << " " << files_filename << std::endl;
}

}
