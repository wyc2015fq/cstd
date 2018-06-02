// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
//#include "CBillboardSceneNode.h"
//#include "IVideoDriver.h"
//#include "ISceneManager.h"
//#include "ICameraSceneNode.h"
//#include "os.h"
//! constructor
CBillboardSceneNode::CBillboardSceneNode(ISceneNode* parent, ISceneManager* mgr, s32 id,
    const float3 position, const dimension2df& size,
    SColor colorTop, SColor colorBottom)
  : IBillboardSceneNode(parent, mgr, id, position)
{
#ifdef _DEBUG
  setDebugName("CBillboardSceneNode");
#endif
  setSize(size);
  indices[0] = 0;
  indices[1] = 2;
  indices[2] = 1;
  indices[3] = 0;
  indices[4] = 3;
  indices[5] = 2;
  vertices[0].TCoords.set(1.0f, 1.0f);
  vertices[0].Color = colorBottom;
  vertices[1].TCoords.set(1.0f, 0.0f);
  vertices[1].Color = colorTop;
  vertices[2].TCoords.set(0.0f, 0.0f);
  vertices[2].Color = colorTop;
  vertices[3].TCoords.set(0.0f, 1.0f);
  vertices[3].Color = colorBottom;
}
//! pre render event
void CBillboardSceneNode::OnRegisterSceneNode()
{
  if (IsVisible) {
    SceneManager->registerNodeForRendering(this);
  }
  ISceneNode::OnRegisterSceneNode();
}
//! render
void CBillboardSceneNode::render()
{
  IVideoDriver* driver = SceneManager->getVideoDriver();
  ICameraSceneNode* camera = SceneManager->getActiveCamera();
  if (!camera || !driver) {
    return;
  }
  // make billboard look to camera
  float3 pos = getAbsolutePosition();
  float3 campos = camera->getAbsolutePosition();
  float3 target = camera->getTarget();
  float3 up = camera->getUpVector();
  float3 view = target - campos;
  view.normalize();
  float3 horizontal = up.crossProduct(view);
  if (horizontal.getLength() == 0) {
    horizontal.set(up.Y, up.X, up.Z);
  }
  horizontal.normalize();
  float3 topHorizontal = horizontal * 0.5f * TopEdgeWidth;
  horizontal *= 0.5f * Size.Width;
  // pointing down!
  float3 vertical = horizontal.crossProduct(view);
  vertical.normalize();
  vertical *= 0.5f * Size.Height;
  view *= -1.0f;
  for (s32 i = 0; i < 4; ++i) {
    vertices[i].Normal = view;
  }
  /* Vertices are:
  2--1
  |\ |
  | \|
  3--0
  */
  vertices[0].Pos = pos + horizontal + vertical;
  vertices[1].Pos = pos + topHorizontal - vertical;
  vertices[2].Pos = pos - topHorizontal - vertical;
  vertices[3].Pos = pos - horizontal + vertical;
  // draw
  if (DebugDataVisible & EDS_BBOX) {
    driver->setTransform(ETS_WORLD, AbsoluteTransformation);
    SMaterial m;
    m.Lighting = false;
    driver->setMaterial(m);
    driver->draw3DBox(BBox, SColor(0, 208, 195, 152));
  }
  driver->setTransform(ETS_WORLD, IdentityMatrix);
  driver->setMaterial(Material);
  driver->drawIndexedTriangleList(vertices, 4, indices, 2);
}
//! returns the axis aligned bounding box of this node
const F3AABBOX& CBillboardSceneNode::getBoundingBox() const
{
  return BBox;
}
//! sets the size of the billboard
void CBillboardSceneNode::setSize(const dimension2df& size)
{
  Size = size;
  if (equals(Size.Width, 0.0f)) {
    Size.Width = 1.0f;
  }
  TopEdgeWidth = Size.Width;
  if (equals(Size.Height, 0.0f)) {
    Size.Height = 1.0f;
  }
  const f32 avg = (Size.Width + Size.Height) / 6;
  BBox.MinEdge.set(-avg, -avg, -avg);
  BBox.MaxEdge.set(avg, avg, avg);
}
void CBillboardSceneNode::setSize(f32 height, f32 bottomEdgeWidth, f32 topEdgeWidth)
{
  Size.set(bottomEdgeWidth, height);
  TopEdgeWidth = topEdgeWidth;
  if (equals(Size.Height, 0.0f)) {
    Size.Height = 1.0f;
  }
  if (equals(Size.Width, 0.f) && equals(TopEdgeWidth, 0.f)) {
    Size.Width = 1.0f;
    TopEdgeWidth = 1.0f;
  }
  const f32 avg = (max_(Size.Width, TopEdgeWidth) + Size.Height) / 6;
  BBox.MinEdge.set(-avg, -avg, -avg);
  BBox.MaxEdge.set(avg, avg, avg);
}
SMaterial& CBillboardSceneNode::getMaterial(u32 i)
{
  return Material;
}
//! returns amount of materials used by this scene node.
u32 CBillboardSceneNode::getMaterialCount() const
{
  return 1;
}
//! gets the size of the billboard
const dimension2df& CBillboardSceneNode::getSize() const
{
  return Size;
}
//! Gets the widths of the top and bottom edges of the billboard.
void CBillboardSceneNode::getSize(f32& height, f32& bottomEdgeWidth,
    f32& topEdgeWidth) const
{
  height = Size.Height;
  bottomEdgeWidth = Size.Width;
  topEdgeWidth = TopEdgeWidth;
}
//! Writes attributes of the scene node.
void CBillboardSceneNode::serializeAttributes(IAttributes* out, SAttributeReadWriteOptions* options) const
{
  IBillboardSceneNode::serializeAttributes(out, options);
  out->addFloat("Width", Size.Width);
  out->addFloat("TopEdgeWidth", TopEdgeWidth);
  out->addFloat("Height", Size.Height);
  out->addColor("Shade_Top", vertices[1].Color);
  out->addColor("Shade_Down", vertices[0].Color);
}
//! Reads attributes of the scene node.
void CBillboardSceneNode::deserializeAttributes(IAttributes* in, SAttributeReadWriteOptions* options)
{
  IBillboardSceneNode::deserializeAttributes(in, options);
  Size.Width = in->getAttributeAsFloat("Width");
  Size.Height = in->getAttributeAsFloat("Height");
  if (in->existsAttribute("TopEdgeWidth")) {
    TopEdgeWidth = in->getAttributeAsFloat("TopEdgeWidth");
    if (Size.Width != TopEdgeWidth) {
      setSize(Size.Height, Size.Width, TopEdgeWidth);
    }
  }
  else {
    setSize(Size);
  }
  vertices[1].Color = in->getAttributeAsColor("Shade_Top");
  vertices[0].Color = in->getAttributeAsColor("Shade_Down");
  vertices[2].Color = vertices[1].Color;
  vertices[3].Color = vertices[0].Color;
}
//! Set the color of all vertices of the billboard
//! \param overallColor: the color to set
void CBillboardSceneNode::setColor(const SColor& overallColor)
{
  for (int vertex = 0; vertex < 4; ++vertex) {
    vertices[vertex].Color = overallColor;
  }
}
//! Set the color of the top and bottom vertices of the billboard
//! \param topColor: the color to set the top vertices
//! \param bottomColor: the color to set the bottom vertices
void CBillboardSceneNode::setColor(const SColor& topColor,
    const SColor& bottomColor)
{
  vertices[0].Color = bottomColor;
  vertices[1].Color = topColor;
  vertices[2].Color = topColor;
  vertices[3].Color = bottomColor;
}
//! Gets the color of the top and bottom vertices of the billboard
//! \param[out] topColor: stores the color of the top vertices
//! \param[out] bottomColor: stores the color of the bottom vertices
void CBillboardSceneNode::getColor(SColor& topColor,
    SColor& bottomColor) const
{
  bottomColor = vertices[0].Color;
  topColor = vertices[1].Color;
}
//! Creates a clone of this scene node and its children.
ISceneNode* CBillboardSceneNode::clone(ISceneNode* newParent, ISceneManager* newManager)
{
  if (!newParent) {
    newParent = Parent;
  }
  if (!newManager) {
    newManager = SceneManager;
  }
  CBillboardSceneNode* nb = new CBillboardSceneNode(newParent,
      newManager, ID, RelativeTranslation, Size);
  nb->cloneMembers(this, newManager);
  nb->Material = Material;
  nb->TopEdgeWidth = this->TopEdgeWidth;
  if (newParent) {
    nb->drop();
  }
  return nb;
}

