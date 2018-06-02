// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
#ifndef __I_PARTICLE_SPHERE_EMITTER_H_INCLUDED__
#define __I_PARTICLE_SPHERE_EMITTER_H_INCLUDED__
//#include "IParticleEmitter.h"
//! A particle emitter which emits from a spherical space.
class IParticleSphereEmitter : public IParticleEmitter
{
public:
  //! Set the center of the sphere for particle emissions
  void setCenter(const float3 center) = 0;
  //! Set the radius of the sphere for particle emissions
  void setRadius(f32 radius) = 0;
  //! Get the center of the sphere for particle emissions
  const float3 getCenter() = 0;
  //! Get the radius of the sphere for particle emissions
  f32 getRadius() = 0;
  //! Get emitter type
  E_PARTICLE_EMITTER_TYPE getType() const {
    return EPET_SPHERE;
  }
};
#endif

