
#ifndef _RAYTRACER_INL_
#define _RAYTRACER_INL_

typedef struct IntersectResult {
  float distance;
  float position[3];
  float normal[3];
} IntersectResult;

typedef struct IGeometry {
  float reflectiveness;
  void* material;
  void* geometry;
  int (*intersect)(void* geo, const float* origin, const float* direction, IntersectResult* result);
  int (*sample)(void* mat, const float* origin, const float* direction, const float* position, const float* normal, float* color);
} IGeometry;

typedef struct plane_t {
  float normal[3];
  float d;
  float position[3];
} plane_t;

int plane_intersect(plane_t* plane, const float* origin, const float* direction, IntersectResult* result)
{
  float b, a = fvec_dot(direction, plane->normal);
  float v1[3];

  if (a >= 0) {
    return 0;
  }

  fvec_sub(v1, origin, plane->position);
  b = fvec_dot(plane->normal, v1);
  result->distance = -b / a;
  fvec_getpoint(result->position, origin, direction, result->distance);
  fvec_setv(result->normal, plane->normal);
  return 1;
}

IGeometry* setgeo_plane(IGeometry* geo, float nx, float ny, float nz, float d)
{
  plane_t* plane = MALLOC(plane_t, 1);
  geo->geometry = plane;
  geo->intersect = plane_intersect;
  plane->d = d;
  fvec_set(plane->normal, nx, ny, nz);
  fvec_muls(plane->position, plane->normal, d);
  return geo;
}

typedef struct sphere_t {
  float center[3];
  float radius;
} sphere_t;

int sphere_intersect(sphere_t* sphere, const float* origin, const float* direction, IntersectResult* result)
{
  float v[3];
  float DdotV;
  fvec_sub(v, origin, sphere->center);
  DdotV = fvec_dot(direction, v);

  if (DdotV <= 0) {
    float a0 = fvec_sqrlen(v) - sphere->radius * sphere->radius;
    float discr = DdotV * DdotV - a0;

    if (discr >= 0) {
      result->distance = -DdotV - (float)sqrt(discr);
      fvec_getpoint(result->position, origin, direction, result->distance);
      fvec_sub(result->normal, result->position, sphere->center);
      fvec_normalize(result->normal, result->normal);
      return 1;
    }
  }

  return 0;
}

IGeometry* setgeo_sphere(IGeometry* geo, float cx, float cy, float cz, float radius)
{
  sphere_t* sphere = MALLOC(sphere_t, 1);
  geo->geometry = sphere;
  geo->intersect = sphere_intersect;
  sphere->radius = radius;
  fvec_set(sphere->center, cx, cy, cz);
  return geo;
}

typedef struct CheckerMaterial {
  float scale;
} CheckerMaterial;

int checker_sample(CheckerMaterial* mat, const float* origin, const float* direction, const float* position, const float* normal, float* color)
{
  int d = (int)(position[0] * mat->scale + 0.5f) + (int)(position[1] * mat->scale + 0.5f) + (int)(position[2] * mat->scale + 0.5f);
  return (d & 1) ? fvec_set(color, 0, 0, 0) : fvec_set(color, 1, 1, 1);
}

IGeometry* setmat_checker(IGeometry* geo, float scale, float reflectiveness)
{
  CheckerMaterial* mat = MALLOC(CheckerMaterial, 1);
  geo->material = mat;
  mat->scale = scale;
  geo->reflectiveness = reflectiveness;
  geo->sample = checker_sample;
  return geo;
}

typedef struct PhongMaterial {
  float diffuse[3];
  float specular[3];
  float shininess;
} PhongMaterial;

static float lightDir[3] = {0.577350f, 0.577350f, 0.577350f};
static float lightColor[3] = {1, 1, 1};

int phong_sample(PhongMaterial* mat, const float* origin, const float* direction, const float* position, const float* normal, float* color)
{
  float NdotH, NdotL = fvec_dot(normal, lightDir);
  float H[3];
  float diffuseTerm[3];
  float specularTerm[3];
  float cc[3];

  fvec_sub(H, lightDir, direction);
  fvec_normalize(H, H);
  NdotH = fvec_dot(normal, H);
  fvec_muls(diffuseTerm, mat->diffuse, max(NdotL, (float)0));
  fvec_muls(specularTerm, mat->specular, (float)pow(max(NdotH, (float)0), mat->shininess));
  fvec_add(cc, diffuseTerm, specularTerm);
  fvec_modulate(color, lightColor, cc);
  return 0;
}

IGeometry* setmat_phong(IGeometry* geo, COLOR diffuse, COLOR specular, float shininess, float reflectiveness)
{
  PhongMaterial* mat = MALLOC(PhongMaterial, 1);
  geo->material = mat;
  geo->reflectiveness = reflectiveness;
  mat->shininess = shininess;
  fvec_setrgb(mat->diffuse, diffuse);
  fvec_setrgb(mat->specular, specular);
  geo->sample = phong_sample;
  return geo;
}

int rayTraceRecursive(const IGeometry* geos, int ngeo, const float* origin, const float* direction, long maxReflect, float* color)
{
  int i, j = -1;
  IntersectResult minResult = {0};
  minResult.distance = FLT_MAX;

  for (i = 0; i < ngeo; ++i) {
    IntersectResult result = {0};
    int ret = geos[i].intersect(geos[i].geometry, origin, direction, &result);

    if (ret && (result.distance < minResult.distance)) {
      minResult = result;
      j = i;
    }
  }

  if (j >= 0) {
    float reflectiveness = geos[j].reflectiveness;
    geos[j].sample(geos[j].material, origin, direction, minResult.position, minResult.normal, color);
    fvec_muls(color, color, 1 - reflectiveness);

    if ((reflectiveness > 0) && (maxReflect > 0)) {
      float ray_direction[3];
      float reflectedColor[3] = {0};
      fvec_getpoint(ray_direction, direction, minResult.normal, -2 * fvec_dot(minResult.normal, direction));

      if (rayTraceRecursive(geos, ngeo, minResult.position, ray_direction, maxReflect - 1, reflectedColor)) {
        fvec_getpoint(color, color, reflectedColor, reflectiveness);
      }
    }

    return 1;
  }

  return 0;
}

int raytracer(img_t* im, const IGeometry* geos, int ngeo, const float* eye, const float* front, const float* refUp, float fov, int maxReflect)
{
  float right[3];
  float up[3];
  float fovScale;
  float r[3], u[3];

  fvec_cross(right, front, refUp);
  fvec_cross(up, right, front);
  fovScale = (float)tan(fov * (PI  * 0.5f / 180)) * 2;

  {
    int x, y;
    float dx = 1.0f / im->w;
    float dy = 1.0f / im->h;
    imdraw_fill(im, rgb(0, 0, 0));

    for (y = 0; y < im->h; ++y) {
      float sy = 1 - dy * y;
      uchar* ptr = im->tt.data + y * im->s;

      for (x = 0; x < im->w; ++x, ptr += im->c) {
        float sx = dx * x;
        float direction[3];
        float color[3] = {0};
        fvec_muls(r, right, (sx - 0.5f) * fovScale);
        fvec_muls(u, up, (sy - 0.5f) * fovScale);
        fvec_add(r, r, front);
        fvec_add(r, r, u);
        fvec_normalize(direction, r);

        if (rayTraceRecursive(geos, ngeo, eye, direction, maxReflect, color)) {
          ptr[0] = (uchar)(BOUND(color[0], 0, 1) * 255);
          ptr[1] = (uchar)(BOUND(color[1], 0, 1) * 255);
          ptr[2] = (uchar)(BOUND(color[2], 0, 1) * 255);
        }
      }
    }
  }

  return 0;
}

int freegeo(IGeometry* geo, int ngeo)
{
  int i;

  for (i = 0; i < ngeo; ++i) {
    FREE(geo[i].geometry);
    FREE(geo[i].material);
  }

  return 0;
}

#endif // _RAYTRACER_INL_
