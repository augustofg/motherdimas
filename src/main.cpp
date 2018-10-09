#include <iostream>
#include <string>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "display/display.hpp"
#include "docopt.h"
#include "utils/msglog.hpp"
#include "interactions/interactions.hpp"
#include "sound/sound.hpp"

msglog::msglog logmsg;
using namespace display;
using namespace std::chrono_literals;

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

	/*
	 * Initialize the screen
	 */
    ScreenManager manager(640,640);
    manager.init();

	/*
	 * Get the render context
	 */
    SDL_Renderer* gameRenderer = manager.getGameRenderer();

    BackGround scenario;
    scenario.loadMedias(assets_dir, gameRenderer);
    scenario.initBackGround();

    Player drill;
    drill.loadMedias(assets_dir, gameRenderer);

    int cam_pos_x = 0;
    int cam_pos_y = -320;

    int hole_pos_x = 0;
    int hole_pos_y = 0;

    MapPosition position;

    //Main loop flag
    bool quit = false;
    //Event handler
    SDL_Event event;

	SDL_Init(SDL_INIT_AUDIO);
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

	sound::SoundSample drill_sample, rocket_sample;
	drill_sample.load_media_file(assets_dir + "/construction-jackhammer-excavator.wav");
	rocket_sample.load_media_file(assets_dir + "/rocket-launch.wav");

	bool arrow_up_was_pressed = false;
	bool arrow_down_was_pressed = false;
	int score = 0;

	MapPosition pos;
	pos.x = 0;
	pos.y = -64;
	drill.setPosition(pos);

	std::condition_variable exit_render_var;
	std::mutex mutex_render;
	std::atomic<bool> exit_render{false};

	/*
	 * Render thread
	 */
	std::thread render_thread([&manager, &scenario, &drill, &cam_pos_x, &cam_pos_y, &exit_render_var, &mutex_render, &exit_render]
								  {
									  /*
									   * Update the screen contents
									   */
									  std::unique_lock<std::mutex> lk(mutex_render);
									  SDL_Renderer* gameRenderer = manager.getGameRenderer();

									  while (1)
									  {
										  manager.clearScreen();
										  scenario.render(gameRenderer, cam_pos_x, cam_pos_y);
										  drill.render(gameRenderer, cam_pos_x, cam_pos_y);
										  manager.updateScreen();

										  /*
										   * Wait for 33ms or for a notification to exit
										   */
										  if (exit_render_var.wait_for(lk, 33*1ms, [&exit_render]{return exit_render.load();}))
										  {
											  break;
										  }
									  }
								  });

    // Creates and initializes socket
    int socket_fd, connection_fd;
    struct sockaddr_in myself, client;
    socklen_t client_size = (socklen_t)sizeof(client);

    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    printf("Socket criado\n");

    myself.sin_family = AF_INET;
    myself.sin_port = htons(3002);
    inet_aton("127.0.0.1", &(myself.sin_addr));

    printf("Tentando abrir porta 3002\n");
    if (bind(socket_fd, (struct sockaddr*)&myself, sizeof(myself)) != 0) {
        printf("Problemas ao abrir porta\n");
        return 0;
    }
    printf("Abri porta 3002!\n");

    listen(socket_fd, 2);
    printf("Estou ouvindo na porta 3002!\n");

    //While application is running
    while(!quit){

        printf("Vou travar ate receber alguma coisa\n");
        connection_fd = accept(socket_fd, (struct sockaddr*)&client, &client_size);
        printf("Recebi uma mensagem:\n");

        //Handle events on queue
		recv(connection_fd, &event, sizeof(SDL_Event), 0);
		if(event.type == SDL_KEYDOWN){

			switch(event.key.keysym.sym){
				/*
				 * Move the drill down if the down arrow is
				 * pressed
				 */
			case SDLK_UP:
				if(!arrow_up_was_pressed)
				{
					rocket_sample.play(10, 10, -1);
					arrow_up_was_pressed = true;
				}
				cam_pos_y -= 64;
				position = drill.getPosition();
				position.y -= 64;
				drill.setPosition(position);
				break;

				/*
				 * Move the drill up if the up arrow is pressed
				 */
			case SDLK_DOWN:
				if(!arrow_down_was_pressed)
				{
					drill_sample.play(400, 600);
					arrow_down_was_pressed = true;
				}
				cam_pos_y += 64;
				position = drill.getPosition();
				position.y += 64;
				drill.setPosition(position);
				score += scenario.digHole(position.x / 64, position.y / 64);
				break;

				/*
				 * Move the drill to the left if the left arrow is
				 * pressed
				 */
			case SDLK_LEFT:
				position = drill.getPosition();
				position.x -= 64;
				drill.setPosition(position);
				break;

				/*
				 * Move the drill to the right if the right arrow is
				 * pressed
				 */
			case SDLK_RIGHT:
				position = drill.getPosition();
				position.x += 64;
				drill.setPosition(position);
				break;

            case SDLK_q:
                quit = true;
                break;
			}
		}
		/*
		 * Stop the sound effects when the keys are released
		 */
		else if(event.type == SDL_KEYUP){
			switch(event.key.keysym.sym){
			case SDLK_UP:
				if (arrow_up_was_pressed)
				{
					rocket_sample.stop();
					arrow_up_was_pressed = false;
				}
				break;
			case SDLK_DOWN:
				if (arrow_down_was_pressed)
				{
					arrow_down_was_pressed = false;
				}
				break;
			}
		}
	}

	close(socket_fd);
	std::cout << "You died!" << std::endl << "Score: " << score << std::endl;
	exit_render = true;
	exit_render_var.notify_one();
	render_thread.join();
    return 0;
}
