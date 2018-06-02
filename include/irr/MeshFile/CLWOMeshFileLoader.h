// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
#ifndef __C_LWO_MESH_FILE_LOADER_H_INCLUDED__
#define __C_LWO_MESH_FILE_LOADER_H_INCLUDED__
//#include "IMeshLoader.h"
//#include "SMeshBuffer.h"
//#include "irrString.h"
class IReadFile;
class IFileSystem;
struct SMesh;
class ISceneManager;
//! Meshloader capable of loading Lightwave 3D meshes.
class CLWOMeshFileLoader : public IMeshLoader
{
public:
  //! Constructor
  CLWOMeshFileLoader(ISceneManager* smgr, IFileSystem* fs);
  //! destructor
  ~CLWOMeshFileLoader();
  //! returns true if the file maybe is able to be loaded by this class
  //! based on the file extension (e.g. ".bsp")
  bool isALoadableFileExtension(const char* filename) const;
  //! creates/loads an animated mesh from the file.
  //! \return Pointer to the created mesh. Returns 0 if loading failed.
  //! If you no longer need the mesh, you should call IAnimatedMesh::drop().
  //! See IUnknown::drop() for more information.
  IAnimatedMesh* createMesh(IReadFile* file);
private:
  struct tLWOMaterial;
  bool readFileHeader();
  bool readChunks();
  void readObj1(u32 size);
  void readTagMapping(u32 size);
  void readVertexMapping(u32 size);
  void readDiscVertexMapping(u32 size);
  void readObj2(u32 size);
  void readMat(u32 size);
  u32 readString(char* name, u32 size = 0);
  u32 readVec(vector3df& vec);
  u32 readVX(u32& num);
  u32 readColor(SColor& color);
  ITexture* loadTexture(const char* file);
  ISceneManager* SceneManager;
  IFileSystem* FileSystem;
  IReadFile* File;
  SMesh* Mesh;
  vector3df > Points;
  u32* > Indices;
  strv_t UvName;
  u32* > UvIndex;
  strv_t DUvName;
  u32* > VmPolyPointsIndex;
  vector2df > > VmCoordsIndex;
  u16* MaterialMapping;
  vector2df > > TCoords;
  tLWOMaterial* > Materials;
  strv_t Images;
  u8 FormatVersion;
};
#endif

