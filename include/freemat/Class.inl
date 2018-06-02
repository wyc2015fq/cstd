
// The meta-info for a class
typedef struct UserClassMetaInfo {
  const char name[32];
  str_t fieldNames[1];
  str_t parentClasses[1];
} UserClassMetaInfo;

// Class information is mapped to a class template through an ID number
//static QMutex classMutex;
typedef struct classTable_t {
  UserClassMetaInfo* v;
  int n;
} classTable_t;

static classTable_t classTable[1] = {0};

// some behavioral observations on classes.
//  The first call to "class" is the definitive one.
//  The exact order of the structure fieldnames must be the same
//     for all objects
//  The list of parent objects must also be the same for all objects
//  So, classes are stored as the following:
//     class UserClass {
//         StringVector fieldNames;
//         StringVector parentClasses;
//     }
//  Also, somewhere we require a table that
//  tracks the hierarchy relationship of the classes.
//
// To Do:
//   change grepping code to look for classes
//   change function eval code to handle classes
//
// These are both done.  Next is the issue of parent classes.
// What does it mean when we have one or more parent classes?
// The structure is simple enough (simply add a new field with
// the name of the parent class).  But what about methods?
// When we call a method of the parent class on the current class
// what does it get passed?
//
//  The answer:
//   Suppose g is of class1, and inherits class2, and class3.
//   Then g has fields class2 and class3.
//   When we call
//     method(g), where method is a member function of class2, then
//   effectively, the output is the same as
//     method(g.class2)
//     p = g
//     p.class2 = method(g.class2)
//   Odd - that's not quite right...  it must be more complicated than that
// Class related issues
//    rhs subscripting
//    assignment
//
// What about function pointers?   - done
//
// Need overload capability for
//   And
//   Or
//   Uplus
//   a(s1,s2,...,sn) - subsref
//   a(s1, ..., sn) - subsasgn
//   b(a) - subsindex
//   [a b] - horzcat
//   [a; b] - vertcat
//   Colon
//   end (!)
//
// More ideas on overloading classes...
//
// What happens when the parent classes are different sizes - resolved
//    force parent classes to be the same size as the created object
//
// In c++, polymorphism is done through the notion of a pointer and
// type casting.  But we can't do exactly the same thing... Because
// when we type-cast, only methods and fields from the type-cast
// object are present...
//
// What we want is
//   a.class1.class2.foo = 32
// In this case, a is of some class (e.g., class3).  But we want to
// call some method on a that belongs to class2.  now, inside the
// method, we want something like
//    x.foo = 32
// but _x_ has to be tagged with prefix information, because _x_ is
// really of class class3.  The tag has to be on the object because
// if there are multiple arguments to the function, they can be
// typecast at different levels.  Also, it tracks only the _instance_
// of the array, not the core array itself.  So the information has
// to be tagged on the array somehow.
//
// One idea is to replace the class name with the class path.  So if
// a is of type class3, but we want to access it as a type class2,
// we "cast" it to type class3:class1:class2.  Then, when accessing
// members of "a", we use the class list to determine the indexing
// sequence.  This casting operation can be done at the dispatch
// level.  Because the "struct" operation simply strips the class name
// from the object, it will still return the intact data array.
//
// To track precedence...
// 1.  Assume that inheritance and precedence do not interact (only the
// outermost class determines precedence).
// 2.  For each class, a list of superior classes is provided.
// 3.  A inf B <--> B sup A
// Precedence is then a simple question of testing interactions.

//
// To revisit the class implementation...
//
// The fundamental issue is that I stored the parent classes
// as fields within the base class.  I'm not sure why I did this,
// but it makes everything more complicated.  I have now replaced
// it with a simpler mechanism that simply merges the parent class
// fields into the fields of the child.
//
// This means that the following is no longer valid:
//   a.class1.class2.foo = 32
//  But I don't really see the point for it.

#if 0
static mxArray ClassAux(const mxArray* s, const char* classname, const StringVector& parentNames,
    const ArrayVector& parents)
{
  QMutexLocker lock(&classMutex);

  if (!s.isEmpty() && (s->classID != Struct)) {
    throw_Exception("first argument to 'class' function must be a structure");
  }

  // First look up the class ID
  UserClassMetaInfo meta;

  if (!classTable.contains(classname)) {
    meta.fieldNames = FieldNames(s);

    for (i = 0; i < parentNames.size(); i++) {
      if (!classTable.contains(parentNames[i])) {
        throw_Exception("parent object types must be defined at time of class creation");
      }

      meta.parentClasses.push_back(parentNames[i]);
    }

    classTable[classname] = meta;
  }
  else {
    meta = classTable.value(classname);
  }

  StringVector s_fields(FieldNames(s));

  if (meta.fieldNames != s_fields) {
    qDebug() << "Registered names " << meta.fieldNames << " vs passed " << s_fields;
    throw_Exception("fieldnames of structure provided must match the fieldnames for the registered class");
  }

  // Now check to make sure all of the parent objects are the same size as the source object
  for (i = 0; i < parents.size(); i++)
    if (s->dims != parents[i]->dims) {
      throw_Exception("parent object much match dimensions of the structure used to make the object");
    }

  mxArray retval(Struct);
  StructArray& rp(retval.structPtr());
  const StructArray& sp(s.constStructPtr());

  // First prepend the fields of the parent classes
  for (i = 0; i < parentNames.size(); i++) {
    const StructArray& pp(parents[i].constStructPtr());
    StringVector p_fields(FieldNames(pp));

    for (j = 0; j < p_fields.size(); j++) {
      rp[p_fields.at(j)] = pp[p_fields.at(j)];
    }
  }

  // Then append the class specific fields
  for (i = 0; i < s_fields.size(); i++) {
    rp[s_fields.at(i)] = sp[s_fields.at(i)];
  }

  rp.mxSetClassName(classname);
  rp.updateDims();
  return retval;
}

//DOCBLOCK class_class

//DOCBLOCK class_constructors

//DOCBLOCK class_horzcat

//DOCBLOCK class_vertcat

//DOCBLOCK class_or

//DOCBLOCK class_and

//DOCBLOCK class_lt

//DOCBLOCK class_gt

//DOCBLOCK class_le

//DOCBLOCK class_ge

//DOCBLOCK class_eq

//DOCBLOCK class_ne

//DOCBLOCK class_plus

//DOCBLOCK class_minus

//DOCBLOCK class_mtimes

//DOCBLOCK class_times

//DOCBLOCK class_rdivide

//DOCBLOCK class_ldivide

//DOCBLOCK class_mrdivide

//DOCBLOCK class_mldivide


//DOCBLOCK class_uminus

//DOCBLOCK class_uplus

//DOCBLOCK class_not

//DOCBLOCK class_mpower

//DOCBLOCK class_power

//DOCBLOCK class_ctranspose

//DOCBLOCK class_transpose

//DOCBLOCK class_colon

//DOCBLOCK class_subsref

//DOCBLOCK class_subsasgn

//@@Signature
//function class ClassFunction
//input varargin
//output classname
//DOCBLOCK class_subsindex
ArrayVector ClassFunction(int nargout, const ArrayVector& arg)
{
  if (arg.size() == 0) {
    throw_Exception("class function requires at least one argument");
  }

  if (arg.size() == 1) {
    return ArrayVector(mxArray(arg[0].className()));
  }

  ArrayVector parents;
  StringVector parentNames;

  for (i = 2; i < arg.size(); i++) {
    if (!arg[i].mxIsUserClass()) {
      throw_Exception("parent objects must be user defined classes");
    }

    parents.push_back(arg[i]);
    parentNames.push_back(arg[i].className());
  }

  return ArrayVector(ClassAux(arg[0], arg[1].asString(), parentNames, parents));
}
#endif

static int MarkUserClasses(int n, mxArray* t, int* set)
{
  int j, k=0;
  for (j = 0; j < n; j++) {
    if (mxIsUserClass(t+j)) {
      set[k++] = j;
    }
  }

  return k;
}

// Ideally, this would be the only place where the class name is mangled.
// However, currently, the same op is repeated in the Interface implementation
// code.
char* ClassMangleName(const char* className, const char* funcName, char* buf, int len)
{
  _snprintf(buf, len, "@%s:%s", className, funcName);
  return buf;
}

void clearUserClasses()
{
  free(classTable->v);
  memset(classTable, 0, sizeof(classTable_t));
}

static fun_t* ClassSearchOverload(interpreter_t* eval, const mxArray* t, int n_userset, const int* userset, const char* name)
{
  int k = 0;
  fun_t* val = NULL;
  char buf[256];

  for (k=0; k < n_userset; ++k) {
    if (val = lookupFunction(eval->context, ClassMangleName(className(t+userset[k]), name, buf, 256))) {
      break;
    }
  }

  return val;
}

int ClassMatrixConstructor(int nrow, int ncol, mxArray* m, interpreter_t* eval, mxArray* out)
{
  int i, np=0;
  // Process the rows...
  // If a row has no user defined classes, then
  // use the standard matrixConstructor
  mxArray* rows = CALLOC(mxArray, nrow);
  int* userset = CALLOC(int, nrow);
  int n_userset=0;

  for (i = 0; i < nrow; i++) {
    mxArray* m1 = m+i*ncol;
    // check this row - make a list of columns that are
    // user classes
    n_userset = MarkUserClasses(ncol, m1, userset);

    if (0==n_userset) {
      HCat(ncol, m1, rows+i);
    }
    else {
      mxArray* p = rows+i;
      fun_t* val = ClassSearchOverload(eval, m1, n_userset, userset, "horzcat");

      if (val==NULL) {
        throw_Exception("no overloaded version of horzcat found");
      }

      // scan through the list of user defined classes - look
      // for one that has "horzcat" overloaded
      updateCode(val, eval);
      np = interpreter_doFunction(eval, val, m1, 1, 1, 0, p);

      if (0==np) {
        interpreter_warningMessage(eval, "%s", "'horzcat' called for user defined class and it returned nothing.  Substituting empty array for result.");
      }
    }
  }
  
  np = 1;
  // Check for a singleton - handle with special case
  if (nrow == 1) {
    mxCopyArray(out, rows);
  } else {
  // At this point we have a vector arrays that have to vertically
  // concatenated.  There may not be any objects in it, so we have
    // to rescan.
    n_userset = MarkUserClasses(nrow, rows, userset);
    
    if (0==n_userset) {
      // OK - we don't have any user-defined classes anymore,
      // so we want to call matrix constructor, which needs
      // an ArrayMatrix instead of an ArrayVector.
      VCat(nrow, rows, out);
    }
    else {
      // We do have a user defined class - look for a vertcat defined
      fun_t* val = ClassSearchOverload(eval, rows, n_userset, userset, "vertcat");
      
      if (NULL==val) {
        throw_Exception("no overloaded version of vertcat found");
      }
      
      updateCode(val, eval);
      np = interpreter_doFunction(eval, val, rows, 1, 1, 0, out);
      
      if (0==np) {
        mxFreeArray(out);
      }
    }
  }
  
  FREE(userset);
  mxFreeArrays(rows, nrow);

  return np;
}

int ClassUnaryOperator(const mxArray* a, const char* funcname, interpreter_t* eval, mxArray* out)
{
  char buf[256] = {0};
  fun_t* val;
  int n;

  if (val = lookupFunction(eval->context, ClassMangleName(className(a), funcname, buf, 256))) {
    updateCode(val, eval);
    n = interpreter_doFunction(eval, val, a, 1, 1, 0, out);
  }
  
  _snprintf(buf, 256, "Unable to find a definition of %s for arguments of class %s", funcname, className(a));
  throw_Exception(buf);
  return n;
}

// ClassResolveHelper
static fun_t* ClassResolveHelper(interpreter_t* eval, const char* className, const char* funcName)
{
  fun_t* val = NULL;
  UserClassMetaInfo* einfo = NULL;
  char buf[256];
  if (val = lookupFunction(eval->context, ClassMangleName(className, funcName, buf, 256))) {
    return val;
  }
#if 0
  einfo = classTable[className];

  // Now check each of the parent classes
  for (i = 0; i < einfo.parentClasses.size(); i++) {
    if (ClassResolveHelper(eval, einfo.parentClasses.at(i), funcName, val)) {
      return TRUE;
    }
  }
    
#endif
  return 0;
}

#if 0
BOOL isParentClass(const char* parent, const char* child)
{
  UserClassMetaInfo einfo = classTable[child];

  for (i = 0; i < einfo.parentClasses.size(); i++)
    if (einfo.parentClasses.at(i) == parent) {
      return TRUE;
    }

  return FALSE;
}

BOOL ClassResolveFunction(interpreter_t* eval, mxArray* args, const char* funcName, fun_t*& val)
{
  context_t* context = eval->context;
  return ClassResolveHelper(eval, args.className(), funcName, val);
}

static mxArray ClassTriOp(const mxArray* a, const mxArray* b, mxArray c, fun_t* val, interpreter_t* eval)
{
  val->updateCode(eval);
  ArrayVector m, n;
  m.push_back(a);
  m.push_back(b);
  m.push_back(c);
  n = eval->doFunction(val, m, 1);

  if (!n.empty()) {
    return n[0];
  }
  else {
    return EmptyConstructor();
  }
}

#endif
int ClassTrinaryOperator(const mxArray* a, const mxArray* b, const mxArray* c, const char* funcname, interpreter_t* eval, mxArray* out)
{
#if 0
  fun_t* val;

  if (a.mxIsUserClass()) {
    if (val = lookupFunction(eval->context, ClassMangleName(className(a), funcname), val)) {
      return ClassTriOp(a, b, c, val, eval);
    }

    throw_Exception(const char*("Unable to find a definition of ") + funcname +
        " for arguments of class " + className(a));
  }
  else if (b.mxIsUserClass()) {
    if (val = lookupFunction(eval->context, ClassMangleName(b.className(), funcname), val)) {
      return ClassTriOp(a, b, c, val, eval);
    }

    throw_Exception(const char*("Unable to find a definition of ") + funcname +
        " for arguments of class " + b.className());
  }
  else if (c.mxIsUserClass()) {
    if (val = lookupFunction(eval->context, ClassMangleName(c.className(), funcname), val)) {
      return ClassTriOp(a, b, c, val, eval);
    }

    throw_Exception(const char*("Unable to find a definition of ") + funcname +
        " for arguments of class " + c.className());
  }

  throw_Exception("unexpected argument types for classtrinaryoperator");
#endif
  return 0;
}


int ClassBinaryOperator(const mxArray* a, const mxArray* b, const char* funcname, interpreter_t* eval, mxArray* out)
{
  fun_t* val = NULL;
  char buf[256];
  const char* szClass = NULL;
  if (mxIsUserClass(a)) {
    szClass = ClassMangleName(className(a), funcname, buf, 256);
  }
  else if (mxIsUserClass(b)) {
    szClass = ClassMangleName(className(b), funcname, buf, 256);
  }

  if (szClass) {
    if (val = lookupFunction(eval->context, szClass)) {
      // TODO - add "inferiorto", etc and class precedence
      mxArray m[2] = {0};
      int n=0;
      updateCode(val, eval);
      m[0] = *a;
      m[1] = *b;
      n = interpreter_doFunction(eval, val, m, 2, 1, 0, out);
      return n;
    }

    _snprintf(buf, 256, "Unable to find a definition of %s for arguments of class %s", funcname, className(a));
    throw_Exception(buf);
  } else {
    throw_Exception("unexpected argument types for classbinaryoperator");
  }
  return 0;
}

#if 0
mxArray IndexExpressionToStruct(interpreter_t* eval, const tree_t* t, mxArray r)
{
  StringVector fNames;
  fNames.push_back("type");
  fNames.push_back("subs");
  ArrayVector type_args;
  ArrayVector subs_args;

  for (index = 1; index < t->m_childrenlen; index++) {
    if (t.child(index).is(TOK_PARENS)) {
      ArrayVector m;
      const tree_t* s(t.child(index));

      for (p = 0; p < s->m_childrenlen; p++) {
        eval->multiexpr(s.child(p), m);
      }

      eval->subsindex(m);
      //        m = eval->varExpressionList(t[index].children(),r);
      //        // Scan through the expressions... adjust for "colon" calls
      //        AdjustColonCalls(m,t[index].children());

      // Removing to fix bug 3354203
      //      if (m.size() == 0)
      //  throw_Exception("Expected indexing expression!");

      // Take the arguments and push them into a cell array...
      ArrayMatrix q;
      q.push_back(m);
      type_args.push_back(mxArray(const char*("()")));
      subs_args.push_back(CellConstructor(q));
    }

    if (t.child(index).is(TOK_BRACES)) {
      ArrayVector m;
      const tree_t* s(t.child(index));

      for (p = 0; p < s->m_childrenlen; p++) {
        eval->multiexpr(s.child(p), m);
      }

      eval->subsindex(m);

      //        m = eval->varExpressionList(t[index].children(),r);
      //        AdjustColonCalls(m,t[index].children());
      if (m.size() == 0) {
        throw_Exception("Expected indexing expression!");
      }

      // Take the arguments and push them into a cell array...
      ArrayMatrix q;
      q.push_back(m);
      type_args.push_back(mxArray(const char*("{}")));
      subs_args.push_back(CellConstructor(q));
    }

    if (t.child(index).is('.')) {
      type_args.push_back(mxArray(const char*(".")));
      subs_args.push_back(mxArray(t.child(index).first().text()));
    }
  }

  mxArray retval(Struct);
  StructArray& rp(retval.structPtr());
  rp["type"] = ArrayVectorToBasicArray(type_args);
  rp["subs"] = ArrayVectorToBasicArray(subs_args);
  rp.updateDims();
  return retval;
}

static ArrayVector ClassSubsrefCall(interpreter_t* eval, const tree_t* t, mxArray r, fun_t* val)
{
  ArrayVector p;
  p.push_back(r);
  p.push_back(IndexExpressionToStruct(eval, t, r));
  val->updateCode(eval);
  ArrayVector n = eval->doFunction(val, p, 1);
  return n;
}

BOOL ClassHandleMethod(mxArray r, const char* funcName, interpreter_t* eval)
{
  fun_t* p;

  if (!ClassResolveFunction(eval, r, funcName, p)) {
    return FALSE;
  }

  p->updateCode(eval);
  ArrayVector q(r);
  eval->doFunction(p, q, 1);
  return TRUE;
}

// What is special here...  Need to be able to do field indexing
//
ArrayVector ClassRHSExpression(mxArray r, const tree_t* t, interpreter_t* eval)
{
  mxArray q;
  mxArray n, p;
  fun_t* val;

  // New handle-type classes support normal C++ syntax for
  // method invokation... so foo.bar(args) is really
  // bar(foo,args).  Lovely.
  if (!eval->inMethodCall(r.className()) &&
      r.constStructPtr().isHandleClass() &&
      t.second().is('.') &&
      (((t->m_childrenlen == 2) ||
          ((t->m_childrenlen == 3) && (t.child(2).is(TOK_PARENS)))))) {
    fun_t* val;

    if (ClassResolveFunction(eval, r, t.second().first().text(), val)) {
      val->updateCode(eval);
      ArrayVector q(r);

      if (t->m_childrenlen == 2) {
        return eval->doFunction(val, q, 1);
      }
      else {
        const tree_t* s(t.child(2));

        for (p = 0; p < s->m_childrenlen; p++) {
          eval->multiexpr(s.child(p), q);
        }

        return eval->doFunction(val, q, 1);
      }
    }
  }

  // Try and look up subsref, _unless_ we are already in a method
  // of this class
  if (!eval->inMethodCall(r.className()))
    if (ClassResolveFunction(eval, r, "subsref", val)) {
      // Overloaded subsref case
      return ClassSubsrefCall(eval, t, r, val);
    }

  ArrayVector rv;

  for (index = 1; index < t->m_childrenlen; index++) {
    if (!rv.empty()) {
      throw_Exception("Cannot reindex an expression that returns multiple values.");
    }

    if (t.child(index).is(TOK_PARENS)) {
      ArrayVector m;
      const tree_t* s(t.child(index));

      for (p = 0; p < s->m_childrenlen; p++) {
        eval->multiexpr(s.child(p), m);
      }

      eval->subsindex(m);

      //     m = eval->varExpressionList(t->child(index).children(),r);
      if (m.size() == 0) {
        throw_Exception("Expected indexing expression!");
      }
      else if (m.size() == 1) {
        q = r.get(m.front());
        r = q;
      }
      else {
        q = r.get(m);
        r = q;
      }
    }

    if (t.child(index).is(TOK_BRACES)) {
      ArrayVector m;
      const tree_t* s(t.child(index));

      for (p = 0; p < s->m_childrenlen; p++) {
        eval->multiexpr(s.child(p), m);
      }

      eval->subsindex(m);

      //     m = eval->varExpressionList(t->child(index).children(),r);
      if (m.size() == 0) {
        throw_Exception("Expected indexing expression!");
      }
      else if (m.size() == 1) {
        rv = ArrayVectorFromCellArray(r.get(m.front()));
      }
      else {
        rv = ArrayVectorFromCellArray(r.get(m));
      }

      if (rv.size() == 1) {
        r = rv[0];
        rv = ArrayVector();
      }
      else if (rv.size() == 0) {
        throw_Exception("Empty expression!");
        r = EmptyConstructor();
      }
    }

    if (t.child(index).is('.')) {
      rv = r.get(t.child(index).first().text());

      if (rv.size() <= 1) {
        r = rv[0];
        rv = ArrayVector();
      }
    }

    if (t.child(index).is(TOK_DYN)) {
      const char* field;

      try {
        mxArray fname(eval->expression(t.child(index).first()));
        field = fname.asString();
      }
      catch (Exception& e) {
        throw_Exception("dynamic field reference to structure requires a string argument");
      }

      rv = r.get(field);

      if (rv.size() <= 1) {
        r = rv[0];
        rv = ArrayVector();
      }
    }
  }

  if (rv.empty()) {
    rv.push_back(r);
  }

  return rv;
}

void ClassAssignExpression(ArrayReference dst, const tree_t* t, const mxArray* value, interpreter_t* eval)
{
  fun_t* val;

  if (!ClassResolveFunction(eval, *dst, "subsasgn", val))
    throw_Exception(const char*("The method 'subsasgn' is not defined for objects of class ") +
        dst->className());

  ArrayVector p;
  p.push_back(*dst);
  p.push_back(IndexExpressionToStruct(eval, t, *dst));
  p.push_back(value);
  val->updateCode(eval);
  BOOL overload(eval->getStopOverload());
  eval->setStopOverload(TRUE);
  ArrayVector n = eval->doFunction(val, p, 1);
  eval->setStopOverload(overload);

  if (!n.empty()) {
    *dst = n[0];
  }
  else
    eval->warningMessage(const char*("'subsasgn' for class ") + dst->className() +
        const char*(" did not return a value... operation has no effect."));
}

void defineHierarchyForClass(const char* classname, StringVector parents)
{
  QMutexLocker lock(&classMutex);
  UserClassMetaInfo info;
  info.parentClasses = parents;
  classTable[classname] = info;
}

BOOL isUserClassDefined(const char* classname)
{
  QMutexLocker lock(&classMutex);
  return (classTable.contains(classname));
}

//@@Signature
//sfunction makehandleclass MakeHandleClassFunction
//input x
//output handle
ArrayVector MakeHandleClassFunction(int nargout, const ArrayVector& arg,
    interpreter_t* eval)
{
  if (arg.size() == 0) {
    return ArrayVector();
  }

  mxArray t = arg[0];
  StructArray& sa = t.structPtr();
  StructArray* mc = new StructArray(t.structPtr());
  StructArray handle(mc, eval);
  return mxArray(handle);
}

#endif
