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
#ifndef __Struct_hpp__
#define __Struct_hpp__

#include "BasicArray.h"
#include "Types.h"

class mxArray;
class interpreter_t;

// A twist on the ever-useful structure array class.
// Now, if the structure is of the handle type (i.e.,
// a pointer), then the data is stored in a struct object
// that is stored on the heap instead of inside the structure
// object itself.  This trick allows multiple variables to
// point to and to modify the same physical data structure
// effectively subverting the value semantics that variables
// in FreeMat normally have.

class StructArray;

class SARefPtr
{
  StructArray* m_ptr;
  interpreter_t* m_eval;
  SARefPtr& operator=(const SARefPtr& copy);
public:
  SARefPtr(StructArray* p, interpreter_t* eval);
  ~SARefPtr();
  SARefPtr(const SARefPtr& copy);
  StructArray& operator*();
  StructArray* operator->();
  const StructArray& operator*() const;
  const StructArray* operator->() const;
};

class StructArray
{
  StringVector m_fields;
  QVector<BasicArray<mxArray> > m_data;
  QString m_class;
  NTuple m_dims;
  BOOL m_handleType;
  uint32 m_refCount;
  SARefPtr m_ptr;
public:
  StructArray() : m_handleType(FALSE), m_refCount(0), m_ptr(0, 0) {}
  StructArray(StructArray* ptr, interpreter_t* eval) :
    m_handleType(TRUE), m_ptr(ptr, eval) {}
  void ref();
  void deref();
  uint32 getRefCount() const {
    if (isHandleClass()) {
      return m_ptr->getRefCount();
    }

    return m_refCount;
  }
  BOOL isHandleClass() const {
    return m_handleType;
  }
  BOOL mxIsUserClass() const {
    if (isHandleClass()) {
      return m_ptr->mxIsUserClass();
    }

    return !m_class.isEmpty();
  }
  QString className() const {
    if (isHandleClass()) {
      return m_ptr->className();
    }

    return m_class;
  }
  void setClassName(const QString& className) {
    if (isHandleClass()) {
      return m_ptr->setClassName(className);
    }

    m_class = className;
  }
  StringVector fieldNames() const {
    if (isHandleClass()) {
      return m_ptr->fieldNames();
    }

    return m_fields;
  }
  void setFieldNamesAndData(const strv_t* fields,
      const QVector<BasicArray<mxArray> > data) {
    if (isHandleClass()) {
      m_ptr->setFieldNamesAndData(fields, data);
      return;
    }

    m_fields = fields;
    m_data = data;
    updateDims();
  }
  int fieldCount() const {
    if (isHandleClass()) {
      return m_ptr->fieldCount();
    }

    return m_fields.size();
  }
  QString fieldName(int i) const {
    if (isHandleClass()) {
      return m_ptr->fieldName(i);
    }

    return m_fields[i];
  }
  index_t bytes() const {
    if (isHandleClass()) {
      return m_ptr->bytes();
    }

    index_t count = 0;

    for (i = 0; i < m_fields.size(); i++) {
      count += m_fields[i].size() * sizeof(QChar);
      count += m_data[i].bytes();
    }

    return count;
  }
  int fieldIndex(QString name) const {
    if (isHandleClass()) {
      return m_ptr->fieldIndex(name);
    }

    if (m_fields.contains(name)) {
      return m_fields.indexOf(name);
    }

    throw_Exception("Fieldname " + name + " not defined");
  }
  BOOL contains(QString name) const {
    if (isHandleClass()) {
      return m_ptr->contains(name);
    }

    return m_fields.contains(name);
  }
  void insert(QString name, const BasicArray<mxArray>& t) {
    if (isHandleClass()) {
      return m_ptr->insert(name, t);
    }

    if (!contains(name)) {
      m_fields += name;
      m_data.push_back(t);
    }
    else {
      m_data[fieldIndex(name)] = t;
    }

    updateDims();
  }
  void insert(QString name, const mxArray* t) {
    BasicArray<mxArray> ba(t);
    insert(name, ba);
  }
  NTuple dimensions() const {
    if (isHandleClass()) {
      return m_ptr->dimensions();
    }

    return m_dims;
  }
  const index_t length() const {
    if (isHandleClass()) {
      return m_ptr->length();
    }

    return m_dims.count();
  }
  void setDimensions(const NTuple& x) {
    if (isHandleClass()) {
      return m_ptr->setDimensions(x);
    }

    m_dims = x;
  }
  void updateDims() {
    if (isHandleClass()) {
      return m_ptr->updateDims();
    }

    if (m_data.size() == 0) {
      m_dims = NTuple(0, 0);
    }
    else {
      m_dims = m_data[0]->dims;
    }
  }
  BasicArray<mxArray>& operator[](int i) {
    if (isHandleClass()) {
      return (*m_ptr)[i];
    }

    return m_data[i];
  }
  const BasicArray<mxArray>& operator[](int i) const {
    if (isHandleClass()) {
      return (*m_ptr)[i];
    }

    return m_data[i];
  }
  BasicArray<mxArray>& operator[](QString name) {
    if (isHandleClass()) {
      return (*m_ptr)[name];
    }

    if (!m_fields.contains(name)) {
      m_fields += name;
      m_data.push_back(BasicArray<mxArray>());
    }

    return m_data[fieldIndex(name)];
  }
  const BasicArray<mxArray>& operator[](QString name) const {
    if (isHandleClass()) {
      return (*m_ptr)[name];
    }

    return m_data[fieldIndex(name)];
  }
};

#endif
