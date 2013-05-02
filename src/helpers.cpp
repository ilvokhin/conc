#include "helpers.h"

#include <algorithm>
#include <iterator>
#include <iostream>
#include <cstdlib>
#include <utility>
#include <cctype>
#include <iostream> // TODO

namespace conc
{

std::string normalize(std::string& s)
{
	int b = 0, e = (int) s.size() - 1;
	std::transform(s.begin(), s.end(), s.begin(), ::tolower);
	while( ispunct(s[b]) && b < (int) s.size() ) b++;
	while( ispunct(s[e]) && e > 0 ) e--;
	if( e < b ) {  std::cout << "Error: " << s << std::endl; return std::string(""); } // FIXME
	return s.substr(b, e - b + 1);
}

std::string get_random_name(int len = 20) // FIXME: random generator
{
	static const std::string alphas = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
	std::string s(len, '\0');
	for(int i = 0; i < len; i++)
		s[i] = alphas[ std::rand() % alphas.size() ]; //TODO: here
	std::cout << s << std::endl;
	return s + "tmp";
}

bool operator < (const Term& lhs, const Term& rhs)
{
	if( lhs.word != rhs.word ) return lhs.word < rhs.word;
	else if ( lhs.file != rhs.file ) return lhs.file < rhs.file;
	return lhs.pos < rhs.pos;
}

bool operator == (const Term& lhs, const Term& rhs)
{
	return (lhs.word == rhs.word) && (lhs.file == rhs.file) && (lhs.pos == rhs.pos);
}

bool operator < (const std::pair<Term, int>& lhs,const std::pair<Term, int>& rhs)
{
	return lhs.first < rhs.first;
}

std::istream& operator >> (std::istream& is, Term& t)
{
	is >> t.word >> t.file >> t.pos;
	return is;
}

std::ostream& operator << (std::ostream& os, const Term& t)
{
	os << t.word << " " << t.file << " " << t.pos;
	return os;
}

std::ostream& operator << (std::ostream& os, const std::vector<Term>& v)
{
	std::copy(v.begin(), v.end(), std::ostream_iterator<Term>(os, "\n"));
	return os;
}

std::string save(std::vector<Term>& buf)
{
	std::cout << "save function" << std::endl;
	std::sort(buf.begin(), buf.end());
	std::string tmp_idx_name = get_random_name();
	std::ofstream out(tmp_idx_name.c_str());
	out << buf;
	buf.clear();
	return tmp_idx_name;
}

}
