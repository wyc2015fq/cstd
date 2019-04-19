# clang static analyzer源码分析（四） - 电影旅行敲代码 - CSDN博客
2016年09月07日 15:06:32[电影旅行敲代码](https://me.csdn.net/dashuniuniu)阅读数：2359
# 引子
在[上一篇文章](http://blog.csdn.net/dashuniuniu/article/details/52438233)中，非常粗略的过了一遍clang static analyzer相关源码的头文件，这篇文章简要介绍下clang static analyzer中的与CFG相关的类。
clang static analyzer中的[CFG](https://code.woboq.org/llvm/clang/include/clang/Analysis/CFG.h.html#clang::CFG)是AST层级的，也就是CFG的CFGElement还是AST树中的结点，CFG只是在AST上套了一层图结构。这一点在CFG类的注释信息中能够明确看出来。
## CFGElement
CFG中每个基本块的基本构成元素就是CFGElement，CFGElement是所有CFG基本元素的基类。声明如下：
```
/// CFGElement - Represents a top-level expression in a baic block.
class CFGElement {
public:
    enum Kind {
        // main kind
        Statement,
        Initializer,
        NewAllocator,
        // dtor kind
        AutomaticObjectObjectDtor,
        DeleteDtor,
        BaseDtor,
        MemberDtor,
        TemporaryDtor,
        DTOT_BEGIN = AutomaticObjectDtor,
        DTOR_END = TemporaryDtor
    };
protected:
    llvm::PointerIntPair<void*, 2> Data1;
    llvm::PointerIntPair<void*, 2> Data2;
    /// \brief Convert to the specified CFGElement type, asserting that this
    /// CFGElement is of the desired type.
    template<typename T>
    T castAs() const{
        assert(T::isKind(*this));
        T t;
        CFGElement& e = t;
        e = *this;
        return t;
    }
    // ...
};
```
CFGElement的定义很简单，就是两个数据成员Data1、Data2以及一些getAs()和castAs()模板函数。其中Data1和Data2在不同的情况下具有不同的含义，在CFGElement是普通语句的情况Data1表示的是AST中的节点，Data2是nullptr，没有任何含义；在CFGElement是ImplicitDtor的情况下，Data1表示的是VarDecl，Data2有可能表达式的是DeleteExpr，也有可能是Local Object所在的CompoundStmt。
CFGElement有很多的子类，分别表达式source-level上不同的语句类型，甚至包括C++中编译器隐式生成的Dtor。CFGElement的类图如下：
![这里写图片描述](https://img-blog.csdn.net/20160906212122805)
- CFGStmt
```cpp
class CFGStmt : public CFGElement {
public:
    CFGStmt(Stmt *S) : CFGElement(Statement, S) {}
    const Stmt *getStmt() const {
        return static_cast<const Stmt*>(Data1.getPointer());
    }
    // ...
};
```
CFGStmt可以说是CFGBlock中最常见的类型，只要是Stmt类型的语句在构建CFGBlock的时候都可以使用CFGStmt表示。Stmt的类图这里有参见[Stmt Class](http://clang.llvm.org/doxygen/classStmt.html)。
- CFGInitializer
```cpp
/// CFGInitializer - Represents C++ base or member initializer from
/// constructor's initialization list.
class CFGInitializer : public CFGElement {
public:
    CFGInitializer(CXXCtorInitializer *initializer) 
        : CFGElement(Initializer, initializer) {}
    CXXCtorInitializer* getInitializer() const {
        return static_cast<CXXCtorInitializer*>(Data1.getPointer());
    }
    // ...
}
```
CFGInitializer表示的是C++中比较常用一种语句类型，就是构造函数中的初始化列表中的初始化语句，例如 “Base(int num) : mem(num) {}” 中的 “mem(num)” 就可以使用CFGInitializer表示。
- CFGNewAllocator
```cpp
class CFGNewAllocator : public CFGElement {
public:
    explicit CFGNewAllocator(const CXXNewExpr *S)
        : CFGElement(NewAllocator, S) {}
    // Get the new expression
    const CXXNewExpr *getAllocatorExpr() const {
        return static_cast<CXXNewExpr*>(Data1.getPointer());
    }
    // ...
};
```
同样CFGNewAllocator也是C++中常见的语句，就是new 表达式，在AST中使用[CXXNewExpr](https://code.woboq.org/llvm/clang/include/clang/AST/ExprCXX.h.html#clang::CXXNewExpr)表示，例如”new CXXNewExpr(foo)”。
- CFGAutomaticObjDtor
```
/// CFGImplicitDtor - Represents C++ object destructor implicitly generated
/// by compiler on various occasions.
class CFGImplicitDtor : public CFGElement {
protected:
    CFGImplicitDtor() {}
    CFGImplicitDtor(Kind kind, const void *data1, const void *data2 = nullptr)
        : CFGElement(kind, data1, data2) {
        assert(kind >= DTOR_BEGIN && kind <= DTOR_END);
    }
};
```
CFGImplicitDtor代表了CFGBlock中的一类元素，就是编译器隐式产生的析构函数，例如栈上对象、临时对象以及绑定了临时对象的const引用在离开所在的Scope时都要调用对象的析构函数，另外CXXDeleteExpr以及子类的析构函数都要调用对象或者基类的析构函数。CFGImplicitDtor有五个子类，在CFGElement的类图中也有明确展示，这五个子类分别是CFGAutomaticObjDtor、CFGBaseDtor、CFGDeleteDtor、CFGMemberDtor以及CFGTemporaryDtor，我就不一一列举了。
## CFGTerminator
CFGTerminator是一类比较特殊的对象，这个不应该算作是CFGBlock的一部分，因为在构建CFG的过程中CFGTerminator已经“幻化”称为CFGBlock之间的边了。就像你编写一个checker使用checkPreStmt(const IfStmt*, …) 定位不到任何If语句一样。
就像下面的代码，在dump该代码CFG的时候，CFGBlock其实谁不存在IfStmt的。
```
int func(int parm)
{
    if (parm > 10)
        return 0;
    else
        return -parm;
}
// ---------------CFG As below----------------
int func(int parm)
 [B4 (ENTRY)]
   Succs (1): B3
 [B1]
   1: parm
   2: [B1.1] (ImplicitCastExpr, LValueToRValue, int)
   3: -[B1.2]
   4: return [B1.3];
   Preds (1): B3
   Succs (1): B0
 [B2]
   1: 0
   2: return [B2.1];
   Preds (1): B3
   Succs (1): B0
 [B3]
   1: parm
   2: [B3.1] (ImplicitCastExpr, LValueToRValue, int)
   3: 10
   4: [B3.2] > [B3.3]
   T: if [B3.4]
   Preds (1): B4
   Succs (2): B2 B1
 [B0 (EXIT)]
   Preds (2): B1 B2
```
在B3基本块中的最后一条语句是”[B3.2] > [B3.3]”，而”if [B3.4]” 则是作为Terminator存在的。
## CFGBlock
CFGBlock是CFG的“组成元素”，内部由一系列的CFGElement组成，当然还包括一系列的前驱（predecessor）和后继（successor）基本块以及Terminator。
```
int func(int parm)
{
    if (parm > 10)
        return 0;
    else
        return -parm;
}
```
上面代码的CFG如下图所示：
![这里写图片描述](https://img-blog.csdn.net/20160907101403571)
B3块有前驱基本块Entry，后继基本块B1和B2，而B3基本块中的CFGElement基本都是AST中的一些子元素。我们打印AST，将其与AST一一比对得到下面这张图。
![这里写图片描述](https://img-blog.csdn.net/20160907105412048)
上图中基本块B3中的CFGElement其实都是AST中的结点，只是CFGBlock会将AST中的子节点切的很碎，然后符号执行就是在这些非常碎的语句上进行。
CFGBlock的定义如下：
```
/// CFGBlock - Presents a single basic block in a source-level CFG.
/// It consists of:
/// 
/// (1) A set of statements /expressions(which may contain subexpressions).
/// (2) A "terminator" statement (not in the set of statements).
/// (3) A list of successors and predecessors.
/// 上面三句话说明了CFGBlock的构成。
/// Terminator: The terminator respresents the type of control-flow that occurs
/// at the end of the basic block. The terminator is a Stmt* referring to an
/// AST node that has control-flow: if-statements, breaks, loops, etc.
/// If the control-flow is conditional, the condition expression will appear
/// within the set of statements in the block (usually the last statement).
/// 上面的描述最后一句话，如果分支语句是条件分支的话，条件语句是作为CFGBlock的一部分存在的。
/// 这个也符合程序的执行过程，而且Terminator确实也只是一个跳转而已。
class CFGBlock {
    /// 使用vector存放CFGElement
    class ElementList {
    };
    /// Stmts - The set of statements in the basic block.
    ElementList Elements;
    /// ...
    /// Terminator - The terminator for a basic block that
    /// indicates the type of control-flow that occurs between a block
    /// and its successors.
    CFGTerminator Terminator;
    /// BlockID - A numerical ID assigned to a CFGBlock during construction
    /// of the CFG.
    unsigned BlockID;
 private:
     /// Predecessors/Successors -Keep track of the predecessor / successor
     /// CFG blocks.
     typedef BumpVector<AdjacentBlock> AdjacentBlocks;
     AdjacentBlocks Preds;
     AdjacentBlocks Succs;
    /// Parent - The parent CFG that owns this CFGblocks.
    CFG *Parent;
    /// 一些列的set*()方法和get*()方法
    /// ...
};
```
CFGBlock的定义来看，CFGBlock可以看作是一系列ASTNode的容器，附带上一些helper method.
## CFG
遍历AST构建control-flow graph最终会得到CFG类对象，CFG类主要包含一组CFGBlock以及一系列创建CFG的方法，下面是CFG类的简单定义。
```
/// CFG - Represents a source-level, intra-procedural CFG that represents the 
/// control-flow of a Stmt. The Stmt can represent an entire function body, 
/// or a single expression. A CFG will always contain one empty block that
/// represents the Exit point of the CFG. A CFG will also contain a designated
/// Entry block. The CFG solely represents control-flow; it consists of
/// CFGBlocks which are simply containers of Stmt*'s in the AST the CFG
/// was constructed from.
/// 注意："The CFG solely represents control-flow;"这句话表明，CFG只是用于
/// 表示控制流信息。而且CFG含有两个特别的元素Entry和Exit。
class CFG {
public:
    //===-------------------------------------------===//
    // CFGConstruction & Manipulation
    //===-------------------------------------------===//
    class BuildOptions {
    // ...      
    };
    /// buildCFG - Builds a CFG from an AST.
    static std::unique_ptr<CFG> buildCFG(const Decl *D, Stmt *AST, ASTContext *C,
                                        const BuildOptions &BO);
    /// createBlock - Create a new block in the CFG. The CFG owns the block;
    /// the caller should not directly free it.
    CFGBlock *createBlock();
    /// setEntry - Set the entry block of the CFG. This is typically used
    /// only during CFG construction. Most CFG clients expect that the
    /// entry block has no predecessors and contains no statements.
    void setEntry(CFGBlock *B) { Entry = B; }
private:
    CFGBlock *Entry;
    CFGBlock *Exit;
    unsigned NumBlockIDs;
    typedef BumpVector<CFGBlock*> CFGBlockListTy;
    CFGBlockListTy Blocks;
};
```
从上面可以看到CFG也就是一组CFGBlock，然后有巨多的helper method，在一些成员方法中最最重要还是BuildCFG(const Decl *D, const Stmt *AST, )方法，其中Decl其实就是FunctionDecl或者是CXXMethodDecl，而Stmt一般就是函数体。我们在[clang static analyzer源码分析（一）](http://blog.csdn.net/dashuniuniu/article/details/50773316)中提到的HandleCode()时，其中就有一行语句就是用来获取（创建）CFG的，而HandleCode()的主体就是function decl。
***注：[clang::CFG](https://code.woboq.org/llvm/clang/include/clang/Analysis/CFG.h.html#clang::CFG)与llvm中的[CFG](https://code.woboq.org/llvm/llvm/include/llvm/Analysis/CFG.h.html)是两回事，llvm没有显示的CFG结构，llvm中的CFG具体体现在inmemory-IR中。***
这里有一篇**slides**《[**Optimizing for Bugs Fixed**](http://i-cav.org/2016/wp-content/uploads/2016/03/OptimizingforBugsFixed.pdf)》中间涉及到了**clang static analyzer**选择**source-level CFG**的优缺点，下面是这篇slides的截图。
![这里写图片描述](https://img-blog.csdn.net/20161015143814438)
这里提到使用**source-level CFG**的优势就是方便编码位置信息，因为**clang static analyzer**在报错的时候会将位置信息以及产生该**bug**的路径给显示出来。而这样的缺点就是**clang CFG**就是很复杂，很难维护，需要记录并模拟的**AST**结点很多。相比之下，**LLVM IR**就更低层。
而比较好的解决办法就是使用更加底层的**IR**来作为静态分析的主体，此时的**CFG**也更加底层，摆脱了前段冗余复杂语言相关的特性（特别是**C++**）。它们列出了**Swift**的解决办法，就是在**IR**与**AST**间建立联系（话说，编译器不都是这样设计的吗），即保证了**CFG**的简单，又可以获取位置信息。 下面是图示： 
![这里写图片描述](https://img-blog.csdn.net/20161015144608333)
## CFG::buildCFG()
前面提到buildCFG()是CFG中用于创建CFG对象的静态函数，这个函数主要在[AnalysisDeclContext::getCFG()](https://code.woboq.org/llvm/clang/lib/Analysis/AnalysisDeclContext.cpp.html#219)中调用，基本上所有用到CFG的地方都是通过getCFG()来创建CFG的。
buildCFG()函数主要有两个操作，创建CFGBuilder，然后开始递归遍历AST创建CFG。整个遍历过程是逆序的，也就是succeccor限于predecessot创建，并且对于一条复杂语句来说，先创建其SubExpression，才会创建整个语句，这个过程有点儿类似于后序遍历过程。具体的创建过程我就不再分析了，不同的语句创建规则不甚相同，但都不是很复杂。
下一篇文章会分析clang static analyzer的内存模型以及值模型。
