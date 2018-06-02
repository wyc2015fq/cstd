// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
#ifndef __I_PARTICLE_RING_EMITTER_H_INCLUDED__
#define __I_PARTICLE_RING_EMITTER_H_INCLUDED__
//#include "IParticleEmitter.h"
//! A particle emitter which emits particles along a ring shaped area.
class IParticleRingEmitter : public IParticleEmitter
{
public:
  //! Set the center of the ring
  void setCenter(const float3 center) = 0;
  //! Set the radius of the ring
  void setRadius(f32 radius) = 0;
  //! Set the thickness of the ring
  void setRingThickness(f32 ringThickness) = 0;
  //! Get the center of the ring
  const float3 getCenter() = 0;
  //! Get the radius of the ring
  f32 getRadius() = 0;
  //! Get the thickness of the ring
  f32 getRingThickness() = 0;
  //! Get emitter type
  E_PARTICLE_EMITTER_TYPE getType() const {
    return EPET_RING;
  }
};
#endif

