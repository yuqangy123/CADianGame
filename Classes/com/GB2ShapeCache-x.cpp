//
//  GB2ShapeCache-x.cpp
//  
//  Loads physics sprites created with http://www.PhysicsEditor.de
//  To be used with cocos2d-x
//
//  Generic Shape Cache for box2d
//
//  Created by Thomas Broquist
//
//      http://www.PhysicsEditor.de
//      http://texturepacker.com
//      http://www.code-and-web.de
//  
//  All rights reserved.
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//  
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//  
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//  THE SOFTWARE.
//

#include "GB2ShapeCache-x.h"
#include "Box2D/Box2D.h"
//#include "cocoa/CCNS.h"

using namespace cocos2d;

/**
 * Internal class to hold the fixtures
 */
class FixtureDef {
public:
    FixtureDef()
    : next(NULL) {}
    
    ~FixtureDef() {
        delete next;
        delete fixture.shape;
    }
    
    FixtureDef *next;
    b2FixtureDef fixture;
    int callbackData;
};

class BodyDef {
public:
	BodyDef()
	: fixtures(NULL) {}
	
	~BodyDef() {
		if (fixtures)
			delete fixtures;
	}
	
	FixtureDef *fixtures;
	CCPoint anchorPoint;
};

static GB2ShapeCache *_sharedGB2ShapeCache = NULL;

GB2ShapeCache* GB2ShapeCache::sharedGB2ShapeCache(void) {
	if (!_sharedGB2ShapeCache) {
		_sharedGB2ShapeCache = new GB2ShapeCache();
        _sharedGB2ShapeCache->init();
	}
	
	return _sharedGB2ShapeCache;
}

bool GB2ShapeCache::init() {
	return true;
}

void GB2ShapeCache::reset() {
	std::map<std::string, shapeBodyInfo >::iterator iter;
	for (iter = shapeObjects.begin() ; iter != shapeObjects.end() ; ++iter) {
        delete iter->second.pBodyDef;
        iter->second.pBodyDef = NULL;
        
        iter->second.pRoadPath->release();
        iter->second.pRoadPath = NULL;
	}
	shapeObjects.clear();
}

PointArray* GB2ShapeCache::addFixturesToBody(b2Body *body, const std::string &shape, float density, float friction, float frestitution)
{
	std::map<std::string, shapeBodyInfo>::iterator pos = shapeObjects.find(shape);
	//assert(pos != shapeObjects.end());
	if(pos == shapeObjects.end())return nullptr;

	BodyDef *so = (*pos).second.pBodyDef;

	//change forever, no copy
	FixtureDef *fix = so->fixtures;
	while (fix) {
		fix->fixture.density = density;
		fix->fixture.friction = friction;
		fix->fixture.restitution = frestitution;
		body->CreateFixture(&fix->fixture);
		fix = fix->next;
	}
    
    return (*pos).second.pRoadPath;
}

void GB2ShapeCache::addFixturesToBody(b2Body *body, const std::string &shape) {
	std::map<std::string, shapeBodyInfo>::iterator pos = shapeObjects.find(shape);
	if(pos == shapeObjects.end())return;
	
	BodyDef *so = (*pos).second.pBodyDef;

	FixtureDef *fix = so->fixtures;
    while (fix) {
        body->CreateFixture(&fix->fixture);
        fix = fix->next;
    }
}

cocos2d::CCPoint GB2ShapeCache::anchorPointForShape(const std::string &shape) {
	std::map<std::string, shapeBodyInfo>::iterator pos = shapeObjects.find(shape);
	//assert(pos != shapeObjects.end());
	if(pos == shapeObjects.end())return CCPoint(0.0, 0.0);
	
	BodyDef *bd = (*pos).second.pBodyDef;
	return bd->anchorPoint;
}

typedef CCDictionary ObjectDict;

void GB2ShapeCache::addShapesWithFile(const std::string &plist) {
    std::string fullName = cocos2d::FileUtils::getInstance()->fullPathForFilename(plist);
    ObjectDict *dict = CCDictionary::createWithContentsOfFile(fullName.c_str());
	CCAssert(dict != NULL, "Shape-file not found"); // not triggered - cocos2dx delivers empty dict if non was found
    CCAssert(dict->count() != 0, "plist file empty or not existing");
	
	ObjectDict *metadataDict = (ObjectDict *)dict->objectForKey("metadata");
    int format = static_cast<CCString *>(metadataDict->objectForKey("format"))->intValue();
    ptmRatio = static_cast<CCString *>(metadataDict->objectForKey("ptm_ratio"))->floatValue();
	CCAssert(format == 1, "Format not supported");

	ObjectDict *bodyDict = (ObjectDict *)dict->objectForKey("bodies");

    b2Vec2 vertices[b2_maxPolygonVertices];
		
	//bodyDict->begin();
	std::string bodyName;
	ObjectDict *bodyData = ObjectDict::create();
    
	//while ((bodyData = (ObjectDict *)bodyDict->next(&bodyName))) {
    
    
	DictElement* _elmt;
	CCDICT_FOREACH(bodyDict, _elmt){
		bodyName = _elmt->getStrKey();
		if( shapeObjects.end() != shapeObjects.find(bodyName) )
			continue;

        std::map<float, float>  roadNode;//高度－X值
		BodyDef *bodyDef = new BodyDef();
		bodyData = (ObjectDict *)_elmt->getObject();
		
		bodyDef->anchorPoint = PointFromString(static_cast<CCString *>(bodyData->objectForKey("anchorpoint"))->getCString());
		
		CCArray *fixtureList = (CCArray *)(bodyData->objectForKey("fixtures"));
        FixtureDef **nextFixtureDef = &(bodyDef->fixtures);

		//CCMutableArray<ObjectDict *>::CCMutableArrayIterator iter;
		CCObject* _object;
		CCARRAY_FOREACH(fixtureList, _object){
		//for (iter = fixtureList->begin(); iter != fixtureList->end(); ++iter) {
            b2FixtureDef basicData;
            bodyData = (ObjectDict*)_object;
			//fixtureData.allKeysForObject(_object);

            basicData.filter.categoryBits = static_cast<CCString *>(bodyData->objectForKey("filter_categoryBits"))->intValue();
            basicData.filter.maskBits = static_cast<CCString *>(bodyData->objectForKey("filter_maskBits"))->intValue();
            basicData.filter.groupIndex = static_cast<CCString *>(bodyData->objectForKey("filter_groupIndex"))->intValue();
            basicData.friction = static_cast<CCString *>(bodyData->objectForKey("friction"))->floatValue();
            basicData.density = static_cast<CCString *>(bodyData->objectForKey("density"))->floatValue();
            basicData.restitution = static_cast<CCString *>(bodyData->objectForKey("restitution"))->floatValue();
            //basicData.isSensor = (bool)static_cast<CCString *>(bodyData->objectForKey("isSensor"))->boolValue();
            basicData.isSensor = true;
            
			CCString *cb = static_cast<CCString *>(bodyData->objectForKey("userdataCbValue"));
			
            int callbackData = 0;
			
			if (cb)
				callbackData = cb->intValue();
            
			std::string fixtureType = static_cast<CCString *>(bodyData->objectForKey("fixture_type"))->getCString();
			
			if (fixtureType == "POLYGON") {
				CCArray *polygonsArray = (CCArray *)(bodyData->objectForKey("polygons"));
				
				//CCMutableArray<ObjectDict *>::CCMutableArrayIterator iter;
				CCObject* iter;
				CCARRAY_FOREACH(polygonsArray, iter){
				//for (iter = polygonsArray->begin(); iter != polygonsArray->end(); ++iter) {
                    FixtureDef *fix = new FixtureDef();
                    fix->fixture = basicData; // copy basic data
                    fix->callbackData = callbackData;
					
                    b2PolygonShape *polyshape = new b2PolygonShape();
                    int vindex = 0;
                    
					CCArray *polygonArray = (CCArray*)(iter);

                    assert(polygonArray->count() <= b2_maxPolygonVertices);

					CCObject* piter;
                    
					CCARRAY_FOREACH(polygonArray, piter){
// 					CCMutableArray<CCString *>::CCMutableArrayIterator piter;
// 					for (piter = polygonArray->begin(); piter != polygonArray->end(); ++piter) {

						CCString *str = (CCString*)piter;
						if(str)
						{
							CCPoint offset = PointFromString(str->getCString());
							vertices[vindex].x = (offset.x / ptmRatio) ; 
							vertices[vindex].y = (offset.y / ptmRatio) ;
                            
                            //评估路径节点
                            float derr = 10;
                            bool added = false;
                            for (auto itr = roadNode.begin(); itr != roadNode.end(); ++itr)
                            {
                                float h = itr->first;
                                if( (h+derr) >= vertices[vindex].y && (h-derr) <= vertices[vindex].y )
                                {
                                    added = true;
                                    if (itr->second > vertices[vindex].x)
                                    {
                                        float key = itr->first;
                                        roadNode.erase(itr);
                                        roadNode[key] = vertices[vindex].x;
                                    }
                                    break;
                                }
                                
                            }
                            if(!added)
                            {
                                roadNode[vertices[vindex].y] = vertices[vindex].x;
                            }
                            
							vindex++;
						}
                    }
                    
                    polyshape->Set(vertices, vindex);
                    fix->fixture.shape = polyshape;
                    
                    // create a list
                    *nextFixtureDef = fix;
                    nextFixtureDef = &(fix->next);
				}
				
			} else if (fixtureType == "CIRCLE") {
				FixtureDef *fix = new FixtureDef();
                fix->fixture = basicData; // copy basic data
                fix->callbackData = callbackData;
                
                ObjectDict *circleData = (ObjectDict *)bodyData->objectForKey("circle");
                
                b2CircleShape *circleShape = new b2CircleShape();
				
                circleShape->m_radius = static_cast<CCString *>(circleData->objectForKey("radius"))->floatValue() / ptmRatio;
				CCPoint p = PointFromString(static_cast<CCString *>(circleData->objectForKey("position"))->getCString());
                circleShape->m_p = b2Vec2(p.x / ptmRatio, p.y / ptmRatio);
                fix->fixture.shape = circleShape;
				
                // create a list
                *nextFixtureDef = fix;
                nextFixtureDef = &(fix->next);

			} else {
				CCAssert(0, "Unknown fixtureType");
			}
            
            PointArray* pRoadPathArray = CCPointArray::create(roadNode.size());
            for (auto itr = roadNode.begin(); itr != roadNode.end(); ++itr)
            {
                pRoadPathArray->addControlPoint(ccp(itr->second, itr->first));
            }
			
			// add the body element to the hash
            shapeObjects[bodyName].pRoadPath = pRoadPathArray;
			shapeObjects[bodyName].pBodyDef = bodyDef;
		}
	}
}