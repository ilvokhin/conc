#include "output_format.h"

#include <algorithm>
#include <iterator>
#include <fstream>
#include <sstream>
#include <cctype>

#include <iostream> // TODO: remove

namespace conc
{
	std::string OutputWord::format(std::vector<std::pair<Term, Term> >& pos, int (*check) (int))
	{
		
		std::stringstream ss;
		for(std::vector<std::pair<Term, Term> >::iterator it = pos.begin(); it != pos.end(); it++)
		{
			std::ifstream in( files[(it->first).file].c_str(), std::ios::binary );
			int left_off = offset? offset + 1 : 1, right_off = offset? offset + 1 : 1, 
			pos_left = (it->first).pos, pos_right = (it->second).pos;
			in.seekg(pos_left? --pos_left : 0);
			int ch = '\0';
			std::string left;
			while( left_off > 0 && pos_left > 0 )
			{
				ch = in.peek();
				if( check(ch) ) left_off--;
				in.seekg(--pos_left);
				left.push_back((ch == '\t' || ch == '\n' )? ' ' : ch);
			}
			std::reverse(left.begin(), left.end());
			in.seekg((it->first).pos);
			std::string q;
			for(int i = (it->first).pos; i < pos_right && in; i++)
				q.push_back(in.get());
			std::string right;
			while( right_off > 0 && ((ch = in.get()) != EOF) )
			{
				if( check(ch) ) right_off--;
				right.push_back((ch == '\t' || ch == '\n' )? ' ' : ch);
			}
			ss << "Result: " << left << q << right << std::endl;
			if( check == isnewline ) ss << std::endl;
			
		}
		std::cout << ss.str() << std::endl; // TODO: delete
		return ss.str();
	}
	std::string OutputWord::get_result(std::vector<std::pair<Term, Term> >& pos)
	{
		return format(pos, isspace);
	}
	std::string OutputPar::get_result(std::vector<std::pair<Term, Term> >& pos)
	{
		return format(pos, isnewline);
	}
	std::string OutputSent::get_result(std::vector<std::pair<Term, Term> >& pos)
	{
		return std::string();
	}
}
