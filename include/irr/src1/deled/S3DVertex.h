// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
#ifndef __S_3D_VERTEX_H_INCLUDED__
#define __S_3D_VERTEX_H_INCLUDED__
//#include "vector3d.h"
//#include "vector2d.h"


#if 0
//! default constructor
S3DVertex() {}
//! constructor
S3DVertex(f32 x, f32 y, f32 z, f32 nx, f32 ny, f32 nz, SColor c, f32 tu, f32 tv)
: Pos(x, y, z), Normal(nx, ny, nz), Color(c), TCoords(tu, tv) {}
//! constructor
S3DVertex(const float3 pos, const float3 normal,
          SColor color, const vector2d<f32>& tcoords)
    : Pos(pos), Normal(normal), Color(color), TCoords(tcoords) {}

  bool operator==(const S3DVertex& other) const {
    return ((Pos == other.Pos) && (Normal == other.Normal) &&
        (Color == other.Color) && (TCoords == other.TCoords));
  }
  bool operator!=(const S3DVertex& other) const {
    return ((Pos != other.Pos) || (Normal != other.Normal) ||
        (Color != other.Color) || (TCoords != other.TCoords));
  }
  bool operator<(const S3DVertex& other) const {
    return ((Pos < other.Pos) ||
        ((Pos == other.Pos) && (Normal < other.Normal)) ||
        ((Pos == other.Pos) && (Normal == other.Normal) && (Color < other.Color)) ||
        ((Pos == other.Pos) && (Normal == other.Normal) && (Color == other.Color) && (TCoords < other.TCoords)));
  }
  E_VERTEX_TYPE getType() const {
    return EVT_STANDARD;
  }
  S3DVertex getInterpolated(const S3DVertex& other, f32 d) {
    d = clamp(d, 0.0f, 1.0f);
    return S3DVertex(Pos.getInterpolated(other.Pos, d),
        Normal.getInterpolated(other.Normal, d),
        Color.getInterpolated(other.Color, d),
        TCoords.getInterpolated(other.TCoords, d));
  }
//! Vertex with two texture coordinates.
/** Usually used for geometry with lightmaps
or other special materials.
*/
  //! default constructor
  //! constructor with two different texture coords, but no normal
  S3DVertex(f32 x, f32 y, f32 z, SColor c, f32 tu, f32 tv, f32 tu2, f32 tv2)
    : S3DVertex(x, y, z, 0.0f, 0.0f, 0.0f, c, tu, tv), TCoords2(tu2, tv2) {}
  //! constructor with two different texture coords, but no normal
  S3DVertex(const float3 pos, SColor color,
      const vector2d<f32>& tcoords, const vector2d<f32>& tcoords2)
    : S3DVertex(pos, float3(), color, tcoords), TCoords2(tcoords2) {}
  //! constructor with all values
  S3DVertex(const float3 pos, const float3 normal, const SColor& color,
      const vector2d<f32>& tcoords, const vector2d<f32>& tcoords2)
    : S3DVertex(pos, normal, color, tcoords), TCoords2(tcoords2) {}
  //! constructor with all values
  S3DVertex(f32 x, f32 y, f32 z, f32 nx, f32 ny, f32 nz, SColor c, f32 tu, f32 tv, f32 tu2, f32 tv2)
    : S3DVertex(x, y, z, nx, ny, nz, c, tu, tv), TCoords2(tu2, tv2) {}
  //! constructor with the same texture coords and normal
  S3DVertex(f32 x, f32 y, f32 z, f32 nx, f32 ny, f32 nz, SColor c, f32 tu, f32 tv)
    : S3DVertex(x, y, z, nx, ny, nz, c, tu, tv), TCoords2(tu, tv) {}
  //! constructor with the same texture coords and normal
  S3DVertex(const float3 pos, const float3 normal,
      SColor color, const vector2d<f32>& tcoords)
    : S3DVertex(pos, normal, color, tcoords), TCoords2(tcoords) {}
  //! constructor from S3DVertex
  //! Second set of texture coordinates
  //! Equality operator
  bool operator==(const S3DVertex& other) const {
    return ((static_cast<S3DVertex>(*this) == other) &&
        (TCoords2 == other.TCoords2));
  }
  //! Inequality operator
  bool operator!=(const S3DVertex& other) const {
    return ((static_cast<S3DVertex>(*this) != other) ||
        (TCoords2 != other.TCoords2));
  }
  bool operator<(const S3DVertex& other) const {
    return ((static_cast<S3DVertex>(*this) < other) ||
        ((static_cast<S3DVertex>(*this) == other) && (TCoords2 < other.TCoords2)));
  }
  E_VERTEX_TYPE getType() const {
    return EVT_2TCOORDS;
  }
  S3DVertex getInterpolated(const S3DVertex& other, f32 d) {
    d = clamp(d, 0.0f, 1.0f);
    return S3DVertex(Pos.getInterpolated(other.Pos, d),
        Normal.getInterpolated(other.Normal, d),
        Color.getInterpolated(other.Color, d),
        TCoords.getInterpolated(other.TCoords, d),
        TCoords2.getInterpolated(other.TCoords2, d));
  }
//! Vertex with a tangent and binormal vector.
/** Usually used for tangent space normal mapping. */
  //! default constructor
  //! constructor
  S3DVertex(f32 x, f32 y, f32 z, f32 nx = 0.0f, f32 ny = 0.0f, f32 nz = 0.0f,
      SColor c = 0xFFFFFFFF, f32 tu = 0.0f, f32 tv = 0.0f,
      f32 tanx = 0.0f, f32 tany = 0.0f, f32 tanz = 0.0f,
      f32 bx = 0.0f, f32 by = 0.0f, f32 bz = 0.0f)
    : S3DVertex(x, y, z, nx, ny, nz, c, tu, tv), Tangent(tanx, tany, tanz), Binormal(bx, by, bz) { }
  //! constructor
  S3DVertex(const float3 pos, SColor c,
      const vector2df& tcoords)
    : S3DVertex(pos, float3(), c, tcoords) { }
  //! constructor
  S3DVertex(const float3 pos,
      const float3 normal, SColor c,
      const vector2df& tcoords,
      const float3 tangent = float3(),
      const float3 binormal = float3())
    : S3DVertex(pos, normal, c, tcoords), Tangent(tangent), Binormal(binormal) { }
  bool operator==(const S3DVertex& other) const {
    return ((static_cast<S3DVertex>(*this) == other) &&
        (Tangent == other.Tangent) &&
        (Binormal == other.Binormal));
  }
  bool operator!=(const S3DVertex& other) const {
    return ((static_cast<S3DVertex>(*this) != other) ||
        (Tangent != other.Tangent) ||
        (Binormal != other.Binormal));
  }
  bool operator<(const S3DVertex& other) const {
    return ((static_cast<S3DVertex>(*this) < other) ||
        ((static_cast<S3DVertex>(*this) == other) && (Tangent < other.Tangent)) ||
        ((static_cast<S3DVertex>(*this) == other) && (Tangent == other.Tangent) && (Binormal < other.Binormal)));
  }
  E_VERTEX_TYPE getType() const {
    return EVT_TANGENTS;
  }
  S3DVertex getInterpolated(const S3DVertex& other, f32 d) {
    d = clamp(d, 0.0f, 1.0f);
    return S3DVertex(Pos.getInterpolated(other.Pos, d),
        Normal.getInterpolated(other.Normal, d),
        Color.getInterpolated(other.Color, d),
        TCoords.getInterpolated(other.TCoords, d),
        Tangent.getInterpolated(other.Tangent, d),
        Binormal.getInterpolated(other.Binormal, d));
  }
};
#endif


#endif

