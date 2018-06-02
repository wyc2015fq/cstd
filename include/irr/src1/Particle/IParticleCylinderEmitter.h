// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
#ifndef __I_PARTICLE_CYLINDER_EMITTER_H_INCLUDED__
#define __I_PARTICLE_CYLINDER_EMITTER_H_INCLUDED__
//#include "IParticleEmitter.h"
//! A particle emitter which emits from a cylindrically shaped space.
class IParticleCylinderEmitter : public IParticleEmitter
{
public:
  //! Set the center of the radius for the cylinder, at one end of the cylinder
  void setCenter(const float3 center) = 0;
  //! Set the normal of the cylinder
  void setNormal(const float3 normal) = 0;
  //! Set the radius of the cylinder
  void setRadius(f32 radius) = 0;
  //! Set the length of the cylinder
  void setLength(f32 length) = 0;
  //! Set whether or not to draw points inside the cylinder
  void setOutlineOnly(bool outlineOnly = true) = 0;
  //! Get the center of the cylinder
  const float3 getCenter() = 0;
  //! Get the normal of the cylinder
  const float3 getNormal() = 0;
  //! Get the radius of the cylinder
  f32 getRadius() = 0;
  //! Get the center of the cylinder
  f32 getLength() = 0;
  //! Get whether or not to draw points inside the cylinder
  bool getOutlineOnly() = 0;
  //! Get emitter type
  E_PARTICLE_EMITTER_TYPE getType() const {
    return EPET_CYLINDER;
  }
};
#endif

