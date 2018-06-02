/*
 * Copyright (c) 2009 Samit Basu
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */
#ifndef __Array_hpp__
#define __Array_hpp__

#include <QSharedData>
#include "Types.h"
#include "BasicArray.h"
#include "IndexArray.h"
#include "FastList.h"
#include "SparseMatrix.h"
#include "Exception.h"
class StringArray;
class StructArray;
class mxArray;

typedef FastList<mxArray> ArrayVector;
typedef QVector<ArrayVector> ArrayMatrix;

template <typename V>
inline DataClass GetDataClass(V c);

size_t ByteSizeOfDataClass(DataClass);

typedef struct {
  int Class : 5;
  unsigned Scalar : 1;
  unsigned Complex : 1;
  unsigned Sparse : 1;
} Type;

class SharedObject : public QSharedData
{
  Type m_type;
  void* m_p;
public:
  SharedObject(Type t, void* p);
  SharedObject(const SharedObject& copy);
  SharedObject& operator=(const SharedObject& copy);
  ~SharedObject();
  inline void* ptr() const {
    return m_p;
  }
  inline Type type() const {
    return m_type;
  }
};

typedef struct {
  QSharedDataPointer<SharedObject> p;
  union {
    BOOL Bool;
    int8 Int8;
    uint8 UInt8;
    int16 Int16;
    uint16 UInt16;
    int32 Int32;
    uint32 UInt32;
    int64 Int64;
    uint64 UInt64;
    float Float;
    double Double;
  };
} Data;

class mxArray
{
public:
  inline mxArray() {
    m_real.Double = 0;
    m_imag.Double = 0;
    m_type.Class = Double;
    m_type.Complex = 0;
    m_type.Sparse = 0;
    m_type.Scalar = 1;
  }
  // Defined in ArrayPrivate
  template <typename T> inline explicit mxArray(T real);
  template <typename T> static inline mxArray create(T real) {
    return mxArray(real);
  }

  template <typename T> inline explicit mxArray(T real, T imag);
  template <typename T> static inline mxArray create(T real, T imag) {
    return mxArray(real, imag);
  }

  template <typename T>
  inline mxArray(const mxArray* r) {
    m_type.Class = GetDataClass<T>(T(0));
    m_type.Complex = 0;
    m_type.Sparse = 0;
    m_type.Scalar = 0;
    m_real.p = new SharedObject(m_type, new BasicArray<T>(r));
  }

  template <typename T> static inline mxArray create(const mxArray* r) {
    return mxArray(r);
  }

  template <typename T>
  inline mxArray(const mxArray* r, const mxArray* i) {
    m_type.Class = GetDataClass<T>(T(0));
    m_type.Complex = 1;
    m_type.Sparse = 0;
    m_type.Scalar = 0;
    m_real.p = new SharedObject(m_type, new BasicArray<T>(r));
    m_imag.p = new SharedObject(m_type, new BasicArray<T>(i));
  }
  template <typename T> static inline mxArray create(const mxArray* r, const mxArray* i) {
    return mxArray(r, i);
  }

  template <typename T>
  inline mxArray(BasicArray<T>* r) {
    m_type.Class = GetDataClass<T>(T(0));
    m_type.Complex = 0;
    m_type.Sparse = 0;
    m_type.Scalar = 0;
    m_real.p = new SharedObject(m_type, r);
  }
  template <typename T>  static inline mxArray create(BasicArray<T>* r) {
    return mxArray(r);
  }


  template <typename T>
  inline mxArray(BasicArray<T>* r, BasicArray<T>* i) {
    m_type.Class = GetDataClass<T>(T(0));
    m_type.Complex = 1;
    m_type.Sparse = 0;
    m_type.Scalar = 0;
    m_real.p = new SharedObject(m_type, r);
    m_imag.p = new SharedObject(m_type, i);
  }
  template <typename T> static inline mxArray create(BasicArray<T>* r, BasicArray<T>* i) {
    return mxArray(r, i);
  }


  mxArray(DataClass t, const NTuple& dims = NTuple(0, 0));
  static inline mxArray create(DataClass t, const NTuple& dims = NTuple(0, 0)) {
    return mxArray(t, dims);
  }

  explicit mxArray(const QChar& t);
  explicit mxArray(const QChar&, const QChar&);
  explicit mxArray(const QString& text);
  template <typename T>
  inline mxArray(const SparseMatrix<T>& real) {
    m_type.Class = GetDataClass<T>(T(0));
    m_type.Complex = 0;
    m_type.Sparse = 1;
    m_type.Scalar = 0;
    m_real.p = new SharedObject(m_type, new SparseMatrix<T>(real));
  }
  template <typename T>
  mxArray(const SparseMatrix<T>& real, const SparseMatrix<T>& imag) {
    m_type.Class = GetDataClass<T>(T(0));
    m_type.Complex = 1;
    m_type.Sparse = 1;
    m_type.Scalar = 0;
    m_real.p = new SharedObject(m_type, new SparseMatrix<T>(real));
    m_imag.p = new SharedObject(m_type, new SparseMatrix<T>(imag));
  }
  mxArray(const StructArray& real);
  static mxArray scalarConstructor(DataClass t) {
    mxArray ret;
    ret.m_type.Class = t;
    ret.m_type.Complex = 0;
    ret.m_type.Sparse = 0;
    ret.m_type.Scalar = 1;
    return ret;
  }
  const NTuple dimensions() const;
  const index_t length() const {
    return dimensions().count();
  }
  const index_t rows() const {
    return dimensions()[0];
  }
  const index_t columns() const {
    return dimensions()[1];
  }
  const index_t cols() const {
    return dimensions()[1];
  }
  inline const Type type() const {
    return m_type;
  }
  inline const DataClass dataClass() const {
    return DataClass(m_type.Class);
  }
  QString className() const;
  BOOL mxIsUserClass() const;
  inline BOOL isArray() const {
    return (m_type.Scalar == 0);
  }
  inline BOOL isVector() const {
    return dimensions().isVector();
  }
  inline BOOL isColumnVector() const {
    return dimensions().isColumnVector();
  }
  inline BOOL isRowVector() const {
    return dimensions().isRowVector();
  }
  inline BOOL is2D() const {
    return dimensions().is2D();
  }
  inline BOOL isSquare() const {
    return (is2D() && (rows() == columns()));
  }
  inline BOOL isString() const {
    return dataClass() ==  StringArray;
  }
  inline BOOL isSparse() const {
    return m_type.Sparse == 1;
  }
  inline BOOL isReferenceType() const {
    return ((dataClass() == Invalid) || (dataClass() == CellArray) ||
        (dataClass() == Struct));
  }
  QString asString() const;
  int asInteger() const;
  double asDouble() const;
  inline BOOL isDouble() const {
    return dataClass() == Double;
  }
  inline BOOL isScalar() const {
    return ((m_type.Scalar == 1) || dimensions() mxIsScalar());
  }
  template <typename T>
  inline mxArray* real() {
    if (!m_real.p) {
      throw_Exception("Illegal request for real array part of scalar - this is bug!");
    }

    if (m_type.Class != GetDataClass(T(0))) {
      throw_Exception("data type mismatch in array -- bug");
    }

    if (isSparse()) {
      throw_Exception("Illegal request for (non-sparse) real part of sparse array -- this is an internal bug!");
    }

    return (*reinterpret_cast<BasicArray<T>*>(m_real.p->ptr()));
  }
  inline const StructArray& constStructPtr() const {
    if (!m_real.p) {
      throw_Exception("Illegal request for real part of undefined structure array - this is bug!");
    }

    return (*reinterpret_cast<const StructArray*>(m_real.p->ptr()));
  }
  inline StructArray& structPtr() {
    if (!m_real.p) {
      throw_Exception("Illegal request for real part of undefined structure array - this is bug!");
    }

    return (*reinterpret_cast<StructArray*>(m_real.p->ptr()));
  }
  template <typename T>
  inline const SparseMatrix<T>& constRealSparse() const {
    if (!m_real.p) {
      throw_Exception("Illegal request for (sparse) real part of scalar - this is bug!");
    }

    if (m_type.Class != GetDataClass(T(0))) {
      throw_Exception("data type mismatch in array -- bug");
    }

    if (!isSparse()) {
      throw_Exception("Illegal request for (sparse) real part of non-sparse array -- this is an internal bug!");
    }

    return (*reinterpret_cast<const SparseMatrix<T> *>(m_real.p->ptr()));
  }
  template <typename T>
  inline const SparseMatrix<T>& constImagSparse() const {
    if (!m_imag.p) {
      throw_Exception("Illegal request for (sparse) imaginary part of real-only array");
    }

    if (m_type.Class != GetDataClass(T(0))) {
      throw_Exception("data type mismatch in array -- bug");
    }

    if (!isSparse()) {
      throw_Exception("Illegal request for (sparse) real part of non-sparse array -- this is an internal bug!");
    }

    return (*reinterpret_cast<const SparseMatrix<T> *>(m_imag.p->ptr()));
  }
  template <typename T>
  inline SparseMatrix<T>& realSparse() {
    if (!m_real.p) {
      throw_Exception("Illegal request for (sparse) real part of scalar - this is bug!");
    }

    if (m_type.Class != GetDataClass(T(0))) {
      throw_Exception("data type mismatch in array -- bug");
    }

    if (!isSparse()) {
      throw_Exception("Illegal request for (sparse) real part of non-sparse array -- this is an internal bug!");
    }

    return (*reinterpret_cast<SparseMatrix<T> *>(m_real.p->ptr()));
  }
  template <typename T>
  inline SparseMatrix<T>& imagSparse() {
    if (!m_imag.p) {
      m_imag.p = new SharedObject(m_type, new SparseMatrix<T>(dimensions()));
      m_type.Complex = 1;
    }

    if (m_type.Class != GetDataClass(T(0))) {
      throw_Exception("data type mismatch in array -- bug");
    }

    if (!isSparse()) {
      throw_Exception("Illegal request for (sparse) real part of non-sparse array -- this is an internal bug!");
    }

    return (*reinterpret_cast<SparseMatrix<T> *>(m_imag.p->ptr()));
  }
  template <typename T>
  inline const mxArray* constReal() const {
    if (!m_real.p) {
      throw_Exception("Illegal request for real part of scalar - this is bug!");
    }

    if (m_type.Class != GetDataClass(T(0))) {
      throw_Exception("data type mismatch in array -- bug");
    }

    if (isSparse()) {
      throw_Exception("Illegal request for (non-sparse) real part of sparse array -- this is an internal bug!");
    }

    return (*reinterpret_cast<const BasicArray<T>*>(m_real.p->ptr()));
  }
  template <typename T>
  inline mxArray* imag() {
    if (!m_imag.p) {
      m_imag.p = new SharedObject(m_type, new BasicArray<T>(dimensions()));
      m_type.Complex = TRUE;
    }

    if (m_type.Class != GetDataClass(T(0))) {
      throw_Exception("data type mismatch in array -- bug");
    }

    if (isSparse()) {
      throw_Exception("Illegal request for (non-sparse) real part of sparse array -- this is an internal bug!");
    }

    return (*reinterpret_cast<BasicArray<T>*>(m_imag.p->ptr()));
  }
  template <typename T>
  inline const mxArray* constImag() const {
    if (!m_imag.p) {
      throw_Exception("Illegal request for imaginary part of real-only array");
    }

    if (m_type.Class != GetDataClass(T(0))) {
      throw_Exception("data type mismatch in array -- bug");
    }

    if (isSparse()) {
      throw_Exception("Illegal request for (non-sparse) imaginary part of sparse array -- this is an internal bug!");
    }

    return (*reinterpret_cast<const BasicArray<T>*>(m_imag.p->ptr()));
  }
  template <typename T>
  inline BasicArray<T> fortran() const {
    if (m_type.Class != GetDataClass(T(0))) {
      throw_Exception("data type mismatch in array -- bug");
    }

    if (isSparse()) {
      throw_Exception("Illegal request for (non-sparse) real and imaginary part of sparse array -- this is an internal bug!");
    }

    if (m_imag.p) {
      return MergeComplex(constReal<T>(), constImag<T>());
    }
    else {
      return MergeComplex(constReal<T>());
    }
  }

  inline BOOL allReal() const {
    return (m_type.Complex == 0);
  }
  inline BOOL isComplex() const {
    return !allReal();
  }

  void ensureNotScalarEncoded() {
    if (m_type.Scalar == 1) {
      *this = asDenseArray();
    }
  }
  inline BOOL isEmpty() const {
    return ((dataClass() == Invalid) || (length() == 0));
  }

  BOOL operator==(const mxArray* b) const;
  inline BOOL operator!=(const mxArray* b) const {
    return !(*this == b);
  }
  void addField(QString name);
  void* getVoidPointer();
  const void* getConstVoidPointer() const;

  index_t bytes() const;
  index_t address() const;
private:
  Data m_real;
  Data m_imag;
  Type m_type;
};

class ArrayIterator : public BaseIterator<mxArray, mxArray>
{
public:
  ArrayIterator(mxArray* ptr, int dim) :
    BaseIterator<mxArray, mxArray>(ptr, dim) {}
};

class ConstArrayIterator : public ConstBaseIterator<mxArray, mxArray>
{
public:
  ConstArrayIterator(const mxArray* ptr, int dim) :
    ConstBaseIterator<mxArray, mxArray>(ptr, dim) {}
};


#include "ArrayPrivate.h"

#endif
