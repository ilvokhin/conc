#ifndef __search_h__
#define __search_h__

#include "helpers.h"

#include <vector>
#include <string>
#include <map>

namespace conc
{

class BSBI_Search
{
	std::vector<std::string> files;
	std::string index, word2index_pos_filename;
	std::map<std::string, int> word2index_pos;
	
	std::vector<std::vector<Term> > collect(const std::vector<std::string>&);
public:
	BSBI_Search(const std::string&);
	virtual std::vector<std::pair<Term, Term> > find(std::vector<std::string>&);
	std::vector<std::string>& get_files();
};

}

#endif
