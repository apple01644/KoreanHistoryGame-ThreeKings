#pragma once
#include "act.h"

size_t mother(const size_t i)
{
	if (gui.at(i).id == gui.at(i).parent)
	{
		return gui.at(i).id;
	}
	else
	{
		return mother(gui.at(i).parent);
	}
}
void parent_front(const size_t id)
{
	//for (unsigned int mom = mother(id); mom != gui.size() - 1; ++mom)
	//{
	//	for (unsigned int a = 0; a < gui.size(); a++)
	//	{
	//		if (gui.at(a).parent == mom)
	//		{
	//			gui.at(a).parent = mom + 1;
	//		}
	//		else if (gui.at(a).parent == mom + 1)
	//		{
	//			gui.at(a).parent = mom;
	//		}
	//	}
	//	gui.at(mom).id = mom + 1;
	//	gui_key[gui.at(mom).var["name"]] = mom + 1;
	//	gui.at(mom + 1).id = mom;
	//	gui_key[gui.at(mom).var["name"]] = mom;
	//	std::iter_swap(gui.at(mom), gui.at(mom + 1));
	//}
} 
void parent_remove(const unsigned int id)
{
	gui.at(gui.at(id).parent).removing = true;
}

void BackMap_mousedown(size_t owner, Arg_Mouse e)
{
	unsigned int sec = map_reg[Str((unsigned int)((e.x + (scr_w / 2.0 + map_x) * map_p - scr_w / 2.0) / map_p)) + ":" + Str((unsigned int)((e.y + (scr_h / 2.0 + map_y) * map_p - scr_h / 2.0) / map_p))];
	if (script["select_leader"] == "")
	{
		if (prv[sec].enable)
		{

			/*if (e.button == SDL_BUTTON_LEFT)
			{
				if (!prv.at(sec).waste_land)
				{
					if (prv.at(sec).var["CON"] == script["CON"])
					{
						if (script["CON"] == "REB")
						{
							auto I = nat.cbegin();
							script["TEMP"] = I->first;
							int i = 0;
							do
							{
								I = nat.cbegin();
								for (i = rand() % nat.size(); i > 0; --i)
								{
									++I;
								}
								++i;
								script["TEMP"] = I->first;
							} while ((I->second.pnum != 0 || I->first == "REB" || I->first == "NAV") && i < 300);
							if (i < 300)
							{
								prv.at(sec).var["OWN"] = script["TEMP"];
								prv.at(sec).var["CON"] = script["TEMP"];
							}
						}
						else
						{
							prv.at(sec).var["OWN"] = script["CON"];
						}
					}
					else
					{
						prv.at(sec).var["CON"] = script["CON"];
					}
				}
			}*/
			if (e.button == SDL_BUTTON_LEFT)
			{
				if (!prv.at(sec).waste_land)
				{
					if (prv.at(sec).var["OWN"] == prv.at(sec).var["CON"])
					{
						script["CON"] = prv.at(sec).var["CON"];
						gui.at(gui_key["nation_flag"]).var["source"] = "flag\\" + script["CON"];
						/*Man obj;
						obj.var["name"] = "가가 가가";
						obj.var["LOC"] = Str(sec);
						obj.var["CON"] = prv.at(sec).var["CON"];
						man[ALL_MAN++] = obj;
						LOG_W("man born");*/
					}
					else
					{
						prv[sec].var["OWN"] = prv.at(sec).var["CON"];
/*
						for (unsigned int i = 0; i < 6; ++i)
						{
							Man obj;
							obj.var["name"] = "가가 가가";
							obj.var["LOC"] = Str(sec);
							obj.var["CON"] = prv.at(sec).var["CON"];
							man[ALL_MAN++] = obj;
						}*/
						//LOG_W("man born");
					}
				}
			}
	/*		if (e.button == SDL_BUTTON_MIDDLE)
			{
				script["CON"] = prv.at(sec).var["OWN"];
				gui.at(gui_key["nation_flag"]).var["source"] = "flag\\" + script["CON"];
			}*/
			if (e.button == SDL_BUTTON_MIDDLE)
			{
				if (!prv.at(sec).waste_land)
				{
					prv.at(sec).var["OWN"] = "REB";
					prv.at(sec).var["CON"] = "REB";
					gui.at(gui_key["nation_flag"]).var["source"] = "flag\\REB";
				}
			}
		}
	}
	else
	{
		if (e.button == SDL_BUTTON_LEFT)
		{
			script["select_leader"] = "";
			gui.at(gui_key.at(script["leader_flag"])).enable = false;
		}
		if (e.button == SDL_BUTTON_RIGHT)
		{
			auto I = Num(script["select_leader"]);
			if (man.at(I).var["LOC"] != Str(sec))
			{
				put_path(I, Num(man.at(I).var["LOC"]), sec);
				/*if (map_reg["map_conect"+man.at(I).var["LOC"] + "/" + Str(sec)] != 0)
				{
					
					man.at(I).com.clear();
					man.at(I).com.push_back(sec);
				}*/
			}
			else
			{
				man.at(I).com.clear();
			/*	if (prv.at(sec).var["CON"] != man.at(I).var["CON"])
				{
					prv.at(sec).var["CON"] = man.at(I).var["CON"];
				}
				else
				{
					prv.at(sec).var["OWN"] = man.at(I).var["CON"];
				}*/
			}
		}
	}
}

void LittleUp_mousedown(size_t owner, Arg_Mouse e)
{
	if (e.button == SDL_BUTTON_LEFT)
	{
		size_t sec = gui_key.at(script["leader_flag"]);
		script["select_leader"] = gui.at(owner).var["id"];
		gui.at(sec).enable = true;
		gui.at(sec).x = 0;
		gui.at(sec).y = 0;
		gui.at(sec).w = gui.at(owner).w;
		gui.at(sec).h = gui.at(owner).h;
		gui.at(sec).parent = owner;
		return;
	}
	if (e.button == SDL_BUTTON_RIGHT && script["select_leader"] == "")
	{
		man.erase(Num(gui.at(owner).var["id"]));
		
		size_t sec = gui_key.at(script["leader_flag"]);
		if (gui.at(sec).parent == owner)
		{
			gui.at(sec).parent = sec;
		}
		gui.at(owner).removing = true;
	}
}
void Highlight_leader_flag_mousedown(size_t owner, Arg_Mouse e)
{
	if (e.button == SDL_BUTTON_LEFT)
	{
		gui[owner].enable = false;
		gui[owner].parent = gui.at(owner).id;
		script["select_leader"] = "";
		return;
	}
}
void Yeartimer_step(size_t owner)
{
	if (pause)
	{
		gui.at(owner).var["text"] = "Pause";
		gui.at(owner).var["color"] = "red";
		return;
	}
	gui.at(owner).var["color"] = "black";
	
	script["buf"] = "";
	for (auto I : script["year_timer_place_holder"])
	{
		if (I == 'Y')
		{
			script.at("buf") += script.at("year");
			continue;
		}
		if (I == 'M')
		{
			script.at("buf") += script.at("month");
			continue;
		}
		if (I == 'D')
		{
			script.at("buf") += script.at("day");
			continue;
		}
		script.at("buf").push_back(I);
	}
	gui.at(owner).var["text"] = script.at("buf");
} 

void BlowupLabel_step(size_t owner)
{
	if (Num(gui.at(owner).var["life"]) <= 0)
	{
		gui.at(owner).removing = true;
	}
	if (Num(gui.at(owner).var["delay"]) <= 0)
	{
		gui.at(owner).y -= 1;
		gui.at(owner).var["size"] = Str((int)(Num(gui.at(owner).var["size"]) + (40 - Num(gui.at(owner).var["size"])) / 6));
		gui.at(owner).var["delay"] = "60";
	}
	else
	{
		gui.at(owner).var["delay"] = Str((int)(Num(gui.at(owner).var["delay"]) - (STEP_NOW - STEP_LAST)));
		gui.at(owner).var["life"] = Str((int)(Num(gui.at(owner).var["life"]) - (STEP_NOW - STEP_LAST)));
	}
}

void init_event()
{
	std::string sec = "";

	event_key[sec = "Yeartimer_step"].type = ev_step;
	event_key[sec].step = Yeartimer_step;

	event_key[sec = "LittleUp_mousedown"].type = ev_mousedown;
	event_key[sec].mousedown = LittleUp_mousedown;

	event_key[sec = "Highlight_leader_flag"].type = ev_mousedown;
	event_key[sec].mousedown = Highlight_leader_flag_mousedown;
}