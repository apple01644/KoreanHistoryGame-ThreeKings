#include "main2.h"

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
	if (s2 == "{")
	{
		scope.push_back(s1);
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
	scope.clear();
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
					for (int l = k + 2; l < path.length(); l++)
					{
						if (path.at(l) == '.')
						{
							prv[id].name = path.substr(k + 1, l - k - 1);
							break;
						}
					}
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
		bool used = false;
		for (std::string line; std::getline(in, line); i++, used = false)
		{
			if (i == 0)
			{
				line = line.substr(3);
			}

			for (int j = 0; j < line.length(); j++)
			{
				if (line.at(j) == ' ' || line.at(j) == '\t' || line.at(j) == '\r')
				{
					line.erase(line.begin() + j);
				}
			}
			for (int j = 0; j < line.length(); j++)
			{
				if (line.at(j) == '=')
				{
					file_proc(id, line.substr(0, j), line.substr(j + 1, line.length() - j - 1));
					used = true;
					break;
				}
			}
			if (!used)
			{
				if (line == "}")
				{
					scope.pop_back();
				}
			}

			int j = 0;
			for (std::string s : scope)
			{
				LOG_H(std::to_string(j),s);
				j++;
			}


		}
		in.close();
		if (scope.end() - scope.begin() != 0)
		{
			LOG_O("Syntax Error of ", path);
			quit = true;
		}
		else
		{
			LOG_O("Succeed Reading of ", path);
		}
	}
	else
	{
		LOG_W("Failed Reading of ", path);
	}
}
void gfx_read(std::string path, int i, std::string tag)
{
	std::string key = "";
	for (int j = path.length() - 1; j > 0; j--)
	{
		if (path.at(j) == '\\')
		{
			for (int k = j + 1; k < path.length(); k++)
			{
				if (path.at(k) == '.')
				{
					key = path.substr(j + 1, k - j - 1);
				}
			}
			break;
		}
	}

	LOG_H("Key is ", tag + "\\" + key);
	gfx[tag + "\\" + key].s = IMG_Load(path.c_str());
	if (gfx[tag + "\\" + key].s == NULL)
	{
		LOG_W("CANT READ FILE AS SPRITE", path);
		LOG_W(IMG_GetError());
		quit = true;
		return;
	}
	else
	{
		SDL_SetColorKey(gfx[tag + "\\" + key].s, SDL_TRUE, SDL_MapRGB(gfx[tag + "\\" + key].s->format, 255, 0, 255));
		LOG_O("SUCCESS READ FILE AS SPRITE", path);
		gfx[tag + "\\" + key].t = SDL_CreateTextureFromSurface(REND, gfx[tag + "\\" + key].s);
		keys[tag + "[" + std::to_string(i) + "]"] = tag + "\\" + key;
		if (gfx[tag + "\\" + key].t == NULL)
		{
			LOG_W("CANT CONVERT TO TEXTURE", path);
			LOG_W(SDL_GetError());
			quit = true;
			return;
		}
	}

}
void music_read(std::string path, int i, std::string tag)
{
	std::string key = "";
	for (int j = path.length() - 1; j > 0; j--)
	{
		if (path.at(j) == '\\')
		{
			for (int k = j + 1; k < path.length(); k++)
			{
				if (path.at(k) == '.')
				{
					key = path.substr(j + 1, k - j - 1);
				}
			}
			break;
		}
	}

	LOG_H("Key is ", tag + "\\" + key);
	sfx[tag + "\\" + key] = Mix_LoadMUS(path.c_str());
	if (sfx[tag + "\\" + key] == NULL)
	{
		LOG_W("CANT READ FILE AS MUSIC", path);
		LOG_W(Mix_GetError());
		quit = true;
		return;
	}
	else
	{
		LOG_O("SUCCESS READ FILE AS Music", path);
		keys[tag + "[" + std::to_string(i) + "]"] = tag + "\\" + key;
	}

}
void data_proc( std::string s1, std::string s2)
{
	if (s2 == "{")
	{
		scope.push_back("}" + s1);
		return;
	}
	if (s2 == "[")
	{
		scope.push_back("]" + s1);
		keys[s1] = "0";
		return;
	}
	if (s2 == "<")
	{
		scope.push_back(">" + s1);
		if (s1 == "Man")
		{
			Man m;
			keys[s1] = std::to_string(man.size());
			LOG_O("GOOD!", keys["Man"]);
			man.push_back(m);
		}
		else if (s1 == "Party")
		{
			Party m;
			if (keys[s1] == "")
			{
				keys[s1] = "0";
			}
			else
			{
				keys[s1] = std::to_string(std::stoi(keys[s1]) + 1);
			}
			LOG_O("GOOD!", keys["Party"]);
			party[std::stoi(keys[s1])] = m;
		}
		else
		{
			LOG_W("Error! in data proc", s1);
			//quit = true;
		}
		return;
	}

	if (scope.begin() == scope.end())
	{
		keys[s1] = s2;
	}
	else if ((scope.end() - 1)->at(0)=='>')
	{
		std::string s = (scope.end() - 1)->substr(1);
		if (s == "Man")
		{
			auto I = man.begin();
			for (int i = 0; i < std::stoi(keys[s]); i++)
			{
				I++;
			}
			if (s1 == "name")
			{
				I->name = s2;
			}
			else if (s1 == "potrait")
			{
				I->potrait = s2;
			}
			else if (s1 == "live")
			{
				if (s2 == "true")
				{
					I->live = true;
				}
				else if (s2 == "false")
				{
					I->live = false;
				}
			}
			else if (s1 == "born_year")
			{
				I->born_year= std::stoi(s2);
			}
			else if (s1 == "money")
			{
				I->money = std::stoi(s2);
			}
			else if (s1 == "prestige")
			{
				I->prestige = std::stoi(s2);
			}
			else if (s1 == "fascist")
			{
				I->fascist = std::stoi(s2);
			}
			else if (s1 == "liberty")
			{
				I->liberty = std::stoi(s2);
			}
			else if (s1 == "honor")
			{
				I->honor = std::stoi(s2);
			}
			else if (s1 == "ambition")
			{
				I->ambition = std::stoi(s2);
			}
			else if (s1 == "religion")
			{
				I->religion = std::stoi(s2);
			}
			else if (s1 == "id")
			{
				I->id = std::stoi(s2);
				LOG_W("the id", s2);
			}
		}
		else if (s == "Party")
		{
			if (s1 == "name")
			{
				party[std::stoi(keys[s])].name = s2;
			}
			else if (s1 == "fascist")
			{
				party[std::stoi(keys[s])].facism = std::stoi(s2);
			}
			else if (s1 == "liberty")
			{
				party[std::stoi(keys[s])].liberty = std::stoi(s2);
			}
			else if (s1 == "color" && false)
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
						s2 = s2.substr(0, i);
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
				party[std::stoi(keys[s])].c = color(rgb[0], rgb[1], rgb[2]);
			}
		}
	}
	LOG_H(s1,s2);
}
void data_read(std::string path)
{
	std::ifstream in(path.c_str());

	for (int i = 0; i < 16; i++)
	{
		tmp[i] = 0;
	}
	scope.clear();
	if (in.is_open())
	{
		int i = 0;
		bool used = false;
		for (std::string line; std::getline(in, line); i++, used = false)
		{
			if (line == "" or line == "癤풫an")
			{
				continue;
			}
			bool can_do_it = true;
			for (int j = 0; j < line.length(); j++)
			{
				if (line.at(j) == '#')
				{
					if (j > 0)
					{
						if (line.at(j - 1) == '\\')
						{
							continue;
						}
					}
					if (j == 0)
					{
						can_do_it = false;
						break;
					}
					line = line.substr(0, j);
					break;
				}
			}
			if (!can_do_it)
			{
				continue;
			}

			for (int j = 0; j < line.length() - 1; j++)
			{
				if (line.at(j) == '=')
				{
					if (line.at(j + 1) == '=')
					{
						for (int k = j; k < line.length(); k++) {
							if (line.at(k) == ' ')
							{
								line.insert(line.begin() + k, ' ');
								k++;
							}
						}

						data_proc(line.substr(0, j), line.substr(j + 2, line.length() - j - 2));
						used = true;
						break;
					}
				}
			}

			for (int j = 0; j < line.length(); j++)
			{
				if (line.at(j) == ' ' || line.at(j) == '\t' || line.at(j) == '\r')
				{
					line.erase(line.begin() + j);
				}
				else if (line.at(j) == '"')
				{
					line.erase(line.begin() + j);
					break;
				}
			}

			if (!used)
			{
				for (int j = 0; j < line.length(); j++)
				{
					if (line.at(j) == '=')
					{
						int k;
						data_proc(line.substr(0, j), line.substr(j + 1, line.length() - j - 1));
						used = true;
						break;
					}
				}
			}
			if (!used)
			{
				if (line == "}" || line == "]" || line == ">")
				{
					if (line != (scope.end() - 1)->substr(0, 1)) 
					{
						LOG_W("Syntax Error of ", path);
						used = true;
						quit = true;
					}
					scope.pop_back();
				}
			}
			if (!used && scope.size() > 0)
			{
				if ((scope.end() - 1)->at(0) == ']')
				{
					keys[(scope.end() - 1)->substr(1) + "[" + keys[(scope.end() - 1)->substr(1)] + "]"] = line;
					keys[(scope.end() - 1)->substr(1)] = std::to_string(std::stoi(keys[(scope.end() - 1)->substr(1)]) + 1);
					used = true;
				}
				if ((scope.end() - 1)->at(0) == '>')
				{
					keys[(scope.end() - 1)->substr(1)] = std::to_string(std::stoi(keys[(scope.end() - 1)->substr(1)]) + 1);
					used = true;
				}
			}
		}
		in.close();

		if (scope.end() - scope.begin() != 0)
		{
			LOG_W("Syntax Error of ", path);
			quit = true;
		}
		else
		{
			LOG_O("Succeed Reading of ", path);
		}
	}
	else
	{
		LOG_O("Failed Reading of ", path);
	}


}

void prov_set()
{
	//SDL_Delay(60000);
	SDL_Surface* map = IMG_Load((currentDir + "\\" + "map\\prov.bmp").c_str());

	int w, h;

	w = map->w;
	h = map->h;
	unsigned char* pixels = (unsigned char*)map->pixels;

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
			SDL_SetColorKey(prov, SDL_TRUE, 0);
			prv[i].t = SDL_CreateTextureFromSurface(REND, prov);
		}
	}
	tmp[0] = 0;
}

void data_set()
{
}
void start()
{
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2");
	srand(time(NULL));

	set_color();

	Mix_PlayMusic(sfx["music\\You_re_Not_Wrong"], -1);
	std::string path = currentDir + "\\" + "map\\provinces";
	for (auto p : std::experimental::filesystem::directory_iterator(path))
	{
		char* pStr;
		int strSize = WideCharToMultiByte(CP_ACP, 0, p.path().c_str(), -1, NULL, 0, NULL, NULL);
		pStr = new char[strSize];
		WideCharToMultiByte(CP_ACP, 0, p.path().c_str(), -1, pStr, strSize, 0, 0);
		file_prov_read(pStr);
	}


	for (int i = 0; i < MAX_PROV; i++)
	{
		for (int j = 0; j < prv[i].man * 3; j++)
		{
			Pop p;
			prv[i].pop.push_back(p);
		}
	}

	data_set();
	tmp[0] = 1;

	SDL_Rect r;
	set_rect(&r,0,0,0,0);
	SDL_Point p;
	int num = 0;
	std::thread trd_step(prov_set);
	do
	{
		SDL_SetRenderDrawColor(REND, 0x23, 0x23, 0x23, 0xFF);
		SDL_RenderClear(REND);

		r.x = scr_w / 2 - 320;
		r.y = scr_h / 2 - 240;
		r.w = 640;
		r.h = 480;


		r.x -= 16;
		r.y -= 16;
		r.w += 32;
		r.h += 32;
		SDL_RenderCopy(REND, gfx["ui\\back_board"].t, NULL, &r);

		set_rect(&r,0,0,800,128);
		SDL_RenderCopy(REND, gfx["ui\\game_logo"].t, NULL, &r);

		r.x = scr_w / 2 - 320;
		r.y = scr_h / 2 - 240;
		r.w = 640;
		r.h = 480;
		set_rect(&r, scr_w / 2 - 320, scr_h / 2 - 240, 640, 480);
		SDL_RenderCopy(REND, gfx[keys["loading[" + std::to_string(num) +"]"]].t, NULL, &r);

		std::string s = keys["tip[" + std::to_string(rand() % std::stoi(keys["tip"])) + "]"];
		tmp[1] = get_chrs(s) * (scr_h / 2 - 480 - 24) * 0.6;
		set_rect(&r, scr_w / 2 - tmp[1] / 2, scr_h / 2 + 360 + 12, tmp[1], scr_h / 2 - 480 - 24);
		r.x -= 12;
		r.y -= 12;
		r.w += 24;
		r.h += 24;
		r.x += 6;
		r.y += 6;
		r.w -= 12;
		r.h -= 12;
		SDL_RenderCopy(REND, gfx["ui\\paper"].t, NULL, &r);
		set_rect(&r, scr_w / 2 - tmp[1] / 2, scr_h / 2 + 360 + 12, tmp[1], scr_h / 2 - 480 - 24);
		p.x = r.x + r.w / 2;
		p.y = r.y + r.h / 2;
		draw_string(2,s, Color["c_black"], &p, r.h, 0.6, middle_align + center_align);


		SDL_RenderPresent(REND);
		SDL_Delay(3000);
		num = (num + 1) % std::stoi(keys["loading"]);
	} while (tmp[0] == 1);

	trd_step.join();

	Mix_PlayMusic(sfx["music\\Touching_Moment"],-1);	
	safe_start();
}

void normal_start()
{
	pop_cong(200, 200);
	pop_quit(200, 200);
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
	SDL_Point p;

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
	
	return;
}

void draw_item(std::vector<Widget>::iterator I, const SDL_Rect r, const unsigned char type)
{

	if (type == wd_none)
	{
		if (I->avail_mousedown_ev)
		{
			SDL_RenderCopy(REND, gfx["ui\\fuchsia-black"].t, NULL, &r);
		}
		else
		{
			SDL_RenderCopy(REND, gfx["ui\\black-white"].t, NULL, &r);
		}
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
			if (J->parent == I->id && I->enable)
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
			pop_quit(scr_w / 2 - 200, scr_h / 2 - 100);
			return;
		}
		case SDL_KEYDOWN: {
			switch (e->key.keysym.sym)
			{
			case SDLK_ESCAPE: {
				pop_quit(scr_w / 2 - 200, scr_h / 2 - 100);
				break;
			}
			}
		}
		}
		
		SDL_GetMouseState(&x, &y);

		int ii_mousedown = -1;
		int mot_mousedown = -1;
		int ii_mousehover = -1;
		int mot_mousehover = -1;
		int ii_mouseup = -1;
		int mot_mouseup = -1;

		for (auto i =0; i < gui.size(); i++)
		{
			auto I = *(gui.begin() + i);
			if (I.enable)
			{
				if (I.avail_mousestep_ev)
				{
					I.mousestep_ev(i, x, y);
				}
				if (I.rx <= x &&
					I.ry <= y &&
					I.rx + I.w >= x &&
					I.ry + I.h >= y)
				{
					if (I.avail_mousedown_ev)
					{
						if (e->type == SDL_MOUSEBUTTONDOWN)
						{
							if (mother(i) > mot_mousedown)
							{
								mot_mousedown = mother(i);
								ii_mousedown = i;
							}
						}
					}
					if (I.avail_mouseup_ev)
					{
						if (e->type == SDL_MOUSEBUTTONUP)
						{
							if (mother(i) > mot_mouseup)
							{
								mot_mouseup = mother(i);
								ii_mouseup = i;
							}
						}
					}
					if (I.avail_mousehover_ev)
					{
						if (mother(i) > mot_mousehover)
						{
							mot_mousehover = mother(i);
							ii_mousehover = i;
						}
					}
				}
			}
		}

		if (ii_mousedown != -1)
		{
			(gui.begin() + ii_mousedown)->mousedown_ev(ii_mousedown, x, y);
		}
		if (ii_mousehover != -1)
		{
			(gui.begin() + ii_mousehover)->mousehover_ev(ii_mousehover, x, y);
		}
		if (ii_mouseup != -1)
		{
			(gui.begin() + ii_mouseup)->mouseup_ev(ii_mouseup, x, y);
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

	char buf[320] = { 0, };
	sprintf_s(buf, "%d년 %02d월 %02d일\n", year, mon, day);
	gui[ikeys["@ui\\timer_text"]].var["text"] = buf;

	auto I = man.begin();
	for (int a = 0; a < man.size(); a++, I++)
	{
		if (I->id == play_id)
		{
			gui[ikeys["@ui\\right_hand[0]_label"]].var["text"] = "자금 : " + std::to_string(I->money);
			gui[ikeys["@ui\\right_hand[1]_label"]].var["text"] = "명예 : " + std::to_string(I->prestige);
			gui[ikeys["@ui\\right_hand[2]_label"]].var["text"] = "사상 : " + get_ideology(I->fascist, I->liberty);
			gui[ikeys["@ui\\right_hand[3]_label"]].var["text"] = "직업 : 백수";
			gui[ikeys["@ui\\potrait"]].var["img"] = I->potrait;
			break;
		}
	}

	for (auto i = 0; i < gui.size(); i++)
	{
		auto I = (gui.begin() + i);
		if (I->id == I->parent && I->enable)
		{
			I->rx = I->x;
			I->ry = I->y;
			set_rect(&r, I->rx, I->ry, I->w, I->h);
			draw_item(I, r, I->type);
			child_ui(i,I,r);
		}

	}

	SDL_RenderPresent(REND);
	return;
}

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
	SDL_Surface* surf = TTF_RenderUTF8_Solid(fonts[i], s.c_str(), color);
	SDL_Texture* t = SDL_CreateTextureFromSurface(REND, surf);
	SDL_FreeSurface(surf);
	SDL_RenderCopy(REND, t, NULL, r);
	SDL_DestroyTexture(t);
}
void draw_string(int ind, std::string s, SDL_Color color, SDL_Point* p, int size, float ratio , int opt)
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
	draw_string_s(ind, s, color,&r);
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

bool init()
{
	setlocale(LC_ALL, "");
	std::ios_base::sync_with_stdio(false);
	char curDir[1000];
	_getcwd(curDir, 1000);
	currentDir = curDir;
	LOG_H("This Programm run in ", currentDir);
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

	/*int flags = MIX_INIT_OGG | MIX_INIT_MP3;
	int result = 0;
	if (flags != (result = Mix_Init(flags))) {
		LOG_W("MIX INIT WITH MP3 Failed", std::to_string(result));
		LOG_W(Mix_GetError());
		return false;
	}*/
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
	SDL_Surface* Logo = IMG_Load((currentDir + "\\gfx\\ui\\logo.bmp").c_str());
	SDL_Surface* Back = IMG_Load((currentDir + "\\gfx\\ui\\right_heard_body.bmp").c_str());
	LOG_H(IMG_GetError());
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
	
	int num = 0;
	std::string path , tag;

	tag = "music";
	path = currentDir + "\\" + tag + "\\";
	for (auto p : std::experimental::filesystem::directory_iterator(path))
	{
		char* pStr;
		int strSize = WideCharToMultiByte(CP_ACP, 0, p.path().c_str(), -1, NULL, 0, NULL, NULL);
		pStr = new char[strSize];
		WideCharToMultiByte(CP_ACP, 0, p.path().c_str(), -1, pStr, strSize, 0, 0);
		music_read(pStr,num,tag);
		num++;
	}
	keys[tag] = num;




	tag = "common";
	path = currentDir + "\\" + "common\\";
	for (auto p : std::experimental::filesystem::directory_iterator(path))
	{
		char* pStr;
		int strSize = WideCharToMultiByte(CP_ACP, 0, p.path().c_str(), -1, NULL, 0, NULL, NULL);
		pStr = new char[strSize];
		WideCharToMultiByte(CP_ACP, 0, p.path().c_str(), -1, pStr, strSize, 0, 0);
		data_read(pStr);
		num++;
	}
	keys[tag] = num;

	tag = "characters";
	path = currentDir + "\\" + tag + "\\";
	for (auto p : std::experimental::filesystem::directory_iterator(path))
	{
		char* pStr;
		int strSize = WideCharToMultiByte(CP_ACP, 0, p.path().c_str(), -1, NULL, 0, NULL, NULL);
		pStr = new char[strSize];
		WideCharToMultiByte(CP_ACP, 0, p.path().c_str(), -1, pStr, strSize, 0, 0);
		data_read(pStr);
		num++;
	}
	keys[tag] = num;

	tag = "ui";
	path = currentDir + "\\" + "gfx\\" + tag;
	num = 0;
	for (auto p : std::experimental::filesystem::directory_iterator(path))
	{
		char* pStr;
		int strSize = WideCharToMultiByte(CP_ACP, 0, p.path().c_str(), -1, NULL, 0, NULL, NULL);
		pStr = new char[strSize];
		WideCharToMultiByte(CP_ACP, 0, p.path().c_str(), -1, pStr, strSize, 0, 0);
		gfx_read(pStr, num, tag);
		num++;
	}
	keys[tag] = num;


	tag = "potrait";
	path = currentDir + "\\" + "gfx\\" + tag;
	num = 0;
	for (auto p : std::experimental::filesystem::directory_iterator(path))
	{
		char* pStr;
		int strSize = WideCharToMultiByte(CP_ACP, 0, p.path().c_str(), -1, NULL, 0, NULL, NULL);
		pStr = new char[strSize];
		WideCharToMultiByte(CP_ACP, 0, p.path().c_str(), -1, pStr, strSize, 0, 0);
		gfx_read(pStr, num, tag);
		num++;
	}
	keys[tag] = num;

	tag = "loading";
	path = currentDir + "\\" + "gfx\\" + tag;
	num = 0;
	for (auto p : std::experimental::filesystem::directory_iterator(path))
	{
		char* pStr;
		int strSize = WideCharToMultiByte(CP_ACP, 0, p.path().c_str(), -1, NULL, 0, NULL, NULL);
		pStr = new char[strSize];
		WideCharToMultiByte(CP_ACP, 0, p.path().c_str(), -1, pStr, strSize, 0, 0);
		gfx_read(pStr, num, tag);
		num++;
	}
	keys[tag] = std::to_string(num);

	return success;
}
void close()
{
	for (auto i = gfx.begin(); i != gfx.end(); i++)
	{
		if ((*i).second.t != NULL)
		{
			SDL_DestroyTexture((*i).second.t);
		}
		if ((*i).second.s != NULL)
		{
			SDL_FreeSurface((*i).second.s);
		}
	}
	for (auto i = sfx.begin(); i != sfx.end(); i++)
	{
		if ((*i).second != NULL)
		{
			Mix_FreeMusic((*i).second);
		}
	}

	for (int a = 0; a < MAX_FONT; a++)
	{
		if (fonts[a] != NULL)
		{
			TTF_CloseFont(fonts[a]);
			fonts[a] = NULL;
		}
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
	std::cout << ESCAPE << "[1;32m[OK]" << ESCAPE << "[1;37m " << s << " : " << s2 << "\n";
}
void LOG_Stop()
{
	std::string a;
	std::cout << "Press Any Key to Continue...";
	std::cin.ignore();
	return;
}
