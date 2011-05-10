// Sprite.h
// By Frank Luna
// August 24, 2004.

#ifndef SPRITE_H
#define SPRITE_H

#include <windows.h>
#include "Circle.h"
#include "Vec2.h"
#include "Rect.h"

class Sprite
{
public:
	Sprite(HINSTANCE hAppInst, int imageID, int maskID, 
		const Circle& bc, const Vec2& p0, const Vec2& v0);

	~Sprite();

	int width();
	int height();

	void update(float dt);
	void draw(HDC hBackBufferDC, HDC hSpriteDC);
	void draw(HDC hBackBufferDC, HDC hSpriteDC, Rect sourceRect);
	void setNewImage(int newImageID);

public:
	// Make public
	Circle    mBoundingCircle;
	Vec2      mPosition; 
	Vec2      mVelocity;
	Rect	  mBoundingRect;

private:
	// Make copy constructor and assignment operator private
	// so client cannot copy Sprites.  We do this because
	// this class is not designed to be copied because it
	// is not efficient--copying bitmaps is slow (lots of memory).
	Sprite(const Sprite& rhs);
	Sprite& operator=(const Sprite& rhs);

protected:
	HINSTANCE mhAppInst;
	HBITMAP   mhImage;
	HBITMAP   mhMask;
	BITMAP    mImageBM;
	BITMAP    mMaskBM;
};

#endif // SPRITE_H