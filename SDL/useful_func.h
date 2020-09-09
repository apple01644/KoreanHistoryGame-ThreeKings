#pragma once
#include"action.h"

int get_lines(std::wstring s, int max_line)
{
	int lines = 0;
	const size_t chr_len = s.length();
	const wchar* chr = s.c_str();
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
int get_chrs(std::wstring s)
{
	const size_t chr_len = s.length();
	const wchar* chr = s.c_str();
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

void draw_string(std::wstring ind, std::wstring ws, std::wstring color, SDL_Point* p, unsigned int size, unsigned int opt)
{
	std::string s = conv.to_bytes(ws);
	if (s.length() == 0 || size == 0)
	{
		return;
	}


	SDL_Surface* surf = TTF_RenderUTF8_Solid(tfx[ind].t, s.c_str(), Color[color]);
	SDL_Texture* t = SDL_CreateTextureFromSurface(REND, surf);


	SDL_Rect r;
	r.x = p->x;
	r.y = p->y;
	r.w = surf->w * size / 72;
	r.h = surf->h * size / 72;


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

	SDL_FreeSurface(surf);
	SDL_RenderCopy(REND, t, NULL, &r);
	SDL_DestroyTexture(t);

}
void draw_line(std::wstring ind, std::wstring s, unsigned int max_line, std::wstring color, SDL_Point* p, unsigned int size, unsigned int opt)
{
	unsigned int lines = 0;
	const size_t chr_len = s.length();
	const wchar* chr = s.c_str();
	unsigned int i, chrs = 0, bias = 0, st = 0;
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

			draw_string(ind, s.substr(st, i - st), color, p, size, opt);
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
		draw_string(ind, s.substr(st, i - st), color, p, size, opt);
	}
}

template<typename T, typename T2>
size_t ray_ui(size_t owner, T x, T2 y)
{
	x = (int)x;
	y = (int)y;
	size_t r = 0;
	{
		auto I = gui.at(owner);
		if (I.rx < x && I.rx + I.w > x && I.ry < y && I.ry + I.h > y)
		{
			r = owner;
		}
	}
	auto I = gui.begin();
	for (size_t i = 0; i < gui.size() && I != gui.end(); ++i, ++I)
	{
		if (I->enable && I->parent == owner && I->id != owner)
		{
			size_t b = ray_ui(i, x, y);
			if (b != 0)
			{
				r = b;
			}
		}
	}
	return r;
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

void gui_remove(const size_t id)
{
	for (size_t a = 0; a < gui.size(); a++)
	{
		if (a != id && gui.at(a).parent == id)
		{
			gui.at(a).removing = true;
		}
	}
	gui.at(id).remove();
	for (size_t a = id; a < gui.size(); a++)
	{
		for (size_t b = 0; b < gui.size(); b++)
		{
			if (gui.at(b).parent == gui.at(a).id)
			{
				gui.at(b).parent -= 1;
			}
		}
		gui.at(a).id -= 1;
		gui_key[gui.at(a).var[L"name"]] -= 1;
	}
}

std::wstring Var(std::wstring s) {
	if (*s.begin() == '"' && *s.rbegin() == '"')
	{
		return s.substr(1, s.size() - 2);
	}
	return s;
}

void incode_define(std::wstring s)
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

	std::wstring s1 = L"";
	std::wstring s2 = L"";
	std::wstring S = L"";
	std::vector <std::wstring> scope;
	std::wstring name = L"";
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
					LOG_V(L"script[L" + name + L"." + S.substr(0, a) + L"]", S.substr(a + 1));
					script[name + L"." + S.substr(0, a)] = S.substr(a + 1);
					break;
				}
			}
			begin = i + 1;
			scope.pop_back();
			continue;
		}
		name = L"";
		for (int a = 0; a < scope.size(); ++a)
		{
			name += L"." + scope.at(a);
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
					LOG_V(L"script[L" + name + s1 + L"[L" + Str(index) + L"]" + L"]", s2);
					script[name + s1 + L"[L" + Str(index) + L"]"] = s2;
					index++;
					s2 = L"";
				}
				else if (s[i + 1] == ']')
				{
					s2 += s[i];
					LOG_V(L"script[L" + name + s1 + L"[L" + Str(index) + L"]" + L"]", s2);
					script[name + s1 + L"[L" + Str(index) + L"]"] = s2;
					index++;
					s2 = L"";
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
					LOG_V(L"script[L" + name + L"." + S.substr(0, a) + L"]", S.substr(a + 1));
					script[name + L"." + S.substr(0, a)] = S.substr(a + 1);
					break;
				}
			}

			begin = i + 1;
			continue;
		}
		if (s[i] == '=' && name.empty())
		{
			LOG_V(L"script[L" + s.substr(0, i) + L"]", s.substr(i + 1));
			script[s.substr(0, i)] = s.substr(i + 1);
			continue;
		}
	}
}

void incode_prov(std::wstring s)
{
	std::wstring s1 = L"";
	std::wstring s2 = L"";
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
			key[L"argc"] = L"0";
			key[L"func"] = L"";
			for (unsigned int j = 0; j < s1.size(); j++)
			{
				if (s1.at(j) == '&')
				{
					key[L"func"] = s1.substr(k, j - k);
					k = j + 1;
				}
				if (s1.at(j) == ',' || j == s1.size() - 1)
				{
					if (j == s1.size() - 1)
					{
						key[L"argv[" + key[L"argc"] + L"]"] = s1.substr(k);
					}
					else
					{
						key[L"argv[" + key[L"argc"] + L"]"] = s1.substr(k, j - k);
						k = j + 1;
					}
					key[L"argc"] = Str(Num(key[L"argc"]) + 1);
				}
			}
			while (Num(s2) + 1 > prv.size())
			{
				Province prov;
				prov.id = prv.size();
				prv.push_back(prov);
			}
			LOG_O(L"Province Color Defined", s2);
			if (key[L"func"] == L"color")
			{
				prv.at(Num(s2)).c = color(Num(key[L"argv[0]"]), Num(key[L"argv[1]"]), Num(key[L"argv[2]"]));
			}
		}
	}
}
void incode_prv(unsigned int& sec, std::wstring s)
{
	std::wstring s1 = L"";
	std::wstring s2 = L"";
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
			if (s2 == L"select")
			{
				sec = Num(s1);				
				continue;
			}
			if (s2 == L"waste_land")
			{
				if (sec == -1) continue;
				if (s1 == L"false")
				{
					prv.at(sec).waste_land = false;
				}
				else if (s1 == L"true")
				{
					prv.at(sec).waste_land = true;
				}
				continue;
			}
			if (s2 == L"name") {
				prv.at(sec).name = s1;
				continue;
			}
			if (s2 == L"OWN") {
				if (nat.find(s1) == nat.end()) throw;
				prv.at(sec).OWN = s1;
				continue;
			}
			if (s2 == L"CON") {
				if (nat.find(s1) == nat.end()) throw;
				prv.at(sec).CON = s1;
				continue;
			}
			prv.at(sec).var[s2] = s1;
		}
	}
}
void incode_nat(std::wstring *sec, std::wstring s)
{
	std::wstring s1 = L"";
	std::wstring s2 = L"";
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


			key[L"argc"] = L"0";
			key[L"func"] = L"";
			unsigned int k = 0;
			for (unsigned int j = 0; j < s1.size(); j++)
			{
				if (s1.at(j) == '&')
				{
					key[L"func"] = s1.substr(k, j - k);
					k = j + 1;
				}
				if (s1.at(j) == ',' || j == s1.size() - 1)
				{
					if (j == s1.size() - 1)
					{
						key[L"argv[" + key[L"argc"] + L"]"] = s1.substr(k);
					}
					else
					{
						key[L"argv[" + key[L"argc"] + L"]"] = s1.substr(k, j - k);
						k = j + 1;
					}
					key[L"argc"] = Str(Num(key[L"argc"]) + 1);
				}
			}
			if (key[L"func"] == L"color")
			{
				s1 = Str(color(Num(key[L"argv[0]"]), Num(key[L"argv[1]"]), Num(key[L"argv[2]"])));
			}

			if (s2 == L"name")
			{
				Nation natn;
				nat[s1] = natn;
				*sec = s1;
				break;
			}
			if (s2 == L"color")
			{
				nat[*sec].c = Num(s1);
				break;
			}
			nat[*sec].var[s2] = s1;

		}
	}
}
void incode_ui(Widget* wd_p, std::wstring s1, std::wstring s2)
{
	if (s1 == L"x")
	{
		wd_p->x = Num(Var(s2));
		if (wd_p->x < 0)
		{
			wd_p->x += scr_w;
		}
		return;
	}
	if (s1 == L"y")
	{
		wd_p->y = Num(Var(s2));
		if (wd_p->y < 0)
		{
			wd_p->y += scr_h;
		}
		return;
	}
	if (s1 == L"w")
	{
		wd_p->w = (Uint16)Num(Var(s2));
		return;
	}
	if (s1 == L"h")
	{
		wd_p->h = (Uint16)Num(Var(s2));
		return;
	}
	if (s1 == L"id")
	{
		wd_p->change_key(Var(s2));
		return;
	}
	if (s1 == L"enable")
	{
		if (s2 == L"true")
		{
			wd_p->enable = true;
		}
		if (s2 == L"false")
		{
			wd_p->enable = false;
		}
		return;
	}
	if (s1 == L"mousedown")
	{
		s2 = Var(s2);
		if (event_key.at(s2).type != ev_mousedown)
		{
			LOG_W(L"UI id-" + wd_p->var.at(L"name"), L"Incorrect Eventtype(mousedown)");
			return;
		}
		LOG_O(L"UI id-" + wd_p->var.at(L"name"), L"Correct Eventtype(mousedown)");
		wd_p->ev.Event_mousedown = event_key.at(s2).mousedown;
		return;
	}
	if (s1 == L"step")
	{
		s2 = Var(s2);
		if (event_key.at(s2).type != ev_step)
		{
			LOG_W(L"UI id-" + wd_p->var.at(L"name"), L"Incorrect Eventtype(step)");
			return;
		}
		LOG_O(L"UI id-" + wd_p->var.at(L"name"), L"Correct Eventtype(step)");
		wd_p->ev.Event_step = event_key.at(s2).step;
		return;
	}
	wd_p->var[s1] = Var(s2);
}
void incode_man(unsigned int *sec, std::wstring s)
{
	std::wstring s1 = L"";
	std::wstring s2 = L"";
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
			if (s2 == L"select")
			{
				*sec = Num(s1);
				man[*sec] = Man();
				continue;
			}
			man.at(*sec).var[s2] = s1;
		}
	}
}

void read_as_define(std::wstring s)
{
	std::wstring s2;
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
void read_as_prov(std::wstring s)
{
	std::wstring s2;
	auto J = s.begin();
	bool comment = false;
	bool long_comment = false;
	for (auto I = s.begin(); I != s.end(); ++I)
	{
		if (long_comment)
		{
			if (*I == '/')
			{
				if (*(I - 1) == '*')
				{
					long_comment = false;
					J = I + 1;
				}
			}
			continue;
		}
		if (comment)
		{
			if (*I == '\n')
			{
				comment = false;
				J = I + 1;
			}
			continue;
		}
		if (*I == '/')
		{
			if (*(I + 1) == '*')
			{
				long_comment = true;
				continue;
			}
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
void read_as_prv(std::wstring s)
{
	std::wstring s2;
	auto J = s.begin();
	bool comment = false;
	bool long_comment = false;
	unsigned int sec = -1;
	auto P = prv.end();
	for (auto I = s.begin(); I != s.end(); ++I)
	{
		if (long_comment)
		{
			if (*I == '/')
			{
				if (*(I - 1) == '*')
				{
					long_comment = false;
				}
			}
			continue;
		}
		if (comment)
		{
			if (*I == '\n')
			{
				comment = false;
				J = I + 1;
			}
			continue;
		}
		if (*I == '/')
		{
			if (*(I + 1) == '*')
			{
				long_comment = true;
				continue;
			}
		}
		if (*I == ';')
		{
			incode_prv(sec, s2.assign(J, I));
			J = I + 1;
			continue;
		}
		if (*I == 35)
		{
			comment = true;
			continue;
		}

	}
	std::wcout << prv.at(sec).OWN << " : " << prv.at(sec).CON;
}
void read_as_nat(std::wstring s)
{
	std::wstring s2;
	auto J = s.begin();
	bool comment = false;
	auto P = prv.end();
	std::wstring sec = L"NAV";
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
void read_as_ui(std::wstring s)
{
	std::wstring s2;
	std::wstring s3;
	std::vector<std::wstring> scope;
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
							LOG_W(L"UI PARENT ERROR!");
							quit = true;
						}
					}
					else
					{
						Widget wd(0, 0, 20, 20, wd_none, Str(gui.size()));

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
								if (*s3.rbegin() == '/')
								{
									s3 = s3.substr(0, s3.size() - 1);
								}

								if (begin == 0)
								{
									do {
										if (s3 == L"image")
										{
											wd.type = wd_image;
											break;
										}
										if (s3 == L"label")
										{
											wd.type = wd_label;
											wd.var[L"ind"] = L"default";
											wd.var[L"size"] = L"40";
											wd.var[L"opt"] = L"0";
											wd.var[L"color"] = L"black";
											wd.var[L"text"] = L"default";
											break;
										}
										if (s3 == L"text")
										{
											wd.type = wd_text;
											wd.var[L"ind"] = L"default";
											wd.var[L"size"] = L"40";
											wd.var[L"line"] = L"0";
											wd.var[L"opt"] = L"0";
											wd.var[L"color"] = L"black";
											wd.var[L"text"] = L"default";
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
							scope.push_back(wd.var[L"name"]);
						}
						LOG_O(L"UI CREATED SUCCESSFULLY", wd.var[L"name"]);
						gui.push_back(wd);

					}

					break;
				}
				if (i == s.size())
				{
					LOG_W(L"UI SYNTAX ERROR", L"Can't find Closed Bracket");
					quit = true;
					break;
				}
			}
		}
	}
	if (comment)
	{
		LOG_W(L"UI Syntax Error", L"Cant Find EOF");
		quit = true;
	}
}
void read_as_man(std::wstring s)
{
	std::wstring s2;
	auto J = s.begin();
	bool comment = false;
	bool long_comment = false;
	auto P = prv.end();
	unsigned int sec = (unsigned int)man.size();
	for (auto I = s.begin(); I != s.end(); ++I)
	{
		if (long_comment)
		{
			if (*I == '/')
			{
				if (*(I - 1) == '*')
				{
					long_comment = false;
				}
			}
			continue;
		}
		if (comment)
		{
			if (*I == '\n')
			{
				comment = false;
				J = I + 1;
			}
			continue;
		}
		if (*I == '/')
		{
			if (*(I + 1) == '*')
			{
				long_comment = true;
				continue;
			}
		}
		if (*I == ';')
		{
			incode_man(&sec, s2.assign(J, I));
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

void read_gfx(const std::wstring path, const std::wstring tag)
{
	if (path.find_last_of(L".") != std::wstring::npos && path.find_last_of(L"/") != std::wstring::npos)
	{
		std::wstring name = tag + path.substr(path.find_last_of(L"/") + 1, path.find_last_of(L".") - path.find_last_of(L"/") - 1);

		gfx[name].s = IMG_Load(conv.to_bytes(path).c_str());
		SDL_SetColorKey(gfx[name].s, SDL_TRUE, color(255, 0, 255));
		if (gfx[name].s == NULL)
		{
			LOG_W(L"FAIL TO SURFACE", conv.from_bytes(IMG_GetError()));
		}
		else
		{
			gfx[name].t = SDL_CreateTextureFromSurface(REND, gfx[name].s);
			if (gfx[name].t == NULL)
			{
				LOG_W(L"SURFACE TO TEXTURE", path);
			}
			else
			{
				LOG_O(L"SUCCESS READ IMAGE FILE(L" + name + L")", path);
			}
		}
	}
	else
	{
		LOG_W(L"FILE NAME ERROR", path);
		quit = true;
	}
}
void read_sfx(const std::wstring path, const std::wstring tag)
{
	if (path.find_last_of(L".") != std::wstring::npos && path.find_last_of(L"/") != std::wstring::npos)
	{
		std::wstring name = path.substr(path.find_last_of(L"/") + 1, path.find_last_of(L".") - path.find_last_of(L"/") - 1);
		sfx[name] = Mix_LoadMUS(conv.to_bytes(path).c_str());
		if (sfx[name] == NULL)
		{
			LOG_W(L"FAIL TO SOUND", path);
			quit = true;
		}
		else
		{
			LOG_O(L"SUCCESS READ SOUND FILE", path);
		}
	}
	else
	{
		LOG_W(L"FILE NAME ERROR", path);
		quit = true;
	}
}
void read_tfx(const std::wstring path, const std::wstring tag)
{
	if (path.find_last_of(L".") != std::wstring::npos && path.find_last_of(L"/") != std::wstring::npos)
	{
		std::wstring name = tag + path.substr(path.find_last_of(L"/") + 1, path.find_last_of(L".") - path.find_last_of(L"/") - 1);

		tfx[name].t = TTF_OpenFont(conv.to_bytes(path).c_str(), 144);

		if (tfx[name].t == NULL)
		{
			LOG_W(L"FAIL TO FONT", path);
		}
		else
		{
			LOG_O(L"SUCCESS READ FONT FILE(L" + name + L")", path);
		}
	}
	else
	{
		LOG_W(L"FILE NAME ERROR", path);
		quit = true;
	}
}

void read_ui(const std::wstring path, const std::wstring tag)
{
	if (path.find_last_of(L".") != std::wstring::npos && path.find_last_of(L"/") != std::wstring::npos)
	{
		std::wstring name = path.substr(path.find_last_of(L"/") + 1, path.find_last_of(L".") - path.find_last_of(L"/") - 1);

		std::ifstream ifs(path);
		std::stringstream ss;
		ss << ifs.rdbuf();
		read_as_ui(conv.from_bytes(ss.str()));
		LOG_O(L"SUCCESS READ UI FILE", path);
	}
	else
	{
		LOG_W(L"FILE NAME ERROR", path);
		quit = true;
	}
}
void read_define(const std::wstring path, const std::wstring tag)
{
	if (path.find_last_of(L".") != std::wstring::npos && path.find_last_of(L"/") != std::wstring::npos)
	{
		std::wstring name = path.substr(path.find_last_of(L"/") + 1, path.find_last_of(L".") - path.find_last_of(L"/") - 1);

		std::ifstream ifs(path);
		std::stringstream ss;
		ss << ifs.rdbuf();
		read_as_define(conv.from_bytes(ss.str()));

		LOG_O(L"SUCCESS READ DEFINE FILE", path);
	}
	else
	{
		LOG_W(L"FILE NAME ERROR", path);
		quit = true;
	}
}

void read_folder(const std::wstring path, const std::wstring tag, const fn_str2 fn)
{
	for (auto p : std::experimental::filesystem::directory_iterator(path))
	{
		if (std::experimental::filesystem::is_directory(p))
		{
			read_folder(p.path().generic_wstring(), tag + p.path().generic_wstring().substr(path.size() + 1) + L"\\", fn);
			continue;
		}

		fn(p.path().generic_wstring(), tag);
	}

}

void read_map()
{
	{
		std::wstring path = executeDir + L"\\map\\prov.txt";
		if (path.find_last_of(L".") != std::wstring::npos && path.find_last_of(L"\\") != std::wstring::npos)
		{
			std::wstring name = path.substr(path.find_last_of(L"\\") + 1, path.find_last_of(L".") - path.find_last_of(L"\\") - 1);

			std::ifstream ifs(path);
			std::stringstream ss;
			ss << ifs.rdbuf();
			read_as_prov(conv.from_bytes(ss.str()));
			LOG_O(L"SUCCESS READ MAP FILE", path);
		}
		else
		{
			LOG_W(L"FILE NAME ERROR", path);
			quit = true;
		}
	}

	SDL_Surface* map = IMG_Load(conv.to_bytes(executeDir + L"\\" + L"map\\prv.bmp").c_str());
	SDL_Surface* geo = IMG_Load(conv.to_bytes(executeDir + L"\\" + L"map\\geo.bmp").c_str());

	map_w = map->w;
	map_h = map->h;
	unsigned char* pixels = (unsigned char*)map->pixels;
	unsigned char* geo_pixels = (unsigned char*)geo->pixels;

	for (auto I = prv.begin(); I != prv.end(); ++I)
	{
		I->x1 = map_w;
		I->y1 = map_h;
	}
#pragma loop(hint_parallel(0))  
	for (unsigned int a = 0; a < map_w; ++a)
	{
		for (unsigned int b = 0; b < map_h; ++b)
		{
			map_reg[a + map_w * b] = 0;
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
					break;
				}
			}
		}
	}
	{
		unsigned int i = 0;
		for (auto I = prv.begin(); I != prv.end(); ++I, ++i)
		{
			if (I->x1 <= I->x2 &&  I->y1 <= I->y2)
			{
				for (unsigned int a = I->x1; a <= I->x2 + 1; ++a)
				{
					for (unsigned int b = I->y1; b <= I->y2 + 1; ++b)
					{
						if (color(pixels[3 * (b * map_w + a) + 2], pixels[3 * (b * map_w + a) + 1], pixels[3 * (b * map_w + a)]) == I->c)
						{
							map_reg[a + map_w * b] = i;
						}
					}
				}
			}
		}
		i = 0;
		for (auto I = prv.begin(); I != prv.end(); ++I, ++i)
		{
			if (I->x1 <= I->x2 &&  I->y1 <= I->y2 && !I->waste_land)
			{
				SDL_Surface* prov = SDL_CreateRGBSurfaceWithFormat(0, I->x2 - I->x1 + 1, I->y2 - I->y1 + 1, 24, SDL_PIXELFORMAT_BGR888);
				SDL_Surface* geogrp = SDL_CreateRGBSurfaceWithFormat(0, I->x2 - I->x1 + 1, I->y2 - I->y1 + 1, 24, SDL_PIXELFORMAT_BGR888);
				SDL_Surface* lineprov = SDL_CreateRGBSurfaceWithFormat(0, I->x2 - I->x1 + 1, I->y2 - I->y1 + 1, 24, SDL_PIXELFORMAT_BGR888);
				unsigned int w_t, h_t;

				w_t = prov->w;
				h_t = prov->h;
				unsigned char* pixels_t = (unsigned char*)prov->pixels;
				unsigned char* pixels_geo_t = (unsigned char*)geogrp->pixels;
				unsigned char* pixels_line_t = (unsigned char*)lineprov->pixels;


				int W[4][2] = { {0,1},{-1,0},{0, -1},{1 ,0 } };
				int c, d;
#pragma loop(hint_parallel(0))
				for (unsigned int a = 0; a <= w_t; ++a)
				{
					for (unsigned int b = 0; b <= h_t; ++b)
					{
						c = a + I->x1;
						d = b + I->y1;
						if (map_reg[c + map_w * d] == i)
						{
							I->px = (float)((1.0 * I->px * I->pnum + a) / (I->pnum + 1));
							I->py = (float)((1.0 * I->py * I->pnum + b) / (I->pnum + 1));
							I->pnum++;
						}
						for (unsigned int w = 0; w < 4; ++w)
						{
							c = a + W[w][0];
							d = b + W[w][1];
							if (c >= 0 && d >= 0 && c < (int)map_w && d < (int)map_h)
							{
								map_connect[{map_reg[a + map_w * b], map_reg[c + map_w * d]}] = 1;
							}
						}

					}
				}
#pragma loop(hint_parallel(0))
				for (unsigned int a = 0; a <= w_t; ++a)
				{
					for (unsigned int b = 0; b <= h_t; ++b)
					{
						c = a + I->x1;
						d = b + I->y1;
						if (map_reg[c + map_w * d] == i)
						{
							//if (map_reg[Str(c) + ":" + Str(d) + "b"] == 0)
							//if (sqrt(pow(I->px - a, 2) + pow(I->py - b, 2)) - (w_t + h_t) / 2 + map_reg[Str(c) + ":" + Str(d) + "r"] * 6 + 20 > 0 || map_reg[Str(c) + ":" + Str(d) + "r"] == 0)
							{
								pixels_t[((a)+(w_t) * (b)) * 4 + 0] = 255;
								pixels_t[((a)+(w_t) * (b)) * 4 + 1] = 255;
								pixels_t[((a)+(w_t) * (b)) * 4 + 2] = 255;
								if ((a + b + I->x1 + I->y1) % 6 < 2)
								{
									pixels_line_t[((a)+(w_t) * (b)) * 4 + 0] = 255;
									pixels_line_t[((a)+(w_t) * (b)) * 4 + 1] = 255;
									pixels_line_t[((a)+(w_t) * (b)) * 4 + 2] = 255;
								}
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
				if (I->t == nullptr) throw;
				I->gt = SDL_CreateTextureFromSurface(REND, geogrp);
				if (I->gt == nullptr) throw;
				I->lt = SDL_CreateTextureFromSurface(REND, lineprov);
				if (I->lt == nullptr) throw;
			}
		}
	}
	for (size_t i = 0; i < prv.size(); ++i)
	{

		for (size_t j = 0; j < prv.size(); ++j)
		{
			if (map_connect[{i, j}] == 1 && i != j)
			{
				auto I0 = prv.at(i);
				auto I1 = prv.at(j);
				map_connect[{i, j}] = (int)(sqrt(pow(I0.px + I0.x1 - I1.x1 - I1.px, 2) + pow(I0.py + I0.y1 - I1.y1 - I1.py, 2)));
			}
		}
	}
}

void read_prv(const std::wstring path, const std::wstring tag) {

	if (path.find_last_of(L".") != std::wstring::npos && path.find_last_of(L"/") != std::wstring::npos)
	{
		std::wstring name = path.substr(path.find_last_of(L"/") + 1, path.find_last_of(L".") - path.find_last_of(L"/") - 1);

		std::ifstream ifs(path);
		std::stringstream ss;
		ss << ifs.rdbuf();
		read_as_prv(conv.from_bytes(ss.str()));


		LOG_O(L"SUCCESS READ PROVINCE FILE", path);
	}
	else
	{
		LOG_W(L"FILE NAME ERROR", path);
		quit = true;
	}
}
void read_man(const std::wstring path, const std::wstring tag) {

	if (path.find_last_of(L".") != std::wstring::npos && path.find_last_of(L"/") != std::wstring::npos)
	{
		std::wstring name = path.substr(path.find_last_of(L"/") + 1, path.find_last_of(L".") - path.find_last_of(L"/") - 1);

		std::ifstream ifs(path);
		std::stringstream ss;
		ss << ifs.rdbuf();
		read_as_man(conv.from_bytes(ss.str()));


		LOG_O(L"SUCCESS READ CHARACTER FILE", path);
	}
	else
	{
		LOG_W(L"FILE NAME ERROR", path);
		quit = true;
	}
}

void read_nat(const std::wstring path, const std::wstring tag) {

	if (path.find_last_of(L".") != std::wstring::npos && path.find_last_of(L"/") != std::wstring::npos)
	{
		std::wstring name = path.substr(path.find_last_of(L"/") + 1, path.find_last_of(L".") - path.find_last_of(L"/") - 1);

		std::ifstream ifs(path);
		std::stringstream ss;
		ss << ifs.rdbuf();
		read_as_nat(conv.from_bytes(ss.str()));
		LOG_O(L"SUCCESS READ NATION FILE", path);
	}
	else
	{
		LOG_W(L"FILE NAME ERROR", path);
		quit = true;
	}
}

/*
void gui_remove(int id)apple
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
		//ikeys[(gui.begin() + a)->var[L"name"]] -= 1;
	}
}
*/

