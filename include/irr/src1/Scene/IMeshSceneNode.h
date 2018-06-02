// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
#ifndef __I_MESH_SCENE_NODE_H_INCLUDED__
#define __I_MESH_SCENE_NODE_H_INCLUDED__
//#include "ISceneNode.h"
class IShadowVolumeSceneNode;
class IMesh;
//! A scene node displaying a static mesh
class IMeshSceneNode : public ISceneNode
{
public:
  //! Constructor
  /** Use setMesh() to set the mesh to display.
  */
  IMeshSceneNode(ISceneNode* parent, ISceneManager* mgr, s32 id,
      const float3 position = float3(0, 0, 0),
      const float3 rotation = float3(0, 0, 0),
      const float3 scale = float3(1, 1, 1))
    : ISceneNode(parent, mgr, id, position, rotation, scale) {}
  //! Sets a new mesh to display
  /** \param mesh Mesh to display. */
  void setMesh(IMesh* mesh) = 0;
  //! Get the currently defined mesh for display.
  /** \return Pointer to mesh which is displayed by this node. */
  IMesh* getMesh(void) = 0;
  //! Creates shadow volume scene node as child of this node.
  /** The shadow can be rendered using the ZPass or the zfail
  method. ZPass is a little bit faster because the shadow volume
  creation is easier, but with this method there occur ugly
  looking artifacs when the camera is inside the shadow volume.
  These error do not occur with the ZFail method.
  \param shadowMesh: Optional custom mesh for shadow volume.
  \param id: Id of the shadow scene node. This id can be used to
  identify the node later.
  \param zfailmethod: If set to true, the shadow will use the
  zfail method, if not, zpass is used.
  \param infinity: Value used by the shadow volume algorithm to
  scale the shadow volume (for zfail shadow volume we support only
  finite shadows, so camera zfar must be larger than shadow back cap,
  which is depend on infinity parameter).
  \return Pointer to the created shadow scene node. This pointer
  should not be dropped. See IReferenceCounted::drop() for more
  information. */
  IShadowVolumeSceneNode* addShadowVolumeSceneNode(const IMesh* shadowMesh = 0,
      s32 id = -1, bool zfailmethod = true, f32 infinity = 1000.0f) = 0;
  //! Sets if the scene node should not copy the materials of the mesh but use them in a read only style.
  /** In this way it is possible to change the materials of a mesh
  causing all mesh scene nodes referencing this mesh to change, too.
  \param readonly Flag if the materials shall be read-only. */
  void setReadOnlyMaterials(bool readonly) = 0;
  //! Check if the scene node should not copy the materials of the mesh but use them in a read only style
  /** This flag can be set by setReadOnlyMaterials().
  \return Whether the materials are read-only. */
  bool isReadOnlyMaterials() = 0;
};
#endif

