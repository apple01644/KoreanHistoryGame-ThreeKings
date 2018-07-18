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
#include <locale>
#include <limits>

#pragma execution_character_set("utf-8")

extern const int scr_w = 1920;
extern const int scr_h = 1080;
float scr_r = scr_h / 1080.0;
extern const int MAX_SPRITE = 32;
extern const int MAX_MAP = 1;
extern const int MAX_PARTY = 9;
extern const int MAX_FONT = 4;
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
int get_lines(std::string, int);
void draw_string_s(int, std::string, SDL_Color, SDL_Rect*);
void draw_string(int, std::string, SDL_Color, SDL_Point*, int, float,int);
void draw_line(int, std::string, int, SDL_Color, SDL_Point*, int, float, int);
std::unordered_map<unsigned long, bool> char_id;
std::unordered_map<std::string, long> stats;
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
	"ttf\\NanumBarunGothicBold.ttf",
	"ttf\\NanumBarunGothic.ttf"
};
extern const unsigned int font_size[MAX_FONT] = {
	72,
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
std::unordered_map<std::string, SDL_Color> Color;
void set_color()
{
	Color["c_white"] = { 255, 255, 255 };
	Color["c_black"] = { 0, 0, 0 };
	Color["c_green"] = { 0, 170, 0 };
	Color["c_red"] = { 170, 0, 0 };
	Color["c_yellow"] = { 170, 170, 0 };
}

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
std::vector<std::string> scope;
std::unordered_map<std::string, std::string> keys;
std::unordered_map<std::string, int> ikeys;
std::unordered_map<std::string, signed long> relation;

int max_sprite = 1;

struct Pop
{
	unsigned support = rand() % 5;
	unsigned issue = 0;
	unsigned income = 0;
	char fascist = rand() % 256 - 128;
	char liberty = rand() % 256 - 128;
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

	bool enable = true;

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
	unsigned long owner = 0;
};
Party party[MAX_PARTY];

struct Man {
	std::string name = "이름없음";
	std::string potrait = "potrait\\cjy";
	std::string work = "None";

	unsigned int born_year = 0;
	unsigned long id = 0;
	bool live = false;
	long money = 0;
	long prestige = 0;
	char fascist = rand() % 256 - 128;
	char liberty = rand() % 256 - 128;
	char party = -1;
	char charge = rand() % 64;
	unsigned char honor = rand() % 256;
	unsigned char ambition = rand() % 256;
	unsigned char religion = rand() % 256;
	std::unordered_map<std::string, bool> traits;
	void set(unsigned int year, std::string named, long moneye, char p_fasc, char p_libe, unsigned char hon, unsigned char amb, unsigned char reg, std::string c)
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
			potrait = c;
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
	unsigned char good = 0;
};

struct Force{
	unsigned long owner = 0;
	std::string name;
	long money = 0;
	unsigned long power = 0;
	unsigned char good = 0;

};

struct Company{
	unsigned long owner = rand() % 30 + 1;
	std::string name;
	long money = 0;
	unsigned long power = 0;
	unsigned char good = 0;
};

struct Education{
	unsigned long owner = 0;
	std::string name;
	long money = 0;
	unsigned long power = 0;
	unsigned char good = 0;

};

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
	unsigned char type = wd_none;
	bool enable = true;

	std::unordered_map<std::string, std::string> var;

	void(*mousedown_ev)(int, int, int);
	bool avail_mousedown_ev = false;

	void(*mousehover_ev)(int, int, int);
	bool avail_mousehover_ev = false;

	void(*mouseup_ev)(int, int, int);
	bool avail_mouseup_ev = false;

	void(*mousestep_ev)(int, int, int);
	bool avail_mousestep_ev = false;

	void(*mousewheel_ev)(int, int, int, int);
	bool avail_mousewheel_ev = false;

	void(*step_ev)(int);
	bool avail_step_ev = false;

	void move_left();
	void remove();
	void init(int X, int Y, unsigned int W, unsigned int H, unsigned char Type, std::string s);
	Widget(int X, int Y, unsigned int W, unsigned int H, unsigned char Type, std::string s) {
		init(X, Y, W, H, Type, s);
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
	var["name"] = s;
	ikeys[s] = id;
}
void Widget::move_left()
{

};
void Widget::remove()
{
	ikeys.erase((gui.begin() + id)->var["name"]);
	gui.erase(gui.begin() + id);
	//L//OG_W("1");
};

struct Message
{
	unsigned long sender = 0;
	std::string s;
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
unsigned int game_speed = 5;

std::list<Man> man;

std::list<Message> msg;
std::list<Media> media;
std::list<Force> force;
std::list<Company> company;

std::list<Education> education;

bool play_av = false;

unsigned int president_id;
unsigned int prime_id;
unsigned int congress_id;
unsigned int play_id = 1;

void pop_char(int, int,int);
void pop_comp(int, int);
void pop_comp_info(int, int);
void pop_gove(int, int);
void pop_gun(int, int);
void pop_media(int, int);
void pop_law(int, int);
void pop_cong(int, int);
void pop_edu(int, int);
void pop_party(int, int);
void pop_prov(int, int);
void pop_speech(int, int, int, int);

void normal_start();
int mother(const int i)
{
	if ((gui.begin() + i)->id == (gui.begin() + i)->parent)
	{
		return (gui.begin() + i)->id;
	}
	else
	{
		return mother((gui.begin() + i)->parent);
	}
}
std::string get_ideology(char f, char l)
{
	if (f > 60)
	{
		if (l > 60)
		{
			return "전체주의";
		}
		else if (l > 20)
		{
			return "국수주의";
		}
		else if (l > -20)
		{
			return "국가사회주의";
		}
		else if (l > -60)
		{
			return "공산주의";
		}
		else
		{
			return "왕정주의";
		}
	}
	else if (f > 20)
	{
		if (l > 60)
		{
			return "근본주의";
		}
		else if (l > 20)
		{
			return "보수주의";
		}
		else if (l > -20)
		{
			return "과두주의";
		}
		else if (l > -60)
		{
			return "스탈린주의";
		}
		else
		{
			return "사회주의";
		}
	}
	else if (f > -20)
	{
		if (l > 60)
		{
			return "자유주의";
		}
		else if (l > 20)
		{
			return "자유보수주의";
		}
		else if (l > -20)
		{
			return "중도주의";
		}
		else if (l > -60)
		{
			return "신자유주의";
		}
		else
		{
			return "사회민주주의";
		}
	}
	else if (f > -60)
	{
		if (l > 60)
		{
			return "자본주의";
		}
		else if (l > 20)
		{
			return "봉건주의";
		}
		else if (l > -20)
		{
			return "의회주의";
		}
		else if (l > -60)
		{
			return "집산주의";
		}
		else
		{
			return "무정부사회주의";
		}
	}
	else
	{
		if (l > 60)
		{
			return "무정부주의";
		}
		else if (l > 20)
		{
			return "대중주의";
		}
		else if (l > -20)
		{
			return "민주주의";
		}
		else if (l > -60)
		{
			return "노조주의";
		}
		else
		{
			return "무정부공산주의";
		}
	}
}

std::string get_case(std::string s)
{
	if (s == "None")
	{
		return "ui\\potrait_case";
	}
	else if (s == "국회의원" || s == "국회의장")
	{
		return "ui\\potrait_case_gold_cong";
	}
	else if (s == "대통령")
	{
		return "ui\\potrait_case_blue";
	}
	else if (s == "시장" || s == "도지사" || s == "총리")
	{
		return "ui\\potrait_case_gold";
	}
	else if (s == "기업가")
	{
		return "ui\\potrait_case_company";
	}
	else if (s == "건달")
	{
		return "ui\\potrait_case_gun";
	}
	else if (s == "판사")
	{
		return "ui\\potrait_case_law";
	}
	else if (s == "기자")
	{
		return "ui\\potrait_case_media";
	}
	else
	{
		return "ui\\potrait_case";
	}
}
void msg_push(unsigned long id, std::string s)
{
	Message m;
	m.s = s;
	m.sender = id;
	msg.push_front(m);
}
std::string now_time() {
	return std::to_string(year)+"년 "+std::to_string(mon)+"월 " +std::to_string(day) + "일";
}
std::string empty_gui_slot(std::string);
std::locale loc;
bool mon_on = true;