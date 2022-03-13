/*-------------------------------------------------------------------------*/
/*-
https://github.com/JoanStinson/SteeringBehaviors
Please refer to above for License

Inspired by above steering application
Point click steering

*/
/*-------------------------------------------------------------------------*/
#define _USE_MATH_DEFINES
#include <math.h>


#include "gameapp.h"
#include "mycar.h"

#define RAD2DEG (180.0f / (float)M_PI)
#define DEG2RAD ((float)M_PI / 180.0f)

using namespace std;

gameApp * gameApp::s_pInstance = 0;

gameApp * gameApp::Instance()
{
	if (s_pInstance == 0)
	{
		s_pInstance = new gameApp();
	}
	return s_pInstance;
}

gameApp::gameApp()
{
	win_fullscreen = false;
	win_width = 1280;
	win_height = 720;

	quit = false;

	SDL_Init(SDL_INIT_VIDEO);

	if ((IMG_Init(IMG_INIT_PNG)&IMG_INIT_PNG) != IMG_INIT_PNG) {
		cout << "IMG_Init: Failed to init required img support!" << endl;
		cout << "IMG_Init: " << IMG_GetError() << endl;
	}

	window = SDL_CreateWindow("Game",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
		win_width, win_height, 0);

	if (win_fullscreen)
		SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);

	renderer = SDL_CreateRenderer(window, -1, 0);


	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);

	last_update = (float)SDL_GetTicks();

	mclick = false;

	draw_sprite = false;
	sprite_w = 0;
	sprite_h = 0;
	sprite_num_frames = 1;
	sprite_texture = 0;
	color = { 255,255,255,255 };
}

gameApp::~gameApp()
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	IMG_Quit();
	SDL_Quit();
}

void gameApp::run()
{
	SDL_Event event;
	myCar car(5,5);

	loadSpriteTexture("../img/car.png", 1);

    while (!quit)
    {
		SDL_PollEvent(&event);

		/* Keyboard events */
		switch (event.type)
		{
			case SDL_MOUSEMOTION:
			case SDL_MOUSEBUTTONDOWN:
				if (event.button.button == SDL_BUTTON_LEFT)
				{
					car.target[0] = (float)(event.button.x)/32;
					car.target[1] = (float)(event.button.y)/32;
					mclick = true;
				}
				if (event.key.keysym.scancode == SDL_SCANCODE_SPACE)
					draw_sprite = !draw_sprite;
				break;
			case SDL_KEYDOWN:
				if ((event.key.keysym.scancode == SDL_SCANCODE_Q) || 
					(event.key.keysym.scancode == SDL_SCANCODE_ESCAPE))
				{
					quit = true;
				}
				if (event.key.keysym.scancode == SDL_SCANCODE_F)
				{
					setFullScreen();
				}
				break;
			case SDL_QUIT:
				quit = true;
				break;
			default:
				break;
		}

		dtime = (float)(SDL_GetTicks() - last_update) / 1000.0f;
		last_update = (float)SDL_GetTicks();

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); //TODO set bg color
		SDL_RenderClear(renderer);

		if (mclick)
		{
			if (car.gotto(car.target,dtime))
				mclick = false;
		}
		car.update(dtime);

		draw(car.target,car.position,car.angle);

		SDL_RenderPresent(renderer);


    }

}

void gameApp::getWinSize(float *size)
{
	size[0] = (float)win_width;
	size[1] = (float)win_height;
}

void gameApp::setWindowTitle(const char *title)
{
	SDL_SetWindowTitle(window, title);
}

void gameApp::setFullScreen()
{
	win_fullscreen = !win_fullscreen;
	if (win_fullscreen)
		SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
	else
	{
		SDL_SetWindowFullscreen(window, 0);
		SDL_SetWindowDisplayMode(window, NULL);
	}
}

bool gameApp::loadSpriteTexture(const char* filename, int _num_frames)
{
	if (_num_frames < 1) return false;

	SDL_Surface *image = IMG_Load(filename);
	if (!image) {
		cout << "IMG_Load: " << IMG_GetError() << endl;
		return false;
	}
	sprite_texture = SDL_CreateTextureFromSurface(getRenderer(), image);

	sprite_num_frames = _num_frames;
	sprite_w = image->w / sprite_num_frames;
	sprite_h = image->h;
	draw_sprite = true;

	if (image)
		SDL_FreeSurface(image);

	return true;
}

void gameApp::draw(float target[], float pos[], float orientation)
{
	float a = pos[0] - target[0];
	float b = pos[1] - target[1];
	float dist = sqrt(a*a + b*b);
	int r = 5;

	int ptarget[2],ppos[2];
	ptarget[0] = (int)(target[0])*32;
	ptarget[1] = (int)(target[1])*32;
	ppos[0] = (int)(pos[0])*32;
	ppos[1] = (int)(pos[1])*32;

	if (mclick)
	{
		drawCircle(getRenderer(), 
			(int)ptarget[0], (int)ptarget[1], 15, 255, 0, 0, 255);
		if (dist >= r) {
			drawCircle(getRenderer(), 
				ptarget[0], ptarget[1], r*32, 0, 0, 255, 1);
		}
		else {
			drawCircle(getRenderer(), 
				ptarget[0], ptarget[1], r*32, 0, 255, 0, 1);
		}

	}


	if (draw_sprite)
	{
		Uint32 sprite = 0.9;
		
		SDL_Rect srcrect = { (int)sprite * sprite_w, 0, sprite_w, sprite_h };
		SDL_Rect dstrect = { (int)ppos[0] - (sprite_w / 2), 
			(int)ppos[1] - (sprite_h / 2), 
			sprite_w, sprite_h };
		SDL_Point center = { sprite_w / 2, sprite_h / 2 };
		SDL_RenderCopyEx(getRenderer(), sprite_texture, &srcrect, &dstrect, 
			orientation, &center, SDL_FLIP_NONE);

		/*-
		SDL_Rect rectline;
		rectline.x = (pos[0] - 2)*32;
		rectline.y = (pos[1] - 2)*32;
		rectline.w = 4*32;
		rectline.h = 4*32;
		SDL_SetRenderDrawColor( getRenderer(), 255, 255, 255, 255 );
		SDL_RenderDrawRect(getRenderer(), &rectline);
		*/
	}
	else 
	{
		drawCircle(getRenderer(), 
			(int)ppos[0], (int)ppos[1], 
			15, color.r, color.g, color.b, color.a);
		SDL_RenderDrawLine(getRenderer(), 
			(int)ppos[0], (int)ppos[1], 
			(int)(ppos[0]+15*cos(orientation*DEG2RAD)), 
			(int)(ppos[1]+15*sin(orientation*DEG2RAD)));
	}
}

void gameApp::setPixel(SDL_Renderer *rend, int x, int y, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
	SDL_SetRenderDrawColor(rend, r, g, b, a);
	SDL_RenderDrawPoint(rend, x, y);
}

void gameApp::drawCircle(SDL_Renderer *surface, int n_cx, int n_cy, int rad, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
	if (rad < 3) return;

	for (int radius = rad - 2; radius <= rad; radius++)
	{
		double error = (double)-radius;
		double x = (double)radius - 0.5;
		double y = (double)0.5;
		double cx = n_cx - 0.5;
		double cy = n_cy - 0.5;

		while (x >= y)
		{
			setPixel(surface, (int)(cx + x), (int)(cy + y), r, g, b, a);
			setPixel(surface, (int)(cx + y), (int)(cy + x), r, g, b, a);

			if (x != 0)
			{
				setPixel(surface, (int)(cx - x), (int)(cy + y), r, g, b, a);
				setPixel(surface, (int)(cx + y), (int)(cy - x), r, g, b, a);
			}

			if (y != 0)
			{
				setPixel(surface, (int)(cx + x), (int)(cy - y), r, g, b, a);
				setPixel(surface, (int)(cx - y), (int)(cy + x), r, g, b, a);
			}

			if (x != 0 && y != 0)
			{
				setPixel(surface, (int)(cx - x), (int)(cy - y), r, g, b, a);
				setPixel(surface, (int)(cx - y), (int)(cy - x), r, g, b, a);
			}

			error += y;
			++y;
			error += y;

			if (error >= 0)
			{
				--x;
				error -= x;
				error -= x;
			}
		}
	}
}
