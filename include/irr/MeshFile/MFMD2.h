
#include "cfile.h"
#include "img/imgio.inl"

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
  //! the buffer that contains the most recent animation
  IMeshBuffer InterpolationBuffer[1];
  //! md2 vertex data
  //! keyframe transformations
  SKeyFrameTransform* FrameTransforms;
  //! keyframe vertex data
  SMD2Vert* FrameList; // numFrames*numVertices
  //! bounding boxes for each keyframe
  F3AABBOX* BoxList; // FrameCount
  //! named animations
  SAnimationData* AnimationData;
  int AnimationData_size;
  u32 FrameCount;
  f32 FramesPerSecond;
} CAnimatedMeshMD2;
//! loads an md2 file
bool MFMD2_loadFile(stream_t* file, CAnimatedMeshMD2* mesh) {
  char buf[256];
  int i=0, j, s, ti, count=0;
  int n = stream_read(file, buf, 4*17);
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
  MYREALLOC(mesh->FrameList, numFrames*numVertices);
  // allocate space in vertex arrays

  // allocate interpolation buffer vertices
  MYREALLOC(mesh->InterpolationBuffer->Vertices, count);
  // populate triangles
  MYREALLOC(mesh->InterpolationBuffer->Indices, count);
  for (i = 0; i < count; i += 3) {
    mesh->InterpolationBuffer->Indices[i] = i;
    mesh->InterpolationBuffer->Indices[i+1] = (i + 1);
    mesh->InterpolationBuffer->Indices[i+2] = (i + 2);
  }
  // read texture coordinates
  stream_seek(file, offsetTexcoords, SEEK_SET);
  MYREALLOC(textureCoords, numTexcoords);
  if (!stream_read(file, textureCoords, sizeof(SMD2TextureCoordinate)*numTexcoords)) {
    free(textureCoords);
    llogprintf(ELL_ERROR, "MD2 Loader: Error reading TextureCoords.");
    return false;
  }
  if ('b'==ENDIANNESS) {
    char* ppbuf = (char*)textureCoords;
    j=0;
    for (i = 0; i < numTexcoords; ++i) {
      MEM_GET16L(ppbuf, j, textureCoords[i].s);
      MEM_GET16L(ppbuf, j, textureCoords[i].t);
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
  if ('b'==ENDIANNESS) {
    char* ppbuf = (char*)triangles;
    j=0;
    for (i = 0; i < numTriangles; ++i) {
      MEM_GET16L(ppbuf, j, triangles[i].vertexIndices[0]);
      MEM_GET16L(ppbuf, j, triangles[i].vertexIndices[1]);
      MEM_GET16L(ppbuf, j, triangles[i].vertexIndices[2]);
      MEM_GET16L(ppbuf, j, triangles[i].textureIndices[0]);
      MEM_GET16L(ppbuf, j, triangles[i].textureIndices[1]);
      MEM_GET16L(ppbuf, j, triangles[i].textureIndices[2]);
    }
  }
#if 0
  if (1) {
    int aa[512] = {0}, tt=0;
    for (i = 0; i < 512; ++i) {
      aa[i] = -1;
    }
    for (i=0; i<numTriangles; ++i) {
      for (j=0; j<3; ++j) {
        int id = triangles[i].vertexIndices[j];
        int ic = triangles[i].textureIndices[j];
        if (aa[id]<0) {
          aa[id] = ic;
        } else {
          if (textureCoords[aa[id]].s==textureCoords[ic].s && textureCoords[aa[id]].t==textureCoords[ic].t) {} else {
            printf("%d %d  %d %d\n", textureCoords[aa[id]].s, textureCoords[ic].s,  textureCoords[aa[id]].t, textureCoords[ic].t);
          }
        }
      }
    }
    for (i = 0; i < numTexcoords; ++i) {
      if (aa[textureCoords[i].s]<0) {
        aa[textureCoords[i].s] = textureCoords[i].t;
        ++tt;
      } else {
        printf("%d, %d\n", textureCoords[i].s, textureCoords[i].t);
      }
    }
    tt=tt;
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
      if ('b'==ENDIANNESS) {
        char* ppbuf = (char*)frame;
        int* pibuf = (int*)frame;
        j=0;
        MEM_GET32L(ppbuf, j, pibuf[0]);
        MEM_GET32L(ppbuf, j, pibuf[1]);
        MEM_GET32L(ppbuf, j, pibuf[2]);
        MEM_GET32L(ppbuf, j, pibuf[3]);
        MEM_GET32L(ppbuf, j, pibuf[4]);
        MEM_GET32L(ppbuf, j, pibuf[5]);
      }
      // store frame data
      adata.begin = i;
      adata.end = i;
      adata.fps = 7;
      // Add new named animation if necessary
      if (frame->name[0]) {
        // get animation name
        j=0;
        for (s = 0; s < 16 && frame->name[s] != 0 && (frame->name[s] < '0' || frame->name[s] > '9'); ++s) {
          adata.name[j++] += frame->name[s];
        }
        // Does this keyframe have the same animation name as the current animation?
        if (mesh->AnimationData_size>0 && 0 == strcmp(mesh->AnimationData[mesh->AnimationData_size - 1].name, adata.name)) {
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
          SMD2Vert* v = mesh->FrameList+j*3+ti;
          u32 num = triangles[j].vertexIndices[ti];
          v->Pos[0] = frame->vertices[num].vertex[0];
          v->Pos[2] = frame->vertices[num].vertex[1];
          v->Pos[1] = frame->vertices[num].vertex[2];
          v->NormalIdx = frame->vertices[num].lightNormalIndex;
        }
      }
      // calculate bounding boxes
      if (numVertices) {
        F3AABBOX* box = mesh->BoxList + i;
        float3 pos;
        F3AABBOX_reset(box, 0);
        for (j = 0; j < numTriangles * 3; ++j) {
          pos[0] = (f32)(mesh->FrameList[i*3+j].Pos[0]) * mesh->FrameTransforms[i].scale[0] + mesh->FrameTransforms[i].translate[0];
          pos[1] = (f32)(mesh->FrameList[i*3+j].Pos[1]) * mesh->FrameTransforms[i].scale[1] + mesh->FrameTransforms[i].translate[1];
          pos[2] = (f32)(mesh->FrameList[i*3+j].Pos[2]) * mesh->FrameTransforms[i].scale[2] + mesh->FrameTransforms[i].translate[2];
          F3AABBOX_addInternalPoint(box, pos);
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
        mesh->InterpolationBuffer->Vertices[t * 3 + n].TCoords[0] = (textureCoords[triangles[t].textureIndices[n]].s + 0.5f) * dmaxs;
        mesh->InterpolationBuffer->Vertices[t * 3 + n].TCoords[1] = (textureCoords[triangles[t].textureIndices[n]].t + 0.5f) * dmaxt;
        mesh->InterpolationBuffer->Vertices[t * 3 + n].Color = _ARGB(255, 255, 255, 255);
      }
    }
  }
  // clean up
  FREE(triangles);
  FREE(textureCoords);
  // init buffer with start frame.
  return true;
}


#undef MD2_MAGIC_NUMBER
#undef MD2_VERSION
#undef MD2_MAX_VERTS
