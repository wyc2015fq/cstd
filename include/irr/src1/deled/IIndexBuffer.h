// Copyright (C) 2008-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
#ifndef __I_INDEX_BUFFER_H_INCLUDED__
#define __I_INDEX_BUFFER_H_INCLUDED__

//#include "irrArray.h"


  
typedef struct CIndexBuffer IIndexBuffer;

struct CIndexBuffer 
{
public:
  union {
    s16* Indices_16;
    s32* Indices_32;
    void* Indices;
  };
  E_INDEX_TYPE type;
  int m_used;
  int m_size;
  CIndexBuffer(E_INDEX_TYPE IndexType) : Indices(0), MappingHint(EHM_NEVER), ChangedID(1) {
    m_used = 0;
    m_size = 0;
    setType(IndexType);
  }
  int tt_type() const {
    return EIT_16BIT==type ? CV_2S : CV_4S;
  }
  CIndexBuffer(const IIndexBuffer& IndexBufferCopy) : Indices(0), MappingHint(EHM_NEVER), ChangedID(1) {
    int n = IndexBufferCopy.size();
    setType(IndexBufferCopy.getType());
    reallocate(n);
    castcpy(Indices, IndexBufferCopy.Indices, n, tt_type(), IndexBufferCopy.tt_type());
  }
  ~CIndexBuffer() {
    if (Indices) {
      free(Indices);
    }
  }
  //void setType(E_INDEX_TYPE IndexType);
  void setType(E_INDEX_TYPE IndexType) {
    if (type!=IndexType) {
      type = IndexType;
      int n = size();
      CIndexBuffer NewIndices(IndexType);
      NewIndices.reallocate(n);
      castcpy(NewIndices.Indices, Indices, n, NewIndices.tt_type(), tt_type());
      memswap(sizeof(CIndexBuffer), this, &NewIndices);
    }
  }
  void* getData() {
    return Indices;
  }
  E_INDEX_TYPE getType() const {
    return type;
  }
  u32 stride() const {
    return (type==EIT_16BIT) ? sizeof(s16) : sizeof(s32);
  }
  u32 size() const {
    return m_used;
  }
  void push_back(const u32& element) {
    ASSERT(m_used < m_size);
    ++m_used;
  }
  u32 operator [](u32 index) const {
    return (type==EIT_16BIT) ? Indices_16[index] : Indices_32[index];
  }
  u32 getLast() {
    return (type==EIT_16BIT) ? Indices_16[m_used-1] : Indices_32[m_used-1];
  }
  void setValue(u32 index, u32 value) {
    if (type==EIT_16BIT) {
      Indices_16[index] = (s16)value;
    } else {
      Indices_32[index] = value;
    }
  }
  void set_used(u32 usedNow) {
    m_used = usedNow;
  }
  void reallocate(u32 new_size) {
    if (new_size>m_size) {
      m_size = new_size;
      if (type==EIT_16BIT) {
        REALLOC(s16, Indices_16, m_size);
      } else {
        REALLOC(s32, Indices_32, m_size);
      }
    }
  }
  u32 allocated_size() const {
    return m_size;
  }
  void* pointer() {
    return Indices;
  }
  //! get the current hardware mapping hint
  E_HARDWARE_MAPPING getHardwareMappingHint() const {
    return MappingHint;
  }
  //! set the hardware mapping hint, for driver
  void setHardwareMappingHint(E_HARDWARE_MAPPING NewMappingHint) {
    MappingHint = NewMappingHint;
  }
  //! flags the mesh as changed, reloads hardware buffers
  void setDirty() {
    ++ChangedID;
  }
  //! Get the currently used ID for identification of changes.
  /** This shouldn't be used for anything outside the VideoDriver. */
  u32 getChangedID() const {
    return ChangedID;
  }
  E_HARDWARE_MAPPING MappingHint;
  u32 ChangedID;
};


#endif

