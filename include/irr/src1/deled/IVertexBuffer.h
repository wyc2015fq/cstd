
#ifndef _IVERTEXBUFFER_H_
#define _IVERTEXBUFFER_H_

typedef struct CVertexBuffer IVertexBuffer;
typedef struct CVertexBuffer
{
  CVertexBuffer(E_VERTEX_TYPE vertexType) : Vertices(0),
    MappingHint(EHM_NEVER), ChangedID(1) {
    setType(vertexType);
  }
  CVertexBuffer(const IVertexBuffer& VertexBufferCopy) :
  Vertices(0), MappingHint(EHM_NEVER),
    ChangedID(1) {
    setType(VertexBufferCopy.getType());
    reallocate(VertexBufferCopy.size());
    
    for (int n = 0; n < VertexBufferCopy.size(); ++n) {
      push_back(VertexBufferCopy[n]);
    }
  }
  ~CVertexBuffer() {
    free(Vertices);
  }
  void setType(E_VERTEX_TYPE vertexType) {
    S3DVertex* NewVertices = 0;
    type = EVT_STANDARD;
  }
  void* getData() {
    return Vertices;
  }
  E_VERTEX_TYPE getType() const {
    return type;
  }
  u32 stride() const {
    return sizeof(S3DVertex);
  }
  u32 size() const {
    return used;
  }
  void push_back(const S3DVertex& element) {
    int n = used;
    ++used;
    REALLOC(S3DVertex, Vertices, used);
    Vertices[n] = element;
  }
  S3DVertex& operator [](const u32 index) const {
    return Vertices[index];
  }
  S3DVertex& getLast() {
    return Vertices[m_size-1];
  }
  int used;
  void set_used(u32 usedNow) {
    used = usedNow;
    if (m_size<used) {
      m_size = used;
      REALLOC(S3DVertex, Vertices, m_size);
    }
  }
  void reallocate(u32 new_size) {
    m_size = new_size;
    REALLOC(S3DVertex, Vertices, m_size);
  }
  u32 allocated_size() const {
    return m_size;
  }
  S3DVertex* pointer() {
    return Vertices;
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
} CVertexBuffer;



#endif // _IVERTEXBUFFER_H_
