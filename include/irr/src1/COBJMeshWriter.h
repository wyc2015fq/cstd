// Copyright (C) 2008-2012 Christian Stehno
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
#ifndef __IRR_OBJ_MESH_WRITER_H_INCLUDED__
#define __IRR_OBJ_MESH_WRITER_H_INCLUDED__
//#include "IMeshWriter.h"
//#include "irrString.h"
class IFileSystem;
class IMeshBuffer;
class ISceneManager;
//! class to write meshes, implementing a OBJ writer
class COBJMeshWriter : public IMeshWriter
{
public:
  COBJMeshWriter(ISceneManager* smgr, IFileSystem* fs);
  ~COBJMeshWriter();
  //! Returns the type of the mesh writer
  EMESH_WRITER_TYPE getType() const;
  //! writes a mesh
  bool writeMesh(IWriteFile* file, IMesh* mesh, s32 flags = EMWF_NONE);
protected:
  // create vector output with line end into string
  void getVectorAsStringLine(const float3 v,
      char* s) const;
  // create vector output with line end into string
  void getVectorAsStringLine(const vector2df& v,
      char* s) const;
  // create color output with line end into string
  void getColorAsStringLine(const SColor& color,
      const c8* const prefix, char* s) const;
  ISceneManager* SceneManager;
  IFileSystem* FileSystem;
};
// end
// end
#endif

