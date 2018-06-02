// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
#ifndef __IRR_STL_MESH_WRITER_H_INCLUDED__
#define __IRR_STL_MESH_WRITER_H_INCLUDED__
//#include "IMeshWriter.h"
//#include "irrString.h"
class IMeshBuffer;
class ISceneManager;
//! class to write meshes, implementing a STL writer
class CSTLMeshWriter : public IMeshWriter
{
public:
  CSTLMeshWriter(ISceneManager* smgr);
  ~CSTLMeshWriter();
  //! Returns the type of the mesh writer
  EMESH_WRITER_TYPE getType() const;
  //! writes a mesh
  bool writeMesh(IWriteFile* file, IMesh* mesh, s32 flags = EMWF_NONE);
protected:
  // write binary format
  bool writeMeshBinary(IWriteFile* file, IMesh* mesh, s32 flags);
  // write text format
  bool writeMeshASCII(IWriteFile* file, IMesh* mesh, s32 flags);
  // create vector output with line end into string
  void getVectorAsStringLine(const float3 v,
      char* s) const;
  // write face information to file
  void writeFace(IWriteFile* file, const float3 v1,
      const float3 v2, const float3 v3);
  ISceneManager* SceneManager;
};
// end
// end
#endif

