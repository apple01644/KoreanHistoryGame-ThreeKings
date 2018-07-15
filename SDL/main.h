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
#include <ctime>
#include <thread>
#include <list>
#include <direct.h>
#include <filesystem>
#include <locale>
#include <io.h>
#include <fcntl.h>
#include <unordered_map>
#include <map>
#include <limits>

#pragma execution_character_set("utf-8")

extern const int scr_w = 1920;
extern const int scr_h = 1080;
extern const int MAX_SPRITE = 32;
extern const int MAX_MAP = 1;
extern const int MAX_PARTY = 9;
extern const int MAX_FONT = 3;
extern const int MAX_PROV = 17;
extern const char ESCAPE = 0x1b;
int NUM_SPR = 0;

void start();
void step();
void draw();
void ui(SDL_Event*);

bool init();
bool loadMedia();
void safe_start();
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

int get_chrs(std::string);
void draw_string_s(int, std::string, SDL_Color, SDL_Rect*);
void draw_string(int, std::string, SDL_Color, SDL_Point*, int, float,int);

std::unordered_map<unsigned long, bool> char_id;
enum {
	left_align = 0,
	right_align = 1,
	center_align = 2,
	top_align = 3,
	bottom_align = 6,
	middle_align = 9
};

std::string currentDir;

extern std::string font_file[MAX_FONT] = {
	"ttf\\NanumBarunGothicLight.ttf",
	"ttf\\NanumBrush.ttf",
	"ttf\\NanumBarunGothicBold.ttf"
};
extern const unsigned int font_size[MAX_FONT] = {
	72,
	72,
	72
};
extern TTF_Font* fonts[MAX_FONT] = {};

SDL_Window* WNDW = NULL;
SDL_Surface* SURF;
SDL_Renderer* REND;


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
std::unordered_map<std::string, Mix_Music*> sfx;

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

struct Man {
	std::string name = "이름없음";
	unsigned int born_year = 0;
	unsigned long id = 0;
	bool live = false;
	long money = 0;
	long prestige = 0;
	char fascist = rand();
	char liberty = rand();
	unsigned char honor = rand();
	unsigned char ambition = rand();
	unsigned char religion = rand();
	std::unordered_map<std::string, bool> traits;
	void set(unsigned int year, std::string named, long moneye, char p_fasc, char p_libe, unsigned char hon, unsigned char amb, unsigned char reg)
	{
		if (born_year == 0)
		{
			for (long int i = 1; i < 32767; i++)
			{
				if (!char_id[i])
				{
					char_id[i] = true;
					id = i;
					break;
				}
			}
			born_year = year;
			name = named;
			money = moneye;
			fascist = p_fasc;
			liberty = p_libe;
			honor = hon;
			ambition = amb;
			religion = reg;
			live = true;
		}
		else
		{
			LOG_W("MUTIPLE DEFINE CHARACTER");
		}
	}
};


struct Media{
	unsigned long owner = 0;
	std::string name;
	long money = 0;
	unsigned long power = 0;
};

struct Force{
	unsigned long owner = 0;
	std::string name;
	long money = 0;
	unsigned long power = 0;
	char good = 0;

};

struct Company{
	unsigned long owner = 0;
	std::string name;
	long money = 0;
	unsigned long power = 0;
};

struct Education{
	unsigned long owner = 0;
	std::string name;
	long money = 0;
	unsigned long power = 0;
	unsigned int type = 0;

};

enum {
	wd_none,
	wd_image
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

	unsigned char type = wd_none;

	std::unordered_map<std::string, std::string> var;

	void(*ev)(int, int, int);
	bool avail_ev = false;
	void move_left();
	void remove();
};

std::vector<Widget> gui;

void Widget::move_left()
{

};
void Widget::remove()
{
	gui.erase(gui.begin() + id);
	//L//OG_W("1");
};

enum { msgtype_None,
msgType_Imagebox
};

struct Message
{
	unsigned int type = msgtype_None;
};

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
int tmp_s[16];
std::vector<std::string> scope;
std::unordered_map<std::string, std::string> keys;
std::list<Man> man;
std::list<Message> msg;
std::list<Media> media;
std::list<Force> force;
std::list<Company> company;
std::list<Education> education;

unsigned long play_id = 1;
bool play_av = false;
std::list<Man>::iterator player = man.begin();
