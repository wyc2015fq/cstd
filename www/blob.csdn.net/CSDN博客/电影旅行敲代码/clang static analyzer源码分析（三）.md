# clang static analyzer源码分析（三） - 电影旅行敲代码 - CSDN博客
2016年09月05日 22:07:17[电影旅行敲代码](https://me.csdn.net/dashuniuniu)阅读数：2325
## 引子
在[clang static analyzer源码分析（二）](http://blog.csdn.net/dashuniuniu/article/details/52434781)中我们简要介绍了**ExplodedGraph**以及如何调试**clang static analyzer**。今天这篇文章重点分析一下**clang static analyzer**对于**path-sentitive analysis**的代码架构。
## path-sensitive analysis的源码架构
clang static analyzer关于path-sensitive analysis的源码结构如下： 
***note:clang 3.6***
```
- PathSensitive/
  APSIntType.h
  AnalysisManager.h
  BasicValueFactory.h
  BlockCounter.h
  CallEvent.h
  CheckerContext.h
  CheckerHelpers.h
  ConstraintManager.h
  CoreEngine.h
  DynamicTypeInfo.h
  Environment.h
  ExplodedGraph.h
  ExprEngine.h
  FunctionSymmary.h
  MemRegion.h
  ProgramState_Fwd.h
  ProgramState.h
  Store.h
  StoreRef.h
  SubEngine.h
  SummaryManager.h
  SValBuilder.h
  SVals.h
  SymbolManager.h
  TaintManager.h
  TaintManager.h
  WorkList.h
```
下面我们依次介绍各个文件主要内容。
### APSIntType.h
这个文件主要定义了 APSInt 类，这个类用于在静态分析的时候表示各种不同类型的Integer（常量）。
> 
This file implements the APSInt class, **which is a simple class that represents an arbitrary sized integer that knows its signedness.**
这个类继承自 llvm::APInt，其中APInt的直观解释就是 “**arbitrary precision integers**“，主要用于unsigned integers，所以APSInt就表示 “**arbitrary precision integers singed or unsigned**“。
### AnalysisManager.h
这个文件声明了**AnalysisManager**类，这个类前面我们也提到过，含有多个和静态分析的核心数据成员，例如**CheckerManager**、**DiagnosticsEngine**以及**ASTContext**等。然后提供了一些**helper**方法。
### BasicValueFactory.h
这个文件定义了BasicValueFactory.h类，这个类可以说是引擎的整数池，其中存放了在静态分析过程中得到的APSInt。其中还定义了一系列的helper方法，例如根据一个int值通过”**getValue(uint64_t X, QualType T)**“获取一个APSInt对象，以及根据AST类型QualType通过”**getMaxValue(QualType T)**“获取该类型的最大值以及最小值等。
在静态分析过程中整形的边界以及类型非常重要，有时会涉及到非常严重的报告，例如”**malloc(-1)**” 这会分配很大一块内存。或者是”**while(s < 300)**“中，s是char类型，这回导致死循环。
```
typdef llvm::FoldingSet<llvm::FoldingSetNodeWrapper<llvm::APSInt>> 
    APSIntSetTy;
APSIntSetTy APSIntSet;
```
### BlockCounter.h
这个文件定义了**BlockCounter**类，这个类主要用于记录沿一条路径上，某一基本块总共执行了多少次。
> 
This file defines BlockCounter, an abstract data type used to count the number of times a given block has been visited **along a path** analyzed by CoreEngine.
### CallEvent.h
这个文件定义了静态分析过程中的函数调用，由于在**ExplodedGraph**中，同一个函数调用在不同的**ExplodedNode**中算作两种不同的调用，因为调用的**Context**不相同。
另外，该文件还针对多态定义了一些其他的数据结构。具体的内容等到剖析到**clang static analyzer**如何符号执行函数调用时在详细介绍。
> 
This file defines CallEvent and its subclasses, **which represents path-sensitive instances** of different kinds of function and method calls(C, C++ and Objective-C).
### CheckerContext.h
该文件定义了**CheckerContext**类，该类用于给**Checker**提供上下文信息。随便挑一个**Checker**，打开源码都可以看到有**CheckerContext**的影子，例如我们打开**DivZeroChecker.cpp**，其中有一个回调函数是”**checkPreStmt(const BinaryOperator *B, )**“，该函数会在分析引擎分析到**BinaryOperator**之前的时候，调用该回调函数。该函数其中有一个参数是**CheckerContext**，这个参数基本上可以提供到所有分析要用的信息。例如在获取**BinaryOperator**右侧符号值的时候就需要用到**CheckerContext**，另外在下面也需要通过**CheckerContext**获得**ConstrainManager**。
```
// ...
void DivZeroChecker::checkPreStmt(const BinaryOperator *B,
                    CheckerContext &C) const {
    BinaryOperator::Opcode Op = B->getOpcode();
    if (Op != BO_Div &&
        Op != BO_Rem &&
        Op != BO_DivAssign &&
        Op != BO_RemAssign)
        return;
    if (!B->getRHS->getType()->isScalarType())
        return;
    SVal Denom = C.getState()->getSVal(B->getRHS(), C.getLocationContext());
    Optional<DefinedSVal> DV = Denom.getAs<DefinedSVal>();
    // Divied-by-undefined handled in the generic checking for uses of
    // undefined values.
    if (!DV)
        return;
    // Check for divide by zero.
    ConstraintManager &CM = C.getConstraintManger();
}
// ...
```
### ConstraintManager.h
该文件定义了**ConstraintManager**类，这个类可以说是**clang staic analyzer**中最重要的一个类，这个类主要用于管理**SymExpr**的限制条件。例如在对**Branch**进行符号执行的时候，需要对未知的符号值进行约束，**ConstraintManager**提供了很多**assume*()**方法对符号值进行约束，如果得到了新的约束，则将该约束注册到**ProgramState**中。
例如**ConstraintManager**中的**assumeSymEQ()**方法部分源码如下，如果能够对符号值约束出新的区间，则将这个结果注册到**ProgramState**中，如最后一行**return**语句。
```cpp
ProgramStateRef
RangeConstraintManager::assumeSymEQ(ProgramStateRef, SymbolRef Sym,
                                    const llvm::APSInt &Int,
                                    const llvm::APSInt &Adjustment) {
    // Before we do any real work, see if the value can even show up.
    APSIntType AdjustmentType(Adjustment);
    if (AdjustmentType.testInRange(Int, true) != APSIntType::RTR_Within)
        return nullptr;
    // [Int-Adjustment, Int-Adjustment]
    llvm::APSInt AdjInt = AdjustmentType.convert(Int) - Adjustment;
    RangeSet New = GetRange(St, Sym).Intersect(getBasicVals(), F, AdjInt, AdjInt);
    return New.isEmpty() ? nullptr : St->set<ConstraintRange>(Sym, New);
}
```
关于**ConstraintManager**，我们会在后面进行详细的分析。
### CoreEngine.h
该文件定义**CoreEngine**类，这个类是符号执行的核心类，这个类主要在**CFG**上进行符号执行，并实时构建出**ExplodedGraph**。这个类我们会在后面作详细介绍。
> 
This file defines a generic engine for **intraprocedural, path-sensitive, dataflow analysis via graph reachability**.
### Environment.h
**Environment**负责存储当前程序点所能引用到的表达式到其对应的符号值的映射关系。
```matlab
if (func(x) + 10 > a) {} else {}
```
例如在符号执行 “***func(x) + 10 > a***“的时候，需要首先获取到”***func(x)***“的值以及变量a的值，此时 { “***func(x) -> SVal***“, “***a -> SVal***“} 就是在evaluate “***func(x) + 10***” > “***a***” 这条语句时所需要的**environment**，但是在符号执行下一条语句之前会将上一条语句的**environment**清除掉。其实这个**environment**的声明周期只存在于一条语句之内，并且是路径敏感的。详细的我们会在后面继续介绍。
### ExplodedGraph.h
该文件定义了两个类，**ExplodedNode**和**ExplodedGraph**，我们在前一篇文章中介绍到过**exploded graph**是**clang static analyzer**在**CFG**上进行符号执行的“结果”。
> 
This file defines the template classes ExplodedNode and ExplodedGraph, **which represents a path-sensitive, intra-procedural “exploded graph.”**
后面我们会详细介绍**ExplodedNode**和**ExplodedGraph**这两个类。
### ExprEngine.h
该文件定义了**ExprEngine**类，这个类构建在**CoreEngine**上，**ExprEngine**类定义了很多**transfer function**。
- void ProcessStmt(const CFGStmt S, ExplodedNode *Pred);
- void ProcessInitializer(const CFGInitializer I, ExplodedNode *Pred);
- void ProcessImplicitDtor(const CFGImplicitDtor D, ExplodedNode *Pred);
- void ProcessNewAllocator(const CXXNewExpr *NE, ExplodedNode *Pred);
- …
相应的**transfer function**有很多，这里就不一一列举了。
### FunctionSymmary.h
该文件定义了**FunctionSummariesTy**类，这个类定义了关于函数的摘要信息，只是这个摘要信息单单用于静态分析中函数信息的记录。
> 
This file defines a summary of a function gathered/used by static analysis.
```
// ...
/// Marks the IDs of the basic blocks visited during the analyzes.
llvm::SmallBitVector VisitedBasicBlocks;
/// Total number of blocks in the function.
unsigned TotalBasicBlocks : 30;
/// True if this function has been checked against the rules for which
/// functions may be inlined.
unsigned InlineChecked : 1;
unsigned MayInline : 1;
/// The number of times the function has been inlined.
unsigned TimesInlined : 32;
// ...
```
关于Function Summary信息的记录存放在一个map中，该map存储着当前TU所有的FunctionDecl对应的摘要信息。
```
class FunctionSummariesTy {
  typedef llvm::DenseMap<const Decl*, FunctionSummary> MapTy;
  MapTy Map;
};
```
该摘要信息在静态分析的过程中，特别是在决定是否对该函数进行inline的时候，起着关键性的作用。
### MemRegion.h
该文件定义了对内存位置进行抽象的一些类，例如**MemRegion**、**SubRegion**、**AllocaRegion**、**MemSpaceRegion**以及**StackSpaceRegion**等。后面我们会分析**clang static analyzer**的内存模型。
```
MemRegion - The root abstract class for all memory regions.
MemSpaceRegion - A memory region that represents a "memory space";for example, the set of global variables, the stack frame, etc.
NonStaticGlobalSpaceRegion - The region for all the non-static global variables.
// ...
```
### ProgramState.h
该文件定义了**ProgramState**类，表示在程序分析过程中的程序状态。
```
/// \class ProgramState
/// ProgramState - This class encapsulates:
///     1. A mapping from expressions to values (Environment);
///     2. A mapping from locations to values(Store);
///     3. Constraints on symbolic values (GenericDataMap)
/// Together these represent the "abstract state" of a program.
```
**Environment**是当前状态所处的环境，**store**是当前状态下所存储的**SVal**，**GenericDataMap**存储用户定义的数据。
### Store.h
该文件定义**StoreManager**类，这个类用于在进行静态分析的时候，对存储进行相应的修改，例如创建新的**MemRegion**、删除**DeadBinding**或者是在进入新的**StackFrame**的时候（也就是**analysis CallEvent**）都需要调用**StoreManager**中相应的接口。
```
/// Return the value bound to specified location in a given state.
/// \param[in] store The current store.
/// \param[in] loc The symbolic memory location.
/// \param[in] T An optional type that provides a hint indicating the 
///     expected type of the returned value. This is used if the value is
///     lazily computed.
/// \return The value bound to the location \c loc.
virtual SVal getBinding(Store store, Loc loc, QualType T = QualType()) = 0;
virtual StoreRef Bind(Store store, Loc loc, SVal val) = 0;
virtual StoreRef killBinding(Store ST, Loc L) = 0;
/// ...
```
### StoreRef.h
该文件定义了**StoreRef**类，该类是内存**Store**的包装类。**Store**是通过**llvm::ImmutableMap**实现的，而**llvm::ImmutableMap**是通过**AVLTree**实现的。
```
class StoreRef {
    Store store;
    StoreManager &mgr;
    // ...
};
```
### SValBuilder.h
该文件定义了**SValBuilder**类，这个类主要用于符号执行（**symbolical evaluators**），根据**SVals**以及相应的运算规则构建新的**SVal**。注意：**SymExpr**不是通过**SValBuilder**构建的。
> 
This file defines SValBuilder, **a class that defines the interface for “symbolical evaluators” which construct an SVal from an expression**.
**SValBuilder**是**clang static analyzer**中为数不多的根据当前值构建新值（其实就是符号执行）的类，**SValBuilder**提供了丰富的***eval*()***方法，来根据值以及值的类型创建新的值。由于在构建CFG的过程中，已经将**Expression**拆分成**BinaryOperator**了（除了自增、自减以及类型转换运算），所以运算几乎都是二元运算。
```
SVal evalCast(SVal val, QualType castTy, QualType originType);
// Handles casts of type CK_IntegeralCast
SVal evalIntegralCast(ProgramStateRef state, SVal val, QualType castTy,
                        QualType originalType);
virtual SVal evalMinus(NonLoc val) = 0;
/// Create a new value which represents a binary expression with two non-location operands.
virtual SVal evalBinOpNN(ProgramStateRef state, BinaryOperator::Opcode op,
                        NonLoc lhs, NonLoc ths, QualType resultTy) = 0;
/// Create a new value which represents a binary expression with two memory
/// location operands.,
virtual SVal evalBinOpLL(ProgramStateRef state, BinaryOperator::Opcode op,
                        Loc lhs, Loc rhs, QualType resultTy) = 0;
/// ...
```
与**eval*()**类具有同等重要性的方法是**assume*()**方法，[assume*()](https://code.woboq.org/llvm/clang/lib/StaticAnalyzer/Core/SimpleConstraintManager.cpp.html#_ZN5clang4ento23SimpleConstraintManager9assumeAuxEN4llvm18IntrusiveRefCntPtrIKNS0_12ProgramStateEEENS0_6NonLocEb)方法和**ConstraintManager**紧密相关，主要用于对于一个条件表达式进行求解，判断该条件表达式能否约束到**true**分支的状态或者**false**分支的状态。
***注：clang static analyzer没有专门的SMT solver，而是在符号执行的过程中，将约束添加到变量上（range set），然后使用一系列的assume*()方法来进行求解。只是assume*()很简单，有很大的限制，具体参见[canReasonAbout()](https://code.woboq.org/llvm/clang/lib/StaticAnalyzer/Core/SimpleConstraintManager.cpp.html#_ZNK5clang4ento23SimpleConstraintManager14canReasonAboutENS0_4SValE)。但是貌似可以在options上附加自定义的SMT，可惜我没有尝试过（其实是不懂），就不过多介绍了***
> 
**Our current constraint solver is fairly simple, keeping sets of possible (scalar) values for symbols and applying a set-union each time a new constraint is added.** The implementation uses sets of ranges as a fairly compact way to represent an accumulation of constraints on integers.  –[Clang-DEV](http://clang-developers.42468.n3.nabble.com/Static-Analyzer-symbolic-expressions-in-a-program-td4030798.html#a4049002)
### SVals.h
该类定义了**SVal**，**Loc**以及**NonLoc**等类，这些类用于表达静态分析过程中的得到的值，例如变量值，内存的地址值或者未知的符号值。这些类可以说是静态分析过程中的血肉，后面我们会详细介绍这些类。
### SymbolManager.h
该文件定义了一些符号类，以及符号值管理器**SymbolManager**和无用符号清除器**SymbolReaper**。在分析过程中会得到很多符号类，比如：**SymbolCast**、**BinarySymExpr**以及**IntSymExpr**等。同样，后面再详细介绍这些类。
```
class SymbolManager {
    // ...
    typedef llvm::FoldingSet<SymExpr> DataSetTy;
    DataSetTy DataSet;
    // ...
};
/// \brief A class responsible for cleaning up unused symbols.
    enum SymbolStatus {
        NotProcessed,
        HaveMarkDependents
    };
    typedef llvm::DenseSet<SymbolRef> SymbolSetTy;
    SymbolMapTy TheLiving;
    SymbolMapTy TheDead;
    // ...
}
```
### WorkList.h
**clang static anlayzer**在遍历**CFG**时，使用工作队列算法（**worklist algorithm**）分析可达性并扩展**ExplodedGraph**。**WorkList.h**定义了**WorkListUnit**类以及**WorkList**类，其中WorkList类是一个虚基类，被**DFS**、**BFS**以及**BFSBlockDFSContents**三个类继承，这三个派生类表示了不同**CFG**遍历规则。
## path-sensitive dataflow analysis. How?
该标题是一个[技术问答](http://lambda-the-ultimate.org/node/1903)，这是一个对**path-sensitive analysis**感兴趣的beginner提出来的（我连**beginner**都算不上），他提到很多的书籍对**dataflow analysis**都有介绍，但是鲜有书籍对**path-sensitive analysis**进行详细的介绍。
***这里我又要无耻地做个搬运工了。***
Making a DFA path-sensitive is pretty easy conceptually, and of a PITA in practice.
***注：PITA - pain in the ass…***
So, in a path-insensitive DFA, you build a control-flow graph that tells you which basic block can transfer control to which other ones. Then, in your flow functions for the analysis, you take the meet over all the predecessors of a node in order to compute the value for the current node.
To make the data flow analysis path-sensitive, **what you need to do is label each edge in the CFA with a test expression under which it will transfer control**. So if you have code like
```
L1: ...
    ...
    if x = 5 then goto L2 else goto L3
```
then you will have a control flow graph with at least:
```
L1
            |
 +----------+----------+
 |                     |
 | x = 5               | x != 5
 |                     |
 L2                    L3
```
Then you modify your flow function in the DFA. **When computing the predecessor/successor of a node to take a meet over, you use symbolic execution on the edge’s test expression to figure out whether the edge is possible or not** – if you can show it’s false, then you know that the edge can’t really be taken, and you can leave it out of the set of nodes to do the meet over.
如果修改为下面的样子，我们可以清楚的观察到哪些路径可行，哪些不可行。
```
extern int global;
int func()
{
    if (global > 10)
    {
        if (global < 10)
            return 0;
        else
            return global - 10;
    }
    else
    {
        return 0;
    }
}
```
上面的***global*** 是外部变量，无法推断其值，所以假定其为**int**类型值区间内的任意值。
```
Entry
                                  |
                                  |
                     +------------+------------+
                     |                         |
                     | global > 10             | global <= 10
 +-------------------+------------------+      |
 |                                      |      |
 | global < 10 && global > 10           |      |
 | infeasible                           |      |
 |                                      |      |
return 0                   return global - 10 |
                                               |
                                            return 0
```
上面的示意图中，***global*** 是外部变量，所以可以对其进行任何假设，然后在**true**分支对***global*** 约束为 **(10, INT_MAX ]**，在**else**分支对***global*** 约束为 **[INT_MIN, 10]**。这些约束在**clang static analyzer**中就是通过**ConstraintManager**管理的，而***global*** 的地址和值都是**SVal**，在分支的地方进行 ***assume*()***，例如假设 “**global < 10 && global > 10**” 是否成立。
另外在**clang static analyzer**中，默认是通过 ***DFS*** 来对**CFG**进行扩展的，先对某一路径探索完成或者达到***steps*** 的上限，然后才会遍历另外一条路径。
后面我们会针对性的介绍**clang static analyzer**的核心类以及对一些常见情况的分析。
