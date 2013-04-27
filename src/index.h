#ifndef __index_hpp__
#define __index_hpp__

#include <string>
#include <vector>
#include <map>

namespace conc
{

class Index
{
protected:
	std::string path,
        main,
        index,
        word2num_filename,
        file2num_filename,
        word2index_pos_filename;
        std::map<std::string, int> word2num, file2num, word2_idx_pos;
	std::vector<std::string> files;

public:
	Index
	(
		std::string path, 
		std::string main, 
		std::string index, 
		std::string word2num_filename, 
		std::string file2num_filename, 
		std::string word2index_pos_filename
	): path(path), main(path+main), index(path+index), 
		word2num_filename(path+word2num_filename), file2num_filename(path+file2num_filename), 
		word2index_pos_filename(path+word2index_pos_filename)
	{ }
		
	virtual std::string build(const std::vector<std::string>&) = 0;
};

class BSBI_Index: public Index
{
	static const int BUF_SIZE = 8192;
	std::vector<std::string> make_temp_indexes();
	std::string merge(const std::vector<std::string>&);
	void clear_temp_indexes(const std::vector<std::string>&);
public:
	BSBI_Index
	(
		std::string path = "./",
                std::string main = "main",
                std::string index = "index",
                std::string word2num_filename = "word2num",
                std::string file2num_filename = "file2num",
                std::string word2index_pos_filename = "word2index_pos"
	): Index(path, main, index, word2num_filename, file2num_filename, word2index_pos_filename)
	{ }
	virtual std::string build(const std::vector<std::string>&);
};

class SPIMI_Index: public Index
{
public:
	SPIMI_Index
        (
		std::string path = "./",
                std::string main = "main",
                std::string index = "index",
                std::string word2num_filename = "word2num",
                std::string file2num_filename = "file2num",
                std::string word2index_pos_filename = "word2index_pos"
        ): Index(path, main, index, word2num_filename, file2num_filename, word2index_pos_filename)
	{ }
	virtual std::string build(const std::vector<std::string>&);
};

}

#endif
