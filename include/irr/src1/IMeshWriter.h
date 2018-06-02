// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
#ifndef __IRR_I_MESH_WRITER_H_INCLUDED__
#define __IRR_I_MESH_WRITER_H_INCLUDED__
class IWriteFile;
class IMesh;
//! Interface for writing meshes
class IMeshWriter
{
public:
  //! Destructor
  ~IMeshWriter() {}
  //! Get the type of the mesh writer
  /** For own implementations, use MAKE_IRR_ID as shown in the
  EMESH_WRITER_TYPE enumeration to return your own unique mesh
  type id.
  \return Type of the mesh writer. */
  EMESH_WRITER_TYPE getType() = 0;
  //! Write a static mesh.
  /** \param file File handle to write the mesh to.
  \param mesh Pointer to mesh to be written.
  \param flags Optional flags to set properties of the writer.
  \return True if sucessful */
  bool writeMesh(IWriteFile* file, IMesh* mesh,
      s32 flags = EMWF_NONE) = 0;
  // Writes an animated mesh
  // for future use, no writer is able to write animated meshes currently
  /* \return Returns true if sucessful */
  //virtual bool writeAnimatedMesh(IWriteFile* file,
  // IAnimatedMesh* mesh,
  // s32 flags=EMWF_NONE) = 0;
};
// end
// end
#endif

