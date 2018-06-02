// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
#ifndef __I_PARTICLE_EMITTER_H_INCLUDED__
#define __I_PARTICLE_EMITTER_H_INCLUDED__
//#include "IAttributeExchangingObject.h"
//#include "SParticle.h"
//! Types of built in particle emitters
enum E_PARTICLE_EMITTER_TYPE {
  EPET_POINT = 0,
  EPET_ANIMATED_MESH,
  EPET_BOX,
  EPET_CYLINDER,
  EPET_MESH,
  EPET_RING,
  EPET_SPHERE,
  EPET_COUNT
};
//! Names for built in particle emitters
const c8* const ParticleEmitterTypeNames[] = {
  "Point",
  "AnimatedMesh",
  "Box",
  "Cylinder",
  "Mesh",
  "Ring",
  "Sphere",
  0
};
//! A particle emitter for using with particle systems.
/** A Particle emitter emitts new particles into a particle system.
*/
class IParticleEmitter : public IAttributeExchangingObject
{
public:
  //! Prepares an array with new particles to emitt into the system
  /** \param now Current time.
  \param timeSinceLastCall Time elapsed since last call, in milliseconds.
  \param outArray Pointer which will point to the array with the new
  particles to add into the system.
  \return Amount of new particles in the array. Can be 0. */
  s32 emitt(u32 now, u32 timeSinceLastCall, SParticle* outArray) = 0;
  //! Set direction the emitter emits particles
  void setDirection(const float3 newDirection) = 0;
  //! Set minimum number of particles the emitter emits per second
  void setMinParticlesPerSecond(u32 minPPS) = 0;
  //! Set maximum number of particles the emitter emits per second
  void setMaxParticlesPerSecond(u32 maxPPS) = 0;
  //! Set minimum starting color for particles
  void setMinStartColor(const SColor& color) = 0;
  //! Set maximum starting color for particles
  void setMaxStartColor(const SColor& color) = 0;
  //! Set the maximum starting size for particles
  void setMaxStartSize(const dimension2df& size) = 0;
  //! Set the minimum starting size for particles
  void setMinStartSize(const dimension2df& size) = 0;
  //! Set the minimum particle life-time in milliseconds
  void setMinLifeTime(u32 lifeTimeMin) = 0;
  //! Set the maximum particle life-time in milliseconds
  void setMaxLifeTime(u32 lifeTimeMax) = 0;
  //! Set maximal random derivation from the direction
  void setMaxAngleDegrees(s32 maxAngleDegrees) = 0;
  //! Get direction the emitter emits particles
  const float3 getDirection() = 0;
  //! Get the minimum number of particles the emitter emits per second
  u32 getMinParticlesPerSecond() = 0;
  //! Get the maximum number of particles the emitter emits per second
  u32 getMaxParticlesPerSecond() = 0;
  //! Get the minimum starting color for particles
  const SColor& getMinStartColor() = 0;
  //! Get the maximum starting color for particles
  const SColor& getMaxStartColor() = 0;
  //! Get the maximum starting size for particles
  const dimension2df& getMaxStartSize() = 0;
  //! Get the minimum starting size for particles
  const dimension2df& getMinStartSize() = 0;
  //! Get the minimum particle life-time in milliseconds
  u32 getMinLifeTime() = 0;
  //! Get the maximum particle life-time in milliseconds
  u32 getMaxLifeTime() = 0;
  //! Get maximal random derivation from the direction
  s32 getMaxAngleDegrees() = 0;
  //! Get emitter type
  E_PARTICLE_EMITTER_TYPE getType() const {
    return EPET_POINT;
  }
};
typedef IParticleEmitter IParticlePointEmitter;
#endif

