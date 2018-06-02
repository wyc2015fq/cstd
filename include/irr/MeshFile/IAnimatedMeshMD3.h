// Copyright (C) 2007-2012 Nikolaus Gebhardt / Thomas Alten
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
#ifndef __I_ANIMATED_MESH_MD3_H_INCLUDED__
#define __I_ANIMATED_MESH_MD3_H_INCLUDED__
//#include "IAnimatedMesh.h"
//#include "IQ3Shader.h"
//#include "quaternion.h"
enum eMD3Models {
  EMD3_HEAD = 0,
  EMD3_UPPER,
  EMD3_LOWER,
  EMD3_WEAPON,
  EMD3_NUMMODELS
};
//! Animation list
enum EMD3_ANIMATION_TYPE {
  // Animations for both lower and upper parts of the player
  EMD3_BOTH_DEATH_1 = 0,
  EMD3_BOTH_DEAD_1,
  EMD3_BOTH_DEATH_2,
  EMD3_BOTH_DEAD_2,
  EMD3_BOTH_DEATH_3,
  EMD3_BOTH_DEAD_3,
  // Animations for the upper part
  EMD3_TORSO_GESTURE,
  EMD3_TORSO_ATTACK_1,
  EMD3_TORSO_ATTACK_2,
  EMD3_TORSO_DROP,
  EMD3_TORSO_RAISE,
  EMD3_TORSO_STAND_1,
  EMD3_TORSO_STAND_2,
  // Animations for the lower part
  EMD3_LEGS_WALK_CROUCH,
  EMD3_LEGS_WALK,
  EMD3_LEGS_RUN,
  EMD3_LEGS_BACK,
  EMD3_LEGS_SWIM,
  EMD3_LEGS_JUMP_1,
  EMD3_LEGS_LAND_1,
  EMD3_LEGS_JUMP_2,
  EMD3_LEGS_LAND_2,
  EMD3_LEGS_IDLE,
  EMD3_LEGS_IDLE_CROUCH,
  EMD3_LEGS_TURN,
  //! Not an animation, but amount of animation types.
  EMD3_ANIMATION_COUNT
};
struct SMD3AnimationInfo {
  //! First frame
  s32 first;
  //! Last frame
  s32 num;
  //! Looping frames
  s32 looping;
  //! Frames per second
  s32 fps;
};
// byte-align structures
//#include "irrpack.h"
//! this holds the header info of the MD3 file
struct SMD3Header {
  c8  headerID[4];  //id of file, always "IDP3"
  s32 Version;  //this is a version number, always 15
  s8  fileName[68]; //sometimes left Blank... 65 chars, 32bit aligned == 68 chars
  s32 numFrames;  //number of KeyFrames
  s32 numTags;  //number of 'tags' per frame
  s32 numMeshes;  //number of meshes/skins
  s32 numMaxSkins;  //maximum number of unique skins used in md3 file. artefact md2
  s32 frameStart; //starting position of frame-structur
  s32 tagStart; //starting position of tag-structures
  s32 tagEnd;   //ending position of tag-structures/starting position of mesh-structures
  s32 fileSize;
} PACK_STRUCT;
//! this holds the header info of an MD3 mesh section
struct SMD3MeshHeader {
  c8 meshID[4];   //id, must be IDP3
  c8 meshName[68];  //name of mesh 65 chars, 32 bit aligned == 68 chars
  s32 numFrames;    //number of meshframes in mesh
  s32 numShader;    //number of skins in mesh
  s32 numVertices;  //number of vertices
  s32 numTriangles; //number of Triangles
  s32 offset_triangles; //starting position of Triangle data, relative to start of Mesh_Header
  s32 offset_shaders; //size of header
  s32 offset_st;    //starting position of texvector data, relative to start of Mesh_Header
  s32 vertexStart;  //starting position of vertex data,relative to start of Mesh_Header
  s32 offset_end;
} PACK_STRUCT;
//! Compressed Vertex Data
struct SMD3Vertex {
  s16 position[3];
  u8 normal[2];
} PACK_STRUCT;
//! Texture Coordinate
struct SMD3TexCoord {
  f32 u;
  f32 v;
} PACK_STRUCT;
//! Triangle Index
struct SMD3Face {
  s32 Index[3];
} PACK_STRUCT;
// Default alignment
//#include "irrunpack.h"
//! Holding Frame Data for a Mesh
struct SMD3MeshBuffer  {
  SMD3MeshHeader MeshHeader;
  char* Shader;
  s32* Indices;
  SMD3Vertex* Vertices;
  SMD3TexCoord* Tex;
};
//! hold a tag info for connecting meshes
/** Basically its an alternate way to describe a transformation. */
struct SMD3QuaternionTag {
  char Name[32];
  vector3df position;
  quaternion rotation;
  ~SMD3QuaternionTag() {
    position.X = 0.f;
  }
  // construct copy constructor
  SMD3QuaternionTag(const SMD3QuaternionTag& copyMe) {
    *this = copyMe;
  }
  // construct for searching
  SMD3QuaternionTag(const char* name) {
    cstr_cpy(Name, 32, name, -1);
  }
  // construct from a position and euler angles in degrees
  SMD3QuaternionTag(const vector3df& pos, const vector3df& angle)
    : position(pos), rotation(angle* DEGTORAD) {}
  // set to matrix
  void setto(matrix4& m) {
    rotation.getMatrix(m, position);
  }
  bool operator == (const SMD3QuaternionTag& other) const {
    return 0 == strcmp(Name, other.Name);
  }
  SMD3QuaternionTag& operator=(const SMD3QuaternionTag& copyMe) {
    cstr_cpy(Name, 32, copyMe.Name, -1);
    position = copyMe.position;
    rotation = copyMe.rotation;
    return *this;
  }
};
//! holds a associative list of named quaternions
struct SMD3QuaternionTagList {
  SMD3QuaternionTag* Container;
  int Container_size;
  SMD3QuaternionTagList() {
    Container = NULL;
    Container_size = 0;
  }
  // construct copy constructor
  SMD3QuaternionTagList(const SMD3QuaternionTagList& copyMe) {
    *this = copyMe;
  }
  int linear_search(const char* name) {
    int i;
    for (i = 0; i < Container_size; ++i) {
      if (0 == strcmp(Container[i].Name, name)) {
        return i;
      }
    }
    return -1;
  }
  SMD3QuaternionTag* get(const char* name) {
    s32 index = linear_search(name);
    if (index >= 0) {
      return &Container[index];
    }
    return 0;
  }
  u32 size() const {
    return Container_size;
  }
  void set_used(u32 new_size) {
  }
  const SMD3QuaternionTag& operator[](u32 index) const {
    return Container[index];
  }
  SMD3QuaternionTag& operator[](u32 index) {
    return Container[index];
  }
  void push_back(const SMD3QuaternionTag& other) {
    ++Container_size;
    MYREALLOC(Container, Container_size);
    Container[Container_size - 1] = other;
  }
private:
  SMD3QuaternionTagList& operator = (const SMD3QuaternionTagList& copyMe) {
    ASSERT(0);
    Container = copyMe.Container;
    return *this;
  }
};
//! Holding Frames Buffers and Tag Infos
struct SMD3Mesh {
  SMD3Mesh() {
    MD3Header.numFrames = 0;
  }
  ~SMD3Mesh() {
  }
  char Name[32];
  SMD3MeshBuffer** Buffer;
  int Buffer_size;
  SMD3QuaternionTagList TagList;
  SMD3Header MD3Header;
};
//! Interface for using some special functions of MD3 meshes
class IAnimatedMeshMD3 : public IAnimatedMesh
{
public:
  //! tune how many frames you want to render inbetween.
  void setInterpolationShift(u32 shift, u32 loopMode) = 0;
  //! get the tag list of the mesh.
  SMD3QuaternionTagList* getTagList(s32 frame, s32 detailLevel, s32 startFrameLoop, s32 endFrameLoop) = 0;
  //! get the original md3 mesh.
  SMD3Mesh* getOriginalMesh() = 0;
};
#endif

