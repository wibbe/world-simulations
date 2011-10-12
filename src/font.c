
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <memory.h>

#include "game.h"

/* Include data for the fonts */
#include "font_helvetica_17_def.h"
#include "font_helvetica_17_img.h"
#include "font_monaco_12_def.h"
#include "font_monaco_12_img.h"

typedef struct Font
{
	unsigned char  char_width[256];
	unsigned char  char_height;
	unsigned int   list;
	unsigned int	texture;
} Font;

static Font _fonts[FONT_COUNT];

static void font_load(int kind, int width, int height, unsigned char *font_img, int font_def[256][4])
{
	Font *font = &_fonts[kind];
	int i;

	font->char_height = font_def[80][3];
	
	/* Copy char width */
	for (i = 0; i < 256; ++i)
		font->char_width[i] = font_def[i][2];

	/* Build font data */
	font->list = glGenLists(256);
	
	for (i = 0; i < 256; ++i)
	{
		float x = (float)font_def[i][0] / (float)width;
		float y = (float)font_def[i][1] / (float)height;
		float w = (float)font_def[i][2] / (float)width;
		float h = (float)font_def[i][3] / (float)height;
		
		/* Special case if it is a space */
		if ((char)i == ' ')
		{
			glNewList(font->list + i, GL_COMPILE);
			glTranslated(font->char_width['I'], 0, 0);
			glEndList();
		}
		else
		{
			glNewList(font->list + i, GL_COMPILE);
			glBegin(GL_QUADS);
	
			glTexCoord2f(x, y + h);
			glVertex2i(0, font->char_height);
	
			glTexCoord2f(x + w, y + h);
			glVertex2i(font->char_width[i], font->char_height);
	
			glTexCoord2f(x + w, y);
			glVertex2i(font->char_width[i], 0);
	
			glTexCoord2f(x, y);
			glVertex2i(0, 0);
	
			glEnd();		
			glTranslated(font->char_width[i], 0, 0);
			glEndList();
		}
	}
	
	/* Create texture */
	glGenTextures(1, &font->texture);
	glBindTexture(GL_TEXTURE_2D, font->texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, width, height, 0, GL_ALPHA, 
	             GL_UNSIGNED_BYTE, font_img);
	glBindTexture(GL_TEXTURE_2D, 0);
}


void font_initialize()
{	
	/* Load the different fonts */
	font_load(FONT_HELVETICA_17, helvetica_17_width, helvetica_17_height, helvetica_17_img, helvetica_17_def);
	font_load(FONT_MONACO_12, monaco_12_width, monaco_12_height, monaco_12_img, monaco_12_def);
}

void font_print(int type, int x, int y, char* msg, ...)
{
	assert(type >= 0 && type < FONT_COUNT);
	
	Font *font = &_fonts[type];
	char buff[512];
 	va_list args;

	va_start(args, msg);
	vsprintf(buff, msg, args);
	va_end(args);
	
	glBindTexture(GL_TEXTURE_2D, font->texture);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glTranslated(x, y, 0);
	
	glListBase(font->list);
	glCallLists(strlen(buff), GL_UNSIGNED_BYTE, buff);

	glPopMatrix();
	glBindTexture(GL_TEXTURE_2D, 0);
}

int font_get_height(int font)
{
	assert(font >= 0 && font < FONT_COUNT);
	return _fonts[font].char_height;
}

int font_get_width(int type, char* msg, ...)
{
	assert(type >= 0 && type < FONT_COUNT);
	Font *font = &_fonts[type];
	
	char buff[512];
 	va_list args;

	va_start(args, msg);
	vsprintf(buff, msg, args);
	va_end(args);
		
	int i, width, len = strlen(buff);
	
	width = 0;	
	for (i = 0; i < len; ++i)
		width += font->char_width[((unsigned char)buff[i])];
 
  	return width;			
}

#if 0
void font_print(Font *font, int x, int y, char* msg, ...)
{
	char buff[512];
 	va_list args;

	va_start(args, msg);
	vsprintf(buff, msg, args);
	va_end(args);
	
	glBindTexture(GL_TEXTURE_2D, font->texture);

	glPushMatrix();
	glLoadIdentity();
	glTranslated(x, y, 0);
	
	glListBase(font->gl_list);
	glCallLists(strlen(buff), GL_UNSIGNED_BYTE, buff); // GL_BYTE

	glPopMatrix();		
}
#endif
