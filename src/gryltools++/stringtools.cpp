#include "stringtools.hpp"
#include <sstream>
#include <iomanip>
#include <algorithm>

namespace gtools{

void StringTools::escapeSpecials( std::string& str, bool escapeExtended ){
    std::string buf;
    buf.reserve(8);
    for(size_t i = 0; i < str.size(); i++){
        if( getEscapeSequence( str[i], buf, escapeExtended ) ){
            str.erase(i, 1);
            str.insert(i, buf);
        }
    }
}

bool StringTools::getEscapeSequence( char c, std::string& escapeBuff, bool escapeExtended ){
    switch( c ){
    case '\0':
        escapeBuff = "\\0"; return true;
    case '\'':
        escapeBuff = "\\\'"; return true;
    case '\"':
        escapeBuff = "\\\""; return true;
    case '\?':
        escapeBuff = "\\?"; return true;
    case '\\':
        escapeBuff = "\\\\"; return true;
    case '\a':
        escapeBuff = "\\a"; return true;
    case '\b':
        escapeBuff = "\\b"; return true;
    case '\f':
        escapeBuff = "\\f"; return true;
    case '\n':
        escapeBuff = "\\n"; return true;
    case '\r':
        escapeBuff = "\\r"; return true;
    case '\t':
        escapeBuff = "\\t"; return true;
    case '\v':    
        escapeBuff = "\\v"; return true;
    }
    
    if( escapeExtended && (c < 32 || c >= 127) ){
        escapeBuff = "\\x" + getHexValue( &c, 1, HEX_LEADING_ZERO );
        return true;
    }

    return false;
}

bool StringTools::isEscapable( char c, bool escapeExtended ){
    if( c=='\0' || c=='\'' || c=='\"' || c=='\?' || c=='\\' || c=='\v' ||
        c=='\a' || c=='\b' || c=='\f' || c=='\n' || c=='\r' || c=='\t' ||
        ( escapeExtended && (c < 32 || c >= 127) ) )
        return true;
    return false;
}

void StringTools::getHexValue( const char* arr, size_t size, 
                        std::string& str, int flags ){
    std::stringstream sstr;
    if( flags & HEX_0X )
        sstr<<"0x";

    for(const char* c = arr; c < arr + size; c++){
        sstr << std::setfill('0') << std::setw(2) << std::hex;
        sstr << (((unsigned int)*c) & 0x000000FF);
    }
    str.assign( std::move( sstr.str() ) );

    if( flags & HEX_CAPS )
        std::transform( str.begin(), str.end(), str.begin(), ::toupper );

    if( !(flags & HEX_LEADING_ZERO) ){
        size_t start = ( ( flags & HEX_0X ) ? 2 : 0 );
        // Remove leading zeroes.
        while( str[start] == '0' && (start < str.size()-1) )
            str.erase( start, 1 );
    }

}

std::string StringTools::getHexValue( const char* arr, size_t size, int flags ){
    std::string str;
    getHexValue( arr, size, str, flags );
    return str;
}

}

