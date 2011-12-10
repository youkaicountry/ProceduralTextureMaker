#ifndef __PTEXTURE_
#define __PTEXTURE_

#include <vector.h>

class PTexture
{
   public:
   PTexture();
   PTexture(int xsize, int ysize);
   ~PTexture();

   int getXSize();
   int getYSize();

   float getHeight(int x, int y);
   float getHeightD(int x, int y);
   int getRed(int x, int y);
   int getGreen(int x, int y);
   int getBlue(int x, int y);
   float getRedD(int x, int y);
   float getGreenD(int x, int y);
   float getBlueD(int x, int y);
   int getExtendRed(int x, int y);
   int getExtendGreen(int x, int y);
   int getExtendBlue(int x, int y);
   float getExtendRedD(int x, int y);
   float getExtendGreenD(int x, int y);
   float getExtendBlueD(int x, int y);
   int getA8R8G8B8(int x, int y);
   void getA8R8G8B8row(int y, int *buffer);


   void setColourD(int x, int y, float red, float green, float blue); //0-255.0
   void setColour(int x, int y, int red, int green, int blue); //sets colour and clamps
   void setColourWrap(int x, int y, int red, int green, int blue); //sets colour and wraps
   void setColourWrapD(int x, int y, float red, float green, float blue);
   void setA8R8G8B8(int x, int y, int argb);
   void copyFromTexture(PTexture *src);  //TODO:  Make this function cleaner
   
  float *imager;
  float *imageg;
  float *imageb;

   int xdim;
   int ydim;

   private:
   
   
   
   int flatCoord(int x, int y)
   {     
   //if (tindex > (this->xdim * this->ydim)-1) { tindex = (this->xdim * this->ydim)-1;}
   //if (tindex < 0) {tindex = 0;}
   if (x < 0) {x = 0;}
   if (y < 0) {y = 0;}
   if (x >= this->xdim) {x = this->xdim-1;}
   if (y >= this->ydim) {y = this->ydim-1;}
   int tindex = (y * this->xdim) + x;
   return tindex;  }

   void init();
};


/*class PColour
{
   public:
   PColour();
   PColour(int nred, int ngreen, int nblue);
   
   ~PColour();
   
   int red;
   int green;
   int blue;
   
   void setRed(int nred);
   void setGreen(int ngreen);
   void setBlue(int nblue);
   
   int getRed();
   int getGreen();
   int getBlue();
   
   void setColour(int nred, int ngreen, int nblue);

};

class PTexture
{
   public:
   PTexture();
   PTexture(int xsize, int ysize);
   ~PTexture();
   
   int getXSize();
   int getYSize();
   
   PColour* getColour(int x, int y);
   float getHeight(int x, int y);
   int getRed(int x, int y);
   int getGreen(int x, int y);
   int getBlue(int x, int y);
   int getA8R8G8B8(int x, int y);
   
   void setColour(int x, int y, PColour col);
   void setColour(int x, int y, int red, int green, int blue);
   
   private:
   vector<PColour*> image;
   int xdim;
   int ydim;
   
   int flatCoord(int x, int y);
};*/

#endif
