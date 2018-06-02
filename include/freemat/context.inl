
/**
 * This class represents a reference to an array in a scope
 */
typedef struct ArrayReference {
  mxArray* m_ptr;
  BOOL m_global;
  scope_t* m_scope;
} ArrayReference;

/**
 * A context_t is a stack of scopes with the (peculiar) property that
 * the top and bottom of the stack (global and local scopes respectively)
 * are searched regularly.  The context is responsible for determining
 * if variables and functions exist, and if so, for returning their
 * values and accepting modifications to them.  A context also keeps
 * track of loop depth.  Furthermore, the context (starting with 3.1)
 * now "owns" all of the functions that are defined.  These used to be
 * tracked with the scopes, but that lead to a memory leak of FunctionDef
 * objects.
 */

typedef struct CodeTable {
  fun_t* v;
  int n;
} CodeTable;

typedef struct context_t {
  /**
   * The normal stack of scopes.
   */
  scope_t scopestack[256];
  int scopestacklen;
  /**
   * The stack of scopes that have been "bypassed"
   */
  scope_t* bypassstack[256];
  int bypassstacklen;
  /**
   * The stack of scopes that have been "reserved" -- need it
   * because we need to move scopes out of sequence.
  */
  scope_t* reserveStack[256];
  int reserveStacklen;
  /**
   * The table of functions
   */
  CodeTable codeTab[1];
  /**
   * List of functions that are "temporary" and should be flushed
   */
  vstr_t tempFunctions[1];
  /**
   * The set of builtin functions
   */
  CodeTable builtinTab[1];
  /**
   * The set of captured mfunctions
  */
  CodeTable mFunctions[1];
  /**
   * Mutex to control access to this context class.
   */
  //QMutex mutex;
  /**
   * Pointer to current scope - must be an active scope --
   * inactive scopes are used for debug tracking only
   */
  scope_t* activeScope;
  /**
   * Pointer to the bottom scope
   */
  scope_t* bottomScope;
  /**
   * Pointer to global scope
   */
  scope_t* topScope;
} context_t;

static scope_t* lastActiveScope(context_t* s)
{
  int i;
  for (i = s->scopestacklen - 1; i >= 0; i--) {
    if (s->scopestack[i].active) {
      return s->scopestack+i;
    }
  }
  
  return s->scopestack;
}

/**
* Push the given scope onto the bottom of the scope stack.
*/
static int pushScope(context_t* s, const char* name, const char* detail, BOOL nestflag/* = FALSE*/)
{
  scope_t* t = s->scopestack + s->scopestacklen;
  //QMutexLocker lock(&mutex);
  if (s->scopestacklen > 100) {
    throw_Exception("Allowable stack depth exceeded...");
  }
  
  strcpy(t->name, name);
  t->isNested = nestflag;
  strcpy(t->detail, detail);
  s->bottomScope = t;
  s->activeScope = lastActiveScope(s);
  s->scopestacklen++;
  return 0;
}
#if 0
/**
* Create a context and initialize it with a global scope and a
* base scope.
*/
int context_set(context_t* s, scope_t* global)
{
#if 0
  // mutex(QMutex::Recursive)
  s->scopestack[s->scopestack++] = global;
  pushScope(s, "base", 0, FALSE);
  topScope = scopestack.front();
  bottomScope = scopestack.back();
  bottomScope->setActive(TRUE);
  activeScope = lastActiveScope();
#endif
}
int debugDump(context_t* s)
{
  int i;
  for (i = 0; i < s->scopestacklen; i++) {
    qDebug() << "scope " << i << " " << s->scopestack[i]->getName() << s->scopestack[i]->isActive() << s->scopestack[i]->listAllVariables();
    printf("scope %d %s %d", i, s->scopestack[i]->name, s->scopestack[i]->active, );
  }

  for (i = 0; i < bypassstack.size(); i++) {
    qDebug() << "bypass " << i << " " << bypassstack[i]->getName() << bypassstack[i]->isActive() << bypassstack[i]->listAllVariables();
  }

  for (i = 0; i < reserveStack.size(); i++) {
    qDebug() << "reserve " << i << " " << reserveStack[i]->getName() << reserveStack[i]->isActive() << reserveStack[i]->listAllVariables();
  }
}
/**
* Get the pointer to the mutex that protects this context.
* This mutex only needs to be used when the GUI thread wants
* to access the context (which is "owned" by the interpreter_t
* thread).
*/
static QMutex* getMutex()
{
  return &mutex;
}
static BOOL isScopeActive() const
{
  return bottomScope->isActive();
}
static int setScopeActive(BOOL x)
{
  bottomScope->setActive(x);
  activeScope = lastActiveScope();
}
static int scopeStepTrap() const
{
  return bottomScope->stepTrap();
}
static int setScopeStepTrap(int x)
{
  bottomScope->setStepTrap(x);
}
static int scopeStepCurrentLine() const
{
  return bottomScope->stepCurrentLine();
}
static int setScopeStepCurrentLine(int x)
{
  bottomScope->setStepCurrentLine(x);
}
static int scopeTokenID() const
{
  return bottomScope->tokenID();
}
static int setScopeTokenID(int x)
{
  bottomScope->setTokenID(x);
}
static const char* scopeDetailString()
{
  return bottomScope->detailString();
}
static int setScopeDetailString(const char* x)
{
  bottomScope->setDetailString(x);
}
static int scopeNargin()
{
  return activeScope->nargin();
}
static int setScopeNargin(int x)
{
  activeScope->setNargin(x);
}
static int scopeNargout()
{
  return activeScope->nargout();
}
static int setScopeNargout(int x)
{
  activeScope->setNargout(x);
}
static int updateScopePointers()
{
  QMutexLocker lock(&mutex);

  if (!scopestack.isEmpty()) {
    topScope = scopestack.front();
    bottomScope = scopestack.back();
    activeScope = lastActiveScope();
  }
  else {
    topScope = bottomScope = activeScope = NULL;
  }
}
static int reserveScope()
{
  QMutexLocker lock(&mutex);
  reserveStack.push_back(scopestack.back());
  scopestack.pop_back();
  updateScopePointers();
}
static int unreserveScope()
{
  QMutexLocker lock(&mutex);

  if (reserveStack.size() > 0) {
    scopestack.push_back(reserveStack.back());
    reserveStack.pop_back();
  }

  updateScopePointers();
}
/**
* Bypass the prescribed number of scopes.  These scopes are
* placed on the bypassstack.  This effectively makes a different
* scope active, and then allows us to restore the bypass scopes.
* a count of -1 means all scopes are bypassed (except the base scope)
*/
static int bypassScope(int count)
{
  QMutexLocker lock(&mutex);

  if (count < 0) {
    count = (s->scopestacklen - 2);
  }

  if (count > s->scopestacklen) {
    return;
  }

  for (i = 0; i < count; i++) {
    bypassstack.push_back(scopestack.back());
    scopestack.pop_back();
  }

  updateScopePointers();
}
static int restoreScope(int count)
{
  QMutexLocker lock(&mutex);

  if (count > bypassstack.size()) {
    return;
  }

  for (i = 0; i < count; i++) {
    scopestack.push_back(bypassstack.back());
    bypassstack.pop_back();
  }

  updateScopePointers();
}
/**
* Every call to bypassScope should be matched by a call to
* restoreBypassedScopes, or memory leaks will occur.
*/
static int restoreBypassedScopes()
{
  QMutexLocker lock(&mutex);

  for (i = 0; i < bypassstack.size(); i++) {
    scopestack.push_back(bypassstack[bypassstack.size() - 1 - i]);
  }

  bypassstack.clear();
  updateScopePointers();
}
/**
* Pop the bottom scope off of the scope stack.  The scope is
* deleted.
* Throws an Exception if the global scope is popped.
*/
static int popScope()
{
  QMutexLocker lock(&mutex);

  if (s->scopestacklen == 1) {
    throw_Exception("Attempt to pop global scope off of context stack!");
  }

  scopestack.pop_back();
  delete bottomScope;
  bottomScope = scopestack.back();
  activeScope = lastActiveScope();
}
static int scopeDepth()
{
  return s->scopestacklen;
}
/**
* Insert the given variable into the right scope - the global
* scope if the array is in the global list, and mangled in
* global list if it is persistent.
*/
static int insertVariable(const const char*& varName, const mxArray* var)
{
  scope_t* active;
  const char* mapName;

  if (activeScope->isVariablePersistent(varName)) {
    mapName = activeScope->getMangledName(varName);
    active = topScope;
  }
  else if (activeScope->isVariableGlobal(varName)) {
    mapName = varName;
    active = topScope;
  }
  else {
    activeScope->insertVariable(varName, var);
    return;
  }

  active->insertVariable(mapName, var);
}

/**
* Insert a variable into the local scope - do not check the
* global list.
*/
static int insertVariableLocally(const char* varName, const mxArray* var)
{
  activeScope->insertVariable(varName, var);
}
/**
* Return a pointer to the given variable.  The search
* logic is:
*   - If the variable is persistent in the current scope
*     (at the bottom of the scope stack), mangle its name
*     using the scope, and look for it in the global scope.
*   - If the variable is global in the current scope (at the
*     bottom of the scope stack, look for it in the global
*     scope.
*   - Look for the variable in the local scope.
* If the variable is not found, an empty variable is constructed
* with the given name, and inserted into the scope that was
* searched.  A pointer to this newly created variable is returned.
*/
static ArrayReference lookupVariable(const const char* varName)
{
  scope_t* active;
  const char* mapName;
  BOOL global = FALSE;

  if (activeScope->isVariablePersistent(varName)) {
    mapName = activeScope->getMangledName(varName);
    active = topScope;
  }
  else if (activeScope->isVariableGlobal(varName)) {
    mapName = varName;
    active = topScope;
    global = TRUE;
  }
  else {
    mxArray* dp = activeScope->lookupVariable(varName);

    if (!dp && activeScope->isnested()) {
      // If the variable is not defined in the current scope,
      // loop up through the scope stack, checking for nested
      // scopes that may have the variable defined.
      //
      // FIXME
      //
      int i = s->scopestacklen - 2;

      while ((!dp) && (i >= 0) && s->scopestack[i]->nests(scopestack[i + 1]->getName())) {
        dp = s->scopestack[i]->lookupVariable(varName);

        if (!dp) {
          i--;
        }
      }

      if (dp) {
        return (ArrayReference(dp, FALSE, s->scopestack[i]));
      }
      else {
        return (ArrayReference(dp, FALSE, activeScope));
      }
    }
    else {
      return (ArrayReference(dp, FALSE, activeScope));
    }
  }

  return (ArrayReference(active->lookupVariable(mapName), global, active));
}
#endif
static BOOL variableLocalToCurrentScope(context_t* s, const char* varName)
{
  return scope_variableLocal(s->activeScope, varName);
}
static int setVariablesAccessed(context_t* s, const vstr_t* va)
{
  //setVariablesAccessed(s->activeScope, va);
  strv_copy(s->activeScope->variablesAccessed, va);
}
static int setLocalVariablesList(context_t* s, vstr_t* rv)
{
  //setLocalVariables(s->activeScope, rv);
  strv_copy(s->activeScope->localVariables, rv);
  return 0;
}
static fun_t* findFunc(CodeTable* s, const char* varName)
{
  int i;

  for (i = 0; i < s->n; ++i) {
    if (0 == strcmp(s->v[i].name, varName)) {
      return s->v + i;
    }
  }

  return 0;
}

static fun_t* insertFunc(CodeTable* s, const char* varName)
{
  int n = s->n++;
  s->v = realloc(s->v, s->n * sizeof(var_t));
  ASSERT(strlen(varName) < 16);
  strncpy(s->v[n].name, varName, 16);
  return s->v;
}
/**
* Look for a variable, but only locally.
*/
static mxArray* lookupVariableLocally(context_t* s, const char* varName)
{
  return lookupVariable(s->activeScope, varName);
}
/**
* Insert a function definition into the code table.
*/
static fun_t* insertFunction(context_t* s, const char* varName, BOOL temporary)
{
  fun_t* f = insertFunc(s->codeTab, varName);

  if (temporary) {
    strv_push_str(s->tempFunctions, STR1(varName));
  }

  return f;
}
#if 0
/**
* Remove a function definition from the code table.
*/
static int deleteFunction(const const char*& funcName)
{
  codeTab.deleteSymbol(funcName);
}
/**
* Flush temporary function definitions from the global context
*/
static int flushTemporaryGlobalFunctions()
{
  for (i = 0; i < tempFunctions.size(); i++) {
    deleteFunction(tempFunctions[i]);
  }

  tempFunctions.clear();
}
/**
* Capture a m function
*/
static int captureMFunction(fun_t* ptr)
{
  mFunctions.insert(ptr->name, ptr);
}
/**
* Lookup a captured m function
*/
static fun_t* lookupCapturedMFunction(const const char*& name, const const char*& filename)
{
  QList<fun_t*> matches = mFunctions.values(name);

  for (i = 0; i < matches.size(); i++)
    if (((MFunctionDef*)matches[i])->fileName == filename) {
      return matches[i];
    }

  return fun_t*();
}
/**
* Lookup a builtin function
*/
static fun_t* lookupBuiltinFunction(const const char*& funcName)
{
  fun_t** ret = builtinTab.findSymbol(funcName);

  if (ret) {
    return *ret;
  }
  else {
    throw_Exception("Unable to find builtin function definition " + funcName);
  }
}
/**
* Add a built in function to the global scope with the given name.
*/
static int addFunction(const char* name, BuiltInFuncPtr fptr, BOOL jitsafe, int argc_in, int argc_out, ...)
{
  vstr_t args;
  va_list argp;

  if (argc_in > 0) {
    va_start(argp, argc_out);

    for (i = 0; i < argc_in; i++) {
      const char* t = va_arg(argp, const char*);

      if (!t) {
        dbout << "addFunction for function " <<
            name << " is wrong!\n";
      }

      args.push_back(t);
    }

    if (va_arg(argp, const char*) != NULL) {
      dbout << "addFunction for function " <<
          name << " is wrong!\n";
    }

    va_end(argp);
  }

  BuiltInFunctionDef* f2def;
  f2def = new BuiltInFunctionDef;
  f2def->retCount = argc_out;
  f2def->argCount = argc_in;
  f2def->name = name;
  f2def->fptr = fptr;
  f2def->arguments = args;
  f2def->jitsafe = jitsafe;
  insertFunction(f2def, FALSE);

  if (builtinTab.hasSymbol(name)) {
    dbout << "function " << name << " is already in the builtin table";
  }

  builtinTab.insertSymbol(name, f2def);
}
/**
* Add a special function to the global scope with the given name.
*/
static int addSpecialFunction(const char* name, SpecialFuncPtr fptr, BOOL jitsafe, int argc_in, int argc_out, ...)
{
  vstr_t args;
  va_list argp;

  if (argc_in > 0) {
    va_start(argp, argc_out);

    for (i = 0; i < argc_in; i++) {
      const char* t = va_arg(argp, const char*);

      if (!t) {
        dbout << "addSpecialFunction for function " <<
            (name) << " is wrong!\n";
      }

      args.push_back(t);
    }

    if (va_arg(argp, const char*) != NULL) {
      dbout << "addSpecialFunction for function " <<
          (name) << " is wrong!\n";
    }

    va_end(argp);
  }

  SpecialFunctionDef* f2def;
  f2def = new SpecialFunctionDef;
  f2def->retCount = argc_out;
  f2def->argCount = argc_in;
  f2def->name = name;
  f2def->fptr = fptr;
  f2def->arguments = args;
  f2def->jitsafe = jitsafe;
  insertFunction(f2def, FALSE);

  if (builtinTab.hasSymbol(name)) {
    dbout << "function " << name << " is already in the builtin table";
  }

  builtinTab.insertSymbol(name, f2def);
}
/**
* Add a built in function to the global scope with the given name
* and tag it as a graphics function
*/
static int addGfxFunction(const char* name, BuiltInFuncPtr fptr, BOOL jitsafe, int argc_in, int argc_out, ...)
{
  vstr_t args;
  va_list argp;

  if (argc_in > 0) {
    va_start(argp, argc_out);

    for (i = 0; i < argc_in; i++) {
      const char* t = va_arg(argp, const char*);

      if (!t) {
        dbout << "addGfxFunction for function " <<
            (name) << " is wrong!\n";
      }

      args.push_back(t);
    }

    if (va_arg(argp, const char*) != NULL) {
      dbout << "addGfxFunction for function " <<
          (name) << " is wrong!\n";
    }

    va_end(argp);
  }

  BuiltInFunctionDef* f2def;
  f2def = new BuiltInFunctionDef;
  f2def->retCount = argc_out;
  f2def->argCount = argc_in;
  f2def->name = name;
  f2def->fptr = fptr;
  f2def->arguments = args;
  f2def->graphicsFunction = TRUE;
  f2def->jitsafe = jitsafe;
  insertFunction(f2def, FALSE);

  if (builtinTab.hasSymbol(name)) {
    dbout << "function " << name << " is already in the builtin table";
  }

  builtinTab.insertSymbol(name, f2def);
}
/**
* Add a special function to the global scope with the given name, and
* tag it as a graphics function
*/
static int addGfxSpecialFunction(const char* name, SpecialFuncPtr fptr, BOOL jitsafe, int argc_in, int argc_out, ...)
{
  vstr_t args;
  va_list argp;

  if (argc_in > 0) {
    va_start(argp, argc_out);

    for (i = 0; i < argc_in; i++) {
      const char* t = va_arg(argp, const char*);

      if (!t) {
        dbout << "addGfxSpecialFunction for function " <<
            (name) << " is wrong!\n";
      }

      args.push_back(t);
    }

    if (va_arg(argp, const char*) != NULL) {
      dbout << "addGfxSpecialFunction for function " <<
          (name) << " is wrong!\n";
    }

    va_end(argp);
  }

  SpecialFunctionDef* f2def;
  f2def = new SpecialFunctionDef;
  f2def->retCount = argc_out;
  f2def->argCount = argc_in;
  f2def->name = name;
  f2def->fptr = fptr;
  f2def->arguments = args;
  f2def->graphicsFunction = TRUE;
  f2def->jitsafe = jitsafe;
  insertFunction(f2def, FALSE);

  if (builtinTab.hasSymbol(name)) {
    dbout << "function " << name << " is already in the builtin table";
  }

  builtinTab.insertSymbol(name, f2def);
}

static vstr_t listAllFunctions()
{
  return codeTab.getCompletions("");
}

static vstr_t listGlobalVariables()
{
  return topScope->listAllVariables();
}

static vstr_t listAllVariables()
{
  return activeScope->listAllVariables();
}

static int clearGlobalVariableList()
{
  topScope->clearGlobalVariableList();
  activeScope->clearGlobalVariableList();
}

static int clearPersistentVariableList()
{
  topScope->clearPersistentVariableList();
  activeScope->clearPersistentVariableList();
}

static vstr_t getCompletions(const const char*& prefix)
{
  vstr_t local_completions = activeScope->getCompletions(prefix);
  vstr_t global_completions = topScope->getCompletions(prefix);
  vstr_t code_completions = codeTab.getCompletions(prefix);
  vstr_t completions(local_completions);

  for (i = 0; i < global_completions.size(); i++) {
    completions.push_back(global_completions[i]);
  }

  for (i = 0; i < code_completions.size(); i++) {
    completions.push_back(code_completions[i]);
  }

  return completions;
}

#endif
static fun_t* lookupFunction(context_t* s, const char* funcName)
{
  fun_t* ret = findFunc(s->codeTab, funcName);
  return ret;
}

#if 0

/**
* Add a persistent variable to the local stack.  This involves
* two steps:
*   - the name of the variable is added to the persistent variable list
*     in the current scope.
*   - the global scope is checked for the mangled name of the
*     persistent variable.  If the variable does not exist in the
*     global scope, then an empty variable is inserted.
*/
static int addPersistentVariable(const char* var)
{
  // Delete local variables with this name
  activeScope->deleteVariable(var);
  // Delete global variables with this name
  //  topScope->deleteVariable(var);
  activeScope->addPersistentVariablePointer(var);
}
/**
* Add a variable name into the global variable list of the current
* scope.  If the variable does not exist in the global scope, an
* empty variable is added.
*/
static int addGlobalVariable(const char* var)
{
  // Delete local variables with this name
  activeScope->deleteVariable(var);
  // Delete global persistent variables with this name
  topScope->deleteVariable(activeScope->getMangledName(var));
  // Add a point in the local scope to the global variable
  activeScope->addGlobalVariablePointer(var);
}
static int deleteGlobalVariable(const char* var)
{
  topScope->deleteVariable(var);
}
/**
* Delete a variable if its defined.  Handles global and persistent
* variables also.
*/
static int deleteVariable(const char* var)
{
  if (isVariableGlobal(var)) {
    topScope->deleteVariable(var);
    activeScope->deleteGlobalVariablePointer(var);
    return;
  }

  if (isVariablePersistent(var)) {
    topScope->deleteVariable(activeScope->getMangledName(var));
    activeScope->deletePersistentVariablePointer(var);
    return;
  }

  activeScope->deleteVariable(var);
}
#endif
/**
* Increment the loop depth counter in the local scope.
*/
static int context_enterLoop(context_t* s)
{
  enterLoop(s->activeScope);
}
/**
* Decrement the loop depth counter in the local scope.
*/
static int context_exitLoop(context_t* s)
{
  exitLoop(s->activeScope);
}
static BOOL isCurrentScopeNested(context_t* s)
{
  return s->bottomScope->isNested;
}
static int setScopeNesting(context_t* s, BOOL x)
{
  s->bottomScope->isNested = x;
}
static const char* activeScopeName(context_t* s)
{
  return s->activeScope->name;
}
static const char* activeScopeDetailString(context_t* s)
{
  return s->activeScope->detail;
}
static const char* scopeName(context_t* s)
{
  return s->bottomScope->name;
}
static BOOL currentScopeNests(context_t* s, const char* name)
{
  return scope_nests(s->activeScope, name);
}
static BOOL currentScopeVariableAccessed(context_t* s, const char* name)
{
  return scope_variableAccessed(s->activeScope, name);
}
#if 0
static int deactivateCurrentScope(context_t* s)
{
  s->bottomScope->active = FALSE;
  updateScopePointers();
}
/**
* Returns TRUE if the current (local) scope indicates a
* non-zero loop depth.
*/
static BOOL inLoop()
{
  return activeScope->inLoop();
}
/**
* Returns TRUE if the given variable is global.
*/
static BOOL isVariableGlobal(const const char*& varName)
{
  return activeScope->isVariableGlobal(varName);
}
/**
* Returns TRUE if the given variable is persistent
*/
static BOOL isVariablePersistent(const const char*& varName)
{
  return activeScope->isVariablePersistent(varName);
}

/**
* Returns function and line number
*/
static const char* sampleIP(unsigned& ctxt)
{
  QMutexLocker lock(&mutex);
  ctxt = this->scopeTokenID() & 0x0000FFFF;
  return this->scopeName();
}


// Use this class to get access to a parent scope.  It automatically
// bypasses the current scope, and restores on destruction
class ParentScopeLocker
{
  context_t* contxt;
public:
  ParentScopeLocker(context_t* m) {
    contxt = m;
    contxt->bypassScope(1);
  }
  ~ParentScopeLocker() {
    contxt->restoreScope(1);
  }
};

class PopContext
{
  context_t* m_context;
  int m_popSpec;
  int m_scopeDepth;
public:
  PopContext(context_t* context, int popSpec) {
    m_context = context;
    m_popSpec = popSpec;
    m_scopeDepth = context->scopeDepth();

    if (popSpec == 0) {
      return;
    }

    if (popSpec == -1)
      while (context->activeScopeName() != "base") {
        context->bypassScope(1);
      }
    else {
      context->bypassScope(popSpec);
    }
  }
  ~PopContext() {
    while (m_context->scopeDepth() < m_scopeDepth) {
      m_context->restoreScope(1);
    }

    while (m_context->scopeDepth() > m_scopeDepth) {
      m_context->popScope();
    }
  }
};

#endif
