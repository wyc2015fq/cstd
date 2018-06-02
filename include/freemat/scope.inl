
#ifndef __SCOPE_INL__
#define __SCOPE_INL__

/**
 * A Scope is a combination of a variable hashtable and a function hashtable.
 */

typedef struct var_t {
  char name[16];
  mxArray arr[1];
} var_t;

typedef struct VariableTable {
  var_t* v;
  int n;
} VariableTable;

int VariableTable_free(VariableTable* s)
{
  int i;

  for (i = 0; i < s->n; ++i) {
    mxFreeArray(s->v[i].arr);
  }

  FREE(s->v);
  memset(s, 0, sizeof(VariableTable));
  return 0;
}

int getCompletions(VariableTable* s, const char* prefix, vstr_t* retvec)
{
  int i;

  for (i = 0; i < s->n; ++i) {
    if (STR_isbegof(s->v[i].name, -1, prefix, -1)) {
      strv_push_str(retvec, STR1(s->v[i].name));
    }
  }

  return 0;
}
static int findSymbol(VariableTable* s, const char* varName)
{
  int i;

  for (i = 0; i < s->n; ++i) {
    if (0 == strcmp(s->v[i].name, varName)) {
      return i;
    }
  }

  return 0;
}

static int insertSymbol(VariableTable* s, const char* varName, const mxArray* val)
{
  int n = s->n++;
  s->v = realloc(s->v, s->n * sizeof(var_t));
  ASSERT(strlen(varName) < 16);
  strncpy(s->v[n].name, varName, 16);
  mxCopyArray(s->v[n].arr, val);
  return 0;
}
static int deleteSymbol(VariableTable* s, const char* varName)
{
  int i = findSymbol(s, varName);

  if (i >= 0) {
    mxFreeArray(s->v[i].arr);
    MEMCPY(s->v + i, s->v + i + 1, s->n - i - 1);
    s->n--;
  }

  if (0 == s->n) {
    VariableTable_free(s);
  }

  return 0;
}
/**
 * A Scope is a collection of functions and variables all visible
 * at some point in the execution.  The scope also keeps track of
 * the loop level, and a list of the global and persistent variables
 * relevant to the current scope.
 */
typedef struct scope_t {
  /**
   * This is a mutex to protect the scope when multiple threads
   * have access to the scope.  For all scopes (except the global
   * one, this mutex is unused.
   */
  //QMutex *mutex;
  /**
   * The reference count for this Scope
   */
  int refcount;
  /**
   * This is the hash-table of mxArray pointers that forms the
   * symbol table.  Each variable has a name associated with
   * it that must be unique to the Scope.  The Scope owns the
   * variables in the symbol table, and is responsible for
   * destructing them when destroyed.
   */
  VariableTable symTab[1];

  /**
   * The name of the scope.
   */
  char name[64];
  /**
   * Detailed information that indicates where the scope is from.
   */
  char detail[64];
  /**
   * The context information (currently executed token ID)
   */
  int tokid;
  /**
   * The line number for the current step trap (from dbstep)
   */
  int steptrap;
  /**
   * The current line number from which we are stepping
   */
  int stepcurrentline;
  /**
   * Flag that indicates if this scope is active (if it is actually
   * used to store local variables -- for scripts, the corresponding
   * scope is not active).
   */
  BOOL active;
  /**
   * The loop level.  This is used to track the depth of nested
   * loops.
   */
  int loopLevel;
  /**
   * These are the global variables as defined in the current
   * scope.  Global variables are not stored in this Scope, but
   * are deferred to the top scope in the context_t.
   */
  vstr_t globalVars[1];
  /**
   * Persistent variables are similar to global variables in that
   * they are deferred to the top scope in the context_t.  However,
   * unlike global variables, persistent variables are mangled
   * with the name of the scope before being indexed into the global
   * scope.
   */
  vstr_t persistentVars[1];
  /**
   * This string vector contains the names of variables accessed (potentially)
   * in this scope.
   */
  vstr_t variablesAccessed[1];
  /**
   * This string vector contains the names of variables that must be local to this
   * scope.
   */
  vstr_t localVariables[1];
  /**
   * On every call to modify the scope, we have to check the global/persistent
   * variable table.  This is generally expensive, so we cache information
   * about these tables being empty (the usual case).
   */
  BOOL anyPersistents;
  BOOL anyGlobals;
  BOOL isNested;
  /**
   * The number of input/output arguments - used by M-functions that need to know
   */
  int numargin;
  int numargout;
} scope_t;

int scope_free(scope_t* s)
{
  VariableTable_free(s->symTab);
  //str_free(s->detail);
  strv_free(s->globalVars);
  strv_free(s->persistentVars);
  strv_free(s->variablesAccessed);
  strv_free(s->localVariables);
  memset(s, 0, sizeof(scope_t));
  return 0;
}
/**
 * Construct a scope with the given name.
 */
int scope_set(scope_t* s, const char* scopeName, BOOL nested)
{
  strncpy(s->name, scopeName, 60);
  s->active = (TRUE);
  s->isNested = (nested);
  return 0;
}
static BOOL scope_variableAccessed(scope_t* s, const char* varName)
{
  int k = strv_find(s->variablesAccessed, STR1(varName), 1);
  return k >= 0;
}
static BOOL scope_variableLocal(scope_t* s, const char* varName)
{
  int k = strv_find(s->localVariables, STR1(varName), 1);
  return k >= 0;
}
static BOOL scope_nests(scope_t* s, const char* peerName)
{
  // Nesting requires that our peer have a strictly more
  // qualified (longer) name, and that our name is a prefix
  // of that name.
  return STR_isbegof(peerName, -1, s->name, -1);
}
/**
* Lock the scope's mutex
*/
static void lock()
{
  //if (mutex) mutex->lock();
}
/**
* Unlock the scope's mutex
*/
static void unlock()
{
  //if (mutex) mutex->unlock();
}
/**
* Initialize the scope's mutex - only needed in Global
* scope.
*/
static void mutexSetup()
{
  //if (mutex) delete mutex;
  //mutex = new QMutex(QMutex::Recursive);
}

/**
* Insert a variable with the given name.  If the variable
* already exists in the Scope, then the previous definition
* is replaced with the given one.
*/
static void insertVariable(scope_t* s, const char* varName, const mxArray* val)
{
  insertSymbol(s->symTab, varName, val);
}
/**
* Lookup a variable.  Return a pointer to the variable in the symbol
* table if found and NULL otherwise.  Different than lookupFunction
* because in write-back assignments (e.g., A(:,346) = b) it is critical
* to manage the number of copies.
*/
static mxArray* lookupVariable(scope_t* s, const char* varName)
{
  int k = findSymbol(s->symTab, varName);
  return k < 0 ? NULL : s->symTab->v[k].arr;
}
/**
* Check to see if a variable is globally defined.
*/
static BOOL isVariableGlobal(scope_t* s, const char* varName)
{
  int i = 0;

  if (!s->anyGlobals) {
    return FALSE;
  }

  i = strv_find(s->globalVars, STR1(varName), 1);
  return i >= 0;
}
/**
* Add a variable name to the global variables list.
*/
static void addGlobalVariablePointer(scope_t* s, const char* varName)
{
  if (!isVariableGlobal(s, varName)) {
    strv_push_str(s->globalVars, STR1(varName));
    s->anyGlobals = TRUE;
  }
}
/**
* Delete a variable name from the global variables list.
*/
static void deleteGlobalVariablePointer(scope_t* s, const char* varName)
{
  int i = strv_find(s->globalVars, STR1(varName), 1);

  if (i >= 0) {
    strv_del_str(s->globalVars, i);
  }

  if (s->globalVars->n == 0) {
    s->anyGlobals = FALSE;
  }
}
/**
* Check to see if a variable is defined in the persistent
* list.
*/
static BOOL isVariablePersistent(scope_t* s, const char* varName)
{
  int i;

  if (!s->anyPersistents) {
    return FALSE;
  }

  i = strv_find(s->persistentVars, STR1(varName), 1);
  return i >= 0;
}
/**
* Add a variable name to the persistent variables list.
*/
static void addPersistentVariablePointer(scope_t* s, const char* varName)
{
  if (!isVariablePersistent(s, varName)) {
    strv_push_str(s->persistentVars, STR1(varName));
    s->anyPersistents = TRUE;
  }
}
/**
* Delete a variable name from the persistent variables list.
*/
static void deletePersistentVariablePointer(scope_t* s, const char* varName)
{
  int i = strv_find(s->persistentVars, STR1(varName), 1);

  if (i >= 0) {
    strv_del_str(s->persistentVars, i);
  }

  if (s->persistentVars->n == 0) {
    s->anyPersistents = FALSE;
  }
}
/**
* Mangle the name of a variable by prepending
* a "_scopename_" to the name of the variable.
*/
static int getMangledName(scope_t* s, const char* varName, char* buf, int len)
{
  _snprintf(buf, len, "_%s_%s", s->name, varName);
  return 0;
}
/**
* Increment the loop counter.
*/
static void enterLoop(scope_t* s)
{
  s->loopLevel++;
}
/**
* Decrement the loop counter.
*/
static void exitLoop(scope_t* s)
{
  s->loopLevel--;
}
/**
* Test the loop counter.
*/
static BOOL inLoop(scope_t* s)
{
  return (s->loopLevel > 0);
}
/**
* Returns a list of all currently defined variables
* in the active scope.
*/
static int listAllVariables(scope_t* s, vstr_t* names)
{
  int i;
  getCompletions(s->symTab, "", names);

  for (i = 0; i < s->globalVars->n; i++) {
    strv_push_str(names, s->globalVars->v[i]);
  }

  for (i = 0; i < s->persistentVars->n; i++) {
    strv_push_str(names, s->persistentVars->v[i]);
  }

  return 0;
}
/**
* Clear the list of global variable names
*/
static void clearGlobalVariableList(scope_t* s)
{
  strv_free(s->globalVars);
  s->anyGlobals = FALSE;
}
/**
* Clear the list of persistent variable names
*/
static void clearPersistentVariableList(scope_t* s)
{
  strv_free(s->persistentVars);
  s->anyPersistents = FALSE;
}

#endif
