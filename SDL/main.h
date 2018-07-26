#pragma once

#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include <stdlib.h>

#include <cmath>
#include <ctime>
#include <string>
#include <limits>
#include <locale>
#include <thread>
#include <algorithm>
#include <iterator>

#include <vector>
#include <list>
#include <unordered_map>
#include <map>
#include <functional>

#include <windows.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <codecvt>
#include <filesystem>
#include <direct.h>

#define DEBUG
#define Str std::to_string
#define Num std::stoi
#define Dmc std::stof

#pragma execution_character_set("utf-8")

///////////////////////////
//       VARIABLE        //
///////////////////////////
extern const int scr_w = 1920;
extern const int scr_h = 1080;
float scr_r = scr_h / 1080.0;
extern const char ESCAPE = 0x1b;
extern bool quit = false;
std::string executeDir;


///////////////////////////
//     MAJOR CIRCLE      //
///////////////////////////
void start();
void step();
void draw();
void ui(SDL_Event*);

bool init();
bool loadMedia();
void close();

///////////////////////////
//         DEBUG         //
///////////////////////////
void LOG_A(std::string);
void LOG_W(std::string);
void LOG_H(std::string);
void LOG_O(std::string);
void LOG_V(std::string);
void LOG_A(std::string, std::string);
void LOG_W(std::string, std::string);
void LOG_H(std::string, std::string);
void LOG_O(std::string, std::string);
void LOG_V(std::string, std::string);
void LOG_Stop();

enum {
	left_align = 0,
	right_align = 1,
	center_align = 2,
	top_align = 3,
	bottom_align = 6,
	middle_align = 9
};

SDL_Window* WNDW = NULL;
SDL_Surface* SURF;
SDL_Renderer* REND;

///////////////////////////
//         LIST          //
///////////////////////////
struct Gfx
{
	SDL_Texture* t;
	SDL_Surface* s;
};
struct Tfx
{
	TTF_Font* t;
};

std::unordered_map<std::string, SDL_Color> Color;
std::unordered_map<std::string, Gfx> gfx;
std::unordered_map<std::string, Tfx> tfx;
std::unordered_map<std::string, Mix_Music*> sfx;
std::unordered_map<std::string, unsigned int> gui_key;
std::unordered_map<std::string, std::string> script;

enum {
	wd_none,
	wd_image,
	wd_label,
	wd_text
};


class Widget {
public:
	int x = 0;
	int y = 0;
	int rx = 0;
	int ry = 0;
	unsigned int w = 0;
	unsigned int h = 0;
	unsigned int id = 0;
	unsigned int parent = 0;
	bool removing = false;
	unsigned char type = 0;
	bool enable = false;

	std::unordered_map<std::string, std::string> var;

	void remove();
	void init(int X, int Y, unsigned int W, unsigned int H, unsigned char Type, std::string s);
	Widget(int X, int Y, unsigned int W, unsigned int H, unsigned char Type, std::string s) {
		init(X, Y, W, H, Type, s);
		enable = true;
	}
};

std::vector<Widget> gui;

void Widget::init(int X, int Y, unsigned int W, unsigned int H, unsigned char Type, std::string s)
{
	x = X;
	y = Y;
	w = W;
	h = H;
	type = Type;
	id = gui.size();
	parent = id;
	gui_key[s] = id;
	var["name"] = s;
}
void Widget::remove()
{
	gui.erase(gui.begin() + id);
};

struct Province {
	unsigned int x1 = 256;
	unsigned int y1 = 256;
	unsigned int x2 = 0;
	unsigned int y2 = 0;
	unsigned int c = 0;

	std::string name = "";

	bool enable = true;

	SDL_Texture* t;
};
std::vector<Province> prv;

///////////////////////////
//       FUCNTIONAL      //
///////////////////////////


typedef std::function<void(std::string, std::string)> fn_str2;
