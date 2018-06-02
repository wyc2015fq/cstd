#include "cstd.h"
#define NO_ASINH
#define SUPPORT_OPTIMIZER
char* slipcat(char* s, int l, const char* s1, int l1, int ch)
{
  char cc[4] = {ch, 0};
  CSTRINITLEN(s, l);
  if (NULL == s) {
    MYREALLOC(s, l1 + 4);
    cstr_cat3(s, 0, cc, 1, s1, l1, cc, 1);
  }
  else {
    MYREALLOC(s, l + l1 + 4);
    cstr_cat2(s, l, s1, l1, cc, 1);
  }
  return s;
}
#ifndef M_PI
#define M_PI 3.1415926535897932384626433832795
#endif
enum ParseErrorType {
  SYNTAX_ERROR = 0, MISM_PARENTH, MISSING_PARENTH, EMPTY_PARENTH,
  EXPECT_OPERATOR, OUT_OF_MEMORY, UNEXPECTED_ERROR, INVALID_VARS,
  ILL_PARAMS_AMOUNT, PREMATURE_EOS, EXPECT_PARENTH_FUNC,
  FP_NO_ERROR
};
// The functions must be in alphabetical order:
enum OPCODE {
  cAbs, cAcos,
#ifndef NO_ASINH
  cAcosh,
#endif
  cAsin,
#ifndef NO_ASINH
  cAsinh,
#endif
  cAtan,
  cAtan2,
#ifndef NO_ASINH
  cAtanh,
#endif
  cCeil, cCos, cCosh, cCot, cCsc,
#ifndef DISABLE_EVAL
  cEval,
#endif
  cExp, cFloor, cIf, cInt, cLog, cLog10, cMax, cMin,
  cSec, cSin, cSinh, cSqrt, cTan, cTanh,
  // These do not need any ordering:
  cImmed, cJump,
  cNeg, cAdd, cSub, cMul, cDiv, cMod, cPow,
  cEqual, cLess, cGreater, cAnd, cOr,
  cDeg, cRad,
  cFCall, cPCall,
#ifdef SUPPORT_OPTIMIZER
  cVar, cDup, cInv,
#endif
  VarBegin
};
typedef struct Vstack {
  double* v;
  int n;
  int size;
} Vstack;
typedef int (*FunctionPtr)(Vstack* s);
struct FuncPtrData {
  FunctionPtr ptr;
  int params;
};
struct FunctionParser {
  int parseErrorType;
  int evalErrorType;
  int varAmount;
  bool useDegreeConversion;
  char* Variables;
  int Variables_size;
  char* ConstantsNames;
  double* Constants;
  int Constants_size;
  char* FuncPtrNames;
  FuncPtrData* FuncPtrs;
  int FuncPtrs_size;
  char* FuncParserNames;
  FunctionParser** FuncParsers;
  int FuncParsers_size;
  int* ByteCode;
  int ByteCodeSize;
  double* Immed;
  int ImmedSize;
  double* Stack;
  int StackSize;
  // Temp s needed in Compile():
  int StackPtr;
  int** tempByteCode;
  double** tempImmed;
};
struct FuncDefinition {
  const char* name;
  int nameLength;
  int opcode;
  int params;
};
// This is basically strcmp(), but taking 'nameLength' as string
// length (not ending '\0'):
bool str_less(const char* lhs, int lhs_len, const char* rhs, int rhs_len)
{
  int i;
  for (i = 0; i < lhs_len; ++i) {
    if (i == rhs_len) {
      return false;
    }
    const char c1 = lhs[i], c2 = rhs[i];
    if (c1 < c2) {
      return true;
    }
    if (c2 < c1) {
      return false;
    }
  }
  return lhs_len < rhs_len;
}
// This list must be in alphabetical order:
const FuncDefinition Functions[] = {
  { "abs", 3, cAbs, 1 },
  { "acos", 4, cAcos, 1 },
#ifndef NO_ASINH
  { "acosh", 5, cAcosh, 1 },
#endif
  { "asin", 4, cAsin, 1 },
#ifndef NO_ASINH
  { "asinh", 5, cAsinh, 1 },
#endif
  { "atan", 4, cAtan, 1 },
  { "atan2", 5, cAtan2, 2 },
#ifndef NO_ASINH
  { "atanh", 5, cAtanh, 1 },
#endif
  { "ceil", 4, cCeil, 1 },
  { "cos", 3, cCos, 1 },
  { "cosh", 4, cCosh, 1 },
  { "cot", 3, cCot, 1 },
  { "csc", 3, cCsc, 1 },
#ifndef DISABLE_EVAL
  { "eval", 4, cEval, 0 },
#endif
  { "exp", 3, cExp, 1 },
  { "floor", 5, cFloor, 1 },
  { "if", 2, cIf, 0 },
  { "int", 3, cInt, 1 },
  { "log", 3, cLog, 1 },
  { "log10", 5, cLog10, 1 },
  { "max", 3, cMax, 2 },
  { "min", 3, cMin, 2 },
  { "sec", 3, cSec, 1 },
  { "sin", 3, cSin, 1 },
  { "sinh", 4, cSinh, 1 },
  { "sqrt", 4, cSqrt, 1 },
  { "tan", 3, cTan, 1 },
  { "tanh", 4, cTanh, 1 }
};
const int FUNC_AMOUNT = sizeof(Functions) / sizeof(Functions[0]);
// BCB4 does not implement the standard lower_bound function.
// This is used instead:
const FuncDefinition* fp_lower_bound(const FuncDefinition* first,
    const FuncDefinition* last,
    const FuncDefinition* value)
{
  while (first < last) {
    const FuncDefinition* middle = first + (last - first) / 2;
    if (str_less(middle->name, middle->nameLength, value->name, value->nameLength)) {
      first = middle + 1;
    }
    else {
      last = middle;
    }
  }
  return last;
}
void FunctionParser_init(FunctionParser* s)
{
  s->parseErrorType = (FP_NO_ERROR), s->evalErrorType = (0);
}
void FunctionParser_free(FunctionParser* s)
{
  FREE(s->ByteCode);
  FREE(s->Immed);
  FREE(s->Stack);
  FREE(s);
}
#if 0
// Makes a deep-copy of Data:
void FunctionParser_copy(FunctionParser* s, const FunctionParser* cpy)
{
  int i;
  s->varAmount = (cpy->varAmount), s->useDegreeConversion = (cpy->useDegreeConversion),
     s->Variables = (cpy->Variables), s->Constants = (cpy->Constants),
        s->FuncPtrNames = (cpy->FuncPtrNames), s->FuncPtrs = (cpy->FuncPtrs),
           s->FuncParserNames = (cpy->FuncParserNames), s->FuncParsers = (cpy->FuncParsers),
              s->ByteCode = (0), s->ByteCodeSize = (cpy->ByteCodeSize),
                 s->Immed = (0), s->ImmedSize = (cpy->ImmedSize),
                    s->Stack = (0), s->StackSize = (cpy->StackSize);
  MYREALLOC(s->ByteCode, s->ByteCodeSize);
  MYREALLOC(s->Immed, s->ImmedSize);
  MYREALLOC(s->Stack, s->StackSize);
  for (i = 0; i < s->ByteCodeSize; ++i) {
    s->ByteCode[i] = cpy->ByteCode[i];
  }
  for (i = 0; i < s->ImmedSize; ++i) {
    s->Immed[i] = cpy->Immed[i];
  }
  // No need to copy the stack contents because it's obsolete outside Eval()
}
//---------------------------------------------------------------------------
// Function parsing
//---------------------------------------------------------------------------
//===========================================================================
// Error messages returned by ErrorMsg():
const char* ParseErrorMessage[] = {
  "Syntax error",                             // 0
  "Mismatched parenthesis",                   // 1
  "Missing ')'",                              // 2
  "Empty parentheses",                        // 3
  "Syntax error: Operator expected",          // 4
  "Not enough memory",                        // 5
  "An unexpected error ocurred. Please make a full bug report "
  "to warp@iki.fi",                           // 6
  "Syntax error in parameter 'Vars' given to "
  "FunctionParser::Parse()",                  // 7
  "Illegal number of parameters to function", // 8
  "Syntax error: Premature end of string",    // 9
  "Syntax error: Expecting ( after function", // 10
  ""
};
// Returns a pointer to the FuncDefinition instance which 'name' is
// the same as the one given by 'F'. If no such function name exists,
// returns 0.
inline const FuncDefinition* FunctionParser_FindFunction(const char* F, int F_size)
{
  FuncDefinition func[1] = { F, F_size, 0, 0 };
  //while(isalnum(F[func.nameLength])) ++func.nameLength;
  if (func->nameLength) {
    const FuncDefinition* found = fp_lower_bound(Functions, Functions + FUNC_AMOUNT, func);
    if (found == Functions + FUNC_AMOUNT || str_less(func->name, func->nameLength, found->name, found->nameLength)) {
      return 0;
    }
    return found;
  }
  return 0;
}
bool FunctionParser_isValidName(const char* name, int name_size)
{
  int i;
  if (name_size <= 0 || (!isalpha(name[0]) && name[0] != '_')) {
    return false;
  }
  for (i = 0; i < name_size; ++i)
    if (!isalnum(name[i]) && name[i] != '_') {
      return false;
    }
  if (FunctionParser_FindFunction(name, name_size)) {
    return false;
  }
  return true;
}
// Constants:
bool FunctionParser_AddConstant(FunctionParser* s, const char* name, int name_size, double value)
{
  if (FunctionParser_isValidName(name, name_size)) {
    int n = 0;
    if (cstr_splitfind(s->FuncParserNames, -1, name, name_size, 1, 0) >= 0 ||
        cstr_splitfind(s->FuncPtrNames, -1, name, name_size, 1, 0) >= 0) {
      return false;
    }
    s->ConstantsNames = slipcat(s->ConstantsNames,  name, name_size, '|');
    MYREALLOC(s->Constants, s->Constants_size + 1);
    s->Constants[s->Constants_size++] = value;
    return true;
  }
  return false;
}
// Function pointers
bool FunctionParser_AddFunction(FunctionParser* s, const char* name, int name_size, FunctionPtr func, int paramsAmount)
{
  if (paramsAmount == 0) {
    return false;  // Currently must be at least one
  }
  if (FunctionParser_isValidName(name, name_size)) {
    if (cstr_splitfind(s->FuncParserNames, -1, name, name_size, 1, 0) >= 0 ||
        cstr_splitfind(s->ConstantsNames, -1, name, name_size, 1, 0) >= 0) {
      return false;
    }
    s->FuncPtrNames = slipcat(s->FuncPtrNames,  name, name_size, '|');
    MYREALLOC(s->FuncPtrs, s->FuncPtrs_size + 1);
    s->FuncPtrs[s->FuncPtrs_size++] = (FuncPtrData(func, paramsAmount));
    return true;
  }
  return false;
}
bool FunctionParser_checkRecursiveLinking(const FunctionParser* s, const FunctionParser* fp)
{
  int i;
  if (fp == s) {
    return true;
  }
  for (i = 0; i < fp->FuncParsers_size; ++i) {
    if (FunctionParser_checkRecursiveLinking(s, fp->FuncParsers[i])) {
      return true;
    }
  }
  return false;
}
bool FunctionParser_AddFunctionParser(FunctionParser* s, const char* name, int name_size, FunctionParser* parser)
{
  if (parser->varAmount == 0) { // Currently must be at least one
    return false;
  }
  if (FunctionParser_isValidName(name, name_size)) {
    if (cstr_splitfind(s->FuncPtrNames, -1, name, name_size, 1, 0) >= 0 ||
        cstr_splitfind(s->ConstantsNames, -1, name, name_size, 1, 0) >= 0) {
      return false;
    }
    s->FuncParserNames = slipcat(s->FuncParserNames,  name, name_size, '|');
    if (FunctionParser_checkRecursiveLinking(s, parser)) {
      return false;
    }
    MYREALLOC(s->FuncParsers, s->FuncParsers_size + 1);
    s->FuncParsers[s->FuncParsers_size++] = (parser);
    return true;
  }
  return false;
}
// Compile function string to bytecode
// -----------------------------------
bool FunctionParser_Compile(const char* Function)
{
  if (s->ByteCode) {
    delete[] s->ByteCode;
    s->ByteCode = 0;
  }
  if (s->Immed) {
    delete[] s->Immed;
    s->Immed = 0;
  }
  if (s->Stack) {
    delete[] s->Stack;
    s->Stack = 0;
  }
  int* byteCode = NULL;
  MYREALLOC(byteCode, 1024);
  tempByteCode = &byteCode;
  vector<double> immed;
  immed.reserve(1024);
  tempImmed = &immed;
  s->StackSize = StackPtr = 0;
  CompileExpression(Function, 0);
  if (s->parseErrorType != FP_NO_ERROR) {
    return false;
  }
  s->ByteCodeSize = byteCode_size;
  s->ImmedSize = immed_size;
  if (s->ByteCodeSize) {
    s->ByteCode = new int[s->ByteCodeSize];
    memcpy(s->ByteCode, &byteCode[0],
        sizeof(int)*s->ByteCodeSize);
  }
  if (s->ImmedSize) {
    s->Immed = new double[s->ImmedSize];
    memcpy(s->Immed, &immed[0],
        sizeof(double)*s->ImmedSize);
  }
  if (s->StackSize) {
    s->Stack = new double[s->StackSize];
  }
  return true;
}
inline void FunctionParser_AddCompiledByte(int c)
{
  tempByteCode->push_back(c);
}
inline void FunctionParser_AddImmediate(double i)
{
  tempImmed->push_back(i);
}
inline void FunctionParser_AddFunctionOpcode(int opcode)
{
  if (s->useDegreeConversion) {
    switch (opcode) {
    case cCos:
    case cCosh:
    case cCot:
    case cCsc:
    case cSec:
    case cSin:
    case cSinh:
    case cTan:
    case cTanh:
      AddCompiledByte(cRad);
    }
  }
  AddCompiledByte(opcode);
  if (s->useDegreeConversion) {
    switch (opcode) {
    case cAcos:
#ifndef NO_ASINH
    case cAcosh:
    case cAsinh:
    case cAtanh:
#endif
    case cAsin:
    case cAtan:
    case cAtan2:
      AddCompiledByte(cDeg);
    }
  }
}
inline void FunctionParser_incStackPtr()
{
  if (++StackPtr > s->StackSize) {
    ++(s->StackSize);
  }
}
// Compile if()
int FunctionParser_CompileIf(const char* F, int ind)
{
  int ind2 = CompileExpression(F, ind, true); // condition
  sws(F, ind2);
  if (F[ind2] != ',') {
    s->parseErrorType = ILL_PARAMS_AMOUNT;
    return ind2;
  }
  AddCompiledByte(cIf);
  int curByteCodeSize = tempByteCode->size();
  AddCompiledByte(0); // Jump index; to be set later
  AddCompiledByte(0); // Immed jump index; to be set later
  --StackPtr;
  ind2 = CompileExpression(F, ind2 + 1, true); // then
  sws(F, ind2);
  if (F[ind2] != ',') {
    s->parseErrorType = ILL_PARAMS_AMOUNT;
    return ind2;
  }
  AddCompiledByte(cJump);
  int curByteCodeSize2 = tempByteCode->size();
  int curImmedSize2 = tempImmed->size();
  AddCompiledByte(0); // Jump index; to be set later
  AddCompiledByte(0); // Immed jump index; to be set later
  --StackPtr;
  ind2 = CompileExpression(F, ind2 + 1, true); // else
  sws(F, ind2);
  if (F[ind2] != ')') {
    s->parseErrorType = ILL_PARAMS_AMOUNT;
    return ind2;
  }
  // Set jump indices
  (*tempByteCode)[curByteCodeSize] = curByteCodeSize2 + 1;
  (*tempByteCode)[curByteCodeSize + 1] = curImmedSize2;
  (*tempByteCode)[curByteCodeSize2] = tempByteCode->size() - 1;
  (*tempByteCode)[curByteCodeSize2 + 1] = tempImmed->size();
  return ind2 + 1;
}
int FunctionParser_CompileFunctionParams(const char* F, int ind,
    int requiredParams)
{
  int curStackPtr = StackPtr;
  int ind2 = CompileExpression(F, ind);
  if (StackPtr != curStackPtr + requiredParams) {
    s->parseErrorType = ILL_PARAMS_AMOUNT;
    return ind;
  }
  StackPtr -= requiredParams - 1;
  sws(F, ind2);
  return ind2 + 1; // F[ind2] is ')'
}
// Compiles element
int FunctionParser_CompileElement(const char* F, int ind)
{
  sws(F, ind);
  char c = F[ind];
  if (c == '(') {
    ind = CompileExpression(F, ind + 1);
    sws(F, ind);
    return ind + 1; // F[ind] is ')'
  }
  if (isdigit(c) || c == '.' /*|| c=='-'*/) { // Number
    const char* startPtr = &F[ind];
    char* endPtr;
    double val = strtod(startPtr, &endPtr);
    AddImmediate(val);
    AddCompiledByte(cImmed);
    incStackPtr();
    return ind + (endPtr - startPtr);
  }
  if (isalpha(c) || c == '_') { // Function, variable or constant
    const FuncDefinition* func = FindFunction(F + ind);
    if (func) { // is function
      int ind2 = ind + func->nameLength;
      sws(F, ind2); // F[ind2] is '('
      if (strcmp(func->name, "if") == 0) { // "if" is a special case
        return CompileIf(F, ind2 + 1);
      }
#ifndef DISABLE_EVAL
      int requiredParams =
          strcmp(func->name, "eval") == 0 ?
          s->Variables_size : func->params;
#else
      int requiredParams = func->params;
#endif
      ind2 = CompileFunctionParams(F, ind2 + 1, requiredParams);
      AddFunctionOpcode(func->opcode);
      return ind2; // F[ind2-1] is ')'
    }
    Data::VarMap_t::const_iterator vIter =
        FindVariable(F + ind, s->Variables);
    if (vIter != s->Variables.end()) { // is variable
      AddCompiledByte(vIter->second);
      incStackPtr();
      return ind + vIter->first_size;
    }
    Data::ConstMap_t::const_iterator cIter = FindConstant(F + ind);
    if (cIter != s->Constants.end()) { // is constant
      AddImmediate(cIter->second);
      AddCompiledByte(cImmed);
      incStackPtr();
      return ind + cIter->first_size;
    }
    Data::VarMap_t::const_iterator fIter =
        FindVariable(F + ind, s->FuncPtrNames);
    if (fIter != s->FuncPtrNames.end()) { // is user-defined func pointer
      int index = fIter->second;
      int ind2 = ind + fIter->first.length();
      sws(F, ind2); // F[ind2] is '('
      ind2 = CompileFunctionParams(F, ind2 + 1,
          s->FuncPtrs[index].params);
      AddCompiledByte(cFCall);
      AddCompiledByte(index);
      return ind2;
    }
    Data::VarMap_t::const_iterator pIter =
        FindVariable(F + ind, s->FuncParserNames);
    if (pIter != s->FuncParserNames.end()) { // is user-defined func parser
      int index = pIter->second;
      int ind2 = ind + pIter->first.length();
      sws(F, ind2); // F[ind2] is '('
      ind2 = CompileFunctionParams
          (F, ind2 + 1, s->FuncParsers[index]->s->varAmount);
      AddCompiledByte(cPCall);
      AddCompiledByte(index);
      return ind2;
    }
  }
  s->parseErrorType = UNEXPECTED_ERROR;
  return ind;
}
// Compiles '^'
int FunctionParser_CompilePow(const char* F, int ind)
{
  int ind2 = CompileElement(F, ind);
  sws(F, ind2);
  while (F[ind2] == '^') {
    ind2 = CompileUnaryMinus(F, ind2 + 1);
    sws(F, ind2);
    AddCompiledByte(cPow);
    --StackPtr;
  }
  return ind2;
}
// Compiles unary '-'
int FunctionParser_CompileUnaryMinus(const char* F, int ind)
{
  sws(F, ind);
  if (F[ind] == '-') {
    int ind2 = ind + 1;
    sws(F, ind2);
    ind2 = CompilePow(F, ind2);
    sws(F, ind2);
    // if we are negating a constant, negate the constant itself:
    if (tempByteCode->back() == cImmed) {
      tempImmed->back() = -tempImmed->back();
    }
    // if we are negating a negation, we can remove both:
    else if (tempByteCode->back() == cNeg) {
      tempByteCode->pop_back();
    }
    else {
      AddCompiledByte(cNeg);
    }
    return ind2;
  }
  int ind2 = CompilePow(F, ind);
  sws(F, ind2);
  return ind2;
}
// Compiles '*', '/' and '%'
int FunctionParser_CompileMult(const char* F, int ind)
{
  int ind2 = CompileUnaryMinus(F, ind);
  sws(F, ind2);
  char op;
  while ((op = F[ind2]) == '*' || op == '/' || op == '%') {
    ind2 = CompileUnaryMinus(F, ind2 + 1);
    sws(F, ind2);
    switch (op) {
    case '*':
      AddCompiledByte(cMul);
      break;
    case '/':
      AddCompiledByte(cDiv);
      break;
    case '%':
      AddCompiledByte(cMod);
      break;
    }
    --StackPtr;
  }
  return ind2;
}
// Compiles '+' and '-'
int FunctionParser_CompileAddition(const char* F, int ind)
{
  int ind2 = CompileMult(F, ind);
  sws(F, ind2);
  char op;
  while ((op = F[ind2]) == '+' || op == '-') {
    ind2 = CompileMult(F, ind2 + 1);
    sws(F, ind2);
    AddCompiledByte(op == '+' ? cAdd : cSub);
    --StackPtr;
  }
  return ind2;
}
// Compiles '=', '<' and '>'
int FunctionParser_CompileComparison(const char* F, int ind)
{
  int ind2 = CompileAddition(F, ind);
  sws(F, ind2);
  char op;
  while ((op = F[ind2]) == '=' || op == '<' || op == '>') {
    ind2 = CompileAddition(F, ind2 + 1);
    sws(F, ind2);
    switch (op) {
    case '=':
      AddCompiledByte(cEqual);
      break;
    case '<':
      AddCompiledByte(cLess);
      break;
    case '>':
      AddCompiledByte(cGreater);
      break;
    }
    --StackPtr;
  }
  return ind2;
}
// Compiles '&'
int FunctionParser_CompileAnd(const char* F, int ind)
{
  int ind2 = CompileComparison(F, ind);
  sws(F, ind2);
  while (F[ind2] == '&') {
    ind2 = CompileComparison(F, ind2 + 1);
    sws(F, ind2);
    AddCompiledByte(cAnd);
    --StackPtr;
  }
  return ind2;
}
// Compiles '|'
int FunctionParser_CompileOr(const char* F, int ind)
{
  int ind2 = CompileAnd(F, ind);
  sws(F, ind2);
  while (F[ind2] == '|') {
    ind2 = CompileAnd(F, ind2 + 1);
    sws(F, ind2);
    AddCompiledByte(cOr);
    --StackPtr;
  }
  return ind2;
}
// Compiles ','
int FunctionParser_CompileExpression(const char* F, int ind, bool stopAtComma)
{
  int ind2 = CompileOr(F, ind);
  sws(F, ind2);
  if (stopAtComma) {
    return ind2;
  }
  while (F[ind2] == ',') {
    ind2 = CompileOr(F, ind2 + 1);
    sws(F, ind2);
  }
  return ind2;
}
// Return parse error message
// --------------------------
const char* FunctionParser_ErrorMsg() const
{
  if (s->parseErrorType != FP_NO_ERROR) {
    return ParseErrorMessage[s->parseErrorType];
  }
  return 0;
}
//---------------------------------------------------------------------------
// Function evaluation
//---------------------------------------------------------------------------
//===========================================================================
inline int doubleToInt(double d)
{
  return d < 0 ? -int((-d) + .5) : int(d + .5);
}
inline double Min(double d1, double d2)
{
  return d1 < d2 ? d1 : d2;
}
inline double Max(double d1, double d2)
{
  return d1 > d2 ? d1 : d2;
}
inline double DegreesToRadians(double degrees)
{
  return degrees * (M_PI / 180.0);
}
inline double RadiansToDegrees(double radians)
{
  return radians * (180.0 / M_PI);
}
double FunctionParser_Eval(const double* Vars)
{
  const int* const ByteCode = s->ByteCode;
  const double* const Immed = s->Immed;
  double* const Stack = s->Stack;
  const int ByteCodeSize = s->ByteCodeSize;
  int IP, DP = 0;
  int SP = -1;
  for (IP = 0; IP < ByteCodeSize; ++IP) {
    switch (ByteCode[IP]) {
      // Functions:
    case   cAbs:
      Stack[SP] = fabs(Stack[SP]);
      break;
    case  cAcos:
      if (Stack[SP] < -1 || Stack[SP] > 1) {
        evalErrorType = 4;
        return 0;
      }
      Stack[SP] = acos(Stack[SP]);
      break;
#ifndef NO_ASINH
    case cAcosh:
      Stack[SP] = acosh(Stack[SP]);
      break;
#endif
    case  cAsin:
      if (Stack[SP] < -1 || Stack[SP] > 1) {
        evalErrorType = 4;
        return 0;
      }
      Stack[SP] = asin(Stack[SP]);
      break;
#ifndef NO_ASINH
    case cAsinh:
      Stack[SP] = asinh(Stack[SP]);
      break;
#endif
    case  cAtan:
      Stack[SP] = atan(Stack[SP]);
      break;
    case cAtan2:
      Stack[SP - 1] = atan2(Stack[SP - 1], Stack[SP]);
      --SP;
      break;
#ifndef NO_ASINH
    case cAtanh:
      Stack[SP] = atanh(Stack[SP]);
      break;
#endif
    case  cCeil:
      Stack[SP] = ceil(Stack[SP]);
      break;
    case   cCos:
      Stack[SP] = cos(Stack[SP]);
      break;
    case  cCosh:
      Stack[SP] = cosh(Stack[SP]);
      break;
    case   cCot: {
      double t = tan(Stack[SP]);
      if (t == 0) {
        evalErrorType = 1;
        return 0;
      }
      Stack[SP] = 1 / t;
      break;
    }
    case   cCsc: {
      double s = sin(Stack[SP]);
      if (s == 0) {
        evalErrorType = 1;
        return 0;
      }
      Stack[SP] = 1 / s;
      break;
    }
#ifndef DISABLE_EVAL
    case  cEval: {
      s->Stack = new double[s->StackSize];
      double retVal = Eval(&Stack[SP - s->varAmount + 1]);
      delete[] s->Stack;
      s->Stack = Stack;
      SP -= s->varAmount - 1;
      Stack[SP] = retVal;
      break;
    }
#endif
    case   cExp:
      Stack[SP] = exp(Stack[SP]);
      break;
    case cFloor:
      Stack[SP] = floor(Stack[SP]);
      break;
    case    cIf: {
      int jumpAddr = ByteCode[++IP];
      int immedAddr = ByteCode[++IP];
      if (doubleToInt(Stack[SP]) == 0) {
        IP = jumpAddr;
        DP = immedAddr;
      }
      --SP;
      break;
    }
    case   cInt:
      Stack[SP] = floor(Stack[SP] + .5);
      break;
    case   cLog:
      if (Stack[SP] <= 0) {
        evalErrorType = 3;
        return 0;
      }
      Stack[SP] = log(Stack[SP]);
      break;
    case cLog10:
      if (Stack[SP] <= 0) {
        evalErrorType = 3;
        return 0;
      }
      Stack[SP] = log10(Stack[SP]);
      break;
    case   cMax:
      Stack[SP - 1] = Max(Stack[SP - 1], Stack[SP]);
      --SP;
      break;
    case   cMin:
      Stack[SP - 1] = Min(Stack[SP - 1], Stack[SP]);
      --SP;
      break;
    case   cSec: {
      double c = cos(Stack[SP]);
      if (c == 0) {
        evalErrorType = 1;
        return 0;
      }
      Stack[SP] = 1 / c;
      break;
    }
    case   cSin:
      Stack[SP] = sin(Stack[SP]);
      break;
    case  cSinh:
      Stack[SP] = sinh(Stack[SP]);
      break;
    case  cSqrt:
      if (Stack[SP] < 0) {
        evalErrorType = 2;
        return 0;
      }
      Stack[SP] = sqrt(Stack[SP]);
      break;
    case   cTan:
      Stack[SP] = tan(Stack[SP]);
      break;
    case  cTanh:
      Stack[SP] = tanh(Stack[SP]);
      break;
      // Misc:
    case cImmed:
      Stack[++SP] = Immed[DP++];
      break;
    case  cJump:
      DP = ByteCode[IP + 2];
      IP = ByteCode[IP + 1];
      break;
      // Operators:
    case   cNeg:
      Stack[SP] = -Stack[SP];
      break;
    case   cAdd:
      Stack[SP - 1] += Stack[SP];
      --SP;
      break;
    case   cSub:
      Stack[SP - 1] -= Stack[SP];
      --SP;
      break;
    case   cMul:
      Stack[SP - 1] *= Stack[SP];
      --SP;
      break;
    case   cDiv:
      if (Stack[SP] == 0) {
        evalErrorType = 1;
        return 0;
      }
      Stack[SP - 1] /= Stack[SP];
      --SP;
      break;
    case   cMod:
      if (Stack[SP] == 0) {
        evalErrorType = 1;
        return 0;
      }
      Stack[SP - 1] = fmod(Stack[SP - 1], Stack[SP]);
      --SP;
      break;
    case   cPow:
      Stack[SP - 1] = pow(Stack[SP - 1], Stack[SP]);
      --SP;
      break;
    case cEqual:
      Stack[SP - 1] = (Stack[SP - 1] == Stack[SP]);
      --SP;
      break;
    case  cLess:
      Stack[SP - 1] = (Stack[SP - 1] < Stack[SP]);
      --SP;
      break;
    case cGreater:
      Stack[SP - 1] = (Stack[SP - 1] > Stack[SP]);
      --SP;
      break;
    case   cAnd:
      Stack[SP - 1] =
          (doubleToInt(Stack[SP - 1]) &&
              doubleToInt(Stack[SP]));
      --SP;
      break;
    case    cOr:
      Stack[SP - 1] =
          (doubleToInt(Stack[SP - 1]) ||
              doubleToInt(Stack[SP]));
      --SP;
      break;
      // Degrees-radians conversion:
    case   cDeg:
      Stack[SP] = RadiansToDegrees(Stack[SP]);
      break;
    case   cRad:
      Stack[SP] = DegreesToRadians(Stack[SP]);
      break;
      // User-defined function calls:
    case cFCall: {
      int index = ByteCode[++IP];
      int params = s->FuncPtrs[index].params;
      double retVal =
          s->FuncPtrs[index].ptr(&Stack[SP - params + 1]);
      SP -= params - 1;
      Stack[SP] = retVal;
      break;
    }
    case cPCall: {
      int index = ByteCode[++IP];
      int params = s->FuncParsers[index]->s->varAmount;
      double retVal =
          s->FuncParsers[index]->Eval(&Stack[SP - params + 1]);
      SP -= params - 1;
      Stack[SP] = retVal;
      break;
    }
#ifdef SUPPORT_OPTIMIZER
    case   cVar:
      break; // Paranoia. These should never exist
    case   cDup:
      Stack[SP + 1] = Stack[SP];
      ++SP;
      break;
    case   cInv:
      if (Stack[SP] == 0.0) {
        evalErrorType = 1;
        return 0;
      }
      Stack[SP] = 1.0 / Stack[SP];
      break;
#endif
      // Variables:
    default:
      Stack[++SP] = Vars[ByteCode[IP] - VarBegin];
    }
  }
  evalErrorType = 0;
  return Stack[SP];
}
inline void printHex(std::ostream& dest, int n)
{
  dest.width(8);
  dest.fill('0');
  hex(dest); //uppercase(dest);
  dest << n;
}
void FunctionParser_PrintByteCode(std::ostream& dest) const
{
  const int* const ByteCode = s->ByteCode;
  const double* const Immed = s->Immed;
  for (int IP = 0, DP = 0; IP < s->ByteCodeSize; ++IP) {
    printHex(dest, IP);
    dest << ": ";
    int opcode = ByteCode[IP];
    switch (opcode) {
    case cIf:
      dest << "jz\t";
      printHex(dest, ByteCode[IP + 1] + 1);
      dest << endl;
      IP += 2;
      break;
    case cJump:
      dest << "jump\t";
      printHex(dest, ByteCode[IP + 1] + 1);
      dest << endl;
      IP += 2;
      break;
    case cImmed:
      dest.precision(10);
      dest << "push\t" << Immed[DP++] << endl;
      break;
    case cFCall: {
      int index = ByteCode[++IP];
      Data::VarMap_t::const_iterator iter =
          s->FuncPtrNames.begin();
      while (iter->second != index) {
        ++iter;
      }
      dest << "call\t" << iter->first << endl;
      break;
    }
    case cPCall: {
      int index = ByteCode[++IP];
      Data::VarMap_t::const_iterator iter =
          s->FuncParserNames.begin();
      while (iter->second != index) {
        ++iter;
      }
      dest << "call\t" << iter->first << endl;
      break;
    }
    default:
      if (opcode < VarBegin) {
        string n;
        switch (opcode) {
        case cNeg:
          n = "neg";
          break;
        case cAdd:
          n = "add";
          break;
        case cSub:
          n = "sub";
          break;
        case cMul:
          n = "mul";
          break;
        case cDiv:
          n = "div";
          break;
        case cMod:
          n = "mod";
          break;
        case cPow:
          n = "pow";
          break;
        case cEqual:
          n = "eq";
          break;
        case cLess:
          n = "lt";
          break;
        case cGreater:
          n = "gt";
          break;
        case cAnd:
          n = "and";
          break;
        case cOr:
          n = "or";
          break;
        case cDeg:
          n = "deg";
          break;
        case cRad:
          n = "rad";
          break;
#ifndef DISABLE_EVAL
        case cEval:
          n = "call\t0";
          break;
#endif
#ifdef SUPPORT_OPTIMIZER
        case cVar:
          n = "(var)";
          break;
        case cDup:
          n = "dup";
          break;
        case cInv:
          n = "inv";
          break;
#endif
        default:
          n = Functions[opcode - cAbs].name;
        }
        dest << n << endl;
      }
      else {
        dest << "push\tVar" << opcode - VarBegin << endl;
      }
    }
  }
}
#include <assert.h>
#endif
// Parse variables
bool FunctionParser_ParseVars(FunctionParser* s, const char* Vars)
{
  int varNumber = VarBegin;
  int ind1 = 0, ind2;
  int Vars_size = strlen(Vars);
  while (ind1 < Vars_size) {
    if (!isalpha(Vars[ind1]) && Vars[ind1] != '_') {
      return false;
    }
    for (ind2 = ind1 + 1; ind2 < Vars_size && Vars[ind2] != ','; ++ind2)
      if (!isalnum(Vars[ind2]) && Vars[ind2] != '_') {
        return false;
      }
    const char* varName = Vars + ind1;
    int varName_len = ind2 - ind1;
    if (cstr_splitfind(s->Variables, -1, varName, varName_len, 1, 0) < 0) {
      s->Variables = slipcat(s->Variables, -1, varName, varName_len, '|');
      return false;
    }
    ind1 = ind2 + 1;
  }
  return true;
}
// Is given char an operator?
inline bool IsOperator(int c)
{
  return strchr("+-*/%^=<>&|", c) != NULL;
}
// skip whitespace
#define sws(F, Ind)   while (F[Ind] && isspace(F[Ind])) { ++Ind; }
// Main parsing function
int FunctionParser_Parse(FunctionParser* s, const char* Function, const char* Vars, bool useDegrees)
{
  int Function_size = strlen(Function);
  s->Variables_size = 0;
  if (!FunctionParser_ParseVars(s, Vars)) {
    s->parseErrorType = INVALID_VARS;
    return Function_size;
  }
  s->varAmount = s->Variables_size; // this is for Eval()
  const char* Func = Function;
  s->parseErrorType = FP_NO_ERROR;
  s->useDegreeConversion = useDegrees;
  if (!Compile(Func)) {
    return Function_size;
  }
  s->Variables.clear();
  s->parseErrorType = FP_NO_ERROR;
  return -1;
}
#define TESTXY(FN)  {double variables[] = {x, y}; FunctionParser_Parse(fp, #FN , "x,y"); result1 = FunctionParser_Eval(fp, variables); result2 = FN; assert((err = fabs(result1-result2))<0.001); }
int test_fparser()
{
  double result1, result2, err = 0;
  FunctionParser fp[1] = {0};
  double x = 1.5, y = 2.9;
  TESTXY(sqrt(x * x + y * y));
  TESTXY(sin(x * x + y * y));
  FunctionParser_free(fp);
  return 0;
}

