// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
#ifdef _IRR_COMPILE_WITH_STL_WRITER_
//#include "CSTLMeshWriter.h"
//#include "os.h"
//#include "IAttributes.h"
//#include "ISceneManager.h"
//#include "IMeshCache.h"
//#include "IWriteFile.h"
CSTLMeshWriter::CSTLMeshWriter(ISceneManager* smgr)
  : SceneManager(smgr)
{
#ifdef _DEBUG
  setDebugName("CSTLMeshWriter");
#endif
  if (SceneManager) {
    SceneManager->grab();
  }
}
CSTLMeshWriter::~CSTLMeshWriter()
{
  if (SceneManager) {
    SceneManager->drop();
  }
}
//! Returns the type of the mesh writer
EMESH_WRITER_TYPE CSTLMeshWriter::getType() const
{
  return EMWT_STL;
}
//! writes a mesh
bool CSTLMeshWriter::writeMesh(IWriteFile* file, IMesh* mesh, s32 flags)
{
  if (!file) {
    return false;
  }
  os::Printer::log("Writing mesh", file->getFileName());
  if (flags & EMWF_WRITE_COMPRESSED) {
    return writeMeshBinary(file, mesh, flags);
  }
  else {
    return writeMeshASCII(file, mesh, flags);
  }
}
bool CSTLMeshWriter::writeMeshBinary(IWriteFile* file, IMesh* mesh, s32 flags)
{
  // write STL MESH header
  file->write("binary ", 7);
  const char* name(SceneManager->getMeshCache()->getMeshName(mesh));
  const s32 sizeleft = 73 - name_size; // 80 byte header
  if (sizeleft < 0) {
    file->write(name, 73);
  }
  else {
    char* buf = new char[80];
    memset(buf, 0, 80);
    file->write(name, name_size);
    file->write(buf, sizeleft);
    delete [] buf;
  }
  u32 facenum = 0;
  for (int j = 0; j < mesh->getMeshBufferCount(); ++j) {
    facenum += mesh->getMeshBuffer(j)->getIndexCount() / 3;
  }
  file->write(&facenum, 4);
  // write mesh buffers
  int i;
  for (i = 0; i < mesh->getMeshBufferCount(); ++i) {
    IMeshBuffer* buffer = mesh->getMeshBuffer(i);
    if (buffer) {
      const u32 indexCount = buffer->getIndexCount();
      const u16 attributes = 0;
      for (int j = 0; j < indexCount; j += 3) {
        const float3 v1 = buffer->getPosition(buffer->getIndices()[j]);
        const float3 v2 = buffer->getPosition(buffer->getIndices()[j + 1]);
        const float3 v3 = buffer->getPosition(buffer->getIndices()[j + 2]);
        const plane3df tmpplane(v1, v2, v3);
        file->write(&tmpplane.Normal, 12);
        file->write(&v1, 12);
        file->write(&v2, 12);
        file->write(&v3, 12);
        file->write(&attributes, 2);
      }
    }
  }
  return true;
}
bool CSTLMeshWriter::writeMeshASCII(IWriteFile* file, IMesh* mesh, s32 flags)
{
  // write STL MESH header
  file->write("solid ", 6);
  const char* name(SceneManager->getMeshCache()->getMeshName(mesh));
  file->write(name, name_size);
  file->write("\n\n", 2);
  // write mesh buffers
  int i;
  for (i = 0; i < mesh->getMeshBufferCount(); ++i) {
    IMeshBuffer* buffer = mesh->getMeshBuffer(i);
    if (buffer) {
      const u32 indexCount = buffer->getIndexCount();
      for (int j = 0; j < indexCount; j += 3) {
        writeFace(file,
            buffer->getPosition(buffer->getIndices()[j]),
            buffer->getPosition(buffer->getIndices()[j + 1]),
            buffer->getPosition(buffer->getIndices()[j + 2]));
      }
      file->write("\n", 1);
    }
  }
  file->write("endsolid ", 9);
  file->write(name, name_size);
  return true;
}
void CSTLMeshWriter::getVectorAsStringLine(const float3 v, char* s) const
{
  s = char * (v.X);
  s += " ";
  s += char * (v.Y);
  s += " ";
  s += char * (v.Z);
  s += "\n";
}
void CSTLMeshWriter::writeFace(IWriteFile* file,
    const float3 v1,
    const float3 v2,
    const float3 v3)
{
  char* tmp;
  file->write("facet normal ", 13);
  getVectorAsStringLine(plane3df(v1, v2, v3).Normal, tmp);
  file->write(tmp, tmp_size);
  file->write("  outer loop\n", 13);
  file->write("    vertex ", 11);
  getVectorAsStringLine(v1, tmp);
  file->write(tmp, tmp_size);
  file->write("    vertex ", 11);
  getVectorAsStringLine(v2, tmp);
  file->write(tmp, tmp_size);
  file->write("    vertex ", 11);
  getVectorAsStringLine(v3, tmp);
  file->write(tmp, tmp_size);
  file->write("  endloop\n", 10);
  file->write("endfacet\n", 9);
}
// end
// end
#endif

