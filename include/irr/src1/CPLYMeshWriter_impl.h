// Copyright (C) 2008-2012 Christian Stehno
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
#ifdef _IRR_COMPILE_WITH_PLY_WRITER_
//#include "CPLYMeshWriter.h"
//#include "os.h"
//#include "IWriteFile.h"
CPLYMeshWriter::CPLYMeshWriter()
{
#ifdef _DEBUG
  setDebugName("CPLYMeshWriter");
#endif
}
//! Returns the type of the mesh writer
EMESH_WRITER_TYPE CPLYMeshWriter::getType() const
{
  return EMWT_PLY;
}
//! writes a mesh
bool CPLYMeshWriter::writeMesh(IWriteFile* file, IMesh* mesh, s32 flags)
{
  if (!file || !mesh) {
    return false;
  }
  os::Printer::log("Writing mesh", file->getFileName());
  // write PLY header
  char* header =
      "ply\n"
      "format ascii 1.0\n"
      "comment Irrlicht Engine ";
  header +=  IRRLICHT_SDK_VERSION;
  // get vertex and triangle counts
  u32 VertexCount   = 0;
  u32 TriangleCount = 0;
  int i;
  for (i = 0; i < mesh->getMeshBufferCount(); ++i) {
    VertexCount   += mesh->getMeshBuffer(i)->getVertexCount();
    TriangleCount += mesh->getMeshBuffer(i)->getIndexCount() / 3;
  }
  // vertex definition
  header += "\nelement vertex ";
  header += VertexCount;
  header += "\n"
      "property float x\n"
      "property float y\n"
      "property float z\n"
      "property float nx\n"
      "property float ny\n"
      "property float nz\n";
  // todo: writer flags for extended (r,g,b,u,v) and non-standard (alpha,u1,uv,tx,ty,tz) properties
  //  "property uchar red\n"
  //  "property uchar green\n"
  //  "property uchar blue\n"
  //  "property uchar alpha\n"
  //  "property float u\n"
  //  "property float v\n";
  //  "property float u1\n
  //  "property float v1\n"
  //  "property float tx\n"
  //  "property float ty\n"
  //  "property float tz\n"
  // face definition
  header += "element face ";
  header += TriangleCount;
  header += "\n"
      "property list uchar int vertex_indices\n"
      "end_header\n";
  // write header
  file->write(header, header_size);
  // write vertices
  c8 outLine[1024];
  for (i = 0; i < mesh->getMeshBufferCount(); ++i) {
    IMeshBuffer* mb = mesh->getMeshBuffer(i);
    for (int j = 0; j < mb->getVertexCount(); ++j) {
      const float3 pos = mb->getPosition(j);
      const float3 n   = mb->getNormal(j);
      //      const vector2df& tc  = mb->getTCoords(j);
      u8* buf  = (u8*)mb->getVertices();
      switch (mb->getVertexType()) {
      case EVT_STANDARD:
        buf += sizeof(S3DVertex) * j;
        break;
      case EVT_2TCOORDS:
        buf += sizeof(S3DVertex) * j;
        break;
      case EVT_TANGENTS:
        buf += sizeof(S3DVertex) * j;
        break;
      }
      //      SColor &col = ( (S3DVertex*)buf )->Color;
      // x y z nx ny nz red green blue alpha u v [u1 v1 | tx ty tz]\n
      snprintf(outLine, 1024,
          "%f %f %f %f %f %f\n",// %u %u %u %u %f %f\n",
          pos.X, pos.Z, pos.Y, // Y and Z are flipped
          n.X, n.Z, n.Y);
      /*col.getRed(), col.getGreen(), col.getBlue(), col.getAlpha(),
      tc.X, tc.Y);*/
      // write the line
      file->write(outLine, cstr_len(outLine));
    }
  }
  // index of the first vertex in the current mesh buffer
  u32 StartOffset = 0;
  // write triangles
  for (i = 0; i < mesh->getMeshBufferCount(); ++i) {
    IMeshBuffer* mb = mesh->getMeshBuffer(i);
    for (int j = 0; j < mb->getIndexCount(); j += 3) {
      // y and z are flipped so triangles are reversed
      u32 a = StartOffset,
          b = StartOffset,
          c = StartOffset;
      switch (mb->getIndexType()) {
      case EIT_16BIT:
        a += mb->getIndices()[j + 0];
        c += mb->getIndices()[j + 1];
        b += mb->getIndices()[j + 2];
        break;
      case EIT_32BIT:
        a += ((u32*)mb->getIndices()) [j + 0];
        c += ((u32*)mb->getIndices()) [j + 0];
        b += ((u32*)mb->getIndices()) [j + 0];
        break;
      }
      // count a b c\n
      snprintf(outLine, 1024, "3 %u %u %u\n", a, b, c);
      // write the line
      file->write(outLine, cstr_len(outLine));
    }
    // increment offset
    StartOffset += mb->getVertexCount();
  }
  // all done!
  return true;
}
// end
// end
#endif // _IRR_COMPILE_WITH_PLY_WRITER_

