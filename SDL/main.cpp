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
			std::thread trd_ai(run_ai);

			while (!quit)
			{
				step(&e);
			}
			trd_ui.join();
			trd_ai.join();
			SDL_StopTextInput();
			LOG_O("GAME OFF");
		}
	}
	close();
	LOG_Stop();
	return 0;
}

unsigned int day_over()
{
	unsigned int year = Num(script.at("year"));
	unsigned int mon = Num(script.at("month"));
	unsigned int day = Num(script.at("day"));
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
			script["year"] = Str(year);
			script["month"] = Str(mon);
			script["day"] = Str(day);
			return 2;
		}
		else if (day > 29 && is_yun)
		{
			day = 1;
			mon++;
			script["year"] = Str(year);
			script["month"] = Str(mon);
			script["day"] = Str(day);
			return 2;
		}
	}
	else if ((mon % 2 == 1 && mon <= 7) || (mon % 2 == 0 && mon >= 8))
	{
		if (day > 31)
		{
			day = 1;
			mon++;
			script["year"] = Str(year);
			script["month"] = Str(mon);
			script["day"] = Str(day);
			return 2;
		}
	}
	else if (day > 30)
	{
		day = 1;
		mon++;
		script["year"] = Str(year);
		script["month"] = Str(mon);
		script["day"] = Str(day);
		return 2;
	}
	if (mon > 12)
	{
		mon = 1;
		year++;
		script["year"] = Str(year);
		script["month"] = Str(mon);
		script["day"] = Str(day);
		return 3;
	}
	script["year"] = Str(year);
	script["month"] = Str(mon);
	script["day"] = Str(day);
	return 1;
}

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
		wd0.parent = wd0.id;
		gui.push_back(wd0);
	}
	{
		Widget wd0(0, 0, scr_w, scr_h, wd_image, "Backmap");
		wd0.enable = true;
		wd0.id = gui.size();
		wd0.var["source"] = "None";
		wd0.parent = wd0.id;
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
		clr.r = 0;
		clr.g = 255;
		clr.b = 0;
		Color["green"] = clr;
	}
	{
		SDL_Color clr;
		clr.a = 255;
		clr.r = 255;
		clr.g = 0;
		clr.b = 0;
		Color["red"] = clr;
	}
	{
		SDL_Color clr;
		clr.a = 255;
		clr.r = 255;
		clr.g = 255;
		clr.b = 0;

		Color["yellow"] = clr;
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
			LOG_W("This province is unavailable",I.var["name"]);
		}
	}

	//get_path(40, 49);

	//Mix_PlayMusic(sfx["sample"], -1);
}
void run_ai()
{
	//Ai
	unsigned int n = 0;
	unsigned int i = 0;
	size_t syc = 0;
	size_t min_size_t = std::numeric_limits<size_t>::min();
	unsigned int target = 0;
	int size;
	std::vector<unsigned int> own_man;
	std::string ruler, last_nat = "REB";
	bool flag_diffence = false;
	bool flag_mine = false;
	bool flag_myrealm = false;

	std::vector<AiProvince> ai_prv;

	GUI_MUTX.lock();
	for (auto I = prv.begin(); I != prv.end(); ++I)
	{
		ai_prv.push_back(AiProvince());
		if (I->enable && !I->waste_land)
		{
			ai_prv.rbegin()->enable = true;
		}
	}
	GUI_MUTX.unlock();


	while (!quit)
	{
		i = 0;
		GUI_MUTX.lock();

		auto N = nat.find(last_nat);
		if (N == nat.end())
		{
			last_nat = "REB";
		}
		ruler = N->first;

		//Main
		if (ruler != script["CON"])
		{
			for (auto I = man.begin(); I != man.end(); ++I)
			{
				if (I->second.var["live"] == "1")
				{
					if (I->second.var["CON"] == ruler)
					{
						++i;
						break;
					}
				}
			}

			i = 0;
			for (auto I = prv.begin(); I != prv.end(); ++I, ++i)
			{
				ai_prv.at(i).attack = 0;
				if (I->var["CON"] != ruler)
				{
					ai_prv.at(i).attack = 60;
				}
			}

			for (auto I = man.begin(); I != man.end(); ++I)
			{
				if (I->second.var["live"] == "1")
				{
					size = Num(I->second.var["HP"]) / 100;

					//is mine?
					if (I->second.var["CON"] == ruler)
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
					if (I->second.var["CON"] == prv.at(Num(I->second.var["LOC"])).var["CON"])
					{
						flag_diffence = true;
					}
					else
					{
						flag_diffence = false;
					}

					//is my realm?
					if (ruler == prv.at(Num(I->second.var["LOC"])).var["CON"])
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
					ai_prv.at(Num(I->second.var["LOC"])).attack = size;
				}
			}

			
			for (auto R = own_man.begin(); R != own_man.end(); ++R)
			{
				auto I = man.at(*R);
				if (I.var["live"] == "1")
				{
					syc = min_size_t;// *r % 300;
					target = Num(I.var["LOC"]);
					GUI_MUTX.unlock();
					get_len(&ai_prv, Num(I.var["LOC"]));
					GUI_MUTX.lock();
					
					i = 0;
					for (auto P = ai_prv.begin(); P != ai_prv.end(); ++P, ++i)
					{
						if (P->enable && P->nearest != ai_prv.size())
						{
							if (P->attack - P->len * 10 > syc)
							{
								syc = P->attack - P->len * 10;
								target = i;
							}
						}
					}
					if (syc - ai_prv.at(Num(I.var["LOC"])).attack > (I.com.size() != 0 ? 100 : 0))
					{
						if ((unsigned int)Num(I.var["LOC"]) != target)
						{
							put_path(*R, Num(I.var["LOC"]), target);
						}
						ai_prv.at(target).attack -= Num(I.var["HP"]);
						ai_prv.at(Num(I.var["LOC"])).attack += Num(I.var["HP"]);
					}
				}

			}
			
			own_man.clear();
		}

		--N;
		if (N == nat.end())
		{
			last_nat = "REB";
		}
		else
		{
			last_nat = N->first;
		}
		GUI_MUTX.unlock();
	}
}
void step(SDL_Event *e)
{
	
	STEP_LAST = STEP_NOW;
	STEP_NOW = SDL_GetTicks();
	GUI_MUTX.lock();

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
		if (I->second.var["live"] != "1")
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
				if (I->enable && !I->waste_land)
				{
					if (I->var["OWN"] == I->var["CON"])
					{
						//Rebel Uprise
						if (I->var["OWN"] == "REB")
						{
							//LOG_W(I->var["name"]);
							script["TEMP"] = "REB";
							size_t num = std::numeric_limits<size_t>::max();
							size_t prv_num = 0;
							for (auto J : nat)
							{
								if (J.first != "NAV")
								{
									prv_num = 0;
									for (auto K : prv)
									{
										if (K.var["OWN"] == J.first)
										{
											prv_num++;
										}
									}
									if (prv_num < num)
									{
										script["TEMP"] = J.first;
										num = prv_num;
									}
								}
							}
							if (script["TEMP"] != "REB")
							{
								I->var["OWN"] = script["TEMP"];
								I->var["CON"] = script["TEMP"];

								for (unsigned int r = 0; r < 3; ++r)
								{
									Man obj;
									obj.var["name"] = "가가 가가";
									obj.var["LOC"] = Str(i);
									obj.var["HOME"] = Str(i);
									obj.var["CON"] = script["TEMP"];
									man[ALL_MAN++] = obj;
								}
							}
						}

						if (I->var["OWN"] != "NAV")
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
					if (I->pi.man >= 1000)
					{
						Man obj;
						obj.var["name"] = "가가 가가";
						obj.var["LOC"] = Str(i);
						obj.var["HOME"] = Str(i);
						obj.var["CON"] = I->var["OWN"];
						I->pi.man = 0;
						man[ALL_MAN++] = obj;
					}
				}
			}
			for (auto I = man.begin(); I != man.end(); ++I)
			{
				if (I->second.var["live"] == "1")
				{
					//With Man
					{
						size_t enemey = 0;
						for (auto J = man.begin(); J != man.end(); ++J)
						{
							if (J->second.var["live"] == "1" && I->second.var["LOC"] == J->second.var["LOC"] && I->second.var["CON"] != J->second.var["CON"])
							{
								++enemey;
							}
							if (enemey)
							{
								if (I->second.var["purpose"] == "heal")
								{
									I->second.var["purpose"] = "";
									I->second.var["power"] = "0";
								}
								if (I->second.var["purpose"] == "sieze")
								{
									I->second.var["purpose"] = "";
									I->second.var["power"] = "0";
								}
							}
						}
						if (enemey && I->second.var["purpose"] == "")
						{
							for (auto J = man.begin(); J != man.end(); ++J)
							{
								if (J->second.var["live"] == "1" && I->second.var["LOC"] == J->second.var["LOC"] && I->second.var["CON"] != J->second.var["CON"])
								{
									if (rand() % (--enemey + 1) == 0)
									{
										J->second.var["HP"] = Str((int)(Num(J->second.var["HP"]) - (1 + Num(I->second.var["attack"])) * (1 + (I->second.var["CON"] == prv.at(Num(I->second.var["LOC"])).var["CON"] ? 1 : 0)) * Num(I->second.var["HP"]) / 1000));
										if (Num(J->second.var["HP"]) < 0)
										{
											J->second.var["live"] = "0";

											size_t sec = gui_key.at(script["leader_flag"]);
											if (gui.at(sec).parent == gui_key["@leader_" + Str(J->first)])
											{
												gui.at(sec).parent = sec;
											}
											gui.at(gui_key["@leader_" + Str(J->first)]).removing = true;
										}
										else
										{
											if (Num(J->second.var["HP"]) <= 500 && J->second.var["purpose"] != "move")
											{
												auto K = prv.at(Num(J->second.var["LOC"]));

												Widget wd0((Sint32)((K.x1 + K.px - scr_w / 2.0 - map_x) * map_p + scr_w / 2.0), (Sint32)((K.y1 + K.py - scr_h / 2.0 - map_y) * map_p + scr_h / 2.0), 0, 0, wd_label, "SiezeMessage" + Str(rand()));
												wd0.enable = true;
												wd0.id = gui.size();
												wd0.parent = wd0.id;

												wd0.var["ind"] = "default";
												wd0.var["size"] = "20";
												wd0.var["opt"] = "11";
												wd0.var["color"] = "white";
												wd0.var["delay"] = "0";
												wd0.var["life"] = "1600";
												wd0.ev.Event_step = BlowupLabel_step;

												wd0.var["text"] = "후퇴!";

												I->second.var["power"] = "0";
												gui.push_back(wd0);
												run_to_rest(J->first);
												J->second.var["purpose"] = "move";
											}
											if (J->second.var["purpose"] == "sieze")
											{
												J->second.var["power"] = "0";
												J->second.var["purpose"] = "";
											}
											if (J->second.var["purpose"] == "heal")
											{
												J->second.var["purpose"] = "";
												J->second.var["power"] = "0";
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
						auto P = prv.at(Num(I->second.var["HOME"]));
						if (P.enable && !P.waste_land)
						{
							if (P.pi.man > 10)
							{
								P.pi.man -= 10;
							}
						}

						if (I->second.com.size() > 0)
						{
							if (I->second.var["purpose"] != "move")
							{
								I->second.var["purpose"] = "move";
								I->second.var["power"] = "0";
							}
							auto I0 = prv.at(Num(I->second.var["LOC"]));
							auto I1 = prv.at(*I->second.com.begin());

							if (I0.c == I1.c)
							{
								I->second.com.pop_front();
							}
							else if (map_reg["map_conect" + I->second.var["LOC"] + "/" + Str(*I->second.com.begin())] * 3 <= (unsigned int)Num(I->second.var["power"]))
							{
								I->second.var["LOC"] = Str(*I->second.com.begin());
								I->second.com.pop_front();
								I->second.var["power"] = "0";
							}
							else
							{
								I->second.var["power"] = Str(Num(I->second.var["power"]) + 2 + (10 - Num(I->second.var["HP"]) / 100) / 2);
							}
							continue;
						}
					}
					//With Province
					{
						auto J = prv.at(Num(I->second.var["LOC"]));

						//Sieze Land
						if (!J.waste_land && J.var["CON"] != I->second.var["CON"])
						{
							if (I->second.var["purpose"] != "sieze")
							{
								I->second.var["purpose"] = "sieze";
								I->second.var["power"] = "0";
							}
							if (Num(I->second.var["power"]) <= 90)
							{
								I->second.var["power"] = Str(Num(I->second.var["power"]) + 10 + (Num(I->second.var["HP"]) / 100));
							}
							else
							{

								Widget wd0((Sint32)((J.x1 + J.px - scr_w / 2.0 - map_x) * map_p + scr_w / 2.0), (Sint32)((J.y1 + J.px - scr_h / 2.0 - map_y) * map_p + scr_h / 2.0), 0, 0, wd_label, "SiezeMessage" + Str(rand()));
								wd0.enable = true;
								wd0.id = gui.size();
								wd0.parent = wd0.id;

								wd0.var["ind"] = "default";
								wd0.var["size"] = "20";
								wd0.var["opt"] = "11";
								wd0.var["color"] = "white";
								wd0.var["delay"] = "0";
								wd0.var["life"] = "1600";
								wd0.ev.Event_step = BlowupLabel_step;

								wd0.var["text"] = "돌격!";
								if (rand() % 100 < 14)
								{
									prv[Num(I->second.var["LOC"])].var["CON"] = I->second.var["CON"];
									prv[Num(I->second.var["LOC"])].var["OWN"] = I->second.var["CON"];
									wd0.var["text"] = "적 투항";
								}

								I->second.var["power"] = "0";
								gui.push_back(wd0);
							}
						}
						else if (J.var["OWN"] == I->second.var["CON"])
						{
							if (I->second.var["purpose"] != "heal")
							{
								I->second.var["purpose"] = "heal";
								I->second.var["power"] = "0";
							}
							if (Num(I->second.var["power"]) <= 30)
							{
								I->second.var["power"] = Str(Num(I->second.var["power"]) + 2);
							}
							else
							{
								I->second.var["power"] = Str(Num(I->second.var["power"]) + 1);
								if (Num(I->second.var["HP"]) + 30 <= 1000)
								{
									auto P = prv.at(Num(I->second.var["HOME"]));
									if (P.enable && !P.waste_land)
									{
										if (P.pi.man > 30)
										{
											I->second.var["HP"] = Str(Num(I->second.var["HP"]) + 30);
											P.pi.man -= 30;
										}
									}
								}
							}
						}
					}

					if (Num(I->second.var["power"]) > 0)
					{
						I->second.var["power"] = Str(Num(I->second.var["power"]) - 1);
					}
					if (Num(I->second.var["HP"]) < 0)
					{
						LOG_W("HP", I->second.var["HP"]);
						I->second.var["live"] = "0";

						size_t sec = gui_key.at(script["leader_flag"]);
						if (gui.at(sec).parent == gui_key["@leader_" + Str(I->first)])
						{
							gui.at(sec).parent = sec;
						}
						gui.at(gui_key["@leader_" + Str(I->first)]).removing = true;
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
				if (I.enable && I.id == I.parent)
				{ 
					size_t b = ray_ui(i,x,y);
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
				LOG_O("Take a Screenshot.");
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

	GUI_MUTX.unlock();
		
	{
		double DRAG_SENSITIVE = 0.5 * TimeDelta;// / sqrt(map_p);
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
		
	if (map_p > 16)
	{
		map_p = 16;
	}

	double _map_x = map_x;
	double _map_y = map_y;
	double _map_p = map_p;

	for (unsigned int dep = 0; dep < 3; ++dep)
	{
		i = 0;
		for (auto I = prv.begin(); I != prv.end(); ++I, ++i)
		{
			set_rect(r, (I->x1 - scr_w / 2.0 - _map_x) * _map_p + scr_w / 2.0, (I->y1 - scr_h / 2.0 - _map_y) * _map_p + scr_h / 2.0, (I->x2 - I->x1 + 1.0) * _map_p, (I->y2 - I->y1 + 1.0) * _map_p);
			if (I->enable)
			{
				if (dep == 2)
				{
					p->x = (int)(r->x + (I->px) * _map_p + 0.5);
					p->y = (int)(r->y + (I->py) * _map_p + 0.5);

					r->x = (int)(p->x - 3 * map_p + 0.5);
					r->y = (int)(p->y - 3 * map_p + 0.5);
					r->w = (int)(3 * map_p + 0.5);
					r->h = (int)(3 * map_p + 0.5);
					r->x += r->w / 2;
					unsigned int num = 0;

					for (auto J = man.begin(); J != man.end(); ++J)
					{
						if (J->second.var["live"] == "1" && J->second.var["LOC"] == Str(i))
						{
							++num;
							if (num == 5)
							{
								break;
							}
						}
					}

					r->x -= (num - 1) * ((int)(4 * map_p + 0.5)) / 2;
					int bias;
					for (auto J = man.begin(); J != man.end(); ++J)
					{
						if (J->second.var["live"] == "1" && J->second.var["LOC"] == Str(i))
						{
							if (gui_key.find("@leader_" + Str(J->first)) == gui_key.end())
							{
								Widget wd0 = Widget(0, 0, 0, 0, wd_image, "@leader_" + Str(J->first));
								wd0.var["source"] = "flag\\" + J->second.var["CON"];
								wd0.var["id"] = Str(J->first);
								wd0.ev.Event_mousedown = LittleUp_mousedown;
								gui.push_back(wd0);
							}

							size_t gui_sec = gui_key.at("@leader_" + Str(J->first));
							
							bias = (int)((1000.0 - Num(J->second.var["HP"])) / 1000 * r->w / 4);

							gui.at(gui_sec).enable = true;

							r->x += bias;
							r->y += bias;
							r->w -= bias * 2;
							r->h -= bias * 2;

							gui.at(gui_sec).x = r->x;
							gui.at(gui_sec).y = r->y;
							gui.at(gui_sec).w = (Uint16)r->w;
							gui.at(gui_sec).h = (Uint16)r->h;
							if (gui.at(gui_key.at(script["leader_flag"])).parent == gui_sec)
							{
								gui_sec = gui_key.at(script["leader_flag"]);
								gui.at(gui_sec).w = (Uint16)r->w;
								gui.at(gui_sec).h = (Uint16)r->h;
							}

							r->x -= bias;
							r->y -= bias;
							r->w += bias * 2;
							r->h += bias * 2;


							if (num == 0)
							{
								r->x += (int)(0.8 * map_p + 0.5);
							}
							else
							{
								r->x += (int)(4 * map_p + 0.5);
								--num;
							}
						}
					}
				}
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
						//if (_map_p >= 6)
						{
							if (!I->waste_land)
							{
							//	lf = 4 * map_p;//(0.5f / get_chrs(Str(i)) * _map_p * I->pnum / (I->y2 - I->y1 + 1));
							//	TTF_SetFontOutline(tfx["default"].t, 2);
							//	draw_string("default", Str(I->attack), "white", p, (unsigned int)lf, middle_align + center_align);
							//	TTF_SetFontOutline(tfx["default"].t, 0);
							//	draw_string("default", Str(I->attack), "black", p, (unsigned int)lf, middle_align + center_align);
							}
						}
					}
				}
			}
		}
	}
		
	if (script["select_leader"] != "")
	{
		auto I = Num(script["select_leader"]);
		if (man.at(I).com.size() > 0)
		{
			SDL_SetRenderDrawColor(REND, 0x30, 0x30, 0x30, 0xFF);
			if (Num(man.at(I).var["HP"]) <= 500)
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
			//LOG_W("232");
			auto I0 = prv.at(Num(man.at(I).var["LOC"]));
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
			double progress = 1.0 * Num(man.at(I).var["power"]) / map_reg["map_conect" + man.at(I).var["LOC"] + "/" + Str(*man.at(I).com.begin())] / 3.0;// sqrt(pow(I0.px + I0.x1 - I1.x1 - I1.px, 2) + pow(I0.py + I0.y1 - I1.y1 - I1.py, 2));
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

	
	if (true)//_map_p < 10 )
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
				lf = 0.5 * I->second.pw * map_p / sqrt(get_chrs(I->second.var["title"])) *  sqrt(sqrt((double)I->second.pnum));//  * _map_p ;
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

		DRAW_LAST = DRAW_NOW;
		DRAW_NOW = SDL_GetPerformanceCounter();

		double TimeDelta = 60.0 * 100000.0 / (DRAW_NOW - DRAW_LAST);

		GUI_MUTX.lock();
		draw(&r, &p);		
		gui.at(gui_key["fps_rend"]).var["text"] = Str((unsigned int)(TimeDelta + 0.5));
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
		GUI_MUTX.unlock();

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
	WNDW = SDL_CreateWindow("한국사겜", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, scr_w, scr_h, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
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
	init_event();
	return true;
}
bool loadMedia()
{
	bool success = true;

	read_folder(executeDir + "\\gfx","", read_gfx);
	read_folder(executeDir + "\\tfx", "", read_tfx);
	read_folder(executeDir + "\\ui", "", read_ui);
	//read_folder(executeDir + "\\sfx\\music", "", read_sfx);
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
	event_key.clear();
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