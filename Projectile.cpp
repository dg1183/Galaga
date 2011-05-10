#include "Projectile.h"
#include "resource.h"


Projectile::Projectile(HINSTANCE appInst, bool playerOwned, Vec2 loc, Vec2 vel)
{
	Circle bc;
	bc.c = loc;
	bc.r = 6;

	mProjectileSprite = new Sprite(appInst, IDB_PROJECTILE, IDB_PROJECTILE_MASK, bc, loc, vel);
	mPlayerOwned = playerOwned;
}

Projectile::~Projectile(void)
{
	delete mProjectileSprite;
}

void Projectile::draw(HDC backbuffer, HDC spriteDC)
{
	mProjectileSprite->draw(backbuffer, spriteDC);
}

void Projectile::update(float dt)
{
	mProjectileSprite->update(dt);
}
