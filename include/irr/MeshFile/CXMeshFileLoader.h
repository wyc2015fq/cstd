// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
#ifndef __C_X_MESH_FILE_LOADER_H_INCLUDED__
#define __C_X_MESH_FILE_LOADER_H_INCLUDED__
//#include "IMeshLoader.h"
//#include "irrString.h"
//#include "CSkinnedMesh.h"
class IFileSystem;
class IReadFile;
class IMeshManipulator;
//! Meshloader capable of loading x meshes.
class CXMeshFileLoader : public IMeshLoader
{
public:
  //! Constructor
  CXMeshFileLoader(ISceneManager* smgr, IFileSystem* fs);
  //! returns true if the file maybe is able to be loaded by this class
  //! based on the file extension (e.g. ".cob")
  bool isALoadableFileExtension(const char* filename) const;
  //! creates/loads an animated mesh from the file.
  //! \return Pointer to the created mesh. Returns 0 if loading failed.
  //! If you no longer need the mesh, you should call IAnimatedMesh::drop().
  //! See IReferenceCounted::drop() for more information.
  IAnimatedMesh* createMesh(IReadFile* file);
  struct SXTemplateMaterial {
    char* Name; // template name from Xfile
    SMaterial Material; // material
  };
  struct SXMesh {
    SXMesh() : MaxSkinWeightsPerVertex(0), MaxSkinWeightsPerFace(0), BoneCount(0), AttachedJointID(-1), HasSkinning(false), HasVertexColors(false) {}
    // this mesh contains triangulated texture data.
    // because in an .x file, faces can be made of more than 3
    // vertices, the indices data structure is triangulated during the
    // loading process. The IndexCountPerFace array is filled during
    // this triangulation process and stores how much indices belong to
    // every face. This data structure can be ignored, because all data
    // in this structure is triangulated.
    char* Name;
    u32 MaxSkinWeightsPerVertex;
    u32 MaxSkinWeightsPerFace;
    u32 BoneCount;
    u16* IndexCountPerFace; // default 3, but could be more
    SSkinMeshBuffer* > Buffers;
    S3DVertex* Vertices;
    vector2df > TCoords2;
    u32* Indices;
    u32* FaceMaterialIndices; // index of material for each face
    SMaterial* Materials; // material array
    u32* WeightJoint;
    u32* WeightNum;
    s32 AttachedJointID;
    bool HasSkinning;
    bool HasVertexColors;
  };
private:
  bool load(IReadFile* file);
  bool readFileIntoMemory(IReadFile* file);
  bool parseFile();
  bool parseDataObject();
  bool parseDataObjectTemplate();
  bool parseDataObjectFrame(CSkinnedMesh::SJoint* parent);
  bool parseDataObjectTransformationMatrix(matrix4& mat);
  bool parseDataObjectMesh(SXMesh& mesh);
  bool parseDataObjectSkinWeights(SXMesh& mesh);
  bool parseDataObjectSkinMeshHeader(SXMesh& mesh);
  bool parseDataObjectMeshNormals(SXMesh& mesh);
  bool parseDataObjectMeshTextureCoords(SXMesh& mesh);
  bool parseDataObjectMeshVertexColors(SXMesh& mesh);
  bool parseDataObjectMeshMaterialList(SXMesh& mesh);
  bool parseDataObjectMaterial(SMaterial& material);
  bool parseDataObjectAnimationSet();
  bool parseDataObjectAnimation();
  bool parseDataObjectAnimationKey(ISkinnedMesh::SJoint* joint);
  bool parseDataObjectTextureFilename(char* texturename);
  bool parseUnknownDataObject();
  //! places pointer to next begin of a token, and ignores comments
  void findNextNoneWhiteSpace();
  //! places pointer to next begin of a token, which must be a number,
  // and ignores comments
  void findNextNoneWhiteSpaceNumber();
  //! returns next parseable token. Returns empty string if no token there
  char* getNextToken();
  //! reads header of dataobject including the opening brace.
  //! returns false if error happened, and writes name of object
  //! if there is one
  bool readHeadOfDataObject(char** outname = 0);
  //! checks for closing curly brace, returns false if not there
  bool checkForClosingBrace();
  //! checks for one following semicolons, returns false if not there
  bool checkForOneFollowingSemicolons();
  //! checks for two following semicolons, returns false if they are not there
  bool checkForTwoFollowingSemicolons();
  //! reads a x file style string
  bool getNextTokenAsString(char* out);
  void readUntilEndOfLine();
  u16 readBinWord();
  u32 readBinDWord();
  u32 readInt();
  f32 readFloat();
  bool readVector2(vector2df& vec);
  bool readVector3(vector3df& vec);
  bool readMatrix(matrix4& mat);
  bool readRGB(SColor& color);
  bool readRGBA(SColor& color);
  ISceneManager* SceneManager;
  IFileSystem* FileSystem;
  CSkinnedMesh::SJoint* > * AllJoints;
  CSkinnedMesh* AnimatedMesh;
  c8* Buffer;
  const c8* P;
  c8* End;
  // counter for number arrays in binary format
  u32 BinaryNumCount;
  u32 Line;
  char* FilePath;
  CSkinnedMesh::SJoint* CurFrame;
  SXMesh* > Meshes;
  SXTemplateMaterial > TemplateMaterials;
  u32 MajorVersion;
  u32 MinorVersion;
  bool BinaryFormat;
  c8 FloatSize;
};
#endif

