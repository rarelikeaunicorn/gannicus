/*Interface header for GUFG
 *The interface class/struct is intended as an abstraction between the player and the engine
 *This is where things pertaining to inputs should go, as well as sprsibly information like the
 *round timer.
 *
 *Written by Alex Kelly in 2012
 *Licensed under MIT OSI: Additional information in the file "COPYING"
 */

#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#include <stdio.h>
#include "analytics.h"
#ifndef INTERFACE
#define INTERFACE
class gameInstance{
public:
	void readInput();		/*Accepts input into input containers, for use by anything that wants it*/
	virtual void resolve() = 0;	/*Every game type requires a resolve loop.*/
	int drawGlyph(const char *, int, int, int, int, int);
	virtual void loadMisc();
	bool screenInit();
	bool screenInit(int, int);

//Input layer stuff. Players, inputs, etc.
	player * p[2];
	bool * sAxis[2];
	int * posEdge[2];
	bool * negEdge[2];

//Meta-interface stuff. Gameover state, screen size, etc.
	SDL_Surface *screen;
	int screenHeight, screenWidth, floor, wall;
	int freeze;
	bool gameover;
	bool fullscreen;
	float scalingFactor, sf;
	bool initd:1;
	GLuint glyph[91];
};

class interface : public gameInstance{
public:
	interface();
	~interface();
	void loadMisc();
	void createPlayers();
	void createDemons();
	void createDemons(replay*);
	void startGame();
	void loadMatchBackground();
/*Important interface functions that will remain interface functions*/

/*The main game loop runs readInput() and resolve() each exactly once per frame loop.*/
	void genInput();
	void resolve();		/*The main loop of the game*/
	void draw();		/*The primary function in charge of drawing things on the screen.*/
	void drawGame();
	void drawCSelect();
	void drawMainMenu(int);
	void drawRematchMenu();
	void cleanup();
	void runTimer();	/*Currently just a decrementer. May always just be a decrementer.*/
	void roundInit();
	void matchInit();
	void cSelectMenu();
	void mainMenu(int);
	void keyConfig();
	void rematchMenu();
	void checkWin();
	void dragBG(int);
	void doSuperFreeze();
	void unitCollision();
	void readMatchupChart();
	void writeMatchupChart();
	void initialConfig(int);
	void resolveHits();
	void resolveThrows();
	void resolveSummons();
	void addThing(instance*);
	void cullThing(int);
	void addAttractor(attractor*);
	void cullAttractor(int);
	void writeImage(const char*, int, action*);

	bool select[2];
	int selection[2];
	int menu[2];
	int configMenu[2];
	int rMenu;
	SDL_Rect bg;
	SDL_Rect prox;
	bool boxen:1;
	bool firstFrame:1;
	bool shortcut:1;
	bool continuous:1;
	bool single:1;
	bool analytics:1;
	int combo[2];
	int damage[2];
	bool illegit[2];
	int ** matchup;
	int numRounds;
	int grav;	//Gravitational constant. 
	instance ** things;
	int thingComplexity;

	GLuint background;
	Mix_Music *matchMusic;
	Mix_Music *menuMusic;
	Mix_Chunk **announceWinner;
	Mix_Chunk *announceRound[3];
	Mix_Chunk *announceDraw[2];
	Mix_Chunk *announceFight;
	Mix_Chunk *announceEnd[2];
	Mix_Chunk *announceSelect;

	attractor ** globals;
	int attractorComplexity;

//Variables for cSelectMenu 
	int numChars;
	GLuint selectScreen; 
	SDL_Rect wheel;
	int counter[2];

	bool q;
	int timer;
	bool roundEnd:1;
	int endTimer;
	replay * currentMatch;
};
#endif