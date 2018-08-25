#pragma once
#include "main.h"

void put_path(const unsigned int id, const size_t start, const size_t end)
{
	if (start == end)
	{
		return;
	}

	man.at(id).com.clear();

	size_t max = std::numeric_limits<size_t>::max();
	for (auto I = prv.begin(); I != prv.end(); ++I)
	{
		I->len = max;
		I->nearest = max;
		I->outted = false;
	}

	prv.at(end).len = 0;
	prv.at(end).outted = true;
	size_t i, j;
	size_t limit = prv.size();
	while (!prv.at(start).outted && limit > 0)
	{
		i = 0;
		for (auto I = prv.begin(); I != prv.end(); ++I, ++i)
		{
			if (I->outted)
			{
				j = 0;
				for (auto J = prv.begin(); J != prv.end(); ++J, ++j)
				{
					if (!J->outted && J->enable && !J->waste_land && map_reg["map_conect" + Str(i) + "/" + Str(j)] != 0)
					{
						if (J->len > map_reg["map_conect" + Str(i) + "/" + Str(j)] + I->len)
						{
							J->len = map_reg["map_conect" + Str(i) + "/" + Str(j)] + I->len;
							J->nearest = i;
						}
					}
				}
			}
		}
		i = 0;
		j = 0;
		size_t syr = max;
		for (auto I = prv.begin(); I != prv.end(); ++I, ++i)
		{
			if (!I->outted && I->enable && I->len < syr)
			{
				j = i;
				syr = I->len;
			}
		}
		prv.at(j).outted = true;
		limit--;
	}

	if (limit > 0)
	{
		i = start;
		while (i != end)
		{
			man.at(id).com.push_back(i);
			i = prv.at(i).nearest;
		}
		man.at(id).com.push_back(end);
	}
}

void get_len(std::vector<AiProvince> *ai_prv,const size_t start)
{
	size_t max = std::numeric_limits<size_t>::max();
	for (auto I = ai_prv->begin(); I != ai_prv->end(); ++I)
	{
		I->len = max;
		I->nearest = ai_prv->size();
		I->outted = false;
	}

	ai_prv->at(start).len = 0;
	ai_prv->at(start).outted = true;
	size_t i, j;
	size_t limit = ai_prv->size();
	while (limit > 0)
	{
		i = 0;
		for (auto I = ai_prv->begin(); I != ai_prv->end(); ++I, ++i)
		{
			if (I->outted)
			{
				j = 0;
				for (auto J = ai_prv->begin(); J != ai_prv->end(); ++J, ++j)
				{
					if (!J->outted && J->enable && map_reg["map_conect" + Str(i) + "/" + Str(j)] != 0)
					{
						if (J->len > map_reg["map_conect" + Str(i) + "/" + Str(j)] + I->len)
						{
							J->len = map_reg["map_conect" + Str(i) + "/" + Str(j)] + I->len;
							J->nearest = i;
						}
					}
				}
			}
		}
		i = 0;
		j = 0;
		size_t syr = max;
		for (auto I = ai_prv->begin(); I != ai_prv->end(); ++I, ++i)
		{
			if (!I->outted && I->enable && I->len < syr)
			{
				j = i;
				syr = I->len;
			}
		}
		if (j == 0)
		{
			break;
		}
		ai_prv->at(j).outted = true;
		limit--;
	}
}

void run_to_rest(const unsigned int id)
{
	auto I = man.at(id);
	if (I.com.size() == 0)
	{
		unsigned int sec = 0;
		size_t limit = prv.size();

		for (; limit > 0; --limit)
		{
			sec = rand() % prv.size();

			

			if (Str(sec) != I.var["LOC"] && !prv.at(sec).waste_land && prv.at(sec).enable && I.var["CON"] == prv.at(sec).var["CON"] && prv.at(sec).var["CON"] == prv.at(sec).var["OWN"])
			{
				put_path(id, Num(I.var["LOC"]), sec);
				return;
			}
		}
	}
}