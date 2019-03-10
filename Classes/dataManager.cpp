//
//  dataManager.cpp
//  CDianGame
//
//  Created by 余强 on 16/3/23.
//
//

#include "dataManager.hpp"
#include "com/csvParse.hpp"

void CDataManager::init()
{
    if (m_bInit)
        return ;
    
    m_bInit = true;
    
    std::vector<std::map<std::string, std::string>> scoreList;
    csvParse->parse("obstacle", scoreList);
    
    for(auto itr = scoreList.begin(); itr != scoreList.end(); ++itr)
    {
        int k = atoi( (*itr)["mapId"].c_str() );
        int v = atoi( (*itr)["score"].c_str() );
        m_scoreList[k] = v;
    }
    
}


int CDataManager::getScoreWithCustomId(int mapid)
{
    
    std::map< int, int >::iterator iter = m_scoreList.find(mapid);
    if( iter != m_scoreList.end())
    {
        return m_scoreList[mapid];
    }
    return 0;
}








