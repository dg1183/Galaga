#include "galagaButton.h"
#include "Vec2.h"
#include "Circle.h"


galagaButton::galagaButton(HINSTANCE appInst, int defaultimg, int mouseoverimg, int mask, Vec2 pos)
{
	Circle bc;
	bc.c = pos;
	bc.r = 0;
	
	mDefaultImageID = defaultimg;
	mMouseoverImageID = mouseoverimg;
	mMaskID = mask;

	mMouseover = false;

	mButtonSprite = new Sprite(appInst, mDefaultImageID, mMaskID, bc, pos, Vec2(0, 0));
}


galagaButton::~galagaButton(void)
{
	delete mButtonSprite;
}

void galagaButton::draw(HDC backbuffer, HDC spriteDC)
{
	if(mMouseover)
	{
		mButtonSprite->setNewImage(mMouseoverImageID);
	}
	else
	{
		mButtonSprite->setNewImage(mDefaultImageID);
	}

	mButtonSprite->draw(backbuffer, spriteDC);
}