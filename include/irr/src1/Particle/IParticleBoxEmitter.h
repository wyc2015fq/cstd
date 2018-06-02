// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
#ifndef __I_PARTICLE_BOX_EMITTER_H_INCLUDED__
#define __I_PARTICLE_BOX_EMITTER_H_INCLUDED__
//#include "IParticleEmitter.h"
//! A particle emitter which emits particles from a box shaped space
class IParticleBoxEmitter : public IParticleEmitter
{
public:
  //! Set the box shape
  void setBox(const aabbox3df& box) = 0;
  //! Get the box shape set
  const aabbox3df& getBox() = 0;
  //! Get emitter type
  E_PARTICLE_EMITTER_TYPE getType() const {
    return EPET_BOX;
  }
};
#endif

