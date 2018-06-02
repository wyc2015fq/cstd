// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
#ifdef _IRR_COMPILE_WITH_IRR_WRITER_
//#include "CIrrMeshWriter.h"
//#include "os.h"
//#include "IWriteFile.h"
//#include "IXMLWriter.h"
//#include "IAttributes.h"
CIrrMeshWriter::CIrrMeshWriter(IVideoDriver* driver,
    IFileSystem* fs)
  : FileSystem(fs), VideoDriver(driver), Writer(0)
{
#ifdef _DEBUG
  setDebugName("CIrrMeshWriter");
#endif
  if (VideoDriver) {
    VideoDriver->grab();
  }
  if (FileSystem) {
    FileSystem->grab();
  }
}
CIrrMeshWriter::~CIrrMeshWriter()
{
  if (VideoDriver) {
    VideoDriver->drop();
  }
  if (FileSystem) {
    FileSystem->drop();
  }
}
//! Returns the type of the mesh writer
EMESH_WRITER_TYPE CIrrMeshWriter::getType() const
{
  return EMWT_IRR_MESH;
}
//! writes a mesh
bool CIrrMeshWriter::writeMesh(IWriteFile* file, IMesh* mesh, s32 flags)
{
  if (!file) {
    return false;
  }
  Writer = FileSystem->createXMLWriter(file);
  if (!Writer) {
    os::Printer::log("Could not write file", file->getFileName());
    return false;
  }
  os::Printer::log("Writing mesh", file->getFileName());
  // write IRR MESH header
  Writer->writeXMLHeader();
  Writer->writeElement("mesh", false,
      "xmlns", "http://irrlicht.sourceforge.net/IRRMESH_09_2007",
      "version", "1.0");
  Writer->writeLineBreak();
  // add some informational comment. Add a space after and before the comment
  // tags so that some braindead xml parsers (AS anyone?) are able to parse this too.
  char* infoComment = " This file contains a static mesh in the Irrlicht Engine format with ";
  infoComment += char * (mesh->getMeshBufferCount());
  infoComment += " materials.";
  Writer->writeComment(infoComment);
  Writer->writeLineBreak();
  // write mesh bounding box
  writeBoundingBox(mesh->getBoundingBox());
  Writer->writeLineBreak();
  // write mesh buffers
  int i;
  for (i = 0; i < (int)mesh->getMeshBufferCount(); ++i) {
    IMeshBuffer* buffer = mesh->getMeshBuffer(i);
    if (buffer) {
      writeMeshBuffer(buffer);
      Writer->writeLineBreak();
    }
  }
  Writer->writeClosingTag("mesh");
  Writer->drop();
  return true;
}
void CIrrMeshWriter::writeBoundingBox(const aabbox3df& box)
{
  Writer->writeElement("boundingBox", true,
      "minEdge", getVectorAsStringLine(box.MinEdge),
      "maxEdge", getVectorAsStringLine(box.MaxEdge));
}
char* CIrrMeshWriter::getVectorAsStringLine(const float3 v) const
{
  char* str;
  str = char * (v.X);
  str += " ";
  str += char * (v.Y);
  str += " ";
  str += char * (v.Z);
  return str;
}
char* CIrrMeshWriter::getVectorAsStringLine(const vector2df& v) const
{
  char* str;
  str = char * (v.X);
  str += " ";
  str += char * (v.Y);
  return str;
}
void CIrrMeshWriter::writeMeshBuffer(const IMeshBuffer* buffer)
{
  Writer->writeElement("buffer", false);
  Writer->writeLineBreak();
  // write bounding box
  writeBoundingBox(buffer->getBoundingBox());
  Writer->writeLineBreak();
  // write material
  writeMaterial(buffer->getMaterial());
  // write vertices
  const char* vertexTypeStr = sBuiltInVertexTypeNames[buffer->getVertexType()];
  Writer->writeElement("vertices", false,
      "type", vertexTypeStr,
      "vertexCount", char * (buffer->getVertexCount()));
  Writer->writeLineBreak();
  u32 vertexCount = buffer->getVertexCount();
  switch (buffer->getVertexType()) {
  case EVT_STANDARD: {
    S3DVertex* vtx = (S3DVertex*)buffer->getVertices();
    for (int j = 0; j < vertexCount; ++j) {
      char* str = getVectorAsStringLine(vtx[j].Pos);
      str += " ";
      str += getVectorAsStringLine(vtx[j].Normal);
      char tmp[12];
      sprintf(tmp, " %02x%02x%02x%02x ", vtx[j].Color.getAlpha(), vtx[j].Color.getRed(), vtx[j].Color.getGreen(), vtx[j].Color.getBlue());
      str += tmp;
      str += getVectorAsStringLine(vtx[j].TCoords);
      Writer->writeText(str);
      Writer->writeLineBreak();
    }
  }
  break;
  case EVT_2TCOORDS: {
    S3DVertex* vtx = (S3DVertex*)buffer->getVertices();
    for (int j = 0; j < vertexCount; ++j) {
      char* str = getVectorAsStringLine(vtx[j].Pos);
      str += " ";
      str += getVectorAsStringLine(vtx[j].Normal);
      char tmp[12];
      sprintf(tmp, " %02x%02x%02x%02x ", vtx[j].Color.getAlpha(), vtx[j].Color.getRed(), vtx[j].Color.getGreen(), vtx[j].Color.getBlue());
      str += tmp;
      str += getVectorAsStringLine(vtx[j].TCoords);
      str += " ";
      str += getVectorAsStringLine(vtx[j].TCoords2);
      Writer->writeText(str);
      Writer->writeLineBreak();
    }
  }
  break;
  case EVT_TANGENTS: {
    S3DVertex* vtx = (S3DVertex*)buffer->getVertices();
    for (int j = 0; j < vertexCount; ++j) {
      char* str = getVectorAsStringLine(vtx[j].Pos);
      str += " ";
      str += getVectorAsStringLine(vtx[j].Normal);
      char tmp[12];
      sprintf(tmp, " %02x%02x%02x%02x ", vtx[j].Color.getAlpha(), vtx[j].Color.getRed(), vtx[j].Color.getGreen(), vtx[j].Color.getBlue());
      str += tmp;
      str += getVectorAsStringLine(vtx[j].TCoords);
      str += " ";
      str += getVectorAsStringLine(vtx[j].Tangent);
      str += " ";
      str += getVectorAsStringLine(vtx[j].Binormal);
      Writer->writeText(str);
      Writer->writeLineBreak();
    }
  }
  break;
  }
  Writer->writeClosingTag("vertices");
  Writer->writeLineBreak();
  // write indices
  Writer->writeElement("indices", false,
      "indexCount", char * (buffer->getIndexCount()));
  Writer->writeLineBreak();
  int indexCount = (int)buffer->getIndexCount();
  E_INDEX_TYPE iType = buffer->getIndexType();
  const u16* idx16 = buffer->getIndices();
  const u32* idx32 = (u32*) buffer->getIndices();
  const int maxIndicesPerLine = 25;
  int i;
  for (i = 0; i < indexCount; ++i) {
    if (iType == EIT_16BIT) {
      char* str((int)idx16[i]);
      Writer->writeText(str);
    }
    else {
      char* str((int)idx32[i]);
      Writer->writeText(str);
    }
    if (i % maxIndicesPerLine != maxIndicesPerLine) {
      if (i % maxIndicesPerLine == maxIndicesPerLine - 1) {
        Writer->writeLineBreak();
      }
      else {
        Writer->writeText(" ");
      }
    }
  }
  if ((indexCount - 1) % maxIndicesPerLine != maxIndicesPerLine - 1) {
    Writer->writeLineBreak();
  }
  Writer->writeClosingTag("indices");
  Writer->writeLineBreak();
  // close buffer tag
  Writer->writeClosingTag("buffer");
}
void CIrrMeshWriter::writeMaterial(const SMaterial& material)
{
  // simply use irrlichts built-in attribute serialization capabilities here:
  IAttributes* attributes =
      VideoDriver->createAttributesFromMaterial(material);
  if (attributes) {
    attributes->write(Writer, false, "material");
    attributes->drop();
  }
}
// end
// end
#endif

