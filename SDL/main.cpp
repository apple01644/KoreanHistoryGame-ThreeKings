#include "main.h"

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

void day_over()
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
		}
		else if (day > 29 && is_yun)
		{
			day = 1;
			mon++;
		}
	}
	else if ((mon % 2 == 1 && mon <= 7) || (mon % 2 == 0 && mon >= 8))
	{
		if (day > 31)
		{
			day = 1;
			mon++;
		}
	}
	else if (day > 30)
	{
		day = 1;
		mon++;
	}
	if (mon > 12)
	{
		mon = 1;
		year++;
	}

}
void file_proc(int id, std::string s1, std::string s2)
{
	if (s1 == "color")
	{
		for (int i = 0; i < s2.length(); i++)
		{
			if (s2.at(i) == '(')
			{
				s2 = s2.substr(i + 1);
				break;
			}
		}
		for (int i = 0; i < s2.length(); i++)
		{
			if (s2.at(i) == ')')
			{
				s2 = s2.substr(0,i);
				break;
			}
		}
		std::string buf = "";
		int j = 0;
		int rgb[3] = { 0 };
		for (int i = 0; i < s2.length(), j < 3; i++)
		{
			if (s2.at(i) == ',')
			{
				rgb[j] = std::stoi(buf);
				j++;
				buf = "";
			}
			else
			{
				buf += s2.at(i);
			}
			if (i == s2.length() - 1)
			{
				rgb[j] = std::stoi(buf);
				buf = "";
				j++;
			}

		}
		prv[id].c = color(rgb[0], rgb[1], rgb[2]);
		return;
	}
	if (s1 == "man")
	{
		prv[id].man = std::stoi(s2);
		return;
	}
	if (s1 == "clt")
	{
		prv[id].clt = std::stoi(s2);
		return;
	}
	if (s1 == "ind")
	{
		prv[id].ind = std::stoi(s2);
		return;
	}
	if (s1 == "eco")
	{
		prv[id].eco = std::stoi(s2);
		return;
	}
	if (s1 == "scope")
	{
		scope = s2;
		return;
	}
	if (s1 == "index")
	{
		tmp[0] = std::stoi(s2);
		return;
	}
	if (s1 == "pop.support")
	{
		prv[id].pop[tmp[0]].support = std::stoi(s2);
		return;
	}
	if (s1 == "pop.i8ssue")
	{
		prv[id].pop[tmp[0]].issue = std::stoi(s2);
		return;
	}
	if (s1 == "pop.income")
	{
		prv[id].pop[tmp[0]].income = std::stoi(s2);
		return;
	}
}
void file_prov_read(std::string path)
{
	std::ifstream in(path.c_str());
	int id = -1;
	for (int i = 0; i < 16; i++)
	{
		tmp[i] = 0;
	}
	scope = "";
	for (int j = path.length() - 1; j > 0 ; j--)
	{
		if (path.at(j) == '\\')
		{
			for (int k = j + 1; k < path.length(); k++)
			{
				if (path.at(k) == '-')
				{
					id = std::stoi(path.substr(j + 1, k - j - 1)) - 1;
					prv[id].name = path.substr(k + 1);
				}
			}
			break;
		}
	}
	if (id == -1)
	{
		LOG_W("Unknown File name", path);
		return;
	}
	if (in.is_open())
	{
		int i = 0;
		for (std::string line; std::getline(in, line); i++)
		{
			if (i == 0)
			{
				line = line.substr(3);
			}
			for (int j = 0; j < line.length(); j++)
			{
				if (line.at(j) == '=')
				{
					file_proc(id, line.substr(0, j), line.substr(j + 1, line.length() - j - 1));
					break;
				}
			}
			LOG_H(line);
		}
		in.close();
		LOG_O("Succeed Reading of ", path);
	}
	else
	{
		LOG_W("Failed Reading of ", path);
	}
}

void start()
{
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2");
	srand(time(NULL));
	

	for (int i = 0; i < MAX_PROV; i++)
	{
		for (int j = 0; j < prv[i].man * 3; j++)
		{
			Pop p;
			prv[i].pop.push_back(p);
		}
	}

	std::string path = currentDir+"\\"+"map\\provinces";
	for (auto p : std::experimental::filesystem::directory_iterator(path))
	{
		char* pStr;
		int strSize = WideCharToMultiByte(CP_ACP, 0, p.path().c_str(), -1, NULL, 0, NULL, NULL);
		pStr = new char[strSize];
		WideCharToMultiByte(CP_ACP, 0, p.path().c_str(), -1, pStr, strSize, 0, 0);
		file_prov_read(pStr);
	}
	for (int i = 0; i < MAX_SPRITE; i++)
	{
		if (sprite_surf[i] == NULL)
		{
			NUM_SPR = i;
			break;
		}
	}
	LOG_H("NUM_SPR : ", std::to_string(NUM_SPR));

	int w, h;

	w = sprite_surf[0]->w;
	h = sprite_surf[0]->h;
	unsigned char* pixels = (unsigned char*)sprite_surf[0]->pixels;

	for (int a = 0; a < w; a++)
	{
		for (int b = 0; b < h; b++)
		{
			for (int i = 0; i < MAX_PROV; i++)
			{
				if (color(pixels[3 * (b * w + a) + 2], pixels[3 * (b * w + a) + 1], pixels[3 * (b * w + a)]) == prv[i].c)
				{
					if (a < prv[i].x1)
					{
						prv[i].x1 = a;
					}
					if (a > prv[i].x2)
					{
						prv[i].x2 = a;
					}
					if (b < prv[i].y1)
					{
						prv[i].y1 = b;
					}
					if (b > prv[i].y2)
					{
						prv[i].y2 = b;
					}
				}
			}
		}
	}



	for (int i = 0; i < MAX_PROV; i++)
	{
		if (prv[i].x1 <= prv[i].x2 &&  prv[i].y1 <= prv[i].y2)
		{
			SDL_Surface* prov = SDL_CreateRGBSurfaceWithFormat(0, prv[i].x2 - prv[i].x1 + 1, prv[i].y2 - prv[i].y1 + 1, 24, SDL_PIXELFORMAT_BGR888);
			int w_t, h_t;

			w_t = prov->w;
			h_t = prov->h;
			unsigned char* pixels_t = (unsigned char*)prov->pixels;


			int c, d;
			for (int a = 0; a <= w_t; a++)
			{
				for (int b = 0; b <= h_t; b++)
				{
					c = a + prv[i].x1;
					d = b + prv[i].y1;
					if (color(pixels[3 * (d * w + c) + 2], pixels[3 * (d * w + c) + 1], pixels[3 * (d * w + c)]) == prv[i].c)
					{
						pixels_t[((a)+(w_t) * (b)) * 4 + 0] = 255;
						pixels_t[((a)+(w_t) * (b)) * 4 + 1] = 255;
						pixels_t[((a)+(w_t) * (b)) * 4 + 2] = 255;
					}
				}
			}

			for (int a = prv[i].x1; a <= prv[i].x2; a++)
			{
				break;

				for (int b = prv[i].y1; b <= prv[i].y2; b++)
				{
					if (color(pixels[4 * (b * w + a) + 2], pixels[4 * (b * w + a) + 1], pixels[4 * (b * w + a)]) == prv[i].c)
					{
						pixels_t[((a - prv[i].x1) + (w_t) * (b - prv[i].y1)) * 4 + 0] = 255;
						pixels_t[((a - prv[i].x1) + (w_t) * (b - prv[i].y1)) * 4 + 1] = 255;
						pixels_t[((a - prv[i].x1) + (w_t) * (b - prv[i].y1)) * 4 + 2] = 255;
					}
					else
					{
						pixels_t[((a - prv[i].x1) + (w_t) * (b - prv[i].y1)) * 4 + 0] = 0;
						pixels_t[((a - prv[i].x1) + (w_t) * (b - prv[i].y1)) * 4 + 1] = 0;
						pixels_t[((a - prv[i].x1) + (w_t) * (b - prv[i].y1)) * 4 + 2] = 0;
					}
				}
			}
			SDL_SetColorKey(prov,SDL_TRUE, 0);
			prv[i].t = SDL_CreateTextureFromSurface(REND, prov);
		}
	}



	party[0].name = "자유 민주 연합";
	party[0].facism = 10;
	party[0].liberty = 60; 
	party[0].c = color(74,143,0);

	party[1].name = "한나라당";
	party[1].facism = 30;
	party[1].liberty = 40;
	party[1].c = color(0,131,203);

	party[2].name = "새천년 민주당";
	party[2].facism = -10;
	party[2].liberty = 10;
	party[2].c = color(0,153,153);

	party[3].name = "민주 노동당";
	party[3].facism = -30;
	party[3].liberty = 10;
	party[3].c = color(255,102,0);

	party[4].name = "사회당";
	party[4].facism = -60;
	party[4].liberty = -30;
	party[4].c = color(0,0,0);




}
void step()
{
	while(!quit)
	{
		if (delay > 200000000)
		{
			day_over();
			delay = 0;
		}
		delay++;
		//map_step();
	}
	return;
}
void draw()
{
	SDL_SetRenderDrawColor(REND, 0x09, 0x23, 0x66, 0xFF);
	SDL_RenderClear(REND);

	SDL_Rect r;
	SDL_Texture* text;
	double per = 0.66;
	double wper = per * (scr_w / 1920.0);
	double hper = per * (scr_h / 1080.0);

	int max = 0;
	int min = -(~max);

	if (map_mode == 1)
	{
		for (int i = 0; i < MAX_PROV; i++)
		{
			if (min > prv[i].man + prv[i].clt + prv[i].ind + prv[i].eco)
			{
				min = prv[i].man + prv[i].clt + prv[i].ind + prv[i].eco;
			}
			if (max < prv[i].man + prv[i].clt + prv[i].ind + prv[i].eco)
			{
				max = prv[i].man + prv[i].clt + prv[i].ind + prv[i].eco;
			}
		}
	}
	double D = 0;

	for (int i = 0; i < MAX_PROV; i++)
	{
		set_rect(&r, prv[i].x1 * wper + 840 * wper, prv[i].y1 * hper, (prv[i].x2 - prv[i].x1 + 1) * wper, (prv[i].y2 - prv[i].y1 + 1) * hper);

		if (map_mode == 0)
		{
			SDL_SetTextureColorMod(prv[i].t, prv[i].c / 65536, (prv[i].c / 256) % 256, prv[i].c % 256);
		}
		else if (map_mode == 1)
		{
			D = 255 * ((1.0 * prv[i].man + prv[i].clt + prv[i].ind + prv[i].eco - min) / (max - min));
			SDL_SetTextureColorMod(prv[i].t, 255 - D, D, 0);
		}
		else
		{
			SDL_SetTextureColorMod(prv[i].t, prv[i].c / 65536, (prv[i].c / 256) % 256, prv[i].c % 256);
		}

		SDL_RenderCopy(REND, prv[i].t, NULL, &r);
	}

	SDL_SetRenderDrawColor(REND, 0x40, 0x40, 0x40, 0xFF);
	//Potrait
	set_rect(&r, 0, 0, 144, 144);
	SDL_RenderFillRect(REND, &r);

	//Left Menu
	set_rect(&r, 0, 144, 48, 48 * 6);
	SDL_RenderCopy(REND, sprite[17], NULL, &r);

	//Left menu Items
	for (int i = 0; i < 6; i++)
	{
		set_rect(&r, 4, 144 + 48 * i + 4, 40, 40);
		SDL_RenderCopy(REND, sprite[1], NULL, &r);
	}

	set_rect(&r, 12, 144 + 12, 24, 24);
	SDL_RenderCopy(REND, sprite[8], NULL, &r);

	set_rect(&r, 12, 144 + 48 * 1 + 12, 24, 24);
	SDL_RenderCopy(REND, sprite[6], NULL, &r);

	set_rect(&r, 12, 144 + 48 * 2 + 12, 24, 24);
	SDL_RenderCopy(REND, sprite[7], NULL, &r);

	set_rect(&r, 12, 144 + 48 * 3 + 12, 24, 24);
	SDL_RenderCopy(REND, sprite[9], NULL, &r);

	set_rect(&r, 12, 144 + 48 * 4 + 12, 24, 24);
	SDL_RenderCopy(REND, sprite[5], NULL, &r);

	set_rect(&r, 12, 144 + 48 * 5 + 12, 24, 24);
	SDL_RenderCopy(REND, sprite[10], NULL, &r);


	set_rect(&r, scr_w - 244 + 8, 32 + 8, 24, 24);
	SDL_RenderCopy(REND, sprite[12], NULL, &r);


	//Left menu Trailer
	set_rect(&r, 0, 144 + 48 * 6, 24, 32);
	SDL_RenderCopy(REND, sprite[15], NULL, &r);
	set_rect(&r, 24, 144 + 48 * 6 , 24, 32);
	SDL_RenderCopy(REND, sprite[16], NULL, &r);

	//Rigth-hand Info
	set_rect(&r, 144, 0, 100, 144);
	SDL_RenderDrawRect(REND, &r);
	int right_hand_items = 4;
	for (int i = 0; i < right_hand_items; i++)
	{
		set_rect(&r, 144, (i * 144 / right_hand_items), 100, 144 / right_hand_items);
		SDL_RenderCopy(REND, sprite[13], NULL, &r);
	}

	//Right Heard
	set_rect(&r, scr_w - 244, 32, 244, 176);
	SDL_RenderCopy(REND, sprite[13], NULL, &r);

	//Right heard Open
	set_rect(&r, scr_w - 244, 168, 40, 40);
	SDL_RenderCopy(REND, sprite[1], NULL, &r);

	set_rect(&r, scr_w - 244 + 8, 168 + 8, 24, 24);
	SDL_RenderCopy(REND, sprite[11], NULL, &r);

	//Right heard Close
	set_rect(&r, scr_w - 244, 32, 40, 40);
	SDL_RenderCopy(REND, sprite[1], NULL, &r);

	set_rect(&r, scr_w - 244 + 8, 40, 24, 24);
	SDL_RenderCopy(REND, sprite[12], NULL, &r);

	//Issue & News
	set_rect(&r, scr_w / 2 - 400, scr_h - 20, 800, 20);
	SDL_RenderDrawRect(REND, &r);


	//Minimap Header
	set_rect(&r, scr_w - 300, scr_h - 240, 300, 30);
	int minimap_header_items = 8;
	for (int i = 0; i < minimap_header_items; i++)
	{
		set_rect(&r, scr_w - 300 + (300.0 * i / minimap_header_items), scr_h - 240, 300 / minimap_header_items, 30);
		SDL_RenderCopy(REND, sprite[13], NULL, &r);
	}
	set_rect(&r, scr_w - 300 + (300 / minimap_header_items - 25) / 2, scr_h - 240 + 3, 25 , 25);
	SDL_RenderCopy(REND, sprite[18], NULL, &r);
	set_rect(&r, scr_w + (300.0 * 1 / minimap_header_items) - 300 + (300 / minimap_header_items - 25) / 2, scr_h - 240 + 3, 25, 25);
	SDL_RenderCopy(REND, sprite[19], NULL, &r);

	//Minimap
	set_rect(&r, scr_w - 300, scr_h - 210, 300, 170);
	SDL_RenderCopy(REND, sprite[13], NULL, &r);

	//Minimap Trailer
	set_rect(&r, scr_w - 300, scr_h - 40, 300, 40);
	SDL_RenderDrawRect(REND, &r);
	int minimap_trailer_items = 3;
	for (int i = 0; i < minimap_trailer_items; i++)
	{
		set_rect(&r, scr_w - 300 + (i * 300.0 / minimap_trailer_items), scr_h - 40, 300 / minimap_trailer_items, 40);
		if (i == 0)
		{
			SDL_RenderCopy(REND, sprite[2], NULL, &r);
		}
		else if (i == 1)
		{
			SDL_RenderCopy(REND, sprite[3], NULL, &r);
		}
		else if (i == 2)
		{
			SDL_RenderCopy(REND, sprite[4], NULL, &r);
		}
		SDL_RenderDrawRect(REND, &r);
	}

	//Timer
	set_rect(&r, scr_w - 244, 0, 244, 32);
	SDL_RenderCopy(REND, sprite[14], NULL, &r);
	set_rect(&r, scr_w - 244 + 26, 4, 182, 20);
	char buf[320] = {0,};
	sprintf_s(buf, "%d년 %02d월 %02d일\n",year,mon,day);
	text = draw_string(fonts[0], buf, &c_white);
	SDL_RenderCopy(REND, text, NULL, &r);




	SDL_SetRenderDrawColor(REND, 0xFF, 0x00, 0xFF, 0xFF);

	//Potrait
	set_rect(&r, 8, 8, 128, 128);
	SDL_RenderDrawRect(REND, &r);

	

	SDL_DestroyTexture(text);
	return;
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
				case SDLK_r: {
					//map_init();
					break;
				}
				}
			}
		}
	}

	SDL_RenderPresent(REND);
	
}


SDL_Texture* draw_string(TTF_Font* font, std::string s, SDL_Color* color) {

	SDL_Texture* t = SDL_CreateTextureFromSurface(REND, TTF_RenderUTF8_Solid(font, s.c_str(), *color));
	
	return t;
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

bool init()
{
	setlocale(LC_ALL, "");
	char curDir[1000];
	_getcwd(curDir, 1000);
	currentDir = curDir;
	LOG_H("This Programm run in ", currentDir);
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
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
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
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
	for (int a = 0; a < MAX_FONT; a++)
	{
		if (font_file[a] != "")
		{
			fonts[a] = TTF_OpenFont((currentDir + "\\" + font_file[a]).c_str(), font_size[a]);
			if (fonts[a] == NULL)
			{
				LOG_W("FONT_FILE is CORRUPT in " + std::to_string(a) + "\n" + currentDir + "\\" + font_file[a] + "\n");
				success = false;
			}
			else
			{
				LOG_O(("FONT_FILE is LOADED in " + std::to_string(a)));
			}
		}
		else
		{
			LOG_A(("FONT_FILE is EMPTY in " + std::to_string(a)));
		}
	}

	gMusic = Mix_LoadMUS((currentDir + "\\" + "music\\test.wav").c_str());
	if (gMusic == NULL)
	{
		LOG_W("MUSIC_FILE is CORRUPT in " + currentDir + "music\\test.wav");
	}

	for (int a = 0; a < MAX_SPRITE; a++)
	{
		if (sprite_file[a] != "")
		{
			sprite_surf[a] = IMG_Load((currentDir + "\\" + sprite_file[a]).c_str());
			if (sprite_surf[a] == NULL)
			{
				LOG_W("SPRITE_FILE is CORRUPT in " + std::to_string(a) + "\n" + currentDir + "\\" + sprite_file[a] + "\n");
				success = false;
			}
			else
			{
				//SDL_SetColorKey(sprite_surf[2], SDL_TRUE, SDL_MapRGB(sprite_surf[2]->format, 0, 0, 0));
				SDL_SetColorKey(sprite_surf[a], SDL_TRUE, SDL_MapRGB(sprite_surf[a]->format, 255, 0, 255));
				LOG_O(("SPRITE_FILE is LOADED in " + std::to_string(a)));

				sprite[a] = SDL_CreateTextureFromSurface(REND, sprite_surf[a]);
				if (sprite[a] == NULL)
				{
					LOG_W("FAILED MAKE TEXTURE in " + std::to_string(a), SDL_GetError());
					success = false;
				}

			}
		}
		else
		{
			LOG_A(("SPRITE_FILE is EMPTY in " + std::to_string(a)));
		}
	}
	return success;
}
void close()
{
	Mix_FreeMusic(gMusic);



	for (int a = 0; a < MAX_FONT; a++)
	{
		if (fonts[a] != NULL)
		{
			TTF_CloseFont(fonts[a]);
			fonts[a] = NULL;
		}
	}
	for (int a = 0; a < MAX_SPRITE; a++)
	{
		if (sprite[a] != NULL)
		{
			SDL_DestroyTexture(sprite[a]);
			sprite[a] = NULL;
		}
		if (texture[a] != NULL)
		{
			SDL_DestroyTexture(texture[a]);
			texture[a] = NULL;
		}
		if (sprite_surf[a] != NULL)
		{
			SDL_FreeSurface(sprite_surf[a]);
		}
	}

	SDL_DestroyRenderer(REND);
	SDL_DestroyWindow(WNDW);
	REND = NULL;
	WNDW = NULL;
	TTF_Quit();
	Mix_Quit();
	IMG_Quit();
	SDL_Quit();
}

Uint32 color(int r, int g, int b)
{
	return r * 65536 + g * 256 + b;
}

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
	std::cout << ESCAPE << "[1;33m[OK]" << ESCAPE << "[1;37m " << s << " : " << s2 << "\n";
}


void LOG_A(std::wstring s)
{
	std::wcout << ESCAPE << L"[1;33m[Attention]" << ESCAPE << L"[1;37m " << s << L"\n";
}
void LOG_W(std::wstring s)
{
	std::wcout << ESCAPE << L"[1;31m[Warning]" << ESCAPE << L"[1;37m " << s << L"\n";
}
void LOG_H(std::wstring s)
{
	std::wcout << ESCAPE << L"[1;34m[Info]" << ESCAPE << L"[1;37m " << s << L"\n";
}
void LOG_O(std::wstring s)
{
	std::wcout << ESCAPE << L"[1;32m[OK]" << ESCAPE << L"[1;37m " << s << L"\n";
}
void LOG_A(std::wstring s, std::wstring s2)
{
	std::wcout << ESCAPE << L"[1;33m[Info]" << ESCAPE << L"[1;37m " << s << " : " << s2 << L"\n";
}
void LOG_W(std::wstring s, std::wstring s2)
{
	std::wcout << ESCAPE << L"[1;31m[Warning]" << ESCAPE << L"[1;37m " << s << " : " << s2 << L"\n";
}
void LOG_H(std::wstring s, std::wstring s2)
{
	std::wcout << ESCAPE << L"[1;34m[Info]" << ESCAPE << L"[1;37m " << s << " : " << s2 << L"\n";
}
void LOG_O(std::wstring s, std::wstring s2)
{
	std::wcout << ESCAPE << L"[1;33m[OK]" << ESCAPE << L"[1;37m " << s << " : " << s2 << L"\n";
}


void LOG_Stop()
{
	std::string a;
	std::cout << "Press Any Key to Continue...";
	std::cin.ignore();
	return;
}