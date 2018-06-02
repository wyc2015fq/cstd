
#ifdef _WIN32
#define PATHSEP ";"
#else
#define PATHSEP ":"
#endif

#define EXPORT

const int max_line_count = 1000000;

/**
 * The database of compiled code segments
 */
//QMap<int,JITInfo> s->m_codesegments;

int ClearJITCache()
{
  //s->m_codesegments.clear();
}


/**
 * The file system watcher -- watches for changes to the file system
 * Only one interpreter thread should use s watcher at a time.
 */
//QFileSystemWatcher s->m_watch;


#define SaveEndInfo  \
  ArrayReference oldEndRef = endRef; \
  int oldEndCount = endCount; \
  int oldEndTotal = endTotal;

#define RestoreEndInfo \
  endRef = oldEndRef; \
  endCount = oldEndCount; \
  endTotal = oldEndTotal;  \
   
char* homePath()
{
  return "";
}
int TildeExpand(const char* path, char* buf, int len)
{
  if ((strlen(path) > 0) && (path[0] == '~')) {
    _snprintf(buf, len, "%s/%s", homePath(), path + 1);
  }
  else {
    strncpy(buf, path, len);
  }

  return 0;
}

int interpreter_setupWatcher(interpreter_t* s)
{
  if (!s->m_liveUpdateFlag) {
    return 0;
  }

  //  vstr_t pathLists(s->m_watch.directories());
  //  if (!pathLists.isEmpty())
  //    s->m_watch.removePaths(pathLists);
  //   if (!s->m_userPath.isEmpty()) {
  //     for (i=0;i<s->m_userPath.size();i++)
  //       if (DirExists(s->m_userPath[i]))
  //  s->m_watch.addPath(s->m_userPath[i]);
  //   }
  //   if (!s->m_basePath.isEmpty()) {
  //     for (i=0;i<s->m_basePath.size();i++)
  //       if (DirExists(s->m_basePath[i]))
  //  s->m_watch.addPath(s->m_basePath[i]);
  //   }
  //   s->m_watch.addPath(QDir::currentPath());
  return 0;
}

int interpreter_updateFileTool(interpreter_t* s)
{
  // Build the info to send to the file tool
#if 0
  QDir dir(QDir::currentPath());
  dir.setFilter(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot);
  QFileInfoList list(dir.entryInfoList());
  QList<QVariant> files;
  QList<QVariant> entry;
  entry << QVariant(("dir"));
  entry << QVariant(" .. (Parent Folder)");
  entry << QVariant();
  entry << QVariant();
  entry << QVariant("Folder");
  files << QVariant(entry);

  for (i = 0; i < ((int)list.size()); i++) {
    QList<QVariant> entry;
    QFileInfo fileInfo(list.at(i));

    if (fileInfo.isDir()) {
      entry << QVariant(("dir"));
    }
    else {
      entry << QVariant(("file"));
    }

    entry << QVariant(fileInfo.fileName());
    entry << QVariant(fileInfo.size());
    entry << QVariant(fileInfo.lastModified());

    if (fileInfo.isDir()) {
      entry << QVariant(("Folder"));
    }
    else if (fileInfo.suffix().isEmpty()) {
      entry << QVariant("File");
    }
    else {
      entry << QVariant((fileInfo.suffix() + " File"));
    }

    files << QVariant(entry);
  }

  emit updateDirView(QVariant(files));
#endif
  return 0;
}

int interpreter_setPath(interpreter_t* s, const char* path)
{
  strv_split_str(s->m_userPath, STR1(path), PATHSEP, " ", 0);

  interpreter_setupWatcher(s);
  //  rescanPath();
  interpreter_updateFileTool(s);
  return 0;
}

int interpreter_getTotalPath(interpreter_t* s, str_t* retpath)
{
  strv_merge(s->m_basePath, retpath, PATHSEP);
  strv_merge(s->m_userPath, retpath, PATHSEP);
  return 0;
}

int interpreter_getPath(interpreter_t* s, str_t* retpath)
{
  strv_merge(s->m_userPath, retpath, PATHSEP);
  return 0;
}

int interpreter_setLiveUpdateFlag(interpreter_t* s, BOOL t)
{
  s->m_liveUpdateFlag = t;

  if (t) {
    //    connect(&s->m_watch,SIGNAL(directoryChanged(const const char* &)),
    //      s,SLOT(updateFileTool(const const char* &)));
  }

  return 0;
}

int interpreter_changeDir(interpreter_t* s, const char* path)
{
  if ((_chdir(path)) != 0) {
    throw_Exception("Unable to change to specified directory: " + path);
  }

  //  interpreter_updateFileTool(s);
  return 0;
}

int interpreter_updateVariablesTool(interpreter_t* s)
{
  vstr_t varList[1] = {0};

  if (!s->m_liveUpdateFlag) {
    return 0;
  }

#if 0
  listAllVariables(s->context, varList);
  QList<QVariant> vars;

  for (i = 0; i < varList.size(); i++) {
    QList<QVariant> entry;
    // Icon
    entry << QVariant();
    entry << QVariant(varList[i]);
    mxArray* dp = context->lookupVariableLocally(varList[i]);

    if (dp) {
      // class
      if (dp->allReal()) {
        entry << QVariant(dp->className());
      }
      else {
        entry << QVariant(dp->className() + " (complex)");
      }

      // value
      entry << QVariant(SummarizeArrayCellEntry(*dp));
      // size
      entry << QVariant(dp->dimensions().toString());
      // bytes min, max, range, mean, var, std
      entry << QVariant(double(dp->bytes()));
      entry += ComputeVariableStats(dp);
    }

    for (i = entry.size(); i < 10; i++) {
      entry << QVariant();
    }

    vars << QVariant(entry);
  }

  emit updateVarView(QVariant(vars));
  strv_free(varList);
#endif
  return 0;
}

static BOOL InSpecificScope(context_t* context, const char* name, const char* detail)
{
  return (0 == strcmp(context->bottomScope->name, name) && 0 == strcmp(context->bottomScope->detail, detail));
}

static BOOL InKeyboardScope(context_t* context)
{
  return InSpecificScope(context, "keyboard", "keyboard");
}

static int GetStackToolDescription(context_t* context, char* buf, int len)
{
  int line = (int)(LineNumber(context->bottomScope->tokid));

  if (line > 0) {
    return _snprintf(buf, len, "%s %d", context->bottomScope->detail, line);
  }
  else {
    return _snprintf(buf, len, "%s", context->bottomScope->detail);
  }
}

int interpreter_updateStackTool(interpreter_t* s)
{
  vstr_t stackInfo[1] = {0};
#if 0
  // Do a complete dump...
  // Suppose we start with
  int f_depth = context->scopestacklen;
  context->restoreBypassedScopes();
  int t_depth = context->scopeDepth();

  for (i = f_depth; i < t_depth; i++) {
    if (!InKeyboardScope(context) && !context->scopeDetailString().isEmpty()) {
      stackInfo << GetStackToolDescription(context);
    }

    context->bypassScope(1);
  }

  BOOL firstline = TRUE;

  for (i = 0; i < f_depth; i++) {
    if (!InKeyboardScope(context) &&
        !InSpecificScope(context, "docli", "builtin") &&
        !context->scopeDetailString().isEmpty()) {
      if (firstline) {
        stackInfo << const char*("*") + GetStackToolDescription(context);
        firstline = FALSE;
      }
      else {
        stackInfo << GetStackToolDescription(context);
      }
    }

    context->bypassScope(1);
  }

  context->restoreBypassedScopes();

  while (context->scopeDepth() > f_depth) {
    context->bypassScope(1);
  }

  emit updateStackView(stackInfo);
#endif
}

int interpreter_rescanPath(interpreter_t* s)
{
  if (s->m_disablerescan) {
    return 0;
  }

#if 0

  if (!context) {
    return 0;
  }

  context->flushTemporaryGlobalFunctions();

  for (i = 0; i < s->m_basePath.size(); i++) {
    scanDirectory(s->m_basePath[i], FALSE, "");
  }

  for (i = 0; i < s->m_userPath.size(); i++) {
    scanDirectory(s->m_userPath[i], FALSE, "");
  }

  // Scan the current working directory.
  scanDirectory(QDir::currentPath(), TRUE, "");
  interpreter_updateFileTool(s);
#endif
}


int interpreter_setBasePath(vstr_t pth)
{
  //s->m_basePath = pth;
}

int interpreter_setUserPath(vstr_t pth)
{
  //s->m_userPath = pth;
}

static const char* mexExtension()
{
#ifdef Q_OS_LINUX
  return "fmxglx";
#endif
#ifdef Q_OS_MACX
  return "fmxmac";
#endif
#ifdef Q_OS_WIN32
  return "fmxw32";
#endif
  return "fmx";
}

int interpreter_procFileM(interpreter_t* s, const char* fname, const char* fullname, BOOL tempfunc)
{
  fun_t* adef = findFunc(s->context->codeTab, fname);

  if (adef) {
    if (adef->type == FM_BUILT_IN_FUNCTION) {
      interpreter_warningMessage(s, "built in function %s will be shadowed by the script %s", fname, fullname);
    }
  }

  adef = insertFunction(s->context, fname, tempfunc);
  //adef->name = fname;
  strcpy(adef->fileName, fullname);
  adef->temporaryFlag = tempfunc;
}

int interpreter_procFileP(interpreter_t* s, const char* fname, const char* fullname, BOOL tempfunc)
{
  throw_Exception("P-files are not supported in s version of FreeMat");
}

int interpreter_procFileMex(interpreter_t* s, const char* fname, const char* fullname, BOOL tempfunc)
{
#if 0
  fun_t* adef = findFunc(s->context->codeTab, fname);
  adef->name = fname;

  if (adef->LoadSuccessful()) {
    context->insertFunction((MFunctionDef*)adef, tempfunc);
  }
  else {
    delete adef;
  }

#endif
  throw_Exception("MEX-files are not supported in s version of FreeMat");
}

#include "FunctionDef.inl"

int interpreter_scanDirectory(interpreter_t* s, const char* scdir, BOOL tempfunc, const char* prefix)
{
  int i;
  dir_t list[1] = {0};
  char buf[256];
  dir_run(list, scdir, 0, "|.m|.p|.mex|");

  for (i = 0; i < list->n; i++) {
    fileinfo_t* fileInfo = list->v + i;
    const char* fileSuffix = strrchr(fileInfo->name, '.');
    const char* fileBaseName = fileInfo->name;
    const char* fileAbsoluteFilePath = fileInfo->fullpath;

    if (prefix) {
      _snprintf(buf, 256, "%s:%s", prefix, fileBaseName);
      fileBaseName = buf;
    }

    if (0 == stricmp(fileSuffix, ".m")) {
      interpreter_procFileM(s, fileBaseName, fileAbsoluteFilePath, tempfunc);
    }
    else if (0 == stricmp(fileSuffix, ".p")) {
      interpreter_procFileP(s, fileBaseName, fileAbsoluteFilePath, tempfunc);
    }
    else {
      interpreter_procFileMex(s, fileBaseName, fileAbsoluteFilePath, tempfunc);
    }
  }

  dir_free(list);
  return 0;
}

int interpreter_setTerminalWidth(interpreter_t* s, int ncols)
{
  //mutex.lock();
  s->m_ncols = ncols;
  //mutex.unlock();
}

int interpreter_getTerminalWidth(interpreter_t* s)
{
  return s->m_ncols;
}

static BOOL isMFile(const char* arg)
{
  // Not completely right...
  int arg_size = strlen(arg);
  return (((arg[arg_size - 1] == 'm') ||
      (arg[arg_size - 1] == 'p')) &&
      (arg[arg_size - 2] == '.'));
}

char* TrimFilename(const char* arg)
{
  return GetFileName(arg);
}

const char* TrimExtension(const char* arg)
{
  return GetFileExt(arg);
}

static const char* PrivateMangleName(const char* currentFunctionPath, const char* fname)
{
#if 0
  if (currentFunctionPath.isEmpty()) {
    return "";
  }

  // First look to see if we are already a private function
  const char* separator("/");
  int ndx1 = currentFunctionPath.lastIndexOf(separator + "private" + separator);

  if (ndx1 >= 0) {
    // The current function is already in a private directory
    // In that case, try to find a private function in the same directory
    currentFunctionPath.remove(ndx1 + 1, currentFunctionPath.size());
    return currentFunctionPath + "private:" + fname;
  }

  int ndx;
  ndx = currentFunctionPath.lastIndexOf(separator);

  if (ndx >= 0) {
    currentFunctionPath.remove(ndx + 1, currentFunctionPath.size());
  }

  return currentFunctionPath + "private:" + fname;
#endif
  return 0;
}

static const char* LocalMangleName(const char* currentFunctionPath, const char* fname)
{
#if 0
  int ndx = currentFunctionPath.lastIndexOf("/");

  if (ndx >= 0) {
    currentFunctionPath.remove(ndx, currentFunctionPath.size());
  }

  const char* tmp = currentFunctionPath + "/" + fname;
  return currentFunctionPath + "/" + fname;
#endif
  return 0;
}

static int NestedMangleName(char* buf, int len, const char* cfunc, const char* fname)
{
  _snprintf(buf, len, "%s/%s", cfunc, fname);
  return 0;
}

#define FREEMAT_VERSION "1.0"

const char* interpreter_getVersionString()
{
  return "FreeMat v" FREEMAT_VERSION;
}

const char* geterrstr(int errcode) {
  switch(errcode) {
#define ERRCODEDEF(a, b) case a: return b;
#include "errcodedef.txt"
#undef ERRCODEDEF
  default:
    return "thread crashed!! - you have encountered a bug in FreeMat - please file bug report describing what happened";
    break;
  }
  return 0;
}

#define TRR_CALL(fun)   if (s->m_threadErrorState = fun) {return s->m_threadErrorState; }

// Run the thread function
int interpreter_run(interpreter_t* s)
{
  if (s->m_threadFunc) {
    ArrayVector_setsize(s->m_threadFuncRets, s->m_threadNargout);
    TRR_CALL(interpreter_doFunction(s, s->m_threadFunc, s->m_threadFuncArgs->v, s->m_threadFuncArgs->n, s->m_threadNargout, 0, s->m_threadFuncRets->v));
  }
  return 0;
}

int interpreter_doCLI(interpreter_t* s)
{
#if 0
  //  rescanPath();
  emit CWDChanged(QDir::currentPath());
  interpreter_updateFileTool(s);

  if (!s->m_skipflag) {
    sendGreeting();
  }

  try {
    while (1) {
      int scope_stackdepth = context->scopeDepth();

      try {
        evalCLI();
      }
      catch (InterpreterRetallException) {
      }
      catch (InterpreterReturnException& e) {
      }

      while (context->scopeDepth() > scope_stackdepth) {
        context->popScope();
      }
    }
  }
  catch (InterpreterQuitException& e) {
    emit QuitSignal();
  }
  catch (std::exception& e) {
    qDebug() << "crash: " << e.what();
    emit CrashedSignal();
  }
  catch (...) {
    emit CrashedSignal();
  }
#endif
  return 0;
}

int interpreter_sendGreeting(interpreter_t* s)
{
  interpreter_outputMessage(s, " %s\n", interpreter_getVersionString());
  interpreter_outputMessage(s, " Copyright (c) 2002-2008 by Samit Basu\n");
  interpreter_outputMessage(s, " Licensed under the GNU Public License (GPL)\n");
  interpreter_outputMessage(s, " Type <help license> to find out more\n");
  interpreter_outputMessage(s, "      <helpwin> for online help\n");
  interpreter_outputMessage(s, "      <pathtool> to set or change your path\n");
  interpreter_outputMessage(s, " Use <dbauto on/off> to control stop-on-error behavior\n");
  interpreter_outputMessage(s, " Use ctrl-b to stop execution of a function/script\n");
  interpreter_outputMessage(s, " JIT is enabled by default - use jitcontrol to change it\n");
  interpreter_outputMessage(s, " Use <rootpath gui> to set/change where the FreeMat toolbox is installed\n");
  interpreter_outputMessage(s, "");
  return 0;
}

BOOL interpreter_inMFile(const interpreter_t* s)
{
  return (isMFile(activeScopeName(s->context)));
}

int interpreter_debugDump(interpreter_t* s)
{
#if 0
  int depth = context->scopeDepth();
  qDebug() << "******************************\n";

  for (i = 0; i < depth; i++) {
    if (context->isScopeActive()) {
      qDebug() << "In " << context->scopeName() << " (" << context->scopeDetailString() << ")*";
    }
    else {
      qDebug() << "In " << context->scopeName() << " (" << context->scopeDetailString() << ")";
    }

    context->bypassScope(1);
  }

  context->restoreScope(depth);
#endif
}

//DOCBLOCK debug_dbup
int interpreter_dbup(interpreter_t* s)
{
#if 0
  // The stack should look like --
  // base, foo, keyboard, dbup
  // so to do a dbup, we have to save the top two of the
  // stack, move foo to the backup stack, and then restore
  //
  // Consider the following.  The stack looks like s:
  // main: base foo1 keyboard foo2 keyboard dbup
  // bypass: <empty>
  //
  // Now we do a dbup
  // main: base foo1 keyboard keyboard dbup
  // bypass foo2
  //
  // Suppose we
  // We need the "keyboard" states on the stack because they
  // capture the context updates for the command line routines.
  //
  if (InSpecificScope(context, "docli", "builtin")) {
    return 0;
  }

  context->reserveScope();

  while (InKeyboardScope(context)) {
    context->bypassScope(1);
  }

  if (!InSpecificScope(context, "base", "base") &&
      !InSpecificScope(context, "docli", "builtin")) {
    // Bypass a single non-keyboard context
    context->bypassScope(1);
  }

  while (InKeyboardScope(context)) {
    context->bypassScope(1);
  }

  context->unreserveScope();
#endif
}

//DOCBLOCK debug_dbdown
int interpreter_dbdown(interpreter_t* s)
{
#if 0
  // Save the one for the "dbdown" command
  // Save the one for the "keyboard" command that we are currently in
  context->reserveScope();
  // Restore until we get a non-"keyboard" scope
  context->restoreScope(1);

  while (InKeyboardScope(context)) {
    context->restoreScope(1);
  }

  context->unreserveScope();
  dbdown_executed = TRUE;
#endif
}

const char* interpreter_getLocalMangledName(interpreter_t* s, const char* fname)
{
  const char* ret = NULL;
#if 0

  if (isMFile(context->activeScopeName())) {
    ret = LocalMangleName(context->activeScopeDetailString(), fname);
  }
  else {
    ret = fname;
  }
  
#endif
  return ret;
}

const char* interpreter_getPrivateMangledName(interpreter_t* s, const char* fname)
{
  const char* ret = NULL;
  
#if 0
  if (isMFile(context->scopeName())) {
    ret = PrivateMangleName(context->scopeName(), fname);
  }
  else {
    ret = QDir::currentPath() +
        const char * (QDir::separator()) +
        const char * ("private:" + fname);
  }
#endif
  return ret;
}

const char* interpreter_getMFileName(interpreter_t* s)
{
#if 0
  if (isMFile(context->scopeName())) {
    return TrimFilename(TrimExtension(context->scopeName()));
  }
  
#endif
  // TESTME
  //   for (i=cstack.size()-1;i>=0;i--)
  //     if (isMFile(cstack[i].cname))
  //       return TrimFilename(TrimExtension(cstack[i].cname));
  return ("");
}

// called by editor
const char* interpreter_getInstructionPointerFileName(interpreter_t* s)
{
#if 0
  if (!InCLI) {
    return ("");
  }

  ParentScopeLocker lock(context);
  const char* filename(context->scopeName());

  if (isMFile(filename)) {
    return filename;
  }
  
#endif
  return ("");
}

int interpreter_expression(interpreter_t* s, const tree_t* t, mxArray* out);

int interpreter_DoBinaryOperator(interpreter_t* s, const tree_t* t, BinaryFunc fnc, const char* funcname, mxArray* out)
{
  mxArray a[2] = {0};
  mxArray* b = a + 1;
  interpreter_expression(s, t->m_children, a);
  interpreter_expression(s, t->m_children+1, b);

  if (!(mxIsUserClass(a) || mxIsUserClass(b))) {
    fnc(a, b, out);
  } else {
    ClassBinaryOperator(a, b, funcname, s, out);
  }
  mxFreeArrays(a, 2);
  return 0;
}

int interpreter_DoUnaryOperator(interpreter_t* s, const tree_t* t, UnaryFunc fnc, const char* funcname, mxArray* out)
{
  mxArray a[1] = {0};
  interpreter_expression(s, t->m_children, a);

  if (!mxIsUserClass(a)) {
    fnc(a, out);
  } else {
    ClassUnaryOperator(a, funcname, s, out);
  }
  mxFreeArray(a);
  return 0;
}

int interpreter_setPrintLimit(interpreter_t* s, int lim)
{
  s->printLimit = lim;
}

int interpreter_getPrintLimit(interpreter_t* s)
{
  return (s->printLimit);
}

int interpreter_multiexpr(interpreter_t* s, const tree_t* t, mxArray* q, index_t lhsCount, BOOL output_optional);

//DOCBLOCK variables_matrix
//Works
int interpreter_matrixDefinition(interpreter_t* s, const tree_t* t, mxArray* out)
{
  int i, j, nrow=0, ncol=0, ret=0;
  mxArray* m = NULL;
  // Check if any of the elements are user defined classes
  BOOL anyuser = FALSE;

  if (t->m_childrenlen == 0) {
    mxFreeArray(out);
    return 0;
  }

  for (i = 0; i < t->m_childrenlen; i++) {
    const tree_t* s = t->m_children+i;
    ncol = MAX(ncol, s->m_childrenlen); 
  }

  nrow = t->m_childrenlen;
  m = CALLOC(mxArray, ncol*nrow);
  
  for (i = 0; i < t->m_childrenlen; i++) {
    const tree_t* treerow = t->m_children+i;
    mxArray* n = m + ncol*i;
    for (j = 0; j < treerow->m_childrenlen; j++) {
      interpreter_multiexpr(s, treerow->m_children+j, n + j, 1, FALSE);
    }
  }

  for (i = 0; i < nrow && !anyuser; i++) {
    mxArray* n = m + ncol*i;
    for (j = 0; j < ncol && !anyuser; j++) {
      if (mxIsUserClass(n + j)) {
        anyuser = TRUE;
      }
    }
  }

  if (!anyuser) {
    ret=MatrixConstructor(nrow, ncol, m, out);
  }
  else {
    ret=ClassMatrixConstructor(nrow, ncol, m, s, out);
  }
  mxFreeArrays(m, ncol*nrow);
  return ret;
}

//DOCBLOCK variables_cell
//Works
int interpreter_cellDefinition(interpreter_t* s, const tree_t* t, mxArray* out)
{
  int i, j, nrow=0, ncol=0, ret=0;
  mxArray* m = NULL;

  if (t->m_childrenlen == 0) {
    mxSetCellArray(out, 0, 0);
    return 0;
  }
  for (i = 0; i < t->m_childrenlen; i++) {
    const tree_t* s = t->m_children+i;
    ncol = MAX(ncol, s->m_childrenlen); 
  }
  
  nrow = t->m_childrenlen;
  m = CALLOC(mxArray, ncol*nrow);

  for (i = 0; i < t->m_childrenlen; i++) {
    const tree_t* treerow = t->m_children+i;
    mxArray* n = m + ncol*i;

    for (j = 0; j < treerow->m_childrenlen; j++) {
      interpreter_multiexpr(s, treerow->m_children+j, n + j, 1, FALSE);
    }
  }

  CellConstructor(nrow, ncol, m, out);
  mxFreeArrays(m, ncol*nrow);
  return 0;
}

int interpreter_ShortCutOr(interpreter_t* s, const tree_t* t, mxArray* out)
{
  mxArray a[1] = {0};
  interpreter_expression(s, t->m_children, a);

  if (!mxIsScalar(a)) {
    interpreter_DoBinaryOperator(s, t, mxOr, "or", out);
  }
  else {
    // A is a scalar - is it TRUE?
    if ((logical)mxGetScalar(a)) {
      toClass(out, a, Bool, 0);
    }
    else {
      interpreter_DoBinaryOperator(s, t, mxOr, "or", out);
    }
  }

  return 0;
}

int interpreter_ShortCutAnd(interpreter_t* s, const tree_t* t, mxArray* out)
{
  //context->setScopeTokenID(t.context());
  mxArray a[1] = {0};
  interpreter_expression(s, t->m_children, a);
  //context->setScopeTokenID(t.context());

  if (!mxIsScalar(a)) {
    interpreter_DoBinaryOperator(s, t, mxAnd, "and", out);
  }
  else {
    // A is a scalar - is it FALSE?
    if (!(logical)mxGetScalar(a)) {
      toClass(out, a, Bool, 0);
    }
    else {
      interpreter_DoBinaryOperator(s, t, mxAnd, "and", out);
    }
  }

  return 0;
}

#if 0
//Works
// Need to take care

ArrayVector interpreter_handleReindexing(interpreter_t* s, const tree_t* t, const ArrayVector& p)
{
  if (t->m_childrenlen > 2)
    if (p.size() > 1) {
      throw_Exception("reindexing of function expressions not allowed when multiple values are returned by the function");
    }
    else {
      mxArray r;

      if (p.size() == 1) {
        r = p[0];
      }
      else {
        r = EmptyConstructor();
      }

      for (index = 2; index < t->m_childrenlen; index++) {
        deref(r, t.child(index));
      }

      return ArrayVector() << r;
    }
  else {
    return p;
  }
}

int interpreter_multiexpr(interpreter_t* s, const tree_t* t, mxArray* q, index_t lhsCount, BOOL output_optional)
{
  if (t.is(TOK_VARIABLE)) {
    ArrayReference ptr(context->lookupVariable(t->m_children.text()));

    if (!ptr.valid()) {
      ArrayVector p;
      functionExpression(t, int(lhsCount), output_optional, p);
      q += handleReindexing(t, p);
      return 0;
    }

    if (t->m_childrenlen == 1) {
      q.push_back(*ptr);
      return 0;
    }

    if (ptr->mxIsUserClass() && !stopoverload) {
      q += ClassRHSExpression(*ptr, t, s);
      return 0;
    }

    mxArray r(*ptr);

    for (index = 1; index < t->m_childrenlen - 1; index++) {
      deref(r, t.child(index));
    }

    SaveEndInfo;
    endRef = &r;
    const tree_t* s(t.last());

    if (s.is(TOK_PARENS)) {
      ArrayVector m;
      endTotal = s->m_childrenlen;

      if (s->m_childrenlen == 0) {
        q.push_back(r);
      }
      else {
        for (p = 0; p < s->m_childrenlen; p++) {
          endCount = m.size();
          multiexpr(s.child(p), m);
        }

        subsindex(m);

        if (m.size() == 1) {
          q.push_back(r.get(m.front()));
        }
        else {
          q.push_back(r.get(m));
        }
      }
    }
    else if (s.is(TOK_BRACES)) {
      ArrayVector m;
      endTotal = s->m_childrenlen;

      for (p = 0; p < s->m_childrenlen; p++) {
        endCount = m.size();
        multiexpr(s.child(p), m);
      }

      subsindex(m);

      if (m.size() == 1) {
        q += ArrayVectorFromCellArray(r.get(m.front()));
      }
      else {
        q += ArrayVectorFromCellArray(r.get(m));
      }
    }
    else if (s.is('.')) {
      q += r.get(s->m_children.text());
    }
    else if (s.is(TOK_DYN)) {
      const char* field;

      try {
        mxArray fname(expression(s->m_children));
        field = fname.asString();
      }
      catch (Exception& e) {
        throw_Exception("dynamic field reference to structure requires a string argument");
      }

      q += r.get(field);
    }

    RestoreEndInfo;
  }
  else if (!t.is(TOK_KEYWORD)) {
    q.push_back(expression(t));
  }
  return 0;
}
#endif

//Works
int interpreter_doubleColon(interpreter_t* s, const tree_t* t, mxArray* out)
{
  mxArray a[3] = {0};
  mxArray* b = a+1;
  mxArray* c = a+2;
  interpreter_expression(s, t->m_children->m_children, a);
  interpreter_expression(s, t->m_children->m_children+1, b);
  interpreter_expression(s, t->m_children+1, c);
  
  if (!(mxIsUserClass(a) || mxIsUserClass(b) || mxIsUserClass(c))) {
    DoubleColon(a, b, c, out);
  }
  else {
    ClassTrinaryOperator(a, b, c, "colon", s, out);
  }
  mxFreeArrays(a, 3);
  return 0;
}

//DOCBLOCK operators_colon
//Works
int interpreter_unitColon(interpreter_t* s, const tree_t* t, mxArray* out)
{
  mxArray a[3] = {0};
  mxArray* b = a+1;
  interpreter_expression(s, t->m_children, a);
  interpreter_expression(s, t->m_children+1, b);
  
  if (!(mxIsUserClass(a) || mxIsUserClass(b))) {
    UnitColon(a, b, out);
  }
  else {
    ClassBinaryOperator(a, b, "colon", s, out);
  }
  return 0;
}

int interpreter_rhs(interpreter_t* s, const tree_t* t, mxArray* out);
int interpreter_deref(interpreter_t* s, mxArray* r, const tree_t* t);
int interpreter_assignment(interpreter_t* s, const tree_t* var, BOOL printIt, mxArray* b);

// Merge this file with fun_t*.cpp
#define LOOKUP(x,field) x.constStructPtr()[field].get(1)


int interpreter_AnonFuncConstructor(interpreter_t* eval, const tree_t* t, mxArray* out)
{
#if 0
  StringVector variables;
  VariableReferencesList(t.second(), variables);
  StringVector arguments = IdentifierList(t.first());
  ArrayVector vals;
  StringVector fields;

  for (i = 0; i < variables.size(); i++) {
    ArrayReference ptr(eval->getContext()->lookupVariable(variables[i]));

    if (ptr.valid()) {
      fields << variables[i];
      vals << *ptr;
    }

    //    } else if (!arguments.contains(variables[i]))
    //      throw_Exception("variable " + variables[i] + " which is not an argument, must be defined when the anonymous function is declared.");
  }

  mxArray workspace(StructConstructor(fields, vals));
  mxArray args(CellArrayFromArray(CellArrayFromStringVector(arguments)));
  mxArray expr(mxArray(QString(t.text())));
  StringVector fieldnames;
  fieldnames << "args" << "workspace" << "expr";
  ArrayVector values;
  values << args << workspace << expr;
  mxArray ret(StructConstructor(fieldnames, values));
  ret.structPtr().mxSetClassName("anonfunction");
#endif
  return 0;
}

#if 0

QString AnonFuncToString(const mxArray* a)
{
  QString ret = " @(";
  StringVector argnames(StringVectorFromArray(LOOKUP(a, "args")));

  if (argnames.size() > 0) {
    ret += argnames[0];
  }

  for (j = 1; j < argnames.size(); j++) {
    ret += "," + argnames[j];
  }

  ret += ")  " + LOOKUP(a, "expr").asString();
  return ret;
}

//@@Signature
//sfunction @anonfunction:display AnonFuncDispFunction
//input x
//output none
ArrayVector AnonFuncDispFunction(int nargout, const ArrayVector& arg,
    interpreter_t* eval)
{
  for (i = 0; i < arg.size(); i++) {
    eval->outputMessage(AnonFuncToString(arg[i]));
    eval->outputMessage("\n");
  }

  return ArrayVector();
}

//@@Signature
//sfunction @anonfunction:subsref AnonFuncSubsrefFunction
//input x s
//output varargout
ArrayVector AnonFuncSubsrefFunction(int nargout, const ArrayVector& arg, interpreter_t* eval)
{
  // We should be in our own scope currently...
  // Insert the variables from the workspace into the context
  const mxArray* var(LOOKUP(arg[0], "workspace"));
  const StructArray& sp(var.constStructPtr());
  context_t* context = eval->getContext();
  // Borrow the callers scope name and detail... hope this doesn't
  // have unintended side effects... err...
  QString scopeName;
  QString scopeDetail;
  {
    ParentScopeLocker lock(context);
    scopeName = context->scopeName();
    scopeDetail = context->scopeDetailString();
  }
  context->pushScope(scopeName);
  context->setScopeDetailString(scopeDetail);

  for (i = 0; i < sp.fieldCount(); i++) {
    context->insertVariableLocally(sp.fieldName(i), sp[i].get(1));
  }

  // Retrieve the arguments...
  ArrayVector argSet;

  if (arg.size() == 2) {
    if (LOOKUP(arg[1], "type").asString() != "()") {
      throw_Exception("for anonymous functions, only a(x,...,y) is defined");
    }

    const mxArray* sub(LOOKUP(arg[1], "subs"));
    const BasicArray<mxArray>& rp(sub.constReal<mxArray>());

    for (index_t i = 1; i <= rp.length(); i++) {
      argSet.push_back(rp[i]);
    }
  }

  // Assign the arguments to internal variables.
  StringVector args(StringVectorFromArray(LOOKUP(arg[0], "args")));

  for (i = 0; i < MIN(argSet.size(), args.size()); i++) {
    context->insertVariableLocally(args[i], argSet[i]);
  }

  // We need to build a parse TREE
  const tree_t* expTree(ParseExpressionString(LOOKUP(arg[0], "expr").asString()));
  // Excecute the TREE
  ArrayVector outputs;

  try {
    eval->multiexpr(expTree, outputs, 1, TRUE);
  }
  catch (InterpreterBreakException&) {
  }
  catch (InterpreterContinueException&) {
  }
  catch (InterpreterReturnException&) {
  }

  context->popScope();
  return outputs;
}


//@@Signature
//sfunction @anonfunction:feval AnonFuncFevalFunction
//input x varargin
//output varargout
ArrayVector AnonFuncFevalFunction(int nargout, const ArrayVector& arg, interpreter_t* eval)
{
  if (arg.size() == 0) {
    return ArrayVector();
  }

  const mxArray* var(LOOKUP(arg[0], "workspace"));
  const StructArray& sp(var.constStructPtr());
  context_t* context = eval->getContext();
  // Borrow the callers scope name and detail... hope this doesn't
  // have unintended side effects... err...
  QString scopeName;
  QString scopeDetail;
  {
    ParentScopeLocker lock(context);
    scopeName = context->scopeName();
    scopeDetail = context->scopeDetailString();
  }
  context->pushScope(scopeName);
  context->setScopeDetailString(scopeDetail);

  for (i = 0; i < sp.fieldCount(); i++) {
    context->insertVariableLocally(sp.fieldName(i), sp[i].get(1));
  }

  // Assign the arguments to internal variables.
  StringVector argnames(StringVectorFromArray(LOOKUP(arg[0], "args")));

  for (i = 0; i < MIN(arg.size() - 1, argnames.size()); i++) {
    context->insertVariableLocally(argnames[i], arg[i + 1]);
  }

  // We need to build a parse TREE
  const tree_t* expTree(ParseExpressionString(LOOKUP(arg[0], "expr").asString()));
  // Excecute the TREE
  ArrayVector outputs;

  try {
    eval->multiexpr(expTree, outputs);
  }
  catch (InterpreterBreakException& e) {
  }
  catch (InterpreterContinueException& e) {
  }
  catch (InterpreterReturnException& e) {
  }

  context->popScope();
  return outputs;
}


#endif

// Look up a function by name.  Use the arguments (if available) to assist
// in resolving method calls for objects
BOOL interpreter_lookupFunction(interpreter_t* s, const char* funcName, fun_t* val,
                                mxArray* args, int narg, BOOL disableOverload)
{
  int passcount = 0;
#if 0
  while (passcount < 2) {
    // This is the order for function dispatch according to the Matlab manual
    // Nested functions - not explicitly listed in the Matlab manual, but
    // I figure they have the highest priority in the current scope.
    if (isMFile(context->scopeName()) &&
      (context->lookupFunction(NestedMangleName(context->scopeDetailString(), funcName), val))) {
      return TRUE;
    }
    
    if (InCLI && isMFile(context->activeScopeName()) &&
      (context->lookupFunction(NestedMangleName(context->activeScopeDetailString(), funcName), val))) {
      return TRUE;
    }
    
    // Not a nested function of the current scope.  We have to look for nested
    // functions of all parent scopes. Sigh.
    if (context->isCurrentScopeNested()) {
      const char* basename = context->scopeDetailString();
      
      while (!basename.isEmpty()) {
        if (context->lookupFunction(NestedMangleName(basename, funcName), val)) {
          return TRUE;
        }
        
        basename = StripNestLevel(basename);
      }
    }
    
    // Subfunctions
    if (inMFile() &&
      (context->lookupFunction(getLocalMangledName(funcName), val))) {
      return TRUE;
    }
    
    // Private functions
    // Not sure if you have to be an M-file in the current directory
    // to access a private function...
    if (context->lookupFunction(getPrivateMangledName(funcName), val)) {
      return TRUE;
    }
    
    // Class constructor functions
    if (context->lookupFunction(ClassMangleName(funcName, funcName), val)) {
      return TRUE;
    }
    
    if (!(disableOverload || stopoverload)) {
      // Look for a class method
      // Are any of the arguments classes?
      BOOL anyClasses = FALSE;
      int i = 0;
      
      while ((!anyClasses) && (i < args.size())) {
        anyClasses = args[i].mxIsUserClass();
        
        if (!anyClasses) {
          i++;
        }
      }
      
      // Yes, try and resolve the call to a method
      if (anyClasses && ClassResolveFunction(s, args[i], funcName, val)) {
        return TRUE;
      }
    }
    
    if (context->lookupFunction(funcName, val)) {
      return TRUE;
    }
    
    if (passcount == 0) {
      rescanPath();
    }
    
    passcount++;
  }
  
#endif
  return FALSE;
}

int interpreter_FunctionPointer(interpreter_t* s, const tree_t* t, mxArray* out)
{
  if (tree_is(t->m_children, TOK_ANONYMOUS_FUNC)) {
    interpreter_AnonFuncConstructor(s, t->m_children, out);
  }
  else {
#if 0
    fun_t* val;
    if (!interpreter_lookupFunction(s, t->m_children->m_node->m_text, val)) {
      throw_Exception("unable to resolve " + t->m_children.text() + " to a function call");
    }
    
    return FuncPtrConstructor(s, val);
#endif
  }
  return 0;
}

int interpreter_expression(interpreter_t* s, const tree_t* t, mxArray* out)
{
  int index;
  switch (t->m_node->m_tok) {
  case TOK_VARIABLE:
    return interpreter_rhs(s, t, out);

  case TOK_REAL:
  case TOK_IMAG:
  case TOK_REALF:
  case TOK_IMAGF:
  case TOK_STRING:
    return token_fillArray(t->m_node, out);

  case TOK_REINDEX: {
    interpreter_expression(s, t->m_children, out);

    for (index = 1; index < t->m_childrenlen; index++) {
      interpreter_deref(s, out, t->m_children+index);
    }

    return 0;
  }

  case TOK_INCR_PREFIX: {
    mxArray dummy[1] = {0};
    interpreter_rhs(s, t->m_children, dummy);
    mxAddS(dummy, 1, 1, out);
    interpreter_assignment(s, t->m_children, FALSE, out);
    mxFreeArray(dummy);
    return 0;
  }

  case TOK_DECR_PREFIX: {
    mxArray dummy[1] = {0};
    interpreter_rhs(s, t->m_children, dummy);
    mxAddS(dummy, 1, -1, out);
    interpreter_assignment(s, t->m_children, FALSE, out);
    mxFreeArray(dummy);
    return 0;
  }

  case TOK_INCR_POSTFIX: {
    mxArray dummy[1] = {0};
    interpreter_rhs(s, t->m_children, out);
    mxAddS(out, 1, 1, dummy);
    interpreter_assignment(s, t->m_children, FALSE, dummy);
    mxFreeArray(dummy);
    return 0;
  }

  case TOK_DECR_POSTFIX: {
    mxArray dummy[1] = {0};
    interpreter_rhs(s, t->m_children, out);
    mxAddS(out, 1, -1, dummy);
    interpreter_assignment(s, t->m_children, FALSE, dummy);
    mxFreeArray(dummy);
    return 0;
  }

  case TOK_END:
    if (NULL==s->endRef->m_ptr) {
      throw_Exception("END keyword not allowed for undefined variables");
    }

    if (s->endTotal == 1) {
      int n = mxGetNumberOfElements(s->endRef->m_ptr);
      mxSetDoubleScalar(out, n);
      return 0;
    }
    else {
      int n = s->endRef->m_ptr->dims[s->endCount];
      mxSetDoubleScalar(out, n);
      return 0;
    }

  case TOK_COLON: // :
    if (t->m_childrenlen == 0) {
      mxSetString(out, ":", 1);
      return 0;
    }
    else if (t->m_children->m_node->m_tok == TOK_COLON) {
      return interpreter_doubleColon(s, t, out);
    }
    else {
      return interpreter_unitColon(s, t, out);
    }

    break;

  case TOK_MATDEF:
    return interpreter_matrixDefinition(s, t, out);
    break;

  case TOK_CELLDEF:
    return interpreter_cellDefinition(s, t, out);
    break;

  case TOK_PLUS:
    return interpreter_DoBinaryOperator(s, t, mxAdd, "plus", out);
    break;

  case TOK_MINUS:
    return interpreter_DoBinaryOperator(s, t, mxSub, "minus", out);
    break;

  case TOK_MUL:
    return interpreter_DoBinaryOperator(s, t, mxMul, "mtimes", out);
    break;

  case TOK_RDIV:
    return interpreter_DoBinaryOperator(s, t, mxRDiv, "mrdivide", out);
    break;

  case TOK_LDIV:
    return interpreter_DoBinaryOperator(s, t, mxLDiv, "mldivide", out);
    break;

  case TOK_SOR:
    return interpreter_ShortCutOr(s, t, out);
    break;

  case TOK_OR:
    return interpreter_DoBinaryOperator(s, t, mxOr, "or", out);
    break;

  case TOK_SAND:
    return interpreter_ShortCutAnd(s, t, out);
    break;

  case TOK_AND:
    return interpreter_DoBinaryOperator(s, t, mxAnd, "and", out);

  case TOK_LSS:
    return interpreter_DoBinaryOperator(s, t, mxLss, "lt", out);
    break;

  case TOK_LEQ:
    return interpreter_DoBinaryOperator(s, t, mxLeq, "le", out);
    break;

  case TOK_GTR:
    return interpreter_DoBinaryOperator(s, t, mxGtr, "gt", out);
    break;

  case TOK_GEQ:
    return interpreter_DoBinaryOperator(s, t, mxGeq, "ge", out);
    break;

  case TOK_EQU:
    return interpreter_DoBinaryOperator(s, t, mxEqu, "eq", out);
    break;

  case TOK_NEQ:
    return interpreter_DoBinaryOperator(s, t, mxNeq, "ne", out);
    break;

  case TOK_DOTTIMES:
    return interpreter_DoBinaryOperator(s, t, mxDotMul, "times", out);
    break;

  case TOK_DOTRDIV:
    return interpreter_DoBinaryOperator(s, t, mxDotRDiv, "rdivide", out);
    break;

  case TOK_DOTLDIV:
    return interpreter_DoBinaryOperator(s, t, mxDotLDiv, "ldivide", out);
    break;

  case TOK_UNARY_MINUS:
    return interpreter_DoUnaryOperator(s, t, mxNegate, "uminus", out);
    break;

  case TOK_UNARY_PLUS:
    return interpreter_DoUnaryOperator(s, t, mxPlus, "uplus", out);
    break;

  case TOK_TILDE: // ~
    return interpreter_DoUnaryOperator(s, t, mxNot, "not", out);
    break;

  case TOK_CARET: // ^
    return interpreter_DoBinaryOperator(s, t, mxPower, "mpower", out);
    break;

  case TOK_DOTPOWER:
    return interpreter_DoBinaryOperator(s, t, mxDotPow, "power", out);
    break;

  case TOK_SINGLE_QUOTE: // '
    return interpreter_DoUnaryOperator(s, t, Transpose, "ctranspose", out);
    break;

  case TOK_DOTTRANSPOSE:
    return interpreter_DoUnaryOperator(s, t, Transpose, "transpose", out);
    break;

  case TOK_AT: // @
    return interpreter_FunctionPointer(s, t, out);

  default:
    throw_Exception("Unrecognized expression!");
  }
}

int interpreter_deleteHandleClass(interpreter_t* s, mxArray* ap)
{
#if 0
  // We need to call the destructor on
  fun_t* val;

  if (mxIsUserClass(b) && ClassResolveFunction(s, b, "delete", val)) {
    val->updateCode(s);
    ArrayVector args(b);
    doFunction(val, args, 1);
  }
#endif
  return 0;
}

/**
 * This somewhat strange test is used by the switch statement.
 * If x is a scalar, and we are a scalar, s is an equality
 * test.  If x is a string and we are a string, s is a
 * strcmp test.  If x is a scalar and we are a cell-array, s
 * test is applied on an element-by-element basis, looking for
 * any matches.  If x is a string and we are a cell-array, then
 * s is applied on an element-by-element basis also.
*/
int interpreter_block(interpreter_t* s, const tree_t* t);
BOOL interpreter_testCaseStatement(interpreter_t* s, const tree_t* t, const mxArray* s1)
{
  BOOL caseMatched;
  mxArray r[1] = {0};
  interpreter_expression(s, t->m_children, r);
  caseMatched = TestForCaseMatch(s1, r);

  if (caseMatched) {
    interpreter_block(s, t->m_children+1);
  }
  mxFreeArray(r);
  return caseMatched;
}

//DOCBLOCK flow_try
//Works
int interpreter_tryStatement(interpreter_t* s, const tree_t* t)
{
  // Turn off autostop for s statement block
  BOOL autostop_save=0, intryblock_save=0, stackdepth=0;
  autostop_save = s->autostop;
  intryblock_save = s->intryblock;
  s->autostop = FALSE;
  s->intryblock = TRUE;
  // Get the state of the IDnum stack and the
  // contextStack and the cnameStack
  //stackdepth = context->scopeDepth();
  
  interpreter_block(s, t->m_children);
#if 0
  {
    while (context->scopeDepth() > stackdepth) {
      context->popScope();
    }
    
    if (t->m_childrenlen > 1) {
      autostop = autostop_save;
      block(t->m_children[1].m_children);
    }
  }
#endif

  s->autostop = autostop_save;
  s->intryblock = intryblock_save;
}


//DOCBLOCK flow_switch
//Works
int interpreter_switchStatement(interpreter_t* s, const tree_t* t)
{
  int n;
  mxArray switchVal[1] = {0};
  // First, extract the value to perform the switch on.
  interpreter_expression(s, t->m_children, switchVal);

  // Assess its type to determine if s is a scalar switch
  // or a string switch.
  if (!mxIsScalar(switchVal) && !mxIsString(switchVal)) {
    throw_Exception("Switch statements support scalar and string arguments only.");
  }

  for (n = 0; n < t->m_childrenlen && tree_is(t->m_children+n, TOK_CASE); ++n) {
    if (interpreter_testCaseStatement(s, t->m_children+n, switchVal)) {
      mxFreeArray(switchVal);
      return 0;
    }
  }

  if (tree_is(t->m_children+t->m_childrenlen-1, TOK_OTHERWISE)) {
    interpreter_block(s, t->m_children[t->m_childrenlen-1].m_children);
  }
  mxFreeArray(switchVal);
  return 0;
}

//DOCBLOCK flow_if
//Works
int interpreter_ifStatement(interpreter_t* s, const tree_t* t)
{
  BOOL condtest;
  mxArray out[1] = {0};
  interpreter_expression(s, t->m_children, out);
  condtest = RealAllNonZeros(out);

  if (condtest) {
    interpreter_block(s, t->m_children+1);
    return 0;
  }
  else {
    int n;

    for (n = 1; n < t->m_childrenlen && tree_is(t->m_children+n, TOK_ELSEIF); ++n) {
      interpreter_expression(s, t->m_children[n].m_children, out);
      if (RealAllNonZeros(out)) {
        interpreter_block(s, t->m_children[n].m_children+1);
        return 0;
      }
    }
  }

  if (tree_is(t->m_children+t->m_childrenlen-1, TOK_ELSE)) {
    interpreter_block(s, t->m_children[t->m_childrenlen-1].m_children);
  }
  mxFreeArray(out);
  return 0;
}

//DOCBLOCK flow_while
//Works
int interpreter_whileStatement(interpreter_t* s, const tree_t* t)
{
  const tree_t* testCondition;
  const tree_t* codeBlock;
  BOOL breakEncountered = FALSE, conditionTrue;
  mxArray condVar[1] = {0};

  testCondition = (t->m_children);
  codeBlock = (t->m_children+1);
  interpreter_expression(s, testCondition, condVar);
  conditionTrue = RealAllNonZeros(condVar);
  //context->enterLoop();
  breakEncountered = FALSE;

  while (conditionTrue && !breakEncountered) {
    interpreter_block(s, codeBlock);
    if (!breakEncountered) {
      interpreter_expression(s, testCondition, condVar);
      conditionTrue = RealAllNonZeros(condVar);
    }
  }

  //context->exitLoop();
}

//Helper functions for FOR loops.  This template function
//handles the index variable with the correct type.  Reducing
//the net loop time

int ForLoopHelper(interpreter_t* s, const tree_t* codeBlock, const mxArray* indexSet,
    int count, const char* indexName)
{
  int m;
  for (m = 0; m < count; m++) {
#if 0
    mxArray* vp = eval->getContext()->lookupVariableLocally(indexName);

    if ((!vp) || (!mxIsScalar(vp))) {
      eval->getContext()->insertVariableLocally(indexName, mxArray());
      vp = eval->getContext()->lookupVariableLocally(indexName);
    }

    *vp = indexSet.get(m);
    interpreter_block(s, codeBlock);
#endif
  }
}

int ForLoopIterator(const tree_t* codeBlock, const char* indexName,
    mxArray* first, mxArray* last, mxArray* step, interpreter_t* eval)
{
#if 0
  int nsteps;

  if (!(first mxIsScalar() && last mxIsScalar() && step mxIsScalar())) {
    throw_Exception("Loop parameters must be scalar.");
  }

  mxArray* vp = eval->getContext()->lookupVariableLocally(indexName);

  if ((!vp) || (!vp->isScalar())) {
    eval->getContext()->insertVariableLocally(indexName, mxArray());
    vp = eval->getContext()->lookupVariableLocally(indexName);
  }

  BOOL bIntLoop = (IsIntegerDataClass(first) || IsIntegerDataClass(last) || IsIntegerDataClass(step));

  if (bIntLoop) {
    //integer loop path
    mxArray temp1;

    temp1 = DotRightDivide(Subtract(last, first), step);        //( ( l - f ) / s )
    nsteps = temp1.asInteger() + 1;     //( ( l - f ) / s )+1

    if (nsteps < 0) {
      return 0;
    }

    for (m = 0; m < nsteps; m++) {
      *vp = Add(first, DotMultiply(mxArray(m), step));

      TRR_CALL(eval->block(codeBlock));
    }
  }
  else {
    //floating point loop path
    BOOL bFloatLoop = (first->classID == Float || last->classID == Float || step->classID == Float);
    double f = first mxGetScalar();
    double l = last mxGetScalar();
    double s = step mxGetScalar();

    if (bFloatLoop) {
      nsteps = num_for_loop_iter_f(f, s, l);
    }
    else {
      nsteps = num_for_loop_iter(f, s, l);
    }

    for (double m = 0; m < nsteps; m++) { //array variable should be of type double for correct typing of DotMultiply
      *vp = Add(first, DotMultiply(mxArray(m), step));

      try {
        eval->block(codeBlock);
      }
      catch (InterpreterContinueException&) {}
      catch (InterpreterBreakException&) {
        break;
      }
    }
  }
#endif
  return 0;
}

float num_for_loop_iter_f(float first, float step, float last)
{
  int signum, nsteps;
  float mismatch;

  signum = (step > 0) - (step < 0);
  nsteps = (int) floor((last - first) / step) + 1;

  if (nsteps < 0) {
    return 0;
  }

  mismatch = signum * (first + nsteps * step - last);

  if ((mismatch > 0) && (mismatch < 3.*fepsf(last)) && (step != rint(step))) {    //allow overshoot by 3 eps in some cases
    nsteps++;
  }

  return (float)nsteps;
}

double num_for_loop_iter(double first, double step, double last)
{
  int signum = (step > 0) - (step < 0);
  int nsteps = (int) floor((last - first) / step) + 1;
  double mismatch;

  if (nsteps < 0) {
    return 0;
  }

  mismatch = signum * (first + nsteps * step - last);

  if ((mismatch > 0) && (mismatch < 3.*feps(last)) && (step != rint(step))) {    //allow overshoot by 3 eps in some cases
    nsteps++;
  }

  return nsteps;
}

//DOCBLOCK flow_for
//Works

int interpreter_forStatement(interpreter_t* s, const tree_t* t)
{
#if 0
  mxArray indexSet;
  const char* indexVarName;

  /* Get the name of the indexing variable */
  if (!t->m_children.is('=')) {
    throw_Exception("Incorrect format of for operator");
  }

  indexVarName = t->m_children->m_children.text();

  if (t->m_children->m_children+1.is(TOK_MATDEF) ||
      t->m_children->m_children+1.is(TOK_VARIABLE))   {
    //case "for j=[1:10]"
    //case "for j=K" skb
    /* Evaluate the index set */
    indexSet = expression(t->m_children->m_children+1);

    /* Get the code block */
    const tree_t* codeBlock(t->m_children+1);
    index_t elementCount = indexSet.length();
    DataClass loopType(indexSet->classID);
    ContextLoopLocker lock(context);

    switch (loopType) {
    case Invalid:
      throw_Exception("Invalid arrays not supported");

    case Struct:
      throw_Exception("Structure arrays are not supported as for loop index sets");

    case CellArray:
      ForLoopHelper<mxArray>(codeBlock, indexSet,
          elementCount, indexVarName, s);
      break;

    case Bool:
      ForLoopHelper<logical>(codeBlock, indexSet,
          elementCount, indexVarName, s);
      break;

    case UInt8:
      ForLoopHelper<uint8>(codeBlock, indexSet,
          elementCount, indexVarName, s);
      break;

    case Int8:
      ForLoopHelper<int8>(codeBlock, indexSet,
          elementCount, indexVarName, s);
      break;

    case UInt16:
      ForLoopHelper<uint16>(codeBlock, indexSet,
          elementCount, indexVarName, s);
      break;

    case Int16:
      ForLoopHelper<int16>(codeBlock, indexSet,
          elementCount, indexVarName, s);
      break;

    case UInt32:
      ForLoopHelper<uint32>(codeBlock, indexSet,
          elementCount, indexVarName, s);
      break;

    case Int32:
      ForLoopHelper<int32>(codeBlock, indexSet,
          elementCount, indexVarName, s);
      break;

    case UInt64:
      ForLoopHelper<uint64>(codeBlock, indexSet,
          elementCount, indexVarName, s);
      break;

    case Int64:
      ForLoopHelper<int64>(codeBlock, indexSet,
          elementCount, indexVarName, s);
      break;

    case Float:
      ForLoopHelper<float>(codeBlock, indexSet,
          elementCount, indexVarName, s);
      break;

    case Double:
      ForLoopHelper<double>(codeBlock, indexSet,
          elementCount, indexVarName, s);
      break;

    case StringArray:
      ForLoopHelper<uint8>(codeBlock, indexSet,
          elementCount, indexVarName, s);
      break;
    }
  }
  else if (t->m_children->m_children[1].m_node->m_tok == ':') {
    if (t->m_children->m_children[1].m_childrenlen == 0) {
      throw_Exception("Incorrect format of loop operator parameters");
    }

    mxArray first, step, last;
    const tree_t* codeBlock(t->m_children+1);
    ContextLoopLocker lock(context);

    if (t->m_children->m_children[1].m_children.is(':')) {
      first = expression(t->m_children->m_children[1].m_children->m_children);
      step = expression(t->m_children->m_children[1].m_children->m_children+1);
      last = expression(t->m_children->m_children[1].m_children+1);
      ForLoopIterator(codeBlock, indexVarName, first, last, step, s);
      //return doubleColon(t);
    }
    else {
      first = expression(t->m_children->m_children[1].m_children);
      last = expression(t->m_children->m_children[1].m_children+1);
      mxArray tmp(BasicArray<double>(1));
      ForLoopIterator(codeBlock, indexVarName, first, last, tmp, s);
      //return unitColon(t);
    }
  }
#endif
  return 0;
}

//DOCBLOCK variables_global
int interpreter_globalStatement(interpreter_t* s, const tree_t* t)
{
#if 0
  int i;
  for (i = 0; i < t->m_childrenlen; i++) {
    const char* name = t->m_children[i].m_node->m_text;
    context->addGlobalVariable(name);

    if (!context->lookupVariable(name).valid()) {
      context->insertVariable(name, EmptyConstructor());
    }
  }
#endif
}

//DOCBLOCK variables_persistent
int interpreter_persistentStatement(interpreter_t* s, const tree_t* t)
{
#if 0
  int i;
  for (i = 0; i < t->m_childrenlen; i++) {
    const char* name = t.child(i).text();
    context->addPersistentVariable(name);

    if (!context->lookupVariable(name).valid()) {
      context->insertVariable(name, EmptyConstructor());
    }
  }
#endif
}

int interpreter_doDebugCycle(interpreter_t* s)
{
#if 0
  depth++;
  PopContext saver(context, 0);
  {
    context->pushScope("keyboard", "keyboard");
    PopContext saver2(context, 0);
    context->setScopeActive(FALSE);

    try {
      evalCLI();
    }
    catch (InterpreterContinueException& e) {
    }
    catch (InterpreterBreakException& e) {
    }
    catch (InterpreterReturnException& e) {
    }
    catch (InterpreterRetallException& e) {
      depth--;
      throw;
    }
  }
  depth--;
#endif
}

int interpreter_displayArray(interpreter_t* s, mxArray b)
{
#if 0
  // Check for a user defined class
  fun_t* val;

  if (mxIsUserClass(b) && ClassResolveFunction(s, b, "display", val)) {
    if (val->updateCode(s)) {
      refreshBreakpoints();
    }

    ArrayVector args(b);
    ArrayVector retvec(doFunction(val, args, 1));
  }
  else {
    PrintArrayClassic(b, printLimit, s);
  }
#endif
}

//Works
int interpreter_expressionStatement(interpreter_t* s, const tree_t* t0, BOOL printIt)
{
#if 0
  mxArray m[1] = {0};

  if (!s.is(TOK_EXPR)) {
    throw_Exception("Unexpected statement type!");
  }

  const tree_t* t = (t0->m_children);
  // There is a special case to consider here - when a
  // function call is made as a statement, we do not require
  // that the function have an output.
  mxArray b;
  ArrayReference ptr;

  if (tree_is(t, TOK_VARIABLE)) {
    ptr = context->lookupVariable(t->m_children.text());

    if (!ptr.valid()) {
      functionExpression(t, 0, TRUE, m);
      m = handleReindexing(t, m);
      BOOL emptyOutput = FALSE;

      if (m.size() == 0) {
        b = EmptyConstructor();
        emptyOutput = TRUE;
      }
      else {
        b = m[0];
      }

      if (printIt && (!emptyOutput)) {
        outputMessage(("\nans = \n"));
        displayArray(b);
      }
    }
    else {
      multiexpr(t, m);

      if (m.size() == 0) {
        b = EmptyConstructor();
      }
      else {
        b = m[0];

        if (printIt) {
          outputMessage(("\nans = \n"));

          for (j = 0; j < m.size(); j++) {
            char buffer[1000];

            if (m.size() > 1) {
              sprintf(buffer, "\n%d of %d:\n", j + 1, m.size());
              outputMessage((buffer));
            }

            displayArray(m[j]);
          }
        }
      }
    }
  }
  else {
    b = expression(t);

    if (printIt) {
      outputMessage(("\nans = \n"));
      displayArray(b);
    }
  }

  context->insertVariable("ans", b);
  mxFreeArray(m);
#endif
}

int interpreter_multiassign(interpreter_t* s, ArrayReference r, const tree_t* t0, mxArray* data, int ndata)
{
#if 0
  SaveEndInfo;
  endRef = r;

  if (s.is(TOK_PARENS)) {
    ArrayVector m;
    endTotal = s->m_childrenlen;

    if (s->m_childrenlen == 0) {
      throw_Exception("The expression A() is not legal unless A is a function");
    }

    for (p = 0; p < s->m_childrenlen; p++) {
      endCount = m.size();
      multiexpr(s.child(p), m);
    }

    subsindex(m);

    if (m.size() == 1) {
      r->set(m[0], data[0]);
    }
    else {
      r->set(m, data[0]);
    }

    data.pop_front();
  }
  else if (s.is(TOK_BRACES)) {
    ArrayVector m;
    endTotal = s->m_childrenlen;

    for (p = 0; p < s->m_childrenlen; p++) {
      endCount = m.size();
      multiexpr(s.child(p), m);
    }

    subsindex(m);

    if (m.size() == 1) {
      SetCellContents(*r, m[0], data);
    }
    else {
      SetCellContents(*r, m, data);
    }
  }
  else if (s.is('.')) {
    r->set(s->m_children.text(), data);
  }
  else if (s.is(TOK_DYN)) {
    const char* field;

    try {
      mxArray fname(expression(s->m_children));
      field = fname.asString();
    }
    catch (Exception& e) {
      throw_Exception("dynamic field reference to structure requires a string argument");
    }

    r->set(field, data);
  }

  RestoreEndInfo;
#endif
}

int interpreter_assign(interpreter_t* s, ArrayReference r, const tree_t* t0, mxArray* data)
{
#if 0
  SaveEndInfo;
  endRef = r;

  if (s.is(TOK_PARENS)) {
    ArrayVector m;
    endTotal = s->m_childrenlen;

    if (s->m_childrenlen == 0) {
      throw_Exception("The expression A() is not legal unless A is a function");
    }

    for (p = 0; p < s->m_childrenlen; p++) {
      endCount = m.size();
      multiexpr(s.child(p), m);
    }

    subsindex(m);

    if (m.size() == 1) {
      r->set(m[0], data);
    }
    else {
      r->set(m, data);
    }
  }
  else if (s.is(TOK_BRACES)) {
    ArrayVector datav(data);
    ArrayVector m;
    endTotal = s->m_childrenlen;

    for (p = 0; p < s->m_childrenlen; p++) {
      endCount = m.size();
      multiexpr(s.child(p), m);
    }

    subsindex(m);

    if (m.size() == 1) {
      SetCellContents(*r, m[0], datav);
    }
    else {
      SetCellContents(*r, m, datav);
    }
  }
  else if (s.is('.')) {
    ArrayVector datav(data);
    r->set(s->m_children.text(), datav);
  }
  else if (s.is(TOK_DYN)) {
    const char* field;

    try {
      mxArray fname(expression(s->m_children));
      field = fname.asString();
    }
    catch (Exception& e) {
      throw_Exception("dynamic field reference to structure requires a string argument");
    }

    ArrayVector datav(data);
    r->set(field, datav);
  }

  RestoreEndInfo;
#endif
}


int interpreter_createVariable(interpreter_t* s, const char* name, ArrayReference* np)
{
#if 0
  fun_t* p;
  PopContext saver(context, 0);

  // This is annoying.
  //  if (context->lookupFunction(name,p) && (name.size() > 1))
  //    warningMessage("Newly defined variable " + name + " shadows a function of the same name.  Use clear " + name + " to recover access to the function");
  // Are we in a nested scope?
  if (!context->isCurrentScopeNested() || context->variableLocalToCurrentScope(name)) {
    // if not, just create a local variable in the current scope, and move on.
    context->insertVariable(name, EmptyConstructor());
  }
  else {
    // if so - walk up the scope chain until we are no longer nested
    const char* localScopeName = context->scopeName();
    context->bypassScope(1);

    while (context->currentScopeNests(localScopeName)) {
      context->bypassScope(1);
    }

    context->restoreScope(1);

    // We wre now pointing to the highest scope that contains the present
    // (nested) scope.  Now, we start walking down the chain looking for
    // someone who accesses s variable
    while (!context->currentScopeVariableAccessed(name) &&
        context->scopeName() != localScopeName) {
      context->restoreScope(1);
    }

    // Either we are back in the local scope, or we are pointing to
    // a scope that (at least theoretically) accesses a variable with
    // the given name.
    context->insertVariable(name, EmptyConstructor());
  }

  ArrayReference np(context->lookupVariable(name));

  if (!np.valid())
    throw_Exception("error creating variable name " + name +
        " with scope " + context->scopeName());

  return np;
#endif
}

//Works
// The case of a(1,2).foo.goo{3} = rhs
// The TREE looks like s:
// Variable
//    Identifier
//    ()
//      etc
//    .
//      foo
//    .
//      goo
//    {}
//      3
//
// We have to do:
//   a1 = id       data = id
//   a2 = id(etc)  stack[0] = id(etc)
//   a3 = a2.foo   stack[1] = stack[0].foo
//   a4 = a3.goo   stack[2] = stack[1].goo
//   a3{3} = rhs   data{3} = rhs
//   a2.foo = a3
//   id(etc) = a2;

//DOCBLOCK array_assign
int interpreter_assignment(interpreter_t* s, const tree_t* var, BOOL printIt, mxArray* b)
{
#if 0
  const char* name(var->m_children.text());
  ArrayReference ptr(context->lookupVariable(name));

  if (!ptr.valid()) {
    ptr = createVariable(name);
  }

  if (var->m_childrenlen == 1) {
    (*ptr) = b;
  }
  else if (ptr->mxIsUserClass() &&
      !inMethodCall(ptr->className()) &&
      !stopoverload) {
    ClassAssignExpression(ptr, var, b, s);
  }
  else if (var->m_childrenlen == 2) {
    assign(ptr, var->m_children+1, b);
  }
  else {
    ArrayVector stack;
    mxArray data(*ptr);
    int varCount = var->m_childrenlen;

    for (index = 1; index < varCount - 1; index++) {
      if (!data.isEmpty()) {
        try {
          deref(data, var.child(index));
        }
        catch (Exception& e) {
          data = EmptyConstructor();
        }
      }

      stack.push_back(data);
    }

    assign(&data, var.child(varCount - 1), b);
    mxArray rhs(data);

    if (stack.size() > 0) {
      stack.pop_back();
      int ptr = 0;

      while (stack.size() > 0) {
        data = stack.back();
        assign(&data, var.child(varCount - 2 - ptr), rhs);
        rhs = data;
        stack.pop_back();
        ptr++;
      }
    }

    assign(ptr, var.child(1), rhs);
  }

  if (printIt) {
    interpreter_outputMessage(s, "\n");
    outputMessage(name);
    interpreter_outputMessage(s, " = \n");
    displayArray(*ptr);
  }
#endif
  return 0;
}

int interpreter_processBreakpoints(interpreter_t* s, const tree_t* t)
{
#if 0
  for (i = 0; i < bpStack.size(); i++) {
    if ((bpStack[i].cname == context->scopeName()) &&
        ((LineNumber(context->scopeTokenID()) == bpStack[i].tokid))) {
      doDebugCycle();
      context->setScopeTokenID(t.context());
    }
  }

  if (tracetrap > 0) {
    if ((LineNumber(context->scopeTokenID()) != tracecurrentline)) {
      tracetrap--;

      if (tracetrap == 0) {
        doDebugCycle();
      }
    }
  }

  if (context->scopeStepTrap() > 0) {
    if ((LineNumber(context->scopeTokenID())) !=
        context->scopeStepCurrentLine()) {
      context->setScopeStepTrap(context->scopeStepTrap() - 1);

      if (context->scopeStepTrap() == 0) {
        doDebugCycle();
      }
    }
  }
#endif
}


//Trapping at the statement level is much better! - two
//problems... try/catch and multiline statements (i.e.,atell.m)
//The try-catch one is easy, I think...  When a try occurs,
//we capture the stack depth... if an exception occurs, we
//unwind the stack to s depth..
//The second one is trickier - suppose we have a conditional
//statement
//if (a == 3)
//    bfunc
//else
//    cfunc
//end
//s is represented in the parse TREE as a single construct...
//

//
//Works
int interpreter_statementType(interpreter_t* s, const tree_t* t, BOOL printIt);
int interpreter_statement(interpreter_t* s, const tree_t* t)
{
  if (tree_is(t, TOK_QSTATEMENT)) {
    interpreter_statementType(s, t->m_children, FALSE);
  }
  else if (tree_is(t, TOK_STATEMENT)) {
    interpreter_statementType(s, t->m_children, s->m_quietlevel == 0);
  }
  else {
    throw_Exception("Unexpected statement type!\n");
  }
#if 0
  if (Exception& e) {
    if (autostop) {
      errorCount++;
      e.printMe(s);
      stackTrace();
      doDebugCycle();
    }
    else  {
      if (!e.wasHandled() && !InCLI && !intryblock) {
        stackTrace();
        e.markAsHandled();
      }
    }
  }
#endif
  return 0;
}

int interpreter_stackTrace(interpreter_t* s, int skiplevels);
#define throw_InterpreterKillException() return 2
//Works
int interpreter_block(interpreter_t* s, const tree_t* t)
{
  int i;
  for (i = 0; i < t->m_childrenlen; i++) {
    if (s->m_kill) {
      throw_InterpreterKillException();
    }
    
    if (s->m_interrupt) {
      interpreter_outputMessage(s, "Interrupt (ctrl-b) encountered\n");
      interpreter_stackTrace(s, 0);
      s->m_interrupt = FALSE;
      interpreter_doDebugCycle(s);
    }
    else {
      interpreter_statement(s, t->m_children+i);
    }
  }

#if 0
  if (Exception& e) {
    lasterr = e.msg();
    throw;
  }
#endif
}

// I think s is too complicated.... there should be an easier way
// Works
index_t interpreter_countLeftHandSides(interpreter_t* s, const tree_t* t)
{
#if 0
  mxArray lhs;
  ArrayReference ptr(context->lookupVariable(t->m_children.text()));

  if (!ptr.valid()) {
    lhs = EmptyConstructor();
  }
  else {
    lhs = *ptr;
  }

  if (t->m_childrenlen == 1) {
    return 1;
  }

  if (t.last().is(TOK_PARENS)) {
    return 1;
  }

  for (index = 1; index < t->m_childrenlen - 1; index++) {
    try {
      deref(lhs, t.child(index));
    }
    catch (Exception& e) {
      lhs = EmptyConstructor();
    }
  }

  if (t.last().is(TOK_BRACES)) {
    const tree_t* s(t.last());
    ArrayVector m;

    for (p = 0; p < s->m_childrenlen; p++) {
      multiexpr(s.child(p), m);
    }

    subsindex(m);

    if (m.size() == 0) {
      throw_Exception("Expected indexing expression!");
    }

    if (m.size() == 1) {
      // m[0] should have only one element...
      if (IsColonOp(m[0])) {
        return (lhs.length());
      }

      return (IndexArrayFromArray(m[0]).length());
    }
    else {
      int i = 0;
      index_t outputCount = 1;

      while (i < m.size()) {
        if (IsColonOp(m[i])) {
          outputCount *= lhs->dims[i];
        }
        else {
          outputCount *= IndexArrayFromArray(m[i]).length();
        }

        i++;
      }

      return (outputCount);
    }
  }

  if (t.last().is('.')) {
    return std::max((index_t)1, lhs.length());
  }
  
#endif
  return 1;
}

int interpreter_AllColonReference(interpreter_t* s, const mxArray* v, int index, int count, mxArray* out)
{
#if 0
  if (v.mxIsUserClass()) {
    return EmptyConstructor();
  }

  mxArray((":"));
#endif
  return 0;
}

//test
int interpreter_specialFunctionCall(interpreter_t* s, const tree_t* t, BOOL printIt)
{
#if 0
  ArrayVector m;
  vstr_t args;

  for (index = 0; index < t->m_childrenlen; index++) {
    args.push_back(t.child(index).text());
  }

  if (args.empty()) {
    return 0;
  }

  ArrayVector n;

  for (i = 1; i < args.size(); i++) {
    n.push_back(mxArray(args[i]));
  }

  fun_t* val;

  if (!lookupFunction(args[0], val, n)) {
    throw_Exception("unable to resolve " + args[0] + " to a function call");
  }

  if (val->updateCode(s)) {
    refreshBreakpoints();
  }

  m = doFunction(val, n, 0);
#endif
}

int interpreter_setBreakpoint(interpreter_t* s, stackentry bp, BOOL enableFlag)
{
#if 0
  fun_t* val;
  BOOL isFun = lookupFunction(bp.detail, val);

  if (!isFun) {
    warningMessage(("unable to find function ") +
        bp.detail + " to set breakpoint");
    return 0;
  }

  if (val->type() != FM_M_FUNCTION) {
    warningMessage("function " + bp.detail +
        " is not an m-file, and does not support breakpoints");
    return 0;
  }

  //  try {
  //     //    ((MFunctionDef*)val)->SetBreakpoint(bp.tokid,enableFlag);
  //   } catch (Exception &e) {
  //     e.printMe(s);
  //   }
#endif
  return 0;
}

int interpreter_addBreakpoint(interpreter_t* s, stackentry bp)
{
#if 0
  bpStack.push_back(bp);
  refreshBreakpoints();
  emit RefreshBPLists();
#endif
  return 0;
}

int interpreter_refreshBreakpoints()
{
#if 0
  for (i = 0; i < bpStack.size(); i++) {
    setBreakpoint(bpStack[i], TRUE);
  }
#endif
  return 0;
}

//Some notes on the multifunction call...  This one is pretty complicated, and the current logic is hardly transparent.  Assume we have an expression of the form:
//
//[expr1 expr2 ... exprn] = fcall
//
//where fcall is a function call (obviously).  Now, we want to determine how many output arguments fcall should have.  There are several interesting cases to consider:
//
//expr_i is an existing numeric variable -- lhscount += 1
//
//expr_i is an existing cell array -- lhscount += size(expr_i)
//
//expr_i is an existing struct array -- lhscount += size(expr_i)
//
//expr_i does not exist -- lhscount += 1
//
//Where s will fail is in one case.  If expr_i is a cell reference for a variable that does not exist, and has a sized argument, something like
//[eg{1:3}]
//in which case the lhscount += 3, even though eg does not exist.
// WORKS
int interpreter_multiFunctionCall(interpreter_t* s, const tree_t* t, BOOL printIt)
{
#if 0
  ArrayVector m;
  TreeList s;
  mxArray c;
  index_t lhsCount;

  if (!t->m_children.is(TOK_BRACKETS)) {
    throw_Exception("Illegal left hand side in multifunction expression");
  }

  s = t->m_children.children();
  // We have to make multiple passes through the LHS part of the AST.
  // The first pass is to count how many function outputs are actually
  // being requested.
  // Calculate how many lhs objects there are
  lhsCount = 0;

  for (ind = 0; ind < s.size(); ind++) {
    lhsCount += countLeftHandSides(s[ind]);
  }

  multiexpr(t->m_children+1, m, lhsCount);

  int index;

  for (index = 0; (index < s.size()) && (m.size() > 0); index++) {
    const tree_t* var(s[index]);
    const char* name(var->m_children.text());
    ArrayReference ptr(context->lookupVariable(name));

    if (!ptr.valid()) {
      ptr = createVariable(name);
    }

    if (ptr->mxIsUserClass() &&
        !inMethodCall(ptr->className()) &&
        !stopoverload && (var->m_childrenlen > 1)) {
      ClassAssignExpression(ptr, var, m.front(), s);
      m.pop_front();
      return 0;
    }

    if (var->m_childrenlen == 1) {
      (*ptr) = m.front();
      m.pop_front();
    }
    else if (var->m_childrenlen == 2) {
      multiassign(ptr, var->m_children+1, m);
    }
    else {
      ArrayVector stack;
      mxArray data(*ptr);
      int varCount = var->m_childrenlen;

      for (index = 1; index < varCount - 1; index++) {
        if (!data.isEmpty()) {
          try {
            deref(data, var.child(index));
          }
          catch (Exception& e) {
            data = EmptyConstructor();
          }
        }

        stack.push_back(data);
      }

      multiassign(&data, var.child(varCount - 1), m);
      mxArray rhs(data);

      if (stack.size() > 0) {
        stack.pop_back();
        int ptr = 0;

        while (stack.size() > 0) {
          data = stack.back();
          assign(&data, var.child(varCount - 2 - ptr), rhs);
          rhs = data;
          stack.pop_back();
          ptr++;
        }
      }

      assign(ptr, var.child(1), rhs);
    }

    if (printIt) {
      outputMessage(name);
      interpreter_outputMessage(s, " = \n");
      displayArray(*ptr);
    }
  }

  if (index < s.size()) {
    warningMessage("one or more outputs not assigned in call.");
  }
#endif
  return 0;
}

int getArgumentIndex(vstr_t* list, const char* t)
{
  BOOL foundArg = FALSE;
  const char* q;
  int i = 0;

  while (i < list->n && !foundArg) {
    q = list->v[i].s;

    if (q[0] == '&') {
      q++;
    }

    foundArg = 0 == strcmp(q, t);

    if (!foundArg) {
      i++;
    }
  }

  if (foundArg) {
    return i;
  }
  else {
    return -1;
  }
}

//DOCBLOCK functions_function


//DOCBLOCK functions_anonymous

//DOCBLOCK functions_keywords

//DOCBLOCK functions_varargin

//DOCBLOCK functions_varargout

//DOCBLOCK functions_script

//DOCBLOCK functions_special
int interpreter_collectKeywords(interpreter_t* s, const tree_t* q, mxArray* keyvals, int nkey, tree_t keyexpr, vstr_t* keywords)
{
  // Search for the keyword uses -
  // To handle keywords, we make one pass through the arguments,
  // recording a list of keywords used and using ::expression to
  // evaluate their values.
  for (index = 0; index < q->m_childrenlen; index++) {
    if (q.child(index).is(TOK_KEYWORD)) {
      keywords.push_back(q.child(index)->m_children.text());

      if (q.child(index)->m_childrenlen > 1) {
        keyvals.push_back(expression(q.child(index)->m_children+1));
        keyexpr.push_back(q.child(index)->m_children+1);
      }
      else {
        keyvals.push_back(mxArray(BOOL(TRUE)));
        keyexpr.push_back(tree_t());
      }
    }
  }
}

int* interpreter_sortKeywords(interpreter_t* s, ArrayVector& m, vstr_t& keywords,
    vstr_t arguments, ArrayVector keyvals)
{
  // If keywords were used, we have to permute the
  // entries of the arrayvector to the correct order.
  int* keywordNdx = new int[keywords.size()];
  int maxndx;
  maxndx = 0;

  // Map each keyword to an argument number
  for (i = 0; i < keywords.size(); i++) {
    int ndx;
    ndx = getArgumentIndex(arguments, keywords[i]);

    if (ndx == -1) {
      throw_Exception("out-of-order argument /" + keywords[i] + " is not defined in the called function!");
    }

    keywordNdx[i] = ndx;

    if (ndx > maxndx) {
      maxndx = ndx;
    }
  }

  // Next, we have to determine how many "holes" there are
  // in the argument list - we get the maximum list
  int holes;
  holes = maxndx + 1 - keywords.size();
  // At s point, holes is the number of missing arguments
  // If holes > m.size(), then the total number of arguments
  // is just maxndx+1.  Otherwise, its
  // maxndx+1+(m.size() - holes)
  int totalCount;

  if (holes > m.size()) {
    totalCount = maxndx + 1;
  }
  else {
    totalCount = maxndx + 1 + (m.size() - holes);
  }

  // Next, we allocate a vector to hold the values
  ArrayVector toFill;

  for (i = 0; i < totalCount; i++) {
    toFill.push_back(mxArray());
  }

  //  ArrayVector toFill(totalCount);
  BOOL* filled = new BOOL[totalCount];
  int* argTypeMap = new int[totalCount];

  for (i = 0; i < totalCount; i++) {
    filled[i] = FALSE;
    argTypeMap[i] = -1;
  }

  // Finally...
  // Copy the keyword values in
  for (i = 0; i < keywords.size(); i++) {
    toFill[keywordNdx[i]] = keyvals[i];
    filled[keywordNdx[i]] = TRUE;
    argTypeMap[keywordNdx[i]] = i;
  }

  // Fill out the rest of the values from m
  int n = 0;
  int p = 0;

  while (n < m.size()) {
    if (!filled[p]) {
      toFill[p] = m[n];
      filled[p] = TRUE;
      argTypeMap[p] = -2;
      n++;
    }

    p++;
  }

  // Finally, fill in empty matrices for the
  // remaining arguments
  for (i = 0; i < totalCount; i++)
    if (!filled[i]) {
      toFill[i] = EmptyConstructor();
    }

  // Clean up
  delete[] filled;
  delete[] keywordNdx;
  // Reassign
  m = toFill;
  return argTypeMap;
}

// arguments is exactly what it should be - the vector of arguments
// m is vector of argument values
// keywords is the list of values passed as keywords
// keyexpr is the
int interpreter_handlePassByReference(interpreter_t* s, tree_t q, vstr_t arguments,
    ArrayVector m, vstr_t keywords,
    TreeList keyexpr, int* argTypeMap)
{
  tree_t p;
  // M functions can modify their arguments
  int maxsearch = m.size();

  if (maxsearch > arguments.size()) {
    maxsearch = arguments.size();
  }

  int qindx = 0;

  for (i = 0; i < maxsearch; i++) {
    // Was s argument passed out of order?
    if ((keywords.size() > 0) && (argTypeMap[i] == -1)) {
      continue;
    }

    if ((keywords.size() > 0) && (argTypeMap[i] >= 0)) {
      p = keyexpr[argTypeMap[i]];
    }
    else {
      p = q->m_children+1.child(qindx);
      qindx++;

      if (qindx >= q->m_children[1].m_childrenlen) {
        qindx = q->m_children[1].m_childrenlen - 1;
      }
    }

    const char* args(arguments[i]);

    if (args[0] == '&') {
      args.remove(0, 1);

      // This argument was passed by reference
      if (!p.valid() || !(p.is(TOK_VARIABLE))) {
        throw_Exception("Must have lvalue in argument passed by reference");
      }

      assignment(p, FALSE, m[i]);
    }
  }
}

//Test
int interpreter_functionExpression(interpreter_t* s, const tree_t* t, int narg_out, BOOL outputOptional, ArrayVector& output)
{
#if 0
  ArrayVector m, n;
  vstr_t keywords;
  ArrayVector keyvals;
  TreeList keyexpr;
  fun_t* funcDef;
  int* argTypeMap;

  // Because of the introduction of user-defined classes, we have to
  // first evaluate the keywords and the arguments, before we know
  // which function to call.
  // First, check for arguments
  if ((t->m_childrenlen >= 2) && t->m_children+1.is(TOK_PARENS)) {
    // Collect keywords
    interpreter_collectKeywords(t->m_children+1, keyvals, keyexpr, keywords);

    // Evaluate function arguments
    try {
      const tree_t* s(t->m_children+1);

      for (p = 0; p < s->m_childrenlen; p++) {
        interpreter_multiexpr(s.child(p), m);
      }
    }
    catch (Exception& e) {
      // Transmute the error message about illegal use of ':'
      // into one about undefined variables.  Its crufty,
      // but it works.
      if (e.matches("Illegal use of the ':' operator")) {
        throw_Exception("Undefined variable " + t.text());
      }
      else {
        throw;
      }
    }
  }

  // Now that the arguments have been evaluated, we have to
  // find the dominant class
  if (!lookupFunction(t->m_children.text(), funcDef, m))
    throw_Exception("Undefined function or variable " +
        t->m_children.text());

  if (funcDef->updateCode(s)) {
    refreshBreakpoints();
  }

  if (funcDef->scriptFlag) {
    if (t->m_childrenlen >= 2) {
      throw_Exception(("Cannot use arguments in a call to a script."));
    }

    if ((narg_out > 0) && !outputOptional) {
      throw_Exception(("Cannot assign outputs in a call to a script."));
    }

    context->pushScope(((MFunctionDef*)funcDef)->fileName,
        ((MFunctionDef*)funcDef)->name, FALSE);
    context->setScopeActive(FALSE);
    block(((MFunctionDef*)funcDef)->code);

    if (context->scopeStepTrap() >= 1) {
      tracetrap = 1;
      tracecurrentline = 0;
      warningMessage("dbstep beyond end of script " + context->scopeDetailString() +
          " -- setting single step mode\n");
      context->setScopeStepTrap(0);
    }

    context->popScope();
  }
  else {
    // We can now adjust the keywords (because we know the argument list)
    // Apply keyword mapping
    if (!keywords.empty()) {
      argTypeMap = interpreter_sortKeywords(m, keywords, funcDef->arguments, keyvals);
    }
    else {
      argTypeMap = NULL;
    }

    if ((funcDef->inputArgCount() >= 0) &&
        (m.size() > funcDef->inputArgCount())) {
      throw_Exception(("Too many inputs to function ") + t->m_children.text());
    }

    if ((funcDef->outputArgCount() >= 0) &&
        (narg_out > funcDef->outputArgCount() && !outputOptional)) {
      throw_Exception(("Too many outputs to function ") + t->m_children.text());
    }

    n = doFunction(funcDef, m, narg_out);

    // Check for any pass by reference
    if (t.hasChildren() && (funcDef->arguments.size() > 0)) {
      handlePassByReference(t, funcDef->arguments, m, keywords, keyexpr, argTypeMap);
    }
  }

  // Some routines (e.g., min and max) will return more outputs
  // than were actually requested... so here we have to trim
  // any elements received that we didn't ask for.
  // preserve one output if we were called as an expression (for ans)
  if (outputOptional) {
    narg_out = (narg_out == 0) ? 1 : narg_out;
  }

  while (n.size() > narg_out) {
    n.pop_back();
  }

  output += n;
#endif
  return 0;
}


int interpreter_toggleBP(interpreter_t* s, const char* fname, int lineNumber)
{
  if (isBPSet(fname, lineNumber)) {
    const char* fname_string(fname);

    for (i = 0; i < bpStack.size(); i++)
      if ((bpStack[i].cname == fname_string) &&
          (LineNumber(bpStack[i].tokid) == lineNumber)) {
        deleteBreakpoint(bpStack[i].number);
        return 0;
      }
  }
  else {
    addBreakpoint(fname, lineNumber);
  }
}

MFunctionDef* interpreter_lookupFullPath(interpreter_t* s, const char* fname)
{
  vstr_t allFuncs(context->listAllFunctions());
  fun_t* val;

  for (i = 0; i < allFuncs.size(); i++) {
    BOOL isFun = context->lookupFunction(allFuncs[i], val);

    if (!isFun || !val) {
      return NULL;
    }

    if (val->type() == FM_M_FUNCTION) {
      MFunctionDef* mptr;
      mptr = (MFunctionDef*) val;

      if (mptr->fileName ==  fname) {
        return mptr;
      }
    }
  }

  return NULL;
}

static int bpList = 1;
// Add a breakpoint - name is used to track to a full filename
int interpreter_addBreakpoint(interpreter_t* s, const char* name, int line)
{
  fun_t* val;
  // Map the name argument to a full file name.
  const char* fullFileName;

  if (context->lookupFunction(name, val) && (val->type() == FM_M_FUNCTION)) {
    fullFileName = ((MFunctionDef*) val)->fileName;
  }
  else {
    fullFileName = name;
  }

  // Get a list of all functions
  vstr_t allFuncs(context->listAllFunctions());

  // We make one pass through the functions, and update
  // those functions that belong to the given filename
  for (i = 0; i < allFuncs.size(); i++) {
    BOOL isFun = context->lookupFunction(allFuncs[i], val);

    if (!isFun || !val) {
      throw_Exception("Cannot add breakpoint to " + name + " :  it does not appear to be a valid M file.");
    }

    if (val->type() == FM_M_FUNCTION) {
      MFunctionDef* mptr = (MFunctionDef*) val;

      if (mptr->fileName == fullFileName) {
        mptr->updateCode(s);
      }
    }
  }

  // Refresh the list of all functions
  allFuncs = context->listAllFunctions();
  // Search through the list for any function defined  - for each function,
  // record the line number closest to it
  MemBlock<int> line_dist_block(allFuncs.size());
  int* line_dist = &line_dist_block;

  for (i = 0; i < allFuncs.size(); i++) {
    line_dist[i] = 2 * max_line_count;
  }

  for (i = 0; i < allFuncs.size(); i++) {
    BOOL isFun = context->lookupFunction(allFuncs[i], val);

    if (!isFun || !val) {
      throw_Exception("Cannot add breakpoint to " + name + " :  it does not appear to be a valid M file.");
    }

    if (val->type() == FM_M_FUNCTION) {
      MFunctionDef* mptr = (MFunctionDef*) val;

      if (mptr->fileName == fullFileName) {
        try {
          int dline = mptr->ClosestLine(line);
          line_dist[i] = dline;
        }
        catch (Exception& e) {
        }
      }
    }
  }

  // Second pass through it - find the function with a line number closest to the
  // desired one, but not less than it
  int best_func = -1;
  int best_dist = 2 * max_line_count;

  for (i = 0; i < allFuncs.size(); i++) {
    if ((line_dist[i] >= line) && ((line_dist[i] - line) < best_dist)) {
      best_func = i;
      best_dist = line_dist[i] - line;
    }
  }

  if (best_dist > max_line_count)
    //    warningMessage(("Cannot set breakpoint at line ")+line+" of file "+name + ".  \r\nThis can be caused by an illegal line number, or a function that is not on the path or in the current directory.");
  {
    emit IllegalLineOrCurrentPath(name, line);
  }
  else {
    addBreakpoint(stackentry(fullFileName, allFuncs[best_func], best_dist + line, bpList++));
  }
}

BOOL interpreter_isBPSet(interpreter_t* s, const char* fname, int lineNumber)
{
  for (i = 0; i < bpStack.size(); i++)
    if ((bpStack[i].cname == fname) &&
        (LineNumber(bpStack[i].tokid) == lineNumber)) {
      return TRUE;
    }

  return FALSE;
}

// called by editor
BOOL interpreter_isInstructionPointer(interpreter_t* s, const char* fname, int lineNumber)
{
  if (!InCLI) {
    return FALSE;
  }

  ParentScopeLocker lock(context);
  const char* filename(context->scopeName());
  int token(context->scopeTokenID());
  return ((fname == filename) && ((lineNumber == LineNumber(token)) ||
      ((lineNumber == 1) && (LineNumber(token) == 0))));
}

int interpreter_listBreakpoints(interpreter_t* s)
{
  for (i = 0; i < bpStack.size(); i++) {
    const char* buffer = const char * ("%1   %2 line %3\n").arg(bpStack[i].number)
        .arg(bpStack[i].cname).arg(LineNumber(bpStack[i].tokid));
    outputMessage(buffer);
  }
}

int interpreter_deleteBreakpoint(interpreter_t* s, int number)
{
  for (i = 0; i < bpStack.size(); i++)
    if (bpStack[i].number == number) {
      bpStack.remove(i);
      emit RefreshBPLists();
      return 0;
    }

  warningMessage("Unable to delete specified breakpoint (does not exist)");
  emit RefreshBPLists();
  return 0;
}

int interpreter_stackTrace(interpreter_t* s, int skiplevels)
{
  BOOL firstline = TRUE;
  int depth = context->scopeDepth();
  context->bypassScope(skiplevels);

  for (i = 0; i < (depth - skiplevels); i++) {
    if ((context->scopeName() == "keyboard") &&
        (context->scopeDetailString() == "keyboard")) {
      context->bypassScope(1);
      continue;
    }

    if (firstline) {
      firstline = FALSE;
    }
    else {
      outputMessage(("    "));
    }

    outputMessage(("In ") + context->scopeName() + "("
        + context->scopeDetailString() + ")");
    int line = int(LineNumber(context->scopeTokenID()));

    if (line > 0)
      outputMessage((" at line " +
          const char * ().setNum(LineNumber(context->scopeTokenID()))));

    interpreter_outputMessage(s, "\r\n");
    context->bypassScope(1);
  }

  context->restoreScope(depth);
}

BOOL interpreter_inMethodCall(interpreter_t* s, const char* classname)
{
  if (context->scopeDetailString().isEmpty()) {
    return FALSE;
  }

  if (context->scopeDetailString()[0] != '@') {
    return FALSE;
  }

  return (context->scopeDetailString().mid(1, classname.size()) == classname);
}

BOOL interpreter_lookupFunction(interpreter_t* s, const char* funcName, fun_t*& val)
{
  ArrayVector dummy;
  return(lookupFunction(funcName, val, dummy));
}

BOOL IsNestedName(interpreter_t* s, const char* basename)
{
  return (basename.lastIndexOf("/") >= 0);
}

const char* StripNestLevel(interpreter_t* s, const char* basename)
{
  int ndx = basename.lastIndexOf("/");

  if (ndx >= 0) {
    basename.remove(ndx, basename.size());
  }
  else {
    basename = "";
  }

  return basename;
}

//DOCBLOCK variables_functionhandles

//DOCBLOCK variables_indexing


// This has a few shortcomings that prevent it from being
// 100% correct.
//
//   1.  subsindex is not called for argument
//       expressions of user-defined classes.
//   2.  "end" no longer works.
//
// To fix "end", we should use a source transformation technique.
// The original TREE looks like s
//
//   variable
//      -> t
//      -> ()
//          -> 2
//          -> end
//
// This should be translated into:
//
//   _t = end(t,2)
//
// This is done in Transform.cpp...
//
// This does not cover:
//    Function pointers
//    subsindex
//
//

//
//
int interpreter_deref(interpreter_t* s, mxArray* r, const tree_t* s)
{
  SaveEndInfo;
  endRef = &r;

  if (s.is(TOK_PARENS)) {
    ArrayVector m;
    endTotal = s->m_childrenlen;

    if (s->m_childrenlen == 0) {
      r = r;
    }
    else {
      for (p = 0; p < s->m_childrenlen; p++) {
        endCount = m.size();
        multiexpr(s.child(p), m);
      }

      subsindex(m);

      if (m.size() == 1) {
        r = r.get(m[0]);
      }
      else {
        r = r.get(m);
      }
    }
  }
  else if (s.is(TOK_BRACES)) {
    ArrayVector m;
    endTotal = s->m_childrenlen;

    for (p = 0; p < s->m_childrenlen; p++) {
      endCount = m.size();
      multiexpr(s.child(p), m);
    }

    subsindex(m);

    if (m.size() == 1) {
      r = ArrayFromCellArray(r.get(m[0]));
    }
    else {
      r = ArrayFromCellArray(r.get(m));
    }
  }
  else if (s.is('.')) {
    r = r.get(s->m_children.text()).front();
  }
  else if (s.is(TOK_DYN)) {
    const char* field;

    try {
      mxArray fname(expression(s->m_children));
      field = fname.asString();
    }
    catch (Exception& e) {
      throw_Exception("dynamic field reference to structure requires a string argument");
    }

    r = r.get(field).front();
  }

  RestoreEndInfo;
}

int interpreter_rhs(interpreter_t* s, const tree_t* t, mxArray* out)
{
  ArrayReference ptr(context->lookupVariable(t->m_children.text()));

  if (!ptr.valid()) {
    ArrayVector m;
    functionExpression(t, 1, FALSE, m);
    m = handleReindexing(t, m);

    if (m.size() >= 1) {
      return m[0];
    }
    else {
      return EmptyConstructor();
    }
  }

  if (t->m_childrenlen == 1) {
    return *ptr;
  }

  if (ptr->mxIsUserClass() && !stopoverload) {
    ArrayVector m(ClassRHSExpression(*ptr, t, s));

    if (m.size() >= 1) {
      return m[0];
    }
    else {
      return EmptyConstructor();
    }
  }

  mxArray r(*ptr);

  for (index = 1; index < t->m_childrenlen; index++) {
    deref(r, t.child(index));
  }

  return r;
}


int interpreter_getErrorCount(interpreter_t* s)
{
  int retval = errorCount;
  errorCount = 0;
  return retval;
}

interpreter_set(interpreter_t* s, context_t* aContext)
{
  errorCount = 0;
  lasterr = ("");
  context = aContext;
  depth = 0;
  printLimit = 1000;
  autostop = FALSE;
  intryblock = FALSE;
  jitcontrol = JITOff;
  jitcount = 0;
  stopoverload = FALSE;
  s->m_skipflag = FALSE;
  s->m_noprompt = FALSE;
  s->m_liveUpdateFlag = FALSE;
  tracetrap = 0;
  tracecurrentline = 0;
  endRef = NULL;
  s->m_interrupt = FALSE;
  s->m_kill = FALSE;
  s->m_diaryState = FALSE;
  s->m_diaryFilename = "diary";
  s->m_captureState = FALSE;
  s->m_capture = "";
  s->m_profile = FALSE;
  s->m_quietlevel = 0;
  s->m_enableWarnings = TRUE;
  s->m_disablerescan = FALSE;
  context->pushScope("base", "base", FALSE);
}

BOOL interpreter_getDisableRescan(interpreter_t* s)
{
  return s->m_disablerescan;
}

int interpreter_setDisableRescan(interpreter_t* s, BOOL flag)
{
  s->m_disablerescan = flag;
}

BOOL interpreter_getStopOverload(interpreter_t* s)
{
  return stopoverload;
}

int interpreter_setStopOverload(interpreter_t* s, BOOL flag)
{
  stopoverload = flag;
}

// stackentry& interpreter_activeDebugStack() {
//   if (cstack.isEmpty()) throw_Exception("Debug stack is corrupted -- please file a bug report that reproduces s problem!");
//   if (cstack.size() < 2) return cstack[0];
//   return cstack[cstack.size()-2];
// }

// const stackentry& interpreter_activeDebugStack() const {
//   if (cstack.isEmpty()) throw_Exception("Debug stack is corrupted -- please file a bug report that reproduces s problem!");
//   if (cstack.size() < 2) return cstack[0];
//   return cstack[cstack.size()-2];
// }


// We want dbstep(n) to cause us to advance n statements and then
// stop.  we execute statement-->set step trap,
int interpreter_dbstepStatement(interpreter_t* s, const tree_t* t)
{
  int lines = 1;

  if (t.hasChildren()) {
    mxArray lval(expression(t->m_children));
    lines = lval.asInteger();
  }

  // Get the current function
  fun_t* val;

  if (context->scopeName() == "base") {
    return 0;
  }

  ParentScopeLocker lock(context);

  if (!lookupFunction(context->scopeDetailString(), val)) {
    warningMessage(("unable to find function ") + context->scopeDetailString() + " to single step");
    return 0;
  }

  context->setScopeStepTrap(lines);
  context->setScopeStepCurrentLine(LineNumber(context->scopeTokenID()));
}

int interpreter_dbtraceStatement(interpreter_t* s, const tree_t* t)
{
  int lines = 1;

  if (t.hasChildren()) {
    mxArray lval(expression(t->m_children));
    lines = lval.asInteger();
  }

  // Get the current function
  fun_t* val;

  if (context->scopeDetailString() == "base") {
    return 0;
  }

  ParentScopeLocker lock(context);

  if (!lookupFunction(context->scopeDetailString(), val)) {
    warningMessage(("unable to find function ") + context->scopeDetailString() + " to single step");
    return 0;
  }

  tracetrap = lines;
  tracecurrentline = LineNumber(context->scopeTokenID());
}

int interpreter_statementType(interpreter_t* s, const tree_t* t, BOOL printIt)
{
  mxArray b[1] = {0};
  // check the debug flag
  //context->setScopeTokenID(t.context());
  interpreter_processBreakpoints(s, t);

  switch (t->m_node->m_tok) {
  case '=':
    interpreter_expression(s, t->m_children+1, b);
    interpreter_assignment(s, t->m_children, printIt, b);
    mxFreeArray(b);
  break;

  case TOK_MULTI:
    interpreter_multiFunctionCall(s, t, printIt);
    break;

  case TOK_SPECIAL:
    interpreter_specialFunctionCall(s, t, printIt);
    break;

  case TOK_FOR:
    interpreter_forStatement(s, t);
    break;

  case TOK_WHILE:
    interpreter_whileStatement(s, t);
    break;

  case TOK_IF:
    interpreter_ifStatement(s, t);
    break;

  case TOK_BREAK:
    //if (context->inLoop())
    {
      //throw_InterpreterBreakException();
    }

    break;

  case TOK_CONTINUE:
    //if (context->inLoop())
    {
      //throw InterpreterContinueException();
    }

    break;

  case TOK_DBSTEP:
    //interpreter_dbstepStatement(t);
    //RefreshBPLists();
    //throw InterpreterReturnException();
    break;

  case TOK_DBTRACE:
    interpreter_dbtraceStatement(t);
    RefreshBPLists();
    throw_InterpreterReturnException();
    break;

  case TOK_DBUP:
    interpreter_dbup();
    break;

  case TOK_DBDOWN:
    interpreter_dbdown();
    break;

  case TOK_RETURN:
    throw InterpreterReturnException();
    break;

  case TOK_SWITCH:
    interpreter_switchStatement(t);
    break;

  case TOK_TRY:
    interpreter_tryStatement(t);
    break;

  case TOK_QUIT:
    throw InterpreterQuitException();
    break;

  case TOK_RETALL:
    throw InterpreterRetallException();
    break;

  case TOK_KEYBOARD:
    interpreter_doDebugCycle();
    break;

  case TOK_GLOBAL:
    interpreter_globalStatement(t);
    break;

  case TOK_PERSISTENT:
    interpreter_persistentStatement(t);
    break;

  case TOK_EXPR:
    interpreter_expressionStatement(t, printIt);
    break;

  case TOK_NEST_FUNC:
    break;

  default:
    throw_Exception("Unrecognized statement type");
  }
  return 0;
}

// static const char* EvalPrep(const char* line) {
//   const char* buf1 = line;
//   if (buf1.endsWith('\n'))
//     buf1.chop(1);
//   if (buf1.endsWith('\r'))
//     buf1.chop(1);
//   if (buf1.size() > 20)
//     buf1 = buf1.left(20) + "...";
//   return buf1;
// }

int interpreter_ExecuteLine(interpreter_t* s, const char* txt)
{
  mutex.lock();
  cmd_buffer.push_back(txt);
  bufferNotEmpty.wakeAll();
  mutex.unlock();

  if (s->m_diaryState) {
    interpreter_diaryMessage(s, txt);
  }
}

//PORT
int interpreter_evaluateString(interpreter_t* s, const char* line, BOOL propogateExceptions)
{
  tree_t b;
  tree_t t;
  s->m_interrupt = FALSE;
  Scanner S(line, "");
  Parser P(S);

  try {
    b = P.process();
    t = b;

    if (!t.is(TOK_SCRIPT)) {
      throw_Exception("Function definition unexpected!");
    }

    t = t->m_children;
  }
  catch (Exception& e) {
    if (propogateExceptions) {
      throw;
    }

    errorCount++;
    e.printMe(s);
    return 0;
  }

  try {
    block(t);
  }
  catch (Exception& e) {
    if (propogateExceptions) {
      throw;
    }

    errorCount++;
    e.printMe(s);
  }
}

const char* interpreter_getLastErrorString(interpreter_t* s)
{
  return lasterr;
}

int interpreter_setLastErrorString(interpreter_t* s, const char* txt)
{
  lasterr = txt;
}

int interpreter_setGreetingFlag(interpreter_t* s, BOOL skip)
{
  s->m_skipflag = skip;
}

int interpreter_setNoPromptFlag(interpreter_t* s, BOOL noprompt)
{
  s->m_noprompt = noprompt;
}

BOOL NeedsMoreInput(interpreter_t* eval, const char* txt)
{
  // Check for ... or an open []
  try {
    Scanner S(txt, "");

    while (!S.next().is(TOK_EOF)) {
      S.consume();
    }

    if (S.inContinuationState() || S.inBracket()) {
      return TRUE;
    }
  }
  catch (Exception& e) {
  }

  try {
    Scanner S(txt, "");
    Parser P(S);
    P.process();
    return FALSE;
  }
  catch (Exception& e) {
    if (e.msg().left(13) == "Expecting end") {
      return TRUE;
    }
  }

  return FALSE;
}

int interpreter_sleepMilliseconds(interpreter_t* s, unsigned long msecs)
{
  QThread::msleep(msecs);
}

const char* interpreter_getLine(interpreter_t* s, const char* prompt)
{
  if (!s->m_noprompt) {
    emit SetPrompt(prompt);
  }

  if (s->m_diaryState) {
    interpreter_diaryMessage(s, prompt);
  }

  const char* retstring;
  emit EnableRepaint();
  mutex.lock();

  if (cmd_buffer.isEmpty()) {
    bufferNotEmpty.wait(&mutex);
  }

  retstring = cmd_buffer.front();
  cmd_buffer.erase(cmd_buffer.begin());
  mutex.unlock();
  emit DisableRepaint();
  return retstring;
}

// This is a "generic" CLI routine.  The user interface (non-debug)
// version of s is "docli"
int interpreter_evalCLI(interpreter_t* s)
{
  const char* prompt;
  BOOL rootCLI;
  interpreter_setupWatcher(s);

  while (1) {
    const char* fname;
    int line = 0;

    if ((depth == 0) || (context->scopeDepth() < 2)) {
      prompt = "--> ";
      rootCLI = TRUE;
    }
    else {
      int bypasscount = 0;

      while (InKeyboardScope(context)) {
        bypasscount++;
        context->bypassScope(1);
      }

      fname = context->scopeName();
      line = LineNumber(context->scopeTokenID());
      const char* scopename = context->scopeDetailString();

      if (scopename == "builtin") {
        scopename = context->scopeName();
      }

      if (scopename == "docli") {
        scopename = "base";
      }

      prompt = const char * ("[%1,%2]--> ").arg(scopename).arg(line);
      context->restoreScope(bypasscount);
      rootCLI = FALSE;
    }

    if (rootCLI) {
      tracetrap = 0;
      context->setScopeStepTrap(0);
    }

    if (s->m_captureState) {
      s->m_capture += prompt;
    }
    else {
      if (!s->m_noprompt) {
        emit SetPrompt(prompt);
      }

      if (s->m_diaryState) {
        interpreter_diaryMessage(s, prompt);
      }
    }

    if (s->m_liveUpdateFlag) {
      updateVariablesTool();
      updateStackTool();
      emit ShowActiveLine(fname, line);
    }

    const char* cmdset;
    const char* cmdline;
    emit EnableRepaint();
    mutex.lock();

    while ((cmdset.isEmpty() ||
        NeedsMoreInput(s, cmdset)) && (!s->m_interrupt)) {
      if (cmd_buffer.isEmpty()) {
        bufferNotEmpty.wait(&mutex);
      }

      cmdline = cmd_buffer.front();
      cmd_buffer.erase(cmd_buffer.begin());
      cmdset += cmdline;

      if (s->m_captureState) {
        s->m_capture += cmdline;
      }
    }

    mutex.unlock();
    emit DisableRepaint();

    if (s->m_interrupt) {
      s->m_interrupt = FALSE;
      continue;
    }

    int scope_stackdepth = context->scopeDepth();
    setInCLI(TRUE);
    dbdown_executed = FALSE;
    evaluateString(cmdset, FALSE);

    if (!dbdown_executed) {
      while (context->scopeDepth() > scope_stackdepth) {
        context->popScope();
      }
    }
  }
}


//
// Convert a list of variable into indexing expressions
//  - for user defined classes, we call subsindex for
//  - the object
mxArray interpreter_subsindex(interpreter_t* s, const mxArray* m)
{
  if (m.mxIsUserClass() && !stopoverload) {
    mxArray t(ClassUnaryOperator(m, "subsindex", s));
    return Add(t.toClass(Double), mxArray(index_t(1)));
  }

  return m;
}

int interpreter_subsindex(interpreter_t* s, ArrayVector& m)
{
  for (p = 0; p < ((int)m.size()); p++) {
    m[p] = subsindex(m[p]);
  }
}
