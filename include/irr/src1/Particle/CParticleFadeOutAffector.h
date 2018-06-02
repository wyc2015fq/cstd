// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
#ifndef __C_PARTICLE_FADE_OUT_AFFECTOR_H_INCLUDED__
#define __C_PARTICLE_FADE_OUT_AFFECTOR_H_INCLUDED__
//#include "IParticleFadeOutAffector.h"
//! Particle Affector for fading out a color
class CParticleFadeOutAffector : public IParticleFadeOutAffector
{
public:
  CParticleFadeOutAffector(const SColor& targetColor, u32 fadeOutTime);
  //! Affects a particle.
  void affect(u32 now, SParticle* particlearray, u32 count);
  //! Sets the targetColor, i.e. the color the particles will interpolate
  //! to over time.
  void setTargetColor(const SColor& targetColor) {
    TargetColor = targetColor;
  }
  //! Sets the amount of time it takes for each particle to fade out.
  void setFadeOutTime(u32 fadeOutTime) {
    FadeOutTime = fadeOutTime ? static_cast<f32>(fadeOutTime) : 1.0f;
  }
  //! Sets the targetColor, i.e. the color the particles will interpolate
  //! to over time.
  const SColor& getTargetColor() const {
    return TargetColor;
  }
  //! Sets the amount of time it takes for each particle to fade out.
  u32 getFadeOutTime() const {
    return static_cast<u32>(FadeOutTime);
  }
  //! Writes attributes of the object.
  //! Implement this to expose the attributes of your scene node animator for
  //! scripting languages, editors, debuggers or xml serialization purposes.
  void serializeAttributes(IAttributes* out, SAttributeReadWriteOptions* options) const;
  //! Reads attributes of the object.
  //! Implement this to set the attributes of your scene node animator for
  //! scripting languages, editors, debuggers or xml deserialization purposes.
  //! \param startIndex: start index where to start reading attributes.
  //! \return: returns last index of an attribute read by this affector
  void deserializeAttributes(IAttributes* in, SAttributeReadWriteOptions* options);
private:
  SColor TargetColor;
  f32 FadeOutTime;
};
#endif

