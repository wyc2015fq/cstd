// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
//#include "CParticleRotationAffector.h"
//#include "IAttributes.h"
//! constructor
CParticleRotationAffector::CParticleRotationAffector(const float3 speed, const float3 pivotPoint)
  : PivotPoint(pivotPoint), Speed(speed), LastTime(0)
{
#ifdef _DEBUG
  setDebugName("CParticleRotationAffector");
#endif
}
//! Affects an array of particles.
void CParticleRotationAffector::affect(u32 now, SParticle* particlearray, u32 count)
{
  if (LastTime == 0) {
    LastTime = now;
    return;
  }
  f32 timeDelta = (now - LastTime) / 1000.0f;
  LastTime = now;
  if (!Enabled) {
    return;
  }
  int i;
  for (i = 0; i < count; ++i) {
    if (Speed.X != 0.0f) {
      particlearray[i].pos.rotateYZBy(timeDelta * Speed.X, PivotPoint);
    }
    if (Speed.Y != 0.0f) {
      particlearray[i].pos.rotateXZBy(timeDelta * Speed.Y, PivotPoint);
    }
    if (Speed.Z != 0.0f) {
      particlearray[i].pos.rotateXYBy(timeDelta * Speed.Z, PivotPoint);
    }
  }
}
//! Writes attributes of the object.
void CParticleRotationAffector::serializeAttributes(IAttributes* out, SAttributeReadWriteOptions* options) const
{
  out->addVector3d("PivotPoint", PivotPoint);
  out->addVector3d("Speed", Speed);
}
//! Reads attributes of the object.
void CParticleRotationAffector::deserializeAttributes(IAttributes* in, SAttributeReadWriteOptions* options)
{
  PivotPoint = in->getAttributeAsVector3d("PivotPoint");
  Speed = in->getAttributeAsVector3d("Speed");
}

