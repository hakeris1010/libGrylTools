#include "stringtools.hpp"

namespace gtools{

void StringTools::escapeSpecials( std::string& str, bool escapeExtended ){
    for(size_t i = 0; i < str.size(); i++){
        if(str[i]=='\'' || \
    }
}

bool StringTools::getEscapeSequence( char c, std::string& escapeBuff, bool escapeExtended ){
    switch(a){
    case '\0':
        escapeBuff = "\\0"; return;
    case '\'':
        escapeBuff = "\\\'"; return;
    case '\"':
        escapeBuff = "\\\""; return;
    case '\?':
        escapeBuff = "\\?"; return;
    case '\\':
        escapeBuff = "\\\\"; return;
    case '\a':
        escapeBuff = "\\a"; return;
    case '\b':
        escapeBuff = "\\b"; return;
    case '\f':
        escapeBuff = "\\f"; return;
    case '\n':
        escapeBuff = "\\n"; return;
    case '\r':
        escapeBuff = "\\r"; return;
    case '\t':
        escapeBuff = "\\t"; return;
    case '\v':    
        escapeBuff = "\\v"; return;
    }
    
    if( escapeExtended && (c < 32 || c >= 127) ){
        escapeBuff = "\\x";
        escapeBuff.push_back( (c/16) +  );
    }
}

bool StringTools::isEscapable( char c, bool escapeExtended ){
    std::string tmp;
    getEscapeSequence( c, tmp, escapeExtended );
    return !tmp.empty();
}

}

