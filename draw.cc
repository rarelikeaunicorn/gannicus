/*Drawing routines for project: Ground Up Fighting Game
 *
 *Written by Alex Kelly in 2012, under MIT OSI
 *For detailed license information, see the file COPYING in this directory
 */

#include "interface.h"
#include <math.h>
#include <SDL/SDL_opengl.h>
void interface::draw()
{
	GLuint t1 = aux::surface_to_texture(p[0]->sprite);
	SDL_Surface * back = SDL_DisplayFormatAlpha(background);
	SDL_Rect bar1, bar2, rounds1[numRounds], rounds2[numRounds];
	
	if(p[0]->pick->health >= 0) bar1.w = p[0]->pick->health; else bar1.w = 1; 
	if(p[1]->pick->health >= 0) bar2.w = p[1]->pick->health; else bar2.w = 1;
	
	bar1.x = 50 + (300 - bar1.w); bar2.x = 450;
	bar1.h = 5; bar2.h = 5;
	bar1.y = 5; bar2.y = 5;
	
	
	for(int i = 0; i < numRounds; i++){
		rounds1[i].y = 12; rounds1[i].w = 10; rounds1[i].h = 5;
		rounds2[i].y = 12; rounds2[i].w = 10; rounds2[i].h = 5;
		rounds1[i].x = 340 - 12 * i; rounds2[i].x = 450 + 12 * i;
	}

	if(p[0]->sprite) SDL_BlitSurface(p[0]->sprite, NULL, back, &p[0]->spr);
	else {
		SDL_FillRect(back, &p[0]->collision, SDL_MapRGB(back->format, 255, 255, 255));
		for(int i = 0; i < p[0]->regComplexity; i++)
			SDL_FillRect(back, &p[0]->hitreg[i], SDL_MapRGB(back->format, 0, 255, 0));
		for(int i = 0; i < p[0]->hitComplexity; i++)
			SDL_FillRect(back, &p[0]->hitbox[i], SDL_MapRGB(back->format, 255, 0, 0));
	}
	if(p[1]->sprite) SDL_BlitSurface(p[1]->sprite, NULL, back, &p[1]->spr);
	else{
		SDL_FillRect(back, &p[1]->collision, SDL_MapRGB(back->format, 255, 255, 255));
		for(int i = 0; i < p[1]->regComplexity; i++)
			SDL_FillRect(back, &p[1]->hitreg[i], SDL_MapRGB(back->format, 0, 255, 0));
		for(int i = 0; i < p[1]->hitComplexity; i++)
			SDL_FillRect(back, &p[1]->hitbox[i], SDL_MapRGB(back->format, 255, 0, 0));
	}
	SDL_FillRect(screen, &screen->clip_rect, SDL_MapRGB(screen->format, 255, 212, 120));
	SDL_BlitSurface(back, &bg, screen, NULL);
	
	for(int i = 0; i < numRounds; i++){
		if(p[0]->rounds > i) SDL_FillRect(screen, &rounds1[i], SDL_MapRGB(screen->format, 0, 255, 255));
		else SDL_FillRect(screen, &rounds1[i], SDL_MapRGB(screen->format, 0, 0, 0));
		if(p[1]->rounds > i) SDL_FillRect(screen, &rounds2[i], SDL_MapRGB(screen->format, 0, 255, 255));
		else SDL_FillRect(screen, &rounds2[i], SDL_MapRGB(screen->format, 0, 0, 0));
	}

	SDL_FillRect(screen, &bar1, SDL_MapRGB(screen->format, 255, 0, 0));
	SDL_FillRect(screen, &bar2, SDL_MapRGB(screen->format, 255, 0, 0));
	
	for(int i = 0; i < 2; i++){
		p[i]->pick->drawMeters(screen, i);
	}
	
	SDL_UpdateRect(screen, 0, 0, 0, 0);
	SDL_FreeSurface(back);
}


void player::spriteInit()
{
	int realPosY = collision.y;
	int realPosX = 0;
	for(int i = 0; i < hitComplexity; i++){
		if(hitbox[i].y < realPosY) realPosY = hitbox[i].y;
		if(hitbox[i].x < realPosX) realPosX = hitbox[i].x;
	}
	for(int i = 0; i < regComplexity; i++){
		if(hitreg[i].y < realPosY) realPosY = hitreg[i].y;
		if(hitreg[i].x < realPosX) realPosX = hitreg[i].x;
	}

	
	/*Doing moves*/
//	if(pick->freeze > 0) pick->freeze--;
//	else 
		sprite = pick->draw(facing);
	if(facing == -1) {
		if(sprite) spr.x = posX - sprite->w;
	} else {
		if(sprite) spr.x = posX;
	}
	if(sprite) spr.y = realPosY;
}

SDL_Surface * character::draw(int facing)
{
	SDL_Surface * temp;
	if(freeze < 0) freeze = 0;
	temp = cMove->draw(facing, freeze, meter);
	if(freeze > 0) freeze--;
	if(cMove->currentFrame == cMove->frames){
		cMove->init();
		cMove = cMove->next;
	}
	return temp;
}

void character::drawMeters(SDL_Surface *& screen, int ID)
{	
	SDL_Rect m;
	int R = 0, G = 255, B = 0;
	if(meter[0] >= 0) m.w = meter[0]; else m.w = 1; 
	if(ID == 0) m.x = 100; 
	else m.x = 500 + (200 - m.w);
	m.h = 5; m.y = 580;

	if(m.w < 100) R = 191;
	else if(m.w < 200) B = 255;
	
	SDL_FillRect(screen, &m, SDL_MapRGB(screen->format, R, G, B));
}

SDL_Surface * move::draw(int facing, bool freeze, int *& meter)
{
	SDL_Surface * temp;
	if(facing == -1) temp = fSprite[currentFrame];
	else temp = sprite[currentFrame];
	if(freeze == 0) 
		step(meter);
	return temp;
}
