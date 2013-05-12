#include "output_format.h"

#include <algorithm>
#include <iterator>
#include <fstream>
#include <cctype>

#include <iostream> // TODO: remove

namespace conc
{
	std::string OutputWord::get_result(std::vector<std::pair<Term, Term> >& pos)
	{
		for(std::vector<std::pair<Term, Term> >::iterator it = pos.begin(); it != pos.end(); it++)
		{
			std::ifstream in( files[(it->first).file].c_str(), std::ios::binary );
			int left_off = offset + 1, right_off = offset + 1, 
			pos_left = (it->first).pos, pos_right = (it->second).pos;
			in.seekg(pos_left? --pos_left : 0);
			char ch = '\0';
			std::string left;
			while( left_off > 0 && pos_left > 0 )
			{
				ch = in.peek();
				if( isspace(ch) ) left_off--;
				in.seekg(--pos_left);
				left.push_back((ch == '\t' || ch == '\n' )? ' ' : ch);
			}
			std::reverse(left.begin(), left.end());
			in.seekg((it->first).pos);
			std::string q;
			for(int i = (it->first).pos; i < pos_right; i++)
				q.push_back(in.get());
			std::cout << "Result: " << left << q << std::endl;
		}
		return std::string();
	}
}
