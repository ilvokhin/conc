#ifndef __pb_h__
#define __pb_h__

#include <iostream>

class ProgressBar
{
	static const int width = 70;
	std::ostream& os;
	int last;
	int* cur;
	double progress;
public:
	ProgressBar(std::ostream& os, int last, int* cur, double progress = 0.0): 
	os(os), last(last), cur(cur), progress(progress) {};
	void operator()();
};

#endif
