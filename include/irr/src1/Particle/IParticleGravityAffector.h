// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
#ifndef __I_PARTICLE_GRAVITY_AFFECTOR_H_INCLUDED__
#define __I_PARTICLE_GRAVITY_AFFECTOR_H_INCLUDED__
//#include "IParticleAffector.h"
//! A particle affector which applies gravity to particles.
class IParticleGravityAffector : public IParticleAffector
{
public:
  //! Set the time in milliseconds when the gravity force is totally lost
  /** At that point the particle does not move any more. */
  void setTimeForceLost(f32 timeForceLost) = 0;
  //! Set the direction and force of gravity in all 3 dimensions.
  void setGravity(const float3 gravity) = 0;
  //! Get the time in milliseconds when the gravity force is totally lost
  f32 getTimeForceLost() = 0;
  //! Get the direction and force of gravity.
  const float3 getGravity() = 0;
  //! Get emitter type
  E_PARTICLE_AFFECTOR_TYPE getType() const {
    return EPAT_GRAVITY;
  }
};
#endif // __I_PARTICLE_GRAVITY_AFFECTOR_H_INCLUDED__

