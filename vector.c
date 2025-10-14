#include "vector.h"


double modV2i(Vec2i *vec)
{
  return sqrt(
      (double)
      ((vec->x * vec->x) + (vec->y*vec->y))
  );
}

double modV2f(Vec2f *vec)
{
  return sqrt(
      (double)
      ((vec->x * vec->x) + (vec->y*vec->y))
  );
}

double modV3f(Vec3f *vec)
{
  return sqrt(
      (double)
      ((vec->x * vec->x) + (vec->y*vec->y) + (vec->z * vec->z))
  );
}

double modV3i(Vec3i *vec)
{
  return sqrt(
      (double)
      ((vec->x * vec->x) + (vec->y*vec->y) + (vec->z * vec->z))
  );
}


void sumV2i(Vec2i *veco, Vec2i *vecf)
{
  veco->x += vecf->x;
  veco->y += vecf->y;
}

void sumV2f(Vec2f *veco, Vec2f *vecf)
{
  veco->x += vecf->x;
  veco->y += vecf->y;
}

void sumV3f(Vec3f *veco, Vec3f *vecf)
{
  veco->x += vecf->x;
  veco->y += vecf->y;
  veco->z += vecf->z;
}

void sumV3i(Vec3i *veco, Vec3i *vecf)
{
  veco->x += vecf->x;
  veco->y += vecf->y;
  veco->z += vecf->z;
}


void subV2i(Vec2i *veco, Vec2i *vecf)
{
  veco->x -= vecf->x;
  veco->y -= vecf->y;
}

void subV2f(Vec2f *veco, Vec2f *vecf)
{
  veco->x -= vecf->x;
  veco->y -= vecf->y;
}

void subV3f(Vec3f *veco, Vec3f *vecf)
{
  veco->x -= vecf->x;
  veco->y -= vecf->y;
  veco->z -= vecf->z;
}

void subV3i(Vec3i *veco, Vec3i *vecf)
{
  veco->x -= vecf->x;
  veco->y -= vecf->y;
  veco->z -= vecf->z;
}


void dotV2i(Vec2i *veco, Vec2i *vecf)
{
  veco->x = ((veco->x * vecf->x) + (veco->x * vecf->y));
  veco->y = ((veco->y * vecf->x) + (veco->y * vecf->y));
}

void dotV2f(Vec2f *veco, Vec2f *vecf)
{
  veco->x = ((veco->x * vecf->x) + (veco->x * vecf->y));
  veco->y = ((veco->y * vecf->x) + (veco->y * vecf->y));
}

void dotV3f(Vec3f *veco, Vec3f *vecf)
{
  veco->x = ((veco->x * vecf->x) + (veco->x * vecf->y) + (veco->x * vecf->z));
  veco->y = ((veco->y * vecf->x) + (veco->y * vecf->y) + (veco->y * vecf->z));
  veco->z = ((veco->z * vecf->x) + (veco->z * vecf->y) + (veco->z * vecf->z));
}

void dotV3i(Vec3i *veco, Vec3i *vecf)
{
  veco->x = ((veco->x * vecf->x) + (veco->x * vecf->y) + (veco->x * vecf->z));
  veco->y = ((veco->y * vecf->x) + (veco->y * vecf->y) + (veco->y * vecf->z));
  veco->z = ((veco->z * vecf->x) + (veco->z * vecf->y) + (veco->z * vecf->z));
}

void scalar_multV2i(Vec2i *veco, float x)
{
  veco->x = (int) (veco->x * x);
  veco->y = (int) (veco->y * x);
}

void scalar_multV2f(Vec2f *veco, float x)
{
  veco->x *= (veco->x * x);
  veco->y *= (veco->y * x);
}

void scalar_multV3f(Vec3f *veco, float x)
{
  veco->x *= (veco->x * x);
  veco->y *= (veco->y * x);
  veco->z *= (veco->z * x);
}

void scalar_multV3i(Vec3i *veco, float x)
{
  veco->x = (int) (veco->x * x);
  veco->y = (int) (veco->y * x);
  veco->z = (int) (veco->z * x);
}


void rotateV2i(Vec2i *vec, float angle_rad)
{
  vec->x = (int) ((vec->x * cos(angle_rad)) - (vec->y * sin(angle_rad)));
  vec->y = (int) ((vec->x * sin(angle_rad)) + (vec->y * cos(angle_rad)));
}

void rotateV2f(Vec2f *vec, float angle_rad)
{
  vec->x = (float) ((vec->x * cos(angle_rad)) - (vec->y * sin(angle_rad)));
  vec->y = (float) ((vec->x * sin(angle_rad)) + (vec->y * cos(angle_rad)));
}

