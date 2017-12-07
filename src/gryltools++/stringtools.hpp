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
        static void escapeSpecials( std::string& str, bool escapeExtendeds = false );
        bool StringTools::getEscapeSequence( char c, std::string& escapeBuff, 
                                             bool escapeExtended );
        bool StringTools::isEscapable( char c, bool escapeExtended );
}

}

#endif // STRINGTOOLS_HPP_INCLUDED

