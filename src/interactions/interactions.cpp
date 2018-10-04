#include <cmath>
#include "interactions.hpp"

namespace interactions
{
	Colision::Colision(float lethal_speed)
	{
		this->lethal_speed = lethal_speed;
	}

	bool Colision::colide(display::Player &p, display::BackGround &b)
	{
		// auto p_pos = p.getPosition();
		// for (int i = 0; i < b.backGrdObj.size(); i++)
		// {
		// 	auto b_coords = b.backGrdObj[i]->getCoordinates();

		// 	float distance = sqrt(((p_pos.y - b_coords.y + 1) * (p_pos.y - b_coords.y + 1)) + ((p_pos.x - b_coords.x) * (p_pos.x - b_coords.x)));

		// 	if (distance < 0.1)
		// 	{
		// 		if (b.backGrdObj[i]->getType() == display::BlockType::Earth)
		// 		{
		// 			p_pos.x = b_coords.x;
		// 			p.setPosition(p_pos);
		// 			return false;
		// 		}
		// 		p_pos.y = b_coords.y - 1.0;
		// 		p.setSpeed(0);
		// 		p.setPosition(p_pos);
		// 		return true;
		// 	}
		// }
		return false;
	}
}
