#ifndef __Helpers_hpp__
#define __Helpers_hpp__

#include <iostream> // TODO
#include <fstream>
#include <vector>
#include <string>

namespace conc
{

struct Term
{
        int word, file, pos;
	Term(int word, int file, int pos): word(word), file(file), pos(pos) {}
};

std::string normalize(std::string& s);
std::string get_random_name(int);
bool cmp(const Term&, const Term&);
std::ostream& operator << (std::ostream&, const Term&);
std::ostream& operator << (std::ostream&, const std::vector<Term>&);
std::string save(std::vector<Term>&);
bool operator < (const Term&, const Term&);

}

#endif
