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
}

void normal_start()
{
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
}

void draw_item(std::vector<Widget>::iterator I, const SDL_Rect r, const unsigned char type)
{

	if (type == wd_none)
	{
		SDL_RenderCopy(REND, gfx["ui\\black-white"].t, NULL, &r);
		return;
	}
	if (type == wd_image)
	{
		if (I->var["img"] != "" && gfx[I->var["img"]].t != NULL)
		{
			SDL_RenderCopy(REND, gfx[I->var["img"]].t, NULL, &r);
		}
		else if (I->var["img"] != "None")
		{
			SDL_RenderCopy(REND, gfx["ui\\fuchsia-black"].t, NULL, &r);
		}
		return;
	}
	if (type == wd_label)
	{
		SDL_Point p;
		p.x = I->rx;
		p.y = I->ry;
		draw_string(std::stoi(I->var["ind"]), I->var["text"], Color[I->var["color"]], &p, std::stoi(I->var["size"]), std::stof(I->var["ratio"]), std::stoi(I->var["opt"]));
		return;
	}
	if (type == wd_text)
	{
		SDL_Point p;
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
		return;
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

	SDL_RenderPresent(REND);
}

void read_code(std::string path)
{
	//std::filesystem a;
	
	//std::filesystem::path p(name);
	//std::filesystem::directory_iterator start(p);
	//std::filesystem::directory_iterator end;
	//std::transform(start, end, std::back_inserter(v), path_leaf_string());
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

	read_code("./common");





	return true;
}
bool loadMedia()
{
	SDL_Surface* Logo = IMG_Load((executeDir + "\\gfx\\ui\\logo.bmp").c_str());
	SDL_Surface* Back = IMG_Load((executeDir + "\\gfx\\ui\\right_heard_body.bmp").c_str());
	SDL_SetColorKey(Logo, SDL_TRUE, color(255, 0, 255));
	SDL_Texture* Logo_t = SDL_CreateTextureFromSurface(REND,Logo);
	SDL_Texture* Back_t = SDL_CreateTextureFromSurface(REND, Back);

	SDL_Rect r;
	set_rect(&r, scr_w / 2 - 512, scr_h / 2 - 512, 1024, 1024);

	SDL_RenderCopy(REND, Back_t, NULL, NULL);
	SDL_RenderCopy(REND, Logo_t, NULL, &r);

	SDL_RenderPresent(REND);
	SDL_FreeSurface(Logo);
	SDL_FreeSurface(Back);
	SDL_DestroyTexture(Logo_t);
	SDL_DestroyTexture(Back_t);


	bool success = true;
	
	int num = 0;
	std::string path , tag;

	return success;
}
void close()
{
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