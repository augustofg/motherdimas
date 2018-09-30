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
    Block::Block(SDL_Texture* texture, BlockType type, MapCoordinates coords)
    {
        this->texture = texture;
        this->type = type;
        this->coordinates = coords;
    }

    void Block::render(SDL_Renderer* screenRenderer, int cam_pos_x, int cam_pos_y)
    {
        int effective_x = (this->coordinates.x)*64 - cam_pos_x;
        int effective_y = (this->coordinates.y)*64 - cam_pos_y;
        if(effective_x < 640 && effective_y < 640 && effective_x >= -63 && effective_y >= -63){

            //Set rendering space and render to screen
            SDL_Rect renderQuad = {effective_x, effective_y, 64, 64};
            SDL_RenderCopy(screenRenderer, texture, NULL, &renderQuad);
        }
    }

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
        this->position = {0.5,-64};
    }

	bool Player::loadMedias(std::string path, SDL_Renderer* screenRenderer)
	{
		std::string filename = path + "/player.png";

		SDL_Surface* loadedSurface = IMG_Load(filename.c_str());
		SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0xFF, 0, 0xFF));

		this->player_texture = SDL_CreateTextureFromSurface(screenRenderer, loadedSurface);
		SDL_FreeSurface(loadedSurface);
		return true;
	}

    void Player::render(SDL_Renderer* screenRenderer, int cam_pos_x, int cam_pos_y)
    {
        int effective_x = (this->position.x) - cam_pos_x;
        int effective_y = (this->position.y) - cam_pos_y;
        if(effective_x < 640 && effective_y < 640 && effective_x >= -63 && effective_y >= -63){

            //Set rendering space and render to screen
            SDL_Rect renderQuad = {effective_x, effective_y, 64, 64};
            SDL_RenderCopy(screenRenderer, this->player_texture, NULL, &renderQuad);
		}
    }

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
    
    void BackGround::initBackGround()
    {
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
                
                std::unique_ptr<Block> newBlock(new Block(texture, type, coordinates));
                backGrdObj.push_back(std::move(newBlock));
            }
        }        
    }
    
    int BackGround::digHole(int block_pos_x, int block_pos_y)
    {
        if(block_pos_x < 0 || block_pos_x >= 10 || block_pos_y < 0 || block_pos_y >= 10000){
            logmsg.error_msg("There's no block in such position");
            return 0;
        }
        
        int score;
        int real_pos = 10*block_pos_y + block_pos_x;
        BlockType actual_block = this->backGrdObj[real_pos]->getType();
        
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
        
        this->backGrdObj[real_pos]->setType(Hole);
        this->backGrdObj[real_pos]->setTexture(this->hole);
        return score;
    }
    
    void BackGround::render(SDL_Renderer* screenRenderer, int cam_pos_x, int cam_pos_y)
    {
        for(int i = 0; i < backGrdObj.size(); i++){
            backGrdObj[i]->render(screenRenderer, cam_pos_x, cam_pos_y);
        }
    }

//-------------------------------- SCREEEN MANAGER ------------------------------------------
    ScreenManager::ScreenManager(int dim_x, int dim_y)
    {
        this->screenWidth = dim_x;
        this->screenHeight = dim_y;
        gameRenderer = NULL;
        gameWindow = NULL;
    }

    ScreenManager::~ScreenManager()
    {
        //Destroy window	
        SDL_DestroyRenderer(gameRenderer);
        SDL_DestroyWindow(gameWindow);
        gameWindow = NULL;
        gameRenderer = NULL;

        //Quit SDL subsystems
        IMG_Quit();
        SDL_Quit();
    }
    
    bool ScreenManager::init()
    {
        //Initialization flag
        bool success = true;

        //Initialize SDL
        if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
        {
            std::stringstream msg_error;
            msg_error << "SDL could not initialize! SDL Error: " << SDL_GetError();
            logmsg.error_msg(msg_error.str());
            success = false;
        }
        else
        {
            //Set texture filtering to linear
            if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
            {
                logmsg.error_msg("Warning: Linear texture filtering not enabled!");
            }

            //Create window
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
                //Create renderer for window
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
                    //Initialize renderer color ---------------- SUJEITO A MUDANÇA -------------
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
        //Clear screen
        SDL_SetRenderDrawColor( gameRenderer, 0, 0xFF, 0xFF, 0xFF );
        SDL_RenderClear( gameRenderer );
    }

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
