#pragma once
#include "Sprite.h"
#include "Rect.h"
#include "Vec2.h"
#include <string>
using namespace std;

class Alien
{
public:
	enum Type {
		BLUE,
		GREEN,
		YELLOW
	};
	
	Alien(HINSTANCE appInst, Type type, Vec2 loc, Vec2 vel);
	~Alien(void);

	void update(float dt);
	void draw(HDC backBuffer, HDC spriteHDC);
	Sprite* mAlienSprite;
	
private:
	Rect mSpriteLocation;
};

