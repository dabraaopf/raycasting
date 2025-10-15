#ifndef VECTOR_MODULE
#define VECTOR_MODULE
#include <stddef.h>
#include <math.h>

struct Vec2int
{
  size_t x;
  size_t y;
};

struct Vec2float
{
  double x;
  double y;
};

struct Vec3int
{
  size_t x;
  size_t y;
  size_t z;
};

struct Vec3float
{
  float x;
  float y;
  float z;
};

typedef struct Vec2int Vec2i;
typedef struct Vec2float Vec2f;
typedef struct Vec3int Vec3i;
typedef struct Vec3float Vec3f;


double modV2i(Vec2i *vec);
double modV2f(Vec2f *vec);
double modV3f(Vec3f *vec);
double modV3i(Vec3i *vec);
void sumV2i(Vec2i *veco, Vec2i *vecf);
void sumV2f(Vec2f *veco, Vec2f *vecf);
void sumV3f(Vec3f *veco, Vec3f *vecf);
void sumV3i(Vec3i *veco, Vec3i *vecf);
void subV2i(Vec2i *veco, Vec2i *vecf);
void subV2f(Vec2f *veco, Vec2f *vecf);
void subV3f(Vec3f *veco, Vec3f *vecf);
void subV3i(Vec3i *veco, Vec3i *vecf);
void dotV2i(Vec2i *veco, Vec2i *vecf);
void dotV2f(Vec2f *veco, Vec2f *vecf);
void dotV3f(Vec3f *veco, Vec3f *vecf);
void dotV3i(Vec3i *veco, Vec3i *vecf);
void scalar_multV2i(Vec2i *veco, float x);
void scalar_multV2f(Vec2f *veco, float x);
void scalar_multV3f(Vec3f *veco, float x);
void scalar_multV3i(Vec3i *veco, float x);
void rotateV2i(Vec2i *vec, float angle_rad);
void rotateV2f(Vec2f *vec, float angle_rad);
#endif
