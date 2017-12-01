/** GLog++ logger.
 *  - Base class for loggers. 
 *  - Also Includes an overly-accessible extern logger.
 */ 

#include <iostream>
#include <memory>

namespace gtools{

class GLogger{
    protected:
        std::shared_ptr< std::ostream& > basicStream;
        bool active = true;

    public:
        GLogger();
        GLogger( std::ostream& strm );
        virtual ~GLogger() {}

        virtual void setFile( std::ostream& fl );
        virtual std::ostream& getFile() const;

        bool isActive() const;
        bool setActive(bool val);
        
        template <typename T>
        virtual GLogger& operator<<(const T& val);

        virtual void logf(const char* fmt, ...);
};

extern GLogger log;

}
