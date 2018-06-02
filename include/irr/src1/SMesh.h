// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
#ifndef __S_MESH_H_INCLUDED__
#define __S_MESH_H_INCLUDED__
//#include "irrArray.h"
//! Simple implementation of the IMesh interface.
struct SMesh : public IMesh {
  //! constructor
  SMesh() {
#ifdef _DEBUG
    setDebugName("SMesh");
#endif
  }
  //! destructor
  ~SMesh() {
    // drop buffers
    int i;
    for (i = 0; i < MeshBuffers_size; ++i) {
      MeshBuffers[i]->drop();
    }
  }
  //! clean mesh
  void clear() {
    int i;
    for (i = 0; i < MeshBuffers_size; ++i) {
      MeshBuffers[i]->drop();
    }
    MeshBuffers.clear();
    BoundingBox.reset(0.f, 0.f, 0.f);
  }
  //! returns amount of mesh buffers.
  u32 getMeshBufferCount() const {
    return MeshBuffers_size;
  }
  //! returns pointer to a mesh buffer
  IMeshBuffer* getMeshBuffer(u32 nr) const {
    return MeshBuffers[nr];
  }
  //! returns a meshbuffer which fits a material
  /** reverse search */
  IMeshBuffer* getMeshBuffer(const SMaterial& material) const {
    for (s32 i = (s32)MeshBuffers_size - 1; i >= 0; --i) {
      if (material == MeshBuffers[i]->getMaterial()) {
        return MeshBuffers[i];
      }
    }
    return 0;
  }
  //! returns an axis aligned bounding box
  const F3AABBOX& getBoundingBox() const {
    return BoundingBox;
  }
  //! set user axis aligned bounding box
  void setBoundingBox(const aabbox3df& box) {
    BoundingBox = box;
  }
  //! recalculates the bounding box
  void recalculateBoundingBox() {
    if (MeshBuffers_size) {
      BoundingBox = MeshBuffers[0]->getBoundingBox();
      int i;
      for (i = 1; i < MeshBuffers_size; ++i) {
        BoundingBox.addInternalBox(MeshBuffers[i]->getBoundingBox());
      }
    }
    else {
      BoundingBox.reset(0.0f, 0.0f, 0.0f);
    }
  }
  //! adds a MeshBuffer
  /** The bounding box is not updated automatically. */
  void addMeshBuffer(IMeshBuffer* buf) {
    if (buf) {
      buf->grab();
      MeshBuffers.push_back(buf);
    }
  }
  //! sets a flag of all contained materials to a new value
  void setMaterialFlag(E_MATERIAL_FLAG flag, bool newvalue) {
    int i;
    for (i = 0; i < MeshBuffers_size; ++i) {
      MeshBuffers[i]->getMaterial().setFlag(flag, newvalue);
    }
  }
  //! set the hardware mapping hint, for driver
  void setHardwareMappingHint(E_HARDWARE_MAPPING newMappingHint, E_BUFFER_TYPE buffer = EBT_VERTEX_AND_INDEX) {
    int i;
    for (i = 0; i < MeshBuffers_size; ++i) {
      MeshBuffers[i]->setHardwareMappingHint(newMappingHint, buffer);
    }
  }
  //! flags the meshbuffer as changed, reloads hardware buffers
  void setDirty(E_BUFFER_TYPE buffer = EBT_VERTEX_AND_INDEX) {
    int i;
    for (i = 0; i < MeshBuffers_size; ++i) {
      MeshBuffers[i]->setDirty(buffer);
    }
  }
  //! The meshbuffers of this mesh
  IMeshBuffer* > MeshBuffers;
  //! The bounding box of this mesh
  F3AABBOX BoundingBox;
};
#endif

