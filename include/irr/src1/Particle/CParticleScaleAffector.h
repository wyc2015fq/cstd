// Copyright (C) 2010-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
#ifndef C_PARTICLE_SCALE_AFFECTOR_H
#define C_PARTICLE_SCALE_AFFECTOR_H
//#include "IParticleAffector.h"
class CParticleScaleAffector : public IParticleAffector
{
public:
  CParticleScaleAffector(const dimension2df& scaleTo = dimension2df(1.0f, 1.0f));
  void affect(u32 now, SParticle* particlearray, u32 count);
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
  //! Get emitter type
  E_PARTICLE_AFFECTOR_TYPE getType() const;
protected:
  dimension2df ScaleTo;
};
}
}
#endif // C_PARTICLE_SCALE_AFFECTOR_H

