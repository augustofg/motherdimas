#include <iostream>
#include <vector>
#include <sstream>
#include <random>
#include <ctime>
#include "../utils/msglog.hpp"
#include "display.hpp"

namespace display{

//--------------------------------GRAPHIC OBJECT ---------------------------------------
    
    void GraphicObject::render(SDL_Renderer* screenRenderer, int cam_pos_x, int cam_pos_y)
    {
        
    }
    
//----------------------------------- BLOCK ---------------------------------------------
    
    //Block class constructor, initializes all of it's atributes
    Block::Block(SDL_Texture* texture, BlockType type, MapCoordinates coords)
    {
        this->texture = texture;
        this->type = type;
        this->coordinates = coords;
    }
    
    //Class responsible for render a block on the screen, given the coordinates of the game's camera
    void Block::render(SDL_Renderer* screenRenderer, int cam_pos_x, int cam_pos_y)
    {
        //Calculates the effective coordinates of the block, relative to the camera position
        int effective_x = (this->coordinates.x)*64 - cam_pos_x;
        int effective_y = (this->coordinates.y)*64 - cam_pos_y;
        
        //If the block is in the field of vision of the camera, it is renderized
        if(effective_x < 640 && effective_y < 640 && effective_x >= -63 && effective_y >= -63){

            //Set rendering space 
            SDL_Rect renderQuad = {effective_x, effective_y, 64, 64};
            //Render block to the screen
            SDL_RenderCopy(screenRenderer, this->texture, NULL, &renderQuad);
        }
    }

    //Getters and setters of the class Block
    MapCoordinates Block::getCoordinates()
    {
        return this->coordinates;
    }

    BlockType Block::getType()
    {
        return this->type;
    }

    void Block::setType(BlockType newType)
    {
        this->type = newType;
    }
    
    void Block::setTexture(SDL_Texture* texture)
    {
        this->texture = texture;
    }

//----------------------------------- Player ---------------------------------------------
	Player::Player()
    {
        this->position = {0,-1};
    }

    //Load file needed to render the driller on the screen
	bool Player::loadMedias(std::string path, SDL_Renderer* screenRenderer)
	{
		std::string filename = path + "/player.png";

		SDL_Surface* loadedSurface = IMG_Load(filename.c_str());
        
        /*Sets color keying on the driller to magenta, it allows the background of the 
          driller png image (the file /player.png*) to be transparent on the screen*/
		SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0xFF, 0, 0xFF));

        //Creates texture and free old loaded surface 
		this->player_texture = SDL_CreateTextureFromSurface(screenRenderer, loadedSurface);
		SDL_FreeSurface(loadedSurface);
		return true;
	}

	//Renders the main player (the driller) into the scrren
    void Player::render(SDL_Renderer* screenRenderer, int cam_pos_x, int cam_pos_y)
    {
        //The implementation of this method is quite simillar it's equivalent in the Block class
        int effective_x = (this->position.x) - cam_pos_x;
        int effective_y = (this->position.y) - cam_pos_y;
        if(effective_x < 640 && effective_y < 640 && effective_x >= -63 && effective_y >= -63){

            //Set rendering space and render to screen
            SDL_Rect renderQuad = {effective_x, effective_y, 64, 64};
            SDL_RenderCopy(screenRenderer, this->player_texture, NULL, &renderQuad);
		}
    }

    //Getters and setters of the player class
    MapPosition Player::getPosition()
    {
        return this->position;
    }

	float Player::getSpeed()
	{
		return this->speed_y;
	}

	void Player::setPosition(MapPosition pos)
	{
		this->position = pos;
	}

	void Player::setSpeed(float speed_y)
	{
		this->speed_y = speed_y;
	}

//--------------------------------- BACKGROUND ---------------------------------------
    
    //Class responsible for load a single file specified by the 'path' argument into the texture given as an argument of the method
    bool BackGround::loadFromFile(std::string path, SDL_Renderer* screenRenderer, SDL_Texture** texture)
    {
        //The final texture
        SDL_Texture* newTexture = NULL;
        
        //Load image at a specified path
        SDL_Surface* loadedSurface = IMG_Load(path.c_str());
        if( loadedSurface == NULL )
        {
            std::stringstream msg_error;
            msg_error << "Unable to load image " << path << " ! SDL_image Error: " << IMG_GetError();
            logmsg.error_msg(msg_error.str());
        }
        
        else
        {
            //Create texture from surface pixels
            newTexture = SDL_CreateTextureFromSurface(screenRenderer, loadedSurface);
            if( newTexture == NULL )
            {
                std::stringstream msg_error;
                msg_error << "Unable to create texture from " << path << " ! SDL Error: " << SDL_GetError();
                logmsg.error_msg(msg_error.str());
            }

            //Get rid of old loaded surface
            SDL_FreeSurface( loadedSurface );
        }

        //Return success
        (*texture) = newTexture;
        return (*texture)!= NULL;
    }
    
    /*Load all the medias needed to render the background of the game into the texture atributes of the class,
     if all of the files are loaded correctly the method returns true, otherwise it returns false*/
    bool BackGround::loadMedias(std::string path, SDL_Renderer* screenRenderer)
    {
        bool success = true;
        
        std::string fileDiamond = path + "/diamond_block.png";
        std::string fileEarth = path + "/earth_block.png";
        std::string fileGold = path + "/gold_block.png";
        std::string fileIron = path + "/iron_block.png";
        std::string fileSilver = path + "/silver_block.png";
        std::string fileTin = path + "/tin_block.png";
        std::string fileHole = path + "/hole_block.png";
        
        // Checks if the files are loaded correctly 
        if(!loadFromFile(fileDiamond, screenRenderer, &diamond)){
            logmsg.error_msg("Failed to load diamond_block.png texture image!");
            success = false;
        }
        if(!loadFromFile(fileEarth, screenRenderer, &earth)){
            logmsg.error_msg("Failed to load earth_block.png texture image!");
            success = false;
        }
        if(!loadFromFile(fileGold, screenRenderer, &gold)){
            logmsg.error_msg("Failed to load gold_block.png texture image!");
            success = false;
        }
        if(!loadFromFile(fileSilver, screenRenderer, &silver)){
            logmsg.error_msg("Failed to load silver_block.png texture image!");
            success = false;
        }
        if(!loadFromFile(fileIron, screenRenderer, &iron)){
            logmsg.error_msg("Failed to load iron_block.png texture image!");
            success = false;
        }
        if(!loadFromFile(fileTin, screenRenderer, &tin)){
            logmsg.error_msg("Failed to load tin_block.png texture image!");
            success = false;
        }
        if(!loadFromFile(fileHole, screenRenderer, &hole)){
            logmsg.error_msg("Failed to load hole_block.png texture image!");
            success = false;
        }
            
        return success;
    }
    
    //Initializes all of the blocks that make up the background of the game
    void BackGround::initBackGround()
    {   
        /*This block of code introduces randomness at the generation of the 
          background of the scenario*/
        std::default_random_engine generator;
        std::uniform_int_distribution<int> distribution(0,4095);
        generator.seed((int)time(NULL));
        
        for(int i = 0; i < 1000; i++){
            for(int j = 0; j < 10; j++){
                MapCoordinates coordinates;
                coordinates.x = j;
                coordinates.y = i;                
                
                BlockType type;
                SDL_Texture* texture;
                
                int dice_roll = distribution(generator);
                
                /*To make up the scenario, we choose something like an exponential
                 *distribution of the blocks, such that, the blocks with greater value
                 *appear less, and the blocks with smaller valuer apear more*/
                if(dice_roll >= 0 && dice_roll <= 15){
                    type = Diamond;
                    texture = this->diamond; 
                }
                
                else if(dice_roll >= 16 && dice_roll <= 63){
                    type = Gold;
                    texture = this->gold; 
                }
                
                else if(dice_roll >= 64 && dice_roll <= 255){
                    type = Silver;
                    texture = this->silver; 
                }
                
                else if(dice_roll >= 256 && dice_roll <= 1023){
                    type = Tin;
                    texture = this->tin; 
                }
                
                else if(dice_roll >= 1024 && dice_roll <= 2047){
                    type = Iron;
                    texture = this->iron; 
                }
                
                else if(dice_roll >= 2048 && dice_roll <= 4095){
                    type = Earth;
                    texture = this->earth; 
                }
                
                else{
                    std::stringstream msg_error;
                    msg_error << "o valor e " << std::to_string(dice_roll);
                    logmsg.debug_msg(msg_error.str(), 0);
                }
                
                // Generates the block and put it on the vector that represents the background itself
                std::unique_ptr<Block> newBlock(new Block(texture, type, coordinates));
                backGrdObj.push_back(std::move(newBlock));
            }
        }        
    }
    
    /* Given the coordinates of a block, turns tha block into a hole, indicating tha the driller 
     * has passed over that block */
    int BackGround::digHole(int block_pos_x, int block_pos_y)
    {
        if(block_pos_x < 0 || block_pos_x >= 10 || block_pos_y < 0 || block_pos_y >= 10000){
            return 0;
        }
        
        int score = 0;
        // Calculates the position the block at the vector using it's coordinates
        int real_pos = 10*block_pos_y + block_pos_x;
        BlockType actual_block = this->backGrdObj[real_pos]->getType();
        
        // Decides the score of the player based on the block that was drilled
        switch(actual_block){
            case Earth:
            score = 0;
            break;
            
            case Iron:
            score = 100;
            break;
            
            case Tin:
            score = 500;
            break;
            
            case Silver:
            score = 4000;
            break;
            
            case Gold:
            score = 20000;
            break;
            
            case Diamond:
            score = 100000;
            break;           
        }
        
        // Sets the block drilled to a hole
        this->backGrdObj[real_pos]->setType(Hole);
        this->backGrdObj[real_pos]->setTexture(this->hole);
        return score;
    }
    
    // Renders the entire background in the field of vision of the camera
    void BackGround::render(SDL_Renderer* screenRenderer, int cam_pos_x, int cam_pos_y)
    {
        //Calls the render method of each block that makes up the scenario
        for(int i = 0; i < backGrdObj.size(); i++){
            backGrdObj[i]->render(screenRenderer, cam_pos_x, cam_pos_y);
        }
    }

//-------------------------------- SCREEEN MANAGER ------------------------------------------

    //Contructor of the screen manager, initializes all of the atributes of the class
    ScreenManager::ScreenManager(int dim_x, int dim_y)
    {
        //Defines the dimensions of the screen
        this->screenWidth = dim_x;
        this->screenHeight = dim_y;
        //Initializes the render e the window of the game
        gameRenderer = NULL;
        gameWindow = NULL;
    }

    ScreenManager::~ScreenManager()
    {
        //Destroys window	
        SDL_DestroyRenderer(gameRenderer);
        SDL_DestroyWindow(gameWindow);
        gameWindow = NULL;
        gameRenderer = NULL;

        //Quit SDL subsystems
        IMG_Quit();
        SDL_Quit();
    }
    
    //Initializes the screen manager
    bool ScreenManager::init()
    {
        //Initialization flag
        bool success = true;

        //Initializes SDL
        if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
        {
            std::stringstream msg_error;
            msg_error << "SDL could not initialize! SDL Error: " << SDL_GetError();
            logmsg.error_msg(msg_error.str());
            success = false;
        }
        else
        {
            //Sets texture filtering to linear
            if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
            {
                logmsg.error_msg("Warning: Linear texture filtering not enabled!");
            }

            //Creates window
            gameWindow = SDL_CreateWindow( "Motherdimas", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screenWidth, screenHeight, SDL_WINDOW_SHOWN );
            if( gameWindow == NULL )
            {
                std::stringstream msg_error;
                msg_error << "Window could not be created! SDL Error: " << SDL_GetError();
                logmsg.error_msg(msg_error.str());
                success = false;
            }
            else
            {
                //Creates renderer for the window
                gameRenderer = SDL_CreateRenderer( gameWindow, -1, SDL_RENDERER_ACCELERATED );
                if( gameRenderer == NULL )
                {
                    std::stringstream msg_error;
                    msg_error << "Renderer could not be created! SDL Error:" << SDL_GetError(); 
                    logmsg.error_msg(msg_error.str());
                    success = false;
                }
                else
                {
                    //Initialize renderer color of the window to cyan
                    SDL_SetRenderDrawColor( gameRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

                    //Initialize PNG loading
                    int imgFlags = IMG_INIT_PNG;
                    if( !( IMG_Init( imgFlags ) & imgFlags ) )
                    {
                        std::stringstream msg_error;
                        msg_error << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError();
                        logmsg.error_msg(msg_error.str());
                        success = false;
                    }
                }
            }
        }

        return success;
    }

    void ScreenManager::clearScreen()
    {
        //Sets color of the window to cyan and clear screen
        SDL_SetRenderDrawColor( gameRenderer, 0, 0xFF, 0xFF, 0xFF );
        SDL_RenderClear( gameRenderer );
    }

    //Getters and setters of the screen manager
    void ScreenManager::updateScreen()
    {
        SDL_RenderPresent( gameRenderer );
    }

    int ScreenManager::getScreenWidth()
    {
        return screenWidth;
    }

    int ScreenManager::getScreenHeight()
    {
        return screenHeight;
    }
    
    SDL_Window* ScreenManager::getGameWindow() 
    {
        return gameWindow;
    }
    
    SDL_Renderer* ScreenManager::getGameRenderer()
    {
        return gameRenderer;
    }

}
