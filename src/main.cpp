#include <iostream>
#include <string>

#include "display/display.hpp"
#include "docopt.h"
#include "utils/msglog.hpp"

msglog::msglog logmsg;

static const char _USAGE[] =
    R"(MotherDimas.
Usage:
  __PROGNAME__ --assets-path apath [--debug-level=<num>]
  __PROGNAME__ (-h | --help)
  __PROGNAME__ --version
Options:
  -h --help           Show this screen.
  --version           Show version.
  --assets-path apath Set the assets directory.
  --debug-level=<num> Debug output verbosity level [default: -1].
)";

void search_and_replace(std::string& str, const std::string& old_str, const std::string& new_str)
{
	std::string::size_type index = 0;
	std::string::size_type old_str_len = old_str.length();
	std::string::size_type new_str_len = new_str.length();
	while ((index = str.find(old_str, index)) != std::string::npos)
	{
		str.replace(index, old_str_len, new_str);
		index += new_str_len;
	}
}

int main(int argc, char *argv[])
{
	std::string USAGE = _USAGE;
	search_and_replace(USAGE, "__PROGNAME__", argv[0]);

	std::map<std::string, docopt::value> args
		= docopt::docopt(USAGE,
						 { argv + 1, argv + argc },
						 true,               // show help if requested
						 "MotherDimas V0.1"); // version string

	if (args["--debug-level"])
	{
		logmsg.set_debug_level(atoi(args["--debug-level"].asString().c_str()));
		logmsg.debug_msg("Verbose mode", 0);
	}

	std::string assets_dir = args["--assets-path"].asString();
    
    using namespace display;
    
    ScreenManager manager(640,640);
    manager.init();
    SDL_Renderer* gameRenderer = manager.getGameRenderer();
    
    BackGround scenario;    
    scenario.loadMedias(assets_dir, gameRenderer);
    scenario.initBackGround();
    
    Player drill;
    drill.loadMedias(assets_dir, gameRenderer);
    
    int cam_pos_x = 0;
    int cam_pos_y = -320;
    
    MapPosition position;
    
    //Main loop flag
    bool quit = false;
    //Event handler
    SDL_Event event;


    //While application is running
    while(!quit){
        
        //Handle events on queue
        while(SDL_PollEvent(&event) != 0){
            if(event.type == SDL_QUIT){
                quit = true;
            }
            
            else if(event.type == SDL_KEYDOWN){
                
                switch(event.key.keysym.sym){
                    case SDLK_UP:
                    cam_pos_y -= 10;
                    position = drill.getPosition();
                    position.y -= 10;
                    drill.setPosition(position);                    
                    break;
                        
                    case SDLK_DOWN:
                    cam_pos_y += 10;
                    position = drill.getPosition();
                    position.y += 10;
                    drill.setPosition(position);
                    break;
                        
                    case SDLK_LEFT:
                    position = drill.getPosition();
                    position.x -= 10;
                    drill.setPosition(position);
                    break;
                        
                    case SDLK_RIGHT:
                    position = drill.getPosition();
                    position.x += 10;
                    drill.setPosition(position);
                    break;
                }
            }
        }
        
        manager.clearScreen();
        scenario.render(gameRenderer, cam_pos_x, cam_pos_y);
        drill.render(gameRenderer, cam_pos_x, cam_pos_y);
        manager.updateScreen();         
    }
    

	std::cout << "Hello!" << std::endl;
    return 0;
}
