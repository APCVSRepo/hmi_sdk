#include "HMISDK/include/utils/stringutil.h"
#include <iostream>
#include <string>
#include <vector>


std::string& trim(std::string &s)
{
    if (s.empty())
    {
        return s;
    }

    s.erase(0,s.find_first_not_of(" "));
    s.erase(s.find_last_not_of(" ") + 1);
    return s;
}

void split(const std::string& src, const std::string& separator, std::vector<std::string>& dest)
{
    std::string str = src;
    std::string substring;
    std::string::size_type start = 0, index;
    do
    {
        index = str.find_first_of(separator,start);
        if (index != std::string::npos)
        {
            substring = str.substr(start,index-start);
            dest.push_back(substring);
            start = str.find_first_not_of(separator,index);
            if (start == std::string::npos)
            {
                return;
            }
        }
    }while(index != std::string::npos);

    //the last token
    substring = str.substr(start);
    dest.push_back(substring);
}
