#pragma once
#include "main.h"

void put_path(const unsigned int id, const size_t start, const size_t end)
{
	static std::map<std::pair<size_t, size_t>, std::list<size_t>> pathes;
	static std::vector<SpiderMap> spider_map = {};
	if (start == end) return;

	if (pathes.find({ start, end }) == pathes.end())
	{
		spider_map.resize(prv.size());
		size_t max = std::numeric_limits<size_t>::max();
		for (auto& e : spider_map)
		{
			e.len = max;
			e.nearest = max;
			e.outted = false;
		}

		spider_map.at(end).len = 0;
		spider_map.at(end).outted = true;
		size_t i, j;
		size_t limit = prv.size();
		while (!spider_map.at(start).outted && limit > 0)
		{
			i = 0;
			for (auto I = spider_map.begin(); I != spider_map.end(); ++I, ++i)
			{
				if (I->outted)
				{
					j = 0;
					for (auto J = spider_map.begin(); J != spider_map.end(); ++J, ++j)
					{
						if (!J->outted && J->enable && map_connect[{i, j}] != 0)
						{
							if (J->len > map_connect[{i, j}] + I->len)
							{
								J->len = map_connect[{i, j}] + I->len;
								J->nearest = i;
							}
						}
					}
				}
			}
			i = 0;
			j = 0;
			size_t syr = max;
			for (auto I = spider_map.begin(); I != spider_map.end(); ++I, ++i)
			{
				if (!I->outted && I->enable && I->len < syr)
				{
					j = i;
					syr = I->len;
				}
			}
			spider_map.at(j).outted = true;
			limit--;
		}

		std::list<size_t> path = {};
		if (limit > 0)
		{
			i = start;
			while (i != end)
			{
				path.push_back(i);
				i = spider_map.at(i).nearest;
			}
			path.push_back(end);
		}
		pathes[{start, end}] = path;
	}

	man.at(id).com.clear();
	for (auto flag : pathes.at({ start, end })) man.at(id).com.push_back(flag);
}

const std::vector<SpiderMap> get_spider_map(const size_t start)
{
	static std::map<size_t, std::vector<SpiderMap>> map_list = {};
	if (map_list.find(start) == map_list.end()) {

		std::vector<SpiderMap> spider_map = {};
		size_t max = std::numeric_limits<size_t>::max();
		for (auto I = prv.begin(); I != prv.end(); ++I)
		{
			SpiderMap e = {};
			e.len = max;
			e.nearest = spider_map.size();
			e.outted = false;
			e.enable = !I->waste_land;
			spider_map.push_back(e);
		}

		spider_map.at(start).len = 0;
		spider_map.at(start).outted = true;
		size_t i, j;
		size_t limit = spider_map.size();
		while (limit > 0)
		{
			i = 0;
			for (auto I = spider_map.begin(); I != spider_map.end(); ++I, ++i)
			{
				if (I->outted)
				{
					j = 0;
					for (auto J = spider_map.begin(); J != spider_map.end(); ++J, ++j)
					{
						if (!J->outted && J->enable && map_connect[{i, j}] != 0)
						{
							if (J->len > map_connect[{i, j}] + I->len)
							{
								J->len = map_connect[{i, j}] + I->len;
								J->nearest = i;
							}
						}
					}
				}
			}
			i = 0;
			j = 0;
			size_t syr = max;
			for (auto I = spider_map.begin(); I != spider_map.end(); ++I, ++i)
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
			spider_map.at(j).outted = true;
			limit--;
		}
		map_list[start] = spider_map;
	}
	return map_list.at(start);
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



			if (Str(sec) != I.var[L"LOC"] && !prv.at(sec).waste_land && !prv.at(sec).waste_land && I.var[L"CON"] == prv.at(sec).var[L"CON"] && prv.at(sec).var[L"CON"] == prv.at(sec).var[L"OWN"])
			{
				put_path(id, Num(I.var[L"LOC"]), sec);
				return;
			}
		}
	}
}