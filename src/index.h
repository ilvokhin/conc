#ifndef __index_h__
#define __index_h__

#include <string>
#include <vector>
#include <map>

namespace conc
{

class Index
{
protected:
	const static std::string main;
	std::vector<std::string> files;
public:
	virtual void build(const std::vector<std::string>&) = 0;
};

class BSBI_Index: public Index
{
	std::string path, lexicon, index, word2index_pos_filename, files_filename;
	std::map<std::string, int> word2_idx_pos;
	
	static const int BUF_SIZE = 8192;
	std::vector<std::string> make_temp_indexes();
	void merge(const std::vector<std::string>&);
	void bind(void);
	void save_files(void);
	void clear(const std::vector<std::string>&);

public:
	BSBI_Index
	(
		std::string path = "./",
		std::string lexicon = "lexicon",
                std::string index = "index", // need to save
                std::string word2index_pos_filename = "word2index_pos", // need to save
		std::string files_filename = "files"
	): path(path), lexicon(path+lexicon), index(path+index), word2index_pos_filename(path + word2index_pos_filename), 
		files_filename(path+files_filename)
	{ }
	virtual void build(const std::vector<std::string>&);
};

class SPIMI_Index: public Index
{
public:
	virtual void build(const std::vector<std::string>&);
};
}

#endif
