# clang static analyzer源码分析（五） - 电影旅行敲代码 - CSDN博客
2016年09月08日 17:22:41[电影旅行敲代码](https://me.csdn.net/dashuniuniu)阅读数：2310
# 引子
上一篇文章我们介绍了**[CFG](https://code.woboq.org/llvm/clang/include/clang/Analysis/CFG.h.html)**，path-sensitive analysis就是在**CFG**上进行的，今天这篇文章主要介绍clang static analyzer的内存模型，相关的有 **MemRegion**以及**MemRegion**等相关的类。
**—————————更新————————–**
关于**clang static analyzer-memory model**的设计思想可以阅读《[**A Memory Model for Static Analysis of C Programs**](http://lcs.ios.ac.cn/~xuzb/canalyze/memmodel.pdf)》这篇论文，下面这篇文章过于琐碎，繁杂，反而没有阅读论文来的清晰。如果有兴趣的话可以看一些另外一篇文章《[**clang static analyzer源码分析（番外篇）：RegionStore以及evalCall()中的conservativeEvalCall**](http://blog.csdn.net/dashuniuniu/article/details/52849373)》，这篇文章详细介绍了RegionStore相关的内容。 
**—————————更新————————–**
## 内存建模
符号执行的基础是对程序中的变量，表达式有一个合理的表示。在计算机程序中任何变量的基础都是在内存中的实际存储的值，无论一个变量有多少别名，别名的结构有多复杂，处理一个变量或它的别名归根结底都是处理其对应的内存空间。
**clang static analyzer**通过区域（**region**）来做内存空间的抽象，那么存储（**store**）就是一个从区域（**region**）到值得映射，值就是前面提到过的**SVal**。表达式都有左值和右值之分，**clang**中的**Expr**可以使用 ***isLValue()*** 和 ***isRValue()*** 以及 ***isXValue()*** 来区分。
> 
C and C++ give somewhat different rules for this concept, but in general, **the result of an l-value expression identifies a specific object** whereas **the result of an r-value expression is a value detached from any specific storage**.
由于有左右值的存在，所以程序中很多语句都可拆分为更细的步骤。例如下面的 “**int num = a**“中的”**a**“其实有两个步骤，第一步取得***a*** 的左值（storage），第二步取得 ***a*** 对应内存空间中所存储的值。
```
int func(int a)
{
    int num = a;
    return num;
}
```
上面的代码对应的AST如下，”**int num = a**“中 ***a*** 其实由两个表达式表示，**DeclRefExpr**和**ImplicitCastExpr**（***LValueToRValue***），并且在**CFGBlock**中 ***a*** 也是由两个**CFGElement**组成的。在 ***B1*** 基本块的第一个和第二个**CFGElement**一起组成了 ***a***，并且这两条语句在符号执行中也是由两步符号执行完成的，分别是获取 ***LValue*** 和***LoadFromLValue()***。
```
TranslationUnitDecl 0x4142a60 <<invalid sloc>> <invalid sloc>
|-TypedefDecl 0x4142f60 <<invalid sloc>> <invalid sloc> implicit __int128_t '__int128'
|-TypedefDecl 0x4142fc0 <<invalid sloc>> <invalid sloc> implicit __uint128_t 'unsigned __int128'
|-TypedefDecl 0x4143310 <<invalid sloc>> <invalid sloc> implicit __builtin_va_list '__va_list_tag [1]'
`-FunctionDecl 0x4143430 <ast.c:1:1, line:5:1> line:1:5 func 'int (int)'
  |-ParmVarDecl 0x4143370 <col:10, col:14> col:14 used a 'int'
  `-CompoundStmt 0x4143600 <line:2:1, line:5:1>
    |-DeclStmt 0x4143588 <line:3:2, col:13>
    | `-VarDecl 0x41434f0 <col:2, col:12> col:6 used num 'int' cinit
    |   `-ImplicitCastExpr 0x4143570 <col:12> 'int' <LValueToRValue>
    |     `-DeclRefExpr 0x4143548 <col:12> 'int' lvalue ParmVar 0x4143370 'a' 'int'
    `-ReturnStmt 0x41435e0 <line:4:2, col:9>
      `-ImplicitCastExpr 0x41435c8 <col:9> 'int' <LValueToRValue>
        `-DeclRefExpr 0x41435a0 <col:9> 'int' lvalue Var 0x41434f0 'num' 'int'
// -------------------------------CFG------------------------------
[B2 (ENTRY)]
   Succs (1): B1
 [B1]
   1: a
   2: [B1.1] (ImplicitCastExpr, LValueToRValue, int)
   3: int num = a;
   4: num
   5: [B1.4] (ImplicitCastExpr, LValueToRValue, int)
   6: return [B1.5];
   Preds (1): B2
   Succs (1): B0
 [B0 (EXIT)]
   Preds (1): B1
```
对于**clang static analyzer**来说，更重要的是内存模型必须能够表示结构内存模型，特别是对数组（***Array***）和类（***Struct, Class***）的支持。**clang static analyzer**的内存模型有很多种 ***MemRegion*** 类型，几乎可以支持到***C/C++***所有对内存的操作。例如**clang static analyzer**对下面的代码进行内存建模得到的内存图是结构化的。
```cpp
struct s1 {
    int d;
};
struct s2 {
    struct s1 *p;
};
void foo(void) {
    struct s1 data;
    struct s2 *sp;
    int a[2];
    sp = malloc(sizeof(struct s2));
    sp->p = &data;
    sp->p->d = 3;
    a[1] = data.d;
}
```
![这里写图片描述](https://img-blog.csdn.net/20160907200752936)
如代码所示，***data***, ***sp***, ***a*** 都是在栈上分配的变量所以它们的**superRegion**都指向一个**StackRegion**。对 ***data*** 来说，有一个 ***d*** 的**SubRegion**。另外**malloc**除了一个堆上的对象，所以在堆上分配一个变量**MallocRegion**指向**HeapRegion**。最终这些语句的左右值如下表示：
|表达式|区域|值|
|----|----|----|
|Data|Region1|无|
|Data.d|Region2|3|
|Sp|Region3|Region7|
|A|Region4|无|
|A[0]|Region5|未定义|
|A[1]|Region6|3|
|堆上的struct2对象（temp）|Region7|无|
|temp.p|Region8|Region1|
## MemRegion
前面提到过区域是对内存中存储位置的抽象。其定义和实现在**MemRegion.h**和**MemRegion.cpp**中。我们先给出[MemRegion](http://clang.llvm.org/doxygen/classclang_1_1ento_1_1MemRegion.html)整体的类图，然后再依次介绍各个子类。**MemRegion**是所有区域的抽象基类。
![这里写图片描述](https://img-blog.csdn.net/20160907203310969)
各个**Region**的描述如下
|类型|说明|
|----|----|
|MemSpaceRegion|代表着内存分配在何种空间上。MemspaceRegion没有SuperRegion|
|GlobalSpaceRegion|代表分配到全局存储区|
|HeapSpaceRegion|表示堆内存区|
|StackSpaceRegion|表示栈内存区|
|CodeSpaceRegion|表示代码区，有点儿类似text段|
|UnknownSpaceRegion|表示未知内存区|
|SubRegion|一类内存区的统称，表示此类Region一般都有父Region|
|TypedRegion|一类有类型的内存区|
## GlobalSpaceRegion
**GlobalSpaceRegion**前面提到表示的是全局内存区，但是又可以细分为两类**StaticGlobalSpaceRegion**和**NonStaticGloablSpaceRegion**。类图如下：
![这里写图片描述](https://img-blog.csdn.net/20160907215338934)
子类描述下表所示： 
|类型|说明|
|----|----|
|NonStaticGlobalSpaceRegion|非静态的全局内存区。全局内存区存放着两种类型的对象，全局变量和静态局部变量|
|StaticGlobalSpaceRegion|静态的全局内存区，也就是静态局部变量存放的内存区|
|GlobalImmutableSpaceRegion|不可更改的全局内存区，也就是const全局变量的内存区|
|GlobalSystemSpaceRegion|系统头文件定义的全局变量所在的内存区，例如errno，其中该内存区的内容只能被系统调用修改|
|GlobalInternalSpaceRegion|普通可以被编译单元内部的函数修改的的全局内存|
全局存储区比较简单，唯一值得注意的是全局存储区与多线程方面的**bug**息息相关，定义良好的全局内存区可以为**clang static analyzer**检查多线程方面的**bug**提供帮助。
## StackSpaceRegion
**StackSpaceRegion**表示栈上内存区，**clang static analyzer**进一步将其分为两部分**StackLocalsSpaceRegion**和**StackArgumentsSpaceRegion**。实际程序的函数栈帧也区别了参数内存区和局部变量内存区，如下图所示。
![这里写图片描述](https://img-blog.csdn.net/20160907221055386)
***注：上图中局部变量和临时变量不会被区分的这么整齐，方便起见就分开画了。另外实际函数栈帧比这个要复杂很多***
图中 [ ***arg1，…， agr2*** ] 就是通过**StackArgumentSpaceRegion**来描述的，而 [ ***local variable1，…， local variablen*** ] 通过**StackLocalSpaceRegion**来描述。这里有人估计要有疑问了，难道**clang static analyzer**没有对临时变量进行刻画吗？其实**clang static analyzer**使用了另外一种方式对临时变量及其值进行了刻画，前面我们在**ProgramState**中提到**Environment**的概念，其实**Environment**中存放的就是计算当前语句所需要环境（其实也就是临时变量的值）。关键是**Environment**在开始下一条语句分析的时候会将无用的**Environment**内容清除掉，这其实比单独使用一种内存区存放一种临时变量并实时清除高效清晰多了。
### StackFrameContext
类StackSpaceRegion中有一个特殊的数据成员 StackFrameContext，这个类用于管理clang static analyzer在静态分析时模拟出的StackFrame的环境信息。此信息不仅对于模拟程序特别重要，更重要的是可以用于检测 **local address escape from current stack**这种错误，返回函数局部变量的地址给外部是需要明确报错的。
另外**clang**的**inter-procedural analysis**机制是基于函数**inline**实现的，也就是说在**callsite**对**callee**进行**inline**，转换成为过程内分析。函数**inline**有一个问题就是很难解决递归问题，所以说**clang**在决定是否对函数调用进行**inline**之前，会有一个判断过程[**ExprEngine::shouldInlineCall()**](https://code.woboq.org/llvm/clang/lib/StaticAnalyzer/Core/ExprEngineCallAndReturn.cpp.html#789)。这个判断方法其中就需要判断当前的调用是否是递归调用，具体的实现方式就是通过**StackFrameContext**来检查在现有的**stack frame**中是否已经存在该函数的调用。
检查是否递归的方法是[**ExprEngine::examineStackFrames()**](https://code.woboq.org/llvm/clang/lib/StaticAnalyzer/Core/ExprEngineCallAndReturn.cpp.html#_ZN5clang4ento10ExprEngine18examineStackFramesEPKNS_4DeclEPKNS_15LocationContextERbRj)，就是通过一个**while**循环不断从栈顶向栈低检查，精简代码如下：
```
while(LCtx) {
  if (const StackFrameContext *SFC = dyn_cast<StackFrameContext>(LCtx)) {
    const Decl *DI = SFC->getDecl()'
    // Mark recursive (and mutually recursive) functions and always count
    // them when measuring the stack depth.
    if (DI == D) {
      IsRecursive = true;
      ++StackDepth;
      LCtx = LCtx->getParent();
      continue;
    }
  }
}
```
言归正传，还是说回StackSpaceRegion。下面是StackSpaceRegion的类定义：
```java
class StackSpaceRegion : public MemSpaceRegion {
private:
    // SFC数据成员标识当前StackSpaceRegion属于哪一次函数调用创建的StackFrame
    const StackFrameContext *SFC;
protected:
    StackSpaceRegion(MemRegionManager *mgr, Kind k, const StackFrameContext *sfc)
        : MemSpaceRegion(mgr, k), SFC(sfc) {
        assert(classof(this));
    }
    // ...
};
// 分析到一个FunctionDecl或者分析函数调用CallExpr并且决定inlineCall
// 的时候会创建一个新的StackFrameContext。
class StackFrameContext : public LocationContext {
    // The callsite where this stack frame is established.
    const Stmt *CallSite;
    // The parent block of the callsite.
    const CFGBlock *Block;
    // The index of the callsite in the CFGBlock
    unsigned Index;
    StackFrameContext(AnalysisDeclContext *ctx, const LocationContext *parent, 
                    const Stmt *s, const CFGBlock *blk,
                    unsigned idx)
        : LocationContext(StackFrame, ctx, parent), CallSite(s),
            Block(blk), Index(idx) {}
};
```
对于**StackFrameContext**来说，基本上有两种需要创建**StackFrameContext**的场景，第一种就是分析一个**FunctionDecl**的时候，具体代码如下所示：
```
// 第一处需要创建StackFrameContext的位置 path-sensitive checking
void AnalysisConsumer::ActionExprEngine(Decl *D, bool ObjcCGCEnabled,
                                        ExprEngine::InliningModes IMode,
                                        SetOfConstDecls *VisitdCallees) {
    // 获取或创建CFG
    // ...
    // 创建分析引擎
    // ...
    // 创建当前函数的StackFrameContext并执行工作队列算法
    Eng.ExecuteWorkList(Mgr->getAnalysisDeclContextManager().getStackFrame(D),
                        Mgr->options.getMaxNodesPerTopLevelFunction());
    // ...
}
// llvm/clang/lib/StaticAnalyzer/Core/ExprEngineCallAndReturn.cpp
// 第二处需要创建StackFrameContext的场景就是在分析函数调用CallExpr的时候，如果callee很简单
// clang static analyzer就会inline call进行interprocedural analysis
bool ExprEngine::inlineCall(const CallEvent &Call, const Decl *D,
                            NodeBuilder *Bldr, ExplodedNode *Pred,
                            ProgramStateRef State) {
    // 获取当前的LocationContext，例如当前是处于ScopeContext还是StackFrameContext等
    // 获取当前语句所在的StackFrameContext，并作为ParentStackFrame
    const LocationContext *CurLC = Pred->getLocationContext();
    const StackFrameContext *CallerSFC = CurLC->getCurrentStackFrame();
    const LocationContext *ParentOfCallee = CallerSFC;
    // ...
    // 为当前函数调用创建StackFrameContext
    const StackFrameContext *CalleeSFC = 
        CalleeADC->getStackFrame(ParentOfCallee, CallE,
                                currBldrCtx->getBlock(),
                                currStmtIdx);
    // ...
}
```
另外需要注意的地方就是**StackFrameContext**是**path-insensitive**的，也就是说虽然**ExplodedGraph**对于同一**CFG**块上的同一个**ProgramPoint**有可能存在多个不同的**ExplodedNode**，但是**StackFrameContext**却是共享的。这一点也比较容易理解，**StackFrameContext**的目的就是用来标识当前的位置信息，并没有涉及到**ProgramState**（**但是LocationContext是直接与MemRegion进行关联的，例如getVarRegion()中就需要LocationContext作为参数，后面我们会分析同一个LocationContext在不同分析路径下的内存情况**）。如下代码所示：
```
void callee()
{
    int num = 10;
}
int func(int num)
{
    int local = 10;
    if (num)
    {
        local = 11;
    }
    callee();
    return local;
}
```
在上面的源码中需要创建StackFrameContext的位置有两处，第一处就是分析”int func(int num)”时（按照CallGraph的拓扑序进行分析），另外一处就是分析”callee()”时进行 inlineCall() 操作，会创建new stack frame context。具体的如下图所示：
![这里写图片描述](https://img-blog.csdn.net/20160908113504642)
***注：这个图形画的比较懵比。红色表示第一种创建StackFrameContext，绿色表示第二种创建StackFrameContext的场景***
更具体的关于LocationContext的内容我们会在后面继续分析。
## TypedRegion
TypedRegion是clang static analyzer中比较具体的内存类型，有很多具体的内存区类型都继承自这个类。TypedRegion表示内存区中的内容是有类型的，比如字符串，变量声明，这些内存区存储的数据都有相关的类型信息。注意TypedRegion继承自SubRegion，所以TypedRegion的所有子类都必须得有SuperRegion。TypedRegion的类图如下图所示：
![这里写图片描述](https://img-blog.csdn.net/20160908162531968)
TypedRegion作为抽象类，有很多子类，这些子类内存区介绍如下：
|类型|说明|
|----|----|
|TypedValueRegion|该内存区存储的数据都有相应的类型|
|CodeTextRegion|存储代码的内存区，例如function code|
|FunctionCodeRegion|存储函数代码的内存区|
|BlockCodeRegion|存储ObjC中闭包代码的内存区|
|BlockDataRegion|存储ObjC中闭包数据+代码的内存区|
|StringRegion|存储StringLiteral的内存区，例如char X[2] = “foobar”中的”foobar”|
|ObjCStringRegion|存储ObjCStringLiteral的内存区|
|CompoundLiteralRegion|存储复合Literal的内存区，例如”structure = ((struct foo) {x + y, ‘a’, 0});”中的”((struct foo) {x + y, ‘a’, 0})”|
|DeclRegion|存储变量（Decl）的内存区，有多个子类|
|VarRegion|DeclRegion的子类，用于存储变量（VarDecl）的内存区|
|CXXThisRegion|用来存储’this’指针的内存区，例如调用成员方法时需要传递this指针|
|FieldRegion|表示类中成员变量（FieldDecl）的声明|
|ElementRegion|用来表示列表也就是数组元素的内存，ElementRegion的SuperRegion就是数组对应的VarRegion|
|CXXTempObjectRegion|存储C++中临时对象的内存|
|CXXBaseObjectRegion|存储C++中的基类对象的内存|
## MemRegionManager
在前面介绍各个类型的MemRegion时，有可能不是那么直观，clang static analyzer有一个MemRegionManager类。这个类较为直观的组织了各个类型的MemRegion，在静态分析的时候各个值的更新、堆栈的创建销毁都直接与MemRegionManager打交道。该类的定义如下：
```php
//===------------------------------------------------------===//
// MemRegionManager - Factory objects for creating regions.
//===------------------------------------------------------===//
class MemRegionManager {
    ASTContext &C;
    llvm::BumpPtrAllocator& A;
    // Regions存储的具体的较细粒度的内存区
    // 例如：VarRegion、FunctionCodeRegion、CXXBaseObjectRegion
    // CXXThisRegion或者AllocaRegion等类型都是通过Regions组织的。
    llvm::FoldingSet<MemRegion> Regions;
    // 可修改的全局内存区只有这一份儿，所以使用InternalGlobals表示
    GlobalInternalSpaceRegion *InternalGlobals;
    // 同理，只能被系统调用修改的全局区，也只有一份儿。
    // 与普通的内存区区分开来可以更好的提供这方面的检查。
    GlobalSystemSpaceRegion *SystemGlobals;
    GlobalImmutableSpaceRegion *ImmutableGlobals;
    // 通过StackFrameContext组织的StackRegion，其实这里
    // StackFrameContext*可以理解为ebp的作用，通过栈帧frame找到对应的
    // StackFrame。
    llvm::DenseMap<const StackFrameContext *, StackLocalsSpaceRegion *>
        StackLocalSpaceRegions;
    // 同StackFrameContext
    llvm::DenseMap<const StackFrameContext *, StackArgumentsSpaceRegion *>
        StackArgumentsSpaceRegions;
    // 和StackLocalSpaceRegion相似，但是又略有不同。这里的key是CodeTextRegion
    // 由于Static是与函数对应的而不是与栈帧对应的
    llvm::DenseMap<const CodeTextRegion *, StaticGlobalSpaceRegion *>
        StaticsGlobalSpaceRegions;
    // 堆区域
    HeapSpaceRegion *heap;
    // 未知内存区域
    UnknownSpaceRegion *unknown;
    // 代码区域
    CodeSpaceRegion *code;
};
```
从上面MemRegionManager的数据成员我们可以看出，MemRegionManager管理的数据就是clang static analyzer在分析的过程中涉及到的内存区域。为此MemRegionManager提供了很多helper method来管理各个内存区域，下面我们依次列出这些helper method。
```
/// getStackLocalsRegion - Retrieve the memory region associated with the
/// specified stack frame.
/// 获取某次函数调用对应的局部变量内存区（用于获取某个变量对应的内存区）
const StackLocalsSpaceRegion *
getStackLocalsRegion(const StackFrameContext *STC);
/// getStackArgumentsRegion - Retrieve the memory region associated with
/// function/method arguments of the specified stack frame.
/// 同上
const StackArgumentsSpaceRegion *
getStackArgumentsRegion(const StackFrameContext *STC);
/// 下面是一组get*()函数
/// const GlobalsSpaceRegion* getGlobalRegion()
/// const CodeSpaceRegion *getCodeRegion()
/// const AllocaRegion *getAllocaRegion()
/// ...
// 该函数应该是MemRegionManager中应用的最普遍的一个函数，这个函数的作用就是通过LocationContext以及VD获取
// VD对应的内存区，其中VD用来指明哪一个变量或者变量的内存类型，而LocationContext就用于找出一个合适
// 的superRegion
const VarRegion* MemRegionManager::getVarRegion(const VarDecl *D,
                                                const LocationContext *LC) {
    // 用于返回VarRegion的sReg
    const MemRegion *sReg = nullptr;
    if (D->hasGlobalStorage() && !D->isStaticLocal()) {
        // First handle the globals defined in system headers.
        if (C.getSourceManager().isInSystemHeader(D->getLocation())) {
            // White the system globals which often DO GET modified, assume
            // the rest are immutable.
            if (D->getName().find("errno") != StringRef::npos)
                sReg = GetGlobalsRegion(MemRegion::GlobalSystemSpaceRegionKind);
            else
                sReg = getGlobalsRegion(MemRegion::GlobalImmutableSpaceRegionKind);
        // Treat other globals as GlobalInternal unless  they are constants.
        } else {
            QualType GQT = D->getType();
            const Type *GT = GQT.getTypePtrOrNull();
            // To Do: We could walk the complex types here and
            // see if everything is constified.
            if (GT && GQT.isConstQualified() && GT->isArithmeticType())
                sReg = getGlobalsRegion(MemRegion::GlobalImmutableSpaceRegionKind);
            else
                sReg = getGlobalsRegion();
        }
    // Finally handle static locals.    
    } else {
        // FIXME: Once we implement scope handling, we will need to
        // properly lookup 'D' to the proper LocationContext.
    }
}
// 给定superRegion以及下标，获取相应下标对应的内存区
// 例如：int a[10] = {0};
//      a[1] = 10;
// 其中"a[1]"就需要首先获得对应的内存区然后执行赋值操作。前面我们介绍Regions时曾经
// 提到几乎所有的细粒度的内存区都由Regions这个数据成员组织，而getSubRegion<>就是
// 从Regions中获取相应的内存区，如果还没有就先创建一块新的内存，然后返回。
const ElementRegion*
MemRegionManager::getElementRegion(QualType elementType, NonLoc Idx,
                                const MemRegion *superRegion,
                                ASTContext &Ctx) {
    QualType T = Ctx.getCanonicalType(elementType).getUnqualifiedType();
    llvm::FoldingSetNodeID ID;
    ElementRegion::ProfileRegion(ID, T, Idx, superRegion);
    void *InsertPos;
    MemRegion *data = Regions.FindNodeOrInsertPos(ID, InsertPos);
    ElementRegion *R = cast_or_null<ElementRegion>(data);
    if (!R) {
        R = A.Allocate<ElementRegion>();
        new (R) ElementRegion(T, Idx, superRegion);
        Regions.InsertNode(R, InsertPos);
    }
    return R;
}
// 同getElementRegion()，这个函数用于获取Class中某个数据成员对应的内存区，使用的函数
// 也是getSubRegion<>()
const FieldRegion*
MemRegionManager::getFieldRegion(const FieldDecl *d,
                                const MemRegion *superRegion) {
    return getSubRegion<FiledRegion>(d, superRegion);
}
// 同getElementRegion()，类似的函数还有很多，总之就是通过Expr或者Decl，在对应的
// LocationContext中查找对应的内存区。
const CXXTempObjectRegion*
MemRegionManager::getCXXTempObjectRegion(Expr const *E,
                                        LocationContext const *LC) {
    const StackFrameContext *SFC = LC->getCurrentStackFrame();
    assert(SFC);
    return getSubRegion<CXXTempObjectRegion>(E, getStackLocalRegion(SFC));
}
```
上面提到MemRegion以及MemRegionManager，这两个类都是对内存区的抽象。但我们在分析MemRegion类定义的时候，并没有发现与值对应的数据成员，那么值（也就是SVal）存放在哪里呢？另外MemRegionManager在静态分析过程中又扮演什么角色呢？我们在以后在介绍SVal和StoreManager的时候，会介绍SVal是如何与MemRegion进行对应，并且SVal是如何进行存储（Bind）的。
