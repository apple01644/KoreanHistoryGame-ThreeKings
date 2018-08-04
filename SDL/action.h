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
	int sec = map_reg[Str((unsigned int)((e.x + (scr_w / 2.0 + map_x) * map_p - scr_w / 2.0) / map_p)) + ":" + Str((unsigned int)((e.y + (scr_h / 2.0 + map_y) * map_p - scr_h / 2.0) / map_p))];
	if (prv[sec].enable)
	{
		script["SEC"] = Str(sec);
		if (e.button == SDL_BUTTON_LEFT)
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
		}
		if (e.button == SDL_BUTTON_MIDDLE)
		{
			script["CON"] = prv.at(sec).var["OWN"];
			gui.at(gui_key["nation_flag"]).var["source"] = "flag\\" + script["CON"];
		}
		if (e.button == SDL_BUTTON_RIGHT)
		{
			script["CON"] = "REB";
			gui.at(gui_key["nation_flag"]).var["source"] = "flag\\REB";
		}
	}
}

void LittleUp_mousedown(size_t owner, Arg_Mouse e)
{
	gui.at(owner).var["source"] = "";
}