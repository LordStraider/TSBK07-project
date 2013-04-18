#ifndef __TGA_LOADER__
#define __TGA_LOADER__

#ifdef __APPLE__
	#include <OpenGL/gl.h>
	#include <OpenGL/glu.h>
#else
//	#include <GL/glee.h> Might be needed for Windows - not tested
	#include <GL/glew.h>
	#include <GL/glut.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Constants for SaveTGA
#define	TGA_ERROR_FILE_OPEN				-5
#define TGA_ERROR_READING_FILE			-4
#define TGA_ERROR_INDEXED_COLOR			-3
#define TGA_ERROR_MEMORY				-2
#define TGA_ERROR_COMPRESSED_FILE		-1
#define TGA_OK							 0

typedef struct					// Create A Structure for .tga loading.
{
	GLubyte	*imageData;			// Image Data (Up To 32 Bits)
	GLuint	bpp;				// Image Color Depth In Bits Per Pixel.
	GLuint	width;				// Image Width
	GLuint	height;				// Image Height
	GLuint	texID;				// Texture ID Used To Select A Texture
	GLfloat	texWidth, texHeight;
} TextureData, *TextureDataPtr;					// Structure Name

bool LoadTGATexture(char *filename, TextureData *texture);
void LoadTGATextureSimple(char *filename, GLuint *tex);

bool LoadTGATextureData(char *filename, TextureData *texture);	// Loads A TGA File Into Memory but doesn't create the texture.
void SaveTGA(TextureData *tex, char *filename);

#endif
