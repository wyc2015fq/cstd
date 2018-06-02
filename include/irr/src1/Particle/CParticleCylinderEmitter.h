// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
#ifndef __C_PARTICLE_CYLINDER_EMITTER_H_INCLUDED__
#define __C_PARTICLE_CYLINDER_EMITTER_H_INCLUDED__
//#include "IParticleCylinderEmitter.h"
//#include "irrArray.h"
//! A default box emitter
class CParticleCylinderEmitter : public IParticleCylinderEmitter
{
public:
  //! constructor
  CParticleCylinderEmitter(
      const float3 center, f32 radius,
      const float3 normal, f32 length,
      bool outlineOnly = false, const float3 direction = float3(0.0f, 0.03f, 0.0f),
      u32 minParticlesPerSecond = 20,
      u32 maxParticlesPerSecond = 40,
      const SColor& minStartColor = SColor(255, 0, 0, 0),
      const SColor& maxStartColor = SColor(255, 255, 255, 255),
      u32 lifeTimeMin = 2000,
      u32 lifeTimeMax = 4000,
      s32 maxAngleDegrees = 0,
      const dimension2df& minStartSize = dimension2df(5.0f, 5.0f),
      const dimension2df& maxStartSize = dimension2df(5.0f, 5.0f)
  );
  //! Prepares an array with new particles to emitt into the system
  //! and returns how much new particles there are.
  s32 emitt(u32 now, u32 timeSinceLastCall, SParticle* outArray);
  //! Set the center of the radius for the cylinder, at one end of the cylinder
  void setCenter(const float3 center) {
    Center = center;
  }
  //! Set the normal of the cylinder
  void setNormal(const float3 normal) {
    Normal = normal;
  }
  //! Set the radius of the cylinder
  void setRadius(f32 radius) {
    Radius = radius;
  }
  //! Set the length of the cylinder
  void setLength(f32 length) {
    Length = length;
  }
  //! Set whether or not to draw points inside the cylinder
  void setOutlineOnly(bool outlineOnly) {
    OutlineOnly = outlineOnly;
  }
  //! Set direction the emitter emits particles
  void setDirection(const float3 newDirection) {
    Direction = newDirection;
  }
  //! Set direction the emitter emits particles
  void setMinParticlesPerSecond(u32 minPPS) {
    MinParticlesPerSecond = minPPS;
  }
  //! Set direction the emitter emits particles
  void setMaxParticlesPerSecond(u32 maxPPS) {
    MaxParticlesPerSecond = maxPPS;
  }
  //! Set direction the emitter emits particles
  void setMinStartColor(const SColor& color) {
    MinStartColor = color;
  }
  //! Set direction the emitter emits particles
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
  //! Maximal random derivation from the direction
  void setMaxAngleDegrees(s32 maxAngleDegrees) {
    MaxAngleDegrees = maxAngleDegrees;
  }
  //! Get the center of the cylinder
  const float3 getCenter() const {
    return Center;
  }
  //! Get the normal of the cylinder
  const float3 getNormal() const {
    return Normal;
  }
  //! Get the radius of the cylinder
  f32 getRadius() const {
    return Radius;
  }
  //! Get the center of the cylinder
  f32 getLength() const {
    return Length;
  }
  //! Get whether or not to draw points inside the cylinder
  bool getOutlineOnly() const {
    return OutlineOnly;
  }
  //! Gets direction the emitter emits particles
  const float3 getDirection() const {
    return Direction;
  }
  //! Gets direction the emitter emits particles
  u32 getMinParticlesPerSecond() const {
    return MinParticlesPerSecond;
  }
  //! Gets direction the emitter emits particles
  u32 getMaxParticlesPerSecond() const {
    return MaxParticlesPerSecond;
  }
  //! Gets direction the emitter emits particles
  const SColor& getMinStartColor() const {
    return MinStartColor;
  }
  //! Gets direction the emitter emits particles
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
  //! Maximal random derivation from the direction
  s32 getMaxAngleDegrees() const {
    return MaxAngleDegrees;
  }
  //! Writes attributes of the object.
  void serializeAttributes(IAttributes* out, SAttributeReadWriteOptions* options) const;
  //! Reads attributes of the object.
  void deserializeAttributes(IAttributes* in, SAttributeReadWriteOptions* options);
private:
  SParticle > Particles;
  float3 Center;
  float3 Normal;
  float3 Direction;
  dimension2df MaxStartSize, MinStartSize;
  u32 MinParticlesPerSecond, MaxParticlesPerSecond;
  SColor MinStartColor, MaxStartColor;
  u32 MinLifeTime, MaxLifeTime;
  f32 Radius;
  f32 Length;
  u32 Time;
  u32 Emitted;
  s32 MaxAngleDegrees;
  bool OutlineOnly;
};
#endif

