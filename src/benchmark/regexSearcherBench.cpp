#include <iostream>
#include <string>
#include <regex>

int main ()
{
    std::string reg = R"((\b[[:alpha:]]+\b)|(\b\d+\b))";
    std::regex r( reg );
    std::string str = "foo bar 123";
    
    std::cout << "Target sequence: "<< str << std::endl;
    std::cout << "Regular expression: "<< reg << std::endl;
     
    std::cout<< "\nMatching with std::sregex_iterator.\n";
    std::string s = str;
    for(std::sregex_iterator i = std::sregex_iterator(s.begin(), s.end(), r);
                             i != std::sregex_iterator();
                             ++i)
    {
        std::smatch m = *i;
        std::cout << "\nMatch value: " << m.str() << " at Position " << m.position() << 
                     ". Matches: "<< m.size() <<"\n";

        for(auto index = 1; index < m.size(); ++index ){
            if (!m[index].str().empty()) {
                std::cout << "Capture group ID: " << index-1 << std::endl;
            }
        }
    }  

    std::cout<< "\n\nMatching with std::regex_search.\n";
    s = str;
    std::smatch m;
    while (std::regex_search (s, m, r)) {
        std::cout<<"Match value: "<< m.str() <<", Matches: "<< m.size() <<"\n";

        // Start from 1st submatch, because 0th is the whole regex.
        for (size_t i = 1; i < m.size(); i++)
            std::cout<<" "<< i <<": "<< m[i] <<"\n";

        std::cout << "\n";
        s = m.suffix().str();
    } 

    return 0;
}


