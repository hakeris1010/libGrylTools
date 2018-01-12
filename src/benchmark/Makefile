CXX= g++
CXXFLAGS= -std=c++14 -O0 -finline-functions
INCLUDES= -I../../include -I../../src
LDFLAGS= -L../../lib/static

SOURCES= readerPerformanceBenchmarks.cpp \
		 regexSearcherBench.cpp

PROGLIBS= -lgryltools

.cpp.o:
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $*.cpp -o $*.o

all: $(SOURCES:.cpp=.o)
	mkdir -p bin
	for file in $^ ; do \
		g++ $(LDFLAGS) -o bin/$${file%.o} $$file $(PROGLIBS) ; \
	done ;

clean:
	$(RM) *.o 


