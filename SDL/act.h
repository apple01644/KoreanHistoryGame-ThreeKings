#pragma once
#include "main.h"

void bad_word(int i, int j)
{
	agr(j,i, -2);
	auto I = man.begin();
	for (; I != man.end(); I++)
	{
		if (I->id == i)
		{
			auto J = man.begin();
			for (; J != man.end();)
			{
				if (J->id == j)
				{
					J->prestige -= I->prestige / 100;
					I->prestige -= J->prestige / 100;

					if (I->traits["찌라시"] && false)
					{
						auto K = man.begin();
						for (; K != man.end(); K++)
						{
							if (K->live)
							{
								if (I->id != K->id && J->id != K->id)
								{
									agr(K->id,J->id,-5);
								}
							}
						}
						I->traits["찌라시"] = false;
					}

					break;
				}
				J++;
			}
			break;
		}
	}
	agr(i, j, -10);
}
void good_word(int i, int j)
{
	//agr(j, i, 2);
	auto I = man.begin();
	for (; I != man.end();I++)
	{
		if (I->id == i)
		{
			auto J = man.begin();
			for (; J != man.end();)
			{
				if (J->id == j)
				{
					J->prestige += I->prestige / 3000;
					I->prestige += J->prestige / 3000;
					break;
				}
				J++;
			}
			break;
		}
	}
	//agr(i, j, 10);
}



void speech(int id, int Prv, int pop)
{
	auto I = man.begin();
	for (; I != man.end(); I++)
	{
		if (I->live && I->id == id)
		{
			I->charge -= 60;
			I->money -= 10;

			int eng = rand() % 9 + 2;

			if (I->traits["깡패의 지원"])
			{
				I->traits["깡패의 지원"] = false;
				eng += 10;
			}
			
			if (prv[Prv].pop[pop].fascist > I->fascist)
			{
				prv[Prv].pop[pop].fascist -= eng;
			}
			else if (prv[Prv].pop[pop].fascist < I->fascist)
			{
				prv[Prv].pop[pop].fascist += eng;
			}

			if (prv[Prv].pop[pop].liberty > I->liberty)
			{
				prv[Prv].pop[pop].liberty -= eng;
			}
			else if (prv[Prv].pop[pop].liberty < I->liberty)
			{
				prv[Prv].pop[pop].liberty += eng;
			}

			if (I->traits["교수의 지원"])
			{
				I->traits["교수의 지원"] = false;
				for (int r = 0; r < 3; r++)
				{
					pop = rand() % prv[Prv].pop.size();
					if (prv[Prv].pop[pop].fascist > I->fascist)
					{
						prv[Prv].pop[pop].fascist -= eng;
					}
					else if (prv[Prv].pop[pop].fascist < I->fascist)
					{
						prv[Prv].pop[pop].fascist += eng;
					}

					if (prv[Prv].pop[pop].liberty > I->liberty)
					{
						prv[Prv].pop[pop].liberty -= eng;
					}
					else if (prv[Prv].pop[pop].liberty < I->liberty)
					{
						prv[Prv].pop[pop].liberty += eng;
					}
				}
			}

			eng = rand() % 9 + 2;

			break;
		}
	}
}
void bribe(int i, int comp)
{
	auto I = man.begin();
	for (; I != man.end(); I++)
	{
		if (I->live && I->id == i)
		{
			int j = 0;
			auto o = company.begin();
			for (;o != company.end();o++,j++)
			{
				if (j == comp)
				{
					auto J = man.begin();
					for (; J != man.end(); J++)
					{
						if (J->id == o->owner)
						{
							int money = 0;
							for (int a = 0; a < 5; a++)
							{
								if (gr(I->id, J->id) > 30 + 30 * a)
								{
									money += 50;
								}
								else
								{
									break;
								}
							}
							if (I->id == play_id)
							{
								if (money == 0)
								{
									msg_push(J->id,"죄송하지만 이 이야기는 없던걸로 하겠습니다.");
								}
								else
								{
									msg_push(J->id, "그 제안을 흔쾌히 받아들이겠습니다.\n곧 자금 "+std::to_string(money)+"을 받을수 있을 겁니다.");
									J->money -= money;
									I->money += money;
								}
							}
							agr(I->id, J->id, -20);
						}

					}
					break;
				}
			}
			break;
		}
	}
}
void loby(int i, int comp)
{
	auto I = man.begin();
	for (; I != man.end(); I++)
	{
		if (I->live && I->id == i)
		{
			int j = 0;
			auto o = company.begin();
			for (; o != company.end(); o++, j++)
			{
				if (j == comp)
				{
					auto J = man.begin();
					for (; J != man.end(); J++)
					{
						if (J->live && J->id == o->owner)
						{
							int rel = 5;
							if (gr(I->id, J->id) > 30)
							{
								rel += 10;
							}

							I->prestige -= 15;
							o->power += 3;
							agr(I->id, J->id, rel);


							if (I->id == play_id)
							{
								if (rel == 5)
								{
									msg_push(J->id, "아 좋습니다.\n다음도 이런 관계를 유지 하길 희망합니다.");
								}
								else
								{
									msg_push(J->id, "저희 회사는 귀하를 매우 사모하고 있습니다.\n언제든 필요하면 말해주십시오. 당신의 친우 " + J->name);
								}
							}

						}
					}


					break;
				}
			}
			break;
		}
	}
}

void lobi(int i, int j)
{
	auto I = man.begin();
	for (; I != man.end(); I++)
	{
		if (I->live && I->id == i)
		{
			auto J = man.begin();
			for (; J != man.end(); J++)
			{
				if (J->id == j)
				{
					if (I->id == play_id)
					{
						if (gr(I->id, J->id) < 30)
						{
							msg_push(J->id, "좋습니다.\n앞으로도 이런 관계를 유지 하길 원합니다.");
						}
						else
						{
							msg_push(J->id, "당신은 나에게 있어 큰 동료입니다.\n당신 덕분에 오늘도 편히 생활합니다.");
							J->money += 50;
							I->money -= 50;
							agr(I->id, J->id, 10);
						}
					}
					agr(I->id, J->id, 20);
				}
			}
			break;
		}
	}
}



void mafia(int i, int frc)
{
	auto I = man.begin();
	for (; I != man.end(); I++)
	{
		if (I->live && I->id == i)
		{
			auto o = force.begin();
			int j = 0;
			for (; o != force.end(); o++, j++)
			{
				if (j == frc)
				{
					auto J = man.begin();
					for (; J != man.end(); J++)
					{
						if (J->id == o->owner)
						{
							if (I->traits["깡패의 지원"])
							{
								agr(I->id, J->id, 20);
							}
							else if (gr(I->id, J->id) < 30)
							{

							}
							else
							{
								I->traits["깡패의 지원"] == true;
							}
							if (I->id == play_id)
							{
								if (I->traits["깡패의 지원"])
								{
									msg_push(J->id, "당신은 이미 다른 파의 지원을 받고 있습니다.\n그 건이 끝나면 그때 다시 알려주십시요.");
								}
								else if (gr(I->id, J->id) < 30)
								{
									msg_push(J->id, "우리는 우리에 뜻대로 행동할겁니다.\n이런 제안을 하기전에 우리의 관계를\n생각해보시오");
								}
								else
								{
									msg_push(J->id, "좋습니다.\n당신이 다음 유세하는 구역에서 활동을 시작하겠습니다.");
								}
							}
							agr(I->id, J->id, -20);
						}
					}
				}
			}
			break;
		}
	}
}

void booking(int i, int edu)
{
	auto I = man.begin();
	for (; I != man.end(); I++)
	{
		if (I->live && I->id == i)
		{
			auto o = education.begin();
			int j = 0;
			for (; o != education.end(); o++, j++)
			{
				if (j == edu)
				{
					auto J = man.begin();
					for (; J != man.end(); J++)
					{
						if (J->id == o->owner)
						{
							if (I->traits["교수의 지원"])
							{
								agr(I->id, J->id, 20);
							}
							else if (gr(I->id, J->id) < 30)
							{

							}
							else
							{
								I->traits["교수의 지원"] == true;
							}
							if (I->id == play_id)
							{
								if (I->traits["교수의 지원"])
								{
									msg_push(J->id, "선생은 이미 다른 사람으로 부터 지원 받고 있습니다.\n그 일을 끝마친뒤에 다시 연락해 주시오.");
								}
								else if (gr(I->id, J->id) < 30)
								{
									msg_push(J->id, "우리는 우리에 뜻대로 행동할겁니다.\n이런 제안을 하기전에 우리의 관계를\n생각해보시오");
								}
								else
								{
									msg_push(J->id, "아주 좋군요\n다음에 어디서 유세 하시겠습니다.\n그리로 찾아뵙기로 하죠.");
								}
							}
							agr(I->id, J->id, -20);
						}
					}
				}
			}
			break;
		}
	}
}

void jirassi(int i, int med)
{
	auto I = man.begin();
	for (; I != man.end(); I++)
	{
		if (I->live && I->id == i)
		{
			auto o = education.begin();
			int j = 0;
			for (; o != education.end(); o++, j++)
			{
				if (j == med)
				{
					auto J = man.begin();
					for (; J != man.end(); J++)
					{
						if (J->id == o->owner)
						{
							if (I->traits["찌라시"])
							{
								agr(I->id, J->id, 20);
							}
							else if (gr(I->id, J->id) < 30)
							{

							}
							else
							{
								I->traits["찌라시"] == true;
							}
							if (I->id == play_id)
							{
								if (I->traits["찌라시"])
								{
									msg_push(J->id, "글쎄오시다.\n이미 당신은 이슈의 중심에 있어서\n관련된 기사를 내기 어렵습니다.");
								}
								else if (gr(I->id, J->id) < 30)
								{
									msg_push(J->id, "우리는 우리에 뜻대로 행동할겁니다.\n이런 제안을 하기전에 우리의 관계를\n생각해보시오");
								}
								else
								{
									msg_push(J->id, "아주 좋군요\n당신이 비난하시면 저희도 그를 같이 비난하겠습니다.");
								}
							}
							agr(I->id, J->id, -20);
						}
					}
				}
			}
			break;
		}
	}
}
void impress(int i, int med)
{
	auto I = man.begin();
	for (; I != man.end(); I++)
	{
		if (I->live && I->id == i)
		{
			auto o = education.begin();
			int j = 0;
			for (; o != education.end(); o++, j++)
			{
				if (j == med)
				{
					auto J = man.begin();
					for (; J != man.end(); J++)
					{
						if (J->id == o->owner)
						{
							if (gr(I->id, J->id) < 30)
							{

							}
							else
							{
								I->prestige+= 50;
							}
							if (I->id == play_id)
							{
								if (gr(I->id, J->id) < 30)
								{
									msg_push(J->id, "우리는 우리에 뜻대로 행동할겁니다.\n이런 제안을 하기전에 우리의 관계를\n생각해보시오");
								}
								else
								{
									msg_push(J->id, "아주 좋군요\n곧 당신의 선행을 칭찬하는 기사에 대해서 작성하겠습니다.");
								}
							}
							agr(I->id, J->id, -20);
						}
					}
				}
			}
			break;
		}
	}
}