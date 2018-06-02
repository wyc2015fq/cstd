
index_t getSliceIndex(const dim_type* dimensions, const mxArray* index_arr)
{
  index_t offset;
  dim_type dim[mxMAXDIMS] = {0};
  int i, len = mxGetNumberOfElements(index_arr);
  const index_t* index = (const index_t*)index_arr->data;
  ASSERT(mxINDEX_CLASS==index_arr->classID);
  dim[0] = 1;
  
  for (i = 1; i < len; i++) {
    dim[i] = (dim_type)index[i];
  }
  
  offset = NDim_map(dimensions, dim);
  return offset;
}

BOOL IsColonOp(const mxArray* arr)
{
  if (arr->classID == StringArray) {
    char buf[20];
    int len = mxGetString(arr, buf, 10);
    return (buf[0] == ':' && buf[1]==0);
  } else if (mxIsScalar(arr)) {
    int tt = (int)mxGetScalar(arr);
    return (tt == -1);
  }
  
  return FALSE;
}

int ExpandColons(const mxArray* ndx_arr, int len, mxArray* retvec)
{
  index_t* ret;
  int i;
  if (!IsColonOp(ndx_arr)) {
    mxCopyArray(retvec, ndx_arr);
    return 0;
  }
  mxSetNumericMatrix(retvec, len, 1, mxINDEX_CLASS, 0);
  ret = (index_t*)retvec->data;
  
  for (i = 0; i < len; i++) {
    ret[i] = i + 1;
  }
  
  return 0;
}

#if 0

BOOL isSliceIndexCase(const NTuple& dims, const IndexArrayVector& index)
{
  if (IsColonOp(index[0])) {
    BOOL allScalars = TRUE;
    
    for (i = 1; i < index.size(); i++)
      allScalars = allScalars && !IsColonOp(index[i]) && index[i] mxIsScalar()
      && (index[i][1] <= dims[1]);
    
    return allScalars;
  }
  
  return FALSE;
}

BasicArray<BOOL> GetDeletionMap(const IndexArray& vec, index_t length)
{
  BasicArray<BOOL> retvec(NTuple(length, 1));
  
  for (index_t i = 1; i <= vec.length(); i++) {
    retvec[vec[i]] = TRUE;
  }
  
  return retvec;
}

BOOL DimensionCovered(const IndexArray& ndx, index_t length)
{
  return AllTrue(GetDeletionMap(ndx, length));
}

IndexArray ScalarToIndex(index_t val)
{
  return IndexArray(val);
}

IndexArrayVector ScalarToIndex(const NTuple& val)
{
  IndexArrayVector ret;
  
  for (i = 0; i < val.lastNotOne(); i++) {
    ret.push_back(IndexArray(val[i]));
  }
  
  return ret;
}

{
  mxArray slice(const IndexArrayVector& index) const {
    index_t offset = getSliceIndex(dimensions(), index);
    mxArray retvec;
    retvec.m_dims = NTuple(dimensions()[0], 1);
    retvec.m_offset = m_offset + offset;
    retvec.m_data = m_data;
    retvec.m_count = retvec.m_dims.count();
    return retvec;
  }
  mxArray slice(const NTuple& pos) const {
    index_t offset = dimensions().map(pos) - 1;
    mxArray retvec;
    retvec.m_dims = NTuple(dimensions()[0], 1);
    retvec.m_offset = m_offset + offset;
    retvec.m_data = m_data;
    retvec.m_count = retvec.m_dims.count();
    return retvec;
  }
  int del(const IndexArrayVector& index) {
    // The strategy for dealing with deletions is simplified relative
    // to 3.x code.  An NDim deletion is only valid if there is one
    // dimension that is not covered.
    QVector<BOOL> covered(index.size());

    for (i = 0; i < index.size(); i++) {
      index_t dimLen = dimensions()[i];
      covered[i] = (IsColonOp(index[i]) || DimensionCovered(index[i], dimLen));
    }

    // Count the number of uncovered dimensions
    int uncovered_count = 0;
    int first_uncovered = 0;

    for (i = 0; i < index.size(); i++)
      if (!covered[i]) {
        first_uncovered = i;
        uncovered_count++;
      }

    if (uncovered_count == 0) {
      m_data.clear();
      m_dims = NTuple(0, 0);
      m_offset = 0;
      m_count = 0;
      return;
    }

    if (uncovered_count > 1) {
      throw_Exception("Deletion A(:,...,:) = [] cannot have more than one non-singular dimension");
    }

    index_t dimLen = dimensions()[first_uncovered];
    BasicArray<BOOL> map(GetDeletionMap(index[first_uncovered], dimLen));
    int newSize = 0;

    for (index_t i = 1; i <= map.length(); i++)
      if (!map.get(i)) {
        newSize++;
      }

    // Create an output array..
    NTuple outdims(dimensions());
    outdims[first_uncovered] = newSize;
    mxArray retvec(outdims);
    ConstBasicIterator<T> source(this, first_uncovered);
    BasicIterator<T> dest(&retvec, first_uncovered);

    while (source.isValid() && dest.isValid()) {
      for (index_t i = 1; i <= dimLen; i++) {
        if (!map.get(i)) {
          dest.set(source.get());
          dest.next();
        }

        source.next();
      }

      source.nextSlice();
      dest.nextSlice();
    }

    m_data = retvec.m_data;
    m_offset = retvec.m_offset;
    m_dims = retvec.m_dims;
    m_count = m_dims.count();
  }
  int del(const IndexArray& index) {
    if (IsColonOp(index)) {
      m_data.clear();
      m_dims = NTuple(0, 0);
      m_offset = 0;
      m_count = m_dims.count();
      return;
    }

    BasicArray<BOOL> map(GetDeletionMap(index, length()));
    index_t newSize = 0;

    for (index_t i = 1; i <= map.length(); i++)
      if (!map[i]) {
        newSize++;
      }

    if (newSize == length()) {
      return;
    }

    NTuple newDim;

    if (isRowVector()) {
      newDim = NTuple(1, newSize);
    }
    else {
      newDim = NTuple(newSize, 1);
    }

    Vector<T> rdata((size_t)newSize);
    int j = 0;

    for (index_t i = 1; i <= map.length(); i++)
      if (!map[i]) {
        rdata[j++] = get(i);
      }

    m_data = rdata;
    m_offset = 0;
    m_dims = newDim;
    m_count = m_dims.count();
  }
  int resize(const NTuple& pos) {
    if (pos.extends(m_dims)) {
      m_dims = pos;
      m_count = pos.count();
      m_data.resize(pos.count());
      return;
    }

    mxArray retval(pos);
    Transformer<mxArray, T> transform(&retval, this);

    while (transform.isValid()) {
      for (i = 0; i < transform.size(); i++) {
        transform.set(transform.get());
        transform.next();
      }

      transform.nextSlice();
    }

    *this = retval;
  }
  int resize(index_t len) {
    if (len > length()) {
      NTuple newDim;

      if (isEmpty() || isScalar()) {
        newDim = NTuple(1, len);
      }
      else if (isVector()) {
        if (rows() != 1) {
          newDim = NTuple(len, 1);
        }
        else {
          newDim = NTuple(1, len);
        }
      }
      else {
        m_dims = NTuple(1, length());
        newDim = NTuple(1, len);
      }

      resize(newDim);
    }

    m_count = m_dims.count();
  }
  int printMe(std::ostream& o) const {
    o << dimensions() << "[";

    for (index_t i = 1; i <= length(); i++) {
      o << get(i) << " ";
    }

    o << "]\n";
  }
  inline int reshape(const NTuple& pos) {
    if (m_dims.count() == pos.count()) {
      m_dims = pos;
    }
    else {
      throw_Exception("Illegal reshape");
    }
  }
  inline index_t bytes() const {
    if (sizeof(T) <= 8) {
      return sizeof(T) * length();
    }

    index_t total = 0;

    for (index_t i = 1; i <= length(); i++) {
      total += TSizeOf<T>(get(i));
    }

    return total;
  }
};

template <typename T>
mxArray Uniform(const NTuple& dims, T val)
{
  mxArray retval(dims);
  retval.fill(val);
  return retval;
}

template <typename T>
std::ostream& operator<<(std::ostream& o, const mxArray* arg)
{
  arg.printMe(o);
  return o;
}

template <typename T>
mxArray MergeComplex(const mxArray* real, const mxArray* imag)
{
  NTuple retdim(real->dims);

  if (retdim != imag->dims) {
    throw_Exception("Cannot merge arrays of unequal size into complex array");
  }

  retdim[0] = retdim[0] * 2;
  mxArray retval(retdim);

  for (index_t i = 1; i <= real.length(); i++) {
    retval.setNoBoundsCheck(2 * i - 1, real.getNoBoundsCheck(i));
    retval.setNoBoundsCheck(2 * i, imag.getNoBoundsCheck(i));
  }

  return retval;
}

template <typename T>
mxArray MergeComplex(const mxArray* real)
{
  NTuple retdim(real->dims);
  retdim[0] = retdim[0] * 2;
  mxArray retval(retdim);

  for (index_t i = 1; i <= real.length(); i++) {
    retval.setNoBoundsCheck(2 * i - 1, real.getNoBoundsCheck(i));
  }

  return retval;
}
#endif
int Split_impl(const mxArray* A, int isimag, mxArray* B)
{
  isimag = !!isimag;
  if (A->iscomplex) {
    int i, len = mxGetNumberOfElements(A);
    mxArray ttt[5] = {0};
    mxArray* B_out = (A==B) ? ttt : B;
    mxSetNumericArray(B_out, A->n_dims, A->dims, A->classID, 0, 0);
    switch (A->classID) {
    case mxUNKNOWN_CLASS:
    case mxCELL_CLASS:
    case mxSTRUCT_CLASS:
      {
        ASSERT(0);
      } break;
#define CLASSSIZEDEF(hh, ID, TYPE, FMT)  case ID: { typedef struct {TYPE re[2];}TYPE1; TYPE1* a = (TYPE1*)A->data;TYPE *b = (TYPE*)B_out->data; for (i = 0; i < len; i++) { b[i] = a[i].re[isimag]; } } break;
      CLASSSIZEDEFNUMBER(hh, CLASSSIZEDEF);
#undef CLASSSIZEDEF
    }
    if (B_out!=B) {
      mxCopyArray(B, B_out);
    }
    mxFreeArrays(ttt, 5);
  } else {
    if (isimag) {
      mxSetNumericArray(B, A->n_dims, A->dims, A->classID, 0, 0);
      mxFillArray(B, 0, 0);
    } else {
      mxCopyArray(B, A);
    }
  }
  return 0;
}

#define SplitReal(A, B) Split_impl(A, 0, B)
#define SplitImag(A, B) Split_impl(A, 1, B)

BOOL MergedArrayHasComplexComponents(const mxArray* arr)
{
  if (arr->iscomplex) {
    int i, len = mxGetNumberOfElements(arr);
    switch (arr->classID) {
    case mxUNKNOWN_CLASS:
    case mxCELL_CLASS:
    case mxSTRUCT_CLASS:
      return FALSE;
      break;
#define CLASSSIZEDEF(hh, ID, TYPE, FMT)  case ID: {TYPE* src = (TYPE*)(arr->data); for (i=0; i<len; ++i) {if (src[i*2+1]!=0) {return TRUE;} } } break;
      CLASSSIZEDEFNUMBER(hh, CLASSSIZEDEF);
#undef CLASSSIZEDEF
    }
  }
  return FALSE;
}

#if 0
template <typename T>
mxArray Apply(const mxArray* arg, T(*func)(T))
{
  mxArray retval(arg->dims);
  Transformer<mxArray, T> transform(&retval, &arg);

  while (transform.isValid()) {
    for (i = 0; i < transform.size(); i++) {
      transform.set(func(transform.get()));
      transform.next();
    }

    transform.nextSlice();
  }

  return retval;
}

#endif
int Transpose(const mxArray* arr, mxArray* ret)
{
  mxArray ttt[5];
  mxArray* retvec = (arr==ret) ? ttt : ret;
  int i, j, m = arr->dims[0], n = arr->dims[1];
  if (!mxIs2D(arr)) {
    throw_Exception("Cannot transpose multi-dimensional arrays");
  }
  
  if (!arr->iscomplex) {
    mxSetNumericMatrix(retvec, arr->dims[1], arr->dims[0], arr->classID, arr->iscomplex);
    switch (arr->classID) {
    case mxUNKNOWN_CLASS:
    case mxCELL_CLASS:
    case mxSTRUCT_CLASS:
      {
        mxArray* src = (mxArray*)(arr->data), *dst = (mxArray*)(retvec->data);
        for (i=0; i<m; ++i) {
          for (j=0; j<n; ++j) {
            mxCopyArray(&dst[j+i*n], &src[i+j*m]);
          }
        }
      }
      break;
#define CLASSSIZEDEF(hh, ID, TYPE, FMT)  case ID: {TYPE* src = (TYPE*)(arr->data), *dst = (TYPE*)(retvec->data); for (i=0; i<m; ++i) {for (j=0; j<n; ++j){dst[j+i*n] = src[i+j*m];}} } break;
      CLASSSIZEDEFNUMBER(hh, CLASSSIZEDEF);
#undef CLASSSIZEDEF
    }
  } else {
    DataClass out_type = TypeToSigned(arr->classID);
    mxConvertArray(ttt+1, arr, out_type, arr->classID);
    arr = ttt+1;
    switch (arr->classID) {
    default:
      ASSERT(0);
      break;
#define CLASSSIZEDEF(hh, ID, TYPE, FMT)  case ID: {typedef struct {TYPE re, im;}TYPE1; TYPE1* src = (TYPE1*)(arr->data), *dst = (TYPE1*)(retvec->data); for (i=0; i<m; ++i) {for (j=0; j<n; ++j){dst[j+i*n] = src[i+j*m];dst[j+i*n].im = -dst[j+i*n].im;}} } break;
      CLASSSIZEDEF_SIGNED(hh, CLASSSIZEDEF);
#undef CLASSSIZEDEF
    }
  }
  if (retvec!=ret) {
    mxCopyArray(ret, retvec);
  }
  mxFreeArrays(ttt, 5);
  return 0;
}

#if 0
template <typename T>
static inline T neg(T v)
{
  return -v;
}

template <typename T>
mxArray Negate(const mxArray* arg)
{
  return Apply<T>(arg, neg);
}

template <typename T>
BOOL IsPositive(const mxArray* arg)
{
  for (index_t i = 1; i <= arg.length(); i++)
    if (arg.getNoBoundsCheck(i) <= 0) {
      return FALSE;
    }

  return TRUE;
}

template <typename T>
BOOL IsPositiveOrNaN(const mxArray* arg)
{
  for (index_t i = 1; i <= arg.length(); i++)
    if (!IsNaN(i) && (arg.getNoBoundsCheck(i) <= 0)) {
      return FALSE;
    }

  return TRUE;
}

template <typename T>
BOOL IsNonNegative(const mxArray* arg)
{
  for (index_t i = 1; i <= arg.length(); i++)
    if (!IsNonNegative(arg.getNoBoundsCheck(i))) {
      return FALSE;
    }

  return TRUE;
}

template <typename T>
BOOL IsNonNegativeOrNaN(const mxArray* arg)
{
  for (index_t i = 1; i <= arg.length(); i++)
    if (!IsNaN(i) && (arg.getNoBoundsCheck(i) < 0)) {
      return FALSE;
    }

  return TRUE;
}

template <typename T>
BOOL IsInteger(const mxArray* arg)
{
  for (index_t i = 1; i <= arg.length(); ++i)
    if (!IsInteger(arg.getNoBoundsCheck(i))) {
      return FALSE;
    }

  return TRUE;
}

#endif
BOOL mxIsEqu(const mxArray* A, const mxArray* B) {
  int i, m = A->dims[0], n = A->dims[1];
  if (0!=NDim_cmp(A->dims, B->dims)) {
    return FALSE;
  }
  if (A->classID!=B->classID) {
    return FALSE;
  }
  
  switch (A->classID) {
  case mxUNKNOWN_CLASS:
  case mxCELL_CLASS:
  case mxSTRUCT_CLASS:
    {
      mxArray* a = (mxArray*)(A->data), *b = (mxArray*)(B->data);
      for (i=0; i<m*n; ++i) {
        if (!mxIsEqu(&a[i], &b[i])) {
          return FALSE;
        }
      }
    }
    break;
#define CLASSSIZEDEF(hh, ID, TYPE, FMT)  case ID: {TYPE* a = (TYPE*)(A->data), *b = (TYPE*)(B->data); for (i=0; i<m*n; ++i) { if (a[i] != b[i]) {return FALSE;} } } break;
    CLASSSIZEDEFNUMBER(hh, CLASSSIZEDEF);
#undef CLASSSIZEDEF
  }
  return TRUE;
}
BOOL IsSymmetric(const mxArray* arr)
{
  int i, j, m = arr->dims[0], n = arr->dims[1];
  if (!mxIs2D(arr)) {
    throw_Exception("Symmetry check not valid for N-dimensional arrays");
  }
  
  switch (arr->classID) {
  case mxUNKNOWN_CLASS:
  case mxCELL_CLASS:
  case mxSTRUCT_CLASS:
    {
      mxArray* src = (mxArray*)(arr->data);
      for (i=0; i<m; ++i) {
        for (j=0; j<n; ++j) {
          //mxEqu(&dst[j+i*n], &src[i+j*m]);
          if (!mxIsEqu(&src[j+i*n], &src[i+j*m])) {
            return FALSE;
          }
        }
      }
    }
    break;
#define CLASSSIZEDEF(hh, ID, TYPE, FMT)  case ID: {TYPE* src = (TYPE*)(arr->data); for (i=0; i<m; ++i) {for (j=0; j<n; ++j){if (src[j+i*n] != src[i+j*m]) {return FALSE;}}} } break;
    CLASSSIZEDEFNUMBER(hh, CLASSSIZEDEF);
#undef CLASSSIZEDEF
  }

  return TRUE;
}

#if 0
template <typename T>
BOOL IsSymmetric(const mxArray* arg, const mxArray* img)
{
  if (!arg->dims.is2D() || !img->dims.is2D()) {
    throw_Exception("Symmetry check not valid for N-dimensional arrays");
  }

  if (arg->dims != img->dims) {
    throw_Exception("Symmetry check requires both arrays must be the same size");
  }

  for (index_t i = 1; i <= arg.cols(); i++)
    for (index_t j = i; j <= arg.rows(); j++)
      if ((arg[NTuple(i, j)] != arg[NTuple(j, i)]) ||
          (img[NTuple(i, j)] != -img[NTuple(j, i)])) {
        return FALSE;
      }

  return TRUE;
}
#endif

int GetDiagonal(const mxArray* A, int diagonal, mxArray* B)
{
  mxArray ttt[5] = {0};
  int i, outLen;
  mxArray* B_out = (A==B) ? ttt : B;
  int m = A->dims[0];

  if (!mxIs2D(A)) {
    throw_Exception("Cannot get diagonal for N-dimensional arrays");
  }
  if (diagonal < 0) {
    outLen = MAX(0, MIN(A->dims[0] + diagonal, A->dims[1]));
  } else {
    outLen = MAX(0, MIN(A->dims[0], A->dims[1] - diagonal));
  }
  if (outLen == 0) {
    mxFreeArray(B);
    return 0;
  }
  mxSetNumericMatrix(B_out, outLen, 1, A->classID, A->iscomplex);
  
  if (!A->iscomplex) {
    switch (A->classID) {
    case mxUNKNOWN_CLASS:
    case mxCELL_CLASS:
    case mxSTRUCT_CLASS:
      {
        mxArray* a = (mxArray*)A->data, *b = (mxArray*)B->data;
        if (diagonal < 0) {
          for (i = 0; i < outLen; i++) {
            mxCopyArray(&b[i], &a[(i - diagonal)+m*i]);
          }
        } else {
          for (i = 0; i < outLen; i++) {
            mxCopyArray(&b[i], &a[i+m*(i + diagonal)]);
          }
        }
      } break;
#define CLASSSIZEDEF(hh, ID, TYPE, FMT)  case ID: { TYPE* a = (TYPE*)A->data, *b = (TYPE*)B->data; if (diagonal < 0) {  for (i = 0; i < outLen; i++) {    b[i] = a[(i - diagonal)+m*i];  }}else {  for (i = 0; i < outLen; i++) {    b[i] = a[i+m*(i + diagonal)];  }}  } break;
      CLASSSIZEDEFNUMBER(hh, CLASSSIZEDEF);
#undef CLASSSIZEDEF
    }
  } else {
    switch (A->classID) {
    case mxUNKNOWN_CLASS:
    case mxCELL_CLASS:
    case mxSTRUCT_CLASS:
      {
        ASSERT(0);
      } break;
#define CLASSSIZEDEF(hh, ID, TYPE, FMT)  case ID: { typedef struct {TYPE re, im;}TYPE1; TYPE1* a = (TYPE1*)A->data, *b = (TYPE1*)B->data; if (diagonal < 0) {  for (i = 0; i < outLen; i++) {    b[i] = a[(i - diagonal)+m*i];  }}else {  for (i = 0; i < outLen; i++) {    b[i] = a[i+m*(i + diagonal)];  }}  } break;
      CLASSSIZEDEFNUMBER(hh, CLASSSIZEDEF);
#undef CLASSSIZEDEF
    }
  }
  
  mxFreeArrays(ttt, 5);
  return 0;
}

int DiagonalArray(const mxArray* A, int diagonal, mxArray* B)
{
  mxArray ttt[5] = {0};
  int i, outLen = mxGetNumberOfElements(A);
  int M = outLen + abs(diagonal);
  mxArray* B_out = (A==B) ? ttt : B;
  mxSetNumericMatrix(B_out, M, M, A->classID, A->iscomplex);
  
  if (!A->iscomplex) {
    switch (A->classID) {
    case mxUNKNOWN_CLASS:
    case mxCELL_CLASS:
    case mxSTRUCT_CLASS:
      {
        mxArray* a = (mxArray*)A->data, *b = (mxArray*)B->data;
        if (diagonal < 0) {
          for (i = 0; i < outLen; i++) {
            mxCopyArray(&b[(i - diagonal)+M*i], &a[i]);
          }
        } else {
          for (i = 0; i < outLen; i++) {
            mxCopyArray(&b[i+M*(i + diagonal)], &a[i]);
          }
        }
      } break;
#define CLASSSIZEDEF(hh, ID, TYPE, FMT)  case ID: { TYPE* a = (TYPE*)A->data, *b = (TYPE*)B->data; if (diagonal < 0) { for (i = 0; i < outLen; i++) {  b[(i - diagonal)+M*i] = a[i]; } } else { for (i = 0; i < outLen; i++) { b[i+M*(i + diagonal)] = a[i]; } }  } break;
      CLASSSIZEDEFNUMBER(hh, CLASSSIZEDEF);
#undef CLASSSIZEDEF
    }
  } else {
    switch (A->classID) {
    case mxUNKNOWN_CLASS:
    case mxCELL_CLASS:
    case mxSTRUCT_CLASS:
      {
        ASSERT(0);
      } break;
#define CLASSSIZEDEF(hh, ID, TYPE, FMT)  case ID: { typedef struct {TYPE re, im;}TYPE1; TYPE1* a = (TYPE1*)A->data, *b = (TYPE1*)B->data; if (diagonal < 0) { for (i = 0; i < outLen; i++) {  b[(i - diagonal)+M*i] = a[i*2+0]; } } else { for (i = 0; i < outLen; i++) { b[i+M*(i + diagonal)] = a[i]; } }  } break;
      CLASSSIZEDEFNUMBER(hh, CLASSSIZEDEF);
#undef CLASSSIZEDEF
    }
  }
  
  mxFreeArrays(ttt, 5);
  return 0;
}
#if 0
int Permute(const mxArray* arg, NTuple perm, mxArray* B)
{
  mxArray retval(arg->dims.permute(perm));
  ConstBasicIterator<T> iter(&arg, 0);

  while (iter.isValid()) {
    for (index_t i = 1; i <= iter.size(); i++) {
      retval[iter.pos().replace(0, i).permute(perm)] = iter.get();
      iter.next();
    }

    iter.nextSlice();
  }

  return retval;
}
#endif
BOOL AllZeros(const mxArray* arr)
{
  int i, len = mxGetNumberOfElements(arr);
  
  switch (arr->classID) {
  case mxUNKNOWN_CLASS:
  case mxCELL_CLASS:
  case mxSTRUCT_CLASS:
    ASSERT(0);
    return 0;
#define CLASSSIZEDEF(hh, ID, TYPE, FMT)  case ID: {TYPE* a = (TYPE*)arr->data; for (i = 0; i < len; i++) { if (a[i] != 0) { return FALSE; } } } break;
    CLASSSIZEDEFNUMBER(hh, CLASSSIZEDEF);
#undef CLASSSIZEDEF
  }

  return TRUE;
}

BOOL AllNonZeros(const mxArray* arr)
{
  int i, len = mxGetNumberOfElements(arr);
  
  switch (arr->classID) {
  case mxUNKNOWN_CLASS:
  case mxCELL_CLASS:
  case mxSTRUCT_CLASS:
    ASSERT(0);
    return 0;
#define CLASSSIZEDEF(hh, ID, TYPE, FMT)  case ID: {TYPE* a = (TYPE*)arr->data; for (i = 0; i < len; i++) { if (a[i] == 0) { return FALSE; } } } break;
    CLASSSIZEDEFNUMBER(hh, CLASSSIZEDEF);
#undef CLASSSIZEDEF
  }

  return TRUE;
}

BOOL AnyNotFinite(const mxArray* arg)
{
  int i, len = mxGetNumberOfElements(arg)*(arg->iscomplex ? 2:1);
  switch (arg->classID) {
  case mxSINGLE_CLASS:
    {
      float* data = (float*)arg->data;
      for (i = 0; i <len; i++) {
        if (!IsFinite_f32(data[i])) {
          return TRUE;
        }
      }
    }
    break;
  case mxDOUBLE_CLASS:
    {
      double* data = (double*)arg->data;
      for (i = 0; i <len; i++) {
        if (!IsFinite_f64(data[i])) {
          return TRUE;
        }
      }
    }
    break;
  default:
    return FALSE;
  }

  return FALSE;
}
#if 0


template <typename T>
BOOL AnyNaN(const mxArray* arg)
{
  for (index_t i = 1; i <= arg.length(); i++)
    if (IsNaN(arg.getNoBoundsCheck(i))) {
      return TRUE;
    }

  return FALSE;
}

template <typename T>
T MinValue(const mxArray* arg)
{
  if (arg.isEmpty()) {
    throw_Exception("Cannot use this min function on empty arrays.");
  }

  T retval = arg[1];

  for (i = 2; i <= arg.length(); i++)
    if (retval > arg[i]) {
      retval = arg[i];
    }

  return retval;
}

template <typename T>
T MaxValue(const mxArray* arg)
{
  if (arg.isEmpty()) {
    throw_Exception("Cannot use this max function on empty arrays.");
  }

  T retval = arg[1];

  for (i = 2; i <= arg.length(); i++)
    if (retval < arg[i]) {
      retval = arg[i];
    }

  return retval;
}

#endif
int Find(const mxArray* arr, mxArray* out)
{
  int i, count = 0, len = mxGetNumberOfElements(arr);
  index_t* ind=0;
  
  switch (arr->classID) {
  case mxUNKNOWN_CLASS:
  case mxCELL_CLASS:
  case mxSTRUCT_CLASS:
    ASSERT(0 && "Function 'find' is not defined for values of class 'cell'.");
    return 0;
#define CLASSSIZEDEF(hh, ID, TYPE, FMT)  case ID: { TYPE* a = (TYPE*)arr->data; for (i = 0; i < len; i++) { if (a[i] != 0) { count++; } } } break;
CLASSSIZEDEFNUMBER(hh, CLASSSIZEDEF);
#undef CLASSSIZEDEF
  }
  
  ASSERT(0 && "unknown class");
  
  mxSetNumericMatrix(out, 1, count, mxINDEX_CLASS, 0);
  ind = (index_t*)out->data;
  
  switch (arr->classID) {
  case mxUNKNOWN_CLASS:
  case mxCELL_CLASS:
  case mxSTRUCT_CLASS:
    return 0;
#define CLASSSIZEDEF(hh, ID, TYPE, FMT)  case ID: { TYPE* a = (TYPE*)arr->data; for (i = 0; i < len; i++) { if (a[i] != 0) { ind[count++] = i+1; } } } break;
CLASSSIZEDEFNUMBER(hh, CLASSSIZEDEF);
#undef CLASSSIZEDEF
  }
  
  return 0;
}

#if 0
inline BOOL AllTrue(const BasicArray<BOOL>& arg)
{
  for (i = 1; i <= arg.length(); i++)
    if (!arg.getNoBoundsCheck(i)) {
      return FALSE;
    }

  return TRUE;
}


template <typename T, typename S>
mxArray ConvertBasicArray(const BasicArray<S>& source)
{
  mxArray dest(source->dims);
  T* ret = dest.data();
  const S* src = source.constData();
  size_t len = size_t(source.length());

  for (size_t i = 0; i < len; i++) {
    ret[i] = CastConvert<T, S>(src[i]);
  }

  return dest;
}

template <typename S>
class BasicIterator : public BaseIterator<BasicArray<S>, S>
{
public:
  BasicIterator(BasicArray<S>* ptr, int dim) :
    BaseIterator<BasicArray<S>, S>(ptr, dim) {}
};

template <typename S>
class ConstBasicIterator : public ConstBaseIterator<BasicArray<S>, S>
{
public:
  ConstBasicIterator(const BasicArray<S>* ptr, int dim) :
    ConstBaseIterator<BasicArray<S>, S>(ptr, dim) {}
};

template <typename T>
class ConstSpinIterator
{
  const mxArray* m_ptr;
public:
  ConstSpinIterator(const mxArray* ptr) : m_ptr(ptr) {}
  inline T get() const {
    return m_ptr->get(1);
  }
  inline int next() {}
};


#endif

int ToBasicArray(const int* x, int n, int clss, mxArray* ret)
{
  mxSetNumericArray(ret, 1, &n, clss, mxREAL, x);
  return 0;
}

#if 0
template <typename T>
SparseMatrix<T> ToRealSparse(const mxArray* data)
{
  if (data->issparse == 1) {
    return data.constRealSparse<T>();
  }

  mxArray cdata(data);

  if (cdata mxIsScalar()) {
    cdata = data.asDenseArray();
  }

  if (!cdata.is2D()) {
    throw_Exception("Sparse matrix cannot be created from multidimensional arrays");
  }

  return SparseMatrix<T>(cdata.constReal<T>());
}

template <typename T>
SparseMatrix<T> ToImagSparse(const mxArray* data)
{
  if (data->issparse == 1) {
    return data.constImagSparse<T>();
  }

  mxArray cdata(data);

  if (cdata mxIsScalar()) {
    cdata = cdata.asDenseArray();
  }

  if (!cdata.is2D()) {
    throw_Exception("Sparse matrix cannot be created from multidimensional arrays");
  }

  return SparseMatrix<T>(cdata.constImag<T>());
}

template <typename T>
mxArray DiagonalArray(const mxArray* in)
{
  mxArray retmat(NTuple(in.length(), in.length()));

  for (index_t i = 1; i <= in.length(); i++) {
    retmat.set(NTuple(i, i), in.get(i));
  }

  return mxArray(retmat);
}

template <typename T>
mxArray DiagonalArray(const mxArray* in_r,
    const mxArray* in_i)
{
  mxArray retmat_r(NTuple(in_r.length(), in_r.length()));
  mxArray retmat_i(NTuple(in_i.length(), in_i.length()));

  for (index_t i = 1; i <= in_r.length(); i++) {
    retmat_r.set(NTuple(i, i), in_r.get(i));
    retmat_i.set(NTuple(i, i), in_i.get(i));
  }

  return mxArray(retmat_r, retmat_i);
}

static mxArray EmptyConstructor()
{
  return mxArray(Double);
}

template <typename T>
static mxArray BasicArrayFromNative(const T* data, NTuple dims)
{
  mxArray rp(dims);

  for (i = 0; i < dims.count(); i++) {
    rp.set(i + 1, data[i]);
  }

  return rp;
}


const mxArray StringArrayFromStringVector(const vstr_t* arg, QChar pad)
{
  int maxlen = 0;

  for (i = 0; i < arg.size(); i++) {
    maxlen = qMax(maxlen, arg[i].size());
  }

  mxArray ret(StringArray, NTuple(arg.size(), maxlen));
  BasicArray<QChar>& sp(ret.real<QChar>());

  for (i = 0; i < arg.size(); i++) {
    for (j = 0; j < arg[i].size(); j++) {
      sp.set(NTuple(index_t(i + 1), index_t(j + 1)), arg[i][j]);
    }

    for (j = arg[i].size(); j < maxlen; j++) {
      sp.set(NTuple(index_t(i + 1), index_t(j + 1)), pad);
    }
  }

  return ret;
}

mxArray Vectorize(const mxArray* arg)
{
  mxArray A(arg);
  A.reshape(NTuple(A.length(), 1));
  return A;
}

BOOL IsCellStringArray(const mxArray* arg)
{
  if (arg->classID != CellArray) {
    return FALSE;
  }

  const BasicArray<mxArray>& rp(arg.constReal<mxArray>());

  for (index_t i = 1; i <= arg.length(); i++)
    if (!rp[i].isString()) {
      return FALSE;
    }

  return TRUE;
}

#endif

BOOL IsUnsigned(const mxArray* x)
{
  switch (x->classID) {
  default:
    return FALSE;

  case Bool:
  case UInt8:
  case StringArray:
  case UInt16:
  case UInt32:
  case UInt64:
    return TRUE;
  }
}


BOOL IsInteger(const mxArray* x)
{
  if (mxIsAllReal(x)) {
    double tt;
    switch (x->classID) {
    default:
      return FALSE;
      
    case Int8:
    case UInt8:
    case Int16:
    case UInt16:
    case Int32:
    case UInt32:
    case Int64:
    case UInt64:
      return TRUE;
      
    case Float:
    case Double:
      tt = mxGetScalar(x);
      return tt==(int)tt;
    }
  }

  return FALSE;
}

#if 0
static int ValidateIndex(const mxArray* d)
{
  const BasicArray<index_t>& dp(d.constReal<index_t>());

  for (index_t n = 1; n <= dp.length(); n++)
    if (dp[n] < 1) {
      throw_Exception("index values must be >= 1");
    }
}
#endif

int IndexArrayFromArray(const mxArray* index, mxArray* out)
{
  if (!mxIsAllReal(index)) {
    throw_Exception("Complex part of index ignored");
  }

  if (index->classID == Bool) {
    return Find(index, out);
  }
  
  if (IsColonOp(index)) {
    return mxSetIndexScalar(out, -1);
  }
  
  return mxConvertArray(out, index, mxINDEX_CLASS, 0);
}

int ArrayVectorFromCellArray(const mxArray* arg, mxArray* out)
{
  int i, n = mxGetNumberOfElements(arg);
  mxArray* arr = (mxArray*)arg->data;
  if (0==n) {
    return 0;
  }

  if (arg->classID != CellArray) {
    throw_Exception("Unsupported type for call to toArrayVector");
  }

  if (out) {
    for (i = 0; i < n; i++) {
      mxCopyArray(out+i, arr+i);
    }
  }

  return n;
}

int ArrayFromCellArray(const mxArray* arg, mxArray* out)
{
  if (arg->classID != CellArray) {
    throw_Exception("Unsupported type for call to ArrayFromCellArray");
  }

  if (mxGetNumberOfElements(arg) != 1) {
    throw_Exception("ArrayFromCellArray called with non-scalar argument");
  }
  
  mxCopyArray(out, (mxArray*)arg->data);
  return 0;
}
int CellArrayFromArray(const mxArray* arg, mxArray* out)
{
  mxSetNumericMatrix(out, 1, 1, mxCELL_CLASS, 0);
  mxCopyArray((mxArray*)out->data, arg);
  return 0;
}

int CellArrayFromArrayVector(const mxArray* arg, int cnt, mxArray* out)
{
  int i;
  mxArray* p;
  mxSetCellMatrix(out, 1, cnt);

  p = (mxArray*)out->data;
  for (i = 0; i < cnt; i++) {
    mxCopyArray(p+i, arg+i);
  }

  return 0;
}

#if 0

const mxArray CellArrayFromStringVector(const vstr_t* arg)
{
  mxArray ret(CellArray, NTuple(1, arg.size()));
  BasicArray<mxArray>& rp(ret.real<mxArray>());

  for (i = 0; i < arg.size(); i++) {
    rp.set(index_t(i + 1), mxArray(arg[i]));
  }

  return rp;
}

StringVector StringVectorFromArray(const mxArray* arg)
{
  if (arg->classID == Double) {
    return StringVectorFromArray(arg.toClass(StringArray));
  }

  if (arg.isString()) {
    const BasicArray<QChar>& ap(arg.constReal<QChar>());
    ConstBasicIterator<QChar> iter(&ap, 1);
    StringVector ret;

    while (iter.isValid()) {
      QString t(int(iter.size()), QChar(0));

      for (index_t i = 1; i <= iter.size(); i++) {
        t[int(i - 1)] = iter.get();
        iter.next();
      }

      iter.nextSlice();
      ret << t;
    }

    return ret;
  }

  if (!IsCellStringArray(arg)) {
    throw_Exception("Cannot convert array to a set of a strings");
  }

  StringVector ret;
  const BasicArray<mxArray>& rp(arg.constReal<mxArray>());

  for (index_t i = 1; i <= rp.length(); i++) {
    ret.push_back(rp[i].asString());
  }

  return ret;
}

int SetCellContents(mxArray* cell, const mxArray* index,
    ArrayVector& data)
{
  if (cell->classID != CellArray) {
    if (!cell.isEmpty()) {
      throw_Exception("A{B} = C only supported for cell arrays.");
    }
    else {
      cell = cell.toClass(CellArray);
    }
  }

  if (IsColonOp(index)) {
    if (cell.length() > data.size()) {
      throw_Exception("Not enough right hand side values to satisfy left hand side expression.");
    }

    cell.set(index, CellArrayFromArrayVector(data, data.size()));
    data.clear();
    return;
  }

  IndexArray ndx(IndexArrayFromArray(index));

  if (ndx.length() > data.size()) {
    throw_Exception("Not enought right hand side values to satisfy left hand side expression.");
  }

  cell.set(ndx, CellArrayFromArrayVector(data, ndx.length()));
}

int SetCellContents(mxArray* cell, const ArrayVector& index,
    ArrayVector& data)
{
  if (cell->classID != CellArray) {
    throw_Exception("A{B1,B2,...BN} = B only supported for cell arrays.");
  }

  IndexArrayVector addr;
  NTuple dims;

  for (i = 0; i < index.size(); i++) {
    addr.push_back(IndexArrayFromArray(index[i]));

    if (IsColonOp(addr[i])) {
      dims[i] = cell->dims[i];
    }
    else {
      dims[i] = addr[i].length();
    }
  }

  if (data.size() < dims.count()) {
    throw_Exception("Not enough right hand side values to satisfy left hand side expression");
  }

  cell.set(addr, CellArrayFromArrayVector(data, dims.count()));
}

QStringList FieldNames(const mxArray* arg)
{
  if (arg->classID != Struct) {
    throw_Exception("fieldnames only valid for structure arrays");
  }

  const StructArray& rp(arg.constStructPtr());
  return rp.fieldNames();
}
#endif

static int do_single_sided_algo_double(double a, double b, double* pvec, int adder, int count)
{
  double d = a;
  int i;

  for (i = 0; i < count; i++) {
    pvec[i * adder] = (double) d;
    d += b;
  }
  return 0;
}

static int do_double_sided_algo_double(double a, double b, double c, double* pvec, int adder, int count)
{
  if (count % 2) {
    do_single_sided_algo_double(a, b, pvec, adder, count / 2);
    do_single_sided_algo_double(c, -b, pvec + (count - 1)*adder, -adder, count / 2 + 1);
  }
  else {
    do_single_sided_algo_double(a, b, pvec, adder, count / 2);
    do_single_sided_algo_double(c, -b, pvec + (count - 1)*adder, -adder, count / 2);
  }
  return 0;
}

int RangeConstructor(double minval, double stepsize, double maxval, BOOL vert, mxArray* out)
{
  double ntest_min, ntest_max;
  int npts;
  BOOL use_double_sided;
  dim_type dim[mxMAXDIMS] = {0};

  if (stepsize == 0) {
    throw_Exception("step size must be nonzero in colon expression");
  }

  if (!IsFinite_f64(minval) || !IsFinite_f64(stepsize) || !IsFinite_f64(maxval)) {
    mxFreeArray(out);
    return 0;
  }

  //ideally, n = (c-a)/b
  // But this really defines an interval... we let
  // n_min = min(c-a)/max(b)
  // n_max = max(c-a)/min(b)
  // where min(x) = {y \in fp | |y| is max, |y| < |x|, sign(y) = sign(x)}
  //       max(x) = {y \in fp | |y| is min, |y| > |x|, sign(y) = sign(x)}
  ntest_min = nextafter(maxval - minval, 0) / nextafter(stepsize, stepsize + stepsize);
  ntest_max = nextafter(maxval - minval, maxval - minval + stepsize) / nextafter(stepsize, 0);
  npts = (int) floor(ntest_max);
  use_double_sided = (ntest_min <= npts) && (npts <= ntest_max);
  npts++;

  if (npts < 0) {
    npts = 0;
  }

  if (vert) {
    NDim_set2(dim, npts, 1);
  }
  else {
    NDim_set2(dim, 1, npts);
  }

  mxSetNumericArray(out, 2, dim, mxDOUBLE_CLASS, 0, 0);

  if (use_double_sided) {
    do_double_sided_algo_double(minval, stepsize, maxval, (double*)out->data, 1, npts);
  }
  else {
    do_single_sided_algo_double(minval, stepsize, (double*)out->data, 1, npts);
  }

  return 0;
}

int UnitColon(const mxArray* A, const mxArray* B, mxArray* out)
{
  if (!mxIsScalar(A) || !mxIsScalar(B)) {
    throw_Exception("Both arguments to (:) operator must be scalars.");
  }
  
  if (A->iscomplex || B->iscomplex) {
    throw_Exception("Both arguments to (:) operator must be real.");
  }
  
  RangeConstructor(mxGetScalar(A), 1, mxGetScalar(B), FALSE, out);
  if ((A->classID == Float) || (B->classID == Float)) {
    mxConvertArray(out, out, Float, FALSE);
  }
  
  return 0;
}

int DoubleColon(const mxArray* A, const mxArray* B, const mxArray* C, mxArray* out)
{
  if (!mxIsScalar(A) || !mxIsScalar(B) || !mxIsScalar(C)) {
    throw_Exception("All three arguments to (:) operator must be scalars.");
  }
  
  if (A->iscomplex || B->iscomplex || C->iscomplex) {
    throw_Exception("All arguments to (:) operator must be real.");
  }

  RangeConstructor(mxGetScalar(A), mxGetScalar(B), mxGetScalar(C), FALSE, out);

  if ((A->classID == Float) || (B->classID == Float) || (C->classID == Float)) {
    mxConvertArray(out, out, Float, FALSE);
  }
  
  return 0;
}

#if 0

BasicArray<mxArray> ArrayVectorToBasicArray(const ArrayVector& a)
{
  BasicArray<mxArray> retvec(NTuple(a.size(), 1));

  for (i = 0; i < a.size(); i++) {
    retvec.set(index_t(i + 1), a.at(i));
  }

  return retvec;
}

NTuple ConvertArrayToNTuple(const mxArray* A)
{
  if (A.length() > NDims)
    throw_Exception(QString("MAT Variable has more dimensions ") +
        QString("than maxDims (currently set to ") +
        NDims + ")."); // FIXME - more graceful ways to do this

  mxArray B(A.asDenseArray().toClass(Index));
  const BasicArray<index_t>& rp(B.constReal<index_t>());
  NTuple dm;

  for (index_t i = 1; i <= B.length(); i++) {
    dm[int(i - 1)] = rp[i];
  }

  return dm;
}

mxArray ConvertNTupleToArray(const NTuple& D)
{
  index_t Dsize(D.lastNotOne());
  BasicArray<index_t> rp(NTuple(1, Dsize));

  for (index_t i = 1; i <= Dsize; i++) {
    rp.set(i, D[int(i - 1)]);
  }

  return mxArray(rp);
}

template <typename T>
static BOOL Tanynotfinite(const mxArray* ptr)
{
  if (ptr->isScalar())
    if (ptr->allReal()) {
      return (!IsFinite(ptr->constRealScalar<T>()));
    }
    else {
      return (!IsFinite(ptr->constRealScalar<T>()) || !IsFinite(ptr->constImagScalar<T>()));
    }
  else if (ptr->isSparse())
    if (ptr->allReal()) {
      return AnyNotFinite(ptr->constRealSparse<T>());
    }
    else {
      return (AnyNotFinite(ptr->constRealSparse<T>()) || AnyNotFinite(ptr->constImagSparse<T>()));
    }
  else if (ptr->allReal()) {
    return AnyNotFinite(ptr->constReal<T>());
  }
  else {
    return (AnyNotFinite(ptr->constReal<T>()) || AnyNotFinite(ptr->constImag<T>()));
  }
}

#define MacroAnyNotFinite(ctype,cls)    \
  case cls: return Tanynotfinite<ctype>(&A);

BOOL AnyNotFinite(const mxArray* A)
{
  if (A.isReferenceType()) {
    return FALSE;
  }

  switch (A->classID) {
  default:
    return FALSE;
    MacroExpandCasesSimple(MacroAnyNotFinite);
  }
}

#undef MacroAnyNotFinite


template <typename T>
static BOOL Tanynan(const mxArray* ptr)
{
  if (ptr->isScalar())
    if (ptr->allReal()) {
      return (IsNaN(ptr->constRealScalar<T>()));
    }
    else {
      return (IsNaN(ptr->constRealScalar<T>()) || IsNaN(ptr->constImagScalar<T>()));
    }
  else if (ptr->isSparse())
    if (ptr->allReal()) {
      return AnyNaN(ptr->constRealSparse<T>());
    }
    else {
      return (AnyNaN(ptr->constRealSparse<T>()) || AnyNaN(ptr->constImagSparse<T>()));
    }
  else if (ptr->allReal()) {
    return AnyNaN(ptr->constReal<T>());
  }
  else {
    return (AnyNaN(ptr->constReal<T>()) || AnyNaN(ptr->constImag<T>()));
  }
}


#define MacroAnyNaN(ctype,cls)      \
  case cls: return Tanynan<ctype>(&A);

BOOL AnyNaN(const mxArray* A)
{
  if (A.isReferenceType()) {
    return FALSE;
  }

  switch (A->classID) {
  default:
    return FALSE;
    MacroExpandCasesSimple(MacroAnyNaN);
  }
}

template <typename T>
static BOOL Tispositive(const mxArray* ptr)
{
  if (ptr->isScalar()) {
    if (ptr->allReal()) {
      return (ptr->constRealScalar<T>() >= 0);
    }
    else {
      return FALSE;
    }
  }
  else if (ptr->isSparse()) {
    return IsPositive(ptr->constRealSparse<T>());
  }
  else {
    return IsPositive(ptr->constReal<T>());
  }
}

#define MacroIsPositive(ctype,cls)    \
  case cls: return Tispositive<ctype>(&A);

BOOL IsPositive(const mxArray* A)
{
  switch (A->classID) {
  default:
    return TRUE;
    MacroExpandCasesSigned(MacroIsPositive);
  }
}

#undef MacroIsPositive

template <typename T>
static BOOL Tispositiveornan(const mxArray* ptr)
{
  if (ptr->isScalar()) {
    if (ptr->allReal()) {
      return (IsNaN(ptr->constRealScalar<T>()) || (ptr->constRealScalar<T>() > 0));
    }
    else {
      return FALSE;
    }
  }
  else if (ptr->isSparse()) {
    return IsPositiveOrNaN(ptr->constRealSparse<T>());
  }
  else {
    return IsPositiveOrNaN(ptr->constReal<T>());
  }
}

#define MacroIsPositiveOrNaN(ctype,cls)   \
  case cls: return Tispositiveornan<ctype>(&A);

BOOL IsPositiveOrNaN(const mxArray* A)
{
  switch (A->classID) {
  default:
    return TRUE;
    MacroExpandCasesSigned(MacroIsPositiveOrNaN);
  }
}


template <typename T>
static BOOL Tisnonnegativeornan(const mxArray* ptr)
{
  if (ptr->isScalar()) {
    if (ptr->allReal()) {
      return (IsNaN(ptr->constRealScalar<T>()) || (ptr->constRealScalar<T>() >= 0));
    }
    else {
      return FALSE;
    }
  }
  else if (ptr->isSparse()) {
    return IsNonNegativeOrNaN(ptr->constRealSparse<T>());
  }
  else {
    return IsNonNegativeOrNaN(ptr->constReal<T>());
  }
}

#define MacroIsNonNegativeOrNaN(ctype,cls)    \
  case cls: return Tisnonnegativeornan<ctype>(&A);

BOOL IsNonNegativeOrNaN(const mxArray* A)
{
  switch (A->classID) {
  default:
    return TRUE;
    MacroExpandCasesSigned(MacroIsNonNegativeOrNaN);
  }
}


#undef MacroIsPositive


template <typename T>
static BOOL Tissymmetric(const mxArray* ptr)
{
  if (ptr->isScalar()) {
    return (ptr->constImagScalar<T>() == 0);
  }
  else if (ptr->isSparse())
    if (ptr->allReal()) {
      return IsSymmetric(ptr->constRealSparse<T>());
    }
    else {
      return IsSymmetric(ptr->constRealSparse<T>(), ptr->constImagSparse<T>());
    }
  else if (ptr->allReal()) {
    return IsSymmetric(ptr->constReal<T>());
  }
  else {
    return IsSymmetric(ptr->constReal<T>(), ptr->constImag<T>());
  }
}

#define MacroIsSymmetric(ctype,cls)   \
  case cls: return Tissymmetric<ctype>(&A);

BOOL IsSymmetric(const mxArray* A)
{
  switch (A->classID) {
  default:
    return TRUE;
    MacroExpandCasesSimple(MacroIsSymmetric);
  }
}

#undef MacroIsSymmetric

template <typename T>
static mxArray Tgetdiagonal(const mxArray* ptr, int diagonal)
{
  if (ptr->isScalar())
    if (diagonal == 0) {
      return *ptr;
    }
    else {
      return mxArray(ptr->dataClass());
    }
  else if (ptr->isSparse())
    if (ptr->allReal()) {
      return mxArray(GetDiagonal(ptr->constRealSparse<T>(), diagonal));
    }
    else
      return mxArray(GetDiagonal(ptr->constRealSparse<T>(), diagonal),
          GetDiagonal(ptr->constImagSparse<T>(), diagonal));
  else if (ptr->allReal()) {
    return mxArray(GetDiagonal(ptr->constReal<T>(), diagonal));
  }
  else
    return mxArray(GetDiagonal(ptr->constReal<T>(), diagonal),
        GetDiagonal(ptr->constImag<T>(), diagonal));
}

#define MacroGetDiagonal(ctype,cls)     \
  case cls: return Tgetdiagonal<ctype>(&a,diagonal);

mxArray GetDiagonal(const mxArray* a, int diagonal)
{
  switch (a->classID) {
  default:
    throw_Exception("unsupported type for get diagonal");
    MacroExpandCasesSimple(MacroGetDiagonal);
  }
}

#undef MacroGetDiagonal

template <typename T>
static mxArray Tdiagonalarray(const mxArray* ptr, int diagonal)
{
  if (ptr->isSparse())
    if (ptr->allReal()) {
      return mxArray(DiagonalArray(ptr->constRealSparse<T>(), diagonal));
    }
    else
      return mxArray(DiagonalArray(ptr->constRealSparse<T>(), diagonal),
          DiagonalArray(ptr->constImagSparse<T>(), diagonal));
  else if (ptr->allReal()) {
    return mxArray(DiagonalArray(ptr->constReal<T>(), diagonal));
  }
  else
    return mxArray(DiagonalArray(ptr->constReal<T>(), diagonal),
        DiagonalArray(ptr->constImag<T>(), diagonal));
}

#define MacroDiagonalArray(ctype,cls)     \
  case cls: return Tdiagonalarray<ctype>(&A,diagonal);

mxArray DiagonalArray(const mxArray* A, int diagonal)
{
  switch (A->classID) {
  default:
    throw_Exception("unsupported type for diagonal array constructor");
    MacroExpandCasesSimple(MacroDiagonalArray);
  }
}

#undef MacroDiagonalArray

#endif
/**
 * Perform an eigen decomposition of the matrix A - This version computes the 
 * eigenvectors, and returns the eigenvalues in a diagonal matrix
 */

static int HandleEigenVectorsRealMatrix(mxArray* eigenvals, mxArray* Vp, int N, mxArray* D, mxArray* V) {
  // Make a pass through the eigenvals, and look for complex eigenvalues.
  BOOL complexEigenvalues = MergedArrayHasComplexComponents(eigenvals);
  mxArray ttt[5] = {0};
  if (!complexEigenvalues) {
    mxArray* tmp = ttt+0;
    SplitReal(eigenvals, tmp);
    DiagonalArray(tmp, 0, D);
    mxCopyArray(V, Vp);
  } else {
    DiagonalArray(eigenvals, 0, D);
    mxCopyArray(V, Vp);
#if 0
    V = Array(GetDataClass<T>(T(0)),NTuple(N,N));
    index_t i = 1;
    while (i <= N) {
      if ((i < N) && (eigenvals[2*i] != 0)) {
        for (index_t j=1;j<=N;j++) {
          V.set(NTuple(j,i),Array(Vp.get(NTuple(j,i)),Vp.get(NTuple(j,i+1))));
          V.set(NTuple(j,i+1),Array(Vp.get(NTuple(j,i)),-Vp.get(NTuple(j,i+1))));
        }
        i += 2;
      } else {
        for (index_t j=1;j<=N;j++) 
          V.set(NTuple(j,i),Array(Vp.get(NTuple(j,i))));
        i++;
      }
    }
#endif
  }
  mxFreeArrays(ttt, 5);
  return 0;
}
#if 0


// Do a horizontal concatenation of a set of arrays
// Horizontal concatenation means that all dimensions
// except for the number of columns must match.  The
// type logic is the opposite of FreeMat 3.6!  The smallest
// type dominates the type of the output (space-preserving
// as opposed to precision-preserving).  This change is to
// support the M-lab model.

// This is the generic version that works along any dimension.
// It only works with dense arrays, and not with structure arrays

// How this algorithm is supposed to work:
// We want to concatenate several arrays into a single array.
// [a,b,c,d]
// We want to iterate over the output array, and select which
// array an element belongs to - this
// adjusted is the index
//
//

// #  Wes Campaigne replied on June 5th, 2007 at 11:57 pm :

// How does cat choose the dominant class, i.e., the class of its return value?

// A quick experiment with the basic types seems to reveal a descending priority of:
// char
// integer types (if there are multiple different integer-typed operands, the type of the left-most of these is dominant)
// single
// double
// logical

// Appropriate warnings are given most of the time, but truncation of floats to char doesnt throw any. And promotion of one integer type (or logicals) to a bigger integer type DOES throw a warning, even though its unnecessary.

// Is there a particular reasoning behind all this behaviour? I havent noticed it documented anywhere.

// Combining Unlike Classes

// Matrices and arrays can be composed of elements of most any MATLAB data type as long as all elements in the matrix are of the same type. If you do include elements of unlike classes when constructing a matrix, MATLAB converts some elements so that all elements of the resulting matrix are of the same type. (See Built-In Classes (Data Types) for information on any of the MATLAB classes discussed here.)

// Data type conversion is done with respect to a preset precedence of classes. The following table shows the five classes you can concatenate with an unlike type without generating an error (that is, with the exception of character and logical).

// Table is usual C,I,S,D,L, with C-L combination made illegal

// For example, concatenating a double and single matrix always yields a matrix of type single. MATLAB converts the double element to single to accomplish this.
// Combining Unlike Integer Types

// If you combine different integer types in a matrix (e.g., signed with unsigned, or 8-bit integers with 16-bit integers), MATLAB returns a matrix in which all elements are of one common type. MATLAB sets all elements of the resulting matrix to the data type of the left-most element in the input matrix. For example, the result of the following concatenation is a vector of three 16-bit signed integers:

// A = [int16(450) uint8(250) int32(1000000)]

// MATLAB also displays a warning to inform you that the result may not be what you had expected:

// A = [int16(450) uint8(250) int32(1000000)];
// Warning: Concatenation with dominant (left-most) integer class
// may overflow other operands on conversion to return class.

// You can disable this warning by entering the following two commands directly after the operation that caused the warning. The first command retrieves the message identifier associated with the most recent warning issued by MATLAB. The second command uses this identifier to disable any further warnings of that type from being issued:

// [msg, intcat_msgid] = lastwarn;
// warning('off', intcat_msgid);

// To reenable the warning so that it will now be displayed, use

// warning('on', intcat_msgid);

// You can use these commands to disable or enable the display of any MATLAB warning.

// Example of Combining Unlike Integer Sizes.   After disabling the integer concatenation warnings as shown above, concatenate the following two numbers once, and then switch their order. The return value depends on the order in which the integers are concatenated. The left-most type determines the data type for all elements in the vector:

// A = [int16(5000) int8(50)]
// A =
//    5000   50

// B = [int8(50) int16(5000)]
// B =
//    50   127

// The first operation returns a vector of 16-bit integers. The second returns a vector of 8-bit integers. The element int16(5000) is set to 127, the maximum value for an 8-bit signed integer.

// The same rules apply to vertical concatenation:

// C = [int8(50); int16(5000)]
// C =
//     50
//    127

//       Note   You can find the maximum or minimum values for any MATLAB integer type using the intmax and intmin functions. For floating-point types, use realmax and realmin.

// Example of Combining Signed with Unsigned.   Now do the same exercise with signed and unsigned integers. Again, the left-most element determines the data type for all elements in the resulting matrix:

// A = [int8(-100) uint8(100)]
// A =
//    -100   100

// B = [uint8(100) int8(-100)]
// B =
//    100   0

// The element int8(-100) is set to zero because it is no longer signed.

// MATLAB evaluates each element prior to concatenating them into a combined array. In other words, the following statement evaluates to an 8-bit signed integer (equal to 50) and an 8-bit unsigned integer (unsigned -50 is set to zero) before the two elements are combined. Following the concatenation, the second element retains its zero value but takes on the unsigned int8 type:

// A = [int8(50), uint8(-50)]
// A =
//   50    0

// Combining Integer and Noninteger Data

// If you combine integers with double, single, or logical classes, all elements of the resulting matrix are given the data type of the left-most integer. For example, all elements of the following vector are set to int32:

// A = [TRUE pi int32(1000000) single(17.32) uint8(250)]

// Empty Matrices

// If you construct a matrix using empty matrix elements, the empty matrices are ignored in the resulting matrix:

// A = [5.36; 7.01; []; 9.44]
// A =
//     5.3600
//     7.0100
//     9.4400

// Concatenation Examples

// Here are some examples of data type conversion during matrix construction.

// Combining Single and Double Types.   Combining single values with double values yields a single matrix. Note that 5.73*10^300 is too big to be stored as a single, thus the conversion from double to single sets it to infinity. (The class function used in this example returns the data type for the input value):

// x = [single(4.5) single(-2.8) pi 5.73*10^300]
// x =
//     4.5000   -2.8000    3.1416       Inf

// class(x)              % Display the data type of x
// ans =
//    single

// Combining Integer and Double Types.   Combining integer values with double values yields an integer matrix. Note that the fractional part of pi is rounded to the nearest integer. (The int8 function used in this example converts its numeric argument to an 8-bit integer):

// x = [int8(21) int8(-22) int8(23) pi 45/6]
// x =
//     21   -22    23     3     7

// class(x)
// ans =
//    int8

// Combining Character and Double Types.   Combining character values with double values yields a character matrix. MATLAB converts the double elements in this example to their character equivalents:

// x = ['A' 'B' 'C' 68 69 70]
// x =
//    ABCDEF

// class(x)
// ans =
//    char

// Combining Logical and Double Types.   Combining logical values with double values yields a double matrix. MATLAB converts the logical TRUE and FALSE elements in this example to double:

// x = [TRUE FALSE FALSE pi sqrt(7)]
// x =
//     1.0000         0         0    3.1416    2.6458

// class(x)
// ans =
//    double


#endif

static BOOL AnyOfType(int pdata_size, const mxArray* pdata, DataClass t)
{
  int i;
  for (i = 0; i < pdata_size; i++) {
    if (pdata[i].classID == t) {
      return TRUE;
    }
  }
  return FALSE;
}

static DataClass ComputeCatType(int pdata_size, const mxArray* pdata)
{
  int i;
  if (AnyOfType(pdata_size, pdata, CellArray)) {
    return CellArray;
  }

  if (AnyOfType(pdata_size, pdata, Struct)) {
    return Struct;
  }

  if (AnyOfType(pdata_size, pdata, StringArray)) {
    return StringArray;
  }

  // Check for an integer type - choose the first one found.
  for (i = 0; i < pdata_size; i++) {
    if (pdata[i].classID == Int8 ||
        pdata[i].classID == UInt8 ||
        pdata[i].classID == Int16 ||
        pdata[i].classID == UInt16 ||
        pdata[i].classID == Int32 ||
        pdata[i].classID == UInt32 ||
        pdata[i].classID == Int64 ||
        pdata[i].classID == UInt64) {
      return pdata[i].classID;
    }
  }

  // So there are no integers, no cell, struct, strings
  // All that is left is logical, float, double.  Next we
  // look for floats, then doubles, and last, bools
  if (AnyOfType(pdata_size, pdata, Float)) {
    return Float;
  }

  if (AnyOfType(pdata_size, pdata, Double)) {
    return Double;
  }

  return Bool;
}


int mxConvertSubArrayImpl(const dim_type* dims, int* arr0, int clss0, const dim_type* pos0, const dim_type* step0, const int* arr1, int clss1, const dim_type* pos1, const dim_type* step1) {
  dim_type posz[mxMAXDIMS] = {0};
  int off0 = NDim_getoff(pos0, step0);
  int off1 = NDim_getoff(pos1, step1);
#define CLASSSIZEDEF(hh, ID, TYPE, FMT)  case ID: { dst_type* dst = (dst_type*)arr0+off0; TYPE* src = (TYPE*)arr1+off1; for (;NDim_next(dims, posz);) { dst[NDim_getoff(posz, step0)] = (dst_type)src[NDim_getoff(posz, step1)]; } } break;
#define CLASSSIZEDEF1(hh, ID, TYPE, FMT)  case ID: { typedef TYPE dst_type; switch (clss1) { hh; } } break;
  switch (clss0) {
  case mxCELL_CLASS:
  case mxSTRUCT_CLASS:
    ASSERT(0);
    return 0;
    CLASSSIZEDEFNUMBER(CLASSSIZEDEFNUMBER(hh, CLASSSIZEDEF), CLASSSIZEDEF1);
  }
#undef CLASSSIZEDEF
#undef CLASSSIZEDEF1
  return 0;
}

int mxConvertSubArray(mxArray* arr0, const dim_type* dims, const dim_type* pos0, const mxArray* arr1, const dim_type* pos1) {
  dim_type step0[mxMAXDIMS] = {0};
  dim_type step1[mxMAXDIMS] = {0};
  dim_type _pos0[mxMAXDIMS] = {0};
  dim_type _pos1[mxMAXDIMS] = {0};
  pos0 = pos0 ? pos0 : _pos0;
  pos1 = pos1 ? pos1 : _pos1;
  NDim_getstep(arr0->dims, step0);
  NDim_getstep(arr1->dims, step1);
  mxConvertSubArrayImpl(dims, arr0->data, arr0->classID, pos0, step0, arr1->data, arr1->classID, pos1, step1);
  return 0;
}

// FIX it here... [0 0] [3 0 4] --> [3 0 4]
int NCat_Impl(int pdata_size, const mxArray* pdata, int catdim, mxArray* out)
{
  DataClass cls;
  BOOL userClassCase = FALSE;
  const char* classname = NULL;
  mxArray* data = NULL;
  int i, j, data_size=0;
  dim_type outdims[mxMAXDIMS] = {0};
  BOOL foundNonzero = FALSE;

  // Must have something to work with
  if (pdata_size == 0) {
    mxFreeArray(out);
    return 0;
  }

  // First pass, compute the output class
  cls = ComputeCatType(pdata_size, pdata);
  // Check for the case of concatenation of user defined classes
  userClassCase = FALSE;

  if ((cls == Struct) && mxIsUserClass(pdata)) {
    classname = className(pdata);

    for (i = 1; i < pdata_size; i++) {
      if (!mxIsUserClass(pdata+i) || 0 != strcmp(className(pdata+i), classname)) {
        throw_Exception("Cannot concatenate user defined classes of different type");
      }
    }

    userClassCase = TRUE;
  }

  data = CALLOC(mxArray, pdata_size);
  // Next pass, if we are in the case of a cell array, we convert the arrays to cells
  // We also strip the empties
  for (i = 0; i < pdata_size; i++) {
    if (pdata[i].dims[0]*pdata[i].dims[1]) {
      if (cls == CellArray) {
        if (pdata[i].classID == CellArray) {
          mxCopyArray(data+i, pdata+i);
        }
        else {
          CellArrayFromArray(pdata+i, data+i);
        }
      }
      else {
        mxCopyArray(data+i, pdata+i);
      }
    }
  }

  {
    // Next step, we compute the output dimensions
    // Compute the output dimensions and validate each of the elements
    
    for (i = 0; i < data_size; i++) {
      const dim_type* eldims = data[i].dims;
      
      if (NDim_count(eldims) != 0) {
        ++data_size;
        if (!foundNonzero) {
          foundNonzero = TRUE;
          NDim_copy(outdims, eldims);
        }
        else {
          for (j = 0; j < NDims; j++) {
            if ((eldims[j] != outdims[j]) && (j != catdim)) {
              throw_Exception(QString("Mismatch in dimension %1 for elements being concatenated along dimension %2").arg(j + 1).arg(catdim + 1));
            }
          }
          
          outdims[catdim] += eldims[catdim];
        }
      }
    }
  }

  if (!foundNonzero) {
    NDim_zero(outdims);
  }
  
  mxSetNumericArray(out, mxMAXDIMS, outdims, cls, mxREAL, 0);
  // Compute the output dataclass
  if (data_size > 0) {
    dim_type pos[mxMAXDIMS] = {0};
    for (i=0; i<data_size; ++i) {
      mxArray* t = data+i;
      mxConvertSubArray(out, t->dims, pos, t, 0);
      pos[catdim] += t->dims[catdim];
    }

    if (userClassCase) {
      mxSetClassName(out, classname);
    }
  }
  mxFreeArrays(data, pdata_size);
  return 0;
}

int HCat(int pdata_size, const mxArray* pdata, mxArray* out)
{
  return NCat_Impl(pdata_size, pdata, 1, out);
}

int VCat(int pdata_size, const mxArray* pdata, mxArray* out)
{
  return NCat_Impl(pdata_size, pdata, 0, out);
}

int MatrixConstructor(int nrow, int ncol, const mxArray* pdata, mxArray* out)
{
  int i;
  mxArray* hcatresults = CALLOC(mxArray, nrow);
  for (i = 0; i < nrow; i++) {
    HCat(ncol, pdata+i, hcatresults+i);
  }

  mxFreeArrays(hcatresults, nrow);
  VCat(nrow, hcatresults, out);
  return 0;
}

int CellConstructor(int nrow, int ncol, const mxArray* data, mxArray* out)
{
  int i;
  mxArray* hcatresults = CALLOC(mxArray, nrow);

  for (i = 0; i < nrow; i++) {
    CellArrayFromArrayVector(data+i*ncol, ncol, hcatresults+i);
  }

  return VCat(nrow, hcatresults, out);
}

#if 0
mxArray StructConstructor(const vstr_t* fnames, const ArrayVector& values)
{
  if (fnames.size() != values.size()) {
    throw_Exception("Number of field names must match number of values in structure constructor.");
  }

  /**
   * First, we have to make sure that each entry of "values" have
   *  1.  cell arrays of the same size,
   *  2.  single element cell arrays,
   *  3.  single values.
   */
  NTuple dims;
  BOOL nonSingularFound = FALSE;

  for (i = 0; i < values.size(); i++) {
    /**
     * Check the type of the entry.  If its a non-cell array, then
     * then ignore this entry.
     */
    if (values[i].classID == CellArray) {
      /**
       * This is a cell-array, so look for non-scalar cell-arrays.
       */
      if (!values[i] mxIsScalar()) {
        if (!nonSingularFound) {
          nonSingularFound = TRUE;
          dims = values[i]->dims;
        }
        else if (dims != values[i]->dims) {
          throw_Exception("mxArray dimensions of non-scalar entries must agree in structure construction.");
        }
      }
    }
  }

  /**
   * At this point we can construct the dimensions of the output.
   */
  if (!nonSingularFound) {
    dims = NTuple(1, 1);
  }

  /**
   * The dimensions of the object have been identified.  Set the
   * dimensions of the object and the field names.  Then allocate
   * the space.
   */
  StructArray qp;

  for (i = 0; i < fnames.size(); i++) {
    BasicArray<mxArray> rp(dims);
    mxArray rval = values[i];

    if (!rval.isEmpty() && rval->issparse) {
      throw_Exception("Sparse arrays not supported for struct constructor.");
    }

    if (rval.isEmpty()) {
      rp.fill(rval);
    }
    else {
      if (rval->classID == CellArray) {
        if (rval mxIsScalar()) {
          rp.fill(ArrayFromCellArray(rval));
        }
        else {
          rp = rval.constReal<mxArray>();
        }
      }
      else {
        rp.fill(rval);
      }
    }

    qp.insert(fnames[i], rp);
  }

  qp.updateDims();
  return mxArray(qp);
}

template <typename T>
static BOOL RealAllZeros(const mxArray* t)
{
  if (t mxIsScalar()) {
    return (t.constRealScalar<T>() == 0);
  }
  else if (t->issparse) {
    return AllZeros(t.constRealSparse<T>());
  }
  else {
    return AllZeros(t.constReal<T>());
  }
}

#define MacroRealZeros(ctype,cls)   \
  case cls: return RealAllZeros<ctype>(t);

BOOL RealAllZeros(const mxArray* t)
{
  switch (t->classID) {
  default:
    throw_Exception("Unhandled case -- argument must be numeric");
    MacroExpandCasesSimple(MacroRealZeros);
  }
}

#endif
static BOOL RealAllNonZeros(const mxArray* arr)
{
  BOOL ret = FALSE;
  if (mxIsEmpty(arr)) {
    return FALSE;
  }

  if (mxIsScalar(arr)) {
    double t1;
    t1 = mxGetScalar(arr);
    ret = (t1 != 0);
  }
  else {
    mxArray r[1] = {0};
    SplitReal(arr, r);
    ret = AllNonZeros(r);
    mxFreeArray(r);
  }
  return ret;
}

#if 0

template <typename T>
static mxArray TPermute(const mxArray* x, const NTuple& dp)
{
  if (x mxIsAllReal()) {
    return mxArray(Permute(x.constReal<T>(), dp));
  }
  else {
    return mxArray(Permute(x.constReal<T>(), dp), Permute(x.constImag<T>(), dp));
  }
}

static mxArray StructPermute(const mxArray* x, const NTuple& dp)
{
  const StructArray& sp(x.constStructPtr());
  StructArray ret;

  for (i = 0; i < sp.fieldCount(); i++) {
    ret.insert(sp.fieldName(i), Permute(sp[i], dp));
  }

  ret.updateDims();
  return mxArray(ret);
}

#define MacroPermute(ctype,cls)     \
  case cls: return TPermute<ctype>(A,dp);

mxArray Permute(const mxArray* A, const NTuple& dp)
{
  // Check for an identity permutation
  NTuple outdims(A->dims.permute(dp));
  BOOL id_perm = TRUE;

  for (i = 0; i < NDims; i++)
    if (dp[i] != (i + 1)) {
      id_perm = FALSE;
    }

  if (id_perm) {
    return A;
  }

  if (A.is2D()) {
    BOOL isTranspose = TRUE;

    for (i = 2; i < NDims; i++)
      if (dp[i] != (i + 1)) {
        isTranspose = FALSE;
      }

    if (dp[0] != 2) {
      isTranspose = FALSE;
    }

    if (dp[1] != 1) {
      isTranspose = FALSE;
    }

    if (isTranspose) {
      return Transpose(A);
    }
  }

  if (A->issparse) {
    throw_Exception("illegal permutation for sparse arrays");
  }

  if (A mxIsScalar()) {
    return A;
  }

  if (A.isEmpty()) {
    return mxArray(A->classID, outdims);
  }

  switch (A->classID) {
  default:
    throw_Exception("Unsupported dataclass for permute");

  case Struct:
    return StructPermute(A, dp);
    MacroExpandCasesAll(MacroPermute);
  }
}

#endif

static BOOL mxIsString(const mxArray* arr) {
  return (arr->classID == StringArray);
}
static BOOL mxIsReferenceType(const mxArray* arr) {
  return ((arr->classID == Invalid) || (arr->classID == CellArray) ||
    (arr->classID == Struct));
}
// s is the switch value to test, x is the values to test against
BOOL TestForCaseMatch(const mxArray* s, const mxArray* x)
{
  if (!(mxIsScalar(s) || mxIsString(s))) {
    throw_Exception("Switch argument must be a scalar or a string");
  }

  // And we had better not be a reference type
  if (mxIsReferenceType(s)) {
    throw_Exception("Switch argument cannot be a reference type (struct or cell array)");
  }

  // If x is a scalar, we just need to call the scalar version
  if (((x->classID != CellArray) && mxIsScalar(x)) || mxIsString(x)) {
    double t1 = mxGetScalar(s);
    double t2 = mxGetScalar(x);
    return t1 == t2;
  }

#if 0
  const BasicArray<mxArray>& xp(x.constReal<mxArray>());
  xp_length = ;

  for (i = 0; i < xp_length; i++) {
    if (s == xp[i]) {
      return TRUE;
    }
  }
  
#endif
  return FALSE;
}
#if 0

static BasicArray<index_t> DecompressCCSCols(const BasicArray<index_t>& colstart, index_t len)
{
  BasicArray<index_t> retval(NTuple(1, len));
  index_t p = 1;

  for (index_t i = 2; i <= colstart.length(); i++)
    for (index_t j = 1; j <= (colstart[i] - colstart[i - 1]); j++) {
      retval[p] = index_t(i - 2);
      p = p + 1;
    }

  return retval;
}

template <typename T>
static SparseMatrix<T> CCSToSparse(const BasicArray<index_t>& rowstart,
    const BasicArray<index_t>& colstart,
    const mxArray* Adata)
{
  BasicArray<index_t> cols(DecompressCCSCols(colstart, Adata.length()));
  SparseMatrix<T> retvec(NTuple(MaxValue(rowstart) + 1, MaxValue(cols) + 1));

  for (index_t i = 1; i <= cols.length(); i++) {
    retvec[NTuple(rowstart[i] + 1, cols[i] + 1)] = Adata[i];
  }

  return retvec;
}

template <typename T>
static mxArray IJVToSparse(const BasicArray<index_t>& irp,
    const BasicArray<index_t>& jcp,
    const mxArray* pr, const mxArray* pi,
    BOOL complexFlag)
{
  if (!complexFlag) {
    const mxArray* pr_dense(pr.asDenseArray());
    return mxArray(CCSToSparse(irp, jcp, pr_dense.constReal<T>()));
  }
  else {
    const mxArray* pr_dense(pr.asDenseArray());
    const mxArray* pi_dense(pi.asDenseArray());
    return mxArray(CCSToSparse(irp, jcp, pr_dense.constReal<T>()),
        CCSToSparse(irp, jcp, pi_dense.constImag<T>()));
  }
}

#define MacroIJVToSparse(ctype,cls)         \
  case cls: return IJVToSparse<ctype>(irp,jcp,pr,pi,complexFlag);

mxArray MatIJVToSparse(const mxArray* ir, const mxArray* jc,
    const mxArray* pr, const mxArray* pi, BOOL complexFlag)
{
  const BasicArray<index_t>& irp(ir.constReal<index_t>());
  const BasicArray<index_t>& jcp(jc.constReal<index_t>());

  switch (pr->classID) {
  default:
    throw_Exception("Unsupported data type " + pr.className() + " in sparse constructor");
    MacroExpandCasesSimple(MacroIJVToSparse);
  }
}

template <typename T>
int SparseToIJVReal(const SparseMatrix<T>& x, mxArray* rows, mxArray* cols, mxArray* vals)
{
  QVector<uint32> rowstart;
  QVector<uint32> colstart;
  QVector<T> xdata;
  SparseToCCS(x, rowstart, colstart, xdata);
  rows = mxArray(ToBasicArray(rowstart)).toClass(Index);
  cols = mxArray(ToBasicArray(colstart)).toClass(Index);
  vals = mxArray(ToBasicArray(xdata));
}

template <typename T>
int SparseToIJVComplex(const SparseMatrix<T>& xr, const SparseMatrix<T>& xi,
    mxArray* rows, mxArray* cols, mxArray* vals)
{
  QVector<uint32> rowstart;
  QVector<uint32> colstart;
  QVector<T> xdata_real;
  QVector<T> xdata_imag;
  SparseToCCS(xr, rowstart, colstart, xdata_real);
  SparseToCCS(xi, rowstart, colstart, xdata_imag);
  rows = mxArray(ToBasicArray(rowstart)).toClass(Index);
  cols = mxArray(ToBasicArray(colstart)).toClass(Index);
  vals = mxArray(ToBasicArray(xdata_real), ToBasicArray(xdata_imag));
}

template <typename T>
int SparseToIJV(const mxArray* x, mxArray* rows, mxArray* cols, mxArray* vals)
{
  if (x mxIsAllReal()) {
    SparseToIJVReal(x.constRealSparse<T>(), rows, cols, vals);
  }
  else {
    SparseToIJVComplex(x.constRealSparse<T>(), x.constImagSparse<T>(), rows, cols, vals);
  }
}

#define MacroSparseToIJV(ctype,cls)       \
  case cls: return SparseToIJV<ctype>(x,rows,cols,vals);

int SparseToIJVMat(const mxArray* x, mxArray* rows, mxArray* cols, mxArray* vals)
{
  if (!x->issparse) {
    throw_Exception("x must be sparse");
  }

  switch (x->classID) {
  default:
    throw_Exception("unhandled case");
    MacroExpandCasesSimple(MacroSparseToIJV);
  }
}

#undef MacroSparseToIJV

template <typename T>
mxArray ToSparse(const mxArray* data)
{
  if (data mxIsAllReal()) {
    return mxArray(ToRealSparse<T>(data));
  }
  else {
    return mxArray(ToRealSparse<T>(data), ToImagSparse<T>(data));
  }
}

#define MacroToSparse(ctype,cls)    \
  case cls: return ToSparse<ctype>(data);

mxArray ToSparse(const mxArray* data)
{
  switch (data->classID) {
  default:
    throw_Exception("unhandled case for sparse");
    MacroExpandCases(MacroToSparse);
  }
}

#undef MacroToSparse

BOOL Equals(const vstr_t* a, const vstr_t* b)
{
  if (a.size() != b.size()) {
    return FALSE;
  }

  for (i = 0; i < a.size(); i++)
    if (a[i] != b[i]) {
      return FALSE;
    }

  return TRUE;
}

#endif
