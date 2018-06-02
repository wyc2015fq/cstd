// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
//#include "CTriangleSelector.h"
//#include "ISceneNode.h"
//#include "IAnimatedMeshSceneNode.h"
//! constructor
CTriangleSelector::CTriangleSelector(ISceneNode* node)
  : SceneNode(node), AnimatedNode(0), LastMeshFrame(0)
{
#ifdef _DEBUG
  setDebugName("CTriangleSelector");
#endif
  BoundingBox.reset(0.f, 0.f, 0.f);
}
//! constructor
CTriangleSelector::CTriangleSelector(const F3AABBOX& box, ISceneNode* node)
  : SceneNode(node), AnimatedNode(0), LastMeshFrame(0)
{
#ifdef _DEBUG
  setDebugName("CTriangleSelector");
#endif
  BoundingBox = box;
  // TODO
}
//! constructor
CTriangleSelector::CTriangleSelector(const IMesh* mesh, ISceneNode* node)
  : SceneNode(node), AnimatedNode(0), LastMeshFrame(0)
{
#ifdef _DEBUG
  setDebugName("CTriangleSelector");
#endif
  createFromMesh(mesh);
}
CTriangleSelector::CTriangleSelector(IAnimatedMeshSceneNode* node)
  : SceneNode(node), AnimatedNode(node), LastMeshFrame(0)
{
#ifdef _DEBUG
  setDebugName("CTriangleSelector");
#endif
  if (!AnimatedNode) {
    return;
  }
  IAnimatedMesh* animatedMesh = AnimatedNode->getMesh();
  if (!animatedMesh) {
    return;
  }
  LastMeshFrame = (u32)AnimatedNode->getFrameNr();
  IMesh* mesh = animatedMesh->getMesh(LastMeshFrame);
  if (mesh) {
    createFromMesh(mesh);
  }
}
void CTriangleSelector::createFromMesh(const IMesh* mesh)
{
  const u32 cnt = mesh->getMeshBufferCount();
  u32 totalFaceCount = 0;
  for (int j = 0; j < cnt; ++j) {
    totalFaceCount += mesh->getMeshBuffer(j)->getIndexCount();
  }
  totalFaceCount /= 3;
  Triangles.reallocate(totalFaceCount);
  BoundingBox.reset(0.f, 0.f, 0.f);
  int i;
  for (i = 0; i < cnt; ++i) {
    const IMeshBuffer* buf = mesh->getMeshBuffer(i);
    const u32 idxCnt = buf->getIndexCount();
    const u16* const indices = buf->getIndices();
    for (int j = 0; j < idxCnt; j += 3) {
      Triangles.push_back(triangle3df(
          buf->getPosition(indices[j + 0]),
          buf->getPosition(indices[j + 1]),
          buf->getPosition(indices[j + 2])));
      const triangle3df& tri = Triangles.getLast();
      BoundingBox.addInternalPoint(tri.pointA);
      BoundingBox.addInternalPoint(tri.pointB);
      BoundingBox.addInternalPoint(tri.pointC);
    }
  }
}
void CTriangleSelector::updateFromMesh(const IMesh* mesh) const
{
  if (!mesh) {
    return;
  }
  u32 meshBuffers = mesh->getMeshBufferCount();
  u32 triangleCount = 0;
  BoundingBox.reset(0.f, 0.f, 0.f);
  int i;
  for (i = 0; i < meshBuffers; ++i) {
    IMeshBuffer* buf = mesh->getMeshBuffer(i);
    u32 idxCnt = buf->getIndexCount();
    const u16* indices = buf->getIndices();
    for (int index = 0; index < idxCnt; index += 3) {
      triangle3df& tri = Triangles[triangleCount++];
      tri.pointA = buf->getPosition(indices[index + 0]);
      tri.pointB = buf->getPosition(indices[index + 1]);
      tri.pointC = buf->getPosition(indices[index + 2]);
      BoundingBox.addInternalPoint(tri.pointA);
      BoundingBox.addInternalPoint(tri.pointB);
      BoundingBox.addInternalPoint(tri.pointC);
    }
  }
}
void CTriangleSelector::update(void) const
{
  if (!AnimatedNode) {
    return;  //< harmless no-op
  }
  const u32 currentFrame = (u32)AnimatedNode->getFrameNr();
  if (currentFrame == LastMeshFrame) {
    return;  //< Nothing to do
  }
  LastMeshFrame = currentFrame;
  IAnimatedMesh* animatedMesh = AnimatedNode->getMesh();
  if (animatedMesh) {
    IMesh* mesh = animatedMesh->getMesh(LastMeshFrame);
    if (mesh) {
      updateFromMesh(mesh);
    }
  }
}
//! Gets all triangles.
void CTriangleSelector::getTriangles(triangle3df* triangles,
    s32 arraySize, s32& outTriangleCount,
    const matrix4* transform) const
{
  // Update my triangles if necessary
  update();
  u32 cnt = Triangles_size;
  if (cnt > (u32)arraySize) {
    cnt = (u32)arraySize;
  }
  matrix4 mat;
  if (transform) {
    mat = *transform;
  }
  if (SceneNode) {
    mat *= SceneNode->getAbsoluteTransformation();
  }
  int i;
  for (i = 0; i < cnt; ++i) {
    mat.transformVect(triangles[i].pointA, Triangles[i].pointA);
    mat.transformVect(triangles[i].pointB, Triangles[i].pointB);
    mat.transformVect(triangles[i].pointC, Triangles[i].pointC);
  }
  outTriangleCount = cnt;
}
//! Gets all triangles which lie within a specific bounding box.
void CTriangleSelector::getTriangles(triangle3df* triangles,
    s32 arraySize, s32& outTriangleCount,
    const F3AABBOX& box,
    const matrix4* transform) const
{
  // Update my triangles if necessary
  update();
  matrix4 mat(matrix4::EM4CONST_NOTHING);
  aabbox3df tBox(box);
  if (SceneNode) {
    SceneNode->getAbsoluteTransformation().getInverse(mat);
    mat.transformBoxEx(tBox);
  }
  if (transform) {
    mat = *transform;
  }
  else {
    mat.makeIdentity();
  }
  if (SceneNode) {
    mat *= SceneNode->getAbsoluteTransformation();
  }
  outTriangleCount = 0;
  if (!tBox.intersectsWithBox(BoundingBox)) {
    return;
  }
  s32 triangleCount = 0;
  const u32 cnt = Triangles_size;
  int i;
  for (i = 0; i < cnt; ++i) {
    // This isn't an accurate test, but it's fast, and the
    // API contract doesn't guarantee complete accuracy.
    if (Triangles[i].isTotalOutsideBox(tBox)) {
      continue;
    }
    triangles[triangleCount] = Triangles[i];
    mat.transformVect(triangles[triangleCount].pointA);
    mat.transformVect(triangles[triangleCount].pointB);
    mat.transformVect(triangles[triangleCount].pointC);
    ++triangleCount;
    if (triangleCount == arraySize) {
      break;
    }
  }
  outTriangleCount = triangleCount;
}
//! Gets all triangles which have or may have contact with a 3d line.
void CTriangleSelector::getTriangles(triangle3df* triangles,
    s32 arraySize, s32& outTriangleCount,
    const F3LINE& line,
    const matrix4* transform) const
{
  // Update my triangles if necessary
  update();
  F3AABBOX box(line.start);
  box.addInternalPoint(line.end);
  // TODO: Could be optimized for line a little bit more.
  getTriangles(triangles, arraySize, outTriangleCount,
      box, transform);
}
//! Returns amount of all available triangles in this selector
s32 CTriangleSelector::getTriangleCount() const
{
  return Triangles_size;
}
/* Get the number of TriangleSelectors that are part of this one.
Only useful for MetaTriangleSelector others return 1
*/
u32 CTriangleSelector::getSelectorCount() const
{
  return 1;
}
/* Get the TriangleSelector based on index based on getSelectorCount.
Only useful for MetaTriangleSelector others return 'this' or 0
*/
ITriangleSelector* CTriangleSelector::getSelector(u32 index)
{
  if (index) {
    return 0;
  }
  else {
    return this;
  }
}
/* Get the TriangleSelector based on index based on getSelectorCount.
Only useful for MetaTriangleSelector others return 'this' or 0
*/
const ITriangleSelector* CTriangleSelector::getSelector(u32 index) const
{
  if (index) {
    return 0;
  }
  else {
    return this;
  }
}

