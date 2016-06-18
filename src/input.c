#include "input.h"
#include "SDL/SDL.h"

/*
    KEY LISTING
    -----------
    Start   - C
    Select  - X
    A       - S
    B       - D
    X       - W
    Y       - E
    Up      - UP
    Down    - DOWN
    Left    - LEFT
    Right   - RIGHT
    Exit    - ESC
*/

void initInput(){
    _input.up = 0;
    _input.down = 0;
    _input.left = 0;
    _input.right = 0;
    _input.a = 0;
    _input.b = 0;
    _input.x = 0;
    _input.y = 0; 
    _input.start = 0; 
    _input.select = 0;
    
    _inputRead.up = 0;
    _inputRead.down = 0;
    _inputRead.left = 0;
    _inputRead.right = 0;
    _inputRead.a = 0;
    _inputRead.b = 0;
    _inputRead.x = 0;
    _inputRead.y = 0; 
    _inputRead.start = 0; 
    _inputRead.select = 0;
}

void getInput(){
    SDL_Event event;
    
    while (SDL_PollEvent(&event)){
        switch (event.type){
            case SDL_QUIT:
                exit(0);
                break;
                
            case SDL_KEYDOWN:
                //switch here for different key down actions
                switch(event.key.keysym.sym){
    				case SDLK_UP:
						_input.up = 1;
						break;
					
					case SDLK_DOWN:
						_input.down = 1;
						break;
						
					case SDLK_LEFT:
						_input.left = 1;
						break;
						
					case SDLK_RIGHT:
						_input.right = 1;
						break;
					
					case SDLK_s:
						_input.a = 1;
						break;
						
					case SDLK_d:
						_input.b = 1;
						break;
					
					case SDLK_w:
						_input.x = 1;
						break;
						
					case SDLK_e:
						_input.y = 1;
						break;
					
					case SDLK_x:
						_input.select = 1;
						break;
						
					case SDLK_c:
						_input.start = 1;
						break;
					
					case SDLK_ESCAPE:
						exit(0);
						break;
						
                    default:
                        break;
                }
                break;
                
            case SDL_KEYUP:
                //switch statement here for different keyup actions
                switch(event.key.keysym.sym){
                    case SDLK_UP:
						_input.up = 0;
						_inputRead.up = 0;
						break;
					
					case SDLK_DOWN:
						_input.down = 0;
						_inputRead.down = 0;
						break;
						
					case SDLK_LEFT:
						_input.left = 0;
						_inputRead.left = 0;
						break;
						
					case SDLK_RIGHT:
						_input.right = 0;
						_inputRead.right = 0;
						break;
					
					case SDLK_s:
						_input.a = 0;
						_inputRead.a = 0;
						break;
						
					case SDLK_d:
						_input.b = 0;
						_inputRead.b = 0;
						break;
					
					case SDLK_w:
						_input.x = 0;
						_inputRead.x = 0;
						break;
						
					case SDLK_e:
						_input.y = 0;
						_inputRead.y = 0;
						break;
					
					case SDLK_x:
						_input.select = 0;
						_inputRead.select = 0;
						break;
						
					case SDLK_c:
						_input.start = 0;
						_inputRead.start = 0;
						break;
                    
                    default:
                        break;
                }
                break;
                
            default:
                break;
        }
    }
}

void setInputAllRead(){
    _inputRead.up = 1;
    _inputRead.down = 1;
    _inputRead.left = 1;
    _inputRead.right = 1;
    _inputRead.a = 1;
    _inputRead.b = 1;
    _inputRead.x = 1;
    _inputRead.y = 1; 
    _inputRead.start = 1; 
    _inputRead.select = 1;
}