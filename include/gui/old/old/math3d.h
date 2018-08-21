
#ifndef _MATH3D_H_
#define _MATH3D_H_
typedef enum E_TYPE {
  EIT_BYTE,
  EIT_UBYTE,
  EIT_SHORT,
  EIT_USHORT,
  EIT_INT,
  EIT_UINT,
  EIT_FLOAT,
  EIT_DOUBLE,
  EIT_2BYTES,
  EIT_3BYTES,
  EIT_4BYTES,
} E_TYPE;
//! Enumeration for all primitive types there are.
typedef enum E_PRIMITIVE_TYPE {
  EPT_POINTS = 0,  //! All vertices are non-connected points.
  EPT_LINE_STRIP,  //! All vertices form a single connected line.
  EPT_LINE_LOOP,  //! Just as LINE_STRIP, but the last and the first vertex is also connected.
  EPT_LINES,  //! Every two vertices are connected creating n/2 lines.
  EPT_TRIANGLE_STRIP,  //! After the first two vertices each vertex defines a new triangle.
  EPT_TRIANGLE_FAN,  //! After the first two vertices each vertex defines a new triangle.
  EPT_TRIANGLES,  //! Explicitly set all vertices for each triangle.
  EPT_QUAD_STRIP,  //! After the first two vertices each further tw vetices create a quad with the preceding two.
  EPT_QUADS,  //! Every four vertices create a quad.
  EPT_POLYGON,  //! Just as LINE_LOOP, but filled.
  EPT_POINT_SPRITES  //! The single vertices are expanded to quad billboards on the GPU.
} E_PRIMITIVE_TYPE;

typedef struct FAABBOX3 {
  float a[3]; //min
  float b[3]; //max
} FAABBOX3;
typedef struct S3DVertex {
  float3 Pos; //! Position
  float3 Normal; //! Normal vector
  COLOR Color; //! Color
  float2 TCoords2;
  float2 TCoords; //! Texture coordinates
  float3 Tangent; //! Tangent vector along the x-axis of the texture
  float3 Binormal; //! Binormal vector (tangent x normal)
} S3DVertex;
typedef struct S2DVertex {
  IPOINT Pos;  // position
  IPOINT TCoords;  // texture coordinates
  int ZValue;    // zvalue
  COLOR Color;
} S2DVertex;
typedef enum E_HARDWARE_MAPPING {
  EHM_NEVER = 0, //! Don't store on the hardware
  EHM_STATIC,  //! Rarely changed, usually stored completely on the hardware
  EHM_DYNAMIC,  //! Sometimes changed, driver optimized placement
  EHM_STREAM  //! Always changed, cache optimizing on the GPU
} E_HARDWARE_MAPPING;
typedef struct IMeshBuffer {
  //u32 ChangedID_Vertex;
  //u32 ChangedID_Index;
  //! hardware mapping hint
  //E_HARDWARE_MAPPING MappingHint_Vertex;
  //E_HARDWARE_MAPPING MappingHint_Index;
  //SMaterial Material; //! Material for this meshbuffer.
  S3DVertex* Vertices;  //! Vertices of this buffer
  int Vertices_size;
  int* Indices;  //! Indices into the vertices of this buffer.
  int Indices_size;
  FAABBOX3 BoundingBox;  //! Bounding box of this meshbuffer.
} IMeshBuffer;
static void FAABBOX3_reset(FAABBOX3* box)
{
  VSET3(box->a, FLT_MAX, FLT_MAX, FLT_MAX);
  VSET3(box->b, FLT_MIN, FLT_MIN, FLT_MIN);
}
static void FAABBOX3_addInternalPoint(FAABBOX3* box, const float* v)
{
  box->a[0] = MIN(box->a[0], v[0]);
  box->a[1] = MIN(box->a[0], v[1]);
  box->a[2] = MIN(box->a[0], v[2]);
  box->b[0] = MAX(box->b[0], v[0]);
  box->b[1] = MAX(box->b[0], v[1]);
  box->b[2] = MAX(box->b[0], v[2]);
}
static void FAABBOX3_getCenter(const FAABBOX3* box, float* v)
{
  VADD3(v, box->b, box->a);
  VSCALE3(v, v, 0.5f);
}
static void FAABBOX3_repair(FAABBOX3* box)
{
  float t;

  if (box->a[0] > box->b[0]) {
    t = box->a[0];
    box->a[0] = box->b[0];
    box->b[0] = t;
  }

  if (box->a[1] > box->b[1]) {
    t = box->a[1];
    box->a[1] = box->b[1];
    box->b[1] = t;
  }

  if (box->a[2] > box->b[2]) {
    t = box->a[2];
    box->a[2] = box->b[2];
    box->b[2] = t;
  }
}

#endif // _MATH3D_H_
