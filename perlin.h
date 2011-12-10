
#ifndef PERLIN_H
#define PERLIN_H

#define INTERPOLATION_LINEAR 0
#define INTERPOLATION_COSINE 1
#define INTERPOLATION_QUADRATIC 2
//#define INTERPOLATION_CUBIC  2

double LinearInterpolation(double a, double b, double x);
double CosineInterpolation(double a, double b, double x);
double CrazyInterpolation(double a, double b, double x);
double Noise1(int x, int y, int seed);
double SmoothNoise1(int x, int y, int seed);
double InterpolatedNoise(double x, double y, int seed, int interptype);
double PerlinNoise_2D(double x, double y, double Persistance, int Octaves,
                     int seed, int interptype);

#endif
