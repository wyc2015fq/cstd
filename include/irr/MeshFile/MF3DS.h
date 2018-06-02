
#define MAXNAME 32
enum e3DSChunk {
  // Primary chunk
  C3DS_MAIN3DS = 0x4D4D,
    // Main Chunks
    C3DS_EDIT3DS = 0x3D3D,
    C3DS_KEYF3DS = 0xB000,
    C3DS_VERSION = 0x0002,
    C3DS_MESHVERSION = 0x3D3E,
    // sub chunks of C3DS_EDIT3DS
    C3DS_EDIT_MATERIAL = 0xAFFF,
    C3DS_EDIT_OBJECT   = 0x4000,
    // sub chunks of C3DS_EDIT_MATERIAL
    C3DS_MATNAME       = 0xA000,
    C3DS_MATAMBIENT    = 0xA010,
    C3DS_MATDIFFUSE    = 0xA020,
    C3DS_MATSPECULAR   = 0xA030,
    C3DS_MATSHININESS  = 0xA040,
    C3DS_MATSHIN2PCT   = 0xA041,
    C3DS_TRANSPARENCY  = 0xA050,
    C3DS_TRANSPARENCY_FALLOFF  = 0xA052,
    C3DS_REFL_BLUR     = 0xA053,
    C3DS_TWO_SIDE      = 0xA081,
    C3DS_WIRE          = 0xA085,
    C3DS_SHADING       = 0xA100,
    C3DS_MATTEXMAP     = 0xA200,
    C3DS_MATSPECMAP    = 0xA204,
    C3DS_MATOPACMAP    = 0xA210,
    C3DS_MATREFLMAP    = 0xA220,
    C3DS_MATBUMPMAP    = 0xA230,
    C3DS_MATMAPFILE    = 0xA300,
    C3DS_MAT_TEXTILING = 0xA351,
    C3DS_MAT_USCALE    = 0xA354,
    C3DS_MAT_VSCALE    = 0xA356,
    C3DS_MAT_UOFFSET   = 0xA358,
    C3DS_MAT_VOFFSET   = 0xA35A,
    // subs of C3DS_EDIT_OBJECT
    C3DS_OBJTRIMESH    = 0x4100,
    // subs of C3DS_OBJTRIMESH
    C3DS_TRIVERT       = 0x4110,
    C3DS_POINTFLAGARRAY = 0x4111,
    C3DS_TRIFACE       = 0x4120,
    C3DS_TRIFACEMAT    = 0x4130,
    C3DS_TRIUV         = 0x4140,
    C3DS_TRISMOOTH     = 0x4150,
    C3DS_TRIMATRIX     = 0x4160,
    C3DS_MESHCOLOR     = 0x4165,
    C3DS_DIRECT_LIGHT  = 0x4600,
    C3DS_DL_INNER_RANGE = 0x4659,
    C3DS_DL_OUTER_RANGE = 0x465A,
    C3DS_DL_MULTIPLIER = 0x465B,
    C3DS_CAMERA        = 0x4700,
    C3DS_CAM_SEE_CONE  = 0x4710,
    C3DS_CAM_RANGES    = 0x4720,
    // subs of C3DS_KEYF3DS
    C3DS_KF_HDR        = 0xB00A,
    C3DS_AMBIENT_TAG   = 0xB001,
    C3DS_OBJECT_TAG    = 0xB002,
    C3DS_CAMERA_TAG    = 0xB003,
    C3DS_TARGET_TAG    = 0xB004,
    C3DS_LIGHTNODE_TAG = 0xB005,
    C3DS_KF_SEG        = 0xB008,
    C3DS_KF_CURTIME    = 0xB009,
    C3DS_KF_NODE_HDR   = 0xB010,
    C3DS_PIVOTPOINT    = 0xB013,
    C3DS_BOUNDBOX      = 0xB014,
    C3DS_MORPH_SMOOTH  = 0xB015,
    C3DS_POS_TRACK_TAG = 0xB020,
    C3DS_ROT_TRACK_TAG = 0xB021,
    C3DS_SCL_TRACK_TAG = 0xB022,
    C3DS_NODE_ID       = 0xB030,
    // Viewport definitions
    C3DS_VIEWPORT_LAYOUT = 0x7001,
    C3DS_VIEWPORT_DATA   = 0x7011,
    C3DS_VIEWPORT_DATA_3 = 0x7012,
    C3DS_VIEWPORT_SIZE   = 0x7020,
    // different color chunk types
    C3DS_COL_RGB    = 0x0010,
    C3DS_COL_TRU    = 0x0011,
    C3DS_COL_LIN_24 = 0x0012,
    C3DS_COL_LIN_F  = 0x0013,
    // percentage chunk types
    C3DS_PERCENTAGE_I = 0x0030,
    C3DS_PERCENTAGE_F = 0x0031,
    C3DS_CHUNK_MAX   = 0xFFFF
};

// byte-align structures
//#include "irrpack.h"
typedef struct ChunkHeader {
  u16 id;
  s32 length;
} ChunkHeader;
// Default alignment
//#include "irrunpack.h"
typedef struct ChunkData {
  ChunkHeader header;
  s32 read;
} ChunkData;
typedef struct SCurrentMaterial {
  SMaterial Material;
  char* Name;
  char* Filename[5];
  f32 Strength[5];
} SCurrentMaterial;
typedef struct SMaterialGroup {
  char MaterialName[MAXNAME];
  u16 faceCount;
  u16* faces;
} SMaterialGroup;
typedef struct C3DSMeshFileLoader {
  ISceneManager* SceneManager;
  IFileSystem* FileSystem;
  f32* Vertices;
  u16* Indices;
  u32* SmoothingGroups;
  u16* TempIndices;
  f32* TCoords;
  u16 CountVertices;
  u16 CountFaces; // = CountIndices/4
  u16 CountTCoords;
  SMaterialGroup* MaterialGroups;
  int MaterialGroups_size;
  SCurrentMaterial* Materials;
  int Materials_size;
  char* MeshBufferNames;
  float16 TransformationMatrix;
  IMesh* Mesh;
} C3DSMeshFileLoader;

void MF3DS_readChunkData(stream_t* file, ChunkData* data)
{
  char buf[6];
  int i=0;
  stream_read(file, buf, 6);
  MEM_GET16L(buf, i, data->header.id);
  MEM_GET32L(buf, i, data->header.length);
  data->read += 6;
}
bool MF3DS_readPercentageChunk(stream_t* file, ChunkData* chunk, f32* percentage)
{
  int i;
  //llogprintf("Load percentage chunk.", ELL_DEBUG);
  ChunkData data[1] = {0};
  short intpercentage;
  float fpercentage;
  
  MF3DS_readChunkData(file, data);
  switch (data->header.id) {
  case C3DS_PERCENTAGE_I:
    // read short
    stream_read(file, &intpercentage, 2), i=0;
    BYTEORDER16L(intpercentage);
    *percentage = intpercentage / 100.0f;
    data->read += 2;
    break;
  case C3DS_PERCENTAGE_F:
    // read float
    stream_read(file,&fpercentage, sizeof(float));
    data->read += sizeof(float);
    BYTEORDER32L(fpercentage);
    *percentage = (f32)fpercentage;
    break;
  default:
    // unknown percentage chunk
    llogprintf(ELL_WARNING, "Unknown percentage chunk in 3Ds file.");
    stream_seek(file, data->header.length - data->read, SEEK_CUR);
    data->read += data->header.length - data->read;
    break;
  }
  chunk->read += data->read;
  return true;
}
bool MF3DS_readColorChunk(stream_t* file, ChunkData* chunk, SColor* out)
{
  //llogprintf(ELL_DEBUG, "Load color chunk.");
  ChunkData data[1] = {0};
  u8 c[3];
  f32 cf[3];
  MF3DS_readChunkData(file, data);
  switch (data->header.id) {
  case C3DS_COL_TRU:
  case C3DS_COL_LIN_24:
    // read 8 bit data
    stream_read(file, c, sizeof(c));
    *out = _ARGB(255, c[0], c[1], c[2]);
    data->read += sizeof(c);
    break;
  case C3DS_COL_RGB:
  case C3DS_COL_LIN_F: {
    // read float data
    stream_read(file,cf, sizeof(cf));
    if ('b'==ENDIANNESS) {
      BYTESWAP_32(cf[0]);
      BYTESWAP_32(cf[1]);
      BYTESWAP_32(cf[2]);
    }
    *out = _ARGB(255, (s32)(cf[0] * 255.0f), (s32)(cf[1] * 255.0f), (s32)(cf[2] * 255.0f));
    data->read += sizeof(cf);
  }
  break;
  default:
    // unknown color chunk size
    llogprintf(ELL_WARNING, "Unknown size of color chunk in 3Ds file.");
    stream_seek(file,data->header.length - data->read, SEEK_CUR);
    data->read += data->header.length - data->read;
    break;
  }
  chunk->read += data->read;
  return true;
}
bool MF3DS_readMaterialChunk(stream_t* file, ChunkData* parent, C3DSMeshFileLoader* s) {
  // llogprintf("Load material chunk.", ELL_DEBUG);
  u16 matSection = 0;
  f32 percentage;
  s16 flags;
  s16 testval;
  long pos;
  SCurrentMaterial CurrentMaterial = {0};
  while (parent->read < parent->header.length) {
    ChunkData data[1] = {0};
    MF3DS_readChunkData(file, data);
    switch (data->header.id) {
    case C3DS_MATNAME: {
      c8* c = MALLOC(c8, data->header.length - data->read);
      stream_read(file, c, data->header.length - data->read);
      if (cstr_len(c)) {
        CurrentMaterial.Name = c;
      }
      data->read += data->header.length - data->read;
      FREE(c);
    }
    break;
    case C3DS_MATAMBIENT:
      MF3DS_readColorChunk(file, data, &CurrentMaterial.Material.AmbientColor);
      break;
    case C3DS_MATDIFFUSE:
      MF3DS_readColorChunk(file, data, &CurrentMaterial.Material.DiffuseColor);
      break;
    case C3DS_MATSPECULAR:
      MF3DS_readColorChunk(file, data, &CurrentMaterial.Material.SpecularColor);
      break;
    case C3DS_MATSHININESS:
      MF3DS_readPercentageChunk(file, data, &CurrentMaterial.Material.Shininess);
      CurrentMaterial.Material.Shininess = (1.f - CurrentMaterial.Material.Shininess) * 128.f;
      break;
    case C3DS_TRANSPARENCY:
      MF3DS_readPercentageChunk(file, data, &percentage);
      if (percentage > 0.0f) {
        CurrentMaterial.Material.MaterialTypeParam = percentage;
        CurrentMaterial.Material.MaterialType = EMT_TRANSPARENT_VERTEX_ALPHA;
      }
      else {
        CurrentMaterial.Material.MaterialType = EMT_SOLID;
      }
      break;
    case C3DS_WIRE:
      CurrentMaterial.Material.Wireframe = true;
      break;
    case C3DS_TWO_SIDE:
      CurrentMaterial.Material.BackfaceCulling = false;
      break;
    case C3DS_SHADING:
      stream_read(file,&flags, 2);
      BYTEORDER16L(flags);
      switch (flags) {
      case 0:
        CurrentMaterial.Material.Wireframe = true;
        break;
      case 1:
        CurrentMaterial.Material.Wireframe = false;
        CurrentMaterial.Material.GouraudShading = false;
        break;
      case 2:
        CurrentMaterial.Material.Wireframe = false;
        CurrentMaterial.Material.GouraudShading = true;
        break;
      default:
        // phong and metal missing
        break;
      }
      data->read += data->header.length - data->read;
      break;
    case C3DS_MATTEXMAP:
    case C3DS_MATSPECMAP:
    case C3DS_MATOPACMAP:
    case C3DS_MATREFLMAP:
    case C3DS_MATBUMPMAP: {
      matSection = data->header.id;
      // Should contain a percentage chunk, but does not always have it
      pos = stream_tell(file);
      stream_read(file,&testval, 2);
      BYTEORDER16L(testval);
      stream_seek(file,pos, SEEK_SET);
      if ((testval == C3DS_PERCENTAGE_I) || (testval == C3DS_PERCENTAGE_F)) {
        switch (matSection) {
        case C3DS_MATTEXMAP:
          MF3DS_readPercentageChunk(file, data, &CurrentMaterial.Strength[0]);
          break;
        case C3DS_MATSPECMAP:
          MF3DS_readPercentageChunk(file, data, &CurrentMaterial.Strength[1]);
          break;
        case C3DS_MATOPACMAP:
          MF3DS_readPercentageChunk(file, data, &CurrentMaterial.Strength[2]);
          break;
        case C3DS_MATBUMPMAP:
          MF3DS_readPercentageChunk(file, data, &CurrentMaterial.Strength[4]);
          break;
        }
      }
    }
    break;
    case C3DS_MATMAPFILE: {
      // read texture file name
      c8* c = MALLOC(c8, data->header.length - data->read);
      stream_read(file,c, data->header.length - data->read);
      switch (matSection) {
      case C3DS_MATTEXMAP:
        CurrentMaterial.Filename[0] = c;
        break;
      case C3DS_MATSPECMAP:
        CurrentMaterial.Filename[1] = c;
        break;
      case C3DS_MATOPACMAP:
        CurrentMaterial.Filename[2] = c;
        break;
      case C3DS_MATREFLMAP:
        CurrentMaterial.Filename[3] = c;
        break;
      case C3DS_MATBUMPMAP:
        CurrentMaterial.Filename[4] = c;
        break;
      }
      data->read += data->header.length - data->read;
      FREE(c);
    }
    break;
    case C3DS_MAT_TEXTILING: {
      s16 flags;
      stream_read(file,&flags, 2);
      if ('b'==ENDIANNESS) {
        BYTESWAP_16(flags);
      }
      data->read += 2;
    }
    break;
    case C3DS_MAT_USCALE:
    case C3DS_MAT_VSCALE:
    case C3DS_MAT_UOFFSET:
    case C3DS_MAT_VOFFSET: {
      f32 value;
      u32 i = 0, j = 0, k = 0;

      stream_read(file,&value, 4);
      BYTEORDER32L(value);
      if (matSection != C3DS_MATTEXMAP) {
        i = 1;
      }
      if (data->header.id == C3DS_MAT_VSCALE) {
        j = 1;
        k = 1;
      }
      else if (data->header.id == C3DS_MAT_UOFFSET) {
        j = 2;
        k = 0;
      }
      else if (data->header.id == C3DS_MAT_VOFFSET) {
        j = 2;
        k = 1;
      }
      CurrentMaterial.Material.TextureLayer[i].TextureMatrix[j*4+k] = value;
      data->read += 4;
    }
    break;
    default:
      // ignore chunk
      stream_seek(file,data->header.length - data->read, SEEK_CUR);
      data->read += data->header.length - data->read;
    }
    parent->read += data->read;
  }
  s->Materials[s->Materials_size++] = CurrentMaterial;
  return true;
}
bool MF3DS_readTrackChunk(stream_t* file, ChunkData* data, IMeshBuffer* mb, const float* pivot)
{
  //llogprintf("Load track chunk.", ELL_DEBUG);
  char buf[20];
  int i=0;
  u16 flags;
  u32 flags2;
  f32 angle = 0.0f;
  float3 vec;
  // Track flags
  stream_read(file,buf, 20);
  MEM_GET16L(buf, i, flags);
  MEM_GET32L(buf, i, flags2);
  MEM_GET32L(buf, i, flags2);
  MEM_GET32L(buf, i, flags2);
  MEM_GET32L(buf, i, flags2); // Num keys
  MEM_GET16L(buf, i, flags); // TCB flags
  data->read += 20;
  if (data->header.id == C3DS_ROT_TRACK_TAG) {
    // Angle
    stream_read(file, &angle, sizeof(f32));
    BYTEORDER32L(angle);
    data->read += sizeof(f32);
  }
  stream_read(file,vec, sizeof(f32)*3);
  if ('b'==ENDIANNESS) {
    BYTESWAP_32(vec[0]);
    BYTESWAP_32(vec[1]);
    BYTESWAP_32(vec[2]);
  }
  data->read += 12;
  VSUB3(vec, vec, pivot);
  // apply transformation to mesh buffer
  if (false) { //mb
    S3DVertex* vertices = mb->Vertices;
    if (data->header.id == C3DS_POS_TRACK_TAG) {
      int i;
      for (i = 0; i < mb->Vertices_size; ++i) {
        VADD3(vertices[i].Pos, vertices[i].Pos, vec);
      }
    }
    else if (data->header.id == C3DS_ROT_TRACK_TAG) {
      //TODO
    }
    else if (data->header.id == C3DS_SCL_TRACK_TAG) {
      //TODO
    }
  }
  // skip further frames
  stream_seek(file,data->header.length - data->read, SEEK_CUR);
  data->read += data->header.length - data->read;
  return true;
}
void MF3DS_readString(stream_t* file, ChunkData* data, char* out, int maxlen)
{
  c8 c = 1;
  int i, out_size=0;
  --maxlen;
  for (i=0; stream_read(file, &c, sizeof(c8)) && c; ++out_size) {
    if (i<maxlen) {
      out[i++] = c;
    }
  }
  out[i] = 0;
  data->read += out_size + 1;
}
bool MF3DS_readFrameChunk(stream_t* file, ChunkData* parent, C3DSMeshFileLoader* s)
{
  //llogprintf(ELL_DEBUG, "Load frame chunk.");
  ChunkData data[1] = {0};
  char name[256];
  IMeshBuffer* mb = 0;
  float3 pivot, bboxCenter;
  //KF_HDR is always at the beginning
  MF3DS_readChunkData(file, data);
  if (data->header.id != C3DS_KF_HDR) {
    return false;
  }
  else {
    u16 version;
    u32 flags;
    //llogprintf(ELL_DEBUG, "Load keyframe header.");
    stream_read(file,&version, 2);
    BYTEORDER16L(version);
    MF3DS_readString(file, data, name, 256);
    stream_read(file,&flags, 4);
    BYTEORDER32L(flags);
    data->read += 4;
    parent->read += data->read;
  }
  data->read = 0;
  while (parent->read < parent->header.length) {
    MF3DS_readChunkData(file, data);
    switch (data->header.id) {
    case C3DS_OBJECT_TAG: {
      //llogprintf(ELL_DEBUG, "Load object tag.");
      mb = 0;
      VSET3(pivot, 0.0f, 0.0f, 0.0f);
    }
    break;
    case C3DS_KF_SEG: {
      u32 flags;
      //llogprintf(ELL_DEBUG, "Load keyframe segment.");
      stream_read(file,&flags, 4);
      BYTEORDER32L(flags);
      stream_read(file,&flags, 4);
      BYTEORDER32L(flags);
      data->read += 8;
    }
    break;
    case C3DS_KF_NODE_HDR: {
      s16 flags;
      int i, n = data->header.length - data->read - 6;
      c8* c = MALLOC(c8, n+1);
      c[n] = 0;
      llogprintf(ELL_DEBUG, "Load keyframe node header.");
      stream_read(file, c, n);
      // search mesh buffer to apply these transformations to
      i = cstr_splitfind(s->MeshBufferNames, -1, c, -1, 0, 0);
      if (i>=0) {
        mb = s->Mesh->MeshBuffers[i];
        break;
      }
      
      stream_read(file, &flags, 2);
      BYTEORDER16L(flags);
      stream_read(file,&flags, 2);
      BYTEORDER16L(flags);
      stream_read(file,&flags, 2);
      BYTEORDER16L(flags);
      data->read += data->header.length - data->read;
      FREE(c);
    }
    break;
    case C3DS_KF_CURTIME: {
      u32 flags;
      //llogprintf(ELL_DEBUG, "Load keyframe current time.");
      stream_read(file,&flags, 4);
      BYTEORDER32L(flags);
      data->read += 4;
    }
    break;
    case C3DS_NODE_ID: {
      u16 flags;
      //llogprintf(ELL_DEBUG, "Load node ID.");
      stream_read(file,&flags, 2);
      BYTEORDER16L(flags);
      data->read += 2;
    }
    break;
    case C3DS_PIVOTPOINT: {
      llogprintf(ELL_DEBUG, "Load pivot point.");
      stream_read(file, pivot, sizeof(f32)*3);
      if ('b'==ENDIANNESS) {
        BYTESWAP_32(pivot[0]);
        BYTESWAP_32(pivot[1]);
        BYTESWAP_32(pivot[2]);
      }
      data->read += 12;
    }
    break;
    case C3DS_BOUNDBOX: {
      F3AABBOX bbox[1] = {0};
      //llogprintf("ELL_DEBUG, Load bounding box.");
      // abuse bboxCenter as temporary variable
      stream_read(file, bboxCenter, sizeof(f32)*3);
      if ('b'==ENDIANNESS) {
        BYTESWAP_32(bboxCenter[0]);
        BYTESWAP_32(bboxCenter[1]);
        BYTESWAP_32(bboxCenter[2]);
      }
      F3AABBOX_reset(bbox, bboxCenter);
      stream_read(file,bboxCenter, sizeof(f32)*3);
      if ('b'==ENDIANNESS) {
        BYTESWAP_32(bboxCenter[0]);
        BYTESWAP_32(bboxCenter[1]);
        BYTESWAP_32(bboxCenter[2]);
      }
      F3AABBOX_addInternalPoint(bbox, bboxCenter);
      F3AABBOX_getCenter(bbox, bboxCenter);
      data->read += 24;
    }
    break;
    case C3DS_MORPH_SMOOTH: {
      f32 flag;
      //llogprintf(ELL_DEBUG, "Load morph smooth.");
      stream_read(file, &flag, 4);
      if ('b'==ENDIANNESS) {
        BYTESWAP_32(flag);
      }
      data->read += 4;
    }
    break;
    case C3DS_POS_TRACK_TAG:
    case C3DS_ROT_TRACK_TAG:
    case C3DS_SCL_TRACK_TAG:
      {
        float3 v;
        VSUB3(v, bboxCenter, pivot);
        MF3DS_readTrackChunk(file, data, mb, v);
      }
      break;
    default:
      // ignore chunk
      stream_seek(file,data->header.length - data->read, SEEK_CUR);
      data->read += data->header.length - data->read;
      break;
    }
    parent->read += data->read;
    data->read = 0;
  }
  return true;
}
void MF3DS_readVertices(stream_t* file, ChunkData* data, C3DSMeshFileLoader* s)
{
  int i, vertexBufferByteSize;
  llogprintf(ELL_DEBUG, "Load vertices.");
  stream_read(file,&s->CountVertices, sizeof(s->CountVertices));
  if ('b'==ENDIANNESS) {
    BYTESWAP_16(s->CountVertices);
  }
  data->read += sizeof(s->CountVertices);
  vertexBufferByteSize = s->CountVertices * sizeof(f32) * 3;
  if (data->header.length - data->read != vertexBufferByteSize) {
    llogprintf(ELL_ERROR, "Invalid size of vertices found in 3ds file %d", (s->CountVertices));
    return;
  }
  MYREALLOC(s->Vertices, s->CountVertices * 3);
  stream_read(file, s->Vertices, vertexBufferByteSize);
  if ('b'==ENDIANNESS) {
    for (i = 0; i < s->CountVertices * 3; i++) {
      BYTESWAP_32(s->Vertices[i]);
    }
  }
  data->read += vertexBufferByteSize;
}
void MF3DS_readIndices(stream_t* file, ChunkData* data, C3DSMeshFileLoader* s)
{
  int i, indexBufferByteSize;
  llogprintf(ELL_DEBUG, "Load indices.");
  stream_read(file, &s->CountFaces, sizeof(s->CountFaces));
  if ('b'==ENDIANNESS) {
    BYTESWAP_16(s->CountFaces);
  }
  data->read += sizeof(s->CountFaces);
  indexBufferByteSize = s->CountFaces * sizeof(u16) * 4;
  // Indices are u16s.
  // After every 3 Indices in the array, there follows an edge flag.
  MYREALLOC(s->Indices, s->CountFaces * 4);
  stream_read(file, s->Indices, indexBufferByteSize);
  if ('b'==ENDIANNESS) {
    for (i = 0; i < s->CountFaces * 4; ++i) {
      BYTESWAP_16(s->Indices[i]);
    }
  }
  data->read += indexBufferByteSize;
}
void MF3DS_readMaterialGroup(stream_t* file, ChunkData* data, C3DSMeshFileLoader* s)
{
  int i;
  SMaterialGroup* group = s->MaterialGroups + s->MaterialGroups_size;
  //llogprintf(ELL_DEBUG, "Load material group.");
  MF3DS_readString(file, data, group->MaterialName, MAXNAME);
  stream_read(file,&group->faceCount, sizeof(group->faceCount));
  if ('b'==ENDIANNESS) {
    BYTESWAP_16(group->faceCount);
  }
  data->read += sizeof(group->faceCount);
  // read faces
  MYREALLOC(group->faces, group->faceCount);
  stream_read(file, group->faces, sizeof(u16) * group->faceCount);
  if ('b'==ENDIANNESS) {
    for (i = 0; i < group->faceCount; ++i) {
      BYTESWAP_16(group->faces[i]);
    }
  }
  data->read += sizeof(u16) * group->faceCount;
}
void MF3DS_readTextureCoords(stream_t* file, ChunkData* data, C3DSMeshFileLoader* s)
{
  int i, tcoordsBufferByteSize;
  llogprintf(ELL_DEBUG, "Load texture coords.");
  stream_read(file,&s->CountTCoords, sizeof(s->CountTCoords));
  if ('b'==ENDIANNESS) {
    BYTESWAP_16(s->CountTCoords);
  }
  data->read += sizeof(s->CountTCoords);
  tcoordsBufferByteSize = s->CountTCoords * sizeof(f32) * 2;
  if (data->header.length - data->read != tcoordsBufferByteSize) {
    llogprintf(ELL_WARNING, "Invalid size of tcoords found in 3ds file.");
    return;
  }
  MYREALLOC(s->TCoords, s->CountTCoords * 3);
  stream_read(file, s->TCoords, tcoordsBufferByteSize);
  if ('b'==ENDIANNESS) {
    for (i = 0; i < s->CountTCoords * 2; i++) {
      BYTESWAP_32(s->TCoords[i]);
    }
  }
  data->read += tcoordsBufferByteSize;
}
bool MF3DS_readObjectChunk(stream_t* file, ChunkData* parent, C3DSMeshFileLoader* s)
{
  int i, j;
  //llogprintf(ELL_DEBUG, "Load object chunk.");
  while (parent->read < parent->header.length) {
    ChunkData data[1] = {0};
    MF3DS_readChunkData(file, data);
    switch (data->header.id) {
    case C3DS_OBJTRIMESH:
      MF3DS_readObjectChunk(file, data, s);
      break;
    case C3DS_TRIVERT:
      MF3DS_readVertices(file, data, s);
      break;
    case C3DS_POINTFLAGARRAY:
      {
        u16 numVertex, flags;
        stream_read(file,&numVertex, sizeof(u16));
        if ('b'==ENDIANNESS) {
          BYTESWAP_16(numVertex);
        }
        for (i = 0; i < numVertex; ++i) {
          stream_read(file,&flags, sizeof(u16));
          if ('b'==ENDIANNESS) {
            BYTESWAP_16(flags);
          }
        }
        data->read += (numVertex + 1) * sizeof(u16);
      }
      break;
    case C3DS_TRIFACE:
      MF3DS_readIndices(file, data, s);
      MF3DS_readObjectChunk(file, data, s); // read smooth and material groups
      break;
    case C3DS_TRIFACEMAT:
      MF3DS_readMaterialGroup(file, data, s);
      break;
    case C3DS_TRIUV: // getting texture coordinates
      MF3DS_readTextureCoords(file, data, s);
      break;
    case C3DS_TRIMATRIX: {
      f32 mat[12];
      int i;
      stream_read(file, mat, 12 * sizeof(f32));
      mmakeIdentity(s->TransformationMatrix);
      for (i = 0; i < 4; ++i) {
        for (j = 0; j < 3; ++j) {
          if ('b'==ENDIANNESS) {
            BYTESWAP_32(mat[i*3+j]);
          }
          s->TransformationMatrix[i*4+j] = mat[i*3+j];
        }
      }
      data->read += 12 * sizeof(f32);
                         }
      break;
    case C3DS_MESHCOLOR: {
      u8 flag;
      stream_read(file,&flag, sizeof(u8));
      ++data->read;
                         }
      break;
    case C3DS_TRISMOOTH: { // TODO
      MYREALLOC(s->SmoothingGroups, s->CountFaces);
      stream_read(file, s->SmoothingGroups, s->CountFaces * sizeof(u32));
      if ('b'==ENDIANNESS) {
        for (i = 0; i < s->CountFaces; ++i) {
          BYTESWAP_32(s->SmoothingGroups[i]);
        }
      }
      data->read += s->CountFaces * sizeof(u32);
                         }
      break;
    default:
      // ignore chunk
      stream_seek(file,data->header.length - data->read, SEEK_CUR);
      data->read += data->header.length - data->read;
      break;
    }
    parent->read += data->read;
  }
  return true;
}
void MF3DS_cleanUp(C3DSMeshFileLoader* s)
{
  FREE(s->Vertices);
  s->CountVertices = 0;
  s->Vertices = 0;
  FREE(s->Indices);
  s->Indices = 0;
  s->CountFaces = 0;
  FREE(s->SmoothingGroups);
  s->SmoothingGroups = 0;
  FREE(s->TCoords) ;
  s->TCoords = 0;
  s->CountTCoords = 0;
  FREE(s->MaterialGroups);
}
typedef struct mem_info_t {
  int used;
  int size;
} mem_info_t;
#define mem_push(s, m, pn, p)  s = mem_delins(s, m, 0, 0, 0, m->used, pn, p)
CV_INLINE void* mem_delins(void* s, mem_info_t* m, int j, int dn, void* d, int i, int pn, const void* p) {
  char* pc = (char*)s;
  if (dn>0) {
    if (d) {
      memcpy(d, pc+j, dn);
    }
    memmove(pc + j, pc+j+dn, m->used-j-dn);
  }
  if (pn>0) {
    int newsize = pn+m->used;
    if (newsize>m->size) {
      newsize = next_power_1_5(newsize);
      MYREALLOC(pc, newsize);
      if (m->used>i) {
        memmove(pc+i+pn, pc+i, m->used-i);
      }
      if (p) {
        memcpy(pc+i, p, pn);
      } else {
        memset(pc+i, 0, pn);
      }
      m->size = newsize;
    }
  }
  return pc;
}
void MF3DS_loadMaterials(stream_t* file, C3DSMeshFileLoader* s)
{
  int i;
  mem_info_t mem[1] = {0};
  // create a mesh buffer for every material
  if (0==s->Materials_size) {
    llogprintf(ELL_INFORMATION, "No materials found in 3ds file.");
  }
  MYREALLOC(s->Mesh->MeshBuffers, s->Materials_size);
  for (i = 0; i < s->Materials_size; ++i) {
    IMeshBuffer* m = MALLOC(IMeshBuffer, 1);
    char* name = NULL;
    mem_push(s->MeshBufferNames, mem, 1, "|");
    s->Mesh->MeshBuffers[i] = (m);
#if 0
    m->Material = s->Materials[i].Material;
    if (cstr_len(name = s->Materials[i].Filename[0])>0) {
      ITexture* texture = 0;
      if (imread(name)) {
        texture = SceneManager->getVideoDriver()->getTexture(name);
      }
      if (!texture) {
        const char* fname = FileSystem->getFileDir(modelFilename) + "/" + FileSystem->getFileBasename(name);
        if (FileSystem->existFile(fname)) {
          texture = SceneManager->getVideoDriver()->getTexture(fname);
        }
      }
      if (!texture)
        llogprintf(ELL_WARNING, "Could not load a texture for entry in 3ds file %s", name);
      else {
        m->getMaterial().setTexture(0, texture);
      }
    }
    if (Materials[i].Filename[2]_size) {
      ITexture* texture = 0;
      if (FileSystem->existFile(Materials[i].Filename[2])) {
        texture = SceneManager->getVideoDriver()->getTexture(Materials[i].Filename[2]);
      }
      if (!texture) {
        const char* fname = FileSystem->getFileDir(modelFilename) + "/" + FileSystem->getFileBasename(Materials[i].Filename[2]);
        if (FileSystem->existFile(fname)) {
          texture = SceneManager->getVideoDriver()->getTexture(fname);
        }
      }
      if (!texture) {
        llogprintf("Could not load a texture for entry in 3ds file",
            Materials[i].Filename[2], ELL_WARNING);
      }
      else {
        m->getMaterial().setTexture(0, texture);
        m->getMaterial().MaterialType = EMT_TRANSPARENT_ADD_COLOR;
      }
    }
    if (Materials[i].Filename[3]_size) {
      ITexture* texture = 0;
      if (FileSystem->existFile(Materials[i].Filename[3])) {
        texture = SceneManager->getVideoDriver()->getTexture(Materials[i].Filename[3]);
      }
      if (!texture) {
        const char* fname = FileSystem->getFileDir(modelFilename) + "/" + FileSystem->getFileBasename(Materials[i].Filename[3]);
        if (FileSystem->existFile(fname)) {
          texture = SceneManager->getVideoDriver()->getTexture(fname);
        }
      }
      if (!texture) {
        llogprintf("Could not load a texture for entry in 3ds file",
            Materials[i].Filename[3], ELL_WARNING);
      }
      else {
        m->getMaterial().setTexture(1, m->getMaterial().getTexture(0));
        m->getMaterial().setTexture(0, texture);
        m->getMaterial().MaterialType = EMT_REFLECTION_2_LAYER;
      }
    }
    if (Materials[i].Filename[4]_size) {
      ITexture* texture = 0;
      if (FileSystem->existFile(Materials[i].Filename[4])) {
        texture = SceneManager->getVideoDriver()->getTexture(Materials[i].Filename[4]);
      }
      if (!texture) {
        const char* fname = FileSystem->getFileDir(modelFilename) + "/" + FileSystem->getFileBasename(Materials[i].Filename[4]);
        if (FileSystem->existFile(fname)) {
          texture = SceneManager->getVideoDriver()->getTexture(fname);
        }
      }
      if (!texture)
        llogprintf("Could not load a texture for entry in 3ds file",
            Materials[i].Filename[4], ELL_WARNING);
      else {
        m->getMaterial().setTexture(1, texture);
        SceneManager->getVideoDriver()->makeNormalMapTexture(texture, Materials[i].Strength[4] * 10.f);
        m->getMaterial().MaterialType = EMT_PARALLAX_MAP_SOLID;
        m->getMaterial().MaterialTypeParam = .035f;
      }
    }
#endif
  }
}
void MF3DS_composeObject(stream_t* file, const char* name, C3DSMeshFileLoader* s)
{
#if 0
  int i;
  // llogprintf(ELL_DEBUG, "Compose object.");
  if (s->Mesh->MeshBuffers_size != s->Materials_size) {
    MF3DS_loadMaterials(file, s);
  }
  if (0==s->MaterialGroups_size) {
    // no material group, so add all
    SMaterialGroup group;
    group.faceCount = s->CountFaces;
    group.faces = MALLOC(u16, group.faceCount);
    for (u16 i = 0; i < group.faceCount; ++i) {
      group.faces[i] = i;
    }
    MaterialGroups.push_back(group);
    // if we've got no material, add one without a texture
    if (Materials.empty()) {
      SCurrentMaterial m;
      Materials.push_back(m);
      SMeshBuffer* mb = new SMeshBuffer();
      Mesh->addMeshBuffer(mb);
      mb->getMaterial() = Materials[0].Material;
      mb->drop();
      // add an empty mesh buffer name
      MeshBufferNames.push_back("");
    }
  }
  for (i = 0; i < s->MaterialGroups_size; ++i) {
    SMeshBuffer* mb = 0;
    SMaterial* mat = 0;
    u32 mbPos;
    // -3 because we add three vertices at once
    u32 maxPrimitives = min_(SceneManager->getVideoDriver()->getMaximalPrimitiveCount(), (u32)((1 << 16) - 1)) - 3; // currently hardcoded s16 max value for index pointers
    // find mesh buffer for this group
    for (mbPos = 0; mbPos < Materials_size; ++mbPos) {
      if (MaterialGroups[i].MaterialName == Materials[mbPos].Name) {
        mb = (SMeshBuffer*)Mesh->getMeshBuffer(mbPos);
        mat = &Materials[mbPos].Material;
        MeshBufferNames[mbPos] = name;
        break;
      }
    }
    if (mb != 0) {
      // add geometry to the buffer.
      S3DVertex vtx;
      vector3df vec;
      vtx.Color = mat->DiffuseColor;
      if (mat->MaterialType == EMT_TRANSPARENT_VERTEX_ALPHA) {
        vtx.Color.setAlpha((int)(255.0f * mat->MaterialTypeParam));
      }
      vtx.Normal.set(0, 0, 0);
      for (s32 f = 0; f < MaterialGroups[i].faceCount; ++f) {
        u32 vtxCount = mb->Vertices_size;
        if (vtxCount > maxPrimitives) {
          IMeshBuffer* tmp = mb;
          mb = new SMeshBuffer();
          Mesh->addMeshBuffer(mb);
          mb->drop();
          Mesh->MeshBuffers[mbPos] = Mesh->MeshBuffers.getLast();
          Mesh->MeshBuffers[Mesh->MeshBuffers_size - 1] = tmp;
          mb->getMaterial() = tmp->getMaterial();
          vtxCount = 0;
        }
        for (s32 v = 0; v < 3; ++v) {
          s32 idx = Indices[MaterialGroups[i].faces[f] * 4 + v];
          if (CountVertices > idx) {
            vtx.Pos[0] = Vertices[idx * 3 + 0];
            vtx.Pos[2] = Vertices[idx * 3 + 1];
            vtx.Pos[1] = Vertices[idx * 3 + 2];
            //            TransformationMatrix.transformVect(vtx.Pos);
          }
          if (CountTCoords > idx) {
            vtx.TCoords[0] = TCoords[idx * 2 + 0];
            vtx.TCoords[1] = 1.0f - TCoords[idx * 2 + 1];
          }
          mb->Vertices.push_back(vtx);
        }
        // compute normal
        plane3d<f32> pl(mb->Vertices[vtxCount].Pos, mb->Vertices[vtxCount + 2].Pos,
          mb->Vertices[vtxCount + 1].Pos);
        mb->Vertices[vtxCount].Normal = pl.Normal;
        mb->Vertices[vtxCount + 1].Normal = pl.Normal;
        mb->Vertices[vtxCount + 2].Normal = pl.Normal;
        // add indices
        mb->Indices.push_back(vtxCount);
        mb->Indices.push_back(vtxCount + 2);
        mb->Indices.push_back(vtxCount + 1);
      }
    }
    else {
      llogprintf("Found no matching material for Group in 3ds file.", ELL_WARNING);
    }
  }
#endif
  MF3DS_cleanUp(s);
}
bool MF3DS_readChunk(stream_t* file, ChunkData* parent, C3DSMeshFileLoader* s)
{
  while (parent->read < parent->header.length) {
    ChunkData data[1] = {0};
    MF3DS_readChunkData(file, data);
    switch (data->header.id) {
    case C3DS_VERSION: {
      u16 version;
      stream_read(file,&version, sizeof(u16));
      if ('b'==ENDIANNESS) {
        BYTESWAP_16(version);
      }
      stream_seek(file,data->header.length - data->read - 2, SEEK_CUR);
      data->read += data->header.length - data->read;
      if (version != 0x03) {
        llogprintf(ELL_ERROR, "3ds file version is other than 3.");
      }
    }
    break;
    case C3DS_EDIT_MATERIAL:
      MF3DS_readMaterialChunk(file, data, s);
      break;
    case C3DS_KEYF3DS:
      MF3DS_readFrameChunk(file, data, s);
      break;
    case C3DS_EDIT3DS:
      break;
    case C3DS_MESHVERSION:
    case 0x01: {
      u32 version;
      stream_read(file,&version, sizeof(u32));
      if ('b'==ENDIANNESS) {
        BYTESWAP_32(version);
      }
      data->read += sizeof(u32);
    }
    break;
    case C3DS_EDIT_OBJECT: {
      char name[256];
      MF3DS_readString(file, data, name, 256);
      MF3DS_readObjectChunk(file, data, s);
      MF3DS_composeObject(file, name, s);
    }
    break;
    default:
      // ignore chunk
      stream_seek(file,data->header.length - data->read, SEEK_CUR);
      data->read += data->header.length - data->read;
      break;
    }
    parent->read += data->read;
  }
  return true;
}
int MF3DS_createMesh(stream_t* file, IAnimatedMesh* am)
{
  ChunkData data[1] = {0};
  C3DSMeshFileLoader s[1] = {0};
  bool ret = false;
  MF3DS_readChunkData(file, data);
  if (data->header.id != C3DS_MAIN3DS) {
    return 0;
  }
  s->Mesh = MALLOC(IMesh, 1);
  if (MF3DS_readChunk(file, data, s)) {
#if 0
    int i;
    // success
    for (i = 0; i < s->Mesh->MeshBuffers_size; ++i) {
      IMeshBuffer* mb = s->Mesh->MeshBuffers[i];
      // drop empty buffers
      if (mb->Indices_size == 0 || mb->Vertices_size == 0) {
        s->Mesh->MeshBuffers(i--);
      }
      else {
        if (mb->Material.MaterialType == EMT_PARALLAX_MAP_SOLID) {
          SMesh tmp;
          tmp.addMeshBuffer(mb);
          mb->drop();
          IMesh* tangentMesh = SceneManager->getMeshManipulator()->createMeshWithTangents(&tmp);
          Mesh->MeshBuffers[i] = tangentMesh->getMeshBuffer(0);
          // we need to grab because we replace the buffer manually.
          Mesh->MeshBuffers[i]->grab();
          // clean up intermediate mesh struct
          tangentMesh->drop();
        }
        Mesh->MeshBuffers[i]->recalculateBoundingBox();
      }
    }
    s->Mesh->recalculateBoundingBox();
    am = new SAnimatedMesh();
    am->Type = EAMT_3DS;
    am->addMesh(Mesh);
    am->recalculateBoundingBox();
    ret = true;
#endif
  }
  MF3DS_cleanUp(s);
  return ret;
}
