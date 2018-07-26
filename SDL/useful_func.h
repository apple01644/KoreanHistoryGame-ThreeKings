#pragma once
#include"action.h"

int get_lines(std::string s, int max_line)
{
	int lines = 0;
	unsigned int chr_len = s.length();
	const char* chr = s.c_str();
	int beg = 0, chrs = 0, bias = 0, all_line = 0;
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
	unsigned int chr_len = s.length();
	const char* chr = s.c_str();
	int beg = 0, chrs = 0, bias = 0, all_line = 0;
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
void draw_string_s(int i, std::string s, SDL_Color color, SDL_Rect* r) {
	/*
	SDL_Surface* surf = TTF_RenderUTF8_Solid(fonts[i], s.c_str(), color);
	SDL_Texture* t = SDL_CreateTextureFromSurface(REND, surf);
	SDL_FreeSurface(surf);
	SDL_RenderCopy(REND, t, NULL, r);
	SDL_DestroyTexture(t);*/
}
void draw_string(int ind, std::string s, SDL_Color color, SDL_Point* p, int size, float ratio, int opt)
{
	SDL_Rect r;
	r.x = p->x;
	r.y = p->y;
	r.w = get_chrs(s) * size * ratio;
	r.h = size;
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
	draw_string_s(ind, s, color, &r);
}
void draw_line(int ind, std::string s, int max_line, SDL_Color color, SDL_Point* p, int size, float ratio, int opt)
{
	int lines = 0;
	const unsigned int chr_len = s.length();
	const char* chr = s.c_str();
	int i, beg = 0, chrs = 0, bias = 0, st = 0;
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

void set_rect(SDL_Rect *r, int x, int y, int w, int h)
{
	r->x = x;
	r->y = y;
	r->w = w;
	r->h = h;
}
void set_rect(SDL_Rect *r, int x, int y)
{
	r->x = x;
	r->y = y;
}

Uint32 color(int r, int g, int b)
{
	return r * 65536 + g * 256 + b;
}

void gui_remove(int id)
{
	for (int a = 0; a < gui.size(); a++)
	{
		if (a != id && gui[a].parent == id)
		{
			gui[a].removing = true;
		}
	}
	gui[id].remove();
	for (int a = id; a < gui.size(); a++)
	{
		for (int b = 0; b < gui.size(); b++)
		{
			if (gui[b].parent == gui[a].id)
			{
				gui[b].parent -= 1;
			}
		}
		gui[a].id -= 1;
		gui_key[gui[a].var["name"]] -= 1;
	}
}

std::string Var(std::string s) {
	if (*s.begin() == '"' && *(s.end() - 1) == '"')
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
	bool used = false;
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
			name += "." + scope[a];
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
void read_as_define(std::wstring ws)
{
	std::string s;
	s.assign(ws.begin(), ws.end());
	std::string s2;
	auto J = s.begin();
	bool ignore = false;
	bool comment = false;

	for (auto I = s.begin(); I != s.end(); ++I)
	{
		if (ignore)
		{
			ignore = false;
		}
		else
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
				ignore = true;
				J = I + 1;
				continue;
			}
			else if (*I == '#')
			{
				comment = true;				
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
	wd_p->var[s1] = Var(s2);
}
void read_as_ui(std::wstring ws)
{
	std::string s;
	s.assign(ws.begin(), ws.end());
	std::string s2;
	std::string s3;
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
						s2.erase(s2.begin());
					}
					else
					{
						Widget wd(0,0,20,20,wd_none,Str(gui.size()));
						
						for (unsigned int j = 0, begin = 0; j < s2.size(); ++j)
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
									LOG_H(s3);
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

						if (*(s2.end() - 1) == '/')
						{
							gui.push_back(wd);
						}
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
			LOG_W("FILE TO SURFACE", path);
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
			LOG_W("FILE TO SOUND", path);
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

void read_ui(const std::string path,const std::string tag)
{
	if (path.find_last_of(".") != std::string::npos && path.find_last_of("\\") != std::string::npos)
	{
		std::string name = path.substr(path.find_last_of("\\") + 1, path.find_last_of(".") - path.find_last_of("\\") - 1);

		std::wifstream wif(path);
		wif.imbue(std::locale(std::locale::empty(), new std::codecvt_utf8<wchar_t>));
		std::wstringstream wss;
		wss << wif.rdbuf();
		read_as_ui(wss.str());
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

		std::wifstream wif(path);
		wif.imbue(std::locale(std::locale::empty(), new std::codecvt_utf8<wchar_t>));
		std::wstringstream wss;
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