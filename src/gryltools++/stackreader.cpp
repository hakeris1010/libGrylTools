#include "stackreader.hpp"

namespace gtools{

StackReader::StackReader( std::istream& is, size_t bufferSize, size_t prioritySize ) 
    : iStream( is ), use_C( false )
{
    // Allocate a block capable of holding this much characters.
    priorityStack.assign( prioritySize + bufferSize, '\0' );
    stackPtr = priorityStack.size() - 1;
}

StackReader::StackReader( FILE* inp, size_t bufferSize, size_t prioritySize )
    : cStream( inp ), use_C( true )
{
    
}


virtual StackReader::~StackReader()
{
    
}


bool StackReader::updateBuffer()
{
    
}


bool StackReader::isReadable()
{
    
}

bool StackReader::getChar  ( char& c, int skipmode )
{
    
}

bool StackReader::getString( char* st, size_t sz, int skipmode )
{
    
}

bool StackReader::getString( std::string& str, int skipmode )
{
    
}


bool StackReader::skipWhitespace( int skipmode )
{
    
}

bool StackReader::skipUntil( char c )
{
    
}


bool StackReader::putChar( char c )
{
    
}

bool StackReader::putString( const char* str, size_t sz )
{
    
}




}

