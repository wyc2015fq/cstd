// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
// The code for the TerrainSceneNode is based on the GeoMipMapSceneNode
// developed by Spintz. He made it available for Irrlicht and allowed it to be
// distributed under this licence. I only modified some parts. A lot of thanks go to him.
#ifndef __C_TERRAIN_SCENE_NODE_H__
#define __C_TERRAIN_SCENE_NODE_H__
//#include "ITerrainSceneNode.h"
class IFileSystem;
class IReadFile;
}
struct SMesh;
class ITextSceneNode;
//! A scene node for displaying terrain using the geo mip map algorithm.
class CTerrainSceneNode : public ITerrainSceneNode
{
public:
  //! constructor
  //! \param parent: The node which this node is a child of.  Making this node a child of another node, or
  //! making it a parent of another node is yet untested and most likely does not work properly.
  //! \param mgr: Pointer to the scene manager.
  //! \param id: The id of the node
  //! \param maxLOD: The maximum LOD ( Level of Detail ) for the node.
  //! \param patchSize: An E_GEOMIPMAP_PATCH_SIZE enumeration defining the size of each patch of the terrain.
  //! \param position: The absolute position of this node.
  //! \param rotation: The absolute rotation of this node. ( NOT YET IMPLEMENTED )
  //! \param scale: The scale factor for the terrain.  If you're using a heightmap of size 128x128 and would like
  //! your terrain to be 12800x12800 in game units, then use a scale factor of ( vector ( 100.0f, 100.0f, 100.0f ).
  //! If you use a Y scaling factor of 0.0f, then your terrain will be flat.
  CTerrainSceneNode(ISceneNode* parent, ISceneManager* mgr, IFileSystem* fs, s32 id,
      s32 maxLOD = 4, E_TERRAIN_PATCH_SIZE patchSize = ETPS_17,
      const float3 position = float3(0.0f, 0.0f, 0.0f),
      const float3 rotation = float3(0.0f, 0.0f, 0.0f),
      const float3 scale = float3(1.0f, 1.0f, 1.0f));
  ~CTerrainSceneNode();
  //! Initializes the terrain data.  Loads the vertices from the heightMapFile.
  bool loadHeightMap(IReadFile* file,
      SColor vertexColor = SColor(255, 255, 255, 255), s32 smoothFactor = 0);
  //! Initializes the terrain data.  Loads the vertices from the heightMapFile.
  bool loadHeightMapRAW(IReadFile* file, s32 bitsPerPixel = 16,
      bool signedData = true, bool floatVals = false, s32 width = 0, SColor vertexColor = SColor(255, 255, 255, 255), s32 smoothFactor = 0);
  //! Returns the material based on the zero based index i. This scene node only uses
  //! 1 material.
  //! \param i: Zero based index i. UNUSED, left in for purposes.
  //! \return Returns the single material this scene node uses.
  SMaterial& getMaterial(u32 i);
  //! Returns amount of materials used by this scene node ( always 1 )
  //! \return Returns current count of materials used by this scene node ( always 1 )
  u32 getMaterialCount() const;
  //! Gets the last scaling factor applied to the scene node.  This value only represents the
  //! last scaling factor presented to the node.  For instance, if you make create the node
  //! with a scale factor of ( 1.0f, 1.0f, 1.0f ) then call setScale ( 50.0f, 5.0f, 50.0f ),
  //! then make another call to setScale with the values ( 2.0f, 2.0f, 2.0f ), this will return
  //! float3 ( 2.0f, 2.0f, 2.0f ), although the total scaling of the scene node is
  //! float3 ( 100.0f, 10.0f, 100.0f ).
  //! \return Returns the last scaling factor passed to the scene node.
  const float3 getScale() const {
    return TerrainData.Scale;
  }
  //! Scales the scene nodes vertices by the vector specified.
  //! \param scale: Scaling factor to apply to the node.
  void setScale(const float3 scale);
  //! Gets the last rotation factor applied to the scene node.
  //! \return Returns the last rotation factor applied to the scene node.
  const float3 getRotation() const {
    return TerrainData.Rotation;
  }
  //! Rotates the node. This only modifies the relative rotation of the node.
  //! \param rotation: New rotation of the node in degrees.
  void setRotation(const float3 rotation);
  //! Sets the pivot point for rotation of this node.
  //! NOTE: The default for the RotationPivot will be the center of the individual tile.
  void setRotationPivot(const float3 pivot);
  //! Gets the last positioning vector applied to the scene node.
  //! \return Returns the last position vector applied to the scene node.
  const float3 getPosition() const {
    return TerrainData.Position;
  }
  //! Moves the scene nodes vertices by the vector specified.
  //! \param newpos: Vector specifying how much to move each vertex of the scene node.
  void setPosition(const float3 newpos);
  //! Updates the scene nodes indices if the camera has moved or rotated by a certain
  //! threshold, which can be changed using the SetCameraMovementDeltaThreshold and
  //! SetCameraRotationDeltaThreshold functions.  This also determines if a given patch
  //! for the scene node is within the view frustum and if it's not the indices are not
  //! generated for that patch.
  void OnRegisterSceneNode();
  //! Render the scene node
  void render();
  //! Return the bounding box of the entire terrain.
  const F3AABBOX& getBoundingBox() const;
  //! Return the bounding box of a patch
  const F3AABBOX& getBoundingBox(s32 patchX, s32 patchZ) const;
  //! Return the number of indices currently used to draw the scene node.
  u32 getIndexCount() const {
    return IndicesToRender;
  }
  //! Returns the mesh
  IMesh* getMesh();
  //! Returns a pointer to the buffer used by the terrain (most users will not need this)
  IMeshBuffer* getRenderBuffer() {
    return RenderBuffer;
  }
  //! Gets the meshbuffer data based on a specified Level of Detail.
  //! \param mb: A reference to an IDynamicMeshBuffer object
  //! \param LOD: The Level Of Detail you want the indices from.
  void getMeshBufferForLOD(IDynamicMeshBuffer& mb, s32 LOD = 0) const;
  //! Gets the indices for a specified patch at a specified Level of Detail.
  //! \param indices: A reference to an array of u32 indices.
  //! \param patchX: Patch x coordinate.
  //! \param patchZ: Patch z coordinate.
  //! \param LOD: The level of detail to get for that patch.  If -1, then get
  //! the CurrentLOD.  If the CurrentLOD is set to -1, meaning it's not shown,
  //! then it will retrieve the triangles at the highest LOD (0).
  //! \return: Number of indices put into the buffer.
  s32 getIndicesForPatch(u32* indices,
      s32 patchX, s32 patchZ, s32 LOD = 0);
  //! Populates an array with the CurrentLOD of each patch.
  //! \param LODs: A reference to a s32> to hold the values
  //! \return Returns the number of elements in the array
  s32 getCurrentLODOfPatches(s32* LODs) const;
  //! Manually sets the LOD of a patch
  //! \param patchX: Patch x coordinate.
  //! \param patchZ: Patch z coordinate.
  //! \param LOD: The level of detail to set the patch to.
  void setLODOfPatch(s32 patchX, s32 patchZ, s32 LOD = 0);
  //! Returns center of terrain.
  const float3 getTerrainCenter() const {
    return TerrainData.Center;
  }
  //! Returns center of terrain.
  f32 getHeight(f32 x, f32 y) const;
  //! Sets the movement camera threshold which is used to determine when to recalculate
  //! indices for the scene node.  The default value is 10.0f.
  void setCameraMovementDelta(f32 delta) {
    CameraMovementDelta = delta;
  }
  //! Sets the rotation camera threshold which is used to determine when to recalculate
  //! indices for the scene node.  The default value is 1.0f.
  void setCameraRotationDelta(f32 delta) {
    CameraRotationDelta = delta;
  }
  //! Sets whether or not the node should dynamically update it its associated selector when
  //! the geomipmap data changes.
  //! param bVal: Boolean value representing whether or not to update selector dynamically.
  //! NOTE: Temporarily disabled while working out issues with DynamicSelectorUpdate
  void setDynamicSelectorUpdate(bool bVal) {
    DynamicSelectorUpdate = false;
  }
  //! Override the default generation of distance thresholds for determining the LOD a patch
  //! is rendered at. If any LOD is overridden, then the scene node will no longer apply
  //! scaling factors to these values. If you override these distances and then apply
  //! a scale to the scene node, it is your responsibility to update the new distances to
  //! work best with your new terrain size.
  bool overrideLODDistance(s32 LOD, f64 newDistance);
  //! Scales the two textures
  void scaleTexture(f32 scale = 1.0f, f32 scale2 = 0.0f);
  //! Returns type of the scene node
  ESCENE_NODE_TYPE getType() const {
    return ESNT_TERRAIN;
  }
  //! Writes attributes of the scene node.
  void serializeAttributes(IAttributes* out,
      SAttributeReadWriteOptions* options = 0) const;
  //! Reads attributes of the scene node.
  void deserializeAttributes(IAttributes* in,
      SAttributeReadWriteOptions* options = 0);
  //! Creates a clone of this scene node and its children.
  ISceneNode* clone(ISceneNode* newParent,
      ISceneManager* newManager);
private:
  friend class CTerrainTriangleSelector;
  struct SPatch {
    SPatch()
      : Top(0), Bottom(0), Right(0), Left(0), CurrentLOD(-1) {
    }
    SPatch* Top;
    SPatch* Bottom;
    SPatch* Right;
    SPatch* Left;
    s32 CurrentLOD;
    aabbox3df BoundingBox;
    float3 Center;
  };
  struct STerrainData {
    STerrainData(s32 patchSize, s32 maxLOD, const float3 position, const float3 rotation, const float3 scale)
      : Patches(0), Size(0), Position(position), Rotation(rotation),
        Scale(scale), PatchSize(patchSize), CalcPatchSize(patchSize - 1),
        PatchCount(0), MaxLOD(maxLOD) {
    }
    SPatch*   Patches;
    s32   Size;
    float3 Position;
    float3 Rotation;
    float3 RotationPivot;
    float3 Scale;
    float3 Center;
    s32   PatchSize;
    s32   CalcPatchSize;
    s32   PatchCount;
    s32   MaxLOD;
    aabbox3df BoundingBox;
    f64 > LODDistanceThreshold;
  };
  void preRenderCalculationsIfNeeded();
  void preRenderLODCalculations();
  void preRenderIndicesCalculations();
  //! get indices when generating index data for patches at varying levels of detail.
  u32 getIndex(const s32 PatchX, const s32 PatchZ, const s32 PatchIndex, u32 vX, u32 vZ) const;
  //! smooth the terrain
  void smoothTerrain(IDynamicMeshBuffer* mb, s32 smoothFactor);
  //! calculate smooth normals
  void calculateNormals(IDynamicMeshBuffer* mb);
  //! create patches, stuff that needs to only be done once for patches goes here.
  void createPatches();
  //! calculate the internal STerrainData structure
  void calculatePatchData();
  //! calculate or recalculate the distance thresholds
  void calculateDistanceThresholds(bool scalechanged = false);
  //! sets the CurrentLOD of all patches to the specified LOD
  void setCurrentLODOfPatches(s32 i);
  //! sets the CurrentLOD of TerrainData patches to the LODs specified in the array
  void setCurrentLODOfPatches(const s32* lodarray);
  //! Apply transformation changes( scale, position, rotation )
  void applyTransformation();
  STerrainData TerrainData;
  SMesh* Mesh;
  IDynamicMeshBuffer* RenderBuffer;
  u32 VerticesToRender;
  u32 IndicesToRender;
  bool DynamicSelectorUpdate;
  bool OverrideDistanceThreshold;
  bool UseDefaultRotationPivot;
  bool ForceRecalculation;
  float3 OldCameraPosition;
  float3 OldCameraRotation;
  float3 OldCameraUp;
  f32             OldCameraFOV;
  f32 CameraMovementDelta;
  f32 CameraRotationDelta;
  f32 CameraFOVDelta;
  // needed for (de)serialization
  f32 TCoordScale1;
  f32 TCoordScale2;
  s32 SmoothFactor;
  char* HeightmapFile;
  IFileSystem* FileSystem;
};
#endif // __C_TERRAIN_SCENE_NODE_H__

