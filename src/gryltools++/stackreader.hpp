#ifndef STACKREADER_HPP_INCLUDED
#define STACKREADER_HPP_INCLUDED

#include <istream>
#include <cstdio>
#include <string>

namespace gtools{

class StackReader{
    protected:
        std::istream& iStream;
        FILE* cStream = nullptr;

        // The stack consists of 2 parts - the filereaded buffer, and the
        // priority buffer - extra space for storing putback'd bytes.
        // Once the stack becomes empty, the updateBuffer() is triggered, 
        // reading another chunk from the stream.
        std::string priorityStack;
        size_t stackPtr;

        const bool use_C;
        bool readable = true;

        bool updateBuffer();

    public:
        const static size_t DEFAULT_READBUFFER = 256;
        const static size_t DEFAULT_PRIORITY_STACK = 256; 

        const static int SKIPMODE_NOSKIP = 0;
        const static int SKIPMODE_SKIPWS = 1;
        const static int SKIPMODE_SKIPWS_NONEWLINE = 2;

        StackReader( std::istream& is, size_t bufferSize = DEFAULT_READBUFFER, 
                                       size_t prioritySize = DEFAULT_PRIORITY_STACK );
        StackReader( FILE* inp, size_t bufferSize = DEFAULT_READBUFFER, 
                                size_t prioritySize = DEFAULT_PRIORITY_STACK );

        virtual ~StackReader();

        bool isReadable();
        bool getChar  ( char& c, int skipmode = SKIPMODE_NOSKIP );
        bool getString( char* st, size_t sz, int skipmode = SKIPMODE_NOSKIP );
        bool getString( std::string& str, int skipmode = SKIPMODE_NOSKIP );

        bool skipWhitespace( int skipmode = SKIPMODE_SKIPWS );
        bool skipUntil( char c );

        bool putChar( char c );
        bool putString( const char* str, size_t sz );

};

}

#endif // STACKREADER_HPP_INCLUDED

