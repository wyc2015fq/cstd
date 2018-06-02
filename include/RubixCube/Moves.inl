

#include <math.h>


// struct CSide (facet of Rubik's Cube)
// name of faces (central facet of each faces of the Rubik's Cube)
static char pid[] = { "    D     F L R B     U    " };

// translate integer, float and reverse

static float gVex(t_Mask mask, float module)
{
  mask &= 3;
  ASSERT(mask != 0);
  return (mask == 2) ? 0.0f : (mask == 3) ? module : -module;
}

static WORD gfPos(float x)
{
  return ((x < -0.1f) ? 1 : ((x > 0.1f) ? 3 : 2));
}

float f3Module(const float* v)
{
  return (float)sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
}

void f3Sub(float* v, const float* y)
{
  v[0] -= y[0], v[1] -= y[1], v[2] -= y[2];
}

void f3SetMask(float* v, t_Mask pos,        // position
    const float* pMat,    // orientation matrix
    float mod)        // module of face size
{
  float x = gVex(pos >>= 9, mod), y = gVex(pos >>= 2, mod), z = gVex(pos >>= 2, mod);

  v[0] = x * pMat[0 * 3 + 0] + y * pMat[1 * 3 + 0] + z * pMat[2 * 3 + 0];
  v[1] = x * pMat[0 * 3 + 1] + y * pMat[1 * 3 + 1] + z * pMat[2 * 3 + 1];
  v[2] = x * pMat[0 * 3 + 2] + y * pMat[1 * 3 + 2] + z * pMat[2 * 3 + 2];
}


t_Mask f3Pack(const float* v)           // compact an unit vector
{
  return (gfPos(v[0]) << 9) | (gfPos(v[1]) << 11) | (gfPos(v[2]) << 13);
}


// sommet en projection 2D (position dans le plan x, y et eloignement du plan z)
typedef struct {
  POINT v;
  int z;
} t_VERTEX;

static float m_ds3D = 0.42f;    // half a side length of a 3D face
static float m_ds2D = 0.45f;    // half a side length of a 2D face

// Calculate the 8 vertices of a cube in perspective 2D
void CubeVertexList_Set(t_VERTEX* m_vertex,
    t_Mask cube,   // cube elementaire
    float* pMeo, // matrice d'orientation du cube
    float dim,     // module de construction du cube
    float size,    // dimension du RubixCube affiche
    float yPos)    // distance observateur)
{
  float x, y, z;
  float Mco[9] = {0};
  M33_M33(Mco, t_Matrix(cube), pMeo, dim);// orientation and size
  float pos[3] = {0};
  f3SetMask(pos, cube, pMeo, dim);          // position of Rubik's Cube
  float x0 = pos[0], y0 = pos[1] + yPos, z0 = pos[2];
  float u = m_ds3D, v = u, w = u;
  t_VERTEX* pV = m_vertex;

  // calculate the 8 vertices of the cube by symmetry 2-2
  for (short i = 0; i < 2; ++i, u = -u) {
    for (short j = 0; j < 2; ++j, v = -v) {
      for (short k = 0; k < 2; ++k, w = -w, pV++) {
        // passing from reference of the cubic to reference observer
        x = x0 + Mco[0 * 3 + 0] * u + Mco[1 * 3 + 0] * v + Mco[2 * 3 + 0] * w,
        z = y0 + Mco[0 * 3 + 1] * u + Mco[1 * 3 + 1] * v + Mco[2 * 3 + 1] * w,
        y = z0 + Mco[0 * 3 + 2] * u + Mco[1 * 3 + 2] * v + Mco[2 * 3 + 2] * w;

        // perspective translation to reference screen 2D (simplified)
        if (z == 0.0) {
          z = 0.0001f;  // to avoid the crash if 0
        }

        pV->v.x = int(x / z * size);
        pV->v.y = int(-y / z * size);
        pV->z   = int(-z);// observer distance for Z buffer management
      }
    }
  }
}
// Calculate the 8 vertices of a cube in 2D
void CubeVertexList_Set(t_VERTEX* m_vertex,
    t_Mask cube,   // cube elementaire
    const float* Meo,  // matrice d'orientation du cube
    float dim)     // dimension face elementaire
{
  BYTE i, j, k;
  float u = m_ds2D, v = u, w = u;
  float Mco[9] = {0};
  M33_M33(Mco, t_Matrix(cube), Meo, dim);// orientation and size
  float pos[3] = {0};
  f3SetMask(pos, cube, Meo, dim);          // cube position
  t_VERTEX* pV = m_vertex;               // 8 vertices of the cube

  // calculate the 8 vertices of the cube by symmetry 2-2
  for (i = 0; i < 2; ++i, u = -u) {
    for (j = 0; j < 2; ++j, v = -v) {
      for (k = 0; k < 2; ++k, w = -w, pV++) {
        // passing from reference of the cubic to reference observer
        // (expanded flat)
        pV->v.x = int(pos[0] + Mco[0 * 3 + 0] * u + Mco[1 * 3 + 0] * v + Mco[2 * 3 + 0] * w);
        pV->z =  -int(Mco[0 * 3 + 1] * u + Mco[1 * 3 + 1] * v + Mco[2 * 3 + 1] * w);
        pV->v.y = -int(pos[2] + Mco[0 * 3 + 2] * u + Mco[1 * 3 + 2] * v + Mco[2 * 3 + 2] * w);
      }
    }
  }
}

typedef struct CSide {              // face of a cube (projection 2 D)
  int     m_z, m_zt[4];   // observer distance from the face
  IPOINT  m_poly[4];      // coordinates of the face
  IRECT   m_box;          // box of the face
  t_Mask  m_cube;         // Cube owner of the face
  BYTE    m_noSide;       // number of the face (color)
  BYTE    m_noDscr;       // descripteur en déroule ?plat (idem couleur)
  struct CSide*  m_pSide;        // pointer to the side classified
} CSide;

void CSide_Init(CSide* s)
{
  s->m_pSide = NULL;
  s->m_noSide = 0xFF;
}

inline BYTE CSide_GetCubeNo(const CSide* s)
{
  return cubeNo(s->m_pSide->m_cube);
}
inline void CSide_Swap(CSide* s, CSide* pSide)
{
  CSide* pTmp = s->m_pSide;
  s->m_pSide = pSide->m_pSide, pSide->m_pSide = pTmp;
}
inline void CSide_Restore(CSide* s)
{
  s->m_pSide = s;
}
//inline IRECT CSide_GetBox(const CSide* s) {  return s->m_box;}
// Calculating the bounding rectangle (box) of a list of faces
IRECT CSide_GetRect(const CSide* pSide, int nbSide)
{
  int x1 = pSide->m_poly[0].x, x2 = x1, y1 = pSide->m_poly[0].y, y2 = y1;

  for (int n = 0; n < nbSide && pSide->m_pSide; n++, pSide++) {
    const IPOINT* pt = pSide->m_poly;

    for (int i = 0; i < 4; i++, pt++) {
      // calculating the bounding rectangle (box) of the facet
      if (pt->x < x1) {
        x1 = pt->x;
      }

      if (pt->x > x2) {
        x2 = pt->x;
      }

      if (pt->y < y1) {
        y1 = pt->y;
      }

      if (pt->y > y2) {
        y2 = pt->y;
      }
    }
  }

  return iRECT(x1, y1, x2, y2);
}


#define MODE_TOOLTIP 5

typedef struct CCubes { // Figure composed of 27 cubes arranged in 3*3*3 (Rubik's Cube)
  t_Mask  m_cube[27];  // position et orientation des cubes constituants

  float   m_dim;       // dimension d'une face en 3D

  ISIZE   m_screen;    // dimensions fenetre affichage
  IRECT   m_box;       // rectangle englobant la figure sur l'ecran

  // au maximum 27*3 faces visibles + 1 face de travail pour Sides::Create()
  BYTE    m_nbSide;    // nombre de faces visibles
  CSide   m_side[81];  // ensemble de faces visibles (activees par Sides::Create())

  CSide   m_si2D[54];  // faces calculee en cube developpe a plat

  t_Matrix m_oldMove;

  // variables fournies par l'application :
  int*    m_pYObs;     // distance observateur
  float* m_pMro;     // pt matrice passage repere RubixCube vers Observateur
  float* m_pMmr;     // pt matrice passage repere partie Mobile vers RubixCube
} CCubes;

#if 0
void    SZSort(int low, int high);// tri des faces visibles en z
void    SPSort(int low, int high);// tri des faces en position cube

//CCubes(int asdf) {    int fdfd=0;  }
void Set(const CCubes* pCubes, float dim = 1.f, int* yObs = NULL, float* pObs = NULL, float* pMob = NULL);
void Set(float dim = 1.f, int* yObs = NULL, float* pObs = NULL, float* pMob = NULL);

void  SetScreen(int dx, int dy);

IRECT Comput(t_Mask mask = 0);
void  Fixe(t_Mask mask = 0, bool sort = true);

IRECT Comput2D(int mod, IPOINT ofs, const t_Side dscr[6]);
IRECT Comput2D(int mod = 15, IPOINT ofs = iPOINT(5, 5));
void  Draw(img_t* im, const IRECT* pclip);
void  Draw2D(img_t* im, const IRECT* pclip);
void  DrawSide(img_t* im, const IRECT* pclip, BYTE noSide, int size, const IPOINT& org = iPOINT(0, 0));

BYTE  GetSide(const IPOINT& pos) const;
BYTE  GetCubeNo(const IPOINT& pos) const;
BYTE  GetSide2D(const IPOINT& pos) const;
t_Command GetMove(int x, int y, BYTE cubeNo);
DWORD GetHash() const;
BYTE  GetState() const;
t_GROUP SidesColors(bool def) const;

int Create(BYTE facets[54], bool er = false);

BOOL Read(const char* fileName, char* pComments = NULL, int len = 0);
BOOL Write(const char* fileName, char* comments = "", int len = 0);
int TypeSelect(const char* msg) const;

void CmdApply(const char* cmd);
void CmdAddRevers(t_Command cmd, char* cmdLine, int cmdLineLen, DWORD curs = DWORD(-1));
t_Command CmdExtract(char* cmdLine, DWORD* pCurs = NULL);

int ColorsCheck() const;

t_CUBES CCubes_ScrambledCubies(c) const;
DWORD CCubes_CenterSidesRotation(c) const;
int SetCenterSidesRotation(DWORD centers);
int GetCentersMsg(char* out, int len, const char* order = "ULFRBD") const;

int Dump(char* out, int len, int mode) const;
void CubeSort(int low, int hig);
int FileTypeSelect(const char* msg) const;

#endif

inline t_Mask CCubes_CubeOrigine(BYTE noCube, t_Matrix or = MatrixUnit)
{
  return cubePos(noCube) | or;
}

inline void CCubes_Init(CCubes* c, t_Matrix or = MatrixUnit)
{
  BYTE n;

  for (n = 0; n < 81; ++n) {
    CSide_Init(c->m_side + n);
  }

  for (n = 0; n < 54; ++n) {
    CSide_Init(c->m_si2D + n);
  }

  for (n = 0; n < 27; n++) {
    c->m_cube[n] = CCubes_CubeOrigine(n, or);
  }
}

inline t_Matrix CCubes_Orientation(const CCubes* c)
{
  return t_Matrix(c->m_cube[13]);
}

// Compare the positions of the sides in held flat
int CSide_Compare(const CSide* s, const CSide* pSide)
{
  return (s->m_box.t == pSide->m_box.t) ? s->m_box.l - pSide->m_box.l : s->m_box.t - pSide->m_box.t;
}

// Classification, depending on their position,
// faces calculated by Comput2D
void CCubes_SPSort(CCubes* c, int low, int hig)

{
  ASSERT(low >= 0 && hig < 54 && low < hig);
  // Extrait le z de l'element median
  CSide* pMs = c->m_si2D[(low + hig) / 2].m_pSide;
  int lo = low, hi = hig;

  if (NULL == pMs) {
    return ;
  }

  // loop until the intersection index
  while (lo <= hi) {
    // look at the top and at the bottom elements not in place
    while (lo < hig && CSide_Compare(pMs, c->m_si2D[lo].m_pSide) > 0) {
      ++lo;
    }

    while (hi > low && CSide_Compare(pMs, c->m_si2D[hi].m_pSide) < 0) {
      --hi;
    }

    // echange les elements rencontres
    if (lo <= hi) {
      CSide_Swap(c->m_si2D + lo++, c->m_si2D + hi--);
    }
  }

  // recursive achieve the sort of remains
  if (low < hi) {
    CCubes_SPSort(c, low, hi);
  }

  if (lo  < hig) {
    CCubes_SPSort(c, lo, hig);
  }
}
void  CCubes_SPSort(CCubes* c)
{
  CCubes_SPSort(c, 0, 53);
}

inline IRECT CCubes_GetSide2DRect(const CCubes* c)
{
  return CSide_GetRect(c->m_si2D, 54);
}
inline IRECT CCubes_GetSideRect(const CCubes* c)
{
  return CSide_GetRect(c->m_side, c->m_nbSide);
}

inline void CCubes_SPUnSort(CCubes* c)
{
  for (int i = 0; i < 54; i++) {
    CSide_Restore(c->m_si2D + i);
  }
}

inline CSide* CCubes_GetSide(const CCubes* c, const BYTE no)
{
  return (no < c->m_nbSide) ? c->m_side[no].m_pSide : NULL;
}
inline CSide* CCubes_GetSide2D(const CCubes* c, const BYTE no)
{
  return (no < 54) ? c->m_si2D[no].m_pSide : NULL;
}
inline int CCubes_Si2DSortIndex(const CCubes* c, int i)
{
  return c->m_si2D[i].m_pSide - c->m_si2D;
}

void CCubes_SetCubes(CCubes* c, const CCubes* cubes)
{
  memcpy(c->m_cube, cubes->m_cube, sizeof(c->m_cube));
}
inline t_Mask CCubes_GetCube(const CCubes* c, int index)
{
  return (index >= 0 && index < 27) ? c->m_cube[index] : 0;
}

void CCubes_Set(CCubes* c,
    const CCubes* pCubes,   // copy position of this Cube
    float dim,     // size of a Rubik's Cube face
    int* pYObs,    // distance viever
    float* pMro, // transition matrix repere Cube Viewer
    float* pMmr) // transition matrix repere part Mobile Cube

{
  c->m_dim  = dim;
  c->m_pYObs = pYObs;
  c->m_pMro = pMro;
  c->m_pMmr = pMmr;
  c->m_nbSide = 0;
  c->m_oldMove = pMmr ? M33_Pack(pMmr) : 0;

  CCubes_Init(c);

  if (pCubes) {
    memcpy(c->m_cube, pCubes->m_cube, 54);
  }
}

void CCubes_Set(CCubes* c,
    float dim,     // size of a Rubik's Cube face
    int* pYObs,    // distance viever
    float* pMro, // transition matrix repere Cube Viewer
    float* pMmr) // transition matrix repere part Mobile Cube
{
  c->m_dim  = dim;
  c->m_pYObs = pYObs;
  c->m_pMro = pMro;
  c->m_pMmr = pMmr;
  c->m_nbSide = 0;
  c->m_oldMove = pMmr ? M33_Pack(pMmr) : 0;
  CCubes_Init(c);
}

// Tests if the position is inside the facet
BOOL CSide_PtInSide(const CSide* s, IPOINT pos)
{
  if (iPtInRect(&s->m_box, pos.x, pos.y)) {
    // the point is inside the box
    IPOINT pV1, pV2;
    int s0;
    BYTE i;
    PTSUB(pV1, s->m_poly[3], pos), PTSUB(pV2, s->m_poly[0], pos);
    s0 = pV1.x * pV2.y - pV1.y * pV2.x;

    // compare the sign of the surfaces of triangles formed by point and
    // the sides of the face
    for (i = 1; i < 4; i++) {
      pV1 = pV2, PTSUB(pV2, s->m_poly[i], pos);
      int sn = pV1.x * pV2.y - pV1.y * pV2.x;

      if ((sn ^ s0) < 0) {
        return FALSE;
      }
    }

    return TRUE;
  }

  return FALSE;
}



void CCubes_SetScreen(CCubes* c, int cx, int cy)
{
  c->m_screen.w = cx, c->m_screen.h = cy;
}
// Determining the calculated face by 'Comput2D', designed by the position
BYTE CCubes_GetSide2D(const CCubes* c, IPOINT pos)
{
  for (BYTE i = 0; i < 54; i++) {
    if (CSide_PtInSide(c->m_si2D[i].m_pSide, pos)) {
      return i;
    }
  }

  return 0xFF;
}

// Calculate the deformation applied to the Rubik's Cube
t_Command CCubes_GetMove(CCubes* c, int x, int y,   // Moving the mouse
    BYTE cubeNo)    // selected cubie

{
  // calculate the change order of the cube (according to a mouse movement)
  float dx, dy, dz;
  BYTE axe;

  // be put in the coordinate of the object
  dx = c->m_pMro[0 * 3 + 0] * y + c->m_pMro[0 * 3 + 2] * x;
  dy = c->m_pMro[1 * 3 + 0] * y + c->m_pMro[1 * 3 + 2] * x;
  dz = c->m_pMro[2 * 3 + 0] * y + c->m_pMro[2 * 3 + 2] * x;

  // determine the axis of motion to execute
  short nx = (short)(dx), ny = (short)(dy), nz = (short)(dz), n;

  if (dx < 0.0f) {
    dx = -dx;
  }

  if (dy < 0.0f) {
    dy = -dy;
  }

  if (dz < 0.0f) {
    dz = -dz;
  }

  n = nx, axe = 0; // axe =>x

  if (dx < dy) {
    dx = dy, n = ny, axe = 1;  // axe =>y
  }

  if (dx < dz) {
    dx = dz, n = nz, axe = 2;  // axe =>z
  }

  t_Command cmd = toCommand(axe, cubeNo);

  if (cmd == 0) {
    return cmd;
  }

  t_Command o = (cmd & 0x1F) / 6;

  if (n < 0) {
    o = 4 - o;
  }

  return (cmd & 0x60) + (cmd & 0x1F) % 6 + o * 6;
}

// Rank facets according to their distance z
// to draw the most distant and the first
// and deal the problems of masking
void CCubes_SZSort(CCubes* c, int low, int hig)
{
  ASSERT(low >= 0 && hig < c->m_nbSide && low < hig);
  // extract the z of the middle element
  int mz = c->m_side[(low + hig) / 2].m_pSide->m_z;
  int lo = low, hi = hig;

  // loop until the intersection index
  while (lo <= hi) {
    // look at the top and at the bottom elements not in place
    while (lo < hig && mz > c->m_side[lo].m_pSide->m_z) {
      ++lo;
    }

    while (hi > low && mz < c->m_side[hi].m_pSide->m_z) {
      --hi;
    }

    // exchange elements encountered
    if (lo <= hi) {
      CSide_Swap(c->m_side + lo++, c->m_side + hi--);
    }
  }

  // recursive achieve the sort of remains
  if (low < hi) {
    CCubes_SZSort(c, low, hi);
  }

  if (lo  < hig) {
    CCubes_SZSort(c, lo, hig);
  }
}

inline static void CubeSwap(int i, int j, t_Mask cube[])
{
  t_Mask t = cube[i];

  if (i != j) {
    cube[i] = cube[j], cube[j] = t;
  }
}


// Cubies sorting
void CCubes_CubeSort(CCubes* c, int low, int hig)

{
  t_Mask m = c->m_cube[(low + hig) / 2];
  int lo = low, hi = hig;

  // loop until the intersection index
  while (lo <= hi) {
    // look at the top and at the bottom elements not in place
    while (lo < hig && m > c->m_cube[lo]) {
      ++lo;
    }

    while (hi > low && m < c->m_cube[hi]) {
      --hi;
    }

    // echange les elements rencontres
    if (lo <= hi) {
      CubeSwap(lo++, hi--, c->m_cube);
    }
  }

  // recursive achieve the sort of remains
  if (low < hi) {
    CCubes_CubeSort(c, low, hi);
  }

  if (lo  < hig) {
    CCubes_CubeSort(c, lo, hig);
  }
}

bool CCubes_Cmp(const CCubes* c, const CCubes* cube)
{
  CCubes c1, c2;
  CCubes_Set(&c1, c, 1.f, NULL, NULL, NULL);
  CCubes_Set(&c2, cube, 1.f, NULL, NULL, NULL);
  CCubes_CubeSort(&c1, 0, 26), CCubes_CubeSort(&c2, 0, 26);
  return memcmp(c1.m_cube, c2.m_cube, 32) == 0;
}

extern void InitColorsRotateTable();

extern BYTE GetCubeSideColor(t_Matrix or , BYTE face);
extern t_Matrix GetOrientationSide(BYTE noSide);

// Gives the orientation of the cube on the basis of colors of two faces
extern t_Matrix GetCubeOrientation(BYTE Upper, BYTE Front);
extern t_Matrix GetCubeOrientation(t_GROUP sides, bool rev);
extern t_Matrix GetCubeOrientation(t_GROUP colors, t_GROUP noSide, t_Matrix def = MatrixUnit);
extern t_Matrix GetCubeOrientation(const BYTE facets[54]);

extern t_Matrix NewOrient(const char* frontUp, const char* colors = "ORGBWY");


// gives the number of vertices of the face
// 4 ------- 0
// | \    up  \
// |   \     |  \
// |    6 ------- 2
// left   |    |    |right
// 5  - | -  1    |
// \  |     \   |
// \| down  \ |
// 7 ------- 3

static const short cubeFaces[6][4] = {
  // 0 Right    1 Left
  {0, 1, 3, 2}, {6, 7, 5, 4}, // Show side
  // 2 Back     3 Front          vertex
  {4, 5, 1, 0}, {2, 3, 7, 6}, // rotate CW,
  // 4 Up       5 Down           hide CCW
  {0, 2, 6, 4}, {1, 5, 7, 3}
};


bool CubeVertexList_IsZPos(const t_VERTEX* m_vertex, BYTE noSide)

{
  return (m_vertex[cubeFaces[noSide][0]].z > 0
      && m_vertex[cubeFaces[noSide][1]].z > 0
      && m_vertex[cubeFaces[noSide][2]].z > 0
      && m_vertex[cubeFaces[noSide][3]].z > 0);
}

void CubeVertexList_Side(const t_VERTEX* m_vertex, BYTE noSide,      // no of the face 0..5
    CSide* pSide,     // face to update
    IPOINT ofs // offset to add at x and y
                        )
{
  int v, z;

  // generate the polygon defining the face
  for (v = 0, z = 0; v < 4; v++) {
    // isolates the 4 vertices of the face
    const t_VERTEX* pVx = m_vertex + cubeFaces[noSide][v];
    PTADD(pSide->m_poly[v], ofs, pVx->v);
    z += pSide->m_zt[v] = pVx->z;
  }

  pSide->m_z = z / 4;     // average distance from the center of the face
}

// Calculate an elementary face in flat developed representation

// Initializes a face of the cube, determines if it is visible
BOOL CSide_Create(CSide* s, t_VERTEX* vrtx, // position of the vertices
    BYTE noSide,           // facet number, color
    t_Mask cube,           // orientation & cube position
    BYTE noCube,           // cune no (initial position)
    IPOINT org)            // origin of display picture

{
  // Creation of a face, return TRUE if the face is turned towards the observer
  int x1, x2, y1, y2, x, y, v;

  // make the polygon defining the facet
  CubeVertexList_Side(vrtx, noSide, s, org);

  // tests whether the facet is turned towards the observer
  int dx1 = s->m_poly[0].x - s->m_poly[1].x, dy1 = s->m_poly[0].y - s->m_poly[1].y;
  int dx2 = s->m_poly[0].x - s->m_poly[2].x, dy2 = s->m_poly[0].y - s->m_poly[2].y;

  if (dx1 * dy2 > dx2 * dy1) {           // it is visible

    // calculate the bounding rectangle of the facet (box)
    x1 = x2 = s->m_poly[0].x, y1 = y2 = s->m_poly[0].y;

    for (v = 1; v < 4; v++) {
      if ((x = s->m_poly[v].x) < x1) {
        x1 = x;
      }

      if (x > x2) {
        x2 = x;
      }

      if ((y = s->m_poly[v].y) < y1) {
        y1 = y;
      }

      if (y > y2) {
        y2 = y;
      }
    }

    s->m_box = iRECT(x1, y1, x2 + 1, y2 + 1); // box

    // facet attributs
    s->m_cube   = cube;        // position and orientation of the cube
    s->m_noSide = noSide;      // face number => facet color
    s->m_noDscr = 0xFF;        // not used in 3D
    s->m_pSide  = s;        // initialize the classification of the facets
    return TRUE;                     // visible facet
  }

  return FALSE;                       // not visible facet
}


BOOL CSide_Create2D(CSide* s, float* meo,  // viever direction matrix
    t_Mask cube,   // elementary cube
    BYTE noCube,   // cube no (info for debug)
    BYTE noDscr,   // entry no of descripteur, color
    IPOINT org,    // offset position to draw
    float dim)     // size of an facet

{
  t_VERTEX vrtx[8] = {0};
  CubeVertexList_Set(vrtx, cube, meo, dim);  // 8 sommets du cube elementaire

  // must choose the right face whose all vertices z is positive (front),
  // which allow people to know the color

  for (BYTE noSide = 0; noSide < 6; noSide++) {
    // eliminates the faces rear and side
    if (!CubeVertexList_IsZPos(vrtx, noSide)) {
      continue;
    }

    // the 4 vertex of this facet
    CubeVertexList_Side(vrtx, noSide, s, org);

    s->m_box = iRECT_pp(s->m_poly[0], s->m_poly[2]);
    iRectNormalize(&s->m_box);  // box
    s->m_z     = 0;
    s->m_cube  = cube;
    s->m_noSide = noSide;      // facet color 0..5
    s->m_noDscr = noDscr;
    s->m_pSide = s;
    return TRUE;
  }

  return FALSE;
}

// Draw the facet
void CSide_Draw(const CSide* s, img_t* im, const IRECT* pclip)
{
  int len = 4;
  COLOR clr = GetBrush(s->m_noSide);
  imdrawaa_poly_solid_i(im, pclip, s->m_poly, &len, 1, clr, 0);
}
char* CSide_Mode(short mode)
{
  // retourne le mode d'affichage de la face
  static char* modeMsg[MODE_TOOLTIP] = {
    "Affiche n°position (1..27)",  // 0
    "Affiche [planx,plany,planz]", // 1
    "Position CCubes_Orientation cube",   // 2
    "Matrice orientation face",    // 3
    "Position, Distance"
  };         // 4

  return modeMsg[mode % MODE_TOOLTIP];
}



// COMPUTS AND DRAW A RUBIK'S CUBE


// |           |
// ---2---+---+---0---
// | 0 | 0 | 0 |
// +---+---+---+                Arrows point to "top"
// | 0 Right 0 |                    of the cube
// +---+---+---+
// | 0 | 0 | 0 |           |                  |y
// ---3---+---+---1---+---+---0---               |_ _ _ _ _ _
// | 5 | 5 | 5 | 2 | 2 | 2 |                  |\_4_\_4_\_4_\
// +---+---+---+---+---+---+                  |1|\_4_\_4_\_4_\
// | 5 Down| 5 | 2  Back 2 |                  | |1|\_4_\_4_\_4_\
// +---+---+---+---+---+---+                  |1| |1| 3 | 3 | 3 |
// | 5 | 5 | 5 x 2 | 2 | 2 |           |      | |1| |___|___|___|
// ----7---+---+--z5y--+---+---4---+---+---0---   |1| |1| 3 | 3 | 3 |
// |           z 1 | 1 | 1 | 4 | 4 | 4 |       \|1| |___|___|___|--x
// +---+---+---+---+---+---+         \|1| 3 | 3 | 3 |
// | 1 Left  1 | 4 |Up | 4 |           \|___|___|___|
// +---+---+---+---+---+---+             \
// | 1 | 1 | 1 | 4 | 4 | 4 |               \
// ---7---+---+---6---+---+---2---             z
// |           | 3 | 3 | 3 |
// |           +---+---+---+
// | 3 Front 3 |
// +---+---+---+
// | 3 | 3 | 3 |
// ----7---+---+---3---
// |           |


// face orientation (default color is indicative)
static const char m0[9] = {0, -1, 0, 1, 0, 0, 0, 0, 1}; // R, orange
static const char m1[9] = {0, 1, 0, -1, 0, 0, 0, 0, 1}; // L, rouge

static const char m2[9] = { -1, 0, 0, 0, -1, 0, 0, 0, 1}; // B, vert
static const char m3[9] = {1, 0, 0, 0, 1, 0, 0, 0, 1}; // F, bleu (unit)

static const char m4[9] = {1, 0, 0, 0, 0, 1, 0, -1, 0}; // U, blanc
static const char m5[9] = {1, 0, 0, 0, 0, -1, 0, 1, 0}; // D, jaune

// faces names (x=> R-L, y => B-F, z=> U-D
static const t_Side dscr[6] = {
  // plan  |orient,      position x,y
  {0x0600 | MatrixPack(m0), 5, 3}, // 0 Right
  {0x8600 | MatrixPack(m1), 1, 3}, // 1 Left
  {0x1800 | MatrixPack(m2), 7, 3}, // 2 Back
  {0x9800 | MatrixPack(m3), 3, 3}, // 3 Front
  {0x6000 | MatrixPack(m4), 3, 1}, // 4 Up
  {0xE000 | MatrixPack(m5), 3, 5}  // 5 Down
};

t_Matrix GetOrientationSide(BYTE noSide)
{
  return t_Matrix(dscr[noSide].plan);
}

// table of faces for a view held flat R, L, B, F, U, D to generate a belt
// to extract the ascii position
static const t_Side dscrText[6] = {    // band R L B F U D
  {0x0600 | MatrixPack(m0), 1, 5}, {0x8600 | MatrixPack(m1), 1, 1},
  {0x1800 | MatrixPack(m2), 1, 7}, {0x9800 | MatrixPack(m3), 1, 3},
  {0x6000 | MatrixPack(m4), 1, 9}, {0xE000 | MatrixPack(m5), 1, 11}
};
static const t_Side dscrBand[6] = {    // band R L B F U D
  {0x0600 | MatrixPack(m0), 1, 1}, {0x8600 | MatrixPack(m1), 1, 3},
  {0x1800 | MatrixPack(m2), 1, 5}, {0x9800 | MatrixPack(m3), 1, 7},
  {0x6000 | MatrixPack(m4), 1, 9}, {0xE000 | MatrixPack(m5), 1, 11}
};
static BYTE moveCubes[21] =
{0, 1, 2, 3, 5, 6, 7, 8, 9, 11, 13, 15, 17, 18, 19, 20, 21, 23, 24, 25, 26};

// Calculates the faces of the Rubik's Cube in a continuous movement
// the whole cube or a deformation
// deformation order of the cube
// defines the relevant tranche
IRECT CCubes_Comput(CCubes* c, t_Mask mask)
{
  // calculate the position of the cubes during a continuous movement
  t_Mask cmp = (mask & 0x0100) ? 0 : (mask & 0x3F) << 9, msk = mask & 0x7E00;
  float size = (float)(c->m_screen.w), yObs = (float)(*c->m_pYObs);
  IPOINT org = iPOINT(c->m_screen.w / 2, c->m_screen.h / 2);
  float Mmo[9] = {0};
  M33_M33(Mmo, c->m_pMro, c->m_pMmr);  // orientation matrix of the moving cubes
  IRECT boxSave = (c->m_box);

  // loop on the calculation of each cubie
  c->m_nbSide = 0, iRectSetEmpty(&c->m_box);

  for (BYTE noCube = 0; noCube < 27; noCube++) {
    t_Mask cube = c->m_cube[noCube];
    // plan selection cubies (fixed or mobile)
    float* pMeo = (mask && (cube & msk) == cmp) ? Mmo : c->m_pMro;

    // calculation of the 8 vertices of the cubie in 2D perspective
    t_VERTEX vrtxList[8] = {0};
    CubeVertexList_Set(vrtxList, cube, pMeo, c->m_dim, size, yObs);

    // calculation of the 6 faces of the cubie and memorizing visible
    for (BYTE noSide = 0; noSide < 6; noSide++)
      if (c->m_nbSide < 81 // au maximum, 3 faces par cube sont visibles
          && CSide_Create(c->m_side + c->m_nbSide, vrtxList, noSide, cube, noCube, org)) {
        // the face is visible ie facing the viewer
        c->m_box = iRectUnion(c->m_box, c->m_side[c->m_nbSide++].m_box);
      }
  }

  // classification of visible faces in z (Z buffer) for masking relative
  CCubes_SZSort(c, 0, c->m_nbSide - 1);

  return iRectUnion(boxSave, c->m_box);                // rectangle de modification dessin
}



// Calculates the state of Rubik's Cube after a deformation
// and resets the deformation matrix

void CCubes_Fixe(CCubes* c, t_Mask mask,            // deformation order
    bool sort)

{
  // fixes the position of the cubes was about 90 degrees
  t_Mask cmp = (mask & 0x0100) ? 0 : (mask & 0x3F) << 9, msk = mask & 0x7E00;
  t_Matrix mx = M33_Pack(c->m_pMmr);             // deformation matrix
  float move[9] = {0};
  M33_M33(move, mx, 1.f);

  for (BYTE noCube = 0; noCube < 27; noCube++) {
    t_Mask cube = c->m_cube[noCube];       // reading the old state

    if ((cube & msk) == cmp) {          // checks if update
      float Mco[9] = {0};
      M33_M33(Mco, t_Matrix(cube), move, 1.f); // cubes orientation matrix
      float pos[3] = {0};
      f3SetMask(pos, cube, move, 1.f);        // vector of cubes positions
      c->m_cube[noCube] = M33_Pack(Mco) | f3Pack(pos); // save the new state
    }
  }

  if (sort) {
    CCubes_CubeSort(c, 0, 26);  // cubies sorting
  }

  M33_Set(c->m_pMmr, 1.f);                   // reinitit the matrix
  c->m_oldMove = mx;                        // save matrix value for the debug
}



// Calculate the faces of the developed representation of the Rubik's Cube

IRECT CCubes_Comput2D(CCubes* c, int mod,           // cube sizing module
    IPOINT ofs,        // offset for the display
    const t_Side dscr[6]) // display sides table

{
  IRECT rect = iRECT(ofs.x, ofs.y, ofs.x, ofs.y);
  BYTE noSide, noCube, outSide;
  float fmod = float(mod);

  // extract the faces corresponding to each page descriptor, they
  // correspond to the faces of the Rubik's Cube
  for (noSide = 0, outSide = 0; noSide < 6; noSide++) {
    const t_Side& side = dscr[noSide];

    if (side.plan == 0) {
      continue;  // empty page
    }

    IPOINT org = iPOINT((side.x * 3 * mod) / 2 + ofs.x, (side.y * 3 * mod) / 2 + ofs.y);
    float mos[9] = {0};
    M33_M33(mos, t_Matrix(side.plan), 1.f);   // orientation face matrix

    for (noCube = 0; noCube < 27; noCube++) {
      t_Mask cube = c->m_cube[noCube];

      // isolate the cubies belonging to the face defined by the descriptor
      if (CubeChk(cube, side.plan)) {
        // the cubic corresondant the descriptor is found,
        // (9 cubies must correspond to this definition)
        ASSERT(outSide < 54);         // 6 plans x 9 cubies

        // calculate the corresponding surface
        if (CSide_Create2D(c->m_si2D + outSide, mos, cube, noCube, noSide, org, fmod)) {
          rect = iRectUnion(rect, c->m_si2D[outSide++].m_box);
        }
      }
    }
  }

  return rect;
}



// Calculate the faces of the developed representation of the Rubik's Cube
//= 15, IPOINT ofs = iPOINT(5, 5)
IRECT CCubes_Comput2D(CCubes* c, int mod,           // cube sizing module
    IPOINT ofs)        // position of display

{
  return CCubes_Comput2D(c, mod, ofs, dscr);
}


IRECT CCubes_Comput2D(CCubes* c)
{
  return CCubes_Comput2D(c, 15, iPOINT(5, 5));
}

// Drawing of the Rubik's Cube (calculate by Comput)

void CCubes_Draw(const CCubes* c, img_t* im, const IRECT* pclip)
{
  for (BYTE i = 0; i < c->m_nbSide; i++) {
    CSide_Draw(c->m_side[i].m_pSide, im, pclip);
  }
}



// Drawing the Rubik's Cube faces (calculate by Comput2D)

void CCubes_Draw2D(const CCubes* c, img_t* im, const IRECT* pclip)
{
  for (short i = 0; i < 54 && c->m_si2D[i].m_pSide; i++) {
    CSide_Draw(c->m_si2D + i, im, pclip);
  }
}

// Calculate and draw a face of the Rubik's Cube
void CCubes_DrawSide(const CCubes* c, img_t* im, const IRECT* pclip,           // CDC to ouput
    BYTE noSide,        // no de la face
    int size,           // taille de la face
    const IPOINT& org)  // offset image
{
  CCubes cubes = *c;
  t_Side dscrIn[6];

  // Calculate the facets of the face defined
  memset(dscrIn, 0, sizeof(dscrIn));
  dscrIn[0].plan = dscr[noSide].plan;
  dscrIn[0].x = dscrIn[0].y = 1;
  CCubes_Comput2D(&cubes, size / 3, org, dscrIn);

  // draw the 9 facets composing the face
  for (UINT i = 0; i < 9 && cubes.m_si2D[i].m_pSide; i++) {
    CSide_Draw(cubes.m_si2D + i, im, pclip);
  }
}


// Determining the calculated face by 'Comput', designed by the position

BYTE CCubes_GetSide(const CCubes* c, IPOINT pos)
{
  int i;

  for (i = c->m_nbSide; --i >= 0;) {
    if (CSide_PtInSide(c->m_side[i].m_pSide, pos)) {
      break;
    }
  }

  return BYTE(i);
}



// Determining the cubie designed by the position
BYTE CCubes_GetCubeNo(const CCubes* c, IPOINT pos)
{
  for (int i = c->m_nbSide; --i >= 0;) {
    if (CSide_PtInSide(c->m_side[i].m_pSide, pos)) {
      return CSide_GetCubeNo(c->m_side + i);
    }
  }

  return 0xFF;
}



// Conversion of the state of Rubik's Cube in an ASCII string
int CCubes_GetText(const CCubes* c, char* text, int len,
    t_Matrix or ,          // New Direction (optional)
    const char* colors  // 6 color letters + 6 face letters
                  )

{
  float mob[9] = {0};
  int i, j, k;
  M33_Set(mob, 1.f);
  CCubes cubes = {0};
  CCubes_Set(&cubes, c, 1.f, NULL, NULL, mob);

  ASSERT(strlen(colors) == 12);

  if ( or > 1) {
    // calculate the new direction
    M33_M33(mob, or , 1.f);
    CCubes_Fixe(&cubes, 0, true);                    // set the new direction
  }

  // Construction and ordering of the held flat
  IPOINT pt = iPOINT(0, 0);
  CCubes_Comput2D(&cubes, 50, pt, dscrText);
  CCubes_SPSort(&cubes);

  // for each face of the Rubik's Cube
  for (i = 0, k = 0; i < 6; i++) {
    if (text[0] != 0) {
      STRcat_c(text, len, ' ');
    }

    STRcat_c(text, len, colors[cubes.m_si2D[k].m_pSide->m_noDscr + 6]);
    STRcat_c(text, len, ':');

    // color output of the 9 facets
    for (j = 0; j < 9; j++, k++) {
      STRcat_c(text, len, colors[cubes.m_si2D[k].m_pSide->m_noSide]);
    }
  }

  return 0;
}

// 1, "ORGBWYRLBFUD"
//int CCubes_GetText(char* out, int len, t_Matrix or, const char* colorsSides) const;
int CCubes_GetText(const CCubes* c, char* out, int len)
{
  return CCubes_GetText(c, out, len, 1, "ORGBWYRLBFUD");
}


// Calculate the 3 colors of the centers of the faces

t_GROUP CCubes_SidesColors(const CCubes* c, bool ref)  // reference or this Cube
{
  if (ref) {
    return RefColors(CENTER_FACES);
  }
  else {
    BYTE facets[54];
    char tmp[256] = {0};
    CCubes_GetText(c, tmp, 256);
    SetSideText(tmp, facets, "ORGBWYUDLRFB");

    return GetSideColors(CENTER_FACES, facets);
  }
}



// Initialize a Rubik's Cube from a table of 54 colors
int CCubes_Create(CCubes* c, BYTE facets[54], // table of 54 facets colors
    bool er)         // true : erase the facets in error
{
  CCubes cubes;
  BYTE noCube, i;
  bool e = false;

  CCubes_Set(&cubes, 1.f, NULL, NULL, NULL);
  // calculate the orientation of the Rubik's Cube
  t_Matrix or = GetCubeOrientation(facets);

  if ( or == 0) {
    return -1;  // central facets in error, incorrectly positioned
  }

  // put to zero all the cubies to test the overwrite
  for (i = 0; i < 27; i++) {
    cubes.m_cube[i] = 0;
  }

  for (i = 0; i < 27; i++) {
    // facets colors of the Cube
    DWORD nSides = GetNoSides(i);
    DWORD colors = GetSideColors(nSides, facets);
    // Cube orientation
    t_Matrix cOr = GetCubeOrientation(RefColors(nSides), colors, or);
    // t_Matrix cOr = CMatrix::ComputOrient(RefColors(nSides), colors, or);

    if (cOr) {
      t_COLORS cvrt = cvrtColors(colors);

      // find the number of the cube with the color signature
      noCube = GetNoCube(cvrt);

      // number cubie corresponding to the signature color must to be found
      // and only one !
      if (noCube >= 27) {
        return -3;
      }

      if (cubes.m_cube[noCube] != 0) {
        return -4;
      }

      // a cubie retains its signature color whatever its rotations
      cubes.m_cube[noCube] = cOr | cubePos(i); // coding of the cubie

    }
    else {
      // error, an elementary cubie can to be built with the available
      // colors for its facets.
      e = true;

      if (!er) {
        break;
      }

      // if er == true, erase the facets in error
      SetSideColors(nSides, GROUP_EMPTY, facets);
    }
  }

  if (e) {
    return -2;  // Error, the cubies could not to be constructed
  }

  // ok, Rubik's Cube update
  CCubes_SetCubes(c, &cubes);
  return 0;
}
// Content analysis to determine a file format
int CCubes_FileTypeSelect(const char* msg)
{
  // test format RubixCube
  if (str_ncmp(msg, "RUBIK", 5) && strlen(msg) > 32) {
    return 1;
  }

  // test format Color
  if (str_ncmp(msg, "COLORS", 6) && strlen(msg) > 32) {
    return 2;
  }

  const char* test = "BDFGLORUWY(/\r\n";
  DWORD counts = 0;
  int i, ix;

  // calculate the signature of the content
  for (i = 0; i < strlen(msg); i++)
    if ((ix = str_findchr(test, msg[i])) > 9) {
      break;  // end of data
    }
    else if (ix >= 0) {
      counts += 1 << (ix * 3);
    }

  switch (counts) {
  case 0x514ca252 :
    return 3; // "L:OWGGORWGB F:YROGGOWRR R:BGRYRBBYG ...

  case 0x01208248 :
    return 4; // Singmaster

  default :
    return 0;
  }
}
// Calculate the state of Rubik's Cube
DWORD CCubes_GetHash(const CCubes* c)

{
  CCubes cubes;
  DWORD h = 0, i;
  CCubes_Set(&cubes, c, 1.f, NULL, NULL, NULL);
  CCubes_CubeSort(&cubes, 0, 26);

  for (i = 0; i < 21; i++) {
    h += h * 2 + cubes.m_cube[ moveCubes[i] ];
  }

  return h;
}
// Read a Rubik's Cube from a file
BOOL CCubes_Read(CCubes* c, const char* fileName, char* pComments, int len)
{
  static const char* test = ("/(\n\r");
  char* text = ReadText(fileName);
  int i, ix, type = CCubes_FileTypeSelect(text);
  int ret = FALSE;

  if (pComments) {
    for (i = 0; i < strlen(text) - 1; i++) {
      if ((ix = str_findchr(test, text[i])) >= 0) {
        if (ix < 2) {
          STRmid_cpy(text, -1, i, -1, pComments, len);
        }
        else if (text[i + 1] == '\r' || text[i + 1] == '\n') {
          STRmid_cpy(text, -1, i + 2, -1, pComments, len);
        }
        else {
          STRmid_cpy(text, -1, i + 1, -1, pComments, len);
        }

        break;
      }
    }
  }

  if (type == 1) {
    // internal format RubixCube
    CCubes cube;
    CCubes_Set(&cube, 1.f, NULL, NULL, NULL);

    for (i = 0; i < 27; i++) {
      cube.m_cube[i] = cubePos(i) | text[i + 6] - char('A') + 1;
    }

    if (char(CCubes_GetHash(&cube) % 26 + 'A') == text[5]) {
      CCubes_SetCubes(c, &cube);
      ret = TRUE;
    }
  }
  else if (type == 2) {
    // read colors

    ReadColors(text);
    ret = TRUE;
  }
  else if (type == 3) {
    // read format "L:OWGGORWGB F:YROGGOWRR R:BGRYRBBYG B:WRGOBWOWR...
    BYTE facets[54];

    if (SetSideText(text, facets, "ORGBWYUDLRFB") == 0 && CCubes_Create(c, facets, false) == 0) {
      ret = TRUE;
    }

  }
  else if (type == 4) {
    // read format Singmaster
    BYTE facets[54];

    if (SingmasterToFacets(text, facets) && CCubes_Create(c, facets, false) == 0) {
      ret = TRUE;
    }
  }

  free(text);

  return FALSE;
}

// Writing a Rubik's Cube in a file
BOOL CCubes_Write(const CCubes* c, const char* fileName, char* comments, int len)
{
  enum {textlen = 4096};
  char text[1024] = {0};
  char tmp[256] = {0};
  FILE* file = NULL;
  file = fopen(fileName, "wb");

  if (NULL == file) {
    return FALSE;
  }

  if (STR_icmp_last_n(fileName, -1, ".txt", 4, 4) == 0) {
    // write formats ascii
    CCubes_GetText(c, text, textlen);
  }
  else if (STR_icmp_last_n(fileName, -1, ".sgm", 4, 4) == 0) {
    // write format Singmaster
    static const char* colorsId = ("ORGBWYUDLRFB");
    BYTE facets[54];
    CCubes_GetText(c, tmp, 256);
    SetSideText(tmp, facets, colorsId);
    ToSingmasterString(text, textlen, facets, colorsId);
  }
  else if (STR_icmp_last_n(fileName, -1, ".col", 4, 4) == 0) {
    // facets colors
    WriteColors(text, len);
  }
  else {
    // write internal format RubixCube
    CCubes cube;
    CCubes_Set(&cube, c, 1.f, NULL, NULL, NULL);

    // header with check sum
    CCubes_CubeSort(&cube, 0, 26);
    _snprintf(text, textlen, "RUBIK%c", CCubes_GetHash(&cube) % 26 + 'A');

    // one letter for each cubie orientation ('A' to 'X' -> 1 to 24)
    for (int i = 0; i < 27; i++) {
      STRcat_c(text, textlen, char(cube.m_cube[i] + 'A' - 1));
    }
  }

  STRcat2(text, textlen, "\r\n", 2, comments, -1);
  fwrite(text, strlen(text), 1, file);
  return TRUE;
}

// Execute the command of deformation
void CCubes_CmdApply(CCubes* c, const char* cmd)   // command string
{
  float mob[9] = {0};
  M33_Set(mob, 1.f);
  CCubes cubes;
  int length = strlen(cmd);
  CCubes_Set(&cubes, c, 1.f, NULL, NULL, mob);

  // TRACE("CCubes_CmdApply %s (%d)\n", cmd.CmdConvert(), length);
  for (int i = 0; i < length; i++) {
    t_Command ci = cmd[i];

    if (ci == 1) {
      continue;
    }
    else if (ci == 4) {
      ASSERT(0);
    }
    else if (ci == 5) {
      // initialization with a series of values
      ASSERT(length - i >= 27);

      for (int j = 0; j < 27; j++) {
        c->m_cube[i] = cubePos(i) | cmd[i + j + 1];
      }

      i += 27;
    }
    else if (ci >= 6) {
      t_Mask move = CmdCubeSet(ci);

      M33_CmdSet(mob, ci);
      CCubes_Fixe(&cubes, move, false);
    }
  }

  CCubes_CubeSort(&cubes, 0, 26); // cubies sorting
  CCubes_SetCubes(c, &cubes);
}

// Storing the history of commands
void CCubes_CmdAddRevers(CCubes* c, t_Command cmd,  // new commande
    char* cmdLine, int cmdLineLen,// history
    DWORD curs)         // source position of command
{
  if (cmd == 1) {
    // skip a line;
    STRins_c(cmdLine, -1, 0, 0, cmd, cmdLine, cmdLineLen);
  }
  else if (cmd == 4) {
    // reset the Rubik's Cube
    CCubes cubes;
    int i;
    CCubes_Set(&cubes, c, 1.f, NULL, NULL, NULL);
    CCubes_CubeSort(&cubes, 0, 26);

    for (i = 0; i < 27; i++) {
      STRins_c(cmdLine, -1, i, 0, BYTE(cubes.m_cube[i]) + 0x60, cmdLine, cmdLineLen);
    }

    STRins_c(cmdLine, -1, 0, 0, 5, cmdLine, cmdLineLen);
  }
  else if (cmd == 5) {
    ASSERT(0);
  }
  else if (cmd & 0x80) {
    // orientation of the Rubik's Cube
    STRins_c(cmdLine, -1, 0, 0, cmd ^ 0x40, cmdLine, cmdLineLen);
  }
  else if (cmd >= 6) {
    // deformation of the Rubik's Cube
    char flag = cmd & 0xE0, c = cmd & 0x1F;
    STRins_c(cmdLine, -1, 0, 0, char((4 - c / 6) * 6 + c % 6 + flag), cmdLine, cmdLineLen);
  }

  if (cmd && curs != DWORD(-1)) {
    // save the source position of command
    char tmp[32];
    _snprintf(tmp, 32, "\2%08X", curs);
    STRins(cmdLine, -1, 0, 0, tmp, -1, cmdLine, cmdLineLen);
  }
}

// Extracting the commands of history
t_Command CCubes_CmdExtract(CCubes* c, char* cmdLine, // history
    DWORD* pCurs)         // source id of cmd

{
  int len;
  char tmp[32];

  while ((len = strlen(cmdLine)) > 0) {
    t_Command cmd = cmdLine[0];

    STRdel(cmdLine, -1, 0, 1);

    if (cmd == 2) {
      if (len >= 8) {
        if (pCurs) {
          STRmid_cpy(cmdLine, -1, 0, 8, tmp, 32);
          *pCurs = strtoul(tmp, NULL, 16);
        }

        STRdel(cmdLine, -1, 0, 8);
      }
      else {
        cmdLine[0] = 0;
      }
    }
    else if (cmd == 5) {
      if (len >= 27) {
        for (int i = 0; i < 27; i++) {
          c->m_cube[i] = cubePos(i) | (cmdLine[i] - 0x60);
        }

        STRdel(cmdLine, -1, 0, 27);
      }
      else {
        cmdLine[0] = 0;
      }

      return 0;
    }
    else if (cmd >= 6) {
      return cmd;
    }
  }

  return 0;
}



// Test whether the colors of edges and vertices are compatible
// and also that the Rubik's Cube can be ordered

int CCubes_ColorsCheck(const CCubes* c)
{
  BYTE no, np, center[1], centerFace[6], edge[12], corner[8], facets[54];
  BYTE cp[4] = {0, 0, 0, 0}, *pTb[4] = {center, centerFace, edge, corner};
  t_Matrix or = MatrixInvert(CCubes_Orientation(c));
  t_CUBES pres = 0;
  char tmp[256];
  int er;

  // fill the facet table, the cube moved into position origin for
  // calculating the parity of permutations of places of the cubies
  CCubes_GetText(c, tmp, 256, or , "ORGBWYRLBFUD");
  er = SetSideText(tmp, facets, "ORGBWYUDLRFB");

  if (er < 0) {
    return er;
  }

  if (GetCubeOrientation(facets) != 1) {
    return -1;  // error : incorrectly positioned central facets
  }

  // examine the colors of the facets of the 27 cubies
  for (no = 0; no < 27; no++) {
    DWORD nSides = GetNoSides(no);
    DWORD colors = GetSideColors(nSides, facets);
    BYTE nbc = SidesCount(nSides);
    BYTE noCube;      // cube corresponding to the color signature
    char cnt = CheckSidesColors(colors, &noCube);

    if (cnt < 0) {
      return cnt;  // error : color illegal or disordered (-2, -3)
    }

    // count the number of rotations of the color
    cp[nbc] += cnt;

    // mark the presence of cubies
    pres |= cvrtCube(noCube);
    *pTb[nbc]++ = noCube;
  }

  if (pres != ALL_CUBES) {
    return -4;  // error : cubes are duplicated, others lack
  }

  // A negative error indicates that it is materially impossible to build
  // a Rubik's Cube with the colors of facets. It should only occur in a
  // program to create, not in a test.

  // checking program and content of facets tables
  ASSERT(pTb[0] == center + 1 && pTb[1] == centerFace + 6
      && pTb[2] == edge + 12 && pTb[3] == corner + 8);

  // calculate the parity of the permutations of locations of the cubies
  np = PermutationsParity(edge, 12) + PermutationsParity(corner, 8);

  TRACE("CCubes_ColorsCheck edge=%d corner=%d permut=%d\n", cp[2], cp[3], np);

  // Possible errors of consistency of orientation and position of the
  // cubies
  return (cp[2] % 2 ? 1 : 0) + (cp[3] % 3 ? 2 : 0) + (np % 2 ? 4 : 0);
  // A positive error indicates that it is not possible to order the
  // Rubik's Cube
}






// Determine the cubic misaligned

t_CUBES CCubes_ScrambledCubies(const CCubes* c)
{
  t_Matrix or = CCubes_Orientation(c);
  t_CUBES cubeBits = 0;      // bits non-aligned cubes
  CCubes cube;
  CCubes_Set(&cube, c, 1.f, NULL, NULL, NULL);
  CCubes_CubeSort(&cube, 0, 26);

  // search cubies no correctly oriented
  for (int i = 0; i < 27; i++)
    if (t_Matrix(cube.m_cube[i]) != or) {
      cubeBits |= cvrtCube(i);
    }

  return cubeBits;
}



// Calculate the state of Rubik's Cube

BYTE CCubes_GetState(const CCubes* c)
{
  static const t_CUBES cf = GetCubesVSC(1); // bits cubes centers of faces
  t_CUBES cubeBits = CCubes_ScrambledCubies(c);     // set bits non-aligned cubes

  // 0x02 => center faces scrambled, 0x01 => Rubik's Cube scrambled
  return ((cubeBits & cf) ? 2 : 0) | ((cubeBits & ~cf) ? 1 : 0);
}

// no cube with one facet  R  L  B  F  U  D
static const BYTE cf[6] = {14, 12, 16, 10, 22, 4};
//static const char* codeCf = ("RLBFUD");

// Calculate the rotation state of the facets center side
// of the Rubik's Cube

DWORD CCubes_CenterSidesRotation(const CCubes* c)
{
  t_Matrix or = CCubes_Orientation(c), co;
  DWORD centers = 0, n;
  CCubes cube;
  BYTE cubeNo, i;
  CCubes_Set(&cube, c, 1.f, NULL, NULL, NULL);
  CCubes_CubeSort(&cube, 0, 26);

  for (i = 0; i < 6; i++) {
    cubeNo = cf[i];

    // Skip the aligned cubies
    if ((co = t_Matrix(cube.m_cube[cubeNo])) == or) {
      continue;
    }

    float m[9] = {0};
    M33_M33(m, co, 1.f);
    n = 0;

    while (++n < 4 && M33_Pack(M33_Rotate90(m, i / 2, (i & 1) == 0)) != or) ;

    centers |= (n << (i * 4));
  }

  ASSERT((centers & 0xFFCCCCCC) == 0);
  return centers;
}

// Give the message corresponding to the face (debug)
int CSide_GetMsg(const CSide* s, char* msg, int len, short mode,         // mode of display
    const CCubes* pCube)

{
  // retourne les caracteristiques de la face
  DWORD centers = pCube ? CCubes_CenterSidesRotation(pCube) : 0;
  BYTE v = BYTE((s->m_cube >> 9) & 0x003F), n1, n2, n3;
  BYTE k = cubeNo(s->m_cube);
  char faceLeter = pid[k];
  char tmp[256] = {0};

  switch (mode % MODE_TOOLTIP) {
  case 0:
    tmp[0] = 0;
    faceLeter != char(' ') ? GetMsgCenter(tmp, 256, centers, faceLeter) : 0;
    _snprintf(msg, len, "Cube n?d, %s code %04X ", k + 1, tmp, s->m_cube);
    break;

  case 1:
    n1 = k % 3, n2 = (k % 9) / 3, n3 = k / 9;
    _snprintf(msg, len, "Cube [%d,%d,%d], Emplacement [%d,%d,%d], %c code %04X ",
        n1, n2, n3, v & 3, (v >> 2) & 3, (v >> 4) & 3, pid[k], s->m_cube);
    break;

  case 2:
    _snprintf(tmp, 256, "Code cube_%d : ", k + 1);
    M33_DumpMat(msg, len, (t_Matrix)(s->m_cube), tmp);
    break;

  case 3: {
    // CMatrix f(t_Matrix(dscr[m_noSide].plan));
    float f[9];
    float c[9];
    M33_M33(f, GetOrientationSide(s->m_noSide), 1.f);
    M33_M33(c, t_Matrix(s->m_cube), f, 1.f);
    _snprintf(tmp, 256, "CCubes_Orientation face_%d : ", s->m_noSide);
    M33_DumpMat(msg, len, M33_Pack(c), tmp);
  }
  break;

  case 4:
    _snprintf(msg, len, "pos=(%d,%d) sid=%d d=%d", 0, 0, 0, 0);
    break;
  }

  return strlen(msg);
}



// Change the rotation state of the facets center side
// of the Rubik's Cube

int CCubes_SetCenterSidesRotation(CCubes* c, DWORD centers)
{
  t_Matrix or = CCubes_Orientation(c);
  CCubes_CubeSort(c, 0, 26);

  for (int face = 0, n; face < 6; face++) {
    BYTE noCube = cf[face];
    float m[9] = {0};
    M33_M33(m, or , 1.f);

    n = (centers >> (face * 4)) & 3;    // extract rotation of this facet

    while (--n >= 0) {
      M33_Rotate90(m, face / 2, (face & 1) != 0);
    }

    c->m_cube[noCube] = CCubes_CubeOrigine(noCube, M33_Pack(m));
  }

  return IsCentersLegal(centers) ? 0 : 8;
}



// Calculate the rotation state message of the facets center side
// of the Rubik's Cube
int CCubes_GetCentersMsg(const CCubes* c, char* msg, int len, const char* order)// facets rotation
{
  int i, ix;
  DWORD centers = CCubes_CenterSidesRotation(c);
  STRcpy(msg, len, "C:", -1);              // => Centers

  for (i = 0; i < 6; i++) {
    ix = str_findchr(codeCf, order[i]);
    ASSERT(ix >= 0 && ix < 6);
    msg += char('0' + ((centers >> (ix * 4)) & 3));
  }

  return 0;
}



// Debug : Dump parameters
int CCubes_Dump(const CCubes* c, char* msg, int len, int mode)
{
  char tmp[256];
  int i, j, k;

  switch (mode & 0x000F) {
  case 0 :
    STRcpy(msg, len, "CCubes_Dim\n", -1);
    _snprintf(tmp, 256, " dim=%.4f z=%d\n", double(c->m_dim), *c->m_pYObs);
    STRcat(msg, len, tmp, -1);
    break;

  case 1 :
    STRcpy(msg, len, "CCubes_cubes\n", -1);

    for (i = 0; i < 3; i++) {
      for (j = 0; j < 3; j++) {
        k = j * 3 + i * 9;
        _snprintf(tmp, 256, " %04X-%04X-%04X", c->m_cube[k], c->m_cube[k + 1], c->m_cube[k + 2]);
        STRcat(msg, len, tmp, -1);
      }

      STRcat(msg, len, "\n", -1);
    }

    M33_DumpMat(tmp, 256, CCubes_Orientation(c), "Center");
    STRcat2(msg, len, tmp, -1, "\n", -1);
    _snprintf(tmp, 256, "status = %d orient : ", CCubes_GetState(c));
    STRcat(msg, len, tmp, -1);
    GetMsgCenter(tmp, 256, CCubes_CenterSidesRotation(c), -1);
    STRcat(msg, len, tmp, -1);
    break;

  case 2 :
    STRcpy(msg, len, "Matrix Mro (orientation)\n", -1);
    M33_Dump(c->m_pMro, tmp, 256, 3);
    STRcat(msg, len, tmp, -1);
    break;

  case 3 :
    STRcpy(msg, len, "Matrix Mmr (deformation)\n", -1);
    M33_Dump(c->m_pMmr, tmp, 256, 3);
    STRcat(msg, len, tmp, -1);
    M33_DumpMat(tmp, 256, c->m_oldMove, " Mmr old");
    STRcat2(msg, len, tmp, -1, "\n", -1);
    break;

  case 4 :
    STRcpy(msg, len, "CCubes_graph\n", -1);
    _snprintf(tmp, 256, " box(%d,%d) side=%d\n", RCW(&c->m_box), RCH(&c->m_box), c->m_nbSide);
    STRcat(msg, len, tmp, -1);
    break;

  case 5 :
    CCubes_GetText(c, tmp, 256);
    _snprintf(msg, len, "CCubes facets colors\n%s\nHash code = %08X\n", tmp, CCubes_GetHash(c));
    break;

  case 6 :
    msg = "$6"; // pour forcer l'affichage de la representation developpee
    break;

  case 7 :
    for (i = 0; i < 6; i++) {
      t_Matrix m = t_Matrix(dscr[i].plan);
      _snprintf(tmp, 256, " Face_%d %02X :", i, m);
      M33_DumpMat(msg, len, m, tmp);
      STRcat(msg, len, "\n", 1);
    }

    break;

    // pour terminer l'affichage
  default:
    STRcpy(msg, len, "$$", 1);
    // hide msg
  }

  return 0;
}

static t_GROUP sideColorsTable[25] = { DWORD(-1), DWORD(-1), };
static t_Matrix cubeOr[6][6];

void InitColorsRotateTable()
{
  for (int i = 0; i < 6; i++)
    for (int j = 0; j < 6; j++) {
      cubeOr[i][j] = 0;
    }

  for (t_Matrix or = 1; or < 25; or ++) {
    float mob[9] = {0};
    M33_M33(mob, or , 1.f);
    CCubes cubes;
    CCubes_Set(&cubes, 1.f, NULL, NULL, mob);

    // Cube with the new orientation
    CCubes_Fixe(&cubes, 0, true);

    t_GROUP sidesColors = CCubes_SidesColors(&cubes, false);
    sideColorsTable[ or ] = sidesColors;

    BYTE Upper = GetCenterColor(sidesColors, 4);  // U
    BYTE Front = GetCenterColor(sidesColors, 3);  // F

    ASSERT(cubeOr[Upper][Front] == 0);
    // update the 'Upper-Front' table
    cubeOr[Upper][Front] = or;

    TRACE("InitColorsRotateTable up=%d fr=%d or=%d\n", Upper, Front, or);
  }

  // BYTE faces[6];
  // for (BYTE face = 0; face < 6; face++)
  // faces[face] = GetCenterColor(RefColors(CENTER_FACES), face);

  // TRACE("InitColorsRotateTable %d %d %d %d %d %d\n",
  // faces[0], faces[1], faces[2], faces[3], faces[4], faces[5]);
}

BYTE GetCubeSideColor(t_Matrix or , BYTE face)
{
  ASSERT(BYTE( or - 1) < 24);
  return GetCenterColor(sideColorsTable[ or ], face);
}


// By convention, the orientation of a cube is defined by the color of 2
// sides, the front and top face. This program allows rotation of a cube
// placed in the desired direction. The original cube, defined by the unitary
// matrix (1,0,0) (0,1,0) (0,0,1), presented before the face 3 (default blue
// B) and top face 4 (default W White). Position 34 (default BW) is the
// original position.

t_Matrix GetCubeOrientation(BYTE Upper, BYTE Front)
{
  return cubeOr[Upper][Front];
}


// The variable "sides"of type DWORD contains in both endian color both
// sides reference orientation. It will put the cube in the original
// position if desired or "neg" to bring a cube oriented to the original
// position.

t_Matrix GetCubeOrientation(t_GROUP sides, bool neg)


{
  BYTE Front = BYTE_0(sides), Upper = BYTE_1(sides);
  t_Matrix or = cubeOr[Upper][Front];

  return neg ? MatrixInvert( or) : or;
}



// Determine the orientation of a cubie from the colors of two facets
// and reference colors of these two facets

t_Matrix GetCubeOrientation(t_GROUP colors,        // facets colors
    t_GROUP noSide,     // reference colors
    t_Matrix def) // default value

{
  if (sideColorsTable[1] == DWORD(-1)) {
    InitColorsRotateTable();
  }

  if (BYTE(noSide >> 8) < 6) {
    // only two or three facet colors

    // edge cubes or corner cubes
    t_Matrix or = MatrixProduct(GetCubeOrientation(noSide, false),
        GetCubeOrientation(colors, true));

    if ( or == 0) {
      return 0;
    }

    // additional check for the corner cubies (3 facets visible)
    if (BYTE(noSide >> 16) < 6) {

      // A given cubie, at a given place, has only three possible
      // orientations of the 6 combinations of three colors. The visible
      // facets remain the same whatever the position and orientation.
      if ( or != MatrixProduct(GetCubeOrientation(noSide >> 8, false),
          GetCubeOrientation(colors >> 8, true))) {
        return 0;
      }
    }

    return or;
  }

  return def;
}



// Determine the orientation of a Rubik's Cube,
// thanks to the color table (central facets)

t_Matrix GetCubeOrientation(const BYTE facets[54])

{
  // Two central aspects are sufficient to calculate the orientation.
  // We check that the other four facets confirm this orientation.
  t_GROUP nSides0 = GROUP(40, 4, 22), refColor0 = RefColors(nSides0);
  t_GROUP nSides1 = GROUP(49, 13, 31), refColor1 = RefColors(nSides1);
  t_GROUP color0 = GetSideColors(nSides0, facets);
  t_GROUP color1 = GetSideColors(nSides1, facets);
  t_Matrix or = GetCubeOrientation(refColor0, color0, 0);

  if ( or != GetCubeOrientation(refColor1, color1, 0)) {
    return 0;  // Error : central facets incorrectly positioned
  }

  return or;
}



t_Matrix NewOrient(const char* frontUp,// command string, two faces char
    const char* colors) // ref code string
{
  union {
    DWORD d;
    char c[4];
  } sides;
  int i;
  ASSERT(strlen(colors) == 6);

  // decode la couleur des faces directrices
  sides.d = (DWORD)(-1);

  for (i = 0; i < strlen(frontUp); i++) {
    sides.c[i] = (char)str_findchr(colors, frontUp[i]);

    if (sides.c[i] < 0 || sides.c[i] >= 6) {
      return 0;  // erreur dans le codage orientation
    }
  }

  return GetCubeOrientation(sides.d, true);
}

// Reverse a sequence of commands
int CmdReverse(char* cmd)
{
  int i, j = 0;
  char tmp[256];

  for (i = strlen(cmd); --i >= 0;) {
    // reverse the code
    t_Command c = cmd[i];

    if (c & 0x80) {
      c ^= 0x40;
    }
    else if (c >= 6) {
      c = (c & 0x60) + (4 - (c & 0x1F) / 6) * 6 + (c & 0x1F) % 6;
    }
    else if (c == 4 || c == 5) {
      ASSERT(0);
    }

    tmp[j++] = c;
  }

  memcpy(cmd, tmp, j);
  cmd[j] = 0;

  return j;
}



// Modify the command sequence of deformation of the Cube

int CmdTranslate(char* out, int len, const char* cmd,     // command string
    const BYTE oper[6],  // permutation order
    WORD sgn)            // sign change order

{
  int i, j = 0;

  for (i = 0; i < strlen(cmd); i++) {
    t_Command c = cmd[i];
    BYTE t, o, x;

    // the program is limited to coded commands with the side number. A
    // voluntary error is caused if another code comes.
    if (c & 0x80) {
      ASSERT(0);
    }
    else if (c >= 6) {
      x = (c & 0x1F) % 6;
      t = oper[x];
      o = (c & 0x1F) / 6;

      if ((sgn >> x) & 1) {
        o = 4 - o;
      }

      c = (c & 0x60) + o * 6 + t;
    }
    else if (c == 4 || c == 5) {
      ASSERT(0);
    }

    ASSERT(j < len);
    out[j++] = c;
  }

  ASSERT(j < len);
  out[j] = 0;

  return j;
}

// Generate a sample of each command code for testing
int CmdPattern(char* out, int len)
{
  t_Command c;
  int j = 0;

  for (c = 6;  c < 12; c++) {
    ASSERT(j < len);
    out[j++] = c;
  }

  for (c = 12; c < 18; c += 2) {
    ASSERT(j < len);
    out[j++] = c;
  }

  for (c = 18; c < 24; c++) {
    ASSERT(j < len);
    out[j++] = c;
  }

  for (c = 13; c < 18; c += 2) {
    ASSERT(j < len);
    out[j++] = c;
  }

  ASSERT(j < len);
  out[j] = 0;

  return j;
}



// Random generation of a sequence of commands

int CmdRandom(char* out, int len, bool pos,            // with orientation of the Cube
    int length,          // length of generate sequence
    int rnd)             // random input sequence

{
  static bool init = true;
  char tmp[256] = {0};
  int i, j = 0;

  if (rnd >= 0) {
    srand(rnd);
  }
  else if (init) {
    srand((unsigned)time(NULL)), init = false;
  }

  // Generate random orientation of the Rubik's Cube
  if (pos) {
    out[j++] = char(((64 * rand()) / RAND_MAX) | 0x80);
  }

  while (strlen(out) + (i = strlen(tmp)) < length) {
    // generate orders of mixing the Rubik's Cube
    tmp[i++] = char((18 * rand()) / RAND_MAX + 6);
    tmp[i] = 0;

    if (i > 1) {
      CmdOptimize(tmp);

      if (i > 3) {
        out[j++] = tmp[0];
        STRdel(tmp, 256, 0, 1);
        --i;
      }
    }
  }

  STRcat(out, len, tmp, -1);
  return strlen(out);
}

static WORD rotTable[25] = { WORD(-1), WORD(-1),};
// 0x02cf,0x02ba,0x02c8,0x02be,0x008e,0x007f,0x0089,0x007b,
// 0x0256,0x0247,0x0251,0x0243,0x002e,0x001f,0x0029,0x001b,
// 0x0149,0x013b,0x014e,0x013f,0x01ae,0x019f,0x01a9,0x019b };


// Rotate the command sequence
int CmdRotate(char* out, int len, const char* cmd,  // commands input
    t_Matrix or)            // angle to rotate
{
  BYTE oper[6];

  ASSERT( or > 0 && or < 25);
  OrdinalToPermut(rotTable[ or ], oper, 6, 0);
  return CmdTranslate(out, len, cmd, oper, 0);
}

void InitRotateTable()
{
  char pattern[256] = {0};
  char tmp[256] = {0};
  CCubes cref;
  BYTE n = 0;
  CCubes_Set(&cref, 1.f, NULL, NULL, NULL);
  CmdPattern(pattern, countof(pattern));
  CCubes_CmdApply(&cref, pattern);
  // commands sequence to reorganize the Cube
  CmdReverse(pattern);

  // for each orientation
  for (t_Matrix or = 1; or < 25; or ++) {
    float mob[9] = {0};
    M33_M33(mob, or , 1.f);
    CCubes cubes;
    CCubes_Set(&cubes, &cref, 1.f, NULL, NULL, mob);

    // Cube with the new orientation
    CCubes_Fixe(&cubes, 0, true);

    // research the sequence of commands to reorganize the Cube with rotation
    for (WORD ordinal = 0; ordinal < 720; ordinal++) {
      CCubes test;
      BYTE oper[6] = {0};
      CCubes_Set(&test, &cubes, 1.f, NULL, NULL, NULL);
      OrdinalToPermut(ordinal, oper, 6, 0);
      CmdTranslate(tmp, 256, pattern, oper, 0);
      CCubes_CmdApply(&test, tmp);

      if (CCubes_GetState(&test) == 0) {
        // ok, found
        rotTable[MatrixInvert( or)] = ordinal;
        n++;
        break;
      }
    }

    ASSERT(n == or);
  }

  ASSERT(rotTable[1] == 719);

  for (int k = 1; k < 25; k += 8) {
    TRACE("InitRotTable 0x%04x,0x%04x,0x%04x,0x%04x,0x%04x,0x%04x,0x%04x,0x%04x,\n",
        rotTable[k + 0], rotTable[k + 1], rotTable[k + 2], rotTable[k + 3],
        rotTable[k + 4], rotTable[k + 5], rotTable[k + 6], rotTable[k + 7]);
  }
}



// Search for an optimized sequence of commands to correcting the orientation
// of the center facets of the faces


// Table of sequences of commands to change the orientation of the facet
// center faces without disturbing the Rubik's Cube (Herbert Kociemba, Cube
// Explorer 5.00 - Cubes With Twisted Centers).

// These 73 control sequences optimized for solving the 2048 different
// configurations of cubes order, with the facet center faces misdirected due
// to different symmetry operations or inversions performed on the Cube.

static char* cmdTable[] = {
  "U2 R2 F2 B2 L2 D2 R2 F2 B2 L2",                   //(10) u++ d++
  "U2 L' R B2 D2 F2 L R' D2 B2",                     //(10) u++ f++
  "U2 F2 L R' U2 F2 B2 U2 L' R F2 D2",               //(12) u++ d++ f++ b++
  "U R L U2 R' L' U R L U2 R' L'",                   //(12) u++
  "U L R' U D' L R' U' D B2 F2 L' R D'",             //(14) u+ d- f++ b++ r- l+
  "U R L F2 B2 R' L' D' R L F2 B2 R' L'",            //(14) u+ d-
  "U R L U' D F2 R L' U D R' L B2 D'",               //(14) u+ d+ f++ b++ r+ l+
  "U F B L R' U' D' F' U D L' R F' B'",              //(14) u+ f-
  "U R L U2 D2 R' L' F2 B2 U' D' R2 L2 D",           //(14) u++ d++ f++ b++ r++ l++
  "U F2 L2 U F2 R2 B2 D F2 L2 D R2 B2 L2",           //(14) u++ d++ f++ l++
  "U R L F2 B2 R' L' D F B R2 L2 F' B' D2",          //(15) u+ d- f++ b++ r++ l++
  "U R U R2 U' R' B2 L2 F2 D R2 F2 L B2 L",          //(15) u+ d+
  "U B F R' L U' D' B F' U D R L' B' F' D'",         //(16) u+ d- f- b+
  "U F L' R U' D F B' L' U D' L R' F' B D'",         //(16) u+ d- f+ l-
  "U F2 U2 F2 U2 L2 R2 B2 D' F2 U2 B2 U2 L2 R2 B2",  //(16) u+ d- f++ b++
  "U L2 F2 B2 R2 D' F B R2 U2 F B L2 U2 F2 L2",      //(16) u+ d- f++ l++
  "U B F R L' U' D' B' F' U D R' L B' F' D",         //(16) u+ d+ f- b-
  "U F2 R2 L2 B2 D F2 U2 B U2 R2 L2 D2 F D2 B2",     //(16) u+ d+ f+ b+
  "U2 R L D' F' B' R2 L2 D2 F B U' R' L' F2 B2",     //(16) u+ d+ f++ b++ r++ l++
  "U R L U2 L2 U' D R' L D2 F2 U' D' R L' D'",       //(16) u+ d+ f++ r+ l-
  "U F B L' R U' D' F2 B' U D L R' F' B' D2",        //(16) u+ d++ f++ b-
  "U F' B L' R U R' F B' U' D L R' F L' D'",         //(16) u+ f+ r- l-
  "U F U' D' F2 L' R F B U F' B' L R' F2 D",         //(16) u+ f+
  "U F U2 D2 F B' R L F2 R L B U' D' F2 D",          //(16) u++ d++ f++ r++ l++
  "R L D2 R' L' D F2 R2 D2 L2 B2 R2 U2 B2 L2 D",     //(16) u++ d++ f++
  "U R U D' F' B R L' D' F B2 U' D R' L F B'",       //(17) u+ d- f+ b++ r+
  "U R L' B' U D F' B R' L U' D F2 R F B' D'",       //(17) u+ d+ f++ b- r+
  "U F U B L2 B R2 F2 U F2 R2 B2 U' D F' U' F2",     //(17) u+ d+ f++ l++
  "U R U D F' B R' L U' D F2 D' R L' F B' D'",       //(17) u+ f++ r+
  "U R' F2 R L' U' D F B' R' L' U R' L F' B D'",     //(17) u+ f++ r++ l-
  "U D F2 U D' F R2 U2 D2 L2 B' U2 B2 R2 D2 L2 F2",  //(17) u++ f+ b+
  "U D R2 U D' L2 U2 F2 L F2 U2 D2 B2 R' D2 L2 F2",  //(17) u++ f++ b++ r+ l+
  "U R U R2 U' D' L' F2 D2 B2 R' D L U D' B2 D'",    //(17) u++ f++ r++
  "U R' F B' D R L' F' U D' L F B' U' R L' B D'",    //(18) u+ d- f+ b- r+ l-
  "R L' U D' L F B' U D' R L' F' U D' F B' U2 D2",   //(18) u+ d- f+ b++ r++ l-
  "F B U2 R2 L2 F' B' U R2 F2 U2 R2 F2 R2 D2 R2 F2 D",//(18) u+ d- f++ r++ l++
  "U F B' R2 F B L2 B2 D' F2 R2 D2 R2 F2 R2 D2 B2 L2",//(18) u+ d- f++
  "F U' D' B R L F' U' D' B R L F U' D' B' R L",     //(18) u+ d+ f+ b+ r- l-
  "U F L R' U' D F' B L R2 B2 U D' L' R F B' D",     //(18) u+ d+ f+ b++ r++ l+
  "R2 U2 L' F2 D2 U2 L2 R2 F2 R' U2 R2 D' L2 B2 F2 R2 U'",//(18) u+ d+ f++ b++ r- l-
  "U L R F2 B2 U2 L' R' D' F B' L2 F B U2 D2 L2 F2", //(18) u+ d+ f++ b++
  "U R U D' F' B R L' D' F2 B2 U' D R' L F B' D'",   //(18) u+ d++ f++ b++ r+
  "U R L2 U D F' B R' L U' D F2 D R L' F B' D'",     //(18) u+ d++ f++ r+ l++
  "U L U D L2 D2 B' F L R' U L R U2 R2 B F' D'",     //(18) u+ d++ r++ l+
  "F U R L' U' D R' L F B' R' L F' U D' R L' B'",    //(18) u+ f+ b++
  "U D' F' B' L2 B2 U F B' U' D' B' L2 R2 F L2 D2 B",//(18) u+ f+ r++ l++
  "F U R2 F B' U D F' B L2 U' D' F B D F' B' D2 B",  //(19) u+ d- f+ b+ r++ l++
  "U R U D R2 U' D' L U D' R L F2 L2 U' D R L' D'",  //(19) u+ d- f++ r+ l-
  "U R2 F R2 B R2 U D R2 L F' B' U' R2 D' F' B' L' D",//(19) u+ d+ f- b- r++
  "U2 B2 L B2 D' U R L' B F D U F' R' L D U B' F'",  //(19) u+ d+ f- l+
  "U F B L2 U D' F' B' U' D L2 F' L R F2 L' R' B' D",//(19) u+ d+ f+ b-
  "U R L U2 L2 F U D F' B' R L' U D' F2 R' F' B D",  //(19) u+ d+ f+ r+ l++
  "U R F' B' L2 B2 U D' R' L B R2 U' D F' B R' L' D",//(19) u+ d+ f++ b- r+ l++
  "U2 F' B' R2 U D B L' R' U' D F B R U' D' F2 L R", //(19) u+ d+ f++ b+ r-
  "U R2 F2 B U2 L F2 U2 D2 B2 R D2 B' L2 D F2 R2 L2 B2",//(19) u+ d+ f++ r+ l+
  "U B U' B' U' B' U' B U F2 R2 U2 L2 F B2 R2 D2 L2 B2",//(19) u+ d++ f-
  "U R2 U2 D2 L2 F B R L' U' D' B U' D' R' L F B D2",//(19) u+ d++ f++ b- r++ l++
  "U' L' U L U L U L' U' R' L' U2 B2 D2 L B2 U2 F2 R2",//(19) u+ d++ f++ r+
  "U R F2 R' L B2 U D F B R' L U' R' L' F' B L2 D",  //(19) u+ d++ f++ r++ l-
  "U F2 R L' U' D F B R' L' F' U D' R L F B R2 F2",  //(19) u+ f+ b++ r- l-
  "U F B' L' R' U F2 U2 D2 B2 L' R' F' B U D L R2 D",//(19) u+ f++ b++ l-
  "L' D2 B2 R' U2 B2 D2 L' F2 L2 U L U' L' U' L' U' L U",//(19) u+ f++ r-
  "F U F B' R2 L U' D F' B2 R L' D' R L' B U D' R' B'",//(20) u+ d- f+ b+ r- l-
  "F' U D B2 D2 F B' R' L U D' R' F B' U' D L2 B2 R' L'",//(20) u+ d- f+ b++ r+ l++
  "U R' L' F' B U D R D' F B' R L U' D' R2 U2 F U2 R2",//(20) u+ d- f+ r+
  "U R U' D' F' B R L D R2 U2 B U' D R L' B2 D2 F' B'",//(20) u+ d- f++ b- r+
  "L R D' F' B' L2 R B2 L' F' B' U' L R D2 F L2 F2 B' U2",//(20) u+ d+ f+ b- r- l+
  "U F2 R2 B2 L F2 U2 D2 B2 R' D R2 F2 L2 B R2 U2 D2 L2 F'",//(20)u+ d+ f+ b+ r+ l+
  "U F L2 F R2 F U D' R' D2 R2 F B' U' F B' L2 D2 L' D'",//(20) u+ d++ f+ b++ r- l-
  "U L R U2 D' B2 U R2 D L R' U D L2 R' F2 L D2 R D'",//(20) u+ d++ f++ b++ r++ l+
  "U L R B' L2 D2 R2 F2 U D' L U D F' U2 R2 D2 B2 L R'",//(20) u+ f+ b+ l+
  "U D' F B U F' B' U' D' B' U2 F2 B L2 D2 B U2 B R2 B",//(20) u+ f++ b- r++ l++
  "U D R' L U D' B R L' U D R' L B D2 L2 B' U2 D2 F R2"//(21) u+ d+ f+ b+ r+ l-
};

// Good selection of symmetry operations
static WORD ordiTbl[] = {
  0x0600, 0x0a40, 0x06c0, 0x1fc0, 0x0a00, 0x0b80, 0x07c0, 0x1ec0, 0x0bc0, 0x0698,
  0x2380, 0x4fc0, 0x5380, 0x2240, 0x5240, 0x4ec0, 0xae80, 0xaf80, 0xb200, 0x2213,
  0xb27f, 0x0a4f, 0xb3c0, 0x1e93, 0x23ff, 0x1ebf, 0x07cf, 0xb3bf, 0x1f93, 0x223f,
  0xaeff, 0x0b8f, 0x06cf, 0x1fbf, 0xafff, 0x4f93, 0x523f, 0x4e93, 0x53ff, 0x4fbf,
  0x4ebf, 0x67c0, 0x66c0, 0x91c0, 0x90c0, 0x6b80, 0x9440, 0x0a13, 0x943f, 0x6a40,
  0x9580, 0x0793, 0x0bff, 0x07bf, 0x95ff, 0x0693, 0x0a3f, 0x90bf, 0x06bf, 0x91bf,
  0x4e98, 0x5218, 0x67bf, 0x66bf, 0x6a3f, 0x6bff
};

#define TS ((sizeof cmdTable)/sizeof(PVOID))  // number of items in the table
#define OS ((sizeof ordiTbl)/sizeof(WORD))

// Table noting the rotations for the facet center of faces, calculated from
// the list of command sequences above (GenerateTable ()).
static DWORD centersTbl[TS] = {0, };
static WORD ix = 0, *xordiTbl = NULL;
// Conversion of a number less than 4096 in a state of centers facing
static inline DWORD ConvertCenters(UINT n)
{
  return ((n & 0x003) << 0) | ((n & 0x00C) << 2) | ((n & 0x030) << 4)
      | ((n & 0x0C0) << 6) | ((n & 0x300) << 8) | ((n & 0xC00) << 10);
}


inline WORD CountCenterFacets(DWORD centers, const WORD code[4])
{
  WORD cnt = 0;

  for (int i = 0; i < 6; i++, centers >>= 4) {
    cnt += code[centers & 3];
  }

  return cnt;
}

// table centersTbl generation
static void GenerateTable()
{
  if (0 == centersTbl[0]) {
    char cmd[256] = {0};

    // Creation of a table noting the rotation of the facet center of surfaces
    for (int i = 0; i < TS; i++) {
      CCubes cube;
      CCubes_Set(&cube, 1.f, NULL, NULL, NULL);
      CmdExtract(cmd, 256, cmdTable[i], 0, NULL);
      CCubes_CmdApply(&cube, cmd);
      centersTbl[i] = CCubes_CenterSidesRotation(&cube);
      // check that these commands do not mix the Cube
      ASSERT(CCubes_GetState(&cube) == 2);
    }
  }

  // for the test and debug operations
  // testGenerate();
}

static void ordiTblGenerate()
{
  char text[256] = {0};
  char tmp[256] = {0};
  int len = 0;

  for (WORD n = 0; n < ix; n++) {
    WORD data = xordiTbl[n];
    _snprintf(tmp, 256, "0x%04X", (data % 720) * 64 + data / 720);

    if (strlen(text) > len + 60) {
      STRcat(text, 256, ",\n   ", -1);
      len = strlen(text);
    }
    else if (text[0] != 0) {
      STRcat_c(text, 256, ',');
    }

    STRcat(text, 256, tmp, -1);
  }

  TRACE("static WORD ordiTbl[%d] = {\n   %s };", ix, text);
}
// Search a sequence of good order correcting the Orientation of the center facet surfaces
int ResolveCenters(DWORD centers,   // center facets Orientation
    char* out, int len) // out commands

{
  static const WORD codeCntAbs[4] = { 0x1000, 0x0001, 0x0010, 0x0001 };
  DWORD centTable, centObj, negCenters = NegateCenters(centers);
  WORD cntAbs = CountCenterFacets(centers, codeCntAbs);
  char cmd[256] = {0};
  char cmdTmp[256] = {0};
  GenerateTable();

  out[0] = 0;

  if (centers == 0) {
    return 0;  // It's yet ok
  }

  // Only deals a Rubik's Cube already ordered
  if (!IsCentersLegal(centers)) {
    return 8;
  }

  // Search in the orders table stored as a command may agree to amend the
  // right way the orientation of the faces. Search by successive switching
  // of the control sequence that corrects the orientation of the central
  // facets without mixing the Rubik's Cube.
  for (int i = 0; i < TS; i++) {
    centTable = centersTbl[i];

    // Removes commands that do not alter the same number of faces
    if (CountCenterFacets(centTable, codeCntAbs) != cntAbs) {
      continue;
    }

    // Reading the sequence of commands in the table
    cmd[0] = 0;
    CmdExtract(cmd, 256, cmdTable[i], 0, NULL);

    // Search by successive switching of the control sequence that corrects
    // the orientation of the central facets without mixing the Rubik's
    // Cube.

    // Legal operations of symmetry are extracted of the 720 * 64 possible
    // combinations of transcoding. A legal transaction is recognizable by
    // the fact that it does not mix the Cube. In addition, good operation
    // changes the state faces so as to cancel all the rotations defined.
    // For convenience, these tests are conducted in reverse order of that
    // comment.
    for (WORD ordinal = 0; ordinal < OS; ordinal++) {
      WORD ot = ordiTbl[ordinal], ordSgn = ot & 0x003F, ordCmd = ot >> 6;
      BYTE oper[6]; // permutation sequence (one order for each side)

      // Create a permutation sequence
      OrdinalToPermut(ordCmd, oper, 6, 0);

      // Permutes the central facets orientation outcome of the table
      centObj = CenterTranslate(centTable, oper, ordSgn);

      // Eliminates the permutation that does not lead to the result
      if (centObj != centers && centObj != negCenters) {
        continue;
      }

      // Generates the corresponding sequence of commands
      CmdTranslate(cmdTmp, 256, cmd, oper, ordSgn);

      // Test the new sequence of commands on a Rubik's Cube. Removes this
      // sequence if it mixes the Cube (illegal swapping)
      CCubes tmp;
      CCubes_Set(&tmp, 1.f, NULL, NULL, NULL);
      CCubes_CmdApply(&tmp, cmdTmp);

      if (CCubes_GetState(&tmp) != 2) {
        continue;
      }

      // Ok, the command sequence is found! Signs are adjusted
      if (centObj != negCenters) {
        CmdReverse(cmdTmp);
      }

      STRcpy(out, len, cmdTmp, -1);
      return 0;
    }
  }

  // Not found error
  return -8;
}
// Test program and possibly of the table ordiTbl generation
static void testGenerate()
{
  int n0 = 0, n1 = 0;
  int err;
  UINT j;
  char cmd[256] = {0};
  char msg[256] = {0};

  ix = 0;
  // This program verifies that the 2048 cases are handled properly

  if (xordiTbl) {
    for (j = 0; j < 720 * 64; j++) {
      xordiTbl[j] = (WORD)j;
    }
  }

  for (j = 0; j < (1 << 12); j++) {
    DWORD centers = ConvertCenters(j);

    if (!IsCentersLegal(centers)) {
      continue;
    }

    n0++;

    if ((err = ResolveCenters(centers, cmd, 256)) != 0) {
      n1++;

      GetMsgCenter(msg, 256, centers, -1);
      TRACE("GeneError %06x %d %s\n", centers, err, msg);
    }
    else {
      CmdConvert(msg, 256, cmd, false);
      TRACE("Genetable %4d %4d %06x %s\n", n0, ix, centers, msg);
    }
  }

  if (xordiTbl) {
    ordiTblGenerate();
    free(xordiTbl);
    xordiTbl = NULL;
  }

  // 'total' must be 2048 and 'error' must be 0
  TRACE("GenerateTable total=%d error=%d\n", n0, n1);
}

static void SetData(WORD index)
{
  if (xordiTbl && index >= ix) {
    if (index > ix) {
      WORD t = xordiTbl[ix];
      xordiTbl[ix] = xordiTbl[index];
      xordiTbl[index] = t;
    }

    ix++;
  }
}


