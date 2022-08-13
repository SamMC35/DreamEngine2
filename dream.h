#ifndef DREAM_H
#define DREAM_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

#include <string>
using namespace std;

struct Vector2	//Vector2 for position and size
{
    float x,y,w,h;
};

struct Color
{
   Uint8 r,g,b;
};

class Texture
{
    SDL_Texture* texture;	//Texture

public:
    int width, height;

    void LoadFile(string);		//Load file
    SDL_Texture* ReturnTexture();	//Return texture
    void ScaleTexture(int,int);		//Scale texture
    void FlipTexture(int);		//Flip texture
    void KeyTexture(Color);

    void DrawTexture(Vector2);    //Draw Texture as is
    void DrawTexture(Vector2, Vector2);
};

class Timer
{
    Uint32 present_time;
    Uint32 last_time;

public:
    Timer();
    void reset_time();
    Uint32 get_time();
};



class Font
{
public:
  TTF_Font *font;
  void addFont(char*, int);
};

class Music
{
	Mix_Music* music;

public:
	void add_music(char*);
	void play_music();
};

class SFX
{
    Mix_Chunk* sfx;
    
public:
    void add_sound(char*);
    void play_sound(int);
};

//Window and Rendering stuff
void CreateWindow(char*, int, int, bool);
void ClearScreen();
void Render(int);
void CloseWindow();

//Input press
bool CheckKeyPress(int);
bool CheckCtrlPress(int, int);
float CheckAxisState(int i , int axis);
int CheckHatState(int i, int axis);

//Text Input
void WriteText(string, Vector2, Color, Font);

//Primitive Shape Functions
void DrawPixel(Vector2, Color);
void DrawQuad(Vector2, Color);
void DrawLine(Vector2, Color);

//Collision
bool IsColliding(Vector2, Vector2);

//Texture rendering
void RenderSprites(Texture*, Vector2);

//Audio 
void SetPitch(int);
void SetAudioFlag(bool);

bool done();
void kill_sw();

#endif
