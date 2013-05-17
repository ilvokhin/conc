CXX = g++
CXXFLAGS = -pedantic -Wall -O3
CXXFLAGS_WITH_PB = $(CXXFLAGS) -pthread -std=c++0x -DPB

BUILDER_SRC = $(addprefix src/, builder.cpp index.cpp helpers.cpp) 
BUILDER_SRC_WITH_PB = $(BUILDER_SRC) src/pb.cpp
BUILDER_OBJ = $(BUILDER_SRC:.cpp=.o)
BUILDER_OBJ_WITH_PB = $(BUILDER_SRC_WITH_PB:.cpp=.o)

SEARCHER_SRC = $(addprefix src/, searcher.cpp output_format.cpp search.cpp helpers.cpp)
SEARCHER_OBJ = $(SEARCHER_SRC:.cpp=.o)

all: builder_pb searcher

builder_pb: $(BUILDER_OBJ_WITH_PB)
	$(CXX) $(CXXFLAGS_WITH_PB) $(BUILDER_OBJ_WITH_PB) -o builder

searcher: $(SEARCHER_OBJ)
	$(CXX) $(CXXFLAGS) $(SEARCHER_OBJ) -o searcher

builder: $(BUILDER_OBJ)
	$(CXX) $(CXXFLAGS) $(BUILDER_OBJ) -o builder

BUILDER_OBJ:
SEARCHER_OBJ:
BUILDER_OBJ_WITH_PB:

clean:
	rm src/*.o
	rm builder
	rm searcher
