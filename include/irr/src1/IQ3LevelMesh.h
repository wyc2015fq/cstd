// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
#ifndef __I_Q3_LEVEL_MESH_H_INCLUDED__
#define __I_Q3_LEVEL_MESH_H_INCLUDED__
//#include "IAnimatedMesh.h"
//#include "IQ3Shader.h"
//! Interface for a Mesh which can be loaded directly from a Quake3 .bsp-file.
/** The Mesh tries to load all textures of the map.*/
class IQ3LevelMesh : public IAnimatedMesh
{
public:
  //! loads the shader definition from file
  /** \param filename Name of the shaderfile, defaults to /scripts if fileNameIsValid is false.
  \param fileNameIsValid Specifies whether the filename is valid in the current situation. */
  const quake3::IShader* getShader(const c8* filename, bool fileNameIsValid = true) = 0;
  //! returns a already loaded Shader
  const quake3::IShader* getShader(u32 index) = 0;
  //! get's an interface to the entities
  quake3::IEntity*& getEntityList() = 0;
  //! returns the requested brush entity
  /** \param num The number from the model key of the entity.
  Use this interface if you parse the entities yourself.*/
  IMesh* getBrushEntityMesh(s32 num) = 0;
  //! returns the requested brush entity
  IMesh* getBrushEntityMesh(quake3::IEntity& ent) = 0;
};
#endif

