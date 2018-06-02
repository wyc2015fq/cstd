#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int main1() {
  char *source = "hello";
  char* src_copy = strdup(source);
  char* src = src_copy;
  char* end = src + strlen(src) - 1;
  while(src < end) {
    char a = *src;
    char b = *end;
    *src++ = b;
    *end-- = a;
  }
  printf("%s\n", src_copy);
  free(src_copy);
  return 0;
}

#include <string>
#include <math.h>

#define PI 3.1415926
float rand_float() {
  float rnd = (1.f * rand())/RAND_MAX;
  return rnd;
}
class car {
    float speed;
    float x, y;
    std::string name;
public:
    car() : name("car"), speed(10), x(0.f), y(0.f) {}
    car(std::string name0, float speed0) : name(name0), speed(speed0), x(0.f), y(0.f) {}
    void setSpeed(float speed0) {
      speed = speed0;
    }
    void setName(const std::string& name0) {
      name = name0;
    }
    void updataPos() {
      float dir = 2.0 * PI * rand_float();
      x += speed * cos(dir);
      y += speed * sin(dir);
    }
    void printPos() const {
      printf("%10s %6.2f %6.2f\n", name.c_str(), x, y);
    }
};

#include <time.h>
#include <windows.h>
int main() {
  int i, j;
  char* names[] = {"car", "truck", "ambulance"};
  float speeds[] = {10, 20, 30};
  car cars[3];
  srand(time(0));
  for (i=0; i<3; ++i) {
    cars[i].setName(names[i]);
    cars[i].setSpeed(speeds[i]);
  }
  for (j=0; j<10; ++j) {
    for (i=0; i<3; ++i) {
      cars[i].updataPos();
      cars[i].printPos();
    }
    //Sleep(1);
  }
  for (i=0; i<3; ++i) {
    cars[i].printPos();
  }
  return 0;
}
