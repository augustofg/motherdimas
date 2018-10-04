/*
 * Process the interactions of the player with the world
 */

#ifndef INTERACTIONS_HPP_
#define INTERACTIONS_HPP_

#include <vector>
#include "../display/display.hpp"

// namespace display
// {
// 	struct Pos
// 	{
// 		float x;
// 		float y;
// 	};
// }

namespace interactions
{
	// struct Vel
	// {
	// 	float x;
	// 	float y;
	// };

	// struct MapCoordinates
	// {
	// 	int x;
	// 	int y;
	// };

	class Colision
	{
		float lethal_speed;
	public:
		Colision(float lethal_speed);
		bool colide(display::Player& p, display::BackGround& b);
	};

	// class Physical : display::GraphicObject
	// {
	// public:
	// 	Physical();
	// };

	// class MineBlock : display::GraphicObject
	// {
	// 	display::Pos pos;
	// 	MapCoordinates coord;
	// public:
	// 	void render(SDL_Renderer* grender, SDL_Texture* textures[], display::Pos pos);
	// };

	// class Player : display::GraphicObject
	// {
	// 	display::Pos pos;
	// 	Vel vel;
	// public:
	// 	void render(SDL_Renderer* grender, SDL_Texture* textures[], display::Pos pos);
	// 	display::Pos get_pos();
	// 	void set_pos(display::Pos pos);
	// 	Vel get_vel();
	// 	void set_vel(Vel vel);
	// };

	// class Map : display::GraphicObject
	// {
	// 	std::vector<MineBlock> blocks;
	// public:
	// 	void render(SDL_Renderer* grender, SDL_Texture* textures[], display::Pos pos);
	// };
}

#endif
