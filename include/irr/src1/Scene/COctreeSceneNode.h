// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
#ifndef __C_OCTREE_SCENE_NODE_H_INCLUDED__
#define __C_OCTREE_SCENE_NODE_H_INCLUDED__
//#include "IMeshSceneNode.h"
//#include "Octree.h"
//! implementation of the IBspTreeSceneNode
class COctreeSceneNode : public IMeshSceneNode
{
public:
  //! constructor
  COctreeSceneNode(ISceneNode* parent, ISceneManager* mgr, s32 id,
      s32 minimalPolysPerNode = 512);
  //! destructor
  ~COctreeSceneNode();
  void OnRegisterSceneNode();
  //! renders the node.
  void render();
  //! returns the axis aligned bounding box of this node
  const F3AABBOX& getBoundingBox() const;
  //! creates the tree
  bool createTree(IMesh* mesh);
  //! returns the material based on the zero based index i. To get the amount
  //! of materials used by this scene node, use getMaterialCount().
  //! This function is needed for inserting the node into the scene hirachy on a
  //! optimal position for minimizing renderstate changes, but can also be used
  //! to directly modify the material of a scene node.
  SMaterial& getMaterial(u32 i);
  //! returns amount of materials used by this scene node.
  u32 getMaterialCount() const;
  //! Writes attributes of the scene node.
  void serializeAttributes(IAttributes* out, SAttributeReadWriteOptions* options = 0) const;
  //! Reads attributes of the scene node.
  void deserializeAttributes(IAttributes* in, SAttributeReadWriteOptions* options = 0);
  //! Returns type of the scene node
  ESCENE_NODE_TYPE getType() const {
    return ESNT_OCTREE;
  }
  //! Sets a new mesh to display
  void setMesh(IMesh* mesh);
  //! Get the currently defined mesh for display.
  IMesh* getMesh(void);
  //! Sets if the scene node should not copy the materials of the mesh but use them in a read only style.
  void setReadOnlyMaterials(bool readonly);
  //! Check if the scene node should not copy the materials of the mesh but use them in a read only style
  bool isReadOnlyMaterials() const;
  //! Creates shadow volume scene node as child of this node
  //! and returns a pointer to it.
  IShadowVolumeSceneNode* addShadowVolumeSceneNode(const IMesh* shadowMesh,
      s32 id, bool zfailmethod = true, f32 infinity = 10000.0f);
  //! Removes a child from this scene node.
  //! Implemented here, to be able to remove the shadow properly, if there is one,
  //! or to remove attached childs.
  bool removeChild(ISceneNode* child);
private:
  void deleteTree();
  F3AABBOX Box;
  Octree<S3DVertex>* StdOctree;
  Octree<S3DVertex>::SMeshChunk > StdMeshes;
  Octree<S3DVertex>* LightMapOctree;
  Octree<S3DVertex>::SMeshChunk > LightMapMeshes;
  Octree<S3DVertex>* TangentsOctree;
  Octree<S3DVertex>::SMeshChunk > TangentsMeshes;
  E_VERTEX_TYPE VertexType;
  SMaterial > Materials;
  char* MeshName;
  s32 MinimalPolysPerNode;
  s32 PassCount;
  IMesh* Mesh;
  IShadowVolumeSceneNode* Shadow;
  //! use VBOs for rendering where possible
  bool UseVBOs;
  //! use visibility information together with VBOs
  bool UseVisibilityAndVBOs;
  //! use bounding box or frustum for calculate polys
  bool BoxBased;
};
#endif

