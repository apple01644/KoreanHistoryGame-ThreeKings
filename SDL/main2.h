#pragma once
#include"action.h"

std::string empty_gui_slot(std::string s)
{
	for (int i = 0; i <= 2147483647; i++)
	{
		if (ikeys.find(s + "-" + std::to_string(i)) == ikeys.end())
		{
			return s + "-" + std::to_string(i);
		}
	}
	LOG_W("OVER THE MAX");
	quit = true;
	return "ERROR";
}
void gui_remove(int id)
{
	for (int a = 0; a < gui.size(); a++)
	{
		if (a != id && (gui.begin() + a)->parent == id)
		{
			(gui.begin() + a)->removing = true;
		}
	}
	(gui.begin() + id)->remove();
	for (int a = id; a < gui.size(); a++)
	{
		for (int b = 0; b < gui.size(); b++)
		{
			if ((gui.begin() + b)->parent == (gui.begin() + a)->id)
			{
				(gui.begin() + b)->parent -= 1;
			}
		}
		(gui.begin() + a)->id -= 1;
		ikeys[(gui.begin() + a)->var["name"]] -= 1;
	}
}

void pop_char(int x, int y)
{
	std::string id = empty_gui_slot("@ui\\potrait");
	{
		Widget temp(x, y, 140, 140, wd_image, id);
		temp.var["img"] = "potrait\\bgh";
		gui.push_back(temp);
	}
	{
		Widget temp(0, 0, 40, 40, wd_image, id + "_cancel");
		temp.var["img"] = "potrait\\kdj";
		temp.parent = ikeys[id];
		temp.avail_mousedown_ev = true;
		temp.mousedown_ev = parent_remove;
		gui.push_back(temp);
	}
}
void pop_comp(int x, int y)
{
	std::string id = empty_gui_slot("@ui\\company");
	{
		Widget temp(x, y, 500, 700, wd_image, id);
		temp.var["img"] = "ui\\body";
		gui.push_back(temp);
	}
	{
		Widget temp(450, 10, 40, 40, wd_image, id + "_cancel");
		temp.var["img"] = "ui\\left_menu_button";
		temp.parent = ikeys[id];
		temp.avail_mousedown_ev = true;
		temp.mousedown_ev = parent_remove;
		gui.push_back(temp);
	}
	{
		Widget temp(458, 18, 24, 24, wd_image, id + "_cancel_button");
		temp.var["img"] = "ui\\heard_menu_close";
		temp.parent = ikeys[id];
		temp.avail_mousedown_ev = true;
		temp.mousedown_ev = parent_remove;
		gui.push_back(temp);
	}
	{
		Widget temp(250, 25, 24, 24, wd_label, id + "_title");

		temp.parent = ikeys[id];
		temp.var["text"] = "기업";
		temp.var["opt"] = std::to_string(middle_align + center_align);
		temp.var["ind"] = "2";
		temp.var["color"] = "c_white";
		temp.var["size"] = "40";
		temp.var["ratio"] = "0.8";
		gui.push_back(temp);
	}
	{
		Widget temp(20, 120, 294, 560, wd_image, id + "_content");
		temp.var["img"] = "ui\\body";
		temp.parent = ikeys[id];
		gui.push_back(temp);
	}
	{
		Widget temp(334, 120, 147, 560, wd_image, id + "_buttons");
		temp.var["img"] = "ui\\body";
		temp.parent = ikeys[id];
		gui.push_back(temp);
	}
	for (int i = 0; i < 3; i++)
	{
		Widget temp(0, 450 - i * 100, 147, 80, wd_image, id + "_button[" + std::to_string(i) + "]");
		temp.var["img"] = "ui\\button";
		temp.parent = ikeys[id + "_buttons"];
		gui.push_back(temp);
	}
}

void pop_comp_info(int x, int y)
{
	std::string id = empty_gui_slot("@ui\\company_info");
	{
		Widget temp(x, y, 400, 550, wd_image, id);
		temp.var["img"] = "ui\\body";
		gui.push_back(temp);
	}
	{
		Widget temp(350, 10, 40, 40, wd_image, id + "_cancel");
		temp.var["img"] = "ui\\left_menu_button";
		temp.parent = ikeys[id];
		temp.avail_mousedown_ev = true;
		temp.mousedown_ev = parent_remove;
		gui.push_back(temp);
	}
	{
		Widget temp(358, 18, 24, 24, wd_image, id + "_cancel_button");
		temp.var["img"] = "ui\\heard_menu_close";
		temp.parent = ikeys[id];
		temp.avail_mousedown_ev = true;
		temp.mousedown_ev = parent_remove;
		gui.push_back(temp);
	}
	{
		Widget temp(200, 25, 24, 24, wd_label, id + "_title");

		temp.parent = ikeys[id];
		temp.var["text"] = "샘숭";
		temp.var["opt"] = std::to_string(middle_align + center_align);
		temp.var["ind"] = "2";
		temp.var["color"] = "c_white";
		temp.var["size"] = "40";
		temp.var["ratio"] = "0.8";
		gui.push_back(temp);
	}
	{
		Widget temp(20, 100, 360, 100, wd_image, id + "_owner");
		temp.var["img"] = "ui\\body";
		temp.parent = ikeys[id];
		gui.push_back(temp);
	}
	{
		Widget temp(5, 5, 90, 90, wd_image, id + "_owner_face");
		temp.var["img"] = "potrait\\yjy";
		temp.parent = ikeys[id + "_owner"];
		gui.push_back(temp);
	}
	{
		Widget temp(230, 50, 0, 0, wd_label, id + "_owner_label");
		temp.var["text"] = "이건이건희 아나타노 엘지와";
		temp.var["opt"] = std::to_string(middle_align + center_align);
		temp.var["ind"] = "0";
		temp.var["color"] = "c_white";
		temp.var["size"] = "20";
		temp.var["ratio"] = "0.7";
		temp.parent = ikeys[id + "_owner"];
		gui.push_back(temp);
	}
}
void pop_gove(int x, int y)
{
	std::string id = empty_gui_slot("@ui\\company_info");
	{
		Widget temp(x, y, 400, 520, wd_image, id);
		temp.var["img"] = "ui\\body";
		gui.push_back(temp);
	}
	{
		Widget temp(350, 10, 40, 40, wd_image, id + "_cancel");
		temp.var["img"] = "ui\\left_menu_button";
		temp.parent = ikeys[id];
		temp.avail_mousedown_ev = true;
		temp.mousedown_ev = parent_remove;
		gui.push_back(temp);
	}
	{
		Widget temp(358, 18, 24, 24, wd_image, id + "_cancel_button");
		temp.var["img"] = "ui\\heard_menu_close";
		temp.parent = ikeys[id];
		temp.avail_mousedown_ev = true;
		temp.mousedown_ev = parent_remove;
		gui.push_back(temp);
	}
	{
		Widget temp(200, 25, 24, 24, wd_label, id + "_title");

		temp.parent = ikeys[id];
		temp.var["text"] = "행정부";
		temp.var["opt"] = std::to_string(middle_align + center_align);
		temp.var["ind"] = "2";
		temp.var["color"] = "c_white";
		temp.var["size"] = "40";
		temp.var["ratio"] = "0.8";
		gui.push_back(temp);
	}
	{
		Widget temp(20, 40, 360, 190, wd_image, id + "_owner");
		temp.var["img"] = "ui\\body";
		temp.parent = ikeys[id];
		gui.push_back(temp);
	}
	{
		Widget temp(10, 5, 180, 180, wd_image, id + "_owner1_face");
		temp.var["img"] = "potrait\\yjy";
		temp.parent = ikeys[id + "_owner"];
		gui.push_back(temp);
	}
	{
		Widget temp(190, 25, 160, 160, wd_image, id + "_owner2_face");
		temp.var["img"] = "potrait\\bgh";
		temp.parent = ikeys[id + "_owner"];
		gui.push_back(temp);
	}
	{
		Widget temp(20, 250, 360, 110, wd_image, id + "_major");
		temp.var["img"] = "ui\\body";
		temp.parent = ikeys[id];
		gui.push_back(temp);
	}
	int mans = 8;
	int side;
	if (3 < mans)
	{
		side = 250 / (mans - 1);
	}
	else
	{
		side = 90;
	}
	for (int i = 0; i < mans; i++)
	{
		Widget temp(10 + side * i, 10, 90, 90, wd_image, id + "_major["+std::to_string(i)+"]" );
		temp.var["img"] = "potrait\\cih";
		temp.parent = ikeys[id + "_major"];
		gui.push_back(temp);
	}

	{
		Widget temp(20, 370, 360, 110, wd_image, id + "_miner");
		temp.var["img"] = "ui\\body";
		temp.parent = ikeys[id];
		gui.push_back(temp);
	}
	mans = 3;
	if (3 < mans)
	{
		side = 250 / (mans - 1);
	}
	else
	{
		side = 90;
	}
	for (int i = 0; i < mans; i++)
	{
		Widget temp(10 + side * i, 10, 90, 90, wd_image, id + "_miner[" + std::to_string(i) + "]");
		temp.var["img"] = "potrait\\acs";
		temp.parent = ikeys[id + "_miner"];
		gui.push_back(temp);
	}
}

void pop_gun(int x, int y)
{
	std::string id = empty_gui_slot("@ui\\gun");
	{
		Widget temp(x, y, 400, 520, wd_image, id);
		temp.var["img"] = "ui\\body";
		gui.push_back(temp);
	}
	{
		Widget temp(20, 60, 360, 440, wd_image, id + "_body");
		temp.var["img"] = "ui\\body";
		temp.parent = ikeys[id];
		gui.push_back(temp);
	}
	{
		Widget temp(0, 0, 360, 120, wd_image, id + "_body[0]");
		temp.var["img"] = "ui\\body";
		temp.parent = ikeys[id + "_body"];
		gui.push_back(temp);
	}
	{
		Widget temp(350, 10, 40, 40, wd_image, id + "_cancel");
		temp.var["img"] = "ui\\left_menu_button";
		temp.parent = ikeys[id];
		temp.avail_mousedown_ev = true;
		temp.mousedown_ev = parent_remove;
		gui.push_back(temp);
	}
	{
		Widget temp(358, 18, 24, 24, wd_image, id + "_cancel_button");
		temp.var["img"] = "ui\\heard_menu_close";
		temp.parent = ikeys[id];
		temp.avail_mousedown_ev = true;
		temp.mousedown_ev = parent_remove;
		gui.push_back(temp);
	}
	{
		Widget temp(200, 25, 24, 24, wd_label, id + "_title");

		temp.parent = ikeys[id];
		temp.var["text"] = "무력";
		temp.var["opt"] = std::to_string(middle_align + center_align);
		temp.var["ind"] = "2";
		temp.var["color"] = "c_white";
		temp.var["size"] = "40";
		temp.var["ratio"] = "0.8";
		gui.push_back(temp);
	}
}

void pop_media(int x, int y)
{
	std::string id = empty_gui_slot("@ui\\media");
	{
		Widget temp(x, y, 400, 520, wd_image, id);
		temp.var["img"] = "ui\\body";
		gui.push_back(temp);
	}
	{
		Widget temp(20, 130, 360, 370, wd_image, id + "_body");
		temp.var["img"] = "ui\\body";
		temp.parent = ikeys[id];
		gui.push_back(temp);
	}
	{
		Widget temp(0, 0, 200, 370, wd_image, id + "_body_left");
		temp.var["img"] = "ui\\body";
		temp.parent = ikeys[id + "_body"];
		gui.push_back(temp);
	}
	{
		Widget temp(220, 0, 140, 370, wd_image, id + "_body_right");
		temp.var["img"] = "ui\\body";
		temp.parent = ikeys[id + "_body"];
		gui.push_back(temp);
	}
	{
		Widget temp(20, 60, 360, 50, wd_image, id + "_mark");
		temp.var["img"] = "ui\\body";
		temp.parent = ikeys[id];
		gui.push_back(temp);
	}
	int max_mark = 6;
	for (int i = 0; i < max_mark; i++)
	{
		{
			Widget temp(360 / max_mark * i, 0, 360 / max_mark, 50, wd_image, id + "_mark[" + std::to_string(i) + "]");
			temp.var["img"] = "ui\\body";
			temp.parent = ikeys[id + "_mark"];
			gui.push_back(temp); 
		}
		{
			Widget temp(360 / max_mark / 2, 25, 0, 0, wd_label, id + "_mark[" + std::to_string(i) + "]" + "_title");

			temp.parent = ikeys[id + "_mark[" + std::to_string(i) + "]"];
			temp.var["text"] = "언론";
			temp.var["opt"] = std::to_string(middle_align + center_align);
			temp.var["ind"] = "3";
			temp.var["color"] = "c_white";
			temp.var["size"] = "30";
			temp.var["ratio"] = "0.8";
			gui.push_back(temp);
		}
	}
	{
		Widget temp(350, 10, 40, 40, wd_image, id + "_cancel");
		temp.var["img"] = "ui\\left_menu_button";
		temp.parent = ikeys[id];
		temp.avail_mousedown_ev = true;
		temp.mousedown_ev = parent_remove;
		gui.push_back(temp);
	}
	{
		Widget temp(358, 18, 24, 24, wd_image, id + "_cancel_button");
		temp.var["img"] = "ui\\heard_menu_close";
		temp.parent = ikeys[id];
		temp.avail_mousedown_ev = true;
		temp.mousedown_ev = parent_remove;
		gui.push_back(temp);
	}
	{
		Widget temp(200, 25, 24, 24, wd_label, id + "_title");

		temp.parent = ikeys[id];
		temp.var["text"] = "언론";
		temp.var["opt"] = std::to_string(middle_align + center_align);
		temp.var["ind"] = "2";
		temp.var["color"] = "c_white";
		temp.var["size"] = "40";
		temp.var["ratio"] = "0.8";
		gui.push_back(temp);
	}
}


void pop_law(int x, int y)
{
	std::string id = empty_gui_slot("@ui\\media");
	{
		Widget temp(x, y, 400, 520, wd_image, id);
		temp.var["img"] = "ui\\body";
		gui.push_back(temp);
	}
	{
		Widget temp(20, 60, 360, 440, wd_image, id + "_body");
		temp.var["img"] = "ui\\body";
		temp.parent = ikeys[id];
		gui.push_back(temp);
	}
	{
		Widget temp(0, 0, 300, 440, wd_image, id + "_body_left");
		temp.var["img"] = "ui\\body";
		temp.parent = ikeys[id + "_body"];
		gui.push_back(temp);
	}
	{
		Widget temp(320, 0, 40, 440, wd_image, id + "_body_right");
		temp.var["img"] = "ui\\body";
		temp.parent = ikeys[id + "_body"];
		gui.push_back(temp);
	}
	{
		Widget temp(350, 10, 40, 40, wd_image, id + "_cancel");
		temp.var["img"] = "ui\\left_menu_button";
		temp.parent = ikeys[id];
		temp.avail_mousedown_ev = true;
		temp.mousedown_ev = parent_remove;
		gui.push_back(temp);
	}
	{
		Widget temp(358, 18, 24, 24, wd_image, id + "_cancel_button");
		temp.var["img"] = "ui\\heard_menu_close";
		temp.parent = ikeys[id];
		temp.avail_mousedown_ev = true;
		temp.mousedown_ev = parent_remove;
		gui.push_back(temp);
	}
	{
		Widget temp(200, 25, 24, 24, wd_label, id + "_title");

		temp.parent = ikeys[id];
		temp.var["text"] = "사법부";
		temp.var["opt"] = std::to_string(middle_align + center_align);
		temp.var["ind"] = "2";
		temp.var["color"] = "c_white";
		temp.var["size"] = "40";
		temp.var["ratio"] = "0.8";
		gui.push_back(temp);
	}
}

void safe_start()
{
	const int minimap_trailer_items = std::stoi(keys["minimap_trailer_items"]);
	const int left_menu_items = std::stoi(keys["left_menu_items"]);
	const int minimap_header_items = std::stoi(keys["minimap_header_items"]);
	const int right_hand_items = std::stoi(keys["right_hand_items"]);

	{
		Widget temp(0, 0, 140, 140, wd_image, "@ui\\potrait");
		temp.var["img"] = "potrait\\kys";
		gui.push_back(temp);
	}
	{
		Widget temp(0, 0, 140, 140, wd_image, "@ui\\potrait_case");
		temp.var["img"] = "ui\\potrait_case_blue";
		temp.parent = ikeys["@ui\\potrait"];
		gui.push_back(temp);
	}
	{
		Widget temp(300, 300, 140, 100, wd_image, "@ui\\tooltip");
		temp.var["img"] = "ui\\body";
		gui.push_back(temp);
	}


	{
		Widget temp(scr_w / 2 - 400, scr_h - 20, 800, 800, wd_image, "@ui\\issue");
		temp.var["img"] = "ui\\paper";
		gui.push_back(temp);
	}

	{
		Widget temp(scr_w - 244, 32, 244, 176, wd_image, "@ui\\heard");
		temp.var["img"] = "ui\\right_heard_body";
		gui.push_back(temp);
	}
	{
		Widget temp(0, 0, 40, 40, wd_image, "@ui\\heard_open_back");
		temp.var["img"] = "ui\\right_heard_body";
		temp.parent = ikeys["@ui\\heard"];
		gui.push_back(temp);
	}
	{
		Widget temp(0, 136, 40, 40, wd_image, "@ui\\heard_close_back");
		temp.var["img"] = "ui\\right_heard_body";
		temp.parent = ikeys["@ui\\heard"];
		gui.push_back(temp);
	}
	{
		Widget temp(8, 8, 24, 24, wd_image, "heard_menu_open");
		temp.var["img"] = "ui\\heard_menu_open";
		temp.parent = ikeys["@ui\\heard_open_back"];
		gui.push_back(temp);
	}
	{
		Widget temp(8, 8, 24, 24, wd_image, "heard_menu_close");
		temp.var["img"] = "ui\\heard_menu_close";
		temp.parent = ikeys["@ui\\heard_close_back"];
		gui.push_back(temp);
	}
	{
		Widget temp(144, 0, 100, 144, wd_none, "@ui\\right_hand");
		gui.push_back(temp);
	}

	for (int i = 0; i < right_hand_items; i++)
	{
		Widget temp(0, (i * 144 / right_hand_items), 100, 144 / right_hand_items, wd_image, "@ui\\right_hand[" + std::to_string(i) + "]");
		temp.var["img"] = "ui\\right_heard_body";
		temp.parent = ikeys["@ui\\right_hand"];

		gui.push_back(temp);
	}

	{
		Widget temp(scr_w - 244, 0, 244, 32, wd_image, "@ui\\timer");
		temp.var["img"] = "ui\\timer_body";
		gui.push_back(temp);
	}
	{
		Widget temp(122, 10, 0, 0, wd_label, "@ui\\timer_text");
		temp.parent = ikeys["@ui\\timer"];
		temp.var["text"] = "ui\\timer_body";
		temp.var["opt"] = std::to_string(middle_align + center_align);
		temp.var["ind"] = "2";
		temp.var["color"] = "c_white";
		temp.var["size"] = "20";
		temp.var["ratio"] = "0.6";
		gui.push_back(temp);
	}


	{
		Widget temp(scr_w - 300, scr_h - 210, 300, 170, wd_image, "@ui\\minimap");
		temp.var["img"] = keys["minimap_background"];
		gui.push_back(temp);
	}
	{
		Widget temp(0, 170, 300, 40, wd_none, "@ui\\minimap_trailer");
		temp.parent = ikeys["@ui\\minimap"];
		gui.push_back(temp);
	}
	for (int i = 0; i < minimap_trailer_items; i++)
	{
		Widget temp((i * 300.0 / minimap_trailer_items), 0, 300 / minimap_trailer_items, 40, wd_image, "minimap_item[" + std::to_string(i) + "]");
		temp.parent = ikeys["@ui\\minimap_trailer"];

		temp.var["img"] = keys["minimap_trailer_items[" + std::to_string(i) + "]"];

		gui.push_back(temp);
	}
	{
		Widget temp(0, -30, 300, 30, wd_image, "@ui\\minimap_header");
		temp.parent = ikeys["@ui\\minimap"];
		temp.var["img"] = keys["minimap_header_back"];
		gui.push_back(temp);
	}

	for (int i = 0; i < minimap_header_items; i++)
	{
		{
			Widget temp((i * 300.0 / minimap_header_items), 0, 300 / minimap_header_items, 30, wd_image, "minimap_header_back[" + std::to_string(i) + "]");
			temp.parent = ikeys["@ui\\minimap_header"];

			temp.var["img"] = keys["minimap_header_back"];

			gui.push_back(temp);
		}
		{
			Widget temp(0, 0, 300 / minimap_header_items, 30, wd_image, "minimap_header_item[" + std::to_string(i) + "]");
			temp.parent = ikeys["minimap_header_back[" + std::to_string(i) + "]"];

			temp.var["img"] = keys["minimap_header_items[" + std::to_string(i) + "]"];

			gui.push_back(temp);
		}
	}

	{
		Widget temp(0, 144, 48, 48 * left_menu_items, wd_image, "@ui\\left_menu");
		temp.var["img"] = "ui\\left_menu_body";
		gui.push_back(temp);
	}
	for (int i = 0; i < left_menu_items; i++)
	{
		{
			Widget temp(4, 48 * i, 40, 40, wd_image, "@ui\\left_menu_button[" + std::to_string(i) + "]");
			temp.parent = ikeys["@ui\\left_menu"];
			temp.var["img"] = "ui\\left_menu_button";
			gui.push_back(temp);
		}
		{
			Widget temp(8, 8, 24, 24, wd_image, "@ui\\left_menu_items[" + std::to_string(i) + "]");
			temp.parent = ikeys["@ui\\left_menu_button[" + std::to_string(i) + "]"];
			temp.var["img"] = keys["left_menu_items[" + std::to_string(i) + "]"];
			gui.push_back(temp);
		}
	}
	gui[ikeys["@ui\\left_menu_button[0]"]].avail_mousedown_ev = true;
	gui[ikeys["@ui\\left_menu_button[0]"]].mousedown_ev = LeftMenu_Item0_Click;
	gui[ikeys["@ui\\left_menu_button[1]"]].avail_mousedown_ev = true;
	gui[ikeys["@ui\\left_menu_button[1]"]].mousedown_ev = LeftMenu_Item1_Click;
	gui[ikeys["@ui\\left_menu_button[2]"]].avail_mousedown_ev = true;
	gui[ikeys["@ui\\left_menu_button[2]"]].mousedown_ev = LeftMenu_Item2_Click;
	gui[ikeys["@ui\\left_menu_button[3]"]].avail_mousedown_ev = true;
	gui[ikeys["@ui\\left_menu_button[3]"]].mousedown_ev = LeftMenu_Item3_Click;
	gui[ikeys["@ui\\left_menu_button[4]"]].avail_mousedown_ev = true;
	gui[ikeys["@ui\\left_menu_button[4]"]].mousedown_ev = LeftMenu_Item4_Click;
	gui[ikeys["@ui\\left_menu_button[5]"]].avail_mousedown_ev = true;
	gui[ikeys["@ui\\left_menu_button[5]"]].mousedown_ev = LeftMenu_Item5_Click;
	
	{
		Widget temp(0, 48 * left_menu_items, 48, 32, wd_none, "@ui\\left_menu_trailer");
		temp.parent = ikeys["@ui\\left_menu"];
		gui.push_back(temp);
	}
	{
		Widget temp(0, 0, 24, 32, wd_image, "@ui\\left_menu_trailer_left");
		temp.parent = ikeys["@ui\\left_menu_trailer"];
		temp.var["img"] = "ui\\left_arrow";
		gui.push_back(temp);
	}
	{
		Widget temp(24, 0, 24, 32, wd_image, "@ui\\left_menu_trailer_right");
		temp.parent = ikeys["@ui\\left_menu_trailer"];
		temp.var["img"] = "ui\\right_arrow";
		gui.push_back(temp);
	}
	normal_start();
}