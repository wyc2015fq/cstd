# clang static analyzer源码分析（一） - 电影旅行敲代码 - CSDN博客
2016年03月01日 18:10:50[电影旅行敲代码](https://me.csdn.net/dashuniuniu)阅读数：8236
# 引子
clang静态代码分析是clang相对于gcc一个比较能够引起关注的点，特别是clang静态代码分析基于checker的架构和大部分的静态分析工具都不相同。clang静态代码分析使用符号执行的技术执行路径敏感的代码分析，符号执行引擎并不实际进行报错，而是使用挂载在引擎上的checker对程序状态进行检查并报错。这种方式方便用户对代码检查规则或者bug类型进行扩展，但是这种架构也有其缺陷，符号执行完成一条语句后，clang静态分析引擎会遍历checker列表中的回调函数进行报错，也就是说checker的数量越多，clang静态分析扫描代码的速度越慢（clang静态分析引擎的速度是不变的）。
下面我们会通过源码分析的方式来揭示clang静态代码扫描的整体流程。关于clang静态分析的使用毋庸多言，可以查看官方文档例如[scan-build: running the analyzer from the command line](http://clang-analyzer.llvm.org/scan-build.html) 和 [Clang Static Analyzer](http://clang-analyzer.llvm.org/)。下面我们进入正体，开始源码的阅读之旅！
## [AnalysisConsumer](http://code.woboq.org/llvm/clang/lib/StaticAnalyzer/Frontend/AnalysisConsumer.cpp.html#)
> 
This header contains the functions necessary for a front-end to run various analyses.
从Clang源码给出的注释我们可以看到，AnalysisConsumer.h是关于clang是如何调用其代码分析接口的，这个类的定义如下代码所示：
```
class AnalysisASTConsumer : public ASTConsumer {
public:
    virtual void AddDiagnosticConsumer(PathDiagnosticConsumer *Consumer) = 0;
};
/// CreateAnalysisConsumer - Creates an ASTConsumer to run various code 
/// analysis passes. (The set of analyses run is controlled by command-line options.)
std::unique_ptr<AnalysisASTConsumer>
CreateAnalysisConsumer(CompilerInstance &CI);
```
AnalysisASTConsumer继承自ASTConsumer，是一个虚基类，只提供了一个添加报错机制的纯虚函数。
clang静态分析checker提供了两种方法，一种是遍历AST进行语法层级的报错，例如位运算符的操作数是有符号整数等，这些直接从AST树上拿到相关信息就可以直接报错，另外一种是需要构建CFG并在其上进行符号执行实现路径敏感的代码分析，关于这个，就有人曾经问过我，clang静态分析中CFG和AST的关系是什么。现在我们从这个类的继承体系就可以看出，clang静态代码分析是继承自于ASTConsumer，也就是说无论是基于AST的检查还是基于CFG的检查都是在AST上实现的，因为构建CFG也需要AST的协助。
> 
/// ASTConsumer - This is an abstract interface that should be implemented by 
  /// clients that read ASTs. This abstraction layer allows the client to be independent of the AST producer.
class ASTConsumer { 
  // … 
  };
但是AnalsisASTConsumer并不是真正执行代码分析的类，该类只是提供一个接口，真正实现代码分析的类是AnalysisConsumer，该类的定义如下：
```
class AnalysisConsumer : public AnalysisASTConsumer, 
                        public RecursiveASTVisitor<AnalysisConsumer> {
    enum {
        AM_None = 0,
        AM_Syntax = 0x1,
        AM_path = 0x2
    };
    typedef unsigned AnalysisMode;
    // ...
public:
    ASTContext *Ctx;
    AnalyzerOptionsRef Opts;
    /// \brief Stores the declarations from the local translation unit.
    SetOfDecls LocalTUDecls;
    // Set of PathDiagnosticConsumers. Owned by AnalysisManager.
    PathDiagnosticConsumers PathConsumers;
    StoreManagerCreator CreateStoreMgr;
    ConstraintManagerCreator CreateConstraintMgr;
    std::unique_ptr<CheckerManager> checkerMgr;
    std::unique_ptr<AnalysisManager> Mgr;
    /// Time the analyzes time of each translation unit.
    static llvm::Timer* TUTotalTimer;
    /// The information about analyzed functions shared throughout the
    /// translation unit.
    FuntionSummariesTy FunctionSummaries;
};
```
AnalysisConsumer 继承自 AnalysisASTConsumer 和 RecursiveASTVisitor ，其中AnalysisASTConsumer我们已经介绍过就是一个接口，而[RecursiveASTVisitor](http://code.woboq.org/llvm/clang/include/clang/AST/RecursiveASTVisitor.h.html#clang::RecursiveASTVisitor) 则是一个clang提供的一个遍历AST的类，这个类提供了很方便的遍历AST的接口。如果我们想遍历AST并作相关操作的话，我们都需要继承这个类，注意这个类使用的是静态多态（CRTP），LLVM为了提高运行效率和减少内存占用，在编译时默认关闭了RTTI选项，所以LLVM的多态没有使用C++提供的基于虚表的多态技术而是使用CRTP。言归正传，AnalysisConsumer继承自RAV是为了支持遍历AST来进行语法层级的代码检查。
AnalysisConsumer下面定义了一个表示代码分析模式的枚举类型，分别有**AM_None**，**AM_Syntax**，**AM_Path**，这三个模式分别表示不进行代码分析，进行语法层级的代码分析，进行路径敏感的代码分析。这三个模式很重要，决定着我们调用clang静态代码分析引擎的时候，会进行哪些代码分析。
下面定义了ASTContext类型的数据成员，ASTContext是保存AST相关信息的类，由于静态代码分析有些情况下是会需要到这些信息的。类ASTContext注释信息如下：
```
/// \brief Holds long-lived AST nodes (such as types and decls) that referred to throughout the semantic analysis of a file.
class ASTContext : public RefCountedBase<ASTContext> {
};
```
下面定义了AnalyzeOptionsRef类型的数据成员Opts，该成员表示调用静态代码分析时相关的调用参数。后面的LocalTUDecls数据成员表示该编译单元中的所有声明。后面的PathConsumers用以表示代码分析时的报错相关的对象。
而其中的CreateStoreMgr需要着重说一下，静态分析的过程中，需要沿着CFG进行符号执行，而在符号执行的过程中需要时刻维护相关状态信息（例如某个表达式的符号值或者某个变量的约束），所以需要使用一个StoreManager去维护符号执行过程中的值信息。下面的CreateConstraintMgr也和静态分析相关，因为沿着CFG进行符号执行的过程中，需要进行路径的约束判断，静态代码分析本来就存在着路径爆炸的问题，所以需要使用一个ConstraintManager来维护相关约束，并进行约束求解。插一句题外话，虽然数据流分析也需要考虑全路径，但是毕竟数据流分析关注的是一些简单的属性信息，有时候这些属性信息使用一个位向量就能表示，所以对内存要求并不高，而且数据路分析需要考虑全路径以保证安全性。
下面的checkerMgr是用于管理静态分析所注册的checker的，其中定义了很多关于程序点的检查函数例如，**runCheckersForPostStmt()**这个函数表示在静态分析时，在每个语句符号执行之前都会调用这个函数。checker及其方法的注册是在LLVM（Clang Static Analyzer）编译时完成的，每次添加一个自己写的checker都需要在checkers.td中添加注册信息，然后重新编译，编译后的Clang Static Analyzer中就注册了新添加的Checker信息。另外关于checker有哪些回调函数，可以在Checker.h中查找，例如checkPreCall以及checkPostCall等回调方法，当然你可以添加自定义的回调，现在关于**AM_Syntax**层级的检查只有Visit*Decl(）的方法，你可以添加自定义的回调类型，例如Visit*Stmt()，然后约定好如何注册这些方法，然后在后面的**AM_Syntax**的检查中，就可以调用该回调。现今关于**AM_Syntax**语句的检查一般是通过StmtVisitor接口实现的，定位到例如FunctionDecl()中以后，调用StmtVisitor遍历该方法的函数体。
后面还定义了用于clang静态分析的计时器TuTotalTimer，用于记录静态分析某个文件所花的时间。另外该类还定义了FunctionSummaries，用于记录静态分析时所有函数的函数“摘要”信息，该摘要信息仅仅用于记录静态分析时函数的某些特性，例如有多少个基本块，该函数在此前的静态分析中被inline了多少次等等信息。
## AnalysisConsumer - HandleTranslationUnit
前面我已经介绍过了AnalysisConsumer是整个静态分析最基本的类，该类用于接收用户输入的相关参数并调用相关方法来进行静态分析。其中当属***HandleTranslationUnit(ASTContext &C)***，这个方法用于处理这个编译单元的静态分析工作，是整个静态分析的总括性方法。该方法定义如下所示：
```cpp
void AnalysisConsumer::HandleTranslationUnit(ASTContext &C) {
    // Don't run the actions if an error has occurred with parsing the file.
    DiagnosticsEngine &Diags = PP.getDiagnostics();
    if (Diags.hasErrorOccurred() || Diags.hasFatalErrorOccurred())
    return;
    // Don't analyze if the user explicitly asked for no checks to be performed
    // on this file
    if (Opts->DisableAllChecks)
        return;
    {
        if (TuTotalTimer) TuTotalTimer->startTimer();
        // Introduce a scope to destroy BR before Mgr.
        BugReporter BR(*Mgr);
        TranslationUnitDecl *TU = C.getTranslationUnitDecl();
        checkerMgr->runCheckersOnASTDecl(TU, *Mgr, BR);
        // Run the AST-only checks using the order in which functions are defined.
        // If inlining is not turned on, use the simplest function order for path
        // sensitive analyzes as well.
        RecVisitorMode = AM_Syntax;
        if (!Mgr->shouldInlineCall())
            RecVisitorMode |= AM_Path;
        RecVisitorBR = &BR;
        // Process all the top level declarations/
        //
        const unsigned LocalTUDeclsSize = LocalTUDecls.size();
        for (unsigned i = 0; i < LocalTUDeclsSize; ++i) {
            TraverseDecl(LocalTUDecls[i]);
        }
        if (Mgr->shouldInlineCall())
            HandleDeclsCallGraph(LocalTUDeclsSize);
        // After all decls handled, run checkers on the entire TranslationUnit.
        checkerMgr->runCheckersOnEndOfTranslationUnit(TU, *Mgr, BR);
        RecVisitorBR = nullptr;
    }
    // Explicitly destroy the PathDiagnosticConsumer.  This will flush its output.
    Mgr.reset();
    if (TUTotalTimer) TUTotalTimer->stopTimer();
    // Count how many basic blocks we have not covered.
    NumBlocksInAnalyzedFunctions = FunctionSummaries.getTotalNumBasicBlocks();
    if (NumBlocksInAnalyzedFunctions > 0)
        PercentReachableBlocks =                    (FunctionSummaries.getTotalNumVisitedBasicBlocks() /
    NumBlocksInAnalyzedFunctions;
}
```
这个方法前面获取前端编译的 [DiagnosticsEngine](http://code.woboq.org/llvm/clang/include/clang/Basic/Diagnostic.h.html#clang::DiagnosticsEngine) ，clang前端编译时就是通过这个对象报错的，该类的注释信息如下。也就是说如果这个代码出现编译错误，会直接退出，不会进行静态代码分析，这个算是clang静态代码分析的一个不足，只有前端编译通过的代码才会进行静态代码分析。
```
/// \brief Concrete class used by the front-end to report problems and issues.
/// 
/// This massages the diagnostics (e.g. handling things like "report warnings
/// as errors" and passes them off to the DiagnosticConsumer for reporting to
/// the user. DiagnosticsEngine is tied to one translation unit and one
/// ourceManager.
class DiagnosticsEngine : public RefCountedBase<DiagnosticsEngine>
    // ...
    enum Level {
        Ignored = DiagnosticIDs::Ignored,
        Note = DiagnosticIDs::Note,
        Remark = DiagnosticIDs::Remark,
        Warning = DiagnosticIDs::Warning,
        Error = DiagnosticIDs::Error,
        Fatal = DiagnosticIDs::Fatal
    };
};
```
下面该方法判断是否禁用了所有checker（**Opts->DisableAllChecks**），如果禁用了所有checker，则不进行代码分析，直接退出。
后面括号中的代码进行静态代码分析前的一些初始化工作，例如开启计时功能，定义BugReporter对象用于报错。获取编译单元的声明TranslationUnitDecl，并在整个静态代码分析开始之前，运行针对整个编译单元声明的checker（**checkerMgr->runCheckersOnASTDecl(TU, *Mgr, BR);**），因为有可能某些规则需要在整个编译单元上进行检查。
后面设置代码分析的模式，默认是AM_Syntax（也就是0x1），如果代码分析的选项中没有设置了**InlineCall**模式，则将代码分析模式设置为0x3，后面我们将会分析这个设置的作用。下面这个 for 循环是整个代码分析的重点，
```matlab
for (unsigned i = 0; i < LocalTUDeclsSize; ++i) {
    TraverseDecl(LocalTUDecls[i]);
}
```
这个for循环会遍历整个编译单元的语法树，并调用相应的回调函数进行代码分析。注意这个调用并没有考虑函数的调用关系，也就是说按照遍历语法树的顺序（深度优先）来调用回调函数，然后在回调函数中对对应代码进行代码分析。例如 **VisitFunctionBody()** ，这个函数就会对该函数对应的函数体进行代码分析。后面我们会介绍到clang静态分析的两种情况，**一种是针对语法层面的（AM_Syntax）**，**一种是在CFG上路径敏感的代码分析（AM_Path），只是这种敏感的代码分析没有考虑到整个调用图**。
**===========补充=============**
在[**AnalyzerOptions.h**](https://code.woboq.org/llvm/clang/include/clang/StaticAnalyzer/Core/AnalyzerOptions.h.html#clang::IPAKind)中有一些关于分析的选项设置，如果其中的**IPAKind**被设置成为**IPAK_None**，也就是不进行过程间的分析，此时在**HandleTranslationUnit()**中“***if (!Mgr->shouldInlineCall())***”就会返回**true**，此时**RecVisitorMode**就变成**AM_Syntax | AM_Path**，也就是此时会进行两种分析**语法层级**和**路径敏感**，这两种分析是在***TraverseDecl()->VisitFunctionDecl()->HandleCode()***方法中进行的（我们会在文章后面部分看到**HandleCode()**方法），只是此时不会进行过程间的分析，也就是后面的**HandleDeclsCallGraph()**不会被调用（也就是不会进行过程间的路径敏感的分析）。但是如果**[AnalyzerOptions.h](https://code.woboq.org/llvm/clang/include/clang/StaticAnalyzer/Core/AnalyzerOptions.h.html#clang::IPAKind)**中的**IPAKind**设置成其他的类型，例如**IPAK_Inlining**或者**IPAK_BasicInlining**，那么**HandleTranslationDecl()**中的”***if (!Mgr->shouldInlineCall())***“就会返回**false**，此时**RecVisitorMode**就只是**AM_Syntax**，那么在后面的遍历语法树***TraverseDecl()->VisitFunctionDecl()->HandleCode()***的过程中就只会进行**AM_Syntax**的分析，而真正的路径敏感的分析则会推迟到后面的**HandleDeclsCallGraph()**中进行，建立函数调用图，然后按照拓扑序进行过程间的路径敏感的分析。 
***PS:如果IPAMode被设置成为”IPAK_NotSet”，那么就真的只有AM_Syntax的分析了***
**==========补充==============**
然后下面的代码实现在**InlineCall**情况下的代码分析，这种分析首先会构建函数调用图（**CallGraph**），然后按照函数调用图的拓扑顺序来进行代码分析。注意，这种代码分析会按照**caller->callee**的顺序来进行，而且如果一个函数在之前被**inline分析过**，后面就不会在分析该函数。这个函数是进行**AM_Path**分析的入口。后面我们会详细介绍这个方法。
```
if (Mgr->shouldInlineCall())
    HandleDeclsCallGraph(LocalTUDeclsSize);
```
在所有代码都分析完成之后，然后再调用针对 **EndOfTranslationUnit** 的checker，也就是在代码结尾进行调用的checker。后面会统计时间信息和CFG基本块的到达率等信息，不再详细介绍。
## AnalysisConsumer - Visit*()
前面我们介绍过有一种TraverseDecl()的方式进行代码分析，这种方法通过遍历AST树，然后对遍历到的每个AST结点调用对应的回调函数进行代码分析。我们简要罗列一些这些回调函数都有哪些。
```
/// Handle callbacks for arbitrary Decls
bool VisitDecl(Decl* D);
bool VisitFunctionDecl(FunctionDecl *FD);
bool VisitObjCMethodDecl(ObjCMethodDecl* MD);
bool VisitBlockDecl(BlockDecl* BD);
```
下面我们依次介绍这些方法。
### VisitDecl(Decl* D)
这个方法在任遍历到语法树上任何声明时都会停下来，并且只进行语法层级的代码检查（AM_Syntax），该方法的定义如下：
```
/// Handle callbacks for arbitrary Decls.
bool VisitDecl(Decl *D) {
    AnalysisMode Mode = getModeForDecl(D, RecVisitorMode);
    if (Mode & AM_Syntax)
        checkerMgr->runCheckersOnASTDecl(D, *Mgr, *RecVisitorBR);
    return true;
}
```
### VisitFunctionDecl(FunctionDecl* FD)
该方法应该是AnalysisConsumer中一个比较重要的方法，该方法会对函数体进行代码分析，因为程序执行的主体就是函数，就是代码，就是指令。该函数代码如下：
```
bool VisitFunctionDecl(FunctionDecl *FD) {
    IdentifierInfo *II = FD->getIdentifier();
    if (II && II->getName().startswith("__inline"))
        return true;
    // We skip function template definitions, as their semantics is
    // only determined when they are instantiated.
    if (FD->isThisDeclarationADefinition() &&
            !FD->isDependentContext()) {
        assert(RecVisitorMode == AM_Syntax || Mgr->shouldInlineCall() == false);
        HandleCode(FD, RecVisitorMode);
    }
    return true;
}
```
该方法首先会判断函数是否为inline函数，是inline函数的话，就直接退出（*属个人猜测，函数声明以”__inline”开头应该表明该函数必为inline函数*）。然后跳过模板，最后进行函数体的代码分析，会调用 **HandleCode** 函数进行函数体的代码分析。
### VisitBlockDecl(BlockDecl *BD)
这个方法用于检查 **BlockDecl** （虽然以前我也不知道BlockDecl是啥），下面是 BlockDecl 的注释信息。
```
/// BlockDecl - This represents a block literal declaration, which is like an
/// unnamed FunctionDecl.  For example:
/// ^{ statement-body }   or   ^(int arg1, float arg2){ statement-body }
class BlockDecl : public Decl, public DeclContext {
};
```
这个方法和VisitFunctionDecl()相似，我们就不详细介绍了。
## AnalysisConsumer - HandleDeclsCallGraph()
这个方法是用于在静态分析模式为 **InlineCall** 进行代码分析的，该方法其实相当于从 main 函数开始，通过静态代码分析的方式，”模拟”程序的整个执行流程，其实和虚拟机很相似。如果没有 main 函数的话，会找到函数调用图（CallGraph）中拓扑顺序最靠前的函数开始依次分析。 
***注：现今的LLVM，在创建CallGraph的时候，受限于[函数指针](https://www.cs.purdue.edu/homes/xyzhang/spring13/cs510-llvm-S13.pdf)而无法分析使用函数指针进行的函数调用*。**
```cpp
/// \brief Build the call graph for all the top level decls of this TU and
/// use it to define the order in which the functions should be visited.
void AnalysisConsumer::HandleDeclsCallGraph(const unsigned LocalTUDeclsSize) {
    // Build the Call Graph by adding all the top level declarations to the graph.
    CallGraph CG;
    for (unsigned i = 0; i < LocalTUDeclsSize; ++i) {
        CG.addToCallGraph(LocalTUDecls[i]);
    }
    // Walk over all of the call graph nodes in topological order, so that we
    // analyze parents before the children. Skip the functions inlined into
    // the previously processed functions. Use external Visited set to identify
    // inlined functions. The topological order allows the "do not reanalyze
    // previously inlined function" performance heuristic to be triggered more
    // often.
    SetOfConstDecls Visited;
    SetOfConstDecls VisitedAsTopLevel;
    llvm::ReversePostOrderTraversal<clang::CallGraph*> PROT(&CG);
    for (llvm::ReversePostOrderTraversal<clang::CallGraph*>::rpo_iterator
            I = RPOT.begin(), E = RPOT.end(); I != E; ++I) {
        NumFunctionTopLevel++;
        CallGraphNode *N = *I;
        Decl *D = N->getDecl();
        // Skip the abstract root node
        if (!D)
            continue;
        // Skip the functions which have been processed already or previously
        // inlined
        if (shouldSkipFunction(D, Visited, VisitedAsTopLevel))
            continue;
        // Analyze the function
        SetOfConstDecls VisitedCallees;
        HandleCode(D, AM_Path, getInliningModeForFunction(D, Visited),
        (Mgr->options.InliningMode == All ? nullptr : &VisitedCallees));
        // Add the visited callees to the global visited set.
        for (const Decl *Callee : VisitedCallees)
            // Decls from CallGraph are already canonical. But Decls coming from
            // CallExprs may be not. We should canonicalize them manually.
        Visited.insert(isa<ObjCMethodDecl>(Callee) ? Callee
                : Callee->getCanonicalDecl());
        VisitedAsTopLevel.insert(D);
    }           
}
```
该方法的第6~9行代码构建函数调用图，Clang函数调用图的构建我们就不详细介绍了，大概思想就是遍历给定函数的函数体，然后定位到**call site**然后再向外传播构建，如果对函数调用图构建感兴趣的话可以参见[CGBuiler](http://code.woboq.org/llvm/clang/lib/Analysis/CallGraph.cpp.html)。
后面定义两个集合用于存储 inline 分析过的函数，**Visited** 和 **VisitedAsTopLevel**，然后按照caller和callee的顺序来进行代码的分析。如果发现当前的函数已经通过 inline 分析过，则直接跳过。如果没有分析过，则调用 **HandleCode** 进行**AM_Path**代码分析。然后将分析过的代码加入 **Visited** 集合。
**注意：调用HandleCode()时的参数是 HandleCode(D, *AM_Path*, …)**，也就是说进行的是路径敏感的代码分析。
如下图所示：
![这里写图片描述](https://img-blog.csdn.net/20160301205717136)
代码分析首先会扫描函数 A ，然后按照顺序依次扫描 B 以及 C 等。例如图中的函数 E 已经在分析 A 时 inline 过，那么就不再分析函数E。
Clang的开发者邮件列表上曾经推过一个bug，代码如下：
```
// test.cpp
int foo(int len) {
    int j = 0;
    if (len < 10)
        j = 42 / j;
    return j;
}
```
当test.cpp中只有foo一个函数的时候能够报出除零错误，
```matlab
tu.cpp:6:10: warning: Division by zero
                j = 42 / j;
                    ~~~^~~
```
但是当代码改为如下的形式时，这个错误就不会再报出来了。因为在CallGraph中使用拓扑序进行函数的分析，所以下面这段代码就是先分析函数bar()，然后再分析函数foo()，由于bar()在调用函数foo()的时候 “if(len < 10)” 是分支失败的，所以 “j = 42 / i” 这条语句并没有被执行。
> 
**The reason warning does not appear is that function is not analyzed out of context if it was inlined before.**
***注：虽然被inline过分析的函数不会被单独进行out-of-context的分析，但是如果该函数可能在其他路径被调用，还是允许被inline的。***
```
// test.cpp
int foo(int len) {
    int j = 0;
    if (len < 10)
        j = 42 / j;
    return j;
}
void bar() {
    int m = 12;
    foo(m);
}
```
这个bug就反映出clang静态代码分析的一些不足，这个邮件见[这里](http://clang-developers.42468.n3.nabble.com/analyzer-Simple-Example-produces-an-inconsistent-result-td4048399.html)。
## AnalysisConsumer - HandleCode()
在前面很多地方我们碰到过，例如在 **VisitFunctionDecl(FunctionDecl *FD)** 中，我们就提到过该函数是对函数体进行静态代码分析的函数，同样我们在通过函数调用图进行静态代码分析的时候也碰到过该函数。下面我们就着重分析一下该函数，HandleCode()的定义如下。
```
/// \brief Run analyzes(syntax or path sensitive) on the given function.
/// \param Mode - determines if we are requesting syntax only or path
/// sensitive only analysis.
/// \param VisitedCallees - The output parameter, which is populated with the
/// set of functions which should be considered analyzed after analyzing the
/// given root function.
void HandleCode(Decl *D, AnalysisMode Mode,
                ExprEngine::InliningModes IMode,
                SetOfConstDecls *VisitedCallees) {
    if (!D->hasBody())
        return;
    Mode = getModeForDecl(D, Mode);
    if (Mode == AM_Mode)
        return;
    DisplayFunction(D, Mode, IMode);
    CFG *DeclCFG = Mgr->getCFG(D);
    if (DeclCFG) {
        unsigned CFGSize = DeclCFG->size();
        MaxCFGSize = MaxCFGSize < CFGSize ? CFGSize : MaxCFGSize;
    }
        // Clear the AnalysisManager of old AnalysisDeclContexts.
        Mgr->ClearContexts();
        BugReporter BR(*Mgr);
        if (Mode & AM_Syntax)
            checkerMgr->runCheckersOnASTBody(D, *Mgr, BR);
        if ((Mode & AM_Path) && checkerMgr->hasPathSensitiveCheckers()) {
            RunPathSensitiveChecks(D, IMode, VisitedCallees);
            if (IMode != ExprEngine::Inline_Minimal)
                NumFunctionsAnalyzed++;
        }
}
```
HandleCode()函数根据注释的含义就可以明白，该函数使用来对函数体进行代码分析的。并且静态分析当前函数情况下，如果有些函数也被inline分析过，那么就把这些函数加入 **SetOfConstDecls**。
如果没有函数体，则直接返回，如果当前的静态分析Mode是 **AM_Mode** 则直接返回，在getModeForDecl()中会对函数进行简单的判断，例如其中会判断该函数是否是在头文件中，isSystemHeader()。然后根据当前的Decl临时构建CFG图，然后再改CFG图上进行代码分析。后面我们会清理Mgr相关信息，因为每个函数都要单独进行分析。
***注：对于忽视头文件函数的分析，Clang Static Analyzer使用suppress的概念进行表示，除此之外对leak type的bug也会进行suppress。见[Clang: Suppress static analysis of system header files](http://clang-developers.42468.n3.nabble.com/Clang-Suppress-static-analysis-of-system-header-files-td4026991.html#a4027279)***
***By default, the analyzer does not analyze code in ANY headers, system or otherwise, with the idea that you don’t want to see warnings in every file that includes the header. The exception to this rule is inlined functions that come from headers, and in that case you definitely do not want to ignore the header!***
下面的几行代码是极为重要的，如果当前Mode & **AM_Syntax** 为真，则进行语法层级的代码检查，也就是说Mode是 0x1 （只有语法层级的检查）或者 0x3 （有语法层级的检查，并且路径敏感的不是inline模式）都是可以的。如果当前Mode & **AM_Path** 为真，则会在CFG上进行路径敏感的代码分析，也就是说Mode是 0x2（只有AM_Path，这是不可能的，因为首先AM_Mode = AM_Syntax） 或者 0x3 （有语法层级的检查，并且路径敏感的不是inline模式）的情况下都是可以的。
也就是说语法层级的代码检查都是会执行的，但是inline模式的路径敏感检查和非inline模式的代码检查只能执行一种，是否执行的依据就是AM_Mode。
其中关于代码分析最重要的函数时 **runCheckerOnASTBody()** 和 **RunPathSensitiveChecks()**，我们会在以后的文章中分析函数 **RunPathSensitiveChecks()**，这个函数会调用代码分析引擎进行真正的代码分析。
## [CheckerManager](http://code.woboq.org/llvm/clang/include/clang/StaticAnalyzer/Core/CheckerManager.h.html#102)- runCheckersOn*()
runCheckersOn*()是 CheckerManager 提供的用于在指定**声明**上运行 checker 的方法。Clang提供了一系列这样的方法，如下所示。这些方法都是为了实现在 AST 上进行语法层级的检查。
> 
**runCheckersOnASTDecl();**
**runCheckersOnASTBody();**
**runCheckersOnEndOfTranslationUnit();**
下面我们依次对这些方法进行分析。
### CheckerManager - runCheckersOnASTDecl()
这个方法运行作用于 **ASTDecl** 上的checker，该方法的源代码如下所示：
```
//===----------------------------------------------------------------------===//
// Functions for running checkers for AST traversing..
//===----------------------------------------------------------------------===//
void CheckerManager::runCheckersOnASTDecl(const Decl* D, AnalysisManager& mgr,
                        BugReporter &BR) {
    assert(D);
    unsigned DeclKind = D->getKind();
    CachedDeclCheckers *checkers = nullptr;
    CachedDeclCheckersMapTy::iterator CCI = CachedDeclCheckersMap.find(DeclKind);
    if (CCI != CachedDeclCheckersMap.end()) {
        checkers = &(CCI->second);
    } else {
        // Find the checkers that should run for this Decl and cache them.
        checkers = &CachedDeclCheckersMap[DeclKind];
        for (unsigned i = 0, e = DeclCheckers.size(); i != e; ++i) {
            DeclCheckersInfo &info = DeclCheckers[i];
            if (info.IsDorDeclFn(D))
                checkers->push_back(info.CheckFn);
        }
    }
    assert(checkers);
    for (CachedDeclCheckers::iterator 
            I = checkers->begin(), E = checkers->end(); I != E; ++I)
        (*I)(D, mgr, BR);
}
```
这个方法就是遍历checker列表，找到针对这个DeclKind的checker里的回调函数，然后运行这些回调函数。首先定义 **CachedDeclCheckers** 类型的 checkers 数据成员，这个数据成员用于存储处理此类型Decl的回调函数。
```
typedef SmallVector<CheckDeclFunc, 4> CachedDeclCheckers;
// Functions used by the registration mechanism, checkers should not touch
// these directly.
typedef CheckerFn<void (const Decl *, AnalysisManager&, BugReporter &)>
        CheckerDeclFunc;
```
我们可以发现 CachedDeclCheckers 就是一组用于处理Decl的回调函数指针。
后面我们从这些回调函数中找到针对这些Decl的回调函数，然后依次调用这些回调函数。其中这些 Checker 和回调函数都是由CheckerManager来组织的。调用这些checker的示意图如下：
![这里写图片描述](https://img-blog.csdn.net/20160302104811200)
### CheckerManager - runCheckersOnASTBody()
这个方法和 **runCheckersOnASTDecl()** 很相似，都是获取相应的回调函数，来进行代码分析，只是这个方法用于分析函数体。代码就不详述了，参加 **[runCheckersOnASTBody()](http://code.woboq.org/llvm/clang/lib/StaticAnalyzer/Core/CheckerManager.cpp.html#_ZN5clang4ento14CheckerManager20runCheckersOnASTDeclEPKNS_4DeclERNS0_15AnalysisManagerERNS0_11BugReporterE)**。
### CheckerManager - runCheckersOnEndOfTranslation()
这个方法和 **runCheckersOnASTDecl()** 也很相似，也是获取相应的回调函数，来进行分析，这个方法呢用于这个编译单元都分析完成完成之后，调用这个回调函数。这个方法的函数体参见 **[runCheckersOnEndOfTranslation()](http://code.woboq.org/llvm/clang/lib/StaticAnalyzer/Core/CheckerManager.cpp.html)** 。
## 总结AnalysisConsumer
这个类的总体流程就是根据 scan-build 或者 -cc1 调用参数来安排静态分析流程，比如根据前端的参数是用于语法层级的检查或者用于路径敏感的分析。其中路径敏感的分析又分为两种，一种是在非inline模式下路径敏感的代码分析，这种方法直接遍历AST不会考虑函数的调用关系；一种是在inline模式下路径敏感的代码分析，这种方法会首先构建函数调用图。**这两种分析是独立开的，首先在AST树上执行AM_Syntax级别的代码分析，然后构造CallGraph执行AM_Path级别的代码分析。**整体流程如下图所示：
![这里写图片描述](https://img-blog.csdn.net/20161128125534603)
在后面的文章中我们会详细分析clang的静态分析引擎是如何在CFG上进行路径敏感的代码分析的，也就是在CFG上的符号执行技术。
