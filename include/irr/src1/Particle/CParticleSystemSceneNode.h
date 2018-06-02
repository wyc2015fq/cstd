// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
#ifndef __C_PARTICLE_SYSTEM_SCENE_NODE_H_INCLUDED__
#define __C_PARTICLE_SYSTEM_SCENE_NODE_H_INCLUDED__
//#include "IParticleSystemSceneNode.h"
//#include "irrArray.h"
//#include "irrList.h"
//#include "SMeshBuffer.h"
//! A particle system scene node.
/** A scene node controlling a particle system. The behavior of the particles
can be controlled by setting the right particle emitters and affectors.
*/
class CParticleSystemSceneNode : public IParticleSystemSceneNode
{
public:
  //! constructor
  CParticleSystemSceneNode(bool createDefaultEmitter,
      ISceneNode* parent, ISceneManager* mgr, s32 id,
      const float3 position,
      const float3 rotation,
      const float3 scale);
  //! destructor
  ~CParticleSystemSceneNode();
  //! Gets the particle emitter, which creates the particles.
  IParticleEmitter* getEmitter();
  //! Sets the particle emitter, which creates the particles.
  void setEmitter(IParticleEmitter* emitter);
  //! Adds new particle affector to the particle system.
  void addAffector(IParticleAffector* affector);
  //! Get a list of all particle affectors.
  const list<IParticleAffector*>& getAffectors() const;
  //! Removes all particle affectors in the particle system.
  void removeAllAffectors();
  //! Returns the material based on the zero based index i.
  SMaterial& getMaterial(u32 i);
  //! Returns amount of materials used by this scene node.
  u32 getMaterialCount() const;
  //! pre render event
  void OnRegisterSceneNode();
  //! render
  void render();
  //! returns the axis aligned bounding box of this node
  const F3AABBOX& getBoundingBox() const;
  //! Creates a particle emitter for an animated mesh scene node
  IParticleAnimatedMeshSceneNodeEmitter* createAnimatedMeshSceneNodeEmitter(
      IAnimatedMeshSceneNode* node, bool useNormalDirection = true,
      const float3 direction = float3(0.0f, 0.03f, 0.0f),
      f32 normalDirectionModifier = 100.0f, s32 mbNumber = -1,
      bool everyMeshVertex = false, u32 minParticlesPerSecond = 5,
      u32 maxParticlesPerSecond = 10,
      const SColor& minStartColor = SColor(255, 0, 0, 0),
      const SColor& maxStartColor = SColor(255, 255, 255, 255),
      u32 lifeTimeMin = 2000, u32 lifeTimeMax = 4000,
      s32 maxAngleDegrees = 0,
      const dimension2df& minStartSize = dimension2df(5.0f, 5.0f),
      const dimension2df& maxStartSize = dimension2df(5.0f, 5.0f));
  //! Creates a box particle emitter.
  IParticleBoxEmitter* createBoxEmitter(
      const aabbox3df& box = F3AABBOX(-10, 0, -10, 5, 30, 10),
      const float3 direction = float3(0.0f, 0.03f, 0.0f),
      u32 minParticlesPerSecond = 5,
      u32 maxParticlesPerSecond = 10,
      const SColor& minStartColor = SColor(255, 0, 0, 0),
      const SColor& maxStartColor = SColor(255, 255, 255, 255),
      u32 lifeTimeMin = 2000, u32 lifeTimeMax = 4000,
      s32 maxAngleDegrees = 0,
      const dimension2df& minStartSize = dimension2df(5.0f, 5.0f),
      const dimension2df& maxStartSize = dimension2df(5.0f, 5.0f));
  //! Creates a particle emitter for emitting from a cylinder
  IParticleCylinderEmitter* createCylinderEmitter(
      const float3 center, f32 radius,
      const float3 normal, f32 length,
      bool outlineOnly = false, const float3 direction = float3(0.0f, 0.5f, 0.0f),
      u32 minParticlesPerSecond = 5, u32 maxParticlesPersSecond = 10,
      const SColor& minStartColor = SColor(255, 0, 0, 0),
      const SColor& maxStartColor = SColor(255, 255, 255, 255),
      u32 lifeTimeMin = 2000, u32 lifeTimeMax = 4000,
      s32 maxAngleDegrees = 0,
      const dimension2df& minStartSize = dimension2df(5.0f, 5.0f),
      const dimension2df& maxStartSize = dimension2df(5.0f, 5.0f));
  //! Creates a mesh particle emitter.
  IParticleMeshEmitter* createMeshEmitter(
      IMesh* mesh, bool useNormalDirection = true,
      const float3 direction = float3(0.0f, 0.03f, 0.0f),
      f32 normalDirectionModifier = 100.0f, s32 mbNumber = -1,
      bool everyMeshVertex = false,
      u32 minParticlesPerSecond = 5,
      u32 maxParticlesPerSecond = 10,
      const SColor& minStartColor = SColor(255, 0, 0, 0),
      const SColor& maxStartColor = SColor(255, 255, 255, 255),
      u32 lifeTimeMin = 2000, u32 lifeTimeMax = 4000,
      s32 maxAngleDegrees = 0,
      const dimension2df& minStartSize = dimension2df(5.0f, 5.0f),
      const dimension2df& maxStartSize = dimension2df(5.0f, 5.0f));
  //! Creates a point particle emitter.
  IParticlePointEmitter* createPointEmitter(
      const float3 direction = float3(0.0f, 0.03f, 0.0f),
      u32 minParticlesPerSecond = 5,
      u32 maxParticlesPerSecond = 10,
      const SColor& minStartColor = SColor(255, 0, 0, 0),
      const SColor& maxStartColor = SColor(255, 255, 255, 255),
      u32 lifeTimeMin = 2000, u32 lifeTimeMax = 4000,
      s32 maxAngleDegrees = 0,
      const dimension2df& minStartSize = dimension2df(5.0f, 5.0f),
      const dimension2df& maxStartSize = dimension2df(5.0f, 5.0f));
  //! Creates a ring particle emitter.
  IParticleRingEmitter* createRingEmitter(
      const float3 center, f32 radius, f32 ringThickness,
      const float3 direction = float3(0.0f, 0.03f, 0.0f),
      u32 minParticlesPerSecond = 5,
      u32 maxParticlesPerSecond = 10,
      const SColor& minStartColor = SColor(255, 0, 0, 0),
      const SColor& maxStartColor = SColor(255, 255, 255, 255),
      u32 lifeTimeMin = 2000, u32 lifeTimeMax = 4000,
      s32 maxAngleDegrees = 0,
      const dimension2df& minStartSize = dimension2df(5.0f, 5.0f),
      const dimension2df& maxStartSize = dimension2df(5.0f, 5.0f));
  //! Creates a sphere particle emitter.
  IParticleSphereEmitter* createSphereEmitter(
      const float3 center, f32 radius,
      const float3 direction = float3(0.0f, 0.03f, 0.0f),
      u32 minParticlesPerSecond = 5,
      u32 maxParticlesPerSecond = 10,
      const SColor& minStartColor = SColor(255, 0, 0, 0),
      const SColor& maxStartColor = SColor(255, 255, 255, 255),
      u32 lifeTimeMin = 2000, u32 lifeTimeMax = 4000,
      s32 maxAngleDegrees = 0,
      const dimension2df& minStartSize = dimension2df(5.0f, 5.0f),
      const dimension2df& maxStartSize = dimension2df(5.0f, 5.0f));
  //! Creates a point attraction affector. This affector modifies the positions of the
  //! particles and attracts them to a specified point at a specified speed per second.
  IParticleAttractionAffector* createAttractionAffector(
      const float3 point, f32 speed = 1.0f, bool attract = true,
      bool affectX = true, bool affectY = true, bool affectZ = true);
  //! Creates a scale particle affector.
  IParticleAffector* createScaleParticleAffector(const dimension2df& scaleTo = dimension2df(1.0f, 1.0f));
  //! Creates a fade out particle affector.
  IParticleFadeOutAffector* createFadeOutParticleAffector(
      const SColor& targetColor = SColor(0, 0, 0, 0),
      u32 timeNeededToFadeOut = 1000);
  //! Creates a gravity affector.
  IParticleGravityAffector* createGravityAffector(
      const float3 gravity = float3(0.0f, -0.03f, 0.0f),
      u32 timeForceLost = 1000);
  //! Creates a rotation affector. This affector rotates the particles
  //! around a specified pivot point. The speed is in Degrees per second.
  IParticleRotationAffector* createRotationAffector(
      const float3 speed = float3(5.0f, 5.0f, 5.0f),
      const float3 pivotPoint = float3(0.0f, 0.0f, 0.0f));
  //! Sets the size of all particles.
  void setParticleSize(
      const dimension2df& size = dimension2df(5.0f, 5.0f));
  //! Sets if the particles should be global. If they are, the particles are affected by
  //! the movement of the particle system scene node too, otherwise they completely
  //! ignore it. Default is true.
  void setParticlesAreGlobal(bool global = true);
  //! Remove all currently visible particles
  void clearParticles();
  //! Do manually update the particles.
  //! This should only be called when you want to render the node outside the scenegraph,
  //! as the node will care about this otherwise automatically.
  void doParticleSystem(u32 time);
  //! Writes attributes of the scene node.
  void serializeAttributes(IAttributes* out, SAttributeReadWriteOptions* options = 0) const;
  //! Reads attributes of the scene node.
  void deserializeAttributes(IAttributes* in, SAttributeReadWriteOptions* options = 0);
  //! Returns type of the scene node
  ESCENE_NODE_TYPE getType() const {
    return ESNT_PARTICLE_SYSTEM;
  }
private:
  void reallocateBuffers();
  list<IParticleAffector*> AffectorList;
  IParticleEmitter* Emitter;
  SParticle > Particles;
  dimension2df ParticleSize;
  u32 LastEmitTime;
  s32 MaxParticles;
  SMeshBuffer* Buffer;
  enum E_PARTICLES_PRIMITIVE {
    EPP_POINT = 0,
    EPP_BILLBOARD,
    EPP_POINTSPRITE
  };
  E_PARTICLES_PRIMITIVE ParticlePrimitive;
  bool ParticlesAreGlobal;
};
#endif

