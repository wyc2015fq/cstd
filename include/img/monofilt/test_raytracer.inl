#include <stdio.h>
#include <stdlib.h>
#include <math.h>
//#include <iostream>
#include <time.h>
#include <algorithm>
#include <math.h>

#include "bmpfile.inl"

//#include "../hGraphic32/hGraphic32.h" //简易图像处理基础框架

#ifndef PI
#define PI 3.141592653589793238462643383
#endif // PI

struct Vector3{
  float x;
  float y;
  float z;
  
  inline Vector3(const Vector3& v):x(v.x),y(v.y),z(v.z) {}
  inline Vector3(float _x,float _y,float _z):x(_x),y(_y),z(_z) {}
  inline float length()const { return sqrt(sqrLength()); }
  inline float sqrLength()const { return x*x+y*y+z*z; }
  inline Vector3 normalize()const { float inv = 1/length(); return Vector3(x*inv,y*inv,z*inv); }
  inline Vector3 negate()const { return Vector3(-x, -y, -z); }
  inline Vector3 add(const Vector3& v)const { return Vector3(x + v.x, y + v.y, z + v.z); }
  inline Vector3 subtract(const Vector3& v) const { return Vector3(x - v.x, y - v.y, z - v.z); }
  inline Vector3 multiply(float f) const { return Vector3(x * f, y * f, z * f); }
  inline Vector3 divide(float f) const { float invf = 1/f; return Vector3(x * invf, y * invf, z * invf); }
  inline float dot(const Vector3& v) const { return x * v.x + y * v.y + z * v.z; }
  inline Vector3 cross(const Vector3& v) const { return Vector3(-z * v.y + y * v.z, z * v.x - x * v.z, -y * v.x + x * v.y); }
  
  static inline Vector3 zero(){ return Vector3(0,0,0); }
};

struct Ray3 {
  Vector3 origin;
  Vector3 direction;
  
  inline Ray3(const Ray3& r):origin(r.origin),direction(r.direction){}
  inline Ray3(const Vector3& _origin,const Vector3& _direction):origin(_origin),direction(_direction){}
  inline Vector3 getPoint(float t)const{ return origin.add(direction.multiply(t)); }
};

struct IGeometry;

struct IntersectResult{
  IGeometry*	geometry;
  float		distance;
  Vector3		position;
  Vector3		normal;
  inline IntersectResult():geometry(0),distance(0),position(Vector3::zero()),normal(Vector3::zero()){ }
  static inline IntersectResult noHit() { return IntersectResult(); }
};


#ifndef min
#define min(a, b) ((a)<(b)?(a):(b))
#endif // min

#ifndef max
#define max(a, b) ((a)>(b)?(a):(b))
#endif // min

struct Color{
  float r;
  float g;
  float b;
  inline Color(float _r,float _g,float _b):r(_r),g(_g),b(_b) {  };
  
  inline Color add(const Color& c)const { return Color(r + c.r, g + c.g, b + c.b); }
  inline Color multiply(float s) const { return Color(r * s, g * s, b * s); }
  inline Color modulate(const Color& c) const { return Color(r * c.r, g * c.g, b * c.b); }
  inline void saturate() { r = min(r, (float)1); g = min(g, (float)1); b = min(b, (float)1); }
  
  static inline Color black(){ return Color(0,0,0); }
  static inline Color white(){ return Color(1,1,1); }
  static inline Color red()  { return Color(1,0,0); }
  static inline Color green(){ return Color(0,1,0); }
  static inline Color blue() { return Color(0,0,1); }
};

struct IMaterial{
  float   reflectiveness;
  inline IMaterial(float _reflectiveness=0):reflectiveness(_reflectiveness){}
  
  virtual Color sample(const Ray3& ray,const Vector3& position,const Vector3& normal)=0;
  virtual ~IMaterial(){}
};

struct IGeometry{
  IMaterial* material;
  inline IGeometry():material(0){}
  virtual ~IGeometry(){
    if (material) {
      delete material;
      material=0;
    }
  }
  
  virtual void initialize(){}
  virtual IntersectResult intersect(const Ray3& ray)=0; //todo:做返回值优化
};

struct Sphere:public IGeometry{
  Vector3 center;
  float   radius;
  inline Sphere(const Sphere& s):center(s.center),radius(s.radius) {  }
  inline Sphere(const Vector3& _center,float _radius):center(_center),radius(_radius) {  }
  inline float sqrRadius()const{ return radius*radius; }
  
  virtual IntersectResult intersect(const Ray3& ray){ 
    Vector3 v = ray.origin.subtract(this->center);
    float DdotV = ray.direction.dot(v);
    
    if (DdotV <= 0) {
      float a0 = v.sqrLength() - this->sqrRadius();
      float discr = DdotV * DdotV - a0;
      if (discr >= 0) {
        IntersectResult result;
        result.geometry = this;
        result.distance = -DdotV - sqrt(discr);
        result.position = ray.getPoint(result.distance);
        result.normal = result.position.subtract(this->center).normalize();
        return result;
      }
    }
    
    return IntersectResult::noHit();
  }
};

struct Plane:public IGeometry{
  Vector3 normal;
  float   d;
  
  Vector3 position;
  
  inline Plane(const Plane& p):normal(p.normal),d(p.d), position(Vector3::zero()) {  }
  inline Plane(const Vector3& _normal,float _d):normal(_normal),d(_d), position(Vector3::zero()) {  }
  
  inline void initialize() {
    position = normal.multiply(d);
  }
  
  virtual IntersectResult intersect(const Ray3& ray){ 
    float a = ray.direction.dot(this->normal);
    if (a >= 0)
      return IntersectResult::noHit();
    
    float b = this->normal.dot(ray.origin.subtract(this->position));
    IntersectResult result;
    result.geometry = this;
    result.distance = -b / a;
    result.position = ray.getPoint(result.distance);
    result.normal = this->normal;
    return result;
  }
  
};

#include <vector>

struct Union:public IGeometry{
  std::vector<IGeometry*> geometries;
  inline Union() { }
  
  void push(IGeometry* geometry){
    geometries.push_back(geometry);
  }
  
  inline void initialize() {
    long size=geometries.size();
    for (long i=0;i<size;++i)
      geometries[i]->initialize();
  }
  
  virtual IntersectResult intersect(const Ray3& ray){
    const float Infinity=1e30f; 
    float minDistance = Infinity;
    IntersectResult minResult = IntersectResult::noHit();
    long size=this->geometries.size();
    for (long i=0;i<size;++i){
      IntersectResult result = this->geometries[i]->intersect(ray);
      if (result.geometry && (result.distance < minDistance)) {
        minDistance = result.distance;
        minResult = result;
      }
    }
    return minResult;
  }
  
  inline void clear(){
    long size=geometries.size();
    for (long i=0;i<size;++i)
      delete geometries[i];
    geometries.clear();
  }
  
  virtual ~Union(){
    clear();
  }
};

struct CheckerMaterial:public IMaterial{
  float scale;
  inline CheckerMaterial(float _scale,float _reflectiveness=0):IMaterial(_reflectiveness),scale(_scale) { }
  
  virtual Color sample(const Ray3& ray,const Vector3& position,const Vector3& normal){
    float d=abs((floor(position.x * this->scale) + floor(position.z * this->scale)));
    d=fmod(d,2);
    return  d < 1 ? Color::black() : Color::white();
  }
};

// global temp
static Vector3 lightDir = Vector3(1, 1, 1).normalize();
static Color lightColor = Color::white();

struct PhongMaterial:public IMaterial{
  Color   diffuse;
  Color   specular;
  float   shininess;
  
  inline PhongMaterial(const Color& _diffuse,const Color& _specular,const float& _shininess,float _reflectiveness=0)
    :IMaterial(_reflectiveness),diffuse(_diffuse),specular(_specular),shininess(_shininess){ }
  
  virtual Color sample(const Ray3& ray,const Vector3& position,const Vector3& normal){
    float NdotL = normal.dot(lightDir);
    Vector3 H = (lightDir.subtract(ray.direction)).normalize();
    float NdotH = normal.dot(H);
    Color diffuseTerm = this->diffuse.multiply(max(NdotL, (float)0));
    Color specularTerm = this->specular.multiply(pow(max(NdotH, (float)0), this->shininess));
    return lightColor.modulate(diffuseTerm.add(specularTerm));
  }
};

struct PerspectiveCamera{
  Vector3 eye;
  Vector3 front;
  Vector3 refUp;
  float   fov;
  
  Vector3 right;
  Vector3 up;
  float   fovScale;
  
  inline PerspectiveCamera(const Vector3& _eye,const Vector3& _front,const Vector3& _refUp,float _fov)
    :eye(_eye),front(_front),refUp(_refUp),fov(_fov),
    right(Vector3::zero()),up(Vector3::zero()),fovScale(0) {}
  
  inline void initialize(){
    right = front.cross(refUp);
    up = right.cross(front);
    fovScale = tan(fov* (PI  * 0.5f / 180)) * 2;
  }
  
  inline Ray3 generateRay(float x,float y)const{
    Vector3 r = right.multiply((x - 0.5f) * fovScale);
    Vector3 u = up.multiply((y - 0.5f) * fovScale);
    return Ray3(eye,front.add(r).add(u).normalize());
  }
};



////////////////////
//简易速度测试框架

//等待一个回车输入
static void waitInputChar(){
  while (true){
    int c=getchar();
    if (c=='\n')
      break;
  }
}

//
void canvasTest(const Image32* ctx) {
  long w = ctx->width;
  long h = ctx->height;
  Color32* pixels = (Color32*)(ctx+1);
  memset4(pixels, 0, h*w);

  for (long y = 0; y < h; ++y){
    for (long x = 0; x < w; ++x){
      pixels[x].r = (UInt8)( x * 255 / w );
      pixels[x].g = (UInt8)( y * 255 / h );
      pixels[x].b = 0;
      pixels[x].a = 255;
    }
    (UInt8*&)pixels+=ctx->byte_width;
  }
}

///////////////////
void renderDepth(const Image32* ctx) {
  Union scene;
  scene.push(new Sphere(Vector3(0, 10, -10), 10));
  scene.push(new Plane(Vector3(0, 1, 0), 0));
  
  PerspectiveCamera camera( Vector3(0, 10, 10),Vector3(0, 0, -1),Vector3(0, 1, 0), 90);
  long maxDepth=20;
  
  
  long w = ctx->width;
  long h = ctx->height;
  Color32* pixels = (Color32*)(ctx+1);
  memset4(pixels, 0, h*w);
  
  scene.initialize();
  camera.initialize();
  
  float dx=1.0f/w;
  float dy=1.0f/h;
  float dD=255.0f/maxDepth;
  for (long y = 0; y < h; ++y){
    float sy = 1 - dy*y;
    for (long x = 0; x < w; ++x){
      float sx =dx*x;
      Ray3 ray(camera.generateRay(sx, sy));
      IntersectResult result = scene.intersect(ray);
      if (result.geometry) {
        UInt8 depth = (UInt8)( 255 - min(result.distance*dD,255.0f) );
        pixels[x].r = depth;
        pixels[x].g = depth;
        pixels[x].b = depth;
        pixels[x].a = 255;
      }
    }
    (UInt8*&)pixels+=ctx->byte_width;
  }
}

/////////////////
void renderNormal(const Image32* ctx) {
  Sphere scene(Vector3(0, 10, -10), 10);
  PerspectiveCamera camera( Vector3(0, 10, 10),Vector3(0, 0, -1),Vector3(0, 1, 0), 90);
  long maxDepth=20;
  
  
  long w = ctx->width;
  long h = ctx->height;
  Color32* pixels = (Color32*)(ctx+1);
  memset4(pixels, 0, h*w);
  
  scene.initialize();
  camera.initialize();
  
  float dx=1.0f/w;
  float dy=1.0f/h;
  float dD=255.0f/maxDepth;
  for (long y = 0; y < h; ++y){
    float sy = 1 - dy*y;
    for (long x = 0; x < w; ++x){
      float sx =dx*x;
      Ray3 ray(camera.generateRay(sx, sy));
      IntersectResult result = scene.intersect(ray);
      if (result.geometry) {
        pixels[x].r = (UInt8)( (result.normal.x + 1) * 128);
        pixels[x].g = (UInt8)( (result.normal.y + 1) * 128);
        pixels[x].b = (UInt8)( (result.normal.z + 1) * 128);
        pixels[x].a = 255;
      }
    }
    (UInt8*&)pixels+=ctx->byte_width;
  }
}

void rayTrace(const Image32* ctx) {
  Plane*  plane = new Plane(Vector3(0, 1, 0), 0);
  Sphere* sphere1 = new Sphere(Vector3(-10, 10, -10), 10);
  Sphere* sphere2 = new Sphere(Vector3(10, 10, -10), 10);
  Sphere* sphere3 = new Sphere(Vector3(0, 0, -10), 10);
  plane->material = new CheckerMaterial(0.1f);
  sphere1->material = new PhongMaterial(Color::red(), Color::white(), 16);
  sphere2->material = new PhongMaterial(Color::blue(), Color::white(), 16);
  sphere3->material = new PhongMaterial(Color::green(), Color::white(), 16);
  Union scene;
  scene.push(plane);
  scene.push(sphere1);
  scene.push(sphere2);
  scene.push(sphere3);
  PerspectiveCamera camera( Vector3(0, 5, 15),Vector3(0, 0, -1),Vector3(0, 1, 0), 90);
  
  long w = ctx->width;
  long h = ctx->height;
  Color32* pixels = (Color32*)(ctx+1);
  memset4(pixels, 0, h*w);
  
  scene.initialize();
  camera.initialize();
  
  float dx=1.0f/w;
  float dy=1.0f/h;
  for (long y = 0; y < h; ++y){
    float sy = 1 - dy*y;
    for (long x = 0; x < w; ++x){
      float sx =dx*x;
      Ray3 ray(camera.generateRay(sx, sy));
      IntersectResult result = scene.intersect(ray);
      if (result.geometry) {
        Color color = result.geometry->material->sample(ray, result.position, result.normal);
        color.saturate();
        pixels[x].r = (UInt8)( color.r*255);
        pixels[x].g = (UInt8)( color.g*255);
        pixels[x].b = (UInt8)( color.b*255);
        pixels[x].a = 255;
      }
    }
    (UInt8*&)pixels+=ctx->byte_width;
  }
}

/////////////
Color rayTraceRecursive(IGeometry* scene,const Ray3& ray,long maxReflect) {
  IntersectResult result = scene->intersect(ray);
  
  if (result.geometry){
    float reflectiveness = result.geometry->material->reflectiveness;
    Color color = result.geometry->material->sample(ray, result.position, result.normal);
    color = color.multiply(1 - reflectiveness);
    
    if ((reflectiveness > 0) && (maxReflect > 0)) {
            Vector3 r = result.normal.multiply(-2 * result.normal.dot(ray.direction)).add(ray.direction);
            Ray3 ray = Ray3(result.position, r);
            Color reflectedColor = rayTraceRecursive(scene, ray, maxReflect - 1);
            color = color.add(reflectedColor.multiply(reflectiveness));
    }
    return color;
  }else
    return Color::black();
}

void rayTraceRecursive(const Image32* ctx) {
  Plane*  plane = new Plane(Vector3(0, 1, 0), 0);
  Sphere* sphere1 = new Sphere(Vector3(-10, 10, -10), 10);
  Sphere* sphere2 = new Sphere(Vector3(10, 10, -10), 10);
  plane->material = new CheckerMaterial(0.1f,0.5);
  sphere1->material = new PhongMaterial(Color::red(), Color::white(), 16,0.25);
  sphere2->material = new PhongMaterial(Color::blue(), Color::white(), 16,0.25);
  Union scene;
  scene.push(plane);
  scene.push(sphere1);
  scene.push(sphere2);
  PerspectiveCamera camera( Vector3(0, 5, 15),Vector3(0, 0, -1),Vector3(0, 1, 0), 90);
  long maxReflect=3;
  
  long w = ctx->width;
  long h = ctx->height;
  Color32* pixels = (Color32*)(ctx+1);
  memset4(pixels, 0, h*w);
  
  scene.initialize();
  camera.initialize();
  
  float dx=1.0f/w;
  float dy=1.0f/h;
  for (long y = 0; y < h; ++y){
    float sy = 1 - dy*y;
    for (long x = 0; x < w; ++x){
      float sx =dx*x;
      Ray3 ray(camera.generateRay(sx, sy));
      Color color = rayTraceRecursive(&scene, ray, maxReflect);
      color.saturate();
      pixels[x].r = (UInt8)( color.r*255);
      pixels[x].g = (UInt8)( color.g*255);
      pixels[x].b = (UInt8)( color.b*255);
      pixels[x].a = 255;
    }
    (UInt8*&)pixels+=ctx->byte_width;
  }
}


int test_rayTraceRecursive() {
  int n=256;
  Image32* ctx=newImage(n,n, sizeof(Color32));
  Plane*  plane = new Plane(Vector3(0, 1, 0), 0);
  Sphere* sphere1 = new Sphere(Vector3(-10, 10, -10), 10);
  Sphere* sphere2 = new Sphere(Vector3(10, 10, -10), 10);
  plane->material = new CheckerMaterial(0.1f,0.5);
  sphere1->material = new PhongMaterial(Color::red(), Color::white(), 16,0.25);
  sphere2->material = new PhongMaterial(Color::blue(), Color::white(), 16,0.25);
  Union scene;
  scene.push(plane);
  scene.push(sphere1);
  scene.push(sphere2);
  PerspectiveCamera camera( Vector3(0, 5, 15),Vector3(0, 0, -1),Vector3(0, 1, 0), 90);
  long maxReflect=3;
  
  long w = ctx->width;
  long h = ctx->height;
  Color32* pixels;
  pixels = (Color32*)(ctx+1);
  memset4(pixels, 0, h*w);
  
  scene.initialize();
  camera.initialize();
  
  float dx=1.0f/w;
  float dy=1.0f/h;
  for (;;) {
    pixels = (Color32*)(ctx+1);
    for (long y = 0; y < h; ++y){
      float sy = 1 - dy*y;
      for (long x = 0; x < w; ++x){
        float sx =dx*x;
        Ray3 ray(camera.generateRay(sx, sy));
        Color color = rayTraceRecursive(&scene, ray, maxReflect);
        color.saturate();
        pixels[x].r = (UInt8)( color.r*255);
        pixels[x].g = (UInt8)( color.g*255);
        pixels[x].b = (UInt8)( color.b*255);
        pixels[x].a = 255;
      }
      (UInt8*&)pixels+=ctx->byte_width;
    }
    cvShowImage("test_rayTraceRecursive", ctx->height, ctx->width, (uchar*)(ctx+1), ctx->byte_width, 4);
    int ch=cvWaitKey(10);
    if ('q'==ch) break;
    if (ch>0) printf("%d\n", ch);
    if (2424832==ch) {camera.eye.x-=1;scene.initialize();}
    if (2555904==ch) {camera.eye.x+=1;scene.initialize();}
    //2424832 l
    //2555904 r
    //2490368 u
  //2621440 d
  }
  pfree(ctx);
  return 0;
}




typedef void (*T_proc)(const Image32* dst);

int test(const char* procName,const T_proc testPro,const long runCount){
  int n=512;
  Image32* dstPic=newImage(n,n, sizeof(Color32));
  utime_start(_start_time);
  for (long c=0;c<runCount;++c){
    testPro(dstPic);
  }
  double t0=utime_elapsed(_start_time);
  double fps=runCount/(t0);
  double ms=t0/runCount;
  printf("%20s: %18.2f秒\n", procName, t0);
  
  //保存结果图片
  char buf[256];
  _snprintf(buf, 256, "%s.bmp", procName);
  savebmpfile(buf, dstPic, 32, 32);
  return 0;
}

int test_raytracer(){
  //std::cout<<" 请输入回车键开始测试(可以把进程优先级设置为“实时”)> ";
  //waitInputChar();
  //std::cout<<std::endl;
  _chdir("D:\\code\\cstd\\src\\monofilt\\6张图片\\212");
  
#if 1
  test("canvasTest"    ,canvasTest    ,2);
  test("renderDepth"   ,renderDepth   ,2);
  test("renderNormal"  ,renderNormal  ,2);
  test("rayTrace"      ,rayTrace      ,2);
  test("rayTraceRecursive",rayTraceRecursive,2);
#endif
  
  //waitInputChar();
  return 0;
}

