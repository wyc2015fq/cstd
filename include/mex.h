#ifndef __MEX_H__
#define __MEX_H__
#include <stdlib.h>
// Define a mex interface into FreeMat...
//
// How does this work? we provide a library libmex that we link candidate code to
// to form a new module/dll.  When we load that library, we establish a bridge
// from it to the current interpreter.  That bridge is used to manipulate the
// interpreter.
typedef enum {
  mxUNKNOWN_CLASS = 0,
  mxCELL_CLASS,
  mxSTRUCT_CLASS,
  mxLOGICAL_CLASS,
  mxCHAR_CLASS,
  dummy_CLASS,
  mxDOUBLE_CLASS,
  mxSINGLE_CLASS,
  mxINT8_CLASS,
  mxUINT8_CLASS,
  mxINT16_CLASS,
  mxUINT16_CLASS,
  mxINT32_CLASS,
  mxUINT32_CLASS,
  mxINT64_CLASS,
  mxUINT64_CLASS,
  mxFUNCTION_CLASS,
};
// Assume this has to fit in 32 bits
//   scalar flag  - 1 bit
//   sparse flag  - 1 bit
//   complex flag - 1 bit
//   class flag   - 5 bits
//   user class   - 24 bits
enum {
  Invalid = 0,
  CellArray = 1,
  Struct = 2,
  StringArray = 3,
  Bool = 4,
  Int8 = 5,
  UInt8 = 6,
  Int16 = 7,
  UInt16 = 8,
  Int32 = 9,
  UInt32 = 10,
  Int64 = 11,
  UInt64 = 12,
  Float = 13,
  Double = 14,
  Index = 14
};
typedef int DataClass;
typedef int mxClassID;
typedef unsigned short mxChar;
typedef BOOL   logical;
typedef double index_t;
typedef int dim_type;
#define mxINDEX_CLASS  mxDOUBLE_CLASS
#define mxMAXDIMS 4
enum { NDims = mxMAXDIMS };
typedef enum {
  mxREAL = 0,
  mxCOMPLEX
} mxComplexity;
//#define mxMAXBUF  16
//CLASSSIZEDEF(mxUNKNOWN_CLASS, int, "d")
//CLASSSIZEDEF(mxCELL_CLASS, int, "d")
//CLASSSIZEDEF(mxSTRUCT_CLASS, int, "d")
//CLASSSIZEDEF(dummy_CLASS, int, "d")
#define CLASSSIZEDEF_SIGNED(hh, CLASSSIZEDEF)   \
  CLASSSIZEDEF(hh, mxDOUBLE_CLASS, double, "f") \
  CLASSSIZEDEF(hh, mxSINGLE_CLASS, float, "I32f") \
  CLASSSIZEDEF(hh, mxINT8_CLASS, char, "I8d") \
  CLASSSIZEDEF(hh, mxINT16_CLASS, short, "I16d") \
  CLASSSIZEDEF(hh, mxINT32_CLASS, int, "d") \
  CLASSSIZEDEF(hh, mxINT64_CLASS, __int64, "I64d") \
  CLASSSIZEDEF(hh, mxFUNCTION_CLASS, int, "d")
#define CLASSSIZEDEFNUMBER(hh, CLASSSIZEDEF)   CLASSSIZEDEF_SIGNED(hh, CLASSSIZEDEF) \
  CLASSSIZEDEF(hh, mxLOGICAL_CLASS, logical, "d") \
  CLASSSIZEDEF(hh, mxCHAR_CLASS, mxChar, "c") \
  CLASSSIZEDEF(hh, mxUINT8_CLASS, unsigned char, "I8u") \
  CLASSSIZEDEF(hh, mxUINT16_CLASS, unsigned short, "I16u") \
  CLASSSIZEDEF(hh, mxUINT32_CLASS, unsigned int, "u") \
  CLASSSIZEDEF(hh, mxUINT64_CLASS, unsigned __int64, "I64u") \
  typedef struct {
mxClassID classID;
dim_type dims[mxMAXDIMS];
int n_dims;
//int issparse;
int n_fields;
int iscomplex;
const char** fieldnames;
int* data;
int refID;
} mxArray;
const char* className(const mxArray* a)
{
  return "";
}
typedef int mwSize;
#ifndef __cplusplus
typedef int bool;
#endif
static BOOL mxIsUserClass(const mxArray* A)
{
  return mxSTRUCT_CLASS == A->classID;
}
static BOOL IsIntegerDataClass(DataClass Ain)
{
  switch (Ain) {
  case mxCELL_CLASS:
  case mxSTRUCT_CLASS:
  case mxSINGLE_CLASS:
  case mxDOUBLE_CLASS:
    return FALSE;
  default:
    return TRUE;
  }
  return FALSE;
}
static DataClass TypeToSigned(DataClass Ain)
{
  switch (Ain) {
  case mxUINT8_CLASS:
    return mxINT16_CLASS;
  case mxUINT16_CLASS:
    return mxINT32_CLASS;
  case mxUINT32_CLASS:
    return mxINT32_CLASS;
  case mxCELL_CLASS:
  case mxSTRUCT_CLASS:
    return mxUNKNOWN_CLASS;
  default:
    return Ain;
  }
}
static DataClass ComputeTypes0(DataClass Ain, DataClass Bin, DataClass force_type, DataClass* pvia)
{
  DataClass out = Double, via = Double;
  if (Ain == Float && Bin == Float) {
    via = Double;
    out = Float;
  }
  else if (Ain == Double || Bin == Double) {
    via = Double;
    out = Double;
  }
  else {
    via = Ain;
    out = Ain;
  }
  if (force_type) {
    via = force_type;
  }
  if (pvia) {
    *pvia = via;
  }
  return out;
}
static DataClass ComputeTypes1(const mxArray* A, double B, DataClass* via)
{
  double b1 = (int)B;
  DataClass Bin = (b1 == B) ? mxINT32_CLASS : mxDOUBLE_CLASS;
  return ComputeTypes0(A->classID, Bin, 0, via);
}
static DataClass ComputeTypes2(double A, double B, DataClass* via)
{
  double a1 = (int)A, b1 = (int)B;
  DataClass Ain = (a1 == A) ? mxINT32_CLASS : mxDOUBLE_CLASS;
  DataClass Bin = (b1 == B) ? mxINT32_CLASS : mxDOUBLE_CLASS;
  return ComputeTypes0(Ain, Bin, 0, via);
}
static DataClass ComputeTypes(const mxArray* A, const mxArray* B, DataClass force_type, DataClass* via)
{
  return ComputeTypes0(A->classID, B->classID, force_type, via);
}
#define MacroExpandCasesIntSigned(func)   \
  func(int64,Int64);        \
  func(int32,Int32);        \
  func(int16,Int16);        \
  func(int8,Int8);
#define MacroExpandCasesIntUnsigned(func) \
  func(uint64,UInt64);        \
  func(uint32,UInt32);        \
  func(uint16,UInt16);        \
  func(uint8,UInt8);
#define MacroExpandCasesSigned(func)    \
  func(double,Double);        \
  func(float,Float);        \
  MacroExpandCasesIntSigned(func)
#define MacroExpandCasesNoBool(func)    \
  MacroExpandCasesSigned(func)      \
  MacroExpandCasesIntUnsigned(func);
#define MacroExpandCases(func)      \
  MacroExpandCasesNoBool(func);     \
  func(bool,Bool);
#define MacroExpandCasesSimple(func)    \
  MacroExpandCases(func);
#define MacroExpandCasesNoCell(func)    \
  MacroExpandCasesSimple(func);     \
  func(QChar,StringArray);
#define MacroExpandCasesAll(func)   \
  MacroExpandCasesNoCell(func);     \
  func(Array,CellArray);
typedef bool mxLogical;
typedef short mxCHAR;
int* mxMalloc(size_t n)
{
  int* p = 0;
  if (n > 0) {
    p = malloc(n);
    memset(p, 0, n);
  }
  return p;
}
int* mxCalloc(size_t n, size_t b)
{
  return mxMalloc(n * b);
}
int* mxRealloc(int* ptr, size_t size)
{
  int* p = 0;
  p = realloc(ptr, size);
  return p;
}
int mxFree(int* ptr)
{
  if (ptr) {
    free(ptr);
  }
  return 0;
}
static bool NDim_is2D(const dim_type* m_data)
{
  int i;
  for (i = 2; i < NDims; i++)
    if (m_data[i] != 1) {
      return FALSE;
    }
  return TRUE;
}
static bool NDim_isValid(const dim_type* m_data)
{
  int i;
  for (i = 0; i < NDims; i++) {
    if (m_data[i] < 0) {
      return FALSE;
    }
  }
  return TRUE;
}
static int NDim_zero(dim_type* m_data)
{
  int i;
  for (i = 0; i < NDims; i++) {
    m_data[i] = 0;
  }
  return 0;
}
static dim_type NDim_maxdim(dim_type* m_data)
{
  int i;
  dim_type v = m_data[0];
  for (i = 1; i < NDims; i++) {
    if (m_data[i] > v) {
      v = m_data[i];
    }
  }
  return v;
}
static const dim_type NDim_count(const dim_type* m_data)
{
  int i;
  dim_type ret = 1;
  for (i = 0; i < NDims; i++) {
    ret *= m_data[i];
  }
  return ret;
}
static dim_type NDim_rows(const dim_type* m_data)
{
  return m_data[0];
}
static dim_type NDim_cols(const dim_type* m_data)
{
  return m_data[1];
}
static bool NDim_isVector(const dim_type* m_data)
{
  return (NDim_is2D(m_data) && ((m_data[0] == 1) || (m_data[1] == 1)));
}
static bool NDim_isColumnVector(const dim_type* m_data)
{
  return (NDim_is2D(m_data) && (m_data[1] == 1));
}
static bool NDim_isRowVector(const dim_type* m_data)
{
  return (NDim_is2D(m_data) && (m_data[0] == 1));
}
static bool NDim_isScalar(const dim_type* m_data)
{
  return (NDim_count(m_data) == 1);
}
static dim_type NDim_stride(const dim_type* m_data, int dim)
{
  int i;
  dim_type nextCoeff = 1;
  if ((dim < 0) || (dim >= NDims)) {
    ASSERT(0 && "Illegal range in stride");
  }
  for (i = 1; i <= dim; i++) {
    nextCoeff *= m_data[i - 1];
  }
  return nextCoeff;
}
static dim_type NDim_map(const dim_type* m_data, const dim_type* pos)
{
  int i;
  dim_type retval = 0;
  dim_type nextCoeff = 1;
  for (i = 0; i < NDims; i++) {
    retval += nextCoeff * pos[i];
    nextCoeff *= m_data[i];
  }
  return retval;
}
static int NDim_map2(const dim_type* m_data, dim_type vecndx, dim_type* pos)
{
  int i;
  int64 vecndxi = (int64)(vecndx);
  vecndxi--;
  for (i = 0; i < NDims; i++) {
    pos[i] = (dim_type)(vecndxi % ((int64) m_data[i])) + 1;
    vecndxi /= ((int64) m_data[i]);
  }
}
static bool NDim_validate(const dim_type* m_data, const dim_type* pos)
{
  int i;
  for (i = 0; i < NDims; i++) {
    if ((pos[i] <= 0) || (pos[i] > m_data[i])) {
      return FALSE;
    }
    return TRUE;
  }
}
static int NDim_ripplePinned(const dim_type* m_data, dim_type* pos, int pin_dim)
{
  int i;
  if ((pin_dim < 0) || (pin_dim >= NDims)) {
    ASSERT(0 && "Illegal range in stride");
  }
  for (i = 0; i < (NDims - 1); i++) {
    int64 dim = (int64) m_data[i];
    if (i == pin_dim) {
      dim = 1;
    }
    if (pos[i] > dim) {
      pos[i] = 0;
      pos[i + 1]++;
    }
  }
  return 0;
}
static int NDim_next(const dim_type* m_data, dim_type* pos)
{
  int i;
  pos[0]++;
  for (i = 0; i < (NDims - 1); i++) {
    if (pos[i] < m_data[i]) {
      return 1;
    }
    else {
      pos[i] = 0;
      pos[i + 1]++;
    }
  }
  return 0;
}
static int NDim_prev(const dim_type* m_data, dim_type* pos)
{
  int i;
  pos[0]--;
  for (i = 0; i < (NDims - 1); i++) {
    if (pos[i] >= 0) {
      return 1;
    }
    else {
      pos[i] = m_data[i] - 1;
      pos[i + 1]--;
    }
  }
  return 0;
}
static int NDim_getstep(const dim_type* m_data, dim_type* step)
{
  int i;
  step[0] = 1;
  for (i = 1; i < NDims; ++i) {
    step[i] = step[i - 1] * m_data[i - 1];
  }
  return 0;
}
static int NDim_getoff(const dim_type* m_data, const dim_type* step)
{
  int i, off = 0;
  for (i = 1; i < NDims; ++i) {
    off += step[i] * m_data[i];
  }
  return off;
}
static int NDim_forceOne(const dim_type* m_data, int pin_dim, dim_type* ret)
{
  if ((pin_dim < 0) || (pin_dim >= NDims)) {
    ASSERT(0 && "Illegal range in forceOne");
  }
  MEMCPY(ret, m_data, NDims);
  ret[pin_dim] = 1;
  return 0;
}
static int NDim_increment2(const dim_type* m_data, dim_type* pos, int pin_dim)
{
  if ((pin_dim < 0) || (pin_dim >= NDims)) {
    ASSERT(0 && "Illegal range in increment");
  }
  if (pin_dim == 0) {
    pos[1]++;
  }
  else {
    pos[0]++;
  }
  NDim_ripplePinned(m_data, pos, pin_dim);
}
static int NDim_replace(const dim_type* m_data, int dim, dim_type val, dim_type* ret)
{
  if ((dim < 0) || (dim >= NDims)) {
    ASSERT(0 && "Illegal range in replace");
  }
  MEMCPY(ret, m_data, NDims);
  ret[dim] = val;
  return 0;
}
static int NDim_set2(dim_type* m_data, dim_type rows, dim_type cols)
{
  int i;
  m_data[0] = rows;
  m_data[1] = cols;
  for (i = 2; i < NDims; i++) {
    m_data[i] = 1;
  }
  return 0;
}
static int NDim_set3(dim_type* m_data, dim_type rows, dim_type cols, dim_type slices)
{
  int i;
  m_data[0] = rows;
  m_data[1] = cols;
  m_data[2] = slices;
  for (i = 3; i < NDims; i++) {
    m_data[i] = 1;
  }
  return 0;
}
static int NDim_init(dim_type* m_data)
{
  int i;
  m_data[0] = 0;
  m_data[1] = 0;
  for (i = 2; i < NDims; i++) {
    m_data[i] = 1;
  }
  return 0;
}
static int NDim_set(dim_type* m_data, int dim, dim_type len)
{
  if (len < 0) {
    ASSERT(0 && "Negative dimensions are not allowed");
  }
  if ((dim < 0) || (dim >= NDims)) {
    ASSERT(0 && "Illegal range in set");
  }
  m_data[dim] = len;
  return 0;
}
static int NDim_copy(dim_type* m_data, const dim_type* dims)
{
  memcpy(m_data, dims, NDims * sizeof(dim_type));
  return 0;
}
static int NDim_cmp(const dim_type* m_data, const dim_type* alt)
{
  int i;
  for (i = 0; i < NDims; i++) {
    if (m_data[i] < alt[i]) {
      return -1;
    }
    else if (m_data[i] > alt[i]) {
      return 1;
    }
  }
  return 0;
}
// Compare this NTuple to the given size.  Extension
// is defined as:
//  All dimensions are the same, except for the
//  last non-singleton dimension
static bool NDim_extends(const dim_type* m_data, const dim_type* size)
{
  int firstDifferent = NDims;
  int i = 0;
  while (i < NDims && (size[i] == m_data[i])) {
    i++;
  }
  firstDifferent = i;
  if (firstDifferent == NDims) {
    return FALSE;
  }
  i = firstDifferent + 1;
  while (i < NDims && (size[i] == 1) && (m_data[i] == 1)) {
    i++;
  }
  if (i == NDims) {
    return TRUE;
  }
  return FALSE;
}
int NDim_permute(const dim_type* m_data, const dim_type* perm, dim_type* ret)
{
  int i;
  for (i = 0; i < NDims; i++) {
    ret[i] = m_data[(int)(perm[i] - 1)];
  }
  return 0;
}
static int NDim_lastNotOne(const dim_type* m_data)
{
  int last_not_one = NDims;
  while ((m_data[last_not_one - 1] == 1) &&
      (last_not_one > 2)) {
    last_not_one--;
  }
  return last_not_one;
}
static int NDim_firstNonsingular(const dim_type* m_data)
{
  int non_singular = 0;
  while ((m_data[non_singular] == 1) && (non_singular < NDims)) {
    non_singular++;
  }
  if (non_singular == NDims) {
    return 0;
  }
  return non_singular;
}
static int NDim_lastSingular(const dim_type* m_data)
{
  int last_singular = NDims;
  while ((m_data[last_singular - 1] == 1) &&
      (last_singular > 0)) {
    last_singular--;
  }
  return last_singular;
}
static int NDim_toString(const dim_type* m_data, char* ret, int len)
{
  int i, l, last_not_one = NDim_lastNotOne(m_data);
  _snprintf(ret, len, "%f", m_data[0]);
  for (i = 1; i < last_not_one; i++) {
    l = strlen(ret);
    _snprintf(ret + l, len - l, "x%f", m_data[i]);
  }
  l = strlen(ret);
  return l;
}
static int NDim_print(const dim_type* t)
{
  int i;
  printf("<");
  for (i = 0; i < NDims; i++) {
    printf("%f ", t[i]);
  }
  printf(">");
  return 0;
}
static int NDim_max(const dim_type* a, const dim_type* b, dim_type* ret)
{
  int i;
  for (i = 0; i < NDims; i++) {
    ret[i] = (a[i] > b[i]) ? a[i] : b[i];
  }
  return 0;
}
mxClassID mxGetClassID(const mxArray* arr)
{
  return arr->classID;
}
const char* mxGetClassName(const mxArray* arr)
{
  switch (arr->classID) {
  default:
    return "Unknown class?!";
  case mxUNKNOWN_CLASS:
    return ("");
  case mxCELL_CLASS:
    return ("cell");
  case mxSTRUCT_CLASS:
    return ("struct");
  case mxCHAR_CLASS:
    return ("char");
  case mxLOGICAL_CLASS:
    return ("logical");
#define MacroClassName(ctype,cls)  case cls: return #cls ;
    MacroExpandCasesNoBool(MacroClassName);
#undef MacroClassName
  }
  return 0;
}
int* mxGetData(const mxArray* arr)
{
  return arr->data;
}
int* mxGetDimensions(const mxArray* arr)
{
  return (int*)arr->dims;
}
int mxGetElementSize(const mxArray* arr);
double mxGetEps(int);
int mxGetField(const mxArray* arr, int index, const char* field_name);
int mxGetFieldByNumber(const mxArray* arr, int index, int field_number);
const char* mxGetFieldNameByNumber(const mxArray* arr, int field_number);
int mxGetFieldNumber(const mxArray* arr, const char* field_name);
int* mxGetImagData(const mxArray* arr);
double mxGetInf(int);
int* mxGetIr(const mxArray* arr);
int* mxGetJc(const mxArray* arr);
mxLogical* mxGetLogicals(const mxArray* arr);
int mxGetM(const mxArray* arr)
{
  return arr->dims[0];
}
// ап
int mxGetN(const mxArray* arr)
{
  return arr->dims[1];
}
double mxGetNaN(int);
int mxGetNumberOfDimensions(const mxArray* arr)
{
  return arr->n_dims;
}
int mxGetNumberOfElements(const mxArray* arr)
{
  return NDim_count(arr->dims);
}
int mxGetNumberOfFields(const mxArray* arr);
int mxGetNzmax(const mxArray* arr);
double mxGetScalar(const mxArray* arr)
{
  switch (arr->classID) {
  case mxUNKNOWN_CLASS:
  case mxCELL_CLASS:
  case mxSTRUCT_CLASS:
    ASSERT(0);
    return 0;
#define CLASSSIZEDEF(hh, ID, TYPE, FMT)  case ID: return (double)*((TYPE*)arr->data);
    CLASSSIZEDEFNUMBER(hh, CLASSSIZEDEF);
#undef CLASSSIZEDEF
  }
  ASSERT(0 && "unknown class");
  return 0;
}
int mxGetString(const mxArray* arr, char* buf, int buflen)
{
  int n = arr->dims[0] * arr->dims[1];
  ASSERT(mxCHAR_CLASS == arr->classID);
  n = MIN(n, buflen - 1);
  memcpy(buf, arr->data, n);
  buf[n] = 0;
  return 0;
}
bool mxIsColumnVector(const mxArray* arr)
{
  return NDim_isColumnVector(arr->dims);
}
bool mxIsRowVector(const mxArray* arr)
{
  return NDim_isRowVector(arr->dims);
}
bool mxIsVector(const mxArray* arr)
{
  return NDim_isRowVector(arr->dims) || NDim_isColumnVector(arr->dims);
}
bool mxIs2D(const mxArray* arr)
{
  return NDim_is2D(arr->dims);
}
bool mxIsSquare(const mxArray* arr)
{
  return NDim_is2D(arr->dims) && (arr->dims[0] == arr->dims[1]);
}
bool mxIsCell(const mxArray* arr);
bool mxIsChar(const mxArray* arr);
bool mxIsClass(const mxArray* arr, const char* name);
bool mxIsComplex(const mxArray* arr);
bool mxIsDouble(const mxArray* arr);
bool mxIsEmpty(const mxArray* arr)
{
  return (mxUNKNOWN_CLASS == arr->classID || 0 == mxGetNumberOfElements(arr));
}
bool mxIsFinite(double value);
bool mxIsFromGlobalWS(const mxArray* arr);
bool mxIsInf(double value);
bool mxIsInt8(const mxArray* arr);
bool mxIsInt16(const mxArray* arr);
bool mxIsInt32(const mxArray* arr);
bool mxIsInt64(const mxArray* arr);
bool mxIsLogical(const mxArray* arr);
bool mxIsLogicalScalar(const mxArray* arr);
bool mxIsLogicalScalarTrue(const mxArray* arr);
bool mxIsNaN(double value);
bool mxIsNumeric(const mxArray* arr);
bool mxIsSingle(const mxArray* arr);
bool mxIsAllReal(const mxArray* arr)
{
  return !arr->iscomplex;
}
bool mxIsSparse(const mxArray* arr)
{
  return 0;//arr->issparse;
}
bool mxIsStruct(const mxArray* arr);
bool mxIsUint8(const mxArray* arr);
bool mxIsUint16(const mxArray* arr);
bool mxIsUint32(const mxArray* arr);
bool mxIsUint64(const mxArray* arr);
bool mxIsScalar(const mxArray* arr)
{
  return 1 == mxGetNumberOfElements(arr);
}
int mxAddField(mxArray* arr, const char* field_name);
char* mxArrayToString(const mxArray* arr);
int mxAssert(int expr, char* error_message);
int mxAssertS(int expr, char* error_message);
int mxCalcSingleSubscript(const mxArray* arr, int nsubs, int* subs);
int mxSetCellMatrix(mxArray* arr, int m, int n);
int mxSetCharArray(mxArray* arr, int ndim, const int* dims);
int mxSetCharMatrixFromStrings(mxArray* arr, int m, const char** str);
int mxSetDoubleMatrix(mxArray* arr, int m, int n, mxComplexity ComplexFlag);
int mxSetLogicalArray(mxArray* arr, int ndim, const int* dims);
int mxSetLogicalMatrix(mxArray* arr, int m, int n);
int mxSetLogicalScalar(mxArray* arr, mxLogical value);
int mxGetClassSize(mxClassID clss)
{
  static int sz[32] = {0};
  static int inited = 0;
  if (!inited) {
    inited = 1;
    sz[mxUNKNOWN_CLASS] = 0;
    sz[mxCELL_CLASS] = sizeof(mxArray);
    sz[mxSTRUCT_CLASS] = sizeof(mxArray);
#define CLASSSIZEDEF(hh, ID, TYPE, FMT)  sz[ID] = sizeof(TYPE);
    CLASSSIZEDEFNUMBER(hh, CLASSSIZEDEF);
#undef CLASSSIZEDEF
  }
  return sz[clss];
}
#define mxFreeArray mxDestroyArray
int mxDestroyArray(mxArray* arr)
{
  if (mxCELL_CLASS == arr->classID || mxSTRUCT_CLASS == arr->classID) {
    int i, n = mxGetNumberOfElements(arr);
    mxArray* arr1 = (mxArray*)arr->data;
    for (i = 0; i < n; ++i) {
      mxDestroyArray(arr1 + i);
    }
  }
  mxFree(arr->data);
  memset(arr, 0, sizeof(mxArray));
  return 0;
}
int mxSetData(mxArray* arr, int* data_ptr)
{
  if (arr->data) {
    free(arr->data);
  }
  arr->data = data_ptr;
  return 0;
}
int mxSetDimensions(mxArray* arr, const int* dims, int ndim)
{
  int i;
  ASSERT(ndim < mxMAXDIMS);
  if (3 == ndim && 1 == dims[2]) {
    ndim = 2;
  }
  for (i = 0; i < ndim; ++i) {
    arr->dims[i] = dims[i];
  }
  for (; i < mxMAXDIMS; ++i) {
    arr->dims[i] = 0;
  }
  if (1 == ndim) {
    ndim = 2;
    arr->dims[1] = 1;
  }
  arr->n_dims = ndim;
  return 0;
}
int mxSetNumericArray(mxArray* arr, int ndim, const int* dims, mxClassID clss, mxComplexity ComplexFlag, const int* data)
{
  int i, sz = mxGetClassSize(clss);
  mxFreeArray(arr);
  for (; ndim > 0 && 1 == dims[ndim - 1];) {
    --ndim;
  }
  for (i = 0; i < ndim; ++i) {
    sz *= dims[i];
  }
  sz *= mxCOMPLEX == ComplexFlag ? 2 : 1;
  if (sz > 0) {
    arr->data = (int*)mxRealloc(arr->data, sz);
    if (data) {
      memcpy(arr->data, data, sz);
    }
  }
  mxSetDimensions(arr, dims, ndim);
  arr->iscomplex = mxCOMPLEX == ComplexFlag;
  arr->classID = clss;
  return 0;
}
int mxSetMatrix(mxArray* arr, int m, int n, int d, mxClassID clss, mxComplexity ComplexFlag, int* data)
{
  int dims[] = {m, n, d};
  int ndim = d > 1 ? 3 : 2;
  return mxSetNumericArray(arr, ndim, dims, clss, ComplexFlag, data);
}
int mxCopyArray(mxArray* arr, const mxArray* arr1)
{
  if (arr != arr1) {
    if (mxCELL_CLASS == arr->classID || mxSTRUCT_CLASS == arr->classID) {
      int i, n = mxGetNumberOfElements(arr1);
      mxArray* aa = (mxArray*)arr->data;
      mxArray* aa1 = (mxArray*)arr1->data;
      mxSetNumericArray(arr, arr1->n_dims, arr1->dims, arr1->classID, arr1->iscomplex, 0);
      for (i = 0; i < n; ++i) {
        mxCopyArray(aa + i, aa1 + i);
      }
    }
    else {
      mxSetNumericArray(arr, arr1->n_dims, arr1->dims, arr1->classID, arr1->iscomplex, arr1->data);
    }
  }
  return 0;
}
int mxConvertArray(mxArray* arr, const mxArray* src, int clss, int iscomplex)
{
  int i, len;
  mxArray _src[1] = {0};
  if (src->classID == clss) {
    return mxCopyArray(arr, src);
  }
  if (arr == src) {
    mxCopyArray(_src, src);
    src = _src;
  }
  mxSetNumericArray(arr, src->n_dims, src->dims, clss, iscomplex, 0);
  len = mxGetNumberOfElements(src);
  if (iscomplex == src->iscomplex) {
    len *= iscomplex ? 2 : 1;
    switch (clss) {
    case mxCELL_CLASS:
    case mxSTRUCT_CLASS:
      ASSERT(0);
      break;
#define CLASSSIZEDEF(hh, ID, TYPE, FMT)  case ID: { dst_type* dst = (dst_type*)arr->data; TYPE* a = (TYPE*)src->data; for (i = 0; i < len; i++) {dst[i] = (dst_type)a[i];} } break;
#define CLASSSIZEDEF1(hh, ID, TYPE, FMT)  case ID: { typedef TYPE dst_type; switch (src->classID) { hh; } } break;
      CLASSSIZEDEFNUMBER(CLASSSIZEDEFNUMBER(hh, CLASSSIZEDEF), CLASSSIZEDEF1);
#undef CLASSSIZEDEF
#undef CLASSSIZEDEF1
    }
  }
  else if (iscomplex && !src->iscomplex) {
    switch (clss) {
    case mxCELL_CLASS:
    case mxSTRUCT_CLASS:
      ASSERT(0);
      break;
#define CLASSSIZEDEF(hh, ID, TYPE, FMT)  case ID: { dst_type* dst = (dst_type*)arr->data; TYPE* a = (TYPE*)src->data; for (i = 0; i < len; i++) {dst[i*2+0] = (dst_type)a[i];dst[i*2+1]=0;} } break;
#define CLASSSIZEDEF1(hh, ID, TYPE, FMT)  case ID: { typedef TYPE dst_type; switch (src->classID) { hh; } } break;
      CLASSSIZEDEFNUMBER(CLASSSIZEDEFNUMBER(hh, CLASSSIZEDEF), CLASSSIZEDEF1);
#undef CLASSSIZEDEF
#undef CLASSSIZEDEF1
    }
  }
  else {
    ASSERT(0 && "can not convert complex to real!");
    switch (clss) {
    case mxCELL_CLASS:
    case mxSTRUCT_CLASS:
      ASSERT(0);
      break;
#define CLASSSIZEDEF(hh, ID, TYPE, FMT)  case ID: { dst_type* dst = (dst_type*)arr->data; TYPE* a = (TYPE*)src->data; for (i = 0; i < len; i++) {dst[i] = (dst_type)a[i*2];} } break;
#define CLASSSIZEDEF1(hh, ID, TYPE, FMT)  case ID: { typedef TYPE dst_type; switch (src->classID) { hh; } } break;
      CLASSSIZEDEFNUMBER(CLASSSIZEDEFNUMBER(hh, CLASSSIZEDEF), CLASSSIZEDEF1);
#undef CLASSSIZEDEF
#undef CLASSSIZEDEF1
    }
  }
  mxFreeArray(_src);
  return 0;
}
int mxFillArray(mxArray* arr, double re, double im)
{
  int i, len = mxGetNumberOfElements(arr);
  switch (arr->classID) {
  case mxCELL_CLASS:
  case mxSTRUCT_CLASS:
    ASSERT(0);
    break;
#define CLASSSIZEDEF(hh, ID, TYPE, FMT)  case ID: { TYPE* dst = (TYPE*)arr->data, re1=(TYPE)re, im1=(TYPE)im; if (arr->iscomplex) {for (i = 0; i < len; i++) {dst[i] = re1;}} else {for (i = 0; i < len; i++) {dst[2*i] = re1;dst[2*i+1] = im1;}} } break;
    CLASSSIZEDEFNUMBER(hh, CLASSSIZEDEF);
#undef CLASSSIZEDEF
  }
  return 0;
}
int mxSetCellArray(mxArray* arr, int ndim, const int* dims)
{
  return mxSetNumericArray(arr, ndim, dims, mxCELL_CLASS, mxREAL, 0);
}
int mxSetCellMatrix(mxArray* arr, int m, int n)
{
  int dims[] = {m, n};
  return mxSetNumericArray(arr, 2, dims, mxCELL_CLASS, mxREAL, 0);
}
int mxSetNumericMatrix(mxArray* arr, int m, int n, mxClassID clss, mxComplexity ComplexFlag)
{
  int dims[] = {m, n};
  return mxSetNumericArray(arr, 2, dims, clss, ComplexFlag, 0);
}
int mxSetNumericMatrix3(mxArray* arr, int m, int n, int d, mxClassID clss, mxComplexity ComplexFlag)
{
  int dims[] = {m, n, d};
  return mxSetNumericArray(arr, 3, dims, clss, ComplexFlag, 0);
}
int mxSetDoubleScalar(mxArray* arr, double value)
{
  mxSetNumericMatrix(arr, 1, 1, mxDOUBLE_CLASS, mxREAL);
  *(double*)arr->data = value;
  return 0;
}
int mxSetDoubleComplex(mxArray* arr, double r, double i)
{
  mxSetNumericMatrix(arr, 1, 1, mxDOUBLE_CLASS, mxCOMPLEX);
  ((double*)arr->data)[0] = r;
  ((double*)arr->data)[1] = i;
  return 0;
}
int mxSetSingleScalar(mxArray* arr, float value)
{
  mxSetNumericMatrix(arr, 1, 1, mxSINGLE_CLASS, mxREAL);
  *(float*)arr->data = value;
  return 0;
}
int mxSetIndexScalar(mxArray* arr, index_t value)
{
  mxSetNumericMatrix(arr, 1, 1, mxINDEX_CLASS, mxREAL);
  *(index_t*)arr->data = value;
  return 0;
}
int mxSetSingleComplex(mxArray* arr, float r, float i)
{
  mxSetNumericMatrix(arr, 1, 1, mxSINGLE_CLASS, mxCOMPLEX);
  ((float*)arr->data)[0] = r;
  ((float*)arr->data)[1] = i;
  return 0;
}
int mxSetSparse(mxArray* arr, int m, int n, int nzmax, mxComplexity ComplexFlag);
int mxSetSparseLogicalMatrix(mxArray* arr, int m, int n, int nzmax);
int mxSetString(mxArray* arr, const char* str, int n)
{
  n = n < 0 ? strlen(str) : n;
  mxSetNumericMatrix(arr, 1, n, mxCHAR_CLASS, mxREAL);
  memcpy(arr->data, str, n);
  return 0;
}
int mxSetStructArray(int ndim, const int* dims, int nfields, const char** field_names);
int mxSetStructMatrix(int m, int n, int nfields, const char** field_names);
int mxDuplicateArray(const mxArray* in);
int mxGetCell(const mxArray* arr, int index);
mxChar* mxGetChars(const mxArray* arr);
int mxRemoveField(mxArray* arr, int field_number);
int mxSetCell(mxArray* arr, int index, mxArray* value);
int mxSetClassName(mxArray* arr, const char* classname)
{
  ASSERT(0);
  return 0;
}
int mxSetField(mxArray* arr, int index,  const char* field_name, mxArray* value);
int mxSetFieldByNumber(mxArray* arr, int index,  int field_number, mxArray* value);
int mxSetImagData(mxArray* arr, int* pi);
int mxSetIr(mxArray* arr, int* ir);
int mxSetJc(mxArray* arr, int* jc);
int mxSetM(mxArray* arr, int m);
int mxSetN(mxArray* arr, int n);
int mxSetNzmax(mxArray* arr, int nzmax);
int mxSetPi(mxArray* arr, double* pi);
int mxSetPr(mxArray* arr, double* pr);
int mexAtExit(int (*ExitFcn)(int));
int mexCallMATLAB(int nlhs, mxArray* plhs[], int nrhs,  mxArray* prhs[], const char* command_name);
int mexErrMsgIdAndTxt(const char* identifier, const char* error_msg, ...);
int mexErrMsgTxt(const char* error_msg)
{
  printf("err: %s", error_msg);
  return 0;
}
int mexEvalString(const char* command);
int mexFunction(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[]);
const char* mexFunctionName(int);
const mxArray* mexGet(double handle, const char* property);
mxArray* mexGetVariable(const char* workspace, const char* var_name);
const mxArray* mexGetVariablePtr(const char* workspace, const char* var_name);
bool mexIsGlobal(const mxArray* arr);
bool mexIsLocked(int);
int mexLock(int);
int mexMakeArrayPersistent(mxArray* arr);
int mexMakeMemoryPersistent(int* ptr);
int mexPrintf(const char* format, ...);
int mexPutVariable(const char* workspace, const char* var_name, const mxArray* arr);
int mexSet(double handle, const char* property,  mxArray* value);
int mexSetTrapFlag(int trap_flag);
int mexUnlock(int);
int mexWarnMsgIdAndTxt(const char* identifier, const char* warning_msg, ...);
int mexWarnMsgTxt(const char* warning_msg);
#endif // __MEX_H__

