// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
#ifndef __I_CAMERA_SCENE_NODE_H_INCLUDED__
#define __I_CAMERA_SCENE_NODE_H_INCLUDED__
//#include "ISceneNode.h"
//#include "IEventReceiver.h"
struct SViewFrustum;
//! Scene Node which is a (controlable) camera.
/** The whole scene will be rendered from the cameras point of view.
Because the ICameraScenNode is a SceneNode, it can be attached to any
other scene node, and will follow its parents movement, rotation and so
on.
*/
class ICameraSceneNode : public ISceneNode, public IEventReceiver
{
public:
  //! Constructor
  ICameraSceneNode(ISceneNode* parent, ISceneManager* mgr, s32 id,
      const float3 position = float3(0, 0, 0),
      const float3 rotation = float3(0, 0, 0),
      const float3 scale = float3(1.0f, 1.0f, 1.0f))
    : ISceneNode(parent, mgr, id, position, rotation, scale), IsOrthogonal(false) {}
  //! Sets the projection matrix of the camera.
  /** The matrix4 class has some methods to build a
  projection matrix. e.g:
  matrix4::buildProjectionMatrixPerspectiveFovLH.
  Note that the matrix will only stay as set by this method until
  one of the following Methods are called: setNearValue,
  setFarValue, setAspectRatio, setFOV.
  \param projection The new projection matrix of the camera.
  \param isOrthogonal Set this to true if the matrix is an
  orthogonal one (e.g. from matrix4::buildProjectionMatrixOrtho).
  */
  void setProjectionMatrix(const matrix4& projection, bool isOrthogonal = false) = 0;
  //! Gets the current projection matrix of the camera.
  /** \return The current projection matrix of the camera. */
  const matrix4& getProjectionMatrix() = 0;
  //! Gets the current view matrix of the camera.
  /** \return The current view matrix of the camera. */
  const matrix4& getViewMatrix() = 0;
  //! Sets a custom view matrix affector.
  /** The matrix passed here, will be multiplied with the view
  matrix when it gets updated. This allows for custom camera
  setups like, for example, a reflection camera.
  \param affector The affector matrix. */
  void setViewMatrixAffector(const matrix4& affector) = 0;
  //! Get the custom view matrix affector.
  /** \return The affector matrix. */
  const matrix4& getViewMatrixAffector() = 0;
  //! It is possible to send mouse and key events to the camera.
  /** Most cameras may ignore this input, but camera scene nodes
  which are created for example with
  ISceneManager::addCameraSceneNodeMaya or
  ISceneManager::addCameraSceneNodeFPS, may want to get
  this input for changing their position, look at target or
  whatever. */
  bool OnEvent(const SEvent& event) = 0;
  //! Sets the look at target of the camera
  /** If the camera's target and rotation are bound ( @see
  bindTargetAndRotation() ) then calling this will also change
  the camera's scene node rotation to match the target.
  Note that setTarget uses the current absolute position
  internally, so if you changed setPosition since last rendering you must
  call updateAbsolutePosition before using this function.
  \param pos Look at target of the camera, in world co-ordinates. */
  void setTarget(const float3 pos) = 0;
  //! Sets the rotation of the node.
  /** This only modifies the relative rotation of the node.
  If the camera's target and rotation are bound ( @see
  bindTargetAndRotation() ) then calling this will also change
  the camera's target to match the rotation.
  \param rotation New rotation of the node in degrees. */
  void setRotation(const float3 rotation) = 0;
  //! Gets the current look at target of the camera
  /** \return The current look at target of the camera, in world co-ordinates */
  const float3 getTarget() = 0;
  //! Sets the up vector of the camera.
  /** \param pos: New upvector of the camera. */
  void setUpVector(const float3 pos) = 0;
  //! Gets the up vector of the camera.
  /** \return The up vector of the camera, in world space. */
  const float3 getUpVector() = 0;
  //! Gets the value of the near plane of the camera.
  /** \return The value of the near plane of the camera. */
  f32 getNearValue() = 0;
  //! Gets the value of the far plane of the camera.
  /** \return The value of the far plane of the camera. */
  f32 getFarValue() = 0;
  //! Gets the aspect ratio of the camera.
  /** \return The aspect ratio of the camera. */
  f32 getAspectRatio() = 0;
  //! Gets the field of view of the camera.
  /** \return The field of view of the camera in radians. */
  f32 getFOV() = 0;
  //! Sets the value of the near clipping plane. (default: 1.0f)
  /** \param zn: New z near value. */
  void setNearValue(f32 zn) = 0;
  //! Sets the value of the far clipping plane (default: 2000.0f)
  /** \param zf: New z far value. */
  void setFarValue(f32 zf) = 0;
  //! Sets the aspect ratio (default: 4.0f / 3.0f)
  /** \param aspect: New aspect ratio. */
  void setAspectRatio(f32 aspect) = 0;
  //! Sets the field of view (Default: PI / 2.5f)
  /** \param fovy: New field of view in radians. */
  void setFOV(f32 fovy) = 0;
  //! Get the view frustum.
  /** Needed sometimes by bspTree or LOD render nodes.
  \return The current view frustum. */
  const SViewFrustum* getViewFrustum() = 0;
  //! Disables or enables the camera to get key or mouse inputs.
  /** If this is set to true, the camera will respond to key
  inputs otherwise not. */
  void setInputReceiverEnabled(bool enabled) = 0;
  //! Checks if the input receiver of the camera is currently enabled.
  bool isInputReceiverEnabled() = 0;
  //! Checks if a camera is orthogonal.
  bool isOrthogonal() const {
    _IRR_IMPLEMENT_MANAGED_MARSHALLING_BUGFIX;
    return IsOrthogonal;
  }
  //! Binds the camera scene node's rotation to its target position and vice vera, or unbinds them.
  /** When bound, calling setRotation() will update the camera's
  target position to be along its +Z axis, and likewise calling
  setTarget() will update its rotation so that its +Z axis will
  point at the target point. FPS camera use this binding by
  default; other cameras do not.
  \param bound True to bind the camera's scene node rotation
  and targetting, false to unbind them.
  @see getTargetAndRotationBinding() */
  void bindTargetAndRotation(bool bound) = 0;
  //! Queries if the camera scene node's rotation and its target position are bound together.
  /** @see bindTargetAndRotation() */
  bool getTargetAndRotationBinding(void) = 0;
  //! Writes attributes of the camera node
  void serializeAttributes(IAttributes* out, SAttributeReadWriteOptions* options = 0) const {
    ISceneNode::serializeAttributes(out, options);
    if (!out) {
      return;
    }
    out->addBool("IsOrthogonal", IsOrthogonal);
  }
  //! Reads attributes of the camera node
  void deserializeAttributes(IAttributes* in, SAttributeReadWriteOptions* options = 0) {
    ISceneNode::deserializeAttributes(in, options);
    if (!in) {
      return;
    }
    if (in->findAttribute("IsOrthogonal")) {
      IsOrthogonal = in->getAttributeAsBool("IsOrthogonal");
    }
  }
protected:
  void cloneMembers(ICameraSceneNode* toCopyFrom) {
    IsOrthogonal = toCopyFrom->IsOrthogonal;
  }
  bool IsOrthogonal;
};
#endif

