#include <stdio.h>
#include <iostream>
#include <SDL/SDL.h>
#include <unistd.h>
#include <stdlib.h>
#include <fstream>
using namespace std;
#include "player.h"
player::player()
{
	init();
}

player::player(int id)
{
	ID = id;
	init();
}

void player::init()
{
	/*Initialize input containers*/
	for(int i = 0; i < 30; i++)
		inputBuffer[i] = 5;

	/*Yeah yeah, I know, char* to literal conversion. I'm lazy right now. Will fix later. Maybe with cstring*/
	inputName[0] = "Up";
	inputName[1] = "Down";
	inputName[2] = "Left";
	inputName[3] = "Right";
	inputName[4] = "A";
	inputName[5] = "B";
	inputName[6] = "C";
	inputName[7] = "D";
	inputName[8] = "E";
	inputName[9] = "Start";

	if(!readConfig()) writeConfig();

	/*Set up the sprite*/

	rounds = 0;
	deltaX = 0;
	deltaY = 0;
	regComplexity = 0;
	hitComplexity = 0;
	momentumComplexity = 0;
	momentum = NULL;
	lCorner = 0;
	rCorner = 0;
}

bool player::readConfig()
{
	char fname[30];
	sprintf(fname, "Misc/.p%i.conf", ID);
	ifstream read;
	read.open(fname);
	if(read.fail()) {
		read.close();
		return 0;
	}
	else{
		char * token;
		char buffer[30];
		for(int i = 0; i < 10; i++){
			read.get(buffer, 30, ':'); read.ignore(); 
			input[i].type = atoi(buffer);
			read.ignore();
			read.get(buffer, 30, '\n'); read.ignore();
			switch (input[i].type){
			case SDL_JOYAXISMOTION:
				token = strtok(buffer, " \n");
				input[i].jaxis.which = atoi(token);
				token = strtok(NULL, " \n");
				input[i].jaxis.axis = atoi(token);
				token = strtok(NULL, " \n");
				input[i].jaxis.value = atoi(token);
				break;
			case SDL_JOYBUTTONDOWN:
				token = strtok(buffer, " \n");
				input[i].jbutton.which = atoi(token);
				token = strtok(NULL, " \n");
				input[i].jbutton.button = atoi(token);
				break;
			case SDL_KEYDOWN:
				token = strtok(buffer, " \n");	
				input[i].key.keysym.sym = (SDLKey)atoi(token);
				break;
			default:
				break;
			}
		}
		read.close();
		return 1;
	}
}

void player::writeConfig()
{
	char fname[30];
	printf("Player %i:\n", ID);
	sprintf(fname, "Misc/.p%i.conf", ID);
	ofstream write;
	SDL_Event temp; 
	write.open(fname);
	/*Set up ALL the inputs*/
	for(int i = 0; i < 10; i++){
		printf("Please enter a command for %s\n", inputName[i]);
		/*Set up dummy event*/

		/*Flag for breaking the loop*/
		bool configFlag = 0;

		while(SDL_PollEvent(&temp));
		while (configFlag == 0){
			if (SDL_PollEvent(&temp)) {
				switch (temp.type) {
				case SDL_JOYAXISMOTION:
					if(temp.jaxis.value != 0){
						input[i] = temp;
						write << (int)temp.type << " : " << (int)temp.jaxis.which << " " << (int)temp.jaxis.axis << " " << (int)temp.jaxis.value << "\n";
						printf("Set to Joystick %i axis %i value %i\n", input[i].jaxis.which, input[i].jaxis.axis, input[i].jaxis.value);
						configFlag = 1;
					}
					break;
				case SDL_JOYBUTTONDOWN:
					input[i] = temp;
					write << (int)temp.type << " : " << (int)temp.jbutton.which << " " << (int)temp.jbutton.button << "\n";
					printf("Set to Joystick %i button %i\n", input[i].jbutton.which, input[i].jbutton.button);
					configFlag = 1;
					break;
				case SDL_KEYDOWN:
					input[i] = temp;
					write << (int)temp.type << " : " << (int)temp.key.keysym.sym << "\n";
					printf("Set to keyboard %s\n", SDL_GetKeyName(input[i].key.keysym.sym));
					configFlag = 1;
					break;
				default: 
					break;
				}
			}
		}
	}
	write.close();
}

void player::characterSelect(int i)
{
	pick = NULL;
	switch(i){
	case 1:
		pick = new red;
		break;
	case 2:
		pick = new yellow;
		break;
	default:
		pick = new character;
		break;
	}
}

void player::updateRects()
{
	if(pick->cMove != NULL) {
		pick->cMove->pollRects(collision, hitreg, regComplexity, hitbox, hitComplexity);
		for(int i = 0; i < hitComplexity; i++){
			if(facing == -1) hitbox[i].x = posX - hitbox[i].x - hitbox[i].w;
			else hitbox[i].x += posX;
			hitbox[i].y += posY;
		}
		for(int i = 0; i < regComplexity; i++){
			if(facing == -1) hitreg[i].x = posX - hitreg[i].x - hitreg[i].w;
			else hitreg[i].x += posX;
			hitreg[i].y += posY;
		}
		if(facing == -1) collision.x = posX - collision.x - collision.w;
		else collision.x += posX;
		collision.y += posY;
	}
}

void player::combineDelta()
{
	for(int i = 0; i < momentumComplexity; i++){
		deltaX += momentum[i].x;
		deltaY += momentum[i].y;

		if(momentum[i].w <= 0) {
			removeVector(i);
			i--;
		}
		else momentum[i].w--;
	}
	posX += deltaX;
	posY += deltaY;
	updateRects();
}

void player::enforceGravity(int grav, int floor)
{
	SDL_Rect g; g.x = 0; g.y = 3; g.w = 0; g.h = 0;

	if(collision.y + collision.h < floor && pick->aerial == 0){
		pick->aerial = 1;
	}

	else if(pick->aerial && !pick->freeze) addVector(g);
}

void player::checkBlocking()
{
	int st;
	if(pick->cMove == pick->airBlock || pick->cMove == pick->standBlock || pick->cMove == pick->crouchBlock)
		st = ((hitstun*)pick->cMove)->counter;
	else st = 0;

	switch(inputBuffer[0]){
	case 7:
	case 4:
		if(pick->aerial && (*pick->airBlock) > pick->cMove) {
			pick->airBlock->init(st);
			pick->cMove = pick->airBlock;
		}
		else if((*pick->standBlock) > pick->cMove) {
			pick->standBlock->init(st);
			pick->cMove = pick->standBlock;
		}
		updateRects();
		break;
	case 1:
		if(pick->aerial && (*pick->airBlock) > pick->cMove) {
			pick->airBlock->init(st);
			pick->cMove = pick->airBlock;
		}
		else if((*pick->crouchBlock) > pick->cMove) {
			pick->crouchBlock->init(st);
			pick->cMove = pick->crouchBlock;
		}
		updateRects();
		break;
	default:
		break;
	}
}

void player::checkCorners(int floor, int left, int right)
{
	/*Offset variables. I could do these calculations on the fly, but it's easier this way.
	Essentially, this represents the offset between the sprite and the collision box, since
	even though we're *checking* collision, we're still *moving* spr*/
	int lOffset = posX - collision.x;
	int rOffset = posX - (collision.x + collision.w);
	int hOffset = posY - (collision.y);

	/*Floor, or "Bottom corner"*/

	if (collision.y + collision.h > floor){
		if(pick->aerial == 1){
			if(pick->cMove == pick->airBlock){
				pick->standBlock->init(pick->airBlock->counter);
				pick->cMove = pick->standBlock;
			} else { 
				if(pick->cMove) pick->cMove->land(pick->cMove);
				if(!pick->cMove) pick->cMove = pick->neutral;
			}
			pick->aerial = 0; 
			updateRects();
			hOffset = posY - (collision.y);
			deltaX = 0;
		}
		deltaY = 0;
		posY = floor - collision.h + hOffset;
	}
	updateRects();

	/*Walls, or "Left and Right" corners

	This not only keeps the characters within the stage boundaries, but flags them as "in the corner"
	so we can specialcase collision checks for when one player is in the corner.*/

	if(collision.x <= left){
		if(facing == 1 && collision.x <= 25) lCorner = 1;
		if(collision.x < left) 
			posX = left + lOffset;
	} else lCorner = 0;
	if(collision.x + collision.w >= right){
		if(facing == -1 && collision.x + collision.w >= 1575) rCorner = 1;
		if(collision.x + collision.w > right){
			posX = right + rOffset;
		}
	} else rCorner = 0;
	updateRects(); //Update rectangles or the next collision check will be wrong.
}

void player::checkFacing(player * other){
	int comparison, midpoint;
	midpoint = collision.x + collision.w/2;
	comparison = other->collision.x + other->collision.w/2;
	if(other->posX < posX) comparison += collision.w % 2; 
	else {
		midpoint += collision.w % 2;
	}
	if (lCorner) facing = 1;
	else if (rCorner) facing = -1;
	else if (midpoint < comparison){
		if(facing == -1) posX -= spr.w;
		facing = 1;
	}
	else if (midpoint > comparison){
		if(facing == 1) posX += spr.w;
		facing = -1;
	}
	updateRects();
}

int player::dragBG(int left, int right)
{
	if(collision.x < left) return collision.x - left;
	else if (collision.x + collision.w > right) return (collision.x + collision.w) - right;
	else return 0;
}

player::~player(){
	SDL_FreeSurface(sprite);
	delete pick;
}

void player::pushInput(bool axis[4], bool down[5], bool up[5])
{
	int temp = 5 + axis[0]*3 - axis[1]*3 - axis[2]*facing + axis[3]*facing;
	inputBuffer[0] = temp;

	for(int i = 29; i > 0; i--){
		inputBuffer[i] = inputBuffer[i-1];
	}

	pick->prepHooks(inputBuffer, down, up);
}

void player::pullVolition()
{
	int top = 0;
	for(int i = 0; i < momentumComplexity; i++)
		if(momentum[i].h > top) top = momentum[i].h;
	if(pick->cMove->stop){
		if(pick->cMove->currentFrame == 0){ 
			deltaX = 0; deltaY = 0;
			if(pick->cMove->stop == 2)
				momentumComplexity = 0;
		}
	}
	SDL_Rect * temp = pick->cMove->delta[pick->cMove->currentFrame];
	for(int i = 0; i < pick->cMove->deltaComplexity[pick->cMove->currentFrame]; i++){
		if(temp[i].x || temp[i].y || temp[i].h){
			if(temp[i].h >= top || top == 0){
				addVector(temp[i]);
			}
		}
	}
}

void player::addVector(SDL_Rect &v)
{
	int i;
	SDL_Rect * temp;
	temp = new SDL_Rect[momentumComplexity+1];
	for(i = 0; i < momentumComplexity; i++){
		temp[i].x = momentum[i].x;
		temp[i].y = momentum[i].y;
		temp[i].w = momentum[i].w;
		temp[i].h = momentum[i].h;
	}
	temp[i].x = v.x*facing;
	temp[i].y = v.y;
	temp[i].w = v.w;
	temp[i].h = v.h;
	if(momentumComplexity > 0) delete [] momentum;
	momentum = temp;
	momentumComplexity++;
}

void player::removeVector(int n)
{
	if(momentumComplexity < 0 || !momentum) return;
	for(int i = n; i < momentumComplexity-1; i++){
		momentum[i].x = momentum[i+1].x;
		momentum[i].y = momentum[i+1].y;
		momentum[i].w = momentum[i+1].w;
		momentum[i].h = momentum[i+1].h;
	}
	momentumComplexity--;
}

void player::takeHit(player * attacker, move * attack)
{

}

void player::readEvent(SDL_Event & event, bool *& sAxis, bool *& posEdge, bool *& negEdge)
{
	bool type = 0;
	for(int i = 0; i < 10; i++)
		if(input[i].type == event.type) type = 1;
	if(!type) return;
	switch(event.type){
	case SDL_JOYAXISMOTION:
		for(int i = 0; i < 4; i++){
			if(event.jaxis.which == input[i].jaxis.which && event.jaxis.axis == input[i].jaxis.axis && event.jaxis.value == input[i].jaxis.value)
				sAxis[i] = 1;
			if(event.jaxis.which == input[i].jaxis.which && event.jaxis.axis == input[i].jaxis.axis && event.jaxis.value == 0)
				sAxis[i] = 0;
		}
		break;
	case SDL_JOYBUTTONDOWN:
		for(int i = 4; i < 9; i++){
			if(event.jbutton.which == input[i].jbutton.which && event.jbutton.button == input[i].jbutton.button)
				posEdge[i-4] = 1;
		}
		break;
	case SDL_JOYBUTTONUP:
		for(int i = 4; i < 9; i++){
			if(event.jbutton.which == input[i].jbutton.which && event.jbutton.button == input[i].jbutton.button)
				negEdge[i-4] = 1;
		}
		break;
	case SDL_KEYDOWN:
		for(int i = 0; i < 4; i++) {
			if(event.key.keysym.sym == input[i].key.keysym.sym) 
				sAxis[i] = 1;
		}
		for(int i = 4; i < 9; i++) {
			if(event.key.keysym.sym == input[i].key.keysym.sym)
				posEdge[i-4] = 1;
		}
	case SDL_KEYUP:
		for(int i = 0; i < 4; i++){
			if(event.key.keysym.sym == input[i].key.keysym.sym)
				sAxis[i] = 0;
		}
		for(int i = 4; i < 9; i++){
			if(event.key.keysym.sym == input[i].key.keysym.sym)
				negEdge[i-4] = 1;
		}
		break;
	}
}
