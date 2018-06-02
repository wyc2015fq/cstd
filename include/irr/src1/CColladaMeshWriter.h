// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
#ifndef __IRR_C_COLLADA_MESH_WRITER_H_INCLUDED__
#define __IRR_C_COLLADA_MESH_WRITER_H_INCLUDED__
//#include "IColladaMeshWriter.h"
//#include "irrMap.h"
//#include "IVideoDriver.h"
class IXMLWriter;
class IFileSystem;
}
//! Callback interface for properties which can be used to influence collada writing
// (Implementer note: keep namespace labels here to make it easier for users copying this one)
class CColladaMeshWriterProperties  : public IColladaMeshWriterProperties
{
public:
  //! Which lighting model should be used in the technique (FX) section when exporting effects (materials)
  E_COLLADA_TECHNIQUE_FX getTechniqueFx(const SMaterial& material) const;
  //! Which texture index should be used when writing the texture of the given sampler color.
  s32 getTextureIdx(const SMaterial& material, E_COLLADA_COLOR_SAMPLER cs) const;
  //! Return which color from Irrlicht should be used for the color requested by collada
  E_COLLADA_IRR_COLOR getColorMapping(const SMaterial& material, E_COLLADA_COLOR_SAMPLER cs) const;
  //! Return custom colors for certain color types requested by collada.
  SColor getCustomColor(const SMaterial& material, E_COLLADA_COLOR_SAMPLER cs) const;
  //! Return the settings for transparence
  E_COLLADA_TRANSPARENT_FX getTransparentFx(const SMaterial& material) const;
  //! Transparency value for that material.
  f32 getTransparency(const SMaterial& material) const;
  //! Reflectivity value for that material
  f32 getReflectivity(const SMaterial& material) const;
  //! Return index of refraction for that material
  f32 getIndexOfRefraction(const SMaterial& material) const;
  //! Should node be used in scene export? By default all visible nodes are exported.
  bool isExportable(const ISceneNode* node) const;
  //! Return the mesh for the given nod. If it has no mesh or shouldn't export it's mesh return 0.
  IMesh* getMesh(ISceneNode* node);
  //! Return if the node has it's own material overwriting the mesh-materials
  bool useNodeMaterial(const ISceneNode* node) const;
};
class CColladaMeshWriterNames  : public IColladaMeshWriterNames
{
public:
  CColladaMeshWriterNames(IColladaMeshWriter* writer);
  char* nameForMesh(const IMesh* mesh, int instance);
  char* nameForNode(const ISceneNode* node);
  char* nameForMaterial(const SMaterial& material, int materialId, const IMesh* mesh, const ISceneNode* node);
protected:
  char* nameForPtr(const void* ptr) const;
private:
  IColladaMeshWriter* ColladaMeshWriter;
};
//! class to write meshes, implementing a COLLADA (.dae, .xml) writer
/** This writer implementation has been originally developed for irrEdit and then
merged out to the Irrlicht Engine */
class CColladaMeshWriter : public IColladaMeshWriter
{
public:
  CColladaMeshWriter(ISceneManager* smgr, IVideoDriver* driver, IFileSystem* fs);
  ~CColladaMeshWriter();
  //! Returns the type of the mesh writer
  EMESH_WRITER_TYPE getType() const;
  //! writes a scene starting with the given node
  bool writeScene(IWriteFile* file, ISceneNode* root);
  //! writes a mesh
  bool writeMesh(IWriteFile* file, IMesh* mesh, s32 flags = EMWF_NONE);
  // Restrict the characters of oldString a set of allowed characters in xs::NCName and add the prefix.
  char* toNCName(const char* oldString, const char* prefix = char * ("_NC_")) const;
protected:
  void reset();
  bool hasSecondTextureCoordinates(E_VERTEX_TYPE type) const;
  void writeUv(const vector2df& vec);
  void writeVector(const vector2df& vec);
  void writeVector(const float3 vec);
  void writeColor(const SColorf& colorf, bool writeAlpha = true);
  CV_INLINE char* toString(const ECOLOR_FORMAT format) const;
  CV_INLINE char* toString(const E_TEXTURE_CLAMP clamp) const;
  CV_INLINE char* toString(const E_COLLADA_TRANSPARENT_FX opaque) const;
  CV_INLINE char* toRef(const char* source) const;
  bool isCamera(const ISceneNode* node) const;
  char* nameForMesh(const IMesh* mesh, int instance) const;
  char* nameForNode(const ISceneNode* node) const;
  char* nameForMaterial(const SMaterial& material, int materialId, const IMesh* mesh, const ISceneNode* node);
  char* nameForMaterialSymbol(const IMesh* mesh, int materialId) const;
  char* findCachedMaterialName(const SMaterial& material) const;
  char* minTexfilterToString(bool bilinear, bool trilinear) const;
  char* magTexfilterToString(bool bilinear, bool trilinear) const;
  char* pathToURI(const char* path) const;
  CV_INLINE bool isXmlNameStartChar(char c) const;
  CV_INLINE bool isXmlNameChar(char c) const;
  s32 getCheckedTextureIdx(const SMaterial& material, E_COLLADA_COLOR_SAMPLER cs);
  SColor getColorMapping(const SMaterial& material, E_COLLADA_COLOR_SAMPLER cs, E_COLLADA_IRR_COLOR colType);
  void writeAsset();
  void makeMeshNames(ISceneNode* node);
  void writeNodeMaterials(ISceneNode* node);
  void writeNodeEffects(ISceneNode* node);
  void writeNodeLights(ISceneNode* node);
  void writeNodeCameras(ISceneNode* node);
  void writeAllMeshGeometries();
  void writeSceneNode(ISceneNode* node);
  void writeMeshMaterials(IMesh* mesh, char* > * materialNamesOut = 0);
  void writeMeshEffects(IMesh* mesh);
  void writeMaterialEffect(const char* materialname, const SMaterial& material);
  void writeMeshGeometry(const char* meshname, IMesh* mesh);
  void writeMeshInstanceGeometry(const char* meshname, IMesh* mesh, ISceneNode* node = 0);
  void writeMaterial(const char* materialname);
  void writeLightInstance(const char* lightName);
  void writeCameraInstance(const char* cameraName);
  void writeLibraryImages();
  void writeColorFx(const SMaterial& material, const char* colorname, E_COLLADA_COLOR_SAMPLER cs, const char* attr1Name = 0, const char* attr1Value = 0);
  void writeAmbientLightElement(const SColorf& col);
  void writeColorElement(const SColor& col, bool writeAlpha = true);
  void writeColorElement(const SColorf& col, bool writeAlpha = true);
  void writeTextureSampler(s32 textureIdx);
  void writeFxElement(const SMaterial& material, E_COLLADA_TECHNIQUE_FX techFx);
  void writeNode(const char* nodeName, const char* content);
  void writeFloatElement(f32 value);
  void writeRotateElement(const float3 axis, f32 angle);
  void writeScaleElement(const float3 scale);
  void writeTranslateElement(const float3 translate);
  void writeMatrixElement(const matrix4& matrix);
  struct SComponentGlobalStartPos {
    SComponentGlobalStartPos() : PosStartIndex(-1), PosLastIndex(-1),
      NormalStartIndex(-1), NormalLastIndex(-1),
      TCoord0StartIndex(-1), TCoord0LastIndex(-1),
      TCoord1StartIndex(-1), TCoord1LastIndex(-1)
    { }
    s32 PosStartIndex;
    s32 PosLastIndex;
    s32 NormalStartIndex;
    s32 NormalLastIndex;
    s32 TCoord0StartIndex;
    s32 TCoord0LastIndex;
    s32 TCoord1StartIndex;
    s32 TCoord1LastIndex;
  };
  IFileSystem* FileSystem;
  IVideoDriver* VideoDriver;
  IXMLWriter* Writer;
  ITexture* > LibraryImages;
  char* Directory;
  // Helper struct for creating geometry copies for the ECGI_PER_MESH_AND_MATERIAL settings.
  struct SGeometryMeshMaterials {
    bool equals(const char* > & names) const {
      if (names_size != MaterialNames_size) {
        return false;
      }
      for (u32 i = 0; i < MaterialNames_size; ++i)
        if (names[i] != MaterialNames[i]) {
          return false;
        }
      return true;
    }
    char* GeometryName;        // replacing the usual ColladaMesh::Name
    char* > MaterialNames; // Material names exported for this instance
    const ISceneNode* > MaterialOwners; // Nodes using this specific mesh-material combination
  };
  // Check per mesh-ptr if stuff has been written for this mesh already
  struct SColladaMesh {
    SColladaMesh() : MaterialsWritten(false), EffectsWritten(false) {
    }
    SGeometryMeshMaterials* findGeometryMeshMaterials(const char* > materialNames) {
      for (u32 i = 0; i < GeometryMeshMaterials_size; ++i) {
        if (GeometryMeshMaterials[i].equals(materialNames)) {
          return &(GeometryMeshMaterials[i]);
        }
      }
      return NULL;
    }
    const char* findGeometryNameForNode(const ISceneNode* node) const {
      if (GeometryMeshMaterials_size < 2) {
        return Name;
      }
      for (u32 i = 0; i < GeometryMeshMaterials_size; ++i) {
        if (GeometryMeshMaterials[i].MaterialOwners.linear_search(node)  >= 0) {
          return GeometryMeshMaterials[i].GeometryName;
        }
      }
      return Name; // (shouldn't get here usually)
    }
    char* Name;
    bool MaterialsWritten;  // just an optimization doing that here in addition to the MaterialsWritten map
    bool EffectsWritten;  // just an optimization doing that here in addition to the EffectsWritten map
    SGeometryMeshMaterials > GeometryMeshMaterials;
  };
  typedef map<IMesh*, SColladaMesh>::Node MeshNode;
  map<IMesh*, SColladaMesh> Meshes;
  // structure for the lights library
  struct SColladaLight {
    SColladaLight() {}
    char* Name;
  };
  typedef map<ISceneNode*, SColladaLight>::Node LightNode;
  map<ISceneNode*, SColladaLight> LightNodes;
  // structure for the camera library
  typedef map<ISceneNode*, char*>::Node CameraNode;
  map<ISceneNode*, char*> CameraNodes;
  // Check per name if stuff has been written already
  // TODO: second parameter not needed, we just don't have a set class yet in Irrlicht
  map<char*, bool> MaterialsWritten;
  map<char*, bool> EffectsWritten;
  // Cache material names
  struct MaterialName {
    MaterialName(const SMaterial& material, const char* name)
      : Material(material), Name(name)
    {}
    SMaterial Material;
    char* Name;
  };
  MaterialName > MaterialNameCache;
};
// end
// end
#endif

