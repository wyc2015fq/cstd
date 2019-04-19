# clang中的活跃性分析 - 电影旅行敲代码 - CSDN博客
2016年02月25日 13:26:58[电影旅行敲代码](https://me.csdn.net/dashuniuniu)阅读数：1747
## 引子
clang和LLVM都实现了活跃性分析，只是两者的目的不同，一个用于后续静态代码分析，一个用于后续的代码生成以及代码优化。clang活跃性分析实现主要在[LiveVariables.h](http://code.woboq.org/llvm/clang/include/clang/Analysis/Analyses/LiveVariables.h.html)和[LiveVariables.cpp](http://code.woboq.org/llvm/clang/lib/Analysis/LiveVariables.cpp.html)这两个文件中，和常见的活跃性分析不同，clang的活跃性分析主要用于clang static analyzer，在静态代码分析的过程中删除死的状态和符号值。
> 
There’s already an analysis, LiveVariables.cpp, that does this. It works on top of the CFG, and does a reverse dataflow analysis(using a worklist algorithm) to compute liveness information for variables. This liveness information is currently consumed by the static analyzer to prune out redundant information from the path state. [Ted Kremenek](http://clang-developers.42468.n3.nabble.com/Data-flow-analysis-in-Clang-td4042447.html#a4042449)
由于clang基于LLVM实现，clang编译完（词法、语法以及语义等）源码后生成LLVM IR，然后LLVM会对生成的IR做各种数据流分析，所以clang没有必要再搞一套自己的数据流分析框架（其实clang就是一个编译器前端）。只是clang后面的静态代码分析为了提高运行效率及内存使用率，需要知道在当前位置哪些语句、哪些变量是**Dead**的，然后删除这些死状态信息。
LLVM也实现了活跃性分析，只是不是迭代版本的，关于LLVM自己的数据流分析框架会在后面的源码剖析部分详细研究。
## clang活跃性分析核心类
clang活跃变量分析主要用到了5个类，如下所示：
- LiveVariables
- LivenessValues
- LiveVariablesImpl
- TransferFunctions
- DataflowWorklist
下面我们依次对着5个类进行分析。
### LiveVariables
LiveVariables是clang活跃性分析总括性的类，用于对整个CFG进行活跃性分析，并使用一个私有数据成员存储活跃性分析的结果。
```
class LiveVariables : public ManagedAnalysis {
public:
    // 表示CFG图中某个程序点处的活跃变量和活跃语句的集合
    // 每个数据流分析都有其对应的值域
    // 对于活跃性分析来说，值域就是所有变量和语句的集合
    // 而某个程序点处的“数据流值”就是值域的子集
    // LivenessValues represent dataflow_value
    class LivenessValues {
        ...;
    }；
    // compute the liveness information for a given CFG
    static LiveVariables *computeLiveness(AnalysisDeclContext &analysisContext, 
            bool killAtAssign);
    /// Return true if a variable is live at the end of a specified block.
    bool isLive(const CFGBlock *B, const VarDecl *D);
    /// Returns true if a variable is live at the beginning of 
    /// the statement. This query only works if liveness information 
    /// has been recorded at the statement level (see runonAllBlocks), and 
    /// only returns liveness information for block-level expressions.
    bool isLive(const Stmt *S, const VarDecl *D);
    /// Returns true the block-level expression "value" is live
    /// before the given block-level expression (see runOnAllBlocks)
    bool isLive(const Stmt *Loc, const Stmt *StmtVal);
    /// 创建一个经过活跃性分析的LiveVariables（其中包含着分析结果）
    static LiveVariables *create(AnalysisDeclContext &analysisContext) {
        return computeLiveness(analysisContext, true);
    }
private:
    LiveVariables(void *impl);
    void *impl; 
};
```
对整个CFG进行活跃性分析的方法是**computeLiveness()**，该方法会在活跃性分析结束时，返回一个LiveVariables的指针。该类有一个私有数据成员**void* impl**，指向活跃性分析的结果。
***注：这里使用的是C++中的[Pimpl（pointer to implementation）](https://msdn.microsoft.com/en-us/library/hh438477.aspx)桥接设计模式，主要是为了分离数据并减轻编译时的压力。***
该类主要是通过静态方法**create()**调用**computeLiveness()**执行活跃性分析，并将分析的结果存储到**void *impl**中。由于活跃性分析其实只需执行一次，所以使用了[单例模式](https://en.wikipedia.org/wiki/Singleton_pattern)，将构造函数设为私有，然后定义一个静态方法调用构造函数来构造对象。
### LivenessValues
前面我们提到过LivenessValues就是各个程序点的数据流值，对于clang活跃性分析来说，数据流值就是 **VarDecl*** 和 **Stmt*** 的集合。LivenessValues定义如下：
```
class LivenessValues {
public:
    llvm::ImmutableSet<const Stmt*> liveStmts;
    llvm::ImmutableSet<const VarDecl*> liveDecls;
    bool equals(const LivenessValues &V) const;
    LivenessValues() 
        : liveStmts(nullptr), liveDecls(nullptr) {}
    LivenessValues(llvm::ImmutableSet<const Stmt*> LiveStmts,
            llvm::ImmutableSet<const VarDecl*> LiveDecls)
            : liveStmts(LiveStmts), liveDecls(LiveDecls) {}
    bool isLive(const Stmt *S) const;
    bool isLive(const VarDecl* D) const;
}
```
**LivenessValues**的定义非常简单，是由两个用于存储**活跃变量**和**活跃语句**的ImmutableSet构成的，ImmutableSet是LLVM自己定义的数据结构（用AVL tree实现），除了ImmutableSet, LLVM还定义了很多其他很高效的数据结构。
### LiveVariablesImpl
由于LiveVariables使用桥接模式实现，那么自然有一些功能需要Impl实现或者承担。LiveVariablesImpl大抵实现了三个功能，其一就是存储活跃性分析的结果，使用map实现结果的存储，map key是**“程序点”**，map value就是**LivenessValues**；其二是进行过程内的活跃性分析，主体函数是runOnBlock()；其三是实现基本块之间分析结果的聚合，主体函数是merge。当然clang的活跃性分析是在源码层面上做的，所以**程序点**其实就是普通的程序语句间的点。LiveVariablesImpl定义如下：
```
class LiveVariablesImpl {
public:
    // 用于存储数据流分析时的相关信息
    AnalysisDeclContext &analysisContext;
    // 下面两个对象用于生成一个ImmutableSet
    llvm::ImmutableSet<const Stmt*>::Factory SSetFact;
    llvm::ImmutableSet<const VarDecl*>::Factory DSetFact;
    // 存储着所有CFG基本块尾部的数据流值
    llvm::DenseMap<const CFGBlock*, LiveVariables::LivenessValues> blocksEndToLiveness;
    // 存储着所有CFG基本块头部的数据路值
    llvm::DenseMap<const CFGBlock*, LiveVariables::LivenessValues> blocksBeginToLiveness; 
    // 存储着所有语句之前程序点的数据流值
    llvm::DenseMap<const Stmt*, LiveVariables::LivenessValues> stmtsBeginToLiveness; 
    // 存储着被赋值的DeclRefExpr(用于生成变量use时排除这些DeclRefExpr)
    llvm::DenseMap<const DeclRefExpr*, unsigned> inAssignment; 
    const bool killAtAssign;
    // 数据流值的合并（控制流约束），对于活跃性分析来说就是简单的合并
    LiveVariables::LivenessValues merge( LiveVariables::LivenessValues valsA,
    LiveVariables::LivenessValues valsB);
    // 在基本块内部进行活跃性分析（遍历基本块内部的所有语句）
    LiveVariables::LivenessValues runOnBlock(
        const CFGBlock *block, LiveVariables::LivenessValues val, LiveVariables::Observer *obs = nullptr);
};
```
从LiveVariablesImpl的定义可以看出，这个类存储着活跃性分析的结果。其中还有一个成员方法用于实现基本块内的活跃性分析（亦即数据流值的向前传播）。
### TransferFunctions
从字面意思上也可以看得出来，这个类表示基本块内部的数据流传递函数，这是数据流分析中最重要的一部分，这个决定着数据流值是如何沿着CFG中的路径传递的。
TransferFunctions主要实现思想就是遍历某个语句的子语句，根据语句的不同，在数据流值上施加不同“影响”，并将数据流值逆向向前传播。
```
class TransferFunctions : public StmtVisitor<TransferFunctions> {
    LiveVariablesImpl &LV;
    LiveVariables::LivenessValues &val;
    const CFGBlock *currentBlock;
public:
    TransferFunctions() {}
    // 遍历二元运算符时的回调函数，函数内会相应的修改val
    void VisitBinaryOperator(BinaryOperator* BO);
    void VisitBlockExpr(BlockExpr* BE);
    // 遍历到变量使用时的回调函数
    void VisitDeclRefExpr(DeclRefExpr* DR);
    // 遍历到声明语句时的回调函数
    void VisitDeclStmt(DeclStmt* DS);
    void Visit(Stmt *S);
};
```
TransferFunctions继承自StmtVisitor<>，StmtVisitor是Clang写的一个遍历给定语句的接口，例如遍历函数体FunctionBody。相似的还有RecursiveASTVisitor( RAV )，只是RAV是用于遍历整个编译单元的。可以把TransferFunctions看做一个switch语句，根据遍历语句的不同向数据流值施加不同的影响。
### DataflowWorklist
这个类主要用于存储等待进行数据流分析的基本块，由于活跃性分析是后向数据流分析，所以先将CFG中的所有基本块逆序存入DataflowWorklist，然后对其中的基本块依次进行数据流分析（当然，也可以使用栈来实现）。另外为了提高效率，如果某个基本块的入口数据流值发生了改变，那么将该基本块的所有前驱加入工作列表。
```
class DataflowWorklist {
    SmallVector<const CFGBlock *, 20> worklist;
    llvm::BitVector enqueueBlocks;
    PostOrderCFGView *POV;
public:
    DataflowWorklist(const CFG &cfg, AnalysisDeclContext &Ctx) :
        enqueueBlocks(cfg.getNumberBlockIDs()),
        POV(Ctx.getAnalysis<PostOrderCFGView>()) {}
    // 将基本块加入工作列表
    void enqueueBlock(const CFGBlock* block);
    // 将当前基本块的前驱基本块加入工作列表
    void enqueuePredecessors(const CFGBlock* block);
    // 从工作列表中删除基本块
    const CFGBlock* dequeue();
    // 工作列表逆序
    void sortWorklist();
};
```
## clang活跃性分析主要方法
### computeLiveness
该函数是LiveVariables类中的一个静态方法，是进行数据流分析的总括性方法。
```cpp
LiveVariables* LiveVariables::computeLiveness(AnalysisDeclContext &AC,
        bool killAtAssign) {
    // No CFG? Bail out
    // 没有CFG则直接退出
    CFG *cfg = AC.getCFG();
    if (!cfg)
        return nullptr;
    // The analysis currently has scalability issues for very large CFGs.
    // Bail out if it looks too large.
    // CFG图太大的话，也直接退出
    if (cfg->getNumBlockIDs() > 300000)
        return nullptr;
    // LV存储着数据流分析的结果
    LiveVariablesImpl* LV = new LiveVariablesImpl(AC, killAtAssign);
    // 构建一个工作列表，将EXIT基本块作为整个数据流分析的开始
    DataflowWorklist worklist(*cfg, AC);
    // 获取CFG中基本块的个数，构建一个BitVector
    llvm::BitVector everAnalyzedBlock(cfg->getNumBlockIDs());
    // FIXME: we should enqueue using post order.
    for (CFG::const_iterator it = cfg->begin(), ei = cfg->end(); it != ei; ++it) {
        const CFGBlock* block = *it;
        // 将当前基本块入队列
        worklist.enqueueBlock(block);
        // FIXME: Scan for DeclRefExpr using in the LHS of an assignment.
        // 首先后向数据流分析中需要获取到哪些变量被用到
        // 用的方法是定位到DeclRefExpr，然后获取到其VarDecl，将其加入到use集里
        // 但是后向数据流分析中，不能判断DeclRefExpr在左侧还是在右侧
        // 所以先将左侧的DeclRefExpr存储起来，然后数据流分析再将其use集里的左侧的DeclRefExpr删除
        if (killAtAssign)
            for (CFGBlock::const_iterator bi = block->begin(), be = block->end();
                bi != be; ++bi){
                if (Optional<CFGStmt> cs = bi->getAs<CFGStmt>()) {
                    // 然后在里面判断是否是二元赋值运算符
                    // 如果左侧是DeclRefExpr的话，则存储起来
                }
            }
    }
    // 将worklist逆序
    worklist.sortWorklist();
    // 这个while循环就是用于数据流分析的
    // 该循环得出的数据流解，最终得到一个最小不动点
    // 数据流值LivenessValues就是在该循环里不断迭代更新的
    while (const CFGBlock* block = worklist.dequeue()) {
        // Determine if the block's end value has changed. If not, we have nothing left to do for this block.
        // 由于活跃性分析是迭代分析，所以需要一个标识来判断是否到达一个稳定解
        // 所以需要存储基本块旧的出口值，然后判断新值和旧的值是否相等
        LivenessValues &prevVal = LV->blocksEndToLiveness[block];
        // Merge the values of all successor blocks.
        // 后向数据流分析中，将其后继基本块的入口数据流值合并得到新的数据流值
        // 活跃性分析，交汇运算是并集
        for (CFGBlock::const_succ_iterator it = block->succ_begin(),
                ei = block->succ_end(); it != ei; ++it) {
            if (const CFGBlock* succ = *it) {
                val = LV->merge(val, LV->blocksBeginToLiveness[succ]);
            }
        }   
        // 如果该基本块没有分析过，则直接将得到的数据流值更新到LV中
        if (!everAnalyzedBlock[block->getBlockID()])
            everAnalyzedBlock[block->getBlockID()] = true;
        // 如果该基本块分析过，则判断新值和旧值是否相同
        // 相同的话，则直接跳过该基本块
        else if (prevVal.equals(val))
            continue;
        prevVal = val;
        // update the dataflow value for the start of this block
        // 下面这行代码实现的是基本块内部的数据流分析，将基本块的out[val] -> in[val]
        LV->blocksBeginToLiveness[block] = LV->runOnBlock(block, val);
        worklist.enqueuePredecessors(block);
    }
    return new LiveVariables(LV);
}
```
computeLiveness()方法的核心代码就是while循环，主要是做两个工作，求解数据流方程和控制流方程。由于活跃性分析是后向数据流分析，所以先“求解”控制流方程，亦即合并当前基本块对应后继基本块的入口数据流值，然后“求解”数据流方程，亦即将当前基本块的出口数据流值后向传播到基本块的入口处。最后将求得的入口数据流值回写到LV( LiveVariablesImpl )中。
### runOnBlock
该方法主要用于实现基本块内的活跃性分析，并对相应位置的数据流值进行实时的更新。
```cpp
LiveVariables::LivenessValues LiveVariablesImpl::runOnBlock(const CFGBlock *block,
    LiveVariables::LivenessValues val, LiveVariables::Observer *obs) {
    TransferFunctions TF(*this, val, obs, block);
    // Visit the terminator(if any)
    if (const Stmt* term = block->getTerminator())
        TF.Visit(const_cast<Stmt*>(term));
    // Apply the transfer functions for all Stmts in the block
    for (CFGBlock::const_reverse_iterator it = block->rbegin(),
        ei = block->rend(); it != ei, ++it) {
        const CFGElement &elem = *it;
        // 首先判断当前CFG元素是否是自动生成的析构函数或者指向临时值的常量引用
        // 由于编译器在编译的时候需要添加些处理代码，例如析构函数等
        // 这些操作在SourceCode并不存在，所以首先对其作处理
        // 如果是的话，就将相应的变量置为活的
        if (Optional<CFGAutomaticObjDtor> Dtor = 
                elem.getAs<CFGAutomaticObjDtor>()) {
            val.liveDecls = DSetFact.add(val.liveDecls, Dtor->getVarDecl());
            continue;
        }
        // 如果不是stmt则跳出并继续下一循环，
        if (!elem.getAs<CFGStmt>())
            continue;
        const Stmt* S = elem.castAs<CFGStmt>().getStmt();
        // 在是stmt也就是源码级别的程序语句，那么stmtVisitor就派上用场了
        // 可以使用TF(TransferFunctions)根据程序语句对数据流值进行相应的修改
        TF.Visit(const_cast<Stmt*>(S));
        // 对数据流值进行相应的更新
        stmtsToLiveness[S] = val;
    }
    return val;
}
```
runOnBlock()函数的逻辑比较简单，在做特殊处理后，对TransferFunctions的一次包装。在这个函数中出现了CFGAutomaticObjDtor，这个是CFG中的一个特殊类型，定义如下：
```cpp
/// CFGAutomaticObjDtor - Represents C++ object destructor implicitly generated
/// for automatic object(objects created on the stack) or temporary bound to const reference at the point
/// of leaving its local scope.
class CFGAutomaticObjDtor: public CFGImplicitDtor {
public:
    CFGAutomaticObjDtor(const VarDecl *var, const Stmt *stmt)
        : CFGImplicitDtor(AutomaticObjectDtor, var, stmt) {}
    const VarDecl *getVarDecl() const {
        return static_cast<VarDecl*>(Datal.getPointer());
    }
    // Get statement end of which triggered the destructor call.
    const Stmt* getTriggerStmt() const {
        return static_cast<Stmt*>(Data2.getPointer());
    }
};
class CFGImplicitDtor : public CFGElement {
protected:
    CFGImplicitDtor() {}
    // ...
};
/// CFGDeleteDtor - Represents C++ object destructor generated
/// from a to delete
class CFGDeleteDtor : public CFGImplicitDtor {
public:
    // ...
    const CXXDeleteExpr *getDeleteExpr() const{
        return static_cast<CXXDeleteExpr*>(Data2.getPointer());
    }
};
/// CFGTemporaryDtor - Represents C++ object destructor implicitly generated
/// at the end of full expressions for temporary object.
class CFGTemporaryDtor : public CFGImplicitDtor {
public:
    CFGTemporaryDtor(CXXBindTemporaryExpr *expr)
        : CFGImplicitDtor(TemporaryDtor, expr, nullptr) {}
    // ...
}
// ...
```
从定义中可以看出**CFGAutomaticObjDtor**是众多**CFGImplicitDtor**中的一种，表示为栈上对象或者指向临时变量的常量左值引用超出边界时隐式生成的析构函数。其中常量引用可以用于延长临时变量的声明周期，但是常量引用也有其scope，当超出其scope bound的时候就会析构该临时对象，关于常量引用等相关技术细节也是很有意思的。这里举一个小例子，这个例子取自stackoverflow上的[Why not non-const reference to temporary objects?](http://stackoverflow.com/questions/13826897/why-not-non-const-reference-to-temporary-objects)话题。后面还会详细介绍C++引用相关的知识。
```cpp
#include <string>
#include <iostream>
using namespace std;
class Sandbox
{
public:
    Sandbox(const string& n) : member(n) {}
    const string& member;
};
int main()
{
    Sandbox sandbox(string("four"));
    cout << "The answer is: " << sandbox.member << endl;
    return 0;
}
// Give output of:
// The answer is:
// Instead of:
// The answer is: four
```
**string(“four”)**会产生一个临时变量，该临时变量的绑定到Sandbox的构造函数中的常量引用形参**n**上，n的作用域仅限于Sandbox的构造函数，当其构造完成后，那个临时string类型的变量就自动析构掉了。
> 
The lifetime extension is not transitive through a function argument. §12.2/5 [class.temporary]
**A temporary bound to a reference parameter in a function call (§5.2.2 [expr.call]) persists until the completion of the full expression containing the call.**
也就是调用构造函数的那个表达式完成之后，该临时变量就直接析构掉了，所以输出结果什么都没有。类似的这种编译器添加的处理代码还有很多，像上面我么给出的继承自**CFGImplicitDtor**的类有很多，这些东西对于我们理解C++的一些特性非常有帮助。
言归正传，**runOnBlock**函数在处理完内部基本块的活跃性分析之后，生成该基本块入口处的新数据流值，并将其更新到LV中
## clang静态分析和clang的活跃性分析的关系
在clang静态分析中，有一个**ExprEngine**类，这个类实现了路径敏感的数据流分析，在CFG上执行数据流传输函数（transfer functions）用来创建扩展图（ExplodedGraph），扩展图就是将CFG进行路径展开后得到的图，关于其中的细节，我们会在后面Clang静态代码分析的源码剖析中详述。**ExprEngin**中有一个removeDead()方法，该方法实现静态分析器的垃圾回收。
下面是**removeDead**方法的定义：
```
/// \brief Run the analyzer's garbage collection - remove dead symbols and bindings from the state.
    void ExprEngine::removeDead(ExplodedNode *Pred, ExplodedNodeSet &Out,
                         const Stmt *ReferenceStmt,
                         const LocationContext *LC,
                         const Stmt *DiagnosticStmt,
                         ProgramPoint::Kind K) {
    ProgramStateRef CleandedState = Pred->getState();
    // ...
    // SymbolReaper类就是一个删除死符号值的类
    SymbolReaper SymReaper(SFC, ReferenceStmt, SymMgr, getStoreManager());
    // Clang静态分析和普通代码分析器不同的地方在于
    // 其使用了checker的架构，静态分析核心引擎执行代码分析
    // 使用checker挂在到执行引擎上，然后在合适的地方回调这些checker的函数
    getCheckerManager().runCheckersForLiveSymbols(CleanedState, SymReaper);
}
```
其中SymbolReaper是removeDead()方法的核心类，这个类实现死符号值的消除，这个类肯定会用到我们前面所叙述的活跃性分析的信息。SymbolReaper类的定义如下：
```
/// \brief A class responsible for cleaning up unused symbols.
class SymbolReaper {
    enum SymbolStatus {
        NotProcessed,
        HaveMarkedDependents
    };
    // 活符号值的Map
    SymbolMapTy TheLiving;
    // 正在使用的符号信息的Map
    SymbolSetTy MetadataInUse;
    // 死符号值的Map
    SymbolSetTy TheDead;
    // 当前静态分析到的Stack栈上下文环境（也就是所处函数的信息）
    const StackFrameContext* LCtx;
    // 当前静态分析到的语句位置
    // 前面进行活跃性分析得到的结果中，有一项是关于各个Stmt位置的数据流值的结果
    const Stmt* Loc;
    // ...
public:
    bool isLive(SymbolRef sym);
    bool isLiveRegion(const MemRegion* region);
    //
    SymbolReaper::isLive(const Stmt *ExprVal, const LocationContext *ELCtx) const {
        if (LCtx == nullptr)
        return false;
        if (LCtx != ELCtx) {
        // If the reaper's location context is a parent of the expression's
        // location context, then the expression value is now "out of scope".
            if (LCtx->isParentOf(ELCtx))
                return false;
            return true;
        }
      // If no statement is provided, everything is this and parent contexts is live.
        if (!Loc)
            return true;
        // 下面这行代码就是执行活跃性分析的代码，然后使用isLive()获取活跃性分析的结果
        return LCtx->getAnalysis<RelaxedLiveVariables>()->isLive(Loc, ExprVal);
    }
};
```
其中最后一行代码就是用到了我们前面提到的clang的活跃性分析。后面我们会在其他博客中详细研究clang静态分析相关的路径敏感的数据流分析和LLVM的数据流分析框架。
