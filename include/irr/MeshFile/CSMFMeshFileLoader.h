// Copyright (C) 2010-2012 Gaz Davidson
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
#ifndef __C_SMF_MESH_LOADER_H_INCLUDED__
#define __C_SMF_MESH_LOADER_H_INCLUDED__
//#include "IMeshLoader.h"
//#include "SMesh.h"
class IVideoDriver;
}
//! Class which can load
class CSMFMeshFileLoader : public IMeshLoader
{
public:
  CSMFMeshFileLoader(IVideoDriver* driver);
  //! Returns true if the file might be loaded by this class.
  bool isALoadableFileExtension(const char* filename) const;
  //! Creates/loads an animated mesh from the file.
  IAnimatedMesh* createMesh(IReadFile* file);
private:
  void loadLimb(IReadFile* file, SMesh* mesh, const matrix4& parentTransformation);
  IVideoDriver* Driver;
};
class BinaryFile
{
public:
  //! reads most types from the given file, moving the file pointer along
  static void read(IReadFile* file, void* out, int len, bool bigEndian = false);
  //! reads a 3d vector from the file, moving the file pointer along
  static void read(IReadFile* file, vector3df& outVector2d, bool bigEndian = false);
  //! reads a 2d vector from the file, moving the file pointer along
  static void read(IReadFile* file, vector2df& outVector2d, bool bigEndian = false);
  //! reads a null terminated string from the file, moving the file pointer along
  static void read(IReadFile* file, char* outString, bool bigEndian = false);
};
}
#endif // __C_SMF_MESH_LOADER_H_INCLUDED__

