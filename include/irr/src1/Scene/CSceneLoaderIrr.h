// Copyright (C) 2010-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
#ifndef __C_SCENE_LOADER_IRR_H_INCLUDED__
#define __C_SCENE_LOADER_IRR_H_INCLUDED__
//#include "ISceneLoader.h"
//#include "IXMLReader.h"
class IFileSystem;
}
class ISceneManager;
//! Class which can load a scene into the scene manager.
class CSceneLoaderIrr : public ISceneLoader
{
public:
  //! Constructor
  CSceneLoaderIrr(ISceneManager* smgr, IFileSystem* fs);
  //! Destructor
  ~CSceneLoaderIrr();
  //! Returns true if the class might be able to load this file.
  bool isALoadableFileExtension(const char* filename) const;
  //! Returns true if the class might be able to load this file.
  bool isALoadableFileFormat(IReadFile* file) const;
  //! Loads the scene into the scene manager.
  bool loadScene(IReadFile* file, ISceneUserDataSerializer* userDataSerializer = 0,
      ISceneNode* rootNode = 0);
private:
  //! Recursively reads nodes from the xml file
  void readSceneNode(IXMLReader* reader, ISceneNode* parent,
      ISceneUserDataSerializer* userDataSerializer);
  //! read a node's materials
  void readMaterials(IXMLReader* reader, ISceneNode* node);
  //! read a node's animators
  void readAnimators(IXMLReader* reader, ISceneNode* node);
  //! read any other data into the user serializer
  void readUserData(IXMLReader* reader, ISceneNode* node,
      ISceneUserDataSerializer* userDataSerializer);
  ISceneManager*   SceneManager;
  IFileSystem* FileSystem;
  //! constants for reading and writing XML.
  //! Not made static due to portability problems.
  // TODO: move to own header
  const char* IRR_XML_FORMAT_SCENE;
  const char* IRR_XML_FORMAT_NODE;
  const char* IRR_XML_FORMAT_NODE_ATTR_TYPE;
  const char* IRR_XML_FORMAT_ATTRIBUTES;
  const char* IRR_XML_FORMAT_MATERIALS;
  const char* IRR_XML_FORMAT_ANIMATORS;
  const char* IRR_XML_FORMAT_USERDATA;
};
#endif

