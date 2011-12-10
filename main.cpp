#include <cstdlib>
#include <iostream>
#include <windows.h>
#include "ptexture.h"
#include "texturecontainer.h"
#include <irrlicht.h>
#include "irrevent.h"
#include "timesensor.h"
#include "pngwriter.h"


using namespace std;
using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;
#pragma comment(lib, "Irrlicht.lib")

char luafile[512];
CIrrLichtEvent irrevent;
double movespeed = .1;
vector<ITexture*> textvect;
vector<ITexture*> wtextvect;
ISceneNode* node;
int tnum = 0;
wchar_t buf[80];
char temp[512];
char luaftemp[512];
bool rexec = false;
bool sexec = false;
bool fexec = false;
bool wexec = false;
bool periodexec = false;
bool commaexec = false;
bool mexec = false;
int viewtype = 0;
TextureContainer* tc = new TextureContainer();

void fillTextureVector(IVideoDriver* driver)
{
    //int handle = tc->addNewWorkingTexture(64, 64);
    //tc->setPixel(handle, 4, 4, 170, 255, 2);
    tc->clearTextureContainer();  
    tc->loadLua(luafile);
    //printf("Working A8R8G8B8: %x\n", tc->getWorkingA8R8G8B8(handle, 4, 4));
  
     //for (int i = 0;i < textvect.size();i++)
     //{
     //   delete textvect[i];
     //}
     driver->removeAllTextures();
     textvect.clear();
     wtextvect.clear();
     //printf("HHHHHHHHHHHHHHH\n");
  
    for (int v = 0;v < tc->getFinalNumber();v++)
    {
       textvect.push_back(driver->addTexture(dimension2d<s32>(tc->getFinalXSize(v), tc->getFinalYSize(v)), itoa(v, temp, 10)));
       //printf("\nto lock\n");
       s32* sp = (s32*)textvect[v]->lock();
       for (int xx = 0;xx < tc->getFinalXSize(v);xx++)
       {
            for (int yy = 0;yy < tc->getFinalYSize(v);yy++)
            {
               sp[yy * tc->getFinalXSize(v) + xx] = tc->getFinalA8R8G8B8(v, xx, yy);
            //printf("hex: %x\n", tc->getFinalA8R8G8B8(0, xx, yy));
            }
       }
       //printf("\nto unlock\n");
       textvect[v]->unlock();
       //printf("\nto mipmap\n");
       textvect[v]->regenerateMipMapLevels();
       //printf("\nafter mipmap\n");
    }
    
    for (int v = 0;v < tc->getWorkingNumber();v++)
    {
       wtextvect.push_back(driver->addTexture(dimension2d<s32>(tc->getWorkingXSize(v), tc->getWorkingYSize(v)), itoa(v+tc->getFinalNumber(), temp, 10)));
       //printf("\nto lock\n");
       s32* sp = (s32*)wtextvect[v]->lock();
       for (int xx = 0;xx < tc->getWorkingXSize(v);xx++)
       {
            for (int yy = 0;yy < tc->getWorkingYSize(v);yy++)
            {
               sp[yy * tc->getWorkingXSize(v) + xx] = tc->getWorkingA8R8G8B8(v, xx, yy);
            //printf("hex: %x\n", tc->getFinalA8R8G8B8(0, xx, yy));
            }
       }
       //printf("\nto unlock\n");
       wtextvect[v]->unlock();
       //printf("\nto mipmap\n");
       wtextvect[v]->regenerateMipMapLevels();
       //printf("\nafter mipmap\n");
    }
    //printf("\n\n\n\n\n");
      return;
}

int main(int argc, char *argv[])
{
   TimeSensor* gamesensor = TimeSensor::getNewClockSensor();
   gamesensor->setLambda(.07);gamesensor->setFrequency(50);
	gamesensor->reset();
	
	char* ch = new char[512];
	ch[0] = 0;
	if (argc > 1)
	{
      strcat(ch, argv[1]);
      
   } 
    else
    {
       printf("Script File Name? (without extension)\n?: ");
       cin >> ch;
    }
      strcpy(luaftemp, ch);
      strcpy(luafile, "scripts/");
      strcat(luafile, ch);
      strcat(luafile, ".lua");
      delete ch;
   
    video::E_DRIVER_TYPE driverType;
    
    //driverType = video::EDT_SOFTWARE2;
    driverType = video::EDT_OPENGL;
    //driverType = video::EDT_DIRECT3D9;
   
   IrrlichtDevice *device =
		createDevice( driverType, dimension2d<s32>(640, 480), 32,
			false, false, false, 0);
	device->setWindowCaption(L"Procedural Texture Viewer");
	IVideoDriver* driver = device->getVideoDriver();
	ISceneManager* smgr = device->getSceneManager();
	IGUIEnvironment* guienv = device->getGUIEnvironment();
	irrevent.Init();
   device->setEventReceiver(&irrevent);
   gui::IGUIStaticText* viewtypetext = guienv->addStaticText(L"",
	        core::rect<s32>(10,10,130,22), true, true, NULL, -1, true);
   gui::IGUIStaticText* texturetext = guienv->addStaticText(L"",
	        core::rect<s32>(140,10,250,22), true, true, NULL, -1, true);
	gui::IGUIStaticText* texturesizetext = guienv->addStaticText(L"",
	        core::rect<s32>(10,30,130,42), true, true, NULL, -1, true);
	gui::IGUIStaticText* texturecsizetext = guienv->addStaticText(L"",
	        core::rect<s32>(140,30,250,42), true, true, NULL, -1, true);
	gui::IGUIStaticText* sizetext = guienv->addStaticText(L"",
	        core::rect<s32>(260,10,380,22), true, true, NULL, -1, true);	        
    //PTexture* pt = new PTexture(100, 100);
    //pt->setColour(5, 5, 255, 255, 255);
    //printf("red: %d, green: %d, blue: %d\n", pt->getRed(5, 5), pt->getGreen(5, 5), pt->getBlue(5, 5));
    //printf("A8R8G8B8: %x\n", pt->getA8R8G8B8(5, 5));

    //printf("Final A8R8G8B8: %x\n", tc->getFinalA8R8G8B8(handle, 4, 4));
      printf("Loading Script\n");
      fillTextureVector(driver);
      printf("Done Loading Script\n");
      
	  //IAnimatedMeshSceneNode* node = smgr->addAnimatedMeshSceneNode( mesh );
	  ICameraSceneNode* cam = smgr->addCameraSceneNode(0, vector3df(0,0,-15), vector3df(0,0,0));
      
      node = smgr->addCubeSceneNode(10, NULL, 1, vector3df(0, 0, 0));
      node->setMaterialTexture(0, textvect[tnum]);
      node->setMaterialFlag(video::EMF_LIGHTING, false);
      node->setMaterialFlag(video::EMF_ANISOTROPIC_FILTER, false);
      node->setMaterialFlag(video::EMF_BILINEAR_FILTER, false);
      node->setMaterialFlag(video::EMF_TRILINEAR_FILTER, false);
         
         //node->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL);
		
		printf("launching\n");
		while(device->run())
	   {
         if (gamesensor->poll())
         {
		/*
		Anything can be drawn between a beginScene() and an endScene()
		call. The beginScene clears the screen with a color and also the
		depth buffer if wanted. Then we let the Scene Manager and the
		GUI Environment draw their content. With the endScene() call
		everything is presented on the screen.
		*/
         if (irrevent.IsKeyDown(KEY_UP))
         {
            double px = cam->getPosition().X;
            double py = cam->getPosition().Y;
            double pz = cam->getPosition().Z;
            cam->setPosition(vector3df(px, py+movespeed, pz));
            cam->setTarget(vector3df(px, py+movespeed, 0));
         }

         if (irrevent.IsKeyDown(KEY_DOWN))
         {
            double px = cam->getPosition().X;
            double py = cam->getPosition().Y;
            double pz = cam->getPosition().Z;
            cam->setPosition(vector3df(px, py-movespeed, pz));
            cam->setTarget(vector3df(px, py-movespeed, 0));
         }

         if (irrevent.IsKeyDown(KEY_LEFT))
         {
             double px = cam->getPosition().X;
             double py = cam->getPosition().Y;
             double pz = cam->getPosition().Z;
             cam->setPosition(vector3df(px-movespeed, py, pz));
             cam->setTarget(vector3df(px-movespeed, py, 0));
         }

         if (irrevent.IsKeyDown(KEY_RIGHT))
         {
             double px = cam->getPosition().X;
             double py = cam->getPosition().Y;
             double pz = cam->getPosition().Z;
             cam->setPosition(vector3df(px+movespeed, py, pz));
             cam->setTarget(vector3df(px+movespeed, py, 0));
         }

         if (irrevent.IsKeyDown(KEY_ADD))
         {
             double px = cam->getPosition().X;
             double py = cam->getPosition().Y;
             double pz = cam->getPosition().Z;
             cam->setPosition(vector3df(px, py, pz+movespeed));
             cam->setTarget(vector3df(px, py, 0));
         }

         if (irrevent.IsKeyDown(KEY_SUBTRACT))
         {
             double px = cam->getPosition().X;
             double py = cam->getPosition().Y;
             double pz = cam->getPosition().Z;
             cam->setPosition(vector3df(px, py, pz-movespeed));
             cam->setTarget(vector3df(px, py, 0));
         }
         
         if (irrevent.IsKeyDown(KEY_KEY_R))
         {
            if (!rexec)
            {
               rexec = true;
               fillTextureVector(driver);
               //printf("\nafterfill\n");
               
             if (viewtype == 1)
              {
                  if (tnum > wtextvect.size()-1) {tnum = wtextvect.size()-1;}
                  if (tnum < 0) {tnum = 0;}
                  node->setMaterialTexture(0, wtextvect[tnum]);
              }
              else
              {
                  if (tnum > textvect.size()-1) {tnum = textvect.size()-1;}
                  if (tnum < 0) {tnum = 0;}
                  node->setMaterialTexture(0, textvect[tnum]);
               }
               
               
            }      
         }
         else
         {
            rexec = false;
         }
         
         if (irrevent.IsKeyDown(KEY_KEY_S))
         {
            if (!sexec)
            {
               sexec = true;
               if (viewtype == 1)
               {
                  //working
                  char* tempstr = new char[512];
                  strcpy(tempstr, "output/");
                  strcat(tempstr, luaftemp);
                  printf(luaftemp);
                  //tempstr[strlen(tempstr)-4] = 0;
                  //strcat(tempstr, "_w");
                  sprintf(tempstr+strlen(tempstr), "_w%d.png", tnum);
                  int xs = tc->workingtextures[tnum]->getXSize();
                  int ys = tc->workingtextures[tnum]->getYSize();
                  double r, g, b;
                  pngwriter image(xs, ys, 1.0, tempstr);
                  image.setcompressionlevel(9);
                  for(int yy = 0;yy<ys;yy++)
                  {
                     for(int xx = 0;xx<xs;xx++)
                     {
                        r = ((double)tc->workingtextures[tnum]->getRed(xx, yy))/255.0;
                        g = ((double)tc->workingtextures[tnum]->getGreen(xx, yy))/255.0;
                        b = ((double)tc->workingtextures[tnum]->getBlue(xx, yy))/255.0;
                        image.plot(xx+1, ys-yy, r, g, b);
                     }
                  }
                  image.close();
                  delete tempstr;
               }
               else
               {
                  //final
                  char* tempstr = new char[512];
                  strcpy(tempstr, "output/");
                  strcat(tempstr, luaftemp);
                  //tempstr[strlen(tempstr)-4] = 0;
                  //strcat(tempstr, "_w");
                  sprintf(tempstr+strlen(tempstr), "_f%d.png", tnum);
                  int xs = tc->finaltextures[tnum]->getXSize();
                  int ys = tc->finaltextures[tnum]->getYSize();
                  double r, g, b;
                  pngwriter image(xs, ys, 1.0, tempstr);
                  image.setcompressionlevel(9);
                  for(int yy = 0;yy<ys;yy++)
                  {
                     for(int xx = 0;xx<xs;xx++)
                     {
                        r = ((double)tc->finaltextures[tnum]->getRed(xx, yy))/255.0;
                        g = ((double)tc->finaltextures[tnum]->getGreen(xx, yy))/255.0;
                        b = ((double)tc->finaltextures[tnum]->getBlue(xx, yy))/255.0;
                        image.plot(xx+1, ys-yy, r, g, b);
                     }
                  }
                  image.close();
                  delete tempstr;
               }

            }
         }
         else
         {
            sexec = false;
         }
         
         if (irrevent.IsKeyDown(KEY_KEY_F))
         {
            if (!fexec)
            {
               fexec = true;
                  //working
                  for (int i = 0; i < tc->finaltextures.size();i++)
                  {
                  char* tempstr = new char[512];
                  strcpy(tempstr, "output/");
                  strcat(tempstr, luaftemp);
                  //tempstr[strlen(tempstr)-4] = 0;
                  //strcat(tempstr, "_w");
                  sprintf(tempstr+strlen(tempstr), "_f%d.png", i);
                  int xs = tc->finaltextures[i]->getXSize();
                  int ys = tc->finaltextures[i]->getYSize();
                  double r, g, b;
                  pngwriter image(xs, ys, 1.0, tempstr);
                  image.setcompressionlevel(9);
                  for(int yy = 0;yy<ys;yy++)
                  {
                     for(int xx = 0;xx<xs;xx++)
                     {
                        r = ((double)tc->finaltextures[i]->getRed(xx, yy))/255.0;
                        g = ((double)tc->finaltextures[i]->getGreen(xx, yy))/255.0;
                        b = ((double)tc->finaltextures[i]->getBlue(xx, yy))/255.0;
                        image.plot(xx+1, ys-yy, r, g, b);
                     }
                  }
                  image.close();
                  delete tempstr;
               }

            }
         }
         else
         {
            fexec = false;
         }
         
         if (irrevent.IsKeyDown(KEY_KEY_W))
         {
            if (!wexec)
            {
               wexec = true;
                  //working
                  for (int i = 0; i < tc->workingtextures.size();i++)
                  {
                  char* tempstr = new char[512];
                  strcpy(tempstr, "output/");
                  strcat(tempstr, luaftemp);
                  //tempstr[strlen(tempstr)-4] = 0;
                  //strcat(tempstr, "_w");
                  sprintf(tempstr+strlen(tempstr), "_f%d.png", i);
                  int xs = tc->workingtextures[i]->getXSize();
                  int ys = tc->workingtextures[i]->getYSize();
                  double r, g, b;
                  pngwriter image(xs, ys, 1.0, tempstr);
                  image.setcompressionlevel(9);
                  for(int yy = 0;yy<ys;yy++)
                  {
                     for(int xx = 0;xx<xs;xx++)
                     {
                        r = ((double)tc->workingtextures[i]->getRed(xx, yy))/255.0;
                        g = ((double)tc->workingtextures[i]->getGreen(xx, yy))/255.0;
                        b = ((double)tc->workingtextures[i]->getBlue(xx, yy))/255.0;
                        image.plot(xx+1, ys-yy, r, g, b);
                     }
                  }
                  image.close();
                  delete tempstr;
               }

            }
         }
         else
         {
            wexec = false;
         }
         
         if (irrevent.IsKeyDown(KEY_PERIOD))
         {    
            if (!periodexec)
            {
              periodexec = true;
              if (viewtype == 0)
              {
                 if (tnum < (textvect.size()-1)) 
                    {
                        tnum++;node->setMaterialTexture(0, textvect[tnum]);
                    }
               }
               else
               {
                    if (tnum < (wtextvect.size()-1))
                    {
                        tnum++;node->setMaterialTexture(0, wtextvect[tnum]);
                    }
               }
            }
         }
         else
         {
            periodexec = false;
         }
         
         if (irrevent.IsKeyDown(KEY_COMMA))
         {
            if (!commaexec)
            {
              commaexec = true;
              if (viewtype == 0)
              {
                 if (tnum > 0)
                    {
                        tnum--;node->setMaterialTexture(0, textvect[tnum]);
                    }
               }
               else
               {
                    if (tnum > 0)
                    {
                        tnum--;node->setMaterialTexture(0, wtextvect[tnum]);
                    }
               }
            }
         }
         else
         {
            commaexec = false;
         }
         
         if (irrevent.IsKeyDown(KEY_KEY_M))
         {
            if (!mexec)
            {
              mexec = true;
              if (viewtype == 0) 
              {
                  viewtype = 1;
                  if (tnum > wtextvect.size()-1) {tnum = wtextvect.size()-1;}
                  if (tnum < 0) {tnum = 0;}
                  node->setMaterialTexture(0, wtextvect[tnum]);
              }
              else 
              {
                  viewtype = 0;
                  if (tnum > textvect.size()-1) {tnum = textvect.size()-1;}
                  if (tnum < 0) {tnum = 0;}
                  node->setMaterialTexture(0, textvect[tnum]);
               }
            }
         }
         else
         {
            mexec = false;
         }
         //if (irrevent.IsKeyDown(KEY_DOWN)) {theKeyState.setDown(true);}
         //if (irrevent.IsKeyDown(KEY_LEFT)) {theKeyState.setLeft(true);}
         //if (irrevent.IsKeyDown(KEY_RIGHT)) {theKeyState.setRight(true);}
         if (irrevent.IsKeyDown(KEY_ESCAPE)) {break;}
		   
         driver->beginScene(true, true, SColor(255,100,101,140));
		   
         driver->draw2DRectangle(video::SColor(255,255,255,255),
                                                 core::rect<int>(10,10,130,22),
                                                 NULL);
                                                 
         driver->draw2DRectangle(video::SColor(255,255,255,255),
                                                 core::rect<int>(140,10,250,22),
                                                 NULL);
                                                 
         driver->draw2DRectangle(video::SColor(255,255,255,255),
                                                 core::rect<int>(10,30,130,42),
                                                 NULL);                                                 
         
         driver->draw2DRectangle(video::SColor(255,255,255,255),
                                                 core::rect<int>(140,30,250,42),
                                                 NULL);
 
         driver->draw2DRectangle(video::SColor(255,255,255,255),
                                                 core::rect<int>(260,10,380,22),
                                                 NULL); 
                                                 
                                 
         if (viewtype == 0) {swprintf(buf,80,L"Texture #: %d / %d",tnum, textvect.size()-1);}
           if (viewtype == 1) {swprintf(buf,80,L"Texture #: %d / %d",tnum, wtextvect.size()-1);}
                   texturetext->setText(buf);         
         if (viewtype == 0) {swprintf(buf,80,L"This Texture: %d kb",tc->getFinalTextureMemSize(tnum)/1024);}
         if (viewtype == 1) {swprintf(buf,80,L"This Texture: %d kb",tc->getWorkingTextureMemSize(tnum)/1024);} 
                   texturesizetext->setText(buf);        
         swprintf(buf,80,L"Texture Container: %d kb",tc->getTextureContainerMemSize()/1024);
                   texturecsizetext->setText(buf);         
         if (viewtype == 0) {swprintf(buf,80,L"Dim: %dx%d",tc->getFinalXSize(tnum), tc->getFinalYSize(tnum));}
         if (viewtype == 1) {swprintf(buf,80,L"Dim: %dx%d",tc->getWorkingXSize(tnum), tc->getWorkingYSize(tnum));}         
                   sizetext->setText(buf);
                
         if (viewtype == 0) {swprintf(buf,80,L"Viewing: Final");}         
         if (viewtype == 1) {swprintf(buf,80,L"Viewing: Working");}
		                      viewtypetext->setText(buf);                  
         smgr->drawAll();
		   guienv->drawAll();

		   driver->endScene();
         }
      }
	   
	   device->drop();
		
    return EXIT_SUCCESS;
}
