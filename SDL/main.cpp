#include "useful_func.h"

#define SDL_MAIN_HANDLED

unsigned int day_over()
{
	unsigned int year = Num(script.at(L"year"));
	unsigned int mon = Num(script.at(L"month"));
	unsigned int day = Num(script.at(L"day"));
	bool is_yun = false;
	if (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0))
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
			script[L"year"] = Str(year);
			script[L"month"] = Str(mon);
			script[L"day"] = Str(day);
			return 2;
		}
		else if (day > 29 && is_yun)
		{
			day = 1;
			mon++;
			script[L"year"] = Str(year);
			script[L"month"] = Str(mon);
			script[L"day"] = Str(day);
			return 2;
		}
	}
	else if ((mon % 2 == 1 && mon <= 7) || (mon % 2 == 0 && mon >= 8))
	{
		if (day > 31)
		{
			day = 1;
			mon++;
			script[L"year"] = Str(year);
			script[L"month"] = Str(mon);
			script[L"day"] = Str(day);
			return 2;
		}
	}
	else if (day > 30)
	{
		day = 1;
		mon++;
		script[L"year"] = Str(year);
		script[L"month"] = Str(mon);
		script[L"day"] = Str(day);
		return 2;
	}
	if (mon > 12)
	{
		mon = 1;
		year++;
		script[L"year"] = Str(year);
		script[L"month"] = Str(mon);
		script[L"day"] = Str(day);
		return 3;
	}
	script[L"year"] = Str(year);
	script[L"month"] = Str(mon);
	script[L"day"] = Str(day);
	return 1;
}

void start()
{
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 16);
	//glEnable(GL_MULTISAMPLE);
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2");
	{
		Widget wd0(404, 404, 404, 404, wd_image, L"Error!");
		wd0.enable = false;
		wd0.id = gui.size();
		wd0.parent = wd0.id;
		gui.push_back(wd0);
	}
	{
		Widget wd0(0, 0, scr_w, scr_h, wd_image, L"Backmap");
		wd0.enable = true;
		wd0.id = gui.size();
		wd0.var[L"source"] = L"None";
		wd0.parent = wd0.id;
		wd0.ev.Event_mousedown = BackMap_mousedown;
		gui.push_back(wd0);
	}
	{
		Nation nt;
		nt.c = color(255, 255, 255);
		nt.var[L"title"] = L"원주민";
		nat[L"NAV"] = nt;
	}
	{
		Nation nt;
		nt.c = color(60, 0, 0);
		nt.var[L"title"] = L"반군";
		nat[L"REB"] = nt;
	}

	{
		SDL_Color clr;
		clr.a = 255;
		clr.r = 0;
		clr.g = 0;
		clr.b = 0;
		Color[L"black"] = clr;
	}
	{
		SDL_Color clr;
		clr.a = 255;
		clr.r = 0;
		clr.g = 255;
		clr.b = 0;
		Color[L"green"] = clr;
	}
	{
		SDL_Color clr;
		clr.a = 255;
		clr.r = 255;
		clr.g = 0;
		clr.b = 0;
		Color[L"red"] = clr;
	}
	{
		SDL_Color clr;
		clr.a = 255;
		clr.r = 255;
		clr.g = 255;
		clr.b = 0;

		Color[L"yellow"] = clr;
	}
	{
		SDL_Color clr;
		clr.a = 255;
		clr.r = 255;
		clr.g = 255;
		clr.b = 255;

		Color[L"white"] = clr;
	}
	script[L"CON"] = L"REB";
	gui.at(gui_key[L"nation_flag"]).var[L"source"] = L"flag\\REB";


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
		if (I.waste_land)
		{
			LOG_W(L"This province is waste_land", I.name);
		}
	}

	script[L"year"] = L"110";
	script[L"month"] = L"1";
	script[L"day"] = L"1";
	//get_path(40, 49);

	//Mix_PlayMusic(sfx[L"sample"], -1);
}
void go_ai()
{
	//Ai
	static unsigned int n = 0;
	static unsigned int i = 0;
	static size_t syc = 0;
	static size_t min_size_t = std::numeric_limits<size_t>::min();
	static unsigned int target = 0;
	static int size;
	static std::vector<unsigned int> own_man;
	static auto protagonist = nat.begin();
	static bool flag_diffence = false;
	static bool flag_mine = false;
	static bool flag_myrealm = false;

	std::vector<AiProvince> ai_prv;
	ai_prv.resize(prv.size());

	if ((++protagonist) == nat.end()) protagonist = nat.begin();
	if (protagonist->first == L"NAV" || protagonist->first == L"REB" || protagonist->first == script[L"CON"]) return;

	auto ruler = protagonist->first;
	protagonist->second.total_unit = 0;
	i = 0;
	for (auto I = prv.begin(); I != prv.end(); ++I, ++i)
	{
		ai_prv.at(i).attack = 0;
		if (I->CON != ruler)
		{
			ai_prv.at(i).attack = 60;
		}
	}

	for (auto I = man.begin(); I != man.end(); ++I)
	{
		if (I->second.var[L"live"] == L"1")
		{
			size = Num(I->second.var[L"HP"]) / 100;

			//is mine?
			if (I->second.var[L"CON"] == ruler)
			{
				//if (i->second.com.size() == 0)
				{
					own_man.push_back(I->first);
				}
				flag_mine = true;
			}
			else
			{
				flag_mine = false;
			}

			//is diffend?
			if (I->second.var[L"CON"] == prv.at(Num(I->second.var[L"LOC"])).CON)
			{
				flag_diffence = true;
			}
			else
			{
				flag_diffence = false;
			}

			//is my realm?
			if (ruler == prv.at(Num(I->second.var[L"LOC"])).CON)
			{
				flag_myrealm = true;
			}
			else
			{
				flag_myrealm = false;
			}

			if (flag_mine)
			{
				if (flag_diffence)
				{
					size /= 2;
				}
				else
				{
					size += 40;
				}
			}
			else
			{
				size *= -1;
				if (flag_diffence)
				{
					size *= 2;
				}
				else if (flag_myrealm)
				{
					size += 60;
				}
			}
			ai_prv.at(Num(I->second.var[L"LOC"])).attack = size;
		}
	}


	for (auto R = own_man.begin(); R != own_man.end(); ++R)
	{
		auto I = man.at(*R);
		if (I.var[L"live"] == L"1")
		{
			syc = min_size_t;// *r % 300;
			target = Num(I.var[L"LOC"]);
			auto& spider_map = get_spider_map(Num(I.var[L"LOC"]));

			i = 0;
			auto A = ai_prv.begin();
			for (auto P = spider_map.begin(); P != spider_map.end(); ++P, ++i, ++A)
			{
				if (P->enable && P->nearest != ai_prv.size())
				{
					if (A->attack - P->len * 10 > syc)
					{
						syc = A->attack - P->len * 10;
						target = i;
					}
				}
			}
			if (syc - ai_prv.at(Num(I.var[L"LOC"])).attack > (I.com.size() != 0 ? 100 : 0))
			{
				if ((unsigned int)Num(I.var[L"LOC"]) != target)
				{
					put_path(*R, Num(I.var[L"LOC"]), target);
				}
				ai_prv.at(target).attack -= Num(I.var[L"HP"]);
				ai_prv.at(Num(I.var[L"LOC"])).attack += Num(I.var[L"HP"]);
			}
		}

	}

	own_man.clear();

}
void step(SDL_Event *e)
{
	nat;
	STEP_LAST = STEP_NOW;
	STEP_NOW = SDL_GetTicks();

	int day_off = 0;
	if (!pause)
	{
		TIME_STACK += (STEP_NOW - STEP_LAST);
		while (TIME_STACK >= 400)
		{
			day_off = day_over();
			TIME_STACK = 0;
		}

	}

	for (auto I = man.begin(); I != man.begin(); ++I)
	{
		if (I->second.var[L"live"] != L"1")
		{
			man.erase(I);
		}
	}

	double TimeDelta = (double)(STEP_NOW - STEP_LAST);


	{
		{
			size_t i = 0;
			for (auto I : gui)
			{
				if (I.enable && I.ev.Event_step != NULL)
				{
					I.ev.Event_step(i);
				}
				++i;
			}
		}

		if (day_off > 0)
		{
			unsigned int i = 0;
			for (auto I = prv.begin(); I != prv.end(); ++I, ++i)
			{
				if (!I->waste_land)
				{
					if (I->OWN == I->CON)
					{
						//Rebel Uprise
						if (I->OWN == L"REB")
						{
							//LOG_W(I->var[L"name"]);
							script[L"TEMP"] = L"REB";
							for (auto J : nat)
							{
								if (J.first != L"NAV")
								{
									size_t prv_num = 0;
									for (auto K : prv)
									{
										if (K.OWN == J.first) prv_num++;

									}
									if (prv_num == 0)
									{
										script[L"TEMP"] = J.first;
									}
								}
							}
							if (script[L"TEMP"] != L"REB")
							{
								I->OWN = script[L"TEMP"];
								I->CON = script[L"TEMP"];

								for (unsigned int r = 0; r < 3; ++r)
								{
									Man obj;
									obj.var[L"name"] = L"가가 가가";
									obj.var[L"LOC"] = Str(i);
									obj.var[L"HOME"] = Str(i);
									obj.var[L"CON"] = script[L"TEMP"];
									nat.at(I->OWN).total_unit += 1;
									man[ALL_MAN++] = obj;
								}
							}
						}

						if (I->OWN != L"NAV")
						{
							if (I->pi.man < 1000)
							{
								I->pi.man += 10;
							}
						}
					}
					else
					{
						if (I->pi.man < 1000)
						{
							I->pi.man += 3;
						}
					}
					if (I->pi.man >= 1000 && I->OWN != L"NAV" && nat.at(I->OWN).total_unit < 5)
					{
						Man obj;
						obj.var[L"name"] = L"가가 가가";
						obj.var[L"LOC"] = Str(i);
						obj.var[L"HOME"] = Str(i);
						obj.var[L"CON"] = I->OWN;
						nat.at(I->OWN).total_unit += 1;
						I->pi.man = 0;
						man[ALL_MAN++] = obj;
					}
				}
			}
			for (auto I = man.begin(); I != man.end(); ++I)
			{
				if (I->second.var[L"live"] == L"1")
				{
					//With Man
					{
						size_t enemey = 0;
						for (auto J = man.begin(); J != man.end(); ++J)
						{
							if (J->second.var[L"live"] == L"1" && I->second.var[L"LOC"] == J->second.var[L"LOC"] && I->second.var[L"CON"] != J->second.var[L"CON"])
							{
								++enemey;
							}
							if (enemey)
							{
								if (I->second.var[L"purpose"] == L"heal")
								{
									I->second.var[L"purpose"] = L"";
									I->second.var[L"power"] = L"0";
								}
								if (I->second.var[L"purpose"] == L"sieze")
								{
									I->second.var[L"purpose"] = L"";
									I->second.var[L"power"] = L"0";
								}
							}
						}
						if (enemey && I->second.var[L"purpose"] == L"")
						{
							for (auto J = man.begin(); J != man.end(); ++J)
							{
								if (J->second.var[L"live"] == L"1" && I->second.var[L"LOC"] == J->second.var[L"LOC"] && I->second.var[L"CON"] != J->second.var[L"CON"])
								{
									if (rand() % (--enemey + 1) == 0)
									{
										J->second.var[L"HP"] = Str((int)(Num(J->second.var[L"HP"]) - (1 + Num(I->second.var[L"attack"])) * (1 + (I->second.var[L"CON"] == prv.at(Num(I->second.var[L"LOC"])).CON ? 1 : 0)) * Num(I->second.var[L"HP"]) / 1000));
										if (Num(J->second.var[L"HP"]) < 0)
										{
											J->second.var[L"live"] = L"0";
											nat.at(J->second.var[L"CON"]).total_unit -= 1;

											size_t sec = gui_key.at(script[L"leader_flag"]);
											if (gui.at(sec).parent == gui_key[L"@leader_" + Str(J->first)])
											{
												gui.at(sec).parent = sec;
											}
											gui.at(gui_key[L"@leader_" + Str(J->first)]).removing = true;
										}
										else
										{
											if (Num(J->second.var[L"HP"]) <= 500 && J->second.var[L"purpose"] != L"move")
											{
												auto K = prv.at(Num(J->second.var[L"LOC"]));

												Widget wd0((Sint32)((K.x1 + K.px - scr_w / 2.0 - map_x) * map_p + scr_w / 2.0), (Sint32)((K.y1 + K.py - scr_h / 2.0 - map_y) * map_p + scr_h / 2.0), 0, 0, wd_label, L"SiezeMessage" + Str(rand()));
												wd0.enable = true;
												wd0.id = gui.size();
												wd0.parent = wd0.id;

												wd0.var[L"ind"] = L"default";
												wd0.var[L"size"] = L"20";
												wd0.var[L"opt"] = L"11";
												wd0.var[L"color"] = L"white";
												wd0.var[L"delay"] = L"0";
												wd0.var[L"life"] = L"1600";
												wd0.ev.Event_step = BlowupLabel_step;

												wd0.var[L"text"] = L"후퇴!";

												I->second.var[L"power"] = L"0";
												gui.push_back(wd0);
												run_to_rest(J->first);
												J->second.var[L"purpose"] = L"move";
											}
											if (J->second.var[L"purpose"] == L"sieze")
											{
												J->second.var[L"power"] = L"0";
												J->second.var[L"purpose"] = L"";
											}
											if (J->second.var[L"purpose"] == L"heal")
											{
												J->second.var[L"purpose"] = L"";
												J->second.var[L"power"] = L"0";
											}
										}
										break;
									}
								}
							}
						}
					}
					//Without
					{
						auto P = prv.at(Num(I->second.var[L"HOME"]));
						if (!P.waste_land)
						{
							if (P.pi.man > 10)
							{
								P.pi.man -= 10;
							}
						}

						if (I->second.com.size() > 0)
						{
							if (I->second.var[L"purpose"] != L"move")
							{
								I->second.var[L"purpose"] = L"move";
								I->second.var[L"power"] = L"0";
							}
							auto I0 = prv.at(Num(I->second.var[L"LOC"]));
							auto I1 = prv.at(*I->second.com.begin());

							if (I0.c == I1.c)
							{
								I->second.com.pop_front();
							}
							else if (map_connect[{Num(I->second.var[L"LOC"]), *I->second.com.begin()}] * 3 <= (unsigned int)Num(I->second.var[L"power"]))
							{
								I->second.var[L"LOC"] = Str(*I->second.com.begin());
								I->second.com.pop_front();
								I->second.var[L"power"] = L"0";
							}
							else
							{
								I->second.var[L"power"] = Str(Num(I->second.var[L"power"]) + 2 + (10 - Num(I->second.var[L"HP"]) / 100) / 2);
							}
							continue;
						}
					}
					//With Province
					{
						auto J = prv.at(Num(I->second.var[L"LOC"]));

						//Sieze Land
						if (!J.waste_land && J.CON != I->second.var[L"CON"])
						{
							if (I->second.var[L"purpose"] != L"sieze")
							{
								I->second.var[L"purpose"] = L"sieze";
								I->second.var[L"power"] = L"0";
							}
							if (Num(I->second.var[L"power"]) <= 90)
							{
								I->second.var[L"power"] = Str(Num(I->second.var[L"power"]) + 10 + (Num(I->second.var[L"HP"]) / 100));
							}
							else
							{

								Widget wd0((Sint32)((J.x1 + J.px - scr_w / 2.0 - map_x) * map_p + scr_w / 2.0), (Sint32)((J.y1 + J.px - scr_h / 2.0 - map_y) * map_p + scr_h / 2.0), 0, 0, wd_label, L"SiezeMessage" + Str(rand()));
								wd0.enable = true;
								wd0.id = gui.size();
								wd0.parent = wd0.id;

								wd0.var[L"ind"] = L"default";
								wd0.var[L"size"] = L"20";
								wd0.var[L"opt"] = L"11";
								wd0.var[L"color"] = L"white";
								wd0.var[L"delay"] = L"0";
								wd0.var[L"life"] = L"1600";
								wd0.ev.Event_step = BlowupLabel_step;

								wd0.var[L"text"] = L"돌격!";
								if (rand() % 100 < 14)
								{
									prv[Num(I->second.var[L"LOC"])].CON = I->second.var[L"CON"];
									prv[Num(I->second.var[L"LOC"])].OWN = I->second.var[L"CON"];
									wd0.var[L"text"] = L"적 투항";
								}

								I->second.var[L"power"] = L"0";
								gui.push_back(wd0);
							}
						}
						else if (J.OWN == I->second.var[L"CON"])
						{
							if (I->second.var[L"purpose"] != L"heal")
							{
								I->second.var[L"purpose"] = L"heal";
								I->second.var[L"power"] = L"0";
							}
							if (Num(I->second.var[L"power"]) <= 30)
							{
								I->second.var[L"power"] = Str(Num(I->second.var[L"power"]) + 2);
							}
							else
							{
								I->second.var[L"power"] = Str(Num(I->second.var[L"power"]) + 1);
								if (Num(I->second.var[L"HP"]) + 30 <= 1000)
								{
									auto P = prv.at(Num(I->second.var[L"HOME"]));
									if (!P.waste_land)
									{
										if (P.pi.man > 30)
										{
											I->second.var[L"HP"] = Str(Num(I->second.var[L"HP"]) + 30);
											P.pi.man -= 30;
										}
									}
								}
							}
						}
					}

					if (Num(I->second.var[L"power"]) > 0)
					{
						I->second.var[L"power"] = Str(Num(I->second.var[L"power"]) - 1);
					}
					if (Num(I->second.var[L"HP"]) < 0)
					{
						LOG_W(L"HP", I->second.var[L"HP"]);
						I->second.var[L"live"] = L"0";

						size_t sec = gui_key.at(script[L"leader_flag"]);
						if (gui.at(sec).parent == gui_key[L"@leader_" + Str(I->first)])
						{
							gui.at(sec).parent = sec;
						}
						gui.at(gui_key[L"@leader_" + Str(I->first)]).removing = true;
					}
				}
			}
		}
	}

	int x, y;
	SDL_GetMouseState(&x, &y);
	while (SDL_PollEvent(e) != 0)
	{
		size_t pass = 0;
		//size_t depth = 0;
		size_t i = 0;
		switch (e->type)
		{
		case SDL_TEXTINPUT: {
			break;
		}
		case SDL_QUIT: {
			quit = true;
			return;
		}
		case SDL_MOUSEBUTTONDOWN: {
			for (auto I : gui)
			{
				if (I.enable && I.id == I.parent)
				{
					size_t b = ray_ui(i, x, y);
					if (b != 0)
					{
						pass = b;
					}
				}
				++i;
			}
			if (pass > 0 && gui.at(pass).ev.Event_mousedown != NULL)
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
			case SDLK_F1:
			{
				//SDL_Delay(10);
				SDL_Surface *sshot = SDL_CreateRGBSurface(0, scr_w, scr_h, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
				SDL_RenderReadPixels(REND, NULL, SDL_PIXELFORMAT_ARGB8888, sshot->pixels, sshot->pitch);

				SDL_SaveBMP(sshot, "screenshot.jpg");
				SDL_FreeSurface(sshot);
				LOG_O(L"Take a Screenshot.");
				break;
			}
			case SDLK_BACKSPACE: {
				while (script[L"buf"].size() > 0)
				{
					if (*script[L"buf"].crbegin() >= -64)
					{
						script[L"buf"].pop_back();
						break;
					}
					script[L"buf"].pop_back();
				}
				break;
			}
			case SDLK_SPACE: {
				pause = !pause;
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

			if (map_p > 16)
			{
				map_p = 16;
			}
			break;
		}
		}

	}

	{
		double DRAG_SENSITIVE = 0.5 * TimeDelta;// / sqrt(map_p);
		if (x <= 20)
		{
			map_x = map_x - (DRAG_SENSITIVE / map_p);
			map_y = map_y + (DRAG_SENSITIVE * (y * 2.0 / scr_h - 1) / map_p);
		}
		else if (y <= 20)
		{
			map_x = map_x + (DRAG_SENSITIVE * (x * 2.0 / scr_w - 1) / map_p);
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

	double lf = 0;

	if (map_p > 16)
	{
		map_p = 16;
	}

	double _map_x = map_x;
	double _map_y = map_y;
	double _map_p = map_p;

	for (unsigned int dep = 0; dep < 3; ++dep)
	{
		for (auto& I : prv)
		{
			auto ID = Str(I.id);
			set_rect(r, (I.x1 - scr_w / 2.0 - _map_x) * _map_p + scr_w / 2.0, (I.y1 - scr_h / 2.0 - _map_y) * _map_p + scr_h / 2.0, (I.x2 - I.x1 + 1.0) * _map_p, (I.y2 - I.y1 + 1.0) * _map_p);

			if (dep == 2)
			{
				p->x = (int)(r->x + (I.px) * _map_p + 0.5);
				p->y = (int)(r->y + (I.py) * _map_p + 0.5);

				r->x = (int)(p->x - 3 * map_p + 0.5);
				r->y = (int)(p->y - 3 * map_p + 0.5);
				r->w = (int)(3 * map_p + 0.5);
				r->h = (int)(3 * map_p + 0.5);
				r->x += r->w / 2;

				int bias;
				for (auto J = man.begin(); J != man.end(); ++J)
				{
					if (J->second.var[L"live"] == L"1" && J->second.var[L"LOC"] == ID)
					{
						if (gui_key.find(L"@leader_" + Str(J->first)) == gui_key.end())
						{
							Widget wd0 = Widget(0, 0, 0, 0, wd_image, L"@leader_" + Str(J->first));
							wd0.var[L"source"] = L"flag\\" + J->second.var[L"CON"];
							wd0.var[L"id"] = Str(J->first);
							wd0.ev.Event_mousedown = LittleUp_mousedown;
							gui.push_back(wd0);
						}

						size_t gui_sec = gui_key.at(L"@leader_" + Str(J->first));

						bias = (int)((1000.0 - Num(J->second.var[L"HP"])) / 1000 * r->w / 4);

						gui.at(gui_sec).enable = true;

						r->x += bias;
						r->y += bias;
						r->w -= bias * 2;
						r->h -= bias * 2;

						gui.at(gui_sec).x = r->x;
						gui.at(gui_sec).y = r->y;
						gui.at(gui_sec).w = (Uint16)r->w;
						gui.at(gui_sec).h = (Uint16)r->h;
						if (gui.at(gui_key.at(script[L"leader_flag"])).parent == gui_sec)
						{
							gui_sec = gui_key.at(script[L"leader_flag"]);
							gui.at(gui_sec).w = (Uint16)r->w;
							gui.at(gui_sec).h = (Uint16)r->h;
						}

						r->x -= bias;
						r->y -= bias;
						r->w += bias * 2;
						r->h += bias * 2;
						r->x += 3 * map_p;
					}
				}
			}
			if (dep == 1)
			{
				p->x = (int)(r->x + I.px * _map_p);
				p->y = (int)(r->y + I.py * _map_p);
				auto J = nat.at(I.OWN);
				nat.at(I.OWN).px = ((1.0 * J.px * J.pnum + p->x) / (J.pnum + 1));
				nat.at(I.OWN).py = ((1.0 * J.py * J.pnum + p->y) / (J.pnum + 1));
				nat.at(I.OWN).pw = ((1.0 * J.pw * J.pnum + I.pnum / (I.y2 - I.y1 + 1)) / (J.pnum + 1));
				nat.at(I.OWN).pnum++;

				/**/
			}
			if ((r->x + r->w > 0 || r->x < scr_w) && (r->y + r->h > 0 || r->y < scr_h))
			{
				if (dep == 0)
				{
					SDL_RenderCopy(REND, I.gt, NULL, r);
					//LOG_W(I.var[L"name"]);
					if (!I.waste_land && I.OWN != L"NAV")
					{
						SDL_SetTextureColorMod(I.t, (Uint8)(nat.at(I.OWN).c / 65536), (Uint8)((nat.at(I.OWN).c / 256) % 256), (Uint8)(nat.at(I.OWN).c % 256));
						SDL_RenderCopy(REND, I.t, NULL, r);
						if (I.OWN != I.CON)
						{
							SDL_SetTextureColorMod(I.lt, (Uint8)(nat.at(I.CON).c / 65536), (Uint8)((nat.at(I.CON).c / 256) % 256), (Uint8)(nat.at(I.CON).c % 256));
							SDL_RenderCopy(REND, I.lt, NULL, r);
						}
					}

				}
				if (dep == 1)
				{
					//if (_map_p >= 6)
					{
						if (!I.waste_land)
						{
							//lf = 4 * map_p;//(0.5f / get_chrs(Str(i)) * _map_p * I->pnum / (I->y2 - I->y1 + 1));
							//TTF_SetFontOutline(tfx[L"default"].t, 2);
							//draw_string(L"default", I.name, L"white", p, (unsigned int)lf, middle_align + center_align);
							//TTF_SetFontOutline(tfx[L"default"].t, 0);
							//draw_string(L"default", I.name, L"black", p, (unsigned int)lf, middle_align + center_align);
						}
					}
				}

			}
		}
	}

	if (script[L"select_leader"] != L"")
	{
		auto I = Num(script[L"select_leader"]);
		if (man.at(I).com.size() > 0)
		{
			SDL_SetRenderDrawColor(REND, 0x30, 0x30, 0x30, 0xFF);
			if (Num(man.at(I).var[L"HP"]) <= 500)
			{
				SDL_SetRenderDrawColor(REND, 0x00, 0xE0, 0xE0, 0xFF);
			}
			if (man.at(I).com.size() > 1)
			{
				auto J0 = man.at(I).com.begin();
				auto J1 = ++(man.at(I).com.begin());
				for (; J1 != man.at(I).com.end(); ++J0, ++J1)
				{
					int x = 0, y = 0, max = (int)(0.5 * map_p + 0.5);
					for (x = -max; x <= max; ++x)
					{
						for (y = -max; y <= max; ++y)
						{
							if (pow(x, 2) + pow(y, 2) < pow(max, 2))
							{
								SDL_RenderDrawLine(REND,
									(int)((prv.at(*J0).x1 + prv.at(*J0).px - scr_w / 2.0 - _map_x) * _map_p + scr_w / 2.0 + x),
									(int)((prv.at(*J0).y1 + prv.at(*J0).py - scr_h / 2.0 - _map_y) * _map_p + scr_h / 2.0 + y),
									(int)((prv.at(*J1).x1 + prv.at(*J1).px - scr_w / 2.0 - _map_x) * _map_p + scr_w / 2.0 + x),
									(int)((prv.at(*J1).y1 + prv.at(*J1).py - scr_h / 2.0 - _map_y) * _map_p + scr_h / 2.0 + y)
								);
							}
						}
					}
				}
			}
			//LOG_W(L"232");
			auto I0 = prv.at(Num(man.at(I).var[L"LOC"]));
			auto I1 = prv.at(*man.at(I).com.begin());
			int x = 0, y = 0, max = (int)(0.5 * map_p + 0.5);
			for (x = -max; x <= max; ++x)
			{
				for (y = -max; y <= max; ++y)
				{
					if (pow(x, 2) + pow(y, 2) < pow(max, 2))
					{
						SDL_RenderDrawLine(REND,
							(int)((I0.x1 + I0.px - scr_w / 2.0 - _map_x) * _map_p + scr_w / 2.0 + x),
							(int)((I0.y1 + I0.py - scr_h / 2.0 - _map_y) * _map_p + scr_h / 2.0 + y),
							(int)((I1.x1 + I1.px - scr_w / 2.0 - _map_x) * _map_p + scr_w / 2.0 + x),
							(int)((I1.y1 + I1.py - scr_h / 2.0 - _map_y) * _map_p + scr_h / 2.0 + y)
						);
					}
				}
			}
			SDL_SetRenderDrawColor(REND, 0x90, 0x90, 0x90, 0xFF);
			x = 0, y = 0, max = (int)(0.3 * map_p + 0.5);
			if (man.at(I).com.size() > 0) {

				double progress = 1.0 * Num(man.at(I).var[L"power"]) / map_connect[{Num(man.at(I).var[L"LOC"]), *man.at(I).com.begin()}] / 3.0;// sqrt(pow(I0.px + I0.x1 - I1.x1 - I1.px, 2) + pow(I0.py + I0.y1 - I1.y1 - I1.py, 2));
				if (progress > 1)
				{
					progress = 1;
				}
				for (x = -max; x <= max; ++x)
				{
					for (y = -max; y <= max; ++y)
					{
						if (pow(x, 2) + pow(y, 2) < pow(max, 2))
						{
							SDL_RenderDrawLine(REND,
								(int)((I0.x1 + I0.px - scr_w / 2.0 - _map_x) * _map_p + scr_w / 2.0 + x),
								(int)((I0.y1 + I0.py - scr_h / 2.0 - _map_y) * _map_p + scr_h / 2.0 + y),
								(int)((I0.x1 + I0.px + (I1.x1 + I1.px - I0.x1 - I0.px) * progress - scr_w / 2.0 - _map_x) * _map_p + scr_w / 2.0 + x),
								(int)((I0.y1 + I0.py + (I1.y1 + I1.py - I0.y1 - I0.py) * progress - scr_h / 2.0 - _map_y) * _map_p + scr_h / 2.0 + y)
							);
						}
					}
				}
			}
		}
	}


	if (true)//_map_p < 10 )
	{
		for (auto I = nat.begin(); I != nat.end(); ++I)
		{

			if (I->second.pnum > 0 && I->first != L"REB" && I->first != L"NAV")
			{
				set_point(p, I->second.px, I->second.py);
				//r->x = p->x - 5;
				//r->y = p->y - 5;
				//r->w = 10;
				//r->h = 10;
				//SDL_SetRenderDrawColor(REND, 0, 0, 255, 255);
				//SDL_RenderFillRect(REND, r);
				lf = 0.5 * I->second.pw * map_p / sqrt(get_chrs(I->second.var[L"title"])) *  sqrt(sqrt((double)I->second.pnum));//  * _map_p ;
				if (_map_p > 1.7)
				{
					TTF_SetFontOutline(tfx[L"default"].t, 2);
					draw_string(L"default", I->second.var[L"title"], L"white", p, (unsigned int)lf, middle_align + center_align);
					TTF_SetFontOutline(tfx[L"default"].t, 0);
				}

				draw_string(L"default", I->second.var[L"title"], L"black", p, (unsigned int)lf, middle_align + center_align);
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
			if (I->var[L"source"] != L"" && gfx[I->var[L"source"]].t != NULL)
			{
				SDL_RenderCopy(REND, gfx[I->var[L"source"]].t, NULL, &r);
			}
			else if (I->var[L"source"] != L"None")
			{
				SDL_RenderCopy(REND, gfx[L"ui\\fuchsia-black"].t, NULL, &r);
			}
			break;
		}
		if (type == wd_label)
		{
			p.x = I->rx;
			p.y = I->ry;
			draw_string(I->var[L"ind"], I->var[L"text"], I->var[L"color"], &p, Num(I->var[L"size"]), Num(I->var[L"opt"]));
			break;
		}
		if (type == wd_text)
		{
			p.x = I->rx;
			p.y = I->ry;

			switch (Num(I->var[L"opt"]) / 3)
			{
			case bottom_align / 3:
				p.y -= Num(I->var[L"size"]) * (get_lines(I->var[L"text"], Num(I->var[L"line"])) - 1);
				break;
			case middle_align / 3:
				p.y -= Num(I->var[L"size"]) * (get_lines(I->var[L"text"], Num(I->var[L"line"])) - 1) / 2;
				break;
			}
			draw_line(I->var[L"ind"], I->var[L"text"], Num(I->var[L"line"]), I->var[L"color"], &p, Num(I->var[L"size"]), Num(I->var[L"opt"]));
			break;
		}
		SDL_RenderCopy(REND, gfx[L"ui\\black-white"].t, NULL, &r);
		break;
	}
}
void child_ui(size_t i, std::vector<Widget>::iterator I, SDL_Rect r)
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
				draw_item(J, r, J->type);
				child_ui(j, J, r);
			}
		}
	}
}
void draw_ui()
{
	static SDL_Rect r;
	static SDL_Point p;
	r.x = 0;
	p.y = 0;

	DRAW_LAST = DRAW_NOW;
	DRAW_NOW = SDL_GetPerformanceCounter();

	double TimeDelta = 60.0 * 100000.0 / (DRAW_NOW - DRAW_LAST);

	draw(&r, &p);
	gui.at(gui_key[L"fps_rend"]).var[L"text"] = Str((unsigned int)(TimeDelta + 0.5)) + L" / " + script[L"CON"];
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

bool init()
{
	setlocale(LC_ALL, "");
	std::ios_base::sync_with_stdio(true);
	//SetConsoleOutputCP(65001);



	wchar_t curDir[1024];
	_wgetcwd(curDir, 1024);
	executeDir = curDir;
	LOG_V(L"EXECUTE DIRECTORY", executeDir);
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		LOG_W(L"SDL_INIT Failed", conv.from_bytes(SDL_GetError()));
		return false;
	}
	WNDW = SDL_CreateWindow("한국사겜", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, scr_w, scr_h, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
	if (WNDW == NULL)
	{
		LOG_W(L"WIN_INIT Failed", conv.from_bytes(SDL_GetError()));
		return false;
	}
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
	GLCN = SDL_GL_CreateContext(WNDW);
	if (GLCN == NULL)
	{
		LOG_W(L"GLC_INIT Failed", conv.from_bytes(SDL_GetError()));
		return false;
	}

	if (SDL_GL_SetSwapInterval(1) < 0)
	{
		LOG_W(L"CANT SET VSYNC");
	}


	if (Mix_OpenAudio(22050, AUDIO_S16SYS, 2, 640) < 0)
	{
		LOG_W(L"MIX_INIT Failed", conv.from_bytes(Mix_GetError()));
		return false;
	}
	SURF = SDL_GetWindowSurface(WNDW);
	REND = SDL_CreateRenderer(WNDW, -1, SDL_RENDERER_ACCELERATED);
	if (REND == NULL)
	{
		LOG_W(L"RND_INIT Failed", conv.from_bytes(SDL_GetError()));
		return false;
	}
	if (TTF_Init() == -1)
	{
		LOG_W(L"TTF_INIT Failed", conv.from_bytes(TTF_GetError()));
		return false;
	}
	init_event();
	return true;
}
bool loadMedia()
{
	bool success = true;

	read_folder(executeDir + L"\\gfx", L"", read_gfx);
	read_folder(executeDir + L"\\tfx", L"", read_tfx);
	read_folder(executeDir + L"\\ui", L"", read_ui);
	//read_folder(executeDir + "\\sfx\\music", "", read_sfx);
	read_folder(executeDir + L"\\define", L"", read_define);
	read_map();
	read_folder(executeDir + L"\\history\\nation", L"", read_nat);
	read_folder(executeDir + L"\\history\\province", L"", read_prv);
	read_folder(executeDir + L"\\history\\character", L"", read_man);
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
	event_key.clear();
}

#ifdef DEBUG

void LOG_A(std::wstring s)
{
	std::wcout << ESCAPE << L"[1;33m[Attention]" << ESCAPE << L"[1;37m " << s << '\n';
}
void LOG_W(std::wstring s)
{
	std::wcout << ESCAPE << L"[1;31m[Warning]" << ESCAPE << L"[1;37m " << s << '\n';
}
void LOG_H(std::wstring s)
{
	std::wcout << ESCAPE << L"[1;34m[Info]" << ESCAPE << L"[1;37m " << s << '\n';
}
void LOG_O(std::wstring s)
{
	std::wcout << ESCAPE << L"[1;32m[OK]" << ESCAPE << L"[1;37m " << s << '\n';
}
void LOG_V(std::wstring s)
{
	std::wcout << ESCAPE << L"[1;37m[OK]" << ESCAPE << L"[1;37m " << s << '\n';
}
void LOG_A(std::wstring s, std::wstring s2)
{
	std::wcout << ESCAPE << L"[1;33m[Info]" << ESCAPE << L"[1;37m " << s << " : " << s2 << '\n';
}
void LOG_W(std::wstring s, std::wstring s2)
{
	std::wcout << ESCAPE << L"[1;31m[Warning]" << ESCAPE << L"[1;37m " << s << " : " << s2 << '\n';
}
void LOG_H(std::wstring s, std::wstring s2)
{
	std::wcout << ESCAPE << L"[1;34m[Info]" << ESCAPE << L"[1;37m " << s << " : " << s2 << '\n';
}
void LOG_O(std::wstring s, std::wstring s2)
{
	std::wcout << ESCAPE << L"[1;32m[OK]" << ESCAPE << L"[1;37m " << s << " : " << s2 << '\n';
}
void LOG_V(std::wstring s, std::wstring s2)
{
	std::wcout << ESCAPE << L"[1;37m[VAR]" << ESCAPE << L"[1;37m " << s << " : " << s2 << '\n';
}
void LOG_Stop()
{
	std::wcout << L"PRESS ANY KEY...";
	//std::wcin.ignore();
	return;
}
#else
void LOG_A(std::wstring s)
{
	logging << "[Attention] " << s << "\n";
}
void LOG_W(std::wstring s)
{
	logging << "[Warning] " << s << "\n";
}
void LOG_H(std::wstring s)
{
	logging << "[Info] " << s << "\n";
}
void LOG_O(std::wstring s)
{
	logging << "[OK] " << s << "\n";
}
void LOG_V(std::wstring s)
{
	logging << "[OK] " << s << "\n";
}
void LOG_A(std::wstring s, std::wstring s2)
{
	logging << "[Info] " << s << " : " << s2 << "\n";
}
void LOG_W(std::wstring s, std::wstring s2)
{
	logging << "[Warning] " << s << " : " << s2 << "\n";
}
void LOG_H(std::wstring s, std::wstring s2)
{
	logging << "[Info] " << s << " : " << s2 << "\n";
}
void LOG_O(std::wstring s, std::wstring s2)
{
	logging << "[OK] " << s << " : " << s2 << "\n";
}
void LOG_V(std::wstring s, std::wstring s2)
{
	logging << "[VAR] " << s << " : " << s2 << "\n";
}
void LOG_Stop()
{
	logging.close();
	return;
}
#endif

int main(int argc, char* args[])
{
	SDL_SetMainReady();
	srand((unsigned int)time(NULL));
	system("color 01");

	if (!init())
	{
		LOG_W(L"System Initialize Failed");
	}
	else
	{
		if (!loadMedia())
		{
			LOG_W(L"System Load Failed");
		}
		else
		{
			start();
			LOG_O(L"GAME ON");
			SDL_Event e;
			SDL_StartTextInput();

			//std::thread trd_ai(run_ai);

			int tick = 0;
			while (!quit)
			{
				++tick;
				if (tick % 3 == 0) go_ai();
				step(&e);
				draw_ui();
			}
			SDL_StopTextInput();
			LOG_O(L"GAME OFF");
		}
	}
	close();
	LOG_Stop();
	return 0;
}
