#include "Player.h"
#include "Circle.h"
#include <string>
#include "resource.h"


Player::Player(HINSTANCE appInst, Vec2 loc, Vec2 vel)
{
	Circle bc;
	bc.c = loc;
	bc.r = 10;
	
	mShipSprite = new Sprite(appInst, IDB_SHIP, IDB_SHIP_MASK, bc, loc, vel);
	mScore = 0;
	mLevelScore = 0;
	mLives = 3;
}

Player::~Player(void)
{
	delete mShipSprite;
}

void Player::update(float dt)
{
	//mAlienSprite->mVelocity.x = 25.0f;
	mShipSprite->update(dt);
}

void Player::draw(HDC backbuffer, HDC spriteDC)
{
	COLORREF oldColor;
	
	mShipSprite->draw(backbuffer, spriteDC);

	char score[32];
	sprintf(score, "Score = %d", mScore + mLevelScore);

	char lives[32];
	sprintf(lives, "Lives = %d", mLives);

	SetBkMode(backbuffer, TRANSPARENT);
	oldColor = SetTextColor(backbuffer, RGB(255,255,255));
	TextOut(backbuffer, 15, 775, score, (int)strlen(score));
	TextOut(backbuffer, 325, 775, lives, (int)strlen(lives));

	SetTextColor(backbuffer, oldColor);
}

void Player::kill()
{
	mLives -= 1;
}

void Player::score(int points)
{
	mLevelScore += points;
}

void Player::resetLevelScore()
{
	mLevelScore = 0;
}

void Player::updateTotalScore()
{
	mScore += mLevelScore;
	mLevelScore = 0;
}