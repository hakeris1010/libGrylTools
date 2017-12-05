#include <iostream>
#include <sstream>
#include <cassert>
#include <algorithm>
#include <gryltools/stackreader.hpp>

static bool debug = false;


void testGetCharWithEndPos( gtools::StackReader& rdr, const char* result, 
                            size_t resSize, size_t endPosition ) {
    char c;
    if(debug)
        std::cout<<"[ Test GetChar ]: buffer:\n\"";
    for( const char* i = result; i < result + resSize; i++ ){
        assert( rdr.getChar( c ) );
        if(debug)
            std::cout<< c;

        assert( c == *i );
        if( i - result < endPosition )
            assert( rdr.isReadable() );
    }
    if(debug)
        std::cout<<"\"\n";
}

void testGetCharWithEndPos( gtools::StackReader& rdr, const std::string& res, size_t endPos ){
    testGetCharWithEndPos( rdr, res.c_str(), res.size(), endPos );
}

void testGetChar( gtools::StackReader& rdr, const char* result, size_t rSize ){
    testGetCharWithEndPos( rdr, result, rSize, -1 );
}

void testGetChar( gtools::StackReader& rdr, const std::string& result ){
    testGetChar( rdr, result.c_str(), result.size() );
}

void testGetString( gtools::StackReader& rdr, int skipmode, const std::string& result, 
                    size_t lns, size_t pss ){
    std::string buf( result.size() , '\0' );
    size_t startPos = (lns==0 ? 5 : 0);
    size_t startLns = 2;
    size_t n = startLns, p = startPos;

    assert( rdr.getString( &buf[0], buf.size(), skipmode, n, p ) );

    if(debug)
        std::cout<< "[ Test GetString ]: buffer:\n\""<< buf <<  \
                    "\"\n[NuLines: "<<n<<", PosLines: "<<p<<"]\n"; 

    assert( buf == result );
    assert( n == (startLns + lns) );
    assert( p == (startPos + pss) );
}

void testSkipWhitespace(gtools::StackReader& rdr, int skipmode, size_t lns, size_t pss){
    size_t startPos = (lns==0 ? 5 : 0);
    size_t startLns = 2;
    size_t n = startLns, p = startPos;

    assert( rdr.skipWhitespace( skipmode, n, p ) );

    if(debug)
        std::cout<<"[ Skip'd WS: NuLines: "<<n<<", PosLines: "<<p<<"] \n";

    assert( n == (startLns + lns) );
    assert( p == (startPos + pss) );
}

void testSkipUntil( gtools::StackReader& rdr, size_t lns, size_t pss, 
                    std::function< bool(char) > delimCbk) 
{
    size_t startPos = (lns==0 ? 5 : 0);
    size_t startLns = 2;
    size_t n = startLns, p = startPos;

    assert( rdr.skipUntil( delimCbk, n, p ) );

    if(debug)
        std::cout<<"[ SkipUntil: NuLines: "<<n<<", PosLines: "<<p<<"] \n"; 

    assert( n == (startLns + lns) );
    assert( p == (startPos + pss) );
}

void testSkipUntilDelim( gtools::StackReader& rdr, const std::string& delims, 
                           size_t lns, size_t pss )
{
    size_t startPos = (lns==0 ? 5 : 0);
    size_t startLns = 2;
    size_t n = startLns, p = startPos;

    assert( rdr.skipUntilDelim( delims, n, p ) );

    if(debug)
        std::cout<<"[ SkipUntilDelim("<<delims<<"): NuLines: "<<n<<", PosLines: "<<p<<"] \n";
    
    assert( n == (startLns + lns) );
    assert( p == (startPos + pss) );
}

void testLastCharactersByGetString( gtools::StackReader& rdr, const std::string& lastChars,
                                    int skipmode, int lns, int pss) {
    std::string buff( lastChars.size(), '\0' );
    std::string lastBuff;
    // Test position after endline (p), so that if no \n's occur, it just gets incremented.
    size_t startPos = (lns==0 ? 5 : 0);
    size_t startLns = 2;
    size_t red = 0, n = startLns, p = startPos;

    // Read until non-full buffer have been read.
    while( (red = rdr.getString( &buff[0], buff.size(), skipmode, n, p )) == buff.size() ){
        for( auto i : buff ){
            if(i == '\n'){
                p = 0;
                n++;
            } else
                p++;
        }
        lastBuff = buff;
    }
    // The end. now set the end buffer.
    lastBuff.append( buff.c_str(), red );
    lastBuff.erase(0, lastBuff.size() - lastChars.size());

    if(debug)
        std::cout<<"[ Last chars of reader: \""<<lastBuff<<"\", Lines: "<<n<<", Pos:"<<p<<"\n";

    assert( lastBuff == lastChars );
    assert( n == (startLns + lns) );
    assert( p == (startPos + pss) );
    assert( !rdr.isReadable() );
}

void testGetCharUnsafe( gtools::StackReader& rdr, const std::string& res, bool end ){
    char c;
    if(debug)
        std::cout<<"[ Test GetCharUnsafe ]: buffer: \n\"";
    for(const char* i = res.c_str(); i < res.c_str()+res.size(); i++){
        rdr.getCharUnsafe( c );
        assert( c == *i );
        if(debug)
            std::cout<<c;
    }
    if(debug)
        std::cout<<"\"\n[ Test GetCharUnsafe end. Left length: "<<rdr.currentLength()<<" ]\n";

    // Update readiness
    rdr.peekChar();
    if(end)
        assert( !rdr.isReadable() );
    else
        assert( rdr.isReadable() );
}

void testGetStringUnsafe( gtools::StackReader& rdr, const std::string& res, bool end ){
    char c;
    std::string buf( res.size(), '\0' );
    rdr.getStringUnsafe( &buf[0], buf.size() );

    if(debug)
        std::cout<<"[ Test GetStringUnsafe. Left length: "<<rdr.currentLength()<<"\n";

    assert( buf == res );

    // Update readiness
    rdr.peekChar(); 

    if(end)
        assert( !rdr.isReadable() );
    else
        assert( rdr.isReadable() );
}

void testCurrentLength( gtools::StackReader& rdr, size_t len ){
    assert( len == rdr.currentLength() );
    if(debug)
        std::cout<<"[ Current length: "<<rdr.currentLength()<<" ]\n";
}

const char* data = 
    "kawaii desu~~ i'm very cute :3  \n  \n \t  nee~~~   \n\t a   \nabcdef ghijk"
    "  \n  \t    \t  gryllotronix woop woop\n da ting goes skrrrrra bnjab    \n\n"
    "mbababd najbd \n asfbaf ayge u88 \n6a   aff a1337;";

int main( int argc, char** argv ){
    if( argc > 1 ){
        if( !strcmp( argv[1], "-v") || !strcmp( argv[1], "--debug") || 
            !strcmp( argv[1], "--verbose") )
            debug = true;
    }

    std::cout<<"[ Testing gtools::StackReader ] ... ";
    if(debug) std::cout<<"\n";

    // Testing streams and buffers.
    std::istringstream iss( data, std::ios::in | std::ios::binary );
    std::string str(32, ' ');
    char c = 'a';

    // Create a stack reader with fetch size of 8, and front space of 8.
    gtools::StackReader rdr( iss, 8, 8 );

    testGetChar(rdr, data, 10);
    testGetChar(rdr, data+10, 20);

    testGetString(rdr, gtools::StackReader::SKIPMODE_SKIPWS, "nee~~~", 2, 5);
    testSkipWhitespace(rdr, gtools::StackReader::SKIPMODE_SKIPWS, 1, 3 );

    testGetChar(rdr, "a");
    testSkipWhitespace(rdr, gtools::StackReader::SKIPMODE_SKIPWS_NONEWLINE, 0, 3 );
    testGetChar(rdr, "\n");

    rdr.putChar( '-' );
    testGetChar(rdr, "-ab");    

    rdr.putString("\n \n desudesu");
    testGetString(rdr, gtools::StackReader::SKIPMODE_SKIPWS, "desudesucd", 2, 2);

    testGetChar(rdr, "ef gh");    
    testSkipUntil(rdr, 1, 3, [](char c) { 
        if(c == '\t')
            return true;
        return false;
    } );

    testGetString(rdr, gtools::StackReader::SKIPMODE_SKIPWS, "g", 0, 8 );    

    testSkipUntilDelim( rdr, "xi", 0, 9 );

    testLastCharactersByGetString(rdr, "1337;", gtools::StackReader::SKIPMODE_SKIPWS, 5, 16); 
    testCurrentLength(rdr, 0);

    rdr.putChar('B');
    testCurrentLength(rdr, 1);

    testGetCharWithEndPos(rdr,"B", 0);
    testCurrentLength(rdr, 0);
    
    rdr.putString(" \n noot n\n  oot");
    testLastCharactersByGetString(rdr, "oot", gtools::StackReader::SKIPMODE_SKIPWS, 2, 6); 

    rdr.putString("testing unsafe");
    testGetCharUnsafe(rdr, "testin", false);
    testGetStringUnsafe(rdr, "g unsafe", true);
 
    if(debug) std::cout<<"\nTest end. Stack Front: "<<rdr.getFrontSize()<<
                         ", Stack Back: "<<rdr.getBackSize()<<"\n";
    std::cout<<"[ Passed! ]\n";

    return 0;
}

