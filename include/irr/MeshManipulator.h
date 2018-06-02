CV_INLINE void getAngleWeight(float3 v, const float3 v1, const float3 v2, const float3 v3)
{
  // Calculate this triangle's weight for each of its three vertices
  // start by calculating the lengths of its sides
  const f32 a = VDISQ3(v2, v3);
  const f32 asqrt = sqrtf(a);
  const f32 b = VDISQ3(v1, v3);
  const f32 bsqrt = sqrtf(b);
  const f32 c = VDISQ3(v1, v2);
  const f32 csqrt = sqrtf(c);
  // use them to find the angle at each vertex
  VSET3(v,
      acosf((b + c - a) / (2.f * bsqrt * csqrt)),
      acosf((-b + c + a) / (2.f * asqrt * csqrt)),
      acosf((b - c + a) / (2.f * bsqrt * asqrt)));
}
#if 0
//! Flips the direction of surfaces. Changes backfacing triangles to frontfacing
//! triangles and vice versa.
//! \param mesh: Mesh on which the operation is performed.
void flipSurfaces(IMesh* mesh)
{
  if (!mesh) {
    return;
  }
  else {
    const u32 bcount = mesh->getMeshBufferCount();
    int i, b;
    for (b = 0; b < bcount; ++b) {
      IMeshBuffer* buffer = mesh->getMeshBuffer(b);
      const u32 idxcnt = buffer->getIndexCount();
      if (buffer->getIndexType() == EIT_16BIT) {
        u16* idx = buffer->getIndices();
        for (i = 0; i < idxcnt; i += 3) {
          const u16 tmp = idx[i + 1];
          idx[i + 1] = idx[i + 2];
          idx[i + 2] = tmp;
        }
      }
      else {
        u32* idx = (u32*)(buffer->getIndices());
        for (i = 0; i < idxcnt; i += 3) {
          const u32 tmp = idx[i + 1];
          idx[i + 1] = idx[i + 2];
          idx[i + 2] = tmp;
        }
      }
    }
  }
}
template <typename T>
void recalculateNormalsT(IMeshBuffer* buffer, bool smooth, bool angleWeighted)
{
  const u32 vtxcnt = buffer->getVertexCount();
  const u32 idxcnt = buffer->getIndexCount();
  const T* idx = reinterpret_cast<T*>(buffer->getIndices());
  if (!smooth) {
    int i;
    for (i = 0; i < idxcnt; i += 3) {
      const float3 v1 = buffer->getPosition(idx[i + 0]);
      const float3 v2 = buffer->getPosition(idx[i + 1]);
      const float3 v3 = buffer->getPosition(idx[i + 2]);
      const vector3df normal = plane3d<f32>(v1, v2, v3).Normal;
      buffer->getNormal(idx[i + 0]) = normal;
      buffer->getNormal(idx[i + 1]) = normal;
      buffer->getNormal(idx[i + 2]) = normal;
    }
  }
  else {
    u32 i;
    for (i = 0; i != vtxcnt; ++i) {
      buffer->getNormal(i).set(0.f, 0.f, 0.f);
    }
    for (i = 0; i < idxcnt; i += 3) {
      const float3 v1 = buffer->getPosition(idx[i + 0]);
      const float3 v2 = buffer->getPosition(idx[i + 1]);
      const float3 v3 = buffer->getPosition(idx[i + 2]);
      const vector3df normal = plane3d<f32>(v1, v2, v3).Normal;
      vector3df weight(1.f, 1.f, 1.f);
      if (angleWeighted) {
        weight = getAngleWeight(v1, v2, v3);  // writing  necessary for borland
      }
      buffer->getNormal(idx[i + 0]) += weight.X * normal;
      buffer->getNormal(idx[i + 1]) += weight.Y * normal;
      buffer->getNormal(idx[i + 2]) += weight.Z * normal;
    }
    for (i = 0; i != vtxcnt; ++i) {
      buffer->getNormal(i).normalize();
    }
  }
}
}
//! Recalculates all normals of the mesh buffer.
/** \param buffer: Mesh buffer on which the operation is performed. */
void CMeshManipulator::recalculateNormals(IMeshBuffer* buffer, bool smooth, bool angleWeighted) const
{
  if (!buffer) {
    return;
  }
  if (buffer->getIndexType() == EIT_16BIT) {
    recalculateNormalsT<u16>(buffer, smooth, angleWeighted);
  }
  else {
    recalculateNormalsT<u32>(buffer, smooth, angleWeighted);
  }
}
//! Recalculates all normals of the mesh.
//! \param mesh: Mesh on which the operation is performed.
void CMeshManipulator::recalculateNormals(IMesh* mesh, bool smooth, bool angleWeighted) const
{
  if (!mesh) {
    return;
  }
  const u32 bcount = mesh->getMeshBufferCount();
  for (int b = 0; b < bcount; ++b) {
    recalculateNormals(mesh->getMeshBuffer(b), smooth, angleWeighted);
  }
}
void calculateTangents(
    float3 normal,
    float3 tangent,
    float3 binormal,
    const float3 vt1, const float3 vt2, const float3 vt3, // vertices
    const vector2df& tc1, const vector2df& tc2, const vector2df& tc3) // texture coords
{
  // choose one of them:
  //#define USE_NVIDIA_GLH_VERSION // use version used by nvidia in glh headers
#define USE_IRR_VERSION
#ifdef USE_IRR_VERSION
  vector3df v1 = vt1 - vt2;
  vector3df v2 = vt3 - vt1;
  normal = v2.crossProduct(v1);
  normal.normalize();
  // binormal
  f32 deltaX1 = tc1.X - tc2.X;
  f32 deltaX2 = tc3.X - tc1.X;
  binormal = (v1 * deltaX2) - (v2 * deltaX1);
  binormal.normalize();
  // tangent
  f32 deltaY1 = tc1.Y - tc2.Y;
  f32 deltaY2 = tc3.Y - tc1.Y;
  tangent = (v1 * deltaY2) - (v2 * deltaY1);
  tangent.normalize();
  // adjust
  vector3df txb = tangent.crossProduct(binormal);
  if (txb.dotProduct(normal) < 0.0f) {
    tangent *= -1.0f;
    binormal *= -1.0f;
  }
#endif // USE_IRR_VERSION
#ifdef USE_NVIDIA_GLH_VERSION
  tangent.set(0, 0, 0);
  binormal.set(0, 0, 0);
  vector3df v1(vt2.X - vt1.X, tc2.X - tc1.X, tc2.Y - tc1.Y);
  vector3df v2(vt3.X - vt1.X, tc3.X - tc1.X, tc3.Y - tc1.Y);
  vector3df txb = v1.crossProduct(v2);
  if (!iszero(txb.X)) {
    tangent.X  = -txb.Y / txb.X;
    binormal.X = -txb.Z / txb.X;
  }
  v1.X = vt2.Y - vt1.Y;
  v2.X = vt3.Y - vt1.Y;
  txb = v1.crossProduct(v2);
  if (!iszero(txb.X)) {
    tangent.Y  = -txb.Y / txb.X;
    binormal.Y = -txb.Z / txb.X;
  }
  v1.X = vt2.Z - vt1.Z;
  v2.X = vt3.Z - vt1.Z;
  txb = v1.crossProduct(v2);
  if (!iszero(txb.X)) {
    tangent.Z  = -txb.Y / txb.X;
    binormal.Z = -txb.Z / txb.X;
  }
  tangent.normalize();
  binormal.normalize();
  normal = tangent.crossProduct(binormal);
  normal.normalize();
  binormal = tangent.crossProduct(normal);
  binormal.normalize();
  plane3d<f32> pl(vt1, vt2, vt3);
  if (normal.dotProduct(pl.Normal) < 0.0f) {
    normal *= -1.0f;
  }
#endif // USE_NVIDIA_GLH_VERSION
}
//! Recalculates tangents for a tangent mesh buffer
template <typename T>
void recalculateTangentsT(IMeshBuffer* buffer, bool recalculateNormals, bool smooth, bool angleWeighted)
{
  if (!buffer || (buffer->getVertexType() != EVT_TANGENTS)) {
    return;
  }
  const u32 vtxCnt = buffer->getVertexCount();
  const u32 idxCnt = buffer->getIndexCount();
  T* idx = reinterpret_cast<T*>(buffer->getIndices());
  S3DVertex* v =
      (S3DVertex*)buffer->getVertices();
  if (smooth) {
    u32 i;
    for (i = 0; i != vtxCnt; ++i) {
      if (recalculateNormals) {
        v[i].Normal.set(0.f, 0.f, 0.f);
      }
      v[i].Tangent.set(0.f, 0.f, 0.f);
      v[i].Binormal.set(0.f, 0.f, 0.f);
    }
    //Each vertex gets the sum of the tangents and binormals from the faces around it
    for (i = 0; i < idxCnt; i += 3) {
      // if this triangle is degenerate, skip it!
      if (v[idx[i + 0]].Pos == v[idx[i + 1]].Pos ||
          v[idx[i + 0]].Pos == v[idx[i + 2]].Pos ||
          v[idx[i + 1]].Pos == v[idx[i + 2]].Pos
          /*||
          v[idx[i+0]].TCoords == v[idx[i+1]].TCoords ||
          v[idx[i+0]].TCoords == v[idx[i+2]].TCoords ||
          v[idx[i+1]].TCoords == v[idx[i+2]].TCoords */
         ) {
        continue;
      }
      //Angle-weighted normals look better, but are slightly more CPU intensive to calculate
      vector3df weight(1.f, 1.f, 1.f);
      if (angleWeighted) {
        weight = getAngleWeight(v[i + 0].Pos, v[i + 1].Pos, v[i + 2].Pos);  // writing  necessary for borland
      }
      vector3df localNormal;
      vector3df localTangent;
      vector3df localBinormal;
      calculateTangents(
          localNormal,
          localTangent,
          localBinormal,
          v[idx[i + 0]].Pos,
          v[idx[i + 1]].Pos,
          v[idx[i + 2]].Pos,
          v[idx[i + 0]].TCoords,
          v[idx[i + 1]].TCoords,
          v[idx[i + 2]].TCoords);
      if (recalculateNormals) {
        v[idx[i + 0]].Normal += localNormal * weight.X;
      }
      v[idx[i + 0]].Tangent += localTangent * weight.X;
      v[idx[i + 0]].Binormal += localBinormal * weight.X;
      calculateTangents(
          localNormal,
          localTangent,
          localBinormal,
          v[idx[i + 1]].Pos,
          v[idx[i + 2]].Pos,
          v[idx[i + 0]].Pos,
          v[idx[i + 1]].TCoords,
          v[idx[i + 2]].TCoords,
          v[idx[i + 0]].TCoords);
      if (recalculateNormals) {
        v[idx[i + 1]].Normal += localNormal * weight.Y;
      }
      v[idx[i + 1]].Tangent += localTangent * weight.Y;
      v[idx[i + 1]].Binormal += localBinormal * weight.Y;
      calculateTangents(
          localNormal,
          localTangent,
          localBinormal,
          v[idx[i + 2]].Pos,
          v[idx[i + 0]].Pos,
          v[idx[i + 1]].Pos,
          v[idx[i + 2]].TCoords,
          v[idx[i + 0]].TCoords,
          v[idx[i + 1]].TCoords);
      if (recalculateNormals) {
        v[idx[i + 2]].Normal += localNormal * weight.Z;
      }
      v[idx[i + 2]].Tangent += localTangent * weight.Z;
      v[idx[i + 2]].Binormal += localBinormal * weight.Z;
    }
    // Normalize the tangents and binormals
    if (recalculateNormals) {
      for (i = 0; i != vtxCnt; ++i) {
        v[i].Normal.normalize();
      }
    }
    for (i = 0; i != vtxCnt; ++i) {
      v[i].Tangent.normalize();
      v[i].Binormal.normalize();
    }
  }
  else {
    vector3df localNormal;
    int i;
    for (i = 0; i < idxCnt; i += 3) {
      calculateTangents(
          localNormal,
          v[idx[i + 0]].Tangent,
          v[idx[i + 0]].Binormal,
          v[idx[i + 0]].Pos,
          v[idx[i + 1]].Pos,
          v[idx[i + 2]].Pos,
          v[idx[i + 0]].TCoords,
          v[idx[i + 1]].TCoords,
          v[idx[i + 2]].TCoords);
      if (recalculateNormals) {
        v[idx[i + 0]].Normal = localNormal;
      }
      calculateTangents(
          localNormal,
          v[idx[i + 1]].Tangent,
          v[idx[i + 1]].Binormal,
          v[idx[i + 1]].Pos,
          v[idx[i + 2]].Pos,
          v[idx[i + 0]].Pos,
          v[idx[i + 1]].TCoords,
          v[idx[i + 2]].TCoords,
          v[idx[i + 0]].TCoords);
      if (recalculateNormals) {
        v[idx[i + 1]].Normal = localNormal;
      }
      calculateTangents(
          localNormal,
          v[idx[i + 2]].Tangent,
          v[idx[i + 2]].Binormal,
          v[idx[i + 2]].Pos,
          v[idx[i + 0]].Pos,
          v[idx[i + 1]].Pos,
          v[idx[i + 2]].TCoords,
          v[idx[i + 0]].TCoords,
          v[idx[i + 1]].TCoords);
      if (recalculateNormals) {
        v[idx[i + 2]].Normal = localNormal;
      }
    }
  }
}
}
//! Recalculates tangents for a tangent mesh buffer
void CMeshManipulator::recalculateTangents(IMeshBuffer* buffer, bool recalculateNormals, bool smooth, bool angleWeighted) const
{
  if (buffer && (buffer->getVertexType() == EVT_TANGENTS)) {
    if (buffer->getIndexType() == EIT_16BIT) {
      recalculateTangentsT<u16>(buffer, recalculateNormals, smooth, angleWeighted);
    }
    else {
      recalculateTangentsT<u32>(buffer, recalculateNormals, smooth, angleWeighted);
    }
  }
}
//! Recalculates tangents for all tangent mesh buffers
void CMeshManipulator::recalculateTangents(IMesh* mesh, bool recalculateNormals, bool smooth, bool angleWeighted) const
{
  if (!mesh) {
    return;
  }
  const u32 meshBufferCount = mesh->getMeshBufferCount();
  for (int b = 0; b < meshBufferCount; ++b) {
    recalculateTangents(mesh->getMeshBuffer(b), recalculateNormals, smooth, angleWeighted);
  }
}
//! Creates a planar texture mapping on the meshbuffer
template<typename T>
void makePlanarTextureMappingT(IMeshBuffer* buffer, f32 resolution)
{
  u32 idxcnt = buffer->getIndexCount();
  T* idx = reinterpret_cast<T*>(buffer->getIndices());
  int i;
  for (i = 0; i < idxcnt; i += 3) {
    plane3df p(buffer->getPosition(idx[i + 0]), buffer->getPosition(idx[i + 1]), buffer->getPosition(idx[i + 2]));
    p.Normal.X = fabsf(p.Normal.X);
    p.Normal.Y = fabsf(p.Normal.Y);
    p.Normal.Z = fabsf(p.Normal.Z);
    // calculate planar mapping worldspace coordinates
    if (p.Normal.X > p.Normal.Y && p.Normal.X > p.Normal.Z) {
      for (int o = 0; o != 3; ++o) {
        buffer->getTCoords(idx[i + o]).X = buffer->getPosition(idx[i + o]).Y * resolution;
        buffer->getTCoords(idx[i + o]).Y = buffer->getPosition(idx[i + o]).Z * resolution;
      }
    }
    else if (p.Normal.Y > p.Normal.X && p.Normal.Y > p.Normal.Z) {
      for (int o = 0; o != 3; ++o) {
        buffer->getTCoords(idx[i + o]).X = buffer->getPosition(idx[i + o]).X * resolution;
        buffer->getTCoords(idx[i + o]).Y = buffer->getPosition(idx[i + o]).Z * resolution;
      }
    }
    else {
      for (int o = 0; o != 3; ++o) {
        buffer->getTCoords(idx[i + o]).X = buffer->getPosition(idx[i + o]).X * resolution;
        buffer->getTCoords(idx[i + o]).Y = buffer->getPosition(idx[i + o]).Y * resolution;
      }
    }
  }
}
}
//! Creates a planar texture mapping on the meshbuffer
void CMeshManipulator::makePlanarTextureMapping(IMeshBuffer* buffer, f32 resolution) const
{
  if (!buffer) {
    return;
  }
  if (buffer->getIndexType() == EIT_16BIT) {
    makePlanarTextureMappingT<u16>(buffer, resolution);
  }
  else {
    makePlanarTextureMappingT<u32>(buffer, resolution);
  }
}
//! Creates a planar texture mapping on the mesh
void CMeshManipulator::makePlanarTextureMapping(IMesh* mesh, f32 resolution) const
{
  if (!mesh) {
    return;
  }
  const u32 bcount = mesh->getMeshBufferCount();
  for (int b = 0; b < bcount; ++b) {
    makePlanarTextureMapping(mesh->getMeshBuffer(b), resolution);
  }
}
//! Creates a planar texture mapping on the meshbuffer
template <typename T>
void makePlanarTextureMappingT(IMeshBuffer* buffer, f32 resolutionS, f32 resolutionT, u8 axis, const float3 offset)
{
  u32 idxcnt = buffer->getIndexCount();
  T* idx = reinterpret_cast<T*>(buffer->getIndices());
  int i;
  for (i = 0; i < idxcnt; i += 3) {
    // calculate planar mapping worldspace coordinates
    if (axis == 0) {
      for (int o = 0; o != 3; ++o) {
        buffer->getTCoords(idx[i + o]).X = 0.5f + (buffer->getPosition(idx[i + o]).Z + offset.Z) * resolutionS;
        buffer->getTCoords(idx[i + o]).Y = 0.5f - (buffer->getPosition(idx[i + o]).Y + offset.Y) * resolutionT;
      }
    }
    else if (axis == 1) {
      for (int o = 0; o != 3; ++o) {
        buffer->getTCoords(idx[i + o]).X = 0.5f + (buffer->getPosition(idx[i + o]).X + offset.X) * resolutionS;
        buffer->getTCoords(idx[i + o]).Y = 1.f - (buffer->getPosition(idx[i + o]).Z + offset.Z) * resolutionT;
      }
    }
    else if (axis == 2) {
      for (int o = 0; o != 3; ++o) {
        buffer->getTCoords(idx[i + o]).X = 0.5f + (buffer->getPosition(idx[i + o]).X + offset.X) * resolutionS;
        buffer->getTCoords(idx[i + o]).Y = 0.5f - (buffer->getPosition(idx[i + o]).Y + offset.Y) * resolutionT;
      }
    }
  }
}
}
//! Creates a planar texture mapping on the meshbuffer
void CMeshManipulator::makePlanarTextureMapping(IMeshBuffer* buffer, f32 resolutionS, f32 resolutionT, u8 axis, const float3 offset) const
{
  if (!buffer) {
    return;
  }
  if (buffer->getIndexType() == EIT_16BIT) {
    makePlanarTextureMappingT<u16>(buffer, resolutionS, resolutionT, axis, offset);
  }
  else {
    makePlanarTextureMappingT<u32>(buffer, resolutionS, resolutionT, axis, offset);
  }
}
//! Creates a planar texture mapping on the mesh
void CMeshManipulator::makePlanarTextureMapping(IMesh* mesh, f32 resolutionS, f32 resolutionT, u8 axis, const float3 offset) const
{
  if (!mesh) {
    return;
  }
  const u32 bcount = mesh->getMeshBufferCount();
  for (int b = 0; b < bcount; ++b) {
    makePlanarTextureMapping(mesh->getMeshBuffer(b), resolutionS, resolutionT, axis, offset);
  }
}
//! Clones a static IMesh into a modifyable SMesh.
// not yet 32bit
SMesh* CMeshManipulator::createMeshCopy(IMesh* mesh) const
{
  int i, b;
  if (!mesh) {
    return 0;
  }
  SMesh* clone = new SMesh();
  const u32 meshBufferCount = mesh->getMeshBufferCount();
  for (b = 0; b < meshBufferCount; ++b) {
    const IMeshBuffer* const mb = mesh->getMeshBuffer(b);
    switch (mb->getVertexType()) {
    case EVT_STANDARD: {
      SMeshBuffer* buffer = new SMeshBuffer();
      buffer->Material = mb->getMaterial();
      const u32 vcount = mb->getVertexCount();
      buffer->Vertices.reallocate(vcount);
      S3DVertex* vertices = (S3DVertex*)mb->getVertices();
      for (i = 0; i < vcount; ++i) {
        buffer->Vertices.push_back(vertices[i]);
      }
      const u32 icount = mb->getIndexCount();
      buffer->Indices.reallocate(icount);
      const u16* indices = mb->getIndices();
      for (i = 0; i < icount; ++i) {
        buffer->Indices.push_back(indices[i]);
      }
      clone->addMeshBuffer(buffer);
      buffer->drop();
    }
    break;
    case EVT_2TCOORDS: {
      SMeshBufferLightMap* buffer = new SMeshBufferLightMap();
      buffer->Material = mb->getMaterial();
      const u32 vcount = mb->getVertexCount();
      buffer->Vertices.reallocate(vcount);
      S3DVertex* vertices = (S3DVertex*)mb->getVertices();
      for (i = 0; i < vcount; ++i) {
        buffer->Vertices.push_back(vertices[i]);
      }
      const u32 icount = mb->getIndexCount();
      buffer->Indices.reallocate(icount);
      const u16* indices = mb->getIndices();
      for (i = 0; i < icount; ++i) {
        buffer->Indices.push_back(indices[i]);
      }
      clone->addMeshBuffer(buffer);
      buffer->drop();
    }
    break;
    case EVT_TANGENTS: {
      SMeshBufferTangents* buffer = new SMeshBufferTangents();
      buffer->Material = mb->getMaterial();
      const u32 vcount = mb->getVertexCount();
      buffer->Vertices.reallocate(vcount);
      S3DVertex* vertices = (S3DVertex*)mb->getVertices();
      for (i = 0; i < vcount; ++i) {
        buffer->Vertices.push_back(vertices[i]);
      }
      const u32 icount = mb->getIndexCount();
      buffer->Indices.reallocate(icount);
      const u16* indices = mb->getIndices();
      for (i = 0; i < icount; ++i) {
        buffer->Indices.push_back(indices[i]);
      }
      clone->addMeshBuffer(buffer);
      buffer->drop();
    }
    break;
    }// end switch
  }// end for all mesh buffers
  clone->BoundingBox = mesh->getBoundingBox();
  return clone;
}
//! Creates a copy of the mesh, which will only consist of unique primitives
// not yet 32bit
IMesh* CMeshManipulator::createMeshUniquePrimitives(IMesh* mesh) const
{
  if (!mesh) {
    return 0;
  }
  SMesh* clone = new SMesh();
  const u32 meshBufferCount = mesh->getMeshBufferCount();
  for (int b = 0; b < meshBufferCount; ++b) {
    const IMeshBuffer* const mb = mesh->getMeshBuffer(b);
    const s32 idxCnt = mb->getIndexCount();
    const u16* idx = mb->getIndices();
    switch (mb->getVertexType()) {
    case EVT_STANDARD: {
      SMeshBuffer* buffer = new SMeshBuffer();
      buffer->Material = mb->getMaterial();
      S3DVertex* v =
          (S3DVertex*)mb->getVertices();
      buffer->Vertices.reallocate(idxCnt);
      buffer->Indices.reallocate(idxCnt);
      for (s32 i = 0; i < idxCnt; i += 3) {
        buffer->Vertices.push_back(v[idx[i + 0 ]]);
        buffer->Vertices.push_back(v[idx[i + 1 ]]);
        buffer->Vertices.push_back(v[idx[i + 2 ]]);
        buffer->Indices.push_back(i + 0);
        buffer->Indices.push_back(i + 1);
        buffer->Indices.push_back(i + 2);
      }
      buffer->setBoundingBox(mb->getBoundingBox());
      clone->addMeshBuffer(buffer);
      buffer->drop();
    }
    break;
    case EVT_2TCOORDS: {
      SMeshBufferLightMap* buffer = new SMeshBufferLightMap();
      buffer->Material = mb->getMaterial();
      S3DVertex* v =
          (S3DVertex*)mb->getVertices();
      buffer->Vertices.reallocate(idxCnt);
      buffer->Indices.reallocate(idxCnt);
      for (s32 i = 0; i < idxCnt; i += 3) {
        buffer->Vertices.push_back(v[idx[i + 0 ]]);
        buffer->Vertices.push_back(v[idx[i + 1 ]]);
        buffer->Vertices.push_back(v[idx[i + 2 ]]);
        buffer->Indices.push_back(i + 0);
        buffer->Indices.push_back(i + 1);
        buffer->Indices.push_back(i + 2);
      }
      buffer->setBoundingBox(mb->getBoundingBox());
      clone->addMeshBuffer(buffer);
      buffer->drop();
    }
    break;
    case EVT_TANGENTS: {
      SMeshBufferTangents* buffer = new SMeshBufferTangents();
      buffer->Material = mb->getMaterial();
      S3DVertex* v =
          (S3DVertex*)mb->getVertices();
      buffer->Vertices.reallocate(idxCnt);
      buffer->Indices.reallocate(idxCnt);
      for (s32 i = 0; i < idxCnt; i += 3) {
        buffer->Vertices.push_back(v[idx[i + 0 ]]);
        buffer->Vertices.push_back(v[idx[i + 1 ]]);
        buffer->Vertices.push_back(v[idx[i + 2 ]]);
        buffer->Indices.push_back(i + 0);
        buffer->Indices.push_back(i + 1);
        buffer->Indices.push_back(i + 2);
      }
      buffer->setBoundingBox(mb->getBoundingBox());
      clone->addMeshBuffer(buffer);
      buffer->drop();
    }
    break;
    }// end switch
  }// end for all mesh buffers
  clone->BoundingBox = mesh->getBoundingBox();
  return clone;
}
//! Creates a copy of a mesh, which will have identical vertices welded together
// not yet 32bit
IMesh* CMeshManipulator::createMeshWelded(IMesh* mesh, f32 tolerance) const
{
  SMesh* clone = new SMesh();
  clone->BoundingBox = mesh->getBoundingBox();
  u16* redirects;
  for (int b = 0; b < mesh->getMeshBufferCount(); ++b) {
    const IMeshBuffer* const mb = mesh->getMeshBuffer(b);
    // reset redirect list
    redirects.set_used(mb->getVertexCount());
    const u16* indices = 0;
    u32 indexCount = 0;
    u16** outIdx = 0;
    switch (mb->getVertexType()) {
    case EVT_STANDARD: {
      SMeshBuffer* buffer = new SMeshBuffer();
      buffer->BoundingBox = mb->getBoundingBox();
      buffer->Material = mb->getMaterial();
      clone->addMeshBuffer(buffer);
      buffer->drop();
      S3DVertex* v =
          (S3DVertex*)mb->getVertices();
      u32 vertexCount = mb->getVertexCount();
      indices = mb->getIndices();
      indexCount = mb->getIndexCount();
      outIdx = &buffer->Indices;
      buffer->Vertices.reallocate(vertexCount);
      int i;
      for (i = 0; i < vertexCount; ++i) {
        bool found = false;
        for (int j = 0; j < i; ++j) {
          if (v[i].Pos.equals(v[j].Pos, tolerance) &&
              v[i].Normal.equals(v[j].Normal, tolerance) &&
              v[i].TCoords.equals(v[j].TCoords) &&
              (v[i].Color == v[j].Color)) {
            redirects[i] = redirects[j];
            found = true;
            break;
          }
        }
        if (!found) {
          redirects[i] = buffer->Vertices_size;
          buffer->Vertices.push_back(v[i]);
        }
      }
      break;
    }
    case EVT_2TCOORDS: {
      SMeshBufferLightMap* buffer = new SMeshBufferLightMap();
      buffer->BoundingBox = mb->getBoundingBox();
      buffer->Material = mb->getMaterial();
      clone->addMeshBuffer(buffer);
      buffer->drop();
      S3DVertex* v =
          (S3DVertex*)mb->getVertices();
      u32 vertexCount = mb->getVertexCount();
      indices = mb->getIndices();
      indexCount = mb->getIndexCount();
      outIdx = &buffer->Indices;
      buffer->Vertices.reallocate(vertexCount);
      int i;
      for (i = 0; i < vertexCount; ++i) {
        bool found = false;
        for (int j = 0; j < i; ++j) {
          if (v[i].Pos.equals(v[j].Pos, tolerance) &&
              v[i].Normal.equals(v[j].Normal, tolerance) &&
              v[i].TCoords.equals(v[j].TCoords) &&
              v[i].TCoords2.equals(v[j].TCoords2) &&
              (v[i].Color == v[j].Color)) {
            redirects[i] = redirects[j];
            found = true;
            break;
          }
        }
        if (!found) {
          redirects[i] = buffer->Vertices_size;
          buffer->Vertices.push_back(v[i]);
        }
      }
      break;
    }
    case EVT_TANGENTS: {
      SMeshBufferTangents* buffer = new SMeshBufferTangents();
      buffer->BoundingBox = mb->getBoundingBox();
      buffer->Material = mb->getMaterial();
      clone->addMeshBuffer(buffer);
      buffer->drop();
      S3DVertex* v =
          (S3DVertex*)mb->getVertices();
      u32 vertexCount = mb->getVertexCount();
      indices = mb->getIndices();
      indexCount = mb->getIndexCount();
      outIdx = &buffer->Indices;
      buffer->Vertices.reallocate(vertexCount);
      int i;
      for (i = 0; i < vertexCount; ++i) {
        bool found = false;
        for (int j = 0; j < i; ++j) {
          if (v[i].Pos.equals(v[j].Pos, tolerance) &&
              v[i].Normal.equals(v[j].Normal, tolerance) &&
              v[i].TCoords.equals(v[j].TCoords) &&
              v[i].Tangent.equals(v[j].Tangent, tolerance) &&
              v[i].Binormal.equals(v[j].Binormal, tolerance) &&
              (v[i].Color == v[j].Color)) {
            redirects[i] = redirects[j];
            found = true;
            break;
          }
        }
        if (!found) {
          redirects[i] = buffer->Vertices_size;
          buffer->Vertices.push_back(v[i]);
        }
      }
      break;
    }
    default:
      os::Printer::log("Cannot create welded mesh, vertex type unsupported", ELL_ERROR);
      break;
    }
    // write the buffer's index list
    u16* Indices = *outIdx;
    Indices.set_used(indexCount);
    int i;
    for (i = 0; i < indexCount; ++i) {
      Indices[i] = redirects[ indices[i] ];
    }
  }
  return clone;
}
//! Creates a copy of the mesh, which will only consist of S3DVertex vertices.
// not yet 32bit
IMesh* CMeshManipulator::createMeshWithTangents(IMesh* mesh, bool recalculateNormals, bool smooth, bool angleWeighted, bool calculateTangents) const
{
  if (!mesh) {
    return 0;
  }
  // copy mesh and fill data into SMeshBufferTangents
  SMesh* clone = new SMesh();
  const u32 meshBufferCount = mesh->getMeshBufferCount();
  for (int b = 0; b < meshBufferCount; ++b) {
    const IMeshBuffer* const original = mesh->getMeshBuffer(b);
    const u32 idxCnt = original->getIndexCount();
    const u16* idx = original->getIndices();
    SMeshBufferTangents* buffer = new SMeshBufferTangents();
    buffer->Material = original->getMaterial();
    buffer->Vertices.reallocate(idxCnt);
    buffer->Indices.reallocate(idxCnt);
    map<S3DVertex, int> vertMap;
    int vertLocation;
    // copy vertices
    const E_VERTEX_TYPE vType = original->getVertexType();
    S3DVertex vNew;
    int i;
    for (i = 0; i < idxCnt; ++i) {
      switch (vType) {
      case EVT_STANDARD: {
        const S3DVertex* v =
            (const S3DVertex*)original->getVertices();
        vNew = S3DVertex(
            v[idx[i]].Pos, v[idx[i]].Normal, v[idx[i]].Color, v[idx[i]].TCoords);
      }
      break;
      case EVT_2TCOORDS: {
        const S3DVertex* v =
            (const S3DVertex*)original->getVertices();
        vNew = S3DVertex(
            v[idx[i]].Pos, v[idx[i]].Normal, v[idx[i]].Color, v[idx[i]].TCoords);
      }
      break;
      case EVT_TANGENTS: {
        const S3DVertex* v =
            (const S3DVertex*)original->getVertices();
        vNew = v[idx[i]];
      }
      break;
      }
      map<S3DVertex, int>::Node* n = vertMap.find(vNew);
      if (n) {
        vertLocation = n->getValue();
      }
      else {
        vertLocation = buffer->Vertices_size;
        buffer->Vertices.push_back(vNew);
        vertMap.insert(vNew, vertLocation);
      }
      // create new indices
      buffer->Indices.push_back(vertLocation);
    }
    buffer->recalculateBoundingBox();
    // add new buffer
    clone->addMeshBuffer(buffer);
    buffer->drop();
  }
  clone->recalculateBoundingBox();
  if (calculateTangents) {
    recalculateTangents(clone, recalculateNormals, smooth, angleWeighted);
  }
  return clone;
}
//! Creates a copy of the mesh, which will only consist of S3DVertex vertices.
// not yet 32bit
IMesh* CMeshManipulator::createMeshWith2TCoords(IMesh* mesh) const
{
  if (!mesh) {
    return 0;
  }
  // copy mesh and fill data into SMeshBufferLightMap
  SMesh* clone = new SMesh();
  const u32 meshBufferCount = mesh->getMeshBufferCount();
  for (int b = 0; b < meshBufferCount; ++b) {
    const IMeshBuffer* const original = mesh->getMeshBuffer(b);
    const u32 idxCnt = original->getIndexCount();
    const u16* idx = original->getIndices();
    SMeshBufferLightMap* buffer = new SMeshBufferLightMap();
    buffer->Material = original->getMaterial();
    buffer->Vertices.reallocate(idxCnt);
    buffer->Indices.reallocate(idxCnt);
    map<S3DVertex, int> vertMap;
    int vertLocation;
    // copy vertices
    const E_VERTEX_TYPE vType = original->getVertexType();
    S3DVertex vNew;
    int i;
    for (i = 0; i < idxCnt; ++i) {
      switch (vType) {
      case EVT_STANDARD: {
        const S3DVertex* v =
            (const S3DVertex*)original->getVertices();
        vNew = S3DVertex(
            v[idx[i]].Pos, v[idx[i]].Normal, v[idx[i]].Color, v[idx[i]].TCoords, v[idx[i]].TCoords);
      }
      break;
      case EVT_2TCOORDS: {
        const S3DVertex* v =
            (const S3DVertex*)original->getVertices();
        vNew = v[idx[i]];
      }
      break;
      case EVT_TANGENTS: {
        const S3DVertex* v =
            (const S3DVertex*)original->getVertices();
        vNew = S3DVertex(
            v[idx[i]].Pos, v[idx[i]].Normal, v[idx[i]].Color, v[idx[i]].TCoords, v[idx[i]].TCoords);
      }
      break;
      }
      map<S3DVertex, int>::Node* n = vertMap.find(vNew);
      if (n) {
        vertLocation = n->getValue();
      }
      else {
        vertLocation = buffer->Vertices_size;
        buffer->Vertices.push_back(vNew);
        vertMap.insert(vNew, vertLocation);
      }
      // create new indices
      buffer->Indices.push_back(vertLocation);
    }
    buffer->recalculateBoundingBox();
    // add new buffer
    clone->addMeshBuffer(buffer);
    buffer->drop();
  }
  clone->recalculateBoundingBox();
  return clone;
}
//! Creates a copy of the mesh, which will only consist of S3DVertex vertices.
// not yet 32bit
IMesh* CMeshManipulator::createMeshWith1TCoords(IMesh* mesh) const
{
  if (!mesh) {
    return 0;
  }
  // copy mesh and fill data into SMeshBuffer
  SMesh* clone = new SMesh();
  const u32 meshBufferCount = mesh->getMeshBufferCount();
  for (int b = 0; b < meshBufferCount; ++b) {
    IMeshBuffer* original = mesh->getMeshBuffer(b);
    const u32 idxCnt = original->getIndexCount();
    const u16* idx = original->getIndices();
    SMeshBuffer* buffer = new SMeshBuffer();
    buffer->Material = original->getMaterial();
    buffer->Vertices.reallocate(idxCnt);
    buffer->Indices.reallocate(idxCnt);
    map<S3DVertex, int> vertMap;
    int vertLocation;
    // copy vertices
    const E_VERTEX_TYPE vType = original->getVertexType();
    S3DVertex vNew;
    int i;
    for (i = 0; i < idxCnt; ++i) {
      switch (vType) {
      case EVT_STANDARD: {
        S3DVertex* v =
            (S3DVertex*)original->getVertices();
        vNew = v[idx[i]];
      }
      break;
      case EVT_2TCOORDS: {
        S3DVertex* v =
            (S3DVertex*)original->getVertices();
        vNew = S3DVertex(
            v[idx[i]].Pos, v[idx[i]].Normal, v[idx[i]].Color, v[idx[i]].TCoords);
      }
      break;
      case EVT_TANGENTS: {
        S3DVertex* v =
            (S3DVertex*)original->getVertices();
        vNew = S3DVertex(
            v[idx[i]].Pos, v[idx[i]].Normal, v[idx[i]].Color, v[idx[i]].TCoords);
      }
      break;
      }
      map<S3DVertex, int>::Node* n = vertMap.find(vNew);
      if (n) {
        vertLocation = n->getValue();
      }
      else {
        vertLocation = buffer->Vertices_size;
        buffer->Vertices.push_back(vNew);
        vertMap.insert(vNew, vertLocation);
      }
      // create new indices
      buffer->Indices.push_back(vertLocation);
    }
    buffer->recalculateBoundingBox();
    // add new buffer
    clone->addMeshBuffer(buffer);
    buffer->drop();
  }
  clone->recalculateBoundingBox();
  return clone;
}
//! Returns amount of polygons in mesh.
s32 CMeshManipulator::getPolyCount(IMesh* mesh) const
{
  if (!mesh) {
    return 0;
  }
  s32 trianglecount = 0;
  for (int g = 0; g < mesh->getMeshBufferCount(); ++g) {
    trianglecount += mesh->getMeshBuffer(g)->getIndexCount() / 3;
  }
  return trianglecount;
}
//! Returns amount of polygons in mesh.
s32 CMeshManipulator::getPolyCount(IAnimatedMesh* mesh) const
{
  if (mesh && mesh->getFrameCount() != 0) {
    return getPolyCount(mesh->getMesh(0));
  }
  return 0;
}
//! create a new AnimatedMesh and adds the mesh to it
IAnimatedMesh* CMeshManipulator::createAnimatedMesh(IMesh* mesh, E_ANIMATED_MESH_TYPE type) const
{
  return new SAnimatedMesh(mesh, type);
}
struct vcache {
  u32* tris;
  float score;
  s16 cachepos;
  u16 NumActiveTris;
};
struct tcache {
  u16 ind[3];
  float score;
  bool drawn;
};
const u16 cachesize = 32;
    float FindVertexScore(vcache* v)
{
  const float CacheDecayPower = 1.5f;
  const float LastTriScore = 0.75f;
  const float ValenceBoostScale = 2.0f;
  const float ValenceBoostPower = 0.5f;
  const float MaxSizeVertexCache = 32.0f;
  if (v->NumActiveTris == 0) {
    // No tri needs this vertex!
    return -1.0f;
  }
  float Score = 0.0f;
  int CachePosition = v->cachepos;
  if (CachePosition < 0) {
    // Vertex is not in FIFO cache - no score.
  }
  else {
    if (CachePosition < 3) {
      // This vertex was used in the last triangle,
      // so it has a fixed score.
      Score = LastTriScore;
    }
    else {
      // Points for being high in the cache.
      const float Scaler = 1.0f / (MaxSizeVertexCache - 3);
      Score = 1.0f - (CachePosition - 3) * Scaler;
      Score = powf(Score, CacheDecayPower);
    }
  }
  // Bonus points for having a low number of tris still to
  // use the vert, so we get rid of lone verts quickly.
  float ValenceBoost = powf(v->NumActiveTris,
      -ValenceBoostPower);
  Score += ValenceBoostScale * ValenceBoost;
  return Score;
}
/*
  A specialized LRU cache for the Forsyth algorithm.
*/
class f_lru
{
public:
  f_lru(vcache* v, tcache* t): vc(v), tc(t) {
    for (u16 i = 0; i < cachesize; i++) {
      cache[i] = -1;
    }
  }
  // Adds this vertex index and returns the highest-scoring triangle index
  u32 add(u16 vert, bool updatetris = false) {
    bool found = false;
    int i, j, t;
    // Mark existing pos as empty
    for (i = 0; i < cachesize; i++) {
      if (cache[i] == vert) {
        // Move everything down
        for (j = i; j; j--) {
          cache[j] = cache[j - 1];
        }
        found = true;
        break;
      }
    }
    if (!found) {
      if (cache[cachesize - 1] != -1) {
        vc[cache[cachesize - 1]].cachepos = -1;
      }
      // Move everything down
      for (i = cachesize - 1; i; i--) {
        cache[i] = cache[i - 1];
      }
    }
    cache[0] = vert;
    u32 highest = 0;
    float hiscore = 0;
    if (updatetris) {
      // Update cache positions
      for (i = 0; i < cachesize; i++) {
        if (cache[i] == -1) {
          break;
        }
        vc[cache[i]].cachepos = i;
        vc[cache[i]].score = FindVertexScore(&vc[cache[i]]);
      }
      // Update triangle scores
      for (i = 0; i < cachesize; i++) {
        if (cache[i] == -1) {
          break;
        }
        const u16 trisize = vc[cache[i]].tris_size;
        for (t = 0; t < trisize; t++) {
          tcache* tri = &tc[vc[cache[i]].tris[t]];
          tri->score =
              vc[tri->ind[0]].score +
              vc[tri->ind[1]].score +
              vc[tri->ind[2]].score;
          if (tri->score > hiscore) {
            hiscore = tri->score;
            highest = vc[cache[i]].tris[t];
          }
        }
      }
    }
    return highest;
  }
private:
  s32 cache[cachesize];
  vcache* vc;
  tcache* tc;
};
/**
Vertex cache optimization according to the Forsyth paper:
http://home.comcast.net/~tom_forsyth/papers/fast_vert_cache_opt.html
The function is thread-safe (read: you can optimize several meshes in different threads)
\param mesh Source mesh for the operation.  */
IMesh* CMeshManipulator::createForsythOptimizedMesh(const IMesh* mesh) const
{
  if (!mesh) {
    return 0;
  }
  SMesh* newmesh = new SMesh();
  newmesh->BoundingBox = mesh->getBoundingBox();
  const u32 mbcount = mesh->getMeshBufferCount();
  for (int b = 0; b < mbcount; ++b) {
    const IMeshBuffer* mb = mesh->getMeshBuffer(b);
    if (mb->getIndexType() != EIT_16BIT) {
      os::Printer::log("Cannot optimize a mesh with 32bit indices", ELL_ERROR);
      newmesh->drop();
      return 0;
    }
    const u32 icount = mb->getIndexCount();
    const u32 tcount = icount / 3;
    const u32 vcount = mb->getVertexCount();
    const u16* ind = mb->getIndices();
    vcache* vc = new vcache[vcount];
    tcache* tc = new tcache[tcount];
    f_lru lru(vc, tc);
    int i;
    // init
    for (i = 0; i < vcount; i++) {
      vc[i].score = 0;
      vc[i].cachepos = -1;
      vc[i].NumActiveTris = 0;
    }
    // First pass: count how many times a vert is used
    for (i = 0; i < icount; i += 3) {
      vc[ind[i]].NumActiveTris++;
      vc[ind[i + 1]].NumActiveTris++;
      vc[ind[i + 2]].NumActiveTris++;
      const u32 tri_ind = i / 3;
      tc[tri_ind].ind[0] = ind[i];
      tc[tri_ind].ind[1] = ind[i + 1];
      tc[tri_ind].ind[2] = ind[i + 2];
    }
    // Second pass: list of each triangle
    for (i = 0; i < tcount; i++) {
      vc[tc[i].ind[0]].tris.push_back(i);
      vc[tc[i].ind[1]].tris.push_back(i);
      vc[tc[i].ind[2]].tris.push_back(i);
      tc[i].drawn = false;
    }
    // Give initial scores
    for (i = 0; i < vcount; i++) {
      vc[i].score = FindVertexScore(&vc[i]);
    }
    for (i = 0; i < tcount; i++) {
      tc[i].score =
          vc[tc[i].ind[0]].score +
          vc[tc[i].ind[1]].score +
          vc[tc[i].ind[2]].score;
    }
    switch (mb->getVertexType()) {
    case EVT_STANDARD: {
      S3DVertex* v = (S3DVertex*) mb->getVertices();
      SMeshBuffer* buf = new SMeshBuffer();
      buf->Material = mb->getMaterial();
      buf->Vertices.reallocate(vcount);
      buf->Indices.reallocate(icount);
      map<const S3DVertex, const u16> sind; // search index for fast operation
      typedef map<const S3DVertex, const u16>::Node snode;
      // Main algorithm
      u32 highest = 0;
      u32 drawcalls = 0;
      for (;;) {
        if (tc[highest].drawn) {
          bool found = false;
          float hiscore = 0;
          for (int t = 0; t < tcount; t++) {
            if (!tc[t].drawn) {
              if (tc[t].score > hiscore) {
                highest = t;
                hiscore = tc[t].score;
                found = true;
              }
            }
          }
          if (!found) {
            break;
          }
        }
        // Output the best triangle
        u16 newind = buf->Vertices_size;
        snode* s = sind.find(v[tc[highest].ind[0]]);
        if (!s) {
          buf->Vertices.push_back(v[tc[highest].ind[0]]);
          buf->Indices.push_back(newind);
          sind.insert(v[tc[highest].ind[0]], newind);
          newind++;
        }
        else {
          buf->Indices.push_back(s->getValue());
        }
        s = sind.find(v[tc[highest].ind[1]]);
        if (!s) {
          buf->Vertices.push_back(v[tc[highest].ind[1]]);
          buf->Indices.push_back(newind);
          sind.insert(v[tc[highest].ind[1]], newind);
          newind++;
        }
        else {
          buf->Indices.push_back(s->getValue());
        }
        s = sind.find(v[tc[highest].ind[2]]);
        if (!s) {
          buf->Vertices.push_back(v[tc[highest].ind[2]]);
          buf->Indices.push_back(newind);
          sind.insert(v[tc[highest].ind[2]], newind);
        }
        else {
          buf->Indices.push_back(s->getValue());
        }
        vc[tc[highest].ind[0]].NumActiveTris--;
        vc[tc[highest].ind[1]].NumActiveTris--;
        vc[tc[highest].ind[2]].NumActiveTris--;
        tc[highest].drawn = true;
        for (u16 j = 0; j < 3; j++) {
          vcache* vert = &vc[tc[highest].ind[j]];
          for (u16 t = 0; t < vert->tris_size; t++) {
            if (highest == vert->tris[t]) {
              vert->tris.erase(t);
              break;
            }
          }
        }
        lru.add(tc[highest].ind[0]);
        lru.add(tc[highest].ind[1]);
        highest = lru.add(tc[highest].ind[2], true);
        drawcalls++;
      }
      buf->setBoundingBox(mb->getBoundingBox());
      newmesh->addMeshBuffer(buf);
      buf->drop();
    }
    break;
    case EVT_2TCOORDS: {
      S3DVertex* v = (S3DVertex*) mb->getVertices();
      SMeshBufferLightMap* buf = new SMeshBufferLightMap();
      buf->Material = mb->getMaterial();
      buf->Vertices.reallocate(vcount);
      buf->Indices.reallocate(icount);
      map<const S3DVertex, const u16> sind; // search index for fast operation
      typedef map<const S3DVertex, const u16>::Node snode;
      // Main algorithm
      u32 highest = 0;
      u32 drawcalls = 0;
      for (;;) {
        if (tc[highest].drawn) {
          bool found = false;
          float hiscore = 0;
          for (int t = 0; t < tcount; t++) {
            if (!tc[t].drawn) {
              if (tc[t].score > hiscore) {
                highest = t;
                hiscore = tc[t].score;
                found = true;
              }
            }
          }
          if (!found) {
            break;
          }
        }
        // Output the best triangle
        u16 newind = buf->Vertices_size;
        snode* s = sind.find(v[tc[highest].ind[0]]);
        if (!s) {
          buf->Vertices.push_back(v[tc[highest].ind[0]]);
          buf->Indices.push_back(newind);
          sind.insert(v[tc[highest].ind[0]], newind);
          newind++;
        }
        else {
          buf->Indices.push_back(s->getValue());
        }
        s = sind.find(v[tc[highest].ind[1]]);
        if (!s) {
          buf->Vertices.push_back(v[tc[highest].ind[1]]);
          buf->Indices.push_back(newind);
          sind.insert(v[tc[highest].ind[1]], newind);
          newind++;
        }
        else {
          buf->Indices.push_back(s->getValue());
        }
        s = sind.find(v[tc[highest].ind[2]]);
        if (!s) {
          buf->Vertices.push_back(v[tc[highest].ind[2]]);
          buf->Indices.push_back(newind);
          sind.insert(v[tc[highest].ind[2]], newind);
        }
        else {
          buf->Indices.push_back(s->getValue());
        }
        vc[tc[highest].ind[0]].NumActiveTris--;
        vc[tc[highest].ind[1]].NumActiveTris--;
        vc[tc[highest].ind[2]].NumActiveTris--;
        tc[highest].drawn = true;
        for (u16 j = 0; j < 3; j++) {
          vcache* vert = &vc[tc[highest].ind[j]];
          for (u16 t = 0; t < vert->tris_size; t++) {
            if (highest == vert->tris[t]) {
              vert->tris.erase(t);
              break;
            }
          }
        }
        lru.add(tc[highest].ind[0]);
        lru.add(tc[highest].ind[1]);
        highest = lru.add(tc[highest].ind[2]);
        drawcalls++;
      }
      buf->setBoundingBox(mb->getBoundingBox());
      newmesh->addMeshBuffer(buf);
      buf->drop();
    }
    break;
    case EVT_TANGENTS: {
      S3DVertex* v = (S3DVertex*) mb->getVertices();
      SMeshBufferTangents* buf = new SMeshBufferTangents();
      buf->Material = mb->getMaterial();
      buf->Vertices.reallocate(vcount);
      buf->Indices.reallocate(icount);
      map<const S3DVertex, const u16> sind; // search index for fast operation
      typedef map<const S3DVertex, const u16>::Node snode;
      // Main algorithm
      u32 highest = 0;
      u32 drawcalls = 0;
      for (;;) {
        if (tc[highest].drawn) {
          bool found = false;
          float hiscore = 0;
          for (int t = 0; t < tcount; t++) {
            if (!tc[t].drawn) {
              if (tc[t].score > hiscore) {
                highest = t;
                hiscore = tc[t].score;
                found = true;
              }
            }
          }
          if (!found) {
            break;
          }
        }
        // Output the best triangle
        u16 newind = buf->Vertices_size;
        snode* s = sind.find(v[tc[highest].ind[0]]);
        if (!s) {
          buf->Vertices.push_back(v[tc[highest].ind[0]]);
          buf->Indices.push_back(newind);
          sind.insert(v[tc[highest].ind[0]], newind);
          newind++;
        }
        else {
          buf->Indices.push_back(s->getValue());
        }
        s = sind.find(v[tc[highest].ind[1]]);
        if (!s) {
          buf->Vertices.push_back(v[tc[highest].ind[1]]);
          buf->Indices.push_back(newind);
          sind.insert(v[tc[highest].ind[1]], newind);
          newind++;
        }
        else {
          buf->Indices.push_back(s->getValue());
        }
        s = sind.find(v[tc[highest].ind[2]]);
        if (!s) {
          buf->Vertices.push_back(v[tc[highest].ind[2]]);
          buf->Indices.push_back(newind);
          sind.insert(v[tc[highest].ind[2]], newind);
        }
        else {
          buf->Indices.push_back(s->getValue());
        }
        vc[tc[highest].ind[0]].NumActiveTris--;
        vc[tc[highest].ind[1]].NumActiveTris--;
        vc[tc[highest].ind[2]].NumActiveTris--;
        tc[highest].drawn = true;
        for (u16 j = 0; j < 3; j++) {
          vcache* vert = &vc[tc[highest].ind[j]];
          for (u16 t = 0; t < vert->tris_size; t++) {
            if (highest == vert->tris[t]) {
              vert->tris.erase(t);
              break;
            }
          }
        }
        lru.add(tc[highest].ind[0]);
        lru.add(tc[highest].ind[1]);
        highest = lru.add(tc[highest].ind[2]);
        drawcalls++;
      }
      buf->setBoundingBox(mb->getBoundingBox());
      newmesh->addMeshBuffer(buf);
      buf->drop();
    }
    break;
    }
    delete [] vc;
    delete [] tc;
  } // for each meshbuffer
  return newmesh;
}
#endif

