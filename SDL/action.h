#pragma once
#include "act.h"

/*
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
void parent_remove(int id, int x, int y)
{
	(gui.begin() + (gui.begin() + id)->parent)->removing = true;
}
*/