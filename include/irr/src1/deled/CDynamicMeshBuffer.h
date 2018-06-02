// Copyright (C) 2008-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
#ifndef __C_DYNAMIC_MESHBUFFER_H_INCLUDED__
#define __C_DYNAMIC_MESHBUFFER_H_INCLUDED__


class CDynamicMeshBuffer: public IDynamicMeshBuffer
{
public:
  //! constructor
  CDynamicMeshBuffer(E_VERTEX_TYPE vertexType, E_INDEX_TYPE indexType) {
    VertexBuffer = new CVertexBuffer(vertexType);
    IndexBuffer = new CIndexBuffer(indexType);
  }
  //! destructor
  ~CDynamicMeshBuffer() {
  }
  IVertexBuffer& getVertexBuffer() const {
    return *VertexBuffer;
  }
  IIndexBuffer& getIndexBuffer() const {
    return *IndexBuffer;
  }
  void setVertexBuffer(IVertexBuffer* newVertexBuffer) {
    VertexBuffer = newVertexBuffer;
  }
  void setIndexBuffer(IIndexBuffer* newIndexBuffer) {
    IndexBuffer = newIndexBuffer;
  }
  //! Get Material of this buffer.
  const SMaterial& getMaterial() const {
    return Material;
  }
  //! Get Material of this buffer.
  SMaterial& getMaterial() {
    return Material;
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
    if (!getVertexBuffer().size()) {
      BoundingBox.reset(0, 0, 0);
    }
    else {
      BoundingBox.reset(getVertexBuffer()[0].Pos);
      int i;

      for (i = 1; i < getVertexBuffer().size(); ++i) {
        BoundingBox.addInternalPoint(getVertexBuffer()[i].Pos);
      }
    }
  }
  SMaterial Material;
  F3AABBOX BoundingBox;
private:
  IVertexBuffer* VertexBuffer;
  IIndexBuffer* IndexBuffer;
};


#endif

