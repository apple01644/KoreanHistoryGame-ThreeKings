#include "useful_func.h"

int main(int argc, char* args[])
{
	srand((unsigned int)time(NULL));
	system("color 01");
	
	for (int i = 0; i < argc; i++)
	{
		LOG_H("ARG-" + Str(i), args[i]);
	}

	if (!init())
	{
		LOG_W("System Initialize Failed");
	}
	else
	{
		if (!loadMedia())
		{
			LOG_W("System Load Failed");
		}
		else
		{
			start();
			LOG_O("GAME ON");
			SDL_Event e;
			SDL_StartTextInput();

			std::thread trd_ui(ui);

			while (!quit)
			{
				step(&e);
			}
			trd_ui.join();
			SDL_StopTextInput();
			LOG_O("GAME OFF");
		}
	}
	close();
	LOG_Stop();
	return 0;
}

/*
int day_over()
{
	bool is_yun = false;
	if (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0));
	{
		is_yun = true;
	}
	day++;
	if (mon == 2)
	{
		if (day > 28 && !is_yun)
		{
			day = 1;
			mon++;
			return 1;
		}
		else if (day > 29 && is_yun)
		{
			day = 1;
			mon++;
			return 1;
		}
	}
	else if ((mon % 2 == 1 && mon <= 7) || (mon % 2 == 0 && mon >= 8))
	{
		if (day > 31)
		{
			day = 1;
			mon++;
			return 1;
		}
	}
	else if (day > 30)
	{
		day = 1;
		mon++;
		return 1;
	}
	if (mon > 12)
	{
		mon = 1;
		year++;
		return 2;
	}
	return 0;

}*/

void start()
{
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 16);
	//glEnable(GL_MULTISAMPLE);
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2");
	{
		Widget wd0(404,404,404,404, wd_image, "Error!");
		wd0.enable = false;
		wd0.id = gui.size();
		wd0.var["source"] = "flag\\낙랑";
		wd0.parent = wd0.id;
		gui.push_back(wd0);
	}
	{
		Widget wd0(0, 0, scr_w, scr_h, wd_image, "Backmap");
		wd0.enable = true;
		wd0.id = gui.size();
		wd0.var["source"] = "None";
		wd0.parent = wd0.id;
		wd0.ev.Enable_mousedown = true;
		wd0.ev.Event_mousedown = BackMap_mousedown;
		gui.push_back(wd0);
	}
	{
		Nation nt;
		nt.c = color(255, 255, 255);
		nt.var["title"] = "원주민";
		nat["NAV"] = nt;
	}
	{
		Nation nt;
		nt.c = color(60, 0, 0);
		nt.var["title"] = "반군";
		nat["REB"] = nt;
	}

	{
		SDL_Color clr;
		clr.a = 255;
		clr.r = 0;
		clr.g = 0;
		clr.b = 0;

		Color["black"] = clr;
	}
	{
		SDL_Color clr;
		clr.a = 255;
		clr.r = 255;
		clr.g = 255;
		clr.b = 255;

		Color["white"] = clr;
	}
	script["CON"] = "REB";
	gui.at(gui_key["nation_flag"]).var["source"] = "flag\\REB";


	if (map_p * map_w < scr_w)
	{
		map_p = 1.0f * scr_w / map_w;
	}
	if (map_p * map_h < scr_h)
	{
		map_p = 1.0f * scr_h / map_h;
	}
	if (scr_w / 2.0 * (1 / map_p - 1) > map_x)
	{
		map_x = (float)(scr_w / 2.0 * (1 / map_p - 1));
	}
	if (scr_h / 2.0 * (1 / map_p - 1) > map_y)
	{
		map_y = (float)(scr_h / 2.0 * (1 / map_p - 1));
	}
	if (map_w + 1.0 - scr_w / 2.0 *(1 + 1 / map_p) < map_x)
	{
		map_x = (float)(map_w + 1.0 - scr_w / 2.0 *(1 + 1 / map_p));
	}
	if (map_h + 1.0 - scr_h / 2.0 *(1 + 1 / map_p) < map_y)
	{
		map_y = (float)(map_h + 1.0 - scr_h / 2.0 *(1 + 1 / map_p));
	}

	for (auto I : prv)
	{
		if (!I.enable)
		{
			LOG_W(I.var["name"]);
		}
	}
	//Mix_PlayMusic(sfx["sample"], -1);
}
void step(SDL_Event *e)
{
	LAST = NOW;
	NOW = SDL_GetPerformanceCounter();

	double TimeDelta = (double)((NOW - LAST) * 1000 / SDL_GetPerformanceFrequency());


	int x, y;
	SDL_GetMouseState(&x, &y);
	while (SDL_PollEvent(e) != 0)
	{
		size_t pass = 0;
		size_t depth = 0;
		size_t i = 0;
		switch (e->type)
		{
		case SDL_TEXTINPUT: {
			script["buf"] += e->text.text;
			break;
		}
		case SDL_QUIT: {
			quit = true;
			return;
		}
		case SDL_MOUSEBUTTONDOWN: {
			for (auto I : gui)
			{
				if (I.enable)
				{
					if (I.rx < x && I.rx + I.w > x && I.ry < y && I.ry + I.h > y && depth < mother(i))
					{
						if (I.ev.Enable_mousedown)
						{
							pass = i;
						}
						else
						{
							pass = 0;
						}
						depth = mother(i);
					}
				}
				++i;
			}
			if (pass > 0)
			{
				Arg_Mouse obj;
				obj.x = e->button.x;
				obj.y = e->button.y;
				obj.button = e->button.button;
				gui.at(pass).ev.Event_mousedown(pass, obj);
			}
			break;
		}
		case SDL_KEYDOWN: {
			switch (e->key.keysym.sym)
			{
			case SDLK_ESCAPE: {
				quit = true;
				break;
			}
			case SDLK_BACKSPACE: {
				while (script["buf"].size() > 0)
				{
					if (*script["buf"].crbegin() >= -64)
					{
						script["buf"].pop_back();
						break;
					}
					script["buf"].pop_back();
				}
				break;
			}
			}
			break;
		}
		case SDL_MOUSEWHEEL:
		{
			map_p = map_p * (float)pow(0.9, -e->wheel.y);
			if (map_p * map_w < scr_w)
			{
				map_p = 1.0f * scr_w / map_w;
			}
			if (map_p * map_h < scr_h)
			{
				map_p = 1.0f * scr_h / map_h;
			}
			break;
		}
		}

	}

		
	{
		double DRAG_SENSITIVE = 0.00015;// / sqrt(map_p);
		if (x <= 20)
		{
			map_x = map_x - (DRAG_SENSITIVE / map_p);
			map_y = map_y + (DRAG_SENSITIVE * (y * 2.0 / scr_h - 1) / map_p);
		}
		else if (y <= 20)
		{
			map_x = map_x +(DRAG_SENSITIVE * (x * 2.0 / scr_w - 1) / map_p);
			map_y = map_y - (DRAG_SENSITIVE / map_p);
		}
		else if (x >= scr_w - 20)
		{
			map_x = map_x + (DRAG_SENSITIVE / map_p);
			map_y = map_y + (DRAG_SENSITIVE  * (y * 2.0 / scr_h - 1) / map_p);
		}
		else if (y >= scr_h - 20)
		{
			map_x = map_x + (DRAG_SENSITIVE * (x * 2.0 / scr_w - 1) / map_p);
			map_y = map_y + (DRAG_SENSITIVE / map_p);
		}

		{
			if (scr_w / 2.0 * (1 / map_p - 1) > map_x)
			{
				map_x = (float)(scr_w / 2.0 * (1 / map_p - 1));
			}
			if (scr_h / 2.0 * (1 / map_p - 1) > map_y)
			{
				map_y = (float)(scr_h / 2.0 * (1 / map_p - 1));
			}
			if (map_w + 1.0 - scr_w / 2.0 *(1 + 1 / map_p) < map_x)
			{
				map_x = (float)(map_w + 1.0 - scr_w / 2.0 *(1 + 1 / map_p));
			}
			if (map_h + 1.0 - scr_h / 2.0 *(1 + 1 / map_p) < map_y)
			{
				map_y = (float)(map_h + 1.0 - scr_h / 2.0 *(1 + 1 / map_p));
			}
		}
	}

	//GUI EVENT
	for (auto I = gui.begin(); I != gui.end(); ++I)
	{
		if (I->enable)
		{
			if (I->rx <= x &&
				I->ry <= y &&
				I->rx + (int)I->w >= x &&
				I->ry + (int)I->h >= y)
			{

			}
		}
	}
}
void draw(SDL_Rect* r, SDL_Point* p)
{
		for (auto I = nat.begin(); I != nat.end(); ++I)
		{
			I->second.pnum = 0;
			I->second.px = 0;
			I->second.py = 0;
			I->second.pw = 0;
		}


		SDL_SetRenderDrawColor(REND, 0x09, 0x23, 0x66, 0xFF);
		SDL_RenderClear(REND);

		int i = 0;
		double lf = 0;
		
		double _map_x = map_x;
		double _map_y = map_y;
		double _map_p = map_p;

		for (unsigned int dep = 0; dep < 2; ++dep)
		{
			for (auto I = prv.begin(); I != prv.end(); ++I, ++i)
			{
				set_rect(r, (I->x1 - scr_w / 2.0 - _map_x) * _map_p + scr_w / 2.0, (I->y1 - scr_h / 2.0 - _map_y) * _map_p + scr_h / 2.0, (I->x2 - I->x1 + 1.0) * _map_p, (I->y2 - I->y1 + 1.0) * _map_p);
				if (I->enable)
				{
					if (dep == 1)
					{
						p->x = (int)(r->x + I->px * _map_p);
						p->y = (int)(r->y + I->py * _map_p);
						auto J = nat.at(I->var["OWN"]);
						nat.at(I->var["OWN"]).px = ((1.0 * J.px * J.pnum + p->x) / (J.pnum + 1));
						nat.at(I->var["OWN"]).py = ((1.0 * J.py * J.pnum + p->y) / (J.pnum + 1));
						nat.at(I->var["OWN"]).pw = ((1.0 * J.pw * J.pnum + I->pnum / (I->y2 - I->y1 + 1)) / (J.pnum + 1));
						nat.at(I->var["OWN"]).pnum++;

						/**/
					}
					if ((r->x + r->w > 0 || r->x < scr_w) && (r->y + r->h > 0 || r->y < scr_h))
					{
						if (dep == 0)
						{
							SDL_RenderCopy(REND, I->gt, NULL, r);
							//LOG_W(I->var["name"]);
							if (!I->waste_land && I->var["OWN"] != "NAV")
							{
								SDL_SetTextureColorMod(I->t, (Uint8)(nat.at(I->var["OWN"]).c / 65536), (Uint8)((nat.at(I->var["OWN"]).c / 256) % 256), (Uint8)(nat.at(I->var["OWN"]).c % 256));
								SDL_RenderCopy(REND, I->t, NULL, r);
								if (I->var["OWN"] != I->var["CON"])
								{
									SDL_SetTextureColorMod(I->lt, (Uint8)(nat.at(I->var["CON"]).c / 65536), (Uint8)((nat.at(I->var["CON"]).c / 256) % 256), (Uint8)(nat.at(I->var["CON"]).c % 256));
									SDL_RenderCopy(REND, I->lt, NULL, r);
								}
							}

						}
						if (dep == 1)
						{
							lf = (0.5f / get_chrs(I->var["name"]) * _map_p * I->pnum / (I->y2 - I->y1 + 1));
							if (_map_p >= 10)
							{
								TTF_SetFontOutline(tfx["default"].t, 2);
								draw_string("default", I->var["name"], "white", p, (unsigned int)lf, middle_align + center_align);
								TTF_SetFontOutline(tfx["default"].t, 0);
								draw_string("default", I->var["name"], "black", p, (unsigned int)lf, middle_align + center_align);
							}
						}
					}
				}
			}
		}
		
		if (_map_p < 10)
		{
			for (auto I = nat.begin(); I != nat.end(); ++I)
			{

				if (I->second.pnum > 0 && I->first != "REB" && I->first != "NAV")
				{
					set_point(p, I->second.px, I->second.py);
					//r->x = p->x - 5;
					//r->y = p->y - 5;
					//r->w = 10;
					//r->h = 10;
					//SDL_SetRenderDrawColor(REND, 0, 0, 255, 255);
					//SDL_RenderFillRect(REND, r);
					lf = 0.5 * I->second.pw * map_p / get_chrs(I->second.var["title"]) *  sqrt(sqrt((double)I->second.pnum));//  * _map_p ;
					if (_map_p > 1.7)
					{
						TTF_SetFontOutline(tfx["default"].t, 2);
						draw_string("default", I->second.var["title"], "white", p, (unsigned int)lf, middle_align + center_align);
						TTF_SetFontOutline(tfx["default"].t, 0);
					}

					draw_string("default", I->second.var["title"], "black", p, (unsigned int)lf, middle_align + center_align);
				}
			}
		}

		for (auto I : man)
		{
			if (I.second.var["live"] == "1")
			{
				if (gui_key.find("@leader_" + Str(I.first)) == gui_key.end())
				{
					Widget wd0 = Widget(0,0,0,0,wd_image, "@leader_" + Str(I.first));
					wd0.var["source"] = "flag\\" + I.second.var["CON"];
					wd0.ev.Enable_mousedown = true;
					wd0.ev.Event_mousedown = LittleUp_mousedown;
					gui.push_back(wd0);
				}
				auto J = prv.at(Num(I.second.var["LOC"]));
				set_rect(r, (J.x1 - scr_w / 2.0 - _map_x) * _map_p + scr_w / 2.0, (J.y1 - scr_h / 2.0 - _map_y) * _map_p + scr_h / 2.0, (J.x2 - J.x1 + 1.0) * _map_p, (J.y2 - J.y1 + 1.0) * _map_p);
				p->x = (int)(r->x + J.px * _map_p);
				p->y = (int)(r->y + J.py * _map_p);

				r->x = (int)(p->x - 3 * map_p + 0.5);
				r->y = (int)(p->y - 3 * map_p + 0.5);
				r->w = (int)(6 * map_p + 0.5);
				r->h = (int)(6 * map_p + 0.5);

				size_t gui_sec =  gui_key.at("@leader_" + Str(I.first));
				if ((r->x + r->w > 0 || r->x < scr_w) && (r->y + r->h > 0 || r->y < scr_h))
				{
					gui.at(gui_sec).enable = true;
					gui.at(gui_sec).x =r->x;
					gui.at(gui_sec).y = r->y;
					gui.at(gui_sec).w = (Uint16)r->w;
					gui.at(gui_sec).h = (Uint16)r->h;
				}
				else
				{
					gui.at(gui_sec).enable = false;
				}
			}
		}
}

void draw_item(std::vector<Widget>::iterator I, const SDL_Rect r, const unsigned char type)
{
	SDL_Point p;

	while (true)
	{
		if (type == wd_image)
		{
			//I->x++;
			if (I->var["source"] != "" && gfx[I->var["source"]].t != NULL)
			{
				SDL_RenderCopy(REND, gfx[I->var["source"]].t, NULL, &r);
			}
			else if (I->var["source"] != "None")
			{
				SDL_RenderCopy(REND, gfx["ui\\fuchsia-black"].t, NULL, &r);
			}
			break;
		}
		if (type == wd_label)
		{
			p.x = I->rx;
			p.y = I->ry;
			draw_string(I->var["ind"], I->var["text"], I->var["color"], &p, Num(I->var["size"]), Num(I->var["opt"]));
			break;
		}
		if (type == wd_text)
		{
			p.x = I->rx;
			p.y = I->ry;

			switch (Num(I->var["opt"]) / 3)
			{
			case bottom_align / 3:
				p.y -= Num(I->var["size"]) * (get_lines(I->var["text"], Num(I->var["line"])) - 1);
				break;
			case middle_align / 3:
				p.y -= Num(I->var["size"]) * (get_lines(I->var["text"], Num(I->var["line"])) - 1) / 2;
				break;
			}
			draw_line(I->var["ind"], I->var["text"], Num(I->var["line"]), I->var["color"], &p, Num(I->var["size"]), Num(I->var["opt"]));
			break;
		}
		SDL_RenderCopy(REND, gfx["ui\\black-white"].t, NULL, &r);
		break;
	}
}
void child_ui(unsigned int i, std::vector<Widget>::iterator I, SDL_Rect r)
{
	for (unsigned int j = 0; j < gui.size(); j++)
	{
		if (j != i)
		{
			auto J = (gui.begin() + j);
			if (J->parent == I->id && J->enable)
			{
				set_point(&r, I->rx, I->ry);
				J->rx = r.x + J->x;
				J->ry = r.y + J->y;
				set_rect(&r, J->rx, J->ry, J->w, J->h);
				draw_item(J,r,J->type);
				child_ui(j, J, r);
			}
		}
	}
}
void ui()
{
	SDL_Rect r;
	SDL_Point p;
	r.x = 0;
	p.y = 0;
	while (!quit)
	{
		draw(&r, &p);

		//GUI REMOVE
		for (bool go = true; go;)
		{
			go = false;
			for (size_t i = 0; i < gui.size(); i++)
			{
				auto I = (gui.begin() + i);
				if (I->removing)
				{
					gui_remove(i);
					go = true;
				}

			}
		}



		//GUI DRAWING
		{
			auto I = gui.begin();
			for (size_t i = 0; i < gui.size() && I != gui.end(); ++i, ++I)
			{
				if (I->id == I->parent && I->enable)
				{
					I->rx = I->x;
					I->ry = I->y;
					set_rect(&r, I->rx, I->ry, I->w, I->h);
					draw_item(I, r, I->type);
					child_ui(i, I, r);
				}

			}
		}

		//for (bool go = true; go;)
		//{
		//	go = false;
		//	for (unsigned i = 0; i < gui.size(); ++i)
		//	{
		//		auto I = (gui.begin() + i);
		//		if (I->removing)
		//		{
		//			gui_remove(i);
		//			go = true;
		//		}

		//	}
		//}

		SDL_RenderPresent(REND);
	}
}

bool init()
{
	setlocale(LC_ALL, "");
	std::ios_base::sync_with_stdio(true);
	//SetConsoleOutputCP(65001);
	


	char curDir[1024];
	_getcwd(curDir, 1024);
	executeDir = curDir;
	LOG_V("EXECUTE DIRECTORY", executeDir);
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		LOG_W("SDL_INIT Failed", SDL_GetError());
		return false;
	}
	WNDW = SDL_CreateWindow("통신", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, scr_w, scr_h, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
	if (WNDW == NULL)
	{
		LOG_W("WIN_INIT Failed", SDL_GetError());
		return false;
	}
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
	GLCN = SDL_GL_CreateContext(WNDW);
	if (GLCN == NULL)
	{
		LOG_W("GLC_INIT Failed", SDL_GetError());
		return false;
	}

	if (SDL_GL_SetSwapInterval(1) < 0)
	{
		LOG_W("CANT SET VSYNC");
	}


	if (Mix_OpenAudio(22050, AUDIO_S16SYS, 2, 640) < 0)
	{
		LOG_W("MIX_INIT Failed", Mix_GetError());
		return false;
	}
	SURF = SDL_GetWindowSurface(WNDW);
	REND = SDL_CreateRenderer(WNDW, -1, SDL_RENDERER_ACCELERATED);
	if (REND == NULL)
	{
		LOG_W("RND_INIT Failed", SDL_GetError());
		return false;
	}
	if (TTF_Init() == -1)
	{
		LOG_W("TTF_INIT Failed", TTF_GetError());
		return false;
	}
	return true;
}
bool loadMedia()
{
	bool success = true;

	read_folder(executeDir + "\\gfx","", read_gfx);
	read_folder(executeDir + "\\tfx", "", read_tfx);
	read_folder(executeDir + "\\ui", "", read_ui);
	read_folder(executeDir + "\\sfx\\music", "", read_sfx);
	read_folder(executeDir + "\\define", "", read_define);
	read_map();
	read_folder(executeDir + "\\history\\province", "", read_prv);
	read_folder(executeDir + "\\history\\nation", "", read_nat);
	read_folder(executeDir + "\\history\\character", "", read_man);
	return success;
}
void close()
{
	for (auto i : gfx)
	{
		SDL_DestroyTexture(i.second.t);
		SDL_FreeSurface(i.second.s);
		i.second.t = NULL;
		i.second.s = NULL;
	}
	for (auto i : sfx)
	{
		Mix_FreeMusic(i.second);
		i.second = NULL;
	}
	for (auto i : tfx)
	{
		TTF_CloseFont(i.second.t);
		i.second.t = NULL;
	}


	Mix_CloseAudio();
	SDL_DestroyRenderer(REND);
	SDL_DestroyWindow(WNDW);
	REND = NULL;
	WNDW = NULL;
	TTF_Quit();
	Mix_Quit();
	IMG_Quit();
	SDL_Quit();

	gfx.clear();
	sfx.clear();
	tfx.clear();
	prv.clear();
	nat.clear();
	gui.clear();
	Color.clear();
	gui_key.clear();
	key.clear();
	script.clear();
	map_reg.clear();
	man.clear();
}

#ifdef DEBUG

void LOG_A(std::string s)
{
	std::wcout << ESCAPE << L"[1;33m[Attention]" << ESCAPE << L"[1;37m " << Utf16(s) << std::endl;
}
void LOG_W(std::string s)
{
	std::wcout << ESCAPE << L"[1;31m[Warning]" << ESCAPE << L"[1;37m " << Utf16(s) << std::endl;
}
void LOG_H(std::string s)
{
	std::wcout << ESCAPE << L"[1;34m[Info]" << ESCAPE << L"[1;37m " << Utf16(s) << std::endl;
}
void LOG_O(std::string s)
{
	std::wcout << ESCAPE << L"[1;32m[OK]" << ESCAPE << L"[1;37m " << Utf16(s) << std::endl;
}
void LOG_V(std::string s)
{
	std::wcout << ESCAPE << L"[1;37m[OK]" << ESCAPE << L"[1;37m " << Utf16(s) << std::endl;
}
void LOG_A(std::string s, std::string s2)
{
	std::wcout << ESCAPE << L"[1;33m[Info]" << ESCAPE << L"[1;37m " << Utf16(s) << " : " << Utf16(s2) << std::endl;
}
void LOG_W(std::string s, std::string s2)
{
	std::wcout << ESCAPE << L"[1;31m[Warning]" << ESCAPE << L"[1;37m " << Utf16(s) << " : " << Utf16(s2) << std::endl;
}
void LOG_H(std::string s, std::string s2)
{
	std::wcout << ESCAPE << L"[1;34m[Info]" << ESCAPE << L"[1;37m " << Utf16(s) << " : " << Utf16(s2) << std::endl;
}
void LOG_O(std::string s, std::string s2)
{
	std::wcout << ESCAPE << L"[1;32m[OK]" << ESCAPE << L"[1;37m " << Utf16(s) << " : " << Utf16(s2) << std::endl;
}
void LOG_V(std::string s, std::string s2)
{
	std::wcout << ESCAPE << L"[1;37m[VAR]" << ESCAPE << L"[1;37m " << Utf16(s) << " : " << Utf16(s2) << std::endl; 
}
void LOG_Stop()
{
	std::wcout << L"PRESS ANY KEY...";
	std::wcin.ignore();
	return;
}
#else
void LOG_A(std::string s)
{
	logging << "[Attention] " << s << "\n";
}
void LOG_W(std::string s)
{
	logging << "[Warning] "  << s << "\n";
}
void LOG_H(std::string s)
{
	logging << "[Info] " << s << "\n";
}
void LOG_O(std::string s)
{
	logging << "[OK] "  << s << "\n";
}
void LOG_V(std::string s)
{
	logging << "[OK] " << s << "\n";
}
void LOG_A(std::string s, std::string s2)
{
	logging << "[Info] " << s << " : " << s2 << "\n";
}
void LOG_W(std::string s, std::string s2)
{
	logging << "[Warning] " << s << " : " << s2 << "\n";
}
void LOG_H(std::string s, std::string s2)
{
	logging << "[Info] " << s << " : " << s2 << "\n";
}
void LOG_O(std::string s, std::string s2)
{
	logging << "[OK] " << s << " : " << s2 << "\n";
}
void LOG_V(std::string s, std::string s2)
{
	logging << "[VAR] " << s << " : " << s2 << "\n";
}
void LOG_Stop()
{
	logging.close();
	return;
}
#endif