
#include "draw/raytracer.inl"

int test_raytracer()
{
  IGeometry geo[10] = {0};
  float eye[3] = {0, 5, 105};
  float front[3] = {0, 0, -1};
  float refUp[3] = {0, 1, 0};
  float fov = 90;
  img_t im[1] = {0};
  const char* name = "test_raytracer";
  int ngeo;
  int eye_z = 50;
  int front_z = 1;
  IGeometry* g = geo;
  setmat_checker(setgeo_plane(g++, 0, 1, 0, 0), 0.1f, 0.5);
  setmat_phong(setgeo_sphere(g++, -10, 10, -10, 10), rgb(1, 0, 0), rgb(1, 1, 1), 16, 0.25);
  setmat_phong(setgeo_sphere(g++, 10, 10, -10, 10), rgb(0, 0, 1), rgb(1, 1, 1), 16, 0.25);
  setmat_phong(setgeo_sphere(g++, -10, 30, -10, 10), rgb(0, 1, 1), rgb(1, 1, 1), 16, 0.25);
  setmat_phong(setgeo_sphere(g++, 10, 30, -10, 10), rgb(1, 1, 0), rgb(1, 1, 1), 16, 0.25);
  setmat_checker(setgeo_plane(g++, 0, 0, 1, -100), 0.1f, 0.5);
  ngeo = g - geo;
  imsetsize(im, 512, 512, 3, 1);
  imshow2(name, im);
  cvCreateTrackbar("eye_z", name, &eye_z, 100, 0);
  cvCreateTrackbar("front_z", name, &front_z, 100, 0);

  for (; ;) {
    eye[2] = (float)eye_z;
    refUp[1] = (float)front_z;
    {
      utime_start(_start_time);
      raytracer(im, geo, ngeo, eye, front, refUp, fov, 0);
      printf("%f\n", utime_elapsed(_start_time));
    }

    imshow2(name, im);
    cvWaitKey(50);
  }

  freegeo(geo, ngeo);
  imfree(im);
  return 0;
}

