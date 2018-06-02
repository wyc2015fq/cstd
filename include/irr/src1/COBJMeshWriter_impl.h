// Copyright (C) 2008-2012 Christian Stehno
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
#ifdef _IRR_COMPILE_WITH_OBJ_WRITER_
//#include "COBJMeshWriter.h"
//#include "os.h"
//#include "IAttributes.h"
//#include "ISceneManager.h"
//#include "IMeshCache.h"
//#include "IWriteFile.h"
//#include "IFileSystem.h"
//#include "ITexture.h"
COBJMeshWriter::COBJMeshWriter(ISceneManager* smgr, IFileSystem* fs)
  : SceneManager(smgr), FileSystem(fs)
{
#ifdef _DEBUG
  setDebugName("COBJMeshWriter");
#endif
  if (SceneManager) {
    SceneManager->grab();
  }
  if (FileSystem) {
    FileSystem->grab();
  }
}
COBJMeshWriter::~COBJMeshWriter()
{
  if (SceneManager) {
    SceneManager->drop();
  }
  if (FileSystem) {
    FileSystem->drop();
  }
}
//! Returns the type of the mesh writer
EMESH_WRITER_TYPE COBJMeshWriter::getType() const
{
  return EMWT_OBJ;
}
//! writes a mesh
bool COBJMeshWriter::writeMesh(IWriteFile* file, IMesh* mesh, s32 flags)
{
  if (!file) {
    return false;
  }
  os::Printer::log("Writing mesh", file->getFileName());
  // write OBJ MESH header
  const char* name(FileSystem->getFileBasename(SceneManager->getMeshCache()->getMeshName(mesh), false) + ".mtl");
  file->write("# exported by Irrlicht\n", 23);
  file->write("mtllib ", 7);
  file->write(name, name_size);
  file->write("\n\n", 2);
  // write mesh buffers
  SMaterial* > mat;
  u32 allVertexCount = 1; // count vertices over the whole file
  int i;
  for (i = 0; i < mesh->getMeshBufferCount(); ++i) {
    char* num(i + 1);
    IMeshBuffer* buffer = mesh->getMeshBuffer(i);
    if (buffer && buffer->getVertexCount()) {
      file->write("g grp", 5);
      file->write(num, num_size);
      file->write("\n", 1);
      u32 j;
      const u32 vertexCount = buffer->getVertexCount();
      for (j = 0; j < vertexCount; ++j) {
        file->write("v ", 2);
        getVectorAsStringLine(buffer->getPosition(j), num);
        file->write(num, num_size);
      }
      for (j = 0; j < vertexCount; ++j) {
        file->write("vt ", 3);
        getVectorAsStringLine(buffer->getTCoords(j), num);
        file->write(num, num_size);
      }
      for (j = 0; j < vertexCount; ++j) {
        file->write("vn ", 3);
        getVectorAsStringLine(buffer->getNormal(j), num);
        file->write(num, num_size);
      }
      file->write("usemtl mat", 10);
      num = "";
      for (j = 0; j < mat_size; ++j) {
        if (*mat[j] == buffer->getMaterial()) {
          num = char * (j);
          break;
        }
      }
      if (num == "") {
        num = char * (mat_size);
        mat.push_back(&buffer->getMaterial());
      }
      file->write(num, num_size);
      file->write("\n", 1);
      const u32 indexCount = buffer->getIndexCount();
      for (j = 0; j < indexCount; j += 3) {
        file->write("f ", 2);
        num = char * (buffer->getIndices()[j + 2] + allVertexCount);
        file->write(num, num_size);
        file->write("/", 1);
        file->write(num, num_size);
        file->write("/", 1);
        file->write(num, num_size);
        file->write(" ", 1);
        num = char * (buffer->getIndices()[j + 1] + allVertexCount);
        file->write(num, num_size);
        file->write("/", 1);
        file->write(num, num_size);
        file->write("/", 1);
        file->write(num, num_size);
        file->write(" ", 1);
        num = char * (buffer->getIndices()[j + 0] + allVertexCount);
        file->write(num, num_size);
        file->write("/", 1);
        file->write(num, num_size);
        file->write("/", 1);
        file->write(num, num_size);
        file->write(" ", 1);
        file->write("\n", 1);
      }
      file->write("\n", 1);
      allVertexCount += vertexCount;
    }
  }
  if (mat_size == 0) {
    return true;
  }
  file = FileSystem->createAndWriteFile(name);
  if (file) {
    os::Printer::log("Writing material", file->getFileName());
    file->write("# exported by Irrlicht\n\n", 24);
    int i;
    for (i = 0; i < mat_size; ++i) {
      char* num(i);
      file->write("newmtl mat", 10);
      file->write(num, num_size);
      file->write("\n", 1);
      getColorAsStringLine(mat[i]->AmbientColor, "Ka", num);
      file->write(num, num_size);
      getColorAsStringLine(mat[i]->DiffuseColor, "Kd", num);
      file->write(num, num_size);
      getColorAsStringLine(mat[i]->SpecularColor, "Ks", num);
      file->write(num, num_size);
      getColorAsStringLine(mat[i]->EmissiveColor, "Ke", num);
      file->write(num, num_size);
      num = char * ((double)(mat[i]->Shininess / 0.128f));
      file->write("Ns ", 3);
      file->write(num, num_size);
      file->write("\n", 1);
      if (mat[i]->getTexture(0)) {
        file->write("map_Kd ", 7);
        file->write(mat[i]->getTexture(0)->getName().getPath(), mat[i]->getTexture(0)->getName().getPath().size());
        file->write("\n", 1);
      }
      file->write("\n", 1);
    }
    file->drop();
  }
  return true;
}
void COBJMeshWriter::getVectorAsStringLine(const float3 v, char* s) const
{
  s = char * (-v.X);
  s += " ";
  s += char * (v.Y);
  s += " ";
  s += char * (v.Z);
  s += "\n";
}
void COBJMeshWriter::getVectorAsStringLine(const vector2df& v, char* s) const
{
  s = char * (v.X);
  s += " ";
  s += char * (-v.Y);
  s += "\n";
}
void COBJMeshWriter::getColorAsStringLine(const SColor& color, const c8* const prefix, char* s) const
{
  s = prefix;
  s += " ";
  s += char * ((double)(color.getRed() / 255.f));
  s += " ";
  s += char * ((double)(color.getGreen() / 255.f));
  s += " ";
  s += char * ((double)(color.getBlue() / 255.f));
  s += "\n";
}
// end
// end
#endif

