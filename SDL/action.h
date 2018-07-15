#pragma once
#include "main.h"


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