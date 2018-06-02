
//次物体初始颜色:黑色
#define INIT_SUBObject3d(pS)  FRGBASET(pS->clr, 1.0f, 1.0f, 1.0f, 1.0f)
#define OBJ_VISABEL   0x0001  //顶点可见
typedef struct _Vertex3d {
  //DWORD stage;
  VECTOR3D coord;   // 顶点坐标
  VECTOR3D normal;  // 顶点法线
  VECTOR3D scrn;   // 最终透视到屏幕上的点
  FRGBA clr;//次物体颜色:浮点颜色,RGBA颜色模型
} Vertex3d;
typedef struct _Vertex3d2 {
  //DWORD stage;
  FRGBA clr;//次物体颜色:浮点颜色,RGBA颜色模型
  VECTOR3D coord;   // 顶点坐标
} Vertex3d2;
void initVertex3d(Vertex3d* pV, int n)
{
  for (; n--; ++pV) {
    INIT_SUBObject3d(pV);
    //顶点坐标:坐标原点
    V3SET(pV->coord, 0.0f, 0.0f, 0.0f);
    //顶点法线:指向Z轴
    V3SET(pV->normal, 0.0f, 0.0f, 1.0f);
  }
}
void initVertex3d2(Vertex3d2* pV, int n)
{
  for (; n--; ++pV) {
    INIT_SUBObject3d(pV);
    //顶点坐标:坐标原点
    V3SET(pV->coord, 0.0f, 0.0f, 0.0f);
  }
}
float MaxRadius(HMC hV)
{
  int i, j;
  float mr, d;
  GetMatElem(mr, hV, 0, 0, float);
  mr = ABS(mr);

  for (i = 0; i < hV->h; ++i) {
    for (j = 0; j < hV->w; ++j) {
      d = _PixelT(hV, i, j, float);
      d = ABS(d);
      mr = MAX(mr, d);
    }
  }

  return mr;
}
// 中心化
void Centralization(int vListLen, Vertex3d2* vList, float sc)
{
  int i;
  VECTOR3D maxv = vList[0].coord, minv = vList[0].coord, v;
  float r, invr;

  for (i = 1; i < vListLen; ++i) {
    maxv.x = MAX(maxv.x, vList[i].coord.x), minv.x = MIN(minv.x, vList[i].coord.x);
    maxv.y = MAX(maxv.y, vList[i].coord.y), minv.y = MIN(minv.y, vList[i].coord.y);
    maxv.z = MAX(maxv.z, vList[i].coord.z), minv.z = MIN(minv.z, vList[i].coord.z);
  }

  v.x = (maxv.x + minv.x) / 2;
  v.y = (maxv.y + minv.y) / 2;
  v.z = (maxv.z + minv.z) / 2;

  for (i = 0; i < vListLen; ++i) {
    vList[i].coord.x -= v.x;
    vList[i].coord.y -= v.y;
    vList[i].coord.z -= v.z;
  }

  v.x = (maxv.x - minv.x) / 2;
  v.y = (maxv.y - minv.y) / 2;
  v.z = (maxv.z - minv.z) / 2;
  r = MAX(v.x, v.y);
  r = MAX(r, v.z);
  invr = sc / r;

  for (i = 0; i < vListLen; ++i) {
    vList[i].coord.x *= invr;
    vList[i].coord.y *= invr;
    vList[i].coord.z *= invr;
  }

  return ;
}

typedef struct _CFacet {
  FRGBA clr;//次物体颜色:浮点颜色,RGBA颜色模型
  //小面法线
  //VECTOR3D normal;
  //在图形处理中,小面一般为三角形或四边形
  //小面顶点序号
#define A3SET(A, A0, A1, A2)  ((A)[0]=A0, (A)[1]=A1, (A)[2]=A2)
  int vIndex[3];
  int tIndex[3];
  //int vIndexLen;
} CFacet;
void initCFacet(CFacet* pF, int n)
{
  for (; n--; pF++) {
    INIT_SUBObject3d(pF);
    //小面法线:指向Z轴
    //V3SET(pF->normal, 0.0f, 0.0f, 1.0f);
  }
}
//三维物体,定义或变换于世界坐标系
// 缺省坐标系
// X    水平自左向右 [-1.0, 1.0]
// Y    垂直自下而上 [-1.0, 1.0]
// Z    水平由里向外 [-1.0, 1.0]
//需要应用该类的数据的类有: CPRender等,可以设置这些类为该类的友好类,然后增加保护措施和提供访问数据的机制.
//为了方便访问,我们简单地设置它们为公有成员变量
typedef struct _Object3d {
  char szName[256];
  //顶点表
  Vertex3d2* vList;
  int vListLen;
  //面表
  CFacet* sList;
  int sListLen;
  //面表
  POINT* tList;
  int tListLen;

  //变换矩阵
  //MATRIX3D mGeomTrans;
  //自动颜色,还是赋值颜色
  //自动颜色是指由顶点颜色决定小面和边的颜色
  //赋值颜色是指从外部给小面和边赋颜色值
  //但是,顶点颜色总是需要赋值或通过光照计算
  BOOL bAutoColor;
  float maxRadius; // 最大半径
  Image* tex;
} Object3d;
int SwapXYZ(Object3d* pO, int xIndex, int yIndex, int zIndex)
{
  int i, t, index[3] = {xIndex, yIndex, zIndex};
  uchar* u = (uchar*)(&pO->vList->coord);
  int bw = sizeof(Vertex3d2);

  for (i = 0; i < pO->vListLen; ++i) {
    float* f = (float*)(u + bw * i);
    float x = f[index[0]], y = f[index[1]], z = f[index[2]];
    f[0] = x, f[1] = y, f[2] = z;
  }

  i = (xIndex > yIndex) + (yIndex > zIndex) + (xIndex > zIndex);

  if (0 == i % 2) {
    return 0;
  }

  for (i = 0; i < pO->sListLen; ++i) {
    SWAP(pO->sList[i].vIndex[0], pO->sList[i].vIndex[1], t);
    SWAP(pO->sList[i].tIndex[0], pO->sList[i].tIndex[1], t);
  }

  return 0;
}
void initObject3d(Object3d* pO, int nV, int nF, int nT)
{
  memset(pO, 0, sizeof(Object3d));
  pO->vListLen = nV;
  pO->sListLen = nF;
  pO->tListLen = nT;

  pO->bAutoColor = (TRUE);
  //M3IDENTITY(pO->mGeomTrans);
  pO->vList = MALLOC(Vertex3d2, pO->vListLen);
  pO->sList = MALLOC(CFacet, pO->sListLen);

  if (pO->tListLen) {
    pO->tList = MALLOC(POINT, pO->tListLen);
  }

  initCFacet(pO->sList, pO->sListLen);
  initVertex3d2(pO->vList, pO->vListLen);
}
void delObject3d(Object3d* pO, int n)
{
  for (; n--; ++pO) {
    if (pO->vList) {
      free(pO->vList);
      pO->vListLen = 0;
    }

    if (pO->sList) {
      free(pO->sList);
      pO->sListLen = 0;
    }

    if (pO->tList) {
      free(pO->tList);
      pO->tList = 0;
    }

    if (pO->tex) {
      free(pO->tex);
      pO->tex = 0;
    }
  }
}
//克隆
Object3d* cloneObject3D(Object3d* pO)
{
  Object3d* pCloned = (Object3d*)malloc(sizeof(Object3d));
  //克隆几何及属性
  *pCloned = *pO;
  pCloned->vList = MEMDUP(Vertex3d2, pO->vList, pO->vListLen);
  pCloned->sList = MEMDUP(CFacet, pO->sList, pO->sListLen);
  return pCloned;
}
// 计算顶点、小面法线
VECTOR3D CalcSingleFacetNoraml(const VECTOR3D* v1,  const VECTOR3D* v2,  const VECTOR3D* v3)
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
    VECTOR3D vector1 = {(x2 - x1), (y2 - y1), (z2 - z1)};
    VECTOR3D vector2 = {(x3 - x1), (y3 - y1), (z3 - z1)};
    VECTOR3D noraml;
    V3CROSS(noraml, vector1, vector2);
    return noraml;
  }
}

#define UNITIZE(V)  {float fMag = (float)sqrt(V.x * V.x + V.y * V.y + V.z * V.z);if (fMag < 0.0001f) {fMag = 1.0f;}V.x /= fMag;V.y /= fMag;V.z /= fMag;}


//下面是定义一些块的ID号
#define CHUNK_VERSION        0x0002  // 3ds文件的版本
#define CHUNK_RGB1           0x0010  // 3 floats of RGB
#define CHUNK_RGB2           0x0011  // 3 bytes of RGB
#define CHUNK_RGB3           0x0012  //?
#define CHUNK_AMOUNT         0x0030  // 基本块(Primary Chunk)，位于文件的开始
#define CHUNK_MAIN           0x4D4D  //主块(==整个文件)
#define CHUNK_OBJMESH        0x3D3D  //网格对象的版本号
#define CHUNK_BKGCOLOR       0x1200  //
#define CHUNK_AMBCOLOR       0x2100  //
#define CHUNK_OBJBLOCK       0x4000  //物体块，对象的面、顶点等信息
#define CHUNK_TRIMESH        0x4100  //新的网格对象
#define CHUNK_VERTLIST       0x4110  //顶点块
#define CHUNK_FACELIST       0x4120  //面列表块
#define CHUNK_FACEMAT        0x4130  //面材质块
#define CHUNK_MAPLIST        0x4140  //对象的UV纹理坐标
#define CHUNK_SMOOLIST       0x4150  //
#define CHUNK_TRMATRIX       0x4160  //
#define CHUNK_LIGHT          0x4600  //
#define CHUNK_SPOTLIGHT      0x4610  //
#define CHUNK_CAMERA         0x4700  //
#define CHUNK_MATERIAL       0xAFFF  //纹理信息
#define CHUNK_MATNAME        0xA000  //材质名称
#define CHUNK_AMBIENT        0xA010  //
#define CHUNK_DIFFUSE        0xA020  //对象材质的颜色
#define CHUNK_SPECULAR       0xA030  //
#define CHUNK_SHININESS      0xA040  //
#define CHUNK_SHINE_STRENGTH 0xA041  //
#define CHUNK_DOUBLESIDED    0xA081  //
#define CHUNK_TEXTURE        0xA200  //新材质的头部
#define CHUNK_BUMPMAP        0xA230  //
#define CHUNK_MAPFILENAME    0xA300  //保存纹理的文件名
#define CHUNK_MAPOPTIONS     0xA351  //
#define CHUNK_TILINGU        0xA354  //U方向缩放系数
#define CHUNK_TILINGV        0xA356  //V方向缩放系数
#define CHUNK_OFFSETU        0xA358  //U方向偏移系数
#define CHUNK_OFFSETV        0xA35A  //V方向偏移系数
#define CHUNK_ROTATEW        0xA35C  //W方向旋转角
#define CHUNK_KEYFRAMER      0xB000  //所有关键帧信息的头部

#pragma pack(1)
/** 面的结构定义 */
typedef struct _tFace {
  unsigned short vertIndex[4];         /**< 顶点索引 */
} tFace;
/** 材质信息结构体 */
typedef struct _tMatInfo {
  char  strName[255];        /**< 纹理名称 */
  char  strTexture[255];        /**< 纹理文件名称 */
  int   texureId;          /**< 纹理ID */
  float uTile;           /**< u 重复 */
  float vTile;           /**< v 重复 */
  float uOffset;             /**< u 纹理偏移 */
  float vOffset;           /**< v 纹理偏移 */
  Image tex[1];

  float   pAmbient[4];
  float   pDiffuse[4]; // 对象的RGB颜色
  float   pSpecular[4];
  float   fOffsetU;
  float   fOffsetV;
  float   fTilingU;
  float   fTilingV;
  float   fRotationW;

} tMatInfo;
/** 对象信息结构体 */
typedef struct _t3DObject {
  int  numOfVerts;          /**<  模型中顶点的数目 */
  int  numOfFaces;        /**< 模型中面的数目 */
  int  numTexVertex;        /**< 模型中纹理坐标的数目 */
  int  materialID;        /**< 纹理ID */
  BOOL bHasTexture;       /**< 是否具有纹理映射 */
  Vector3  vtxCenter;       /**< 对象的中心坐标 */
  char strName[255];        /**< 对象的名称 */
  Vector3*  pVerts;       /**< 对象的顶点 */
  //Vector3*  pNormals;         /**< 对象的法向量 */
  Vector2*  pTexVerts;      /**< 纹理UV坐标 */
  tFace* pFaces;          /**< 对象的面信息 */
  Vertex3d* pV;
} t3DObject;

#define MAX_MATERIALS  1000
#define MAX_OBJECTS  1000
/** 模型信息结构体 */
typedef struct _t3DModel {
  int numOfObjects;       /**< 模型中对象的数目 */
  int numOfMaterials;     /**< 模型中材质的数目 */
  tMatInfo pMaterials[MAX_MATERIALS];   /**< 材质链表信息 */
  t3DObject pObject[MAX_OBJECTS];     /**< 模型中对象链表信息 */
} t3DModel;

/** 块信息的结构 */
typedef struct _tChunk {
  unsigned short ID;      /**< 块的ID */
  unsigned int length;      /**< 块的长度 */
} tChunk;
#pragma pack()






#include "loadObject.inl"
