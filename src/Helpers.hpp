#ifndef __Helpers_hpp__
#define __Helpers_hpp__

#include <ostream>
#include <string>

namespace conc
{

struct Term
{
        int word, file, pos;
	Term(int word, file, pos): word(word), file(file), pos(pos) {}
	friend std::ostream& operator << (std::ostream& os, const Term&);
};

std::string normalize(std::string& s);
std::string get_random_name(int);
bool cmp(const Term&, const Term&);
std::ostream& operator << (std::ostream&, const std::vector<Term>);
std::string save(std::vector<Term>&);
}

#endif
