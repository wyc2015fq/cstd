// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
#ifndef __S_MATERIAL_LAYER_H_INCLUDED__
#define __S_MATERIAL_LAYER_H_INCLUDED__


class ITexture;
//! Struct for holding material parameters which exist per texture layer
class SMaterialLayer
{
public:
  //! Default constructor
  SMaterialLayer()
    : Texture(0),
      TextureWrapU(ETC_REPEAT),
      TextureWrapV(ETC_REPEAT),
      BilinearFilter(true),
      TrilinearFilter(false),
      AnisotropicFilter(0),
      LODBias(0),
      TextureMatrix(0)
  {}
  //! Copy constructor
  /** \param other Material layer to copy from. */
  SMaterialLayer(const SMaterialLayer& other) {
    // This pointer is checked during assignment
    TextureMatrix = 0;
    *this = other;
  }
  //! Destructor
  ~SMaterialLayer() {
    MatrixAllocator.destruct(TextureMatrix);
    MatrixAllocator.deallocate(TextureMatrix);
  }
  //! Assignment operator
  /** \param other Material layer to copy from.
  \return This material layer, updated. */
  SMaterialLayer& operator=(const SMaterialLayer& other) {
    // Check for self-assignment!
    if (this == &other) {
      return *this;
    }

    Texture = other.Texture;

    if (TextureMatrix) {
      if (other.TextureMatrix) {
        *TextureMatrix = *other.TextureMatrix;
      }
      else {
        MatrixAllocator.destruct(TextureMatrix);
        MatrixAllocator.deallocate(TextureMatrix);
        TextureMatrix = 0;
      }
    }
    else {
      if (other.TextureMatrix) {
        TextureMatrix = MatrixAllocator.allocate(1);
        MatrixAllocator.construct(TextureMatrix, *other.TextureMatrix);
      }
      else {
        TextureMatrix = 0;
      }
    }

    TextureWrapU = other.TextureWrapU;
    TextureWrapV = other.TextureWrapV;
    BilinearFilter = other.BilinearFilter;
    TrilinearFilter = other.TrilinearFilter;
    AnisotropicFilter = other.AnisotropicFilter;
    LODBias = other.LODBias;
    return *this;
  }
  //! Gets the texture transformation matrix
  /** \return Texture matrix of this layer. */
  matrix4& getTextureMatrix() {
    if (!TextureMatrix) {
      TextureMatrix = MatrixAllocator.allocate(1);
      MatrixAllocator.construct(TextureMatrix, IdentityMatrix);
    }

    return *TextureMatrix;
  }
  //! Gets the immutable texture transformation matrix
  /** \return Texture matrix of this layer. */
  const matrix4& getTextureMatrix() const {
    if (TextureMatrix) {
      return *TextureMatrix;
    }
    else {
      return IdentityMatrix;
    }
  }
  //! Sets the texture transformation matrix to mat
  /** \param mat New texture matrix for this layer. */
  void setTextureMatrix(const matrix4& mat) {
    if (!TextureMatrix) {
      TextureMatrix = MatrixAllocator.allocate(1);
      MatrixAllocator.construct(TextureMatrix, mat);
    }
    else {
      *TextureMatrix = mat;
    }
  }
  //! Inequality operator
  /** \param b Layer to compare to.
  \return True if layers are different, else false. */
  CV_INLINE bool operator!=(const SMaterialLayer& b) const {
    bool different =
        Texture != b.Texture ||
        TextureWrapU != b.TextureWrapU ||
        TextureWrapV != b.TextureWrapV ||
        BilinearFilter != b.BilinearFilter ||
        TrilinearFilter != b.TrilinearFilter ||
        AnisotropicFilter != b.AnisotropicFilter ||
        LODBias != b.LODBias;

    if (different) {
      return true;
    }
    else
      different |= (TextureMatrix != b.TextureMatrix) &&
          TextureMatrix && b.TextureMatrix &&
          (*TextureMatrix != *(b.TextureMatrix));

    return different;
  }
  //! Equality operator
  /** \param b Layer to compare to.
  \return True if layers are equal, else false. */
  CV_INLINE bool operator==(const SMaterialLayer& b) const {
    return !(b != *this);
  }
private:
  friend class SMaterial;
  irrAllocator<matrix4> MatrixAllocator;
};


#endif // __S_MATERIAL_LAYER_H_INCLUDED__

