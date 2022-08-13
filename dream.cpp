#include <iostream>
#include <cstdlib>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_gamecontroller.h>
#include <string>

using namespace std;

#include "dream.h"

Timer fpsTimer;

SDL_Window *window = NULL;    //window
SDL_Renderer *renderer = NULL;    //Renderer

SDL_Surface* surface;
SDL_Texture* texture;

SDL_Joystick *joystick[2];

SDL_Event event;



const Uint8 *inkeys = SDL_GetKeyboardState(NULL);

bool kill = false;

//FPS Stuff

float avgFPS;
int countedFrames;

//Window Creation

void CreateWindow(char* name, int sw, int sh, bool fs)
{
   SDL_Init(SDL_INIT_EVERYTHING);
   TTF_Init();

   window = SDL_CreateWindow(name, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, sw, sh, SDL_WINDOW_OPENGL);
   SDL_SetWindowFullscreen(window, fs);

   renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

   cout << "Number of Joysticks: " << SDL_NumJoysticks() << endl;
   for(int i = 0; i < SDL_NumJoysticks(); i++)
   {

     joystick[i] = SDL_JoystickOpen(i);
     cout << "SUPPROTED: " << i <<endl;
   }

   Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 8, 4096);
}

//Text Writing

void Font::addFont(char* path, int size)
{
  font = TTF_OpenFont(path, size);
}

void WriteText(string text, Vector2 font_pos, Color color, Font font_w)
{
  int texW = 0;
  int texH = 0;

  SDL_Color col = {color.r, color.g, color.b};

  surface = TTF_RenderText_Solid(font_w.font, text.c_str(), col);

  texture = SDL_CreateTextureFromSurface(renderer, surface);

  SDL_QueryTexture(texture, NULL, NULL, &texW, &texH);

  SDL_Rect textrect = {(int)font_pos.x, (int)font_pos.y, texW, texH};

  SDL_RenderCopy(renderer, texture, NULL, &textrect);

  SDL_DestroyTexture(texture);
  SDL_FreeSurface(surface);
}
//Clear screen

void ClearScreen()
{
	SDL_SetRenderDrawColor(renderer,0,0,0,SDL_ALPHA_OPAQUE);
	SDL_RenderClear(renderer);
}

//SFX

void SFX::add_sound(char* path)
{
  if(path == NULL)
  {
    cout << "SFX not found" <<endl;
    return;
  }
  sfx = Mix_LoadWAV(path);
}

void SFX::play_sound(int loops)
{
  Mix_PlayChannel(-1, sfx, loops);
}

void Render(int fps)
{
  fpsTimer.reset_time();
  int scr_fps = 1000/fps;

  avgFPS = countedFrames/(fpsTimer.get_time()/1000.f);
  ++countedFrames;

	SDL_RenderPresent(renderer);

  int frameTicks = fpsTimer.get_time();

  if(frameTicks <= scr_fps)
    SDL_Delay(scr_fps - frameTicks);
}

bool done()
{
  SDL_PollEvent(&event);

  if(event.type == SDL_QUIT || kill)
    return true;

  return false;
}

void kill_sw()
{
  kill = true;
}

void CloseWindow()
{
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);

  SDL_Quit();
  Mix_Quit();
  IMG_Quit();
}

//Texture functions

void Texture::LoadFile(string str)
{
  SDL_Surface *temp = IMG_Load(str.c_str());

  width = temp->w;
  height = temp->h;

  texture = SDL_CreateTextureFromSurface(renderer, temp);

  delete temp;
}

void Texture::ScaleTexture(int swidth, int sheight)
{
  width = swidth;

  height = sheight;
}

void Texture::DrawTexture(Vector2 vect)
{
  SDL_Rect rect = {(int)vect.x,(int)vect.y,width,height};
  SDL_RenderCopy(renderer, texture, NULL, &rect);
}

void Texture::DrawTexture(Vector2 vect, Vector2 area)
{
  SDL_Rect rect = {(int)vect.x,(int)vect.y,width,height};
  SDL_Rect area1 = {(int)area.x,(int)area.y,(int)area.w,(int)area.h};
  SDL_RenderCopy(renderer, texture, &area1, &rect);
}

//Audio

//Input

bool CheckKeyPress(int key)
{
  return (inkeys[key] != 0);
}

bool CheckCtrlPress(int i, int button)
{
  return (SDL_JoystickGetButton(joystick[i], button));
}

float CheckAxisState(int i, int axis)
{
	return(SDL_JoystickGetAxis(joystick[i],axis));
}

//Primitive drawing

void DrawPixel(Vector2 pos, Color color)
{
  SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, SDL_ALPHA_OPAQUE);
  SDL_RenderDrawPoint(renderer, (int)pos.x, (int)pos.y);
}

void DrawLine(Vector2 pos, Color color)
{
  SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, SDL_ALPHA_OPAQUE);
  SDL_RenderDrawLine(renderer, (int)pos.x, (int)pos.y, (int)pos.w, (int)pos.h);
}

void DrawQuad(Vector2 shape, Color color)
{
  SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, SDL_ALPHA_OPAQUE);

  SDL_Rect rect;
  rect.x = (int)shape.x;
  rect.y = (int)shape.y;
  rect.w = (int)shape.w;
  rect.h = (int)shape.h;

  SDL_RenderFillRect(renderer, &rect);
}

//Collision Detection
bool IsColliding(Vector2 a, Vector2 b)
{
  SDL_Rect a_r = {(int)a.x,(int)a.y, (int)a.w, (int)a.h};
  SDL_Rect b_r = {(int)b.x, (int)b.y, (int)b.w, (int)b.h};
  SDL_Rect res;

  if (SDL_HasIntersection(&a_r, &b_r) == SDL_TRUE)
    return true;
  else
    return false;
}

//Timer functions

Timer::Timer()
{
  present_time = SDL_GetTicks();
}

void Timer::reset_time()
{
  present_time = SDL_GetTicks();
}

Uint32 Timer::get_time()
{
  return (SDL_GetTicks() - present_time);
}

