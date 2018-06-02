// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
#ifndef __C_PARTICLE_ATTRACTION_AFFECTOR_H_INCLUDED__
#define __C_PARTICLE_ATTRACTION_AFFECTOR_H_INCLUDED__
//#include "IParticleAttractionAffector.h"
//! Particle Affector for attracting particles to a point
class CParticleAttractionAffector : public IParticleAttractionAffector
{
public:
  CParticleAttractionAffector(
      const float3 point = float3(), f32 speed = 1.0f,
      bool attract = true, bool affectX = true,
      bool affectY = true, bool affectZ = true);
  //! Affects a particle.
  void affect(u32 now, SParticle* particlearray, u32 count);
  //! Set the point that particles will attract to
  void setPoint(const float3 point) {
    Point = point;
  }
  //! Set the speed, in game units per second that the particles will attract to
  //! the specified point
  void setSpeed(f32 speed) {
    Speed = speed;
  }
  //! Set whether or not the particles are attracting or detracting
  void setAttract(bool attract) {
    Attract = attract;
  }
  //! Set whether or not this will affect particles in the X direction
  void setAffectX(bool affect) {
    AffectX = affect;
  }
  //! Set whether or not this will affect particles in the Y direction
  void setAffectY(bool affect) {
    AffectY = affect;
  }
  //! Set whether or not this will affect particles in the Z direction
  void setAffectZ(bool affect) {
    AffectZ = affect;
  }
  //! Get the point that particles are attracted to
  const float3 getPoint() const {
    return Point;
  }
  //! Get the speed that points attract to the specified point
  f32 getSpeed() const {
    return Speed;
  }
  //! Get whether or not the particles are attracting or detracting
  bool getAttract() const {
    return Attract;
  }
  //! Get whether or not the particles X position are affected
  bool getAffectX() const {
    return AffectX;
  }
  //! Get whether or not the particles Y position are affected
  bool getAffectY() const {
    return AffectY;
  }
  //! Get whether or not the particles Z position are affected
  bool getAffectZ() const {
    return AffectZ;
  }
  //! Writes attributes of the object.
  void serializeAttributes(IAttributes* out, SAttributeReadWriteOptions* options) const;
  //! Reads attributes of the object.
  void deserializeAttributes(IAttributes* in, SAttributeReadWriteOptions* options);
private:
  float3 Point;
  f32 Speed;
  bool AffectX;
  bool AffectY;
  bool AffectZ;
  bool Attract;
  u32 LastTime;
};
#endif // __C_PARTICLE_ATTRACTION_AFFECTOR_H_INCLUDED__

