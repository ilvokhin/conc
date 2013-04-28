#ifndef __search_h__
#define __search_h__

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
	virtual void find(std::vector<std::string>&) = 0;
};

class BSBI_Search: public Search
{
	std::string index, word2index_pos_filename;
	std::map<std::string, int> word2index_pos;
public:
	BSBI_Search(const std::string&);
	virtual void find(std::vector<std::string>&);
};

class SPIMI_Search: public Search
{
public:
	virtual void find(std::vector<std::string>&);
};

}

#endif
