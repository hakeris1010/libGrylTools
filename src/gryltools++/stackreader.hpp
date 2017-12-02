#ifndef STACKREADER_HPP_INCLUDED
#define STACKREADER_HPP_INCLUDED

#include <istream>
#include <cstdio>
#include <string>
#include <functional>

namespace gtools{

class StackReader{
protected:
    std::istream& iStream;
    FILE* cStream = nullptr;

    // The stack consists of 2 parts - the filereaded buffer, and the
    // priority buffer - extra space for storing putback'd bytes.
    // Once the stack becomes empty, the updateBuffer() is triggered, 
    // reading another chunk from the stream.
    
    char* stackBuffer;

    size_t stackSize;
    char* stackPtr;
    char* stackEnd;
    size_t fileReadSize = DEFAULT_READBUFFER;

    const bool use_C;
    bool readable = true;
    bool streamReadable = true;

    void setupStack();
    bool fetchBuffer();
    bool ensureSpace( size_t frontSpace, size_t backSpace, bool moveAllowed = true );

    const static int STACK_REALLOC_SPACE_FRONT = 1;
    const static int STACK_REALLOC_SPACE_BACK  = 2;

public:
    const static size_t DEFAULT_READBUFFER = 256;
    const static size_t DEFAULT_PRIORITY_STACK = 256; 
    const static size_t DEFAULT_GROWTH = 256; 

    const static int SKIPMODE_NOSKIP = 0;
    const static int SKIPMODE_SKIPWS = 1;
    const static int SKIPMODE_SKIPWS_NONEWLINE = 2;

    StackReader( std::istream& is, size_t bufferSize = DEFAULT_READBUFFER, 
                                   size_t prioritySize = DEFAULT_PRIORITY_STACK );
    StackReader( FILE* inp, size_t bufferSize = DEFAULT_READBUFFER, 
                            size_t prioritySize = DEFAULT_PRIORITY_STACK );

    virtual ~StackReader();

    bool isReadable() const ;
    bool getChar  ( char& c, int skipmode = SKIPMODE_NOSKIP );
    bool getString( char* st, size_t sz, int skipmode = SKIPMODE_NOSKIP );
    bool getString( std::string& str, int skipmode = SKIPMODE_NOSKIP );

    bool skipWhitespace( int skipmode = SKIPMODE_SKIPWS );
    bool skipWhitespace( int skipmode, size_t& endlines, size_t& posInLine );
    //bool skipUntil( bool (*delimCallback)(char), size_t& endlines, size_t& posInLine );
    bool skipUntil( std::function< bool(char) > delimCbk, size_t& endls, size_t& posls ); 

    bool skipUntilChar( char chr );
    bool skipUntilChar( char chr, size_t& endls, size_t& posls );
    bool skipUntilDelim( const std::string& delims );
    bool skipUntilDelim( const std::string& delims, size_t& endls, size_t& posls );

    bool putChar( char c );
    bool unRead( size_t sz );
    bool putString( const char* str, size_t sz );
};

}

#endif // STACKREADER_HPP_INCLUDED

