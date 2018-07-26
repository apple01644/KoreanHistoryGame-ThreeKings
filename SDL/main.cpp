#include "useful_func.h"

int main(int argc, char* args[])
{
	srand(time(NULL));
	system("color 01");
	
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
			SDL_Event e;
			std::thread trd_step(step);

			while (!quit)
			{
				ui(&e);
			}
			trd_step.join();
			
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
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2");
	srand(time(NULL));

	{
		Widget wd0(300,30,50,80, wd_image, "@ui\\nope");
		wd0.enable = true;
		wd0.var["source"] = script["nanana"];
		wd0.id = gui.size();
		wd0.parent = wd0.id;
		gui.push_back(wd0);
	}

	//Mix_PlayMusic(sfx["sample"], -1);
}
void step()
{
}
void draw()
{
	SDL_SetRenderDrawColor(REND, 0x09, 0x23, 0x66, 0xFF);
	SDL_RenderClear(REND);
	SDL_Rect r;
	SDL_Point p;

	/*for (auto i : prv)
	{
		set_rect(&r, prv.x1 + 840, prv.y1, (prv.x2 - prv.x1 + 1), (prv.y2 - prv.y1 + 1));

		{
			SDL_SetTextureColorMod(prv[i].t, prv.c / 65536, (prv.c / 256) % 256, prv.c % 256);
		}

		SDL_RenderCopy(REND, prv.t, NULL, &r);
	}*/

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
			draw_string(std::stoi(I->var["ind"]), I->var["text"], Color[I->var["color"]], &p, std::stoi(I->var["size"]), std::stof(I->var["ratio"]), std::stoi(I->var["opt"]));
			break;
		}
		if (type == wd_text)
		{
			p.x = I->rx;
			p.y = I->ry;

			switch (std::stoi(I->var["opt"]) / 3)
			{
			case bottom_align / 3:
				p.y -= std::stoi(I->var["size"]) * (get_lines(I->var["text"], std::stoi(I->var["line"])) - 1);
				break;
			case middle_align / 3:
				p.y -= std::stoi(I->var["size"]) * (get_lines(I->var["text"], std::stoi(I->var["line"])) - 1) / 2;
				break;
			}
			draw_line(std::stoi(I->var["ind"]), I->var["text"], std::stoi(I->var["line"]), Color[I->var["color"]], &p, std::stoi(I->var["size"]), std::stof(I->var["ratio"]), std::stoi(I->var["opt"]));
			break;
		}
		SDL_RenderCopy(REND, gfx["ui\\black-white"].t, NULL, &r);
		break;
	}
}
void child_ui(unsigned int i, std::vector<Widget>::iterator I, SDL_Rect r)
{
	for (int j = 0; j < gui.size(); j++)
	{
		if (j != i)
		{
			auto J = (gui.begin() + j);
			if (J->parent == I->id && J->enable)
			{
				set_rect(&r, I->rx, I->ry);
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
	int x, y;
	draw();

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
			}
		}
		}

		SDL_GetMouseState(&x, &y);

		for (auto i = 0; i < gui.size(); i++)
		{
			auto I = *(gui.begin() + i);
			if (I.enable)
			{
				if (I.rx <= x &&
					I.ry <= y &&
					I.rx + I.w >= x &&
					I.ry + I.h >= y)
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
	read_folder(executeDir + "\\ui", "", read_ui);
	read_folder(executeDir + "\\sfx\\music", "", read_sfx);
	read_folder(executeDir + "\\define", "", read_define);

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
#endif