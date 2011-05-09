#include "Alien.h"
#include "Circle.h"
#include <string>
#include "resource.h"


Alien::Alien(HINSTANCE appInst, Type type, Vec2 loc, Vec2 vel)
{
	Circle bc;
	bc.c = loc;
	bc.r = 10;
	
	switch(type)
	{
	case BLUE: 
		mSpriteLocation.minPt.x = 0;
		mSpriteLocation.minPt.y = 0;
		mSpriteLocation.maxPt.x = 25;
		mSpriteLocation.maxPt.y = 21;
		mAlienSprite = new Sprite(appInst, IDB_ALIENS, IDB_ALIENS_MASK, bc, loc, vel);
		break;
	}

}

Alien::~Alien(void)
{
	delete mAlienSprite;
}

void Alien::update(float dt)
{
	//mAlienSprite->mVelocity.x = 25.0f;
	mAlienSprite->update(dt);
}

void Alien::draw(HDC backbuffer, HDC spriteDC)
{
	mAlienSprite->draw(backbuffer, spriteDC, mSpriteLocation);
}



