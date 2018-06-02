
#if 0
template <typename T>
static void* Tconstruct(Type t, const void* copy)
{
  if (t.Scalar == 1) {
    throw_Exception("Tconstruct called for scalar object!");
  }

  if (t.Sparse == 1) {
    const SparseMatrix<T>* p = reinterpret_cast<const SparseMatrix<T>*>(copy);
    return reinterpret_cast<void*>(new SparseMatrix<T>(*p));
  }

  const BasicArray<T>* p = reinterpret_cast<const BasicArray<T>*>(copy);
  return reinterpret_cast<void*>(new BasicArray<T>(*p));
}

#define MacroTConstructCopy(ctype,cls)    \
  case cls: return Tconstruct<ctype>(t,copy);

static void* construct(Type t, const void* copy)
{
  switch (t.Class) {
  default:
    throw_Exception("Unsupported construct");
    MacroExpandCasesAll(MacroTConstructCopy);

  case Struct:
    return reinterpret_cast<void*>(new StructArray(*(reinterpret_cast<const StructArray*>(copy))));
  }
}

#undef MacroTConstructCopy

template <typename T>
static void* Tconstruct_sized(Type t, const NTuple& dims)
{
  if (t.Scalar == 1) {
    throw_Exception("Tconstruct_sized called for scalar object");
  }

  if (t.Sparse == 1) {
    return reinterpret_cast<void*>(new SparseMatrix<T>(dims));
  }

  return reinterpret_cast<void*>(new BasicArray<T>(dims));
}

#define MacroTConstructSized(ctype,cls) \
  case cls: return Tconstruct_sized<ctype>(t,dims);

static void* construct_sized(Type t, const NTuple& dims)
{
  switch (t.Class) {
  default:
    throw_Exception("Unsupported constructor");
    MacroExpandCasesAll(MacroTConstructSized);

  case Struct: {
    StructArray* p = new StructArray;
    p->setDimensions(dims);
    return reinterpret_cast<void*>(p);
  }
  }
}

#undef MacroTConstructSized

template <typename T>
static void Tdestruct(Type t, void* todelete)
{
  if (t.Scalar == 1) {
    throw_Exception("Tdestruct called on scalar");
  }

  if (t.Sparse == 1) {
    delete reinterpret_cast<SparseMatrix<T>*>(todelete);
    return;
  }

  delete reinterpret_cast<BasicArray<T>*>(todelete);
  return;
}

#define MacroTDestruct(ctype,cls) \
  case cls: return Tdestruct<ctype>(t,todelete);

static void destruct(Type t, void* todelete)
{
  switch (t.Class) {
  default:
    throw_Exception("Unsupported construct");
    MacroExpandCasesAll(MacroTDestruct);

  case Struct:
    delete reinterpret_cast<StructArray*>(todelete);
    return;
  }
}

#undef MacroTDestruct

#endif

static BOOL AllNonBoolScalars(int n, const mxArray* index)
{
  int i;
  for (i = 0; i < n; i++) {
    const mxArray* indexi = index+i;
    if (!mxIsScalar(indexi) || (indexi->classID == Bool) || (indexi->classID == StringArray)) {
      return FALSE;
    }
  }

  return TRUE;
}

#if 0

template <typename S, typename T>
static void Tdel_scalar(mxArray* ptr, S ndx)
{
  if (ptr->isSparse()) {
    ptr->realSparse<T>().del(ScalarToIndex(ndx));

    if (!ptr->allReal()) {
      ptr->imagSparse<T>().del(ScalarToIndex(ndx));
    }

    return;
  }

  ptr->real<T>().del(ScalarToIndex(ndx));

  if (!ptr->allReal()) {
    ptr->imag<T>().del(ScalarToIndex(ndx));
  }
}

template <typename S, typename T>
static void Tset_scalar(mxArray* ptr, S ndx, const mxArray* data)
{
  if (data.isEmpty()) {
    return Tdel_scalar<S, T>(ptr, ndx);
  }

  if (ptr->isSparse()) {
    ptr->realSparse<T>().set(ndx, data.constRealScalar<T>());

    if (!data mxIsAllReal()) {
      ptr->imagSparse<T>().set(ndx, data.constImagScalar<T>());
    }

    return;
  }

  ptr->real<T>().set(ndx, data.constRealScalar<T>());

  if (!data mxIsAllReal()) {
    ptr->imag<T>().set(ndx, data.constImagScalar<T>());
  }
  else if (!ptr->allReal()) {
    ptr->imag<T>().set(ndx, T(0));
  }
}

template <typename S>
static const void Tset_string_scalar(mxArray* ptr, S ndx, const mxArray* rhs)
{
  if (rhs.isEmpty()) {
    ptr->real<QChar>().del(ndx);
  }
  else {
    ptr->real<QChar>().set(ndx, rhs.constReal<QChar>()[1]);
  }
}

template <typename S>
static const void Tset_struct_scalar(mxArray* ptr, S ndx, const mxArray* rhs)
{
  if ((rhs->classID != Struct) && !rhs.isEmpty()) {
    throw_Exception("Assignment A(I)=B where A is a structure array implies that B is also a structure array.");
  }

  StructArray& lp(ptr->structPtr());

  if (rhs.isEmpty()) {
    for (i = 0; i < lp.fieldCount(); i++) {
      lp[lp.fieldName(i)].del(ScalarToIndex(ndx));
    }
  }
  else {
    // First loop through the elements
    const StructArray& rp(rhs.constStructPtr());

    for (i = 0; i < rp.fieldCount(); i++) {
      lp[rp.fieldName(i)].set(ndx, rp[i].get(1));
    }
  }

  // Loop through the output and force all arrays to be the same size
  NTuple newSize(0, 0);

  for (i = 0; i < lp.fieldCount(); i++) {
    newSize = max(newSize, lp[i]->dims);
  }

  for (i = 0; i < lp.fieldCount(); i++) {
    lp[i].resize(newSize);
  }

  lp.updateDims();
}


#define MacroSetIndexT(ctype,cls)   \
  case cls: Tset_scalar<index_t,ctype>(this,index,data.toClass(this->dataClass())); return;

void mxArray::set(index_t index, const mxArray* data)
{
  ensureNotScalarEncoded();

  switch (m_type.Class) {
    MacroExpandCasesAll(MacroSetIndexT);

  case Struct:
    Tset_struct_scalar<index_t>(this, index, data);
    return;

  default:
    throw_Exception("Unhandled case for A(n) = B");
  }
}

#undef MacroSetIndexT

#define MacroSetNTuple(ctype,cls)   \
  case cls: Tset_scalar<const NTuple&,ctype>(this,index,data.toClass(this->dataClass())); return;

void mxArray::set(const NTuple& index, const mxArray* data)
{
  ensureNotScalarEncoded();

  switch (m_type.Class) {
    MacroExpandCasesAll(MacroSetNTuple);

  case Struct:
    Tset_struct_scalar<const NTuple&>(this, index, data);
    return;

  default:
    throw_Exception("Unhandled case for A(n1,..,nm) = B");
  }
}

#undef MacroSetNTuple

// Handles assignment of A(ndx) = B, where ndx is an IndexArray or IndexArrayVector,
// and A is a basic array of simple types (excludes strings, cell arrays, and structs).
template <typename S, typename T>
static void Tset(mxArray* ptr, S ndx, const mxArray* data)
{
  mxArray dataTyped(data.toClass(ptr->dataClass()));

  if (!ptr->allReal()) {
    dataTyped.forceComplex();
  }

  if (dataTyped mxIsScalar()) {
    if (ptr->isSparse()) {
      Set(ptr->realSparse<T>(), ndx, dataTyped.constRealScalar<T>());
    }
    else {
      Set(ptr->real<T>(), ndx, dataTyped.constRealScalar<T>());
    }

    if (!dataTyped mxIsAllReal()) {
      if (ptr->isSparse()) {
        Set(ptr->imagSparse<T>(), ndx, dataTyped.constImagScalar<T>());
      }
      else {
        Set(ptr->imag<T>(), ndx, dataTyped.constImagScalar<T>());
      }
    }
  }
  else {
    if (ptr->isSparse()) {
      Set(ptr->realSparse<T>(), ndx, ToRealSparse<T>(dataTyped));
    }
    else {
      Set(ptr->real<T>(), ndx, dataTyped.constReal<T>());
    }

    if (!dataTyped mxIsAllReal()) {
      if (ptr->isSparse()) {
        Set(ptr->imagSparse<T>(), ndx, ToImagSparse<T>(dataTyped));
      }
      else {
        Set(ptr->imag<T>(), ndx, dataTyped.constImag<T>());
      }
    }
  }
}

template <typename S>
static const void Tset_struct(mxArray* ptr, S ndx, const mxArray* rhs)
{
  if (rhs->classID != Struct) {
    throw_Exception("Assignment A(I)=B where A is a structure array implies that B is also a structure array.");
  }

  // First loop through the elements
  const StructArray& rp(rhs.constStructPtr());
  StructArray& lp(ptr->structPtr());
  NTuple newSize(ptr->dimensions());

  for (i = 0; i < rp.fieldCount(); i++) {
    Set(lp[rp.fieldName(i)], ndx, rp[i]);
    newSize = lp[rp.fieldName(i)]->dims;
  }

  // Loop through the output and force all arrays to be the same size
  for (j = 0; j < lp.fieldCount(); j++) {
    lp[j].resize(newSize);
  }

  lp.updateDims();
}

#define MacroSetIndexArray(ctype,cls)   \
  case cls: return Tset<const IndexArray&,ctype>(this,index,data);

void mxArray::set(const IndexArray& index, const mxArray* data)
{
  ensureNotScalarEncoded();

  switch (m_type.Class) {
    MacroExpandCasesAll(MacroSetIndexArray);

  case Struct:
    Tset_struct<const IndexArray&>(this, index, data);
    return;

  default:
    throw_Exception("Unhandled case for A(n) = B");
  }
}

#undef MacroSetIndexArray

#define MacroSetIndexArrayVector(ctype,cls) \
  case cls: return Tset<const IndexArrayVector&,ctype>(this,index,data);

void mxArray::set(const IndexArrayVector& index, const mxArray* data)
{
  ensureNotScalarEncoded();

  switch (m_type.Class) {
    MacroExpandCasesAll(MacroSetIndexArrayVector);

  case Struct:
    Tset_struct<const IndexArrayVector&>(this, index, data);
    return;

  default:
    throw_Exception("Unhandled case for A(n1,...,nm) = B");
  }
}

#undef MacroSetIndexArrayVector

template <typename S>
static void Treshape_struct(mxArray* ptr, S ndx)
{
  StructArray& lp(ptr->structPtr());

  for (i = 0; i < lp.fieldCount(); i++) {
    lp[i].reshape(ndx);
  }

  lp.updateDims();
}

template <typename S, typename T>
static void Treshape(mxArray* ptr, S ndx)
{
  if (ptr->isSparse()) {
    ptr->realSparse<T>().reshape(ndx);

    if (!ptr->allReal()) {
      ptr->imagSparse<T>().reshape(ndx);
    }
  }
  else {
    ptr->real<T>().reshape(ndx);

    if (!ptr->allReal()) {
      ptr->imag<T>().reshape(ndx);
    }
  }
}
#define MacroReshapeNTuple(ctype,cls) \
  case cls: return Treshape<const NTuple&, ctype>(this,size);

void mxArray::reshape(const NTuple& size)
{
  ensureNotScalarEncoded();

  switch (m_type.Class) {
    MacroExpandCasesAll(MacroReshapeNTuple);

  case Struct:
    return Treshape_struct<const NTuple&>(this, size);

  default:
    throw_Exception("Unhandled case for reshape");
  }
}

#undef MacroReshapeNTuple

template <typename S>
static void Tresize_struct(mxArray* ptr, S ndx)
{
  StructArray& lp(ptr->structPtr());

  for (i = 0; i < lp.fieldCount(); i++) {
    lp[i].resize(ndx);
  }

  lp.updateDims();
}

template <typename S, typename T>
static void Tresize(mxArray* ptr, S ndx)
{
  if (ptr->isSparse()) {
    ptr->realSparse<T>().resize(ndx);

    if (!ptr->allReal()) {
      ptr->imagSparse<T>().resize(ndx);
    }
  }
  else {
    ptr->real<T>().resize(ndx);

    if (!ptr->allReal()) {
      ptr->imag<T>().resize(ndx);
    }
  }
}

#define MacroResizeNTuple(ctype,cls) \
  case cls: return Tresize<const NTuple&,ctype>(this,size);

void mxArray::resize(const NTuple& size)
{
  ensureNotScalarEncoded();

  switch (m_type.Class) {
    MacroExpandCasesAll(MacroResizeNTuple);

  case Struct:
    return Tresize_struct<const NTuple&>(this, size);

  default:
    throw_Exception("Unhandled case for resize");
  }
}
#undef MacroResizeNTuple

#define MacroResizeIndex(ctype,cls) \
  case cls: return Tresize<index_t,ctype>(this,size);

void mxArray::resize(index_t size)
{
  ensureNotScalarEncoded();

  switch (m_type.Class) {
    MacroExpandCasesAll(MacroResizeIndex);

  case Struct:
    return Tresize_struct<index_t>(this, size);

  default:
    throw_Exception("Unhandled case for resize");
  }
}

#undef MacroResizeIndex

void mxArray::set(const QString& field, ArrayVector& data)
{
  if (isEmpty() && m_type.Class != Struct) {
    *this = mxArray::create(Struct);
  }

  if (m_type.Class != Struct) {
    throw_Exception("Unsupported type for A.field=B");
  }

  StructArray& rp(structPtr());

  if (isEmpty()) {
    rp.insert(field, BasicArray<mxArray>(NTuple(1, 1)));
  }

  if (!rp.contains(field)) {
    rp.insert(field, BasicArray<mxArray>(dimensions()));
  }

  if (data.size() < length()) {
    throw_Exception("Not enough right hand values to satisfy left hand side expression.");
  }

  BasicArray<mxArray>& val(rp[field]);

  for (index_t i = 1; i <= length(); i++) {
    val.set(i, data.front());
    data.pop_front();
  }

  rp.updateDims();
}

const ArrayVector mxArray::get(const QString& field) const
{
  if (m_type.Class != Struct) {
    throw_Exception("Unsupported type for get(string)");
  }

  const StructArray& rp(constStructPtr());

  if (!rp.contains(field)) {
    throw_Exception("Reference to non-existent field " + field);
  }

  ArrayVector ret;
  const BasicArray<mxArray>& val(rp[field]);

  for (index_t i = 1; i <= val.length(); i++) {
    ret.push_back(val.get(i));
  }

  return ret;
}

template <typename S>
static mxArray Tget_struct(const mxArray* ptr, S ndx)
{
  const StructArray& rp(ptr->constStructPtr());
  mxArray ret(Struct, NTuple(1, 1));
  StructArray& lp(ret.structPtr());

  for (i = 0; i < rp.fieldCount(); i++) {
    lp[rp.fieldName(i)] = Get(rp[i], ndx);
  }

  lp.mxSetClassName(rp.className());
  lp.updateDims();
  return ret;
}

template <typename S, typename T>
static const mxArray Tget(const mxArray* ptr, S ndx)
{
  if (ptr->isSparse()) {
    if (ptr->allReal()) {
      return mxArray(Get(ptr->constRealSparse<T>(), ndx));
    }
    else
      return mxArray(Get(ptr->constRealSparse<T>(), ndx),
          Get(ptr->constImagSparse<T>(), ndx));
  }
  else {
    if (ptr->allReal()) {
      return mxArray(Get(ptr->constReal<T>(), ndx));
    }
    else
      return mxArray(Get(ptr->constReal<T>(), ndx),
          Get(ptr->constImag<T>(), ndx));
  }
}

#define MacroGetIndexArray(ctype,cls) \
  case cls: return Tget<const IndexArray&,ctype>(this,index);

const mxArray mxArray::get(const IndexArray& index) const
{
  if (m_type.Scalar == 1) {
    return asDenseArray().get(index);
  }

  switch (m_type.Class) {
  default:
    throw_Exception("Unsupported type for get(index_t)");
    MacroExpandCasesNoCell(MacroGetIndexArray);

  case CellArray:
    return Get(constReal<mxArray>(), index);

  case Struct:
    return Tget_struct<const IndexArray&>(this, index);
  }
}

#undef MacroGetIndexArray

#define MacroGetIndexArrayVector(ctype,cls) \
  case cls: return Tget<const IndexArrayVector&,ctype>(this,index);

const mxArray mxArray::get(const IndexArrayVector& index) const
{
  if (m_type.Scalar == 1) {
    return asDenseArray().get(index);
  }

  switch (m_type.Class) {
  default:
    throw_Exception("Unsupported type for get(indexarrayvector)");
    MacroExpandCasesNoCell(MacroGetIndexArrayVector);

  case CellArray:
    return Get(constReal<mxArray>(), index);

  case Struct:
    return Tget_struct<const IndexArrayVector&>(this, index);
  }
}

#undef MacroGetIndexArrayVector

template <typename T, typename S>
inline static const mxArray Tcast(DataClass t, const mxArray* ptr)
{
  if (ptr->type().Scalar == 1) {
    if (ptr->allReal()) {
      return mxArray(CastConvert<T, S>(ptr->constRealScalar<S>()));
    }
    else
      return mxArray(CastConvert<T, S>(ptr->constRealScalar<S>()),
          CastConvert<T, S>(ptr->constImagScalar<S>()));
  }

  if (ptr->isSparse()) {
    if (ptr->allReal()) {
      return mxArray(ConvertSparseArray<T, S>(ptr->constRealSparse<S>()));
    }
    else
      return mxArray(ConvertSparseArray<T, S>(ptr->constRealSparse<S>()),
          ConvertSparseArray<T, S>(ptr->constImagSparse<S>()));
  }

  if (ptr->allReal()) {
    return mxArray(ConvertBasicArray<T, S>(ptr->constReal<S>()));
  }
  else
    return mxArray(ConvertBasicArray<T, S>(ptr->constReal<S>()),
        ConvertBasicArray<T, S>(ptr->constImag<S>()));
}

#define MacroClassName(ctype,cls) \
  case cls: return QString(#cls).toLower();

QString mxArray::className() const
{
  if ((dataClass() == Struct) && (constStructPtr().mxIsUserClass())) {
    return constStructPtr().className();
  }
  else {
    if (dataClass() == Float) {
      return QString("single");
    }

    switch (dataClass()) {
    default:
      throw_Exception("Unknown class?!");

    case Invalid:
      return QString("");

    case CellArray:
      return QString("cell");

    case Struct:
      return QString("struct");

    case StringArray:
      return QString("char");

    case Bool:
      return QString("logical");
      MacroExpandCasesNoBool(MacroClassName);
    }
  }
}

#undef MacroClassName

BOOL mxArray::mxIsUserClass() const
{
  return ((dataClass() == Struct) && constStructPtr().mxIsUserClass());;
}

template <typename T>
inline static const mxArray BcastComplex(const mxArray* ptr)
{
  mxArray real(Real(*ptr).toClass(Bool));
  mxArray imag(Imag(*ptr).toClass(Bool));
  return Or(real, imag);
}

#define MacroBCastComplex(ctype,cls) \
  case cls: return BcastComplex<ctype>(ptr);

// Special case the complex-to-BOOL cast, as it has special rules
static mxArray BoolCastComplex(const mxArray* ptr)
{
  switch (ptr->dataClass()) {
  default:
    Exception("Cannot convert given array to logical type");
    MacroExpandCasesNoCell(MacroBCastComplex);
  }
}

#undef MacroBCastComplex

#define MacroTcast(ctype,cls) \
  case cls: return Tcast<T,ctype>(t,ptr);

template <typename T>
inline static const mxArray TcastCase(DataClass t, const mxArray* ptr)
{
  switch (ptr->dataClass()) {
  default:
    throw_Exception("Cannot perform type conversions with this type");
    MacroExpandCases(MacroTcast);
    MacroTcast(QChar, StringArray);
  }
}

#undef MacroTcast

#define MacroTcastCase(ctype,cls) \
  case cls: return TcastCase<ctype>(t,this);

const mxArray mxArray::toClass(DataClass t) const
{
  if (dataClass() == t) {
    return *this;
  }

  if (isEmpty()) {
    return mxArray(t, dimensions());
  }

  if ((t == Bool) && (!allReal())) {
    return BoolCastComplex(this);
  }

  switch (t) {
  default:
    throw_Exception("unhandled case for type conversion");
    MacroExpandCasesSimple(MacroTcastCase);
    MacroTcastCase(QChar, StringArray);
  }
}

#undef MacroTcastCase

template <typename S, typename T>
static mxArray Tget_scalar(const mxArray* ptr, S ndx)
{
  if (ptr->isSparse()) {
    if (ptr->allReal()) {
      return mxArray(ptr->constRealSparse<T>().get(ndx));
    }
    else
      return mxArray(ptr->constRealSparse<T>().get(ndx),
          ptr->constImagSparse<T>().get(ndx));
  }

  if (ptr->allReal()) {
    return mxArray(ptr->constReal<T>().get(ndx));
  }
  else
    return mxArray(ptr->constReal<T>().get(ndx),
        ptr->constImag<T>().get(ndx));
}

template <typename S>
static mxArray Tget_struct_scalar(const mxArray* ptr, S ndx)
{
  const StructArray& rp(ptr->constStructPtr());
  mxArray ret(Struct, NTuple(1, 1));
  StructArray& lp(ret.structPtr());

  for (i = 0; i < rp.fieldCount(); i++) {
    lp[rp.fieldName(i)].set(1, rp[i].get(ndx));
  }

  lp.mxSetClassName(rp.className());
  lp.updateDims();
  return ret;
}

template <typename S>
static mxArray Tget_string_scalar(const mxArray* ptr, S ndx)
{
  BasicArray<QChar> ret(NTuple(1, 1));
  ret.set(1, ptr->constReal<QChar>()[ndx]);
  return ret;
}

template <typename S>
static mxArray Tget_cell_scalar(const mxArray* ptr, S ndx)
{
  BasicArray<mxArray> ret(NTuple(1, 1));
  ret.set(1, ptr->constReal<mxArray>()[ndx]);
  return ret;
}

#define MacroGetNTuple(ctype,cls) \
  case cls: return Tget_scalar<const NTuple&,ctype>(this,index);

const mxArray mxArray::get(const NTuple& index) const
{
  if ((m_type.Scalar == 1) && index mxIsScalar()) {
    return *this;
  }

  if (m_type.Scalar == 1) {
    return asDenseArray().get(index);
  }

  switch (m_type.Class) {
  default:
    throw_Exception("Unsupported type for get(const NTuple&)");
    MacroExpandCasesSimple(MacroGetNTuple);

  case CellArray:
    return Tget_cell_scalar<const NTuple&>(this, index);

  case Struct:
    return Tget_struct_scalar<const NTuple&>(this, index);

  case StringArray:
    return Tget_string_scalar<const NTuple&>(this, index);
  }
}

#undef MacroGetNTuple

#define MacroGetIndexT(ctype,cls) \
  case cls: return Tget_scalar<index_t,ctype>(this,index);

const mxArray mxArray::get(index_t index) const
{
  if ((m_type.Scalar == 1) && (index == 1)) {
    return *this;
  }

  if (m_type.Scalar == 1) {
    return asDenseArray().get(index);
  }

  switch (m_type.Class) {
  default:
    throw_Exception("Unhandled case for get(index)");
    MacroExpandCasesSimple(MacroGetIndexT);

  case CellArray:
    return Tget_cell_scalar<index_t>(this, index);

  case StringArray:
    return Tget_string_scalar<index_t>(this, index);

  case Struct:
    return Tget_struct_scalar<index_t>(this, index);
  }
}

#undef MacroGetIndexT

const mxArray mxArray::get(const mxArray* index) const
{
  if (index mxIsScalar() && (index->classID != Bool) && (index->classID != StringArray)) {
    if (!index mxIsAllReal()) {
      WarningMessage("Complex part of index ignored");
    }

    return get(index.asIndexScalar());
  }
  else {
    return get(IndexArrayFromArray(index));
  }
}

const mxArray mxArray::get(const ArrayVector& index) const
{
  if (AllNonBoolScalars(index)) {
    NTuple addr(1, 1);

    for (i = 0; i < index.size(); i++) {
      addr[i] = index[i].asIndexScalar();
    }

    return get(addr);
  }
  else {
    IndexArrayVector addr;

    for (i = 0; i < index.size(); i++) {
      addr.push_back(IndexArrayFromArray(index[i]));
    }

    return get(addr);
  }
}

void mxArray::set(const mxArray* index, const mxArray* data)
{
  if (isEmpty() && dataClass() != data->classID) {
    *this = toClass(data->classID);
  }

  if (index mxIsScalar() && (index->classID != Bool)
      && (index->classID != StringArray)) {
    if (!index mxIsAllReal()) {
      WarningMessage("Complex part of index ignored");
    }

    set(index.asIndexScalar(), data);
  }
  else {
    set(IndexArrayFromArray(index), data);
  }
}

void mxArray::set(const ArrayVector& index, const mxArray* data)
{
  if (isEmpty() && dataClass() != data->classID) {
    *this = toClass(data->classID);
  }

  if (AllNonBoolScalars(index)) {
    NTuple addr(1, 1);

    for (i = 0; i < index.size(); i++) {
      addr[i] = index[i].asIndexScalar();
    }

    set(addr, data);
  }
  else {
    IndexArrayVector addr;

    for (i = 0; i < index.size(); i++) {
      addr.push_back(IndexArrayFromArray(index[i]));
    }

    set(addr, data);
  }
}

void mxArray::addField(QString name)
{
  if (dataClass() != Struct) {
    throw_Exception("addField only valid for structure arrays");
  }

  if (!structPtr().contains(name)) {
    structPtr().insert(name, BasicArray<mxArray>());
  }
}

#define MacroGetVoidPointer(ctype,cls)          \
  case cls:               \
  if (m_type.Scalar == 1)           \
    return (void*)(&(m_real.cls));          \
  else if (m_type.Sparse == 1)            \
    throw_Exception("getVoidPointer not supported for sparse arrays");  \
  else                  \
    return (void*)(real<ctype>().data());

void* mxArray::getVoidPointer()
{
  switch (dataClass()) {
  default:
    throw_Exception("Unsupported type called for getVoidPointer");
    MacroExpandCasesSimple(MacroGetVoidPointer);
  }
}

#undef MacroGetVoidPointer

#define MacroGetConstVoidPointer(ctype,cls)       \
  case cls:               \
  if (m_type.Scalar == 1)           \
    return (const void*)(&(m_real.cls));        \
  else if (m_type.Sparse == 1)            \
    throw_Exception("getConstVoidPointer not supported for sparse arrays"); \
  else                  \
    return (const void*)(constReal<ctype>().constData());

const void* mxArray::getConstVoidPointer() const
{
  switch (dataClass()) {
  default:
    throw_Exception("Unsupported type called for getConstVoidPointer");
    MacroExpandCasesSimple(MacroGetConstVoidPointer);
  }
}

template <typename T>
static index_t Tbytes(const mxArray* ptr)
{
  index_t count = 0;

  if (ptr->type().Sparse == 1) {
    count = ptr->constRealSparse<T>().bytes();
  }
  else if (ptr->type().Scalar == 1) {
    count = TSizeOf<T>(ptr->constRealScalar<T>());
  }
  else {
    count = ptr->constReal<T>().bytes();
  }

  if (!ptr->allReal()) {
    if (ptr->type().Sparse == 1) {
      count += ptr->constImagSparse<T>().bytes();
    }
    else if (ptr->type().Scalar == 1) {
      count += TSizeOf<T>(ptr->constImagScalar<T>());
    }
    else {
      count += ptr->constImag<T>().bytes();
    }
  }

  return count;
}

#define MacroTbytes(ctype,cls)      \
  case cls: return Tbytes<ctype>(this);

index_t mxArray::bytes() const
{
  switch (dataClass()) {
  default:
    return 0;
    MacroExpandCasesAll(MacroTbytes);

  case Struct:
    return constStructPtr().bytes();
  }
}

index_t mxArray::address() const
{
  if (m_type.Scalar == 1) {
    return 0;
  }
  else {
    union l {
      void* p;
      uint32 y;
    } u;
    u.p = m_real.p->ptr();
    return (index_t)(u.y);
  }
}

template <typename T>
static index_t T_index_scalar(const mxArray* ptr)
{
  index_t a = ptr->constRealScalar<T>();

  if (a < 1) {
    throw_Exception("index values must be >= 1");
  }

  return a;
}

#define MacroAsIndexScalar(ctype,cls) \
  case cls:           \
  return T_index_scalar<ctype>(this);

const index_t mxArray::asIndexScalar() const
{
  switch (dataClass()) {
  default:
    throw_Exception("Unsupported type called on asIndexScalar");

  case Bool:
    if (constRealScalar<BOOL>()) {
      return index_t(1);
    }

    return index_t(0);

  case StringArray:
    throw_Exception("Illegal indexing expression");
    MacroExpandCasesNoBool(MacroAsIndexScalar);
  }
}

#undef MacroAsIndexScalar

template <typename T>
static void T_force_complex(mxArray* ptr)
{
  if (!ptr->allReal()) {
    return;
  }

  if (ptr->type().Scalar == 1) {
    ptr->imagScalar<T>() = 0;
  }
  else if (ptr->type().Sparse == 1) {
    ptr->imagSparse<T>();
  }
  else {
    ptr->imag<T>();
  }
}

#define MacroForceComplex(ctype,cls) \
  case cls: return T_force_complex<ctype>(this);

void mxArray::forceComplex()
{
  switch (dataClass()) {
  default:
    return;
    MacroExpandCasesNoBool(MacroForceComplex);
  }
}


#define MacroAsComplex(ctype,cls) \
  case cls: return T_as_complex<ctype>(this);

const mxArray mxArray::asComplex() const
{
  if (!allReal()) {
    return *this;
  }

  mxArray tmp(*this);
  tmp.forceComplex();
  return tmp;
}

static BOOL Tequals_struct(const mxArray* pA, const mxArray* pB)
{
  const StructArray& ap(pA->constStructPtr());
  const StructArray& bp(pB->constStructPtr());

  if (ap.fieldCount() != bp.fieldCount()) {
    return FALSE;
  }

  for (i = 0; i < ap.fieldCount(); i++) {
    if (!bp.contains(ap.fieldName(i))) {
      return FALSE;
    }

    if (!(ap[i] == bp[ap.fieldName(i)])) {
      return FALSE;
    }
  }

  return TRUE;
}

static BOOL Tequals_string(const mxArray* pA, const mxArray* pB)
{
  return (pA->constReal<QChar>() == pB->constReal<QChar>());
}

static BOOL Tequals_cell(const mxArray* pA, const mxArray* pB)
{
  return (pA->constReal<mxArray>() == pB->constReal<mxArray>());
}

template <typename T>
static BOOL Tequals_scalar(const mxArray* pA, const mxArray* pB)
{
  if (pA->allReal()) {
    return (pA->constRealScalar<T>() == pB->constRealScalar<T>());
  }
  else
    return ((pA->constRealScalar<T>() == pB->constRealScalar<T>()) &&
        (pA->constImagScalar<T>() == pB->constImagScalar<T>()));
}

template <typename T>
static BOOL Tequals_array(const mxArray* pA, const mxArray* pB)
{
  if (pA->isSparse() && pB->isSparse()) {
    if (pA->allReal()) {
      return (pA->constRealSparse<T>() == pB->constRealSparse<T>());
    }
    else
      return ((pA->constRealSparse<T>() == pB->constRealSparse<T>()) &&
          (pA->constImagSparse<T>() == pB->constImagSparse<T>()));
  }
  else {
    const mxArray* dA(pA->asDenseArray());
    const mxArray* dB(pB->asDenseArray());

    if (dA mxIsAllReal()) {
      return (dA.constReal<T>() == dB.constReal<T>());
    }
    else
      return ((dA.constReal<T>() == dB.constReal<T>()) &&
          (dA.constImag<T>() == dB.constImag<T>()));
  }
}

#define MacroScalarEquals(ctype,cls) \
  case cls: return Tequals_scalar<ctype>(this,&b);

#define MacroArrayEquals(ctype,cls) \
  case cls: return Tequals_array<ctype>(this,&b);

// Need to make this more general - how so?
BOOL mxArray::operator==(const mxArray* b) const
{
  if (!isReferenceType() && !b.isReferenceType() && isScalar() && b mxIsScalar()) {
    if (allReal() ^ b mxIsAllReal()) {
      return FALSE;
    }

    if (dataClass() != b->classID) {
      return FALSE;
    }

    switch (dataClass()) {
    default:
      throw_Exception("Unhandled scalar case");
      MacroExpandCasesSimple(MacroScalarEquals);
    }

    return FALSE;
  }

  if (!(dimensions() == b->dims)) {
    return FALSE;
  }

  if (dataClass() != b->classID) {
    return FALSE;
  }

  if (allReal() ^ b mxIsAllReal()) {
    return FALSE;
  }

  switch (dataClass()) {
  default:
    throw_Exception("Unhandled case as argument to == operator for mxArray class");
    MacroExpandCasesSimple(MacroArrayEquals);

  case CellArray:
    return Tequals_cell(this, &b);

  case Struct:
    return Tequals_struct(this, &b);

  case StringArray:
    return Tequals_string(this, &b);
  }

  return FALSE;
}

#undef MacroArrayEquals

#undef MacroScalarEquals

int32 mxArray::asInteger() const
{
  if (m_type.Class == StringArray) {
    return asString().toInt();
  }

  return (this->toClass(Int32).constRealScalar<int32>());
}

double mxArray::asDouble() const
{
  if (m_type.Class == StringArray) {
    return asString().toDouble();
  }

  return (this->toClass(Double).constRealScalar<double>());
}

QString mxArray::asString() const
{
  if (isEmpty()) {
    return QString();
  }

  if (m_type.Class != StringArray) {
    throw_Exception("Cannot convert array to string");
  }

  const BasicArray<QChar>& p(constReal<QChar>());
  QString ret;

  for (i = 0; i < p.length(); i++) {
    ret += p[i + 1];
  }

  return ret;
}


template <typename T>
static mxArray Tscalar_to_dense(const mxArray* ptr)
{
  if (ptr->allReal()) {
    return mxArray(new BasicArray<T>(ptr->constRealScalar<T>()));
  }
  else {
    return mxArray(new BasicArray<T>(ptr->constRealScalar<T>()),
        new BasicArray<T>(ptr->constImagScalar<T>()));
  }
}

template <typename T>
static mxArray Tsparse_to_dense(const mxArray* ptr)
{
  if (ptr->allReal()) {
    return mxArray(ptr->constRealSparse<T>().asDense());
  }
  else
    return mxArray(ptr->constRealSparse<T>().asDense(),
        ptr->constImagSparse<T>().asDense());
}

#define MacroScalarToDense(ctype,cls) \
  case cls: return Tscalar_to_dense<ctype>(this);

#define MacroSparseToDense(ctype,cls) \
  case cls: return Tsparse_to_dense<ctype>(this);

mxArray mxArray::asDenseArray() const
{
  if ((m_type.Scalar == 0) && (m_type.Sparse == 0)) {
    return *this;
  }

  if (m_type.Scalar == 1) {
    switch (m_type.Class) {
    default:
      throw_Exception("Cannot convert supplied array to dense type");
      MacroExpandCasesSimple(MacroScalarToDense);
    }
  }

  switch (m_type.Class) {
  default:
    throw_Exception("Cannot convert supplied sparse array to dense type");
    MacroExpandCasesSimple(MacroSparseToDense);
  }
}

#undef MacroSparseToDense
#undef MacroScalarToDense

#endif
