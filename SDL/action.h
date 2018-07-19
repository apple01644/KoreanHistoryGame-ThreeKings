#pragma once
#include "act.h"

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
	pop_gove(350, 300);
}
void LeftMenu_Item1_Click(int id, int x, int y)
{
	pop_cong(350, 300);
}
void LeftMenu_Item2_Click(int id, int x, int y)
{
	pop_comp(350,300);
}
void LeftMenu_Item3_Click(int id, int x, int y)
{
	pop_law(350, 300);
}
void LeftMenu_Item4_Click(int id, int x, int y)
{
	pop_gun(350,300);
}
void LeftMenu_Item5_Click(int id, int x, int y)
{
	pop_edu(350, 300);
}
void LeftMenu_Item6_Click(int id, int x, int y)
{
	pop_media(350, 300);
}
void LeftMenu_Item7_Click(int id, int x, int y)
{
	pop_prov(350, 300);
}
void LeftMenu_Item8_Click(int id, int x, int y)
{
	pop_party(350, 300);
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
	if (gui[ikeys["@ui\\tooltip"]].enable)
	{
		if (std::stoi(gui[ikeys["@ui\\tooltip"]].var["life"]) > 0)
		{
			(gui.begin() + id)->x = x + 10;
			(gui.begin() + id)->y = y + 10;

			if (x + 10 + 280 > scr_w)
			{
				(gui.begin() + id)->x = x - 290;
			}


			parent_front(id);
			gui[ikeys["@ui\\tooltip"]].var["life"] = std::to_string(std::stoi(gui[ikeys["@ui\\tooltip"]].var["life"]) - 1);
			if (gui[ikeys["@ui\\tooltip"]].var["life"] == "0")
			{
				gui[ikeys["@ui\\tooltip"]].enable = false;
			}
		}
	}
}

void Potrait_Mousedown(int id, int x, int y)
{
	//pop_comp(200, 200);
	if (gui[id].var["id"] != "")
	{
		pop_char(350, 50, std::stoi(gui[id].var["id"]));
	}
}

void Potrait_Hover(int id, int x, int y) {
	//(gui.begin() + id)->var["time"] = std::to_string(std::stoi((gui.begin() + id)->var["time"]) + 3);

	//if (std::stoi((gui.begin() + id)->var["time"]) > 20)
	{
		gui[ikeys["@ui\\tooltip"]].enable = true;
		gui[ikeys["@ui\\tooltip"]].var["life"] = "2";
		auto I = man.begin();
		for (int a = 0; I != man.end(); a++)
		{
			if (std::to_string(I->id) == gui[id].var["id"])
			{
				gui[ikeys["@ui\\tooltip_text"]].var["text"] = "이름 : " +(*I).name;
				gui[ikeys["@ui\\tooltip_text"]].var["text"] += "\n" + std::to_string(year - I->born_year + 1) + "세";
				gui[ikeys["@ui\\tooltip_text"]].var["text"] += "\n자금 : " + std::to_string(I->money);
				gui[ikeys["@ui\\tooltip_text"]].var["text"] += "\n명예 : " + std::to_string(I->prestige);
				if ((*I).work != "None")
				{
					gui[ikeys["@ui\\tooltip_text"]].var["text"] += "\n직업 : " + (*I).work;
				}
				else
				{
					gui[ikeys["@ui\\tooltip_text"]].var["text"] += "\n직업 : 없음";
				}
				break;
			}
			I++;
		}

	}

	//parent_front(id);
}

void Character_Step(int id)
{
	auto I = man.begin();
	for (int a = 0; a < man.size(); a++)
	{
		if (std::to_string(I->id) == (gui.begin() + id)->var["id"])
		{
			break;
		}
		I++;
	}
	gui[ikeys[gui[id].var["name"] + "_owner_case"]].var["img"] = get_case(I->work);
	if (I->work == "None")
	{
		gui[ikeys[gui[id].var["name"] + "_title"]].var["text"] = I->name;
	}
	else
	{
		gui[ikeys[gui[id].var["name"] + "_title"]].var["text"] = I->work + " " + I->name;
	}

	gui[ikeys[(gui.begin() + id)->var["name"] + "_relation_right_label"]].var["text"] = std::to_string(relation[(gui.begin() + id)->var["id"] + "<" + std::to_string(play_id)]);

	if (relation[(gui.begin() + id)->var["id"] + "<" + std::to_string(play_id)] > 0)
	{
		gui[ikeys[(gui.begin() + id)->var["name"] + "_relation_right_label"]].var["color"] = "c_green";
	}
	else if (relation[(gui.begin() + id)->var["id"] + "<" + std::to_string(play_id)] < 0)
	{
		gui[ikeys[(gui.begin() + id)->var["name"] + "_relation_right_label"]].var["color"] = "c_red";
	}
	else
	{
		gui[ikeys[(gui.begin() + id)->var["name"] + "_relation_right_label"]].var["color"] = "c_yellow";
	}
		
	gui[ikeys[gui[id].var["name"] + "right[1]_label"]].var["text"] = std::to_string(year - I->born_year + 1) + "세";;
	gui[ikeys[gui[id].var["name"] + "right[2]_label"]].var["text"] = "자금 : " + std::to_string(I->money);
	gui[ikeys[gui[id].var["name"] + "right[3]_label"]].var["text"] = "명예 : " + std::to_string(I->prestige);
	gui[ikeys[gui[id].var["name"] + "right[0]_label"]].var["text"] = get_ideology(I->fascist, I->liberty);


	if ((gui.begin() + id)->var["id"] == std::to_string(play_id))
	{
		gui[ikeys[(gui.begin() + id)->var["name"] + "_relation_left"]].enable = false;
		gui[ikeys[(gui.begin() + id)->var["name"] + "_relation_right"]].enable = false;
	}
	else
	{
		gui[ikeys[(gui.begin() + id)->var["name"] + "_relation_left_label"]].var["text"] = std::to_string(relation[std::to_string(play_id) + "<" + (gui.begin() + id)->var["id"]]);


		if (relation[std::to_string(play_id) + "<" + (gui.begin() + id)->var["id"]] > 0)
		{
			gui[ikeys[(gui.begin() + id)->var["name"] + "_relation_left_label"]].var["color"] = "c_green";
		}
		else if (relation[std::to_string(play_id) + "<" + (gui.begin() + id)->var["id"]] < 0)
		{
			gui[ikeys[(gui.begin() + id)->var["name"] + "_relation_left_label"]].var["color"] = "c_red";
		}
		else
		{
			gui[ikeys[(gui.begin() + id)->var["name"] + "_relation_left_label"]].var["color"] = "c_yellow";
		}

	}
}

void Profile_step(int id)
{
	auto I = man.begin();
	for (int a = 0; a < man.size(); a++)
	{
		if (I->id == play_id)
		{
			break;
		}
		I++;
	}

	if (I->work == "None")
	{
		gui[ikeys["@ui\\potrait_case"]].var["img"] = "ui\\potrait_case";
	}
	else if (I->work == "국회의원" || I->work == "국회의장")
	{
		gui[ikeys["@ui\\potrait_case"]].var["img"] = "ui\\potrait_case_gold_cong";
	}
	else if (I->work == "대통령")
	{
		gui[ikeys["@ui\\potrait_case"]].var["img"] = "ui\\potrait_case_blue";
	}
	else if (I->work == "시장" || I->work == "도지사" || I->work == "총리")
	{
		gui[ikeys["@ui\\potrait_case"]].var["img"] = "ui\\potrait_case_gold";
	}
	else if (I->work == "기업가")
	{
		gui[ikeys["@ui\\potrait_case"]].var["img"] = "ui\\potrait_case_company";
	}
	else if (I->work == "건달")
	{
		gui[ikeys["@ui\\potrait_case"]].var["img"] = "ui\\potrait_case_gun";
	}
	else if (I->work == "판사")
	{
		gui[ikeys["@ui\\potrait_case"]].var["img"] = "ui\\potrait_case_law";
	}
	else if (I->work == "기자")
	{
		gui[ikeys["@ui\\potrait_case"]].var["img"] = "ui\\potrait_case_media";
	}
	else
	{
		gui[ikeys["@ui\\potrait_case"]].var["img"] = "ui\\potrait_case";
	}

	gui[ikeys[(gui.begin() + id)->var["name"] + "_relation_right_label"]].var["text"] = std::to_string(relation[(gui.begin() + id)->var["id"] + "<" + std::to_string(play_id)]);
	if (relation[(gui.begin() + id)->var["id"] + "<" + std::to_string(play_id)] > 0)
	{
		gui[ikeys[(gui.begin() + id)->var["name"] + "_relation_right_label"]].var["color"] = "c_green";
	}
	else if (relation[(gui.begin() + id)->var["id"] + "<" + std::to_string(play_id)] < 0)
	{
		gui[ikeys[(gui.begin() + id)->var["name"] + "_relation_right_label"]].var["color"] = "c_red";
	}
	else
	{
		gui[ikeys[(gui.begin() + id)->var["name"] + "_relation_right_label"]].var["color"] = "c_yellow";
	}


	gui[ikeys[(gui.begin() + id)->var["name"] + "_relation_left_label"]].var["text"] = std::to_string(relation[std::to_string(play_id) + "<" + (gui.begin() + id)->var["id"]]);


	if (relation[std::to_string(play_id) + "<" + (gui.begin() + id)->var["id"]] > 0)
	{
		gui[ikeys[(gui.begin() + id)->var["name"] + "_relation_left_label"]].var["color"] = "c_green";
	}
	else if (relation[std::to_string(play_id) + "<" + (gui.begin() + id)->var["id"]] < 0)
	{
		gui[ikeys[(gui.begin() + id)->var["name"] + "_relation_left_label"]].var["color"] = "c_red";
	}
	else
	{
		gui[ikeys[(gui.begin() + id)->var["name"] + "_relation_left_label"]].var["color"] = "c_yellow";
	}
}

void LetQuit(int id, int x, int y) {
	quit = true;
}

void Scroll_Step(int id)
{
	


	if (std::stoi(gui[id].var["scroll"]) > 0)
	{
		gui[id].var["scroll"] = std::to_string((std::stoi(gui[id].var["scroll"]) + 0) / 2);
	}
	else if (std::stoi(gui[id].var["scroll"]) < (std::stoi(gui[id].var["items"]) - 1) * std::stoi(gui[id].var["def_h"]))
	{
		//gui[id].var["scroll"] = std::to_string((std::stoi(gui[id].var["scroll"]) + (std::stoi(gui[id].var["items"]) - 1) * 90) / 2);
		if (std::stoi(gui[id].var["scroll"]) > 0)
		{
			//gui[id].var["scroll"] = "0";
		}
	}
	else if (std::stoi(gui[id].var["scroll"]) > std::stoi(gui[id].var["items"]) * std::stoi(gui[id].var["def_h"]))
	{
		gui[id].var["scroll"] = std::to_string(std::stoi(gui[id].var["items"]) * std::stoi(gui[id].var["def_h"]));
	}

	for (int a = 0; a <  std::stoi(gui[id].var["items"]); a++)
	{
		if (std::to_string(a) == gui[id].var["select"])
		{
			gui[ikeys[gui[id].var["name"] + "[" + std::to_string(a) + "]"]].var["img"] = "ui\\button_push";
		}
		else
		{
			gui[ikeys[gui[id].var["name"] + "[" + std::to_string(a) + "]"]].var["img"] = "ui\\button";
		}
		int y = std::stoi(gui[id].var["def_h"]) * a + std::stoi(gui[id].var["scroll"]);
		gui[ikeys[gui[id].var["name"] + "[" + std::to_string(a) + "]"]].y = y;
		gui[ikeys[gui[id].var["name"] + "[" + std::to_string(a) + "]"]].h = std::stoi(gui[id].var["def_h"]);
		if (y >= gui[id].h - std::stoi(gui[id].var["def_h"]) || y + std::stoi(gui[id].var["def_h"]) <= 0)
		{
			gui[ikeys[gui[id].var["name"] + "[" + std::to_string(a) + "]"]].enable = false;
		}
		else if (y + std::stoi(gui[id].var["def_h"]) > gui[id].h)
		{
			gui[ikeys[gui[id].var["name"] + "[" + std::to_string(a) + "]"]].enable = true;
			gui[ikeys[gui[id].var["name"] + "[" + std::to_string(a) + "]"]].h = gui[id].h - y;
		}
		else if (y < 0)
		{
			gui[ikeys[gui[id].var["name"] + "[" + std::to_string(a) + "]"]].enable = true;
			gui[ikeys[gui[id].var["name"] + "[" + std::to_string(a) + "]"]].h = std::stoi(gui[id].var["def_h"]) + y;
			gui[ikeys[gui[id].var["name"] + "[" + std::to_string(a) + "]"]].y = 0;

		}
		else
		{
			gui[ikeys[gui[id].var["name"] + "[" + std::to_string(a) + "]"]].enable = true;
		}
	}
}
void Scroll_Scroll(int id, int x, int y, int dy)
{
	gui[id].var["scroll"] = std::to_string(std::stoi(gui[id].var["scroll"]) + dy * 5);
}

void Scroll_Mousedown(int id, int x, int y)
{
	y -= gui[id].ry;
	for (int a = 0; a < std::stoi(gui[id].var["items"]); a++)
	{
		int Y = std::stoi(gui[id].var["def_h"]) * a + std::stoi(gui[id].var["scroll"]);
		gui[ikeys[gui[id].var["name"] + "[" + std::to_string(a) + "]"]].y = Y;
		gui[ikeys[gui[id].var["name"] + "[" + std::to_string(a) + "]"]].h = std::stoi(gui[id].var["def_h"]);
		if (Y >= gui[id].h - std::stoi(gui[id].var["def_h"]) || Y + std::stoi(gui[id].var["def_h"]) <= 0)
		{
			gui[ikeys[gui[id].var["name"] + "[" + std::to_string(a) + "]"]].enable = false;
		}
		else if (Y + std::stoi(gui[id].var["def_h"]) > gui[id].h)
		{
			gui[ikeys[gui[id].var["name"] + "[" + std::to_string(a) + "]"]].enable = true;
			gui[ikeys[gui[id].var["name"] + "[" + std::to_string(a) + "]"]].h = gui[id].h - Y;
		}
		else if (Y < 0)
		{
			gui[ikeys[gui[id].var["name"] + "[" + std::to_string(a) + "]"]].enable = true;
			gui[ikeys[gui[id].var["name"] + "[" + std::to_string(a) + "]"]].h = std::stoi(gui[id].var["def_h"]) + Y;
			gui[ikeys[gui[id].var["name"] + "[" + std::to_string(a) + "]"]].y = 0;
		}
		else
		{
			gui[ikeys[gui[id].var["name"] + "[" + std::to_string(a) + "]"]].enable = true;
		}
		
		if (gui[ikeys[gui[id].var["name"] + "[" + std::to_string(a) + "]"]].enable &&
			y > Y && 
			y < Y + gui[ikeys[gui[id].var["name"] + "[" + std::to_string(a) + "]"]].h)
		{

			if (gui[id].var["select"] == "-1")
			{
				gui[id].var["select"] = std::to_string(a);
			}
			else
			{
				gui[id].var["select"] = "-1";
			}



			break;
		}
	}
}


void ScrollPOP_Mousedown(int id, int x, int y)
{
	y -= gui[id].ry;
	for (int a = 0; a < std::stoi(gui[id].var["items"]); a++)
	{
		int Y = std::stoi(gui[id].var["def_h"]) * a + std::stoi(gui[id].var["scroll"]);
		gui[ikeys[gui[id].var["name"] + "[" + std::to_string(a) + "]"]].y = Y;
		gui[ikeys[gui[id].var["name"] + "[" + std::to_string(a) + "]"]].h = std::stoi(gui[id].var["def_h"]);
		if (Y >= gui[id].h - std::stoi(gui[id].var["def_h"]) || Y + std::stoi(gui[id].var["def_h"]) <= 0)
		{
			gui[ikeys[gui[id].var["name"] + "[" + std::to_string(a) + "]"]].enable = false;
		}
		else if (Y + std::stoi(gui[id].var["def_h"]) > gui[id].h)
		{
			gui[ikeys[gui[id].var["name"] + "[" + std::to_string(a) + "]"]].enable = true;
			gui[ikeys[gui[id].var["name"] + "[" + std::to_string(a) + "]"]].h = gui[id].h - Y;
		}
		else if (Y < 0)
		{
			gui[ikeys[gui[id].var["name"] + "[" + std::to_string(a) + "]"]].enable = true;
			gui[ikeys[gui[id].var["name"] + "[" + std::to_string(a) + "]"]].h = std::stoi(gui[id].var["def_h"]) + Y;
			gui[ikeys[gui[id].var["name"] + "[" + std::to_string(a) + "]"]].y = 0;
		}
		else
		{
			gui[ikeys[gui[id].var["name"] + "[" + std::to_string(a) + "]"]].enable = true;
		}

		if (gui[ikeys[gui[id].var["name"] + "[" + std::to_string(a) + "]"]].enable &&
			y > Y &&
			y < Y + gui[ikeys[gui[id].var["name"] + "[" + std::to_string(a) + "]"]].h)
		{

			if (gui[id].var["select"] == "-1")
			{
				gui[id].var["select"] = std::to_string(a);



				pop_speech(350, 300, std::stoi(gui[ikeys[gui[mother(id)].var["name"] + "_content"]].var["select"]), a);
			}
			else
			{
				gui[id].var["select"] = "-1";
			}



			break;
		}
	}
}


void Scrollpopideology_Step(int id)
{
	auto I = man.begin();
	for (; I != man.end(); I++)
	{
		if (I->id == play_id)
		{
			break;
		}
	}
	gui[id].var["text"] = get_ideology(prv[std::stoi(gui[id].var["a"])].pop[std::stoi(gui[id].var["i"])].fascist, prv[std::stoi(gui[id].var["a"])].pop[std::stoi(gui[id].var["i"])].liberty);
	gui[ikeys[gui[id].var["name"] + "_power"]].var["text"] = std::to_string((int)sqrt(pow(prv[std::stoi(gui[id].var["a"])].pop[std::stoi(gui[id].var["i"])].fascist - I->fascist, 2) + pow(prv[std::stoi(gui[id].var["a"])].pop[std::stoi(gui[id].var["i"])].liberty - I->liberty, 2)));
}

void Scrollpop_Mousedown(int id, int x, int y)
{
	y -= gui[id].ry;
	
	for (int a = 0; a < std::stoi(gui[id].var["items"]); a++)
	{
		int Y = std::stoi(gui[id].var["def_h"]) * a + std::stoi(gui[id].var["scroll"]);
		{
			{
				gui[ikeys[gui[id].var["name"] + "[" + std::to_string(a) + "]"]].y = Y;
				gui[ikeys[gui[id].var["name"] + "[" + std::to_string(a) + "]"]].h = std::stoi(gui[id].var["def_h"]);
				if (Y >= gui[id].h - std::stoi(gui[id].var["def_h"]) || Y + std::stoi(gui[id].var["def_h"]) <= 0)
				{
					gui[ikeys[gui[id].var["name"] + "[" + std::to_string(a) + "]"]].enable = false;
				}
				else if (Y + std::stoi(gui[id].var["def_h"]) > gui[id].h)
				{
					gui[ikeys[gui[id].var["name"] + "[" + std::to_string(a) + "]"]].enable = true;
					gui[ikeys[gui[id].var["name"] + "[" + std::to_string(a) + "]"]].h = gui[id].h - Y;
				}
				else if (Y < 0)
				{
					gui[ikeys[gui[id].var["name"] + "[" + std::to_string(a) + "]"]].enable = true;
					gui[ikeys[gui[id].var["name"] + "[" + std::to_string(a) + "]"]].h = std::stoi(gui[id].var["def_h"]) + Y;
					gui[ikeys[gui[id].var["name"] + "[" + std::to_string(a) + "]"]].y = 0;
				}
				else
				{
					gui[ikeys[gui[id].var["name"] + "[" + std::to_string(a) + "]"]].enable = true;
				}
			}

			if (gui[ikeys[gui[id].var["name"] + "[" + std::to_string(a) + "]"]].enable &&
				y > Y &&
				y < Y + gui[ikeys[gui[id].var["name"] + "[" + std::to_string(a) + "]"]].h)
			{

				if (gui[id].var["select"] == "-1")
				{
					gui[id].var["select"] = std::to_string(a);

					{
						Widget temp(160, 270, 720, 410, wd_image, gui[mother(id)].var["name"] + "_pop");
						temp.var["img"] = "ui\\body";
						temp.var["items"] = std::to_string(prv[std::stoi(gui[ikeys[gui[id].var["name"] + "[" + std::to_string(a) + "]"]].var["id"])].pop.size());
						temp.var["scroll"] = "0";
						temp.var["def_h"] = "40";
						temp.var["select"] = "-1";

						temp.step_ev = Scroll_Step;
						temp.avail_step_ev = true;
						temp.mousewheel_ev = Scroll_Scroll;
						temp.avail_mousewheel_ev = true;
						temp.mousedown_ev = ScrollPOP_Mousedown;
						temp.avail_mousedown_ev = true;

						temp.parent = ikeys[gui[mother(id)].var["name"]];
						gui.push_back(temp);
					}
					{
						Widget temp(0, -210, 190, 190, wd_image, gui[mother(id)].var["name"] + "_pop_owner");
						temp.var["img"] = "ui\\body";
						temp.parent = ikeys[gui[mother(id)].var["name"] + "_pop"];
						gui.push_back(temp);
					}
					{
						Widget temp(10, 5, 180, 180, wd_image, gui[mother(id)].var["name"] + "_pop_owner1_face");
						temp.var["img"] = "potrait\\who";

						{

							for (int i = 0; i < std::stoi(keys["prov_id_miner"]); i++)
							{
								if (std::stoi((keys["prov_id_miner[" + std::to_string(i) + "]"])) - 1 ==
									std::stoi(gui[ikeys[gui[id].var["name"] + "[" + std::to_string(a) + "]"]].var["id"]))
								{
									auto I = man.begin();
									for (; I != man.end();)
									{
										if (std::to_string(I->id) == keys["miner[" + std::to_string(i) + "]"])
										{
											temp.var["img"] = I->potrait;
											temp.var["id"] = std::to_string(I->id);
											break;
										}
										I++;
									}
									break;
								}
							}
						}
						{
							for (int i = 0; i < std::stoi(keys["prov_id_major"]); i++)
							{
								if (std::stoi(keys["prov_id_major[" + std::to_string(i) + "]"]) - 1 ==
									std::stoi(gui[ikeys[gui[id].var["name"] + "[" + std::to_string(a) + "]"]].var["id"]))
								{
									auto I = man.begin();
									for (; I != man.end();)
									{
										if (std::to_string(I->id) == keys["major[" + std::to_string(i) + "]"])
										{
											temp.var["img"] = I->potrait;
											temp.var["id"] = std::to_string(I->id);
											break;
										}
										I++;
									}
									break;
								}
							}}

						temp.mousehover_ev = Potrait_Hover;
						temp.avail_mousehover_ev = true;
						temp.mousedown_ev = Potrait_Mousedown;
						temp.avail_mousedown_ev = true;

						temp.parent = ikeys[gui[mother(id)].var["name"] + "_pop_owner"];
						gui.push_back(temp);
					}
					{
						Widget temp(0, 0, 180, 180, wd_image, gui[mother(id)].var["name"] + "_pop_owner1_face_case");
						temp.parent = ikeys[gui[mother(id)].var["name"] + "_pop_owner1_face"];
						temp.var["img"] = "ui\\potrait_case_gold";
						gui.push_back(temp);
					}
					{

						for (int i = 0; i < prv[std::stoi(gui[ikeys[gui[id].var["name"] + "[" + std::to_string(a) + "]"]].var["id"])].pop.size(); i++)
						{
							Widget temp(0, std::stoi(gui[id].var["def_h"]) * i, 720, std::stoi(gui[id].var["def_h"]), wd_image, gui[mother(id)].var["name"] + "_pop[" + std::to_string(i) + "]");

							temp.var["img"] = "ui\\button";
							temp.parent = ikeys[gui[mother(id)].var["name"] + "_pop"];
							gui.push_back(temp);
							//if (false)
							{
								Widget temp2(210, 20, 0, 0, wd_label, gui[mother(id)].var["name"] + "_pop[" + std::to_string(i) + "]_name");

								temp2.parent = ikeys[gui[mother(id)].var["name"] + "_pop[" + std::to_string(i) + "]"];
								temp2.var["a"] = gui[ikeys[gui[id].var["name"] + "[" + std::to_string(a) + "]"]].var["id"];
								temp2.var["i"] = std::to_string(i);
								
								//temp2.var["text"] = get_ideology(prv[std::stoi(temp2.var["a"])].pop[std::stoi(temp2.var["i"])].fascist, prv[std::stoi(temp2.var["a"])].pop[std::stoi(temp2.var["i"])].liberty);

								temp2.step_ev = Scrollpopideology_Step;
								temp2.avail_step_ev = true;

								temp2.var["opt"] = std::to_string(middle_align + center_align);
								temp2.var["ind"] = "2";
								temp2.var["color"] = "c_white";
								temp2.var["size"] = "24";
								temp2.var["ratio"] = "0.77";
								gui.push_back(temp2);
							}
							{
								Widget temp3(410, 0, 0, 0, wd_label, gui[mother(id)].var["name"] + "_pop[" + std::to_string(i) + "]_name_power");

								temp3.parent = ikeys[gui[mother(id)].var["name"] + "_pop[" + std::to_string(i) + "]_name"];
								temp3.var["opt"] = std::to_string(middle_align + center_align);
								temp3.var["ind"] = "2";
								temp3.var["color"] = "c_white";
								temp3.var["size"] = "24";
								temp3.var["ratio"] = "0.65";
								gui.push_back(temp3);
							}
						}
					}
				}
				else
				{
					gui[id].var["select"] = "-1";
					if (ikeys[gui[mother(id)].var["name"] + "_pop"] != 0)
					{
						gui[ikeys[gui[mother(id)].var["name"] + "_pop"]].removing = true;
					}
				}



				break;
			}
		}
	}
}
void Companyitems_Step(int id)
{
	//_content[" + std::to_string(a) + "]_power
	auto I = company.begin();
	if (stats["all_comp_power"] != 0)
	{
		for (int a = 0; a < std::stoi(gui[ikeys[gui[id].var["name"] + "_content"]].var["items"]); a++)
		{
			gui[ikeys[gui[id].var["name"] + "_content[" + std::to_string(a) + "]_power"]].w = (174 * I->power) / stats["all_comp_power"];
			I++;
		}
	}
}
void Mediaitems_Step(int id)
{
	auto I = media.begin();
	if (stats["all_media_power"] != 0)
	{
		for (int a = 0; a < std::stoi(gui[ikeys[gui[id].var["name"] + "_content"]].var["items"]); a++)
		{
			gui[ikeys[gui[id].var["name"] + "_content[" + std::to_string(a) + "]_power"]].w = (174 * I->power) / stats["all_media_power"];
			I++;
		}
	}
}
void Forceitems_Step(int id)
{
	auto I = force.begin();
	if (stats["all_force_power"] != 0)
	{
		for (int a = 0; a < std::stoi(gui[ikeys[gui[id].var["name"] + "_content"]].var["items"]); a++)
		{
			gui[ikeys[gui[id].var["name"] + "_content[" + std::to_string(a) + "]_power"]].w = (174 * I->power) / stats["all_force_power"];
			I++;
		}
	}
}
void Educationitems_Step(int id)
{
	auto I = education.begin();
	if (stats["all_education_power"] != 0)
	{
		for (int a = 0; a < std::stoi(gui[ikeys[gui[id].var["name"] + "_content"]].var["items"]); a++)
		{
			gui[ikeys[gui[id].var["name"] + "_content[" + std::to_string(a) + "]_power"]].w = (174 * I->power) / stats["all_education_power"];
			I++;
		}
	}
}

void Partylist_Step(int id)
{
	for (int i = 0; i < 45; i++)
	{
		gui[ikeys[gui[id].var["name"] + "_list[" + std::to_string(i) + "]"]].enable = false;
	}

	gui[ikeys[gui[id].var["name"] + "_party"]].var["text"] = party[std::stoi(gui[ikeys[gui[id].var["name"] + "_toolbar"]].var["selected"])].name;
	gui[ikeys[gui[id].var["name"] + "_ideology"]].var["text"] = get_ideology(party[std::stoi(gui[ikeys[gui[id].var["name"] + "_toolbar"]].var["selected"])].facism, party[std::stoi(gui[ikeys[gui[id].var["name"] + "_toolbar"]].var["selected"])].liberty);

	
	auto I = man.begin();
	for (int i = 0; I != man.end() && i < 45;)
	{
		if (I->live && I->party == std::stoi(gui[ikeys[gui[id].var["name"] + "_toolbar"]].var["selected"]))
		{
			gui[ikeys[gui[id].var["name"] + "_list[" + std::to_string(i) + "]"]].enable = true;
			gui[ikeys[gui[id].var["name"] + "_list[" + std::to_string(i) + "]"]].var["img"] = I->potrait;
			gui[ikeys[gui[id].var["name"] + "_list[" + std::to_string(i) + "]"]].var["id"] = std::to_string(I->id);
			gui[ikeys[gui[id].var["name"] + "_list[" + std::to_string(i) + "]_case"]].var["img"] = gui[ikeys[(gui.begin() + id)->var["name"] + "_owner_case"]].var["img"] = get_case(I->work);

			i++;
		}
		I++;
	}
}

void Partytoolbar_Mousedown(int id, int x, int y)
{
	int new_x = x - gui[id].rx;
	for (int a = 0; a < 5; a++)
	{
		if (a * 80 < new_x && new_x < a * 80 + 80)
		{
			gui[id].var["selected"] = std::to_string(a);
			break;
		}
	}
}


void Char_Real_up(int id, int x, int y)
{
	auto I = man.begin();
	for (; I != man.end();)
	{
		if (I->id == play_id)
		{
			if (I->charge >= 40)
			{
				I->charge -= 40;
				good_word(I->id,std::stoi(gui[ikeys[gui[mother(id)].var["name"] + "_owner"]].var["id"]));
			}
			else
			{
				gui[ikeys["@ui\\tooltip"]].enable = true;
				gui[ikeys["@ui\\tooltip"]].var["life"] = "20";
				gui[ikeys["@ui\\tooltip_text"]].var["text"] = "행동력 40이 필요합니다.";
			}
			break;
		}
		I++;
	}
}
void Char_Real_down(int id, int x, int y)
{
	auto I = man.begin();
	for (; I != man.end();)
	{
		if (I->id == play_id)
		{
			if (I->charge >= 40)
			{
				bad_word(I->id, std::stoi(gui[ikeys[gui[mother(id)].var["name"] + "_owner"]].var["id"]));
				I->charge -= 40;
			}
			else
			{
				gui[ikeys["@ui\\tooltip"]].enable = true;
				gui[ikeys["@ui\\tooltip"]].var["life"] = "40";
				gui[ikeys["@ui\\tooltip_text"]].var["text"] = "행동력 40이 필요합니다.";
			}
			break;
		}
		I++;
	}
}

void Heard_Step(int id)
{
	if (msg.begin() != msg.end())
	{
		gui[id].var["text"] = msg.begin()->s;
		gui[ikeys["@ui\\heard_potrait"]].enable = true;
		auto I = man.begin();
		for (int a = 0; I != man.end(); a++)
		{
			if (I->id == msg.begin()->sender)
			{
				gui[ikeys["@ui\\heard_potrait"]].var["id"] = std::to_string(I->id);
				gui[ikeys["@ui\\heard_potrait"]].var["img"] = I->potrait;
				break;
			}
			I++;
		}
	}
	else
	{
		gui[ikeys["@ui\\heard_potrait"]].enable = false;
	}
}
void Heardclose_mousedown(int id, int x, int y)
{
	if (msg.begin() != msg.end())
	{
		msg.pop_front();
	} 
	else
	{
		gui[ikeys["@ui\\heard_label"]].var["text"] = "";
	}
}
void Heardopen_mousedown(int id, int x, int y)
{
	if (msg.begin() != msg.end())
	{
		pop_char(350, 50, msg.begin()->sender);

		if (empty_gui_slot("@ui\\char") == "@ui\\char-0")
		{
			msg.pop_front();
		}
	}
}



void Mapmode0_Mousedown(int id, int x, int y)
{
	map_mode = 0;
}
void Mapmode1_Mousedown(int id, int x, int y)
{
	map_mode = 1;
}
void Mapmode2_Mousedown(int id, int x, int y)
{
	map_mode = 2;
}
void Mapmode3_Mousedown(int id, int x, int y)
{
	map_mode = 3;
}
void Mapmode4_Mousedown(int id, int x, int y)
{
	map_mode = 4;
}

void  ProfileOwner_Step(int id)
{
	gui[id].var["id"] = std::to_string(play_id);
}

void Company0_Mousedown(int id, int x, int y)
{
	int j = 0;
	for (;j < company.size();)
	{
		if (std::to_string(j) == gui[ikeys[gui[mother(id)].var["name"] + "_content"]].var["select"])
		{
			bribe(play_id, j);
			break;
		}
		j++;
	}
}

void Company1_Mousedown(int id, int x, int y)
{
	int j = 0;
	for (; j < company.size();)
	{
		if (std::to_string(j) == gui[ikeys[gui[mother(id)].var["name"] + "_content"]].var["select"])
		{
			loby(play_id, j);
			break;
		}
		j++;
	}
}

void Force0_Mousedown(int id, int x, int y){
	auto J = force.begin();
	int j = 0;
	for (; j < force.size();)
	{
		if (std::to_string(j) == gui[ikeys[gui[mother(id)].var["name"] + "_content"]].var["select"])
		{
			lobi(play_id, J->owner);
			break;
		}
		j++;
		J++;
	}
}
void Force1_Mousedown(int id, int x, int y) {
	int j = 0;
	for (; j < force.size();)
	{
		if (std::to_string(j) == gui[ikeys[gui[mother(id)].var["name"] + "_content"]].var["select"])
		{
			mafia(play_id, j);
			break;
		}
		j++;
	}
}

void Media0_Mousedown(int id, int x, int y) {
	auto J = media.begin();

	int j = 0;
	for (; j < media.size();)
	{
		if (std::to_string(j) == gui[ikeys[gui[mother(id)].var["name"] + "_content"]].var["select"])
		{
			lobi(play_id, J->owner);
			break;
		}
		j++;
		J++;
	}
}
void Media1_Mousedown(int id, int x, int y) {
	int j = 0;
	for (; j < media.size();)
	{
		if (std::to_string(j) == gui[ikeys[gui[mother(id)].var["name"] + "_content"]].var["select"])
		{
			impress(play_id, j);
			break;
		}
		j++;
	}
}
void Media2_Mousedown(int id, int x, int y) {
	int j = 0;
	for (; j < media.size();)
	{
		if (std::to_string(j) == gui[ikeys[gui[mother(id)].var["name"] + "_content"]].var["select"])
		{
			jirassi(play_id, j);
			break;
		}
		j++;
	}
}

void Education0_Mousedown(int id, int x, int y) {
	int j = 0;
	auto J = education.begin();
	for (; j < education.size();)
	{
		if (std::to_string(j) == gui[ikeys[gui[mother(id)].var["name"] + "_content"]].var["select"])
		{
			lobi(play_id, J->owner);
			break;
		}
		J++;
		j++;
	}
}
void Education1_Mousedown(int id, int x, int y) {
	int j = 0;
	for (; j < education.size();)
	{
		if (std::to_string(j) == gui[ikeys[gui[mother(id)].var["name"] + "_content"]].var["select"])
		{
			booking(play_id, j);
			break;
		}
		j++;
	}
}

void Speech_Mousedown(int id, int x, int y)
{

	auto I = man.begin();
	for (; I != man.end(); I++)
	{
		if (I->live && I->id == play_id)
		{
			if (I->charge >= 60 && I->money > 50)
			{
				speech(play_id, std::stoi(gui[id].var["prv"]), std::stoi(gui[id].var["pop"]));
				gui[mother(id)].removing = true;
				return;
			}
			else
			{
				gui[ikeys["@ui\\tooltip"]].enable = true;
				gui[ikeys["@ui\\tooltip"]].var["life"] = "40";
				gui[ikeys["@ui\\tooltip_text"]].var["text"] = "행동력 60이 필요합니다.\n자금 50이 필요합니다.";
			}
		}
	}
	gui[id].var["text"] = "Hi";
	
}