
#pragma comment (lib, "vfw32")


#include <tchar.h>
#include <math.h>
#include <stdio.h>
#include <vfw.h>
#include <xmmintrin.h>


#define _PI 3.1415926f                      // Value of PI
#define _PIXEL  DWORD                     // Pixel
#define _BITS_PER_PIXEL 32                    // Color depth
//#define _RGB(r,g,b) (((r) << 16) | ((g) << 8) | (b))      // Convert to RGB
//#define _GetRValue(c) ((BYTE)(((c) & 0x00FF0000) >> 16))    // Red color component
//#define _GetGValue(c) ((BYTE)(((c) & 0x0000FF00) >> 8))   // Green color component
//#define _GetBValue(c) ((BYTE)((c) & 0x000000FF))        // Blue color component
#define CAMERA_OFFSET 0.025f                  // Camera offset
#define MAX_OBJECTS   4096                  // Maximum number of objects
#define MAX_LIGHTS    1                   // Maximum number of lights
#define MD2_MAGIC_NUM ('I' + ('D' << 8) + ('P' << 16) + ('2' << 24))  // MD2 file magic number
#define MD2_VERSION   8
//#define SWAP(a,b,t)   {t=a; a=b; b=t;}
//#define ABS(X)  (((X)<0)?-(X):(X))

typedef long fixed;                           // Our new fixed point type
#define itofx(x) ((x) << 8)                   // Integer to fixed point
#define ftofx(x) (long)((x) * 256)            // Float to fixed point
#define dtofx(x) (long)((x) * 256)                // Double to fixed point
#define fxtoi(x) ((x) >> 8)                   // Fixed point to integer
#define fxtof(x) ((float) (x) / 256)              // Fixed point to float
#define fxtod(x) ((double)(x) / 256)              // Fixed point to double
#define Mulfx(x,y) (((x) * (y)) >> 8)             // Multiply a fixed by a fixed
#define Divfx(x,y) (((x) << 8) / (y))             // Divide a fixed by a fixed


typedef enum __SHADING_MODE {
  _SHADING_MODE_NONE = 0x00,
  _SHADING_MODE_CONSTANT = 0x01,
  _SHADING_MODE_FLAT = 0x02,
  _SHADING_MODE_GOURAUD = 0x04,
  _SHADING_MODE_TEXTURE = 0x08
} SHADING_MODE;

typedef enum __TEXTURE_FILTERING {
  _TEXTURE_FILTERING_NONE = 0x0000,
  _TEXTURE_FILTERING_BILINEAR = 0x0001

} TEXTURE_FILTERING;

typedef enum __LIGHT_TYPE {
  _LIGHT_TYPE_NONE = 0x0000,
  _LIGHT_TYPE_AMBIENT = 0x0001,
  _LIGHT_TYPE_INFINITE = 0x0002,
  _LIGHT_TYPE_POINT = 0x0004
} LIGHT_TYPE;

typedef struct MD2HEADER {
  int identifier;
  int version;
  int skin_width;
  int skin_height;
  int framesize;
  int num_skins;
  int num_verts;
  int num_textcoords;
  int num_polys;
  int num_openGLcmds;
  int num_frames;
  int offset_skins;
  int offset_textcoords;
  int offset_polys;
  int offset_frames;
  int offset_openGlcmds;
  int offset_end;
} MD2HEADER;

typedef struct MD2POLY {
  unsigned short vindex[3];
  unsigned short tindex[3];
} MD2POLY;

typedef struct MD2TEXTCOORD {
  short u;
  short v;
} MD2TEXTCOORD;

typedef struct MD2POINT {
  unsigned char v[3];
  unsigned char normal_index;

} MD2POINT;

typedef struct MD2FRAME {
  float scale[3];
  float translate[3];
  char name[16];
  MD2POINT vlist[1];
} MD2FRAME;

typedef struct POINT2D {
  long x;
  long y;
} POINT2D;

typedef struct POINT3D {
  float x;
  float y;
  float z;
  float w;
} POINT3D;

#define V3SET(C, X, Y, Z)  (C.x = X,C.y = Y,C.z = Z,C.w = 1.f)
#define V3OPT(C, A, B, OP)  (C.x = A.x OP B.x,C.y = A.y OP B.y,C.z = A.z OP B.z,C.w = 1.f)
#define V3ADD(C, A, B)  V3OPT(C, A, B, +)
#define V3SUB(C, A, B)  V3OPT(C, A, B, -)
#define V3MUL(C, A, B)  V3OPT(C, A, B, *)
// 向量叉积
#define V3CROSS(C, A, B)  {POINT3D V; V.x = (A.y * B.z - A.z * B.y); V.y = (A.z * B.x - A.x * B.z); V.z = (A.x * B.y - A.y * B.x); C=V; }
//标量与向量之积
#define V3MULA(C, A, fs)    (C.x = A.x * fs,C.y = A.y * fs,C.z = A.z * fs)
//向量内积
#define V3DOT(A, B)         (A.x * B.x + A.y * B.y + A.z * B.z)
#define V3DOT2(A, X, Y, Z)  (A.x * X + A.y * Y + A.z * Z)
#define V3MAGSQ(A)          (A.x * A.x + A.y * A.y + A.z * A.z)
#define V3MAG(A)            ((float)sqrt(V3MAGSQ(A)))
//两向量矢量端点之间的距离
#define V3DIST(A, B)         (float)sqrt((A.x-B.x)*(A.x-B.x) + (A.y-B.y)*(A.y-B.y) + (A.z-B.z)*(A.z-B.z))
//#define V3UNIT(A)           (float)sqrt((A.x-B.x)*(A.x-B.x) + (A.y-B.y)*(A.y-B.y) + (A.z-B.z)*(A.z-B.z))

//向量单位化，返回一个单位向量
POINT3D Unit(POINT3D* pV)
{
  POINT3D vector;
  float fMag = (float)sqrt(pV->x * pV->x + pV->y * pV->y + pV->z * pV->z);

  if (fMag < 0.0001f) {
    fMag = 1.0f;
  }

  vector.x = pV->x / fMag;
  vector.y = pV->y / fMag;
  vector.z = pV->z / fMag;
  return vector;
}

//自身单位化
void Unitize(POINT3D* pV)
{
  float fMag = (float)sqrt(pV->x * pV->x + pV->y * pV->y + pV->z * pV->z);

  if (fMag < 0.0001f) {
    fMag = 1.0f;
  }

  pV->x /= fMag;
  pV->y /= fMag;
  pV->z /= fMag;
}

#define PIE PI
//将this向量进行旋转
//绕轴x旋转, fTheta为角度单位
void RotateX(POINT3D* pV, float fTheta)
{
  //将角度转化为弧度
  float fRad = (float)((fTheta * PIE) / 180.0);
  float yy = (float)(pV->y * cos(fRad) - pV->z * sin(fRad));
  float zz = (float)(pV->y * sin(fRad) + pV->z * cos(fRad));
  pV->y = yy;
  pV->z = zz;
}

//将this向量进行旋转
//绕轴y旋转, fTheta为角度单位
void RotateY(POINT3D* pV, float fTheta)
{
  //将角度转化为弧度
  float fRad = (float)((fTheta * PIE) / 180.0);
  float zz = (float)(pV->z * cos(fRad) - pV->x * sin(fRad));
  float xx = (float)(pV->z * sin(fRad) + pV->x * cos(fRad));
  pV->x = xx;
  pV->z = zz;
}

//将this向量进行旋转
//绕轴y旋转, fTheta为角度单位
void RotateZ(POINT3D* pV, float fTheta)
{
  //将角度转化为弧度
  float fRad = (float)((fTheta * PIE) / 180.0);
  float xx = (float)(pV->x * cos(fRad) - pV->y * sin(fRad));
  float yy = (float)(pV->x * sin(fRad) + pV->y * cos(fRad));
  pV->x = xx;
  pV->y = yy;
}

//绕任意轴旋转, fTheta为角度,
//必须指出, 这里的轴为一个自由向量, 自由向量都可以从坐标原点画出
void Rotate(POINT3D* pV, float fTheta,  POINT3D axis)
{
  float fx, fy, fz, fRad, c, s, xx, yy, zz;
  //单位化
  Unitize(&axis);

  //旋转轴的方向数
  fx = axis.x,  fy = axis.y,  fz = axis.z;

  //将角度转化为弧度
  fRad = (float)((fTheta * PIE) / 180.0);

  //角度的正弦和余弦
  c = (float)cos(fRad);
  s = (float)sin(fRad);


  //多次变换的合成,参考相应的计算机图形学书籍,
  //变换的具体过程请参考:
  //<计算机图形学>(第三版),孙家广,清华大学出版社

  //下面是最后的结果
  xx = (fx * fx * (1.0f - c) + c)      * pV->x +
      (fx * fy * (1.0f - c) - fz * s) * pV->y +
      (fx * fz * (1.0f - c) + fy * s) * pV->z;

  yy = (fy * fx * (1.0f - c) + fz * s) * pV->x +
      (fy * fy * (1.0f - c) + c)      * pV->y +
      (fy * fz * (1.0f - c) - fx * s) * pV->z;

  zz = (fz * fx * (1.0f - c) - fy * s) * pV->x +
      (fz * fy * (1.0f - c) + fx * s) * pV->y +
      (fz * fz * (1.0f - c) + c)      * pV->z;

  //返回最后结果
  pV->x = xx;
  pV->y = yy;
  pV->z = zz;
}

//绕任意轴旋转, fTheta为角度,
//(x, y, z)为旋转轴向量
void Rotate2(POINT3D* pV, float fTheta,  float x, float y, float z)
{
  POINT3D vector;
  V3SET(vector, x, y, z);
  Rotate(pV, fTheta, vector);
}

//计算小面法线
POINT3D Noraml(const POINT3D* v1,  const POINT3D* v2,  const POINT3D* v3)
{
  //右手规则
  POINT3D vector1 = {(v2->x - v1->x), (v2->y - v1->y), (v2->z - v1->z)};
  POINT3D vector2 = {(v3->x - v1->x), (v3->y - v1->y), (v3->z - v1->z)};
  POINT3D noraml;
  V3CROSS(noraml, vector1, vector2);
  return noraml;
}

//计算小面法线
POINT3D Noraml2(const POINT3D* v1,  const POINT3D* v2,  const POINT3D* v3)
{
  //如果一个三角形中包含无穷远点, 则该三角形是一个畸形三角形,
  float w1 = v1->w,  w2 = v2->w,  w3 = v3->w;

  if (ABS(w1) <= 0.00001f) {
    w1 = 1.0f;
  }

  if (ABS(w2) <= 0.00001f) {
    w2 = 1.0f;
  }

  if (ABS(w3) <= 0.00001f) {
    w3 = 1.0f;
  }

  {

    float x1 = v1->x / w1,  y1 = v1->y / w1,  z1 = v1->z / w1;
    float x2 = v2->x / w2,  y2 = v2->y / w2,  z2 = v2->z / w2;
    float x3 = v3->x / w3,  y3 = v3->y / w3,  z3 = v3->z / w3;


    POINT3D vector1 = {(x2 - x1), (y2 - y1), (z2 - z1)};
    POINT3D vector2 = {(x3 - x1), (y3 - y1), (z3 - z1)};
    POINT3D noraml;
    V3CROSS(noraml, vector1, vector2);
    return noraml;
  }
}


float Length_Vector3D(POINT3D v)
{
  return((float)sqrt(v.x * v.x + v.y * v.y + v.z * v.z));
}

POINT3D Cross_Vector3D(POINT3D v1, POINT3D v2)
{
  POINT3D n, v;
  float v1_len = Length_Vector3D(v1);
  float v2_len = Length_Vector3D(v2);

  float cos_fi = (v1.x * v2.x + v1.y * v2.y + v1.z * v2.z) / (v1_len * v2_len);
  float sin_fi = (float)sqrt(1.0f - cos_fi * cos_fi);

  V3SET(n, v1.y * v2.z - v2.y * v1.z, -v1.x * v2.z + v2.x * v1.z, v1.x * v2.y - v2.x * v1.y);

  v.x = sin_fi * n.x;
  v.y = sin_fi * n.y;
  v.z = sin_fi * n.z;
  v.w = 1.0f;
  return v;
}

typedef struct POINT3DI {
  long x;
  long y;
  long z;
  long w;
} POINT3DI;

typedef struct VERTEX3D {
  POINT3D p;
  POINT3D n;
  _PIXEL color;
  POINT2D uv;
} VERTEX3D;

typedef struct PLANE3D {
  POINT3D p;
  POINT3D n;
} PLANE3D;

typedef struct MATRIX3D {
  float m00, m01, m02, m03;
  float m10, m11, m12, m13;
  float m20, m21, m22, m23;
  float m30, m31, m32, m33;
} MATRIX3D;

typedef struct BITMAPFILE {
  BITMAPFILEHEADER bfh;
  BITMAPINFOHEADER bih;
  DWORD dwSize;
  LPDWORD lpData;
} BITMAPFILE;

typedef struct TEXTUREINFO {
  LPDWORD lpTexture;
  DWORD dwSize;
  DWORD dwPitch;
  DWORD dwTextureOffset;
  TEXTURE_FILTERING filter;

} TEXTUREINFO;

typedef struct MATERIALINFO {
  _PIXEL color;
  float ka, kd, ks, power;
  _PIXEL ra, rd, rs;
  TEXTUREINFO* lpTextureInfo;
} MATERIALINFO;

typedef struct LIGHTINFO {
  LIGHT_TYPE type;
  _PIXEL c_ambient;
  _PIXEL c_diffuse;
  _PIXEL c_specular;
  POINT3D pos;
  POINT3D dir;
  float kc, kl, kq;
  float spot_inner, spot_outer;
  float pf;
  BOOL bActive;
} LIGHTINFO;

typedef struct POLY {
  VERTEX3D* vList;
  VERTEX3D* tList;
  POINT3D* uvList;
  int vertex[3];
  int texel[3];
  _PIXEL color;
  _PIXEL shade_color;
  _PIXEL shade_color1;
  _PIXEL shade_color2;
  _PIXEL shade_color3;
  BYTE shade;
  BOOL bBackface;

} POLY;

typedef struct POLYLIST {
  VERTEX3D vList[3];
  VERTEX3D tList[3];
  POINT3D uvList[3];
  _PIXEL color;
  _PIXEL color1;
  _PIXEL color2;
  _PIXEL color3;
  BYTE shade;
  TEXTUREINFO* lpTextureInfo;
} POLYLIST;

typedef struct RENDERLIST {
  POLYLIST** poly_ptrs;
  POLYLIST* poly_data;
  int num_polys;
  int max_polys;

} RENDERLIST;

typedef struct _TRIANGLE {
  POINT3DI p1;
  POINT3DI p2;
  POINT3DI p3;
  _PIXEL color1;
  _PIXEL color2;
  _PIXEL color3;
  _PIXEL color;
  POINT2D t1;
  POINT2D t2;
  POINT2D t3;
  BYTE shadingMode;
  TEXTUREINFO texture;
} TRIANGLE, *LPTRIANGLE;

typedef struct OBJECT3D {
  _TCHAR szName[255];
  int numberVertices;
  int numberPolys;
  VERTEX3D* vlist_local;
  VERTEX3D* vlist_trans;
  POLY* plist;
  POINT3D world_pos;
  float max_radius;
  BOOL bCulled;
  MATERIALINFO* lpMaterialInfo;
  POINT3D* list_texel;
  float ux, uy, uz;
  int md2_num_verts;
  int md2_num_textcoords;
  int md2_num_polys;
  int md2_num_frames;
  int md2_curr_frame;
  TEXTUREINFO* md2_skin;
  MD2POLY* md2_polys;
  POINT3D* md2_vlist;
  POINT3D* md2_tlist;
} OBJECT3D, *_LPOBJECT3D;

typedef struct CAMERA3D {
  POINT3D pos;
  POINT3D dir;
  POINT3D u;
  POINT3D v;
  POINT3D n;
  POINT3D target;
  float view_dist_h;
  float view_dist_v;
  float fov;
  float near_clip_z;
  float far_clip_z;
  float viewplane_width;
  float viewplane_height;
  float viewport_width;
  float viewport_height;
  float viewport_center_x;
  float viewport_center_y;
  float aspect_ratio;
  float zoom;
  PLANE3D rt_clip_plane;
  PLANE3D lt_clip_plane;
  PLANE3D tp_clip_plane;
  PLANE3D bt_clip_plane;
  MATRIX3D mcam;
  MATRIX3D mper;
  MATRIX3D mscr;
} CAMERA3D, *_LPCAMERA3D;

#include "3DObj.inl"

int test_3D()
{
  POINT3D pos = {0};
  OBJECT3D obj[2];
  char* plgf = "D:/code/c/游戏引擎/code/Triangle Renderer Project/Cube.plg";
  loadObject_PLG(obj, 2, plgf);
  delObject(obj, 2);
  return 0;
}

