#include <glogpp.hpp>
#include <cstdarg>

namespace gtools{

GLogger::GLogger() : 
    basicStream( std::make_shared< std::ostream& >( std::cout ) )
{}

GLogger::GLogger( std::ostream& out ) : 
    basicStream( std::make_shared< std::ostream& >( out ) ) 
{}

void GLogger::setFile( std::ostream& fl ){
    basicStream = make_shared< std::ostream& >( fl );
}

std::ostream& GLogger::getFile() const {
    return basicStream.get();
}

bool GLogger::isActive() const {
    return active;
}

bool GLogger::setActive(bool val){
    active = val;
}
        
template <typename T>
GLogger& GLogger::operator<<(const T& val){
    basicStream.get() << val;
    return *this;   
}

void GLogger::logf(const char* fmt, ...){
    va_list vl;
    va_start(vl, fmt);

    // NOT WORKING.

    va_end(vl);
}

    
}
