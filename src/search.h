#ifndef __search_h__
#define __search_h__

#include "helpers.h"

#include <vector>
#include <string>
#include <map>

namespace conc
{

class Search
{
protected:
	std::vector<std::string> files;
public:
	virtual void find(const std::vector<std::string>&) = 0;
};

class BSBI_Search: public Search
{
	std::string index, word2index_pos_filename;
	std::map<std::string, int> word2index_pos;
	
	std::vector<std::vector<Term> > collect(const std::vector<std::string>&);
public:
	BSBI_Search(const std::string&);
	virtual void find(const std::vector<std::string>&);
	// d'ctor needed ?
};

class SPIMI_Search: public Search
{
public:
	virtual void find(const std::vector<std::string>&);
};

}

#endif
