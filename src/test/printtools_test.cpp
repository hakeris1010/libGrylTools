#include <sstream>
#include <iostream>
#include <vector>
#include <cassert>
#include <cstring>
#include <gryltools/printtools.hpp>

using namespace gtools::PrintTools;

bool debug = false;

template<typename InputIterator, typename Callback>
void testInitializerList( InputIterator fr, InputIterator lst, 
        Callback clb, const std::string& result, 
        const ListOutputParams& pars = ListOutputParams() )
{
    std::stringstream sstr;
    outputInitializerList( sstr, fr, lst, clb, pars );

    if( debug )
        std::cout << "[testInitializerList]: End result:\n" << sstr.str() <<"\n";

    assert( result == sstr.str() );
}

int main(int argc, char** argv){
    if( argc > 1 ){
        if( !strcmp( argv[1], "-v") || !strcmp( argv[1], "--debug") || 
            !strcmp( argv[1], "--verbose") )
            debug = true;
    }

    std::cout<<"[ Testing gtools::PrintTools  ] ... ";
    
    std::vector< std::string > strs({"kawaii", "desu", "nyaa~~ :3"});
    testInitializerList( strs.begin(), strs.end(),
        [](auto& os, auto& item, auto& props){
            os << "[..::"<<item<<"::..]";
        },
        "{ \n    [..::kawaii::..] , \n    [..::desu::..] , \n    " \
        "[..::nyaa~~ :3::..]\n}"
    );

    std::cout<<"[ Passed! ]\n";

    return 0;
}

