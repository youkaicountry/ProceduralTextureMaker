#include "ptexture.h"
#include <stdlib.h>
#include <memory.h>
#include <math.h>

PTexture::PTexture()
{
   this->xdim = 100;
   this->ydim = 100;
   init();
   return;
}

PTexture::PTexture(int xsize, int ysize)
{
   this->xdim = xsize;
   this->ydim = ysize;
   init();
   return;
}

void PTexture::init()
{
   int area = xdim*ydim;
   imager = (float *) malloc(area*sizeof(float));
   imageg = (float *) malloc(area*sizeof(float));
   imageb = (float *) malloc(area*sizeof(float));
   if (imager == NULL)
   {
      printf("ERROR:  Could not allocate texture memory\n");
      exit(1);
   }
   // Default black is the opaque color
   for(int i=0;i<area;i++)
   {
      imager[i] = 0.0;
      imageg[i] = 0.0;
      imageb[i] = 0.0;
   }
   return;
}

PTexture::~PTexture()
{
   free(imager);
   free(imageg);
   free(imageb);
   return;
}

int PTexture::getXSize()
{
   return this->xdim;
}

int PTexture::getYSize()
{
   return this->ydim;
}

float PTexture::getHeight(int x, int y)
{
   int c = this->flatCoord(x, y);
   return ((imager[c] + imageg[c] + imageb[c])/3.0)*255.0;
}

float PTexture::getHeightD(int x, int y)
{
   int c = this->flatCoord(x, y);
   return ((imager[c] + imageg[c] + imageb[c])/3.0)*255.0;
}

void PTexture::setA8R8G8B8(int x, int y, int argb)
{
   int c = this->flatCoord(x, y);
   int r = argb >> 16 & 0xFF;
   int g = argb >> 8  & 0xFF;
   int b = argb       & 0xFF;
   imager[c] = ((float)r) / 255.0;
   imageg[c] = ((float)g) / 255.0;
   imageb[c] = ((float)b) / 255.0;
   return;
}

void PTexture::setColour(int x, int y, int red, int green, int blue)
{
   if (red > 255) {red = 255;}
   if (red < 0) {red = 0;}
   if (green > 255) {green = 255;}
   if (green < 0) {green = 0;}
   if (blue > 255) {blue = 255;}
   if (blue < 0) {blue = 0;}
   // You need an operator precedence table to tell, but it's right :-)
   //int argb = ((0xFF00 | red & 0xFF) << 8 | green & 0xFF) << 8 | blue & 0xFF;
   //image[this->flatCoord(x, y)] = argb;
   int c = this->flatCoord(x, y);
   imager[c] = ((float)red) / 255.0;
   imageg[c] = ((float)green) / 255.0;
   imageb[c] = ((float)blue) / 255.0;
   return;
}

void PTexture::setColourD(int x, int y, float red, float green, float blue)
{
   if (red > 255) {red = 255;}
   if (red < 0) {red = 0;}
   if (green > 255) {green = 255;}
   if (green < 0) {green = 0;}
   if (blue > 255) {blue = 255;}
   if (blue < 0) {blue = 0;}
   // You need an operator precedence table to tell, but it's right :-)
   //int argb = ((0xFF00 | red & 0xFF) << 8 | green & 0xFF) << 8 | blue & 0xFF;
   //image[this->flatCoord(x, y)] = argb;
   int c = this->flatCoord(x, y);
   imager[c] = red / 255.0;
   imageg[c] = green / 255.0;
   imageb[c] = blue / 255.0;
   return;
}

void PTexture::setColourWrap(int x, int y, int red, int green, int blue)
{
   if (red < 0) {red = abs(red)+255;}
   if (green < 0) {green = abs(green)+255;}
   if (blue < 0) {blue = abs(blue)+255;}
   red = red % 255;
   green = green % 255;
   blue = blue % 255;
   // You need an operator precedence table to tell, but it's right :-)
   //int argb = ((0xFF00 | red & 0xFF) << 8 | green & 0xFF) << 8 | blue & 0xFF;
   //image[this->flatCoord(x, y)] = argb;
   int c = this->flatCoord(x, y);
   imager[c] = ((float)red) / 255.0;
   imageg[c] = ((float)green) / 255.0;
   imageb[c] = ((float)blue) / 255.0;
   return;
}

void PTexture::setColourWrapD(int x, int y, float red, float green, float blue)
{
   if (red < 0) {red = fabs(red)+255.0;}
   if (green < 0) {green = fabs(green)+255.0;}
   if (blue < 0) {blue = fabs(blue)+255.0;}
   red = (float)((int)red % (int)255);
   green = (float)((int)green % (int)255);
   blue = (float)((int)blue % (int)255);
   // You need an operator precedence table to tell, but it's right :-)
   //int argb = ((0xFF00 | red & 0xFF) << 8 | green & 0xFF) << 8 | blue & 0xFF;
   //image[this->flatCoord(x, y)] = argb;
   int c = this->flatCoord(x, y);
   imager[c] = red / 255.0;
   imageg[c] = green / 255.0;
   imageb[c] = blue / 255.0;
   return;
}

int PTexture::getRed(int x, int y)
{
   return (int)(imager[this->flatCoord(x, y)]*255.0);
}

int PTexture::getGreen(int x, int y)
{
   return (int)(imageg[this->flatCoord(x, y)]*255.0);
}

int PTexture::getBlue(int x, int y)
{
   return (int)(imageb[this->flatCoord(x, y)]*255.0);
}

float PTexture::getRedD(int x, int y)
{
   return (imager[this->flatCoord(x, y)]*255.0);
}

float PTexture::getGreenD(int x, int y)
{
   return (imageg[this->flatCoord(x, y)]*255.0);
}

float PTexture::getBlueD(int x, int y)
{
   return (imageb[this->flatCoord(x, y)]*255.0);
}

int PTexture::getExtendRed(int x, int y)
{
   if (x < 0) {x = 0;}
   if (x >= this->xdim) {x = this->xdim-1;}
   if (y < 0) {y = 0;}
   if (y >= this->ydim) {y = this->ydim-1;}
   return (int)(imager[this->flatCoord(x, y)]*255.0);
}

int PTexture::getExtendGreen(int x, int y)
{
   if (x < 0) {x = 0;}
   if (x >= this->xdim) {x = this->xdim-1;}
   if (y < 0) {y = 0;}
   if (y >= this->ydim) {y = this->ydim-1;}
   return (int)(imageg[this->flatCoord(x, y)]*255.0);
}

int PTexture::getExtendBlue(int x, int y)
{
   if (x < 0) {x = 0;}
   if (x >= this->xdim) {x = this->xdim-1;}
   if (y < 0) {y = 0;}
   if (y >= this->ydim) {y = this->ydim-1;}
   return (int)(imageb[this->flatCoord(x, y)]*255.0);
}

float PTexture::getExtendRedD(int x, int y)
{
   if (x < 0) {x = 0;}
   if (x >= this->xdim) {x = this->xdim-1;}
   if (y < 0) {y = 0;}
   if (y >= this->ydim) {y = this->ydim-1;}
   return (imager[this->flatCoord(x, y)]*255.0);
}

float PTexture::getExtendGreenD(int x, int y)
{
   if (x < 0) {x = 0;}
   if (x >= this->xdim) {x = this->xdim-1;}
   if (y < 0) {y = 0;}
   if (y >= this->ydim) {y = this->ydim-1;}
   return (imageg[this->flatCoord(x, y)]*255.0);
}

float PTexture::getExtendBlueD(int x, int y)
{
   if (x < 0) {x = 0;}
   if (x >= this->xdim) {x = this->xdim-1;}
   if (y < 0) {y = 0;}
   if (y >= this->ydim) {y = this->ydim-1;}
   return (imageb[this->flatCoord(x, y)]*255.0);
}


int PTexture::getA8R8G8B8(int x, int y)
{
   int r = (int)(imager[this->flatCoord(x, y)]*255.0);
   int g = (int)(imageg[this->flatCoord(x, y)]*255.0);
   int b = (int)(imageb[this->flatCoord(x, y)]*255.0);
   return ((0xFF00 | r & 0xFF) << 8 | g & 0xFF) << 8 | b & 0xFF;
}

void PTexture::copyFromTexture(PTexture *src)
{
   int ax = this->getXSize(), ay = this->getYSize();
   int bx = src->getXSize(), by = src->getYSize();
   if ((ax != bx) || (ay != by))    // TODO:  Handle this case better
      return;
   memcpy(this->imager, src->imager, ax*ay*sizeof(float));
   memcpy(this->imageg, src->imageg, ax*ay*sizeof(float));
   memcpy(this->imageb, src->imageb, ax*ay*sizeof(float));
   return;
}

void PTexture::getA8R8G8B8row(int y, int *buffer)
{
   int w = this->getXSize();
   for(int i = 0;i<w;i++)
   {
      buffer[i] = this->getA8R8G8B8(i, y);
   }
   return;
}


/*PColour::PColour()
{
   this->red = 0;
   this->green = 0;
   this->blue = 0;
   return;
}

PColour::PColour(int nred, int ngreen, int nblue)
{
   this->setColour(nred, ngreen, nblue);
   return;
}

PColour::~PColour()
{
   return;
}

void PColour::setRed(int nred)
{
   this->red = nred;
   return;
}

void PColour::setGreen(int ngreen)
{
   this->green = ngreen;
   return;
}

void PColour::setBlue(int nblue)
{
   this->blue = nblue;
   return;
}

int PColour::getRed()
{
   return this->red;
}

int PColour::getGreen()
{
   return this->green;
}

int PColour::getBlue()
{
   return this->blue;
}

void PColour::setColour(int nred, int ngreen, int nblue)
{
   this->red = nred;
   this->green = ngreen;
   this->blue = nblue;
   return;
}

PTexture::PTexture()
{
   this->xdim = 100;
   this->ydim = 100;
   //this->incSize(this->xdim, this->ydim);
   for (int i = 0;i < (this->xdim*this->ydim);i++)
   {
      image.push_back(new PColour());
   }
   return;
}

PTexture::PTexture(int xsize, int ysize)
{
   this->xdim = xsize;
   this->ydim = ysize;
   for (int i = 0;i < (this->xdim*this->ydim);i++)
   {
      image.push_back(new PColour());
   }
   return;
}

PTexture::~PTexture()
{
   for (int i = 0;i < image.size();i++)
   {
      delete image[i];
   }
   return;
}

int PTexture::getXSize()
{
   return this->xdim;
}

int PTexture::getYSize()
{
   return this->ydim;
}


PColour* PTexture::getColour(int x, int y)
{
   return image[this->flatCoord(x, y)];
}

float PTexture::getHeight(int x, int y)
{
   int ta = this->flatCoord(x, y);
   return (image[ta]->getRed() + image[ta]->getGreen() + image[ta]->getBlue()) / 3;
}

void PTexture::setColour(int x, int y, PColour col)
{
   int ta = this->flatCoord(x, y);
   delete image[ta];
   image[ta] = new PColour();
   *image[ta] = col;
   //image[ta] = col; //mem leak? probably not...
   return;
}
   
void PTexture::setColour(int x, int y, int red, int green, int blue)
{
   int ta = this->flatCoord(x, y);
   image[ta]->setRed(red);
   image[ta]->setGreen(green);
   image[ta]->setBlue(blue);
   return;
}

int PTexture::flatCoord(int x, int y)
{
   int tindex = (y * this->xdim) + x;
   if (tindex > (this->xdim * this->ydim)-1) { tindex = (this->xdim * this->ydim)-1;}
   if (tindex < 0) {tindex = 0;}
   return tindex;
}

int PTexture::getRed(int x, int y)
{
   return image[this->flatCoord(x, y)]->getRed();
}

int PTexture::getGreen(int x, int y)
{
   return image[this->flatCoord(x, y)]->getGreen();
}

int PTexture::getBlue(int x, int y)
{
   return image[this->flatCoord(x, y)]->getBlue();
}

int PTexture::getA8R8G8B8(int x, int y)
{
   unsigned char a = 0xff;
   unsigned char r = image[this->flatCoord(x, y)]->getRed();
   unsigned char g = image[this->flatCoord(x, y)]->getGreen();
   unsigned char b = image[this->flatCoord(x, y)]->getBlue();
   
   return (a << 24) + (r << 16) + (g << 8) + b;  
}*/
