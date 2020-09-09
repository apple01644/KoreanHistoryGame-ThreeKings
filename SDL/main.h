#pragma once

#include <codecvt>
#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include <SDL_opengl.h>
#include <GL\GLU.h>

#include <stdlib.h>
#include <stdarg.h>

#include <cmath>
#include <ctime>
#include <string>
#include <limits>
#include <locale>
#include <thread>
#include <mutex>
#include <algorithm>
#include <iterator>
#include <atomic>

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
#define Str std::to_wstring
#define Num std::stoi
#define Dmc std::stof
#pragma warning(disable:4100)
#pragma execution_character_set("utf-8")

typedef wchar_t wchar;



///////////////////////////
//       VARIABLE        //
///////////////////////////
extern const unsigned  int scr_w = 1920;
extern const unsigned  int scr_h = 1080;

std::atomic<double> map_x = 0;
std::atomic<double> map_y = 0;
std::atomic<double> map_p = 1;

unsigned int map_w = 1;
unsigned int map_h = 1;

float scr_r = scr_h / 1080.0;

extern const WCHAR ESCAPE = 0x1b;
extern std::atomic<bool> quit = false;
std::wstring executeDir;


static std::wstring_convert<std::codecvt_utf8<wchar_t>> conv{};

Uint32 STEP_NOW = 0;
Uint32 STEP_LAST = 0;
Uint32 TIME_STACK = 0;
Uint64 DRAW_NOW = 0;
Uint64 DRAW_LAST = 0;

unsigned int ALL_MAN = 0;
bool pause = false;


///////////////////////////
//     MAJOR CIRCLE      //
///////////////////////////
void start();
void step(SDL_Event*);
void draw();
void ui();
void run_ai();

bool init();
bool loadMedia();
void close();

///////////////////////////
//         DEBUG         //
///////////////////////////
void LOG_A(std::wstring);
void LOG_W(std::wstring);
void LOG_H(std::wstring);
void LOG_O(std::wstring);
void LOG_V(std::wstring);
void LOG_A(std::wstring, std::wstring);
void LOG_W(std::wstring, std::wstring);
void LOG_H(std::wstring, std::wstring);
void LOG_O(std::wstring, std::wstring);
void LOG_V(std::wstring, std::wstring);
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
SDL_GLContext GLCN;

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

std::unordered_map<std::wstring, SDL_Color> Color;
std::unordered_map<std::wstring, Gfx> gfx;
std::unordered_map<std::wstring, Tfx> tfx;
std::unordered_map<std::wstring, Mix_Music*> sfx;
std::unordered_map<std::wstring, size_t> gui_key;
std::unordered_map<std::wstring, std::wstring> script;
std::unordered_map<std::wstring, std::wstring> key;
std::map<size_t, unsigned int> map_reg;
std::map<std::pair<size_t, size_t>, unsigned int> map_connect;

struct Arg_Mouse {
	Sint32 x;
	Sint32 y;
	Uint8 button;
};

typedef std::function<void(size_t)> Action_Step;
typedef std::function<void(size_t, Arg_Mouse)> Action_mousedown;

enum Event_type {
	ev_none,
	ev_mousedown,
	ev_step
};

struct EventHandler {
	Action_mousedown Event_mousedown = NULL;
	Action_Step Event_step = NULL;
};

struct Event
{
	UINT8 type = ev_none;
	Action_Step step;
	Action_mousedown mousedown;
};

std::unordered_map <std::wstring, Event> event_key;

enum {
	wd_none,
	wd_image,
	wd_label,
	wd_text
};

class Widget {
public:
	Sint32 x = 0;
	Sint32 y = 0;
	Sint32 rx = 0;
	Sint32 ry = 0;
	Uint16 w = 0;
	Uint16 h = 0;
	size_t id = 0;
	size_t parent = 0;
	bool removing = false;
	unsigned char type = 0;
	bool enable = false;

	std::unordered_map<std::wstring, std::wstring> var;
	EventHandler ev;

	void remove();
	void change_key(std::wstring s);
	void init(Sint32 X, Sint32 Y, Uint16 W, Uint16 H, unsigned char Type, std::wstring s);
	Widget(Sint32 X, Sint32 Y, Uint16 W, Uint16 H, unsigned char Type, std::wstring s) {
		init(X, Y, W, H, Type, s);
		enable = true;
	}
	~Widget()
	{
		var.clear();
	};
};

std::vector<Widget> gui;
size_t focus;

void Widget::init(Sint32 X, Sint32 Y, Uint16 W, Uint16 H, unsigned char Type, std::wstring s)
{
	x = X;
	y = Y;
	w = W;
	h = H;
	type = Type;
	id = gui.size();
	parent = id;
	gui_key[s] = id;
	var[L"name"] = s;
}
void Widget::change_key(std::wstring s) {
	gui_key.erase(var[L"name"]);
	var[L"name"] = s;
	gui_key[s] = id;

};
void Widget::remove()
{
	gui.erase(gui.begin() + id);
};


struct ProvinceInfo {
	unsigned int man;
};
struct Province {
	size_t id = 0;
	unsigned int x1 = 256;
	unsigned int y1 = 256;
	unsigned int x2 = 0;
	unsigned int y2 = 0;
	unsigned int c = 0;

	float px = 0;
	float py = 0;
	unsigned int pnum = 0;
	
	bool waste_land = false;

	SDL_Texture* t = nullptr;;
	SDL_Texture* gt = nullptr;;
	SDL_Texture* lt = nullptr;;

	ProvinceInfo pi;
	std::wstring name = L"undefiend";
	std::wstring CON = L"NAV";
	std::wstring OWN = L"NAV";

	std::unordered_map<std::wstring, std::wstring> var;
	~Province()
	{
		var.clear();
	};
};
std::vector<Province> prv;

struct SpiderMap {
	size_t len = std::numeric_limits<size_t>::max();
	size_t nearest = std::numeric_limits<size_t>::max();
	bool outted = false;
	bool enable = true;
};

struct AiProvince {
	long attack = 0;
	bool enable = false;
};

enum {
	ntf_attack,
	ntf_diffence
};

struct Nation {
	unsigned int c = 0;
	double px = 0;
	double py = 0;
	double pw = 0;
	unsigned int pnum = 0;
	unsigned int focus = ntf_attack;
	unsigned int total_unit = 0;
	std::unordered_map<std::wstring, std::wstring> var;
	~Nation()
	{
		var.clear();
	};
};

std::unordered_map<std::wstring, Nation> nat;

struct Man {
	std::unordered_map<std::wstring, std::wstring> var;
	std::list<size_t> com;

	Man()
	{
		var[L"HP"] = L"800";
		var[L"LOC"] = L"0";
		var[L"power"] = L"0";
		var[L"HOME"] = L"0";
		var[L"live"] = L"1";
		var[L"CON"] = L"NAV";
		var[L"attack"] = Str(rand() % 4 + 3);
	};
	~Man()
	{
		var.clear();
	};
};

std::unordered_map<unsigned int, Man> man;

///////////////////////////
//       FUCNTIONAL      //
///////////////////////////

typedef std::function<void(std::wstring, std::wstring)> fn_str2;

std::ofstream logging("log.log");

