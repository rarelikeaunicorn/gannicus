#include "move.h"
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <math.h>
using namespace std;

airMove::airMove(const char * n)
{
	build(n);
	init();
}

void airMove::land(move *& c)
{
	init();
	c = landing;
}

void airMove::build(const char * n)
{
	move::build(n);
	landing = NULL;
}

void airMove::setLR(move * c)
{
	landing = c;
}

airUtility::airUtility(const char * n)
{
	airMove::build(n);
	airMove::init();
}

bool airUtility::check(bool* pos, bool* neg, int t, int f, int* resource) //Check to see if the move is possible right now.
{
	if(abs(delta[0][0].y) > abs(delta[0][0].x) && resource[1] < 1) return 0;
	else if(abs(delta[0][0].y) < abs(delta[0][0].x) && resource[2] < 1) return 0;
	return utility::check(pos, neg, t, f, resource);
}

void airUtility::execute(move * last, int *& resource){
	if(abs(delta[0][0].y) > abs(delta[0][0].x)) resource[1]--;
	else if(abs(delta[0][0].y) < abs(delta[0][0].x)) resource[2]--;
	move::execute(last, resource);
}

airLooping::airLooping(const char * n)
{
	airMove::build(n);
	airMove::init();
}
