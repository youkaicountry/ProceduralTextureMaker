
#include <math.h>
#include "prochash.h"
#include "perlin.h"
#include "Random.h"

double LinearInterpolation(double a, double b, double x)
{
      return a*(1-x) + b*x;
}

double CosineInterpolation(double a, double b, double x)
{
      double ft = x * 3.1415927;
      double f = (1 - cos(ft)) * .5f;
      return a*(1-f) + b*f;
}

double CrazyInterpolation(double a, double b, double x)
{
      if (x <= .5)
      {
         x *= x;
      }
      else
      {
         x = 1.0 - x;
         x *= x;
         x = 0.5 - x;
      }
      x += x;
      return a*(1-x) + b*x;
}   
 
double Noise1(int x, int y, int seed)
{
    Random r(worldkey2d(x, y, seed));
    //MTRand rnum(worldkey2d(x, y, seed));
    //return rnum.rand();
    return r.nextDouble(1.0);
}

double SmoothNoise1(int x, int y, int seed)
{
    double corners = ( Noise1((x-1), (y-1), (seed)) + Noise1((x+1), (y-1), (seed)) + Noise1((x-1), (y+1),(seed)) + Noise1((x+1), (y+1),(seed)) ) / 16;
    double sides   = ( Noise1(x-1, y,seed)  + Noise1(x+1, y,seed)  + Noise1(x, y-1,seed)  + Noise1(x, y+1,seed) ) /  8;
    double center  =  Noise1(x, y,seed) / 4;
    return corners + sides + center;
}

double InterpolatedNoise(double x, double y, int seed, int interptype)
{
      int integer_X    = (int) x;
      double fractional_X = (double) (x - integer_X);

      int integer_Y    = (int)y;
      double fractional_Y = (double) (y - integer_Y);

      double v1 = SmoothNoise1(integer_X,     integer_Y, seed);
      double v2 = SmoothNoise1(integer_X + 1, integer_Y, seed);
      double v3 = SmoothNoise1(integer_X,     integer_Y + 1, seed);
      double v4 = SmoothNoise1(integer_X + 1, integer_Y + 1, seed);
      
      double i1, i2, i3;

      if (interptype == INTERPOLATION_LINEAR)
      {      
        i1 = LinearInterpolation(v1 , v2 , fractional_X);
        i2 = LinearInterpolation(v3 , v4 , fractional_X);
        i3 = LinearInterpolation(i1 , i2 , fractional_Y);
      }
      
      if (interptype == INTERPOLATION_COSINE)
      {
        i1 = CosineInterpolation(v1 , v2 , fractional_X);
        i2 = CosineInterpolation(v3 , v4 , fractional_X);
        i3 = CosineInterpolation(i1 , i2 , fractional_Y);
      }
      if (interptype == INTERPOLATION_QUADRATIC)
      {
        i1 = CrazyInterpolation(v1 , v2 , fractional_X);
        i2 = CrazyInterpolation(v3 , v4 , fractional_X);
        i3 = CrazyInterpolation(i1 , i2 , fractional_Y); 
      }        
      
      return i3;
}

double PerlinNoise_2D(double x, double y, double Persistance, int Octaves, int seed, int interptype)
{
      double total = 0;
      double p = Persistance;
      int n = Octaves - 1;
      double frequency = 1.0;
      double amplitude = (1.0 - p);
      int i = 0;
      for(i = 0; i < n; i++)
      {

          total = total + InterpolatedNoise(x * frequency, y * frequency, seed, interptype) * amplitude;
          frequency += frequency;
          amplitude *= p;
      }

      return total;
}
