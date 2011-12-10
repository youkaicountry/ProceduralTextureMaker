#ifndef __TEXTURECONTAINER_
#define __TEXTURECONTAINER_

#include <vector.h>
#include "ptexture.h"
#include "lua.hpp"

class TextureContainer
{
   public:
   TextureContainer();
   ~TextureContainer();
   
   //reading operations
   int getWorkingA8R8G8B8(int num, int x, int y);
   int getFinalA8R8G8B8(int num, int x, int y);
   
   
   int getFinalNumber(); //returns number of elements in Final
   int getWorkingNumber();
   
   int getWorkingXSize(int num);
   int getFinalXSize(int num);
   
   int getWorkingYSize(int num);
   int getFinalYSize(int num);
   
   int getWorkingTextureMemSize(int num);
   int getFinalTextureMemSize(int num);
   int getTextureContainerMemSize();
   
   //utility operations
   int addNewWorkingTexture(int xsize, int ysize);
   int makeFinal(int num);
   void copyTexture(int num1, int num2);
   
   void clearTextureContainer();
   
   //loading operations
   void loadLua(char* filename);
   void loadLuaLib(const char *dir, lua_State *L);
   
   //edit operations
   void setPixel(int num, int x, int y, int red, int green, int blue);
   void clearTexture(int num, int red, int green, int blue);
   void setHStrip(int num, int xstart, int length, int y, int red, int green, int blue);
   void setVStrip(int num, int ystart, int length, int x, int red, int green, int blue);
   void applyTransform(int num, float m00, float m01, float m02, float m10, float m11, float m12, int red, int green, int blue);
   void applyScale(int num, float xscale, float yscale, int red, int green, int blue);
   void applyRotationAbout(int num, float theta, float ox, float oy, int red, int green, int blue);
   void makeHeightMap(int num);
   void tint(int num, int red, int green, int blue);
   void quantize(int num, int redvalues, int greenvalues, int bluevalues);
   void fill(int num, int x, int y, int red, int green, int blue);
   void fill2(int num, int x, int y, int red, int green, int blue);
   void filledCircle(int num, int x, int y, int radius, int red, int green, int blue);
   void filledRect(int num, int x1, int y1, int x2, int y2, int red, int green, int blue);
   void apply3Convolution(int num, float* matrix, float divide, float bias, int passes, bool red, bool green, bool blue);
   void noise(int num, int seed, int lored, int logreen, int loblue, int hired, int higreen, int hiblue);
   void fractionalNoise(int num, int seed, int lored, int logreen, int loblue, int hired, int higreen, int hiblue);
   void setWithProbability(int num, int seed, float p, int red, int green, int blue);
   void closestColour(int num, float var, int red, int green, int blue);
   void perlinNoise(int num, float px, float py, float length, float persist, int octaves, int seed);
   void light(int num, int bump, float bumpheight, float lightx, float lighty, float lightz, float ambientbright, float lightbright, float falloffkmult, float falloff, bool shadows); 
   void add(int num1, int num2, int type);
   void line(int num, int x0, int y0, int x1, int y1, int red, int green, int blue);
   void rect(int num, int x1, int y1, int x2, int y2, int red, int green, int blue);
   void circle(int num, int x0, int y0, int radius, int red, int green, int blue);
   void colourReplace(int num, float var, int redrep, int greenrep, int bluerep, int red, int green, int blue);
   void overlay(int num1, int num2, float var, int tred, int tgreen, int tblue);
   //void pixelFit(int num, float probmult, int seedstart, int seedend, int red, int green, int blue);
   void directionLight(int num, int bump, float bumpheight, float lightx, float lighty, float lightz, float ambientbright, float lightbright, bool shadows); 
   void clampBelow(int num, float belowvalue, float tored, float togreen, float toblue);
   void clampAbove(int num, float abovevalue, float tored, float togreen, float toblue);
   void brightnessMult(int num, float scale);
   float getRed(int num, int x, int y);
   float getGreen(int num, int x, int y);
   float getBlue(int num, int x, int y);
   void makeNormalMap(int num, float bumpheight);
   void lightNormal(int num, int normal, float lightx, float lighty, float lightz, float ambientbright, float lightbright, float falloffkmult, float falloff, bool shadows);
   void directionLightNormal(int num, int normal, float lx, float ly, float lz, float ambientbright, float lightbright, bool shadows);

   //exposed lua functions
   static int l_setPixel( lua_State* luaVM );
   static int l_clearTexture( lua_State* luaVM );
   static int l_newTexture( lua_State* luaVM );
   static int l_makeFinal( lua_State* luaVM );
   static int l_setHStrip( lua_State* luaVM );
   static int l_setVStrip( lua_State* luaVM );
   static int l_applyTransform( lua_State* luaVM );
   static int l_applyScale( lua_State* luaVM );
   static int l_applyRotationAbout( lua_State* luaVM );
   static int l_makeHeightMap( lua_State* luaVM );
   static int l_tint( lua_State* luaVM );
   static int l_quantize( lua_State* luaVM );
   static int l_copyTexture( lua_State* luaVM );
   static int l_fill( lua_State* luaVM );
   static int l_filledCircle( lua_State* luaVM );
   static int l_filledRect( lua_State* luaVM );
   static int l_applyBlur( lua_State* luaVM );
   static int l_applyDefocus( lua_State* luaVM );
   static int l_applyEdgeDetect( lua_State* luaVM );
   static int l_applyGaussianBlur( lua_State* luaVM );
   static int l_noise( lua_State* luaVM );
   static int l_fractionalNoise( lua_State* luaVM );
   static int l_setWithProbability(lua_State *luaVM);
   static int l_closestColour(lua_State *luaVM);
   static int l_perlinNoise(lua_State *luaVM);
   static int l_light(lua_State *luaVM);
   static int l_add(lua_State *luaVM);
   static int l_line(lua_State* luaVM);
   static int l_rect(lua_State* luaVM);
   static int l_circle(lua_State* luaVM);
   static int l_colourReplace(lua_State* luaVM);
   static int l_overlay(lua_State* luaVM);
   static int l_directionLight(lua_State *luaVM);
   static int l_applyEmboss(lua_State* luaVM);
   static int l_applyEdgeDetectLaplace(lua_State* luaVM);
   static int l_clampBelow(lua_State* luaVM);
   static int l_clampAbove(lua_State* luaVM);
   static int l_brightnessMult(lua_State* luaVM); 
   static int l_getRed(lua_State* luaVM); 
   static int l_getGreen(lua_State* luaVM); 
   static int l_getBlue(lua_State* luaVM); 
   //to document:
   //static int l_pixelFit(lua_State* luaVM);
   static int l_makeNormalMap(lua_State* luaVM);
   static int l_directionLightNormal(lua_State *luaVM);
   static int l_lightNormal(lua_State *luaVM); 



   vector<PTexture*> workingtextures;
   vector<PTexture*> finaltextures;
   private:
   

   void recurseFill(int num, int x, int y, int fred, int fgreen, int fblue, int tred, int tgreen, int tblue);
   
   // Matrices in order:  m00 m01 m02 m10 m11 m12
};

#endif
