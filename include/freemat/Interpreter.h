
enum {
#define ERRCODEDEF(a, b) a,
#include "errcodedef.txt"
#undef ERRCODEDEF
};

typedef enum JITControlFlag {
  JITOff = 0,
  JITOn = 1,
  JITTrace = 2
} JITControlFlag;

typedef struct stackentry {
  char cname[32];
  char detail[32];
  int tokid;
  int number;
  int steptrap;
  int stepcurrentline;
  int contextnumber;
} stackentry;

typedef int (*BinaryFunc)(const mxArray* a, const mxArray* b, mxArray* c);
typedef int (*UnaryFunc)(const mxArray* a, mxArray* b);

/**
 * This is the class that implements the interpreter - it generally
 * operates on abstract syntax trees (ASTs).
*/
typedef struct interpreter_t {
  /******************************************
   *  Class Members for the interpreter_t     *
   ******************************************/

  /**
   * Members related to "end" references.
   */
  ArrayReference endRef[1];
  int endTotal;
  int endCount;

  /**
   * Debugger related values
   */
  int tracetrap;
  int tracecurrentline;
  BOOL dbdown_executed;
  /**
   * Tracks the number of errors that have occured (used by the documentation
   * compiler helpgen)
   */
  int errorCount;
  /**
   * The context that the intepreter operates in.
   */
  context_t* context;
  /**
   * The debug depth.  Each time the command line interface is invoked
   * via a keyboard command, the debug depth is increased.
   */
  int depth;
  /**
   * The print limit (how many elements get printed before output
   * stops).
   */
  int printLimit;
  /**
   * The last error that occured.
   */
  char lasterr[256];
  /**
   * autostop storage flag
   */
  BOOL autostop;
  /**
   * in try block flag
   */
  BOOL intryblock;
  /**
   * jit control flag
   */
  JITControlFlag jitcontrol;
  /**
   * jit counter - increments each time we successfully jit something
   */
  int jitcount;
  /**
   * jit instance for this interpreter
   */
  //JIT *m_jit;
  /**
   * When this flag is active, autostop does nothing.
   */
  BOOL InCLI;
  /**
   * Disable rescan of the path.
   */
  BOOL m_disablerescan;
  /**
   * This mutex protects the command buffer.
   */
  //QMutex mutex;
  /**
   * The command buffer
   */
  strv_t cmd_buffer[1];
  /**
   * A buffer of return values from graphics calls
   */
  ArrayVector* gfx_buffer;
  int n_gfx_buffer;
  /**
   * A flag to indicate that the gfx call failed
   */
  BOOL gfxErrorOccured;
  /**
   * The corresponding exception.
   */
  char gfxError[256];
  /**
   * A synchronization variable to wait on when the command buffer is empty
   */
  //QWaitCondition bufferNotEmpty;
  /**
   * A synchronization variable to wait on when the gfx buffer is empty
   */
  //QWaitCondition gfxBufferNotEmpty;
  /**
   * A list of breakpoints
   */
  stackentry* bpStack;
  int n_bpStack;
  /**
   * True if the interpreter is in single-step mode
   */
  BOOL inStepMode;
  /**
   * Set this flag to TRUE to stop overloading of methods
   */
  BOOL stopoverload;
  /**
   * The base path - contains all of the directories for FreeMat-shipped code
   */
  strv_t m_basePath[1];
  /**
   * The user path - contains all of the directories for user defined FreeMat code
   */
  strv_t m_userPath[1];
  /**
   * The width of the output in characters
   */
  int m_ncols;
  /**
   * This flag controls the greeting
   */
  BOOL m_skipflag;
  /**
   * Setting this flag suppresses the prompt
   */
  BOOL m_noprompt;
  /**
   * Our thread ID
   */
  int m_threadID;
  /**
   * The thread function we are executing
   */
  fun_t* m_threadFunc;
  /**
   * The arguments to the thread function we are executing
   */
  ArrayVector m_threadFuncArgs[1];
  /**
   * The number of return arguments expected
   */
  int m_threadNargout;
  /**
   * The return value of the thread function
   */
  ArrayVector m_threadFuncRets[1];
  /**
   * The error state of the thread function (TRUE if an exception
   * occured).
   */
  BOOL m_threadErrorState;
  /**
   * Interrupt the current thread at the next opportunity
   */
  BOOL m_interrupt;
  /**
  * Kill the thread at the next opportunity
  */
  BOOL m_kill;
  /**
   * The current state of diary output
   */
  BOOL m_diaryState;
  /**
   * The filename for the diary
   */
  char m_diaryFilename[256];
  /**
   * The capture string
   */
  char m_capture[256];
  /**
   * The capture state
   */
  BOOL m_captureState;
  /**
   * The profile state
   */
  BOOL m_profile;
  /**
   * The quiet level
   */
  int m_quietlevel;
  /**
   * Live updates flag -- determines if this interpreter connects
   * to the GUI.
   */
  BOOL m_liveUpdateFlag;
  /**
   * Enable or disable warning messages
   */
  BOOL m_enableWarnings;
  int (*outputRawText)(interpreter_t* s, const char* str);
} interpreter_t;

int interpreter_diaryMessage(interpreter_t* s, const char* format, ...)
{
  FILE* file = fopen(s->m_diaryFilename, "ab");

  if (file) {
    char buffer[4096];
    va_list ap;
    va_start(ap, format);
    _vsnprintf(buffer, 4096, format, ap);
    va_end(ap);
    fprintf(file, "%s", buffer);
  }
  return 0;
}

const char* TranslateString(const char* x)
{
  //return x.replace("\n","\r\n");
  return x;
}

int interpreter_outputMessage1(interpreter_t* s, const char* msg)
{
  if (s->m_diaryState) {
    interpreter_diaryMessage(s, msg);
  }

  if (s->m_captureState) {
    strcat(s->m_capture, msg);
  }
  else {
    if (s->m_quietlevel < 2) {
      s->outputRawText(s, TranslateString(msg));
    }
  }
  return 0;
}

int interpreter_outputMessage(interpreter_t* s, const char* format, ...)
{
  char buffer[4096];
  va_list ap;
  va_start(ap, format);
  _vsnprintf(buffer, 4096, format, ap);
  va_end(ap);
  interpreter_outputMessage1(s, buffer);
  return 0;
}

int interpreter_errorMessage(interpreter_t* s, const char* msg)
{
  char buf[256];
  _snprintf(buf, 256, "Error: %s\n", msg);

  if (s->m_diaryState) {
    interpreter_diaryMessage(s, buf);
  }

  if (s->m_captureState) {
    strcat(s->m_capture, buf);
  }
  else {
    if (s->m_quietlevel < 2) {
      s->outputRawText(s, TranslateString(buf));
    }
  }
  return 0;
}

int interpreter_warningMessage1(interpreter_t* s, const char* msg)
{
  static const char* lastWarning;
  static BOOL lastWarningRepeat = FALSE;
  char buf[256];
  _snprintf(buf, 256, "Warning: %s\n", msg);

  if (!s->m_enableWarnings) {
    return 0;
  }

  if (s->m_diaryState) {
    interpreter_diaryMessage(s, buf);
  }

  if (s->m_captureState) {
    strcat(s->m_capture, buf);
  }
  else {
    if (s->m_quietlevel < 2) {
      if (lastWarning != msg) {
        s->outputRawText(s, TranslateString(buf));
        lastWarningRepeat = FALSE;
        lastWarning = msg;
      }
      else {
        if (!lastWarningRepeat) {
          s->outputRawText(s, TranslateString("Warning: Last warning repeats... suppressing more of these\r\n"));
          lastWarningRepeat = TRUE;
        }
      }
    }
  }
  return 0;
}

int interpreter_warningMessage(interpreter_t* s, const char* format, ...)
{
  char buffer[4096];
  va_list ap;
  va_start(ap, format);
  _vsnprintf(buffer, 4096, format, ap);
  va_end(ap);
  interpreter_warningMessage1(s, buffer);
  return 0;
}

int interpreter_doFunction(interpreter_t* s, fun_t* f, const mxArray* m, int narg_in, int narg_out, const VariableTable* vtable, mxArray* ret)
{
  //PopContext saver(context, 0);
  pushScope(s->context, f->name, f->detailed, FALSE);
#if 0
  f->evaluateFunc(s, m, narg_out, vtable);
  
  if (context->scopeStepTrap() >= 1) {
    tracetrap = 1;
    tracecurrentline = 0;
    interpreter_warningMessage("dbstep beyond end of function " + context->scopeDetailString() +
      " -- setting single step mode\n");
    context->setScopeStepTrap(0);
  }
#endif
  
  return 0;
}

/******************************************
 *  Public Methods for the interpreter_t    *
******************************************/
#if 0
/**
 * Construct a interpreter_t object with the given context to operate in.
 */
interpreter_t(context_t* aContext);
/**
 * Destruct the interpreter_t object.
 */
~interpreter_t();
/**
 * Set the thread ID to the given number...
 */
inline void setThreadID(int threadID)
{
  m_threadID = threadID;
}
/**
 * Return the thread ID
 */
inline int getThreadID() const
{
  return m_threadID;
}
/**
 * Set the thread function and the arguments
 */
inline void setThreadFunc(FunctionDef* threadFunc,
    int nargout,
    ArrayVector threadFuncArgs)
{
  m_threadFunc = threadFunc;
  m_threadFuncArgs = threadFuncArgs;
  m_threadNargout = nargout;
  m_threadErrorState = FALSE;
}
/**
 * Manipulate the quiet level
 */
inline int getQuietLevel()
{
  return m_quietlevel;
}
inline void setQuietLevel(int t)
{
  m_quietlevel = t;
}
/**
 * Manipulate the try/catch state
 */
inline BOOL getTryCatchActive()
{
  return intryblock;
}
inline void setTryCatchActive(BOOL t)
{
  intryblock = t;
}
/**
 * Manipulate the diary state
 */
inline BOOL getDiaryState()
{
  return m_diaryState;
}
inline void setDiaryState(BOOL t)
{
  m_diaryState = t;
}
inline void setDiaryFilename(QString name)
{
  m_diaryFilename = name;
}
void diaryMessage(QString msg);
/**
 * Manipulate the warning enable state
 */
inline BOOL getEnableWarnings()
{
  return m_enableWarnings;
}
inline void setEnableWarnings(BOOL t)
{
  m_enableWarnings = t;
}
/**
 * Manipulate the capture state
 */
inline void clearCaptureString()
{
  m_capture = "";
}
inline void setCaptureState(BOOL t)
{
  m_captureState = t;
}
inline QString getCaptureString()
{
  return m_capture;
}
/**
 * Manipulate the profile state
 */
inline void setProfileState(BOOL t)
{
  m_profile = t;
}
inline BOOL profileState()
{
  return m_profile;
}
/**
 * Manipulate the live update flag
 */
void setLiveUpdateFlag(BOOL t);
inline BOOL liveUpdateFlag()
{
  return m_liveUpdateFlag;
}
/**
 * Get the result of the thread function evaluation
 */
inline ArrayVector getThreadFuncReturn()
{
  return m_threadFuncRets;
}
/**
 * Get the error state - is TRUE if the thread function throws
 * an exception.
 */
inline BOOL getLastErrorState()
{
  return m_threadErrorState;
}
/**
 * Activate the interrupt for the thread - so that it stops.
 */
inline void setInterrupt()
{
  m_interrupt = TRUE;
}
/**
 * Retrieve the interrupt state for the thread
 */
inline BOOL interrupt()
{
  return m_interrupt;
}
/**
* Activate the kill for the thread - so that it dies.
*/
inline void setKill()
{
  m_kill = TRUE;
}
/**
 * Run a command-line-interface interface from this thread.
 */
void doCLI();
/**
 * Queue a command for execution
 */
void ExecuteLine(QString txt);
/**
 * Retrieve data about the current location of the instruction pointer
 */
QString getMFileName();
QString getInstructionPointerFileName();
/**
 * SET/GET for the instruction pointer
 */
BOOL inMFile() const;
void dbup();
void dbdown();
void debugDump();
/**
 * SET/GET for the in CLI flag
 */
inline void setInCLI(BOOL p)
{
  InCLI = p;
}
inline BOOL inCLI() const
{
  return InCLI;
}
/**
 * Sample the instruction pointer
 */
inline QString sampleInstructionPointer(unsigned& ctxt) const
{
  if (!InCLI) {
    return context->sampleIP(ctxt);
  }
  else {
    ctxt = 0;
    return "CLI";
  }
}
/**
 * The Base Path is the one that contains .m files in the current app bundle
 */
void setBasePath(QStringList);
/**
 * The User Path is the one that the user can tinker with.
 */
void setUserPath(QStringList);
/**
 * Get the current path set for the interface. (user path - legacy interface)
 */
QString getPath();
/**
 * Get the current path set for the interface (base path + user path)
 */
QString getTotalPath();
/**
 * Set the path for the interface. (user path - legacy interface)
 */
void setPath(QString);
/**
 * Return the width of the current "terminal" in
 * characters.
 */
int getTerminalWidth();
/**
 * Set the width of the current "terminal" in characters
 */
void setTerminalWidth(int ncols);
/**
 * Output the following text message.
 */
void outputMessage(QString msg);
void outputMessage(const char* format, ...);
/**
 * Output the following error message.
 */
void errorMessage(QString msg);
/**
 * Output the following warning message.
 */
void warningMessage(QString msg);
/**
 * Start the interpreter running.
 */
void run();
/**
 * Get/Set the overload stop flag
 */
BOOL getStopOverload();
void setStopOverload(BOOL flag);
/**
 * Get/Set the disable rescan flag
 */
BOOL getDisableRescan();
void setDisableRescan(BOOL flag);
/**
 * Retrieve the version string for the interpreter
 */
static QString getVersionString();
/**
 * Retrieve the number of errors that occured.  The
 * side effect is that the error count is set to zero.
 */
int getErrorCount();
/**
 * True if we are currently in the method of a class
 */
BOOL inMethodCall(QString classname);
/**
 * Get the context for the interface.
 */
inline context_t* getContext()
{
  return context;
}
/**
 * Refresh the breakpoints in the code.  Call this after updating
 * code for functions, or breakpoints will not work.
 */
void refreshBreakpoints();
/**
 * Add a new breakpoint
 */
MFunctionDef* lookupFullPath(QString name);
void addBreakpoint(stackentry bp);
void addBreakpoint(QString name, int line) ;
/**
 * Activate a breakpoint in the code.  If the line number for the
 * breakpoint is negative, the breakpoint is set as a step trap.
 * If the line number is positive, and the enable flag is set
 * then a breakpoint is set.  If the enable flag is FALSE then
 * the breakpoint is cleared.
 */
void setBreakpoint(stackentry bp, BOOL enableFlag);
/**
 * List all breakpoints
 */
void listBreakpoints();
/**
 * Delete a breakpoint from the persistent list
 */
void deleteBreakpoint(int number);
/**
 * Generate a stacktrace
 */
void stackTrace(int skiplevels = 0);
/**
 * Step the given number of lines
 */
void dbstepStatement(const tree_t* t);
void dbtraceStatement(const tree_t* t);
/**
 * Set the autostop flag - this flag determines what happens when
 * an exception occurs
 */
inline BOOL AutoStop()
{
  return autostop;
}
inline void setAutoStop(BOOL a)
{
  autostop = a;
}
/**
 * Set the JITControl flag
 */
inline JITControlFlag JITControl() const
{
  return jitcontrol;
}
inline void setJITControl(JITControlFlag a)
{
  jitcontrol = a;
}
/**
 * Increment the JIT block counter
 */
inline void incrementJITCounter()
{
  jitcount++;
}
inline int JITCounter() const
{
  return jitcount;
}
/**
 * Set the print limit (number of element printed prior to truncation).
 */
void setPrintLimit(int);
/**
 * Get the print limit (number of element printed prior to truncation).
 */
int getPrintLimit();
/**
 * The workhorse routine - "evaluate" the contents of a string
 * and execute it.  The flag indicates whether or not exceptions
 * are propogated or printed.
 */
void evaluateString(QString line, BOOL propogateExceptions = FALSE);
/**
 * Get the last error that occurred.
 */
QString getLastErrorString();
/**
 * Set the text for the last error.
 */
void setLastErrorString(QString txt);
/**
 * Set to FALSE to turn off the greeting.
 */
void setGreetingFlag(BOOL skip);
/**
 * Set to TRUE to turn off the "-->" prompt.
 */
void setNoPromptFlag(BOOL noprompt);
/**
 * Register the result of a gfx call
 */
void RegisterGfxResults(ArrayVector m);
/**
 * Register an error that occurs with a gfx call
 */
void RegisterGfxError(QString msg);
/**
 * Simplified interface for function lookup.
 */
BOOL lookupFunction(QString funcName, fun_t*& val);
/**
* Prompt the user for input, and return the answer.
*/
QString getLine(QString prompt);
/**
 * Go to sleep for the specified number of milliseconds
 */
void sleepMilliseconds(unsigned long msecs);
/**
 * Executes a sequence of statements, trapping exceptions
 * as necessary.  The AST looks like
 *   <ignored>
 *      |
 *    statement->statement->statement
 * If an exception occurs, it is caught and rethrown.  The
 * lasterr string is also set to the contents of the exception.
 *
 */
void block(const tree_t* t);
/**
 * Start a command line interface.  Statements are retrieved
 * from the console, and executed sequentially until a "return"
 * statement is executed or the user presses 'CTRL-D'.
 */
void evalCLI();

BOOL isBPSet(QString fname, int lineNumber);
BOOL isInstructionPointer(QString fname, int lineNumber);
void toggleBP(QString fname, int lineNumber);
/**
 * Change the current working directory
 */
void changeDir(QString path);
/**
 * Force a rescan of the current path to look for
 * new function files.
 */
void rescanPath();
/**
 * Execute a function.
 * For functions that accesses the graphics subsystem,  All such
 * accesses must be made through the main thread.  So the interpreter
 * sends a signal out that a graphics call needs to be made, and then
 * waits for the result.  For non-graphics functions, the function is
 * called directly.
 */
ArrayVector doFunction(fun_t* f, ArrayVector& m, int narg_out, VariableTable* vtable = 0);
/**
 * Delete a structure array that is part of a handle class
 * where the handles have all been deleted.
 */
void deleteHandleClass(StructArray* ap);
public slots:
/**
 * Send file info to the file tool
 */
void updateVariablesTool();
void updateStackTool();

/******************************************
 *  Signals for the interpreter_t           *
 ******************************************/
signals:

/**
 * Flush the I/O buffers
 */
void Flush();
/**
 * Send info on the current working directory
 */
void updateDirView(QVariant);
/**
 * Send variable info to the variable viewer
 */
void updateVarView(QVariant);
/**
 * Send the workspace info
 */
void updateStackView(QStringList);
/**
 * User has changed the current working directory
 */
void CWDChanged(QString);
/**
 * Change the prompt
 */
void SetPrompt(QString);
/**
 * Dispatch a graphics call
 */
/**
 * All done.
 */
void QuitSignal();
/**
 * Something went wrong...
 */
void CrashedSignal();
/**
 * Refresh the breakpoints
 */
void RefreshBPLists();
/**
 * Show the current active line
 */
void IllegalLineOrCurrentPath(QString name, int line);
/**
 * Inform the editor of illegal line or file not on current path
 */
void ShowActiveLine(QString name, int line);
/**
 * Enable repainting
 */
void EnableRepaint();
/**
 * Disable repainting
 */
void DisableRepaint();
/**
 * Update the stack trace
 */
void UpdateStackTrace(QStringList trace);
/******************************************
 *  Private Methods for the interpreter_t   *
 ******************************************/
private:
/**
 * Setup the file system watcher -- looks for changes to directories or files
 */
void setupWatcher();
/**
 * Collect information about keyword usage and the relevant
 * expressions from a function call
 */
void collectKeywords(const tree_t* q, ArrayVector& keyvals,
    TreeList& keyexpr, StringVector& keywords);
/**
 * Sort keywords into a proper call order.
 */
int* sortKeywords(ArrayVector& m, StringVector& keywords,
    StringVector arguments, ArrayVector keyvals);
/**
 * For values passed by reference, update the caller's variables.
 */
void handlePassByReference(tree_t q, StringVector arguments,
    ArrayVector m, StringVector keywords,
    TreeList keyexpr, int* argTypeMap);
/**
 * Create a variable in the correct scope, and return a reference to it
 */
ArrayReference createVariable(QString name);
/**
 * Perform a binary operator with the given name
 */
mxArray DoBinaryOperator(const tree_t* t, BinaryFunc fnc, QString fname);
/**
 * Perform a unary operator with the given name
 */
mxArray DoUnaryOperator(const tree_t* t, UnaryFunc fnc, QString fname);
/**
 * Handle the construction of a function pointer
 */
mxArray FunctionPointer(const tree_t* args);
/**
 * Convert a matrix definition of the form: [expr1,expr2;expr3;expr4] into
 * a vector of row definitions.  The first row is the vector [expr1,expr2], and
 * the second is the vector [expr3,expr4].  The AST input should look like:
 *  []
 *   |
 *   ;-> ; -> ... -> NULL
 *   |   |
 *   |   rowDef
 *   rowDef
 */
mxArray matrixDefinition(const tree_t* t);
/**
 * Convert a cell defintion of the form: {expr1,expr2;expr3;expr4} into
 * a vector of row definitions.  The first row is the vector {expr1,expr2}, and
 * the second is the vector {expr3,expr4}.  The AST input should look like:
 *  {}
 *   |
 *   ;-> ; -> ... -> NULL
 *   |   |
 *   |   rowDef
 *   rowDef
 */
mxArray cellDefinition(const tree_t* t);
/**
 * Evaluate the expression pointed to by the AST t into a variable.
 */
mxArray expression(const tree_t* t);

ArrayVector handleReindexing(const tree_t* t, const ArrayVector& p);

/**
 * Evaluate the expression into a variable-array
 */
void multiexpr(const tree_t* t, ArrayVector& m, index_t lhsCount = 1, BOOL output_optional = FALSE);

/**
 * Evaluate a unit colon expression.  The AST input should look like:
 *   :
 *   |
 *   expr1->expr2
 *
 * The output is the expression expr1:expr2, which is the vector
 * [expr1,expr1+1,expr1+2,...,expr1+n], where n is the largest
 * integer such that expr1+n <= expr2.
 */
mxArray unitColon(const tree_t* t);
/**
 * Evaluate a double colon expression.  The AST input should look like:
 *   :
 *   |
 *   :->expr3
 *   |
 *   expr1->expr2
 * The output is the expression expr1:expr2:expr3, which is the
 * vector [expr1,expr1+expr2,expr1+2*expr2,...,expr1+n*expr2], where
 * n is the largest integer such that expr1+n*expr2 <= expr3.
 */
mxArray doubleColon(const tree_t* t);
/**
 * Decode references to ":" inside variable dereferences.
 */
mxArray AllColonReference(mxArray v, int index, int count);
/**
 * Handle statements that are simply expressions
 */
void expressionStatement(const tree_t* t, BOOL printIt);
/**
 * The RHS expression is used to represent an rvalue in an
 * assignment statement (or an implicit assignment such as
 * -->2+3).  The form of the AST depends on the head.  If the
 * head identifier is a function name, then the form of the
 * AST is:
 *    ident
 *     |
 *     ()
 *     |
 *     expr1->expr2->...
 * On the other hand, if the identifier represents a variable
 * (variables are checked for first, incidentally), then the AST
 * looks like:
 *    ident
 *     |
 *     ()->{}->.
 *     |   |   |
 *     |   |   field

 *     |   expr3->...
 *     expr1->expr2->...
 * Throws an Exception
 *    - if the identifier cannot be matched to
 *      either a variable or function.
 *    -
 */
ArrayVector rhsExpression(const tree_t* t, int lhsCount = 1);

void assign(ArrayReference r, const tree_t* s, mxArray* data);

void multiassign(ArrayReference r, const tree_t* s, ArrayVector& m);

void deref(mxArray* r, const tree_t* s);

mxArray rhs(const tree_t* t);
/**
 * Look up an identifier as a potential function name, using a
 * rescan if the identifier is not found on the first pass.
 */
BOOL lookupFunction(QString funcName, fun_t*& val,
    ArrayVector& args, BOOL disableOverload = FALSE);
/**
 * Special case the single assignment statement 'A = B' for speed.
 */
void assignment(const tree_t* var, BOOL printIt, mxArray* b);
/**
 * Try to figure out how many outputs there should be to a multifunction
 * call.  In particular, logic is used to figure out what to do about
 * undefined variables.
 */
index_t countLeftHandSides(const tree_t* t);
/**
 * Evaluate a function and return the results of the function as
 * an ArrayVector.  For scripts, the body of the function is
 * evaluated directly.  Otherwise, the function evaluates itself
 * (using the FunctionDef::evaluateFunction member function).  The
 * arguments to the function are unpacked from the AST ast follows
 *   ident
 *    |
 *    ()
 *    |
 *    expr1->expr2->...
 * The outputOptional flag allows the function to not assign an
 * output.
 * Throws an exception if
 *    - something other than a parenthesis pair "()" follows the
 *      identifier.
 *    - if too many arguments are passed to the function.
 *    - too many outputs are requested from the function.
 */
void functionExpression(const tree_t* t, int narg_out, BOOL outputOptional, ArrayVector& output);
/**
 * A multifunction call is an expression of the sort
 * [expr1,expr2,...,exprn] = func(args).  The AST is
 *    multiCall
 *       |
 *       []->functionExpression_AST
 *       |
 *       ;->NULL
 *       |
 *       rowDefs
 * When calculating the number of left hand sides for the
 * multifunction, single variables without subindexing expressions
 * are counted as a single left hand side.  Thus, an expression
 * such as [A{1:3},B] = func(args) counts for four left hand
 * sides, regardless of the contents of B.
 * If the printIt flag is set, each of the outputs is to be
 * written to the console.
 * Throws an exception if the AST is malformed (i.e., the '[]' is
 * missing, or there are multiple rows in the left hand side.).
 */
void multiFunctionCall(const tree_t* t, BOOL printIt);
/**
 * A special function call is an expression of the form
 * >> func arg1 arg2 arg3
 * which is represented in an AST is
 *     scall
 *       |
 *       fname->arg
 */
void specialFunctionCall(const tree_t* t, BOOL printIt);
/**
 * Handles an if statement, corresponding to an if, a number
 * of elseif blocks and an optional else statement.  The AST looks
 * like
 *     cstat->elseIfBlock->elseCode
 *      |        |
 *      |       cstat->cstat->cstat
 *     expr->codeBlock
 * where each of the elseIf blocks are tested sequentially until
 * one of them succeeds.
 */
void ifStatement(const tree_t* t);
/**
 * Handle a switch statement.  The switch statement tests
 * an expression against a number of case blocks.  If a
 * case block matches, the code in the case block is
 * executed, and the switch statement quits.  If none of the
 * case blocks match, the otherwise code is executed. The
 * AST looks like
 *     expr->caseBlock->otherwiseCode
 *              |
 *             testCase->testCase->...->NULL
 * Where testCase is the AST passed to testCaseStatement. For
 * a switch statement, the switch value must be either a
 * scalar value or a string.  The test values in each case
 * statement can either be the same type, or a cell array.
 * If it is a cell array, then the switch value is compared
 * with each entry in the case expression.
 * Throws an Exception if the switch expression is not
 * either a scalar or a string.
 */
void switchStatement(const tree_t* t);
/**
 * Implements the for control statement.  The AST looks like
 *     ident->codeBlock
 *       |
 *      expr
 * The identifier used as the control variable in the for
 * statement.  The variable defined by the identifier takes
 * on each of the values in the expression.  For each such
 * assignment, the code in the codeBlock is executed.
 */
void forStatement(const tree_t* t);
BOOL tryJitCode(const tree_t* t);
/**
 * Implements the while control statement.  The AST looks like
 *     expr->codeBlock
 * The test expression is evaluated until it fails, and for each
 * successful expression, the codeBlock is executed.
 */
void whileStatement(const tree_t* t);
/**
 * Implements the try statement.  The AST looks like
 *     block->catchBlock
 * The code in block is executed.  If an exception occurs, then
 * the code in catchBlock is executed.
 */
void tryStatement(const tree_t* t);
/**
 * Implements the global statement (really a global declaration).
 * The AST looks like:
 *    ident
 *     |
 *    ident
 *     |
 *    etc.
 * Each identifier is added to the global variable list of
 * the current context.
 */
void globalStatement(const tree_t* t);
/**
 * Implements the persistent statement (really a persistent declaration).
 * The AST looks like:
 *    ident
 *     |
 *    ident
 *     |
 *    etc.
 * Each identifier is added to the persistent variable list of
 * the current context.
 */
void persistentStatement(const tree_t* t);
/**
 * This somewhat strange test is used by the switch statement.
 * If x is a scalar, and we are a scalar, this is an equality
 * test.  If x is a string and we are a string, this is a
 * strcmp test.  If x is a scalar and we are a cell-array, this
 * test is applied on an element-by-element basis, looking for
 * any matches.  If x is a string and we are a cell-array, then
 * this is applied on an element-by-element basis also.
 * The AST for this looks like:
 *     FM_CASE
 *       |
 *      expr->codeBlock
 * The expression is compared to x using mxArray::testForCaseMatch.
 * If a match is found, then the codeBlock is executed.  The result of
 * the test is returned.  Throws an exception if the AST is
 * malformed.
 */
BOOL testCaseStatement(const tree_t* t, mxArray x);
/**
 * Execute the statement described by the AST - the printIt flag
 * determines if the result of the statement should be printed to
 * the console.  The form of the AST required depends on the
 * type of statement being executed.  The various statement types
 * handled are as follows:
 *   =          Assignment of expression to LHS
 *   |
 *  LHS->expr
 *
 *  multicall   Multifunction call
 *   |
 *  multicallBody
 *
 *  FM_FOR      For statement
 *   |
 *  forBody
 *
 *  FM_WHILE    While statement
 *   |
 *  whileBody
 *
 *  FM_IF       If statement
 *   |
 *  ifBody
 *
 *  FM_BREAK    Break statement - change the state of the interpreter to
 *              FM_STATE_BREAK
 *
 *  FM_CONTINUE Continue statement - change the state of the interpreter
 *              to FM_STATE_CONTINUE
 *
 *  FM_RETURN   Return statement - change the state to FM_STATE_RETURN
 *
 *  FM_SWITCH   Switch statement
 *   |
 *  switchBody
 *
 *  FM_TRY      Try statement
 *   |
 *  tryBody
 *
 *  FM_KEYBOARD Enter another CLI session
 *
 *  FM_GLOBAL   A global declaration
 *   |
 *  globalBody
 *
 *  FM_PERSISTENT A persistent declaration
 *   |
 *  persistentBody
 *
 *  rhs         A function call being evaluated as a statement
 *   |
 *  funcName
 *
 *  expr        An expression being evaluated as a statement
 *
 * The function call is trapped before the expression to handle
 * the special variable "ans".
 * Throws an Exception if the statement type is not recognized.
 */
void processBreakpoints(const tree_t* t);
void statementType(const tree_t* t, BOOL printIt);
/**
 * The statement method simply screens out the need for the
 * printIt flag.  It also retrieves the statement context
 * for use in error reporting.  The AST looks like:
 *   qstatement          A quiet statement (printIt -> FALSE)
 *      |
 *    context (optional)
 *      |
 *  statementBody
 *
 *   rstatement          A normal statement (printIt -> TRUE)
 *      |
 *    context (optional)
 *      |
 *  statementBody
 * The context data is supplied by the parse (indicates the
 * line number and filename if necessary).
 */
void statement(const tree_t* t);
/**
 * Handles the logistics of shortcut evaluation
 */
mxArray ShortCutOr(const tree_t* t);
mxArray ShortCutAnd(const tree_t* t);
/**
 * Display an array - added so user classes divert to "display" function
 */
void displayArray(mxArray b);
/**
 * Mangle a function name to get the private version (if it exists)
 */
QString getPrivateMangledName(QString fname);
QString getLocalMangledName(QString fname);
/**
 * Convert variables into indexes, calls "subsindex" for user classes.
 */
mxArray subsindex(const mxArray* m);
void subsindex(ArrayVector& m);
/**
 * Enter a debug cycle
 */
void doDebugCycle();
/**
 * Send a greeting to the user.
 */
void sendGreeting();
/**
 * Scan the given directory for .m, .p, classes, etc.  If tempfunc
 * is TRUE, then these functions are marked as temporary (so changing
 * the working directory flushes them).
 */
void scanDirectory(QString scdir, BOOL tempfunc, QString prefixo);
/**
 * Add the specified .m file to the current context
 */
void procFileM(QString fname, QString fullname, BOOL tempfunc);
/**
 * Add the specified .p file to the current context
 */
void procFileP(QString fname, QString fullname, BOOL tempfunc);
/**
 * Add the specified .mex file to the current context
 */
void procFileMex(QString fname, QString fullname, BOOL tempfunc);

friend mxArray IndexExpressionToStruct(interpreter_t*, const tree_t*, mxArray);
friend ArrayVector ClassRHSExpression(mxArray, const tree_t*, interpreter_t*);
friend ArrayVector PCodeFunction(int, const ArrayVector&, interpreter_t*);
friend class MFunctionDef;
friend class ImportedFunctionDef;
friend class AnonymousFunctionDef;
friend ArrayVector AnonFuncSubsrefFunction(int, const ArrayVector&, interpreter_t*);
friend ArrayVector AnonFuncFevalFunction(int, const ArrayVector&, interpreter_t*);

#endif
