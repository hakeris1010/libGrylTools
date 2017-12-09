#include <sstream>
#include <iostream>
#include <vector>
#include <cassert>
#include <cstring>
#include <gryltools/printtools.hpp>
#include <gryltools/stringtools.hpp>

using namespace gtools::PrintTools;

bool debug = false;

struct Experiment{
    int foo;
    std::string boo;
    std::vector< Experiment > bars;

    Experiment();
    Experiment( int _foo, const std::string& _boo, 
                const std::initializer_list< Experiment >& _bars ) 
        : foo( _foo ), boo( _boo ), bars( _bars )
    {} 
    static void printConstruction( std::ostream&, const Experiment&, const ListOutputParams& );
};

void Experiment::printConstruction( std::ostream& os, const Experiment& item, 
                                    const ListOutputParams& props ){
    std::string expee = item.boo;
    gtools::StringTools::escapeSpecials( expee, true );

    os << "Experiment( "<< item.foo <<", \""<< expee <<"\", ";

    ListOutputParams ps = props;
    ps.tabLeaderSize += 4;

    // WheeCurshon!!
    outputInitializerList( os, item.bars.begin(), item.bars.end(), printConstruction, ps );
    
    os <<" )";
}

template<typename InputIterator, typename Callback>
void testInitializerList( InputIterator fr, InputIterator lst, 
        Callback clb, const std::string& result, 
        const ListOutputParams& pars = ListOutputParams() )
{
    std::stringstream sstr;
    outputInitializerList( sstr, fr, lst, clb, pars );

    std::string woopres( sstr.str() );

    if( debug ){
        std::cout << "[testInitializerList]: End result:\n\"" << woopres <<"\"\n";
        std::cout << " Asserting Size: ("<< result.size() <<" =? "<< woopres.size() <<")\n";

        for(size_t i = 0; i < result.size(); i++){
            if(result[i] != woopres[i]){
                std::cout<<" NonEqual Chars on [ "<<i<<" ]: \'"<<result[i]<<"\'";
                std::cout<<" != \'"<< woopres[i] <<"\' \n";
                break;
            }
        }
        std::cout<<"\n\n";
    }

    assert( result == woopres );
}

int main(int argc, char** argv){
    if( argc > 1 ){
        if( !strcmp( argv[1], "-v") || !strcmp( argv[1], "--debug") || 
            !strcmp( argv[1], "--verbose") )
            debug = true;
    }

    std::cout<<"[ Testing gtools::PrintTools  ] ... ";

    // Number 1
    std::vector< std::string > strs({"kawaii", "desu", "nyaa~~ :3"});

    testInitializerList( strs.begin(), strs.end(),
        [](auto& os, auto& item, auto& props){
            os << "[..::"<<item<<"::..]";
        }, 
        "{ \n"
        "    [..::kawaii::..], \n"
        "    [..::desu::..], \n"
        "    [..::nyaa~~ :3::..]\n"
        "}"
    );

    testInitializerList( strs.begin(), strs.end(),
        [](auto& os, auto& item, auto& props){
            os << "[..::"<<item<<"::..]";
        }, 
        "{ \n"
        "    [..::kawaii::..], [..::desu::..], [..::nyaa~~ :3::..]\n"
        "}",
        ListOutputParams( 4, 4 )
    ); 

    // Number 2

    std::vector< Experiment > exes({ 
        Experiment(1, "\n\fnyaa", {
            Experiment(2, "ww\t\xAB", { }),
            Experiment(5, "ab", {
                Experiment(12, "oo", {})
            })
        }),
        Experiment(45, "kk", {
            Experiment(30, "foo", { })
        } )
    } );
    testInitializerList( exes.begin(), exes.end(), Experiment::printConstruction,
        "{ \n"
        "    Experiment( 1, \"\\n\\fnyaa\", { \n"
        "        Experiment( 2, \"ww\\t\\xab\", { } ), \n"
        "        Experiment( 5, \"ab\", { \n"
        "            Experiment( 12, \"oo\", { } )\n"
        "        } )\n"
        "    } ), \n"
        "    Experiment( 45, \"kk\", { \n"
        "        Experiment( 30, \"foo\", { } )\n"
        "    } )\n"
        "}"
    );

    // End.

    std::cout<<"[ Passed! ]\n";

    return 0;
}

