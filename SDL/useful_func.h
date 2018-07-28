#pragma once
#include"action.h"

int get_lines(std::string s, int max_line)
{
	int lines = 0;
	const size_t chr_len = s.length();
	const char* chr = s.c_str();
	int chrs = 0, bias = 0;
	for (int i = 0; i < chr_len;)
	{
		if (chr[i] >= 0)
		{
			if (chr[i] == '\n')
			{
				lines++;
			}
			bias = 1;
		}
		else if (chr[i] >= -64 && chr[i] <= -33 && chr[i + 1] <= -65)//-128 ~ - 65
		{
			bias = 2;
		}
		else if (chr[i] >= -32 && chr[i] <= -17 && chr[i + 1] <= -65 && chr[i + 2] <= -65)
		{
			bias = 3;
		}
		else if (chr[i] >= -16 && chr[i] <= -9 && chr[i + 1] <= -65 && chr[i + 2] <= -65 && chr[i + 3] <= -65)
		{
			bias = 4;
		}
		chrs++;

		if (max_line > 0 && chrs >= max_line)
		{
			lines++;
			chrs = 0;
		}

		i += bias;
	}
	if (chrs > 0)
	{
		lines++;
	}
	return lines;
}
int get_chrs(std::string s)
{
	const size_t chr_len = s.length();
	const char* chr = s.c_str();
	int chrs = 0, bias = 1;
	for (int i = 0; i < chr_len;)
	{
		if (chr[i] >= 0)
		{
			bias = 1;
		}
		else if (chr[i] >= -64 && chr[i] <= -33 && chr[i + 1] <= -65)//-128 ~ - 65
		{
			bias = 2;
		}
		else if (chr[i] >= -32 && chr[i] <= -17 && chr[i + 1] <= -65 && chr[i + 2] <= -65)
		{
			bias = 3;
		}
		else if (chr[i] >= -16 && chr[i] <= -9 && chr[i + 1] <= -65 && chr[i + 2] <= -65 && chr[i + 3] <= -65)
		{
			bias = 4;
		}
		chrs++;
		i += bias;
	}
	return chrs;
}
void draw_string_s(std::string i, std::string s, SDL_Color color, SDL_Rect* r) {
	
	SDL_Surface* surf = TTF_RenderUTF8_Solid(tfx[i].t, s.c_str(), color);
	SDL_Texture* t = SDL_CreateTextureFromSurface(REND, surf);
	SDL_FreeSurface(surf);
	SDL_RenderCopy(REND, t, NULL, r);
	SDL_DestroyTexture(t);
}
void draw_string(std::string ind, std::string s, std::string color, SDL_Point* p, int size, float ratio, int opt)
{
	if (s.length() == 0 || size == 0)
	{
		return;
	}

	SDL_Rect r;
	r.x = p->x;
	r.y = p->y;
	r.w = (unsigned int)(get_chrs(s) * size * ratio);
	r.h = size;

	
	{
		switch (opt % 3)
		{
		case right_align:
			r.x -= r.w;
			break;
		case center_align:
			r.x -= r.w / 2;
			break;
		}
		switch (opt / 3)
		{
		case bottom_align / 3:
			r.y -= r.h;
			break;
		case middle_align / 3:
			r.y -= r.h / 2;
			break;
		}
	}

	draw_string_s(ind, s, Color[color], &r);
}
void draw_line(std::string ind, std::string s, int max_line, std::string color, SDL_Point* p, int size, float ratio, int opt)
{
	int lines = 0;
	const size_t chr_len = s.length();
	const char* chr = s.c_str();
	int i, chrs = 0, bias = 0, st = 0;
	for (i = 0; i < chr_len;)
	{
		bias = 1;
		if (chr[i] >= 0)
		{
		}
		else if (chr[i] >= -64 && chr[i] <= -33 && chr[i + 1] <= -65)//-128 ~ - 65
		{
			bias = 2;
		}
		else if (chr[i] >= -32 && chr[i] <= -17 && chr[i + 1] <= -65 && chr[i + 2] <= -65)
		{
			bias = 3;
		}
		else if (chr[i] >= -16 && chr[i] <= -9 && chr[i + 1] <= -65 && chr[i + 2] <= -65 && chr[i + 3] <= -65)
		{
			bias = 4;
		}
		else
		{
			break;
		}
		chrs++;
		if ((max_line > 0 && chrs >= max_line) || chr[i] == '\n')
		{
			lines++;

			draw_string(ind, s.substr(st, i - st), color, p, size, ratio, opt);
			p->y += size;
			st = i;
			chrs = 0;
			if (chr[i] == '\n') {
				st++;
			}
		}

		i += bias;
	}
	if (chrs > 0)
	{
		lines++;
		draw_string(ind, s.substr(st, i - st), color, p, size, ratio, opt);
	}
}

template<typename T, typename T2>
void set_rect(SDL_Rect *r, T x, T y, T2 w, T2 h)
{
	r->x = (int)x;
	r->y = (int)y;
	r->w = (unsigned int)w;
	r->h = (unsigned int)h;
}
template<typename T>
void set_point(SDL_Rect *r, T x, T y)
{
	r->x = (int)x;
	r->y = (int)y;
}
template<typename T>
void set_point(SDL_Point *p, T x, T y)
{
	p->x = (int)x;
	p->y = (int)y;
}

Uint32 color(int r, int g, int b)
{
	return r * 65536 + g * 256 + b;
}

void gui_remove(const unsigned int id)
{
	for (unsigned int a = 0; a < gui.size(); a++)
	{
		if (a != id && gui.at(a).parent == id)
		{
			gui.at(a).removing = true;
		}
	}
	gui.at(id).remove();
	for (unsigned int a = id; a < gui.size(); a++)
	{
		for (int b = 0; b < gui.size(); b++)
		{
			if (gui.at(b).parent == gui.at(a).id)
			{
				gui.at(b).parent -= 1;
			}
		}
		gui.at(a).id -= 1;
		gui_key[gui.at(a).var["name"]] -= 1;
	}
}

std::string Var(std::string s) {
	if (*s.begin() == '"' && *s.rbegin() == '"')
	{
		return s.substr(1, s.size() - 2);
	}
	return "";
}

void incode_define(std::string s)
{
	bool str = false;
	for (auto I = s.begin(); I != s.end(); ++I)
	{
		if (*I == '"')
		{
			str = !str;
			*I = '\a';
			continue;
		}
		if (!str && (*I == '\n' || *I == '\t' || *I == '\v' || *I == '\b' || *I == '\r' || *I == '\f' || *I == '\a' || *I == ' '))
		{
			*I = '\a';
			continue;
		}
	}
	s.erase(std::remove(s.begin(), s.end(), '\a'), s.end());

	std::string s1 = "";
	std::string s2 = "";
	std::string S = "";
	std::vector <std::string> scope;
	std::string name = "";
	unsigned int begin = 0;

	for (int i = 0; i < s.size(); ++i)
	{
		if (s[i] == '{')
		{
			scope.push_back(s.substr(begin, i - begin));
			begin = i + 1;
			continue;
		}
		if (s[i] == '}')
		{
			S = s.substr(begin, i - begin);
			for (int a = 0; a < S.size(); ++a)
			{
				if (S[a] == '=')
				{
					LOG_V("script["+ name + "." + S.substr(0, a) +"]", S.substr(a + 1));
					script[name + "." + S.substr(0, a)] = S.substr(a + 1);
					break;
				}
			}
			begin = i + 1;
			scope.pop_back();
			continue;
		}
		name = "";
		for (int a = 0; a < scope.size(); ++a)
		{
			name += "." + scope.at(a);
		}

		if (s[i] == '[')
		{
			s1 = s.substr(begin, i - begin - 1);
			begin = i + 1;
			++i;
			int index = 0;
			for (; s[i] != ']'; ++i)
			{
				if (s[i] == ',')
				{
					LOG_V("script["+ name + s1 + "[" + Str(index) + "]" +"]", s2);
					script[name + s1 + "[" + Str(index) + "]"] = s2;
					index++;
					s2 = "";
				}
				else if (s[i + 1] == ']')
				{
					s2 += s[i];
					LOG_V("script[" + name + s1 + "[" + Str(index) + "]" + "]", s2);
					script[name + s1 + "[" + Str(index) + "]"] = s2;
					index++;
					s2 = "";
				}
				else
				{
					s2 += s[i];
				}
			}
			continue;
		}

		if (s[i] == ',')
		{
			S = s.substr(begin, i - begin);
			
			for (int a = 0; a < S.size(); ++a)
			{
				if (S[a] == '=')
				{
					LOG_V("script[" + name + "." + S.substr(0, a) +"]", S.substr(a + 1));
					script[name + "." + S.substr(0, a)] = S.substr(a + 1);
					break;
				}
			}
			
			begin = i + 1;
			continue;
		}
		if (s[i] == '=' && name.empty())
		{
			LOG_V("script[" + s.substr(0, i) + "]", s.substr(i + 1));
			script[s.substr(0, i)] = s.substr(i + 1);
			continue;
		}
	}
}

void incode_prov(std::string s)
{
	std::string s1 = "";
	std::string s2 = "";
	bool str = false;
	for (auto I = s.begin(); I != s.end(); ++I)
	{
		if (*I == '"')
		{
			str = !str;
			*I = '\a';
			continue;
		}
		if (!str && (*I == '\n' || *I == '\t' || *I == '\v' || *I == '\b' || *I == '\r' || *I == '\f' || *I == '\a' || *I == ' '))
		{
			*I = '\a';
			continue;
		}
	}
	s.erase(std::remove(s.begin(), s.end(), '\a'), s.end());
	for (unsigned int i = 0; i < s.size(); ++i)
	{
		if (s.at(i) == '=')
		{
			s2 = s.substr(0, i);
			s1 = s.substr(i + 1);
			int k = 0;
			key["argc"] = "0";
			key["func"] == "";
			for (unsigned int j = 0; j < s1.size(); j++)
			{
				if (s1.at(j) == '&')
				{
					key["func"] = s1.substr(k, j - k);
					k = j + 1;
				}
				if (s1.at(j) == ',' || j == s1.size() - 1)
				{
					if (j == s1.size() - 1)
					{
						key["argv[" + key["argc"] + "]"] = s1.substr(k);
					}
					else
					{
						key["argv[" + key["argc"] + "]"] = s1.substr(k, j - k);
						k = j + 1;
					}
					key["argc"] = Str(Num(key["argc"]) + 1);
				}
			}
			while (Num(s2) + 1 > prv.size())
			{
				Province prov;
				prv.push_back(prov);
			}
			LOG_O("Province Color Defined", s2);
			if (key["func"] == "color")
			{
				prv.at(Num(s2)).c = color(Num(key["argv[0]"]), Num(key["argv[1]"]), Num(key["argv[2]"]));
			}
		}
	}
}
void incode_prv(unsigned int *sec, std::string s)
{
	std::string s1 = "";
	std::string s2 = "";
	bool str = false;
	for (auto I = s.begin(); I != s.end(); ++I)
	{
		if (*I == '"')
		{
			str = !str;
			*I = '\a';
			continue;
		}
		if (!str && (*I == '\n' || *I == '\t' || *I == '\v' || *I == '\b' || *I == '\r' || *I == '\f' || *I == '\a' || *I == ' '))
		{
			*I = '\a';
			continue;
		}
	}
	s.erase(std::remove(s.begin(), s.end(), '\a'), s.end());
	for (unsigned int i = 0; i < s.size(); ++i)
	{
		if (s.at(i) == '=')
		{
			s2 = s.substr(0, i);
			s1 = s.substr(i + 1);
			if (s2 == "select")
			{
				*sec = Num(s1);
				continue;
			}
			if (s2 == "waste_land")
			{
				if (s1 == "false")
				{
					prv.at(*sec).waste_land = false;
				}
				else if (s1 == "true")
				{
					prv.at(*sec).waste_land = true;
				}
				continue;
			}
			prv.at(*sec).var[s2] = s1;
		}
	}
}
void incode_nat(std::string *sec, std::string s)
{
	std::string s1 = "";
	std::string s2 = "";
	bool str = false;
	for (auto I = s.begin(); I != s.end(); ++I)
	{
		if (*I == '"')
		{
			str = !str;
			*I = '\a';
			continue;
		}
		if (!str && (*I == '\n' || *I == '\t' || *I == '\v' || *I == '\b' || *I == '\r' || *I == '\f' || *I == '\a' || *I == ' '))
		{
			*I = '\a';
			continue;
		}
	}
	s.erase(std::remove(s.begin(), s.end(), '\a'), s.end());
	for (unsigned int i = 0; i < s.size(); ++i)
	{
		if (s.at(i) == '=')
		{
			s2 = s.substr(0, i);
			s1 = s.substr(i + 1);
			
			
			key["argc"] = "0";
			key["func"] = "";
			unsigned int k = 0;
			for (unsigned int j = 0; j < s1.size(); j++)
			{
				if (s1.at(j) == '&')
				{
					key["func"] = s1.substr(k, j - k);
					k = j + 1;
				}
				if (s1.at(j) == ',' || j == s1.size() - 1)
				{
					if (j == s1.size() - 1)
					{
						key["argv[" + key["argc"] + "]"] = s1.substr(k);
					}
					else
					{
						key["argv[" + key["argc"] + "]"] = s1.substr(k, j - k);
						k = j + 1;
					}
					key["argc"] = Str(Num(key["argc"]) + 1);
				}
			}
			if (key["func"] == "color")
			{
				s1 = Str(color(Num(key["argv[0]"]), Num(key["argv[1]"]), Num(key["argv[2]"])));
			}

			if (s2 == "name")
			{
				Nation natn;
				nat[s1] = natn;
				*sec = s1;
				break;
			}
			if (s2 == "color")
			{
				nat[*sec].c = Num(s1);
				break;
			}
		}
	}
}
void incode_ui(Widget* wd_p, std::string s1, std::string s2)
{
	if (s1 == "x")
	{
		wd_p->x = Num(Var(s2));
		return;
	}
	if (s1 == "y")
	{
		wd_p->y = Num(Var(s2));
		return;
	}
	if (s1 == "w")
	{
		wd_p->w = Num(Var(s2));
		return;
	}
	if (s1 == "h")
	{
		wd_p->h = Num(Var(s2));
		return;
	}
	if (s1 == "id")
	{
		wd_p->change_key(s2);
		return;
	}
	wd_p->var[s1] = Var(s2);
}

void read_as_define(std::string s)
{
	std::cout << s;
	std::string s2;
	auto J = s.begin();
	bool comment = false;
	for (auto I = s.begin(); I != s.end(); ++I)
	{
		if (comment)
		{
			if (*I == '\n')
			{
				comment = false;
				J = I + 1;
			}
		}
		else if (*I == ';')
		{
			incode_define(s2.assign(J, I));
			J = I + 1;
			continue;
		}
		else if (*I == '#')
		{
			comment = true;
		}
	}
}
void read_as_prov(std::string s)
{
	std::string s2;
	auto J = s.begin();
	bool comment = false;

	for (auto I = s.begin(); I != s.end(); ++I)
	{
		if (comment )
		{
			if (*I == '\n')
			{
				comment = false;
				J = I + 1;
			}
			continue;
		}
		if (*I == ';')
		{
			incode_prov(s2.assign(J, I));
			J = I + 1;
			continue;
		}
		if (*I == 35)
		{
			comment = true;
			continue;
		}
		
	}
}
void read_as_prv(std::string s)
{
	std::string s2;
	auto J = s.begin();
	bool comment = false;
	auto P = prv.end();
	unsigned int sec = prv.size();
	for (auto I = s.begin(); I != s.end(); ++I)
	{
		if (comment)
		{
			if (*I == '\n')
			{
				comment = false;
				J = I + 1;
			}
			continue;
		}
		if (*I == ';')
		{
			incode_prv(&sec, s2.assign(J, I));
			J = I + 1;
			continue;
		}
		if (*I == 35)
		{
			comment = true;
			continue;
		}

	}
}
void read_as_nat(std::string s)
{
	std::string s2;
	auto J = s.begin();
	bool comment = false;
	auto P = prv.end();
	std::string sec = "NAV";
	for (auto I = s.begin(); I != s.end(); ++I)
	{
		if (comment)
		{
			if (*I == '\n')
			{
				comment = false;
				J = I + 1;
			}
			continue;
		}
		if (*I == ';')
		{
			incode_nat(&sec, s2.assign(J, I));
			J = I + 1;
			continue;
		}
		if (*I == 35)
		{
			comment = true;
			continue;
		}

	}
}
void read_as_ui(std::string s)
{
	std::string s2;
	std::string s3;
	std::vector<std::string> scope;
	unsigned int begin = 0;
	bool comment = false;

	auto I = s.begin();
	for (unsigned int i = 0; i < s.size(); ++i)
	{
		if (comment)
		{
			if (s.at(i) == '"')
			{
				comment = false;
			}
		}
		else if (s.at(i) == '"')
		{
			comment = true;
			continue;
		}
		if (s.at(i) == '<')
		{
			begin = i + 1;
			for (;; i++)
			{
				if (comment)
				{
					if (s.at(i) == '"')
					{
						comment = false;
					}
					continue;
				}
				if (s.at(i) == '"')
				{
					comment = true;
					continue;
				}
				if (s.at(i) == '>')
				{
					s2 = s.substr(begin, i - begin);
					if (*(s2.begin()) == '/')
					{
						if (scope.begin() != scope.end())
						{
							scope.pop_back();
						}
						else
						{
							LOG_W("UI PARENT ERROR!");
							quit = true;
						}
					}
					else
					{
						Widget wd(0,0,20,20,wd_none,Str(gui.size()));
						
						begin = 0;
						for (unsigned int j = 0; j < s2.size(); ++j)
						{
							if (comment)
							{
								if (s2.at(j) == '"')
								{
									comment = false;
								}
								else
								{
									continue;
								}
							}
							else if (s2.at(j) == '"')
							{
								comment = true;
								continue;
							}
							if (s2.at(j) == ' ' || j + 1 == s2.size())
							{
								if (j + 1 == s2.size())
								{
									j++;
								}
								s3 = s2.substr(begin, j - begin);
								if (begin == 0)
								{
									do {
										if (s3 == "image")
										{
											wd.type = wd_image;
											break;
										}
										if (s3 == "label")
										{
											wd.type = wd_label;
											break;
										}
										if (s3 == "text")
										{
											wd.type = wd_text;
											break;
										}
									} while (false);
								}
								else
								{
									for (int k = 0; k < s3.size(); ++k)
									{
										if (s3.at(k) == '=')
										{
											incode_ui(&wd, s3.substr(0, k), s3.substr(k + 1, s3.size() - 1));
											break;
										}
									}
								}
								begin = j + 1;
								continue;
							}
						}
							
						if (scope.rbegin() != scope.rend())
						{
							wd.parent = gui.at(gui_key[*scope.rbegin()]).id;
						}

						if (*s2.rbegin() != '/')
						{
							scope.push_back(wd.var["name"]);
							
						}
						gui.push_back(wd);

					}

					break;
				}
				if (i == s.size())
				{
					LOG_W("UI SYNTAX ERROR","Can't find Closed Bracket");
					quit = true;
					break;
				}
			}
		}
	}
	if (comment)
	{
		LOG_W("UI Syntax Error","Cant Find EOF");
		quit = true;
	}
}

void read_gfx(const std::string path, const std::string tag)
{
	if (path.find_last_of(".") != std::string::npos && path.find_last_of("\\") != std::string::npos)
	{
		std::string name = tag + path.substr(path.find_last_of("\\") + 1, path.find_last_of(".") - path.find_last_of("\\") - 1);
		gfx[name].s = IMG_Load(path.c_str());
		SDL_SetColorKey(gfx[name].s, SDL_TRUE, color(255,0,255));
		if (gfx[name].s == NULL)
		{
			LOG_W("FAIL TO SURFACE", path);
		}
		else
		{
			gfx[name].t = SDL_CreateTextureFromSurface(REND, gfx[name].s);
			if (gfx[name].t == NULL)
			{
				LOG_W("SURFACE TO TEXTURE", path);
			}
			else
			{
				LOG_O("SUCCESS READ IMAGE FILE(" + name + ")", path);
			}
		}
	}
	else
	{
		LOG_W("FILE NAME ERROR", path);
		quit = true;
	}
}
void read_sfx(const std::string path, const std::string tag)
{
	if (path.find_last_of(".") != std::string::npos && path.find_last_of("\\") != std::string::npos)
	{
		std::string name = path.substr(path.find_last_of("\\") + 1, path.find_last_of(".") - path.find_last_of("\\") - 1);
		sfx[name] = Mix_LoadMUS(path.c_str());
		if (sfx[name] == NULL)
		{
			LOG_W("FAIL TO SOUND", path);
			quit = true;
		}
		else
		{
			LOG_O("SUCCESS READ SOUND FILE", path);
		}
	}
	else
	{
		LOG_W("FILE NAME ERROR", path);
		quit = true;
	}
}
void read_tfx(const std::string path, const std::string tag)
{
	if (path.find_last_of(".") != std::string::npos && path.find_last_of("\\") != std::string::npos)
	{
		std::string name = tag + path.substr(path.find_last_of("\\") + 1, path.find_last_of(".") - path.find_last_of("\\") - 1);

		tfx[name].t = TTF_OpenFont(path.c_str(), 144);

		if (tfx[name].t == NULL)
		{
			LOG_W("FAIL TO FONT", path);
		}
		else
		{
			LOG_O("SUCCESS READ FONT FILE(" + name + ")", path);
		}
	}
	else
	{
		LOG_W("FILE NAME ERROR", path);
		quit = true;
	}
}

void read_ui(const std::string path,const std::string tag)
{
	if (path.find_last_of(".") != std::string::npos && path.find_last_of("\\") != std::string::npos)
	{
		std::string name = path.substr(path.find_last_of("\\") + 1, path.find_last_of(".") - path.find_last_of("\\") - 1);

		std::ifstream wif(path);
		wif.imbue(std::locale(std::locale::empty(), new std::codecvt_utf8<char>));
		std::stringstream wss;
		wss << wif.rdbuf();
		LOG_O("SUCCESS READ DEFINE FILE", path);
	}
	else
	{
		LOG_W("FILE NAME ERROR", path);
		quit = true;
	}
}
void read_define(const std::string path, const std::string tag)
{
	if (path.find_last_of(".") != std::string::npos && path.find_last_of("\\") != std::string::npos)
	{
		std::string name = path.substr(path.find_last_of("\\") + 1, path.find_last_of(".") - path.find_last_of("\\") - 1);

		std::ifstream wif(path);
		wif.imbue(std::locale(std::locale::empty()));
		std::stringstream wss;
		wss << wif.rdbuf();
		read_as_define(wss.str());
		
		LOG_O("SUCCESS READ DEFINE FILE", path);
	}
	else
	{
		LOG_W("FILE NAME ERROR", path);
		quit = true;
	}
}

void read_folder(const std::string path, const std::string tag, const fn_str2 fn)
{
	for (auto p : std::experimental::filesystem::directory_iterator(path))
	{
		if (std::experimental::filesystem::is_directory(p))
		{
			read_folder(p.path().generic_u8string(), tag + p.path().generic_u8string().substr(path.size() + 1) + "\\", fn);
			continue;
		}

		char* pStr;
		int strSize = WideCharToMultiByte(CP_ACP, 0, p.path().c_str(), -1, NULL, 0, NULL, NULL);
		pStr = new char[strSize];
		
		WideCharToMultiByte(CP_ACP, 0, p.path().c_str(), -1, pStr, strSize, 0, 0);
		fn(pStr, tag);
	}

}

void read_map()
{
	{
		std::string path = executeDir + "\\map\\prov.txt";
		if (path.find_last_of(".") != std::string::npos && path.find_last_of("\\") != std::string::npos)
		{
			std::string name = path.substr(path.find_last_of("\\") + 1, path.find_last_of(".") - path.find_last_of("\\") - 1);

			std::ifstream wif(path);
			wif.imbue(std::locale(std::locale::empty(), new std::codecvt_utf8<char>));
			std::stringstream wss;
			wss << wif.rdbuf();
			read_as_prov(wss.str());
			LOG_O("SUCCESS READ MAP FILE", path);
		}
		else
		{
			LOG_W("FILE NAME ERROR", path);
			quit = true;
		}
	}

	SDL_Surface* map = IMG_Load((executeDir + "\\" + "map\\prv.bmp").c_str());
	SDL_Surface* geo = IMG_Load((executeDir + "\\" + "map\\geo.bmp").c_str());
	
	map_w = map->w;
	map_h = map->h;
	unsigned char* pixels = (unsigned char*)map->pixels;
	unsigned char* geo_pixels = (unsigned char*)geo->pixels;

	for (auto I = prv.begin(); I != prv.end(); ++I)
	{
		I->x1 = map_w;
		I->y1 = map_h;
	}

	for (unsigned int a = 0; a < map_w; ++a)
	{
		for (unsigned int b = 0; b < map_h; ++b)
		{
			for (auto I = prv.begin(); I != prv.end(); ++I)
			{
				if (color(pixels[3 * (b * map_w + a) + 2], pixels[3 * (b * map_w + a) + 1], pixels[3 * (b * map_w + a)]) == I->c)
				{
					if (a < I->x1)
					{
						I->x1 = a;
					}
					if (a > I->x2)
					{
						I->x2 = a;
					}
					if (b < I->y1)
					{
						I->y1 = b;
					}
					if (b > I->y2)
					{
						I->y2 = b;
					}
				}
			}
		}
	}

	for (auto I = prv.begin(); I != prv.end(); ++I)
	{
		if (I->x1 <= I->x2 &&  I->y1 <= I->y2)
		{
			I->enable = true;
			SDL_Surface* prov = SDL_CreateRGBSurfaceWithFormat(0, I->x2 - I->x1 + 1, I->y2 - I->y1 + 1, 24, SDL_PIXELFORMAT_BGR888);
			SDL_Surface* geogrp = SDL_CreateRGBSurfaceWithFormat(0, I->x2 - I->x1 + 1, I->y2 - I->y1 + 1, 24, SDL_PIXELFORMAT_BGR888);
			SDL_Surface* lineprov = SDL_CreateRGBSurfaceWithFormat(0, I->x2 - I->x1 + 1, I->y2 - I->y1 + 1, 24, SDL_PIXELFORMAT_BGR888);
			unsigned int w_t, h_t;

			w_t = prov->w;
			h_t = prov->h;
			unsigned char* pixels_t = (unsigned char*)prov->pixels;
			unsigned char* pixels_geo_t = (unsigned char*)geogrp->pixels;
			unsigned char* pixels_line_t = (unsigned char*)lineprov->pixels;


			int c, d;
			for (unsigned int a = 0; a <= w_t; ++a)
			{
				for (unsigned int b = 0; b <= h_t; ++b)
				{
					c = a + I->x1;
					d = b + I->y1;
					if (color(pixels[3 * (d * map_w + c) + 2], pixels[3 * (d * map_w + c) + 1], pixels[3 * (d * map_w + c)]) == I->c)
					{
						pixels_t[((a)+(w_t) * (b)) * 4 + 0] = 255;
						pixels_t[((a)+(w_t) * (b)) * 4 + 1] = 255;
						pixels_t[((a)+(w_t) * (b)) * 4 + 2] = 255;
						if ((a + b) % 6 < 2)
						{
							pixels_line_t[((a)+(w_t) * (b)) * 4 + 0] = 255;
							pixels_line_t[((a)+(w_t) * (b)) * 4 + 1] = 255;
							pixels_line_t[((a)+(w_t) * (b)) * 4 + 2] = 255;
						}

						pixels_geo_t[((a)+(w_t) * (b)) * 4 + 0] = geo_pixels[3 * (d * map_w + c) + 2];
						pixels_geo_t[((a)+(w_t) * (b)) * 4 + 1] = geo_pixels[3 * (d * map_w + c) + 1];
						pixels_geo_t[((a)+(w_t) * (b)) * 4 + 2] = geo_pixels[3 * (d * map_w + c)];
					}
				}
			}

			SDL_SetColorKey(prov, SDL_TRUE, 0);
			SDL_SetColorKey(geogrp, SDL_TRUE, 0);
			SDL_SetColorKey(lineprov, SDL_TRUE, 0);
			I->t = SDL_CreateTextureFromSurface(REND, prov);
			I->gt = SDL_CreateTextureFromSurface(REND, geogrp);
			I->lt = SDL_CreateTextureFromSurface(REND, lineprov);
		}
	}
}

void read_prv(const std::string path, const std::string tag) {

	if (path.find_last_of(".") != std::string::npos && path.find_last_of("\\") != std::string::npos)
	{
		std::string name = path.substr(path.find_last_of("\\") + 1, path.find_last_of(".") - path.find_last_of("\\") - 1);
		
		std::ifstream wif(path);
		wif.imbue(std::locale(std::locale::empty()));
		std::stringstream wss;
		wss << wif.rdbuf();
		read_as_prv(wss.str());


		LOG_O("SUCCESS READ PROVINCE FILE", path);
	}
	else
	{
		LOG_W("FILE NAME ERROR", path);
		quit = true;
	}
}

void read_nat(const std::string path, const std::string tag) {

	if (path.find_last_of(".") != std::string::npos && path.find_last_of("\\") != std::string::npos)
	{
		std::string name = path.substr(path.find_last_of("\\") + 1, path.find_last_of(".") - path.find_last_of("\\") - 1);

		std::ifstream wif(path);
		wif.imbue(std::locale(std::locale::empty(), new std::codecvt_utf8<char>));
		std::stringstream wss;
		wss << wif.rdbuf();
		read_as_nat(wss.str());
		LOG_O("SUCCESS READ NATION FILE", path);
	}
	else
	{
		LOG_W("FILE NAME ERROR", path);
		quit = true;
	}
}


/*
void gui_remove(int id)
{
	for (int a = 0; a < gui.size(); a++)
	{
		if (a != id && (gui.begin() + a)->parent == id)
		{
			(gui.begin() + a)->removing = true;
		}
	}
	(gui.begin() + id)->remove();
	for (int a = id; a < gui.size(); a++)
	{
		for (int b = 0; b < gui.size(); b++)
		{
			if ((gui.begin() + b)->parent == (gui.begin() + a)->id)
			{
				(gui.begin() + b)->parent -= 1;
			}
		}
		(gui.begin() + a)->id -= 1;
		//ikeys[(gui.begin() + a)->var["name"]] -= 1;
	}
}
*/