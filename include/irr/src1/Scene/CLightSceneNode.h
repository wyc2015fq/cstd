// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
#ifndef __C_LIGHT_SCENE_NODE_H_INCLUDED__
#define __C_LIGHT_SCENE_NODE_H_INCLUDED__
//#include "ILightSceneNode.h"
//! Scene node which is a dynamic light. You can switch the light on and off by
//! making it visible or not, and let it be animated by ordinary scene node animators.
class CLightSceneNode : public ILightSceneNode
{
public:
  //! constructor
  CLightSceneNode(ISceneNode* parent, ISceneManager* mgr, s32 id,
      const float3 position, SColorf color, f32 range);
  ~CLightSceneNode() { }
  //! pre render event
  void OnRegisterSceneNode();
  //! render
  void render();
  //! set node light data from light info
  void setLightData(const SLight& light);
  //! \return Returns the light data.
  const SLight& getLightData() const;
  //! \return Returns the light data.
  SLight& getLightData();
  //! Sets if the node should be visible or not.
  /** All children of this node won't be visible either, when set
  to true.
  \param isVisible If the node shall be visible. */
  void setVisible(bool isVisible);
  //! returns the axis aligned bounding box of this node
  const F3AABBOX& getBoundingBox() const;
  //! Returns type of the scene node
  ESCENE_NODE_TYPE getType() const {
    return ESNT_LIGHT;
  }
  //! Writes attributes of the scene node.
  void serializeAttributes(IAttributes* out, SAttributeReadWriteOptions* options = 0) const;
  //! Reads attributes of the scene node.
  void deserializeAttributes(IAttributes* in, SAttributeReadWriteOptions* options = 0);
  //! Creates a clone of this scene node and its children.
  ISceneNode* clone(ISceneNode* newParent = 0, ISceneManager* newManager = 0);
  //! Sets the light's radius of influence.
  /** Outside this radius the light won't lighten geometry and cast no
  shadows. Setting the radius will also influence the attenuation, setting
  it to (0,1/radius,0). If you want to override this behavior, set the
  attenuation after the radius.
  \param radius The new radius. */
  void setRadius(f32 radius);
  //! Gets the light's radius of influence.
  /** \return The current radius. */
  f32 getRadius() const;
  //! Sets the light type.
  /** \param type The new type. */
  void setLightType(E_LIGHT_TYPE type);
  //! Gets the light type.
  /** \return The current light type. */
  E_LIGHT_TYPE getLightType() const;
  //! Sets whether this light casts shadows.
  /** Enabling this flag won't automatically cast shadows, the meshes
  will still need shadow scene nodes attached. But one can enable or
  disable distinct lights for shadow casting for performance reasons.
  \param shadow True if this light shall cast shadows. */
  void enableCastShadow(bool shadow = true);
  //! Check whether this light casts shadows.
  /** \return True if light would cast shadows, else false. */
  bool getCastShadow() const;
private:
  SLight LightData;
  F3AABBOX BBox;
  s32 DriverLightIndex;
  bool LightIsOn;
  void doLightRecalc();
};
#endif

