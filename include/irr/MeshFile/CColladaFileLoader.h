// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
#ifndef __C_COLLADA_MESH_FILE_LOADER_H_INCLUDED__
#define __C_COLLADA_MESH_FILE_LOADER_H_INCLUDED__
//#include "IMeshLoader.h"
//#include "IFileSystem.h"
//#include "IVideoDriver.h"
//#include "irrString.h"
//#include "SMesh.h"
//#include "SMeshBuffer.h"
//#include "ISceneManager.h"
//#include "irrMap.h"
//#include "CAttributes.h"
#ifdef _DEBUG
//#define COLLADA_READER_DEBUG
#endif
class IColladaPrefab;
enum ECOLLADA_PARAM_NAME {
  ECPN_COLOR = 0,
  ECPN_AMBIENT,
  ECPN_DIFFUSE,
  ECPN_SPECULAR,
  ECPN_SHININESS,
  ECPN_TRANSPARENCY,
  ECPN_YFOV,
  ECPN_ZNEAR,
  ECPN_ZFAR,
  ECPN_COUNT
};
enum ECOLLADA_PARAM_TYPE {
  ECPT_FLOAT = 0,
  ECPT_FLOAT2,
  ECPT_FLOAT3,
  ECPT_FLOAT4,
  ECPT_COUNT
};
//! Collada Parameter
struct SColladaParam {
  SColladaParam()
    : Name(ECPN_COUNT), Type(ECPT_COUNT) {
    int i;
    for (i = 0; i < 4; ++i) {
      Floats[i] = 0;
    }
  }
  ECOLLADA_PARAM_NAME Name;
  ECOLLADA_PARAM_TYPE Type;
  f32 Floats[4];
};
enum ECOLLADA_INPUT_SEMANTIC {
  ECIS_POSITION = 0,
  ECIS_VERTEX,
  ECIS_NORMAL,
  ECIS_TEXCOORD,
  ECIS_UV,
  ECIS_TANGENT,
  ECIS_IMAGE,
  ECIS_TEXTURE,
  ECIS_COUNT
};
//! Collada Input
struct SColladaInput {
  SColladaInput()
    : Semantic(ECIS_COUNT), Data(0), Offset(0), Set(0), Stride(1) {
  }
  ECOLLADA_INPUT_SEMANTIC Semantic;
  char* Source;
  f32* Data;
  u32 Offset;
  u32 Set;
  u32 Stride;
};
//! Collada images
struct SColladaImage {
  char* Id;
  char* Source;
  dimension2du Dimension;
  bool SourceIsFilename;
};
//! Collada texture
struct SColladaTexture {
  ITexture* Texture;
  char* Id;
};
//! Collada material
struct SColladaMaterial {
  SMaterial Mat;
  char* Id;
  char* InstanceEffectId;
  f32 Transparency;
  CV_INLINE bool operator< (const SColladaMaterial& other) const {
    return Id < other.Id;
  }
};
//! Collada effect (materials, shaders, and programs)
struct SColladaEffect {
  char* Id;
  f32 Transparency;
  strv_t Textures;
  SMaterial Mat;
  // TODO: Parameters looks somewhat lazy workaround, I think we should really read all parameters correct.
  IAttributes* Parameters;
  CV_INLINE bool operator< (const SColladaEffect& other) const {
    return Id < other.Id;
  }
};
struct SNumberArray { // for storing float and int arrays
  char* Name;
  f32 > Data;
};
struct SAccessor {
  SAccessor()
    : Count(0), Offset(0), Stride(1) {}
  // I don't store the source of the accessor here because I assume
  // it to use the array of the source this accessor is located in.
  int Count;
  int Offset;
  int Stride;
  SColladaParam > Parameters; // parameters defining the accessor
};
struct SSource {
  char* Id;
  SNumberArray Array;
  SAccessor > Accessors;
};
class CScenePrefab;
//! Meshloader capable of loading COLLADA meshes and scene descriptions into Irrlicht.
class CColladaFileLoader : public IMeshLoader
{
public:
  //! Constructor
  CColladaFileLoader(ISceneManager* smgr, IFileSystem* fs);
  //! destructor
  ~CColladaFileLoader();
  //! returns true if the file maybe is able to be loaded by this class
  //! based on the file extension (e.g. ".cob")
  bool isALoadableFileExtension(const char* filename) const;
  //! creates/loads an animated mesh from the file.
  //! \return Pointer to the created mesh. Returns 0 if loading failed.
  //! If you no longer need the mesh, you should call IAnimatedMesh::drop().
  //! See IReferenceCounted::drop() for more information.
  IAnimatedMesh* createMesh(IReadFile* file);
private:
  //! skips an (unknown) section in the collada document
  void skipSection(IXMLReaderUTF8* reader, bool reportSkipping);
  //! reads the <COLLADA> section and its content
  void readColladaSection(IXMLReaderUTF8* reader);
  //! reads a <library> section and its content
  void readLibrarySection(IXMLReaderUTF8* reader);
  //! reads a <visual_scene> element and stores it as a prefab
  void readVisualScene(IXMLReaderUTF8* reader);
  //! reads a <scene> section and its content
  void readSceneSection(IXMLReaderUTF8* reader);
  //! reads a <asset> section and its content
  void readAssetSection(IXMLReaderUTF8* reader);
  //! reads a <node> section and its content
  //! if a prefab pointer is passed the nodes are created as scene prefabs children of that prefab
  void readNodeSection(IXMLReaderUTF8* reader, ISceneNode* parent, CScenePrefab* p = 0);
  //! reads a <lookat> element and its content and creates a matrix from it
  matrix4 readLookAtNode(IXMLReaderUTF8* reader);
  //! reads a <matrix> element and its content and creates a matrix from it
  matrix4 readMatrixNode(IXMLReaderUTF8* reader);
  //! reads a <perspective> element and its content and creates a matrix from it
  matrix4 readPerspectiveNode(IXMLReaderUTF8* reader);
  //! reads a <rotate> element and its content and creates a matrix from it
  matrix4 readRotateNode(IXMLReaderUTF8* reader);
  //! reads a <skew> element and its content and creates a matrix from it
  matrix4 readSkewNode(IXMLReaderUTF8* reader);
  //! reads a <boundingbox> element and its content and stores it in bbox
  void readBboxNode(IXMLReaderUTF8* reader, aabbox3df& bbox);
  //! reads a <scale> element and its content and creates a matrix from it
  matrix4 readScaleNode(IXMLReaderUTF8* reader);
  //! reads a <translate> element and its content and creates a matrix from it
  matrix4 readTranslateNode(IXMLReaderUTF8* reader);
  //! reads a <color> element
  SColorf readColorNode(IXMLReaderUTF8* reader);
  //! reads a <float> element
  f32 readFloatNode(IXMLReaderUTF8* reader);
  //! reads a <instance> node
  void readInstanceNode(IXMLReaderUTF8* reader,
      ISceneNode* parent, ISceneNode** outNode,
      CScenePrefab* p = 0, const char* type = char * ());
  //! creates a scene node from Prefabs (with name given in 'url')
  void instantiateNode(ISceneNode* parent, ISceneNode** outNode = 0,
      CScenePrefab* p = 0, const char* url = "",
      const char* type = char * ());
  //! reads a <light> element and stores it as prefab
  void readLightPrefab(IXMLReaderUTF8* reader);
  //! reads a <camera> element and stores it as prefab
  void readCameraPrefab(IXMLReaderUTF8* reader);
  //! reads a <image> element and stores it in the image section
  void readImage(IXMLReaderUTF8* reader);
  //! reads a <texture> element and stores it in the texture section
  void readTexture(IXMLReaderUTF8* reader);
  //! reads a <material> element and stores it in the material section
  void readMaterial(IXMLReaderUTF8* reader);
  //! reads a <effect> element and stores it in the effects section
  void readEffect(IXMLReaderUTF8* reader, SColladaEffect* effect = 0);
  //! reads a <geometry> element and stores it as mesh if possible
  void readGeometry(IXMLReaderUTF8* reader);
  //! parses a float from a char pointer and moves the pointer to
  //! the end of the parsed float
  CV_INLINE f32 readFloat(const c8** p);
  //! parses an int from a char pointer and moves the pointer to
  //! the end of the parsed float
  CV_INLINE s32 readInt(const c8** p);
  //! places pointer to next begin of a token
  void findNextNoneWhiteSpace(const c8** p);
  //! reads floats from inside of xml element until end of xml element
  void readFloatsInsideElement(IXMLReaderUTF8* reader, f32* floats, u32 count);
  //! reads ints from inside of xml element until end of xml element
  void readIntsInsideElement(IXMLReaderUTF8* reader, s32* ints, u32 count);
  //! clears all loaded data
  void clearData();
  //! parses all collada parameters inside an element and stores them in ColladaParameters
  void readColladaParameters(IXMLReaderUTF8* reader, const char* parentName);
  //! returns a collada parameter or none if not found
  SColladaParam* getColladaParameter(ECOLLADA_PARAM_NAME name);
  //! parses all collada inputs inside an element and stores them in Inputs. Reads
  //! until first tag which is not an input tag or the end of the parent is reached
  void readColladaInputs(IXMLReaderUTF8* reader, const char* parentName);
  //! reads a collada input tag and adds it to the input parameter
  void readColladaInput(IXMLReaderUTF8* reader, SColladaInput > & inputs);
  //! returns a collada input or none if not found
  SColladaInput* getColladaInput(ECOLLADA_INPUT_SEMANTIC input);
  //! read Collada Id, uses id or name if id is missing
  char* readId(IXMLReaderUTF8* reader);
  //! changes the XML URI into an internal id
  void uriToId(char* str);
  //! reads a polygons section and creates a mesh from it
  void readPolygonSection(IXMLReaderUTF8* reader,
      SSource > & sources, SMesh* mesh,
      const char* geometryId);
  //! finds a material, possible instancing it
  const SColladaMaterial* findMaterial(const char* materialName);
  //! reads and bind materials as given by the symbol->target bind mapping
  void readBindMaterialSection(IXMLReaderUTF8* reader, const char* id);
  //! create an Irrlicht texture from the SColladaImage
  ITexture* getTextureFromImage(char* uri, SColladaEffect* effect);
  //! read a parameter and value
  void readParameter(IXMLReaderUTF8* reader, IAttributes* parameters);
  ISceneManager* SceneManager;
  IFileSystem* FileSystem;
  IAnimatedMesh* DummyMesh;
  char* CurrentlyLoadingMesh;
  IAnimatedMesh* FirstLoadedMesh;
  char* FirstLoadedMeshName;
  s32 LoadedMeshCount;
  u32 Version;
  bool FlipAxis;
  IColladaPrefab* > Prefabs;
  SColladaParam > ColladaParameters;
  SColladaImage > Images;
  SColladaTexture > Textures;
  SColladaMaterial > Materials;
  SColladaInput > Inputs;
  SColladaEffect > Effects;
  //! meshbuffer reference ("geomid/matname") -> index into MeshesToBind
  map<char*, u32> MaterialsToBind;
  //! Array of buffers for each material binding
  IMeshBuffer* > > MeshesToBind;
  bool CreateInstances;
};
//! following class is for holding and createing instances of library objects,
//! named prefabs in this loader.
class IColladaPrefab
{
public:
  //! creates an instance of this prefab
  ISceneNode* addInstance(ISceneNode* parent,
      ISceneManager* mgr) = 0;
  //! returns id of this prefab
  const char* getId() = 0;
};
#endif

