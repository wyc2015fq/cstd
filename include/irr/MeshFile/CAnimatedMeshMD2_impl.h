// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
#ifdef _IRR_COMPILE_WITH_MD2_LOADER_
//#include "CAnimatedMeshMD2.h"
//#include "irrMath.h"
//! constructor
CAnimatedMeshMD2::CAnimatedMeshMD2()
  : InterpolationBuffer(0), FrameList(0), FrameCount(0), FramesPerSecond((f32)(MD2AnimationTypeList[0].fps << MD2_FRAME_SHIFT))
{
#ifdef _DEBUG
  IAnimatedMesh::setDebugName("CAnimatedMeshMD2 IAnimatedMesh");
  IMesh::setDebugName("CAnimatedMeshMD2 IMesh");
#endif
  InterpolationBuffer = new SMeshBuffer;
}
//! destructor
CAnimatedMeshMD2::~CAnimatedMeshMD2()
{
  delete [] FrameList;
  if (InterpolationBuffer) {
    InterpolationBuffer->drop();
  }
}
//! returns the amount of frames in milliseconds. If the amount is 1, it is a static (=non animated) mesh.
u32 CAnimatedMeshMD2::getFrameCount() const
{
  return FrameCount << MD2_FRAME_SHIFT;
}
//! returns the animated mesh based on a detail level. 0 is the lowest, 255 the highest detail. Note, that some Meshes will ignore the detail level.
IMesh* CAnimatedMeshMD2::getMesh(s32 frame, s32 detailLevel, s32 startFrameLoop, s32 endFrameLoop)
{
  if ((u32)frame > getFrameCount()) {
    frame = (frame % getFrameCount());
  }
  if (startFrameLoop == -1 && endFrameLoop == -1) {
    startFrameLoop = 0;
    endFrameLoop = getFrameCount();
  }
  updateInterpolationBuffer(frame, startFrameLoop, endFrameLoop);
  return this;
}
//! returns amount of mesh buffers. MD2 meshes only have one buffer
u32 CAnimatedMeshMD2::getMeshBufferCount() const
{
  return 1;
}
//! returns pointer to a mesh buffer
IMeshBuffer* CAnimatedMeshMD2::getMeshBuffer(u32 nr) const
{
  if (nr == 0) {
    return InterpolationBuffer;
  }
  else {
    return 0;
  }
}
//! Returns pointer to a mesh buffer which fits a material
IMeshBuffer* CAnimatedMeshMD2::getMeshBuffer(const SMaterial& material) const
{
  if (InterpolationBuffer->Material == material) {
    return InterpolationBuffer;
  }
  else {
    return 0;
  }
}
// updates the interpolation buffer
void CAnimatedMeshMD2::updateInterpolationBuffer(s32 frame, s32 startFrameLoop, s32 endFrameLoop)
{
  u32 firstFrame, secondFrame;
  f32 div;
  // TA: resolve missing ipol in loop between end-start
  if (endFrameLoop - startFrameLoop == 0) {
    firstFrame = frame >> MD2_FRAME_SHIFT;
    secondFrame = frame >> MD2_FRAME_SHIFT;
    div = 1.0f;
  }
  else {
    // key frames
    u32 s = startFrameLoop >> MD2_FRAME_SHIFT;
    u32 e = endFrameLoop >> MD2_FRAME_SHIFT;
    firstFrame = frame >> MD2_FRAME_SHIFT;
    secondFrame = if_c_a_else_b(firstFrame + 1 > e, s, firstFrame + 1);
    firstFrame = s32_min(FrameCount - 1, firstFrame);
    secondFrame = s32_min(FrameCount - 1, secondFrame);
    //div = (frame % (1<<MD2_FRAME_SHIFT)) / (f32)(1<<MD2_FRAME_SHIFT);
    frame &= (1 << MD2_FRAME_SHIFT) - 1;
    div = frame * MD2_FRAME_SHIFT_RECIPROCAL;
  }
  S3DVertex* target = static_cast<S3DVertex*>(InterpolationBuffer->getVertices());
  SMD2Vert* first = FrameList[firstFrame];
  SMD2Vert* second = FrameList[secondFrame];
  // interpolate both frames
  const u32 count = FrameList[firstFrame]_size;
  int i;
  for (i = 0; i < count; ++i) {
    const vector3df one = f3VECTOR(f32(first->Pos.X) * FrameTransforms[firstFrame].scale.X + FrameTransforms[firstFrame].translate.X,
        f32(first->Pos.Y) * FrameTransforms[firstFrame].scale.Y + FrameTransforms[firstFrame].translate.Y,
        f32(first->Pos.Z) * FrameTransforms[firstFrame].scale.Z + FrameTransforms[firstFrame].translate.Z);
    const vector3df two = f3VECTOR(f32(second->Pos.X) * FrameTransforms[secondFrame].scale.X + FrameTransforms[secondFrame].translate.X,
        f32(second->Pos.Y) * FrameTransforms[secondFrame].scale.Y + FrameTransforms[secondFrame].translate.Y,
        f32(second->Pos.Z) * FrameTransforms[secondFrame].scale.Z + FrameTransforms[secondFrame].translate.Z);
    target->Pos = two.getInterpolated(one, div);
    const vector3df n1(
        Q2_VERTEX_NORMAL_TABLE[first->NormalIdx][0],
        Q2_VERTEX_NORMAL_TABLE[first->NormalIdx][2],
        Q2_VERTEX_NORMAL_TABLE[first->NormalIdx][1]);
    const vector3df n2(
        Q2_VERTEX_NORMAL_TABLE[second->NormalIdx][0],
        Q2_VERTEX_NORMAL_TABLE[second->NormalIdx][2],
        Q2_VERTEX_NORMAL_TABLE[second->NormalIdx][1]);
    target->Normal = n2.getInterpolated(n1, div);
    ++target;
    ++first;
    ++second;
  }
  //update bounding box
  InterpolationBuffer->setBoundingBox(BoxList[secondFrame].getInterpolated(BoxList[firstFrame], div));
  InterpolationBuffer->setDirty();
}
//! sets a flag of all contained materials to a new value
void CAnimatedMeshMD2::setMaterialFlag(E_MATERIAL_FLAG flag, bool newvalue)
{
  InterpolationBuffer->Material.setFlag(flag, newvalue);
}
//! set the hardware mapping hint, for driver
void CAnimatedMeshMD2::setHardwareMappingHint(E_HARDWARE_MAPPING newMappingHint,
    E_BUFFER_TYPE buffer)
{
  InterpolationBuffer->setHardwareMappingHint(newMappingHint, buffer);
}
//! flags the meshbuffer as changed, reloads hardware buffers
void CAnimatedMeshMD2::setDirty(E_BUFFER_TYPE buffer)
{
  InterpolationBuffer->setDirty(buffer);
}
//! returns an axis aligned bounding box
const F3AABBOX& CAnimatedMeshMD2::getBoundingBox() const
{
  return InterpolationBuffer->BoundingBox;
}
//! set user axis aligned bounding box
void CAnimatedMeshMD2::setBoundingBox(const aabbox3df& box)
{
  InterpolationBuffer->BoundingBox = box;
}
//! Returns the type of the animated mesh.
E_ANIMATED_MESH_TYPE CAnimatedMeshMD2::getMeshType() const
{
  return EAMT_MD2;
}
//! Returns frame loop data for a special MD2 animation type.
void CAnimatedMeshMD2::getFrameLoop(EMD2_ANIMATION_TYPE l,
    s32& outBegin, s32& outEnd, s32& outFPS) const
{
  if (l < 0 || l >= EMAT_COUNT) {
    return;
  }
  outBegin = MD2AnimationTypeList[l].begin << MD2_FRAME_SHIFT;
  outEnd = MD2AnimationTypeList[l].end << MD2_FRAME_SHIFT;
  // correct to anim between last->first frame
  outEnd += MD2_FRAME_SHIFT == 0 ? 1 : (1 << MD2_FRAME_SHIFT) - 1;
  outFPS = MD2AnimationTypeList[l].fps << MD2_FRAME_SHIFT;
}
//! Returns frame loop data for a special MD2 animation type.
bool CAnimatedMeshMD2::getFrameLoop(const c8* name,
    s32& outBegin, s32& outEnd, s32& outFPS) const
{
  int i;
  for (i = 0; i < AnimationData_size; ++i) {
    if (AnimationData[i].name == name) {
      outBegin = AnimationData[i].begin << MD2_FRAME_SHIFT;
      outEnd = AnimationData[i].end << MD2_FRAME_SHIFT;
      outEnd += MD2_FRAME_SHIFT == 0 ? 1 : (1 << MD2_FRAME_SHIFT) - 1;
      outFPS = AnimationData[i].fps << MD2_FRAME_SHIFT;
      return true;
    }
  }
  return false;
}
//! Returns amount of md2 animations in this file.
s32 CAnimatedMeshMD2::getAnimationCount() const
{
  return AnimationData_size;
}
//! Returns name of md2 animation.
const c8* CAnimatedMeshMD2::getAnimationName(s32 nr) const
{
  if ((u32)nr >= AnimationData_size) {
    return 0;
  }
  return AnimationData[nr].name;
}
#endif // _IRR_COMPILE_WITH_MD2_LOADER_

