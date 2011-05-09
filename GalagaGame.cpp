// GalagaGame.cpp
// By Frank Luna
// August 24, 2004.
 
#include "GalagaGame.h"
#include "windows.h"
#include <cstdio>
#include "resource.h" // For Bitmap resource IDs
#include "Alien.h"

using namespace std;



GalagaGame::GalagaGame(HINSTANCE hAppInst, 
							 HWND hMainWnd, 
		                     Vec2 wndCenterPt)
 : MAX_SHIP_SPEED(100.0f), PROJECTILE_SPEED(200.0f)
{
	// Save input parameters.
	mhAppInst    = hAppInst;
	mhMainWnd    = hMainWnd;
	mWndCenterPt = wndCenterPt;
	mLevel = 0;

	// Players start game with score of zero.
	mScore = 0;

	// The game is initially paused.
	mPaused    = true;

	mLevelDone = true;

	// No recovery time for the ships gun.
	mGunCooldown = 0.0f;

	// Create the sprites:

	Circle bc;
	Vec2   p0 = wndCenterPt;
	Vec2   v0(0.0f, 0.0f);

	mGameBoard = new Sprite(mhAppInst, IDB_GAMEBOARD, 
		IDB_GAMEBOARD_MASK,	bc, p0, v0);

	p0.y = 700;
	bc.c = p0;
	bc.r = 18.0f; // Ship radius = 18
	mSpaceShip = new Sprite(mhAppInst, IDB_SHIP,
		IDB_SHIP_MASK, bc, p0, v0);


	// Initialize the rectangles.
	mShipBounds  = Rect(0, 600, 400, 800); 
	mAlienBounds = Rect(0, 0, 400, 600);
	mBoardBounds = Rect(0, 0, 400, 800);
	//mBlueGoal    = Rect(0, 146, 30, 354);
	//mRedGoal     = Rect(833, 146, 863, 354);
}

GalagaGame::~GalagaGame()
{
	delete mGameBoard;
	delete mSpaceShip;
	while(!mAliens.empty())
		{
			delete mAliens.front();
			mAliens.pop_front();
		}
	
	while(!mProjectiles.empty())
	{
		delete mProjectiles.front();
		mProjectiles.pop_front();
	}
}

void GalagaGame::pause()
{
	mPaused = true;

	// Game is unpaused--release capture on mouse.
	ReleaseCapture();

	// Show the mouse cursor when paused.
	ShowCursor(true); 
}

void GalagaGame::unpause()
{
	// Fix cursor to paddle position.
	POINT p = mSpaceShip->mPosition;
	ClientToScreen(mhMainWnd, &p);

	SetCursorPos(p.x, p.y);
	GetCursorPos(&mLastMousePos);

	mPaused = false;

	// Capture the mouse when not paused.
	SetCapture(mhMainWnd);

	// Hide the mouse cursor when not paused.
	ShowCursor(false); 
}

void GalagaGame::update(float dt)
{
	
	if (mLevelDone)
	{
		while(!mProjectiles.empty())
		{
			delete mProjectiles.front();
			mProjectiles.pop_front();
		}
		pause();
		mLevelDone = false;
		mLevel++;
		loadLevel(mLevel);
	}
	// Only update the game if the game is not paused.
	if( !mPaused )
	{
		
		updateShip(dt);
		if(!mAliens.empty())
			updateAliens(dt);
		if(!mProjectiles.empty())
			updateProjectiles(dt);
		checkLevelDone();

		// Decrease recovery time as time passes.
		if( mGunCooldown > 0.0f )
			mGunCooldown -= dt;
	}
}

void GalagaGame::draw(HDC hBackBufferDC, HDC hSpriteDC)
{
	// Draw the sprites.
	mGameBoard->draw(hBackBufferDC, hSpriteDC);
	
	char buffer[256];
	sprintf(buffer, "IsEmpty: %s", (mProjectiles.empty())?"True":"False");
	SetBkMode(hBackBufferDC, TRANSPARENT);
	TextOut(hBackBufferDC, 20, 10, buffer, (int)strlen(buffer));
	
	if (!mProjectiles.empty())
	{
		list<Projectile*>::iterator ip = mProjectiles.begin();
		while (ip != mProjectiles.end() || mProjectiles.empty())
		{
			(*ip)->draw(hBackBufferDC, hSpriteDC);
			ip++;
		}
	}

	if (!mAliens.empty())
	{
		list<Alien*>::iterator i = mAliens.begin();
		while(i != mAliens.end() || mAliens.empty())
		{
			(*i)->draw(hBackBufferDC, hSpriteDC);
			i++;
		}
	}

	mSpaceShip->draw(hBackBufferDC, hSpriteDC);



	// Draw the player scores.
	char score[32];
	sprintf(score, "Score = %d", mScore);

	SetBkMode(hBackBufferDC, TRANSPARENT);
	TextOut(hBackBufferDC, 15, 45, score, (int)strlen(score));

}

void GalagaGame::updateShip(float dt)
{
	GetCursorPos(&mCurrMousePos);

	// Change in mouse position.
	int dx = mCurrMousePos.x - mLastMousePos.x;
	int dy = mCurrMousePos.y - mLastMousePos.y;

	Vec2 dp((float)dx, (float)dy);

	// Velocity is change in position with respect to time.
	mSpaceShip->mVelocity = dp / dt;

	// Update the blue paddle's position.
	mSpaceShip->update(dt);

	// Make sure the blue paddle stays inbounds.
	mShipBounds.forceInside(mSpaceShip->mBoundingCircle);
	mSpaceShip->mPosition = mSpaceShip->mBoundingCircle.c;

	// The current position is now the last mouse position.
	mLastMousePos = mSpaceShip->mPosition;

	// Keep mouse cursor fixed to paddle.
	ClientToScreen(mhMainWnd, &mLastMousePos);
	SetCursorPos(mLastMousePos.x, mLastMousePos.y);
}

void GalagaGame::checkAlienKills()
{
	list<Projectile*>::iterator i = mProjectiles.begin();
	while(i != mProjectiles.end() && !mProjectiles.empty())
	{
		if(!mAliens.empty())
		{
			list<Alien*>::iterator ia = mAliens.begin();
			while(ia != mAliens.end())
			{
				if (projectileAlienCollision((*ia), (*i)))
				{
					delete (*ia);
					ia = mAliens.erase(ia);

					delete (*i);
					i = mProjectiles.erase(i);
				}
				else
				{
					if(ia != mAliens.end() && !mAliens.empty())
						ia++;
				}
			}
		}

		if(i != mProjectiles.end())
			i++;

	}
}

bool GalagaGame::isPaused()
{
	return mPaused;
}

void GalagaGame::addProjectile(Projectile::Owner owner)
{
	Vec2 pos;
	Vec2 vel;
	
	if (owner == Projectile::PLAYER)
	{
		if (mGunCooldown <= 0.0f)
		{
			pos = mSpaceShip->mPosition;
			vel.x = 0.0f;
			vel.y = -150.0f;

			Projectile* projectile = new Projectile(mhAppInst, owner, pos, vel);
			mProjectiles.push_back(projectile);
			mGunCooldown = 0.1f;
		}
	}
}

bool GalagaGame::projectileAlienCollision(Alien* alien, Projectile* projectile)
{
	Vec2 normal;
	if(projectile->mProjectileSprite->mBoundingCircle.hits(alien->mAlienSprite->mBoundingCircle, normal))
	{
		return true;
	}
	return false;
}

void GalagaGame::updateProjectiles(float dt)
{
	list<Projectile*>::iterator i = mProjectiles.begin();
	for(i; i != mProjectiles.end() || mProjectiles.empty(); i++)
	{
		(*i)->update(dt);

		if(!mAliens.empty())
		{
			list<Alien*>::iterator ia = mAliens.begin();
			for(ia; ia != mAliens.end() || mAliens.empty(); ia++)
			{
				if(projectileAlienCollision((*ia), (*i)))
				{
					if ((*i) == mProjectiles.back())
					{
						
						delete (*ia);
						ia = mAliens.erase(ia);

						delete (*i);
						mProjectiles.pop_back();
						i = mProjectiles.end();

						break;
					}
					else
					{
						delete (*ia);
						ia = mAliens.erase(ia);
						if (mAliens.size() == 0)
							mLevelDone = true;

						delete (*i);
						i = mProjectiles.erase(i);

						break;
					}
				}
			}
		}
		
		if(i != mProjectiles.end())
		{
			if (!mBoardBounds.isPtInside((*i)->mProjectileSprite->mPosition))
				{
					delete (*i);
					i = mProjectiles.erase(i);

					if(i == mProjectiles.end())
						break;
				}
		}
		else
		{
			break;
		}
	}
}

void GalagaGame::updateAlien(Alien* alien, float dt)
{
	alien->mAlienSprite->update(dt);
}		

void GalagaGame::updateAliens(float dt)
{
	list<Alien*>::iterator i = mAliens.begin();
	while(i != mAliens.end())
	{
		(*i)->update(dt);
		i++;
	}

	if(mAliens.front()->mAlienSprite->mVelocity.x > 0)
		mAliens.sort(rightToLeft);
	else
		mAliens.sort(leftToRight);
	
	if(mAlienBounds.forceInside(mAliens.front()->mAlienSprite->mBoundingCircle))
	{
		//Reached the edge for the board, drop down and reflect for all aliens
		list<Alien*>::iterator iter = mAliens.begin();
		while (iter != mAliens.end())
		{
			mAlienBounds.forceInside((*iter)->mAlienSprite->mBoundingCircle);
			(*iter)->mAlienSprite->mVelocity.x *= -1.0f;
			(*iter)->mAlienSprite->mPosition.y += 10.0f;
			iter++;
		}
	}
}

void GalagaGame::loadLevel(int level)
{
	int row = level;
	int rowlength = row + 5;
	int columnwidth = 30;
	int nextcolumn = mWndCenterPt.x - (rowlength * 26 / 2);
	int nextrow = 200;

	for (int r = 0; r < row; r++)
	{
		for(int rl = 0; rl < rowlength; rl++)
		{
			Vec2 p;
			Vec2 v;
			p.x = nextcolumn;
			p.y = nextrow;
			v.x = 50;
			v.y = 0;
			Alien* alien = new Alien(mhAppInst, Alien::BLUE, p, v);
			mAliens.push_back(alien);
			nextcolumn += 30;
		}
		nextcolumn = mWndCenterPt.x - (rowlength * 26 / 2);
		nextrow += 50;
	}
}

bool rightToLeft(Alien* alien1, Alien* alien2)
{
	return (alien1->mAlienSprite->mPosition.x > alien2->mAlienSprite->mPosition.x);
}

bool leftToRight(Alien* alien1, Alien* alien2)
{
	return (alien1->mAlienSprite->mPosition.x < alien2->mAlienSprite->mPosition.x);
}

void GalagaGame::checkLevelDone()
{
	if (mAliens.size() == 0)
	{
		mLevelDone = true;
	}
}

/*
void GalagaGame::increaseScore(bool blue)
{
	if( blue )
		++mBlueScore;
	else
		++mRedScore;

	// A point was just scored, so reset puck to center and pause game.
	mPuck->mPosition = Vec2(mWndCenterPt.x, mWndCenterPt.y);
	mPuck->mVelocity = Vec2(0.0f, 0.0f);
	mPuck->mBoundingCircle.c = Vec2(mWndCenterPt.x, mWndCenterPt.y);

	// After score, pause the game so player can prepare for 
	// next round.
	pause();
}

*/