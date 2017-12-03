#include <iostream>
#include <sstream>
#include <cassert>
#include <gryltools/stackreader.hpp>

static const bool debug = true;

const char* data = 
    "kawaii desu~~ i'm very cute :3  \n  \n \t  nee~~~   \n\t a   \nabcdef ghijk"
    "  \n  \t\n gryllotronix woop woop\n da ting goes skrrrrra bnjab    \n\n"
    "mbababd najbd \n asfbaf ayge u88 6a     ;";

void testGetChar( gtools::StackReader& rdr, const char* result, size_t resSize ){
    char c;
    for( const char* i = result; i < result + resSize; i++ ){
        assert( rdr.getChar( c ) );
        if(debug)
            std::cout<< c;

        assert( c == *i );
        assert( rdr.isReadable() );
    }
    if(debug)
        std::cout<<"\n";
}

void testGetChar( gtools::StackReader& rdr, const std::string& result ){
    testGetChar( rdr, result.c_str(), result.size() );
}

void testGetString( gtools::StackReader& rdr, int skipmode, const std::string& result, 
                    size_t nulines, size_t posline ){
    std::string buf( result.size() , '\0' );
    size_t n=0, p=0;

    assert( rdr.getString( &buf[0], buf.size(), skipmode, n, p ) );
    assert( buf == result );
    assert( n == nulines );
    assert( p == posline );

    if(debug)
        std::cout<<"\n"<< buf <<"\n[NuLines: "<<n<<", PosLines: "<<p<<"]\n";
}

void testSkipWhitespace(gtools::StackReader& rdr, int skipmode, size_t nulines, size_t posline){
    size_t n=0, p=0;

    assert( rdr.skipWhitespace( skipmode, n, p ) );
    assert( n == nulines );
    assert( p == posline );

    if(debug)
        std::cout<<"[ Skip'd WS: NuLines: "<<n<<", PosLines: "<<p<<"] \n";
}

int main(){
    std::cout<<"Testing gtools::StackReader ... ";
    if(debug) std::cout<<"\n";

    // Testing streams and buffers.
    std::istringstream iss( data, std::ios::in | std::ios::binary );
    std::string str(32, ' ');
    char c = 'a';

    // Create a stack reader with fetch size of 16, and front space of 16.
    gtools::StackReader rdr( iss, 3, 5 );

    testGetChar(rdr, data, 10);
    testGetChar(rdr, data+10, 20);

    testGetString(rdr, gtools::StackReader::SKIPMODE_SKIPWS, "nee~~~", 2, 5);
    testSkipWhitespace(rdr, gtools::StackReader::SKIPMODE_SKIPWS, 1, 3 );

    testGetChar(rdr, "a");
    testSkipWhitespace(rdr, gtools::StackReader::SKIPMODE_SKIPWS_NONEWLINE, 1, 0 );

    rdr.putChar( '-' );
    testGetChar(rdr, "-ab");    

 
    if(debug) std::cout<<"\nTest end. Stack Front: "<<rdr.getFrontSize()<<
                         ", Stack Back: "<<rdr.getBackSize()<<"\n";
    std::cout<<"Passed!\n";

    return 0;
}

