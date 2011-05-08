// Rect.cpp
// By Frank Luna
// August 24, 2004.

#include "Rect.h"

Rect::Rect()
{
}

Rect::Rect(const Vec2& a, const Vec2& b)
: minPt(a), maxPt(b)
{
}

Rect::Rect(float x0, float y0, float x1, float y1)
: minPt(x0, y0), maxPt(x1, y1)
{
}


bool Rect::forceInside(Circle& A)
{
	Vec2 p  = A.c;
	float r = A.r;

	// Modify coordinates to force inside.
	if(p.x - r < minPt.x)
		p.x = minPt.x + r + 1;
	if(p.x + r > maxPt.x)
		p.x = maxPt.x - r - 1;


	if(p.y - r < minPt.y)
		p.y = minPt.y + r + 1;
	if(p.y + r > maxPt.y)
		p.y = maxPt.y - r - 1;

	// Save forced position.
	if (A.c != p) //A is outside of rect
	{
		A.c = p;
		return true;
	}
	else
	{
		return false;
	}
}

bool Rect::isPtInside(const Vec2& pt)
{
	return pt.x >= minPt.x && pt.y >= minPt.y &&
		pt.x <= maxPt.x && pt.y <= maxPt.y;
}