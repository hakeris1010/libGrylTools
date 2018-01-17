#include <iostream>
#include <string>
#include <regex>

struct Token{
    size_t id;
    std::string data;
};

/*class Tokenizer{
private:
    char* buf

public:
void getNextToken_Priv( const char* buf, size_t len, Token& tok ){

}

void fetchToBuffer( std::istream& is, char* buf, size_t len ){

}

void getNextToken( std::istream& is, 

};*/

int main ()
{
    std::string reg = R"((\b[[:alpha:]]+\b)|(\b\d+\b)|(\s+)|(.))";
    std::regex r( reg );

    std::string str = "foo bar 123[]  \nqvf";
    
    std::cout << "Target sequence: "<< str << std::endl;
    std::cout << "Regular expression: "<< reg << std::endl;
     
    /*std::cout<< "\nMatching with std::sregex_iterator.\n";

    for(std::sregex_iterator i = std::sregex_iterator(s.begin(), s.end(), r);
                             i != std::sregex_iterator();
                             ++i)
    {
        std::smatch m = *i;
        std::cout << "\nMatch: " << m.str() << "\n Position " << m.position() << 
                     ", Length: "<< m.length() <<", SubMatches: "<< m.size() <<"\n";

        for(auto index = 1; index < m.size(); ++index ){
            if (!m[index].str().empty()) {
                std::cout << " Capture group ID: " << index-1 << "\n";
                break;
            }
        }
    } */ 

    std::cout<< "\n\nMatching with std::regex_search.\n";
    const char* bufBeg = str.c_str();
    const char* bufEnd = bufBeg + str.size();
    std::cmatch m;

    while (std::regex_search (bufBeg, bufEnd, m, r)) {
        std::cout<<"Match value: "<< m.str() <<", Matches: "<< m.size() <<"\n";

        // Start from 1st submatch, because 0th is the whole regex.
        for (size_t i = 1; i < m.size(); i++)
            std::cout<<" "<< i <<": "<< m[i] <<"\n";

        std::cout << "\n";
        bufBeg += m.position() + m.length();
    } 

    return 0;
}


