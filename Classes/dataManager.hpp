//
//  dataManager.hpp
//  CDianGame
//
//  Created by 余强 on 16/3/23.
//
//

#ifndef dataManager_hpp
#define dataManager_hpp

#include <stdio.h>
#include <map>

#define dataManager CDataManager::InstanceEx()
class CDataManager
{
public:
    static CDataManager* InstanceEx(){static CDataManager* ptr = nullptr; if(nullptr == ptr)ptr = new CDataManager(); return ptr;};
    
    void init();
    int getScoreWithCustomId(int mapid);
    
private:
    std::map<int, int> m_scoreList;
    bool m_bInit = false;
};

#endif /* dataManager_hpp */
