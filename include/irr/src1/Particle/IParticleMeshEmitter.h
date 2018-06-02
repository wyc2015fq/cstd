// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
#ifndef __I_PARTICLE_MESH_EMITTER_H_INCLUDED__
#define __I_PARTICLE_MESH_EMITTER_H_INCLUDED__
//#include "IParticleEmitter.h"
//! A particle emitter which emits from vertices of a mesh
class IParticleMeshEmitter : public IParticleEmitter
{
public:
  //! Set Mesh to emit particles from
  void setMesh(IMesh* mesh) = 0;
  //! Set whether to use vertex normal for direction, or direction specified
  void setUseNormalDirection(bool useNormalDirection = true) = 0;
  //! Set the amount that the normal is divided by for getting a particles direction
  void setNormalDirectionModifier(f32 normalDirectionModifier) = 0;
  //! Sets whether to emit min<->max particles for every vertex or to pick min<->max vertices
  void setEveryMeshVertex(bool everyMeshVertex = true) = 0;
  //! Get Mesh we're emitting particles from
  const IMesh* getMesh() = 0;
  //! Get whether to use vertex normal for direction, or direction specified
  bool isUsingNormalDirection() = 0;
  //! Get the amount that the normal is divided by for getting a particles direction
  f32 getNormalDirectionModifier() = 0;
  //! Gets whether to emit min<->max particles for every vertex or to pick min<->max vertices
  bool getEveryMeshVertex() = 0;
  //! Get emitter type
  E_PARTICLE_EMITTER_TYPE getType() const {
    return EPET_MESH;
  }
};
#endif

