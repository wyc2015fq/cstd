// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
#ifndef __T_MESH_BUFFER_H_INCLUDED__
#define __T_MESH_BUFFER_H_INCLUDED__
//#include "irrArray.h"



//! Template implementation of the IMeshBuffer interface

class CMeshBuffer : public IMeshBuffer
{
public:
  //! Default constructor for empty meshbuffer
  CMeshBuffer(): ChangedID_Vertex(1), ChangedID_Index(1), MappingHint_Vertex(EHM_NEVER), MappingHint_Index(EHM_NEVER) {
#ifdef _DEBUG
    setDebugName("SMeshBuffer");
#endif
  }
  //! Get material of this meshbuffer
  /** \return Material of this buffer */
  const SMaterial& getMaterial() const {
    return Material;
  }
  //! Get material of this meshbuffer
  /** \return Material of this buffer */
  SMaterial& getMaterial() {
    return Material;
  }
  //! Get pointer to vertices
  /** \return Pointer to vertices. */
  const void* getVertices() const {
    return Vertices;
  }
  //! Get pointer to vertices
  /** \return Pointer to vertices. */
  void* getVertices() {
    return Vertices;
  }
  //! Get number of vertices
  /** \return Number of vertices. */
  u32 getVertexCount() const {
    return Vertices_size;
  }
  //! Get type of index data which is stored in this meshbuffer.
  /** \return Index type of this buffer. */
  E_INDEX_TYPE getIndexType() const {
    return EIT_16BIT;
  }
  //! Get pointer to indices
  /** \return Pointer to indices. */
  const u16* getIndices() const {
    return Indices;
  }
  //! Get pointer to indices
  /** \return Pointer to indices. */
  u16* getIndices() {
    return Indices;
  }
  //! Get number of indices
  /** \return Number of indices. */
  u32 getIndexCount() const {
    return Indices_size;
  }
  //! Get the axis aligned bounding box
  /** \return Axis aligned bounding box of this buffer. */
  const F3AABBOX& getBoundingBox() const {
    return BoundingBox;
  }
  //! Set the axis aligned bounding box
  /** \param box New axis aligned bounding box for this buffer. */
  //! set user axis aligned bounding box
  void setBoundingBox(const aabbox3df& box) {
    BoundingBox = box;
  }
  //! Recalculate the bounding box.
  /** should be called if the mesh changed. */
  void recalculateBoundingBox() {
    if (0==Vertices_size) {
      BoundingBox.reset(0, 0, 0);
    }
    else {
      BoundingBox.reset(Vertices[0].Pos);
      int i;

      for (i = 1; i < Vertices_size; ++i) {
        BoundingBox.addInternalPoint(Vertices[i].Pos);
      }
    }
  }
  //! Get type of vertex data stored in this buffer.
  /** \return Type of vertex data. */
  E_VERTEX_TYPE getVertexType() const {
    return VertexType;
  }
  //! returns position of vertex i
  const float3 getPosition(u32 i) const {
    return Vertices[i].Pos;
  }
  //! returns position of vertex i
  float3 getPosition(u32 i) {
    return Vertices[i].Pos;
  }
  //! returns normal of vertex i
  const float3 getNormal(u32 i) const {
    return Vertices[i].Normal;
  }
  //! returns normal of vertex i
  float3 getNormal(u32 i) {
    return Vertices[i].Normal;
  }
  //! returns texture coord of vertex i
  const vector2df& getTCoords(u32 i) const {
    return Vertices[i].TCoords;
  }
  //! returns texture coord of vertex i
  vector2df& getTCoords(u32 i) {
    return Vertices[i].TCoords;
  }
  //! Append the vertices and indices to the current buffer
  /** Only works for compatible types, i.e. either the same type
  or the main buffer is of standard type. Otherwise, behavior is
  undefined.
  */
  void append(const S3DVertex* const vertices, u32 numVertices, const u16* const indices, u32 numIndices) {
    if (vertices == Vertices) {
      return;
    }
    
    const u32 vertexCount = getVertexCount();
    const u32 indiceCount = getIndexCount();
    u32 i;
    MYREALLOC(Vertices, vertexCount + numVertices);

    for (i = 0; i < numVertices; ++i) {
      Vertices[numVertices+i] = vertices[i];
      BoundingBox.addInternalPoint(vertices[i].Pos);
    }

    MYREALLOC(Indices, indiceCount + numIndices);

    for (i = 0; i < numIndices; ++i) {
      Indices[indiceCount+i] = (indices[i] + vertexCount);
    }
  }
  //! Append the meshbuffer to the current buffer
  /** Only works for compatible types, i.e. either the same type
  or the main buffer is of standard type. Otherwise, behavior is
  undefined.
  \param other Meshbuffer to be appended to this one.
  */
  void append(const IMeshBuffer* const other) {
    /*
    if (this==other)
      return;
    const u32 vertexCount = getVertexCount();
    u32 i;
    Vertices.reallocate(vertexCount+other->getVertexCount());
    for (i=0; i<other->getVertexCount(); ++i)
    {
      Vertices.push_back(reinterpret_cast<const T*>(other->getVertices())[i]);
    }
    Indices.reallocate(getIndexCount()+other->getIndexCount());
    for (i=0; i<other->getIndexCount(); ++i)
    {
      Indices.push_back(other->getIndices()[i]+vertexCount);
    }
    BoundingBox.addInternalBox(other->getBoundingBox());
    */
  }
  //! get the current hardware mapping hint
  E_HARDWARE_MAPPING getHardwareMappingHint_Vertex() const {
    return MappingHint_Vertex;
  }
  //! get the current hardware mapping hint
  E_HARDWARE_MAPPING getHardwareMappingHint_Index() const {
    return MappingHint_Index;
  }
  //! set the hardware mapping hint, for driver
  void setHardwareMappingHint(E_HARDWARE_MAPPING NewMappingHint, E_BUFFER_TYPE Buffer = EBT_VERTEX_AND_INDEX) {
    if (Buffer == EBT_VERTEX_AND_INDEX || Buffer == EBT_VERTEX) {
      MappingHint_Vertex = NewMappingHint;
    }

    if (Buffer == EBT_VERTEX_AND_INDEX || Buffer == EBT_INDEX) {
      MappingHint_Index = NewMappingHint;
    }
  }
  //! flags the mesh as changed, reloads hardware buffers
  void setDirty(E_BUFFER_TYPE Buffer = EBT_VERTEX_AND_INDEX) {
    if (Buffer == EBT_VERTEX_AND_INDEX || Buffer == EBT_VERTEX) {
      ++ChangedID_Vertex;
    }

    if (Buffer == EBT_VERTEX_AND_INDEX || Buffer == EBT_INDEX) {
      ++ChangedID_Index;
    }
  }
  //! Get the currently used ID for identification of changes.
  /** This shouldn't be used for anything outside the VideoDriver. */
  u32 getChangedID_Vertex() const {
    return ChangedID_Vertex;
  }
  //! Get the currently used ID for identification of changes.
  /** This shouldn't be used for anything outside the VideoDriver. */
  u32 getChangedID_Index() const {
    return ChangedID_Index;
  }
};
#if 0
//! Standard meshbuffer
typedef CMeshBuffer<S3DVertex> SMeshBuffer;
//! Meshbuffer with two texture coords per vertex, e.g. for lightmaps
typedef CMeshBuffer<S3DVertex2TCoords> SMeshBufferLightMap;
//! Meshbuffer with vertices having tangents stored, e.g. for normal mapping
typedef CMeshBuffer<S3DVertexTangents> SMeshBufferTangents;
#else
typedef CMeshBuffer SMeshBuffer;
//! Meshbuffer with two texture coords per vertex, e.g. for lightmaps
typedef CMeshBuffer SMeshBufferLightMap;
//! Meshbuffer with vertices having tangents stored, e.g. for normal mapping
typedef CMeshBuffer SMeshBufferTangents;
#endif


#endif

