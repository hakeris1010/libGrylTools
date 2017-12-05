#mkfile_path := $(abspath $(lastword $(MAKEFILE_LIST)))
#current_dir := $(notdir $(patsubst %/,%,$(dir $(mkfile_path))))

CC = gcc
CXX = g++
AR = ar

CFLAGS= -std=c99 
CXXFLAGS= -std=c++11
LDFLAGS= 

TEST_CFLAGS= -std=c99 -g
TEST_CXXFLAGS= -std=c++11 -g
TEST_LDFLAGS=
TEST_EXEC_ARGS=

DEBUG_CFLAGS= -g -Wall
DEBUG_CXXFLAGS= -g -Wall
DEBUG_LDFLAGS=
RELEASE_CFLAGS= -O2
RELEASE_CXXFLAGS= -O2
RELEASE_LDFLAGS=

LIBDIR= lib
INCLUDEDIR= include
BINDIR= bin
TESTDIR= $(BINDIR)/test

BINDIR_RELEASE= $(BINDIR)/release
BINDIR_DEBUG= $(BINDIR)/debug

#====================================#
#set directories
ZSH_RESULT:=$(shell mkdir -p $(TESTDIR) $(LIBDIR) $(LIBDIR)/static $(LIBDIR)/shared \
			  $(BINDIR_RELEASE) $(BINDIR_DEBUG) $(INCLUDEDIR))

#====================================#
#---------- Gryltools C ---------#

SOURCES_GRYLTOOLS = src/gryltools/grylthread.c \
                    src/gryltools/grylsocks.c \
                    src/gryltools/hlog.c \
                    src/gryltools/gmisc.c

HEADERS_GRYLTOOLS=  src/gryltools/grylthread.h \
                    src/gryltools/grylsocks.h \
                    src/gryltools/hlog.h \
                    src/gryltools/gmisc.h \
                    src/gryltools/systemcheck.h
LIBS_GRYLTOOLS=

#--------- Gryltools C++ --------#
       
SOURCES_GRYLTOOLSPP= src/gryltools++/stackreader.cpp \

HEADERS_GRYLTOOLSPP= src/gryltools++/blockingqueue.hpp \
					 src/gryltools++/stackreader.hpp \
				   # src/gryltools++/glogpp.hpp 

#--------- Test sources ---------#

TEST_C_SOURCES= src/test/test1.c

TEST_CPP_SOURCES= src/test/stackreader_test.cpp 

TEST_LIBS= -lgryltools

#====================================#

GRYLTOOLS_INCL= $(INCLUDEDIR)/gryltools/
GRYLTOOLS_LIB= $(LIBDIR)/static/libgryltools
GRYLTOOLS_SHARED= $(LIBDIR)/shared/libgryltools

GRYLTOOLS_INCLHEAD= #$(addprefix $(GRYLTOOLS_INCL), $(notdir HEADERS_GRYLTOOLS))    

GRYLTOOLS= gryltools

#====================================#

DEBUG_INCLUDES= -Isrc/gryltools -Isrc/gryltools++
DEBUG_LIBCLUDES=
RELEASE_INCLUDES= -I$(GRYLTOOLS_INCL) 
RELEASE_LIBCLUDES=

TEST_INCLUDES=  -I$(INCLUDEDIR)
TEST_LIBCLUDES= -L$(LIBDIR)/static

BINPREFIX= 

#===================================#

# set os-dependent stuff
ifeq ($(OS),Windows_NT)
    LDFLAGS += -lkernel32 -lWs2_32
	#LIBS_GRYLTOOLS += -lWs2_32 -lkernel32

	GRYLTOOLS_SHARED:= $(GRYLTOOLS_SHARED).dll
	GRYLTOOLS_LIB:= $(GRYLTOOLS_LIB).a

else
    CFLAGS += -std=gnu99 -pthread	
	CXXFLAGS += -std=gnu11 -pthread
    LDFLAGS += -pthread

	GRYLTOOLS_SHARED:= $(GRYLTOOLS_SHARED).so
	GRYLTOOLS_LIB:= $(GRYLTOOLS_LIB).a
endif

#====================================#


all: debug test_main

debops: 
	$(eval CFLAGS += $(DEBUG_CFLAGS) $(DEBUG_INCLUDES))
	$(eval CXXFLAGS += $(DEBUG_CXXFLAGS) $(DEBUG_INCLUDES))
	$(eval LDFLAGS += $(DEBUG_LDFLAGS) $(DEBUG_LIBCLUDES))
	$(eval BINPREFIX = $(BINDIR_DEBUG)) 

relops: 
	$(eval CFLAGS += $(RELEASE_CFLAGS) $(RELEASE_INCLUDES)) 
	$(eval CXXFLAGS += $(RELEASE_CXXFLAGS) $(RELEASE_INCLUDES)) 
	$(eval LDFLAGS += $(RELEASE_LDFLAGS) $(RELEASE_LIBCLUDES)) 
	$(eval BINPREFIX = $(BINDIR_RELEASE)) 

debug: debops $(GRYLTOOLS) 
release: relops $(GRYLTOOLS) 

.c.o:
	$(CC) $(CFLAGS) -fpic -c $*.c -o $*.o

.cpp.o:
	$(CXX) $(CXXFLAGS) -fpic -c $*.cpp -o $*.o

gryltools_incl: $(HEADERS_GRYLTOOLS) $(HEADERS_GRYLTOOLSPP)
	mkdir -p $(GRYLTOOLS_INCL) 
	for file in $^ ; do \
		cp $$file $(GRYLTOOLS_INCL) ; \
	done
	
$(GRYLTOOLS_LIB): $(SOURCES_GRYLTOOLS:.c=.o) $(SOURCES_GRYLTOOLSPP:.cpp=.o) 
	$(AR) -rvsc $@ $^ $(LIBS_GRYLTOOLS) 

$(GRYLTOOLS_SHARED): $(SOURCES_GRYLTOOLS:.c=.o) $(SOURCES_GRYLTOOLSPP:.cpp=.o) 
	$(CXX) -shared -o $(GRYLTOOLS_SHARED) $^ -Wl,--whole-archive $(LIBS_GRYLTOOLS) -Wl,--no-whole-archive $(LDFLAGS) 

$(GRYLTOOLS): gryltools_incl $(GRYLTOOLS_LIB) $(GRYLTOOLS_SHARED)   
$(GRYLTOOLS)_debug: debops $(GRYLTOOLS)

#===================================#
# Tests

test_main: test_debops tests_cpp
	for file in $(TESTDIR)/* ; do \
		./$$file $(TEST_EXEC_ARGS) ; \
	done

test_debops: 
	$(eval CFLAGS   = $(TEST_CFLAGS)   $(TEST_INCLUDES) )
	$(eval CXXFLAGS = $(TEST_CXXFLAGS) $(TEST_INCLUDES) )
	$(eval LDFLAGS  = $(TEST_LDFLAGS)  $(TEST_LIBCLUDES))

tests: tests_cpp tests_c

tests_cpp: $(TEST_CPP_SOURCES:.cpp=.o)
	for file in $^ ; do \
		y=$${file%.o} ; \
		exe=$(TESTDIR)/$${y##*/} ; \
		if [ ! -f $$exe ] || [ $$file -nt $$exe ] ; then \
			echo "Recompiling test $$exe ..." ; \
			$(CXX) -o $$exe $$file $(TEST_LIBS) $(LDFLAGS) ; \
		fi ; \
	done

# y=$${file%.o} ; \
# $(CXX) -o $(TESTDIR)/$${y##*/} $$file $(TEST_LIBS) $(LDFLAGS) ; \

tests_c: $(TEST_C_SOURCES:.c=.o)
	for file in $^ ; do \
		y=$${file%.o} ; \
		$(CC) -o $(TESTDIR)/$${y##*/} $$file $(TEST_LIBS) $(LDFLAGS) ; \
	done

#===================================#

clean:
	$(RM) *.o */*.o */*/*.o */*/*/*.o

clean_all: clean
	$(RM) -rf $(BINDIR) $(LIBDIR)

# $(RM) -rf $(BINDIR)/* $(BINDIR)/*/* $(BINDIR)/*/*/* $(LIBDIR)/* 

