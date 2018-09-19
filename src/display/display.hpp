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
		virtual void render(SDL_Renderer* grender, SDL_Texture* textures[], int pos x, int pos y);
	};

	class Renderer
	{
	public:
		Renderer(int dim_x, int dim_y);
		void update_screen(const std::vector<GraphicObject>& obj_list);
		~Renderer();
	}
}

#endif
