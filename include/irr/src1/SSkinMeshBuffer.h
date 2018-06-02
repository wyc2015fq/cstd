// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
#ifndef __I_SKIN_MESH_BUFFER_H_INCLUDED__
#define __I_SKIN_MESH_BUFFER_H_INCLUDED__
//! A mesh buffer able to choose between S3DVertex, S3DVertex and S3DVertex at runtime
struct SSkinMeshBuffer : public IMeshBuffer {
  //! Default constructor
  SSkinMeshBuffer(E_VERTEX_TYPE vt = EVT_STANDARD) :
    ChangedID_Vertex(1), ChangedID_Index(1), VertexType(vt),
    MappingHint_Vertex(EHM_NEVER), MappingHint_Index(EHM_NEVER),
    BoundingBoxNeedsRecalculated(true) {
#ifdef _DEBUG
    setDebugName("SSkinMeshBuffer");
#endif
  }
  //! Get Material of this buffer.
  const SMaterial& getMaterial() const {
    return Material;
  }
  //! Get Material of this buffer.
  SMaterial& getMaterial() {
    return Material;
  }
  //! Get standard vertex at given index
  S3DVertex* getVertex(u32 index) {
    switch (VertexType) {
    case EVT_2TCOORDS:
      return (S3DVertex*)&Vertices_2TCoords[index];
    case EVT_TANGENTS:
      return (S3DVertex*)&Vertices_Tangents[index];
    default:
      return &Vertices_Standard[index];
    }
  }
  //! Get pointer to vertex array
  const void* getVertices() const {
    switch (VertexType) {
    case EVT_2TCOORDS:
      return Vertices_2TCoords;
    case EVT_TANGENTS:
      return Vertices_Tangents;
    default:
      return Vertices_Standard;
    }
  }
  //! Get pointer to vertex array
  void* getVertices() {
    switch (VertexType) {
    case EVT_2TCOORDS:
      return Vertices_2TCoords;
    case EVT_TANGENTS:
      return Vertices_Tangents;
    default:
      return Vertices_Standard;
    }
  }
  //! Get vertex count
  u32 getVertexCount() const {
    switch (VertexType) {
    case EVT_2TCOORDS:
      return Vertices_2TCoords_size;
    case EVT_TANGENTS:
      return Vertices_Tangents_size;
    default:
      return Vertices_Standard_size;
    }
  }
  //! Get type of index data which is stored in this meshbuffer.
  /** \return Index type of this buffer. */
  E_INDEX_TYPE getIndexType() const {
    return EIT_16BIT;
  }
  //! Get pointer to index array
  const u16* getIndices() const {
    return Indices;
  }
  //! Get pointer to index array
  u16* getIndices() {
    return Indices;
  }
  //! Get index count
  u32 getIndexCount() const {
    return Indices_size;
  }
  //! Get bounding box
  const F3AABBOX& getBoundingBox() const {
    return BoundingBox;
  }
  //! Set bounding box
  void setBoundingBox(const aabbox3df& box) {
    BoundingBox = box;
  }
  //! Recalculate bounding box
  void recalculateBoundingBox() {
    if (!BoundingBoxNeedsRecalculated) {
      return;
    }
    BoundingBoxNeedsRecalculated = false;
    switch (VertexType) {
    case EVT_STANDARD: {
      if (Vertices_Standard.empty()) {
        BoundingBox.reset(0, 0, 0);
      }
      else {
        BoundingBox.reset(Vertices_Standard[0].Pos);
        int i;
        for (i = 1; i < Vertices_Standard_size; ++i) {
          BoundingBox.addInternalPoint(Vertices_Standard[i].Pos);
        }
      }
      break;
    }
    case EVT_2TCOORDS: {
      if (Vertices_2TCoords.empty()) {
        BoundingBox.reset(0, 0, 0);
      }
      else {
        BoundingBox.reset(Vertices_2TCoords[0].Pos);
        int i;
        for (i = 1; i < Vertices_2TCoords_size; ++i) {
          BoundingBox.addInternalPoint(Vertices_2TCoords[i].Pos);
        }
      }
      break;
    }
    case EVT_TANGENTS: {
      if (Vertices_Tangents.empty()) {
        BoundingBox.reset(0, 0, 0);
      }
      else {
        BoundingBox.reset(Vertices_Tangents[0].Pos);
        int i;
        for (i = 1; i < Vertices_Tangents_size; ++i) {
          BoundingBox.addInternalPoint(Vertices_Tangents[i].Pos);
        }
      }
      break;
    }
    }
  }
  //! Get vertex type
  E_VERTEX_TYPE getVertexType() const {
    return VertexType;
  }
  //! Convert to 2tcoords vertex type
  void convertTo2TCoords() {
    if (VertexType == EVT_STANDARD) {
      for (int n = 0; n < Vertices_Standard_size; ++n) {
        S3DVertex Vertex;
        Vertex.Color = Vertices_Standard[n].Color;
        Vertex.Pos = Vertices_Standard[n].Pos;
        Vertex.Normal = Vertices_Standard[n].Normal;
        Vertex.TCoords = Vertices_Standard[n].TCoords;
        Vertices_2TCoords.push_back(Vertex);
      }
      Vertices_Standard.clear();
      VertexType = EVT_2TCOORDS;
    }
  }
  //! Convert to tangents vertex type
  void convertToTangents() {
    if (VertexType == EVT_STANDARD) {
      for (int n = 0; n < Vertices_Standard_size; ++n) {
        S3DVertex Vertex;
        Vertex.Color = Vertices_Standard[n].Color;
        Vertex.Pos = Vertices_Standard[n].Pos;
        Vertex.Normal = Vertices_Standard[n].Normal;
        Vertex.TCoords = Vertices_Standard[n].TCoords;
        Vertices_Tangents.push_back(Vertex);
      }
      Vertices_Standard.clear();
      VertexType = EVT_TANGENTS;
    }
    else if (VertexType == EVT_2TCOORDS) {
      for (int n = 0; n < Vertices_2TCoords_size; ++n) {
        S3DVertex Vertex;
        Vertex.Color = Vertices_2TCoords[n].Color;
        Vertex.Pos = Vertices_2TCoords[n].Pos;
        Vertex.Normal = Vertices_2TCoords[n].Normal;
        Vertex.TCoords = Vertices_2TCoords[n].TCoords;
        Vertices_Tangents.push_back(Vertex);
      }
      Vertices_2TCoords.clear();
      VertexType = EVT_TANGENTS;
    }
  }
  //! returns position of vertex i
  const float3 getPosition(u32 i) const {
    switch (VertexType) {
    case EVT_2TCOORDS:
      return Vertices_2TCoords[i].Pos;
    case EVT_TANGENTS:
      return Vertices_Tangents[i].Pos;
    default:
      return Vertices_Standard[i].Pos;
    }
  }
  //! returns position of vertex i
  float3 getPosition(u32 i) {
    switch (VertexType) {
    case EVT_2TCOORDS:
      return Vertices_2TCoords[i].Pos;
    case EVT_TANGENTS:
      return Vertices_Tangents[i].Pos;
    default:
      return Vertices_Standard[i].Pos;
    }
  }
  //! returns normal of vertex i
  const float3 getNormal(u32 i) const {
    switch (VertexType) {
    case EVT_2TCOORDS:
      return Vertices_2TCoords[i].Normal;
    case EVT_TANGENTS:
      return Vertices_Tangents[i].Normal;
    default:
      return Vertices_Standard[i].Normal;
    }
  }
  //! returns normal of vertex i
  float3 getNormal(u32 i) {
    switch (VertexType) {
    case EVT_2TCOORDS:
      return Vertices_2TCoords[i].Normal;
    case EVT_TANGENTS:
      return Vertices_Tangents[i].Normal;
    default:
      return Vertices_Standard[i].Normal;
    }
  }
  //! returns texture coords of vertex i
  const vector2df& getTCoords(u32 i) const {
    switch (VertexType) {
    case EVT_2TCOORDS:
      return Vertices_2TCoords[i].TCoords;
    case EVT_TANGENTS:
      return Vertices_Tangents[i].TCoords;
    default:
      return Vertices_Standard[i].TCoords;
    }
  }
  //! returns texture coords of vertex i
  vector2df& getTCoords(u32 i) {
    switch (VertexType) {
    case EVT_2TCOORDS:
      return Vertices_2TCoords[i].TCoords;
    case EVT_TANGENTS:
      return Vertices_Tangents[i].TCoords;
    default:
      return Vertices_Standard[i].TCoords;
    }
  }
  //! append the vertices and indices to the current buffer
  void append(const void* const vertices, u32 numVertices, const u16* const indices, u32 numIndices) {}
  //! append the meshbuffer to the current buffer
  void append(const IMeshBuffer* const other) {}
  //! get the current hardware mapping hint for vertex buffers
  E_HARDWARE_MAPPING getHardwareMappingHint_Vertex() const {
    return MappingHint_Vertex;
  }
  //! get the current hardware mapping hint for index buffers
  E_HARDWARE_MAPPING getHardwareMappingHint_Index() const {
    return MappingHint_Index;
  }
  //! set the hardware mapping hint, for driver
  void setHardwareMappingHint(E_HARDWARE_MAPPING NewMappingHint, E_BUFFER_TYPE Buffer = EBT_VERTEX_AND_INDEX) {
    if (Buffer == EBT_VERTEX) {
      MappingHint_Vertex = NewMappingHint;
    }
    else if (Buffer == EBT_INDEX) {
      MappingHint_Index = NewMappingHint;
    }
    else if (Buffer == EBT_VERTEX_AND_INDEX) {
      MappingHint_Vertex = NewMappingHint;
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
  u32 getChangedID_Vertex() const {
    return ChangedID_Vertex;
  }
  u32 getChangedID_Index() const {
    return ChangedID_Index;
  }
  //! Call this after changing the positions of any vertex.
  void boundingBoxNeedsRecalculated(void) {
    BoundingBoxNeedsRecalculated = true;
  }
  S3DVertex* Vertices_Tangents;
  S3DVertex* Vertices_2TCoords;
  S3DVertex* Vertices_Standard;
  u16* Indices;
  u32 ChangedID_Vertex;
  u32 ChangedID_Index;
  //ISkinnedMesh::SJoint *AttachedJoint;
  matrix4 Transformation;
  SMaterial Material;
  E_VERTEX_TYPE VertexType;
  F3AABBOX BoundingBox;
  // hardware mapping hint
  E_HARDWARE_MAPPING MappingHint_Vertex: 3;
  E_HARDWARE_MAPPING MappingHint_Index: 3;
  bool BoundingBoxNeedsRecalculated: 1;
};
#endif

