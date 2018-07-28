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
			std::thread trd_step(step);

			while (!quit)
			{
				ui(&e);
			}
			trd_step.join();
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
	//SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2");
	{
		Widget wd0(404,404,404,404, wd_image, "Error!");
		wd0.enable = false;
		wd0.id = (unsigned int)gui.size();
		wd0.parent = wd0.id;
		gui.push_back(wd0);
	}
	{
		Nation nt;
		nt.c = color(255, 255, 255);
		nat["NAV"] = nt;
	}
	{
		Nation nt;
		nt.c = color(0, 0, 0);
		nat["반란군"] = nt;
	}
	{
		Nation nt;
		nt.c = color(0, 0, 128);
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
	//Mix_PlayMusic(sfx["sample"], -1);
}
void step()
{
}
void draw(SDL_Rect* r, SDL_Point* p)
{
	if (map_p * map_w < scr_w)
	{
		map_p = 1.0f * scr_w / map_w;
	}
	if (map_p * map_h < scr_h)
	{
		map_p = 1.0f * scr_h / map_h;
	}

	if (scr_w / 2.0 * (1 / map_p - 1) > map_x )
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

	SDL_SetRenderDrawColor(REND, 0x09, 0x23, 0x66, 0xFF);
	SDL_RenderClear(REND);
	
	int i = 0;
	for (auto I = prv.begin(); I != prv.end(); ++I, ++ i)
	{
		set_rect(r, (I->x1 - scr_w / 2.0 - map_x) * map_p + scr_w / 2.0, (I->y1 - scr_h / 2.0 - map_y) * map_p + scr_h / 2.0, (I->x2 - I->x1 + 1.0) * map_p, (I->y2 - I->y1 + 1.0) * map_p);
		

		if (I->enable && (r->x + r->w > 0 || r->y + r->h > 0 || r->x < scr_w || r->y < scr_h))
		{
			SDL_RenderCopy(REND, I->gt, NULL, r);
			if (!I->waste_land)
			{
				SDL_SetTextureColorMod(I->t, (Uint8)(nat.at(I->var["OWN"]).c / 65536), (Uint8)((nat.at(I->var["OWN"]).c / 256) % 256), (Uint8)(nat.at(I->var["OWN"]).c % 256));
				//SDL_SetRenderDrawColor(REND, (Uint8)(nat.at(I->var["OWN"]).c / 65536), (Uint8)((nat.at(I->var["OWN"]).c / 256) % 256), (Uint8)(nat.at(I->var["OWN"]).c % 256), 255);
				//SDL_RenderFillRect(REND, r);
				SDL_RenderCopy(REND, I->t, NULL, r);
				if (I->var["OWN"] != I->var["CON"])
				{
					SDL_SetTextureColorMod(I->lt, (Uint8)(nat.at(I->var["CON"]).c / 65536), (Uint8)((nat.at(I->var["CON"]).c / 256) % 256), (Uint8)(nat.at(I->var["CON"]).c % 256));
					SDL_RenderCopy(REND, I->lt, NULL, r);
				}
				p->x = r->x + r->w / 2;
				p->y = r->y + r->h / 2;
				draw_string("gulim", Str(i) + " : " + I->var["name"], "black", p, 8 * map_p, 0.6, middle_align + center_align);
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
			draw_string(I->var["ind"], I->var["text"], I->var["color"], &p, Num(I->var["size"]), std::stof(I->var["ratio"]), Num(I->var["opt"]));
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
			draw_line(I->var["ind"], I->var["text"], Num(I->var["line"]), I->var["color"], &p, Num(I->var["size"]), std::stof(I->var["ratio"]), Num(I->var["opt"]));
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
void ui(SDL_Event *e)
{
	SDL_Rect r;
	SDL_Point p;
	int x, y;
	r.x = 0;
	p.y = 0;
	draw(&r, &p);

	while (SDL_PollEvent(e) != 0)
	{
		switch (e->type)
		{
		case SDL_QUIT: {
			quit = true;
			return;
		}
		case SDL_KEYDOWN: {
			switch (e->key.keysym.sym)
			{
			case SDLK_ESCAPE: {
				quit = true;
				break;
			}
			case SDLK_PAGEUP: {
				map_p *= 0.9;
				break;
			}
			case SDLK_PAGEDOWN: {
				map_p *= 1.1;
				break;
			}
			case SDLK_a: {
				map_x -= 5;
				break;
			}
			case SDLK_d: {
				map_x += 5;
				break;
			}
			case SDLK_w: {
				map_y -= 5;
				break;
			}
			case SDLK_s: {
				map_y += 5;
				break;
			}
			}
		}
		}

		SDL_GetMouseState(&x, &y);

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
		
		for (bool go = true; go;)
		{
			go = false;
			for (auto i = 0; i < gui.size(); i++)
			{
				auto I = (gui.begin() + i);
				if (I->removing)
				{
					gui_remove(i);
					go = true;
				}

			}
		}

	}

	
	{
		auto I = gui.begin();
		for (unsigned int i = 0; i < gui.size() && I != gui.end(); ++i, ++I)
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

	for (bool go = true; go;)
	{
		go = false;
		for (unsigned i = 0; i < gui.size(); ++i)
		{
			auto I = (gui.begin() + i);
			if (I->removing)
			{
				gui_remove(i);
				go = true;
			}

		}
	}

	SDL_RenderPresent(REND);
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
		LOG_W("SDL_INIT Failed : ", SDL_GetError());
		return false;
	}
	WNDW = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, scr_w, scr_h, SDL_WINDOW_SHOWN);
	if (WNDW == NULL)
	{
		LOG_W("WIN_INIT Failed : ", SDL_GetError());
		return false;
	}
	if (Mix_OpenAudio(22050, AUDIO_S16SYS, 2, 640) < 0)
	{
		LOG_W("MIX_INIT Failed : ", Mix_GetError());
		return false;
	}
	SURF = SDL_GetWindowSurface(WNDW);
	REND = SDL_CreateRenderer(WNDW, -1, SDL_RENDERER_ACCELERATED);
	if (REND == NULL)
	{
		LOG_W("RND_INIT Failed : ", SDL_GetError());
		return false;
	}
	if (TTF_Init() == -1)
	{
		LOG_W("TTF_INIT Failed : ", TTF_GetError());
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
}

#ifdef DEBUG

void LOG_A(std::string s)
{
	std::cout << ESCAPE << "[1;33m[Attention]" << ESCAPE << "[1;37m " << s << "\n";
}
void LOG_W(std::string s)
{
	std::cout << ESCAPE << "[1;31m[Warning]" << ESCAPE << "[1;37m " << s << "\n";
}
void LOG_H(std::string s)
{
	std::cout << ESCAPE << "[1;34m[Info]" << ESCAPE << "[1;37m " << s << "\n";
}
void LOG_O(std::string s)
{
	std::cout << ESCAPE << "[1;32m[OK]" << ESCAPE << "[1;37m " << s << "\n";
}
void LOG_V(std::string s)
{
	std::cout << ESCAPE << "[1;37m[OK]" << ESCAPE << "[1;37m " << s << "\n";
}
void LOG_A(std::string s, std::string s2)
{
	std::cout << ESCAPE << "[1;33m[Info]" << ESCAPE << "[1;37m " << s << " : " << s2 << "\n";
}
void LOG_W(std::string s, std::string s2)
{
	std::cout << ESCAPE << "[1;31m[Warning]" << ESCAPE << "[1;37m " << s << " : " << s2 << "\n";
}
void LOG_H(std::string s, std::string s2)
{
	std::cout << ESCAPE << "[1;34m[Info]" << ESCAPE << "[1;37m " << s << " : " << s2 << "\n";
}
void LOG_O(std::string s, std::string s2)
{
	std::cout << ESCAPE << "[1;32m[OK]" << ESCAPE << "[1;37m " << s << " : " << s2 << "\n";
}
void LOG_V(std::string s, std::string s2)
{
	std::cout << ESCAPE << "[1;37m[VAR]" << ESCAPE << "[1;37m " << s << " : " << s2 << "\n";
}
void LOG_Stop()
{
	std::string a;
	std::cout << "PRESS ANY KEY...";
	std::cin.ignore();
	return;
}
#else
void LOG_A(std::string) {};
void LOG_W(std::string) {};
void LOG_H(std::string) {};
void LOG_O(std::string) {};
void LOG_V(std::string) {};
void LOG_A(std::string, std::string) {};
void LOG_W(std::string, std::string) {};
void LOG_H(std::string, std::string) {};
void LOG_O(std::string, std::string) {};
void LOG_V(std::string, std::string) {};
void LOG_Stop() {};
#endif