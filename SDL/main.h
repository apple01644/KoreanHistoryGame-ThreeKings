#pragma once
#pragma warning(disable:4244)
#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdlib.h>
#include <string>
#include <windows.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <ctime>`
#include <thread>
#include <list>
#include <direct.h>
#include <filesystem>
#include <locale>
#include <io.h>
#include <fcntl.h>
#include <unordered_map>

#pragma execution_character_set("utf-8")

extern const int scr_w = 1920;
extern const int scr_h = 1080;
extern const int MAX_SPRITE = 32;
extern const int MAX_MAP = 1;
extern const int MAX_PARTY = 9;
extern const int MAX_FONT = 2;
extern const int MAX_PROV = 17;
extern const char ESCAPE = 0x1b;
int NUM_SPR = 0;

void start();
void step();
void draw();
void ui(SDL_Event*);

bool init();
bool loadMedia();
void close();

void LOG_A(std::string);
void LOG_W(std::string);
void LOG_H(std::string);
void LOG_O(std::string);
void LOG_A(std::string, std::string);
void LOG_W(std::string, std::string);
void LOG_H(std::string, std::string);
void LOG_O(std::string, std::string);
void LOG_Stop();

void set_rect(SDL_Rect*, int, int, int, int);
void set_rect(SDL_Rect*, int, int);

void draw_string(int, std::string, SDL_Color, SDL_Rect*);

std::string currentDir;

extern std::string font_file[MAX_FONT] = {
	"ttf\\NanumBarunGothicLight.ttf",
	"ttf\\NanumBrush.ttf"
};
extern const unsigned int font_size[MAX_FONT] = {
	30,
	20
};
extern TTF_Font* fonts[MAX_FONT] = {};

SDL_Window* WNDW = NULL;
SDL_Surface* SURF;
SDL_Renderer* REND;

extern Mix_Music *gMusic = NULL;

///////////////////////////
//         COLOR         //
///////////////////////////

extern Uint32 color(int, int, int);
extern SDL_Color c_black = { 0, 0, 0 };
extern SDL_Color c_white = { 255, 255, 255 };






///////////////////////////
//       IMPORTANT       //
///////////////////////////

struct Gfx
{
	SDL_Texture* t;
	SDL_Surface* s;
};

std::unordered_map<std::string, Gfx> gfx;

int max_sprite = 1;

struct Pop
{
	unsigned support = rand() % MAX_PROV;
	unsigned issue = 0;
	unsigned income = 0;
};

struct Province
{
	unsigned int x1 = 256;
	unsigned int y1 = 256;
	unsigned int x2 = 0;
	unsigned int y2 = 0;
	unsigned int c = 0;

	std::string name = "";

	unsigned int eco = 0;
	unsigned int ind = 0;
	unsigned int clt = 0;
	unsigned int man = 0;

	std::vector<Pop> pop;

	SDL_Texture* t;
};
Province prv[MAX_PROV];

struct Party
{
	char facism = 0;
	char liberty = 0;
	std::string name = "";
	unsigned int c = 0;
};
Party party[MAX_PARTY];


///////////////////////////
//         USER          //
///////////////////////////

int year = 2002;
int mon = 4;
int day = 17;
extern bool quit = false;
int map_mode = 1;
unsigned long long delay = 0;
int tmp[16];
std::vector<std::string> scope;
std::unordered_map<std::string, std::string> keys;