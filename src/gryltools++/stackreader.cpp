#include <iostream>
#include <cstring>
#include "stackreader.hpp"

namespace gtools{

void StackReader::setupStack()
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
 * - If some active data is still on stack, it assumes the data is before the
 *   file fetch sector (before the stackSize-fileReadSize point).
 * @return true if successful.
 */ 
bool StackReader::fetchBuffer()
{
    int red = 0;
    char* readSection = (stackBuffer + stackSize) - fileReadSize;

    if(use_C){
        red = fread( readSection, 1, fileReadSize, cStream );
    }
    else{
        iStream.read( readSection, fileReadSize );  
        red = iStream.gcount();
    }

    if( red <= 0 ){
        if( stackEnd - stackPtr <= 0 ) // No data
            readable = false;
        streamReadable = false;
        return false;
    }
    else{
        if( stackPtr > readSection )
            stackPtr = readSection;

        // Set a stack end to how much we've read. 
        stackEnd = readSection + red;
    }
    return true;
}

/*! Function ensures that stack has at least some space in front and back.
 *  - Reallocated the memory if needed.
 *  @param frontSpace  - minimum space on front to be ensured.
 *  @param backSpace   - minimum space on back to be ensured.
 *  @param moveAllowed - if enough free space, but bad ballance, move data.
 *  @return true if successful.
 */ 
bool StackReader::ensureSpace( size_t frontSpace, size_t backSpace, bool moveAllowed )
{
    size_t newStackSize = 0, newDataStart = 0; // New stack properties.
    size_t dataSz = stackEnd - stackPtr;
        
    // Check if data is present on stack.
    if( dataSz ){ 
        size_t currFront = stackPtr - stackBuffer;
        size_t currBack  = (stackBuffer + stackSize) - stackEnd;
 
        //if( (stackBuffer + frontSpace <= stackPtr) && 
        //    (stackEnd + backSpace <= stackBuffer + stackSize) )
        
        // Check if current spaces on front and back are good.
        if( currFront >= frontSpace && currBack >= backSpace )
            return true;
        else{ 
            // Check if current free space is good, so we just need to move data.
            if( moveAllowed && (currFront + currBack >= frontSpace + backSpace) ){
                newDataStart = frontSpace;
            }
            else{ // Not enough free space. Reallocate!
                // Set new sizes, and the new position of Data.
                currFront = (currFront > frontSpace ? currFront : frontSpace);
                currBack  = (currBack > backSpace ? currBack : backSpace);

                newStackSize = currFront + (dataSz) + currBack;
                newDataStart = currFront;
            }
        }
    }
    else{ // No data is present - just check if current size is good.
        if( stackSize >= frontSpace + backSpace )
            return true;
        else{ // Set the reallocated memory new size.
            newStackSize = frontSpace + backSpace; 
        }
    }

    // At this point some move or reallocation is needed. 
    char* moveDestination = stackBuffer;

    // Check if we have to allocate new memory.
    if( newStackSize ){
        stackSize = newStackSize;
        moveDestination = new char[ newStackSize ];

        if( !moveDestination )
            return false;
    }

    // Check if memmoves needed.
    if( newDataStart ){
        std::memmove( moveDestination + newDataStart, stackPtr, dataSz );
        // Set new stack pointers.
        stackPtr = moveDestination + newDataStart;
        stackEnd = stackPtr + dataSz;
    }

    // If we have reallocated stuff, we need to delete old buffer, and set the new pointer.
    if( newStackSize ){
        delete[] stackBuffer;
        stackBuffer = moveDestination;
    }

    return true;
}

inline bool StackReader::checkSetReadable(){
    if( !readable )
        return false;
    else if( stackPtr >= stackEnd ){
        if( !fetchBuffer() )
            return false;
    }
    return true;
}


bool StackReader::isReadable() const {
    return readable;
}

bool StackReader::getChar( char& chr, int skipmode )
{
    if(skipmode != SKIPMODE_NOSKIP){
        // This should take care of buffer update 
        if( !skipWhitespace( skipmode ) )
           return false; 
    }
    else if( !checkSetReadable() )
        return false;

    // Now get current char and increment the head.
    chr = *( stackPtr++ );

    return true;
}

size_t StackReader::getString( char* st, size_t sz, int skipmode )
{
    if(skipmode != SKIPMODE_NOSKIP){
        // This should take care of buffer update 
        if( !skipWhitespace( skipmode ) )
           return 0; 
    }
    else if( !checkSetReadable() )
        return 0;

    // Now get that string in a Loop!
    size_t read = 0, readTotal = 0;
    while(readTotal < sz && readable){
        read = (size_t)(stackEnd - stackPtr);
        read = (readTotal+read > sz ? sz-readTotal : read);

        std::memcpy( st + readTotal, stackPtr, read );
        stackPtr += read;
        readTotal += read;

        if(stackPtr >= stackEnd){
            if( !fetchBuffer() ) // No more to read
                break; // Because we still read something.
        }
    }

    return readTotal;
}

size_t StackReader::getString( std::string& str, int skipmode )
{
    return getString( &(str[0]), str.size(), skipmode );
}

bool StackReader::skipWhitespace( int skipmode )
{
    size_t a,b;
    return skipWhitespace(skipmode, a, b);
}

bool StackReader::skipWhitespace( int skipmode, size_t& endlines, size_t& posInLine ){
    if(!readable)
        return false;
    
    char* curPosAfterEndl = stackPtr;
    while(1){
        while(stackPtr < stackEnd){
            char c = *stackPtr; // Get a character at current stack pointer position.

            if(c == '\n'){
                ++endlines;
                curPosAfterEndl = stackPtr;

                if(skipmode == SKIPMODE_SKIPWS_NONEWLINE){
                    posInLine = (size_t)(stackPtr - curPosAfterEndl);
                    return true; // Because newlines are not skipped.
                }
            }
            else if( !std::isspace( c ) ){ // Non-whitespace character found.
                posInLine = (size_t)(stackPtr - curPosAfterEndl);
                return true;
            }

            ++stackPtr;
        }
        // If reached this point, it means buffer is exhausted.
        if( !fetchBuffer() )
            break; // No more to read.
    } 

    posInLine = (size_t)(stackPtr - curPosAfterEndl);
    return false;
}

bool StackReader::skipUntilChar( char chr ){ 
    size_t a,b;
    return skipUntilChar( chr, a, b );
}

bool StackReader::skipUntilDelim( const std::string& delims ){
    size_t a,b;
    return skipUntilDelim( delims, a, b );
}

bool StackReader::skipUntilChar( char chr, size_t& endls, size_t& posls )
{
    return skipUntil( [chr](char c){ 
        if( c == chr ) // char found
            return true;
        return false;        
    }, endls, posls );   
}

bool StackReader::skipUntilDelim( const std::string& delims, size_t& endls, size_t& posls )
{
    return skipUntil( [ &delims ](char c){ 
        if(delims.find( c ) != std::string::npos) // char found
            return true;
        return false;        
    }, endls, posls );   
}

bool StackReader::skipUntil( std::function< bool(char) > delimCallback, 
                             size_t& endlines, size_t& posInLine ) 
{
    if(!readable)
        return false;
    
    size_t lastPos = (size_t)stackPtr;
    while(1){
        while(stackPtr < stackEnd){
            char c = *stackPtr; // Get a character at current stack pointer position.

            if(c == '\n'){
                ++endlines;
                lastPos = (size_t)stackPtr;
            }
            else if( delimCallback( c ) ){ // Non-whitespace character found.
                lastPos = (size_t)stackPtr - lastPos;
                return true;
            }

            ++stackPtr;
        }
        // If reached this point, it means buffer is exhausted.
        if( !fetchBuffer() )
            break; // No more to read.
    } 
    lastPos = (size_t)stackPtr - lastPos;
    return false;
}


bool StackReader::putChar( char c )
{
    if(stackPtr <= stackBuffer){ // Full buffer
        if( !ensureSpace(1, 0, true) ) // Reallocate data to a bigger one
            return false;
    }

    *(--stackPtr) = c;

    return true;
}

bool StackReader::putString( const char* str, size_t sz )
{
    if( (stackPtr-sz) <= stackBuffer ){ // Full buffer
        if( !ensureSpace(sz, 0, true) ) // Reallocate data to a bigger one
            return false;
    }

    stackPtr -= sz;
    std::memmove( stackPtr, str, sz );

    return true;
}

// TODO:
bool StackReader::unRead( size_t sz )
{
    // Possible to safely unread only if no relocations were made in past 'sz'-long reads.
    /*if( movesAfterRelocation >= sz && (stackPtr - stackBuffer >= sz) ){
        stackPtr -= sz;
        return true;
    }*/
    return false;
}

}

