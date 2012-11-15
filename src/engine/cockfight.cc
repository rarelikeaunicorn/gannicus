#include "cockfight.h"
cockfight::cockfight()
{
	for(int i = 0; i < 2; i++)
		p[i] = new trainer;
	matchInit();
}

void cockfight::matchInit()
{
	for(int i = 0; i < 2; i++) 
		p[i]->HP = 100;
	winner = -1;
}

void cockfight::resolve()
{
	if(rand() % 2) p[0]->HP--;
	else p[1]->HP--;
	checkWin();
}

void cockfight::checkWin()
{
	for(int i = 0; i < 2; i++){
		if(p[i]->HP == 0){ 
			winner = (i+1)%2;
			gameover = 1;
		}
	}
}

void cockfight::draw()
{
	if(winner > -1) printf("Player %i wins!\n", winner + 1);
}