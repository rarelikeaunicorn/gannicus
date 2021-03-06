#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#include <string>
#include <vector>
#include "span.h"
#include "hud.h"
#ifndef STATUS_DEFINE
#define STATUS_DEFINE

using std::string;
using std::vector;

class instance;
class action;
struct status{
	status();
	status(const status&);
	virtual int cancelState() const;
	virtual SDL_Rect prox() const;
	virtual SDL_Rect prox(instance*) const;
	vector<HUDMeter<int>> meter;
	int mode;
	int posX, posY;
	int age;
	int facing;
	int deltaX, deltaY; 
	int frame;
	int freeze;
	int absorbedHits;
	bool aerial;
	bool rCorner, lCorner;
	action *move, *bufferedMove, *reversal;
	int reversalTimer;
	vector <instance*> offspring;
	int connect, hit, counter;
	int drawX, drawY;
	bool elasticX;
	bool elasticY;
	bool slide;
	bool stick;
	bool rebound;
	int hover;

	bool dead:1;
	int throwInvuln;
	bool reversalFlag:1;
	instance * opponent;
};
#endif
