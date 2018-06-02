// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
#ifndef __C_PARTICLE_ROTATION_AFFECTOR_H_INCLUDED__
#define __C_PARTICLE_ROTATION_AFFECTOR_H_INCLUDED__
//#include "IParticleRotationAffector.h"
//! Particle Affector for rotating particles about a point
class CParticleRotationAffector : public IParticleRotationAffector
{
public:
  CParticleRotationAffector(const float3 speed = float3(5.0f, 5.0f, 5.0f),
      const float3 point = float3());
  //! Affects a particle.
  void affect(u32 now, SParticle* particlearray, u32 count);
  //! Set the point that particles will attract to
  void setPivotPoint(const float3 point) {
    PivotPoint = point;
  }
  //! Set the speed in degrees per second
  void setSpeed(const float3 speed) {
    Speed = speed;
  }
  //! Get the point that particles are attracted to
  const float3 getPivotPoint() const {
    return PivotPoint;
  }
  //! Get the speed in degrees per second
  const float3 getSpeed() const {
    return Speed;
  }
  //! Writes attributes of the object.
  void serializeAttributes(IAttributes* out, SAttributeReadWriteOptions* options) const;
  //! Reads attributes of the object.
  void deserializeAttributes(IAttributes* in, SAttributeReadWriteOptions* options);
private:
  float3 PivotPoint;
  float3 Speed;
  u32 LastTime;
};
#endif // __C_PARTICLE_ROTATION_AFFECTOR_H_INCLUDED__

