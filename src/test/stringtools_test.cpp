#include <string>
#include <gryltools/stringtools.hpp>
#include <cassert>
#include <cstring>
#include <iostream>

bool debug = false;

void testEscapeSpecials(const std::string& input, const std::string& res, bool escExt=false){
    std::string tmp = input;
    gtools::StringTools::escapeSpecials( tmp, escExt );
    if( debug ){
        std::cout<<"[testEscapeSpecials]:\n endBuff: \""<< tmp <<"\"\n";
        std::cout<<" resBuff: \""<<res<<"\"\n Assert size...\n";
    }
    
    assert( tmp.size() == res.size() );

    if( debug )
        std::cout<<" Assert content...\n";
    
    assert( tmp == res );

    /*for(int i = 0; i < tmp.size(); i++){
        if(tmp[i] != res[i]){
            std::cout<<"Char "<<i<<" : "<<tmp[i]<<" is not equal!\n";
            break;
        }
    }*/
}

void testGetEscapeSequence( char c, const std::string& res, bool escExt = true ){
    std::string tmp;
    bool r = gtools::StringTools::getEscapeSequence( c, tmp, escExt );
    if( debug )
        std::cout<<"[testGetEscapeSequence]: endBuff: \""<< tmp <<"\"\n";
    
    assert( r == !tmp.empty() );
    assert( tmp == res );
}

void testIsEscapable( char c, bool res, bool escExt = true ){
    if( debug )
        std::cout<<"[testIsEscapable]: \n";
    assert( gtools::StringTools::isEscapable( c, escExt ) == res );
}

void testGetHexValue( const char* arr, size_t size, const std::string& res, int flgs = -1){
    std::string tmp; 
    if( flgs == -1)
        tmp = gtools::StringTools::getHexValue( arr, size );
    else
        tmp = gtools::StringTools::getHexValue( arr, size, flgs );

    if( debug )
        std::cout<<"[testGetHexValue]:\n endBuff: \""<< tmp <<"\"\n";
    
    assert( tmp == res );
}

int main(int argc, char** argv){
    if( argc > 1 ){
        if( !strcmp( argv[1], "-v") || !strcmp( argv[1], "--debug") || 
            !strcmp( argv[1], "--verbose") )
            debug = true;
    }

    std::cout<<"[ Testing gtools::StringTools ] ... ";
    if(debug)
        std::cout << "\n";

    // Hex Value Case
    if(debug)
        std::cout<<"\n";

    testGetHexValue( "\x01", 1, "0x01" );
    testGetHexValue( "a", 1, "0x61", gtools::StringTools::HEX_0X );
    testGetHexValue( "a\x08", 2, "0x6108" );
    testGetHexValue( "\00\x08", 2, "0x0008" );
    testGetHexValue( "\00\x08", 2, "8", 0 );
    testGetHexValue( "\00ab\x0F", 4, "0X61620F", gtools::StringTools::HEX_CAPS | 
                                             gtools::StringTools::HEX_0X );
    testGetHexValue( "\0\xF8", 2, "0x00f8" );
    testGetHexValue( "\xef", 1, "0xef" );

    // getEscapeSequence
    if(debug)
        std::cout<<"\n";

    testGetEscapeSequence('a', "", true);
    testGetEscapeSequence('\x5B', "", true);
    testGetEscapeSequence('\n', "\\n", true);
    testGetEscapeSequence('\0', "\\0", true);
    testGetEscapeSequence('\"', "\\\"", true);
    testGetEscapeSequence('\xF1', "\\xf1", true);
    testGetEscapeSequence('\x01', "\\x01", true);
    testGetEscapeSequence('\xB1', "", false);

    // escapeSpecials
    if(debug)
        std::cout<<"\n";
 
    testEscapeSpecials( std::string("abc\n\t\0\0za\0\x10nyaa\xD1\xCF", 17), 
                        std::string("abc\\n\\t\\0\\0za\\0\x10nyaa\xd1\xcf", 22), false );
    testEscapeSpecials( std::string("abc\n\t\0\0za\0\x10nyaa\xD1\xCF", 17), 
                        "abc\\n\\t\\0\\0za\\0\\x10nyaa\\xd1\\xcf", true ); 
    testEscapeSpecials( std::string("\0\0\0\0\0\x01\x01\x01\n\0\t", 11),
                        "\\0\\0\\0\\0\\0\\x01\\x01\\x01\\n\\0\\t", true );
    testEscapeSpecials( std::string("\0\0\0\0\0\x01\x01\x01\n\0\t", 11),
                        "\\0\\0\\0\\0\\0\x01\x01\x01\\n\\0\\t", false ); 
    testEscapeSpecials( "\"\\\'\"", "\\\"\\\\\\\'\\\"", true );
                    //    "\'"         \"\\\'\"
    if(debug)
        std::cout<<"\n"; 
    std::cout<<"[ Passed! ]\n";

    return 0;
}




