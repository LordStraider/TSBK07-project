// TGA loader, by Ingemar 2009, based on "tga.c" and some other sources.

#include "LoadTGA2.h"

bool LoadTGATextureData(char *filename, TextureData *texture)	// Loads A TGA File Into Memory
{
	GLuint i;
	GLubyte
		TGAuncompressedheader[12]={ 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0},	// Uncompressed TGA Header
		TGAcompressedheader[12]={ 0, 0, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0},	// Uncompressed TGA Header
		actualHeader[12],	// Used To Compare TGA Header
		header[6];		// First 6 Useful Bytes From The Header
	GLuint bytesPerPixel,		// Holds Number Of Bytes Per Pixel Used In The TGA File
		imageSize,		// Used To Store The Image Size When Setting Aside Ram
		temp;			// Temporary Variable
	long w, h;
	long rowSize, stepSize, bytesRead;
	GLubyte *rowP;
	int err;
	GLubyte rle;
	int b;
	long row, rowLimit;
	GLubyte pixelData[4];
	
	FILE *file = fopen(filename, "rb");			// Open The TGA File
	err = 0;
	if (file == NULL) err = 1;				// Does File Even Exist?
	else if (fread(actualHeader, 1, sizeof(actualHeader), file) != sizeof(actualHeader)) err = 2; // Are There 12 Bytes To Read?
	else if (
				(memcmp(TGAuncompressedheader, actualHeader, sizeof(TGAuncompressedheader)) != 0) &&
				(memcmp(TGAcompressedheader, actualHeader, sizeof(TGAcompressedheader)) != 0)
			)
				err = 3; // Does The Header Match What We Want?
	else if (fread(header, 1, sizeof(header), file) != sizeof(header)) err = 4; // If So Read Next 6 Header Bytes
	
	if (err != 0)
	{
		switch (err)
		{
			case 1: printf("could not open file %s\n", filename); return false;break;
			case 2: printf("could not read header of %s\n", filename); return false;break;
			case 3: printf("unsupported format in %s\n", filename); return false;break;
			case 4: printf("could not read file %s\n", filename); return false;break;
		}
		
		if(file == NULL)		// Did The File Even Exist? *Added Jim Strong*
			return false;
		else
		{
			fclose(file);		// If Anything Failed, Close The File
			return false;
		}
	}
	texture->width  = header[1] * 256 + header[0];	// Determine The TGA Width (highbyte*256+lowbyte)
	texture->height = header[3] * 256 + header[2];	// Determine The TGA Height (highbyte*256+lowbyte)
	if (texture->width <= 0 ||	// Is The Width Less Than Or Equal To Zero
	texture->height <= 0 ||		// Is The Height Less Than Or Equal To Zero
	(header[4] != 24 && header[4] != 32))			// Is The TGA 24 or 32 Bit?
	{
		fclose(file);		// If Anything Failed, Close The File
		return false;
	}
	
	w = 1;
	while (w < texture->width) w = w << 1;
	h = 1;
	while (h < texture->height) h = h << 1;
	texture->texWidth = (GLfloat)texture->width / w;
	texture->texHeight = (GLfloat)texture->height / h;
	
	texture->bpp = header[4];		// Grab The TGA's Bits Per Pixel (24 or 32)
	bytesPerPixel = texture->bpp/8;		// Divide By 8 To Get The Bytes Per Pixel
	imageSize = w * h * bytesPerPixel;	// Calculate The Memory Required For The TGA Data
	rowSize	= texture->width * bytesPerPixel;	// Image memory per row
	stepSize = w * bytesPerPixel;		// Memory per row
	texture->imageData = (GLubyte *)calloc(1, imageSize);
	if (texture->imageData == NULL)				// Does The Storage Memory Exist?
	{
		fclose(file);
		return false;
	}

	if (actualHeader[2] == 2) // uncompressed
	{
		rowP = &texture->imageData[0];
		for (i = 0; i < texture->height; i++)
		{
			bytesRead = fread(rowP, 1, rowSize, file);
			rowP += stepSize;
			if (bytesRead != rowSize)
			{
				free(texture->imageData);	// If So, Release The Image Data
				fclose(file);			// Close The File
				return false;			// Return False
			}
		}
	}
	else
	{ // compressed
		row = 0 + (texture->height -1) * stepSize;
		i = row;
		rowLimit = row + rowSize;
		do
		{
			bytesRead = fread(&rle, 1, 1, file);
			if (rle < 128)
			{ // rle+1 raw pixels
				bytesRead = fread(&texture->imageData[i], 1, (rle+1)*bytesPerPixel, file);
				i += bytesRead;
				if (bytesRead == 0)
					i = imageSize;
			}
			else
			{ // range of rle-127 pixels with a color that follows
				bytesRead = fread(&pixelData, 1, bytesPerPixel, file);
				do
				{
					for (b = 0; b < bytesPerPixel; b++)
						texture->imageData[i+b] = pixelData[b];
					i += bytesPerPixel;
					rle = rle - 1;
				} while (rle > 127);
			}
			if (i >= rowLimit)
			{
				row = row - stepSize;
				rowLimit = row + rowSize;
				i = row;
			}
		} while (i < imageSize);
	}

	for (i = 0; i < (int)(imageSize); i += bytesPerPixel)	// Loop Through The Image Data
	{		// Swaps The 1st And 3rd Bytes ('R'ed and 'B'lue)
		temp = texture->imageData[i];		// Temporarily Store The Value At Image Data 'i'
		texture->imageData[i] = texture->imageData[i + 2];	// Set The 1st Byte To The Value Of The 3rd Byte
		texture->imageData[i + 2] = temp;	// Set The 3rd Byte To The Value In 'temp' (1st Byte Value)
	}
	fclose (file);
	return true;				// Texture Building Went Ok, Return True
}


bool LoadTGATexture(char *filename, TextureData *texture)	// Loads A TGA File Into Memory and uploads to VRAM
{
	GLuint type = GL_RGBA;		// Set The Default GL Mode To RBGA (32 BPP)
	int h, w;
	bool result = LoadTGATextureData(filename, texture); // Loads A TGA File Into Memory
	if (!result)
		return result;
	


	w = 1;
	while (w < texture->width) w = w << 1;
	h = 1;
	while (h < texture->height) h = h << 1;
	
	// Build A Texture From The Data
	glGenTextures(1, &texture[0].texID);			// Generate OpenGL texture IDs
	glBindTexture(GL_TEXTURE_2D, texture[0].texID);		// Bind Our Texture
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	// Linear Filtered
//	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);	// Linear Filtered
	if (texture[0].bpp == 24)						// Was The TGA 24 Bits?
	{
		type=GL_RGB;			// If So Set The 'type' To GL_RGB
	}
//	gluBuild2DMipmaps(GL_TEXTURE_2D, type, w, h, type, GL_UNSIGNED_BYTE, texture[0].imageData);
	glTexImage2D(GL_TEXTURE_2D, 0, type, w, h, 0, type, GL_UNSIGNED_BYTE, texture[0].imageData);
	
	return result;
}

void LoadTGATextureSimple(char *filename, GLuint *tex) // If you really only need the texture object.
{
	TextureData texture;
	
	if (LoadTGATexture(filename, &texture))
	{
		if(texture.imageData != NULL)
			free(texture.imageData);
		*tex = texture.texID;
	}
	else
		*tex = 0;
}


// saves an array of pixels as a TGA image
int tgaSave(char			*filename, 
			 short int		width, 
			 short int		height, 
			 unsigned char	pixelDepth,
			 unsigned char	*imageData)
{
	unsigned char cGarbage = 0, type,mode,aux;
	short int iGarbage = 0;
	int i, w, bytesPerPixel, row, ix;
	FILE *file;
	char /*GLubyte*/ TGAuncompressedheader[12]={ 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0};	// Uncompressed TGA Header

// open file and check for errors
	file = fopen(filename, "w");
	if (file == NULL)
		return(TGA_ERROR_FILE_OPEN);

// compute image type: 2 for RGB(A), 3 for greyscale
	mode = pixelDepth / 8;
	if ((pixelDepth == 24) || (pixelDepth == 32))
		type = 2;
	else
		type = 3;

// write the header
	fwrite(&TGAuncompressedheader, 12, 1, file);
	fwrite(&width, sizeof(short int), 1, file);
	fwrite(&height, sizeof(short int), 1, file);
	fwrite(&pixelDepth, sizeof(unsigned char), 1, file);

	fwrite(&cGarbage, sizeof(unsigned char), 1, file);

// convert the image data from RGB(a) to BGR(A)
	if (mode >= 3)
	for (i=0; i < width * height * mode ; i+= mode)
	{
		aux = imageData[i];
		imageData[i] = imageData[i+2];
		imageData[i+2] = aux;
	}

// save the image data
	w = 1;
	while (w < width) w = w << 1;
	bytesPerPixel = pixelDepth/8;	
	row = width * bytesPerPixel;
	
// Write one row at a time
	for (i = 0; i < height; i++)
	{
		ix = i * w * mode;
		fwrite(&imageData[ix], sizeof(unsigned char), width * mode, file);
	}

	fclose(file);
// release the memory
	free(imageData);

	return(TGA_OK);
}

void SaveTGA(TextureData *tex, char *filename)
{
	tgaSave(filename, tex->width, tex->height, 
			tex->bpp, tex->imageData);
}
