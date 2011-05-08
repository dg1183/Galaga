// Rect.h
// By Frank Luna
// August 24, 2004.

#ifndef RECT_H
#define RECT_H

#include "Circle.h"

class Rect
{
public:
	Rect();
	Rect(const Vec2& a, const Vec2& b);
	Rect(float x0, float y0, float x1, float y1);

	bool forceInside(Circle& A);
	bool isPtInside(const Vec2& pt);

	Vec2 minPt;
	Vec2 maxPt;
};

#endif // RECT_H