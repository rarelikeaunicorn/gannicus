#include "interface.h"
red::red()
{
	delete [] meter;
	
	meter = new int[5];
	
	airHead = new moveTrie;
	head = new moveTrie;
	build("Red");

	init();	
}

void red::touch(void * target)
{
	((interface*)target)->timer+=290;
}

void red::tick(){
	character::tick();
	if(meter[3] < 540) meter[3]++;
	if(meter[3] < 0) meter[3] = 0;
	if(meter[4] > 0) meter[4]--;
}

void red::drawMeters(int ID)
{
	int color1, color2;
	character::drawMeters(ID);
	SDL_Rect c1, c2;
	if(meter[3] >= 0){
		c1.w = meter[3]/2; 
	} else c1.w = 1; 
	if(meter[3] > 270){
		c1.w = 135; c2.w = (meter[3] - 270)/2;
	} else c2.w = 0;
	if(ID == 1){
		c1.x = 130; 
		c2.x = 130;
	}
	else { 
		c1.x = 530 + (135 - c1.w);
		c2.x = 530 + (135 - c2.w);
	}
	c1.h = 5; c2.h = 5;
	c1.y = 438; c2.y = 438;
	if(meter[3] >= 270 && meter[4] < 1) color1 = 255;
	else color1 = 127;
	if(meter[3] >= 540 && meter[4] < 1) color2 = 255;
	else color2 = 127;
	glColor4f(0.0f, 0.0f, (float)color1, 1.0f);
	glRectf((GLfloat)(c1.x), (GLfloat)(c1.y), (GLfloat)(c1.x + c1.w), (GLfloat)(c1.y + c1.h));
	glColor4f((float)color2, 0.0f, (float)color2, 1.0f);
	glRectf((GLfloat)(c2.x), (GLfloat)(c2.y), (GLfloat)(c2.x + c2.w), (GLfloat)(c2.y + c2.h));
//	SDL_FillRect(screen, &c1, SDL_MapRGB(screen->format, 0, 0, color1));
//	SDL_FillRect(screen, &c2, SDL_MapRGB(screen->format, color2, 0, color2)); 
}

void red::init()
{
	character::init();
	meter[3] = 528;
	meter[4] = 0;
}

move * red::createMove(char * fullName)
{
	char * token;
	char type[2] = {fullName[0], fullName[1]};
	char moveName[151];

	token = strtok(fullName, " \t-@?%$!\n");
	sprintf(moveName, "%s/%s", name, token);

	move * m;
	switch(type[0]){
	case '%':
		if(type[1] == 'j') m = new airSpecial(moveName);
		else m = new special(moveName);
		break;
	case '-':
		if(type[1] == 'j') m = new airUtility(moveName);
		else m = new utility(moveName);
		break;
	case '@':
		if(type[1] == 'j') m = new airLooping(moveName);
		else m = new looping(moveName);
		break;
	case '$':
		m = new redCancel(moveName);
		break;
	case 'j':
		m = new airMove(moveName);
		m->feed(neutral, 1);
		break;
	case '!':
//		if(type[1] == 'j') m = new airSuper(moveName); else
		m = new super(moveName);	
		break;
	case '?':
		m = new mash(moveName);
		break;
	default:
		m = new move(moveName);
		break;	
	}
	return m;
}

redCancel::redCancel(char* n) 
{ 
	build(n); 
	init();
}

bool redCancel::check(bool pos[5], bool neg[5], int t, int f, int* resource)
{
	for(int i = 0; i < 5; i++){
		if(button[i] == 1){
			if(!pos[i] && !neg[i]) return 0;
		}
	}
	if(t > tolerance) return 0;
	if(f > activation) return 0;
	if(resource[0] < cost) return 0;
	if(resource[3] < 264 || resource[4] > 0) return 0;
	return 1;
}

void redCancel::execute(move * last, int *& resource)
{
	resource[1] = 1;
	resource[2] = 1;
	resource[3] -= 264;
	resource[4] = 15;
	move::execute(last, resource);
}

redCancel::~redCancel() {}

