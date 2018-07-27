#pragma once
#include "act.h"

int mother(const int i)
{
	if (gui[i].id == gui[i].parent)
	{
		return gui[i].id;
	}
	else
	{
		return mother(gui[i].parent);
	}
}
void parent_front(int id)
{
	//for (int mom = mother(id); mom != gui.size() - 1; ++mom)
	//{
	//	for (int a = 0; a < gui.size(); a++)
	//	{
	//		if (gui[a].parent == mom)
	//		{
	//			gui[a].parent = mom + 1;
	//		}
	//		else if (gui[a].parent == mom + 1)
	//		{
	//			gui[a].parent = mom;
	//		}
	//	}
	//	gui[mom].id = mom + 1;
	//	gui_key[gui[mom].var["name"]] = mom + 1;
	//	gui[mom + 1].id = mom;
	//	gui_key[gui[mom].var["name"]] = mom;
	//	std::iter_swap(gui[mom], gui[mom + 1]);
	//}
}
void parent_remove(unsigned int id)
{
	gui[gui[id].parent].removing = true;
}
