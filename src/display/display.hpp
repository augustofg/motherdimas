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
    //All of the possible types of a block in the game
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
    
    //Coordinates used by the blocks to define their positions on the screen
    struct MapCoordinates
    {
        int x;
        int y;
    };

    //Coordinates used by the player (drill) to define it's positions on the screen
	struct MapPosition
    {
        float x;
        float y;
    };

    // This class was intended to be used to provide polimorphism
	class GraphicObject
	{
        
    public:
        virtual void render(SDL_Renderer* screenRenderer, int cam_pos_x, int cam_pos_y);
	};

    //Class used to represent a single block of the game
    class Block : public GraphicObject
    {
        
    private:
        // The texture of the block depends on it's type
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
    
    /*This class abstracts the Background of the game. It's made up of 1000 rows of 10 blocks each, 
     *each of which can be of type diamond, gold, silver, tin, iron, earth or a hole */
    class BackGround : public GraphicObject 
    {
    
    private:        
        /*This attributes defines the possible textures of the blocks in the background,
          each block loads it's texture from one of these attributes*/
        SDL_Texture* diamond;
        SDL_Texture* earth;
        SDL_Texture* gold;
        SDL_Texture* iron;
        SDL_Texture* silver;
        SDL_Texture* tin;
        SDL_Texture* hole;
        //Vector that represents all of the blocks in the game
        std::vector<std::unique_ptr<Block>> backGrdObj;
        bool loadFromFile(std::string path, SDL_Renderer* screenRenderer, SDL_Texture** texture);

    public:
        bool loadMedias(std::string path, SDL_Renderer* screenRenderer);
        void initBackGround();
        int digHole(int block_pos_x, int block_pos_y);
        void render(SDL_Renderer* screenRenderer, int cam_pos_x, int cam_pos_y);
    };
    
    // This class abstracts the player (the drill)
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
    
    //This class abstracts the screen manager
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
