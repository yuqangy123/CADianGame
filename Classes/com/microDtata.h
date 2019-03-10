//
//  microDtata.h
//  CDianGame
//
//  Created by 余强 on 16/3/22.
//
//

#ifndef microDtata_h
#define microDtata_h

#include <stdio.h>

#define PTM_RATIO 32


enum{
    CATEGORY_ENEMY = 0x01<<0,
    CATEGORY_GROUND = 0x01<<1,
    CATEGORY_ACTOR = 0x01<<2,
    CATEGORY_ITEM = 0x01<<3
};

typedef enum
{
    none = 00,
    lightFilterLine = 10,
    lightFilterBall = 20,
    light = 30
}BodyType;

typedef enum
{
    ballContact = 00,
    screenBottom = 10
}lightDead;

struct BodyUserData
{
    BodyType			type = BodyType::light;
    int					tag = 0;
    void*               target;
    
    static BodyUserData* create(void* target, BodyType tp = BodyType::none, int tag=0)
    {
        BodyUserData* pData = new BodyUserData;
        pData->target = target;
        pData->type = tp;
        pData->tag = tag;
        return pData;
    };
};


///////////////////////////////////////////////////////////////

const float stage_width = 720.0;
const float stage_height = 1280.0;
const float dianliu_jumpH = 550.0f;

//const char* display_autoscale = "FIXED_WIDTH";



#endif /* microDtata_h */
