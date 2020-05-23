
#include "pixelengine.h"

#include <vector>
#include <iostream>
#include "positions.h"


class Road : public olc::PixelGameEngine
{
private:
	olc::Sprite* road = new olc::Sprite("textures/static_road.png");
public:
	void sprite();
};
class Sun : public olc::PixelGameEngine
{
private:
	olc::Sprite* sun = new olc::Sprite("textures/lowdynamic_sun.png");
public:
	void sprite();
};

class Backfield : public olc::PixelGameEngine
{
private:
	olc::Sprite* field = new olc::Sprite("textures/static_field.png");
	olc::Sprite* sky = new olc::Sprite("textures/static_sky.png");
public:
	void sprite();
};

class Spriter
{
public:
	std::vector<Sun> sun;
	std::vector<Road> road;
	std::vector<Backfield> backfield;

	void sprite();
};



class SpriteBuilder
{
protected:
	Spriter* s;
public:
	SpriteBuilder() : s(0) {}
	virtual ~SpriteBuilder() {}
	virtual void createSprite() {}
	virtual void buildSun() {}
	virtual void buildRoad() {}
	virtual void buildBackfield() {}
	virtual Spriter* getSpriter() { return s; }
};

class SpritesStaticBuilder : public SpriteBuilder
{
public:
	void createSprite() { s = new Spriter; }
	void buildSun() { s->sun.push_back(Sun()); }
	void buildRoad() { s->road.push_back(Road()); }
	void buildBackfield() { s->backfield.push_back(Backfield()); }
};



class Director
{
public:
	Spriter* createArmy(SpriteBuilder& builder);
};
