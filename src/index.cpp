#include "helpers.h"
#include "index.h"

#ifdef PB
#include "pb.h"
#include <thread>
#endif

#include <functional>
#include <algorithm>
#include <exception> // TODO: write my own exeption-class
#include <fstream>
#include <iostream>
#include <sstream>
#include <tr1/memory>
#include <utility>
#include <cstdio> // for "remove" function
#include <cctype>
#include <queue>
#include <map>
#include <iterator>

namespace conc
{

void BSBI_Index::build(const std::vector<std::string>& source_files)
{
	inf("Index builder started");
	files = source_files;
	std::vector<std::string> tmp_indexes = make_temp_indexes();
	merge(tmp_indexes);
	bind();
	save_files();
	clear(tmp_indexes);
	inf("All done");
}

std::vector<std::string> BSBI_Index::make_temp_indexes()
{
	int file_num = 0, word_num = 0;
	std::vector<std::string> indexes;
	std::vector<Term> buf;
	buf.reserve(BUF_SIZE);
	std::ofstream lex( lexicon.c_str() );
	std::map<std::string, int> word2num;
	std::stringstream ss;
	long all_tokens = 0;
	for(std::vector<std::string>::const_iterator it = files.begin(); it != files.end(); it++)
	{
		inf("Processing file " + *it);
		std::map<std::string, int>::iterator mit;
		file_num++;
		std::ifstream in( it->c_str() );
		if( !in ) err(), clear(indexes), throw std::exception(); // FIXME: Write my own exeption-class, ask-user what should to do
		#ifdef PB
		in.seekg(0, in.end); int len = in.tellg(), cur = 0;  in.seekg(0, in.beg); 
		std::thread t(ProgressBar(std::cout, len-1, &cur));
		#endif
		std::string tok;
		int tokens = 0;
		while( in >> tok )
		{
			tokens++;
			if( (int) tok.size() == 1 && !isalpha(tok[0]) ) continue;
			tok = normalize(tok);
			if( tok.empty() ) continue;
			int cur_file_num = file_num - 1, cur_word_num;
			if( (mit = word2num.find(tok)) != word2num.end() ) cur_word_num = mit->second;
			else
			{
				cur_word_num = word_num++;
				word2num[tok] = cur_word_num;
				lex << tok << std::endl;
			}
			buf.push_back(Term(cur_word_num, cur_file_num, in.tellg()));
			if( (int) buf.size() == BUF_SIZE ) indexes.push_back(save(buf));
			#ifdef PB 
			cur = in.tellg();
                	#endif
		}
		if( ! buf.empty() ) indexes.push_back(save(buf));
		#ifdef PB
		t.join();
		#endif
		ss.str(std::string()), ss.clear();
		ss << "File " << *it << " processed: " << tokens << " tokens";
		inf(ss.str()), all_tokens += tokens;
	}
	ss.str(std::string()), ss.clear();
        ss << "All files processed: " << all_tokens << " tokens, " << word_num << " terms";
        inf(ss.str());
	return indexes;
}

void BSBI_Index::clear(const std::vector<std::string>& tmp_idx)
{
	for(std::vector<std::string>::const_iterator it = tmp_idx.begin(); it != tmp_idx.end(); it++)
		if( std::remove( it->c_str() ) ) err(*it), err();
	if( std::remove( lexicon.c_str() ) ) err(lexicon), err();
}

void BSBI_Index::merge(const std::vector<std::string>& tmp_idx)
{
	inf("Start merging");
	#ifdef PB
	inf("[No progress shows]");
	#endif
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
		std::ofstream output(index.c_str(), std::ios::binary);
		while( !pq.empty() )
		{
			output.write(reinterpret_cast<const char*>(&(pq.top().first)), sizeof(Term));
			int cur_idx = pq.top().second;
			pq.pop();
			Term term;
			read_all[cur_idx] = !(*(open_files[cur_idx]) >> term);
			if( !read_all[cur_idx] )
				pq.push( std::make_pair(term, cur_idx) );
		}
	}
}
void BSBI_Index::bind(void)
{
	inf("Start binding");
	std::ifstream lex( lexicon.c_str() ), idx ( index.c_str(), std::ios::in | std::ios::binary );
	std::string word;
	idx.seekg(0, idx.end); int end = idx.tellg(); idx.seekg(0, idx.beg);
	#ifdef PB
	lex.seekg(0, lex.end); int len = lex.tellg(), cur = 0; lex.seekg(0, lex.beg);
        std::thread t(ProgressBar(std::cout, len-1, &cur));
        #endif
	while( lex >> word )
	{
		word2_idx_pos[word] = idx.tellg();
		int input[3];
		idx.read(reinterpret_cast<char*>(input), sizeof(input) );
		int word_num = input[0];
		int count = 0;
		while( input[0] == word_num && idx.tellg() != end )
		{
			idx.read(reinterpret_cast<char*>(input), sizeof(input) );
			count++;
		}
		idx.seekg(-1*sizeof(input), idx.cur);
		#ifdef PB
		cur = lex.tellg();
		#endif
	}
	#ifdef PB
	t.join();
	#endif
}
void BSBI_Index::save_files(void) // stupidest map save method
{
	std::ofstream w2ip( word2index_pos_filename.c_str() );
	for(std::map<std::string, int>::iterator it = word2_idx_pos.begin(); it != word2_idx_pos.end(); it++)
		w2ip << it->first << " " << it->second << " " << std::endl;
	std::ofstream f( files_filename.c_str() );
	std::copy(files.begin(), files.end(), std::ostream_iterator<std::string>(f, "\n"));
	std::ofstream main_file ( main.c_str() );
	main_file << index << " " << word2index_pos_filename << " " << files_filename << std::endl;
}

}
