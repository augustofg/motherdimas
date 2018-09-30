/*
 * Rendering pipeline
 */

#ifndef DISPLAY_HPP_
#define DISPLAY_HPP_

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <vector>
#include <memory>

namespace display
{
    enum BlockType 
    {
        Earth,
        Iron,
        Tin,
        Silver,
        Gold,
        Diamond,
        Hole,
    };
    
    struct MapCoordinates
    {
        int x;
        int y;
    };

	struct MapPosition
    {
        float x;
        float y;
    };

	class GraphicObject
	{
        
    public:
        virtual void render(SDL_Renderer* screenRenderer, int cam_pos_x, int cam_pos_y);
	};

    
    class Block : public GraphicObject
    {
        
    private:
        SDL_Texture* texture;
        BlockType type;
        MapCoordinates coordinates;
        
    public:
        Block(SDL_Texture* texture, BlockType type, MapCoordinates coords);
        void render(SDL_Renderer* screenRenderer, int cam_pos_x, int cam_pos_y);
        
        BlockType getType();
        MapCoordinates getCoordinates();
        void setType(BlockType newType);
        void setTexture(SDL_Texture* texture);
    };
    
    
    class BackGround : public GraphicObject 
    {
    
    private:        
        SDL_Texture* diamond;
        SDL_Texture* earth;
        SDL_Texture* gold;
        SDL_Texture* iron;
        SDL_Texture* silver;
        SDL_Texture* tin;
        SDL_Texture* hole;
        
        std::vector<std::unique_ptr<Block>> backGrdObj;
        
    private:
        bool loadFromFile(std::string path, SDL_Renderer* screenRenderer, SDL_Texture** texture);
        
    public:
        bool loadMedias(std::string path, SDL_Renderer* screenRenderer);
        void initBackGround();
        int digHole(int block_pos_x, int block_pos_y);
        void render(SDL_Renderer* screenRenderer, int cam_pos_x, int cam_pos_y);
    };

	class Player
	{
	private:
		SDL_Texture* player_texture;
		MapPosition position;
		int health = 100;
		float speed_y = 0;
	public:
		Player();
        bool loadMedias(std::string path, SDL_Renderer* screenRenderer);
        void render(SDL_Renderer* screenRenderer, int cam_pos_x, int cam_pos_y);
        MapPosition getPosition();
        void setPosition(MapPosition pos);
        float getSpeed();
        void setSpeed(float speed_y);
	};
    
	class ScreenManager 
	{
    
    private:
        int screenWidth;
        int screenHeight;
        //The window we'll be rendering to
        SDL_Window* gameWindow = NULL;
        //The window renderer
        SDL_Renderer* gameRenderer = NULL;
                
    public:
        ScreenManager(int dim_x, int dim_y);
        ~ScreenManager();
        
        //Starts up SDL and creates window 
        bool init();
        void clearScreen();
        void updateScreen();   
        
        int getScreenWidth();
        int getScreenHeight();
        SDL_Window* getGameWindow();
        SDL_Renderer* getGameRenderer();
	};
}

#endif
