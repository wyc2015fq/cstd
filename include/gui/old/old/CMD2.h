
#include "cfile.h"
#include "math3d.h"

#define MD2_MAGIC_NUMBER   844121161
#define MD2_VERSION        8
#define MD2_MAX_VERTS      2048
// byte-align structures
//#include "irrpack.h"
typedef struct SMD2Vertex {
  u8 vertex[3];        // [0] = X, [1] = Z, [2] = Y
  u8 lightNormalIndex; // index in the normal table
} SMD2Vertex;
typedef struct SMD2Frame {
  f32 scale[3];           // first scale the vertex position
  f32 translate[3];       // then translate the position
  c8  name[16];           // the name of the animation that this key belongs to
  SMD2Vertex vertices[1]; // vertex 1 of SMD2Header.numVertices
} SMD2Frame;
typedef struct SMD2Triangle {
  u16 vertexIndices[3];
  u16 textureIndices[3];
} SMD2Triangle;
typedef struct SMD2TextureCoordinate {
  s16 s;
  s16 t;
} SMD2TextureCoordinate;
typedef struct SMD2GLCommand {
  f32 s, t;
  s32 vertexIndex;
} SMD2GLCommand;
//! named animations
typedef struct SAnimationData {
  char name[16];
  s32 begin;
  s32 end;
  s32 fps;
} SAnimationData;
//! scale and translations for keyframes
typedef struct SKeyFrameTransform {
  float3 scale;
  float3 translate;
} SKeyFrameTransform;
typedef struct SMD2Vert {
  uchar Pos[3];
  u8  NormalIdx;
} SMD2Vert;
typedef struct CAnimatedMeshMD2 {
  //IMeshBuffer InterpolationBuffer[1]; //! the buffer that contains the most recent animation
  SKeyFrameTransform* FrameTransforms; //! md2 vertex data keyframe transformations
  //! keyframe vertex data
  SMD2Vert* FrameList; // numFrames*numVertices
  //! bounding boxes for each keyframe
  FAABBOX3* BoxList; // FrameCount
  //! named animations
  SAnimationData* AnimationData;
  S3DVertex* Vertices;
  int Vertices_size;
  int* Indices;
  int Indices_size;
  int AnimationData_size;
  u32 FrameCount;
  f32 FramesPerSecond;
} CAnimatedMeshMD2;

#define MD2_FRAME_SHIFT 2
#define MD2_FRAME_SHIFT_RECIPROCAL  (1.f / (1 << MD2_FRAME_SHIFT))
#define Q2_VERTEX_NORMAL_TABLE_SIZE  162
static const f32 Q2_VERTEX_NORMAL_TABLE[Q2_VERTEX_NORMAL_TABLE_SIZE][3] = {
  { -0.525731f, 0.000000f, 0.850651f},
  { -0.442863f, 0.238856f, 0.864188f},
  { -0.295242f, 0.000000f, 0.955423f},
  { -0.309017f, 0.500000f, 0.809017f},
  { -0.162460f, 0.262866f, 0.951056f},
  {0.000000f, 0.000000f, 1.000000f},
  {0.000000f, 0.850651f, 0.525731f},
  { -0.147621f, 0.716567f, 0.681718f},
  {0.147621f, 0.716567f, 0.681718f},
  {0.000000f, 0.525731f, 0.850651f},
  {0.309017f, 0.500000f, 0.809017f},
  {0.525731f, 0.000000f, 0.850651f},
  {0.295242f, 0.000000f, 0.955423f},
  {0.442863f, 0.238856f, 0.864188f},
  {0.162460f, 0.262866f, 0.951056f},
  { -0.681718f, 0.147621f, 0.716567f},
  { -0.809017f, 0.309017f, 0.500000f},
  { -0.587785f, 0.425325f, 0.688191f},
  { -0.850651f, 0.525731f, 0.000000f},
  { -0.864188f, 0.442863f, 0.238856f},
  { -0.716567f, 0.681718f, 0.147621f},
  { -0.688191f, 0.587785f, 0.425325f},
  { -0.500000f, 0.809017f, 0.309017f},
  { -0.238856f, 0.864188f, 0.442863f},
  { -0.425325f, 0.688191f, 0.587785f},
  { -0.716567f, 0.681718f, -0.147621f},
  { -0.500000f, 0.809017f, -0.309017f},
  { -0.525731f, 0.850651f, 0.000000f},
  {0.000000f, 0.850651f, -0.525731f},
  { -0.238856f, 0.864188f, -0.442863f},
  {0.000000f, 0.955423f, -0.295242f},
  { -0.262866f, 0.951056f, -0.162460f},
  {0.000000f, 1.000000f, 0.000000f},
  {0.000000f, 0.955423f, 0.295242f},
  { -0.262866f, 0.951056f, 0.162460f},
  {0.238856f, 0.864188f, 0.442863f},
  {0.262866f, 0.951056f, 0.162460f},
  {0.500000f, 0.809017f, 0.309017f},
  {0.238856f, 0.864188f, -0.442863f},
  {0.262866f, 0.951056f, -0.162460f},
  {0.500000f, 0.809017f, -0.309017f},
  {0.850651f, 0.525731f, 0.000000f},
  {0.716567f, 0.681718f, 0.147621f},
  {0.716567f, 0.681718f, -0.147621f},
  {0.525731f, 0.850651f, 0.000000f},
  {0.425325f, 0.688191f, 0.587785f},
  {0.864188f, 0.442863f, 0.238856f},
  {0.688191f, 0.587785f, 0.425325f},
  {0.809017f, 0.309017f, 0.500000f},
  {0.681718f, 0.147621f, 0.716567f},
  {0.587785f, 0.425325f, 0.688191f},
  {0.955423f, 0.295242f, 0.000000f},
  {1.000000f, 0.000000f, 0.000000f},
  {0.951056f, 0.162460f, 0.262866f},
  {0.850651f, -0.525731f, 0.000000f},
  {0.955423f, -0.295242f, 0.000000f},
  {0.864188f, -0.442863f, 0.238856f},
  {0.951056f, -0.162460f, 0.262866f},
  {0.809017f, -0.309017f, 0.500000f},
  {0.681718f, -0.147621f, 0.716567f},
  {0.850651f, 0.000000f, 0.525731f},
  {0.864188f, 0.442863f, -0.238856f},
  {0.809017f, 0.309017f, -0.500000f},
  {0.951056f, 0.162460f, -0.262866f},
  {0.525731f, 0.000000f, -0.850651f},
  {0.681718f, 0.147621f, -0.716567f},
  {0.681718f, -0.147621f, -0.716567f},
  {0.850651f, 0.000000f, -0.525731f},
  {0.809017f, -0.309017f, -0.500000f},
  {0.864188f, -0.442863f, -0.238856f},
  {0.951056f, -0.162460f, -0.262866f},
  {0.147621f, 0.716567f, -0.681718f},
  {0.309017f, 0.500000f, -0.809017f},
  {0.425325f, 0.688191f, -0.587785f},
  {0.442863f, 0.238856f, -0.864188f},
  {0.587785f, 0.425325f, -0.688191f},
  {0.688191f, 0.587785f, -0.425325f},
  { -0.147621f, 0.716567f, -0.681718f},
  { -0.309017f, 0.500000f, -0.809017f},
  {0.000000f, 0.525731f, -0.850651f},
  { -0.525731f, 0.000000f, -0.850651f},
  { -0.442863f, 0.238856f, -0.864188f},
  { -0.295242f, 0.000000f, -0.955423f},
  { -0.162460f, 0.262866f, -0.951056f},
  {0.000000f, 0.000000f, -1.000000f},
  {0.295242f, 0.000000f, -0.955423f},
  {0.162460f, 0.262866f, -0.951056f},
  { -0.442863f, -0.238856f, -0.864188f},
  { -0.309017f, -0.500000f, -0.809017f},
  { -0.162460f, -0.262866f, -0.951056f},
  {0.000000f, -0.850651f, -0.525731f},
  { -0.147621f, -0.716567f, -0.681718f},
  {0.147621f, -0.716567f, -0.681718f},
  {0.000000f, -0.525731f, -0.850651f},
  {0.309017f, -0.500000f, -0.809017f},
  {0.442863f, -0.238856f, -0.864188f},
  {0.162460f, -0.262866f, -0.951056f},
  {0.238856f, -0.864188f, -0.442863f},
  {0.500000f, -0.809017f, -0.309017f},
  {0.425325f, -0.688191f, -0.587785f},
  {0.716567f, -0.681718f, -0.147621f},
  {0.688191f, -0.587785f, -0.425325f},
  {0.587785f, -0.425325f, -0.688191f},
  {0.000000f, -0.955423f, -0.295242f},
  {0.000000f, -1.000000f, 0.000000f},
  {0.262866f, -0.951056f, -0.162460f},
  {0.000000f, -0.850651f, 0.525731f},
  {0.000000f, -0.955423f, 0.295242f},
  {0.238856f, -0.864188f, 0.442863f},
  {0.262866f, -0.951056f, 0.162460f},
  {0.500000f, -0.809017f, 0.309017f},
  {0.716567f, -0.681718f, 0.147621f},
  {0.525731f, -0.850651f, 0.000000f},
  { -0.238856f, -0.864188f, -0.442863f},
  { -0.500000f, -0.809017f, -0.309017f},
  { -0.262866f, -0.951056f, -0.162460f},
  { -0.850651f, -0.525731f, 0.000000f},
  { -0.716567f, -0.681718f, -0.147621f},
  { -0.716567f, -0.681718f, 0.147621f},
  { -0.525731f, -0.850651f, 0.000000f},
  { -0.500000f, -0.809017f, 0.309017f},
  { -0.238856f, -0.864188f, 0.442863f},
  { -0.262866f, -0.951056f, 0.162460f},
  { -0.864188f, -0.442863f, 0.238856f},
  { -0.809017f, -0.309017f, 0.500000f},
  { -0.688191f, -0.587785f, 0.425325f},
  { -0.681718f, -0.147621f, 0.716567f},
  { -0.442863f, -0.238856f, 0.864188f},
  { -0.587785f, -0.425325f, 0.688191f},
  { -0.309017f, -0.500000f, 0.809017f},
  { -0.147621f, -0.716567f, 0.681718f},
  { -0.425325f, -0.688191f, 0.587785f},
  { -0.162460f, -0.262866f, 0.951056f},
  {0.442863f, -0.238856f, 0.864188f},
  {0.162460f, -0.262866f, 0.951056f},
  {0.309017f, -0.500000f, 0.809017f},
  {0.147621f, -0.716567f, 0.681718f},
  {0.000000f, -0.525731f, 0.850651f},
  {0.425325f, -0.688191f, 0.587785f},
  {0.587785f, -0.425325f, 0.688191f},
  {0.688191f, -0.587785f, 0.425325f},
  { -0.955423f, 0.295242f, 0.000000f},
  { -0.951056f, 0.162460f, 0.262866f},
  { -1.000000f, 0.000000f, 0.000000f},
  { -0.850651f, 0.000000f, 0.525731f},
  { -0.955423f, -0.295242f, 0.000000f},
  { -0.951056f, -0.162460f, 0.262866f},
  { -0.864188f, 0.442863f, -0.238856f},
  { -0.951056f, 0.162460f, -0.262866f},
  { -0.809017f, 0.309017f, -0.500000f},
  { -0.864188f, -0.442863f, -0.238856f},
  { -0.951056f, -0.162460f, -0.262866f},
  { -0.809017f, -0.309017f, -0.500000f},
  { -0.681718f, 0.147621f, -0.716567f},
  { -0.681718f, -0.147621f, -0.716567f},
  { -0.850651f, 0.000000f, -0.525731f},
  { -0.688191f, 0.587785f, -0.425325f},
  { -0.587785f, 0.425325f, -0.688191f},
  { -0.425325f, 0.688191f, -0.587785f},
  { -0.425325f, -0.688191f, -0.587785f},
  { -0.587785f, -0.425325f, -0.688191f},
  { -0.688191f, -0.587785f, -0.425325f},
};
typedef struct SMD2AnimationType {
  s32 begin, end, fps;
} SMD2AnimationType;
static const SMD2AnimationType MD2AnimationTypeList[21] = {
  {  0,  39,  9}, // STAND
  { 40,  45, 10}, // RUN
  { 46,  53, 10}, // ATTACK
  { 54,  57,  7}, // PAIN_A
  { 58,  61,  7}, // PAIN_B
  { 62,  65,  7}, // PAIN_C
  { 66,  71,  7}, // JUMP
  { 72,  83,  7}, // FLIP
  { 84,  94,  7}, // SALUTE
  { 95, 111, 10}, // FALLBACK
  {112, 122,  7}, // WAVE
  {123, 134,  6}, // POINT
  {135, 153, 10}, // CROUCH_STAND
  {154, 159,  7}, // CROUCH_WALK
  {160, 168, 10}, // CROUCH_ATTACK
  {169, 172,  7}, // CROUCH_PAIN
  {173, 177,  5}, // CROUCH_DEATH
  {178, 183,  7}, // DEATH_FALLBACK
  {184, 189,  7}, // DEATH_FALLFORWARD
  {190, 197,  7}, // DEATH_FALLBACKSLOW
  {198, 198,  5}, // BOOM
};

//! loads an md2 file
bool CMD2_loadFile(stream_t* file, CAnimatedMeshMD2* mesh)
{
  char buf[256];
  int i = 0, j, s, ti, count = 0;
  int n = stream_read(file, buf, 4 * 17);
  s32 MEM_GET32L(buf, i, magic);           // four character code "IDP2"
  s32 MEM_GET32L(buf, i, version);         // must be 8
  s32 MEM_GET32L(buf, i, skinWidth);       // width of the texture
  s32 MEM_GET32L(buf, i, skinHeight);      // height of the texture
  s32 MEM_GET32L(buf, i, frameSize);       // size in bytes of an animation frame
  s32 MEM_GET32L(buf, i, numSkins);        // number of textures
  s32 MEM_GET32L(buf, i, numVertices);     // total number of vertices
  s32 MEM_GET32L(buf, i, numTexcoords);    // number of vertices with texture coords
  s32 MEM_GET32L(buf, i, numTriangles);    // number of triangles
  s32 MEM_GET32L(buf, i, numGlCommands);   // number of opengl commands (triangle strip or triangle fan)
  s32 MEM_GET32L(buf, i, numFrames);       // animation keyframe count
  s32 MEM_GET32L(buf, i, offsetSkins);     // offset in bytes to 64 character skin names
  s32 MEM_GET32L(buf, i, offsetTexcoords); // offset in bytes to texture coordinate list
  s32 MEM_GET32L(buf, i, offsetTriangles); // offset in bytes to triangle list
  s32 MEM_GET32L(buf, i, offsetFrames);    // offset in bytes to frame list
  s32 MEM_GET32L(buf, i, offsetGlCommands);// offset in bytes to opengl commands
  s32 MEM_GET32L(buf, i, offsetEnd);       // offset in bytes to end of file
  SMD2TextureCoordinate* textureCoords = NULL;
  SMD2Triangle* triangles = NULL;

  if (magic != MD2_MAGIC_NUMBER || version != MD2_VERSION) {
    llogprintf(ELL_WARNING, "MD2 Loader: Wrong file header");
    return false;
  }

  // prepare mesh and allocate memory
  mesh->FrameCount = numFrames;
  // create keyframes
  MYREALLOC(mesh->FrameTransforms, numFrames);
  MYREALLOC(mesh->BoxList, numFrames);

  // create vertex arrays for each keyframe
  if (mesh->FrameList) {
    free(mesh->FrameList);
  }

  count = numTriangles * 3;
  MYREALLOC(mesh->FrameList, numFrames * numVertices);
  // allocate space in vertex arrays
  // allocate interpolation buffer vertices
  MYREALLOC(mesh->Vertices, count);
  // populate triangles
  MYREALLOC(mesh->Indices, count);

  for (i = 0; i < count; i += 3) {
    mesh->Indices[i] = i;
    mesh->Indices[i + 1] = (i + 1);
    mesh->Indices[i + 2] = (i + 2);
  }

  // read texture coordinates
  stream_seek(file, offsetTexcoords, SEEK_SET);
  MYREALLOC(textureCoords, numTexcoords);

  if (!stream_read(file, textureCoords, sizeof(SMD2TextureCoordinate)*numTexcoords)) {
    free(textureCoords);
    llogprintf(ELL_ERROR, "MD2 Loader: Error reading TextureCoords.");
    return false;
  }

  if ('b' == ENDIANNESS) {
    for (i = 0; i < numTexcoords; ++i) {
      BYTESWAP_16(textureCoords[i].s);
      BYTESWAP_16(textureCoords[i].t);
    }
  }

  // read Triangles
  stream_seek(file, offsetTriangles, SEEK_SET);
  MYREALLOC(triangles, numTriangles);

  if (!stream_read(file, triangles, numTriangles * sizeof(SMD2Triangle))) {
    FREE(triangles);
    FREE(textureCoords);
    llogprintf(ELL_ERROR, "MD2 Loader: Error reading triangles.");
    return false;
  }

  if ('b' == ENDIANNESS) {
    for (i = 0; i < numTriangles; ++i) {
      BYTESWAP_16(triangles[i].vertexIndices[0]);
      BYTESWAP_16(triangles[i].vertexIndices[1]);
      BYTESWAP_16(triangles[i].vertexIndices[2]);
      BYTESWAP_16(triangles[i].textureIndices[0]);
      BYTESWAP_16(triangles[i].textureIndices[1]);
      BYTESWAP_16(triangles[i].textureIndices[2]);
    }
  }

#if 0

  if (1) {
    int aa[512] = {0}, tt = 0;

    for (i = 0; i < 512; ++i) {
      aa[i] = -1;
    }

    for (i = 0; i < numTriangles; ++i) {
      for (j = 0; j < 3; ++j) {
        int id = triangles[i].vertexIndices[j];
        int ic = triangles[i].textureIndices[j];

        if (aa[id] < 0) {
          aa[id] = ic;
        }
        else {
          if (textureCoords[aa[id]].s == textureCoords[ic].s && textureCoords[aa[id]].t == textureCoords[ic].t) {}
          else {
            printf("%d %d  %d %d\n", textureCoords[aa[id]].s, textureCoords[ic].s,  textureCoords[aa[id]].t, textureCoords[ic].t);
          }
        }
      }
    }

    for (i = 0; i < numTexcoords; ++i) {
      if (aa[textureCoords[i].s] < 0) {
        aa[textureCoords[i].s] = textureCoords[i].t;
        ++tt;
      }
      else {
        printf("%d, %d\n", textureCoords[i].s, textureCoords[i].t);
      }
    }

    tt = tt;
  }

#endif

  // read Vertices
  {
    u8 buffer[MD2_MAX_VERTS * 4 + 128];
    SMD2Frame* frame = (SMD2Frame*)buffer;
    stream_seek(file, offsetFrames, SEEK_SET);
    MYREALLOC(mesh->AnimationData, numFrames);

    for (i = 0; i < numFrames; ++i) {
      SAnimationData adata = {0};
      // read vertices
      stream_read(file, frame, frameSize);

      if ('b' == ENDIANNESS) {
        BYTESWAP_32(triangles[i].vertexIndices[0]);
        BYTESWAP_32(triangles[i].vertexIndices[1]);
        BYTESWAP_32(triangles[i].vertexIndices[2]);
        BYTESWAP_32(triangles[i].textureIndices[0]);
        BYTESWAP_32(triangles[i].textureIndices[1]);
        BYTESWAP_32(triangles[i].textureIndices[2]);
      }

      // store frame data
      adata.begin = i;
      adata.end = i;
      adata.fps = 7;

      // Add new named animation if necessary
      if (frame->name[0]) {
        // get animation name
        j = 0;

        for (s = 0; s < 16 && frame->name[s] != 0 && (frame->name[s] < '0' || frame->name[s] > '9'); ++s) {
          adata.name[j++] += frame->name[s];
        }

        // Does this keyframe have the same animation name as the current animation?
        if (mesh->AnimationData_size > 0 && 0 == strcmp(mesh->AnimationData[mesh->AnimationData_size - 1].name, adata.name)) {
          // Increase the length of the animation
          ++mesh->AnimationData[mesh->AnimationData_size - 1].end;
        }
        else {
          // Add the new animation
          mesh->AnimationData[mesh->AnimationData_size++] = adata;
        }
      }

      // save keyframe scale and translation
      mesh->FrameTransforms[i].scale[0] = frame->scale[0];
      mesh->FrameTransforms[i].scale[2] = frame->scale[1];
      mesh->FrameTransforms[i].scale[1] = frame->scale[2];
      mesh->FrameTransforms[i].translate[0] = frame->translate[0];
      mesh->FrameTransforms[i].translate[2] = frame->translate[1];
      mesh->FrameTransforms[i].translate[1] = frame->translate[2];

      // add vertices
      for (j = 0; j < numTriangles; ++j) {
        for (ti = 0; ti < 3; ++ti) {
          SMD2Vert* v = mesh->FrameList + j * 3 + ti;
          u32 num = triangles[j].vertexIndices[ti];
          v->Pos[0] = frame->vertices[num].vertex[0];
          v->Pos[2] = frame->vertices[num].vertex[1];
          v->Pos[1] = frame->vertices[num].vertex[2];
          v->NormalIdx = frame->vertices[num].lightNormalIndex;
        }
      }

      // calculate bounding boxes
      if (numVertices) {
        FAABBOX3* box = mesh->BoxList + i;
        float3 pos;
        FAABBOX3_reset(box);

        for (j = 0; j < numTriangles * 3; ++j) {
          pos[0] = (f32)(mesh->FrameList[i * 3 + j].Pos[0]) * mesh->FrameTransforms[i].scale[0] + mesh->FrameTransforms[i].translate[0];
          pos[1] = (f32)(mesh->FrameList[i * 3 + j].Pos[1]) * mesh->FrameTransforms[i].scale[1] + mesh->FrameTransforms[i].translate[1];
          pos[2] = (f32)(mesh->FrameList[i * 3 + j].Pos[2]) * mesh->FrameTransforms[i].scale[2] + mesh->FrameTransforms[i].translate[2];
          FAABBOX3_addInternalPoint(box, pos);
        }
      }
    }
  }

  // populate interpolation buffer with texture coordinates and colors
  if (numFrames) {
    int t, n;
    f32 dmaxs = 1.0f / (skinWidth);
    f32 dmaxt = 1.0f / (skinHeight);

    for (t = 0; t < numTriangles; ++t) {
      for (n = 0; n < 3; ++n) {
        SMD2TextureCoordinate* coord = textureCoords + triangles[t].textureIndices[n];
        mesh->Vertices[t * 3 + n].TCoords[0] = (coord->s + 0.5f) * dmaxs;
        mesh->Vertices[t * 3 + n].TCoords[1] = (coord->t + 0.5f) * dmaxt;
        mesh->Vertices[t * 3 + n].Color = _ARGB(255, 255, 255, 255);
      }
    }
  }

  // clean up
  FREE(triangles);
  FREE(textureCoords);
  // init buffer with start frame.
  return true;
}

// updates the interpolation buffer
void CMD2_getMesh(CAnimatedMeshMD2* mesh, s32 frame)
{
#if 0
  s32 startFrameLoop, endFrameLoop;
  u32 firstFrame, secondFrame;
  f32 div;

  // TA: resolve missing ipol in loop between end-start
  if (endFrameLoop - startFrameLoop == 0) {
    firstFrame = frame >> MD2_FRAME_SHIFT;
    secondFrame = frame >> MD2_FRAME_SHIFT;
    div = 1.0f;
  }
  else {
    // key frames
    u32 s = startFrameLoop >> MD2_FRAME_SHIFT;
    u32 e = endFrameLoop >> MD2_FRAME_SHIFT;
    firstFrame = frame >> MD2_FRAME_SHIFT;
    secondFrame = if_c_a_else_b(firstFrame + 1 > e, s, firstFrame + 1);
    firstFrame = IMIN(mesh->FrameCount - 1, firstFrame);
    secondFrame = IMIN(mesh->FrameCount - 1, secondFrame);
    //div = (frame % (1<<MD2_FRAME_SHIFT)) / (f32)(1<<MD2_FRAME_SHIFT);
    frame &= (1 << MD2_FRAME_SHIFT) - 1;
    div = frame * MD2_FRAME_SHIFT_RECIPROCAL;
  }

  S3DVertex* target = static_cast<S3DVertex*>(InterpolationBuffer->getVertices());
  SMD2Vert* first = FrameList[firstFrame];
  SMD2Vert* second = FrameList[secondFrame];
  // interpolate both frames
  const u32 count = FrameList[firstFrame]_size;
  int i;

  for (i = 0; i < count; ++i) {
    const vector3df one = f3VECTOR(f32(first->Pos.X) * FrameTransforms[firstFrame].scale.X + FrameTransforms[firstFrame].translate.X,
        f32(first->Pos.Y) * FrameTransforms[firstFrame].scale.Y + FrameTransforms[firstFrame].translate.Y,
        f32(first->Pos.Z) * FrameTransforms[firstFrame].scale.Z + FrameTransforms[firstFrame].translate.Z);
    const vector3df two = f3VECTOR(f32(second->Pos.X) * FrameTransforms[secondFrame].scale.X + FrameTransforms[secondFrame].translate.X,
        f32(second->Pos.Y) * FrameTransforms[secondFrame].scale.Y + FrameTransforms[secondFrame].translate.Y,
        f32(second->Pos.Z) * FrameTransforms[secondFrame].scale.Z + FrameTransforms[secondFrame].translate.Z);
    target->Pos = two.getInterpolated(one, div);
    const vector3df n1(
        Q2_VERTEX_NORMAL_TABLE[first->NormalIdx][0],
        Q2_VERTEX_NORMAL_TABLE[first->NormalIdx][2],
        Q2_VERTEX_NORMAL_TABLE[first->NormalIdx][1]);
    const vector3df n2(
        Q2_VERTEX_NORMAL_TABLE[second->NormalIdx][0],
        Q2_VERTEX_NORMAL_TABLE[second->NormalIdx][2],
        Q2_VERTEX_NORMAL_TABLE[second->NormalIdx][1]);
    target->Normal = n2.getInterpolated(n1, div);
    ++target;
    ++first;
    ++second;
  }

  //update bounding box
  InterpolationBuffer->setBoundingBox(BoxList[secondFrame].getInterpolated(BoxList[firstFrame], div));
  InterpolationBuffer->setDirty();
#endif
}

#undef MD2_MAGIC_NUMBER
#undef MD2_VERSION
#undef MD2_MAX_VERTS
