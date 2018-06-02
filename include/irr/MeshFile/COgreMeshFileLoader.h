// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
// orginally written by Christian Stehno, modified by Nikolaus Gebhardt
#ifndef __C_OGRE_MESH_FILE_LOADER_H_INCLUDED__
#define __C_OGRE_MESH_FILE_LOADER_H_INCLUDED__
//#include "IMeshLoader.h"
//#include "IFileSystem.h"
//#include "IVideoDriver.h"
//#include "irrString.h"
//#include "SMesh.h"
//#include "SMeshBuffer.h"
//#include "SMeshBufferLightMap.h"
//#include "IMeshManipulator.h"
//#include "quaternion.h"
//#include "CSkinnedMesh.h"
//! Meshloader capable of loading ogre meshes.
class COgreMeshFileLoader : public IMeshLoader
{
public:
  //! Constructor
  COgreMeshFileLoader(IFileSystem* fs, IVideoDriver* driver);
  //! destructor
  ~COgreMeshFileLoader();
  //! returns true if the file maybe is able to be loaded by this class
  //! based on the file extension (e.g. ".cob")
  bool isALoadableFileExtension(const char* filename) const;
  //! creates/loads an animated mesh from the file.
  //! \return Pointer to the created mesh. Returns 0 if loading failed.
  //! If you no longer need the mesh, you should call IAnimatedMesh::drop().
  //! See IReferenceCounted::drop() for more information.
  IAnimatedMesh* createMesh(IReadFile* file);
private:
  // byte-align structures
  //#include "irrpack.h"
  struct ChunkHeader {
    u16 id;
    u32 length;
  } PACK_STRUCT;
  // Default alignment
  //#include "irrunpack.h"
  struct ChunkData {
    ChunkData() : read(0) {}
    ChunkHeader header;
    u32 read;
  };
  struct OgreTexture {
    strv_t Filename;
    char* Alias;
    char* CoordsType;
    char* MipMaps;
    char* Alpha;
  };
  struct OgrePass {
    OgrePass() : AmbientTokenColor(false),
      DiffuseTokenColor(false), SpecularTokenColor(false),
      EmissiveTokenColor(false),
      MaxLights(8), PointSize(1.0f), PointSprites(false),
      PointSizeMin(0), PointSizeMax(0) {}
    SMaterial Material;
    OgreTexture Texture;
    bool AmbientTokenColor;
    bool DiffuseTokenColor;
    bool SpecularTokenColor;
    bool EmissiveTokenColor;
    u32 MaxLights;
    f32 PointSize;
    bool PointSprites;
    u32 PointSizeMin;
    u32 PointSizeMax;
  };
  struct OgreTechnique {
    OgreTechnique() : Name(""), LODIndex(0) {}
    char* Name;
    char* Scheme;
    u16 LODIndex;
    OgrePass > Passes;
  };
  struct OgreMaterial {
    OgreMaterial() : Name(""), ReceiveShadows(true),
      TransparencyCastsShadows(false) {}
    char* Name;
    bool ReceiveShadows;
    bool TransparencyCastsShadows;
    f32 > LODDistances;
    OgreTechnique > Techniques;
  };
  struct OgreVertexBuffer {
    OgreVertexBuffer() : BindIndex(0), VertexSize(0), Data(0) {}
    u16 BindIndex;
    u16 VertexSize;
    f32 > Data;
  };
  struct OgreVertexElement {
    u16 Source,
        Type,
        Semantic,
        Offset,
        Index;
  };
  struct OgreGeometry {
    s32 NumVertex;
    OgreVertexElement > Elements;
    OgreVertexBuffer > Buffers;
    vector3df > Vertices;
    vector3df > Normals;
    s32 > Colors;
    vector2df > TexCoords;
  };
  struct OgreTextureAlias {
    OgreTextureAlias() {};
    OgreTextureAlias(const char* a, const char* b) : Texture(a), Alias(b) {};
    char* Texture;
    char* Alias;
  };
  struct OgreBoneAssignment {
    s32 VertexID;
    u16 BoneID;
    f32 Weight;
  };
  struct OgreSubMesh {
    char* Material;
    bool SharedVertices;
    s32 > Indices;
    OgreGeometry Geometry;
    u16 Operation;
    OgreTextureAlias > TextureAliases;
    OgreBoneAssignment > BoneAssignments;
    bool Indices32Bit;
  };
  struct OgreMesh {
    bool SkeletalAnimation;
    OgreGeometry Geometry;
    OgreSubMesh > SubMeshes;
    OgreBoneAssignment > BoneAssignments;
    vector3df BBoxMinEdge;
    vector3df BBoxMaxEdge;
    f32 BBoxRadius;
  };
  struct OgreBone {
    char* Name;
    vector3df Position;
    quaternion Orientation;
    vector3df Scale;
    u16 Handle;
    u16 Parent;
  };
  struct OgreKeyframe {
    u16 BoneID;
    f32 Time;
    vector3df Position;
    quaternion Orientation;
    vector3df Scale;
  };
  struct OgreAnimation {
    char* Name;
    f32 Length;
    OgreKeyframe > Keyframes;
  };
  struct OgreSkeleton {
    OgreBone > Bones;
    OgreAnimation > Animations;
  };
  bool readChunk(IReadFile* file);
  bool readObjectChunk(IReadFile* file, ChunkData& parent, OgreMesh& mesh);
  bool readGeometry(IReadFile* file, ChunkData& parent, OgreGeometry& geometry);
  bool readVertexDeclaration(IReadFile* file, ChunkData& parent, OgreGeometry& geometry);
  bool readVertexBuffer(IReadFile* file, ChunkData& parent, OgreGeometry& geometry);
  bool readSubMesh(IReadFile* file, ChunkData& parent, OgreSubMesh& subMesh);
  void readChunkData(IReadFile* file, ChunkData& data);
  void readString(IReadFile* file, ChunkData& data, char* out);
  void readBool(IReadFile* file, ChunkData& data, bool& out);
  void readInt(IReadFile* file, ChunkData& data, s32* out, u32 num = 1);
  void readShort(IReadFile* file, ChunkData& data, u16* out, u32 num = 1);
  void readFloat(IReadFile* file, ChunkData& data, f32* out, u32 num = 1);
  void readVector(IReadFile* file, ChunkData& data, vector3df& out);
  void readQuaternion(IReadFile* file, ChunkData& data, quaternion& out);
  void composeMeshBufferMaterial(IMeshBuffer* mb, const char* materialName);
  SMeshBuffer* composeMeshBuffer(const s32* indices, const OgreGeometry& geom);
  SMeshBufferLightMap* composeMeshBufferLightMap(const s32* indices, const OgreGeometry& geom);
  IMeshBuffer* composeMeshBufferSkinned(CSkinnedMesh& mesh, const s32* indices, const OgreGeometry& geom);
  void composeObject(void);
  bool readColor(IReadFile* meshFile, SColor& col);
  void getMaterialToken(IReadFile* file, char* token, bool noNewLine = false);
  void readTechnique(IReadFile* meshFile, OgreMaterial& mat);
  void readPass(IReadFile* file, OgreTechnique& technique);
  void loadMaterials(IReadFile* file);
  bool loadSkeleton(IReadFile* meshFile, const char* name);
  void clearMeshes();
  IFileSystem* FileSystem;
  IVideoDriver* Driver;
  char* Version;
  bool SwapEndian;
  OgreMesh > Meshes;
  char* CurrentlyLoadingFromPath;
  OgreMaterial > Materials;
  OgreSkeleton Skeleton;
  IMesh* Mesh;
  u32 NumUV;
};
#endif

