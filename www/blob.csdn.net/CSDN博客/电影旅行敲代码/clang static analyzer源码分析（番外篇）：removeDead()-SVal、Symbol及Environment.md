# clang static analyzer源码分析（番外篇）：removeDead() - SVal、Symbol及Environment - 电影旅行敲代码 - CSDN博客
2016年11月22日 22:33:45[电影旅行敲代码](https://me.csdn.net/dashuniuniu)阅读数：1374
# 引子
在《[**clang static analyzer**源码分析（番外篇）：**RegionStore**以及**evalCall()**中的**conservativeEvalCall**](http://blog.csdn.net/dashuniuniu/article/details/52849373)》中，我们介绍了**RegionStore**以及**cluster analysis**，里面我们提到除了**invalidate regions**会使用到**cluster analysis**外，**remove dead**也需要**cluster analysis**。除此之外，我们在**clang**中的活跃性分析中，也提到了**remove dead**会使用活跃性分析的结果。虽然我们在以前的文章中对**remove dead**提到过多次，但是从没有详细介绍**remove dead**的机制。因此在这篇文章中，我们将会详细介绍**remove dead**的各个方面。
# **[ExprEngine::removeDead()](https://code.woboq.org/llvm/clang/lib/StaticAnalyzer/Core/ExprEngine.cpp.html#_ZN5clang4ento10ExprEngine10removeDeadEPNS0_12ExplodedNodeERNS0_15ExplodedNodeSetEPKNS_4StmtEPKNS_15LocationContextES8_NS_12ProgramPoint4KindE)**
我们在《[**clang static analyzer**源码分析（番外篇）：**evalCall()**中的**inline**机制](http://blog.csdn.net/dashuniuniu/article/details/52830592#t13)》这篇文章中提到了一个选项**AnalysisPurgeOpt**，这个选项默认清除**DeadSymbol**的粒度为单个**Stmt**，这个在源码中也有所体现，具体体现在**clang**静态分析引擎在处理**Stmt**时的方法中。
```
void ExprEngine::ProcessStmt(const CFGStmt S,
                            ExplodedNode *Pred) {
    // ..
    // Remove dead bindings and symbols.
    ExplodedNodeSet CleanedStates;
    if (shouldRemoveDeadBindings(AMgr, S, Pred, Pred->getLocationContext())){
        removeDead(Pred, CleanedStates, currStmt, Pred->getLocationContext());
    } else
        CleanedStates.Add(Pred);
    // ..
}
```
方法**ProcessStmt()**的第一步就是**remove dead**，也就是对**Environment**、**RegionStore**以及**Constraint**进行清理。由于在**clang**中**Expr**的基类也是**Stmt**，那么**ProcessStmt()**对应用于哪些**Stmt**呢？这里我们列出在**clang**的**CFG**中，哪些类型的**AST**结点可以被看作**Stmt**。下面是在创建**CFG**时，遍历到哪种类型**AST**结点时会将其看做**Stmt**添加到**CFG**中。
```
(anonymous namespace)::CFGBuilder::addInitializer (c)
(anonymous namespace)::CFGBuilder::VisitStmt (c)
(anonymous namespace)::CFGBuilder::VisitAddrLabelExpr (c)
(anonymous namespace)::CFGBuilder::VisitUnaryOperator (c)
(anonymous namespace)::CFGBuilder::VisitLogicalOperator (c)
(anonymous namespace)::CFGBuilder::VisitBinaryOperator (c)
(anonymous namespace)::CFGBuilder::VisitBinaryOperator (c)
(anonymous namespace)::CFGBuilder::VisitBinaryOperator (c)
(anonymous namespace)::CFGBuilder::VisitNoRecurse (c)
(anonymous namespace)::CFGBuilder::VisitCallExpr (c)
(anonymous namespace)::CFGBuilder::VisitCallExpr (c)
(anonymous namespace)::CFGBuilder::VisitChooseExpr (c)
(anonymous namespace)::CFGBuilder::VisitConditionalOperator (c)
(anonymous namespace)::CFGBuilder::VisitDeclSubExpr (c)
(anonymous namespace)::CFGBuilder::VisitForStmt (c)
(anonymous namespace)::CFGBuilder::VisitMemberExpr (c)
(anonymous namespace)::CFGBuilder::VisitObjCForCollectionStmt (c)
(anonymous namespace)::CFGBuilder::VisitObjCAtSynchronizedStmt (c)
(anonymous namespace)::CFGBuilder::VisitPseudoObjectExpr (c)
(anonymous namespace)::CFGBuilder::VisitWhileStmt (c)
(anonymous namespace)::CFGBuilder::VisitUnaryExprOrTypeTraitExpr (c)
(anonymous namespace)::CFGBuilder::VisitStmtExpr (c)
(anonymous namespace)::CFGBuilder::VisitSwitchStmt (c)
(anonymous namespace)::CFGBuilder::VisitCXXCatchStmt (c)
(anonymous namespace)::CFGBuilder::VisitCXXBindTemporaryExpr (c)
(anonymous namespace)::CFGBuilder::VisitCXXConstructExpr (c)
(anonymous namespace)::CFGBuilder::VisitCXXNewExpr (c)
(anonymous namespace)::CFGBuilder::VisitCXXDeleteExpr (c)
(anonymous namespace)::CFGBuilder::VisitCXXFunctionalCastExpr (c)
(anonymous namespace)::CFGBuilder::VisitCXXTemporaryObjectExpr (c)
(anonymous namespace)::CFGBuilder::VisitImplicitCastExpr (c)
```
但并不是静态分析到上述**CFGElement**时都会调用**ExprEngine::removeDead()**方法，就像**ProcessStmt()**方法中显示的那样，只有在**shouldRemoveDeadBindings()**返回**true**的时候才会调用**removeDead()**方法，下面是该方法的判断逻辑。
```
static bool shouldRemoveDeadBindings(AnalysisManager &AMgr,
                        const CFGStmt S,
                        const ExplodedNode *Pred,
                        const LocationContext *LC) {
    // Are we never purging state values?
    if (AMgr.options.AnalysisPurgeOpt == PurgeNone)
        return false;
    // Is this the beginning of a basic block?
    // 如果是BasicBlock入口，则返回TRUE
    if (Pred->getLocation().getAs<BlockEntrance>())
        return true;
    // Is this on a non-expression?
    // 注：这里显示只要下一条语句是Stmt，则直接返回TRUE，也就是调用removeDead()
    if (!isa<Expr>(S.getStmt()))
        return true;
    // Run before processing a call.
    // 函数调用入口则返回TRUE
    if (CallEvent::isCallStmt(S.getStmt()))
        return true;
    // Is this an expression that is consumed by another expression? Is so,
    // postpone cleaning out the state.
    // 如果是Expr类型，则需要判断该Expr值是否会被其他Expr使用。如果会被consumed，则推迟remove dead。
    // 试想：在CFG上进行静态分析时，遇到任何Expr都要调用remove dead，效率过低。
    ParentMap &PM = LC->getAnalysisDeclContext()->getParentMap();
    return !PM.isConsumedExpr(cast<Expr>(S.getStmt()));
}
```
由于**purge**的粒度默认是以**Stmt**为单位的，这也是在效率和空间的一个平衡，如果**purge**太过频繁进行**remove dead**（有可能上一个数量级），那么效率受到很大影响，另外空间上的收益并不会增加很多。另外**shouldRemoveDeadBindings()**也算是控制**remove dead**粒度的另外一种途径。
[***Jordan Rose***](http://clang-developers.42468.n3.nabble.com/Static-Analyzer-Query-regarding-how-symbols-are-marked-as-dead-td4030915.html#a4030932) 对于**removeDead()**有一个很精要的介绍，如下：
> 
The basic algorithm is pretty simple, though:  
  (1) Find out which expressions and variables are still live(LiveVariables). This is cached, per-function, ***context-insensitive*** information. 
  (2) Ask checkers which symbols are known to be in use, though potentially not live (checkLiveSymbols). 
  (3) Mark live any values ***associated with live expressions*** in the Environment. Remove all other bindings. 
  (4) Mark live any values ***accessible via the live regions*** in the Store. Remove all other bindings. 
  (5) Remove any constraints on dead symbols. 
  (6) Report dead symbols to the checkers, ***so that they can stop tracking information dependent on those symbols*** (checkDeadSymbols).
基于***Jordan Rose*** 的说明，将**ExprEngine::removeDead()**方法分为如下三个方面：
- **runCheckersForLiveSymbols()**，通过**checker**中注册的**checkLiveSymbols()**方法来对**Symbol**附上特殊的标识。例如使用**SymbolReaper::markInUse()**对感兴趣的**Symbol**打上正使用的标签，后面的***mark-and-sweep*** 就不会将其回收（即使该**Symbol**是**Dead**的）。
- **ProgramStateManager::removeDeadBindings()**执行真正的**remove dead**逻辑，分别对**Environment**、**RegionStore**和**Constraint**进行清理。
- **runCheckersForDeadSymbols()**，通过**checker**中注册的**checkDeadSymbols()**方法对显示标识为**Dead**的**Symbol**进行某些处理（注意：有些**Dead Symbol**有可能不会被标识为**Dead**，见[**Zombie symbols**](http://clang-developers.42468.n3.nabble.com/analyzer-Zombie-symbols-td4050524.html#a4050749)）。
其中第二步是这篇文章的重点。下面是**ExprEngine::removeDead()**的定义。
```
void ExprEngine::removeDead(ExplodedNode *Pred, 
                        ExplodedNodeSet &Out,
                        const Stmt *ReferenceStmt,
                        const LocationContext *LC,
                        const Stmt *DiagnosticStmt,
                        ProgramPoint::Kind K) {
    # 对remove dead进行计数          
    NumRemoveDeadBindings++;
    ProgramStateRef CleanedState = Pred->getState();
    const StackFrameContext *SFC = LC ? LC->getCurrentStackFrame() : nullptr;
    # 1. 创建SymbolReaper对象，用于此次removeDead()过程
    # 该对象贯穿于remove dead整个过程，用于保存remove dead(Environment、RegionStore)
    # 需要的live或者dead信息。
    SymbolReaper SymReaper(SFC, ReferenceStmt, SymMgr, getStoreManager());
    # 2.针对live symbol调用相关checker的回调方法checkLiveSymbols，针对checker感兴趣的symbol进行
    # 某些处理。
    getCheckerManager().runCheckersForLiveSymbols(CleanedState, SymReaper);
    #3.调用ProgramStateManager::removeDeadBindings()进行真正的remove dead过程。
    // Create a state in which dead bindings are removed from the environment
    // and the store. The function should just return new env and store,
    // not a new state.
    CleanedState = StateMgr.removeDeadBindings(CleanedState, SFC, SymReaper);
    // #4.针对dead symbols调用相关checker的回调函数checkDeadSymbols()，对显示标识为Dead
    // #的
    // run checkers for dead symbols.
}
```
为了介绍**remove dead**，我们先给出一段示例代码。
```
// test.cpp
struct Node { Node *next; int value; };
struct Base { Node n; int weight; };
extern int global;
extern int threshold;
int main() {
    int adjust = 0;
    int *ptr = &adjust;
    int local = 10;
    Node localNode;
    localNode.next = 0;
    localNode.value = *ptr;
    Base localBase;
    localBase.n.next = &localNode;
    // 假如clang静态分析引擎执行到该语句之前，此时Environment中有很多子表达式的条目需要清除，
    // 而且此时也可能有一些变量在此时也是死的，需要清除。
    // 例如：Environment -> localBase(DeclRefExpr)此时是死的
    //      Environment -> localBase.n(MemberExpr)也是死的
    //      ..
    // remove dead需要将Environment中死的条目删除。
    localBase.n.value = localBase.n.next->value + global;
    localBase.weight = 100;
    if (localBase.n.value > threshold) {
        localBase.weight = localBase.weight * 2;
    }
    int result = localBase.n.value * localBase.weight + local;
    return result;
}
```
在上述代码示例中，假如静态分析引擎此时在执行到***“localBase.n.value = localBase.n.next->value + global;”***（该说法不是很严谨，静态分析引擎在**CFG**上进行执行，此时应该是该语句对应的**CFGElement**）语句之前，此时**Environment**中有很多需要死的条目需要清除，**remove dead**就是用来执行该工作。
那么在某个程序点处，**Environment**中有哪些条目需要删除，**RegionStore**中有哪些变量的**Bindings**需要删除是由什么决定的呢？给**remove dead**提供这些信息就是我们前面的提到过的**clang**活跃性分析。我们在《[**clang**中的活跃性分析（续）](http://blog.csdn.net/dashuniuniu/article/details/53087374)》中给出了获取各个程序点活跃信息的方法，详见上一篇文章。另外需要注意的是仅仅依靠这些活跃性分析结果是不够，我们还要基于这些信息进行再处理来决定哪些**Symbol**是活的，关于这方面我们在后面会介绍。
# **[ProgramStateManager::removeDeadBindings()](https://code.woboq.org/llvm/clang/lib/StaticAnalyzer/Core/ProgramState.cpp.html#_ZN5clang4ento19ProgramStateManager18removeDeadBindingsEN4llvm18IntrusiveRefCntPtrIKNS0_12ProgramStateEEEPKNS_17StackFrameContextERNS0_12SymbolReaperE)**
上面我们提到了**ExprEngine::removeDead()**方法的核心就是调用**ProgramStateManager::removeDeadBindings()**进行具体的**remove dead**逻辑。下面是该方法的主要逻辑，该方法很简单，主要进行了三个方面的**purge**（清除）操作。第一对**Environment**进行清除操作，这里都是一些无用的“***临时变量***”也就是子表达式的值。第二就是对**RegionStore**进行清除操作，这里主要对一些死变量进行清除。第三对附加在变量上的**Constraint**进行清除操作。这三个方面分别对应于以下方法：
- EnvironmentManager::removeDeadBindings
- RegionStoreManager::removeDeadBindings
- RangeConstraintManager::removeDeadBindings
后面我们逐步对以上三个方法进行分析。在给出给出**ProgramStateManager::removeDeadBindings()**的定义之前，我们先着重强调以下该方法的一段注释，该注释准确的解释了**remove dead**的执行逻辑。
> 
This code essentially performs a “***[mark-and-sweep](https://en.wikipedia.org/wiki/Tracing_garbage_collection)***” of the VariableBindings. ***The roots are any Block-level exprs and Decls that our liveness algorithm tells us are live.***
We see what Decls ***they may reference***, and keep those around. 
**This code more than likely can be made faster, and the frequency of which this method is called should be experimented with for optimum performance.**
上面的注释给出了三个方面的信息，第一就是**remove dead**本质上就是***mark-and sweep***算法（如果了解该垃圾回收算法，那么对这个概念不会陌生）；第二就是前面活跃性分析提供的结果只是作为***mark-and-sweep***的根集，还要进行进一步的处理；第三就是**remove dead**的频率可以根据实际情况进行调整，已达到性能最优。
下面给出**ProgramStateManager::removeDeadBindings()**方法的定义。
```
ProgramStateRef
ProgramStateManager::removeDeadBindings(ProgramStateRef state,
                                    const StackFrameContext *LCtx,
                                    SymbolReaper& SymReaper) {
    ProgramState NewState = *state;
    // 1.对Environment进行清除
    NewState.Env = EnvMgr.removeDeadBindings(NewState.Env, SymReaper, state);
    // Clean up the store.
    // 2. 对RegionStore进行清除
    StoreRef newStore = StoreMgr->removeDeadBindings(NewState.getStore(), LCtx, SymReaper);
    NewState.setStore(newStore);
    SymReaper.setReaperStore(newStore);
    ProgramStateRef Result = getPersistentState(NewState);
    // 3. 对Constraint进行清除
    return ConstraintMgr->removeDeadBindings(Result, SymReaper);
}
```
**ProgramStateManager::removeDeadBindings()**的方法很简单，主要就是调用我们上面提到的三个方面对应的方法。
## mark-and-sweep（标记-清扫）
上面我们提到了**remove dead**是基于***mark-and-sweep***实现的，这里我们简单介绍该算法。***mark-and-sweep***（也就是标记-清扫式垃圾回收，这里我们沿用龙书中的翻译）是垃圾回收中最基本的方法，该方法属于**tracing garbage collection**（跟踪式垃圾回收）的类型。该方法并不会在产生垃圾的时候立即进行回收，而是周期性运行（就像**clang static analyzer**默认以**Stmt**为单位来进行回收）。**Tracing garbage collection**主要策略就是寻找不可达对象并回收它们的内存空间。***mark-and-sweep*** 中的***mark*** 就是用于寻找不可达对象的过程，而***sweep*** 就是回收它们内存的过程。
### Reachability of an object（对象可达性）
对象可达就是能够从当前根集（**root set**）访问到该对象，无论直接或者是间接的。
> 
**What is a garbage collection(GC) root?**
  Roots are basically ***stack variable/pointers*** that hold object in the heap memory. Roots can also be ***global variables***. So anything pointer that holds the object in the heap but does not reside in heap can be termed as roots.
Garbage Collection needs the roots to determine if the object is live or not. If object is ***transitively or directly*** reachable by any of the roots, then it is not a garbage. If not, it is garbage and needs to be deleted.
然而对于**clang static analyzer**来说，根集的定义就要更严格，基于**clang**活跃性分析的结果（**expressions** 和 **decls**）。
> 
**The roots are any Block-level exprs and Decls that our liveness algorithm tells us are live.** –removeDeadBindings
### mark
上面提到过**mark**的过程可以看做识别不可达对象的过程，该过程很简单，基本上是基于根集（可以看做森林）做的一系列树遍历（**DFS**或者**BFS**），我摘抄[**wiki-Tracing garbage collection**](https://en.wikipedia.org/wiki/Tracing_garbage_collection#Reachability_of_an_object)上的一张图片能够清晰展示该过程。
![这里写图片描述](https://img-blog.csdn.net/20161116104927429)
上面图片所展示的过程就是从根集出发做的一系列树遍历，能够访问到的对象都是可达对象（包括**Object 1**到**Object 6**），都不能被看作垃圾，而剩余的**Object 7**和**Object 8**由于是不可达对象，所以被看作垃圾从而被回收。由于需要对遍历到的可达对象进行标记，以便能够识别出这些可达对象，所以该阶段被称作***mark***。
> 
The first stage is the **mark stage** which does a **tree traversal of the entire ‘root set’** and marks each object that is pointed to  by a root as being ‘in use’. All objects that those objects point to, and so on, are marked as well, so that every object that is reachable via the root set is marked.
### sweep
**sweep**过程就比较简单了，遍历所有对象，将没有标记为***“可达”***的对象进行回收，该过程就是一个循环遍历过程。我们可以从上图中看出***mark*** 与***sweep*** 的过程。
> 
In the second stage, the **sweep stage**, all memory is scanned from start to finish, examining all free or used blocks; **those not marked as being ‘in-use’ are not reachable by any roots, and their memory is freed**. For objects which were marked in-use, the in-use flag is cleared, preparing for the next cycle.
从上面***mark-and-sweep*** 的算法来看，比较重要的两步是获取当前的根集以及从根集进行递归遍历并对这些对象进行标记。**clang static analyzer**在进行真正静态分析之前，已经提前进行过活跃性分析，而这些活跃性分析的结果就作为这些根集。**clang static analyzer**执行路径敏感静态分析的入口是函数[**ActionExprEngine()**](https://code.woboq.org/llvm/clang/lib/StaticAnalyzer/Frontend/AnalysisConsumer.cpp.html#697)，该函数我们在前面的源码分析中还没有分析到该函数（汗），该函数第一步就是创建**CFG**并进行活跃性分析。该函数定义如下：
```
//===---------------------------------------===//
// Path-sensitive checking.
//===----------------------------------------===//
void AnalysisConsumer::ActionExprEngine(Decl *D, bool ObjGCEnabled,
                                ExprEngine::InliningModes IModes,
                                SetOfConstDecls *VisitedCallees) {
    // Construct the analysis engine. First check if the CFG is valid.
    // FIXME: Inter-procedural analysis will need to handle invalid CFGs.
    if (!Mgr->getCFG(D))
        return;
    // See if the LiveVariables analysis scales.
    // 进行RelaxedLiveVariables类型的活跃性分析。
    if (!Mgr->getAnalysisDeclContext(D)->getAnalysis<RelaxedLiveVariables>())
        return;
}
```
现在我们知道了根集如何获取，但是任何通过根集找到可达对象（***mark***）并进行**Dead**清除（***sweep***）呢？文章后面的部分主要用于介绍**clang static analyzer**如何找到可达对象并进行清除的过程。
## clang::ento::SymbolReaper
前面我们提到在**removeDead()**需要创建一个新的**SymbolReaper**对象来保存此次**remove dead**过程中关于***live*** 以及***dead*** 的**Symbol**、**MemRegion**等信息，并提供一系列处理这些信息的方法。
> 
***SymbolReaper*** is an object that contains the ***incomplete understanding*** of live values on the ***mark phase***, which eventually becomes complete and as such ***can be used on the sweep phase***.
上面是**Clang-Dev**上关于**SymbolReaper**的一段描述，**SymbolReaper**是一个在***mark*** 阶段包含不完整***live*** 信息的对象，而这些信息会变得越来越完善并最终用于***sweep*** 阶段。下面是类**SymbolReaper**的定义。
```
/// \brief A class responsible for cleaning up unused symbols.
class SymbolReaper {
    // To Do: 还没有搞懂？？？
    enum SymbolStatus {
        NotProcessed,
        HaveMarkDependents
    };
    typedef llvm::DenseSet<SymbolRef> SymbolSetTy;
    typedef llvm::DenseMap<SymbolRef, SymbolStatus> SymbolMapTy;
    typedef llvm::DenseSet<const MemRegion *> RegionSetTy;
    // （1）live的Symbol
    SymbolMapTy TheLiving;
    // （2）还没有搞懂SymbolMetadata？？？
    // 猜测：SymbolMetadata是由checker使用，更新其状态不同与传统Symbol的一种Symbol类型。
    // Clang-Dev上有一个关于这方面的争论，就是是否允许checker修改任意Symbol的状态。
    // http://clang-developers.42468.n3.nabble.com/Allowing-checkers-to-mark-symbols-as-live-td1047452.html#a1065885
    // 感觉SymbolMetadata应该就是妥协的产物，只能被checker修改，而且checker只能接触到SymbolMetadata
    SymbolSetTy MetadataInUse;
    // （3）Dead的Symbol，注意Dead Symbol在后续是会被转换成Live Symbol的，但是反过来不行。
    SymbolSetTy TheDead;
    // （4）根Region，这个是SymbolReaper通过live Environment得到的比较重要的一个live info，
    // 直接决定了后续分析的复杂度。简单来说该数据成员，就是通过live expression中的
    // value所能表示以及索引到的region。后面我们会对其进行详细分析。
    RegionSetTy RegionRoots;
    // SymbolReaper所处的环境，SymbolReaper与SymbolManager不同的是SymbolReaper是程序点相关的，
    // 所以需要Stmt来表示当前SymbolReaper对象所处的Loc。
    const StackFrameContext *LCtx;
    const Stmt *Loc;
    SymbolManager& SymMgr;
    StoreRef reapedStore;
    // To Do: 还没有搞懂是做什么的???
    llvm::DenseMap<const MemRegion *, unsigned> includedRegionCache;
    /// \brief Construct a reaper object, which removes everything which is not
    /// live before we execute statement s in the given location context.
    SymbolReaper(const StackFrameContext *Ctx, const Stmt *s, SymbolManager& symmgr,
                StoreManager &storeMgr)
        : LCtx(Ctx), Loc(s), SymMgr(symmgr),
          reapedStore(nullptr, storeMgr) {}
    // 下面是一系列的判断某些Symbol，MemRegion以及Stmt值是否活跃的方法，这些方法我们会在后面进行解释。
    bool isLive(SymbolRef sym);
    bool isLiveRegion(const MemRegion *region);
    bool isLive(const Stmt *ExprVal, const LocationContext *LCtx) const;
    bool isLive(const VarRegion *VR, bool includeStoreBindings = false) const;
    // 下面是一系列的设置Symbol Live或Dead的方法。
    void markLive(SymbolRef sym);   
    void markInUse(SymbolRef sym);  
    bool maybeDead(SymbolRef sym);
    void markLive(const MemRegion *region);
    void markElementIndicesLive(const MemRegion *region);
    bool hasDeadSymbols() const {
        return !TheDead.empty();
    }
    /// \brief Returns whether or not a symbol has been confirmed dead.
    /// 
    /// This should only be called once all marking of dead symbols has completed.
    /// (For checkers, this means only in the evalDeadSymbols callback.)
    bool isDead(SymbolRef sym) const {
        return TheDead.count(sym);
    }
};
```
**SymbolReaper**对象在最开始的设计中只是一组**sets**，就像我们在注释中列出的**（1）**、**（2）**那几条，后面变得越来越复杂，最终成为现在这个样子。
#### SymbolReaper::markLive(SymbolRef sym)
***SymbolReaper::markLive(SymbolRef sym)*** 方法很简单，就是将欲***mark*** 为***live*** 的**Symbol**从**SymbolReaper**的***TheDead*** 集合中删除，然后插入到***TheLiving*** 集合。另外，还要检查当前**Symbol**所依赖的**DependentSymbols**，有的话也将这些**Symbols**插入到**TheLiving**集合中。
***注：对于SymbolDependencies，我还不太了解，有了解的请告知***
```
/// \brief Unconditionally marks a symbol as live
///
/// This should never be
/// used by checkers, only by the state infrastructure such as the store and
/// environment. Checkers should instead use metadate symbols and markInUse.
void markLive(SymbolRef sym) {
    TheLiving[sym] = NotProcessed;
    TheDead.erase(sym);
    markDependentsLive(sym);
}
```
### SymbolReaper::markInUse(SymbolRef sym)
该方法只能用于**checker**感兴趣的一些**Symbol**，也就是说这些**Symbol**的类型只能是**SymbolMetadata**类型的。就像下面注释中说的，该方法对于其他类型的**Symbol**没有任何效果，而且该方法只能在**checker**的**checkLiveSymbols()**回调方法中调用，先于其他的**remove dead**阶段（回顾我们前面提到过的**remove dead**的三个方面）。
***注：关于该问题在Clang-Dev中还有一个问题[Allowing checkers to mark symbols as live](http://clang-developers.42468.n3.nabble.com/Allowing-checkers-to-mark-symbols-as-live-td1047452.html#a1065885)与此相关，关于这个问题，我目前还没有一个比较清楚的认识，以后搞清楚之后再补齐***
```
/// \brief Marks a symbol as important to a checker.
/// 
/// For metadata symbols,
/// this will keep the symbol alive as long as its associated region is also
/// live. For other symbols, this has no effect; checkers are not permitted
/// to influence the life of other symbols. This should be used before any
/// symbol marking has occured, i.e. in the MarkLiveSymbols callback.
void markInUse(SymbolRef sym) {
    if (isa<SymbolMetadata>(sym))
        MetadataInUse.insert(sym);
}
```
### SymbolReaper::maybeDead(SymbolRef sym)
**SymbolReaper::maybeDead(SymbolRef sym)**是这些成员方法中比较有意思的一个，该方法判断当前**Symbol**当前是否是**Live**的，如果是**Live**的则返回**false**，如果不是**Live**的则将其插入到**TheDead**集合中的，并返回**true**。也就是说**Live**的优先级要比**Dead**要高的，如果判断是**Live**则不会将其设置为**Dead**，但是反过来即使当前**Symbol**是**Dead**的，照样可以设置为**Live**，所以该方法的名字不是***markDead()***而是***maybeDead()***。
```
/// \brief If a symbol is known to be live, marks the symbol as live.
///
/// Otherwise, if the symbol cannot be proven live, it is marked as dead.
/// Returns true if the symbol is dead, false if live.
bool maybeDead(SymbolRef sym) {
    if (isLive(sym))
        return false;
    TheDead.insert(sym);
    return true;
}
```
### isLive()系列方法
**isLive()**系列方法有四个，分别用来判断**Symbol**、**MemRegion**、**Stmt**（即**Environment**中的**ExprVal**）、**VarRegion**（主要用于判断变量），如下所示：
- **isLive(SymbolRef sym)**
- **isLiveRegion(const MemRegion *region)**
- **isLive(const Stmt *ExprVal, const LocationContext *LCtx) const**
- **isLive(const VarRegion *VR, bool includeStoreBindings = false) const**
#### isLive(SymbolRef Sym)
***isLive(SymbolRef Sym)***用于判断**Symbol**是否是***live*** 的方法，该方法根据**Symbol**具体的类型来判断**Symbol**是否是***live*** 的。
在介绍该方法之前，我们先区分以下**SVals**和**SymbolRef**，**Clang-Dev**有一个相似的问题[**SymbolRef and SVal confusion**](http://clang-developers.42468.n3.nabble.com/SymbolRef-and-SVal-confusion-td4029081.html)，我将[**Ted Kremenek**](http://clang-developers.42468.n3.nabble.com/template/NamlServlet.jtp?macro=user_nodes&user=10660)对该问题的回答摘抄过来。
> 
SVals are value objects. **They should just be viewed as a union of different abstract value that an expression can evaluate to.** They are “nameless” values. ***SVals can wrap SymbolRefs, but they also can wrap other values such as constants, addresses of goto labels, etc.*** When the analyzer evaluates an expression, it computes an SVal. Another way to loot at SVals is that they “box” more interesting values that have different meanings.
**SymbolRefs are “named” symbolic values. They represent the name of a value along a path**. For example, in your code snippert ‘argc’ has an initial value that has a symbol. Along a path the analyzer can reason about properties of that value, e.g., constraints such as the value of argc being greater than 0, etc. ***Constraints cannot be associated with SVals***, as they are values types that are just used to box more persistent named values like symbols.
从上面的描述中我们可以看到，**SVals**的层次更高，**SVals**可以是**Symbolic values**，但是也可以是其他的**values**，例如**concrete integers**等。**SymbolRef**的层次更低，表示一个未知的符号化的值，虽然我们不知道确切的值，但是我们可以在其上附加**constraints**。**SymExpr**是**clang static analyzer**中所有的**Symbol**的基类，表示无法获得确切值的类型，例如我们***extern int a*** 或者***conservative eval call*** 时函数的返回值以及复杂表达式的值都可以使用**SymExpr**的某些子类来表示。**SymbolRef**统一被**SymbolManager**管理。
关于**SVals**和**SymExpr**我们会在其他文章中进行介绍（现在这些文章还遥遥无期），所以这里就简要介绍下。如果想要更准确的关于这两种类型的介绍，可以参考[**checker dev manual**](http://clang-analyzer.llvm.org/checker_dev_manual.html#values)中**Representing Values**这一小节。
**SymExpr**暂时有以下几个子类，**SymbolData**、**SymbolCast**、**BinarySymExpr**，由于这篇文章不是专门介绍**Symbol**的，所以这里不打算过多介绍**SymExpr**。
#### SymbolData
**SymbolData**是一种可以存储到**Region**中的符号（当然存储到**Region**中也是以**SVal**形式存储的），它有如下几种形式，**SymbolRegionValue**、**SymbolConjured**、**SymbolDerived**、**SymbolExtent**、**SymbolMetadata**。
*注：上面中的描述是不合适的，BinarySymExpr也是可以存储到的Region中的。SymbolData是有MemRegion与其关联的SymExpr类型，关联的形式有多种，例如必须有一个MemRegion与之对应，例如SymbolRegionValue，SymbolDerived，SymbolExtent以及SymbolMetadata。**或者说是非表达式的符号值，毕竟[isExpression()](https://code.woboq.org/llvm/clang/include/clang/StaticAnalyzer/Core/PathSensitive/SVals.h.html#_ZNK5clang4ento6nonloc9SymbolVal12isExpressionEv)是通过是否为SymbolData判断的***
**SymbolRegionValue**是我们最容易理解的一种**SymbolData**类型，该符号类型用于表示**MemRegion**中的值。例如表示**形参值**或者**外部全局变量值**。
> 
///\brief A symbol representing the value stored at a MemRegion.
如下代码，形参***num*** 的值就是使用**SymbolRegionValue**表示的。
```
# num值未知，num的值就是用SymbolRegionValue表示的
int inc(int num)
{
    int local = num + 1;
    return local;
}
```
我们使用***debug.ViewExplodedGraph*** 得到**ExplodedGraph**的子图如下所示，红色方框中的的值表示的就是***num*** 和***num+1*** 的值，注意***num*** 使用***reg_$0*** 前缀表示的，***reg_$***（***region_$***的缩写）表示的就是**SymbolRegionValue**类型。 
![这里写图片描述](https://img-blog.csdn.net/20161120172901719)
在**clang static analyzer**中***conj_$***表示**SymbolConjured**，***derived_$***表示**SymbolDerived**，***extent_$***表示**SymbolExtent**，***meta_$***表示**SymbolMetadata**，***reg_$***前缀表示**SymbolRegionValue**类型。关于这些表示可以通过**SymbolManager.cpp**中的一系列**dumpToStream(*)**方法看出来。
**SymbolConjured**我们在《**clang static analyzer**源码分析（番外篇）：**RegionStore**以及**evalCall()**中的**conservativeEvalCall**》中的**[bindReturnValue](http://blog.csdn.net/dashuniuniu/article/details/52849373#t24)**这一小节中有所涉及，其中我们使用**SymbolConjured**来表示一种未知值。而表示未知值就是**SymbolConjured**最基本的用途。
> 
SymbolConjured represents the value of an expression that the analyzer **can’t evaluate**(usually a call expression).
例如复杂运算得到的表达式值，或者两个**SymbolRegionValue**运算得到的值就是使用**SymbolConjured**表示，如下代码所示：
```
extern int global;
int inc(int num)
{
    // 其中num+global表达式的值就是使用SymbolConjured表示
    return num + global;
}
```
**SymbolDerived**我现在还没有搞懂是做什么的，查了些资料，而这些资料又和源码中的注释相悖。这里给出**Jordan Rose**在《**[SymbolRef and SVal confusion](http://clang-developers.42468.n3.nabble.com/SymbolRef-and-SVal-confusion-td4029081.html)**》中给出的解释，
> 
SymbolDerived represents the **contents of a subregion of a SymbolicRegion**, e.g. the third field in a struct passed by value.
***注：the third field in a struct passed by value没有搞明白是什么意思***
按照上面的意思，**SymbolDerived**表示**SymbolicRegion**中**SubRegion**的内容。**Symbolic Pointer**指向的**Region**使用**SymbolicRegion**表示，见《[A Memory Model for Static Analysis of C 
Programs](http://lcs.ios.ac.cn/~xuzb/canalyze/memmodel.pdf)》中**4.1**节最后一段。我尝试使用**Symbolic Pointer**来找到**SymbolDerived**的例子，但是怎么都不成功。例如，下面的示例代码：
```
// test.cpp
struct S { int f; };
int inc(S *s)
{
    // 按理说s指向的内存是SymbolicRegion，其SubRegion s->f的内容应该使用SymbolDerived表示
    // 但是最终却是使用SymbolRegionValue表示的
    int num = s->f;
    return num;
}
```
最终我在**clang-Dev**中找到一个关于**SymbolDerived**的示例，该示例在《**[Random SVal hierarchy questions.](http://clang-developers.42468.n3.nabble.com/analyzer-Random-SVal-hierarchy-questions-td4048398.html)**》中有所涉及，该代码示例如下所示： 
***注：该问题很不错，对于理解Region、SVal和Symbol很有帮助***
```
struct S { 
  int f; 
}; 
struct S getS(); 
void foo() { 
  struct S s = getS(); 
  int x1 = s.f; 
  int x2 = s.f; 
  // Store Bindings at this program point: 
  //   (s,0,default) : conj_$2{struct S} 
  //   (x1,0,direct) : derived_$3{conj_$2{struct S},s->f} 
  //   (x2,0,direct) : derived_$3{conj_$2{struct S},s->f} 
}
```
上面示例中，***getS()*** 会返回一个临时对象，该临时对象处在**SymbolicRegion**，该临时对象也是**symbol values**。此时***s***的值就是**SymbolDerived**类型，如上面注释所示。对于**SymbolDerived**了解的人，还望告知！
**SymbolExtent**在**clang static analyzer**中用来表示内存的大小，而且**SymbolExtent**只有这一种用途。**symbol extent**可以是**symbolic value**也可以不是（因为大部分内存的大小都是已知的，也就是**concrete int**）。但是**SymbolExtent**对象表示的是**symbolic****value**。下面是**SymbolExtent**的定义：
```
/// SymbolExtent - Represents the extent (size of bytes) of a bounded region.
/// Clients should not ask the SymbolManager for a region's extent. Always use
/// SubRegion::getExtent instead -- the value returned may not be a symbol.
/// 注意只有SubRegion可以使用getExtent()返回Region的size，对于MemSpaceRegion
/// 来说内存大小的概念没有意义。注意getExtent()返回的是SVal，由于SVal包裹着其他
/// 类型的值，例如concrete int 或者symbol value，所以getExtent()可以返回symbol value
/// 也可以不是symbol value。
class SymbolExtent : public SymbolData {
};
```
由于**SymbolExtent**与**MemRegion**是紧密相关的，所以只要**MemRegion**是活跃的，**SymbolExtent**就必须是活的。
关于**SymbolExtent Clang-Dev**有一个**Jordy Rose**(也就是**Jordan Rose**)与**Ted Kremenek**的争论的问题**[Symbolic Extents](http://clang-developers.42468.n3.nabble.com/Symbolic-Extents-td920657.html)**，该问题涉及到了**SymbolExtent**设计之初的方方面面，建议仔细阅读。另外我也没有找到**dump**出**SymbolExtent**的代码示例。
**SymbolMetadata**和**SymbolExtent**相似，虽然从字面意思上来看和**Region**没有一丁点儿关系，但是**SymbolMetadata**和**SymbolExtent**却只用于Region相关的场景。**SymbolMedata**用来描述和具体**Region**相关的一些元数据，并且这些数据是路径相关的。其实我感觉两者的名字改为**SymbolRegionMetadata**和**SymbolRegionExtent**更为合适一点。虽然两者都是**Region**相关的符号值，但是两者却有很重要的差别。由于**SymbolExtent**用于描述**SymbolicRegion**的大小，所以即使**Region**中的内容失效，但是**Region**的大小还是有效的，除非将整个**Region**删除，那么描述该**Region**的**SymbolExtent**也将不复存在。而为**Region**设置的一些和**Region**相关的**SymbolMetadata**是可以失效的。**SymbolMetadata**的应用场景很有限，只是在**CStringChecker**中被使用到了。**SymbolMetadata**定义如下：
```
/// SymbolMetadata - Represents path-dependent metadata about a specific region.
/// Metadata symbols remain live as long as they are marked as in use before
/// dead-symbol sweeping AND their associated regions are still alive.
/// Intended for use by checkers.
class SymbolMetadata : public SymbolData {
};
```
**Jordan Rose**在**[SymbolRef and SVal confusion](http://clang-developers.42468.n3.nabble.com/SymbolRef-and-SVal-confusion-td4029081i20.html)**中对两者的区别做了简要的总结，如下。
> 
**SymbolExtent** is (currently) a single -purpose symbol used to represent the size(in bytes) of a region whose size is not statically known(i.e. by the compiler).
- **based on the region itself**
- **dose not get invalidated**
- **live as long as the region is live (in theory)**
**SymbolMetadata** is a general-purpose symbol used to represent invalidatable metadata associated with a particular region.
- **based on the region contents**
- **is invalidated along with region contents**
- **live as long as the region is live and some checker is interested**
#### SymbolCast
**SymbolCast**比较好理解，主要用于表示类型转换表达式（前提是**Symbol**），下面是它的定义。
```
/// \brief Represents a cast expression.
class SymbolCast : public SymExpr {
    const SymExpr *Operand;
    /// Type of the operand.
    QualType FromTy;
    /// The type of the result.
    QualType ToTy;
/// ... 
};
```
#### BinarySymExpr
**BinarySymExpr**是二元符号化的运算表达式，可以理解为符号执行无法求得其值的二元表达式（可以递归表示的，也就是**BinarySymExpr**的一个**Operand**是另一个**BinarySymExpr**），程序中很多表达式都是这种类型。**BinarySymExpr**又可以细分为**SymIntExpr**，**IntSymExpr**以及**SymSymExpr**这几种类型。这些细分的类型对应的**assume*()**逻辑也不相同。
**BinarySymExpr**各个子类对应的示例代码如下所示：
```
extern int global;
void foo(int *x, int *y)
{
    // (1) SymIntExpr
    int local1 = global + 1;
    // (2) SymIntExpr，clang static analyzer会将左右operand交换。见SimpleConstraintManager::evalBinOpNN()
    int local2 = 1 + global;
    // 
    // (3) SymIntExpr
    int local3 = global >> 2;
    // (4) SymSymExpr (reg_$0<x>) == (reg_$1<y>)
    // 由于我对clang static analyzer的理解太过肤浅，所以不明白为什么Symbol(loc) == Symbol(loc) 类型才能成为SymSymExpr。
    // 而Symbol(nonloc) + Symbol(nonloc)不能使用SymSymExpr表示，而是SymbolConjured。莫非是Symbol(nonloc) + Symbol(nonloc)是无意义的？
    if (x == y) global = 10;
    // ...
}
```
***注：我对Symbol的理解还很肤浅，有对clang Symbol熟悉的话，请多交流！***
下面介绍***isLive()*** 对不同种类**Symbol**的判断逻辑，***isLive(SymbolRef sym)*** 的定义如下：
```
bool SymbolReaper::isLive(SymbolRef sym) {
    if (TheLiving.count(sym)) {
        markDependentsLive(sym);
        return true;
    }
    bool KnownLive;
    switch(sym->getKind()) {
    case SymExpr::SymbolRegionValueKind:
        KnownLive = isLiveRegion(cast<SymbolRegionValues>(sym)->getRegion());
        break;  
    case SymExpr::SymbolConjuredKind:
        KnownLive = false;
        break;
    case SymExpr::SymbolDerivedKind:
        KnownLive = isLive(cast<SymbolDerived>(sym)->getParentSymbol());
        break;  
    case SymExpr::SymbolExtentKind:
        KnownLive = isLiveRegion(cast<SymbolExtent>(sym)->getRegion());
        break;  
    case SymExpr::SymbolMetadataKind:
        KnownLive = MetadataInUse.count(sym) && isLiveRegion(cast<SymbolMetadata>(sym)->getRegion());
        if (knownLive)
            MetadataInUse.erase(sym);
        break;
    case SymExpr::SymIntExprKind:
        KnownLive = isLive(cast<SymIntExpr>(sym)->getLHS());
        break;
    case SymExpr::IntSymExprKind:
        KnownLive = isLive(cast<IntSymExpr>(sym)->getRHS());
        break;
    case SymExpr::SymSymExprKind:
        KnownLive = isLive(cast<SymSymExpr>(sym)->getLHS()) &&
                    isLive(cast<SymSymExpr>(sym)->getRHS());
        break;
    case SymExpr::SymbolCastKind:
        KnownLive = isLive(cast<SymbolCast>(sym)->getOperand());
        break;
    }
    if (KnownLive)
        markLive(sym);
    return KnownLive;
}
```
上面是***isLive(SymbolRef)*** 的定义，该函数对多种类型的**Symbol**进行***live*** 判断。首先判断该符号是否已经在**TheLiving**集合中，如果该符号已经是***live*** 的，则继续将该符号依赖的的其他符号设置为***live***，并返回**true**。如果前面判断**Symbol**不在**TheLiving**集合中，那么后面就按照**Symbol**的类型进行对应的判断，如下所示。
|符号类型|说明|
|----|----|
|**SymbolRegionValueKind**|如果当前**SymbolRegionValue**所处的**Region**是***live*** 的话，则该**SymbolRegionValue**也是***live*** 的|
|**SymbolConjuredKind**|如果当前是**SymbolConjured**，则**KnownLive**设置为**false**，也就是暂时设置为***dead***，但是只要有其他途径将**SymbolConjured**设置为***live***，终归还是***live*** 的|
|**SymbolDerivedKind**|只要**SymbolDerived**对应的**ParentSymbol**是***live*** 的，则该**Symbol**也是***live*** 的|
|**SymbolExtentKind**|如果当前是**SymbolExtentKind**，则首先判断该**SymbolExtent**所描述的**Region**是否是***live*** 的，如果所处**Region**是***live*** 的，则当前符号也是***live*** 的|
|**SymbolMetadataKind**|前面我们介绍过**SymbolMetadata**是用于描述**Region**的路径敏感的元数据，这些数据是用户（也就是**Checker**）设置的，这些数据是可以失效的。所以这里首先判断当前**Region**是否有效，然后判断**Checkers**是否将当前的**Symbol**设置为***InUse***，如果是的话，则**SymbolMetadata**才是***live*** 的（从这里就可以看出来**SymbolExtent**和**SymbolMetadata**的区别）|
|**SymIntExprKind**|**SymIntExprKind**是**BinarySymExpr**的子类，只要**SymIntExpr**的***LHS*** 是***live*** 的，那么**SymIntExpr**就是活的。**注：这个逻辑感觉有点儿问题。我感觉应该是“如果说SymIntExpr是*live* 的，那么LHS是*live* 的”**|
|**IntSymExprKind**|如果**IntSymExpr**的***RHS*** 是活的，那么当前**IntSymExpr**就是活的。**注：同上，感觉这个逻辑应该也是有问题的**|
|**SymSymExprKind**|如果**SymSymExpr**的***LHS*** 和***RHS*** 同时是***live*** 的，那么当前**SymSymExpr**就是***live*** 的。**注：同上，感觉这个逻辑应该也是有问题的**|
对于上面**SymIntExpr**、**IntSymExpr**以及**SymSymExpr**的疑问，下面有个例子，说明**SymIntExpr**是否***live*** 依赖于***LHS*** 的逻辑是有问题的。
```
int inc(int num)
{
    int local = num + 10 + num;
    return local + num;
}
```
在**clang static analyzer**执行到***“int local = num + 10 + num;”***语句前面的时候，此时***“num + 10 + num”***的值已经**evaluate**得到。由于***num*** 是**SymbolRegionValue**，并且***num*** 所在的**VarRegion**此时是***live*** 的（因为***num*** 后面还要在***“local + num”*** 中用到），所以***num***（**SymbolRegionValue**）是***live*** 的。此时**Environment**中有**[num : &num]**、**[num : reg_&0<num>]**、**[num + 10 : (reg_&0<num>) + 10]**、**[num : &num]**、**[num : reg_$0<num>]**这些**Expression**，没有一个是***live*** 的，但是由于前面我们判断到***num***（**SymbolRegionValue**）是***live*** 的，所以此时**[num + 10 : (reg_&0<num>) + 10]**是***live*** 的，这个是没有意义的，因为该表达式的值已经没有什么用了。
**注：对于SymIntExpr、IntSymExpr以及SymSymExpr的判断逻辑，我感觉是有问题的，如果有对此了解的，还望多交流！**
#### isLiveRegion(const MemRegion *region)
该函数用于判断**MemRegion**是否是***live*** 的（如果是判断**VarRegion**则会调用下面的***isLive(const VarRegioin)***方法），该函数首先判断**MemRegion**是否在***RegionRoots*** 集合中，如果在的话则直接返回***true***。否则就根据具体的情况进行分析。
在当前**MemRegion**不在***RegionRoots***集合中情况下，首先获取其**BaseRegion**（获取其**BaseRegion**的概念就是脱去**offset**，直接获取当前内存地址所在的变量的**Region**），然后判断其**BaseRegion**是否是***live*** 的。根据**BaseRegion**的类型分别调用***isLive(SymbolRef sym)*** 和***isLive(const VarRegion*)***。如果**BaseRegion**就不是**SymbolicRegion**又不是**VarRegion**，那么就在**BaseRegion**是**AllocaRegion**、**CXXThisRegion**、**MemSpaceRegion**或者**CodeTextRegion**的情况下返回**true**，否则返回**false**。
该函数定义如下：
```
bool SymbolReaper::isLiveRegion(const MemRegion *MR) {
    if (RegionRoots.count(R))
        return true;
    MR = MR->getBaseRegion();
    if (count SymbolicRegion *SR = dyn_cast<SymbolicRegion>(MR))
        return isLive(SR->getSymbol());
    if (const VarRegion *VR = dyn_cast<VarRegion>(MR))
        return isLive(VR, true);
    // FIXME: This is a gross over-approximation. What we really need is a way to
    // tell if anything still refers to this region. Unlike SymbolicRegions,
    // AllocaRegions don't have associated symbols, though, so we don't actually
    // have a way to track their liveness.
    // 注：关于over-approximation我不是很懂！！！所以不会多介绍不懂装懂，还请了解的人给予解答！
    if (isa<AllocaRegion>(MR))
        return true;
    if (isa<CXXThisRegion>(MR))
        return true;
    if (isa<MemSpaceRegion>(MR))
        return true;
    if (isa<CodeTextRegion>(MR))
        return true;
    return false;
}
```
***注：我会尽量了解程序分析中的gross over-approximation，后面再补齐***
#### isLive(const Stmt *ExprVal, const LocationContext *LCtx)
该函数判断某条语句值（基本上都是**Environment**中的**Expr**）在此处是否是***live*** 的，该函数是基于**Relaxed Live Analysis**实现的。我们在《[**Clang**中的活跃性分析（续）](http://blog.csdn.net/dashuniuniu/article/details/53087374)》中介绍了该分析。
后面我们在介绍***EnvironmentManager::removeDeadBindings()***的时候，会提到该函数就是通过调用***isLive(const Stmt *ExprVal)***来判断表达式是否是***live*** 的。
#### isLive(const VarRegion *VR, bool includeStoreBindings = false)
该函数用于判断某个变量对应的**VarRegion**此时是否是***live*** 的，该函数也是基于**Relaxed Live Analysis**的。
***注：全局变量对应的VarRegion一直都是live的***
上面我们介绍了***mark-and-sweep***算法以及**SymbolReaper**，但是并没有给出**remove dead**如何使用这些算法来进行**Environment**、**Store**的清理。下面我依次介绍**Environement**、Store以及**Constraint**的清理过程。
## **[EnvironmentManager::removeDeadBindings()](https://code.woboq.org/llvm/clang/lib/StaticAnalyzer/Core/Environment.cpp.html#_ZN5clang4ento18EnvironmentManager18removeDeadBindingsENS0_11EnvironmentERNS0_12SymbolReaperEN4llvm18IntrusiveRefCntPtrIKNS0_12ProgramStateEEE)**
**Environment**清理的主体思想也是***mark-and-sweep***，找到***live*** 的**[EnvironmentEntry](https://code.woboq.org/llvm/clang/include/clang/StaticAnalyzer/Core/PathSensitive/Environment.h.html#clang::ento::EnvironmentEntry)**之后，将其添加到新的**Environment**中。在对这些**EnvironmentEntry**进行判断的过程中，顺便将***live*****EnvironementEntry**中的**Symbol**也标为***live***（也就是插入到**SymbolReaper**对象中）。由于该函数的注释信息精确的描述了该函数的逻辑，这里单独我摘出来予以强调。
> 
**removeDeadBindings**:
- Remove subexpression bindings.
- Remove dead block expression bindings.
- **Keep live block expression bindings**: 
  (1) **Mark their reachable symbols live in SymbolReaper, see ScanReachableSymbols.**
  (2) **Mark the region in DRoots if the binding is a loc::MemRegionVal.**
该函数的定义如下：
```
Environment
EnvironmentManager::removeDeadBindings(Environment Env,
                                    SymbolReaper &SymReaper,
                                    ProgramStateRef ST) {
    // We construct a new Environment object entirely, as this is cheaper than
    // individually removing all the subexpression bindings (which will greatly
    // outnumber block-level expression bindings).
    // 为了提高效率，创建新的Environment对象，将live的Expression添加到新创建的Environment对象
    Environment NewEnv = getInitialEnvironment();
    MarkLiveCallback CB(SymReaper);
    // (1) 创建ScanReachableSymbols对象，用以递归遍历Expression中的Symbol。
    ScanReachableSymbols RSScaner(ST, CB);
    llvm::ImmutableMapRef<EnvironmentEntry, SVal> 
        EBMapRef(NewEnv.ExprBindings.getRootWithoutRetain(),
        F.getTreeFactory());
    // (2) 遍历Environment中所有的EnvironmentEntry，检查该Entry在当前程序点是否live，
    // 如果是live的，将Entry添加到新创建的Environment中。
    // Iterate over the block-expr bindings.
    for (Environment::iterator I = Env.begin(), E = Env.end();
        I != E; ++I) {
        const EnvironmentEntry &BlkExpr = I.getKey();
        const SVal &X = I.getData();
        // 通过Relaxed Live Analysis的结果，判断当前Entry对应的Expression
        // 是否live。
        if (SymReaper.isLive(BlkExpr.getStmt(), BlkExpr.getLocationContext())) {
            // Copy the binding to the new map.
            // 如果Expression是live的，则将该Entry添加到刚创建的新的Environment中
            EBMapRef = EBMapRef.add(BlkExpr, X);
            // Mark all symbols in the block expr's value live.
            // i. 使用RSScanner遍历当前Expression中的Symbol，并将其标记为live，
            RSScanner.scan(X);
            continue;
        } else {
        // ii. 如果当前Entry对应的Expression不是live的，则将该Expression对应SVal中
        // 的Symbol添加到SymbolReaper的TheDead（如果是live，则跳过）集合中。
            SymExpr::symbol_iterator SI = X.symbol_begin(), SE = X.symbol_end();
            for (; SI != SE; ++SI)
                SymReaper.maybeDead(*SI);
        }
    }
    EewEnv.ExprBindings = EBMapRef.asImmutableMap();
    return NewEnv;
}
```
该函数对应的逻辑很简单，只是有两个部分需要进一步介绍，分别是上面注释中的***i.*** 以及***ii.***，在介绍***i.*** 和***ii.***之前，我们先给出一段代码示例。
```
struct Node { Node *next; int value; };
struct Base { Node n; int weight; };
extern int global;
extern int threshold;
int main() {
    int adjust = 0;
    int *ptr = &adjust;
    int local = 10;
    Node localNode;
    localNode.next = 0;
    localNode.value = *ptr;
    Base localBase;
    localBase.n.next = &localNode;
    // 假设clang static analyzer执行到当前程序点
    localBase.n.value = localBase.n.next->value + global;
    localBase.weight = 100;
    if (localBase.n.value > threshold) {
        localBase.weight = localBase.weight * 2;
    }
    int result = localBase.n.value * localBase.weight + local;
    return result;
}
```
**clang static analyzer**执行到***“localBase.n.value = localBase.n.next->value + global;”***语句之前时的**ExplodedGraph**子图如下所示：
![这里写图片描述](https://img-blog.csdn.net/20161123170509593)
如上图所示，此时**Store**中有**3**个变量（**Cluster**）对应的**ClusterBindings**，分别是变量**local**、**localNode**和**localBase**。此时**Environment**中的**EnvironmentEntry**如上图绿色框所示。而在***“localBase.n.value = localBase.n.next->value + global;”***语句之前的**relaxed live infomation**如下所示：
```
// **************program point***********************
BinaryOperator 0xbcc5918 'int' lvalue '='
|-MemberExpr 0xbcc5778 'int' lvalue .value 0xbc9eac0
| `-MemberExpr 0xbcc5740 'struct Node' lvalue .n 0xbc9ece8
|   `-DeclRefExpr 0xbcc5718 'struct Base' lvalue Var 0xbcc5148 'localBase' 'struct Base'
`-BinaryOperator 0xbcc58f0 'int' '+'
  |-ImplicitCastExpr 0xbcc58c0 'int' <LValueToRValue>
  | `-MemberExpr 0xbcc5860 'int' lvalue ->value 0xbc9eac0
  |   `-ImplicitCastExpr 0xbcc5848 'struct Node *' <LValueToRValue>
  |     `-MemberExpr 0xbcc5810 'struct Node *' lvalue .next 0xbc9ea60
  |       `-MemberExpr 0xbcc57d8 'struct Node' lvalue .n 0xbc9ece8
  |         `-DeclRefExpr 0xbcc57b0 'struct Base' lvalue Var 0xbcc5148 'localBase' 'struct Base'
  `-ImplicitCastExpr 0xbcc58d8 'int' <LValueToRValue>
    `-DeclRefExpr 0xbcc5898 'int' lvalue Var 0xbc9edc8 'global' 'int'
// ****************live info*************************
 local file/code/ForBlog/for_blog_complex1.cpp:20:6
 localBase file/code/ForBlog/for_blog_complex1.cpp:26:7
MemberExpr 0xbcc5778 'int' lvalue .value 0xbc9eac0
`-MemberExpr 0xbcc5740 'struct Node' lvalue .n 0xbc9ece8
  `-DeclRefExpr 0xbcc5718 'struct Base' lvalue Var 0xbcc5148 'localBase' 'struct Base'
BinaryOperator 0xbcc58f0 'int' '+'
|-ImplicitCastExpr 0xbcc58c0 'int' <LValueToRValue>
| `-MemberExpr 0xbcc5860 'int' lvalue ->value 0xbc9eac0
|   `-ImplicitCastExpr 0xbcc5848 'struct Node *' <LValueToRValue>
|     `-MemberExpr 0xbcc5810 'struct Node *' lvalue .next 0xbc9ea60
|       `-MemberExpr 0xbcc57d8 'struct Node' lvalue .n 0xbc9ece8
|         `-DeclRefExpr 0xbcc57b0 'struct Base' lvalue Var 0xbcc5148 'localBase' 'struct Base'
`-ImplicitCastExpr 0xbcc58d8 'int' <LValueToRValue>
  `-DeclRefExpr 0xbcc5898 'int' lvalue Var 0xbc9edc8 'global' 'int'
CompoundStmt 0xbcc5c48
`-BinaryOperator 0xbcc5c20 'int' lvalue '='
  |-MemberExpr 0xbcc5b28 'int' lvalue .weight 0xbc9ed48
  | `-DeclRefExpr 0xbcc5b00 'struct Base' lvalue Var 0xbcc5148 'localBase' 'struct Base'
  `-BinaryOperator 0xbcc5bf8 'int' '*'
    |-ImplicitCastExpr 0xbcc5be0 'int' <LValueToRValue>
    | `-MemberExpr 0xbcc5b88 'int' lvalue .weight 0xbc9ed48
    |   `-DeclRefExpr 0xbcc5b60 'struct Base' lvalue Var 0xbcc5148 'localBase' 'struct Base'
    `-IntegerLiteral 0xbcc5bc0 'int' 2
```
**EnvironmentManager::removeDeadBindings()**函数首先遍历**Environment**中的**EnvironmentEntry**，然后按照上面的**relaxed live information**看哪些**Entry**对应的**Expression**是活跃的，然后使用**ScanReachableSymbols**的对象**RSScanner**遍历这些活跃**Expression**对应**SVal**中的**Symbol**。
### [ScanReachableSymbols](https://code.woboq.org/llvm/clang/include/clang/StaticAnalyzer/Core/PathSensitive/ProgramState.h.html#clang::ento::ScanReachableSymbols)
**ScanReachableSymbols**是一个灵活的用于遍历可达符号的类，这些符号都是**SymExpr**的子类。由于**SymExpr**有可能是递归生成的，而且符号类型也有很多，所以单独提取出一个**ScanReachableSymbols**类也是很有必要的。**ScanReachableSymbols**没有和**SymReaper**进行绑定，用户可以自行定义遍历到可达符号时的行为，例如将遍历到的可达符号存储下来等等。下面是**ScanReachableSymbols**的定义。
```
/// \class ScanReachableSymbols
/// A Utility class that allows to visit the reachable symbols using a custom
/// SymbolVisitor.
class ScanReachableSymbols {
    typedef llvm::DenseSet<const void*> VisitedItems;
    VisitedItems visited;
    ProgramStateRef state;
    // SymbolVisitor就是用户定制的遍历到指定符号时需要执行的逻辑
    SymbolVisitor &visitor;
// public:
    // 在创建ScanReanchableSymbols时需要调用方提供SymbolVisitor示例，回想上面
    // 的EnvironmentManager::removeDeadbindings()函数首先创建了MarkLiveCallback对象，
    // 然后使用该对象示例创建了RSScanner。
    ScanReachableSymbols(ProgramStateRef st, SymbolVisitor &v)
        : state(std::move(st)), visitor(v) {}
    // 下面是一系列针对不同类型Symbol的Scan策略
    bool scan(nonloc::lazyCompoundVal val);
    bool scan(nonloc::CompoundVal val);
    bool scan(SVal val);
    bool scan(const MemRegion *R);
    bool scan(const SymExpr *sym);
};
```
**ScanReachableSymbols**定义了一组***scan()*** 方法，这些方法针对不同类型的值进行相应的遍历策略，创建**ScanReachableSymbols**的时候可以指定不同的**SymbolVisitor**，**SymbolVisitor**指定具体的遍历到某些符号时的回调函数。例如在**removeDeadBindings()**函数中，创建**RSScaner**时，将**MarkLiveCallback**对象赋值给**SymbolVisitor**。从**MarkLiveCallback**的名字可以看出来，这个对象就是在遍历到某些符号时，将这些符号标识成***live***。下面我先将不同类型符号的遍历规则介绍一下，然后给出实际的例子。
#### scan(nonloc::lazyCompoundVal)
对于**[LazyCompoundVal](https://code.woboq.org/llvm/clang/include/clang/StaticAnalyzer/Core/PathSensitive/SVals.h.html#clang::ento::nonloc::LazyCompoundVal)**，我们在《 [**clang static analyzer**源码分析（番外篇）：**RegionStore**以及**evalCall()**中的**conservativeEvalCall**](http://blog.csdn.net/dashuniuniu/article/details/52849373)》提到过**LazyCompoundVal**，**LazyCompoundVal**用于表示聚合类型值。**RegionStore**为了优化聚合类型的赋值传递，使用**LazyCompoundVal**表示聚合类型值。当需要聚合类型值时，我们创建一个**LazyCompoundVal**，当这个值赋值给一个变量时，我们以**Default Binding**的形式将该**LazyCompoundVal**绑定到该变量。使用lazy的方法，这样效率会更高，如下代码所示：
```
# 再一次使用该代码示例
void inc() {
    CGPoint p;
    p.x = 42;       // A Direct binding is made to the FieldRegion p.x.
    CGPoint p2 = p; // A LazyCompoundVal is created for p, along with a 
                    // snapshot of the current state. This value is then
                    // used as a Default binding for the VarRegion p2.
    return p2.x;    // The binding for FieldRegion p2.x is requested.
                    // There is no Direct binding, so we look for a Default
                    // binding to p2 and find the LCV.
                    // Because its an LCV, we look at our requested region
                    // and see that its the .x field. We ask for the value
                    // of p.x within the snapshort, and get back 42.
}
```
基于**LazyCompoundVal**的含义，在遍历到**LazyComoundVal**的时候，我们首先需要获取其原始的值，然后遍历该聚合类型各个数据成员的值。基于此**scan(nonloc::LazyCompoundVal)**的逻辑很简单，该函数定义如下：
```
bool ScanReachableSymbols::scan(nonloc::LazyCompoundVal val) {
    bool wasVisited = !visited.insert(val.getCVData()).second;
    if (wasVisited)
        return true;
    StoreManager &StoreMgr = state->getStateManager().getStoreManager();
    // FIXME: We don't really want to use getBaseRegion() here because pointer
    // arithmetic doesn't apply, but scanReachableSymbols only accepts base
    // regions right now.
    // 我们需要获取LazyCompoundVal对应的原有的聚合值，然后分别scan这些值
    const MemRegion *R = val.getRegion()->getBaseRegion();
    // 所以这里调用RegionStoreManager的scanReachableSymbols()方法对于聚合类型
    // 的数据成员分别进行遍历。
    return StoreMgr.scanReachableSymbols(val.getStore(), R, *this);
}
```
**scan(nonloc::LazyCompoundVal)**函数获取**LazyCompoundVal**对应的**MemRegion**以后，调用**RegionStoreManager::scanReachableSymbols()**方法遍历其数据成员，遍历逻辑如下。
```
bool RegionStoreManager::scanReachableSymbols(Store S, const MemRegion *R,
                                        ScanReachableSymbols &Callbacks) {
    assert(R == R->getBaseRegion() && "Should only be called for base regions");
    RegionBindingsRef B = getRegionBindings(S);
    const ClusterBindings *Cluster = B.lookup(R);
    if (!Cluster)
        return true;
    // 获取LazyCompoundVal对应MemRegion的ClusterBindings。然后遍历ClusterBindings中
    // 的所有Binding，使用ScanReachableSymbols遍历这些数据成员值。
    for (ClusterBindings::iterator RI = Cluster->begin(), RE = Cluster->end();
        RI != RE; ++RI) {
        // Callbacks就是ScanReachableSymbols
        if (!Callbacks.scan(RI.getData()))
            return false;
    }
    return true;
}
```
针对**LazyCompoundVal**，我们给出一段示例，并演示**scan(nonloc::LazyCompoundVal)**是怎么遍历**LazyCompoundVal**的。
```
// test.c
struct A { int mem; int *ptr;};
struct Node {int value; int *ptr;};
extern int global;
int inc(int num)
{
    struct Node n1;
    n1.value = 0;
    n1.ptr = &global;
    struct A a1;
    a1.mem = num + global;
    a1.ptr = &n1.value;
    struct A a2;
    a2 = a1;
    return a2.mem;
}
```
假设**clang static analyzer**执行到***“a1 = a1;”***语句之前，此时**Environment**有以下几个条目 **[a2 : &a2]**、**[a1 : &a1]**、**[a1 : lazyCompoundVal{0xbc89588, a1}]**。首先遍历这些**Entry**，对于**Expression**活跃的**Entry**，使用**ScanReachableSymbols**遍历该**Expression**中的**Symbol**。在该程序点活跃的**Expression**有***&a2***、***lazyCompoundVal***。我们就以其中**lazyCompoundVal**为例，看一下**scan(nonloc::LazyCompoundVal)**的流程。
***注：关于Environment以及live information可以使用debug.ViewExplodedGraph和debug.DumpRelaxedLiveVars得到***
首先是**EnvironmentManager::removeDeadBindings()**中执行**RSScaner.scan(X)**语句，此时匹配到的方法是**scan(SVal val)**，然后该方法会调用**scan(nonloc::LazyCompoundVal)**，该函数会获取该**LazyCompoundVal**对应的**MemRegion**（在调试的过程中使用**p MemRegion->getString()**可以获得完整的**MemRegion**名字），最终会调用到**RegionStoreManager::scanReachableSymbols()**。其中会获取现在分析引擎所有的**RegionBindings**，然后找到变量***a1*** 对应的**ClusterBindings**，***a1*** 对应的**ClusterBindings**中有**2**个**Bindings**，分别是**[(a1, 0, direct) : reg_$0<num>]**、**[(a1, 64, direct) : &global]**，对这两种再次调用***scan*()** 方法（都是调用***scan(SVal val)**）。如下图所示，绿色路径表示对于活的**LazyCompoundVal**遍历路径，对于遍历到相关符号执行的具体动作由**SymbolVisitor**指定。 
![这里写图片描述](https://img-blog.csdn.net/20161204161213434)
#### scan(nonloc::CompoundVal)
**CompoundVal**的遍历规则很简单，就是循环遍历**CompoundVal**中的子**Val**。没有遍历**LazyCompoundVal**时需要的获取**Region**，然后再获取其**ClusterBindings**，然后再获取其子**Val**的过程。
#### scan(SVal val)
***scan(SVal)*** 方法是**ScanReachableSymbols**的入口方法，通过该方法对不同类型的**Val**进行分派，例如前面提到过的**LazyCompoundVal**和**CompoundVal**就是分别分派给***scan(nonloc::LazyCompoundVal)*** 和***scan(nonloc::CompoundVal)*** 实现的。该方法定义很简单，如下所示：
```
bool ScanReachableSymbols::scan(SVal val) {
    if (Optional<loc::MemRegionVal> X = val.getAs<loc::MemRegionVal>())
        return scan(X->getRegion());
    if (Optional<nonloc::LazyCompoundVal> X = val.getAs<nonloc::LazyCompoundVal>())
        return scan(*X);
    if (Optional<nonloc::LocAsInteger> X = val.getAs<nonloc::LocAsInteger>())
        return scan(X->getLoc());
    if (SymbolRef Sym = val.getAsSymbol())
        return scan(Sym);
    if (const SymExpr *Sym = val.getAsSymbolExpression())
        return scan(Sym);
    if (Optional<nonloc::CompoundVal> X = val.getAs<nonloc::CompoundVal>())
        return scan(*X);
    return true;
}
```
#### scan(const MemRegion*)
**scan(const MemRegion*)** 是遍历到**MemRegion**的时候需要调用的函数，从**Environment**循环遍历**EnvironmentEntry**时不会直接调用到**scan(const MemRegion*)**，因为**Entry**都是**<Expr, Val>**的**pair**。可以通过调用***scan(MemRegionVal)***，然后分派给**scan(const MemRegion*)**。我们先给出该函数的定义，然后再进行分析。
```
bool ScanReachableSymbols::scan(const MemRegion *R) {
    // (1) 如果遍历到了MemSpaceRegion，例如StackLocalSpaceRegion或
    // GlobalSystemSpaceRegion时直接返回。
    if (isa<MemSpaceRegion>(R))
        return true;
    bool wasVisited = !visited.insert(R).second;
    if (wasVisited)
        return true;
    // (2) 这里调用visitor的回调函数VisitMemRegion()，对于remove dead来说，
    // visitor就是注册的MarkLiveCallBack对象。
    if (!visitor.VisitMemRegion(R))
        return false;
    // (3) 这里调用visitor的回调函数VisitSymbolicMemRegion()。
    // If this is a symbolic region, visit the symbol for the region.
    if (const SymbolicRegion *SR = dyn_cast<SymbolicRegion>(R))
        if (!visitor.VisitSymbol(SR->getSymbol()))
            return false;
    // (4) 进行了上述的判断后，再判断当前是否是SubRegion，如果是的话，则遍历其SuperRegion。
    // If this is a subregion, also visit the parent regions.
    if (const SubRegion *SR = dyn_cast<SubRegion>(R)) {
        const MemRegion *Super = SR->getSuperRegion();
        if (!scan(super))
            return false;
        // (5) 当遍历完MemRegion以及其对应的SuperRegion后，继续递归遍历其
        // SuperRegion的ClusterBindings。
        // When we reach the topmost region, scan all symbols in it.
        if (isa<MemSpaceRegion>(Super)) {
            StoreManager &StoreMgr = state->getStateManager().getStoreManager();
            if (!StoreMgr.scanReachableSymbols(state->getStore(), SR, *this))
                return false;
        }
    }
    // BlockDataRegion
    return true;
}
```
**scan(const MemRegion*)**方法分为**5**个部分，这里我着重介绍一下**(2)**、**(3)**、**(4)**、**(5)**部分。
对于第**(2)**部分，我们调用**SymbolVisitor**的回调函数**VisitMemRegion()**，对于**remove dead**来说**SymbolVisitor**是**MarkLiveCallback**。对于***MarkLiveCallback::VisitMemRegion()*** 的定义如下：
```
bool VisitMemRegion(const MemRegion *R) override {
    // 回忆上面的markLive(const MemRegion *R)方法，将MemRegion加入到RegionRoots集合
    SymReaper.markLive(R);
    return true;
}
```
对于遍历到的**MemRegion**将其加入到**SymbolReaper**对象中的**RegionRoots**集合。
对于第**(3)**部分，调用**SymbolVisitor**对于**Symbol**的回调函数***VisitSymbol()***，***MarkLiveCallback::VisitSymbol(SymbolRef)*** 定义如下：
```
bool VisitSymbol(SymbolRef sym) override {
    // SymbolReaper的markLive(SymbolRef)方法，将sym添加到TheLiving集合
    SymReaper.markLive(sym);
    return true;
}
```
对于第**(4)**部分，将**MemRegion**添加到**SymReaper**对应的**Living**集合以后，获取当前**MemRegion**的**SuperRegion**，然后递归调用**scan(const MemRegion*)**。
对于第**(5)**部分，遍历**topmost region**对应的**ClusterBindings**，这个类似于**invalidateRegionsWorker**中的**ClusterAnalysis**，对于能够从**topmost region**访问到的**SVal**，都要调用**MarkLiveCallback**对应的回调函数。
我们继续沿着***scan(nonloc::LazyCompoundVal)*** 中的代码示例进行分析，上次我们介绍到***scan(nonloc::LazyCompoundVal)*** 获取**LazyComoundVal**对应的**MemRegion**以后，调用 ***RegionStoreManager::scanReachableSymbols()*** 方法遍历该**LazyCompoundVal**对应的**ClusterBindings**。其中第二个**Bindings**就是**MemRegionVal**，此时我们就需要调用**scan(const MemRegion*)**，这里我们将第二个Bindings的遍历信息补全如下图。
![这里写图片描述](https://img-blog.csdn.net/20161204164622579)
***注：原谅我的画图水平，看懂就好***
#### scan(const SymExpr*)
上面我们介绍了ScanReachableSymbols如何遍历MemRegion。这里我们介绍ScanReachableSymbols如何遍历SymExpr，下面是该方法的定义。
```
bool ScanReachableSymbols::scan(const SymExpr *sym){
    bool wasVisited = !visited.insert(sym).second;
    if (wasVisited)
        return true;
    if (!visitor.VisitSymbol(sym))
        return false;
    switch(sym->getKind()) {
        case SymExpr::SymbolRegionValueKind:
        case SymExpr::SymbolConjuredKind:
        case SymExpr::SymbolDerivedKind:
        case SymExpr::SymbolExtentKind:
        case SymExpr::SymbolMetadataKind:
            break;
        case SymExpr::SymbolCastKind:
            return scan(cast<SymbolCast>(sym)->getOperand());
        case SymExpr::SymbolIntExprKind:
            return scan(cast<SymIntExpr>(sym)->getLHS());
        case SymExpr::IntSymExprKind:
            return scan(cast<IntSymExpr>(sym)->getRHS());
        case SymExpr::SymSymExprKind:
            const SymSymExpr *x = cast<SymSymExpr>(sym);
            return scan(x->getLHS()) && scan(x->getRHS());
    }
    return true;
}
```
**ScanReachableSymbols**主要目的是遍历**Environment**中活跃***EnvironmentEntry*** 中的符号值，并将这些符号添加到**SymbolReaper**对象中。所以**scan(const SymExpr*)**会根据**SymExpr**的具体类型，指定遍历规则，例如对于**SymIntExpr**就需要遍历其**LHS**，并将其**LHS**添加到**SymbolReaper**中的**TheLiving**集合。
至此，我们介绍完了**clang static analyzer**在**remove dead**时对于**Environment**的处理。**Environment**会对活的**EnvironmentEntry**中所能包含到的所有**Symbols**、所有**Region**等等全部标识为**Live**，也就是存放到**SymbolReaper**对象中。不仅如此，对于**Region**来说还会将其**SuperRegion**及其**ClusterBindings**的触角所能接触到的**Region**全部加入**SymbolReaper**中的**RegionRoots**中。这里其实已经相当于做了一部分**cluster analysis**的工作，对于**RegionStore**来说还会有另一部分的**cluster analysis**工作需要做，我们会在后面的文章中继续介绍。
