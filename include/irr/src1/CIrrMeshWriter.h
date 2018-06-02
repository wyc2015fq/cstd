// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
#ifndef __IRR_IRR_MESH_WRITER_H_INCLUDED__
#define __IRR_IRR_MESH_WRITER_H_INCLUDED__
//#include "IMeshWriter.h"
//#include "IVideoDriver.h"
//#include "IFileSystem.h"
class IXMLWriter;
}
class IMeshBuffer;
//! class to write meshes, implementing a IrrMesh (.irrmesh, .xml) writer
/** This writer implementation has been originally developed for irrEdit and then
merged out to the Irrlicht Engine */
class CIrrMeshWriter : public IMeshWriter
{
public:
  CIrrMeshWriter(IVideoDriver* driver, IFileSystem* fs);
  ~CIrrMeshWriter();
  //! Returns the type of the mesh writer
  EMESH_WRITER_TYPE getType() const;
  //! writes a mesh
  bool writeMesh(IWriteFile* file, IMesh* mesh, s32 flags = EMWF_NONE);
protected:
  void writeBoundingBox(const aabbox3df& box);
  void writeMeshBuffer(const IMeshBuffer* buffer);
  void writeMaterial(const SMaterial& material);
  char* getVectorAsStringLine(const float3 v) const;
  char* getVectorAsStringLine(const vector2df& v) const;
  // member variables:
  IFileSystem* FileSystem;
  IVideoDriver* VideoDriver;
  IXMLWriter* Writer;
};
// end
// end
#endif

