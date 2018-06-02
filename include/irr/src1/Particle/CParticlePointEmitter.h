// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
#ifndef __C_PARTICLE_POINT_EMITTER_H_INCLUDED__
#define __C_PARTICLE_POINT_EMITTER_H_INCLUDED__
//#include "IParticleEmitter.h"
//#include "irrArray.h"
//! A default point emitter
class CParticlePointEmitter : public IParticlePointEmitter
{
public:
  //! constructor
  CParticlePointEmitter(
      const float3 direction = float3(0.0f, 0.03f, 0.0f),
      u32 minParticlesPerSecond = 5,
      u32 maxParticlesPerSecond = 10,
      SColor minStartColor = SColor(255, 0, 0, 0),
      SColor maxStartColor = SColor(255, 255, 255, 255),
      u32 lifeTimeMin = 2000,
      u32 lifeTimeMax = 4000,
      s32 maxAngleDegrees = 0,
      const dimension2df& minStartSize = dimension2df(5.0f, 5.0f),
      const dimension2df& maxStartSize = dimension2df(5.0f, 5.0f));
  //! Prepares an array with new particles to emitt into the system
  //! and returns how much new particles there are.
  s32 emitt(u32 now, u32 timeSinceLastCall, SParticle* outArray);
  //! Set direction the emitter emits particles.
  void setDirection(const float3 newDirection) {
    Direction = newDirection;
  }
  //! Set minimum number of particles emitted per second.
  void setMinParticlesPerSecond(u32 minPPS) {
    MinParticlesPerSecond = minPPS;
  }
  //! Set maximum number of particles emitted per second.
  void setMaxParticlesPerSecond(u32 maxPPS) {
    MaxParticlesPerSecond = maxPPS;
  }
  //! Set minimum start color.
  void setMinStartColor(const SColor& color) {
    MinStartColor = color;
  }
  //! Set maximum start color.
  void setMaxStartColor(const SColor& color) {
    MaxStartColor = color;
  }
  //! Set the maximum starting size for particles
  void setMaxStartSize(const dimension2df& size) {
    MaxStartSize = size;
  }
  //! Set the minimum starting size for particles
  void setMinStartSize(const dimension2df& size) {
    MinStartSize = size;
  }
  //! Set the minimum particle life-time in milliseconds
  void setMinLifeTime(u32 lifeTimeMin) {
    MinLifeTime = lifeTimeMin;
  }
  //! Set the maximum particle life-time in milliseconds
  void setMaxLifeTime(u32 lifeTimeMax) {
    MaxLifeTime = lifeTimeMax;
  }
  //! Set maximal random derivation from the direction
  void setMaxAngleDegrees(s32 maxAngleDegrees) {
    MaxAngleDegrees = maxAngleDegrees;
  }
  //! Gets direction the emitter emits particles.
  const float3 getDirection() const {
    return Direction;
  }
  //! Gets minimum number of particles emitted per second.
  u32 getMinParticlesPerSecond() const {
    return MinParticlesPerSecond;
  }
  //! Gets maximum number of particles emitted per second.
  u32 getMaxParticlesPerSecond() const {
    return MaxParticlesPerSecond;
  }
  //! Gets minimum start color.
  const SColor& getMinStartColor() const {
    return MinStartColor;
  }
  //! Gets maximum start color.
  const SColor& getMaxStartColor() const {
    return MaxStartColor;
  }
  //! Gets the maximum starting size for particles
  const dimension2df& getMaxStartSize() const {
    return MaxStartSize;
  }
  //! Gets the minimum starting size for particles
  const dimension2df& getMinStartSize() const {
    return MinStartSize;
  }
  //! Get the minimum particle life-time in milliseconds
  u32 getMinLifeTime() const {
    return MinLifeTime;
  }
  //! Get the maximum particle life-time in milliseconds
  u32 getMaxLifeTime() const {
    return MaxLifeTime;
  }
  //! Get maximal random derivation from the direction
  s32 getMaxAngleDegrees() const {
    return MaxAngleDegrees;
  }
  //! Writes attributes of the object.
  void serializeAttributes(IAttributes* out, SAttributeReadWriteOptions* options) const;
  //! Reads attributes of the object.
  void deserializeAttributes(IAttributes* in, SAttributeReadWriteOptions* options);
private:
  SParticle Particle;
  float3 Direction;
  dimension2df MinStartSize, MaxStartSize;
  u32 MinParticlesPerSecond, MaxParticlesPerSecond;
  SColor MinStartColor, MaxStartColor;
  u32 MinLifeTime, MaxLifeTime;
  s32 MaxAngleDegrees;
  u32 Time;
  u32 Emitted;
};
#endif

