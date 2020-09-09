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
	unsigned int sec = map_reg[(unsigned int)((e.x + (scr_w / 2.0 + map_x) * map_p - scr_w / 2.0) / map_p) + map_w * (unsigned int)((e.y + (scr_h / 2.0 + map_y) * map_p - scr_h / 2.0) / map_p)];
	if (script[L"select_leader"] == L"")
	{
		if (!prv[sec].waste_land)
		{

			if (e.button == SDL_BUTTON_LEFT)
			{
				if (!prv.at(sec).waste_land)
				{
					script[L"CON"] = prv.at(sec).CON;
					gui.at(gui_key[L"nation_flag"]).var[L"source"] = L"flag\\" + script[L"CON"];
					LOG_W(L"ID: " + Str(sec), L"OWN:" + prv.at(sec).OWN + L" / CON:" + prv.at(sec).CON);
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
					prv.at(sec).OWN = L"REB";
					prv.at(sec).CON = L"REB";
					gui.at(gui_key[L"nation_flag"]).var[L"source"] = L"flag\\REB";
				}
			}
		}
	}
	else
	{
		if (e.button == SDL_BUTTON_LEFT)
		{
			script[L"select_leader"] = L"";
			gui.at(gui_key.at(script[L"leader_flag"])).enable = false;
		}
		if (e.button == SDL_BUTTON_RIGHT)
		{
			auto I = Num(script[L"select_leader"]);
			if (man.at(I).var[L"LOC"] != Str(sec))
			{
				put_path(I, Num(man.at(I).var[L"LOC"]), sec);
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
		size_t sec = gui_key.at(script[L"leader_flag"]);
		script[L"select_leader"] = gui.at(owner).var[L"id"];
		gui.at(sec).enable = true;
		gui.at(sec).x = 0;
		gui.at(sec).y = 0;
		gui.at(sec).w = gui.at(owner).w;
		gui.at(sec).h = gui.at(owner).h;
		gui.at(sec).parent = owner;
		return;
	}
	if (e.button == SDL_BUTTON_RIGHT && script[L"select_leader"] == L"")
	{
		man.erase(Num(gui.at(owner).var[L"id"]));

		size_t sec = gui_key.at(script[L"leader_flag"]);
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
		script[L"select_leader"] = L"";
		return;
	}
}
void Yeartimer_step(size_t owner)
{
	if (pause)
	{
		gui.at(owner).var[L"text"] = L"Pause";
		gui.at(owner).var[L"color"] = L"red";
		return;
	}
	gui.at(owner).var[L"color"] = L"black";

	script[L"buf"] = L"";
	for (auto I : script[L"year_timer_place_holder"])
	{
		if (I == 'Y')
		{
			script.at(L"buf") += script.at(L"year");
			continue;
		}
		if (I == 'M')
		{
			script.at(L"buf") += script.at(L"month");
			continue;
		}
		if (I == 'D')
		{
			script.at(L"buf") += script.at(L"day");
			continue;
		}
		script.at(L"buf").push_back(I);
	}
	gui.at(owner).var[L"text"] = script.at(L"buf");
}

void BlowupLabel_step(size_t owner)
{
	if (Num(gui.at(owner).var[L"life"]) <= 0)
	{
		gui.at(owner).removing = true;
	}
	if (Num(gui.at(owner).var[L"delay"]) <= 0)
	{
		gui.at(owner).y -= 1;
		gui.at(owner).var[L"size"] = Str((int)(Num(gui.at(owner).var[L"size"]) + (40 - Num(gui.at(owner).var[L"size"])) / 6));
		gui.at(owner).var[L"delay"] = L"60";
	}
	else
	{
		gui.at(owner).var[L"delay"] = Str((int)(Num(gui.at(owner).var[L"delay"]) - (STEP_NOW - STEP_LAST)));
		gui.at(owner).var[L"life"] = Str((int)(Num(gui.at(owner).var[L"life"]) - (STEP_NOW - STEP_LAST)));
	}
}

void init_event()
{
	std::wstring sec = L"";

	event_key[sec = L"Yeartimer_step"].type = ev_step;
	event_key[sec].step = Yeartimer_step;

	event_key[sec = L"LittleUp_mousedown"].type = ev_mousedown;
	event_key[sec].mousedown = LittleUp_mousedown;

	event_key[sec = L"Highlight_leader_flag"].type = ev_mousedown;
	event_key[sec].mousedown = Highlight_leader_flag_mousedown;
}