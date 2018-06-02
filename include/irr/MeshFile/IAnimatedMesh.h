// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
#ifndef __I_ANIMATED_MESH_H_INCLUDED__
#define __I_ANIMATED_MESH_H_INCLUDED__
//! Interface for an animated mesh.
/** There are already simple implementations of this interface available so
you don't have to implement this interface on your own if you need to:
You might want to use SAnimatedMesh, SMesh,
SMeshBuffer etc. */
class IAnimatedMesh : public IMesh
{
public:
  //! Gets the frame count of the animated mesh.
  /** \return The amount of frames. If the amount is 1,
  it is a static, non animated mesh. */
  
  //! Gets the animation speed of the animated mesh.
  /** \return The number of frames per second to play the
  animation with by default. If the amount is 0,
  it is a static, non animated mesh. */

  //! Sets the animation speed of the animated mesh.
  /** \param fps Number of frames per second to play the
  animation with by default. If the amount is 0,
  it is not animated. The actual speed is set in the
  scene node the mesh is instantiated in.*/
  
  //! Returns the IMesh interface for a frame.
  /** \param frame: Frame number as zero based index. The maximum
  frame number is getFrameCount() - 1;
  \param detailLevel: Level of detail. 0 is the lowest, 255 the
  highest level of detail. Most meshes will ignore the detail level.
  \param startFrameLoop: Because some animated meshes (.MD2) are
  blended between 2 static frames, and maybe animated in a loop,
  the startFrameLoop and the endFrameLoop have to be defined, to
  prevent the animation to be blended between frames which are
  outside of this loop.
  If startFrameLoop and endFrameLoop are both -1, they are ignored.
  \param endFrameLoop: see startFrameLoop.
  \return Returns the animated mesh based on a detail level. */
  
  //! Returns the type of the animated mesh.
  /** In most cases it is not neccessary to use this method.
  This is useful for making a safe downcast. For example,
  if getMeshType() returns EAMT_MD2 it's safe to cast the
  IAnimatedMesh to IAnimatedMeshMD2.
  \returns Type of the mesh. */
  E_ANIMATED_MESH_TYPE getMeshType() const {
    return EAMT_UNKNOWN;
  }
};
#endif

