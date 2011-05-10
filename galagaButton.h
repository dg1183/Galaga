#pragma once
#include "Sprite.h"

class galagaButton
{
public:
	galagaButton(HINSTANCE appInst, int defaultimg, int mouseoverimg, int mask, Vec2 pos);
	~galagaButton(void);

	void draw(HDC backbuffer, HDC spriteDC);

	bool mMouseover;
	Sprite* mButtonSprite;

protected:
	int mDefaultImageID;
	int mMouseoverImageID;
	int mMaskID;

};

