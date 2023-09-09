#include <vector>
#include "includes.h"
#include <iostream>
#include <deque>

class HitboxNode : public cocos2d::CCDrawNode
{
public:
	HitboxNode();
	static HitboxNode *create();

	static HitboxNode *getInstance();

	virtual void drawPolygonHitbox(std::vector<cocos2d::CCPoint> const &points, cocos2d::ccColor4B col);

	virtual void drawForObject(gd::GameObject *obj);

	virtual std::vector<cocos2d::CCPoint> triangleForObject(gd::GameObject *obj);
	virtual std::vector<cocos2d::CCPoint> quadrilateralForObject(gd::GameObject *obj);

	// these are fine tho
	std::deque<cocos2d::CCRect> boo1;
	std::deque<cocos2d::CCRect> boo2;
};