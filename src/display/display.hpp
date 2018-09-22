/*
 * Rendering pipeline
 */

#ifndef DISPLAY_HPP_
#define DISPLAY_HPP_

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <vector>

namespace display
{
	class GraphicObject
	{
	public:
		virtual void render(SDL_Renderer* grender, SDL_Texture* textures[], int pos_x, int pos_y);
	};

	class Render
	{
	public:
		Render(int dim_x, int dim_y);
		void update_screen(const std::vector<GraphicObject>& obj_list);
		~Render();
	};
}

#endif
