// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
#ifndef __I_PARTICLE_ATTRACTION_AFFECTOR_H_INCLUDED__
#define __I_PARTICLE_ATTRACTION_AFFECTOR_H_INCLUDED__
//#include "IParticleAffector.h"
//! A particle affector which attracts or detracts particles.
class IParticleAttractionAffector : public IParticleAffector
{
public:
  //! Set the point that particles will attract to
  void setPoint(const float3 point) = 0;
  //! Set whether or not the particles are attracting or detracting
  void setAttract(bool attract) = 0;
  //! Set whether or not this will affect particles in the X direction
  void setAffectX(bool affect) = 0;
  //! Set whether or not this will affect particles in the Y direction
  void setAffectY(bool affect) = 0;
  //! Set whether or not this will affect particles in the Z direction
  void setAffectZ(bool affect) = 0;
  //! Get the point that particles are attracted to
  const float3 getPoint() = 0;
  //! Get whether or not the particles are attracting or detracting
  bool getAttract() = 0;
  //! Get whether or not the particles X position are affected
  bool getAffectX() = 0;
  //! Get whether or not the particles Y position are affected
  bool getAffectY() = 0;
  //! Get whether or not the particles Z position are affected
  bool getAffectZ() = 0;
  //! Get emitter type
  E_PARTICLE_AFFECTOR_TYPE getType() const {
    return EPAT_ATTRACT;
  }
};
#endif // __I_PARTICLE_ATTRACTION_AFFECTOR_H_INCLUDED__

