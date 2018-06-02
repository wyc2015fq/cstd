// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
#ifndef __C_PARTICLE_MESH_EMITTER_H_INCLUDED__
#define __C_PARTICLE_MESH_EMITTER_H_INCLUDED__
//#include "IParticleMeshEmitter.h"
//#include "irrArray.h"
//! A default box emitter
class CParticleMeshEmitter : public IParticleMeshEmitter
{
public:
  //! constructor
  CParticleMeshEmitter(
      IMesh* mesh, bool useNormalDirection = true,
      const float3 direction = float3(0.0f, 0.0f, 0.0f),
      f32 normalDirectionModifier = 100.0f,
      s32 mbNumber = -1,
      bool everyMeshVertex = false,
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
  //! Set Mesh to emit particles from
  void setMesh(IMesh* mesh);
  //! Set whether to use vertex normal for direction, or direction specified
  void setUseNormalDirection(bool useNormalDirection) {
    UseNormalDirection = useNormalDirection;
  }
  //! Set direction the emitter emits particles
  void setDirection(const float3 newDirection) {
    Direction = newDirection;
  }
  //! Set the amount that the normal is divided by for getting a particles direction
  void setNormalDirectionModifier(f32 normalDirectionModifier) {
    NormalDirectionModifier = normalDirectionModifier;
  }
  //! Sets whether to emit min<->max particles for every vertex per second, or to pick
  //! min<->max vertices every second
  void setEveryMeshVertex(bool everyMeshVertex) {
    EveryMeshVertex = everyMeshVertex;
  }
  //! Set minimum number of particles the emitter emits per second
  void setMinParticlesPerSecond(u32 minPPS) {
    MinParticlesPerSecond = minPPS;
  }
  //! Set maximum number of particles the emitter emits per second
  void setMaxParticlesPerSecond(u32 maxPPS) {
    MaxParticlesPerSecond = maxPPS;
  }
  //! Set minimum starting color for particles
  void setMinStartColor(const SColor& color) {
    MinStartColor = color;
  }
  //! Set maximum starting color for particles
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
  //! Get Mesh we're emitting particles from
  const IMesh* getMesh() const {
    return Mesh;
  }
  //! Get whether to use vertex normal for direciton, or direction specified
  bool isUsingNormalDirection() const {
    return UseNormalDirection;
  }
  //! Get direction the emitter emits particles
  const float3 getDirection() const {
    return Direction;
  }
  //! Get the amount that the normal is divided by for getting a particles direction
  f32 getNormalDirectionModifier() const {
    return NormalDirectionModifier;
  }
  //! Gets whether to emit min<->max particles for every vertex per second, or to pick
  //! min<->max vertices every second
  bool getEveryMeshVertex() const {
    return EveryMeshVertex;
  }
  //! Get the minimum number of particles the emitter emits per second
  u32 getMinParticlesPerSecond() const {
    return MinParticlesPerSecond;
  }
  //! Get the maximum number of particles the emitter emits per second
  u32 getMaxParticlesPerSecond() const {
    return MaxParticlesPerSecond;
  }
  //! Get the minimum starting color for particles
  const SColor& getMinStartColor() const {
    return MinStartColor;
  }
  //! Get the maximum starting color for particles
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
private:
  const IMesh* Mesh;
  s32 >  VertexPerMeshBufferList;
  s32 TotalVertices;
  u32 MBCount;
  s32 MBNumber;
  f32 NormalDirectionModifier;
  SParticle > Particles;
  float3 Direction;
  dimension2df MaxStartSize, MinStartSize;
  u32 MinParticlesPerSecond, MaxParticlesPerSecond;
  SColor MinStartColor, MaxStartColor;
  u32 MinLifeTime, MaxLifeTime;
  u32 Time;
  u32 Emitted;
  s32 MaxAngleDegrees;
  bool EveryMeshVertex;
  bool UseNormalDirection;
};
#endif // __C_PARTICLE_MESH_EMITTER_H_INCLUDED__

