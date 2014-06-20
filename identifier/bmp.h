#ifndef BMP
#define BMP
#include "imatrix.h"
typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef unsigned int DWORD;
typedef long LONG;

typedef struct tagBITMAPFILEHEADER {
 char data[14];
} BITMAPFILEHEADER;

typedef struct rgbmt {
	imt_c* b;
	imt_c* g;
	imt_c* r;
} rgb_mt_t;

typedef struct tagBITMAPINFOHEADER{

  DWORD biSize;
  DWORD biWidth;
  DWORD biHeight;
  WORD biPlanes;
  WORD biBitCount;
  DWORD biCompression;
  DWORD biSizeImage;
  DWORD biXPelsPerMeter;
  DWORD biYPelsPerMeter;
  DWORD biClrUsed;
  DWORD biClrImportant;
} BITMAPINFOHEADER;



BITMAPFILEHEADER fh;
BITMAPINFOHEADER ih;

void bmpHeader(FILE *bmp, BITMAPFILEHEADER* fh, BITMAPINFOHEADER* ih);
void bmpData(FILE *bmp, unsigned int width,unsigned int height,char* buffer);


void bmpHeader(FILE *bmp, BITMAPFILEHEADER* fh, BITMAPINFOHEADER* ih)
{
	fread(fh,14,1,bmp);
	fseek(bmp,14,0);
 	fread(ih, sizeof(BITMAPINFOHEADER), 1, bmp);
}

void bmpData(FILE *bmp, unsigned int width,unsigned int height,char* buffer)
{
	fread(buffer, 3 * width * height, 1, bmp);
}

rgb_mt_t* imread(char* name)
{
	FILE* f = NULL;
	rgb_mt_t* image;
	DWORD offset;
	int i = 0;
	int ri = 0;
	int gi = 0;
	int bi = 0;
	char* buffer;
	unsigned int width = 0;
	unsigned int height = 0;
	unsigned int size = 0;
	image = (rgb_mt_t*)malloc(sizeof(rgb_mt_t));
	if(image == NULL)
	{
	   perror("can not use memory\n");
	   return NULL;
	}
	
	
	f = fopen(name,"r");
	if (f == NULL)
    {
       perror("can not open file\n");
	   return NULL;
    }
	bmpHeader(f,&fh,&ih);
	width = ih.biWidth;
	height = ih.biHeight;
	size = width * height * 3;
	offset = *((DWORD*)(fh.data + 10));
	
	fseek(f, offset, 0);
	buffer = m_malloc(size);
	if(image == NULL)
	{
	   perror("can not use memory\n");
	   free(image);
	   close(f);
	   return NULL;
	}
	image->r = create_zero_c(height,width);
	image->g = create_zero_c(height,width);
	image->b = create_zero_c(height,width);
    if(image->r && image->g && image->b == NULL)
	{
	   perror("can not use memory\n");
	   free(image);
	   m_free(buffer);
	   fclose(f);
	   clear(image->r);	   
	   clear(image->g);
	   clear(image->b);
	   return NULL;
	}
	bmpData(f,width,height,buffer);
	for(i = 0;i < size;i ++)
	{
		if(i%3 == 0)
		{
			image->b->data[(height - bi / width -1) * width + bi % width] = buffer[i];
			bi++;
		}
		else if(i%3 == 1)
		{
			image->g->data[(height - gi / width -1) * width + gi % width] = buffer[i];
			gi++;
		}
		else
		{
			image->r->data[(height - ri / width -1) * width + ri % width] = buffer[i];
			ri++;
		}
	}
	m_free(buffer);
	fclose(f);
	return image;
}

void imshow(char* name,rgb_mt_t* image)
{
	int i = 0;
	int ri = 0;
	int gi = 0;
	int bi = 0;
	char* buffer;
	unsigned int height;
	unsigned int width;
	unsigned int size;
	FILE* f = NULL;
	f = fopen(name,"w");
	buffer = (unsigned char*)m_malloc(image->r->row * image->r->col * 3);
	if (f == NULL)
    {
       perror("can not open file\n");
	}
	height = ih.biHeight;
	width = ih.biWidth;
	ih.biSizeImage = height * width * 3;
	
	for(i = 0;i < ih.biSizeImage;i ++)
	{
		if(i%3 == 0)
		{
			if(bi < ih.biSizeImage / 3)
			{
				buffer[i] = image->b->data[(height - bi / width - 1) * width + bi % width];
				bi++;
			}
		}
		else if(i%3 == 1)
		{
			if(gi < ih.biSizeImage / 3)
			{
				buffer[i] = image->g->data[(height - gi / width -1) * width + gi % width];
				gi++;
			}
		}
		else
		{
			if(ri < ih.biSizeImage / 3)
			{
				buffer[i] = image->r->data[(height - ri / width -1) * width + ri % width];
				ri++;
			}
		}
	}
	
	fseek(f,0,0);
	fwrite(&fh,14,1,f);
	fwrite(&ih,40,1,f);
	fseek(f, *((DWORD*)(fh.data + 10)), 0);
	fwrite(buffer,ih.biWidth * ih.biHeight * 3,1,f);
	m_free(buffer);
	fclose(f);
}

#endif
