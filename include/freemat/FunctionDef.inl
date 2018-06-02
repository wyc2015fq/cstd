
#define MSGBUFLEN 2048

//QMutex functiondefmutex;

int IdentifierList(const tree_t* t, vstr_t* retval)
{
  int index;

  for (index = 0; index < t->m_childrenlen; index++) {
    if (t->m_children[index].m_node->m_tok == TOK_AND) {
      char buf[256] = "&";
      token_t* tok = t->m_children[index].m_children[0].m_node;
      strncat(buf, tok->m_text, tok->m_textlen);
      strv_push_str(retval, STR1(buf));
    }
    else {
      token_t* tok = t->m_children[index].m_node;
      strv_push_str(retval, STR2(tok->m_text, tok->m_textlen));
    }
  }

  return 0;
}

int VariableReferencesList(const tree_t* t, vstr_t* idents)
{
  int i;
  token_t* tok;

  if (t->m_node->m_tok == TOK_NEST_FUNC) {
    return 0;
  }

  if (t->m_node->m_tok == TOK_VARIABLE) {
    BOOL exists = FALSE;

    for (i = 0; i < idents->n; i++) {
      tok = t->m_children[0].m_node;
      exists = str_cmp(idents->v[i], STR2(tok->m_text, tok->m_textlen));

      if (exists) {
        break;
      }
    }

    if (!exists) {
      tok = t->m_children[0].m_node;
      strv_push_str(idents, STR2(tok->m_text, tok->m_textlen));
    }
  }

  for (i = 0; i < t->m_childrenlen; i++) {
    VariableReferencesList(t->m_children + i, idents);
  }

  return 0;
}

int MFunctionDef_set(fun_t* s)
{
  s->functionCompiled = FALSE;
  s->localFunction = FALSE;
  s->pcodeFunction = FALSE;
  s->nestedFunction = FALSE;
  return 0;
}

int MFunctionDef_inputArgCount(fun_t* s)
{
  if (s->arguments->n == 0) {
    return 0;
  }

  if (0 == str_cmp(s->arguments->v[s->arguments->n - 1], STR1("varargin"))) {
    return -1;
  }
  else {
    return s->arguments->n;
  }
}

int MFunctionDef_outputArgCount(fun_t* s)
{
  if (s->returnVals->n == 0) {
    return 0;
  }

  if (0 == str_cmp(s->returnVals->v[s->returnVals->n - 1], STR1("varargout"))) {
    return -1;
  }
  else {
    return s->returnVals->n;
  }
}

int MFunctionDef_printMe(fun_t* s, interpreter_t* eval)
{
  int i;
  interpreter_outputMessage(eval, "Function name:%s", s->name);
  interpreter_outputMessage(eval, "Function class: Compiled M function\n");
  interpreter_outputMessage(eval, "s->returnVals: ");

  for (i = 0; i < s->returnVals->n; i++) {
    interpreter_outputMessage(eval, "%s ", s->returnVals->v[i].s);
  }

  interpreter_outputMessage(eval, "\n");
  interpreter_outputMessage(eval, "s->arguments: ");

  for (i = 0; i < s->arguments->n; i++) {
    interpreter_outputMessage(eval, "%s ", s->arguments->v[i].s);
  }

  interpreter_outputMessage(eval, "\ncode: \n");
  tree_print(s->code, 0);
}

int MFunctionDef_evaluateFunc(fun_t* s, interpreter_t* walker, ArrayVector* inputs, int nargout, VariableTable* workspace, ArrayVector* out)
{
#if 0
  context_t* context;
  BOOL warningIssued;
  int minCount;

  if (!s->code->m_node->m_tok) {
    return 0;
  }

  context = walker->context;
  setScopeNesting(context, s->nestedFunction);
  setVariablesAccessed(context, s->variablesAccessed);
  setLocalVariablesList(context, s->returnVals);

  if (workspace) {
    vstr_t workspaceVars(workspace->getCompletions(""));

    for (i = 0; i < workspaceVars->n; i++) {
      context->insertVariableLocally(workspaceVars[i],
          *workspace->findSymbol(workspaceVars[i]));
    }
  }

  // When the function is called, the number of inputs is
  // at sometimes less than the number of s->arguments requested.
  // Check the argument count.  If this is a non-varargin
  // argument function, then use the following logic:
  minCount = 0;

  if (inputArgCount() != -1) {
    minCount = (((int)inputs->n) < s->arguments->n) ?
        inputs->n : s->arguments->n;

    for (i = 0; i < minCount; i++) {
      QString arg(s->arguments[i]);

      if (arg[0] == '&') {
        arg.remove(0, 1);
      }

      context->insertVariableLocally(arg, inputs[i]);
    }

    context->setScopeNargin(minCount);
  }
  else {
    // Count the number of supplied s->arguments
    int inputCount = inputs->n;
    context->setScopeNargin(inputCount);
    // Get the number of explicit s->arguments
    int explicitCount = s->arguments->n - 1;
    // For each explicit argument (that we have an input for),
    // insert it into the scope.
    minCount = (explicitCount < inputCount) ? explicitCount : inputCount;
    int i;

    for (i = 0; i < minCount; i++) {
      QString arg(s->arguments[i]);

      if (arg[0] == '&') {
        arg.remove(0, 1);
      }

      context->insertVariableLocally(arg, inputs[i]);
    }

    inputCount -= minCount;
    // Put minCount...inputCount
    ArrayVector varargin(inputs.mid(minCount, inputCount));
    context->insertVariableLocally(QString("varargin"),
        CellArrayFromArrayVector(varargin, varargin->n));
  }

  context->setScopeNargout(nargout);

  try {
    int ret = walker->block(code);
    switch (ret) {
    case InterpreterBreakException:
      break;
    case InterpreterContinueException:
      break;
    case InterpreterReturnException:
      break;
    }

    warningIssued = FALSE;

    if (outputArgCount() != -1) {
      // special case - if nargout == 0, and none of the
      // outputs are predefined, we don't do anything
      BOOL nonpredefed = TRUE;

      for (i = 0; i < s->returnVals->n && nonpredefed; i++) {
        mxArray* ptr = context->lookupVariableLocally(s->returnVals[i]);
        nonpredefed = nonpredefed && (!ptr);
      }

      if ((nargout > 0) ||
          ((nargout == 0) && (!nonpredefed))) {
        outputs = ArrayVector();

        for (i = 0; i < s->returnVals->n; i++) {
          outputs.push_back(mxArray());
        }

        //  outputs = ArrayVector(s->returnVals->n);
        for (i = 0; i < s->returnVals->n; i++) {
          mxArray* ptr = context->lookupVariableLocally(s->returnVals[i]);

          if (!ptr) {
            outputs[i] = EmptyConstructor();
          }
          else {
            outputs[i] = *ptr;
          }

          if (!ptr && (i < ((int)nargout)))
            if (!warningIssued) {
              walker->warningMessage("one or more outputs not assigned in call (1)");
              warningIssued = TRUE;
            }
        }
      }
    }
    else {
      outputs = ArrayVector();

      for (i = 0; i < nargout; i++) {
        outputs.push_back(mxArray());
      }

      int explicitCount = s->returnVals->n - 1;

      // For each explicit argument (that we have), insert it
      // into the scope.
      for (i = 0; i < explicitCount; i++) {
        mxArray* ptr = context->lookupVariableLocally(s->returnVals[i]);

        if (!ptr) {
          outputs[i] = EmptyConstructor();
        }
        else {
          outputs[i] = *ptr;
        }

        if (!ptr  && (i < nargout))
          if (!warningIssued) {
            walker->warningMessage("one or more outputs not assigned in call (2)");
            warningIssued = TRUE;
          }
      }

      // Are there any outputs not yet filled?
      if (nargout > explicitCount) {
        mxArray varargout, *ptr;
        // Yes, get a pointer to the "vargout" variable that should be defined
        ptr = context->lookupVariableLocally("varargout");

        if (!ptr) {
          throw_Exception("The special variable 'varargout' was not defined as expected");
        }

        varargout = *ptr;

        if (varargout->classID != CellArray) {
          throw_Exception("The special variable 'varargout' was not defined as a cell-array");
        }

        // Get the data pointer
        const BasicArray<mxArray>& dp(varargout.constReal<mxArray>());
        // Get the length
        int varlen = int(varargout.length());
        int toFill = nargout - explicitCount;

        if (toFill > varlen) {
          throw_Exception("Not enough outputs in varargout to satisfy call");
        }

        for (i = 0; i < toFill; i++) {
          outputs[explicitCount + i] = dp[index_t(i + 1)];
        }
      }

      // Special case - nargout = 0, only variable outputs from function
      if ((nargout == 0) && (explicitCount == 0)) {
        mxArray varargout, *ptr;
        // Yes, get a pointer to the "vargout" variable that should be defined
        ptr = context->lookupVariableLocally("varargout");

        if (ptr) {
          varargout = *ptr;

          if (varargout->classID != CellArray) {
            throw_Exception("The special variable 'varargout' was not defined as a cell-array");
          }

          if (varargout.length() > 0) {
            outputs << varargout.constReal<mxArray>().get(index_t(1));
          }
        }
      }
    }

    // Check for s->arguments that were passed by reference, and
    // update their values.
    for (i = 0; i < minCount; i++) {
      QString arg(s->arguments[i]);

      if (arg[0] == '&') {
        arg.remove(0, 1);
      }

      mxArray* ptr = context->lookupVariableLocally(arg);

      if (ptr) {
        inputs[i] = *ptr;
      }
    }

    // Check the outputs for function pointers
    CaptureFunctionPointers(outputs, walker, this);

    if (workspace) {
      vstr_t workspaceVars(workspace->getCompletions(""));

      for (i = 0; i < workspaceVars->n; i++) {
        mxArray* ptr = context->lookupVariableLocally(workspaceVars[i]);
        workspace->insertSymbol(workspaceVars[i], *ptr);
      }
    }

    //    walker->popScope();
    return outputs;
  }
  catch (Exception& e) {
    if (workspace) {
      vstr_t workspaceVars(workspace->getCompletions(""));

      for (i = 0; i < workspaceVars->n; i++) {
        mxArray* ptr = context->lookupVariableLocally(workspaceVars[i]);
        workspace->insertSymbol(workspaceVars[i], *ptr);
      }
    }

    throw;
  }
  catch (InterpreterRetallException& e) {
    if (workspace) {
      vstr_t workspaceVars(workspace->getCompletions(""));

      for (i = 0; i < workspaceVars->n; i++) {
        mxArray* ptr = context->lookupVariableLocally(workspaceVars[i]);
        workspace->insertSymbol(workspaceVars[i], *ptr);
      }
    }

    throw;
  }
#endif
}

static int ReadFileIntoString(const char* filename, str_t* txt)
{
  str_load(filename, txt);
  return 0;
}

//MFunctionDef* ConvertParseTreeToMFunctionDef(TREE t, string fileName) {
//  MFunctionDef *fp = new MFunctionDef;
//  fp->s->returnVals = IdentifierList(t.first());
//  fp->name = t.second().text();
//  fp->s->arguments = IdentifierList(t->m_children(2));
//  fp->code = t->m_children(3);
//  fp->fileName = fileName;
//  return fp;
//}
//
//MFunctionDef* ConvertParseTreeToMFunctionDefs(treeVector t,
//                string fileName) {
//  MFunctionDef* last = NULL;
//  for (index = t->n-1;index>=0;index--) {
//    MFunctionDef* rp = ConvertParseTreeToMFunctionDef(t[index],fileName);
//    if (index>0)
//      rp->localFunction = TRUE;
//    else
//      rp->localFunction = FALSE;
//    rp->nextFunction = last;
//    if (last)
//      last->prevFunction = rp;
//    last = rp;
//  }
//  return last;
//}

#if 0
static int RegisterNested(const tree_t* t, interpreter_t* eval, fun_t* parent)
{
  int i;
  if ((t->m_node->m_tok == TOK_NEST_FUNC)) {
    fun_t* fp;
    fp->localFunction = parent->localFunction;
    fp->nestedFunction = TRUE;
    fp->s->returnVals = IdentifierList(t.first());
    fp->name = parent->name + "/" + t.second().text();
    fp->s->arguments = IdentifierList(t->m_children(2));
    fp->code = t->m_children(3);
    VariableReferencesList(fp->code, fp->variablesAccessed);
    fp->fileName = parent->fileName;
    // Register any nested functions for the local functions
    eval->getContext()->insertFunction(fp, FALSE);
    RegisterNested(fp->code, eval, fp);
  }
  else {
    for (i = 0; i < t->m_childrenlen; i++) {
      RegisterNested(t->m_children(i), eval, parent);
    }
  }
}

static tree_t ParseMFile(QString fileName)
{
  // Read the file into a string
  QString fileText = ReadFileIntoString(fileName);

  if (!fileName.contains(QDir::separator() + QString("+octave") + QDir::separator())) {
    Scanner S(fileText, fileName);
    Parser P(S);
    return P.process();
  }
  else {
    OctaveScanner S(fileText, fileName);
    Parser P(S, TRUE);
    tree_t t = P.process();
    t.print();
    return t;
  }
}

// Compile the function...
BOOL MFunctionDef_updateCode(interpreter_t* eval)
{
  if (localFunction) {
    return FALSE;
  }

  if (pcodeFunction) {
    return FALSE;
  }

  if (nestedFunction) {
    return FALSE;
  }

  // First, stat the file to get its time stamp
  QFileInfo filestat(fileName);

  if (!functionCompiled || (filestat.lastModified() != timeStamp)) {
    // Record the time stamp
    timeStamp = filestat.lastModified();
    QFile fp(fileName);

    if (!fp.open(QIODevice::ReadOnly)) {
      throw_Exception(QString("Unable to open file :") + fileName);
    }

    BOOL commentsOnly = TRUE;
    helpText.clear();
    QTextStream io(&fp);
    QString cp;

    while (!io.atEnd() && commentsOnly) {
      cp = io.readLine();

      while ((cp->n > 1) && (cp.at(0).isSpace())) {
        cp.remove(0, 1);
      }

      if (cp == "\n" || cp.isEmpty()) {
        continue;
      }

      if (cp.at(0) != QChar('%')) {
        commentsOnly = FALSE;
      }
      else {
        if ((cp->n > 1) && (!cp.endsWith(QChar('\n')))) {
          helpText.push_back(cp + "\n");
        }
        else {
          helpText.push_back(cp);
        }
      }
    }

    if (helpText->n == 0) {
      helpText.push_back(cp);
    }

    try {
      const tree_t* pcode(ParseMFile(fileName));

      if (pcode->m_node->m_tok == TOK_FUNCTION_DEFS)) {
        scriptFlag = FALSE;
        // Get the main function..
        const tree_t* MainFuncCode = pcode.first();
        s->returnVals = IdentifierList(MainFuncCode.first());
        // The name is mangled by the interpreter...  We ignore the
        // name as parsed in the function.
        //  name = MainFuncCode.second().text();
        s->arguments = IdentifierList(MainFuncCode->m_children(2));
        code = MainFuncCode->m_children(3);
        VariableReferencesList(code, variablesAccessed);
        // Register any nested functions
        RegisterNested(code, eval, this);
        localFunction = FALSE;

        // Process the local functions
        for (index = 1; index < pcode->m_childrenlen; index++) {
          const tree_t* LocalFuncCode = pcode->m_children(index);
          MFunctionDef* fp = new MFunctionDef;
          fp->localFunction = TRUE;
          fp->s->returnVals = IdentifierList(LocalFuncCode.first());
          fp->name = name + "/" + LocalFuncCode.second().text();
          fp->s->arguments = IdentifierList(LocalFuncCode->m_children(2));
          fp->code = LocalFuncCode->m_children(3);
          VariableReferencesList(fp->code, fp->variablesAccessed);
          fp->fileName = fileName;
          // Register any nested functions for the local functions
          // local functions are not marked as temporary.  This yields
          // clutter in the name space, but solves the troublesome
          // issue of local functions being flushed by the CD command.
          eval->getContext()->insertFunction(fp, FALSE);
          RegisterNested(fp->code, eval, this);
        }

        functionCompiled = TRUE;
      }
      else {
        scriptFlag = TRUE;
        functionCompiled = TRUE;
        code = pcode.first();
      }
    }
    catch (Exception& e) {
      functionCompiled = FALSE;
      throw;
    }

    return TRUE;
  }

  return FALSE;
}

static BOOL StatementTypeNode(const tree_t* t)
{
  return (t.is('=') || (t->m_node->m_tok == TOK_MULTI) || (t->m_node->m_tok == TOK_SPECIAL) ||
      (t->m_node->m_tok == TOK_FOR) || (t->m_node->m_tok == TOK_WHILE) || (t->m_node->m_tok == TOK_IF) ||
      (t->m_node->m_tok == TOK_BREAK) || (t->m_node->m_tok == TOK_CONTINUE) || (t->m_node->m_tok == TOK_DBSTEP) ||
      (t->m_node->m_tok == TOK_RETURN) || (t->m_node->m_tok == TOK_SWITCH) || (t->m_node->m_tok == TOK_TRY) ||
      (t->m_node->m_tok == TOK_QUIT) || (t->m_node->m_tok == TOK_RETALL) || (t->m_node->m_tok == TOK_KEYBOARD) ||
      (t->m_node->m_tok == TOK_GLOBAL) || (t->m_node->m_tok == TOK_PERSISTENT) || (t->m_node->m_tok == TOK_EXPR));
}

// Find the smallest line number larger than the argument
// if our line number is larger than the target, then we
//
static int TreeLine(const tree_t* t, unsigned& bestLine, unsigned lineTarget)
{
  if (!t.valid()) {
    return;
  }

  // Nested functions are tracked separately - so that we do not
  // check them for line numbers
  if ((t->m_node->m_tok == TOK_NEST_FUNC)) {
    return;
  }

  if (StatementTypeNode(t)) {
    unsigned myLine = (LineNumber(t.context()));

    if ((myLine >= lineTarget) && (myLine < bestLine)) {
      bestLine = myLine;
    }
  }

  for (i = 0; i < t->m_childrenlen; i++) {
    TreeLine(t->m_children(i), bestLine, lineTarget);
  }
}


// Find the closest line number to the requested
unsigned MFunctionDef_ClosestLine(unsigned line)
{
  unsigned bestline;
  bestline = 1000000000;
  TreeLine(code, bestline, line);

  if (bestline == 1000000000)
    throw_Exception(QString("Unable to find a line close to ") +
        QString::number(line) +
        QString(" in routine ") + name);

  return bestline;
}


BuiltInFunctionDef::BuiltInFunctionDef()
{
}

BuiltInFunctionDef::~BuiltInFunctionDef()
{
}

int BuiltInFunctionDef::inputArgCount()
{
  return argCount;
}

int BuiltInFunctionDef::outputArgCount()
{
  return retCount;
}

int BuiltInFunctionDef::printMe(interpreter_t* eval)
{
  interpreter_outputMessage(eval, " Function name:" + name);
  interpreter_outputMessage(eval, " Function class: Built in\n");
  interpreter_outputMessage(eval, QString(" Return count: %1\n").arg(retCount));
  interpreter_outputMessage(eval, QString(" Argument count: %1\n").arg(argCount));
}


ArrayVector BuiltInFunctionDef::evaluateFunc(interpreter_t* walker,
    ArrayVector& inputs,
    int nargout,
    VariableTable*)
{
  return fptr(nargout, inputs);
}

SpecialFunctionDef::SpecialFunctionDef()
{
}

SpecialFunctionDef::~SpecialFunctionDef()
{
}

ArrayVector SpecialFunctionDef::evaluateFunc(interpreter_t* walker,
    ArrayVector& inputs,
    int nargout,
    VariableTable*)
{
  return fptr(nargout, inputs, walker);
}

int SpecialFunctionDef::printMe(interpreter_t* eval)
{
}

FunctionDef::FunctionDef()
{
  scriptFlag = FALSE;
  graphicsFunction = FALSE;
  temporaryFlag = FALSE;
  refcount = 0;
}

int FunctionDef::lock()
{
  QMutexLocker lockit(&functiondefmutex);
  refcount++;
}

int FunctionDef::unlock()
{
  QMutexLocker lockit(&functiondefmutex);
  refcount--;
}

BOOL FunctionDef::referenced()
{
  QMutexLocker lockit(&functiondefmutex);
  return (refcount > 0);
}

FunctionDef::~FunctionDef()
{
}

BOOL ImportedFunctionDef::isPassedAsPointer(int arg)
{
  return (s->arguments[arg].startsWith("&") || (types[arg] == "string")
      || sizeCheckExpressions[arg].valid());
}

static DataClass mapTypeNameToClass(QString name)
{
  if (name == "uint8") {
    return UInt8;
  }

  if (name == "int8") {
    return Int8;
  }

  if (name == "uint16") {
    return UInt16;
  }

  if (name == "int16") {
    return Int16;
  }

  if (name == "uint32") {
    return UInt32;
  }

  if (name == "int32") {
    return Int32;
  }

  if (name == "uint64") {
    return UInt64;
  }

  if (name == "int64") {
    return Int64;
  }

  if (name == "float") {
    return Float;
  }

  if (name == "double") {
    return Double;
  }

  if (name == "string") {
    return StringArray;
  }

  if (name == "int") {
    return Int32;
  }

  throw_Exception("unrecognized type " + name + " in imported function setup");
}

static ffi_type* mapTypeNameToFFIType(QString name)
{
  if (name == "uint8") {
    return &ffi_type_uint8;
  }

  if (name == "int8") {
    return &ffi_type_sint8;
  }

  if (name == "uint16") {
    return &ffi_type_uint16;
  }

  if (name == "int16") {
    return &ffi_type_sint16;
  }

  if (name == "uint32") {
    return &ffi_type_uint32;
  }

  if (name == "int32") {
    return &ffi_type_sint32;
  }

  if (name == "uint64") {
    return &ffi_type_uint64;
  }

  if (name == "int64") {
    return &ffi_type_sint64;
  }

  if (name == "float") {
    return &ffi_type_float;
  }

  if (name == "double") {
    return &ffi_type_double;
  }

  if (name == "string") {
    return &ffi_type_pointer;
  }

  if (name == "int") {
    return &ffi_type_void;
  }

  throw_Exception("unrecognized type " + name + " in imported function setup");
}

static int importCounter = 0;
    ImportedFunctionDef::ImportedFunctionDef(GenericFuncPointer address_arg,
        vstr_t types_arg,
        vstr_t arguments_arg,
        TreeList sizeChecks,
        QString retType_arg,
        QString name)
{
  address = address_arg;
  types = types_arg;
  s->arguments = arguments_arg;
  sizeCheckExpressions = sizeChecks;
  retType = retType_arg;
  argCount = types_arg->n;

  if (retType == "int") {
    retCount = 0;
  }
  else {
    retCount = 1;
  }

  // Build the ffi cif (call interface object)
  ffi_type** args;
  args = (ffi_type**) malloc(sizeof(ffi_type*)*argCount);

  for (i = 0; i < argCount; i++) {
    if (s->arguments[i][0] == '&' || types[i] == "string" ||
    sizeCheckExpressions[i].valid()) {
      args[i] = &ffi_type_pointer;
    }
    else {
      args[i] = mapTypeNameToFFIType(types[i]);
    }
  }

  if (ffi_prep_cif(&cif, FFI_DEFAULT_ABI, argCount,
  mapTypeNameToFFIType(retType), args) != FFI_OK) {
    throw_Exception("unable to import function through ffi!");
  }
}

ImportedFunctionDef::~ImportedFunctionDef()
{
}

int ImportedFunctionDef::printMe(interpreter_t*)
{
}


static QString TrimAmpersand(QString name)
{
  if (!name.startsWith("&")) {
    return name;
  }

  name.remove(0, 1);
  return name;
}

template <class T>
static mxArray DoCIFCall(ffi_cif cif, GenericFuncPointer address,
    vector<int*>& values, DataClass rclass)
{
  T retval;

  if (values->n > 0) {
    ffi_call(&cif, address, &retval, &values[0]);
  }
  else {
    ffi_call(&cif, address, &retval, NULL);
  }

  return mxArray::create(retval);
}

/**
 * Note: Pass-by-reference only really matters for strings, and that
 * is only because for strings, we convert them from Unicode to C strings
 * when passing down, and only bother converting them back if they
 * were passed by reference.
 */


ArrayVector ImportedFunctionDef::evaluateFunc(interpreter_t* walker,
    ArrayVector& inputs,
    int nargout,
    VariableTable*)
{
  /**
   * To actually evaluate the function, we have to process each of
   * the s->arguments and get them into the right form.
   */
  int i;

  for (i = 0; i < inputs->n; i++) {
    inputs[i] = inputs[i].asDenseArray().toClass(mapTypeNameToClass(types[i]));
  }

  /**
   * Next, we count how many of the inputs are to be passed by
   * reference.
   */
  int passByReference = 0;

  for (j = 0; j < inputs->n; j++)
    if ((s->arguments[j][0] == '&') || (types[j] == "string") ||
    (sizeCheckExpressions[j].valid())) {
      passByReference++;
    }

  /**
   * Strings are converted to C strings and stored here
   */
  std::vector<char*> string_store;

  for (i = 0; i < inputs->n; i++)
    if (types[i] == "string") {
      string_store.push_back(strdup(qPrintable(inputs[i].asString())));
    }
    else {
      string_store.push_back(NULL);
    }

  /**
   * Next, we check to see if any bounds-checking expressions are
   * active.
   */
  BOOL boundsCheckActive = FALSE;
  int m = 0;

  while (m < inputs->n && !boundsCheckActive) {
    boundsCheckActive = (sizeCheckExpressions[m++].valid());
  }

  if (boundsCheckActive) {
    /**
     * If the bounds-checking is active, we have to create a
     * new context, and insert the defined s->arguments into the
     * context (much as for an M-function call).
     */
    context_t* context = walker->getContext();
    context->pushScope("bounds_check", name);

    for (i = 0; i < inputs->n; i++) {
      context->insertVariableLocally(TrimAmpersand(s->arguments[i]), inputs[i]);
    }

    /*
     * Next, evaluate each size check expression
     */
    for (i = 0; i < inputs->n; i++) {
      if (sizeCheckExpressions[i].valid()) {
        mxArray ret(walker->expression(sizeCheckExpressions[i]));
        ret = ret.toClass(Int32);
        int len = ret.asInteger();

        if (len != (int)(inputs[i].length())) {
          throw_Exception("array input " + TrimAmpersand(s->arguments[i]) +
              " length different from computed bounds" +
              " check length");
        }
      }
    }

    context->popScope();
  }

  /**
   * Allocate an array of pointers to store for variables passed
   * by reference.
   */
  vector<int*> refPointers;
  refPointers.resize(passByReference);
  /**
   * Allocate an array of value pointers...
   */
  vector<int*> values;
  values.resize(inputs->n);
  int ptr = 0;

  for (i = 0; i < inputs->n; i++) {
    if (types[i] != "string") {
      if ((s->arguments[i][0] == '&') || (sizeCheckExpressions[i].valid())) {
        refPointers[ptr] = inputs[i].getVoidPointer();
        values[i] = &refPointers[ptr];
        ptr++;
      }
      else {
        values[i] = inputs[i].getVoidPointer();
      }
    }
    else {
      refPointers[ptr] = string_store[i];
      values[i] = &refPointers[ptr];
      ptr++;
    }
  }

  mxArray retArray;

  if (retType == "uint8") {
    retArray = DoCIFCall<uint8>(cif, address, values, UInt8);
  }
  else if (retType == "int8") {
    retArray = DoCIFCall<int8>(cif, address, values, Int8);
  }
  else if (retType == "uint16") {
    retArray = DoCIFCall<uint16>(cif, address, values, UInt16);
  }
  else if (retType == "int16") {
    retArray = DoCIFCall<int16>(cif, address, values, Int16);
  }
  else if (retType == "uint32") {
    retArray = DoCIFCall<uint32>(cif, address, values, UInt32);
  }
  else if (retType == "int32") {
    retArray = DoCIFCall<int32>(cif, address, values, Int32);
  }
  else if (retType == "uint64") {
    retArray = DoCIFCall<uint64>(cif, address, values, UInt64);
  }
  else if (retType == "int64") {
    retArray = DoCIFCall<int64>(cif, address, values, Int64);
  }
  else if (retType == "float") {
    retArray = DoCIFCall<float>(cif, address, values, Float);
  }
  else if (retType == "double") {
    retArray = DoCIFCall<double>(cif, address, values, Double);
  }
  else if (retType == "int") {
    DoCIFCall<int32>(cif, address, values, Int32);
    retArray = EmptyConstructor();
  }
  else {
    throw_Exception("Unsupported return tye " + retType + " in imported function call");
  }

  // Strings that were passed by reference have to be
  // special-cased
  for (i = 0; i < inputs->n; i++) {
    if (s->arguments[i].startsWith("&") && (types[i] == "string")) {
      inputs[i] = mxArray(QString(string_store[i]));
    }
  }

  for (i = 0; i < inputs->n; i++)
    if (string_store[i]) {
      free(string_store[i]);
    }

  return ArrayVector(retArray);
}

MexFunctionDef::MexFunctionDef(QString fullpathname)
{
  fullname = fullpathname;
  importSuccess = FALSE;
  lib = new DynLib(fullname);

  try {
    address = (mexFuncPtr) lib->GetSymbol("mexFunction");
    importSuccess = TRUE;
  }
  catch (Exception& e) {
  }
}

BOOL MexFunctionDef::LoadSuccessful()
{
  return importSuccess;
}

MexFunctionDef::~MexFunctionDef()
{
}

int MexFunctionDef::printMe(interpreter_t*)
{
}

ArrayVector MexFunctionDef::evaluateFunc(interpreter_t* walker,
    ArrayVector& inputs,
    int nargout,
    VariableTable*)
{
  // Convert s->arguments to mxArray
  mxArray** args = new mxArray*[inputs->n];

  for (i = 0; i < inputs->n; i++) {
    args[i] = MexArrayFromArray(inputs[i]);
  }

  // Allocate output array vector
  int lhsCount = nargout;
  lhsCount = (lhsCount < 1) ? 1 : lhsCount;
  mxArray** rets = new mxArray*[lhsCount];

  try {
    address(lhsCount, rets, inputs->n, (const mxArray**)args);
  }
  catch (QString& e) {
    throw_Exception(e);
  }

  ArrayVector retvec;

  for (i = 0; i < lhsCount; i++) {
    retvec.push_back(ArrayFromMexArray(rets[i]));
    mxDestroyArray(rets[i]);
  }

  delete[] rets;
  return retvec;
}

#endif
