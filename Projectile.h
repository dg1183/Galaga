#pragma once
#include "Sprite.h"
#include "Vec2.h"
#include "Circle.h"
#include "Rect.h"



class Projectile
{
public:	
	Sprite* mProjectileSprite;

	Projectile(HINSTANCE appInst, bool playerOwned, Vec2 loc, Vec2 vel);
	~Projectile(void);

	void update(float dt);
	void draw(HDC backbuffer, HDC spriteDC);

private:
	bool mPlayerOwned;
	
};

