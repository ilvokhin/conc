#include "pb.h"

void ProgressBar::operator()()
{
	progress = double(*cur) / double(last);
	while( progress <= 1.0 )
	{
		os << "[";
		int pos = width * progress;
		for(int i = 0; i < width; i++)
		{
			if( i < pos ) os << "=";
			else if( i == pos ) os << ">";
			else os << " ";
		}
		os << "] " << int(progress * 100.0) << " %\r";
		os.flush();
		if( *cur < 0 ) *cur = last;
		if( progress == 1.0 ) { os << "\n"; return; } 
		progress = double(*cur) / double(last);
	}
}
