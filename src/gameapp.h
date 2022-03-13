/*-------------------------------------------------------------------------*/
/*-
https://github.com/JoanStinson/SteeringBehaviors
Please refer to above for License

Inspired by above steering application
Point click steering

*/
/*-------------------------------------------------------------------------*/


#ifndef GAMEAPP_H
#define GAMEAPP_H

#include <iostream>
/*-
encounter undefine reference to WinMain

X-Fox:
I was encountering this error while compiling 
my application with SDL. This was caused by 
SDL defining it's own main function in SDL_main.h. 
To prevent SDL define the main function an 
SDL_MAIN_HANDLED macro has to be defined 
before the SDL.h header is included.
*/
#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <SDL_image.h>

class gameApp
{

public:
	gameApp();
	~gameApp();
	static gameApp* Instance();
	void run();
	SDL_Renderer * getRenderer() const { return renderer; }
	void getWinSize(float *);
	void setWindowTitle(const char* title);
	void setFullScreen();

	bool loadSpriteTexture(const char* filename, int num_frames=1);
	void draw(float target[], float pos[], float orientation);
	void setPixel(SDL_Renderer *rend, int x, int y, 
		Uint8 r, Uint8 g, Uint8 b, Uint8 a);
	void drawCircle(SDL_Renderer *surface, int n_cx, int n_cy, int rad, 
		Uint8 r, Uint8 g, Uint8 b, Uint8 a);

private:
	SDL_Window * window;
	SDL_Renderer * renderer;
	static gameApp * s_pInstance;
	
	int win_width;
	int win_height;
	SDL_Color bg_color;
	bool win_fullscreen;
	bool quit;
	float last_update;
	float dtime;

	bool mclick;

	SDL_Texture *sprite_texture;
	SDL_Color color;
	bool draw_sprite;
	int sprite_num_frames;
	int sprite_w;
	int sprite_h;
};


#endif
