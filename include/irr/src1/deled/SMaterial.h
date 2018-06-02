// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
#ifndef __S_MATERIAL_H_INCLUDED__
#define __S_MATERIAL_H_INCLUDED__


class ITexture;
//! Struct for holding parameters for a material renderer
class SMaterial
{
public:
  //! Default constructor. Creates a solid, lit material with white colors
  SMaterial()
    : MaterialType(EMT_SOLID), AmbientColor(255, 255, 255, 255), DiffuseColor(255, 255, 255, 255),
      EmissiveColor(0, 0, 0, 0), SpecularColor(255, 255, 255, 255),
      Shininess(0.0f), MaterialTypeParam(0.0f), MaterialTypeParam2(0.0f), Thickness(1.0f),
      ZBuffer(ECFN_LESSEQUAL), AntiAliasing(EAAM_SIMPLE), ColorMask(ECP_ALL),
      ColorMaterial(ECM_DIFFUSE), BlendOperation(EBO_NONE),
      PolygonOffsetFactor(0), PolygonOffsetDirection(EPO_FRONT),
      Wireframe(false), PointCloud(false), GouraudShading(true),
      Lighting(true), ZWriteEnable(true), BackfaceCulling(true), FrontfaceCulling(false),
      FogEnable(false), NormalizeNormals(false), UseMipMaps(true)
  { }
  //! Copy constructor
  /** \param other Material to copy from. */
  SMaterial(const SMaterial& other) {
    // These pointers are checked during assignment
    int i;

    for (i = 0; i < MATERIAL_MAX_TEXTURES; ++i) {
      TextureLayer[i].TextureMatrix = 0;
    }

    *this = other;
  }
  //! Assignment operator
  /** \param other Material to copy from. */
  SMaterial& operator=(const SMaterial& other) {
    // Check for self-assignment!
    if (this == &other) {
      return *this;
    }

    MaterialType = other.MaterialType;
    AmbientColor = other.AmbientColor;
    DiffuseColor = other.DiffuseColor;
    EmissiveColor = other.EmissiveColor;
    SpecularColor = other.SpecularColor;
    Shininess = other.Shininess;
    MaterialTypeParam = other.MaterialTypeParam;
    MaterialTypeParam2 = other.MaterialTypeParam2;
    Thickness = other.Thickness;
    int i;

    for (i = 0; i < MATERIAL_MAX_TEXTURES; ++i) {
      TextureLayer[i] = other.TextureLayer[i];
    }

    Wireframe = other.Wireframe;
    PointCloud = other.PointCloud;
    GouraudShading = other.GouraudShading;
    Lighting = other.Lighting;
    ZWriteEnable = other.ZWriteEnable;
    BackfaceCulling = other.BackfaceCulling;
    FrontfaceCulling = other.FrontfaceCulling;
    FogEnable = other.FogEnable;
    NormalizeNormals = other.NormalizeNormals;
    ZBuffer = other.ZBuffer;
    AntiAliasing = other.AntiAliasing;
    ColorMask = other.ColorMask;
    ColorMaterial = other.ColorMaterial;
    BlendOperation = other.BlendOperation;
    PolygonOffsetFactor = other.PolygonOffsetFactor;
    PolygonOffsetDirection = other.PolygonOffsetDirection;
    UseMipMaps = other.UseMipMaps;
    return *this;
  }
  //! Gets the texture transformation matrix for level i
  /** \param i The desired level. Must not be larger than MATERIAL_MAX_TEXTURES.
  \return Texture matrix for texture level i. */
  matrix4& getTextureMatrix(u32 i) {
    return TextureLayer[i].getTextureMatrix();
  }
  //! Gets the immutable texture transformation matrix for level i
  /** \param i The desired level.
  \return Texture matrix for texture level i, or identity matrix for levels larger than MATERIAL_MAX_TEXTURES. */
  const matrix4& getTextureMatrix(u32 i) const {
    if (i < MATERIAL_MAX_TEXTURES) {
      return TextureLayer[i].getTextureMatrix();
    }
    else {
      return IdentityMatrix;
    }
  }
  //! Sets the i-th texture transformation matrix
  /** \param i The desired level.
  \param mat Texture matrix for texture level i. */
  void setTextureMatrix(u32 i, const matrix4& mat) {
    if (i >= MATERIAL_MAX_TEXTURES) {
      return;
    }

    TextureLayer[i].setTextureMatrix(mat);
  }
  //! Gets the i-th texture
  /** \param i The desired level.
  \return Texture for texture level i, if defined, else 0. */
  ITexture* getTexture(u32 i) const {
    return i < MATERIAL_MAX_TEXTURES ? TextureLayer[i].Texture : 0;
  }
  //! Sets the i-th texture
  /** If i>=MATERIAL_MAX_TEXTURES this setting will be ignored.
  \param i The desired level.
  \param tex Texture for texture level i. */
  void setTexture(u32 i, ITexture* tex) {
    if (i >= MATERIAL_MAX_TEXTURES) {
      return;
    }

    TextureLayer[i].Texture = tex;
  }
  //! Sets the Material flag to the given value
  /** \param flag The flag to be set.
  \param value The new value for the flag. */
  void setFlag(E_MATERIAL_FLAG flag, bool value) {
      int i;
    switch (flag) {
    case EMF_WIREFRAME:
      Wireframe = value;
      break;

    case EMF_POINTCLOUD:
      PointCloud = value;
      break;

    case EMF_GOURAUD_SHADING:
      GouraudShading = value;
      break;

    case EMF_LIGHTING:
      Lighting = value;
      break;

    case EMF_ZBUFFER:
      ZBuffer = value;
      break;

    case EMF_ZWRITE_ENABLE:
      ZWriteEnable = value;
      break;

    case EMF_BACK_FACE_CULLING:
      BackfaceCulling = value;
      break;

    case EMF_FRONT_FACE_CULLING:
      FrontfaceCulling = value;
      break;

    case EMF_BILINEAR_FILTER: {

      for (i = 0; i < MATERIAL_MAX_TEXTURES; ++i) {
        TextureLayer[i].BilinearFilter = value;
      }
    }
    break;

    case EMF_TRILINEAR_FILTER: {

      for (i = 0; i < MATERIAL_MAX_TEXTURES; ++i) {
        TextureLayer[i].TrilinearFilter = value;
      }
    }
    break;

	case EMF_ANISOTROPIC_FILTER: {
		if (value) {
			for (i = 0; i < MATERIAL_MAX_TEXTURES; ++i) {
				TextureLayer[i].AnisotropicFilter = 0xFF;
			}
		}
		else {
			for (i = 0; i < MATERIAL_MAX_TEXTURES; ++i) {
				TextureLayer[i].AnisotropicFilter = 0;
			}
      }
    }
    break;

    case EMF_FOG_ENABLE:
      FogEnable = value;
      break;

    case EMF_NORMALIZE_NORMALS:
      NormalizeNormals = value;
      break;

    case EMF_TEXTURE_WRAP: {

      for (i = 0; i < MATERIAL_MAX_TEXTURES; ++i) {
        TextureLayer[i].TextureWrapU = (E_TEXTURE_CLAMP)value;
        TextureLayer[i].TextureWrapV = (E_TEXTURE_CLAMP)value;
      }
    }
    break;

    case EMF_ANTI_ALIASING:
      AntiAliasing = value ? EAAM_SIMPLE : EAAM_OFF;
      break;

    case EMF_COLOR_MASK:
      ColorMask = value ? ECP_ALL : ECP_NONE;
      break;

    case EMF_COLOR_MATERIAL:
      ColorMaterial = value ? ECM_DIFFUSE : ECM_NONE;
      break;

    case EMF_USE_MIP_MAPS:
      UseMipMaps = value;
      break;

    case EMF_BLEND_OPERATION:
      BlendOperation = value ? EBO_ADD : EBO_NONE;
      break;

    case EMF_POLYGON_OFFSET:
      PolygonOffsetFactor = value ? 1 : 0;
      PolygonOffsetDirection = EPO_BACK;
      break;

    default:
      break;
    }
  }
  //! Gets the Material flag
  /** \param flag The flag to query.
  \return The current value of the flag. */
  bool getFlag(E_MATERIAL_FLAG flag) const {
    switch (flag) {
    case EMF_WIREFRAME:
      return Wireframe;

    case EMF_POINTCLOUD:
      return PointCloud;

    case EMF_GOURAUD_SHADING:
      return GouraudShading;

    case EMF_LIGHTING:
      return Lighting;

    case EMF_ZBUFFER:
      return ZBuffer != ECFN_NEVER;

    case EMF_ZWRITE_ENABLE:
      return ZWriteEnable;

    case EMF_BACK_FACE_CULLING:
      return BackfaceCulling;

    case EMF_FRONT_FACE_CULLING:
      return FrontfaceCulling;

    case EMF_BILINEAR_FILTER:
      return TextureLayer[0].BilinearFilter;

    case EMF_TRILINEAR_FILTER:
      return TextureLayer[0].TrilinearFilter;

    case EMF_ANISOTROPIC_FILTER:
      return TextureLayer[0].AnisotropicFilter != 0;

    case EMF_FOG_ENABLE:
      return FogEnable;

    case EMF_NORMALIZE_NORMALS:
      return NormalizeNormals;

    case EMF_TEXTURE_WRAP:
      return !(TextureLayer[0].TextureWrapU ||
          TextureLayer[0].TextureWrapV ||
          TextureLayer[1].TextureWrapU ||
          TextureLayer[1].TextureWrapV ||
          TextureLayer[2].TextureWrapU ||
          TextureLayer[2].TextureWrapV ||
          TextureLayer[3].TextureWrapU ||
          TextureLayer[3].TextureWrapV);

    case EMF_ANTI_ALIASING:
      return (AntiAliasing == 1);

    case EMF_COLOR_MASK:
      return (ColorMask != ECP_NONE);

    case EMF_COLOR_MATERIAL:
      return (ColorMaterial != ECM_NONE);

    case EMF_USE_MIP_MAPS:
      return UseMipMaps;

    case EMF_BLEND_OPERATION:
      return BlendOperation != EBO_NONE;

    case EMF_POLYGON_OFFSET:
      return PolygonOffsetFactor != 0;
    }

    return false;
  }
  //! Inequality operator
  /** \param b Material to compare to.
  \return True if the materials differ, else false. */
  CV_INLINE bool operator!=(const SMaterial& b) const {
    bool different =
        MaterialType != b.MaterialType ||
        AmbientColor != b.AmbientColor ||
        DiffuseColor != b.DiffuseColor ||
        EmissiveColor != b.EmissiveColor ||
        SpecularColor != b.SpecularColor ||
        Shininess != b.Shininess ||
        MaterialTypeParam != b.MaterialTypeParam ||
        MaterialTypeParam2 != b.MaterialTypeParam2 ||
        Thickness != b.Thickness ||
        Wireframe != b.Wireframe ||
        PointCloud != b.PointCloud ||
        GouraudShading != b.GouraudShading ||
        Lighting != b.Lighting ||
        ZBuffer != b.ZBuffer ||
        ZWriteEnable != b.ZWriteEnable ||
        BackfaceCulling != b.BackfaceCulling ||
        FrontfaceCulling != b.FrontfaceCulling ||
        FogEnable != b.FogEnable ||
        NormalizeNormals != b.NormalizeNormals ||
        AntiAliasing != b.AntiAliasing ||
        ColorMask != b.ColorMask ||
        ColorMaterial != b.ColorMaterial ||
        BlendOperation != b.BlendOperation ||
        PolygonOffsetFactor != b.PolygonOffsetFactor ||
        PolygonOffsetDirection != b.PolygonOffsetDirection ||
        UseMipMaps != b.UseMipMaps;
    int i;

    for (i = 0; (i < MATERIAL_MAX_TEXTURES) && !different; ++i) {
      different |= (TextureLayer[i] != b.TextureLayer[i]);
    }

    return different;
  }
  //! Equality operator
  /** \param b Material to compare to.
  \return True if the materials are equal, else false. */
  CV_INLINE bool operator==(const SMaterial& b) const {
    return !(b != *this);
  }
  bool isTransparent() const {
    return MaterialType == EMT_TRANSPARENT_ADD_COLOR ||
        MaterialType == EMT_TRANSPARENT_ALPHA_CHANNEL ||
        MaterialType == EMT_TRANSPARENT_VERTEX_ALPHA ||
        MaterialType == EMT_TRANSPARENT_REFLECTION_2_LAYER;
  }
};
//! global const identity Material



#endif

