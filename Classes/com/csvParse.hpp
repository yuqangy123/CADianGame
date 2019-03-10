//
//  csvParse.hpp
//  CDianGame
//
//  Created by 余强 on 16/3/23.
//
//

#ifndef csvParse_hpp
#define csvParse_hpp

#include "cocos2d.h"
#include <stdio.h>
#include <vector>
#include <string>

#endif /* csvParse_hpp */

typedef struct {
    std::string key;
    std::string value;
}csvData;

#define csvParse CCsvParse::InstanceEx()
class CCsvParse
{
public:
    static CCsvParse* InstanceEx(){static CCsvParse* ptr = nullptr; if(nullptr == ptr)ptr = new CCsvParse(); return ptr;};
    
    bool parse(const char* file, std::vector<std::map<std::string, std::string>>& db);
    
};