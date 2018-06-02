// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
//
// This file was originally written by ZDimitor.
// I (Nikolaus Gebhardt) did some few changes to this:
// - replaced logging calls to their os:: counterparts
// - removed some logging calls
// - removed setTexture path and replaced it with the directory of the mesh
// - added EAMT_MY3D file type
// - fixed a memory leak when decompressing RLE data.
// - cleaned multi character constant problems with gcc
// - removed octree child scene node generation because irrlicht is now able to draw
//   scene nodes with transparent and sold materials in them at the same time. (see changes.txt)
// Thanks a lot to ZDimitor for his work on this and that he gave me
// his permission to add it into Irrlicht.
//--------------------------------------------------------------------------------
// This tool created by ZDimitor everyone can use it as wants
//--------------------------------------------------------------------------------
#ifndef __CMY3D_MESH_FILE_LOADER_H_INCLUDED__
#define __CMY3D_MESH_FILE_LOADER_H_INCLUDED__
#ifdef _MSC_VER
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#endif
//#include "IMeshLoader.h"
//#include "SMesh.h"
//#include "SMeshBufferLightMap.h"
//#include "IFileSystem.h"
//#include "IVideoDriver.h"
//#include "irrString.h"
//#include "ISceneManager.h"
// byte-align structures
//#include "irrpack.h"
struct SMyColor {
  SMyColor() {
    ;
  }
  SMyColor(s32 __R, s32 __G, s32 __B, s32 __A)
    : R(__R), G(__G), B(__B), A(__A) {}
  s32 R, G, B, A;
} PACK_STRUCT;
// material header
struct SMyMaterialHeader {
  c8  Name[256];           // material name
  u32 Index;
  SMyColor AmbientColor;
  SMyColor DiffuseColor;
  SMyColor EmissiveColor;
  SMyColor SpecularColor;
  f32 Shininess;
  f32 Transparency;
  u32 TextureCount;        // texture count
} PACK_STRUCT;
// Default alignment
//#include "irrunpack.h"
class CMY3DMeshFileLoader : public IMeshLoader
{
public:
  CMY3DMeshFileLoader(ISceneManager* scmgr, IFileSystem* fs);
  ~CMY3DMeshFileLoader();
  bool isALoadableFileExtension(const char* filename) const;
  IAnimatedMesh* createMesh(IReadFile* file);
  //! getting access to the nodes (with transparent material), creating
  //! while loading .my3d file
  const ISceneNode* > & getChildNodes() const;
private:
  ITexture* readEmbeddedLightmap(IReadFile* file, char* namebuf);
  ISceneManager* SceneManager;
  IFileSystem* FileSystem;
  struct SMyMaterialEntry {
    SMyMaterialEntry()
      : Texture1FileName("null"), Texture2FileName("null"),
        Texture1(0), Texture2(0), MaterialType(EMT_SOLID) {}
    SMyMaterialHeader Header;
    char* Texture1FileName;
    char* Texture2FileName;
    ITexture* Texture1;
    ITexture* Texture2;
    E_MATERIAL_TYPE MaterialType;
  };
  struct SMyMeshBufferEntry {
    SMyMeshBufferEntry() : MaterialIndex(-1), MeshBuffer(0) {}
    SMyMeshBufferEntry(s32 mi, SMeshBufferLightMap* mb)
      : MaterialIndex(mi), MeshBuffer(mb) {}
    s32 MaterialIndex;
    SMeshBufferLightMap* MeshBuffer;
  };
  SMyMaterialEntry*    getMaterialEntryByIndex(u32 matInd);
  SMeshBufferLightMap* getMeshBufferByMaterialIndex(u32 matInd);
  SMyMaterialEntry >   MaterialEntry;
  SMyMeshBufferEntry > MeshBufferEntry;
  ISceneNode* > ChildNodes;
};
#endif // __CMY3D_MESH_FILE_LOADER_H_INCLUDED__

