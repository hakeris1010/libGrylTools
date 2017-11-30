#ifndef BLOCKREADER_HPP_INCLUDED
#define BLOCKREADER_HPP_INCLUDED

#include <istream>
#include <cstdio>

namespace gtools{

class BlockReader{
    protected:
        std::istream& iStream;
        FILE* cStream = nullptr;
        std::string priorityQueue;
        
        size_t queuePosition;
        size_t lineCount = 0;
        size_t posInLine = 0;

        std::vector< std::pair<char, size_t> > countsOfChars;
        std::string monitoredChars;

        const bool use_C;
        bool readable = true;
        bool lineStats = true;

        bool updateBuffer();


    public:
        const static size_t DEFAULT_PRIORITY_CAPACITY = 256;

        const static int SKIPMODE_NOSKIP = 0;
        const static int SKIPMODE_SKIPWS = 1;
        const static int SKIPMODE_SKIPWS_NONEWLINE = 2;

        BlockReader( std::istream& is, size_t priorityCap = DEFAULT_PRIORITY_CAPACITY );
        BlockReader( FILE* inp, size_t priorityCap = DEFAULT_PRIORITY_CAPACITY );
        virtual ~BlockReader();

        bool isReadable();
        bool getChar  ( char& c, int skipmode = SKIPMODE_NOSKIP );
        bool getString( char* st, size_t sz, int skipmode = SKIPMODE_NOSKIP );
        bool getString( std::string& str, int skipmode = SKIPMODE_NOSKIP );

        bool skipWhitespace( int skipmode = SKIPMODE_SKIPWS );

};

}

#endif // BLOCKREADER_HPP_INCLUDED

