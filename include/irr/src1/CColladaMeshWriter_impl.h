// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
// TODO: second UV-coordinates currently ignored in textures
#ifdef _IRR_COMPILE_WITH_COLLADA_WRITER_
//#include "CColladaMeshWriter.h"
//#include "os.h"
//#include "IFileSystem.h"
//#include "IWriteFile.h"
//#include "IXMLWriter.h"
//#include "IAttributes.h"
//#include "IAnimatedMeshSceneNode.h"
//#include "IMeshSceneNode.h"
//#include "ITerrainSceneNode.h"
//#include "ILightSceneNode.h"
//#include "ICameraSceneNode.h"
//#include "ISceneManager.h"
//! Which lighting model should be used in the technique (FX) section when exporting effects (materials)
E_COLLADA_TECHNIQUE_FX CColladaMeshWriterProperties::getTechniqueFx(const SMaterial& material) const
{
  return ECTF_BLINN;
}
//! Which texture index should be used when writing the texture of the given sampler color.
s32 CColladaMeshWriterProperties::getTextureIdx(const SMaterial& material, E_COLLADA_COLOR_SAMPLER cs) const
{
  // So far we just export in a way which is similar to how we import colladas.
  // There might be better ways to do this, but I suppose it depends a lot for which target
  // application we export, so in most cases it will have to be done in user-code anyway.
  switch (cs) {
  case ECCS_DIFFUSE:
    return 2;
  case ECCS_AMBIENT:
    return 1;
  case ECCS_EMISSIVE:
    return 0;
  case ECCS_SPECULAR:
    return 3;
  case ECCS_TRANSPARENT:
    return -1;
  case ECCS_REFLECTIVE:
    return -1;
  };
  return -1;
}
E_COLLADA_IRR_COLOR CColladaMeshWriterProperties::getColorMapping(const SMaterial& material, E_COLLADA_COLOR_SAMPLER cs) const
{
  switch (cs) {
  case ECCS_DIFFUSE:
    return ECIC_DIFFUSE;
  case ECCS_AMBIENT:
    return ECIC_AMBIENT;
  case ECCS_EMISSIVE:
    return ECIC_EMISSIVE;
  case ECCS_SPECULAR:
    return ECIC_SPECULAR;
  case ECCS_TRANSPARENT:
    return ECIC_NONE;
  case ECCS_REFLECTIVE:
    return ECIC_CUSTOM;
  };
  return ECIC_NONE;
}
//! Return custom colors for certain color types requested by collada.
SColor CColladaMeshWriterProperties::getCustomColor(const SMaterial& material, E_COLLADA_COLOR_SAMPLER cs) const
{
  return SColor(255, 0, 0, 0);
}
//! Return the settings for transparence
E_COLLADA_TRANSPARENT_FX CColladaMeshWriterProperties::getTransparentFx(const SMaterial& material) const
{
  // TODO: figure out best default mapping
  return ECOF_A_ONE;
}
//! Transparency value for the material.
f32 CColladaMeshWriterProperties::getTransparency(const SMaterial& material) const
{
  // TODO: figure out best default mapping
  return -1.f;
}
//! Reflectivity value for that material
f32 CColladaMeshWriterProperties::getReflectivity(const SMaterial& material) const
{
  // TODO: figure out best default mapping
  return 0.f;
}
//! Return index of refraction for that material
f32 CColladaMeshWriterProperties::getIndexOfRefraction(const SMaterial& material) const
{
  return -1.f;
}
bool CColladaMeshWriterProperties::isExportable(const ISceneNode* node) const
{
  return node && node->isVisible();
}
IMesh* CColladaMeshWriterProperties::getMesh(ISceneNode* node)
{
  if (!node) {
    return 0;
  }
  if (node->getType() == ESNT_ANIMATED_MESH) {
    return static_cast<IAnimatedMeshSceneNode*>(node)->getMesh()->getMesh(0);
  }
  // TODO: we need some ISceneNode::hasType() function to get rid of those checks
  if (node->getType() == ESNT_MESH
      ||  node->getType() == ESNT_CUBE
      ||  node->getType() == ESNT_SPHERE
      ||  node->getType() == ESNT_WATER_SURFACE
      ||  node->getType() == ESNT_Q3SHADER_SCENE_NODE
     ) {
    return static_cast<IMeshSceneNode*>(node)->getMesh();
  }
  if (node->getType() == ESNT_TERRAIN) {
    return static_cast<ITerrainSceneNode*>(node)->getMesh();
  }
  return 0;
}
// Check if the node has it's own material overwriting the mesh-materials
bool CColladaMeshWriterProperties::useNodeMaterial(const ISceneNode* node) const
{
  if (!node) {
    return false;
  }
  // TODO: we need some ISceneNode::hasType() function to get rid of those checks
  bool useMeshMaterial = ((node->getType() == ESNT_MESH ||
      node->getType() == ESNT_CUBE ||
      node->getType() == ESNT_SPHERE ||
      node->getType() == ESNT_WATER_SURFACE ||
      node->getType() == ESNT_Q3SHADER_SCENE_NODE)
      && static_cast<const IMeshSceneNode*>(node)->isReadOnlyMaterials())
      || (node->getType() == ESNT_ANIMATED_MESH
          && static_cast<const IAnimatedMeshSceneNode*>(node)->isReadOnlyMaterials());
  return !useMeshMaterial;
}
CColladaMeshWriterNames::CColladaMeshWriterNames(IColladaMeshWriter* writer)
  : ColladaMeshWriter(writer)
{
}
char* CColladaMeshWriterNames::nameForMesh(const IMesh* mesh, int instance)
{
  char* name("mesh");
  name += nameForPtr(mesh);
  if (instance > 0) {
    name += "i";
    name += char * (instance);
  }
  return ColladaMeshWriter->toNCName(name);
}
char* CColladaMeshWriterNames::nameForNode(const ISceneNode* node)
{
  char* name;
  // Prefix, because xs::ID can't start with a number, also nicer name
  if (node && node->getType() == ESNT_LIGHT) {
    name = "light";
  }
  else {
    name = "node";
  }
  name += nameForPtr(node);
  if (node) {
    name += char * (node->getName());
  }
  return ColladaMeshWriter->toNCName(name);
}
char* CColladaMeshWriterNames::nameForMaterial(const SMaterial& material, int materialId, const IMesh* mesh, const ISceneNode* node)
{
  char* strMat("mat");
  bool nodeMaterial = ColladaMeshWriter->getProperties()->useNodeMaterial(node);
  if (nodeMaterial) {
    strMat += "node";
    strMat += nameForPtr(node);
    strMat += char * (node->getName());
  }
  strMat += "mesh";
  strMat += nameForPtr(mesh);
  strMat += materialId;
  return ColladaMeshWriter->toNCName(strMat);
}
char* CColladaMeshWriterNames::nameForPtr(const void* ptr) const
{
  char buf[32];
  snprintf(buf, 32, "%p", ptr);
  return char * (buf);
}
CColladaMeshWriter::CColladaMeshWriter(ISceneManager* smgr, IVideoDriver* driver,
    IFileSystem* fs)
  : FileSystem(fs), VideoDriver(driver), Writer(0)
{
#ifdef _DEBUG
  setDebugName("CColladaMeshWriter");
#endif
  if (VideoDriver) {
    VideoDriver->grab();
  }
  if (FileSystem) {
    FileSystem->grab();
  }
  if (smgr) {
    setAmbientLight(smgr->getAmbientLight());
  }
  CColladaMeshWriterProperties* p = new CColladaMeshWriterProperties();
  setDefaultProperties(p);
  setProperties(p);
  p->drop();
  CColladaMeshWriterNames* nameGenerator = new CColladaMeshWriterNames(this);
  setDefaultNameGenerator(nameGenerator);
  setNameGenerator(nameGenerator);
  nameGenerator->drop();
}
CColladaMeshWriter::~CColladaMeshWriter()
{
  if (VideoDriver) {
    VideoDriver->drop();
  }
  if (FileSystem) {
    FileSystem->drop();
  }
}
void CColladaMeshWriter::reset()
{
  LibraryImages.clear();
  Meshes.clear();
  LightNodes.clear();
  CameraNodes.clear();
  MaterialsWritten.clear();
  EffectsWritten.clear();
  MaterialNameCache.clear();
}
//! Returns the type of the mesh writer
EMESH_WRITER_TYPE CColladaMeshWriter::getType() const
{
  return EMWT_COLLADA;
}
//! writes a scene starting with the given node
bool CColladaMeshWriter::writeScene(IWriteFile* file, ISceneNode* root)
{
  if (!file || !root) {
    return false;
  }
  reset();
  Writer = FileSystem->createXMLWriter(file);
  if (!Writer) {
    os::Printer::log("Could not write file", file->getFileName());
    return false;
  }
  Directory = FileSystem->getFileDir(FileSystem->getAbsolutePath(file->getFileName()));
  // make names for all nodes with exportable meshes
  makeMeshNames(root);
  os::Printer::log("Writing scene", file->getFileName());
  // write COLLADA header
  Writer->writeXMLHeader();
  Writer->writeElement("COLLADA", false,
      "xmlns", "http://www.collada.org/2005/11/COLLADASchema",
      "version", "1.4.1");
  Writer->writeLineBreak();
  // write asset data
  writeAsset();
  // write all materials
  Writer->writeElement("library_materials", false);
  Writer->writeLineBreak();
  writeNodeMaterials(root);
  Writer->writeClosingTag("library_materials");
  Writer->writeLineBreak();
  Writer->writeElement("library_effects", false);
  Writer->writeLineBreak();
  writeNodeEffects(root);
  Writer->writeClosingTag("library_effects");
  Writer->writeLineBreak();
  // images
  writeLibraryImages();
  // lights
  Writer->writeElement("library_lights", false);
  Writer->writeLineBreak();
  writeAmbientLightElement(getAmbientLight());
  writeNodeLights(root);
  Writer->writeClosingTag("library_lights");
  Writer->writeLineBreak();
  // cameras
  Writer->writeElement("library_cameras", false);
  Writer->writeLineBreak();
  writeNodeCameras(root);
  Writer->writeClosingTag("library_cameras");
  Writer->writeLineBreak();
  // write meshes
  Writer->writeElement("library_geometries", false);
  Writer->writeLineBreak();
  writeAllMeshGeometries();
  Writer->writeClosingTag("library_geometries");
  Writer->writeLineBreak();
  // write scene
  Writer->writeElement("library_visual_scenes", false);
  Writer->writeLineBreak();
  Writer->writeElement("visual_scene", false, "id", "default_scene");
  Writer->writeLineBreak();
  // ambient light (instance_light also needs a node as parent so we have to create one)
  Writer->writeElement("node", false);
  Writer->writeLineBreak();
  Writer->writeElement("instance_light", true, "url", "#ambientlight");
  Writer->writeLineBreak();
  Writer->writeClosingTag("node");
  Writer->writeLineBreak();
  // Write the scenegraph.
  if (root->getType() != ESNT_SCENE_MANAGER) {
    // TODO: Not certain if we should really write the root or if we should just always only write the children.
    // For now writing root to keep backward compatibility for this case, but if anyone needs to _not_ write
    // that root-node we can add a parameter for this later on in writeScene.
    writeSceneNode(root);
  }
  else {
    // The visual_scene element is identical to our scenemanager and acts as root,
    // so we do not write the root itself if it points to the scenemanager.
    const list<ISceneNode*>& rootChildren = root->getChildren();
    for (list<ISceneNode*>::ConstIterator it = rootChildren.begin();
        it != rootChildren.end();
        ++ it) {
      writeSceneNode(*it);
    }
  }
  Writer->writeClosingTag("visual_scene");
  Writer->writeLineBreak();
  Writer->writeClosingTag("library_visual_scenes");
  Writer->writeLineBreak();
  // instance scene
  Writer->writeElement("scene", false);
  Writer->writeLineBreak();
  Writer->writeElement("instance_visual_scene", true, "url", "#default_scene");
  Writer->writeLineBreak();
  Writer->writeClosingTag("scene");
  Writer->writeLineBreak();
  // close everything
  Writer->writeClosingTag("COLLADA");
  Writer->drop();
  return true;
}
void CColladaMeshWriter::makeMeshNames(ISceneNode* node)
{
  if (!node || !getProperties() || !getProperties()->isExportable(node) || !getNameGenerator()) {
    return;
  }
  IMesh* mesh = getProperties()->getMesh(node);
  if (mesh) {
    if (!Meshes.find(mesh)) {
      SColladaMesh cm;
      cm.Name = nameForMesh(mesh, 0);
      Meshes.insert(mesh, cm);
    }
  }
  const list<ISceneNode*>& children = node->getChildren();
  for (list<ISceneNode*>::ConstIterator it = children.begin(); it != children.end(); ++it) {
    makeMeshNames(*it);
  }
}
void CColladaMeshWriter::writeNodeMaterials(ISceneNode* node)
{
  if (!node || !getProperties() || !getProperties()->isExportable(node)) {
    return;
  }
  char* > materialNames;
  IMesh* mesh = getProperties()->getMesh(node);
  if (mesh) {
    MeshNode* n = Meshes.find(mesh);
    if (!getProperties()->useNodeMaterial(node)) {
      // no material overrides - write mesh materials
      if (n && !n->getValue().MaterialsWritten) {
        writeMeshMaterials(mesh, getGeometryWriting() == ECGI_PER_MESH_AND_MATERIAL ? &materialNames : NULL);
        n->getValue().MaterialsWritten = true;
      }
    }
    else {
      // write node materials
      int i;
      for (i = 0; i < node->getMaterialCount(); ++i) {
        SMaterial& material = node->getMaterial(i);
        char* strMat(nameForMaterial(material, i, mesh, node));
        writeMaterial(strMat);
        if (getGeometryWriting() == ECGI_PER_MESH_AND_MATERIAL) {
          materialNames.push_back(strMat);
        }
      }
    }
    // When we write another mesh-geometry for each new material-list we have
    // to figure out here if we need another geometry copy and create a new name here.
    if (n && getGeometryWriting() == ECGI_PER_MESH_AND_MATERIAL) {
      SGeometryMeshMaterials* geomMat = n->getValue().findGeometryMeshMaterials(materialNames);
      if (geomMat) {
        geomMat->MaterialOwners.push_back(node);
      }
      else {
        SGeometryMeshMaterials gmm;
        if (n->getValue().GeometryMeshMaterials.empty()) {
          gmm.GeometryName = n->getValue().Name;  // first one can use the original name
        }
        else {
          gmm.GeometryName =  nameForMesh(mesh, n->getValue().GeometryMeshMaterials_size);
        }
        gmm.MaterialNames = materialNames;
        gmm.MaterialOwners.push_back(node);
        n->getValue().GeometryMeshMaterials.push_back(gmm);
      }
    }
  }
  const list<ISceneNode*>& children = node->getChildren();
  for (list<ISceneNode*>::ConstIterator it = children.begin(); it != children.end(); ++it) {
    writeNodeMaterials(*it);
  }
}
void CColladaMeshWriter::writeMaterial(const char* materialname)
{
  if (MaterialsWritten.find(materialname)) {
    return;
  }
  MaterialsWritten.insert(materialname, true);
  Writer->writeElement("material", false,
      "id", materialname,
      "name", materialname);
  Writer->writeLineBreak();
  // We don't make a difference between material and effect on export.
  // Every material is just using an instance of an effect.
  char* strFx(materialname);
  strFx += "-fx";
  Writer->writeElement("instance_effect", true,
      "url", (char * ("#") + strFx));
  Writer->writeLineBreak();
  Writer->writeClosingTag("material");
  Writer->writeLineBreak();
}
void CColladaMeshWriter::writeNodeEffects(ISceneNode* node)
{
  if (!node || !getProperties() || !getProperties()->isExportable(node) || !getNameGenerator()) {
    return;
  }
  IMesh* mesh = getProperties()->getMesh(node);
  if (mesh) {
    if (!getProperties()->useNodeMaterial(node)) {
      // no material overrides - write mesh materials
      MeshNode* n = Meshes.find(mesh);
      if (n  && !n->getValue().EffectsWritten) {
        writeMeshEffects(mesh);
        n->getValue().EffectsWritten = true;
      }
    }
    else {
      // write node materials
      int i;
      for (i = 0; i < node->getMaterialCount(); ++i) {
        SMaterial& material = node->getMaterial(i);
        char* materialfxname(nameForMaterial(material, i, mesh, node));
        materialfxname += "-fx";
        writeMaterialEffect(materialfxname, material);
      }
    }
  }
  const list<ISceneNode*>& children = node->getChildren();
  for (list<ISceneNode*>::ConstIterator it = children.begin(); it != children.end(); ++it) {
    writeNodeEffects(*it);
  }
}
void CColladaMeshWriter::writeNodeLights(ISceneNode* node)
{
  if (!node || !getProperties() || !getProperties()->isExportable(node)) {
    return;
  }
  if (node->getType() == ESNT_LIGHT) {
    ILightSceneNode* lightNode = static_cast<ILightSceneNode*>(node);
    const SLight& lightData = lightNode->getLightData();
    SColladaLight cLight;
    cLight.Name = nameForNode(node);
    LightNodes.insert(node, cLight);
    Writer->writeElement("light", false, "id", cLight.Name);
    Writer->writeLineBreak();
    Writer->writeElement("technique_common", false);
    Writer->writeLineBreak();
    switch (lightNode->getLightType()) {
    case ELT_POINT:
      Writer->writeElement("point", false);
      Writer->writeLineBreak();
      writeColorElement(lightData.DiffuseColor, false);
      writeNode("constant_attenuation ", char * (lightData.Attenuation.X));
      writeNode("linear_attenuation  ", char * (lightData.Attenuation.Y));
      writeNode("quadratic_attenuation", char * (lightData.Attenuation.Z));
      Writer->writeClosingTag("point");
      Writer->writeLineBreak();
      break;
    case ELT_SPOT:
      Writer->writeElement("spot", false);
      Writer->writeLineBreak();
      writeColorElement(lightData.DiffuseColor, false);
      writeNode("constant_attenuation ", char * (lightData.Attenuation.X));
      writeNode("linear_attenuation  ", char * (lightData.Attenuation.Y));
      writeNode("quadratic_attenuation", char * (lightData.Attenuation.Z));
      writeNode("falloff_angle", char * (lightData.OuterCone * RADTODEG));
      writeNode("falloff_exponent", char * (lightData.Falloff));
      Writer->writeClosingTag("spot");
      Writer->writeLineBreak();
      break;
    case ELT_DIRECTIONAL:
      Writer->writeElement("directional", false);
      Writer->writeLineBreak();
      writeColorElement(lightData.DiffuseColor, false);
      Writer->writeClosingTag("directional");
      Writer->writeLineBreak();
      break;
    default:
      break;
    }
    Writer->writeClosingTag("technique_common");
    Writer->writeLineBreak();
    Writer->writeClosingTag("light");
    Writer->writeLineBreak();
  }
  const list<ISceneNode*>& children = node->getChildren();
  for (list<ISceneNode*>::ConstIterator it = children.begin(); it != children.end(); ++it) {
    writeNodeLights(*it);
  }
}
void CColladaMeshWriter::writeNodeCameras(ISceneNode* node)
{
  if (!node || !getProperties() || !getProperties()->isExportable(node)) {
    return;
  }
  if (isCamera(node)) {
    ICameraSceneNode* cameraNode = static_cast<ICameraSceneNode*>(node);
    char* name = nameForNode(node);
    CameraNodes.insert(cameraNode, name);
    Writer->writeElement("camera", false, "id", name);
    Writer->writeLineBreak();
    Writer->writeElement("optics", false);
    Writer->writeLineBreak();
    Writer->writeElement("technique_common", false);
    Writer->writeLineBreak();
    if (cameraNode->isOrthogonal()) {
      Writer->writeElement("orthographic", false);
      Writer->writeLineBreak();
      //      writeNode("xmag", char*("1.0")); // TODO: do we need xmag, ymag?
      //      writeNode("ymag", char*("1.0"));
      writeNode("aspect_ratio", char * (cameraNode->getAspectRatio()));
      writeNode("znear", char * (cameraNode->getNearValue()));
      writeNode("zfar", char * (cameraNode->getFarValue()));
      Writer->writeClosingTag("orthographic");
      Writer->writeLineBreak();
    }
    else {
      Writer->writeElement("perspective", false);
      Writer->writeLineBreak();
      writeNode("yfov", char * (cameraNode->getFOV()*RADTODEG));
      writeNode("aspect_ratio", char * (cameraNode->getAspectRatio()));
      writeNode("znear", char * (cameraNode->getNearValue()));
      writeNode("zfar", char * (cameraNode->getFarValue()));
      Writer->writeClosingTag("perspective");
      Writer->writeLineBreak();
    }
    Writer->writeClosingTag("technique_common");
    Writer->writeLineBreak();
    Writer->writeClosingTag("optics");
    Writer->writeLineBreak();
    Writer->writeClosingTag("camera");
    Writer->writeLineBreak();
  }
  const list<ISceneNode*>& children = node->getChildren();
  for (list<ISceneNode*>::ConstIterator it = children.begin(); it != children.end(); ++it) {
    writeNodeCameras(*it);
  }
}
void CColladaMeshWriter::writeAllMeshGeometries()
{
  map<IMesh*, SColladaMesh>::ConstIterator it = Meshes.getConstIterator();
  for (; !it.atEnd(); it++) {
    IMesh* mesh = it->getKey();
    const SColladaMesh& colladaMesh = it->getValue();
    if (getGeometryWriting() == ECGI_PER_MESH_AND_MATERIAL && colladaMesh.GeometryMeshMaterials_size > 1) {
      int i;
      for (i = 0; i < colladaMesh.GeometryMeshMaterials_size; ++i) {
        writeMeshGeometry(colladaMesh.GeometryMeshMaterials[i].GeometryName, mesh);
      }
    }
    else {
      writeMeshGeometry(colladaMesh.Name, mesh);
    }
  }
}
void CColladaMeshWriter::writeSceneNode(ISceneNode* node)
{
  if (!node || !getProperties() || !getProperties()->isExportable(node)) {
    return;
  }
  // Collada doesn't require to set the id, but some other tools have problems if none exists, so we just add it.
  char* nameId(nameForNode(node));
  Writer->writeElement("node", false, "id", nameId);
  Writer->writeLineBreak();
  // DummyTransformationSceneNode don't have rotation, position, scale information
  // But also don't always export the transformation matrix as that forces us creating
  // new DummyTransformationSceneNode's on import.
  if (node->getType() == ESNT_DUMMY_TRANSFORMATION) {
    writeMatrixElement(node->getRelativeTransformation());
  }
  else {
    float3 rot(node->getRotation());
    if (isCamera(node) && !static_cast<ICameraSceneNode*>(node)->getTargetAndRotationBinding()) {
      ICameraSceneNode* camNode = static_cast<ICameraSceneNode*>(node);
      const float3 toTarget = camNode->getTarget() - camNode->getAbsolutePosition();
      rot = toTarget.getHorizontalAngle();
    }
    writeTranslateElement(node->getPosition());
    writeRotateElement(f3VECTOR(1.f, 0.f, 0.f), rot.X);
    writeRotateElement(f3VECTOR(0.f, 1.f, 0.f), rot.Y);
    writeRotateElement(f3VECTOR(0.f, 0.f, 1.f), rot.Z);
    writeScaleElement(node->getScale());
  }
  // instance geometry
  IMesh* mesh = getProperties()->getMesh(node);
  if (mesh) {
    MeshNode* n = Meshes.find(mesh);
    if (n) {
      const SColladaMesh& colladaMesh = n->getValue();
      writeMeshInstanceGeometry(colladaMesh.findGeometryNameForNode(node), mesh, node);
    }
  }
  // instance light
  if (node->getType() == ESNT_LIGHT) {
    LightNode* n = LightNodes.find(node);
    if (n) {
      writeLightInstance(n->getValue().Name);
    }
  }
  // instance camera
  if (isCamera(node)) {
    CameraNode* camNode = CameraNodes.find(node);
    if (camNode) {
      writeCameraInstance(camNode->getValue());
    }
  }
  const list<ISceneNode*>& children = node->getChildren();
  for (list<ISceneNode*>::ConstIterator it = children.begin(); it != children.end(); ++it) {
    writeSceneNode(*it);
  }
  Writer->writeClosingTag("node");
  Writer->writeLineBreak();
}
//! writes a mesh
bool CColladaMeshWriter::writeMesh(IWriteFile* file, IMesh* mesh, s32 flags)
{
  if (!file) {
    return false;
  }
  reset();
  Writer = FileSystem->createXMLWriter(file);
  if (!Writer) {
    os::Printer::log("Could not write file", file->getFileName());
    return false;
  }
  Directory = FileSystem->getFileDir(FileSystem->getAbsolutePath(file->getFileName()));
  os::Printer::log("Writing mesh", file->getFileName());
  // write COLLADA header
  Writer->writeXMLHeader();
  Writer->writeElement("COLLADA", false,
      "xmlns", "http://www.collada.org/2005/11/COLLADASchema",
      "version", "1.4.1");
  Writer->writeLineBreak();
  // write asset data
  writeAsset();
  // write all materials
  Writer->writeElement("library_materials", false);
  Writer->writeLineBreak();
  writeMeshMaterials(mesh);
  Writer->writeClosingTag("library_materials");
  Writer->writeLineBreak();
  Writer->writeElement("library_effects", false);
  Writer->writeLineBreak();
  writeMeshEffects(mesh);
  Writer->writeClosingTag("library_effects");
  Writer->writeLineBreak();
  // images
  writeLibraryImages();
  // write mesh
  Writer->writeElement("library_geometries", false);
  Writer->writeLineBreak();
  char* meshname(nameForMesh(mesh, 0));
  writeMeshGeometry(meshname, mesh);
  Writer->writeClosingTag("library_geometries");
  Writer->writeLineBreak();
  // write scene_library
  if (getWriteDefaultScene()) {
    Writer->writeElement("library_visual_scenes", false);
    Writer->writeLineBreak();
    Writer->writeElement("visual_scene", false, "id", "default_scene");
    Writer->writeLineBreak();
    Writer->writeElement("node", false);
    Writer->writeLineBreak();
    writeMeshInstanceGeometry(meshname, mesh);
    Writer->writeClosingTag("node");
    Writer->writeLineBreak();
    Writer->writeClosingTag("visual_scene");
    Writer->writeLineBreak();
    Writer->writeClosingTag("library_visual_scenes");
    Writer->writeLineBreak();
    // write scene
    Writer->writeElement("scene", false);
    Writer->writeLineBreak();
    Writer->writeElement("instance_visual_scene", true, "url", "#default_scene");
    Writer->writeLineBreak();
    Writer->writeClosingTag("scene");
    Writer->writeLineBreak();
  }
  // close everything
  Writer->writeClosingTag("COLLADA");
  Writer->drop();
  return true;
}
void CColladaMeshWriter::writeMeshInstanceGeometry(const char* meshname, IMesh* mesh, ISceneNode* node)
{
  //<instance_geometry url="#mesh">
  Writer->writeElement("instance_geometry", false, "url", toRef(meshname));
  Writer->writeLineBreak();
  Writer->writeElement("bind_material", false);
  Writer->writeLineBreak();
  Writer->writeElement("technique_common", false);
  Writer->writeLineBreak();
  // instance materials
  // <instance_material symbol="leaf" target="#MidsummerLeaf01"/>
  bool useNodeMaterials = node && node->getMaterialCount() == mesh->getMeshBufferCount();
  int i;
  for (i = 0; i < mesh->getMeshBufferCount(); ++i) {
    char* strMatSymbol(nameForMaterialSymbol(mesh, i));
    char* strMatTarget = "#";
    SMaterial& material = useNodeMaterials ? node->getMaterial(i) : mesh->getMeshBuffer(i)->getMaterial();
    strMatTarget += nameForMaterial(material, i, mesh, node);
    Writer->writeElement("instance_material", false, "symbol", strMatSymbol, "target", strMatTarget);
    Writer->writeLineBreak();
    // TODO: need to handle second UV-set
    // <bind_vertex_input semantic="uv" input_semantic="TEXCOORD" input_set="0"/>
    Writer->writeElement("bind_vertex_input", true, "semantic", "uv", "input_semantic", "TEXCOORD", "input_set", "0");
    Writer->writeLineBreak();
    Writer->writeClosingTag("instance_material");
    Writer->writeLineBreak();
  }
  Writer->writeClosingTag("technique_common");
  Writer->writeLineBreak();
  Writer->writeClosingTag("bind_material");
  Writer->writeLineBreak();
  Writer->writeClosingTag("instance_geometry");
  Writer->writeLineBreak();
}
void CColladaMeshWriter::writeLightInstance(const char* lightName)
{
  Writer->writeElement("instance_light", true, "url", toRef(lightName));
  Writer->writeLineBreak();
}
void CColladaMeshWriter::writeCameraInstance(const char* cameraName)
{
  Writer->writeElement("instance_camera", true, "url", toRef(cameraName));
  Writer->writeLineBreak();
}
bool CColladaMeshWriter::hasSecondTextureCoordinates(E_VERTEX_TYPE type) const
{
  return type == EVT_2TCOORDS;
}
void CColladaMeshWriter::writeVector(const float3 vec)
{
  char tmpbuf[255];
  snprintf(tmpbuf, 255, "%f %f %f", vec.X, vec.Y, vec.Z);
  Writer->writeText(tmpbuf);
}
void CColladaMeshWriter::writeUv(const vector2df& vec)
{
  // change handedness
  char tmpbuf[255];
  snprintf(tmpbuf, 255, "%f %f", vec.X, 1.f - vec.Y);
  Writer->writeText(tmpbuf);
}
void CColladaMeshWriter::writeVector(const vector2df& vec)
{
  char tmpbuf[255];
  snprintf(tmpbuf, 255, "%f %f", vec.X, vec.Y);
  Writer->writeText(tmpbuf);
}
void CColladaMeshWriter::writeColor(const SColorf& colorf, bool writeAlpha)
{
  char tmpbuf[255];
  if (writeAlpha) {
    snprintf(tmpbuf, 255, "%f %f %f %f", colorf.getRed(), colorf.getGreen(), colorf.getBlue(), colorf.getAlpha());
  }
  else {
    snprintf(tmpbuf, 255, "%f %f %f", colorf.getRed(), colorf.getGreen(), colorf.getBlue());
  }
  Writer->writeText(tmpbuf);
}
char* CColladaMeshWriter::toString(const ECOLOR_FORMAT format) const
{
  switch (format) {
  case ECF_A1R5G5B5:
    return char * ("A1R5G5B5");
  case ECF_R5G6B5:
    return char * ("R5G6B5");
  case ECF_R8G8B8:
    return char * ("R8G8B8");
  case ECF_A8R8G8B8:
    return char * ("A8R8G8B8");
  default:
    return char * ("");
  }
}
char* CColladaMeshWriter::toString(const E_TEXTURE_CLAMP clamp) const
{
  switch (clamp) {
  case ETC_REPEAT:
    return char * ("WRAP");
  case ETC_CLAMP:
  case ETC_CLAMP_TO_EDGE:
    return char * ("CLAMP");
  case ETC_CLAMP_TO_BORDER:
    return char * ("BORDER");
  case ETC_MIRROR:
  case ETC_MIRROR_CLAMP:
  case ETC_MIRROR_CLAMP_TO_EDGE:
  case ETC_MIRROR_CLAMP_TO_BORDER:
    return char * ("MIRROR");
  }
  return char * ("NONE");
}
char* CColladaMeshWriter::toString(const E_COLLADA_TRANSPARENT_FX transparent) const
{
  if (transparent & ECOF_RGB_ZERO) {
    return char * ("RGB_ZERO");
  }
  else {
    return char * ("A_ONE");
  }
}
char* CColladaMeshWriter::toRef(const char* source) const
{
  char* ref("#");
  ref += source;
  return ref;
}
bool CColladaMeshWriter::isCamera(const ISceneNode* node) const
{
  // TODO: we need some ISceneNode::hasType() function to get rid of those checks
  if (node->getType() == ESNT_CAMERA
      ||  node->getType() == ESNT_CAMERA_MAYA
      ||  node->getType() == ESNT_CAMERA_FPS) {
    return true;
  }
  return false;
}
char* CColladaMeshWriter::nameForMesh(const IMesh* mesh, int instance) const
{
  IColladaMeshWriterNames* nameGenerator = getNameGenerator();
  if (nameGenerator) {
    return nameGenerator->nameForMesh(mesh, instance);
  }
  return char * ("missing_name_generator");
}
char* CColladaMeshWriter::nameForNode(const ISceneNode* node) const
{
  IColladaMeshWriterNames* nameGenerator = getNameGenerator();
  if (nameGenerator) {
    return nameGenerator->nameForNode(node);
  }
  return char * ("missing_name_generator");
}
char* CColladaMeshWriter::nameForMaterial(const SMaterial& material, int materialId, const IMesh* mesh, const ISceneNode* node)
{
  char* matName;
  if (getExportSMaterialsOnlyOnce()) {
    matName = findCachedMaterialName(material);
    if (!matName.empty()) {
      return matName;
    }
  }
  IColladaMeshWriterNames* nameGenerator = getNameGenerator();
  if (nameGenerator) {
    matName = nameGenerator->nameForMaterial(material, materialId, mesh, node);
  }
  else {
    matName = char * ("missing_name_generator");
  }
  if (getExportSMaterialsOnlyOnce()) {
    MaterialNameCache.push_back(MaterialName(material, matName));
  }
  return matName;
}
// Each mesh-material has one symbol which is replaced on instantiation
char* CColladaMeshWriter::nameForMaterialSymbol(const IMesh* mesh, int materialId) const
{
  char buf[100];
  snprintf(buf, 100, "mat_symb_%p_%d", mesh, materialId);
  return char * (buf);
}
char* CColladaMeshWriter::findCachedMaterialName(const SMaterial& material) const
{
  int i;
  for (i = 0; i < MaterialNameCache_size; ++i) {
    if (MaterialNameCache[i].Material == material) {
      return MaterialNameCache[i].Name;
    }
  }
  return char * ();
}
char* CColladaMeshWriter::minTexfilterToString(bool bilinear, bool trilinear) const
{
  if (trilinear) {
    return char * ("LINEAR_MIPMAP_LINEAR");
  }
  else if (bilinear) {
    return char * ("LINEAR_MIPMAP_NEAREST");
  }
  return char * ("NONE");
}
CV_INLINE char* CColladaMeshWriter::magTexfilterToString(bool bilinear, bool trilinear) const
{
  if (bilinear || trilinear) {
    return char * ("LINEAR");
  }
  return char * ("NONE");
}
bool CColladaMeshWriter::isXmlNameStartChar(char c) const
{
  return (c >= 'A' && c <= 'Z')
      ||  c == '_'
      || (c >= 'a' && c <= 'z')
      || (c >= 0xC0 && c <= 0xD6)
      || (c >= 0xD8 && c <= 0xF6)
      || (c >= 0xF8 && c <= 0x2FF)
      || (c >= 0x370 && c <= 0x37D)
      || (c >= 0x37F && c <= 0x1FFF)
      || (c >= 0x200C && c <= 0x200D)
      || (c >= 0x2070 && c <= 0x218F)
      || (c >= 0x2C00 && c <= 0x2FEF)
      || (c >= 0x3001 && c <= 0xD7FF)
      || (c >= 0xF900 && c <= 0xFDCF)
      || (c >= 0xFDF0 && c <= 0xFFFD)
#if __SIZEOF_WCHAR_T__ == 4 || __WCHAR_MAX__ > 0x10000
      || (c >= 0x10000 && c <= 0xEFFFF)
#endif
      ;
}
bool CColladaMeshWriter::isXmlNameChar(char c) const
{
  return isXmlNameStartChar(c)
      ||  c == '-'
      ||  c == '.'
      || (c >= '0' && c <= '9')
      ||  c == 0xB7
      || (c >= 0x0300 && c <= 0x036F)
      || (c >= 0x203F && c <= 0x2040);
}
// Restrict the characters to a set of allowed characters in xs::NCName.
char* CColladaMeshWriter::toNCName(const char* oldString, const char* prefix) const
{
  char* result(prefix);  // help to ensure id starts with a valid char and reduce chance of name-conflicts
  if (oldString.empty()) {
    return result;
  }
  result.append(oldString);
  // We replace all characters not allowed by a replacement char
  const char REPLACMENT = '-';
  for (u32 i = 1; i < result_size; ++i) {
    if (result[i] == ':' || !isXmlNameChar(result[i])) {
      result[i] = REPLACMENT;
    }
  }
  return result;
}
// Restrict the characters to a set of allowed characters in xs::NCName.
char* CColladaMeshWriter::pathToURI(const char* path) const
{
  char* result;
  // is this a relative path?
  if (path_size > 1
      && path[0] != _IRR_TEXT('/')
      && path[0] != _IRR_TEXT('\\')
      && path[1] != _IRR_TEXT(':')) {
    // not already starting with "./" ?
    if (path[0] != _IRR_TEXT('.')
        ||  path[1] != _IRR_TEXT('/')) {
      result.append("./");
    }
  }
  result.append(path);
  // TODO: make correct URI (without whitespaces)
  return result;
}
void CColladaMeshWriter::writeAsset()
{
  Writer->writeElement("asset", false);
  Writer->writeLineBreak();
  Writer->writeElement("contributor", false);
  Writer->writeLineBreak();
  Writer->writeElement("authoring_tool", false);
  Writer->writeText("Irrlicht Engine / irrEdit");  // this code has originated from irrEdit 0.7
  Writer->writeClosingTag("authoring_tool");
  Writer->writeLineBreak();
  Writer->writeClosingTag("contributor");
  Writer->writeLineBreak();
  // The next two are required
  Writer->writeElement("created", false);
  Writer->writeText("2008-01-31T00:00:00Z");
  Writer->writeClosingTag("created");
  Writer->writeLineBreak();
  Writer->writeElement("modified", false);
  Writer->writeText("2008-01-31T00:00:00Z");
  Writer->writeClosingTag("modified");
  Writer->writeLineBreak();
  Writer->writeElement("revision", false);
  Writer->writeText("1.0");
  Writer->writeClosingTag("revision");
  Writer->writeLineBreak();
  Writer->writeClosingTag("asset");
  Writer->writeLineBreak();
}
void CColladaMeshWriter::writeMeshMaterials(IMesh* mesh, char* > * materialNamesOut)
{
  u32 i;
  for (i = 0; i < mesh->getMeshBufferCount(); ++i) {
    SMaterial& material = mesh->getMeshBuffer(i)->getMaterial();
    char* strMat(nameForMaterial(material, i, mesh, NULL));
    writeMaterial(strMat);
    if (materialNamesOut) {
      materialNamesOut->push_back(strMat);
    }
  }
}
void CColladaMeshWriter::writeMaterialEffect(const char* materialfxname, const SMaterial& material)
{
  if (EffectsWritten.find(materialfxname)) {
    return;
  }
  EffectsWritten.insert(materialfxname, true);
  Writer->writeElement("effect", false,
      "id", materialfxname,
      "name", materialfxname);
  Writer->writeLineBreak();
  Writer->writeElement("profile_COMMON", false);
  Writer->writeLineBreak();
  int numTextures = 0;
  if (getWriteTextures()) {
    // write texture surfaces and samplers and buffer all used imagess
    for (int t = 0; t < 4; ++t) {
      const SMaterialLayer& layer  = material.TextureLayer[t];
      if (!layer.Texture) {
        break;
      }
      ++numTextures;
      if (LibraryImages.linear_search(layer.Texture) < 0) {
        LibraryImages.push_back(layer.Texture);
      }
      char* texName("tex");
      texName += char * (t);
      // write texture surface
      //<newparam sid="tex0-surface">
      char* texSurface(texName);
      texSurface += "-surface";
      Writer->writeElement("newparam", false, "sid", texSurface);
      Writer->writeLineBreak();
      //  <surface type="2D">
      Writer->writeElement("surface", false, "type", "2D");
      Writer->writeLineBreak();
      //          <init_from>internal_texturename</init_from>
      Writer->writeElement("init_from", false);
      char* p(FileSystem->getRelativeFilename(layer.Texture->getName().getPath(), Directory));
      Writer->writeText(toNCName(char * (p)));
      Writer->writeClosingTag("init_from");
      Writer->writeLineBreak();
      //          <format>A8R8G8B8</format>
      Writer->writeElement("format", false);
      ECOLOR_FORMAT format = layer.Texture->getColorFormat();
      Writer->writeText(toString(format));
      Writer->writeClosingTag("format");
      Writer->writeLineBreak();
      //      </surface>
      Writer->writeClosingTag("surface");
      Writer->writeLineBreak();
      //  </newparam>
      Writer->writeClosingTag("newparam");
      Writer->writeLineBreak();
      // write texture sampler
      //  <newparam sid="tex0-sampler">
      char* texSampler(texName);
      texSampler += "-sampler";
      Writer->writeElement("newparam", false, "sid", texSampler);
      Writer->writeLineBreak();
      //      <sampler2D>
      Writer->writeElement("sampler2D", false);
      Writer->writeLineBreak();
      //          <source>tex0-surface</source>
      Writer->writeElement("source", false);
      Writer->writeText(texSurface);
      Writer->writeClosingTag("source");
      Writer->writeLineBreak();
      //      <wrap_s>WRAP</wrap_s>
      Writer->writeElement("wrap_s", false);
      Writer->writeText(toString((E_TEXTURE_CLAMP)layer.TextureWrapU));
      Writer->writeClosingTag("wrap_s");
      Writer->writeLineBreak();
      //      <wrap_t>WRAP</wrap_t>
      Writer->writeElement("wrap_t", false);
      Writer->writeText(toString((E_TEXTURE_CLAMP)layer.TextureWrapV));
      Writer->writeClosingTag("wrap_t");
      Writer->writeLineBreak();
      //      <minfilter>LINEAR_MIPMAP_LINEAR</minfilter>
      Writer->writeElement("minfilter", false);
      Writer->writeText(minTexfilterToString(layer.BilinearFilter, layer.TrilinearFilter));
      Writer->writeClosingTag("minfilter");
      Writer->writeLineBreak();
      //      <magfilter>LINEAR</magfilter>
      Writer->writeElement("magfilter", false);
      Writer->writeText(magTexfilterToString(layer.BilinearFilter, layer.TrilinearFilter));
      Writer->writeClosingTag("magfilter");
      Writer->writeLineBreak();
      // TBD - actually not sure how anisotropic should be written, so for now it writes in a way
      // that works with the way the loader reads it again.
      if (layer.AnisotropicFilter) {
        //      <mipfilter>LINEAR_MIPMAP_LINEAR</mipfilter>
        Writer->writeElement("mipfilter", false);
        Writer->writeText("LINEAR_MIPMAP_LINEAR");
        Writer->writeClosingTag("mipfilter");
        Writer->writeLineBreak();
      }
      //     </sampler2D>
      Writer->writeClosingTag("sampler2D");
      Writer->writeLineBreak();
      //  </newparam>
      Writer->writeClosingTag("newparam");
      Writer->writeLineBreak();
    }
  }
  Writer->writeElement("technique", false, "sid", "common");
  Writer->writeLineBreak();
  E_COLLADA_TECHNIQUE_FX techFx = getProperties() ? getProperties()->getTechniqueFx(material) : ECTF_BLINN;
  writeFxElement(material, techFx);
  Writer->writeClosingTag("technique");
  Writer->writeLineBreak();
  Writer->writeClosingTag("profile_COMMON");
  Writer->writeLineBreak();
  Writer->writeClosingTag("effect");
  Writer->writeLineBreak();
}
void CColladaMeshWriter::writeMeshEffects(IMesh* mesh)
{
  int i;
  for (i = 0; i < mesh->getMeshBufferCount(); ++i) {
    SMaterial& material = mesh->getMeshBuffer(i)->getMaterial();
    char* materialfxname(nameForMaterial(material, i, mesh, NULL));
    materialfxname += "-fx";
    writeMaterialEffect(materialfxname, material);
  }
}
void CColladaMeshWriter::writeMeshGeometry(const char* meshname, IMesh* mesh)
{
  char* meshId(meshname);
  Writer->writeElement("geometry", false, "id", meshId, "name", meshId);
  Writer->writeLineBreak();
  Writer->writeElement("mesh");
  Writer->writeLineBreak();
  // do some statistics for the mesh to know which stuff needs to be saved into
  // the file:
  // - count vertices
  // - check for the need of a second texture coordinate
  // - count amount of second texture coordinates
  // - check for the need of tangents (TODO)
  u32 totalVertexCount = 0;
  u32 totalTCoords2Count = 0;
  bool needsTangents = false; // TODO: tangents not supported here yet
  u32 i = 0;
  for (i = 0; i < mesh->getMeshBufferCount(); ++i) {
    totalVertexCount += mesh->getMeshBuffer(i)->getVertexCount();
    if (hasSecondTextureCoordinates(mesh->getMeshBuffer(i)->getVertexType())) {
      totalTCoords2Count += mesh->getMeshBuffer(i)->getVertexCount();
    }
    if (!needsTangents) {
      needsTangents = mesh->getMeshBuffer(i)->getVertexType() == EVT_TANGENTS;
    }
  }
  SComponentGlobalStartPos* globalIndices = new SComponentGlobalStartPos[mesh->getMeshBufferCount()];
  // write positions
  char* meshPosId(meshId);
  meshPosId += "-Pos";
  Writer->writeElement("source", false, "id", meshPosId);
  Writer->writeLineBreak();
  char* vertexCountStr((int)totalVertexCount * 3);
  char* meshPosArrayId(meshPosId);
  meshPosArrayId += "-array";
  Writer->writeElement("float_array", false, "id", meshPosArrayId,
      "count", vertexCountStr);
  Writer->writeLineBreak();
  for (i = 0; i < mesh->getMeshBufferCount(); ++i) {
    IMeshBuffer* buffer = mesh->getMeshBuffer(i);
    E_VERTEX_TYPE vtxType = buffer->getVertexType();
    u32 vertexCount = buffer->getVertexCount();
    globalIndices[i].PosStartIndex = 0;
    if (i != 0) {
      globalIndices[i].PosStartIndex = globalIndices[i - 1].PosLastIndex + 1;
    }
    globalIndices[i].PosLastIndex = globalIndices[i].PosStartIndex + vertexCount - 1;
    switch (vtxType) {
    case EVT_STANDARD: {
      S3DVertex* vtx = (S3DVertex*)buffer->getVertices();
      for (int j = 0; j < vertexCount; ++j) {
        writeVector(vtx[j].Pos);
        Writer->writeLineBreak();
      }
    }
    break;
    case EVT_2TCOORDS: {
      S3DVertex* vtx = (S3DVertex*)buffer->getVertices();
      for (int j = 0; j < vertexCount; ++j) {
        writeVector(vtx[j].Pos);
        Writer->writeLineBreak();
      }
    }
    break;
    case EVT_TANGENTS: {
      S3DVertex* vtx = (S3DVertex*)buffer->getVertices();
      for (int j = 0; j < vertexCount; ++j) {
        writeVector(vtx[j].Pos);
        Writer->writeLineBreak();
      }
    }
    break;
    }
  }
  Writer->writeClosingTag("float_array");
  Writer->writeLineBreak();
  Writer->writeElement("technique_common", false);
  Writer->writeLineBreak();
  vertexCountStr = char * ((int)totalVertexCount);
  Writer->writeElement("accessor", false, "source", toRef(meshPosArrayId),
      "count", vertexCountStr, "stride", "3");
  Writer->writeLineBreak();
  Writer->writeElement("param", true, "name", "X", "type", "float");
  Writer->writeLineBreak();
  Writer->writeElement("param", true, "name", "Y", "type", "float");
  Writer->writeLineBreak();
  Writer->writeElement("param", true, "name", "Z", "type", "float");
  Writer->writeLineBreak();
  Writer->writeClosingTag("accessor");
  Writer->writeLineBreak();
  Writer->writeClosingTag("technique_common");
  Writer->writeLineBreak();
  Writer->writeClosingTag("source");
  Writer->writeLineBreak();
  // write texture coordinates
  char* meshTexCoord0Id(meshId);
  meshTexCoord0Id += "-TexCoord0";
  Writer->writeElement("source", false, "id", meshTexCoord0Id);
  Writer->writeLineBreak();
  vertexCountStr = char * ((int)totalVertexCount * 2);
  char* meshTexCoordArrayId(meshTexCoord0Id);
  meshTexCoordArrayId += "-array";
  Writer->writeElement("float_array", false, "id", meshTexCoordArrayId,
      "count", vertexCountStr);
  Writer->writeLineBreak();
  for (i = 0; i < mesh->getMeshBufferCount(); ++i) {
    IMeshBuffer* buffer = mesh->getMeshBuffer(i);
    E_VERTEX_TYPE vtxType = buffer->getVertexType();
    u32 vertexCount = buffer->getVertexCount();
    globalIndices[i].TCoord0StartIndex = 0;
    if (i != 0) {
      globalIndices[i].TCoord0StartIndex = globalIndices[i - 1].TCoord0LastIndex + 1;
    }
    globalIndices[i].TCoord0LastIndex = globalIndices[i].TCoord0StartIndex + vertexCount - 1;
    switch (vtxType) {
    case EVT_STANDARD: {
      S3DVertex* vtx = (S3DVertex*)buffer->getVertices();
      for (int j = 0; j < vertexCount; ++j) {
        writeUv(vtx[j].TCoords);
        Writer->writeLineBreak();
      }
    }
    break;
    case EVT_2TCOORDS: {
      S3DVertex* vtx = (S3DVertex*)buffer->getVertices();
      for (int j = 0; j < vertexCount; ++j) {
        writeUv(vtx[j].TCoords);
        Writer->writeLineBreak();
      }
    }
    break;
    case EVT_TANGENTS: {
      S3DVertex* vtx = (S3DVertex*)buffer->getVertices();
      for (int j = 0; j < vertexCount; ++j) {
        writeUv(vtx[j].TCoords);
        Writer->writeLineBreak();
      }
    }
    break;
    }
  }
  Writer->writeClosingTag("float_array");
  Writer->writeLineBreak();
  Writer->writeElement("technique_common", false);
  Writer->writeLineBreak();
  vertexCountStr = char * ((int)totalVertexCount);
  Writer->writeElement("accessor", false, "source", toRef(meshTexCoordArrayId),
      "count", vertexCountStr, "stride", "2");
  Writer->writeLineBreak();
  Writer->writeElement("param", true, "name", "U", "type", "float");
  Writer->writeLineBreak();
  Writer->writeElement("param", true, "name", "V", "type", "float");
  Writer->writeLineBreak();
  Writer->writeClosingTag("accessor");
  Writer->writeLineBreak();
  Writer->writeClosingTag("technique_common");
  Writer->writeLineBreak();
  Writer->writeClosingTag("source");
  Writer->writeLineBreak();
  // write normals
  char* meshNormalId(meshId);
  meshNormalId += "-Normal";
  Writer->writeElement("source", false, "id", meshNormalId);
  Writer->writeLineBreak();
  vertexCountStr = char * ((int)totalVertexCount * 3);
  char* meshNormalArrayId(meshNormalId);
  meshNormalArrayId += "-array";
  Writer->writeElement("float_array", false, "id", meshNormalArrayId,
      "count", vertexCountStr);
  Writer->writeLineBreak();
  for (i = 0; i < mesh->getMeshBufferCount(); ++i) {
    IMeshBuffer* buffer = mesh->getMeshBuffer(i);
    E_VERTEX_TYPE vtxType = buffer->getVertexType();
    u32 vertexCount = buffer->getVertexCount();
    globalIndices[i].NormalStartIndex = 0;
    if (i != 0) {
      globalIndices[i].NormalStartIndex = globalIndices[i - 1].NormalLastIndex + 1;
    }
    globalIndices[i].NormalLastIndex = globalIndices[i].NormalStartIndex + vertexCount - 1;
    switch (vtxType) {
    case EVT_STANDARD: {
      S3DVertex* vtx = (S3DVertex*)buffer->getVertices();
      for (int j = 0; j < vertexCount; ++j) {
        writeVector(vtx[j].Normal);
        Writer->writeLineBreak();
      }
    }
    break;
    case EVT_2TCOORDS: {
      S3DVertex* vtx = (S3DVertex*)buffer->getVertices();
      for (int j = 0; j < vertexCount; ++j) {
        writeVector(vtx[j].Normal);
        Writer->writeLineBreak();
      }
    }
    break;
    case EVT_TANGENTS: {
      S3DVertex* vtx = (S3DVertex*)buffer->getVertices();
      for (int j = 0; j < vertexCount; ++j) {
        writeVector(vtx[j].Normal);
        Writer->writeLineBreak();
      }
    }
    break;
    }
  }
  Writer->writeClosingTag("float_array");
  Writer->writeLineBreak();
  Writer->writeElement("technique_common", false);
  Writer->writeLineBreak();
  vertexCountStr = char * ((int)totalVertexCount);
  Writer->writeElement("accessor", false, "source", toRef(meshNormalArrayId),
      "count", vertexCountStr, "stride", "3");
  Writer->writeLineBreak();
  Writer->writeElement("param", true, "name", "X", "type", "float");
  Writer->writeLineBreak();
  Writer->writeElement("param", true, "name", "Y", "type", "float");
  Writer->writeLineBreak();
  Writer->writeElement("param", true, "name", "Z", "type", "float");
  Writer->writeLineBreak();
  Writer->writeClosingTag("accessor");
  Writer->writeLineBreak();
  Writer->writeClosingTag("technique_common");
  Writer->writeLineBreak();
  Writer->writeClosingTag("source");
  Writer->writeLineBreak();
  // write second set of texture coordinates
  char* meshTexCoord1Id(meshId);
  meshTexCoord1Id += "-TexCoord1";
  if (totalTCoords2Count) {
    Writer->writeElement("source", false, "id", meshTexCoord1Id);
    Writer->writeLineBreak();
    vertexCountStr = char * ((int)totalTCoords2Count * 2);
    char* meshTexCoord1ArrayId(meshTexCoord1Id);
    meshTexCoord1ArrayId += "-array";
    Writer->writeElement("float_array", false, "id", meshTexCoord1ArrayId,
        "count", vertexCountStr);
    Writer->writeLineBreak();
    for (i = 0; i < mesh->getMeshBufferCount(); ++i) {
      IMeshBuffer* buffer = mesh->getMeshBuffer(i);
      E_VERTEX_TYPE vtxType = buffer->getVertexType();
      u32 vertexCount = buffer->getVertexCount();
      if (hasSecondTextureCoordinates(vtxType)) {
        globalIndices[i].TCoord1StartIndex = 0;
        if (i != 0 && globalIndices[i - 1].TCoord1LastIndex != -1) {
          globalIndices[i].TCoord1StartIndex = globalIndices[i - 1].TCoord1LastIndex + 1;
        }
        globalIndices[i].TCoord1LastIndex = globalIndices[i].TCoord1StartIndex + vertexCount - 1;
        switch (vtxType) {
        case EVT_2TCOORDS: {
          S3DVertex* vtx = (S3DVertex*)buffer->getVertices();
          for (int j = 0; j < vertexCount; ++j) {
            writeVector(vtx[j].TCoords2);
            Writer->writeLineBreak();
          }
        }
        break;
        default:
          break;
        }
      } // end this buffer has 2 texture coordinates
    }
    Writer->writeClosingTag("float_array");
    Writer->writeLineBreak();
    Writer->writeElement("technique_common", false);
    Writer->writeLineBreak();
    vertexCountStr = char * ((int)totalTCoords2Count);
    Writer->writeElement("accessor", false, "source", toRef(meshTexCoord1ArrayId),
        "count", vertexCountStr, "stride", "2");
    Writer->writeLineBreak();
    Writer->writeElement("param", true, "name", "U", "type", "float");
    Writer->writeLineBreak();
    Writer->writeElement("param", true, "name", "V", "type", "float");
    Writer->writeLineBreak();
    Writer->writeClosingTag("accessor");
    Writer->writeLineBreak();
    Writer->writeClosingTag("technique_common");
    Writer->writeLineBreak();
    Writer->writeClosingTag("source");
    Writer->writeLineBreak();
  }
  // write tangents
  // TODO
  // write vertices
  char* meshVtxId(meshId);
  meshVtxId += "-Vtx";
  Writer->writeElement("vertices", false, "id", meshVtxId);
  Writer->writeLineBreak();
  Writer->writeElement("input", true, "semantic", "POSITION", "source", toRef(meshPosId));
  Writer->writeLineBreak();
  Writer->writeClosingTag("vertices");
  Writer->writeLineBreak();
  // write polygons
  for (i = 0; i < mesh->getMeshBufferCount(); ++i) {
    IMeshBuffer* buffer = mesh->getMeshBuffer(i);
    const u32 polyCount = buffer->getIndexCount() / 3;
    char* strPolyCount((int)polyCount);
    char* strMat(nameForMaterialSymbol(mesh, i));
    Writer->writeElement("triangles", false, "count", strPolyCount,
        "material", strMat);
    Writer->writeLineBreak();
    Writer->writeElement("input", true, "semantic", "VERTEX", "source", toRef(meshVtxId), "offset", "0");
    Writer->writeLineBreak();
    Writer->writeElement("input", true, "semantic", "TEXCOORD", "source", toRef(meshTexCoord0Id), "offset", "1", "set", "0");
    Writer->writeLineBreak();
    Writer->writeElement("input", true, "semantic", "NORMA", "source", toRef(meshNormalId), "offset", "2");
    Writer->writeLineBreak();
    bool has2ndTexCoords = hasSecondTextureCoordinates(buffer->getVertexType());
    if (has2ndTexCoords) {
      // TODO: when working on second uv-set - my suspicion is that this one should be called "TEXCOORD2"
      // to allow bind_vertex_input to differentiate the uv-sets.
      Writer->writeElement("input", true, "semantic", "TEXCOORD", "source", toRef(meshTexCoord1Id), "idx", "3");
      Writer->writeLineBreak();
    }
    // write indices now
    s32 posIdx = globalIndices[i].PosStartIndex;
    s32 tCoordIdx = globalIndices[i].TCoord0StartIndex;
    s32 normalIdx = globalIndices[i].NormalStartIndex;
    s32 tCoord2Idx = globalIndices[i].TCoord1StartIndex;
    Writer->writeElement("p", false);
    char* strP;
    strP.reserve(100);
    for (int p = 0; p < polyCount; ++p) {
      strP = "";
      strP += buffer->getIndices()[(p * 3) + 0] + posIdx;
      strP += " ";
      strP += buffer->getIndices()[(p * 3) + 0] + tCoordIdx;
      strP += " ";
      strP += buffer->getIndices()[(p * 3) + 0] + normalIdx;
      strP += " ";
      if (has2ndTexCoords) {
        strP += buffer->getIndices()[(p * 3) + 0] + tCoord2Idx;
        strP += " ";
      }
      strP += buffer->getIndices()[(p * 3) + 1] + posIdx;
      strP += " ";
      strP += buffer->getIndices()[(p * 3) + 1] + tCoordIdx;
      strP += " ";
      strP += buffer->getIndices()[(p * 3) + 1] + normalIdx;
      strP += " ";
      if (has2ndTexCoords) {
        strP += buffer->getIndices()[(p * 3) + 1] + tCoord2Idx;
        strP += " ";
      }
      strP += buffer->getIndices()[(p * 3) + 2] + posIdx;
      strP += " ";
      strP += buffer->getIndices()[(p * 3) + 2] + tCoordIdx;
      strP += " ";
      strP += buffer->getIndices()[(p * 3) + 2] + normalIdx;
      if (has2ndTexCoords) {
        strP += " ";
        strP += buffer->getIndices()[(p * 3) + 2] + tCoord2Idx;
      }
      strP += " ";
      Writer->writeText(strP);
    }
    Writer->writeClosingTag("p");
    Writer->writeLineBreak();
    // close index buffer section
    Writer->writeClosingTag("triangles");
    Writer->writeLineBreak();
  }
  // close mesh and geometry
  delete [] globalIndices;
  Writer->writeClosingTag("mesh");
  Writer->writeLineBreak();
  Writer->writeClosingTag("geometry");
  Writer->writeLineBreak();
}
void CColladaMeshWriter::writeLibraryImages()
{
  if (getWriteTextures() && !LibraryImages.empty()) {
    Writer->writeElement("library_images", false);
    Writer->writeLineBreak();
    for (u32 i = 0; i < LibraryImages_size; ++i) {
      char* p(FileSystem->getRelativeFilename(LibraryImages[i]->getName().getPath(), Directory));
      //<image name="rose01">
      char* ncname(toNCName(char * (p)));
      Writer->writeElement("image", false, "id", ncname, "name", ncname);
      Writer->writeLineBreak();
      //  <init_from>../flowers/rose01.jpg</init_from>
      Writer->writeElement("init_from", false);
      Writer->writeText(pathToURI(p));
      Writer->writeClosingTag("init_from");
      Writer->writeLineBreak();
      //  </image>
      Writer->writeClosingTag("image");
      Writer->writeLineBreak();
    }
    Writer->writeClosingTag("library_images");
    Writer->writeLineBreak();
  }
}
void CColladaMeshWriter::writeColorElement(const SColorf& col, bool writeAlpha)
{
  Writer->writeElement("color", false);
  writeColor(col, writeAlpha);
  Writer->writeClosingTag("color");
  Writer->writeLineBreak();
}
void CColladaMeshWriter::writeColorElement(const SColor& col, bool writeAlpha)
{
  writeColorElement(SColorf(col), writeAlpha);
}
void CColladaMeshWriter::writeAmbientLightElement(const SColorf& col)
{
  Writer->writeElement("light", false, "id", "ambientlight");
  Writer->writeLineBreak();
  Writer->writeElement("technique_common", false);
  Writer->writeLineBreak();
  Writer->writeElement("ambient", false);
  Writer->writeLineBreak();
  writeColorElement(col, false);
  Writer->writeClosingTag("ambient");
  Writer->writeLineBreak();
  Writer->writeClosingTag("technique_common");
  Writer->writeLineBreak();
  Writer->writeClosingTag("light");
  Writer->writeLineBreak();
}
s32 CColladaMeshWriter::getCheckedTextureIdx(const SMaterial& material, E_COLLADA_COLOR_SAMPLER cs)
{
  if (!getWriteTextures()
      ||  !getProperties()) {
    return -1;
  }
  s32 idx = getProperties()->getTextureIdx(material, cs);
  if (idx >= 0 && !material.TextureLayer[idx].Texture) {
    return -1;
  }
  return idx;
}
SColor CColladaMeshWriter::getColorMapping(const SMaterial& material, E_COLLADA_COLOR_SAMPLER cs, E_COLLADA_IRR_COLOR colType)
{
  switch (colType) {
  case ECIC_NONE:
    return SColor(255, 0, 0, 0);
  case ECIC_CUSTOM:
    return getProperties()->getCustomColor(material, cs);
  case ECIC_DIFFUSE:
    return material.DiffuseColor;
  case ECIC_AMBIENT:
    return material.AmbientColor;
  case ECIC_EMISSIVE:
    return material.EmissiveColor;
  case ECIC_SPECULAR:
    return material.SpecularColor;
  }
  return SColor(255, 0, 0, 0);
}
void CColladaMeshWriter::writeTextureSampler(s32 textureIdx)
{
  char* sampler("tex");
  sampler += char * (textureIdx);
  sampler += "-sampler";
  // <texture texture="sampler" texcoord="texCoordUv"/>
  Writer->writeElement("texture", true, "texture", sampler, "texcoord", "uv");
  Writer->writeLineBreak();
}
void CColladaMeshWriter::writeFxElement(const SMaterial& material, E_COLLADA_TECHNIQUE_FX techFx)
{
  char* fxLabel;
  bool writeEmission = true;
  bool writeAmbient = true;
  bool writeDiffuse = true;
  bool writeSpecular = true;
  bool writeShininess = true;
  bool writeReflective = true;
  bool writeReflectivity = true;
  bool writeTransparent = true;
  bool writeTransparency = true;
  bool writeIndexOfRefraction = true;
  switch (techFx) {
  case ECTF_BLINN:
    fxLabel = "blinn";
    break;
  case ECTF_PHONG:
    fxLabel = "phong";
    break;
  case ECTF_LAMBERT:
    fxLabel = "lambert";
    writeSpecular = false;
    writeShininess = false;
    break;
  case ECTF_CONSTANT:
    fxLabel = "constant";
    writeAmbient = false;
    writeDiffuse = false;
    writeSpecular = false;
    writeShininess = false;
    break;
  }
  Writer->writeElement(fxLabel, false);
  Writer->writeLineBreak();
  // write all interesting material parameters
  // attributes must be written in fixed order
  if (getProperties()) {
    if (writeEmission) {
      writeColorFx(material, "emission", ECCS_EMISSIVE);
    }
    if (writeAmbient) {
      writeColorFx(material, "ambient", ECCS_AMBIENT);
    }
    if (writeDiffuse) {
      writeColorFx(material, "diffuse", ECCS_DIFFUSE);
    }
    if (writeSpecular) {
      writeColorFx(material, "specular", ECCS_SPECULAR);
    }
    if (writeShininess) {
      Writer->writeElement("shininess", false);
      Writer->writeLineBreak();
      writeFloatElement(material.Shininess);
      Writer->writeClosingTag("shininess");
      Writer->writeLineBreak();
    }
    if (writeReflective) {
      writeColorFx(material, "reflective", ECCS_REFLECTIVE);
    }
    if (writeReflectivity) {
      f32 t = getProperties()->getReflectivity(material);
      if (t >= 0.f) {
        // <transparency>  <float>1.000000</float> </transparency>
        Writer->writeElement("reflectivity", false);
        Writer->writeLineBreak();
        writeFloatElement(t);
        Writer->writeClosingTag("reflectivity");
        Writer->writeLineBreak();
      }
    }
    if (writeTransparent) {
      E_COLLADA_TRANSPARENT_FX transparentFx = getProperties()->getTransparentFx(material);
      writeColorFx(material, "transparent", ECCS_TRANSPARENT, "opaque", toString(transparentFx));
    }
    if (writeTransparency) {
      f32 t = getProperties()->getTransparency(material);
      if (t >= 0.f) {
        // <transparency>  <float>1.000000</float> </transparency>
        Writer->writeElement("transparency", false);
        Writer->writeLineBreak();
        writeFloatElement(t);
        Writer->writeClosingTag("transparency");
        Writer->writeLineBreak();
      }
    }
    if (writeIndexOfRefraction) {
      f32 t = getProperties()->getIndexOfRefraction(material);
      if (t >= 0.f) {
        Writer->writeElement("index_of_refraction", false);
        Writer->writeLineBreak();
        writeFloatElement(t);
        Writer->writeClosingTag("index_of_refraction");
        Writer->writeLineBreak();
      }
    }
  }
  Writer->writeClosingTag(fxLabel);
  Writer->writeLineBreak();
}
void CColladaMeshWriter::writeColorFx(const SMaterial& material, const char* colorname, E_COLLADA_COLOR_SAMPLER cs, const char* attr1Name, const char* attr1Value)
{
  s32 idx = getCheckedTextureIdx(material, cs);
  E_COLLADA_IRR_COLOR colType = idx < 0 ? getProperties()->getColorMapping(material, cs) : ECIC_NONE;
  if (idx >= 0 || colType != ECIC_NONE) {
    Writer->writeElement(colorname, false, attr1Name, attr1Value);
    Writer->writeLineBreak();
    if (idx >= 0) {
      writeTextureSampler(idx);
    }
    else {
      writeColorElement(getColorMapping(material, cs, colType));
    }
    Writer->writeClosingTag(colorname);
    Writer->writeLineBreak();
  }
}
void CColladaMeshWriter::writeNode(const char* nodeName, const char* content)
{
  Writer->writeElement(nodeName, false);
  Writer->writeText(content);
  Writer->writeClosingTag(nodeName);
  Writer->writeLineBreak();
}
void CColladaMeshWriter::writeFloatElement(f32 value)
{
  Writer->writeElement("float", false);
  Writer->writeText(char * ((double)value));
  Writer->writeClosingTag("float");
  Writer->writeLineBreak();
}
void CColladaMeshWriter::writeRotateElement(const float3 axis, f32 angle)
{
  Writer->writeElement("rotate", false);
  char* txt(axis.X);
  txt += " ";
  txt += char * (axis.Y);
  txt += " ";
  txt += char * (axis.Z);
  txt += " ";
  txt += char * ((double)angle);
  Writer->writeText(txt);
  Writer->writeClosingTag("rotate");
  Writer->writeLineBreak();
}
void CColladaMeshWriter::writeScaleElement(const float3 scale)
{
  Writer->writeElement("scale", false);
  char* txt(scale.X);
  txt += " ";
  txt += char * (scale.Y);
  txt += " ";
  txt += char * (scale.Z);
  Writer->writeText(txt);
  Writer->writeClosingTag("scale");
  Writer->writeLineBreak();
}
void CColladaMeshWriter::writeTranslateElement(const float3 translate)
{
  Writer->writeElement("translate", false);
  char* txt(translate.X);
  txt += " ";
  txt += char * (translate.Y);
  txt += " ";
  txt += char * (translate.Z);
  Writer->writeText(txt);
  Writer->writeClosingTag("translate");
  Writer->writeLineBreak();
}
void CColladaMeshWriter::writeMatrixElement(const matrix4& matrix)
{
  Writer->writeElement("matrix", false);
  Writer->writeLineBreak();
  for (int a = 0; a < 4; ++a) {
    char* txt;
    for (int b = 0; b < 4; ++b) {
      if (b > 0) {
        txt += " ";
      }
      // row-column switched compared to Irrlicht
      txt += char * (matrix[b * 4 + a]);
    }
    Writer->writeText(txt);
    Writer->writeLineBreak();
  }
  Writer->writeClosingTag("matrix");
  Writer->writeLineBreak();
}
// end
// end
#endif

