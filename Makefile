#mkfile_path := $(abspath $(lastword $(MAKEFILE_LIST)))
#current_dir := $(notdir $(patsubst %/,%,$(dir $(mkfile_path))))

CC = gcc
CXX = g++
AR = ar

CFLAGS= -std=c99 
CXXFLAGS= -std=c++11
LDFLAGS= 

DEBUG_CFLAGS= -g
DEBUG_CXXFLAGS= -g
DEBUG_LDFLAGS=
RELEASE_CFLAGS= -O2
RELEASE_CXXFLAGS= -O2
RELEASE_LDFLAGS=

LIBDIR= ./lib
INCLUDEDIR= ./include
BINDIR= ./bin
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

SOURCES_GRYLTOOLSPP= 

HEADERS_GRYLTOOLSPP= src/gryltools++/blockingqueue.hpp

#--------- Test sources ---------#

TEST1_SOURCES=  src/test/test1.c 
TEST1_LIBS= $(GRYLTOOLS_LIB)
TEST1= $(TESTDIR)/test1

#---------  Test  list  ---------# 

TESTNAME= $(TEST1) 

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


all: debug

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

test_debug: debops $(TESTNAME)

$(TEST1): $(filter %.o, $(TEST1_SOURCES:.cpp=.o) $(TEST1_SOURCES:.c=.o))
	$(CC) -o $@ $^ $(TEST1_LIBS) $(LDFLAGS)

## $1 - target name, $2 - sources, $3 - libs
#define make_test_target
# $1: \$(patsubst %.c,%.o, $2) $3
#	$(CC) -o $@ $^ $(LDFLAGS)
#endef    
#
#$(foreach elem, $(TESTNAME), $(eval $(call make_test_target, elem, )) )

#===================================#

clean:
	$(RM) *.o */*.o */*/*.o */*/*/*.o

clean_all: clean
	$(RM) $(BINDIR)/*/* $(LIBDIR)/* $(TESTNAME)
