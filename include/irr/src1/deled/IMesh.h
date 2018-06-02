// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
#ifndef __I_MESH_H_INCLUDED__
#define __I_MESH_H_INCLUDED__





class IMeshBuffer;
//! Class which holds the geometry of an object.
/** An IMesh is nothing more than a collection of some mesh buffers
(IMeshBuffer). SMesh is a simple implementation of an IMesh.
A mesh is usually added to an IMeshSceneNode in order to be rendered.
*/
class IMesh
{
public:
  //! Get the amount of mesh buffers.
  /** \return Amount of mesh buffers (IMeshBuffer) in this mesh. */
  u32 getMeshBufferCount() = 0;
  //! Get pointer to a mesh buffer.
  /** \param nr: Zero based index of the mesh buffer. The maximum value is
  getMeshBufferCount() - 1;
  \return Pointer to the mesh buffer or 0 if there is no such
  mesh buffer. */
  IMeshBuffer* getMeshBuffer(u32 nr) = 0;
  //! Get pointer to a mesh buffer which fits a material
  /** \param material: material to search for
  \return Pointer to the mesh buffer or 0 if there is no such
  mesh buffer. */
  IMeshBuffer* getMeshBuffer(const SMaterial& material) = 0;
  //! Get an axis aligned bounding box of the mesh.
  /** \return Bounding box of this mesh. */
  const F3AABBOX& getBoundingBox() = 0;
  //! Set user-defined axis aligned bounding box
  /** \param box New bounding box to use for the mesh. */
  void setBoundingBox(const aabbox3df& box) = 0;
  //! Sets a flag of all contained materials to a new value.
  /** \param flag: Flag to set in all materials.
  \param newvalue: New value to set in all materials. */
  void setMaterialFlag(E_MATERIAL_FLAG flag, bool newvalue) = 0;
  //! Set the hardware mapping hint
  /** This methods allows to define optimization hints for the
  hardware. This enables, e.g., the use of hardware buffers on
  pltforms that support this feature. This can lead to noticeable
  performance gains. */
  void setHardwareMappingHint(E_HARDWARE_MAPPING newMappingHint, E_BUFFER_TYPE buffer = EBT_VERTEX_AND_INDEX) = 0;
  //! Flag the meshbuffer as changed, reloads hardware buffers
  /** This method has to be called every time the vertices or
  indices have changed. Otherwise, changes won't be updated
  on the GPU in the next render cycle. */
  void setDirty(E_BUFFER_TYPE buffer = EBT_VERTEX_AND_INDEX) = 0;
};


#endif

