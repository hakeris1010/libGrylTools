#include <iostream>
#include "stackreader.hpp"

namespace gtools{

StackReader::StackReader( std::istream& is, size_t bufferSize, size_t prioritySize ) 
    : iStream( is ), use_C( false ), fileReadSize( bufferSize )
{
    // Allocate a block capable of holding this much characters.
    priorityStack.assign( prioritySize + bufferSize, '\0' );
    stackPtr = priorityStack.size() - 1;
    stackEnd = stackPtr+1;
}

StackReader::StackReader( FILE* inp, size_t bufferSize, size_t prioritySize )
    : iStream( std::cin ), cStream( inp ), use_C( true ), fileReadSize( bufferSize )
{
    // Allocate a block capable of holding this much characters.
    priorityStack.assign( prioritySize + bufferSize, '\0' );
    stackPtr = priorityStack.size() - 1;
    stackEnd = stackPtr+1;
}

// Destructor does nothing - even if using C file streams, we haven't 
// created that stream.
StackReader::~StackReader(){}

/*! If called, it will fetch n='fileReadSize' characters from a stream.
 * - The data gets put to stack, overwriting any existing data. 
 */ 
bool StackReader::updateBuffer()
{
    int red = 0;
    if(use_C){
        red = fread( ( &priorityStack[0] + priorityStack.size() ) - fileReadSize,
                     1, fileReadSize, cStream );
    }
    else{
        iStream.read( ( &priorityStack[0] + priorityStack.size() ) - fileReadSize,
                      fileReadSize );  
        red = iStream.gcount();
    }

    if( red <= 0 ){
        readable = false;
        return false;
    }
    else{
        stackPtr = priorityStack.size() - fileReadSize; 
        if(red != fileReadSize)
            stackEnd = stackPtr + red; // Decrement a stack size by how much we read.
    }
}

/*! Get the buffer ready for fetching. Will do necessary moves if some data
 *  is still in da buffer.
 */ 
void prepareBuffer(size_t charsToFetch)
{
    if(stackPtr != (stackEnd-1))
        ;
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

