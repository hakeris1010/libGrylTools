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
    inline bool checkSetReadable();

    const static int STACK_REALLOC_SPACE_FRONT = 1;
    const static int STACK_REALLOC_SPACE_BACK  = 2;

public:
    const static size_t DEFAULT_READBUFFER = 256;
    const static size_t DEFAULT_PRIORITY_STACK = 256; 
    const static size_t DEFAULT_GROWTH = 256; 

    const static int SKIPMODE_NOSKIP = 0;
    const static int SKIPMODE_SKIPWS = 1;
    const static int SKIPMODE_SKIPWS_NONEWLINE = 2;

    StackReader( std::istream& is, size_t prioritySize = DEFAULT_PRIORITY_STACK,
                                   size_t bufferSize = DEFAULT_READBUFFER );
                                   
    StackReader( FILE* inp, size_t prioritySize = DEFAULT_PRIORITY_STACK, 
                            size_t bufferSize = DEFAULT_READBUFFER );

    virtual ~StackReader();

    bool isReadable();
    size_t getFrontSize() const;
    size_t getBackSize() const;
    size_t currentLength() const;

    bool getChar( char& chr );
    bool getChar( char& c, int skipmode );
    bool getChar( char& c, int skipmode, size_t& endlines, size_t& posInLine );

    void getCharUnsafe( char& chr );
    void getStringUnsafe( char* str, size_t len );

    size_t getString( std::string& str, int skipmode = SKIPMODE_NOSKIP );
    size_t getString( char* st, size_t sz, int skipmode = SKIPMODE_NOSKIP );
    size_t getString( char* st, size_t sz, int skipmode, size_t& endlines, size_t& posInLine );

    bool skipWhitespace( int skipmode = SKIPMODE_SKIPWS );
    bool skipWhitespace( int skipmode, size_t& endlines, size_t& posInLine );
    bool skipUntil( std::function< bool(char) > delimCbk, size_t& endls, size_t& posls ); 

    bool skipUntilChar( char chr );
    bool skipUntilChar( char chr, size_t& endls, size_t& posls );
    bool skipUntilDelim( const std::string& delims );
    bool skipUntilDelim( const std::string& delims, size_t& endls, size_t& posls );

    bool putChar( char c );
    bool unRead( size_t sz );
    bool putString( const char* str, size_t sz );
    bool putString( const std::string& str );
};

}

#endif // STACKREADER_HPP_INCLUDED

