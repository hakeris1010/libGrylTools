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
        for (auto&& x : m) 
            std::cout << x << " ";
        std::cout << std::endl;
        s = m.suffix().str();
    }

    return 0;
}


