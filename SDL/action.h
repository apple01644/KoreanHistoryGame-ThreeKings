#pragma once
#include "main.h"

void parent_front(int id)
{
	for (int mom = mother(id); mom != gui.size() - 1; mom++)
	{
		for (int a = 0; a < gui.size(); a++)
		{
			if ((gui.begin() + a)->parent == mom)
			{
				(gui.begin() + a)->parent = mom + 1;
			}
			else if ((gui.begin() + a)->parent == mom + 1)
			{
				(gui.begin() + a)->parent = mom;
			}
		}
		(gui.begin() + mom)->id = mom + 1;
		ikeys[(gui.begin() + mom)->var["name"]] = mom + 1;
		(gui.begin() + mom + 1)->id = mom;
		ikeys[(gui.begin() + mom + 1)->var["name"]] = mom;
		std::iter_swap(gui.begin() + mom, gui.begin() + mom + 1);
	}
}
void remove_game(int id, int x, int y)
{
	(gui.begin() + id)->x += 30;
}
void parent_remove(int id, int x, int y)
{
	(gui.begin() + (gui.begin() + id)->parent)->removing = true;
}

void LeftMenu_Item0_Click(int id, int x, int y)
{
	pop_gove(200, 200);
}
void LeftMenu_Item1_Click(int id, int x, int y)
{
	pop_comp(200,200);
}
void LeftMenu_Item2_Click(int id, int x, int y)
{
	pop_law(200, 200);
}
void LeftMenu_Item3_Click(int id, int x, int y)
{
	pop_gun(200,200);
}
void LeftMenu_Item4_Click(int id, int x, int y)
{
	
}
void LeftMenu_Item5_Click(int id, int x, int y)
{
	pop_media(200, 200);
}
void Drag_start(int id, int x, int y)
{
	(gui.begin() + id)->var["hold"] = "1";
	(gui.begin() + id)->var["lx"] = std::to_string(x);
	(gui.begin() + id)->var["ly"] = std::to_string(y);

	parent_front(id);

}
void Drag_step(int id, int x, int y)
{
	if ((gui.begin() + id)->var["hold"] == "1")
	{
		(gui.begin() + id)->x += (x - std::stoi((gui.begin() + id)->var["lx"]));
		(gui.begin() + id)->y += (y - std::stoi((gui.begin() + id)->var["ly"]));
		(gui.begin() + id)->var["lx"] = std::to_string(x);
		(gui.begin() + id)->var["ly"] = std::to_string(y);
	}
	
}
void Drag_end(int id, int x, int y)
{
	(gui.begin() + id)->var["hold"] = "0";
}

void Paper_Click(int id, int x, int y)
{
	parent_front(id);
	if ((gui.begin() + id)->var["going"] == "up")
	{
		(gui.begin() + id)->var["going"] = "down";
		return;
	}
	if ((gui.begin() + id)->var["going"] == "down")
	{
		(gui.begin() + id)->var["going"] = "up";
		return;
	}
}
void Paper_Step(int id, int x, int y)
{
	(gui.begin() + id)->x = scr_w / 2 - 400;

	if ((gui.begin() + id)->var["going"] == "down")
	{
		if ((gui.begin() + id)->y < (scr_h - 20.0))
		{
			(gui.begin() + id)->y = (scr_h - 20.0);
		}
		return;
	}
	if ((gui.begin() + id)->var["going"] == "up")
	{
		if ((gui.begin() + id)->y > (scr_h / 2.0 - 400.0))
		{
			(gui.begin() + id)->y = (scr_h / 2.0 - 400.0);
		}
		return;
	}
}

void Tooltip_Step(int id, int x, int y) {
	(gui.begin() + id)->x = x + 10;
	(gui.begin() + id)->y = y + 10;
	parent_front(id);
}

void Potrait_Hover(int id, int x, int y) {
	//(gui.begin() + id)->var["time"] = std::to_string(std::stoi((gui.begin() + id)->var["time"]) + 3);

	//if (std::stoi((gui.begin() + id)->var["time"]) > 20)
	{
		gui[ikeys["@ui\\tooltip"]].enable = true;
		auto I = man.begin();
		for (int a = 0; I != man.end(); a++)
		{
			if (std::to_string(I->id) == gui[id].var["id"])
			{
				gui[ikeys["@ui\\tooltip_text"]].var["text"] = I->name;
				break;
			}
			I++;
		}

	}

	parent_front(id);
}