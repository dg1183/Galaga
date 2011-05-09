// GalagaGame.h
// By Frank Luna
// August 24, 2004.

#ifndef GALAGA_GAME_H
#define GALAGA_GAME_H

#include <windows.h>
#include "Sprite.h"
#include "Rect.h"
#include "Circle.h"
#include "Alien.h"
#include "Projectile.h"
#include <list>
using namespace std;

bool rightToLeft(Alien* alien1, Alien* alien2);
bool leftToRight(Alien* alien1, Alien* alien2);

class GalagaGame
{
public:
	GalagaGame(HINSTANCE hAppInst, HWND hMainWnd, 
		Vec2 wndCenterPt);
	~GalagaGame();

	void pause();
	void unpause();
	bool isPaused();
	void addProjectile(Projectile::Owner owner);

	void update(float dt);
	void draw(HDC hBackBufferDC, HDC hSpriteDC);

private:
	void updateShip(float dt);
	void checkAlienKills();
	bool projectileAlienCollision(Alien* alien, Projectile* projectile);
	void updateAliens(float dt);
	void updateAlien(Alien* alien, float dt);
	void updateProjectiles(float dt);
	void loadLevel(int level);
	void checkLevelDone();
	//void increaseScore(bool blue);

private:
	HINSTANCE mhAppInst;
	HWND      mhMainWnd;
	Vec2      mWndCenterPt;

	int mScore;
	int mLevel;

	bool mPaused;
	bool mLevelDone;

	const float MAX_SHIP_SPEED;
	const float PROJECTILE_SPEED;

	float mGunCooldown;

	Sprite* mGameBoard;
	Sprite* mSpaceShip;
	list<Alien*> mAliens;
	list<Projectile*> mProjectiles;

	POINT mLastMousePos;
	POINT mCurrMousePos;

	Rect mShipBounds; 
	Rect mBoardBounds;
	Rect mAlienBounds;

};

#endif // GALAGA_GAME_H