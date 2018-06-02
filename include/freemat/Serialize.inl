
#define throw_Exception(x)  return 0;

typedef struct serialize_t {
  stream_t s[1];
  BOOL endianSwap;
} serialize_t;

int serialize_handshakeServer(serialize_t* s)
{
  char hand = 'A';
  unsigned short t;
  stream_write(s->s, (const char*) &hand, sizeof(char));
  hand = 'Y';
  stream_write(s->s, (const char*) &hand, sizeof(char));
  hand = 'B';
  stream_write(s->s, (const char*) &hand, sizeof(char));
  hand = 'S';
  stream_write(s->s, (const char*) &hand, sizeof(char));
  t = 1;
  stream_write(s->s, (const char*)&t, sizeof(short));
  return 0;
}

int serialize_handshakeClient(serialize_t* s)
{
  unsigned short t = 1;
  char hand;
  stream_read(s->s, (char*) &hand, sizeof(char));

  if (hand != 'A') {
    throw_Exception("Handshaking error! Unable to establish serialization");
  }

  stream_read(s->s, (char*) &hand, sizeof(char));

  if (hand != 'Y') {
    throw_Exception("Handshaking error! Unable to establish serialization");
  }

  stream_read(s->s, (char*) &hand, sizeof(char));

  if (hand != 'B') {
    throw_Exception("Handshaking error! Unable to establish serialization");
  }

  stream_read(s->s, (char*) &hand, sizeof(char));

  if (hand != 'S') {
    throw_Exception("Handshaking error! Unable to establish serialization");
  }

  stream_read(s->s, (char*) &t, sizeof(short));

  if (t == 1) {
    s->endianSwap = FALSE;
  }
  else if (t == 256) {
    s->endianSwap = TRUE;
  }
  else {
    throw_Exception("Handshaking error! Unable to resolve byte ordering between server/client");
  }

  return 0;
}

int qToBigEndian(int x)
{
  return byteorder_i4(&x, bigEndian);
}
int qFromBigEndian(int x)
{
  return byteorder_i4(&x, bigEndian);
}

int serialize_sendSignature(serialize_t* s, char type, int count)
{
  int netcount;
  stream_write(s->s, (const char*) &type, 1);
  netcount = qToBigEndian(count);
  stream_write(s->s, (const char*) &netcount, sizeof(int));
  return 0;
}

int serialize_checkSignature(serialize_t* s, char type, int count)
{
  char rtype;
  int rcount;
  stream_read(s->s, (char*) &rtype, 1);
  stream_read(s->s, (char*) &rcount, sizeof(int));
  rcount = qFromBigEndian(rcount);

  if (!((type == rtype) && (count == rcount))) {
    char buffer[1000];
    sprintf(buffer, "Serialization Mismatch: expected <%c,%d>, got <%c,%d>",
        type, count, rtype, rcount);
    throw_Exception(buffer);
  }

  return 0;
}

// Send a stream of base objects
int serialize_putBytes(serialize_t* s, const char* ptr, int count)
{
  serialize_sendSignature(s, 'c', count);
  stream_write(s->s, (const char*) ptr, count * sizeof(char));
  return 0;
}

int serialize_putShorts(serialize_t* s, const short* ptr, int count)
{
  serialize_sendSignature(s, 's', count);
  stream_write(s->s, (const char*) ptr, count * sizeof(short));
  return 0;
}

int serialize_putI64s(serialize_t* s, const int64* ptr, int count)
{
  serialize_sendSignature(s, 'z', count);
  stream_write(s->s, (const char*) ptr, count * sizeof(int64));
  return 0;
}

int serialize_putInts(serialize_t* s, const int* ptr, int count)
{
  serialize_sendSignature(s, 'i', count);
  stream_write(s->s, (const char*) ptr, count * sizeof(int));
  return 0;
}

int serialize_putFloats(serialize_t* s, const float* ptr, int count)
{
  serialize_sendSignature(s, 'f', count);
  stream_write(s->s, (const char*) ptr, count * sizeof(float));
  return 0;
}

int serialize_putDoubles(serialize_t* s, const double* ptr, int count)
{
  serialize_sendSignature(s, 'd', count);
  stream_write(s->s, (const char*) ptr, count * sizeof(double));
  return 0;
}

int serialize_putString(serialize_t* s, str_t p)
{
  unsigned int len;
  serialize_sendSignature(s, 'x', 0);
  len = p.l + 1;
  serialize_putInts(s, (int*)&len, 1);
  serialize_putBytes(s, p.s, len);
  return 0;
}

int serialize_putByte(serialize_t* s, char t)
{
  return serialize_putBytes(s, &t, 1);
}

int serialize_putBool(serialize_t* s, BOOL b)
{
  if (b) {
    serialize_putByte(s, 1);
  }
  else {
    serialize_putByte(s, 0);
  }

  return 0;
}

int serialize_getBytes(serialize_t* s, char* ptr, int count)
{
  serialize_checkSignature(s, 'c', count);
  stream_read(s->s, (char*) ptr, count * sizeof(char));
  return 0;
}

char serialize_getByte(serialize_t* s)
{
  char t;
  serialize_getBytes(s, &t, 1);
  return t;
}

BOOL serialize_getBool(serialize_t* s)
{
  char b;
  b = serialize_getByte(s);
  return (b == 1);
}

int serialize_putShort(serialize_t* s, short t)
{
  return serialize_putShorts(s, &t, 1);
}

int serialize_putInt(serialize_t* s, int t)
{
  return serialize_putInts(s, &t, 1);
}

int serialize_putFloat(serialize_t* s, float t)
{
  return serialize_putFloats(s, &t, 1);
}

int serialize_putDouble(serialize_t* s, double t)
{
  return serialize_putDoubles(s, &t, 1);
}

int serialize_putStringVector(serialize_t* s, const vstr_t* t)
{
  int i;
  serialize_sendSignature(s, 'S', 1);
  serialize_putInt(s, t->n);

  for (i = 0; i < t->n; i++) {
    serialize_putString(s, t->v[i]);
  }

  return 0;
}

int serialize_getShorts(serialize_t* s, short* ptr, int count)
{
  int i;
  serialize_checkSignature(s, 's', count);
  stream_read(s->s, (char*) ptr, count * sizeof(short));

  if (s->endianSwap) {
    char* cptr = (char*) ptr;
    char tmp;

    for (i = 0; i < 2 * count; i += 2) {
      CV_SWAP(cptr[i], cptr[i + 1], tmp);
    }
  }

  return 0;
}

int serialize_getI64s(serialize_t* s, int64* ptr, int count)
{
  serialize_checkSignature(s, 'z', count);
  stream_read(s->s, (char*) ptr, count * sizeof(int64));

  if (s->endianSwap) {
    int i;
    char* cptr = (char*) ptr;
    char tmp;

    for (i = 0; i < 8 * count; i += 8) {
      CV_SWAP(cptr[i], cptr[i + 7], tmp);
      CV_SWAP(cptr[i + 1], cptr[i + 6], tmp);
      CV_SWAP(cptr[i + 2], cptr[i + 5], tmp);
      CV_SWAP(cptr[i + 3], cptr[i + 4], tmp);
    }
  }

  return 0;
}

int serialize_getInts(serialize_t* s, int* ptr, int count)
{
  serialize_checkSignature(s, 'i', count);
  stream_read(s->s, (char*) ptr, count * sizeof(int));

  if (s->endianSwap) {
    int i;
    char* cptr = (char*) ptr;
    char tmp;

    for (i = 0; i < 4 * count; i += 4) {
      CV_SWAP(cptr[i], cptr[i + 3], tmp);
      CV_SWAP(cptr[i + 1], cptr[i + 2], tmp);
    }
  }

  return 0;
}

int serialize_getFloats(serialize_t* s, float* ptr, int count)
{
  serialize_checkSignature(s, 'f', count);
  stream_read(s->s, (char*) ptr, count * sizeof(float));

  if (s->endianSwap) {
    int i;
    char* cptr = (char*) ptr;
    char tmp;

    for (i = 0; i < 4 * count; i += 4) {
      CV_SWAP(cptr[i], cptr[i + 3], tmp);
      CV_SWAP(cptr[i + 1], cptr[i + 2], tmp);
    }
  }

  return 0;
}

int serialize_getDoubles(serialize_t* s, double* ptr, int count)
{
  serialize_checkSignature(s, 'd', count);
  stream_read(s->s, (char*)ptr, count * sizeof(double));

  if (s->endianSwap) {
    int i;
    char* cptr = (char*) ptr;
    char tmp;

    for (i = 0; i < 8 * count; i += 8) {
      CV_SWAP(cptr[i], cptr[i + 7], tmp);
      CV_SWAP(cptr[i + 1], cptr[i + 6], tmp);
      CV_SWAP(cptr[i + 2], cptr[i + 5], tmp);
      CV_SWAP(cptr[i + 3], cptr[i + 4], tmp);
    }
  }

  return 0;
}

int serialize_getString(serialize_t* s, str_t* ret)
{
  int len;
  serialize_checkSignature(s, 'x', 0);
  serialize_getInts(s, (int*) &len, 1);

  if (len == 0) {
    str_free(ret);
    return 0;
  }

  str_setsize(ret, len);
  serialize_getBytes(s, ret->s, len);
  return 0;
}

short serialize_getShort(serialize_t* s)
{
  short t;
  serialize_getShorts(s, &t, 1);
  return t;
}

int serialize_getInt(serialize_t* s)
{
  int t;
  serialize_getInts(s, &t, 1);
  return t;
}

float serialize_getFloat(serialize_t* s)
{
  float t;
  serialize_getFloats(s, &t, 1);
  return t;
}

double serialize_getDouble(serialize_t* s)
{
  double t;
  serialize_getDoubles(s, &t, 1);
  return t;
}

int serialize_getStringVector(serialize_t* s, vstr_t* sv)
{
  int i, L;
  serialize_checkSignature(s, 'S', 1);
  L = serialize_getInt(s);
  strv_setsize(sv, L);

  for (i = 0; i < L; i++) {
    serialize_getString(s, sv->v + i);
  }

  return 0;
}

DataClass serialize_getDataClass(serialize_t* s, BOOL* sparseflag, vstr_t* className, BOOL* complexflag)
{
  unsigned char a;
  int i;
  serialize_checkSignature(s, 'a', 1);
  a = serialize_getByte(s);
  *sparseflag = (a & 16) > 0;
  *complexflag = FALSE;
  // For compatibility reasons, the sparse flag is stuck at
  // 16.  Which is binary:
  //   0001 0000
  // To mask out this bit, we need the following number:
  //   1110 1111 = 255 - 16 = 239
  a = a & 239;

  switch (a) {
  case 1:
    return CellArray;

  case 2:
    return Struct;

  case 3:
    return Bool;

  case 4:
    return UInt8;

  case 5:
    return Int8;

  case 6:
    return UInt16;

  case 7:
    return Int16;

  case 8:
    return UInt32;

  case 9:
    return Int32;

  case 32:
    return UInt64;

  case 33:
    return Int64;

  case 10:
    return Float;

  case 11:
    return Double;

  case 12:
    *complexflag = TRUE;
    return Float;

  case 13:
    *complexflag = TRUE;
    return Double;

  case 14:
    return StringArray;

  case 128: {
    int cnt = serialize_getInt(s);
    strv_setsize(className, cnt);

    for (i = 0; i < cnt; i++) {
      serialize_getString(s, className->v + i);
    }

    return Struct;
  }

  default:
    throw_Exception("Unrecognized array type received!");
  }

  return 0;
}

int serialize_putDataClass(serialize_t* s, DataClass cls, const char* className, BOOL complexflag)
{
  serialize_sendSignature(s, 'a', 1);

  switch (cls) {
  default:
    throw_Exception("Unhandled type in putDataClass.");

  case CellArray:
    serialize_putByte(s, 1);
    return 0;

  case Struct:
    serialize_putByte(s, (uchar)128);
    serialize_putString(s, STR1(className));
    return 0;

  case Bool:
    serialize_putByte(s, 3);
    return 0;

  case UInt8:
    if (complexflag) {
      throw_Exception("complex uint8 not supported");
    }

    serialize_putByte(s, 4);
    return 0;

  case Int8:
    if (complexflag) {
      throw_Exception("complex int8 not supported");
    }
    serialize_putByte(s, 5);
    return 0;

  case UInt16:
    if (complexflag) {
      throw_Exception("complex uint16 not supported");
    }

    serialize_putByte(s, 6);
    return 0;

  case Int16:
    if (complexflag) {
      throw_Exception("complex int16 not supported");
    }

    serialize_putByte(s, 7);
    return 0;

  case UInt32:
    if (complexflag) {
      throw_Exception("complex uint32 not supported");
    }

    serialize_putByte(s, 8);
    return 0;

  case Int32:
    if (complexflag) {
      throw_Exception("complex int32 not supported");
    }

    serialize_putByte(s, 9);
    return 0;

  case UInt64:
    if (complexflag) {
      throw_Exception("complex uint64 not supported");
    }
    serialize_putByte(s, 32);
    return 0;

  case Int64:
    if (complexflag) {
      throw_Exception("complex int64 not supported");
    }

    serialize_putByte(s, 33);
    return 0;

  case Float:
    if (complexflag) {
      serialize_putByte(s, 12);
    }
    else {
      serialize_putByte(s, 10);
    }

    return 0;

  case Double:
    if (complexflag) {
      serialize_putByte(s, 13);
    }
    else {
      serialize_putByte(s, 11);
    }

    return 0;

  case StringArray:
    serialize_putByte(s, 14);
    return 0;
  }
}

int serialize_putDimensions(serialize_t* s, const dim_type* dim)
{
  int i;
  serialize_sendSignature(s, 'D', 1);
  serialize_putInt(s, NDim_lastNotOne(dim));

  for (i = 0; i < NDim_lastNotOne(dim); i++) {
    serialize_putInt(s, (int)(dim[i]));
  }

  return 0;
}

int serialize_getDimensions(serialize_t* s, dim_type* dim)
{
  int i, len;
  serialize_checkSignature(s, 'D', 1);
  NDim_init(dim);
  len = serialize_getInt(s);

  for (i = 0; i < len; i++) {
    dim[i] = serialize_getInt(s);
  }

  return 0;
}

#define MacroPutCase(func,ctype,class,ftype)  { func(s, (ctype*)dat->data, len);  return 0;  }
#define MacroPutComplexCase(func,ctype,class,ftype)  { func(s, (ctype*)dat->data, 2*len);  return 0;  }

int mxFieldNames(const mxArray* arr, vstr_t* sv)
{
  strv_set_cstrs(sv, arr->n_fields, arr->fieldnames);
  return sv->n;
}

int serialize_putArray(serialize_t* s, const mxArray* dat)
{
  int i, j, len;
  DataClass dclass;
  const char* szClass = mxGetClassName(dat);
  serialize_sendSignature(s, 'A', 1);
  dclass = dat->classID;
  serialize_putDataClass(s, dclass, szClass, dat->iscomplex);
  serialize_putDimensions(s, dat->dims);

  if (mxIsEmpty(dat)) {
    return 0;
  }

  len = mxGetNumberOfElements(dat);

  switch (dclass) {
  default:
    throw_Exception("unhandled type in putArray");

  case CellArray: {
    const mxArray* rp = (mxArray*)dat->data;

    for (i = 0; i < len; i++) {
      serialize_putArray(s, rp + i);
    }

    return 0;
  }

  case Struct: {
    vstr_t fnames[1] = {0};
    int i, ncount;
    mxFieldNames(dat, fnames);
    ncount = fnames->n;
    serialize_putInt(s, ncount);

    for (i = 0; i < ncount; i++) {
      serialize_putString(s, fnames->v[i]);
    }

    for (j = 0; j < len; ++j) {
      for (i = 0; i < ncount; i++) {
        mxArray* dp = ((mxArray*)dat->data) + j * ncount + i;
        serialize_putArray(s, dp);
      }
    }

    strv_free(fnames);
    return 0;
  }

  case Bool:
    MacroPutCase(serialize_putBytes, char, Int8, int8);

  case StringArray:
    MacroPutCase(serialize_putBytes, char, UInt8, uint8);

  case UInt8:
    MacroPutCase(serialize_putBytes, char, UInt8, uint8);

  case UInt16:
    MacroPutCase(serialize_putShorts, short, UInt16, uint16);

  case UInt32:
    MacroPutCase(serialize_putInts, int, UInt32, uint32);

  case UInt64:
    MacroPutCase(serialize_putI64s, int64, UInt64, uint64);

  case Int8:
    MacroPutCase(serialize_putBytes, char, Int8, int8);

  case Int16:
    MacroPutCase(serialize_putShorts, short, Int16, int16);

  case Int32:
    MacroPutCase(serialize_putInts, int, Int32, int32);

  case Int64:
    MacroPutCase(serialize_putI64s, int64, Int64, int64);

  case Float:
    if (!dat->iscomplex) {
      MacroPutCase(serialize_putFloats, float, Float, float);
    }
    else {
      MacroPutComplexCase(serialize_putFloats, float, Float, float);
    }

  case Double:
    if (!dat->iscomplex) {
      MacroPutCase(serialize_putDoubles, double, Double, double);
    } else {
      MacroPutComplexCase(serialize_putDoubles, double, Double, double);
    }
  }

  return 0;
}

#define MacroGetCase(func,ctype,class,ftype)  \
  {           \
    BasicArray<ftype> rp(dims);     \
    func((ctype*) rp.data(),int(dims.count())); \
    dat = mxArray(rp).toClass(class);   \
    return 0;         \
  }

#define MacroGetComplexCase(func,ctype,class,ftype)   \
  {               \
    BasicArray<ftype> rp(dims.replace(0,dims[0]*2));    \
    func((ctype*) rp.data(),int(dat.length()));     \
    dat = mxArray(SplitReal(rp),SplitImag(rp)).toClass(class);  \
    return 0;             \
  }

int serialize_getArray(serialize_t* s, mxArray* dat)
{
#if 0
  serialize_checkSignature(s, 'A', 1);
  BOOL sparseflag;
  str_t className[1] = {0};
  BOOL complexflag;
  DataClass dclass = serialize_getDataClass(s, sparseflag, className, complexflag);
  index_t dims[NDims] = {0};
  serialize_getDimensions(s, dims);
  int elCount = NDim_count(dims);

  if (elCount == 0) {
    dat = EmptyConstructor();
    return 0;
  }

  switch (dclass) {
  default:
    throw_Exception("Unhandled type in getArray");

  case CellArray: {
    BasicArray<mxArray> rp(dims);

    for (index_t i = 1; i <= rp.length(); i++) {
      getArray(rp[i]);
    }

    dat = mxArray(rp);
    return 0;
  }

  case Struct: {
    StringVector fnames;
    int ncount(serialize_getInt(s));
    StructArray rp;

    for (i = 0; i < ncount; i++) {
      rp.insert(serialize_getString(s), BasicArray<mxArray>(dims));
    }

    rp.mxSetClassName(className);

    for (index_t j = 1; j <= dims.count(); j++)
      for (i = 0; i < ncount; i++) {
        getArray(rp[i][j]);
      }

    rp.updateDims();
    dat = mxArray(rp);
    return 0;
  }

  case Bool:
    MacroGetCase(getBytes, char, Bool, int8);

  case StringArray:
    MacroGetCase(getBytes, char, StringArray, uint8);

  case UInt8:
    MacroGetCase(getBytes, char, UInt8, uint8);

  case UInt16:
    MacroGetCase(getShorts, short, UInt16, uint16);

  case UInt32:
    MacroGetCase(getInts, int, UInt32, uint32);

  case UInt64:
    MacroGetCase(getI64s, int64, UInt64, uint64);

  case Int64:
    MacroGetCase(getI64s, int64, Int64, int64);

  case Int32:
    if (sparseflag) {
      QVector<QVector<int32> > dp;

      for (index_t i = 1; i <= dims.cols(); i++) {
        int len = serialize_getInt(s);
        QVector<int32> col(len);
        getInts(col.data(), len);
        dp << col;
      }

      dat = FM3Sparse(dp, dims);
      return 0;
    }
    else {
      MacroGetCase(getInts, int, Int32, int32);
    }

  case Float:
    if (!complexflag) {
      if (sparseflag) {
        QVector<QVector<float> > dp;

        for (index_t i = 1; i <= dims.cols(); i++) {
          int len = int(getFloat());
          QVector<float> col(len);
          serialize_getFloats(s, col.data(), len);
          dp << col;
        }

        dat = FM3Sparse(dp, dims);
        return 0;
      }
      else {
        MacroGetCase(getFloats, float, Float, float);
      }
    }
    else {
      if (sparseflag) {
        QVector<QVector<float> > dp;

        for (index_t i = 1; i <= dims.cols(); i++) {
          int len = int(getFloat());
          QVector<float> col(len);
          serialize_getFloats(s, col.data(), len);
          dp << col;
        }

        dat = FM3SparseComplex(dp, dims);
        return 0;
      }
      else {
        MacroGetComplexCase(getFloats, float, Float, float);
      }
    }

  case Double:
    if (!complexflag) {
      if (sparseflag) {
        QVector<QVector<double> > dp;

        for (index_t i = 1; i <= dims.cols(); i++) {
          int len = int(getDouble());
          QVector<double> col(len);
          serialize_getDoubles(s, col.data(), len);
          dp << col;
        }

        dat = FM3Sparse(dp, dims);
        return 0;
      }
      else {
        MacroGetCase(getDoubles, double, Double, double);
      }
    }
    else {
      if (sparseflag) {
        QVector<QVector<double> > dp;

        for (index_t i = 1; i <= dims.cols(); i++) {
          int len = int(getDouble());
          QVector<double> col(len);
          serialize_getDoubles(s, col.data(), len);
          dp << col;
        }

        dat = FM3SparseComplex(dp, dims);
        return 0;
      }
      else {
        MacroGetComplexCase(getDoubles, double, Double, double);
      }
    }
  }

#endif
  return 0;
}



