#include "Helpers.hpp"

#include <algorithm>
#include <iterator>
#include <iostream>
#include <cstdlib>
#include <cctype>

std::string normalize(std::string& s)
{
	int b = 0, e = (int) s.size() - 1;
	std::transform(s.begin(), s.end(), s.begin(), ::tolower);
	while( ispunct(s[b]) && b < (int) s.size() ) b++;
	while( ispunct(s[e]) && e > 0 ) e--;
	return s.substr(b, e - b + 1);
}

std::string get_random_name(int len = 20)
{
	static const string alphas = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
	std::string s(len);
	for(int i = 0; i < len; i++)
		s[i] = alphas[ std::rand() % alphas.size() ];
	return s;
}

bool cmp(const Term& lhs, const Term& rhs)
{
	return (lhs.word < rhs.word)? true : ((lhs.file < rhs.file)? true : lhs.pos < rhs.pos);
}

std::ostream& Term::operator << (std::ostream& os, const Term&)
{
	os << word << " " << file << pos;
	return os;
}

std::ostream& operator << (std::ostream& os, const std::vector<Term> v);
{
	copy(v.begin(), v.end(), ostream_iterator<Term>(os, "\n");
	return os;
}

std::string save(std::vector<Term>& buf)
{
	sort(buf.begin(), buf.end(), cmp);
	std::string tmp_idx_name = get_random_name();
	std::ostream out(tmp_idx_name);
	out << buf;
	buf.clear();
	return tmp_idx_name;
}
