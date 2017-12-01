#include <iostream>
#include "stackreader.hpp"

namespace gtools{

StackReader::setupStack()
{
    // Allocate a block capable of holding this much characters.
    stackPtr = stackBuffer + (stackSize - 1);
    stackEnd = stackBuffer + stackSize;

    stackBuffer = new char[ stackSize ];
}

StackReader::StackReader( std::istream& is, size_t bufferSize, size_t prioritySize ) 
    : iStream( is ), use_C( false ), fileReadSize( bufferSize ), 
      stackSize( prioritySize + bufferSize )
{
    setupStack();
}

StackReader::StackReader( FILE* inp, size_t bufferSize, size_t prioritySize )
    : iStream( std::cin ), cStream( inp ), use_C( true ), 
      fileReadSize( bufferSize ), stackSize( prioritySize + bufferSize )
{
    setupStack();
}

// Destructor does nothing - even if using C file streams, we haven't 
// created that stream.
StackReader::~StackReader()
{
    if(stackBuffer)
        delete[] stackBuffer;
}

/*! If called, it will fetch n='fileReadSize' characters from a stream.
 * - The data gets put to stack, overwriting any existing data. 
 */ 
bool StackReader::updateBuffer()
{
    int red = 0;
    if(use_C){
        red = fread( (stackBuffer + stackSize) - fileReadSize,
                     1, fileReadSize, cStream );
    }
    else{
        iStream.read( (stackBuffer + stackSize) - fileReadSize, fileReadSize );  
        red = iStream.gcount();
    }

    if( red <= 0 ){
        readable = false;
        return false;
    }
    else{
        stackPtr = (stackBuffer + stackSize) - fileReadSize;
        if(red != fileReadSize)
            stackEnd = stackPtr + red; // Set a stack end by how much we read.
    }
}

/*! Get the buffer ready for fetching. Will do necessary moves if some data
 *  is still in da buffer.
 */ 
void prepareBuffer()
{
    // If data exists
    if(stackPtr != (stackEnd-1)){
        size_t cnt = stackEnd - stackPtr;
        // Check if the currently-inbuf char count is higher than the putback space.
        if(cnt > (stackSize-fileReadSize)){ // If yes, realloc da buffer.
            // Just make it bigger. 
            stackSize = stackSize + cnt; 
            char* tmp = new char[ stackSize ]; 
            if(!tmp){
                readable = false;
                delete[] stackBuffer;
                return;
            }

            // Move the data to the new array - so that the end would be just on the 
            // File Fetch Section start.
            std::memmove( tmp + (stackSize - fileReadSize - cnt), stackPtr, cnt );
            delete[] stackBuffer;

            stackBuffer = tmp;
            stackPtr = tmp + (stackSize - fileReadSize - cnt);
            stackEnd = stackPtr + cnt;
        }
    }
    // If no data, we don't have to do anything.
}

bool StackReader::isReadable()
{
    return readable;
}

bool StackReader::getChar( char& c, int skipmode )
{
    if(skipmode != SKIPMODE_NOSKIP)
        skipWhitespace( skipmode );

    // And continue the programming.
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

