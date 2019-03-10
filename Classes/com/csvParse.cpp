//
//  csvParse.cpp
//  CDianGame
//
//  Created by 余强 on 16/3/23.
//
//

#include "csvParse.hpp"
using namespace std;

bool CCsvParse::parse(const char* file, std::vector<std::map<std::string, std::string>>& db)
{
    string fileName = "static_data/";
    fileName += file;
    fileName += ".csv";
    
    string sourcePath = cocos2d::FileUtils::getInstance()->getStringFromFile(fileName);
    
    size_t ndx = sourcePath.find('\n');
    if (ndx == string::npos)
        return false;
    
    std::vector<std::string> hand;
    string subStr = sourcePath.substr(0, ndx-1);
    size_t ndxD = subStr.find(",");
    
    while(ndxD != string::npos)
    {
        std::string subone = subStr.substr(0, ndxD);
        subStr = subStr.substr(ndxD+1, subStr.length());
        hand.push_back(subone);
        ndxD = subStr.find(",");
    }
    sourcePath = sourcePath.substr(ndx+1, sourcePath.length());
    if (sourcePath.empty())
        return false;
    
    ndx = sourcePath.find('\n');
    while (ndx != std::string::npos)
    {
        subStr = sourcePath.substr(0, ndx-1);
        ndxD = subStr.find(",");
        std::map<std::string, std::string> data;
        int n = 0;
        
        while(ndxD != string::npos)
        {
            string subone = subStr.substr(0, ndxD);
            if (subone.empty())
                break;
            subStr = subStr.substr(ndxD+1, subStr.length());
            ndxD = subStr.find(",");
            data[hand[n++]] = subone;
        }
        db.push_back(data);
        
        sourcePath = sourcePath.substr(ndx+1, sourcePath.length());
        if(sourcePath.empty())
            break;
        ndx = sourcePath.find('\n');
    }
    
    return true;
}




