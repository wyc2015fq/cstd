// Copyright (C) 2008-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
#ifndef __I_DYNAMIC_MESH_BUFFER_H_INCLUDED__
#define __I_DYNAMIC_MESH_BUFFER_H_INCLUDED__

/** a dynamic meshBuffer */
class IDynamicMeshBuffer : public IMeshBuffer
{
public:
  IVertexBuffer& getVertexBuffer() = 0;
  IIndexBuffer& getIndexBuffer() = 0;
  void setVertexBuffer(IVertexBuffer* vertexBuffer) = 0;
  void setIndexBuffer(IIndexBuffer* indexBuffer) = 0;
  //! Get the material of this meshbuffer
  /** \return Material of this buffer. */
  SMaterial& getMaterial() = 0;
  //! Get the material of this meshbuffer
  /** \return Material of this buffer. */
  const SMaterial& getMaterial() = 0;
  //! Get the axis aligned bounding box of this meshbuffer.
  /** \return Axis aligned bounding box of this buffer. */
  const aabbox3df& getBoundingBox() = 0;
  //! Set axis aligned bounding box
  /** \param box User defined axis aligned bounding box to use
  for this buffer. */
  void setBoundingBox(const aabbox3df& box) = 0;
  //! Recalculates the bounding box. Should be called if the mesh changed.
  void recalculateBoundingBox() = 0;
  //! Append the vertices and indices to the current buffer
  /** Only works for compatible vertex types.
  \param vertices Pointer to a vertex array.
  \param numVertices Number of vertices in the array.
  \param indices Pointer to index array.
  \param numIndices Number of indices in array. */
  void append(const void* const vertices, u32 numVertices, const u16* const indices, u32 numIndices) {
  }
  //! Append the meshbuffer to the current buffer
  /** Only works for compatible vertex types
  \param other Buffer to append to this one. */
  void append(const IMeshBuffer* const other) {
  }
  // ------------------- To be removed? -------------------  //
  //! get the current hardware mapping hint
  E_HARDWARE_MAPPING getHardwareMappingHint_Vertex() const {
    return getVertexBuffer().MappingHint;
  }
  //! get the current hardware mapping hint
  E_HARDWARE_MAPPING getHardwareMappingHint_Index() const {
    return getIndexBuffer().getHardwareMappingHint();
  }
  //! set the hardware mapping hint, for driver
  void setHardwareMappingHint(E_HARDWARE_MAPPING NewMappingHint, E_BUFFER_TYPE Buffer = EBT_VERTEX_AND_INDEX) {
    if (Buffer == EBT_VERTEX_AND_INDEX || Buffer == EBT_VERTEX) {
      getVertexBuffer().setHardwareMappingHint(NewMappingHint);
    }

    if (Buffer == EBT_VERTEX_AND_INDEX || Buffer == EBT_INDEX) {
      getIndexBuffer().setHardwareMappingHint(NewMappingHint);
    }
  }
  //! flags the mesh as changed, reloads hardware buffers
  void setDirty(E_BUFFER_TYPE Buffer = EBT_VERTEX_AND_INDEX) {
    if (Buffer == EBT_VERTEX_AND_INDEX || Buffer == EBT_VERTEX) {
      getVertexBuffer().setDirty();
    }

    if (Buffer == EBT_VERTEX_AND_INDEX || Buffer == EBT_INDEX) {
      getIndexBuffer().setDirty();
    }
  }
  u32 getChangedID_Vertex() const {
    return getVertexBuffer().getChangedID();
  }
  u32 getChangedID_Index() const {
    return getIndexBuffer().getChangedID();
  }
  // ------------------- Old interface -------------------  //
  //! Get type of vertex data which is stored in this meshbuffer.
  /** \return Vertex type of this buffer. */
  E_VERTEX_TYPE getVertexType() const {
    return getVertexBuffer().getType();
  }
  //! Get access to vertex data. The data is an array of vertices.
  /** Which vertex type is used can be determined by getVertexType().
  \return Pointer to array of vertices. */
  const void* getVertices() const {
    return getVertexBuffer().getData();
  }
  //! Get access to vertex data. The data is an array of vertices.
  /** Which vertex type is used can be determined by getVertexType().
  \return Pointer to array of vertices. */
  void* getVertices() {
    return getVertexBuffer().getData();
  }
  //! Get amount of vertices in meshbuffer.
  /** \return Number of vertices in this buffer. */
  u32 getVertexCount() const {
    return getVertexBuffer().size();
  }
  //! Get type of index data which is stored in this meshbuffer.
  /** \return Index type of this buffer. */
  E_INDEX_TYPE getIndexType() const {
    return getIndexBuffer().getType();
  }
  //! Get access to Indices.
  /** \return Pointer to indices array. */
  const u16* getIndices() const {
    return (u16*)getIndexBuffer().getData();
  }
  //! Get access to Indices.
  /** \return Pointer to indices array. */
  u16* getIndices() {
    return (u16*)getIndexBuffer().getData();
  }
  //! Get amount of indices in this meshbuffer.
  /** \return Number of indices in this buffer. */
  u32 getIndexCount() const {
    return getIndexBuffer().size();
  }
  //! returns position of vertex i
  const float3 getPosition(u32 i) const {
    return getVertexBuffer()[i].Pos;
  }
  //! returns position of vertex i
  float3 getPosition(u32 i) {
    return getVertexBuffer()[i].Pos;
  }
  //! returns texture coords of vertex i
  const vector2df& getTCoords(u32 i) const {
    return getVertexBuffer()[i].TCoords;
  }
  //! returns texture coords of vertex i
  vector2df& getTCoords(u32 i) {
    return getVertexBuffer()[i].TCoords;
  }
  //! returns normal of vertex i
  const float3 getNormal(u32 i) const {
    return getVertexBuffer()[i].Normal;
  }
  //! returns normal of vertex i
  float3 getNormal(u32 i) {
    return getVertexBuffer()[i].Normal;
  }
};


#endif

