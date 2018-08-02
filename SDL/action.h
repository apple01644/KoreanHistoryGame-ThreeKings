#pragma once
#include "act.h"

int mother(const unsigned int i)
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
void parent_front(const unsigned int id)
{
	for (unsigned int mom = mother(id); mom != gui.size() - 1; ++mom)
	{
	/*	for (unsigned int a = 0; a < gui.size(); a++)
		{
			if (gui.at(a).parent == mom)
			{
				gui.at(a).parent = mom + 1;
			}
			else if (gui.at(a).parent == mom + 1)
			{
				gui.at(a).parent = mom;
			}
		}
		gui.at(mom).id = mom + 1;
		gui_key[gui.at(mom).var["name"]] = mom + 1;
		gui.at(mom + 1).id = mom;
		gui_key[gui.at(mom).var["name"]] = mom;
		std::iter_swap(*gui.at(mom), gui.at(mom + 1));*/
	}
}
void parent_remove(const unsigned int id)
{
	gui.at(gui.at(id).parent).removing = true;
}
