#ifndef STRINGTOOLS_HPP_INCLUDED
#define STRINGTOOLS_HPP_INCLUDED

#include <string>

namespace gtools{

class StringTools{
private:
    StringTools() = delete;
    StringTools(const StringTools&) = delete;
    StringTools(StringTools&&) = delete;
    StringTools& operator=(const StringTools&) = delete;
    StringTools& operator=(StringTools&&) = delete;

public:
    const static int HEX_0X           = 1;
    const static int HEX_LEADING_ZERO = 2;
    const static int HEX_CAPS         = 4;

    static void escapeSpecials( std::string& str, bool escapeExtendeds = false );
    static bool getEscapeSequence( char c, std::string& escapeBuff, bool escapeExtended );
    static bool isEscapable( char c, bool escapeExtended = true );
    static void getHexValue( const char* arr, size_t size, std::string& str, 
            int flg=HEX_0X | HEX_LEADING_ZERO );
    static std::string getHexValue( const char* arr, size_t size, 
            int flg=HEX_0X | HEX_LEADING_ZERO );
};

}

#endif // STRINGTOOLS_HPP_INCLUDED

