#include "HitboxNode.hpp"

// straightup copied from maxnuts gdmegaoverlay

HitboxNode::HitboxNode()
{
}

HitboxNode* HitboxNode::create()
{
	auto ret = new HitboxNode();
	if (ret && ret->init())
	{
		// ret->autorelease();
		return ret;
	}
	CC_SAFE_RELEASE(ret);
	return nullptr;
}

HitboxNode* HitboxNode::getInstance()
{
	static HitboxNode* instance;
	if (!instance)
		instance = create();
	return instance;
}

void HitboxNode::drawPolygonHitbox(std::vector<cocos2d::CCPoint> const& points, cocos2d::ccColor4B col)
{
    col.a = (GLubyte)(255.f);
	auto color1 = ccc4FFromccc4B(col);
	auto colorA = (ccc4(col.r, col.g, col.b, (GLubyte)(0.f)));

	this->drawPolygon(const_cast<cocos2d::CCPoint*>(points.data()), points.size(), ccc4FFromccc4B(colorA),
					  0.5f, color1);
}

void HitboxNode::drawForObject(gd::GameObject* obj)
{
    if (obj->m_nObjectID != 749 && obj->getType() == gd::GameObjectType::kGameObjectTypeDecoration) return;

	ccColor4B color;
	color = {(GLubyte)(255.f), (GLubyte)(0.f), (GLubyte)(255.f)};
    switch (obj->getType())
    {
        case gd::GameObjectType::kGameObjectTypeSolid:
            this->drawPolygonHitbox(this->quadrilateralForObject(obj), color);
            return;
        case gd::GameObjectType::kGameObjectTypeSlope:
            this->drawPolygonHitbox(this->triangleForObject(obj), color);
            return;	
        case gd::GameObjectType::kGameObjectTypeHazard:
            color = {(GLubyte)(0.f), (GLubyte)(255.f), (GLubyte)(0.f)};
            
            this->drawPolygonHitbox(this->quadrilateralForObject(obj), color);
    	    return;

    }
return;
}

std::vector<CCPoint> HitboxNode::quadrilateralForObject(gd::GameObject* obj)
{
	constexpr size_t N = 4;
	std::vector<CCPoint> points(N);
	if (obj->m_pObjectOBB2D)
	{
		auto obb = obj->m_pObjectOBB2D;
		points[0] = obb->m_p1_1;
		points[1] = obb->m_p1_2;
		points[2] = obb->m_p1_3;
		points[3] = obb->m_p1_4;
	}
	else
	{
		auto rect = obj->getObjectRect();
		points[0] = CCPointMake(rect.getMinX(), rect.getMinY());
		points[1] = CCPointMake(rect.getMinX(), rect.getMaxY());
		points[2] = CCPointMake(rect.getMaxX(), rect.getMaxY());
		points[3] = CCPointMake(rect.getMaxX(), rect.getMinY());
	}
	return points;
}

std::vector<CCPoint> HitboxNode::triangleForObject(gd::GameObject* obj)
{
	constexpr size_t N = 3;
	std::vector<cocos2d::CCPoint> points(N);
	int i = 0;
	obj->updateOrientedBox();
	if (obj->m_pObjectOBB2D)
	{
		auto obb = obj->m_pObjectOBB2D;
		if (obj->m_bIsFlippedX || !obj->m_bIsFlippedY)
			points[i++] = obb->m_p1_1;
		if (!obj->m_bIsFlippedX || !obj->m_bIsFlippedY)
			points[i++] = obb->m_p1_2;
		if (!obj->m_bIsFlippedX || obj->m_bIsFlippedY)
			points[i++] = obb->m_p1_3;
		if (obj->m_bIsFlippedX || obj->m_bIsFlippedY)
			points[i++] = obb->m_p1_4;
	}
	else
	{
		auto rect = obj->getObjectRect();
		if (obj->m_bIsFlippedX || !obj->m_bIsFlippedY)
			points[i++] = CCPointMake(rect.getMinX(), rect.getMinY());
		if (!obj->m_bIsFlippedX || !obj->m_bIsFlippedY)
			points[i++] = CCPointMake(rect.getMinX(), rect.getMaxY());
		if (!obj->m_bIsFlippedX || obj->m_bIsFlippedY)
			points[i++] = CCPointMake(rect.getMaxX(), rect.getMaxY());
		if (obj->m_bIsFlippedX || obj->m_bIsFlippedY)
			points[i++] = CCPointMake(rect.getMaxX(), rect.getMinY());
	}
	return points;
}