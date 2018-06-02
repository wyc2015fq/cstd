// Copyright (C) 2010-2012 Gaz Davidson / Joseph Ellis
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
#ifdef _IRR_COMPILE_WITH_SMF_LOADER_
//#include "CSMFMeshFileLoader.h"
//#include "SAnimatedMesh.h"
//#include "SMeshBuffer.h"
//#include "IReadFile.h"
//#include "os.h"
//#include "IVideoDriver.h"
CSMFMeshFileLoader::CSMFMeshFileLoader(IVideoDriver* driver)
  : Driver(driver)
{
}
//! Returns true if the file might be loaded by this class.
bool CSMFMeshFileLoader::isALoadableFileExtension(const char* filename) const
{
  return hasFileExtension(filename, "smf");
}
//! Creates/loads an animated mesh from the file.
IAnimatedMesh* CSMFMeshFileLoader::createMesh(IReadFile* file)
{
  // create empty mesh
  SMesh* mesh = new SMesh();
  // load file
  u16 version;
  u8  flags;
  s32 limbCount;
  s32 i;
  BinaryFile::read(file, &version, 2);
  BinaryFile::read(file, &flags, 1);
  BinaryFile::read(file, &limbCount, 4);
  // load mesh data
  matrix4 identity;
  for (i = 0; i < limbCount; ++i) {
    loadLimb(file, mesh, identity);
  }
  // recalculate buffer bounding boxes
  for (i = 0; i < (s32)mesh->getMeshBufferCount(); ++i) {
    mesh->getMeshBuffer(i)->recalculateBoundingBox();
  }
  mesh->recalculateBoundingBox();
  SAnimatedMesh* am = new SAnimatedMesh();
  am->addMesh(mesh);
  mesh->drop();
  am->recalculateBoundingBox();
  return am;
}
void CSMFMeshFileLoader::loadLimb(IReadFile* file, SMesh* mesh, const matrix4& parentTransformation)
{
  matrix4 transformation;
  // limb transformation
  vector3df translate, rotate, scale;
  BinaryFile::read(file, translate);
  BinaryFile::read(file, rotate);
  BinaryFile::read(file, scale);
  transformation.setTranslation(translate);
  transformation.setRotationDegrees(rotate);
  transformation.setScale(scale);
  transformation = parentTransformation * transformation;
  char* textureName, textureGroupName;
  // texture information
  BinaryFile::read(file, textureGroupName);
  BinaryFile::read(file, textureName);
  // attempt to load texture using known formats
  ITexture* texture = 0;
  const c8* extensions[] = {".jpg", ".png", ".tga", ".bmp", 0};
  for (const c8** ext = extensions; !texture && *ext; ++ext) {
    texture = Driver->getTexture(textureName + *ext);
    if (texture) {
      textureName = textureName + *ext;
    }
  }
  // find the correct mesh buffer
  u32 i;
  for (i = 0; i < mesh->MeshBuffers_size; ++i)
    if (mesh->MeshBuffers[i]->getMaterial().TextureLayer[0].Texture == texture) {
      break;
    }
  // create mesh buffer if none was found
  if (i == mesh->MeshBuffers_size) {
    CMeshBuffer<S3DVertex>* mb = new CMeshBuffer<S3DVertex>();
    mb->Material.TextureLayer[0].Texture = texture;
    // horribly hacky way to do this, maybe it's in the flags?
    if (hasFileExtension(textureName, "tga", "png")) {
      mb->Material.MaterialType = EMT_TRANSPARENT_ALPHA_CHANNEL;
    }
    else {
      mb->Material.MaterialType = EMT_SOLID;
    }
    mesh->MeshBuffers.push_back(mb);
  }
  CMeshBuffer<S3DVertex>* mb = (CMeshBuffer<S3DVertex>*)mesh->MeshBuffers[i];
  u16 vertexCount, firstVertex = mb->getVertexCount();
  BinaryFile::read(file, &vertexCount, 2);
  mb->Vertices.reallocate(mb->Vertices_size + vertexCount);
  // add vertices and set positions
  for (i = 0; i < vertexCount; ++i) {
    vector3df pos;
    BinaryFile::read(file, pos);
    transformation.transformVect(pos);
    S3DVertex vert;
    vert.Color = 0xFFFFFFFF;
    vert.Pos = pos;
    mb->Vertices.push_back(vert);
  }
  // set vertex normals
  for (i = 0; i < vertexCount; ++i) {
    vector3df normal;
    BinaryFile::read(file, normal);
    transformation.rotateVect(normal);
    mb->Vertices[firstVertex + i].Normal = normal;
  }
  // set texture coordinates
  for (i = 0; i < vertexCount; ++i) {
    vector2df tcoords;
    BinaryFile::read(file, tcoords);
    mb->Vertices[firstVertex + i].TCoords = tcoords;
  }
  // triangles
  u32 triangleCount;
  // vertexCount used as temporary
  BinaryFile::read(file, &vertexCount, 2);
  triangleCount = 3 * vertexCount;
  mb->Indices.reallocate(mb->Indices_size + triangleCount);
  for (i = 0; i < triangleCount; ++i) {
    u16 index;
    BinaryFile::read(file, &index, 2);
    mb->Indices.push_back(firstVertex + index);
  }
  // read limbs
  s32 limbCount;
  BinaryFile::read(file, &limbCount, 4);
  for (s32 l = 0; l < limbCount; ++l) {
    loadLimb(file, mesh, transformation);
  }
}
//  scene
// todo: at some point in the future let's move these to a place where everyone can use them.
#if _BIGENDIAN
#define _SYSTEM_BIG_ENDIAN_ (true)
#else
#define _SYSTEM_BIG_ENDIAN_ (false)
#endif
void BinaryFile::read(IReadFile* file, void* out, int len, bool bigEndian)
{
  file->read(out, len);
  if (bigEndian != (_SYSTEM_BIG_ENDIAN_)) {
    //out = os::Byteswap::BYTESWAP(out);
    byterev(out, len);
  }
}
//! reads a 3d vector from the file, moving the file pointer along
void BinaryFile::read(IReadFile* file, vector3df& outVector2d, bool bigEndian)
{
  BinaryFile::read(file, &outVector2d.X, sizeof(outVector2d.X), bigEndian);
  BinaryFile::read(file, &outVector2d.Y, sizeof(outVector2d.Y), bigEndian);
  BinaryFile::read(file, &outVector2d.Z, sizeof(outVector2d.Z), bigEndian);
}
//! reads a 2d vector from the file, moving the file pointer along
void BinaryFile::read(IReadFile* file, vector2df& outVector2d, bool bigEndian)
{
  BinaryFile::read(file, &outVector2d.X, sizeof(outVector2d.X), bigEndian);
  BinaryFile::read(file, &outVector2d.Y, sizeof(outVector2d.Y), bigEndian);
}
//! reads a null terminated string from the file, moving the file pointer along
void BinaryFile::read(IReadFile* file, char* outString, bool bigEndian)
{
  c8 c;
  file->read((void*)&c, 1);
  while (c) {
    outString += c;
    file->read((void*)&c, 1);
  }
}
//  io
//  irr
#endif // compile with SMF loader

