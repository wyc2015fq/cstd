// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
#ifndef __C_GEOMETRY_CREATOR_H_INCLUDED__
#define __C_GEOMETRY_CREATOR_H_INCLUDED__
//#include "IGeometryCreator.h"
//#include "SMeshBuffer.h"
//! class for creating geometry on the fly
class CGeometryCreator : public IGeometryCreator
{
  void addToBuffer(const S3DVertex& v, SMeshBuffer* Buffer) const;
public:
  IMesh* createCubeMesh(const float3 size) const;
  IMesh* createHillPlaneMesh(
      const dimension2df& tileSize, const dimension2du& tileCount,
      SMaterial* material, f32 hillHeight, const dimension2df& countHills,
      const dimension2df& textureRepeatCount) const;
  IMesh* createTerrainMesh(IImage* texture,
      IImage* heightmap, const dimension2df& stretchSize,
      f32 maxHeight, IVideoDriver* driver,
      const dimension2du& defaultVertexBlockSize,
      bool debugBorders = false) const;
  IMesh* createArrowMesh(const u32 tesselationCylinder,
      const u32 tesselationCone, const f32 height,
      const f32 cylinderHeight, const f32 width0,
      const f32 width1, const SColor vtxColor0,
      const SColor vtxColor1) const;
  IMesh* createSphereMesh(f32 radius, u32 polyCountX, u32 polyCountY) const;
  IMesh* createCylinderMesh(f32 radius, f32 length, u32 tesselation,
      const SColor& color = 0xffffffff,
      bool closeTop = true, f32 oblique = 0.f) const;
  IMesh* createConeMesh(f32 radius, f32 length, u32 tesselation,
      const SColor& colorTop = 0xffffffff,
      const SColor& colorBottom = 0xffffffff,
      f32 oblique = 0.f) const;
  IMesh* createVolumeLightMesh(
      const u32 subdivideU = 32, const u32 subdivideV = 32,
      const SColor footColor = 0xffffffff,
      const SColor tailColor = 0xffffffff,
      const f32 lpDistance = 8.f,
      const float3 lightDim = f3VECTOR(1.f, 1.2f, 1.f)) const;
};
#endif

