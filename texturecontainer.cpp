#include "texturecontainer.h"
#include <math.h>
#include <list.h>
#include <map>
#include <windows.h>
#include "Random.h"
#include "prochash.h"
#include "perlin.h"
#include "treetex.h"
//#include "hipng.h"
#include "pngwriter.h"
#include "GeoUtil.h" 


using namespace std;

TextureContainer::TextureContainer()
{
   return;  
}

TextureContainer::~TextureContainer()
{
   for (int i = 0;i < workingtextures.size();i++)
   {
      delete workingtextures[i];
   }
   return;
}

int TextureContainer::getWorkingA8R8G8B8(int num, int x, int y)
{
   return workingtextures[num]->getA8R8G8B8(x, y);
}

int TextureContainer::getFinalA8R8G8B8(int num, int x, int y)
{
   return finaltextures[num]->getA8R8G8B8(x, y);
}

int TextureContainer::getWorkingTextureMemSize(int num)
{
   int intsize = sizeof(float);
   int elements = 3;
   return this->getWorkingXSize(num) * this->getWorkingYSize(num) * intsize * elements;
}

int TextureContainer::getFinalTextureMemSize(int num)
{
   int intsize = sizeof(float);
   int elements = 3;
   return this->getFinalXSize(num) * this->getFinalYSize(num) * intsize * elements;
}

void TextureContainer::clearTextureContainer()
{
   for (int i = 0;i < workingtextures.size();i++)
   {
      delete workingtextures[i];
   }
   workingtextures.clear();
   finaltextures.clear();
   return;
}

int TextureContainer::getTextureContainerMemSize()
{
   int intsize = sizeof(float);
   int elements = 3;
   int size = 0;
   for (int i = 0;i < this->workingtextures.size();i++)
   {
      size += this->getWorkingXSize(i) * this->getWorkingYSize(i) * intsize * elements;
   }
   
   for (int i = 0;i < this->finaltextures.size();i++)
   {
      //size += this->getFinalXSize(i) * this->getFinalYSize(i) * intsize * elements;
   }
   
   return size;
}

int TextureContainer::getWorkingXSize(int num)
{
   return workingtextures[num]->getXSize();
}

int TextureContainer::getFinalXSize(int num)
{
   return finaltextures[num]->getXSize();
}

int TextureContainer::getWorkingYSize(int num)
{
   return workingtextures[num]->getYSize();
}

int TextureContainer::getFinalYSize(int num)
{
   return finaltextures[num]->getYSize();
}

int TextureContainer::addNewWorkingTexture(int xsize, int ysize)
{
   int a = workingtextures.size();
   workingtextures.push_back(new PTexture(xsize, ysize));
   return a;
}

int TextureContainer::l_newTexture(lua_State* luaVM)
{
   TextureContainer* tc = (TextureContainer*)lua_touserdata(luaVM, lua_upvalueindex(1));
   int xsize = (int)lua_tonumber( luaVM, -2 );
   int ysize = (int)lua_tonumber( luaVM, -1 );
   int handle = tc->addNewWorkingTexture(xsize, ysize);
   lua_pushnumber(luaVM, handle);
   return 1;
}

int TextureContainer::makeFinal(int num)
{
   int a = finaltextures.size();
   finaltextures.push_back(workingtextures[num]);
   
   int xs = workingtextures[num]->getXSize();
   int ys = workingtextures[num]->getYSize();
   
   /*png_byte* pb = new png_byte[xs*ys*3];
   for (int yy = 0;yy<ys;yy++)
   {
      for (int xx = 0;xx<xs;xx++)
      {
         pb[((yy*workingtextures[num]->xdim)*3)]   = workingtextures[num]->getRed(xx, yy);
         pb[((yy*workingtextures[num]->xdim)*3)+1] = workingtextures[num]->getGreen(xx, yy);
         pb[((yy*workingtextures[num]->xdim)*3)+2] = workingtextures[num]->getBlue(xx, yy);
      }
   }*/
   
   /*float r, g, b;
   pngwriter image(xs, ys, 1.0, "test.png");
   image.setcompressionlevel(9);
   for(int yy = 0;yy<ys;yy++)
   {
      for(int xx = 0;xx<xs;xx++)
      {
         r = ((float)workingtextures[num]->getRed(xx, yy))/255.0;
         g = ((float)workingtextures[num]->getGreen(xx, yy))/255.0;
         b = ((float)workingtextures[num]->getBlue(xx, yy))/255.0;
         image.plot(xx+1, ys-yy, r, g, b);
      }
   }
   image.close();*/
   
   
   /*HIPNG hp;
   hp.file_name = "test.png";
   hp.width = xs;
   hp.height = ys;
   hp.bit_depth = 8;
   hp.color_type = PNG_COLOR_TYPE_RGB;
   hp.interlaced = false;
   hp.num_colors = 0;
   hp.palettep = NULL;
   hp.gamma = -1;
   hp.num_text = 0;
   hp.pComment = NULL;
   hp.current_time = true;
   hp.transparent_palette = NULL;
   hp.num_transparent = 0;
   hp.RGBtransparent_value = 0;
   hp.rows = NULL;
   hp.row_callback_fn = NULL;
   hp.image = pb;
   bool alc = write_standard_png(&hp);*/
   
   return a;
}

int TextureContainer::l_makeFinal( lua_State* luaVM )
{
   TextureContainer* tc = (TextureContainer*)lua_touserdata(luaVM, lua_upvalueindex(1));
   int num = (int)lua_tonumber( luaVM, -1 );
   int handle = tc->makeFinal(num);
   lua_pushnumber(luaVM, handle);
   return 1;
}

int TextureContainer::getFinalNumber()
{
   return this->finaltextures.size();
}

int TextureContainer::getWorkingNumber()
{
   return this->workingtextures.size();
}

struct ltstr
{
   bool operator()(const char* s1, const char* s2) const
   {
      return strcmp(s1, s2) < 0;
   }
};

static int luaFileWriter(lua_State *L, const void *p, size_t sz, void *ud)
{
   fwrite(p, sz, 1, (FILE *) ud);
   return 0;   
}

struct luaFileReaderData
{
   FILE *file;
   void *data;
};

static const char *luaFileReader(lua_State *L, void *data, size_t *sz)
{
   // Is a 
   luaFileReaderData *d = (luaFileReaderData *) data;
   if (feof(d->file))
      return NULL;
   (*sz) = fread(d->data, 1, 4096, d->file);
   return (const char *) d->data;
}

void TextureContainer::loadLuaLib(const char *dir, lua_State *L)
{
   WIN32_FIND_DATA FindFileData;
   HANDLE hFind = INVALID_HANDLE_VALUE;
   DWORD dwError;
   size_t length_of_arg;
   char buffer[4096];
   map<char *, pair<FILETIME, int>, ltstr> m;
   map<char *, pair<FILETIME, int>, ltstr>::iterator iter;
   FILETIME filetime;
   char *temp;
   char tempend[5];
   int i, j, n;
   char extension[2][5] = {".dat", ".lua"};
   
   strcpy(buffer, dir);
   strcat(buffer, "\\*");

   // Find the first file in the directory.
   hFind = FindFirstFile(buffer, &FindFileData);

   if (hFind == INVALID_HANDLE_VALUE) 
      return;
   do
   {
      n = strlen(FindFileData.cFileName);
      temp = (char *) malloc(n+1+strlen(dir)+2);
      strcpy(temp, FindFileData.cFileName);
      for(i=0;i<4;i++)
      {
         j = n-4+i;
         if (j < 0)
            j = 0;
         tempend[i] = temp[j];
         temp[j] = '\0';
         if ((tempend[i] >= 'A') && (tempend[i] <= 'Z'))
            tempend[i] += 'a'-'A';
      }
      tempend[4] = '\0';
      // .dat or .lua?
      j = -1;
      if (strcmp(tempend, ".dat") == 0)
         j = 0;
      else if (strcmp(tempend, ".lua") == 0)
         j = 1;
      
      if (j != -1)
      {
         // it is a .dat or .lua file
         // Is there an existing one in the map?
         iter = m.find(temp);
         filetime = (*iter).second.first;
         if ((iter == m.end())
            || CompareFileTime(&FindFileData.ftLastWriteTime,
                               &filetime) > 0)
            m[temp] = pair<FILETIME,int>(FindFileData.ftLastWriteTime, j);
      }
   } while(FindNextFile(hFind, &FindFileData) != 0);
   //dwError = GetLastError();
   FindClose(hFind);
   
   // Grab each file from the directory
   while(1)
   {
      iter = m.begin();
      if (iter == m.end())
         break;
      temp = (*iter).first;
      j = (*iter).second.second;
      m.erase(iter);

      memmove(temp+strlen(dir)+1, temp, strlen(temp)+1);
      strcpy(temp, dir);
      temp[strlen(temp)] = '\\';
      printf("Loading %s\n", temp);
      n = strlen(temp);
      if (j == 0)
      {
         // .dat file load
         strcpy(temp+n, ".dat");
         luaFileReaderData data;
         data.file = fopen(temp, "rb");
         data.data = malloc(4096);
         if ((data.file != NULL) && (data.data != NULL))
         {
            lua_load(L, luaFileReader, &data, temp);
         }
         free(data.data);
         fclose(data.file);
      }
      else if (j == 1)
      {
         strcpy(temp+n, ".lua");
         // .lua file load
         luaL_loadfile(L, temp);
         strcpy(temp+n, ".dat");
         FILE *file = fopen(temp, "wb");
         if (file != NULL)
         {
            lua_dump(L, luaFileWriter, file);
            fclose(file);
         }
      }
      free(temp);
      // Execute the lua file
      lua_call(L, 0, 0);
   }
   return;
}

void TextureContainer::loadLua(char* filename)
{
   //set up lua garbage
   lua_State* LStat = luaL_newstate();
   luaL_openlibs(LStat);
   
   loadLuaLib("lib", LStat);
   
   int s = luaL_loadfile(LStat, filename);

   TreeTexture::registerOperations(LStat);
   
   lua_pushstring (LStat, "BufferedTexture");
   lua_pushlightuserdata(LStat, this);
   lua_pushcclosure(LStat, BufferedTexture::create, 1);
   lua_settable(LStat, LUA_GLOBALSINDEX);

   lua_pushstring (LStat, "Render");
   lua_pushlightuserdata(LStat, this);
   lua_pushcclosure(LStat, BufferedTexture::render, 1);
   lua_settable(LStat, LUA_GLOBALSINDEX);   
   
   /*lua_pushstring (LStat, "RenderBack");
   lua_pushlightuserdata(LStat, this);
   lua_pushcclosure(LStat, BufferedTexture::renderback, 1);
   lua_settable(LStat, LUA_GLOBALSINDEX);*/
   
   //setPixel
   lua_pushstring (LStat, "setPixel");
   lua_pushlightuserdata(LStat, this);
   lua_pushcclosure(LStat, l_setPixel, 1);
   lua_settable(LStat, LUA_GLOBALSINDEX);
   
   //clearTexture
   lua_pushstring (LStat, "clearTexture");
   lua_pushlightuserdata(LStat, this);
   lua_pushcclosure(LStat, l_clearTexture, 1);
   lua_settable(LStat, LUA_GLOBALSINDEX);
   
   //newTexture
   lua_pushstring (LStat, "newTexture");
   lua_pushlightuserdata(LStat, this);
   lua_pushcclosure(LStat, l_newTexture, 1);
   lua_settable(LStat, LUA_GLOBALSINDEX);
   
   //makeFinal
   lua_pushstring (LStat, "makeFinal");
   lua_pushlightuserdata(LStat, this);
   lua_pushcclosure(LStat, l_makeFinal, 1);
   lua_settable(LStat, LUA_GLOBALSINDEX);

   //setHStrip
   lua_pushstring (LStat, "setHStrip");
   lua_pushlightuserdata(LStat, this);
   lua_pushcclosure(LStat, l_setHStrip, 1);
   lua_settable(LStat, LUA_GLOBALSINDEX);  
   
   //setVStrip
   lua_pushstring (LStat, "setVStrip");
   lua_pushlightuserdata(LStat, this);
   lua_pushcclosure(LStat, l_setVStrip, 1);
   lua_settable(LStat, LUA_GLOBALSINDEX);
   
   //applyTransform
   lua_pushstring (LStat, "applyTransform");
   lua_pushlightuserdata(LStat, this);
   lua_pushcclosure(LStat, l_applyTransform, 1);
   lua_settable(LStat, LUA_GLOBALSINDEX);
   
   //applyScale
   lua_pushstring (LStat, "applyScale");
   lua_pushlightuserdata(LStat, this);
   lua_pushcclosure(LStat, l_applyScale, 1);
   lua_settable(LStat, LUA_GLOBALSINDEX);
   
   //applyRotationAbout
   lua_pushstring (LStat, "applyRotationAbout");
   lua_pushlightuserdata(LStat, this);
   lua_pushcclosure(LStat, l_applyRotationAbout, 1);
   lua_settable(LStat, LUA_GLOBALSINDEX);
   
   //makeHeightMap
   lua_pushstring (LStat, "makeHeightMap");
   lua_pushlightuserdata(LStat, this);
   lua_pushcclosure(LStat, l_makeHeightMap, 1);
   lua_settable(LStat, LUA_GLOBALSINDEX);
   
   //tint
   lua_pushstring (LStat, "tint");
   lua_pushlightuserdata(LStat, this);
   lua_pushcclosure(LStat, l_tint, 1);
   lua_settable(LStat, LUA_GLOBALSINDEX);
   
   //quantize
   lua_pushstring (LStat, "quantize");
   lua_pushlightuserdata(LStat, this);
   lua_pushcclosure(LStat, l_quantize, 1);
   lua_settable(LStat, LUA_GLOBALSINDEX);

   //copyTexture
   lua_pushstring (LStat, "copyTexture");
   lua_pushlightuserdata(LStat, this);
   lua_pushcclosure(LStat, l_copyTexture, 1);
   lua_settable(LStat, LUA_GLOBALSINDEX);

   //fill
   lua_pushstring (LStat, "fill");
   lua_pushlightuserdata(LStat, this);
   lua_pushcclosure(LStat, l_fill, 1);
   lua_settable(LStat, LUA_GLOBALSINDEX);

   //filledCircle
   lua_pushstring (LStat, "filledCircle");
   lua_pushlightuserdata(LStat, this);
   lua_pushcclosure(LStat, l_filledCircle, 1);
   lua_settable(LStat, LUA_GLOBALSINDEX);

   //filledRect
   lua_pushstring (LStat, "filledRect");
   lua_pushlightuserdata(LStat, this);
   lua_pushcclosure(LStat, l_filledRect, 1);
   lua_settable(LStat, LUA_GLOBALSINDEX);

   //filledRect
   lua_pushstring (LStat, "applyBlur");
   lua_pushlightuserdata(LStat, this);
   lua_pushcclosure(LStat, l_applyBlur, 1);
   lua_settable(LStat, LUA_GLOBALSINDEX);

   //applyDefocus
   lua_pushstring (LStat, "applyDefocus");
   lua_pushlightuserdata(LStat, this);
   lua_pushcclosure(LStat, l_applyDefocus, 1);
   lua_settable(LStat, LUA_GLOBALSINDEX);
   
   //applyEdgeDetect
   lua_pushstring (LStat, "applyEdgeDetect");
   lua_pushlightuserdata(LStat, this);
   lua_pushcclosure(LStat, l_applyEdgeDetect, 1);
   lua_settable(LStat, LUA_GLOBALSINDEX); 
   
   //applyGaussianBlur
   lua_pushstring (LStat, "applyGaussianBlur");
   lua_pushlightuserdata(LStat, this);
   lua_pushcclosure(LStat, l_applyGaussianBlur, 1);
   lua_settable(LStat, LUA_GLOBALSINDEX);

    //noise
   lua_pushstring (LStat, "noise");
   lua_pushlightuserdata(LStat, this);
   lua_pushcclosure(LStat, l_noise, 1);
   lua_settable(LStat, LUA_GLOBALSINDEX);

    //fractionalNoise
   lua_pushstring (LStat, "fractionalNoise");
   lua_pushlightuserdata(LStat, this);
   lua_pushcclosure(LStat, l_fractionalNoise, 1);
   lua_settable(LStat, LUA_GLOBALSINDEX);

   //setWithProbability
   lua_pushstring(LStat, "setWithProbability");
   lua_pushlightuserdata(LStat, this);
   lua_pushcclosure(LStat, l_setWithProbability, 1);
   lua_settable(LStat, LUA_GLOBALSINDEX);

   //closestColor
   lua_pushstring(LStat, "closestColour");
   lua_pushlightuserdata(LStat, this);
   lua_pushcclosure(LStat, l_closestColour, 1);
   lua_settable(LStat, LUA_GLOBALSINDEX);
   
   //perlinNoise
   lua_pushstring(LStat, "perlinNoise");
   lua_pushlightuserdata(LStat, this);
   lua_pushcclosure(LStat, l_perlinNoise, 1);
   lua_settable(LStat, LUA_GLOBALSINDEX);
   
   //light
   lua_pushstring(LStat, "light");
   lua_pushlightuserdata(LStat, this);
   lua_pushcclosure(LStat, l_light, 1);
   lua_settable(LStat, LUA_GLOBALSINDEX);
   
   //add
   lua_pushstring(LStat, "add");
   lua_pushlightuserdata(LStat, this);
   lua_pushcclosure(LStat, l_add, 1);
   lua_settable(LStat, LUA_GLOBALSINDEX);   

   //line
   lua_pushstring(LStat, "line");
   lua_pushlightuserdata(LStat, this);
   lua_pushcclosure(LStat, l_line, 1);
   lua_settable(LStat, LUA_GLOBALSINDEX);
   
   //rect
   lua_pushstring(LStat, "rect");
   lua_pushlightuserdata(LStat, this);
   lua_pushcclosure(LStat, l_rect, 1);
   lua_settable(LStat, LUA_GLOBALSINDEX);

   //circle
   lua_pushstring(LStat, "circle");
   lua_pushlightuserdata(LStat, this);
   lua_pushcclosure(LStat, l_circle, 1);
   lua_settable(LStat, LUA_GLOBALSINDEX);
   
   //colourReplace
   lua_pushstring(LStat, "colourReplace");
   lua_pushlightuserdata(LStat, this);
   lua_pushcclosure(LStat, l_colourReplace, 1);
   lua_settable(LStat, LUA_GLOBALSINDEX);
   
   //overlay
   lua_pushstring(LStat, "overlay");
   lua_pushlightuserdata(LStat, this);
   lua_pushcclosure(LStat, l_overlay, 1);
   lua_settable(LStat, LUA_GLOBALSINDEX);
   
   //overlay
   /*lua_pushstring(LStat, "pixelFit");
   lua_pushlightuserdata(LStat, this);
   lua_pushcclosure(LStat, l_pixelFit, 1);
   lua_settable(LStat, LUA_GLOBALSINDEX); */
   
   //applyEmboss
   lua_pushstring(LStat, "applyEmboss");
   lua_pushlightuserdata(LStat, this);
   lua_pushcclosure(LStat, l_applyEmboss, 1);
   lua_settable(LStat, LUA_GLOBALSINDEX);
   
   //applyEdgeDetectLaplace
   lua_pushstring(LStat, "applyEdgeDetectLaplace");
   lua_pushlightuserdata(LStat, this);
   lua_pushcclosure(LStat, l_applyEdgeDetectLaplace, 1);
   lua_settable(LStat, LUA_GLOBALSINDEX);
   
   //directionLight
   lua_pushstring(LStat, "directionLight");
   lua_pushlightuserdata(LStat, this);
   lua_pushcclosure(LStat, l_directionLight, 1);
   lua_settable(LStat, LUA_GLOBALSINDEX);  
   
   //clampBelow
   lua_pushstring(LStat, "clampBelow");
   lua_pushlightuserdata(LStat, this);
   lua_pushcclosure(LStat, l_clampBelow, 1);
   lua_settable(LStat, LUA_GLOBALSINDEX);
   
   //clampAbove
   lua_pushstring(LStat, "clampAbove");
   lua_pushlightuserdata(LStat, this);
   lua_pushcclosure(LStat, l_clampAbove, 1);
   lua_settable(LStat, LUA_GLOBALSINDEX);
   
   //brightnessMult
   lua_pushstring(LStat, "brightnessMult");
   lua_pushlightuserdata(LStat, this);
   lua_pushcclosure(LStat, l_brightnessMult, 1);
   lua_settable(LStat, LUA_GLOBALSINDEX);
   
   //getRed
   lua_pushstring(LStat, "getRed");
   lua_pushlightuserdata(LStat, this);
   lua_pushcclosure(LStat, l_getRed, 1);
   lua_settable(LStat, LUA_GLOBALSINDEX);
   
   //getGreen
   lua_pushstring(LStat, "getGreen");
   lua_pushlightuserdata(LStat, this);
   lua_pushcclosure(LStat, l_getGreen, 1);
   lua_settable(LStat, LUA_GLOBALSINDEX);
   
   //getBlue
   lua_pushstring(LStat, "getBlue");
   lua_pushlightuserdata(LStat, this);
   lua_pushcclosure(LStat, l_getBlue, 1);
   lua_settable(LStat, LUA_GLOBALSINDEX);
   
   //makeNormalMap
   lua_pushstring(LStat, "makeNormalMap");
   lua_pushlightuserdata(LStat, this);
   lua_pushcclosure(LStat, l_makeNormalMap, 1);
   lua_settable(LStat, LUA_GLOBALSINDEX);

   //lightNormal
   lua_pushstring(LStat, "lightNormal");
   lua_pushlightuserdata(LStat, this);
   lua_pushcclosure(LStat, l_lightNormal, 1);
   lua_settable(LStat, LUA_GLOBALSINDEX);
   
   //directionLightNormal
   lua_pushstring(LStat, "directionLightNormal");
   lua_pushlightuserdata(LStat, this);
   lua_pushcclosure(LStat, l_directionLightNormal, 1);
   lua_settable(LStat, LUA_GLOBALSINDEX);
   
   //give the lua script a call
   s = lua_pcall(LStat, 0, 0, 0);

   lua_getfield(LStat, LUA_GLOBALSINDEX, "setup");          /* function to be called */
   lua_pushnumber(LStat, 69);
   lua_pcall(LStat, 1, LUA_MULTRET, 0);
   return;
}

void TextureContainer::setPixel(int num, int x, int y, int red, int green, int blue)
{
   workingtextures[num]->setColour(x, y, red, green, blue);
   return;
}

int TextureContainer::l_setPixel( lua_State* luaVM )
{
   TextureContainer* tc = (TextureContainer*)lua_touserdata(luaVM, lua_upvalueindex(1));
   int num = (int)lua_tonumber( luaVM, -6 );
   int x = (int)lua_tonumber( luaVM, -5 );
   int y = (int)lua_tonumber( luaVM, -4 );
   int red = (int)lua_tonumber( luaVM, -3 );
   int green = (int)lua_tonumber( luaVM, -2 );
   int blue = (int)lua_tonumber( luaVM, -1 );
   tc->setPixel(num, x, y, red, green, blue);
   lua_pushnumber(luaVM, 1);
   return 1;
}

void TextureContainer::setHStrip(int num, int xstart, int length, int y, int red, int green, int blue)
{
   for (int i = 0; i < length;i++)
   {
      workingtextures[num]->setColour(xstart + i, y, red, green, blue);
   }
   return;
}

int TextureContainer::l_setHStrip(lua_State* luaVM)
{
   TextureContainer* tc = (TextureContainer*)lua_touserdata(luaVM, lua_upvalueindex(1));
   int num = (int)lua_tonumber( luaVM, -7 );
   int xstart = (int)lua_tonumber( luaVM, -6 );
   int length = (int)lua_tonumber( luaVM, -5 );
   int y = (int)lua_tonumber( luaVM, -4 );
   int red = (int)lua_tonumber( luaVM, -3 );
   int green = (int)lua_tonumber( luaVM, -2 );
   int blue = (int)lua_tonumber( luaVM, -1 );
   tc->setHStrip(num, xstart, length, y, red, green, blue);
   lua_pushnumber(luaVM, 1);
   return 1;
}

void TextureContainer::setVStrip(int num, int ystart, int length, int x, int red, int green, int blue)
{
   for (int i = 0; i < length;i++)
   {
      workingtextures[num]->setColour(x, ystart + i, red, green, blue);
   }
   return;
}

int TextureContainer::l_setVStrip(lua_State* luaVM)
{
   TextureContainer* tc = (TextureContainer*)lua_touserdata(luaVM, lua_upvalueindex(1));
   int num = (int)lua_tonumber( luaVM, -7 );
   int ystart = (int)lua_tonumber( luaVM, -6 );
   int length = (int)lua_tonumber( luaVM, -5 );
   int x = (int)lua_tonumber( luaVM, -4 );
   int red = (int)lua_tonumber( luaVM, -3 );
   int green = (int)lua_tonumber( luaVM, -2 );
   int blue = (int)lua_tonumber( luaVM, -1 );
   tc->setVStrip(num, ystart, length, x, red, green, blue);
   lua_pushnumber(luaVM, 1);
   return 1;
}

void TextureContainer::clearTexture(int num, int red, int green, int blue)
{
   for (int xx = 0; xx < workingtextures[num]->getXSize();xx++)
   {
      for (int yy = 0; yy < workingtextures[num]->getYSize();yy++)
      {
         workingtextures[num]->setColour(xx, yy, red, green, blue);
      }
   }
   return;
}

int TextureContainer::l_clearTexture(lua_State* luaVM)
{
   TextureContainer* tc = (TextureContainer*)lua_touserdata(luaVM, lua_upvalueindex(1));
   int num = (int)lua_tonumber( luaVM, -4 );
   int red = (int)lua_tonumber( luaVM, -3 );
   int green = (int)lua_tonumber( luaVM, -2 );
   int blue = (int)lua_tonumber( luaVM, -1 );
   tc->clearTexture(num, red, green, blue);
   lua_pushnumber(luaVM, 1);
   return 1;
}

void TextureContainer::applyTransform(int num, float m00, float m01, float m02, float m10, float m11, float m12, int red, int green, int blue)
{
   const float EPSILON = 0.0000001;
   float m[6];
   float ix, iy, jx, jy;
   float sx, sy;   // Point in source space
   int px, py;      // As integer coordinates
   float sx_rbegin, sy_rbegin;
   PTexture *src = workingtextures[num];
   m[0] = m00;
   m[1] = m01;
   m[2] = m02;
   m[3] = m10;
   m[4] = m11;
   m[5] = m12;
   //printf("Forward matrix\n%lf %lf %lf\n%lf %lf %lf\n",
   //   m[0], m[1], m[2], m[3], m[4], m[5]);
   GeoUtil::matinv(m, m);
   ix = m[0];
   iy = m[3];
   jx = m[1];
   jy = m[4];
   //printf("Inverted matrix\n%lf %lf %lf\n%lf %lf %lf\n",
   //   m[0], m[1], m[2], m[3], m[4], m[5]);
   
   int w = src->getXSize();
   int h = src->getYSize();
   PTexture *temp = new PTexture(w, h);
   sx = m[2];
   sy = m[5];
   for(int y=0;y<h;y++)
   {
      sx_rbegin = sx;
      sy_rbegin = sy;
      for(int x=0;x<w;x++)
      {
         // Lookup texture at (sx, sy)
         px = (int) (sx+0.5);
         py = (int) (sy+0.5);
         if ((px < 0) || (px >= w) || (py < 0) || (py >= h))
            temp->setColour(x, y, red, green, blue);
         else
            temp->setA8R8G8B8(x, y, src->getA8R8G8B8(px, py));
         sx += ix;
         sy += iy;
      }
      sx = sx_rbegin+jx;
      sy = sy_rbegin+jy;
   }
   src->copyFromTexture(temp);
   delete temp;

   /*
   float a = m00; float b = m01; float c = m02; 
   float d = m10; float e = m11; float f = m12;
   PTexture* PT = new PTexture(workingtextures[num]->getXSize(), workingtextures[num]->getYSize());
   //for (int xx = 0;xx < PT->getXSize();xx++)
   //{
    //  for (int yy = 0;yy < PT->getYSize();yy++)
    //  {
    //     PT->setColour(xx, yy, workingtextures[num]->getRed(xx, yy), workingtextures[num]->getGreen(xx, yy), workingtextures[num]->getBlue(xx, yy));
    //  }
   //}
   float det = (a*e) - (b*d);
   float pa = (((b*f) - (c*e))/det);
   for (int xx = 0;xx < PT->getXSize();xx++)
   {
      for (int yy = 0;yy < PT->getYSize();yy++)
      {
         float lookx = ((e*xx)/det) - ((b*yy)/det) + pa ;
         float looky = ((-d*xx)/det) + ((a*yy)/det) + (((a*f) - (c*d))/det);
         int lx = (int)(lookx + .5);
         int ly = (int)(looky + .5);
         if (lx < 0 || lx >= PT->getXSize() || ly < 0 || ly > PT->getYSize())
         {
            PT->setColour(xx, yy, red, green, blue);
         }
         else
         {
            PT->setColour(xx, yy, workingtextures[num]->getRed(lx, ly), workingtextures[num]->getGreen(lx, ly), workingtextures[num]->getBlue(lx, ly));
         }
      }
   }
   
   //for (int xx = 0;xx < PT.getXSize();xx++)
   //{
      //for (int yy = 0;yy < PT.getYSize();yy++)
      //{
         //*workingtextures[num] = PT;
      //}
   //}
   for (int xx = 0;xx < PT->getXSize();xx++)
   {
      for (int yy = 0;yy < PT->getYSize();yy++)
      {
         workingtextures[num]->setColour(xx, yy, PT->getRed(xx, yy), PT->getGreen(xx, yy), PT->getBlue(xx, yy));
      }
   }

   delete PT;
   */
   return;
}

int TextureContainer::l_applyTransform( lua_State* luaVM )
{
   TextureContainer* tc = (TextureContainer*)lua_touserdata(luaVM, lua_upvalueindex(1));
   int num = (int)lua_tonumber( luaVM, -10 );
   float a = (float)lua_tonumber( luaVM, -9);
   float b = (float)lua_tonumber( luaVM, -8);
   float c = (float)lua_tonumber( luaVM, -7);
   float d = (float)lua_tonumber( luaVM, -6);
   float e = (float)lua_tonumber( luaVM, -5);
   float f = (float)lua_tonumber( luaVM, -4);
   int red = (int)lua_tonumber( luaVM, -3 );
   int green = (int)lua_tonumber( luaVM, -2 );
   int blue = (int)lua_tonumber( luaVM, -1 );
   tc->applyTransform(num, a, b, c, d, e, f, red, green, blue);
   lua_pushnumber(luaVM, 1);
   return 1;
}

void TextureContainer::applyScale(int num, float xscale, float yscale, int red, int green, int blue)
{
   /*
   float a = xscale;
   float b = 0;
   //float c = 0;
   float c = -(xscale - 1) * .5 * (workingtextures[num]->getXSize()-0);
   float d = 0;
   float e = yscale;
   //float f = 0;
   float f = -((yscale -1) * .5 * (workingtextures[num]->getYSize()) - 0);
   this->applyTransform(num, a, b, c, d, e, f, red, green, blue);
   */
   float t[6], m[6];
   float ox = workingtextures[num]->getXSize()*0.5,
          oy = workingtextures[num]->getYSize()*0.5;
   m[0] = 1.0; m[1] = 0.0; m[2] = -ox+0.5;
   m[3] = 0.0; m[4] = 1.0; m[5] = -oy+0.5;
   t[0] = xscale;   t[1] = 0.0;      t[2] = 0.0;
   t[3] = 0.0;      t[4] = yscale;   t[5] = 0.0;
   GeoUtil::matmul(t, m, m);
   t[0] = 1.0; t[1] = 0.0; t[2] = ox-0.5;
   t[3] = 0.0; t[4] = 1.0; t[5] = oy-0.5;
   GeoUtil::matmul(t, m, m);
   //printf("Matrix:\n%lf %lf %lf\n%lf %lf %lf\n",
   //   m[0], m[1], m[2], m[3], m[4], m[5]);

   this->applyTransform(num, m[0], m[1], m[2], m[3], m[4], m[5], red, green, blue);
   return;
}

int TextureContainer::l_applyScale( lua_State* luaVM )
{
   TextureContainer* tc = (TextureContainer*)lua_touserdata(luaVM, lua_upvalueindex(1));
   int num = (int)lua_tonumber( luaVM, -6 );
   float xscale = (float)lua_tonumber( luaVM, -5 );
   float yscale = (float)lua_tonumber( luaVM, -4 );
   int red = (int)lua_tonumber( luaVM, -3 );
   int green = (int)lua_tonumber( luaVM, -2 );
   int blue = (int)lua_tonumber( luaVM, -1 );
   tc->applyScale(num, xscale, yscale, red, green, blue);
   lua_pushnumber(luaVM, 1);
   return 1;
}

void TextureContainer::applyRotationAbout(int num, float theta, float ox, float oy, int red, int green, int blue)
{
   /*
   float a = 0;
   float b = 0;
   //float c = 0;
   float c = 0;
   float d = 0;
   float e = 0;
   //float f = 0;
   float f = 0;
   */
   float t[6], m[6];
   float c, s;
   theta *= 3.14159265358979/180.0;
   c = cos(theta);
   s = sin(theta);
   m[0] = 1.0; m[1] = 0.0; m[2] = -ox+0.5;
   m[3] = 0.0; m[4] = 1.0; m[5] = -oy+0.5;
   t[0] = c;   t[1] = -s;  t[2] = 0.0;
   t[3] = s;   t[4] = c;   t[5] = 0.0;
   GeoUtil::matmul(t, m, m);
   t[0] = 1.0; t[1] = 0.0; t[2] = ox-0.5;
   t[3] = 0.0; t[4] = 1.0; t[5] = oy-0.5;
   GeoUtil::matmul(t, m, m);
   //printf("Matrix:\n%lf %lf %lf\n%lf %lf %lf\n",
   //   m[0], m[1], m[2], m[3], m[4], m[5]);

   this->applyTransform(num, m[0], m[1], m[2], m[3], m[4], m[5], red, green, blue);
   return;
}

int TextureContainer::l_applyRotationAbout( lua_State* luaVM )
{
   TextureContainer* tc = (TextureContainer*)lua_touserdata(luaVM, lua_upvalueindex(1));
   int num = (int)lua_tonumber( luaVM, -7 );
   float theta = (float)lua_tonumber( luaVM, -6 );
   float ox = (float)lua_tonumber( luaVM, -5 );
   float oy = (float)lua_tonumber( luaVM, -4 );
   int red = (int)lua_tonumber( luaVM, -3 );
   int green = (int)lua_tonumber( luaVM, -2 );
   int blue = (int)lua_tonumber( luaVM, -1 );
   tc->applyRotationAbout(num, theta, ox, oy, red, green, blue);
   lua_pushnumber(luaVM, 1);
   return 1;
}

void TextureContainer::makeHeightMap(int num)
{
   float a;
   for(int xx = 0;xx < workingtextures[num]->getXSize();xx++)
   {
      for(int yy = 0;yy < workingtextures[num]->getYSize();yy++)
      {
         a = workingtextures[num]->getHeightD(xx, yy);
         workingtextures[num]->setColourD(xx, yy, a, a, a);
      }
   }
   return;
}

int TextureContainer::l_makeHeightMap( lua_State* luaVM )
{
   TextureContainer* tc = (TextureContainer*)lua_touserdata(luaVM, lua_upvalueindex(1));
   int num = (int)lua_tonumber( luaVM, -1 );
   tc->makeHeightMap(num);
   lua_pushnumber(luaVM, 1);
   return 1;
}

void TextureContainer::tint(int num, int red, int green, int blue)
{
   for(int xx = 0;xx < workingtextures[num]->getXSize();xx++)
   {
      for(int yy = 0;yy < workingtextures[num]->getYSize();yy++)
      {
         float rnew = (((float)workingtextures[num]->getRed(xx, yy) / 255.0) * ((float)red));
         float gnew = (((float)workingtextures[num]->getGreen(xx, yy) / 255.0) * ((float)green));
         float bnew = (((float)workingtextures[num]->getBlue(xx, yy) / 255.0) * ((float)blue));
         workingtextures[num]->setColour(xx, yy, (int)rnew, (int)gnew, (int)bnew);
      }
   }
   return;
}

int TextureContainer::l_tint( lua_State* luaVM )
{
   TextureContainer* tc = (TextureContainer*)lua_touserdata(luaVM, lua_upvalueindex(1));
   int num = (int)lua_tonumber( luaVM, -4 );
   int red = (int)lua_tonumber( luaVM, -3 );
   int green = (int)lua_tonumber( luaVM, -2 );
   int blue = (int)lua_tonumber( luaVM, -1 );
   tc->tint(num, red, green, blue);
   lua_pushnumber(luaVM, 1);
   return 1;
}

void TextureContainer::quantize(int num, int redvalues, int greenvalues, int bluevalues)
{
   float redamount = ((float)redvalues)/(256.0);
   float greenamount = ((float)greenvalues)/(256.0);
   float blueamount = ((float)bluevalues)/(256.0);
   int xs = workingtextures[num]->getXSize();
   int ys = workingtextures[num]->getYSize();
   for(int yy = 0;yy < ys;yy++)
   {
      for(int xx = 0;xx < xs;xx++)
      {
         float rnew = (float)workingtextures[num]->getRed(xx, yy);
         rnew *= redamount; rnew = floor(rnew); rnew /= redamount;
         
         float gnew = (float)workingtextures[num]->getGreen(xx, yy);
         gnew *= greenamount; gnew = floor(gnew); gnew /= greenamount;

         float bnew = (float)workingtextures[num]->getBlue(xx, yy);
         bnew *= blueamount; bnew = floor(bnew); bnew /= blueamount;

         workingtextures[num]->setColour(xx, yy, (int)rnew, (int)gnew, (int)bnew);
      }
   }
   return;
}

int TextureContainer::l_quantize( lua_State* luaVM )
{
   TextureContainer* tc = (TextureContainer*)lua_touserdata(luaVM, lua_upvalueindex(1));
   int num = (int)lua_tonumber( luaVM, -4 );
   int redvalues = (int)lua_tonumber( luaVM, -3 );
   int greenvalues = (int)lua_tonumber( luaVM, -2 );
   int bluevalues = (int)lua_tonumber( luaVM, -1 );
   tc->quantize(num, redvalues, greenvalues, bluevalues);
   lua_pushnumber(luaVM, 1);
   return 1;
}

void TextureContainer::copyTexture(int num1, int num2)
{
    workingtextures[num1]->copyFromTexture(workingtextures[num2]);
    return;
}

int TextureContainer::l_copyTexture( lua_State* luaVM )
{
   TextureContainer* tc = (TextureContainer*)lua_touserdata(luaVM, lua_upvalueindex(1));
   int num1 = (int)lua_tonumber( luaVM, -2 );
   int num2 = (int)lua_tonumber( luaVM, -1 );
   tc->copyTexture(num1, num2);
   lua_pushnumber(luaVM, 1);
   return 1;
}

void TextureContainer::fill2(int num, int x, int y, int red, int green, int blue)
{
   int a = workingtextures[num]->getRed(x, y);
   int b = workingtextures[num]->getGreen(x, y);
   int c = workingtextures[num]->getBlue(x, y);

   recurseFill(num, x, y, red, green, blue, a, b, c);
   return;
}
   
   

void TextureContainer::fill(int num, int x, int y, int red, int green, int blue)
{
   /*int a = workingtextures[num]->getRed(x, y);
   int b = workingtextures[num]->getGreen(x, y);
   int c = workingtextures[num]->getBlue(x, y);
   this->recurseFill(num, x, y, red, green, blue, a, b, c);
   return;*/

   list<int> lx;
   list<int> ly;
   lx.push_back(x);
   ly.push_back(y);
   list<int>::iterator xi;
   list<int>::iterator yi;
   xi = lx.begin(); yi = ly.begin();
   int a = workingtextures[num]->getRed((*xi), (*yi));
   int b = workingtextures[num]->getGreen((*xi), (*yi));
   int c = workingtextures[num]->getBlue((*xi), (*yi));

   while(lx.empty() == false)
   {
      xi = lx.begin(); yi = ly.begin();
      int thisred = workingtextures[num]->getRed((*xi), (*yi));
      int thisgreen = workingtextures[num]->getGreen((*xi), (*yi));
      int thisblue = workingtextures[num]->getBlue((*xi), (*yi));


   workingtextures[num]->setColour((*xi), (*yi), red, green, blue);

   if (((*xi) + 1) < workingtextures[num]->getXSize())
   {
      int testred = workingtextures[num]->getRed((*xi)+1, (*yi));
      int testgreen = workingtextures[num]->getGreen((*xi)+1, (*yi));
      int testblue = workingtextures[num]->getBlue((*xi)+1, (*yi));
      if (testred == a && testgreen == b && testblue == c)
      {
         if(testred != red || testgreen != green || testblue != blue)
         {
            lx.push_back((*xi)+1); ly.push_back((*yi));
         }
      }
      //this->recurseFill(num, x+1, y, fred, fgreen, fblue, tred, tgreen, tblue);
   }

   if (((*yi) + 1) < workingtextures[num]->getYSize())
   {
      int testred = workingtextures[num]->getRed((*xi), (*yi)+1);
      int testgreen = workingtextures[num]->getGreen((*xi), (*yi)+1);
      int testblue = workingtextures[num]->getBlue((*xi), (*yi)+1);
      if (testred == a && testgreen == b && testblue == c)
      {
         if(testred != red || testgreen != green || testblue != blue)
         {
            lx.push_back((*xi)); ly.push_back((*yi)+1);
         }
      }
      //this->recurseFill(num, x+1, y, fred, fgreen, fblue, tred, tgreen, tblue);
   }

   if (((*yi) - 1) >= 0)
   {
      int testred = workingtextures[num]->getRed((*xi), (*yi)-1);
      int testgreen = workingtextures[num]->getGreen((*xi), (*yi)-1);
      int testblue = workingtextures[num]->getBlue((*xi), (*yi)-1);
      if (testred == a && testgreen == b && testblue == c)
      {
         if(testred != red || testgreen != green || testblue != blue)
         {
            lx.push_back((*xi)); ly.push_back((*yi)-1);
         }
      }
      //this->recurseFill(num, x+1, y, fred, fgreen, fblue, tred, tgreen, tblue);
   }

   if (((*xi)) > 0)
   {
      int testred = workingtextures[num]->getRed((*xi)-1, (*yi));
      int testgreen = workingtextures[num]->getGreen((*xi)-1, (*yi));
      int testblue = workingtextures[num]->getBlue((*xi)-1, (*yi));
      if (testred == a && testgreen == b && testblue == c)
      {
         if(testred != red || testgreen != green || testblue != blue)
         {
            lx.push_back((*xi)-1); ly.push_back((*yi));
         }
      }
      //this->recurseFill(num, x+1, y, fred, fgreen, fblue, tred, tgreen, tblue);
   }

   lx.pop_front();
   ly.pop_front();
   }

   return;
}

int TextureContainer::l_filledCircle( lua_State* luaVM )
{
   TextureContainer* tc = (TextureContainer*)lua_touserdata(luaVM, lua_upvalueindex(1));
   int num = (int)lua_tonumber( luaVM, -7 );
   int x = (int)lua_tonumber( luaVM, -6 );
   int y = (int)lua_tonumber( luaVM, -5 );
   int radius = (int)lua_tonumber( luaVM, -4);
   int red = (int)lua_tonumber( luaVM, -3 );
   int green = (int)lua_tonumber( luaVM, -2 );
   int blue = (int)lua_tonumber( luaVM, -1 );
   tc->filledCircle(num, x, y, radius, red, green, blue);
   lua_pushnumber(luaVM, 1);
   return 1;
}

void TextureContainer::filledRect(int num, int x1, int y1, int x2, int y2, int red, int green, int blue)
{
    int dx = 0;int dy = 0;
    //printf("here");

    if ((x1 == x2) && (y1 == y2))
    {
       //map[this->getFlatCoord(x2, y2, xsize)] = value;
       this->setPixel(num, x2, y2, red, green, blue);
       return;
    }
    if (y1 == y2)
    {
       dx = (x2 - x1) / abs(x2-x1);
       for (int nx = x1;nx != x2;nx += dx)
       {
            this->setPixel(num, nx, y2, red, green, blue);
            //map[this->getFlatCoord(nx, y2, xsize)] = value;
       }
       return;
    }
    if (x1 == x2)
    {
       dy = (y2 - y1) / abs(y2 - y1);
       for (int ny = y1;ny != y2;ny += dy)
       {
            this->setPixel(num, x2, ny, red, green, blue);
            //map[this->getFlatCoord(x2, ny, xsize)] = value;
       }
       return;
    }

    dx = (x2 - x1) / abs(x2-x1);dy = (y2 - y1) / abs(y2 - y1);
    for (int nx = x1;nx != x2;nx += dx)
    {
       for (int ny = y1;ny != y2;ny += dy)
       {
           this->setPixel(num, nx, ny, red, green, blue);
          //map[this->getFlatCoord(nx, ny, xsize)] = value;
       }
    }
    for (int nx = x1;nx != x2;nx += dx)
    {
       this->setPixel(num, nx, y2, red, green, blue);
       //map[this->getFlatCoord(nx, y2, xsize)] = value;
    }
    for (int ny = y1;ny != y2;ny += dy)
    {
       this->setPixel(num, x2, ny, red, green, blue);
       //map[this->getFlatCoord(x2, ny, xsize)] = value;
    }
    this->setPixel(num, x2, y2, red, green, blue);
    //map[this->getFlatCoord(x2, y2, xsize)] = value;
    
    return;
}

int TextureContainer::l_filledRect( lua_State* luaVM )
{
   TextureContainer* tc = (TextureContainer*)lua_touserdata(luaVM, lua_upvalueindex(1));
   int num = (int)lua_tonumber( luaVM, -8 );
   int x1 = (int)lua_tonumber( luaVM, -7 );
   int y1 = (int)lua_tonumber( luaVM, -6 );
   int x2 = (int)lua_tonumber( luaVM, -5 );
   int y2 = (int)lua_tonumber( luaVM, -4 );
   int red = (int)lua_tonumber( luaVM, -3 );
   int green = (int)lua_tonumber( luaVM, -2 );
   int blue = (int)lua_tonumber( luaVM, -1 );
   tc->filledRect(num, x1, y1, x2, y2, red, green, blue);
   lua_pushnumber(luaVM, 1);
   return 1;
}

/*
static int awesomeFill_row_rle(int tcolor, int w,
   int *row, int rowindex, int *dest, int destindex)
{
   // dest[destindex]..dest[destindex+w-1] must be legal
   int i;
   bool state = false, newstate = false;
   int n = 0;

   for(i=0;i<w;i++)
   {
      newstate = (row[rowindex+i] == tcolor);
      if (newstate == state)
         continue;
      state = newstate;
      dest[desindex+(n++)] = i;
   }
   return n;
}

void TextureContainer::awesomeFill(int num, int x, int y, int fred, int fgreen, int fblue, int tred, int tgreen, int tblue)
{
   // RLE the current line
   // For each line, mark which RLE items to fill...
   int w = workingtextures[num]->getXSize(),
       h = workingtextures[num]->getYSize();
   int bufsize = 2*h;
   int *buf = (int *) malloc(bufsize*sizeof(int));
   int *rowbuf = (int *) malloc(w*sizeof(int));
   int *rledata = (int *) malloc(w*sizeof(int));
   int *rlesize = (int *) malloc(w*sizeof(int));
   char *flagvec = (char *) malloc((2*w+7)/8);
   int bufoff = 0;
   if ((buf == NULL) || (rowbuf == NULL) || (flagvec == NULL) ||
       (rledata == NULL) || (rlesize == NULL))
   {
      printf("Memory allocation failed\n");
      abort();
   }
   int firstrow_index = y*w, lastrow_index = firstrow_index;
   int firstrow = y, lastrow = y;
   int i;
   for(i=0;i<h;i++)
      rledata[i] = -1;
   int tcolor = 0xFF000000
                | ((fred   & 0xFF) << 16)
                | ((fgreen & 0xFF) <<  8)
                | ((fblue  & 0xFF)      );
   // At rledata:  For each row, the index of that row's RLE data, or -1 if none exists
   // At rlesize:  For each row, the size of the RLE data for that row.
   // At flagvec:  Whether each element has been inserted into

   // Initial state:  Do the first row.
   workingtextures[num]->getA8R8G8B8row(y, rowbuf);
   rledata[y] = bufoff;
   rlesize[y] = awesomefill_row_rle(tcolor, w, rowbuf, y*w, buf, bufoff);
   bufoff += rlesize[y];
   while(1)
   {
      // RLE of each row

   }


   free(buf);
   return;
}
*/

void TextureContainer::recurseFill(int num, int x, int y, int fred, int fgreen, int fblue, int tred, int tgreen, int tblue)
{
   int a = workingtextures[num]->getRed(x, y);
   int b = workingtextures[num]->getGreen(x, y);
   int c = workingtextures[num]->getBlue(x, y);

   if (a != tred || b != tgreen || c != tblue)
   {
      return;
   }

   if (a == fred && b == fgreen && c == fblue)
   {
      return;
   }


    int y1;

    //draw current scanline from start position to the top
    y1 = y;

    while(workingtextures[num]->getRed(x, y1) == tred && workingtextures[num]->getGreen(x, y1) == tgreen && workingtextures[num]->getBlue(x, y1) == tblue && y1 < workingtextures[num]->getYSize())
    {
        workingtextures[num]->setColour(x, y1, fred, fgreen, fblue);
        y1++;
    }

    //draw current scanline from start position to the bottom
    y1 = y - 1;
    while(workingtextures[num]->getRed(x, y1) == tred && workingtextures[num]->getGreen(x, y1) == tgreen && workingtextures[num]->getBlue(x, y1) == tblue && y1 >= 0)
    {
        workingtextures[num]->setColour(x, y1, fred, fgreen, fblue);
        y1--;
    }

    //test for new scanlines to the left
    y1 = y;
    while(workingtextures[num]->getRed(x, y1) == fred && workingtextures[num]->getGreen(x, y1) == fgreen && workingtextures[num]->getBlue(x, y1) == fblue && y1 < workingtextures[num]->getYSize())
    {
        if(x > 0 && workingtextures[num]->getRed(x-1, y1) == tred && workingtextures[num]->getGreen(x-1, y1) == tgreen && workingtextures[num]->getBlue(x-1, y1) == tblue)
        {
            recurseFill(num, x - 1, y1, fred, fgreen, fblue, tred, tgreen, tblue);
        }
        y1++;
    }
    y1 = y - 1;
    while(workingtextures[num]->getRed(x, y1) == fred && workingtextures[num]->getGreen(x, y1) == fgreen && workingtextures[num]->getBlue(x, y1) == fblue && y1 >= 0)
    {
        if(x > 0 && workingtextures[num]->getRed(x-1, y1) == tred && workingtextures[num]->getGreen(x-1, y1) == tgreen && workingtextures[num]->getBlue(x-1, y1) == tblue)
        {
            recurseFill(num, x - 1, y1, fred, fgreen, fblue, tred, tgreen, tblue);
        }
        y1--;
    }

    //test for new scanlines to the right
    y1 = y;
    while(workingtextures[num]->getRed(x, y1) == fred && workingtextures[num]->getGreen(x, y1) == fgreen && workingtextures[num]->getBlue(x, y1) == fblue && y1 < workingtextures[num]->getYSize())
    {
        if(x < workingtextures[num]->getXSize() - 1 && workingtextures[num]->getRed(x+1, y1) == tred && workingtextures[num]->getGreen(x+1, y1) == tgreen && workingtextures[num]->getBlue(x+1, y1) == tblue)
        {
            recurseFill(num, x + 1, y1, fred, fgreen, fblue, tred, tgreen, tblue);
        }
        y1++;
    }
    y1 = y - 1;
    while(workingtextures[num]->getRed(x, y1) == fred && workingtextures[num]->getGreen(x, y1) == fgreen && workingtextures[num]->getBlue(x, y1) == fblue && y1 >= 0)
    {
        if(x < workingtextures[num]->getXSize() - 1 && workingtextures[num]->getRed(x+1, y1) == tred && workingtextures[num]->getGreen(x+1, y1) == tgreen && workingtextures[num]->getBlue(x+1, y1) == tblue)
        {
            recurseFill(num, x + 1, y1, fred, fgreen, fblue, tred, tgreen, tblue);
        }
        y1--;
    }

   return;
}

void TextureContainer::filledCircle(int num, int x, int y, int radius, int red, int green, int blue)
{
    static float sqrt_2_over_2 = sqrt(2.0) / 2.0;
    int x_position, y_position, d, delta_e, delta_se;
    x_position = -1;
    y_position = radius;
    d = 1 - radius;
    delta_e = -1;
    delta_se = (-radius << 1) + 3;
    int tlength;
    
    //printf("(line %d)\n", __LINE__);
    while (y_position > x_position) 
    {
        delta_e += 2;
        x_position++;

        //printf("(line %d)\n", __LINE__);
        if (d < 0) 
        {
            d += delta_e;
            delta_se += 2;
        }
        else 
        {
            d += delta_se;
            delta_se += 4;
            y_position--;
        }
        //printf("(line %d)\n", __LINE__);
        //printf("x_pos: %d,  y_pos: %d\n", x_position, y_position);
        //printf("x: %d,  y: %d\n", x, y);
        tlength = (x + x_position) - (x - x_position);
        this->setHStrip(num, x - x_position, tlength, y + y_position, red, green, blue);
        //printf("(line %d)\n", __LINE__);
        tlength = (x + x_position) - (x - x_position);
        this->setHStrip(num, x - x_position, tlength, y - y_position,red, green, blue);
        //printf("(line %d)\n", __LINE__);
        tlength = (y+x_position) - (y-x_position);
        this->setVStrip(num, y-x_position, tlength, x+y_position, red, green, blue);
        //printf("(line %d)\n", __LINE__);
        tlength = (y+x_position) - (y-x_position);
        this->setVStrip(num, y-x_position, tlength, x-y_position, red, green, blue);
        //printf("(line %d)\n", __LINE__);
    }
    //printf("(line %d)\n", __LINE__);
        this->filledRect(num, (int)(x + (sqrt_2_over_2 * radius)), (int)(y + (sqrt_2_over_2 * radius)), (int)(x - (sqrt_2_over_2 * radius)), (int)(y - (sqrt_2_over_2 * radius)), red, green, blue);
    //printf("(line %d)\n", __LINE__);
    return;
}
 
int TextureContainer::l_fill( lua_State* luaVM )
{
   TextureContainer* tc = (TextureContainer*)lua_touserdata(luaVM, lua_upvalueindex(1));
   int num = (int)lua_tonumber( luaVM, -6 );
   int x = (int)lua_tonumber( luaVM, -5 );
   int y = (int)lua_tonumber( luaVM, -4 );
   int red = (int)lua_tonumber( luaVM, -3 );
   int green = (int)lua_tonumber( luaVM, -2 );
   int blue = (int)lua_tonumber( luaVM, -1 );
   tc->fill2(num, x, y, red, green, blue);
   lua_pushnumber(luaVM, 1);
   return 1;
} 
       
void TextureContainer::apply3Convolution(int num, float* matrix, float divide, float bias, int passes, bool red, bool green, bool blue)
{
   PTexture* pt = new PTexture(workingtextures[num]->getXSize(), workingtextures[num]->getYSize());
   //PTexture* pt2 = new PTexture(workingtextures[num]->getXSize(), workingtextures[num]->getYSize());
   PTexture* pt2;
 //pt->copyFromTexture(workingtextures[num]);
 int xs = workingtextures[num]->getXSize();
 int ys = workingtextures[num]->getYSize();
 //printf("Matrix:\n%lf %lf %lf\n%lf %lf %lf\n%lf %lf %lf\n",
 //     matrix[0], matrix[1], matrix[2], matrix[3], matrix[4], matrix[5], matrix[6], matrix[7], matrix[8]);
 //printf("Divide: %lf  Bias: %lf\n", divide, bias);
 for(int ii = 0; ii < passes; ii++)
 {
   for(int yy = 0;yy < ys;yy++)
   {
      for(int xx = 0;xx < xs;xx++)
      {
         float outred = workingtextures[num]->getRedD(xx, yy);
         float outgreen = workingtextures[num]->getGreenD(xx, yy);
         float outblue = workingtextures[num]->getBlueD(xx, yy);
         if (red == true)
         {
            float summer = 0;
            summer += matrix[0]*(workingtextures[num]->getExtendRedD(xx-1, yy-1));
            summer += matrix[1]*(workingtextures[num]->getExtendRedD(xx, yy-1));
            summer += matrix[2]*(workingtextures[num]->getExtendRedD(xx+1, yy-1));
            summer += matrix[3]*(workingtextures[num]->getExtendRedD(xx-1, yy));
            summer += matrix[4]*(workingtextures[num]->getExtendRedD(xx, yy));
            summer += matrix[5]*(workingtextures[num]->getExtendRedD(xx+1, yy));
            summer += matrix[6]*(workingtextures[num]->getExtendRedD(xx-1, yy+1));
            summer += matrix[7]*(workingtextures[num]->getExtendRedD(xx, yy+1));
            summer += matrix[8]*(workingtextures[num]->getExtendRedD(xx+1, yy+1));
            summer /= divide;
            summer += bias;
            outred = summer;
         }
         if (green == true)
         {
            float summer = 0;
            summer += matrix[0]*(workingtextures[num]->getExtendGreenD(xx-1, yy-1));
            summer += matrix[1]*(workingtextures[num]->getExtendGreenD(xx, yy-1));
            summer += matrix[2]*(workingtextures[num]->getExtendGreenD(xx+1, yy-1));
            summer += matrix[3]*(workingtextures[num]->getExtendGreenD(xx-1, yy));
            summer += matrix[4]*(workingtextures[num]->getExtendGreenD(xx, yy));
            summer += matrix[5]*(workingtextures[num]->getExtendGreenD(xx+1, yy));
            summer += matrix[6]*(workingtextures[num]->getExtendGreenD(xx-1, yy+1));
            summer += matrix[7]*(workingtextures[num]->getExtendGreenD(xx, yy+1));
            summer += matrix[8]*(workingtextures[num]->getExtendGreenD(xx+1, yy+1));
            summer /= divide;
            summer += bias;
            outgreen = summer;
         }
         if (blue == true)
         {
            float summer = 0;
            summer += matrix[0]*(workingtextures[num]->getExtendBlueD(xx-1, yy-1));
            summer += matrix[1]*(workingtextures[num]->getExtendBlueD(xx, yy-1));
            summer += matrix[2]*(workingtextures[num]->getExtendBlueD(xx+1, yy-1));
            summer += matrix[3]*(workingtextures[num]->getExtendBlueD(xx-1, yy));
            summer += matrix[4]*(workingtextures[num]->getExtendBlueD(xx, yy));
            summer += matrix[5]*(workingtextures[num]->getExtendBlueD(xx+1, yy));
            summer += matrix[6]*(workingtextures[num]->getExtendBlueD(xx-1, yy+1));
            summer += matrix[7]*(workingtextures[num]->getExtendBlueD(xx, yy+1));
            summer += matrix[8]*(workingtextures[num]->getExtendBlueD(xx+1, yy+1));
            summer /= divide;
            summer += bias;
            outblue = summer;
         }
         pt->setColourD(xx, yy, outred, outgreen, outblue);
      }
   }
   
  pt2 = workingtextures[num];
  workingtextures[num] = pt;
  pt = pt2; 
  //workingtextures[num]->copyFromTexture(pt);
  //delete workingtextures[num]; 
  //workingtextures[num] = pt;
  }
  
  
  delete pt;
   return;
}

int TextureContainer::l_applyBlur( lua_State* luaVM )
{
   TextureContainer* tc = (TextureContainer*)lua_touserdata(luaVM, lua_upvalueindex(1));
   int num = (int)lua_tonumber( luaVM, -6 );
   float bias = (0.0 + (float)lua_tonumber( luaVM, -5 ));
   int passes = (int)lua_tonumber( luaVM, -4 );
   bool dored = (bool)lua_tonumber( luaVM, -3 );
   bool dogreen = (bool)lua_tonumber( luaVM, -2 );
   bool doblue = (bool)lua_tonumber( luaVM, -1 );
   float matrix[10];
   matrix[0] = 1; matrix[1] = 1; matrix[2] = 1;
   matrix[3] = 1; matrix[4] = 1; matrix[5] = 1;
   matrix[6] = 1; matrix[7] = 1; matrix[8] = 1;
   float divide = 9;
   tc->apply3Convolution(num, matrix, divide, bias, passes, dored, dogreen, doblue);
   lua_pushnumber(luaVM, 1);
   return 1;
}

int TextureContainer::l_applyDefocus( lua_State* luaVM )
{
   TextureContainer* tc = (TextureContainer*)lua_touserdata(luaVM, lua_upvalueindex(1));
   int num = (int)lua_tonumber( luaVM, -6 );
   float bias = (0.0 + (float)lua_tonumber( luaVM, -5 ));
   int passes = (int)lua_tonumber( luaVM, -4 );
   bool dored = (bool)lua_tonumber( luaVM, -3 );
   bool dogreen = (bool)lua_tonumber( luaVM, -2 );
   bool doblue = (bool)lua_tonumber( luaVM, -1 );
   float matrix[10];
   matrix[0] = 1; matrix[1] = 1; matrix[2] = 1;
   matrix[3] = 1; matrix[4] = -7; matrix[5] = 1;
   matrix[6] = 1; matrix[7] = 1; matrix[8] = 1;
   float divide = 1;
   tc->apply3Convolution(num, matrix, divide, bias, passes, dored, dogreen, doblue);
   lua_pushnumber(luaVM, 1);
   return 1;
}

int TextureContainer::l_applyEdgeDetect( lua_State* luaVM )
{
   TextureContainer* tc = (TextureContainer*)lua_touserdata(luaVM, lua_upvalueindex(1));
   int num = (int)lua_tonumber( luaVM, -6 );
   float bias = 50.0 + ((float)lua_tonumber( luaVM, -5 ));
   int passes = (int)lua_tonumber( luaVM, -4 );
   bool dored = (bool)lua_tonumber( luaVM, -3 );
   bool dogreen = (bool)lua_tonumber( luaVM, -2 );
   bool doblue = (bool)lua_tonumber( luaVM, -1 );
   float matrix[10];
   matrix[0] = -5; matrix[1] = -5; matrix[2] = -5;
   matrix[3] = -5; matrix[4] = 39; matrix[5] = -5;
   matrix[6] = -5; matrix[7] = -5; matrix[8] = -5;
   float divide = 1;
   tc->apply3Convolution(num, matrix, divide, bias, passes, dored, dogreen, doblue);
   lua_pushnumber(luaVM, 1);
   return 1;
}

int TextureContainer::l_applyGaussianBlur( lua_State* luaVM )
{
   TextureContainer* tc = (TextureContainer*)lua_touserdata(luaVM, lua_upvalueindex(1));
   int num = (int)lua_tonumber( luaVM, -6 );
   float bias = 0.0 + ((float)lua_tonumber( luaVM, -5 ));
   int passes = (int)lua_tonumber( luaVM, -4 );
   bool dored = (bool)lua_tonumber( luaVM, -3 );
   bool dogreen = (bool)lua_tonumber( luaVM, -2 );
   bool doblue = (bool)lua_tonumber( luaVM, -1 );
   float matrix[10];
   matrix[0] = 0; matrix[1] = 1; matrix[2] = 0;
   matrix[3] = 1; matrix[4] = 4; matrix[5] = 1;
   matrix[6] = 0; matrix[7] = 1; matrix[8] = 0;
   float divide = 8;
   tc->apply3Convolution(num, matrix, divide, bias, passes, dored, dogreen, doblue);
   lua_pushnumber(luaVM, 1);
   return 1;
}

void TextureContainer::fractionalNoise(int num, int seed, int lored, int logreen, int loblue, int hired, int higreen, int hiblue)
{
   Random* r;
   if (seed < 0) {r = new Random();}
   else {r = new Random(seed);}
   int xs = workingtextures[num]->getXSize();
   int ys = workingtextures[num]->getYSize();
   for (int yy = 0;yy < ys;yy++)
   {
      for (int xx = 0; xx < xs;xx++)
      {
         float t = r->nextDouble();
         int a = (int)(t*(hired-lored+1)+lored);
         int b = (int)(t*(higreen-logreen+1)+logreen);
         int c = (int)(t*(hiblue-loblue+1)+loblue);
         workingtextures[num]->setColour(xx, yy, a, b, c);
      }
   }
   delete r;
   return;
}

int TextureContainer::l_fractionalNoise( lua_State* luaVM )
{
   TextureContainer* tc = (TextureContainer*)lua_touserdata(luaVM, lua_upvalueindex(1));
   int num = (int)lua_tonumber( luaVM, -8 );
   int seed = (int)lua_tonumber( luaVM , -7);
   int lored = (int)lua_tonumber( luaVM, -6 );
   int logreen = (int)lua_tonumber( luaVM, -5 );
   int loblue = (int)lua_tonumber( luaVM, -4 );
   int hired = (int)lua_tonumber( luaVM, -3 );
   int higreen = (int)lua_tonumber( luaVM, -2 );
   int hiblue = (int)lua_tonumber( luaVM, -1 );
   tc->fractionalNoise(num, seed, lored, logreen, loblue, hired, higreen, hiblue);
   lua_pushnumber(luaVM, 1);
   return 1;
}

void TextureContainer::noise(int num, int seed, int lored, int logreen, int loblue, int hired, int higreen, int hiblue)
{
   Random* r;
   if (seed < 0) {r = new Random();}
   else {r = new Random(seed);}
   int xs = workingtextures[num]->getXSize();
   int ys = workingtextures[num]->getYSize();
   for (int yy = 0;yy < ys;yy++)
   {
      for (int xx = 0; xx < xs;xx++)
      {
         int a = r->nextInt(hired-lored+1)+lored;
         int b = r->nextInt(higreen-logreen+1)+logreen;
         int c = r->nextInt(hiblue-loblue+1)+loblue;
         workingtextures[num]->setColour(xx, yy, a, b, c);
      }
   }
   delete r;
   return;
}

int TextureContainer::l_noise( lua_State* luaVM )
{
   TextureContainer* tc = (TextureContainer*)lua_touserdata(luaVM, lua_upvalueindex(1));
   int num = (int)lua_tonumber( luaVM, -8 );
   int seed = (int)lua_tonumber( luaVM , -7);
   int lored = (int)lua_tonumber( luaVM, -6 );
   int logreen = (int)lua_tonumber( luaVM, -5 );
   int loblue = (int)lua_tonumber( luaVM, -4 );
   int hired = (int)lua_tonumber( luaVM, -3 );
   int higreen = (int)lua_tonumber( luaVM, -2 );
   int hiblue = (int)lua_tonumber( luaVM, -1 );
   tc->noise(num, seed, lored, logreen, loblue, hired, higreen, hiblue);
   lua_pushnumber(luaVM, 1);
   return 1;
}



void TextureContainer::closestColour(int num, float var, int red, int green, int blue)
{
   int xs = workingtextures[num]->getXSize();
   int ys = workingtextures[num]->getYSize();
   int *px = new int[xs*ys], *py = new int[xs*ys], *pc = new int[xs*ys];
   int np = 0;
   //float cbest;
   //float cbest;
   int cbest;
   float dx, dy, d, dbest;

  int tcolor = 0xFF000000
                | ((red   & 0xFF) << 16)
                | ((green & 0xFF) <<  8)
                | ((blue  & 0xFF)      );

   float fred, fgreen, fblue;

   for (int yy = 0;yy < ys;yy++)
   {
      for (int xx = 0; xx < xs;xx++)
      {
         fred = fabs(workingtextures[num]->getRedD(xx, yy)-red);
         fgreen = fabs(workingtextures[num]->getGreenD(xx, yy)-green);
         fblue = fabs(workingtextures[num]->getBlueD(xx, yy)-blue);
         if (fred > var || fgreen > var || fblue > var)
         {
            px[np] = xx;
            py[np] = yy;
            pc[np] = workingtextures[num]->getA8R8G8B8(xx, yy);
            np++;
         }
      }
   }

   for (int yy = 0;yy < ys;yy++)
   {
      for (int xx = 0; xx < xs;xx++)
      {
         dbest = 1.0E20;
         cbest = tcolor;
         for(int i=0;i<np;i++)
         {
            dx = xx-px[i];
            dy = yy-py[i];
            d = dx*dx+dy*dy;
            if (d < dbest)
            {
               cbest = pc[i];
               dbest = d;
            }
         }
         workingtextures[num]->setA8R8G8B8(xx, yy, cbest);
      }
   }


   delete px;
   delete py;
   delete pc;
   return;
}

void TextureContainer::setWithProbability(int num, int seed, float p,
   int red, int green, int blue)
{
   Random* r;
   if (seed < 0) {r = new Random();}
   else {r = new Random(seed);}
   int xs = workingtextures[num]->getXSize();
   int ys = workingtextures[num]->getYSize();
   int color = 0xFF000000
               | ((red   & 0xFF) << 16)
               | ((green & 0xFF) <<  8)
               | ((blue  & 0xFF)      );

   for (int yy = 0;yy < ys;yy++)
   {
      for (int xx = 0; xx < xs;xx++)
      {
         float t = r->nextDouble();
         if (t < p)
            workingtextures[num]->setA8R8G8B8(xx, yy, color);
      }
   }
   delete r;
   return;
}

void TextureContainer::perlinNoise(int num, float px, float py, float length, float persist, int octaves, int seed)
{
   Random* r;
   if (seed < 0) {r = new Random(); seed = r->nextInt(100000);}
   int xs = workingtextures[num]->getXSize();
   int ys = workingtextures[num]->getYSize();
   for (int yy = 0;yy < ys;yy++)
   {
      for (int xx = 0;xx < xs;xx++)
      {
         float xpos = px + (length * (((float)(xx))/((float)(xs))));
         float ypos = py + (length * (((float)(yy))/((float)(ys))));
         //float val = PerlinNoise_2D(xpos, ypos, persist, octaves, seed, 1);
         //printf("per: %lf, oct: %d, seed: %d\n", persist, octaves, seed);
         double val = PerlinNoise_2D((double)xpos, (double)ypos, (double)persist, octaves, seed, INTERPOLATION_QUADRATIC);
         val *= 255.0;
         //printf("v: %d\n", (int)val);
         workingtextures[num]->setColourD(xx, yy, (float)val, (float)val, (float)val);
      }
   }
   delete r;
   return;
}

int TextureContainer::l_perlinNoise(lua_State* luaVM)
{
   TextureContainer* tc = (TextureContainer*)lua_touserdata(luaVM, lua_upvalueindex(1));
   int num   = (int)lua_tonumber(luaVM, -7);
   float px   = (float)lua_tonumber(luaVM, -6);
   float py = (float)lua_tonumber(luaVM, -5);
   float length  = (float)lua_tonumber(luaVM, -4);
   float persist  = (float)lua_tonumber(luaVM, -3);
   int octaves   = (int)lua_tonumber(luaVM, -2);
   int seed   = (int)lua_tonumber(luaVM, -1);
   tc->perlinNoise(num, px, py, length, persist, octaves, seed);
   lua_pushnumber(luaVM, 1);
   return 1;
}


static inline int clampmul(int a, int b)
{
   int p = (a*b) >> 8;
   return (p <= 0xFF) ? p : 0xFF;
}

int TextureContainer::l_closestColour(lua_State* luaVM)
{
   TextureContainer* tc = (TextureContainer*)lua_touserdata(luaVM, lua_upvalueindex(1));
   int num   = (int)lua_tonumber(luaVM, -5);
   float var = (float)lua_tonumber(luaVM, -4);
   int red   = (int)lua_tonumber(luaVM, -3);
   int green = (int)lua_tonumber(luaVM, -2);
   int blue  = (int)lua_tonumber(luaVM, -1);
   tc->closestColour(num, var, red, green, blue);
   lua_pushnumber(luaVM, 1);
   return 1;
}

int TextureContainer::l_setWithProbability(lua_State* luaVM)
{
   TextureContainer* tc = (TextureContainer*)lua_touserdata(luaVM, lua_upvalueindex(1));
   int num   = (int) lua_tonumber(luaVM, -6);
   int seed  = (int) lua_tonumber(luaVM, -5);
   float p  =       lua_tonumber(luaVM, -4);
   int red   = (int) lua_tonumber(luaVM, -3);
   int green = (int) lua_tonumber(luaVM, -2);
   int blue  = (int) lua_tonumber(luaVM, -1);
   tc->setWithProbability(num, seed, p, red, green, blue);
   lua_pushnumber(luaVM, 1);
   return 1;
}

void TextureContainer::light(int num, int bump, float bumpheight, float lightx, float lighty, float lightz, float ambientbright, float lightbright, float falloffkmult, float falloff, bool shadows)
{
   int xs = workingtextures[num]->getXSize();
   int ys = workingtextures[num]->getYSize();
   float h1, h2, h3, h4, h5, h6, h7, h8, h9;
   float vx, vy, vz, tx, ty, tz;
   float vl;
   float nvx, nvy, nvz;
   float lx, ly, lz;
   float ll;
   float nlx, nly, nlz;
   float val; 
   float rt, bt, gt;
   float rn, gn, bn;
   float temp1, temp2, temp3, temp4, temp5;
   float temp6, temp7, temp8, temp9, temp10;
   float srtwo = 1.41421356;
   float fallk = 1*falloffkmult;
   
   for(int yy = 0;yy<ys;yy++)
   {
      for(int xx = 0;xx<xs;xx++)
      {
         h1 = bumpheight*(((workingtextures[bump]->getHeightD(xx, yy-1)))/255.0);
         h2 = bumpheight*(((workingtextures[bump]->getHeightD(xx-1, yy)))/255.0);
         h3 = bumpheight*(((workingtextures[bump]->getHeightD(xx, yy)))/255.0);
         h4 = bumpheight*(((workingtextures[bump]->getHeightD(xx+1, yy)))/255.0);
         h5 = bumpheight*(((workingtextures[bump]->getHeightD(xx, yy+1)))/255.0);
         h6 = bumpheight*(((workingtextures[bump]->getHeightD(xx-1, yy-1)))/255.0);
         h7 = bumpheight*(((workingtextures[bump]->getHeightD(xx+1, yy-1)))/255.0);
         h8 = bumpheight*(((workingtextures[bump]->getHeightD(xx-1, yy+1)))/255.0);
         h9 = bumpheight*(((workingtextures[bump]->getHeightD(xx+1, yy+1)))/255.0);
         
         
         //calculate the suface normal and the normalize it
         tx = (h2-h4)/2.0; ty = ((h1-h5))/2.0; tz = 1;
         temp1 = (h6-h3)/srtwo;temp2 = (h3-h7)/srtwo;
         temp3 = (h8-h3)/srtwo;temp4 = (h3-h9)/srtwo;
         temp5 = (temp1 + temp2 + temp3 + temp4) / 4.0;
         
         temp6 = temp1; temp7 = (h7 - h3)/srtwo;
         temp8 = (h3 - h8) / srtwo; temp9 = temp4;
         temp10 = (temp6 + temp7 + temp8 + temp9) / 4.0;
         
         vx = (tx + temp5) / 2.0; vy = (ty + temp10) / 2.0; vz = tz;
         //vx = tx; vy = ty; vz = tz;
         
         vl = sqrt((vx*vx)+(vy*vy)+vz);
         nvx = vx / vl; nvy = vy / vl; nvz = vz / vl;
         
         //NOT NEEDED
         //if (vx = 0) {nvx = 0;}
         //if (vy = 0) {nvx = 0;}
         
         //get light vector
         lx = lightx - xx;
         ly = lighty - yy;
         lz = lightz - h3;
         
         //normalize it
         ll = sqrt((lx*lx)+(ly*ly)+(lz*lz));
         nlx = lx / ll; nly = ly / ll; nlz = lz / ll;
         //if (lx = 0) {nlx = 0;}
         //if (ly = 0) {nlx = 0;}
         
         //dot them
         val = (nvx*nlx)+(nvy*nly)+(nvz*nlz);
         //printf("val: %lf", val);
         
         //generate new space colour
         rt = ((workingtextures[num]->getRedD(xx, yy)));
         gt = ((workingtextures[num]->getGreenD(xx, yy)));
         bt = ((workingtextures[num]->getBlueD(xx, yy)));
         rn = (rt*ambientbright)+((val*lightbright*rt*fallk)/pow(ll,falloff));
         gn = (gt*ambientbright)+((val*lightbright*gt*fallk)/pow(ll,falloff));
         bn = (bt*ambientbright)+((val*lightbright*bt*fallk)/pow(ll,falloff));
         workingtextures[num]->setColourD(xx, yy, rn, gn, bn);
      }
   }
   
   return;
}

int TextureContainer::l_light(lua_State *luaVM)
{
   TextureContainer* tc = (TextureContainer*)lua_touserdata(luaVM, lua_upvalueindex(1));
   int num1 = (int) lua_tonumber(luaVM, -11);
   int num2 = (int) lua_tonumber(luaVM, -10); //handle 2
   float bumpheight = (float)lua_tonumber(luaVM, -9);
   float lightx = (float)lua_tonumber(luaVM, -8);
   float lighty = (float)lua_tonumber(luaVM, -7);
   float lightz = (float)lua_tonumber(luaVM, -6);
   float ambientbright = (float)lua_tonumber(luaVM, -5);
   float lightbright = (float)lua_tonumber(luaVM, -4);
   float falloffkmult = (float)lua_tonumber(luaVM, -3);
   float falloff = (float)lua_tonumber(luaVM, -2);
   int shadows = (int)lua_tonumber(luaVM, -1);
   tc->light(num1, num2, bumpheight, lightx, lighty, lightz, ambientbright, lightbright, falloffkmult, falloff, shadows);
   lua_pushnumber(luaVM, 1);
   return 1;
}

void TextureContainer::add(int num1, int num2, int type)
{
   //type 0 = add wrap
   //type 1 = sub wrap
   //type 2 = add clamp
   //type 3 = sub clamp
   //type 4 = wrap mul
   //type 5 = straight mul
   //type 6 = tint mul
   //type 7 = diff
   //type 8 = average 
   //type 9 = square of the difference clamp
   //type 10 = square of the difference wrap
   int xs1 = workingtextures[num1]->getXSize();
   int ys1 = workingtextures[num1]->getYSize();
   int xs2 = workingtextures[num2]->getXSize();
   int ys2 = workingtextures[num2]->getYSize();
   int xs, ys;

   if (xs1 < xs2)
      xs = xs1;
   else
      xs = xs2;
   if (ys1 < ys2)
      ys = ys1;
   else
      ys = ys2;

   float r1, g1, b1;
   float r2, g2, b2;
   float rn, gn, bn;
   float te;
   
   for(int yy=0;yy<ys;yy++)
   {
      for(int xx=0;xx<xs;xx++)
      {
         r1 = workingtextures[num1]->getRedD(xx, yy);
         r2 = workingtextures[num2]->getRedD(xx, yy);
         g1 = workingtextures[num1]->getGreenD(xx, yy);
         g2 = workingtextures[num2]->getGreenD(xx, yy);
         b1 = workingtextures[num1]->getBlueD(xx, yy);
         b2 = workingtextures[num2]->getBlueD(xx, yy);
         
         if (type == 0)
         {
            rn = r1 + r2;
            gn = g1 + g2;
            bn = b1 + b2;
            workingtextures[num1]->setColourWrapD(xx, yy, rn, gn, bn);
         }
         
         else if (type == 1)
         {
            rn = r1 - r2;
            gn = g1 - g2;
            bn = b1 - b2;
            workingtextures[num1]->setColourWrapD(xx, yy, rn, gn, bn);
         }
         
         else if (type == 2)
         {
            rn = r1 + r2;
            gn = g1 + g2;
            bn = b1 + b2;
            workingtextures[num1]->setColourD(xx, yy, rn, gn, bn);
         }

         else if (type == 3)
         {
            rn = r1 - r2;
            gn = g1 - g2;
            bn = b1 - b2;
            workingtextures[num1]->setColourD(xx, yy, rn, gn, bn);
         }
            
         else if (type == 4)
         {
            rn = r1 * r2;
            gn = g1 * g2;
            bn = b1 * b2;
            workingtextures[num1]->setColourWrapD(xx, yy, rn, gn, bn);
         }
         
         else if (type == 5)
         {
            int c1 = workingtextures[num1]->getA8R8G8B8(xx, yy);
            int c2 = workingtextures[num2]->getA8R8G8B8(xx, yy);
            workingtextures[num1]->setA8R8G8B8(xx, yy,
            0xFF000000 |
            (clampmul((c1 >> 16) & 0xFF, (c2 >> 16) & 0xFF) << 16) |
            (clampmul((c1 >>  8) & 0xFF, (c2 >>  8) & 0xFF) <<  8) |
            (clampmul((c1      ) & 0xFF, (c2      ) & 0xFF)      ));
         }
         
         else if (type == 6)
         {
            rn = (((r1)/255.0)*(r2));
            gn = (((g1)/255.0)*(g2));
            bn = (((b1)/255.0)*(b2));
            workingtextures[num1]->setColourD(xx, yy, rn, gn, bn);
         }   
         
         else if (type == 7)
         {
            rn = fabs(r1-r2);
            gn = fabs(g1-g2);
            bn = fabs(b1-b2);
            workingtextures[num1]->setColourD(xx, yy, rn, gn, bn);
         }
         
         else if (type == 8)
         {
            rn = (r1+r2)/2.0;
            gn = (g1+g2)/2.0;
            bn = (b1+b2)/2.0;
            workingtextures[num1]->setColourD(xx, yy, rn, gn, bn);
         }
         
         else if (type == 9)
         {
            te = r1-r2;
            rn = te*te;
            te = g1-g2;
            gn = te*te;
            te = b1-b2;
            bn = te*te;
            workingtextures[num1]->setColourD(xx, yy, rn, gn, bn);
         }
         
         else if (type == 10)
         {
            te = r1-r2;
            rn = te*te;
            te = g1-g2;
            gn = te*te;
            te = b1-b2;
            bn = te*te;
            workingtextures[num1]->setColourWrapD(xx, yy, rn, gn, bn);
         }
            
      }
   }
   return;
}

int TextureContainer::l_add(lua_State *luaVM)
{
   TextureContainer* tc = (TextureContainer*)lua_touserdata(luaVM, lua_upvalueindex(1));
   int num1 = (int) lua_tonumber(luaVM, -3);
   int num2 = (int) lua_tonumber(luaVM, -2); //handle 2
   int type = (int)lua_tonumber(luaVM, -1);
   tc->add(num1, num2, type);
   lua_pushnumber(luaVM, 1);
   return 1;
}

void TextureContainer::line(int num, int x0, int y0, int x1, int y1, int red, int green, int blue)
   {
        int dy = y1 - y0;
        int dx = x1 - x0;
        int stepx, stepy;

        if (dy < 0) { dy = -dy;  stepy = -1; } else { stepy = 1; }
        if (dx < 0) { dx = -dx;  stepx = -1; } else { stepx = 1; }
        dy <<= 1;                                                  // dy is now 2*dy
        dx <<= 1;                                                  // dx is now 2*dx

        workingtextures[num]->setColour(x0, y0, red, green, blue);
        if (dx > dy) {
            int fraction = dy - (dx >> 1);                         // same as 2*dy - dx
            while (x0 != x1) {
                if (fraction >= 0) {
                    y0 += stepy;
                    fraction -= dx;                                // same as fraction -= 2*dx
                }
                x0 += stepx;
                fraction += dy;                                    // same as fraction -= 2*dy
                workingtextures[num]->setColour(x0, y0, red, green, blue);
            }
        } else {
            int fraction = dx - (dy >> 1);
            while (y0 != y1) {
                if (fraction >= 0) {
                    x0 += stepx;
                    fraction -= dy;
                }
                y0 += stepy;
                fraction += dx;
                workingtextures[num]->setColour(x0, y0, red, green, blue);
            }
        }
      return;
    }

int TextureContainer::l_line(lua_State *luaVM)
{
   TextureContainer* tc = (TextureContainer*)lua_touserdata(luaVM, lua_upvalueindex(1));
   int num = (int) lua_tonumber(luaVM, -8);
   int x0 = (int) lua_tonumber(luaVM, -7); //handle 2
   int y0 = (int) lua_tonumber(luaVM, -6);
   int x1 = (int) lua_tonumber(luaVM, -5);
   int y1 = (int) lua_tonumber(luaVM, -4);
   int red = (int)lua_tonumber(luaVM, -3);
   int green = (int) lua_tonumber(luaVM, -2);
   int blue = (int) lua_tonumber(luaVM, -1);
   tc->line(num, x0, y0, x1, y1, red, green, blue);
   lua_pushnumber(luaVM, 1);
   return 1;
}

void TextureContainer::rect(int num, int x1, int y1, int x2, int y2, int red, int green, int blue)
{
   line(num, x1, y1, x2, y1, red, green, blue);
   line(num, x1, y1, x1, y2, red, green, blue);
   line(num, x2, y1, x2, y2, red, green, blue);
   line(num, x1, y2, x2, y2, red, green, blue);
   return;
}

int TextureContainer::l_rect(lua_State *luaVM)
{
   TextureContainer* tc = (TextureContainer*)lua_touserdata(luaVM, lua_upvalueindex(1));
   int num = (int) lua_tonumber(luaVM, -8);
   int x1 = (int) lua_tonumber(luaVM, -7); //handle 2
   int y1 = (int) lua_tonumber(luaVM, -6);
   int x2 = (int) lua_tonumber(luaVM, -5);
   int y2 = (int) lua_tonumber(luaVM, -4);
   int red = (int)lua_tonumber(luaVM, -3);
   int green = (int) lua_tonumber(luaVM, -2);
   int blue = (int) lua_tonumber(luaVM, -1);
   tc->rect(num, x1, y1, x2, y2, red, green, blue);
   lua_pushnumber(luaVM, 1);
   return 1;
}

void TextureContainer::circle(int num, int x0, int y0, int radius, int red, int green, int blue)
 {
   int f = 1 - radius;
   int ddF_x = 0;
   int ddF_y = -2 * radius;
   int x = 0;
   int y = radius;
 
   workingtextures[num]->setColour(x0, y0 + radius, red, green, blue);
   workingtextures[num]->setColour(x0, y0 - radius, red, green, blue);
   workingtextures[num]->setColour(x0 + radius, y0, red, green, blue);
   workingtextures[num]->setColour(x0 - radius, y0, red, green, blue);
 
   while(x < y) 
   {
     if(f >= 0) 
     {
       y--;
       ddF_y += 2;
       f += ddF_y;
     }
     x++;
     ddF_x += 2;
     f += ddF_x + 1;    
     workingtextures[num]->setColour(x0 + x, y0 + y, red, green, blue);
     workingtextures[num]->setColour(x0 - x, y0 + y, red, green, blue);
     workingtextures[num]->setColour(x0 + x, y0 - y, red, green, blue);
     workingtextures[num]->setColour(x0 - x, y0 - y, red, green, blue);
     workingtextures[num]->setColour(x0 + y, y0 + x, red, green, blue);
     workingtextures[num]->setColour(x0 - y, y0 + x, red, green, blue);
     workingtextures[num]->setColour(x0 + y, y0 - x, red, green, blue);
     workingtextures[num]->setColour(x0 - y, y0 - x, red, green, blue);
   }
   return;
 }

int TextureContainer::l_circle(lua_State *luaVM)
{
   TextureContainer* tc = (TextureContainer*)lua_touserdata(luaVM, lua_upvalueindex(1));
   int num = (int) lua_tonumber(luaVM, -7);
   int x0 = (int) lua_tonumber(luaVM, -6); //handle 2
   int y0 = (int) lua_tonumber(luaVM, -5);
   int radius = (int) lua_tonumber(luaVM, -4);
   int red = (int)lua_tonumber(luaVM, -3);
   int green = (int) lua_tonumber(luaVM, -2);
   int blue = (int) lua_tonumber(luaVM, -1);
   tc->circle(num, x0, y0, radius, red, green, blue);
   lua_pushnumber(luaVM, 1);
   return 1;
}

void TextureContainer::colourReplace(int num, float var, int redrep, int greenrep, int bluerep, int red, int green, int blue)
{
   int xs = workingtextures[num]->getXSize();
   int ys = workingtextures[num]->getYSize();
   
   //int repcolor = 0xFF000000
   //             | ((redrep   & 0xFF) << 16)
    //            | ((greenrep & 0xFF) <<  8)
   //             | ((bluerep  & 0xFF)      );
                
      int tcolor = 0xFF000000
                | ((red   & 0xFF) << 16)
                | ((green & 0xFF) <<  8)
                | ((blue  & 0xFF)      );
      
      float fred, fgreen, fblue;
                
   
   for(int yy = 0;yy<ys;yy++)
   {
      for(int xx = 0;xx<xs;xx++)
      {
               fred = fabs(workingtextures[num]->getRedD(xx, yy)-redrep);
               fgreen = fabs(workingtextures[num]->getGreenD(xx, yy)-greenrep);
               fblue = fabs(workingtextures[num]->getBlueD(xx, yy)-bluerep);
         if (fred <= var && fgreen <= var && fblue <= var)
         {
            workingtextures[num]->setA8R8G8B8(xx, yy, tcolor);
         }
      }
   }
   return;
}

int TextureContainer::l_colourReplace(lua_State *luaVM)
{
   TextureContainer* tc = (TextureContainer*)lua_touserdata(luaVM, lua_upvalueindex(1));
   int num = (int) lua_tonumber(luaVM, -8);
   float var = (float)lua_tonumber(luaVM, -7);
   int redrep = (int) lua_tonumber(luaVM, -6); //handle 2
   int greenrep = (int) lua_tonumber(luaVM, -5);
   int bluerep = (int) lua_tonumber(luaVM, -4);
   int red = (int)lua_tonumber(luaVM, -3);
   int green = (int) lua_tonumber(luaVM, -2);
   int blue = (int) lua_tonumber(luaVM, -1);
   tc->colourReplace(num, var, redrep, greenrep, bluerep, red, green, blue);
   lua_pushnumber(luaVM, 1);
   return 1;
}

void TextureContainer::overlay(int num1, int num2, float var, int tred, int tgreen, int tblue)
{
   int xs1 = workingtextures[num1]->getXSize();
   int ys1 = workingtextures[num1]->getYSize();
   int xs2 = workingtextures[num2]->getXSize();
   int ys2 = workingtextures[num2]->getYSize();
   int xs, ys;
   

   if (xs1 < xs2)
      xs = xs1;
   else
      xs = xs2;
   if (ys1 < ys2)
      ys = ys1;
   else
      ys = ys2;

      //int tcolor = 0xFF000000
      //          | ((tred   & 0xFF) << 16)
      //          | ((tgreen & 0xFF) <<  8)
      //          | ((tblue  & 0xFF)      );

   int temp1;
   float fred, fgreen, fblue;

   for(int yy=0;yy<ys;yy++)
   {
      for(int xx=0;xx<xs;xx++)
      {
         fred = fabs(workingtextures[num2]->getRedD(xx, yy)-tred);
         fgreen = fabs(workingtextures[num2]->getGreenD(xx, yy)-tgreen);
         fblue = fabs(workingtextures[num2]->getBlueD(xx, yy)-tblue);
         temp1 = workingtextures[num2]->getA8R8G8B8(xx, yy);
         if (fred > var || fgreen > var || fblue > var)
         {
            workingtextures[num1]->setA8R8G8B8(xx, yy, temp1);
         }
      }
   }
   
   return;
}

int TextureContainer::l_overlay(lua_State *luaVM)
{
   TextureContainer* tc = (TextureContainer*)lua_touserdata(luaVM, lua_upvalueindex(1));
   int num1 = (int) lua_tonumber(luaVM, -6);
   int num2 = (int) lua_tonumber(luaVM, -5);
   float var = (float)lua_tonumber(luaVM, -4);
   int tred = (int) lua_tonumber(luaVM, -3); //handle 2
   int tgreen = (int) lua_tonumber(luaVM, -2);
   int tblue = (int) lua_tonumber(luaVM, -1);
   tc->overlay(num1, num2, var, tred, tgreen, tblue);
   lua_pushnumber(luaVM, 1);
   return 1;
}

/*void TextureContainer::pixelFit(int num, float probmult, int seedstart, int seedend, int red, int green, int blue)
{
   int xs = workingtextures[num]->getXSize();
   int ys = workingtextures[num]->getYSize();
   int elems = xs*ys;
   int scounter = 0;
   int counter = 0;
   TextureContainer* tc = new TextureContainer();
   tc->addNewWorkingTexture(xs, ys); 
   
   int tcolor = 0xFF000000
      | ((red   & 0xFF) << 16)
      | ((green & 0xFF) <<  8)
      | ((blue  & 0xFF)      );
     
   for (int yy = 0;yy<ys;yy++)
   {
      for (int xx = 0;xx<xs;xx++)
      {
         if (workingtextures[num]->getA8R8G8B8(xx, yy) == tcolor)
         {
            scounter++;
         }
      }
   }
   
   float prob = ((float)scounter) / ((float)elems);
   prob *= probmult;
   int switched = 0;
   
   counter  = 0;
   
   for(int theseed = seedstart;theseed <= seedend;theseed++)
   {
      counter = 0;
      switched = 0;
      tc->setWithProbability(0, theseed, prob, red, green, blue);
      for (int yy = 0;yy<ys;yy++)
      {
         for (int xx = 0;xx<xs;xx++)
         {
            if (tc->workingtextures[0]->getA8R8G8B8(xx, yy) == tcolor)
            {
               if(tc->workingtextures[0]->getA8R8G8B8(xx, yy) == workingtextures[num]->getA8R8G8B8(xx, yy))
               {
                  counter++;
               }
               else
               {
                  switched = 1;
               }
            }
         }
      }
      if (switched == 0)
      {
         printf("seed: %d, prob: %lf, fit: %lf\n", theseed, prob, ((float)counter)/((float)elems));
      }
      tc->clearTexture(0, 0, 0, 0);
   }
   delete tc;
   return;
}

int TextureContainer::l_pixelFit(lua_State *luaVM)
{
   TextureContainer* tc = (TextureContainer*)lua_touserdata(luaVM, lua_upvalueindex(1));
   int num = (int) lua_tonumber(luaVM, -7);
   float probmult = (float) lua_tonumber(luaVM, -6);
   int seedstart = (int) lua_tonumber(luaVM, -5); //handle 2
   int seedend = (int) lua_tonumber(luaVM, -4); //handle 2
   int red = (int) lua_tonumber(luaVM, -3); //handle 2
   int green = (int) lua_tonumber(luaVM, -2);
   int blue = (int) lua_tonumber(luaVM, -1);
   tc->pixelFit(num, probmult, seedstart, seedend, red, green, blue);
   lua_pushnumber(luaVM, 1);
   return 1;
}*/

int TextureContainer::l_applyEmboss( lua_State* luaVM )
{
   TextureContainer* tc = (TextureContainer*)lua_touserdata(luaVM, lua_upvalueindex(1));
   int num = (int)lua_tonumber( luaVM, -6 );
   float bias = 128 + ((float)lua_tonumber( luaVM, -5 ));
   int passes = (int)lua_tonumber( luaVM, -4 );
   bool dored = (bool)lua_tonumber( luaVM, -3 );
   bool dogreen = (bool)lua_tonumber( luaVM, -2 );
   bool doblue = (bool)lua_tonumber( luaVM, -1 );
   float matrix[10];
   matrix[0] = -1; matrix[1] = -1; matrix[2] = 0;
   matrix[3] = -1; matrix[4] = 0; matrix[5] = 1;
   matrix[6] = 0; matrix[7] = 1; matrix[8] = 1;
   float divide = 1;
   tc->apply3Convolution(num, matrix, divide, bias, passes, dored, dogreen, doblue);
   lua_pushnumber(luaVM, 1);
   return 1;
}

int TextureContainer::l_applyEdgeDetectLaplace( lua_State* luaVM )
{
   TextureContainer* tc = (TextureContainer*)lua_touserdata(luaVM, lua_upvalueindex(1));
   int num = (int)lua_tonumber( luaVM, -6 );
   float bias = 0 + ((float)lua_tonumber( luaVM, -5 ));
   int passes = (int)lua_tonumber( luaVM, -4 );
   bool dored = (bool)lua_tonumber( luaVM, -3 );
   bool dogreen = (bool)lua_tonumber( luaVM, -2 );
   bool doblue = (bool)lua_tonumber( luaVM, -1 );
   float matrix[10];
   matrix[0] = 0; matrix[1] = 1; matrix[2] = 0;
   matrix[3] = 1; matrix[4] = -4; matrix[5] = 1;
   matrix[6] = 0; matrix[7] = 1; matrix[8] = 0;
   float divide = 1;
   tc->apply3Convolution(num, matrix, divide, bias, passes, dored, dogreen, doblue);
   lua_pushnumber(luaVM, 1);
   return 1;
}

void TextureContainer::directionLight(int num, int bump, float bumpheight, float lx, float ly, float lz, float ambientbright, float lightbright, bool shadows)
{
   int xs = workingtextures[num]->getXSize();
   int ys = workingtextures[num]->getYSize();
   float h1, h2, h3, h4, h5, h6, h7, h8, h9;
   float vx, vy, vz, tx, ty, tz;
   float vl;
   float nvx, nvy, nvz;
   //float lx, ly, lz;
   float ll;
   float nlx, nly, nlz;
   float val;
   float rt, bt, gt;
   float rn, gn, bn;
   float temp1, temp2, temp3, temp4, temp5;
   float temp6, temp7, temp8, temp9, temp10;
   float srtwo = 1.41421356;
   
   float lsx, lsy, lsz;

   for(int yy = 0;yy<ys;yy++)
   {
      for(int xx = 0;xx<xs;xx++)
      {
         h1 = bumpheight*(((workingtextures[bump]->getHeight(xx, yy-1)))/255.0);
         h2 = bumpheight*(((workingtextures[bump]->getHeight(xx-1, yy)))/255.0);
         h3 = bumpheight*(((workingtextures[bump]->getHeight(xx, yy)))/255.0);
         h4 = bumpheight*(((workingtextures[bump]->getHeight(xx+1, yy)))/255.0);
         h5 = bumpheight*(((workingtextures[bump]->getHeight(xx, yy+1)))/255.0);
         h6 = bumpheight*(((workingtextures[bump]->getHeight(xx-1, yy-1)))/255.0);
         h7 = bumpheight*(((workingtextures[bump]->getHeight(xx+1, yy-1)))/255.0);
         h8 = bumpheight*(((workingtextures[bump]->getHeight(xx-1, yy+1)))/255.0);
         h9 = bumpheight*(((workingtextures[bump]->getHeight(xx+1, yy+1)))/255.0);


         //calculate the suface normal and the normalize it
         tx = (h2-h4)/2.0; ty = (h1-h5)/2.0; tz = 1;
         temp1 = (h6-h3)/srtwo;temp2 = (h3-h7)/srtwo;
         temp3 = (h8-h3)/srtwo;temp4 = (h3-h9)/srtwo;
         temp5 = (temp1 + temp2 + temp3 + temp4) / 4.0;

         temp6 = temp1; temp7 = (h7 - h3)/srtwo;
         temp8 = (h3 - h8) / srtwo; temp9 = temp4;
         temp10 = (temp6 + temp7 + temp8 + temp9) / 4.0;

         vx = (tx + temp5) / 2.0; vy = (ty + temp10) / 2.0; vz = tz;
         //vx = tx; vy = ty; vz = tz;

         vl = sqrt((vx*vx)+(vy*vy)+vz);
         nvx = vx / vl; nvy = vy / vl; nvz = vz / vl;

         //NOT NEEDED
         //if (vx = 0) {nvx = 0;}
         //if (vy = 0) {nvx = 0;}

         //get light vector
         lsx = -lx;
         lsy = -ly;
         lsz = -lz;
         //lx = lightx - xx;
         //ly = lighty - yy;
         //lz = lightz - h3;

         //normalize it
         ll = sqrt((lsx*lsx)+(lsy*lsy)+(lsz*lsz));
         nlx = lsx / ll; nly = lsy / ll; nlz = lsz / ll;
         //if (lx = 0) {nlx = 0;}
         //if (ly = 0) {nlx = 0;}

         //dot them
         val = (nvx*nlx)+(nvy*nly)+(nvz*nlz);
         //printf("val: %lf", val);

         //generate new space colour
         rt = ((workingtextures[num]->getRedD(xx, yy)));
         gt = ((workingtextures[num]->getGreenD(xx, yy)));
         bt = ((workingtextures[num]->getBlueD(xx, yy)));
         rn = (rt*ambientbright)+((val*lightbright*rt));
         gn = (gt*ambientbright)+(val*lightbright*gt);
         bn = (bt*ambientbright)+(val*lightbright*bt);
         workingtextures[num]->setColourD(xx, yy, rn, gn, bn);
      }
   }

   return;
}

int TextureContainer::l_directionLight(lua_State *luaVM)
{
   TextureContainer* tc = (TextureContainer*)lua_touserdata(luaVM, lua_upvalueindex(1));
   int num1 = (int) lua_tonumber(luaVM, -9);
   int num2 = (int) lua_tonumber(luaVM, -8); //handle 2
   float bumpheight = (float)lua_tonumber(luaVM, -7);
   float lightx = (float)lua_tonumber(luaVM, -6);
   float lighty = (float)lua_tonumber(luaVM, -5);
   float lightz = (float)lua_tonumber(luaVM, -4);
   float ambientbright = (float)lua_tonumber(luaVM, -3);
   float lightbright = (float)lua_tonumber(luaVM, -2);
   int shadows = (int)lua_tonumber(luaVM, -1);
   tc->directionLight(num1, num2, bumpheight, lightx, lighty, lightz, ambientbright, lightbright, shadows);
   lua_pushnumber(luaVM, 1);
   return 1;
}

void TextureContainer::clampBelow(int num, float belowvalue, float tored, float togreen, float toblue)
{
   int xs = workingtextures[num]->getXSize();
   int ys = workingtextures[num]->getYSize();
   float r = 0;
   float g = 0;
   float b = 0;
   for(int yy = 0;yy < ys;yy++)
   {
      for(int xx = 0;xx < xs;xx++)
      {
         r = workingtextures[num]->getRedD(xx, yy);
         g = workingtextures[num]->getGreenD(xx, yy);
         b = workingtextures[num]->getBlueD(xx, yy);
         if (r<belowvalue) {r = tored;}
         if (g<belowvalue) {g = togreen;}
         if (b<belowvalue) {b = toblue;}
         workingtextures[num]->setColourD(xx, yy, r, g, b);
      }
   }
   return;
}

void TextureContainer::clampAbove(int num, float abovevalue, float tored, float togreen, float toblue)
{
   int xs = workingtextures[num]->getXSize();
   int ys = workingtextures[num]->getYSize();
   float r = 0;
   float g = 0;
   float b = 0;
   for(int yy = 0;yy < ys;yy++)
   {
      for(int xx = 0;xx < xs;xx++)
      {
         r = workingtextures[num]->getRedD(xx, yy);
         g = workingtextures[num]->getGreenD(xx, yy);
         b = workingtextures[num]->getBlueD(xx, yy);
         if (r>abovevalue) {r = tored;}
         if (g>abovevalue) {g = togreen;}
         if (b>abovevalue) {b = toblue;}
         workingtextures[num]->setColourD(xx, yy, r, g, b);
      }
   }
   return;
}

int TextureContainer::l_clampBelow(lua_State *luaVM)
{
   TextureContainer* tc = (TextureContainer*)lua_touserdata(luaVM, lua_upvalueindex(1));
   int num = (int) lua_tonumber(luaVM, -5);
   float belowvalue = (float)lua_tonumber(luaVM, -4);
   float tored = (float)lua_tonumber(luaVM, -3);
   float togreen = (float)lua_tonumber(luaVM, -2);
   float toblue = (float)lua_tonumber(luaVM, -1);
   tc->clampBelow(num, belowvalue, tored, togreen, toblue);
   lua_pushnumber(luaVM, 1);
   return 1;
}

int TextureContainer::l_clampAbove(lua_State *luaVM)
{
   TextureContainer* tc = (TextureContainer*)lua_touserdata(luaVM, lua_upvalueindex(1));
   int num = (int) lua_tonumber(luaVM, -5);
   float abovevalue = (float)lua_tonumber(luaVM, -4);
   float tored = (float)lua_tonumber(luaVM, -3);
   float togreen = (float)lua_tonumber(luaVM, -2);
   float toblue = (float)lua_tonumber(luaVM, -1);
   tc->clampAbove(num, abovevalue, tored, togreen, toblue);
   lua_pushnumber(luaVM, 1);
   return 1;
}

void TextureContainer::brightnessMult(int num, float scale)
{
   int xs = workingtextures[num]->getXSize();
   int ys = workingtextures[num]->getYSize();
   float r = 0;
   float g = 0;
   float b = 0;
   for(int yy = 0;yy < ys;yy++)
   {
      for(int xx = 0;xx < xs;xx++)
      {
         r = workingtextures[num]->getRedD(xx, yy)*scale;
         g = workingtextures[num]->getGreenD(xx, yy)*scale;
         b = workingtextures[num]->getBlueD(xx, yy)*scale;
         workingtextures[num]->setColourD(xx, yy, r, g, b);
      }
   }
   return;
}

int TextureContainer::l_brightnessMult(lua_State *luaVM)
{
   TextureContainer* tc = (TextureContainer*)lua_touserdata(luaVM, lua_upvalueindex(1));
   int num = (int) lua_tonumber(luaVM, -2);
   float scale = (float)lua_tonumber(luaVM, -1);
   tc->brightnessMult(num, scale);
   lua_pushnumber(luaVM, 1);
   return 1;
}

float TextureContainer::getRed(int num, int x, int y)
{
   return workingtextures[num]->getRedD(x, y);
}

float TextureContainer::getGreen(int num, int x, int y)
{
   return workingtextures[num]->getGreenD(x, y);
}

float TextureContainer::getBlue(int num, int x, int y)
{
   return workingtextures[num]->getBlueD(x, y);
}

int TextureContainer::l_getRed(lua_State *luaVM)
{
   TextureContainer* tc = (TextureContainer*)lua_touserdata(luaVM, lua_upvalueindex(1));
   int num = (int) lua_tonumber(luaVM, -3);
   int x = (int) lua_tonumber(luaVM, -2);
   int y = (int) lua_tonumber(luaVM, -1);
   float result = tc->getRed(num, x, y);
   lua_pushnumber(luaVM, result);
   return 1;
}

int TextureContainer::l_getGreen(lua_State *luaVM)
{
   TextureContainer* tc = (TextureContainer*)lua_touserdata(luaVM, lua_upvalueindex(1));
   int num = (int) lua_tonumber(luaVM, -3);
   int x = (int) lua_tonumber(luaVM, -2);
   int y = (int) lua_tonumber(luaVM, -1);
   float result = tc->getGreen(num, x, y);
   lua_pushnumber(luaVM, result);
   return 1;
}

int TextureContainer::l_getBlue(lua_State *luaVM)
{
   TextureContainer* tc = (TextureContainer*)lua_touserdata(luaVM, lua_upvalueindex(1));
   int num = (int) lua_tonumber(luaVM, -3);
   int x = (int) lua_tonumber(luaVM, -2);
   int y = (int) lua_tonumber(luaVM, -1);
   float result = tc->getBlue(num, x, y);
   lua_pushnumber(luaVM, result);
   return 1;
}

void TextureContainer::makeNormalMap(int num, float bumpheight)
{
   int xs = workingtextures[num]->getXSize();
   int ys = workingtextures[num]->getYSize();
   //printf("num: %d, bumpheight: %lf, xs: %d, ys: %d\n", num, bumpheight, xs, ys);
   float h1, h2, h3, h4, h5, h6, h7, h8, h9;
   float vx, vy, vz, tx, ty, tz;
   float vl;
   float rn, gn, bn;
   float nvx, nvy, nvz;
   float temp1, temp2, temp3, temp4, temp5;
   float temp6, temp7, temp8, temp9, temp10;
   float srtwo = 1.41421356;
   PTexture* pt = new PTexture(xs, ys);
   pt->copyFromTexture(workingtextures[num]);

   for(int yy = 0;yy<ys;yy++)
   {
      for(int xx = 0;xx<xs;xx++)
      {
         h1 = bumpheight*(((pt->getHeightD(xx, yy-1)))/255.0);
         h2 = bumpheight*(((pt->getHeightD(xx-1, yy)))/255.0);
         h3 = bumpheight*(((pt->getHeightD(xx, yy)))/255.0);
         h4 = bumpheight*(((pt->getHeightD(xx+1, yy)))/255.0);
         h5 = bumpheight*(((pt->getHeightD(xx, yy+1)))/255.0);
         h6 = bumpheight*(((pt->getHeightD(xx-1, yy-1)))/255.0);
         h7 = bumpheight*(((pt->getHeightD(xx+1, yy-1)))/255.0);
         h8 = bumpheight*(((pt->getHeightD(xx-1, yy+1)))/255.0);
         h9 = bumpheight*(((pt->getHeightD(xx+1, yy+1)))/255.0);

         //calculate the suface normal and the normalize it
         tx = (h2-h4)/2.0; ty = ((h1-h5))/2.0; tz = 1;
         temp1 = (h6-h3)/srtwo;temp2 = (h3-h7)/srtwo;
         temp3 = (h8-h3)/srtwo;temp4 = (h3-h9)/srtwo;
         temp5 = (temp1 + temp2 + temp3 + temp4) / 4.0;

         temp6 = temp1; temp7 = (h7 - h3)/srtwo;
         temp8 = (h3 - h8) / srtwo; temp9 = temp4;
         temp10 = (temp6 + temp7 + temp8 + temp9) / 4.0;

         vx = (tx + temp5) / 2.0; vy = (ty + temp10) / 2.0; vz = tz;

         vl = sqrt((vx*vx)+(vy*vy)+vz);
         nvx = vx / vl; nvy = vy / vl; nvz = vz / vl;
         //printf("nvx: %lf, nvy: %lf, nvz: %lf\n", nvx, nvy, nvz);
         
         rn = ((nvx+1.0)*255.0)/2.0;
         gn = ((nvy+1.0)*255.0)/2.0;
         bn = ((nvz+1.0)*255.0)/2.0;

         workingtextures[num]->setColourD(xx, yy, rn, gn, bn);
      }
   }
   delete pt;
   return;
}

int TextureContainer::l_makeNormalMap(lua_State *luaVM)
{
   TextureContainer* tc = (TextureContainer*)lua_touserdata(luaVM, lua_upvalueindex(1));
   int num = (int) lua_tonumber(luaVM, -2);
   printf("n: %d\n", num);
   float bumpheight = (float)lua_tonumber(luaVM, -1);
   tc->makeNormalMap(num, bumpheight);
   lua_pushnumber(luaVM, 1);
   return 1;
}

void TextureContainer::directionLightNormal(int num, int normal, float lx, float ly, float lz, float ambientbright, float lightbright, bool shadows)
{
   int xs = workingtextures[num]->getXSize();
   int ys = workingtextures[num]->getYSize();
   //float lx, ly, lz;
   float nvx, nvy, nvz;
   float ll;
   float nlx, nly, nlz;
   float val;
   float rt, bt, gt;
   float rn, gn, bn;
   float temp1, temp2, temp3, temp4, temp5;
   float temp6, temp7, temp8, temp9, temp10;
   float srtwo = 1.41421356;

   float lsx, lsy, lsz;

   for(int yy = 0;yy<ys;yy++)
   {
      for(int xx = 0;xx<xs;xx++)
      {
         nvx = ((workingtextures[normal]->getRedD(xx, yy)*2)/255.0)-1.0;
         nvy = ((workingtextures[normal]->getGreenD(xx, yy)*2)/255.0)-1.0;
         nvz = ((workingtextures[normal]->getBlueD(xx, yy)*2)/255.0)-1.0;


         //NOT NEEDED
         //if (vx = 0) {nvx = 0;}
         //if (vy = 0) {nvx = 0;}

         //get light vector
         lsx = -lx;
         lsy = -ly;
         lsz = -lz;
         //lx = lightx - xx;
         //ly = lighty - yy;
         //lz = lightz - h3;

         //normalize it
         ll = sqrt((lsx*lsx)+(lsy*lsy)+(lsz*lsz));
         nlx = lsx / ll; nly = lsy / ll; nlz = lsz / ll;
         //if (lx = 0) {nlx = 0;}
         //if (ly = 0) {nlx = 0;}

         //dot them
         val = (nvx*nlx)+(nvy*nly)+(nvz*nlz);
         //printf("val: %lf", val);

         //generate new space colour
         rt = ((workingtextures[num]->getRedD(xx, yy)));
         gt = ((workingtextures[num]->getGreenD(xx, yy)));
         bt = ((workingtextures[num]->getBlueD(xx, yy)));
         rn = (rt*ambientbright)+((val*lightbright*rt));
         gn = (gt*ambientbright)+(val*lightbright*gt);
         bn = (bt*ambientbright)+(val*lightbright*bt);
         workingtextures[num]->setColourD(xx, yy, rn, gn, bn);
      }
   }

   return;
}

int TextureContainer::l_directionLightNormal(lua_State *luaVM)
{
   TextureContainer* tc = (TextureContainer*)lua_touserdata(luaVM, lua_upvalueindex(1));
   int num1 = (int) lua_tonumber(luaVM, -8);
   int num2 = (int) lua_tonumber(luaVM, -7); //handle 2
   float lightx = (float)lua_tonumber(luaVM, -6);
   float lighty = (float)lua_tonumber(luaVM, -5);
   float lightz = (float)lua_tonumber(luaVM, -4);
   float ambientbright = (float)lua_tonumber(luaVM, -3);
   float lightbright = (float)lua_tonumber(luaVM, -2);
   int shadows = (int)lua_tonumber(luaVM, -1);
   tc->directionLightNormal(num1, num2, lightx, lighty, lightz, ambientbright, lightbright, shadows);
   lua_pushnumber(luaVM, 1);
   return 1;
}

void TextureContainer::lightNormal(int num, int normal, float lightx, float lighty, float lightz, float ambientbright, float lightbright, float falloffkmult, float falloff, bool shadows)
{
   int xs = workingtextures[num]->getXSize();
   int ys = workingtextures[num]->getYSize();
   float nvx, nvy, nvz;
   float lx, ly, lz;
   float ll;
   float nlx, nly, nlz;
   float val;
   float rt, bt, gt;
   float rn, gn, bn;
   float temp1, temp2, temp3, temp4, temp5;
   float temp6, temp7, temp8, temp9, temp10;
   float srtwo = 1.41421356;
   float fallk = 1*falloffkmult;

   for(int yy = 0;yy<ys;yy++)
   {
      for(int xx = 0;xx<xs;xx++)
      {
         nvx = ((workingtextures[normal]->getRedD(xx, yy)*2)/255.0)-1.0;
         nvy = ((workingtextures[normal]->getGreenD(xx, yy)*2)/255.0)-1.0;
         nvz = ((workingtextures[normal]->getBlueD(xx, yy)*2)/255.0)-1.0;

         //NOT NEEDED
         //if (vx = 0) {nvx = 0;}
         //if (vy = 0) {nvx = 0;}

         //get light vector
         lx = lightx - xx;
         ly = lighty - yy;
         lz = lightz;

         //normalize it
         ll = sqrt((lx*lx)+(ly*ly)+(lz*lz));
         nlx = lx / ll; nly = ly / ll; nlz = lz / ll;
         //if (lx = 0) {nlx = 0;}
         //if (ly = 0) {nlx = 0;}

         //dot them
         val = (nvx*nlx)+(nvy*nly)+(nvz*nlz);
         //printf("val: %lf", val);

         //generate new space colour
         rt = ((workingtextures[num]->getRedD(xx, yy)));
         gt = ((workingtextures[num]->getGreenD(xx, yy)));
         bt = ((workingtextures[num]->getBlueD(xx, yy)));
         rn = (rt*ambientbright)+((val*lightbright*rt*fallk)/pow(ll,falloff));
         gn = (gt*ambientbright)+((val*lightbright*gt*fallk)/pow(ll,falloff));
         bn = (bt*ambientbright)+((val*lightbright*bt*fallk)/pow(ll,falloff));
         workingtextures[num]->setColourD(xx, yy, rn, gn, bn);
      }
   }

   return;
}

int TextureContainer::l_lightNormal(lua_State *luaVM)
{
   TextureContainer* tc = (TextureContainer*)lua_touserdata(luaVM, lua_upvalueindex(1));
   int num1 = (int) lua_tonumber(luaVM, -10);
   int num2 = (int) lua_tonumber(luaVM, -9); //handle 2
   float lightx = (float)lua_tonumber(luaVM, -8);
   float lighty = (float)lua_tonumber(luaVM, -7);
   float lightz = (float)lua_tonumber(luaVM, -6);
   float ambientbright = (float)lua_tonumber(luaVM, -5);
   float lightbright = (float)lua_tonumber(luaVM, -4);
   float falloffkmult = (float)lua_tonumber(luaVM, -3);
   float falloff = (float)lua_tonumber(luaVM, -2);
   int shadows = (int)lua_tonumber(luaVM, -1);
   tc->lightNormal(num1, num2, lightx, lighty, lightz, ambientbright, lightbright, falloffkmult, falloff, shadows);
   lua_pushnumber(luaVM, 1);
   return 1;
}
