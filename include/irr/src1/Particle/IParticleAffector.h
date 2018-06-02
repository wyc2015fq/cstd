// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
#ifndef __I_PARTICLE_AFFECTOR_H_INCLUDED__
#define __I_PARTICLE_AFFECTOR_H_INCLUDED__
//#include "IAttributeExchangingObject.h"
//#include "SParticle.h"
//! Types of built in particle affectors
enum E_PARTICLE_AFFECTOR_TYPE {
  EPAT_NONE = 0,
  EPAT_ATTRACT,
  EPAT_FADE_OUT,
  EPAT_GRAVITY,
  EPAT_ROTATE,
  EPAT_SCALE,
  EPAT_COUNT
};
//! Names for built in particle affectors
const c8* const ParticleAffectorTypeNames[] = {
  "None",
  "Attract",
  "FadeOut",
  "Gravity",
  "Rotate",
  "Scale",
  0
};
//! A particle affector modifies particles.
class IParticleAffector : public IAttributeExchangingObject
{
public:
  //! constructor
  IParticleAffector() : Enabled(true) {}
  //! Affects an array of particles.
  /** \param now Current time. (Same as ITimer::getTime() would return)
  \param particlearray Array of particles.
  \param count Amount of particles in array. */
  void affect(u32 now, SParticle* particlearray, u32 count) = 0;
  //! Sets whether or not the affector is currently enabled.
  void setEnabled(bool enabled) {
    Enabled = enabled;
  }
  //! Gets whether or not the affector is currently enabled.
  bool getEnabled() const {
    return Enabled;
  }
  //! Get emitter type
  E_PARTICLE_AFFECTOR_TYPE getType() = 0;
protected:
  bool Enabled;
};
#endif

