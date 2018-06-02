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
#include "mxArray.h"
#include "Struct.h"
#include "interpreter_t.h"
#include "Algorithms.h"
#include "HandleList.h"
#include "Parser.h"
#include "AnonFunc.h"

#define LOOKUP(x,field) x.constStructPtr()[field].get(1)

// Suppose we have: func1/func2, filename = path1/func1.m
// And we have locally: func1/func2, filename = path2/func1.m
// If we lookup func1/func2, and compare filename to our location indicator
// we can easily spot that we do not have the correct definition for
// the function in question.  But we cannot resurrect the function.  This
// is because if we execute the function in question, it will overwrite
// functions that are locally defined.

static HandleList<VariableTable*> scopeHandles;

fun_t* FuncPtrLookup(interpreter_t* eval, mxArray ptr)
{
  if ((!ptr.mxIsUserClass()) || (ptr.className() != "functionpointer")) {
    throw_Exception("expected function pointer here, instead got " + ptr.className());
  }

  QString name = LOOKUP(ptr, "name").asString();
  QString type = LOOKUP(ptr, "type").asString();

  if (type == "builtin") {
    return eval->getContext()->lookupBuiltinFunction(name);
  }

  if (type == "mfunction") {
    // Retrieve it from the cache of
    QString filename = LOOKUP(ptr, "location").asString();
    fun_t* val = eval->getContext()->lookupCapturedMFunction(name, filename);

    if (!val) {
      throw_Exception("Function pointed to by function pointer does not exist!");
    }

    return val;
  }

  throw_Exception("Not yet finished");
}

mxArray FuncPtrConstructor(interpreter_t* eval, fun_t* val)
{
  StringVector fields;
  fields.push_back("name");
  fields.push_back("type");
  fields.push_back("location");
  fields.push_back("captured");
  fields.push_back("workspace");
  ArrayVector values;
  values.push_back(mxArray(val->name));
  QString typecode;

  switch (val->type()) {
  default:
    typecode = "unknown";
    break;

  case FM_M_FUNCTION:
    typecode = "mfunction";
    break;

  case FM_BUILT_IN_FUNCTION:
  case FM_SPECIAL_FUNCTION:
    typecode = "builtin";
    break;

  case FM_IMPORTED_FUNCTION:
    typecode = "import";
    throw_Exception("Cannot use function pointers on imported functions");
    break;
  }

  values.push_back(mxArray(typecode));
  QString location = "";

  if (val->type() == FM_M_FUNCTION) {
    MFunctionDef* mptr;
    mptr = (MFunctionDef*) val;
    eval->getContext()->captureMFunction(val);
    location = mptr->fileName;
  }

  values.push_back(mxArray(location));
  values.push_back(mxArray(FALSE));
  values.push_back(EmptyConstructor());
  mxArray ret(StructConstructor(fields, values));
  ret.structPtr().mxSetClassName("functionpointer");
  return ret;
}

//DOCBLOCK functions_func2str
//@@Signature
//sfunction func2str Func2StrFunction
//input ptr
//output name
ArrayVector Func2StrFunction(int nargout, const ArrayVector& arg, interpreter_t* eval)
{
  if (arg.size() == 0) {
    return ArrayVector();
  }

  mxArray a = arg[0];

  if (a.className() == "functionpointer") {
    return mxArray(LOOKUP(a, "name").asString());
  }

  if (a.className() == "anonfunction") {
    return mxArray(AnonFuncToString(a));
  }

  throw_Exception("func2str requires either a function pointer or anonymous pointer as an argument");
}

//DOCBLOCK functions_str2func
//@@Signature
//sfunction str2func Str2FuncFunction
//input name
//output ptr
ArrayVector Str2FuncFunction(int nargout, const ArrayVector& arg, interpreter_t* eval)
{
  if (arg.size() == 0) {
    return ArrayVector();
  }

  QString txt = arg[0].asString();

  if (txt.startsWith('@')) {
    tree_t t = ParseExpressionString(txt + "\n");

    if (t.first().is(TOK_ANONYMOUS_FUNC)) {
      return AnonFuncConstructor(eval, t.first());
    }

    throw_Exception("Unable to construct anonymous function from string " + txt);
  }
  else {
    fun_t* val;

    if (!eval->lookupFunction(txt, val)) {
      throw_Exception("Unable to resolve " + txt + " to a function call");
    }

    return FuncPtrConstructor(eval, val);
  }
}

//@@Signature
//sfunction @functionpointer:display FuncPtrDispFunction
//input x
//output none
ArrayVector FuncPtrDispFunction(int nargout, const ArrayVector& arg,
    interpreter_t* eval)
{
  for (i = 0; i < arg.size(); i++) {
    eval->outputMessage(" @" + LOOKUP(arg[i], "name").asString() + "\n");
  }

  return ArrayVector();
}

ArrayVector FevalFunction(int nargout, const ArrayVector& arg, interpreter_t* eval);

//@@Signature
//function @functionpointer:horzcat FuncPtrHorzCatFunction
//input varargin
//output x
ArrayVector FuncPtrHorzCatFunction(int nargout, const ArrayVector& arg)
{
  for (i = 0; i < arg.size(); i++)
    if ((!arg[i].mxIsUserClass()) || (arg[i].className() != "functionpointer")) {
      throw_Exception("Cannot concatenate classes of different types");
    }

  ArrayMatrix t;
  t.push_back(arg);
  return MatrixConstructor(t);
}

static ArrayVector FuncPtrCall(int nargout, ArrayVector& args,
    fun_t* fptr, interpreter_t* eval,
    mxArray* optr)
{
  fptr->updateCode(eval);
  StructArray& rp(optr.structPtr());
  mxArray wsHandle(rp["workspace"].get(1));
  VariableTable* vtable = NULL;

  if (!wsHandle.isEmpty()) {
    int workspaceHandle = wsHandle.asInteger();
    vtable = scopeHandles.lookupHandle(workspaceHandle);
  }

  return (eval->doFunction(fptr, args, nargout, vtable));
}

//@@Signature
//sfunction @functionpointer:subsref FuncPtrSubsrefFunction
//input x s
//output varargout
ArrayVector FuncPtrSubsrefFunction(int nargout, const ArrayVector& arg, interpreter_t* eval)
{
  if (arg.size() == 0) {
    return ArrayVector();
  }

  mxArray mp(arg[0]);
  fun_t* fptr = FuncPtrLookup(eval, mp);

  if (!fptr)
    throw_Exception("Unable to find a definition for function:" +
        LOOKUP(mp, "name").asString());

  ArrayVector fevalArgs;

  if (arg.size() == 2) {
    if (LOOKUP(arg[1], "type").asString() != "()") {
      throw_Exception("for function pointers, only p(x) is defined");
    }

    const mxArray* sub(LOOKUP(arg[1], "subs"));
    const BasicArray<mxArray>& rp(sub.constReal<mxArray>());

    for (index_t i = 1; i <= rp.length(); i++) {
      fevalArgs.push_back(rp[i]);
    }
  }

  return FuncPtrCall(nargout, fevalArgs, fptr, eval, mp);
}

//@@Signature
//sfunction @functionpointer:feval FuncPtrFevalFunction
//input x varargin
//output varargout
ArrayVector FuncPtrFevalFunction(int nargout, const ArrayVector& arg, interpreter_t* eval)
{
  if (arg.size() == 0) {
    return ArrayVector();
  }

  mxArray mp(arg[0]);
  fun_t* fptr = FuncPtrLookup(eval, mp);

  if (!fptr)
    throw_Exception("Unable to find a definition for function:" +
        LOOKUP(mp, "name").asString());

  ArrayVector fevalArgs;

  for (i = 1; i < arg.size(); i++) {
    fevalArgs.push_back(arg[i]);
  }

  return FuncPtrCall(nargout, fevalArgs, fptr, eval, mp);
}

//@@Signature
//sfunction @functionpointer:subsasgn FuncPtrSubsasgnFunction
//input x s y
//output z
ArrayVector FuncPtrSubsasgnFunction(int nargout, const ArrayVector& arg, interpreter_t* eval)
{
  if (arg.size() < 3) {
    throw_Exception("subsasgn requires three input arguments");
  }

  mxArray x(arg[0]);
  mxArray s(arg[1]);
  mxArray y(arg[2]);

  if (!y.isEmpty() && (!y.mxIsUserClass() || y.className() != "functionpointer")) {
    throw_Exception("cannot convert arrays to function pointer type");
  }

  if (LOOKUP(s, "type").asString() != "()") {
    throw_Exception("for function pointers, only p(x) = y is defined");
  }

  const mxArray* sub(LOOKUP(s, "subs"));
  const BasicArray<mxArray>& rp(sub.constReal<mxArray>());

  if (rp.length() != 1) {
    throw_Exception("Expression p(x) = y is invalid");
  }

  x.set(rp.get(1), y);
  return ArrayVector(x);
}

static void CaptureFunctionPointer(mxArray* inp, interpreter_t* walker,
    MFunctionDef* parent)
{
  if (!(LOOKUP(inp, "type").asString() == "mfunction")) {
    return;
  }

  fun_t* ptr = FuncPtrLookup(walker, inp);
  MFunctionDef* mptr = (MFunctionDef*) ptr;

  if (LOOKUP(inp, "captured").toClass(Bool).constRealScalar<BOOL>()) {
    return;
  }

  context_t* context = walker->getContext();
  QString myScope = context->scopeName();
  QString parentScope;
  {
    ParentScopeLocker lock(context);
    parentScope = context->scopeName();
  }

  if (!Scope::nests(parentScope, myScope)) {
    // We need to capture the variables referenced by the
    // function into the workspace
    VariableTable* vptr = new VariableTable;

    for (i = 0; i < mptr->variablesAccessed.size(); i++) {
      ArrayReference ptr(context->lookupVariable(mptr->variablesAccessed[i]));

      if (ptr.valid()) {
        vptr->insertSymbol(mptr->variablesAccessed[i], *ptr);
      }
    }

    inp.structPtr()["workspace"].set(1, mxArray(double(scopeHandles.assignHandle(vptr))));
  }

  inp.structPtr()["captured"].set(1, mxArray(BOOL(TRUE)));
}

void CaptureFunctionPointers(ArrayVector& outputs, interpreter_t* walker,
    MFunctionDef* parent)
{
  for (i = 0; i < outputs.size(); i++) {
    if (outputs[i].mxIsUserClass() &&
        (outputs[i].className() == "functionpointer")) {
      StructArray& rp(outputs[i].structPtr());

      for (index_t j = 1; j <= rp.length(); j++) {
        mxArray fp(outputs[i].get(j));
        CaptureFunctionPointer(fp, walker, parent);
        outputs[i].set(j, fp);
      }
    }
  }
}
