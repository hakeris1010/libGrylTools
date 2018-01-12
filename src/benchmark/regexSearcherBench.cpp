#include <iostream>
#include <string>
#include <regex>

int main ()
{
    std::string s = "noot 98 boot 69";
    std::string reg ("(\\b\\w+\\b)|(\\b\\d+\\b)");

    std::smatch m;
    std::regex e (reg);

    std::cout << "Target sequence: "<< s << std::endl;
    std::cout << "Regular expression: "<< reg << std::endl;
    std::cout << "The following matches and submatches were found:"<< std::endl;

    //const char* it = s;
    //const char* end = s + sizeof(s);

    while (std::regex_search (s, m, e)) {
        std::cout<<"Size: "<< m.size() <<"\n";

        // Start from 1st submatch, because 0th is the whole regex.
        for (size_t i = 1; i < m.size(); i++) 
            std::cout<<" "<< i <<": "<< m[i] <<"\n";

        std::cout << "\n";
        s = m.suffix().str();
    }

    return 0;
}


