#include <iostream>
#include <vector>
#include "canvas_builder.h"
#include "positions.h"



void Road::sprite() {
	std::cout << "ROAD";
		olc::PixelGameEngine::DrawSprite(0, ROAD, road, 1);
}

void Sun::sprite() {
	std::cout << "SUN";
	DrawSprite(401, SUN, sun, 1);
}

void Backfield::sprite() {
	std::cout << "BACKFIELD";
	DrawSprite(0, SKY, sky, 1);
	DrawSprite(0, BUSH_FIELD, field, 1);
}


void Spriter::sprite() {
	int i;
	for (i = 0; i < sun.size(); ++i)  sun[i].sprite();
	for (i = 0; i < road.size(); ++i)  road[i].sprite();
	for (i = 0; i < backfield.size(); ++i)  backfield[i].sprite();
}




Spriter* Director::createArmy(SpriteBuilder& builder)
	{
	builder.createSprite();
	builder.buildSun();
	builder.buildRoad();
	builder.buildBackfield();
	return(builder.getSpriter());
}