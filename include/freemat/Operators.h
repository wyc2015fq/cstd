
#if 0


// Assumes that the operation cannot create complex values from real ones
//
// So the table looks like this (order is unimportant)
// a  b  c v
// s  s  s s
// s  d  s s
// s  c  s s
// s  l  s s
// s  i  e
// d  d  d d
// d  c  d d
// d  l  d d
// d  i  i d
// i1 i2 e
// c  i  e
// c  l  d d
// c  c  d d
// l  l  d d
//
// The resulting rule:
//   default: c is double, v is double
//   if (a or b is single), c is single, v is single
//   if (a or b is an integer)
//     if (same type, or one is a double),
//       c is integer
//     else illegal.
//
// if (a or b single) --> v single, c single
// else {
//   v double
//   if (a and b not integer) --> c double, else c integer
// }
//
// Arithmetic Operations on Floating-Point Numbers
// This section describes which classes you can use in arithmetic operations with floating-point numbers.
// Double-Precision.   You can perform basic arithmetic operations with double and any of the following other classes. When one or more operands is an integer (scalar or array), the double operand must be a scalar. The result is of type double, except where noted otherwise:
//       single  The result is of type single
//       double
//       int* or uint*  The result has the same data type as the integer operand
//       char
//       logical
// This example performs arithmetic on data of types char and double. The result is of type double:
// c = 'uppercase' - 32;
// class(c)
// ans =
//    double
// char(c)
// ans =
//    UPPERCASE
// Single-Precision.   You can perform basic arithmetic operations with single and any of the following other classes. The result is always single:
//       single
//       double
//       char
//       logical
// Arithmetic Operations on Integer Classes
// MATLAB can perform integer arithmetic on the following types of data:
//       Integers or integer arrays of the same integer data type. This yields a result that has the same data type as the operands:
//          x = uint32([132 347 528]) .* uint32(75);
//       Integers or integer arrays and scalar double-precision floating-point numbers. This yields a result that has the same data type as the integer operands:
//          x = uint32([132 347 528]) .* 75.49;
// For all binary operations in which one operand is an array of integer data type and the other is a scalar double, MATLAB computes the operation using elementwise double-precision arithmetic, and then converts the result back to the original integer data type.

static BOOL IsIntegerDataClass(const mxArray* Ain)
{
  return ((Ain->classID >= Int8) && (Ain->classID <= UInt64));
}


mxArray DotOp(const mxArray* Ain, const mxArray* Bin)
{
  if ((Ain->issparse || Bin->issparse) && !IsZeroPreserving<Op>()) {
    return ToSparse(DotOp<Op>(Ain.asDenseArray(), Bin.asDenseArray()));
  }

  DataClass via_type;
  DataClass out_type;
  ComputeTypes(Ain, Bin, via_type, out_type);

  if ((out_type == Float) && (IsIntegerDataClass(Ain) ||
      IsIntegerDataClass(Bin))) {
    throw_Exception("Cannot combine single precision and integer class data");
  }

  if (IsIntegerDataClass(Ain) && IsIntegerDataClass(Bin) &&
      Ain->classID != Bin->classID) {
    throw_Exception("Cannot combine data of different integer data classes");
  }

  if (IsIntegerDataClass(Ain)) {
    out_type = Ain->classID;
  }

  if (IsIntegerDataClass(Bin)) {
    out_type = Bin->classID;
  }

  mxArray F;

  if (via_type == Float) {
    F = DotOp<float, Op>(Ain, Bin, Float);
  }
  else {
    F = DotOp<double, Op>(Ain, Bin, Double);
  }

  return F.toClass(out_type);
}

template <class Op>
mxArray BoolOp(const mxArray* Ain, const mxArray* Bin)
{
  return DotOp<BOOL, Op>(Ain, Bin, Bool).toClass(Bool);
}

// Comparison op -- uses real part only (used by ordinal operators)
template <typename T, class Op>
static mxArray CmpOp(const mxArray* Ain, const mxArray* Bin, DataClass Tclass)
{
  mxArray Acast(Ain.toClass(Tclass));
  mxArray Bcast(Bin.toClass(Tclass));
  mxArray F(Bool, NTuple(0, 0));

  if (Acast->issparse && Bcast->issparse) {
    if (Acast->dims != Bcast->dims) {
      throw_Exception("size mismatch in arguments to binary operator");
    }

    F = DotOp<BOOL, T, Op>(Acast.constRealSparse<T>(),
        Bcast.constRealSparse<T>());
    return F;
  }

  if (!Acast mxIsScalar()) {
    Acast = Acast.asDenseArray();
  }

  if (!Bcast mxIsScalar()) {
    Bcast = Bcast.asDenseArray();
  }

  if (Acast mxIsScalar() && Bcast mxIsScalar()) {
    F = mxArray::create(Op::func(Acast.constRealScalar<T>(),
        Bcast.constRealScalar<T>()));
  }
  else if (Acast mxIsScalar()) {
    F = mxArray::create(Bool, Bcast->dims);
    BOOL* ret = F.real<BOOL>().data();
    const T& Ap = Acast.constRealScalar<T>();
    const T* Bp = Bcast.constReal<T>().constData();
    uint64 q = uint64(Bcast.length());

    for (uint64 i = 0; i < q; i++) {
      ret[i] = Op::func(Ap, Bp[i]);
    }
  }
  else if (Bcast mxIsScalar()) {
    F = mxArray::create(Bool, Acast->dims);
    BOOL* ret = F.real<BOOL>().data();
    const T* Ap = Acast.constReal<T>().constData();
    const T& Bp = Bcast.constRealScalar<T>();
    uint64 q = uint64(Acast.length());

    for (uint64 i = 0; i < q; i++) {
      ret[i] = Op::func(Ap[i], Bp);
    }
  }
  else {
    if (Acast->dims != Bcast->dims) {
      throw_Exception("size mismatch in arguments to binary operator");
    }

    F = mxArray::create(Bool, Acast->dims);
    BOOL* ret = F.real<BOOL>().data();
    const T* Ap = Acast.constReal<T>().constData();
    const T* Bp = Bcast.constReal<T>().constData();
    uint64 q = uint64(Acast.length());

    for (uint64 i = 0; i < q; i++) {
      ret[i] = Op::func(Ap[i], Bp[i]);
    }
  }

  return F;
}

template <class Op>
static mxArray CmpOp(const mxArray* Ain, const mxArray* Bin)
{
  DataClass via_type;
  DataClass out_type;
  ComputeTypes(Ain, Bin, via_type, out_type);
  mxArray F;

  // Check for non zero-preserving cmp functions
  if ((Ain->issparse || Bin->issparse) && !IsZeroPreservingCmp<Op>()) {
    return ToSparse(CmpOp<Op>(Ain.asDenseArray(), Bin.asDenseArray()));
  }

  if (via_type == Float) {
    F = CmpOp<float, Op>(Ain, Bin, Float);
  }
  else {
    F = CmpOp<double, Op>(Ain, Bin, Double);
  }

  return F.toClass(Bool);
}

// Comparison operator -- uses real and imaginary parts
template <typename T, class Op>
static mxArray EqOp(const mxArray* Ain, const mxArray* Bin, DataClass Tclass)
{
  mxArray Acast(Ain.toClass(Tclass));
  mxArray Bcast(Bin.toClass(Tclass));
  mxArray F(Bool, NTuple(0, 0));

  if (Acast->issparse && Bcast->issparse) {
    if (Acast->dims != Bcast->dims) {
      throw_Exception("size mismatch in arguments to binary operator");
    }

    if (Bcast.allReal() && Acast.allReal()) {
      F = DotOp<BOOL, T, Op>(Acast.constRealSparse<T>(),
          Bcast.constRealSparse<T>());
    }
    else {
      Acast.forceComplex();
      Bcast.forceComplex();
      F = DotOp<BOOL, T, Op>(Acast.constRealSparse<T>(),
          Acast.constImagSparse<T>(),
          Bcast.constRealSparse<T>(),
          Bcast.constImagSparse<T>());
    }

    return F;
  }

  if (!Acast mxIsScalar()) {
    Acast = Acast.asDenseArray();
  }

  if (!Bcast mxIsScalar()) {
    Bcast = Bcast.asDenseArray();
  }

  if (Acast mxIsScalar() && Bcast mxIsScalar()) {
    if (Acast.allReal() && Bcast.allReal()) {
      F = mxArray::create(Op::func(Acast.constRealScalar<T>(),
          Bcast.constRealScalar<T>()));
    }
    else {
      Acast.forceComplex();
      Bcast.forceComplex();
      F = mxArray::create(Op::func(Acast.constRealScalar<T>(),
          Acast.constImagScalar<T>(),
          Bcast.constRealScalar<T>(),
          Bcast.constImagScalar<T>()));
    }
  }
  else if (Acast mxIsScalar()) {
    if (Acast.allReal() && Bcast.allReal()) {
      F = mxArray::create(Bool, Bcast->dims);
      BOOL* ret = F.real<BOOL>().data();
      const T& Ap = Acast.constRealScalar<T>();
      const T* Bp = Bcast.constReal<T>().constData();
      uint64 q = uint64(Bcast.length());

      for (uint64 i = 0; i < q; i++) {
        ret[i] = Op::func(Ap, Bp[i]);
      }
    }
    else {
      Acast.forceComplex();
      Bcast.forceComplex();
      F = mxArray::create(Bool, Bcast->dims);
      BOOL* ret = F.real<BOOL>().data();
      const T& Ar = Acast.constRealScalar<T>();
      const T& Ai = Acast.constImagScalar<T>();
      const T* Br = Bcast.constReal<T>().constData();
      const T* Bi = Bcast.constImag<T>().constData();
      uint64 q = uint64(Bcast.length());

      for (uint64 i = 0; i < q; i++) {
        ret[i] = Op::func(Ar, Ai, Br[i], Bi[i]);
      }
    }
  }
  else if (Bcast mxIsScalar()) {
    if (Bcast.allReal() && Acast.allReal()) {
      F = mxArray::create(Bool, Acast->dims);
      BOOL* ret = F.real<BOOL>().data();
      const T* Ap = Acast.constReal<T>().constData();
      const T& Bp = Bcast.constRealScalar<T>();
      uint64 q = uint64(Acast.length());

      for (uint64 i = 0; i < q; i++) {
        ret[i] = Op::func(Ap[i], Bp);
      }
    }
    else {
      Acast.forceComplex();
      Bcast.forceComplex();
      F = mxArray::create(Bool, Acast->dims);
      BOOL* ret = F.real<BOOL>().data();
      const T* Ar = Acast.constReal<T>().constData();
      const T* Ai = Acast.constImag<T>().constData();
      const T& Br = Bcast.constRealScalar<T>();
      const T& Bi = Bcast.constImagScalar<T>();
      uint64 q = uint64(Acast.length());

      for (uint64 i = 0; i < q; i++) {
        ret[i] = Op::func(Ar[i], Ai[i], Br, Bi);
      }
    }
  }
  else {
    if (Acast->dims != Bcast->dims) {
      throw_Exception("size mismatch in arguments to binary operator");
    }

    if (Bcast.allReal() && Acast.allReal()) {
      F = mxArray::create(Bool, Acast->dims);
      BOOL* ret = F.real<BOOL>().data();
      const T* Ap = Acast.constReal<T>().constData();
      const T* Bp = Bcast.constReal<T>().constData();
      uint64 q = uint64(Acast.length());

      for (uint64 i = 0; i < q; i++) {
        ret[i] = Op::func(Ap[i], Bp[i]);
      }
    }
    else {
      Acast.forceComplex();
      Bcast.forceComplex();
      F = mxArray::create(Bool, Acast->dims);
      BOOL* ret = F.real<BOOL>().data();
      const T* Ar = Acast.constReal<T>().constData();
      const T* Ai = Acast.constImag<T>().constData();
      const T* Br = Bcast.constReal<T>().constData();
      const T* Bi = Bcast.constImag<T>().constData();
      uint64 q = uint64(Acast.length());

      for (uint64 i = 0; i < q; i++) {
        ret[i] = Op::func(Ar[i], Ai[i], Br[i], Bi[i]);
      }
    }
  }

  return F;
}

template <class Op>
static mxArray EqOp(const mxArray* Ain, const mxArray* Bin)
{
  if ((Ain->issparse || Bin->issparse) && !IsZeroPreservingCmp<Op>()) {
    return (ToSparse(EqOp<Op>(Ain.asDenseArray(), Bin.asDenseArray())));
  }

  DataClass via_type;
  DataClass out_type;
  ComputeTypes(Ain, Bin, via_type, out_type);
  mxArray F;

  if (via_type == Float) {
    F = EqOp<float, Op>(Ain, Bin, Float);
  }
  else {
    F = EqOp<double, Op>(Ain, Bin, Double);
  }

  return F.toClass(Bool);
}

// Real --> Real
template <typename S, typename T, class Op>
static SparseMatrix<S> UnaryOp(const SparseMatrix<T>& A)
{
  ConstSparseIterator<T> aspin(&A);
  SparseMatrix<S> retval(A->dims);

  while (aspin.isValid()) {
    retval.set(aspin.pos(), Op::func(aspin.value()));
    aspin.next();
  }

  return retval;
}

// Complex --> Complex
template <typename S, typename T, class Op>
static void UnaryOp(const SparseMatrix<T>& A_real,
    const SparseMatrix<T>& A_imag,
    SparseMatrix<S>& C_real,
    SparseMatrix<S>& C_imag)
{
  C_real = SparseMatrix<S>(A_real->dims);
  C_imag = SparseMatrix<S>(A_imag->dims);
  ConstComplexSparseIterator<T> a_spin(&A_real, &A_imag);
  NTuple dim(A_real->dims);

  while (a_spin.isValid()) {
    S value_c_real, value_c_imag;
    Op::func(a_spin.realValue(), a_spin.imagValue(),
        value_c_real, value_c_imag);
    C_real.set(a_spin.pos(), value_c_real);
    C_imag.set(a_spin.pos(), value_c_imag);
    a_spin.next();
  }
}

// Perform the operation via a typed intermediary
template <typename T, class Op>
static mxArray UnaryOp(const mxArray* Ain, DataClass Tclass)
{
  mxArray Acast(Ain.toClass(Tclass));
  mxArray F(Tclass, NTuple(0, 0));

  if (Acast->issparse) {
    if (Acast.allReal()) {
      F = UnaryOp<T, T, Op>(Acast.constRealSparse<T>());
    }
    else {
      SparseMatrix<T> Freal(Acast->dims);
      SparseMatrix<T> Fimag(Acast->dims);
      UnaryOp<T, T, Op>(Acast.constRealSparse<T>(),
          Acast.constImagSparse<T>(),
          Freal, Fimag);
      F = mxArray(Freal, Fimag);
    }

    return F;
  }

  if (!Acast mxIsScalar()) {
    Acast = Acast.asDenseArray();
  }

  if (Acast mxIsScalar()) {
    if (Acast.allReal()) {
      F = mxArray::create(Op::func(Acast.constRealScalar<T>()));
    }
    else {
      F = mxArray::create(T(0), T(0));
      Op::func(Acast.constRealScalar<T>(),
          Acast.constImagScalar<T>(),
          F.realScalar<T>(), F.imagScalar<T>());
    }
  }
  else {
    if (Acast.allReal()) {
      F = mxArray(Tclass, Acast->dims);
      T* ret = F.real<T>().data();
      const T* Ap = Acast.constReal<T>().constData();
      uint64 q = uint64(Acast.length());

      for (uint64 i = 0; i < q; i++) {
        ret[i] = Op::func(Ap[i]);
      }
    }
    else {
      F = mxArray(Tclass, Acast->dims);
      T* Cr = F.real<T>().data();
      T* Ci = F.imag<T>().data();
      const T* Ar = Acast.constReal<T>().constData();
      const T* Ai = Acast.constImag<T>().constData();
      uint64 q = uint64(Acast.length());

      for (uint64 i = 0; i < q; i++) {
        Op::func(Ar[i], Ai[i], Cr[i], Ci[i]);
      }
    }
  }

  return F;
}

template <class Op>
static mxArray UnaryOp(const mxArray* Ain)
{
  // Check for non zero-preserving unary functions
  if (Ain->issparse && !IsZeroPreservingUnary<Op>()) {
    return ToSparse(UnaryOp<Op>(Ain.asDenseArray()));
  }

  if (Ain->classID == Float) {
    return UnaryOp<float, Op>(Ain, Float);
  }
  else {
    return UnaryOp<double, Op>(Ain, Double);
  }
}

template <typename T, class Op>
static mxArray VectorOp(const mxArray* real,
    const mxArray* imag,
    index_t out, int dim)
{
  NTuple outdims(real->dims);
  outdims[dim] = out;
  BasicArray<T> F_real(outdims);
  BasicArray<T> F_imag(outdims);
  ConstBasicIterator<T> source_real(&real, dim);
  ConstBasicIterator<T> source_imag(&imag, dim);
  BasicIterator<T> dest_real(&F_real, dim);
  BasicIterator<T> dest_imag(&F_imag, dim);
  BasicArray<T> in_buffer_real(NTuple(real->dims[dim], 1));
  BasicArray<T> in_buffer_imag(NTuple(imag->dims[dim], 1));
  BasicArray<T> out_buffer_real(NTuple(out, 1));
  BasicArray<T> out_buffer_imag(NTuple(out, 1));

  while (source_real.isValid() && dest_real.isValid()) {
    for (index_t i = 1; i <= source_real.size(); i++) {
      in_buffer_real[i] = source_real.get();
      in_buffer_imag[i] = source_imag.get();
      source_real.next();
      source_imag.next();
    }

    Op::func(in_buffer_real, in_buffer_imag,
        out_buffer_real, out_buffer_imag);

    for (index_t i = 1; i <= out; i++) {
      dest_real.set(out_buffer_real[i]);
      dest_imag.set(out_buffer_imag[i]);
      dest_real.next();
      dest_imag.next();
    }

    source_real.nextSlice();
    source_imag.nextSlice();
    dest_real.nextSlice();
    dest_imag.nextSlice();
  }

  return mxArray(F_real, F_imag);
}

template <typename T, class Op>
static mxArray VectorOp(const SparseMatrix<T>& real, index_t out, int dim)
{
  if (dim == 0) {
    ConstSparseIterator<T> spin_real(&real);
    NTuple outdims(real->dims);
    outdims[dim] = out;
    SparseMatrix<T> retval(outdims);

    while (spin_real.isValid()) {
      SparseSlice<T> this_col;
      index_t col_number = spin_real.col();
      Op::func(spin_real, this_col);
      retval.data()[col_number] = this_col;
    }

    return mxArray(retval);
  }
  else {
    return Transpose(VectorOp<T, Op>(Transpose(real), out, 0));
  }
}

template <typename T, class Op>
static mxArray VectorOp(const SparseMatrix<T>& real,
    const SparseMatrix<T>& imag,
    index_t out, int dim)
{
  if (dim == 0) {
    ConstComplexSparseIterator<T> spin_complex(&real, &imag);
    NTuple outdims(real->dims);
    outdims[dim] = out;
    SparseMatrix<T> retval_real(outdims);
    SparseMatrix<T> retval_imag(outdims);

    while (spin_complex.isValid()) {
      SparseSlice<T> this_real_col;
      SparseSlice<T> this_imag_col;
      index_t col_number = spin_complex.col();
      Op::func(spin_complex, this_real_col, this_imag_col);
      retval_real.data()[col_number] = this_real_col;
      retval_imag.data()[col_number] = this_imag_col;
    }

    return mxArray(retval_real, retval_imag);
  }
  else
    return Transpose(VectorOp<T, Op>(Transpose(real),
        Transpose(imag), out, 0));
}

template <typename T, class Op>
static mxArray VectorOp(const mxArray* real, index_t out, int dim)
{
  NTuple outdims(real->dims);
  outdims[dim] = out;
  BasicArray<T> F(outdims);
  ConstBasicIterator<T> source(&real, dim);
  BasicIterator<T> dest(&F, dim);
  BasicArray<T> in_buffer(NTuple(real->dims[dim], 1));
  BasicArray<T> out_buffer(NTuple(out, 1));

  while (source.isValid() && dest.isValid()) {
    for (index_t i = 1; i <= source.size(); i++) {
      in_buffer[i] = source.get();
      source.next();
    }

    Op::func(in_buffer, out_buffer);

    for (index_t i = 1; i <= out; i++) {
      dest.set(out_buffer[i]);
      dest.next();
    }

    source.nextSlice();
    dest.nextSlice();
  }

  return mxArray(F);
}

template <typename T, class Op>
static mxArray VectorOp(const mxArray* Ain, index_t out, int dim, DataClass Tclass)
{
  mxArray Acast(Ain.toClass(Tclass));

  if (Acast->issparse) {
    if (Acast.allReal()) {
      return VectorOp<T, Op>(Acast.constRealSparse<T>(), out, dim);
    }
    else
      return VectorOp<T, Op>(Acast.constRealSparse<T>(),
          Acast.constImagSparse<T>(), out, dim);
  }
  else {
    if (Acast mxIsScalar()) {
      Acast = Acast.asDenseArray();
    }

    if (Acast.allReal()) {
      return VectorOp<T, Op>(Acast.constReal<T>(), out, dim);
    }
    else {
      return VectorOp<T, Op>(Acast.constReal<T>(), Acast.constImag<T>(), out, dim);
    }
  }
}

template <class Op>
static mxArray VectorOp(const mxArray* Ain, int out, int dim)
{
  out = qMax(0, out);

  if (Ain->classID == Float) {
    return VectorOp<float, Op>(Ain, out, dim, Float);
  }
  else {
    return VectorOp<double, Op>(Ain, out, dim, Double);
  }
}


template <typename T, class Op>
static mxArray VectorOpDynamic(const mxArray* real,
    const mxArray* imag,
    index_t out, int dim, Op& op)
{
  NTuple outdims(real->dims);
  outdims[dim] = out;
  BasicArray<T> F_real(outdims);
  BasicArray<T> F_imag(outdims);
  ConstBasicIterator<T> source_real(&real, dim);
  ConstBasicIterator<T> source_imag(&imag, dim);
  BasicIterator<T> dest_real(&F_real, dim);
  BasicIterator<T> dest_imag(&F_imag, dim);
  BasicArray<T> in_buffer_real(NTuple(real->dims[dim], 1));
  BasicArray<T> in_buffer_imag(NTuple(imag->dims[dim], 1));
  BasicArray<T> out_buffer_real(NTuple(out, 1));
  BasicArray<T> out_buffer_imag(NTuple(out, 1));

  while (source_real.isValid() && dest_real.isValid()) {
    for (index_t i = 1; i <= source_real.size(); i++) {
      in_buffer_real[i] = source_real.get();
      in_buffer_imag[i] = source_imag.get();
      source_real.next();
      source_imag.next();
    }

    op.func(in_buffer_real, in_buffer_imag,
        out_buffer_real, out_buffer_imag);

    for (index_t i = 1; i <= out; i++) {
      dest_real.set(out_buffer_real[i]);
      dest_imag.set(out_buffer_imag[i]);
      dest_real.next();
      dest_imag.next();
    }

    source_real.nextSlice();
    source_imag.nextSlice();
    dest_real.nextSlice();
    dest_imag.nextSlice();
  }

  return mxArray(F_real, F_imag);
}

template <typename T, class Op>
static mxArray VectorOpDynamic(const SparseMatrix<T>& real, index_t out,
    int dim, Op& op)
{
  if (dim == 0) {
    ConstSparseIterator<T> spin_real(&real);
    NTuple outdims(real->dims);
    outdims[dim] = out;
    SparseMatrix<T> retval(outdims);

    while (spin_real.isValid()) {
      SparseSlice<T> this_col;
      index_t col_number = spin_real.col();
      op.func(spin_real, this_col);
      retval.data()[col_number] = this_col;
    }

    return mxArray(retval);
  }
  else {
    return Transpose(VectorOpDynamic<T, Op>(Transpose(real), out, 0, op));
  }
}

template <typename T, class Op>
static mxArray VectorOpDynamic(const SparseMatrix<T>& real,
    const SparseMatrix<T>& imag,
    index_t out, int dim,
    Op& op)
{
  if (dim == 0) {
    ConstComplexSparseIterator<T> spin_complex(&real, &imag);
    NTuple outdims(real->dims);
    outdims[dim] = out;
    SparseMatrix<T> retval_real(outdims);
    SparseMatrix<T> retval_imag(outdims);

    while (spin_complex.isValid()) {
      SparseSlice<T> this_real_col;
      SparseSlice<T> this_imag_col;
      index_t col_number = spin_complex.col();
      op.func(spin_complex, this_real_col, this_imag_col);
      retval_real.data()[col_number] = this_real_col;
      retval_imag.data()[col_number] = this_imag_col;
    }

    return mxArray(retval_real, retval_imag);
  }
  else
    return Transpose(VectorOpDynamic<T, Op>(Transpose(real),
        Transpose(imag), out, 0, op));
}

template <typename T, class Op>
static mxArray VectorOpDynamic(const mxArray* real,
    index_t out, int dim,
    Op& op)
{
  NTuple outdims(real->dims);
  outdims[dim] = out;
  BasicArray<T> F(outdims);
  ConstBasicIterator<T> source(&real, dim);
  BasicIterator<T> dest(&F, dim);
  BasicArray<T> in_buffer(NTuple(real->dims[dim], 1));
  BasicArray<T> out_buffer(NTuple(out, 1));

  while (source.isValid() && dest.isValid()) {
    for (index_t i = 1; i <= source.size(); i++) {
      in_buffer[i] = source.get();
      source.next();
    }

    op.func(in_buffer, out_buffer);

    for (index_t i = 1; i <= out; i++) {
      dest.set(out_buffer[i]);
      dest.next();
    }

    source.nextSlice();
    dest.nextSlice();
  }

  return mxArray(F);
}

template <typename T, class Op>
static mxArray VectorOpDynamic(const mxArray* Ain, index_t out, int dim,
    DataClass Tclass, Op& op)
{
  mxArray Acast(Ain.toClass(Tclass));

  if (Acast->issparse) {
    if (Acast.allReal()) {
      return VectorOpDynamic<T, Op>(Acast.constRealSparse<T>(), out, dim, op);
    }
    else
      return VectorOpDynamic<T, Op>(Acast.constRealSparse<T>(),
          Acast.constImagSparse<T>(), out, dim, op);
  }
  else {
    if (Acast mxIsScalar()) {
      Acast = Acast.asDenseArray();
    }

    if (Acast.allReal()) {
      return VectorOpDynamic<T, Op>(Acast.constReal<T>(), out, dim, op);
    }
    else {
      return VectorOpDynamic<T, Op>(Acast.constReal<T>(), Acast.constImag<T>(), out, dim, op);
    }
  }
}

template <class Op>
static mxArray VectorOpDynamic(const mxArray* Ain, int out, int dim,
    Op& op)
{
  out = qMax(0, out);

  if (Ain->classID == Float) {
    return VectorOpDynamic<float, Op>(Ain, out, dim, Float, op);
  }
  else {
    return VectorOpDynamic<double, Op>(Ain, out, dim, Double, op);
  }
}

template <typename T, class Op>
static mxArray BiVectorOp(const mxArray* real,
    const mxArray* imag,
    index_t out, int dim,
    mxArray* D)
{
  NTuple outdims(real->dims);
  outdims[dim] = out;
  BasicArray<T> F_real(outdims);
  BasicArray<T> F_imag(outdims);
  BasicArray<index_t> Ddata(outdims);
  ConstBasicIterator<T> source_real(&real, dim);
  ConstBasicIterator<T> source_imag(&imag, dim);
  BasicIterator<T> dest_real(&F_real, dim);
  BasicIterator<T> dest_imag(&F_imag, dim);
  BasicArray<T> in_buffer_real(NTuple(real->dims[dim], 1));
  BasicArray<T> in_buffer_imag(NTuple(imag->dims[dim], 1));
  BasicArray<T> out_buffer_real(NTuple(out, 1));
  BasicArray<T> out_buffer_imag(NTuple(out, 1));
  BasicArray<index_t> out_buffer_index(NTuple(out, 1));
  BasicIterator<index_t> D_iter(&Ddata, dim);

  while (source_real.isValid() && dest_real.isValid()) {
    for (index_t i = 1; i <= source_real.size(); i++) {
      in_buffer_real[i] = source_real.get();
      in_buffer_imag[i] = source_imag.get();
      source_real.next();
      source_imag.next();
    }

    Op::func(in_buffer_real, in_buffer_imag,
        out_buffer_real, out_buffer_imag,
        out_buffer_index);

    for (index_t i = 1; i <= out; i++) {
      dest_real.set(out_buffer_real[i]);
      dest_imag.set(out_buffer_imag[i]);
      D_iter.set(out_buffer_index[i]);
      dest_real.next();
      dest_imag.next();
      D_iter.next();
    }

    source_real.nextSlice();
    source_imag.nextSlice();
    dest_real.nextSlice();
    dest_imag.nextSlice();
    D_iter.nextSlice();
  }

  D = mxArray(Ddata);
  return mxArray(F_real, F_imag);
}

template <typename T, class Op>
static mxArray BiVectorOp(const SparseMatrix<T>& real, index_t out, int dim,
    mxArray* D)
{
  if (dim == 0) {
    ConstSparseIterator<T> spin_real(&real);
    NTuple outdims(real->dims);
    outdims[dim] = out;
    SparseMatrix<T> retval(outdims);
    SparseMatrix<index_t> Dval(outdims);

    while (spin_real.isValid()) {
      SparseSlice<T> this_col;
      SparseSlice<index_t> this_index;
      index_t col_number = spin_real.col();
      Op::func(spin_real, this_col, this_index);
      retval.data()[col_number] = this_col;
      Dval.data()[col_number] = this_index;
    }

    D = mxArray(Dval);
    return mxArray(retval);
  }
  else {
    mxArray F = Transpose(BiVectorOp<T, Op>(Transpose(real), out, 0, D));
    D = Transpose(D);
    return F;
  }
}

template <typename T, class Op>
static mxArray BiVectorOp(const SparseMatrix<T>& real,
    const SparseMatrix<T>& imag, index_t out, int dim,
    mxArray* D)
{
  if (dim == 0) {
    ConstComplexSparseIterator<T> spin_complex(&real, &imag);
    NTuple outdims(real->dims);
    outdims[dim] = out;
    SparseMatrix<T> retval_real(outdims);
    SparseMatrix<T> retval_imag(outdims);
    SparseMatrix<index_t> Dval(outdims);

    while (spin_complex.isValid()) {
      SparseSlice<T> this_real_col;
      SparseSlice<T> this_imag_col;
      SparseSlice<index_t> this_index;
      index_t col_number = spin_complex.col();
      Op::func(spin_complex, this_real_col, this_imag_col, this_index);
      retval_real.data()[col_number] = this_real_col;
      retval_imag.data()[col_number] = this_imag_col;
      Dval.data()[col_number] = this_index;
    }

    D = mxArray(Dval);
    return mxArray(retval_real, retval_imag);
  }
  else {
    mxArray F = Transpose(BiVectorOp<T, Op>(Transpose(real),
        Transpose(imag), out, 0, D));
    D = Transpose(D);
    return F;
  }
}

template <typename T, class Op>
static mxArray BiVectorOp(const mxArray* real, index_t out, int dim,
    mxArray* D)
{
  NTuple outdims(real->dims);
  outdims[dim] = out;
  BasicArray<T> F(outdims);
  BasicArray<index_t> Ddata(outdims);
  ConstBasicIterator<T> source(&real, dim);
  BasicIterator<T> dest(&F, dim);
  BasicIterator<index_t> destD(&Ddata, dim);
  BasicArray<T> in_buffer(NTuple(real->dims[dim], 1));
  BasicArray<T> out_buffer1(NTuple(out, 1));
  BasicArray<index_t> out_buffer2(NTuple(out, 1));

  while (source.isValid() && dest.isValid() && destD.isValid()) {
    for (index_t i = 1; i <= source.size(); i++) {
      in_buffer[i] = source.get();
      source.next();
    }

    Op::func(in_buffer, out_buffer1, out_buffer2);

    for (index_t i = 1; i <= out; i++) {
      dest.set(out_buffer1[i]);
      destD.set(out_buffer2[i]);
      dest.next();
      destD.next();
    }

    source.nextSlice();
    dest.nextSlice();
    destD.nextSlice();
  }

  D = mxArray(Ddata);
  return mxArray(F);
}

template <typename T, class Op>
static ArrayVector BiVectorOp(const mxArray* Ain, index_t out,
    int dim, DataClass Tclass)
{
  mxArray Acast(Ain.toClass(Tclass));
  mxArray F, D;

  if (Acast->issparse) {
    if (Acast.allReal()) {
      F = BiVectorOp<T, Op>(Acast.constRealSparse<T>(), out, dim, D);
    }
    else
      F = BiVectorOp<T, Op>(Acast.constRealSparse<T>(),
          Acast.constImagSparse<T>(), out, dim, D);
  }

  if (Acast mxIsScalar()) {
    Acast = Acast.asDenseArray();
  }

  if (Acast.allReal()) {
    F = BiVectorOp<T, Op>(Acast.constReal<T>(), out, dim, D);
  }
  else {
    F = BiVectorOp<T, Op>(Acast.constReal<T>(), Acast.constImag<T>(), out, dim, D);
  }

  F.toClass(Ain->classID);
  ArrayVector ret;
  ret.push_back(F);
  ret.push_back(D);
  return ret;
}

template <class Op>
static ArrayVector BiVectorOp(const mxArray* Ain, int out, int dim)
{
  out = qMax(0, out);

  if (Ain->classID == Float) {
    ArrayVector Ret(BiVectorOp<float, Op>(Ain, out, dim, Float));
    Ret[0] = Ret[0].toClass(Ain->classID);
    return Ret;
  }
  else {
    ArrayVector Ret(BiVectorOp<double, Op>(Ain, out, dim, Double));

    if (Ain->classID != StringArray) {
      Ret[0] = Ret[0].toClass(Ain->classID);
    }

    return Ret;
  }
}

template <typename T, class XIter, class YIter, class Op>
static mxArray StringOp(XIter xi, YIter yi, const NTuple& dims, const ArrayVector& eargs)
{
  BasicArray<T> retvec(dims);

  for (index_t i = 1; i <= dims.count(); i++) {
    ArrayVector t;
    t.push_back(xi.get());
    t.push_back(yi.get());

    for (j = 0; j < eargs.size(); j++) {
      t.push_back(eargs[j]);
    }

    retvec.set(i, Op::func(t));
    xi.next();
    yi.next();
  }

  return retvec;
}

template <typename T, class Op>
static mxArray StringOp(const ArrayVector& arg)
{
  if ((arg[0]->classID != CellArray) && (arg[1]->classID != CellArray)) {
    return mxArray(Op::func(arg));
  }

  mxArray ax(arg[0]);
  mxArray ay(arg[1]);

  if (ax->classID != CellArray) {
    ax = CellArrayFromArray(ax);
  }

  if (ay->classID != CellArray) {
    ay = CellArrayFromArray(ay);
  }

  const BasicArray<mxArray>& x(ax.constReal<mxArray>());
  const BasicArray<mxArray>& y(ay.constReal<mxArray>());
  ArrayVector eargs(arg);
  eargs.pop_front();
  eargs.pop_front();

  if (x mxIsScalar() && !y mxIsScalar()) {
    ConstSpinIterator<mxArray> ix(&x);
    ConstBasicIterator<mxArray> iy(&y, 0);
    return StringOp<T, ConstSpinIterator<mxArray>, ConstBasicIterator<mxArray>, Op>(ix, iy, y->dims, eargs);
  }
  else if (!x mxIsScalar() && y mxIsScalar()) {
    ConstBasicIterator<mxArray> ix(&x, 0);
    ConstSpinIterator<mxArray> iy(&y);
    return StringOp<T, ConstBasicIterator<mxArray>, ConstSpinIterator<mxArray>, Op>(ix, iy, x->dims, eargs);
  }
  else {
    if (x->dims != y->dims) {
      throw_Exception("cell array arguments to string functions must be the same size");
    }

    ConstBasicIterator<mxArray> ix(&x, 0);
    ConstBasicIterator<mxArray> iy(&y, 0);
    return StringOp<T, ConstBasicIterator<mxArray>, ConstBasicIterator<mxArray>, Op>(ix, iy, x->dims, eargs);
  }
}

template <class Op>
static mxArray StringOp(const mxArray* arg)
{
  if (arg.isString() && !arg.isVector()) {
    StringVector p(StringVectorFromArray(arg));
    StringVector q;

    for (i = 0; i < p.size(); i++) {
      QString x(p[i]);
      mxArray y(Op::func(mxArray(x)));
      q.push_back(y.asString());
    }

    return StringArrayFromStringVector(q, QChar(' '));
  }

  if (arg->classID != CellArray) {
    return mxArray(Op::func(arg));
  }

  if (arg mxIsScalar()) {
    return mxArray(Op::func(ArrayFromCellArray(arg)));
  }

  mxArray ax(arg);
  const BasicArray<mxArray>& x(ax.constReal<mxArray>());
  BasicArray<mxArray> y(ax->dims);

  for (index_t i = 1; i <= ax.length(); i++) {
    y[i] = Op::func(x[i]);
  }

  return mxArray(y);
}


#endif
