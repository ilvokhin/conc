#include "Helpers.hpp"
#include "Index.hpp"

#include <algorithm>
#include <exception> // TODO: write my own exeption-class
#include <fstream>

namespace conc
{
	virtual std::vector<std::string> BSBI_Index::make_temp_indexes(const std::vector<std::string>& files)
	{
		int file_num = 0, word_num = 0;
		std::vector<std::string> indexes;
		std::vector<Term> buf;
		buf.reserve(BUF_SIZE);
		for(std::vector<std::string>::const_iterator it = files.begin(); it != files.end(); it++)
		{
			if( (std::map<std::string, int>::iterator mit = file2num.find(*it)) != file2num.end() )
				continue; 
			else
				file2num[*it] = file_num++;
			std::istream in( *it );
			if( !in ) throw std::exception("Can't open file\n"); // FIXME: Write my own exeption-class, ask-user what should to do
			string tok;
			while( in >> tok )
			{
				tok = normalize(tok);
				int cur_file_num = file_num - 1, cur_word_num;
				if( (std::map<std::string, int>::iterator mit = word2num.find(s)) != word2num.end() )
					cur_word_num = it->second();
				else
				{
					cur_word_num = word_num++;
					word2num[tok] = cur_word_num;
				}
				buf.push_back(Term(cur_word_num, cur_file_num, in.tellg()));
				if( (int) buf.size() == BUF_SIZE )
					indexes.push_back(save(buf));
			}
		}
		return indexes;
	}
}
