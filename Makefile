CXX = mpicc
CXXFLAGS = -std=c++11 -O3 -march=native -fopenmp

TARGETS = $(basename $(wildcard *.c))

all : $(TARGETS)

%:%.cpp *.h
	$(CXX) $(CXXFLAGS) $< $(LIBS) -o $@

clean:
	-$(RM) $(TARGETS) *~

.PHONY: all, clean