#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>


const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 640;

SDL_Window* gWindow = NULL;

bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{
		//Create window
		gWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( gWindow == NULL )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
	}

	return success;
}

void close()
{
	//Destroy window
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;

	//Quit SDL subsystems
	SDL_Quit();
}

int main() {
    int socket_fd;
    struct sockaddr_in target;
    
    if(!init()){
        printf( "Failed to initialize!\n" );
    }
    
    else{
        socket_fd = socket(AF_INET, SOCK_STREAM, 0);
        printf("Socket criado\n");

        target.sin_family = AF_INET;
        target.sin_port = htons(3002);
        inet_aton("127.0.0.1", &(target.sin_addr));
        printf("Tentando conectar\n");
        if (connect(socket_fd, (struct sockaddr*)&target, sizeof(target)) != 0) {
            printf("Problemas na conexao\n");
            return 0;
        }
        printf("Conectei ao servidor\n");

        /* Agora, meu socket funciona como um descritor de arquivo usual */
        SDL_Event event;
        int exit = 0;
        
        while(!exit){

            //Handle events on queue
            SDL_WaitEvent(&event);  
            send(socket_fd, &event, sizeof(SDL_Event), 0);   
            
            if(event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_q){
                exit = 1;
            }
        }
    }
    
    close(socket_fd);
    close();
    return 0;
}
