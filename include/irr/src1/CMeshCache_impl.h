// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
//#include "CMeshCache.h"
//#include "IAnimatedMesh.h"
static const SNamedPath emptyNamedPath;
CMeshCache::~CMeshCache()
{
  clear();
}
//! adds a mesh to the list
void CMeshCache::addMesh(const char* filename, IAnimatedMesh* mesh)
{
  mesh->grab();
  MeshEntry e(filename);
  e.Mesh = mesh;
  Meshes.push_back(e);
}
//! Removes a mesh from the cache.
void CMeshCache::removeMesh(const IMesh* const mesh)
{
  if (!mesh) {
    return;
  }
  int i;
  for (i = 0; i < Meshes_size; ++i) {
    if (Meshes[i].Mesh == mesh || (Meshes[i].Mesh && Meshes[i].Mesh->getMesh(0) == mesh)) {
      Meshes[i].Mesh->drop();
      Meshes.erase(i);
      return;
    }
  }
}
//! Returns amount of loaded meshes
u32 CMeshCache::getMeshCount() const
{
  return Meshes_size;
}
//! Returns current number of the mesh
s32 CMeshCache::getMeshIndex(const IMesh* const mesh) const
{
  int i;
  for (i = 0; i < Meshes_size; ++i) {
    if (Meshes[i].Mesh == mesh || (Meshes[i].Mesh && Meshes[i].Mesh->getMesh(0) == mesh)) {
      return (s32)i;
    }
  }
  return -1;
}
//! Returns a mesh based on its index number
IAnimatedMesh* CMeshCache::getMeshByIndex(u32 number)
{
  if (number >= Meshes_size) {
    return 0;
  }
  return Meshes[number].Mesh;
}
//! Returns a mesh based on its name.
IAnimatedMesh* CMeshCache::getMeshByName(const char* name)
{
  MeshEntry e(name);
  s32 id = Meshes.binary_search(e);
  return (id != -1) ? Meshes[id].Mesh : 0;
}
//! Get the name of a loaded mesh, based on its index.
const char* CMeshCache::getMeshName(u32 index) const
{
  if (index >= Meshes_size) {
    return emptyNamedPath;
  }
  return Meshes[index].NamedPath;
}
//! Get the name of a loaded mesh, if there is any.
const char* CMeshCache::getMeshName(const IMesh* const mesh) const
{
  if (!mesh) {
    return emptyNamedPath;
  }
  int i;
  for (i = 0; i < Meshes_size; ++i) {
    if (Meshes[i].Mesh == mesh || (Meshes[i].Mesh && Meshes[i].Mesh->getMesh(0) == mesh)) {
      return Meshes[i].NamedPath;
    }
  }
  return emptyNamedPath;
}
//! Renames a loaded mesh.
bool CMeshCache::renameMesh(u32 index, const char* name)
{
  if (index >= Meshes_size) {
    return false;
  }
  Meshes[index].NamedPath.setPath(name);
  Meshes.sort();
  return true;
}
//! Renames a loaded mesh.
bool CMeshCache::renameMesh(const IMesh* const mesh, const char* name)
{
  int i;
  for (i = 0; i < Meshes_size; ++i) {
    if (Meshes[i].Mesh == mesh || (Meshes[i].Mesh && Meshes[i].Mesh->getMesh(0) == mesh)) {
      Meshes[i].NamedPath.setPath(name);
      Meshes.sort();
      return true;
    }
  }
  return false;
}
//! returns if a mesh already was loaded
bool CMeshCache::isMeshLoaded(const char* name)
{
  return getMeshByName(name) != 0;
}
//! Clears the whole mesh cache, removing all meshes.
void CMeshCache::clear()
{
  int i;
  for (i = 0; i < Meshes_size; ++i) {
    Meshes[i].Mesh->drop();
  }
  Meshes.clear();
}
//! Clears all meshes that are held in the mesh cache but not used anywhere else.
void CMeshCache::clearUnusedMeshes()
{
  int i;
  for (i = 0; i < Meshes_size; ++i) {
    if (Meshes[i].Mesh->getReferenceCount() == 1) {
      Meshes[i].Mesh->drop();
      Meshes.erase(i);
      --i;
    }
  }
}

