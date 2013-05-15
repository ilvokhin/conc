#ifndef __output_format_h__
#define __output_format_h__

#include "helpers.h"

#include <utility>
#include <vector>
#include <string>

namespace conc
{

class Output
{
protected:
	std::vector<std::string> files;
	int offset;
	std::vector<std::string> format(std::vector<std::pair<Term, Term> >&, int (*check)(int));
public:
	Output(const std::vector<std::string>& files, const int offset = 0): files(files), offset(offset) {}
	virtual std::vector<std::string> get_result(std::vector<std::pair<Term, Term> >&) = 0;
};

class OutputWord: public Output
{
public:
	OutputWord(const std::vector<std::string>& files, const int offset = 0): Output(files, offset) {}
	virtual std::vector<std::string> get_result(std::vector<std::pair<Term, Term> >&);
};

class OutputSent: public Output
{
public:
	OutputSent(const std::vector<std::string>& files, const int offset = 0): Output(files, offset) {}
	virtual std::vector<std::string> get_result(std::vector<std::pair<Term, Term> >&);
};

class OutputPar: public Output
{
public:
	OutputPar(const std::vector<std::string>& files, const int offset = 0): Output(files, offset) {}
	virtual std::vector<std::string> get_result(std::vector<std::pair<Term, Term> >&);
};

}

#endif
