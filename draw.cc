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
	glClear(GL_COLOR_BUFFER_BIT);

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glEnable( GL_TEXTURE_2D );
	glBindTexture(GL_TEXTURE_2D, background);
	glBegin(GL_QUADS);
		glTexCoord2i(0, 0);
		glVertex3f((GLfloat)(-bg.x)*scalingFactor, (GLfloat)(-bg.y)*scalingFactor, 0.f);

		glTexCoord2i(1, 0);
		glVertex3f((GLfloat)(bg.w - bg.x)*scalingFactor, (GLfloat)(-bg.y)*scalingFactor, 0.f);

		glTexCoord2i(1, 1);
		glVertex3f((GLfloat)(bg.w - bg.x)*scalingFactor, (GLfloat)(bg.h - bg.y)*scalingFactor, 0.f);

		glTexCoord2i(0, 1);
		glVertex3f((GLfloat)(-bg.x)*scalingFactor, (GLfloat)(bg.h - bg.y)*scalingFactor, 0.f);
	glEnd();

	glDisable( GL_TEXTURE_2D );
	for(int i = 0; i < 2; i++){
		p[i]->drawMeters(numRounds, scalingFactor);
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	}

	glEnable( GL_TEXTURE_2D );
	for(int i = 0; i < thingComplexity; i++){
		if(things[i]->spriteCheck()) 
			things[i]->draw(bg.x, bg.y, scalingFactor);
		glDisable(GL_TEXTURE_2D);
		if(!things[i]->spriteCheck())
			things[i]->drawBoxen(bg.x, bg.y, scalingFactor);
		if(i < 2)
			p[i]->drawHitParticle(bg.x, bg.y, scalingFactor);
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		glEnable( GL_TEXTURE_2D );
	}
	glDisable( GL_TEXTURE_2D );
	if(freeze > 0){
		glColor4f(0.0f, 0.0f, 0.0f, 0.4f);
		glRectf(0.0f, 0.0f, (GLfloat)screenWidth*scalingFactor, (GLfloat)screenHeight*scalingFactor);
		freeze--;
	}
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	SDL_GL_SwapBuffers();
}

void player::drawMeters(int n, float scalingFactor)
{
	SDL_Rect r[n];
	for(int i = 0; i < n; i++){
		r[i].y = 24; r[i].w = 20; r[i].h = 10;
		if(ID == 1) r[i].x = 680 - 24 * i; 
		else r[i].x = 900 + 24 * i;
	}
	for(int i = 0; i < n; i++){
		if(rounds > i) glColor4f(0.0f, 1.0f, 1.0f, 1.0f);
		else glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
		glRectf((GLfloat)(r[i].x)*scalingFactor, (GLfloat)(r[i].y)*scalingFactor, (GLfloat)(r[i].x + r[i].w)*scalingFactor, (GLfloat)(r[i].y + r[i].h)*scalingFactor);
	}
	glFlush();
	pick()->drawMeters(ID, scalingFactor);
	glFlush();
}

void character::drawMeters(int ID, float scalingFactor)
{
	SDL_Rect m;
	SDL_Rect h;
	if(health >= 0) h.w = health*2; else h.w = 1; 

	if(ID == 1) h.x = 100 + (600 - h.w); 
	else h.x = 900;
	h.h = 10;
	h.y = 10;

	int R = 0, G = 255, B = 0;
	if(meter[0] >= 0) m.w = meter[0]*2; else m.w = 1; 
	if(ID == 1) m.x = 100;
	else m.x = 1000 + (400 - m.w);
	m.h = 10; m.y = 860;

	if(m.w < 200) R = 191;
	else if(m.w < 400) B = 255;
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
	glRectf((GLfloat)(h.x)*scalingFactor, (GLfloat)(h.y)*scalingFactor, (GLfloat)(h.x + h.w)*scalingFactor, (GLfloat)(h.y + h.h)*scalingFactor);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glColor4f((float)R, (float)G, (float)B, 1.0f);
	glRectf((GLfloat)(m.x)*scalingFactor, (GLfloat)(m.y)*scalingFactor, (GLfloat)(m.x + m.w)*scalingFactor, (GLfloat)(m.y + m.h)*scalingFactor);
}

void instance::drawBoxen(int x, int y, float scalingFactor)
{
	glColor4f(1.0f, 1.0f, 1.0f, 0.5f);
	glRectf((GLfloat)(collision.x - x)*scalingFactor, (GLfloat)(collision.y - y)*scalingFactor, (GLfloat)(collision.x + collision.w - x)*scalingFactor, (GLfloat)(collision.y + collision.h - y)*scalingFactor);
	for(int i = 0; i < regComplexity; i++){
		glFlush();
		glColor4f(0.0f, 1.0f, (GLfloat)(ID - 1.0f)/2.0f, 0.5f);
		glRectf((GLfloat)(hitreg[i].x - x)*scalingFactor, (GLfloat)(hitreg[i].y - y)*scalingFactor, (GLfloat)(hitreg[i].x + hitreg[i].w - x)*scalingFactor, (GLfloat)(hitreg[i].y + hitreg[i].h - y)*scalingFactor);
	}
	for(int i = 0; i < hitComplexity; i++){
		glFlush();
		glColor4f(1.0f, 0.0f, (GLfloat)(ID - 1.0f)/2.0f, 0.5f);
		glRectf((GLfloat)(hitbox[i].x - x)*scalingFactor, (GLfloat)(hitbox[i].y - y)*scalingFactor, (GLfloat)(hitbox[i].x + hitbox[i].w - x)*scalingFactor, (GLfloat)(hitbox[i].y + hitbox[i].h - y)*scalingFactor);
	}
	glFlush();
	glDisable( GL_TEXTURE_2D );
}

void instance::draw(int x, int y, float scalingFactor)
{
	int realPosY = collision.y;
	int realPosX = posX;

	for(int i = 0; i < hitComplexity; i++){
		if(hitbox[i].y < realPosY) realPosY = hitbox[i].y;
		if(facing == 1){
			if(hitbox[i].x < realPosX) realPosX = hitbox[i].x;
		} else {
			if(hitbox[i].x + hitbox[i].w > realPosX) realPosX = hitbox[i].x + hitbox[i].w;
		}
	}
	for(int i = 0; i < regComplexity; i++){
		if(hitreg[i].y < realPosY) realPosY = hitreg[i].y;
		if(facing == 1){
			if(hitreg[i].x < realPosX) realPosX = hitreg[i].x;
		} else {
			if(hitreg[i].x + hitreg[i].w > realPosX) realPosX = hitreg[i].x + hitreg[i].w;
		}
	}
	if(secondInstance)
		glColor4f(0.75f, 0.5f, 0.85f, 1.0f);
	pick()->draw(cMove, facing, realPosX - x, realPosY - y, currentFrame, scalingFactor);
}

void player::drawHitParticle(int x, int y, float scalingFactor)
{
	/*Stand-in for now, just to indicate block type*/
	if(particleLife > 0){
		switch (particleType){
		case 1:
			glColor4f(1.0f, 0.0f, 0.0f, 0.7f);
			break;
		case 0:
			glColor4f(0.0f, 0.0f, 0.0f, 0.7f);
			break;
		case -1:
			glColor4f(1.0f, 1.0f, 1.0f, 0.7f);
			break;
		}
		glRectf((GLfloat)(posX - 10 * facing - x)*scalingFactor, (GLfloat)(posY - y)*scalingFactor, (GLfloat)(posX - 50*facing - x)*scalingFactor, (GLfloat)(posY + 40 - y)*scalingFactor);
		particleLife--;
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	}
}

void avatar::draw(action *& cMove, int facing, int x, int y, int f, float scalingFactor)
{
	cMove->draw(facing, x, y, f, scalingFactor);
}

void action::draw(int facing, int x, int y, int f, float scalingFactor)
{
	if(sprite[f]){
		glBindTexture(GL_TEXTURE_2D, sprite[f]);
		glBegin(GL_QUADS);
		if(facing == 1){
			glTexCoord2i(0, 0);
			glVertex3f((GLfloat)(x)*scalingFactor, (GLfloat)(y)*scalingFactor, 0.f);

			glTexCoord2i(1, 0);
			glVertex3f((GLfloat)(x + width[f]*2)*scalingFactor, (GLfloat)(y)*scalingFactor, 0.f);

			glTexCoord2i(1, 1);
			glVertex3f((GLfloat)(x + width[f]*2)*scalingFactor, (GLfloat)(y + height[f]*2)*scalingFactor, 0.f);

			glTexCoord2i(0, 1);
			glVertex3f((GLfloat)(x)*scalingFactor, (GLfloat)(y + height[f]*2)*scalingFactor, 0.f);
		} else {
			glTexCoord2i(0, 0);
			glVertex3f((GLfloat)(x)*scalingFactor, (GLfloat)(y)*scalingFactor, 0.f);

			glTexCoord2i(1, 0);
			glVertex3f((GLfloat)(x - width[f]*2)*scalingFactor, (GLfloat)(y)*scalingFactor, 0.f);

			glTexCoord2i(1, 1);
			glVertex3f((GLfloat)(x - width[f]*2)*scalingFactor, (GLfloat)(y + height[f]*2)*scalingFactor, 0.f);

			glTexCoord2i(0, 1);
			glVertex3f((GLfloat)(x)*scalingFactor, (GLfloat)(y + height[f]*2)*scalingFactor, 0.f);
		}
		glEnd();
	}
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
}

bool instance::spriteCheck()
{
	return pick()->spriteCheck(cMove, currentFrame);
}
bool avatar::spriteCheck(action *& cMove, int f)
{
	if(cMove == NULL) return 0;
	else return cMove->spriteCheck(f);
}

bool action::spriteCheck(int f)
{
	if(sprite[f]) return 1;
	else return 0;
}
