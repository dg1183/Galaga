#pragma once
#include "Sprite.h"
#include "Rect.h"
#include "Vec2.h"
#include <string>
using namespace std;

class Player
{
public:
	Player(HINSTANCE appInst, Vec2 loc, Vec2 vel);
	~Player(void);
	
	void update(float dt);
	void draw(HDC backBuffer, HDC spriteHDC);
	void kill();
	void score(int points);
	void resetLevelScore();
	void updateTotalScore();
	Sprite* mShipSprite;
	
private:
	Rect mSpriteLocation;
	int mScore;
	int mLevelScore;
	int mLives;
};

