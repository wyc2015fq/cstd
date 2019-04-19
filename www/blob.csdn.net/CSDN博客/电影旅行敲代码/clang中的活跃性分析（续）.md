# clang中的活跃性分析（续） - 电影旅行敲代码 - CSDN博客
2016年11月10日 17:43:00[电影旅行敲代码](https://me.csdn.net/dashuniuniu)阅读数：1014
# 引子
我们在《[**clang**中的活跃性分析](http://blog.csdn.net/dashuniuniu/article/details/50723930)》中介绍了**clang**中的活跃性分析，活跃性分析的目的以及实现方式。其中提到活跃性分析是在**source-code level**的**CFG**上进行的，目的主要是服务于**clang static analyzer**。列举了**clang**活跃性分析的关键对象以及关键方法，例如**LiveVariables**是**clang**活跃性分析总括性质的类，**LiveValues**是程序点处的数据流值（包括**live variable**和**live statements**），**DataflowWorkList**用来表示数据流分析的工作队列，以及**computeLiveness()**和**runOnBlock()**等方法。
但是我们在介绍**clang**的活跃性分析的时候，没有给出实际代码来进行对应的分析，这篇文章给出一个对应的代码示例。并且讨论了**LiveVariables**和**RelaxedLiveVariables**的区别。
## 示例代码
我们借鉴**wiki**上**[Live variable analysis](https://en.wikipedia.org/wiki/Live_variable_analysis)**中的示例，示例代码如下所示：
```
int main(int argc, char *argv[]) {
    int a, b, c, d, x;
    a = 3;
    b = 5;
    d = 4;
    x = 100;
    if (a > b) {
        c = a + b;
        d = 2;
    }
    c = 4;
    return b * d + c;
}
```
上面的代码示例也是**clang developers**中一个问题所使用的示例[**Question on LiveVariables analysis**](http://clang-developers.42468.n3.nabble.com/Question-on-LiveVariables-analysis-td3404776.html)。我们使用**“$clang -cc1 -analyze -analyzer-checker=debug.DumpCFG test.cpp”**得到**CFG**如下所示：
```
int main(int argc, char *argv[])
 [B4 (ENTRY)]
   Succs (1): B3
 [B1]
   1: 4
   2: c
   3: [B1.2] = [B1.1]
   4: b
   5: [B1.4] (ImplicitCastExpr, LValueToRValue, int)
   6: d
   7: [B1.6] (ImplicitCastExpr, LValueToRValue, int)
   8: [B1.5] * [B1.7]
   9: c
  10: [B1.9] (ImplicitCastExpr, LValueToRValue, int)
  11: [B1.8] + [B1.10]
  12: return [B1.11];
   Preds (2): B2 B3
   Succs (1): B0
 [B2]
   1: a
   2: [B2.1] (ImplicitCastExpr, LValueToRValue, int)
   3: b
   4: [B2.3] (ImplicitCastExpr, LValueToRValue, int)
   5: [B2.2] + [B2.4]
   6: c
   7: [B2.6] = [B2.5]
   8: 2
   9: d
  10: [B2.9] = [B2.8]
   Preds (1): B3
   Succs (1): B1
 [B3]
   1: int a;
   2: int b;
   3: int c;
   4: int d;
   5: int x;
   6: 3
   7: a
   8: [B3.7] = [B3.6]
   9: 5
  10: b
  11: [B3.10] = [B3.9]
  12: 4
  13: d
  14: [B3.13] = [B3.12]
  15: 100
  16: x
  17: [B3.16] = [B3.15]
  18: a
  19: [B3.18] (ImplicitCastExpr, LValueToRValue, int)
  20: b
  21: [B3.20] (ImplicitCastExpr, LValueToRValue, int)
  22: [B3.19] > [B3.21]
   T: if [B3.22]
   Preds (1): B4
   Succs (2): B2 B1
 [B0 (EXIT)]
   Preds (1): B1
```
上面给出的是字面形式的**CFG**，下面是图片形式的**CFG**。从**CFG**中我们可以看到**clang**活跃性分析和通常编译后端活跃性分析的区别，**clang**活跃性分析的基本单位是**Expr**，就像**DeclRefExpr**和**BinaryOperator**都有可能作为某个运算的操作数。例如**B3**基本块**index**为**22**处，***[B3.19] > [B3.21]*** 中的两个操作数分别是关于变量***a***和变量***b***的隐式类型转换表达式（***LValueToRValue***）。这个两个隐式类型转换表达式在**index**为**22**之前的位置还是**Live**的，在该程序点以后就是**Dead**的。**clang活跃性分析的基本单位就是这些Expr**。而编译后端的活跃性分析的粒度更细，没有这些比较高层的类型信息。
![这里写图片描述](https://img-blog.csdn.net/20161110153410093)
下面我们给出该代码示例对应的活跃性分析结果。
## 活跃性分析结果
### LiveVariables & RelaxedLiveVariables
**clang**提供了两种活跃性分析，一种是[**LiveVairables**](https://code.woboq.org/llvm/clang/include/clang/Analysis/Analyses/LiveVariables.h.html#95)，一种是[**RelaxedLiveVariables**](https://code.woboq.org/llvm/clang/include/clang/Analysis/Analyses/LiveVariables.h.html#108)。从两者的名字应该也能开出来**RelaxedLiveVariables**在活跃性分析的时候是没有**LiveVariables**精确的，两者的唯一区别在于**赋值运算是否应该Kill左侧的变量**，具体是通过***computeLiveness(, bool killAtAssign)***中第二个参数实现的。相关细节在[VisitBinaryOperator(BinaryOperator *B)](https://code.woboq.org/llvm/clang/lib/Analysis/LiveVariables.cpp.html#_ZN12_GLOBAL__N_117TransferFunctions19VisitBinaryOperatorEPN5clang14BinaryOperatorE)函数中有很明确的展现，该函数的源代码如下所示：
```
void TransferFunctions::VisitBinaryOperator(BinaryOperator *B) {
    if (B->isAssignmentOp()) {
        if (!LV.killAtAssign)
            return;
        // Assigning to a variables?
        Expr *LHS = B->getLHS()->IgnoreParens();
        // 如果左侧是DeclRefExpr，并且不是引用，则kill掉左侧DeclRefExpr所指向的变量
        // 注意这里没有做别名分析，例如: 
        // int &a = b; 
        // a = 10; clang在这里直接返回，do_nothing，并没有做别名分析
        if (DeclRefExpr *DR = dyn_cast<DeclRefExpr>(LHS))
            if (const VarDecl *VD = dyn_cast<VarDecl>(DR->getDecl())) {
                // Assignments to reference don't kill the ref's address
                if (VD->getType()->isReferenceType())
                    return;
                if (!isAlwaysAlive(VD)) {
                    // The variable is now dead.
                    val.liveDecls = LV.DSetFact.remove(val.liveDecls, VD);
                }
                if (observer)
                    oberver->observerKill(DR);
            }
    }
}
```
关于活跃性分析参见[数据流分析（二）](http://blog.csdn.net/dashuniuniu/article/details/50709160)中的活跃变量分析，里面提到如果对某个变量重新赋值，那么要***kill*** 掉该变量，如下图所示：
![这里写图片描述](https://img-blog.csdn.net/20161110160715609)
还是以开头的示例代码为例，使用**LiveVariables**打印出的结果如下：
```
[ B0 (live variables at block exit) ]
[ B1 (live variables at block exit) ]
[ B2 (live variables at block exit) ]
 b <test.cpp:3:9> 
 d <test.cpp:3:15>
[ B3 (live variables at block exit) ]
 a <test.cpp:3:6>
 b <test.cpp:3:9>
 d <test.cpp:3:15>
[ B4 (live variables at block exit) ]
```
而使用**RelaxedLiveVariables**打印出的结果如下。
```
[ B0 (live variables at block exit) ]
[ B1 (live variables at block exit) ]
[ B2 (live variables at block exit) ]
 b <test.cpp:3:9>
 // 其实在B2结尾，c是不活跃的。因为if (a > b) {c = a + b; d = 2;} c = 4;
 // 在{c = a + b; d = 2;} 结尾维护c的值没有任何意义，因为c会被重新赋值
 // 但是在RelaxedLiveVariables中会放松界限，在c = 4; 处不会kill c
 // 由于活跃性分析是may分析，放宽条件也是安全的。
 c <test.cpp:3:12> 
 d <test.cpp:3:15>
[ B3 (live variables at block exit) ]
 a <test.cpp:3:6>
 b <test.cpp:3:9>
 c <test.cpp:3:12>
 d <test.cpp:3:15>
[ B4 (live variables at block exit) ]
```
虽然**RelaxedLiveVariables**放宽了条件，但不至于所有的变量都看做是活跃的。例如示例代码中的变量 ***x*** 就不是活跃的。这里稍微介绍以下变量是如何添加到活跃性结果中的，**clang**中使用**DeclRefExpr**来表示变量的使用，无论是左值还是右值，如果是右值的话，那么**DeclRefExpr**外面会套一层**ImplicitCastExpr**（***LValurToRValue***）。所以我们的关注点应该是[**VisitDeclRefExpr(DeclRefExpr *DR)**](https://code.woboq.org/llvm/clang/lib/Analysis/LiveVariables.cpp.html#_ZN12_GLOBAL__N_117TransferFunctions16VisitDeclRefExprEPN5clang11DeclRefExprE)，该函数如下所示：
```
void TransferFunctions::VisitDeclRefExpr(DeclRefExpr *DR) {
    if (const VarDecl *D = dyn_cast<VarDecl>(DR->getDecl()))
        if (!isAlwaysAlive(D) && LV.inAssignment.find(DR) == LV.inAssignment.end())
        // 对于DeclRefExpr，则将其对应的VarDecl添加到LivenessValue中
            val.liveDecls = LV.DSetFact.add(val.liveDecls, D);
}
```
### debug.DumpRelaxedLiveVars
官方提供的***debug.DumpLiveVars***采用的是**LiveVariables**（较为精确的活跃性分析），会在对变量赋值时 ***kill*** 掉该变量的活跃性。既然**RelaxedLiveVariables**不是很精确，那么它被用在哪里呢？**RelaxedLiveVariables**被用在**clang**静态分析引擎中，其实也就是说**LiveVariables**反而是没有用的（除了打印**live variables**之外）。
那么**clang**静态分析引擎为什么采用的是不精确的**RelaxedLiveVariables**？答案就在于**RelaxedLiveVariables**的不精确上，由于**LiveVariables**会在变量赋值时杀死该变量的活跃性，试想如果**clang**静态分析引擎如果采用的是**LiveVariables**，那么在每次遇到变量赋值语句之后，需要调用**removeDead()**时都要删除关于该变量的所有信息，例如调用**MemRegionManager**删除关于该变量的**Region**，调用**RegionStoreManager**删除该变量的**binding**值。其实这些都是没有必要的，删除了该变量的**region**，在下次遇到新的定义时（该变量被重新赋值），还要创建**region**，创建新的**binding**，效率不高，而且不符合程序语义。所以索性在变量赋值时，不让该变量失活（***其实我们这里关于活跃性分析中变量的概念有些模糊***），保留变量分配的**region**，保留变量的**binding**。
由于**clang**没有提供**RelaxedLiveVariables**的接口，所以如果想要获得**RelaxedLiveVariables**的信息，需要注册一个新的**checker**，也就是在**DebugCheckers.cpp**中添加一个新的**checker**类，并在**Checkers.td**中进行注册（***注意：clang4.0 将Checkers.td从clang/lib/StaticAnalyzer/Checkers中移到了include/clang/StaticAnalyzer/Checkers中***）。添加的代码如下：
```
// clang/lib/StaticAnalyzer/Checkers/DebugChecker.cpp
namespace {
    class RelaxedLiveVariablesDumper : public Checker<check::ASTCodeBody> {
    public:
        void checkASTCodeBody(const Decl *D, AnalysisManager& mgr,
                            BugReporter &BR) const {
            if (RelaxedLiveVariables* L = mgr.getAnalysis<RelaxedLiveVariables>(D)) {
                L->dumpBlockLiveness(mgr.getSourceManager());
            }
        }
    };
}
void ento::registerRelaxedLiveVariablesDumper(CheckerManager &mgr) {
    mgr.registerChecker<RelaxedLiveVariablesDumper>();
}
```
注册该**checker**到**Checkers.td**中。
```python
// clang4.0 
// clang/include/clang/StaticAnalyzer/Checkers/Checkers.td
def RelaxedLiveVariablesDumper : Checker<"DumpRelaxedLiveVars">,
    HelpText<"Print results of live variable analysis">,
    DescFile<"DebugCheckers.cpp">;
```
然后重新编译，使用**clang -cc1 -analyzer-checker-help | grep debug**得到**debug.DumpRelaxedLiveVars**的结果。
### dumpStmtsLiveness
**clang**提供的**dump**活跃性分析结果的方法很单一，是以**BasicBlock**为单位打印出来的，没有提供程序语句级别的**dump**方法，并且只打印了**live variables**。
**dump**出来的结果对于分析我们前面提过的**removeDead()**提供不了多少帮助，因为**removeDead()**默认是以**Stmt**为单位进行的，我们需要获取程序语句之间活跃性分析的结果。虽然**clang**活跃性分析可以得到这些结果，但是并没有提供这方面的接口。因此我模仿官方的[**LiveVariablesImpl::dumpBlockLiveness()**](https://code.woboq.org/llvm/clang/lib/Analysis/LiveVariables.cpp.html#_ZN12_GLOBAL__N_117LiveVariablesImpl17dumpBlockLivenessERKN5clang13SourceManagerE)，添加了一个简单的打印程序语句级别的方法***LiveVariablesImpl::dumpStmtsLiveness()***，该方法的定义如下：
```cpp
// clang/lib/Analysis/LiveVariables.cpp
// WangLiushuai add this method at 2016-11-8
void LiveVariablesImpl::dumpStmtsLiveness(const SourceManager &M) {
    std::vector<const Stmt *> vec;
    for (llvm::DenseMap<const Stmt *, LiveVariables::LivenessValues>::iterator 
            it = stmtsToLiveness.begin(), ei = stmtsToLiveness.end();
            it != ei; ++it) {
        vec.push_back(it->first);
    }
    std::sort(vec.begin(), vec.end(), [](const Stmt *A, const Stmt *B) {
        return A->getLocStart() < B->getLocStart();     
    });
    std::vector<const VarDecl*> declVec;
    std::vector<const Stmt *> stmtVec;
    // Get the liveness info.
    for (std::vector<const Stmt *>::iterator
            it = vec.begin(), ei = vec.end(); it != ei; ++it) {
        declVec.clear();
        stmtVec.clear();
        // (1) dump the stmt(pos) info
        llvm::errs() << "**************program point***********************\n";
        (*it)->dump();
        LiveVariables::LivenessValues vals = stmtsToLiveness[*it];
        // (2) dump the live info.
        llvm::errs() << "*************live info*************************\n";
        for (llvm::ImmutableSet<const VarDecl *>::iterator si = 
                vals.liveDecls.begin(),
                se = vals.liveDecls.end(); si != se; ++si) {
            declVec.push_back(*si);
        }
        for (llvm::ImmutableSet<const Stmt *>::iterator si = 
                vals.liveStmts.begin(),
                se = vals.liveStmts.end(); si != se; ++si) {
            stmtVec.push_back(*si);
        }
        std::sort(declVec.begin(), declVec.end(), [](const Decl *A, const Decl *B) {
            return A->getLocStart() < B->getLocStart();
        });
        std::sort(stmtVec.begin(), stmtVec.end(), [](const Stmt *A, const Stmt *B) {
            return A->getLocStart() < B->getLocStart();     
        });
        // (2.1) dump the var live info.
        for (std::vector<const VarDecl *>::iterator di = declVec.begin(),
                de = declVec.end(); di != de; ++di) {
            llvm::errs() << " " << (*di)->getDeclName().getAsString() << " ";
            (*di)->getLocation().dump(M);
            llvm::errs() << "\n";
        }
        // (2.2) dump the stmts live info.
        for (std::vector<const Stmt *>::iterator si = stmtVec.begin(),
                se = stmtVec.end(); si != se; ++si) {
            (*si)->dump();
            llvm::errs() << "\n";
        }
        llvm::errs() << "\n";
    }
}
```
***注：ubuntu16.04，clang-4.0***
上述方法很简单，基本是将**LivenessValues**中的活跃性分析结果打印出来。打印的顺序是按照代码在程序中的位置来打印的，并且是按照**CFG**中各个元素的顺序来打印的。打印的内容很多，这里我摘取一部分进行分析。
## 活跃性分析结果
下面我会按照从出口块到入口块的顺序进行依次分析（其实就是***post order***），内容会有些繁杂，我会摘取部分作为以后分析**removeDead()**的基础内容。
### 基本块B1
基本块B1是出口块的前驱基本块，该基本块的源码如下：
```
int main(int argc, char *argv[]) {
    int a, b, c, d, x; 
    a = 3;
    b = 5;
    d = 4;
    x = 100;
    if (a > b) {       
        c = a + b;
        d = 2;
    }
    c = 4;             <------ B1 start
    return b * d + c;  <------ B1 end
}
// CFG
[B1]
   1: 4
   2: c
   3: [B1.2] = [B1.1]
   4: b
   5: [B1.4] (ImplicitCastExpr, LValueToRValue, int)
   6: d
   7: [B1.6] (ImplicitCastExpr, LValueToRValue, int)
   8: [B1.5] * [B1.7]
   9: c
  10: [B1.9] (ImplicitCastExpr, LValueToRValue, int)
  11: [B1.8] + [B1.10]
  12: return [B1.11];
   Preds (2): B2 B3
   Succs (1): B0
```
添加完自定义的方法以后，重新编译**clang**然后使用**-analyzer-checker=debug.DumpRelaxedLiveVars**得到的基本块B1的结果如下。
```
**************program point***********************
BinaryOperator 0xaec2ff8 'int' lvalue '='
|-DeclRefExpr 0xaec2fb0 'int' lvalue Var 0xaec29e8 'c' 'int'
`-IntegerLiteral 0xaec2fd8 'int' 4
****************live info*************************
 b for_blog.cpp:3:9
 c for_blog.cpp:3:12
 d for_blog.cpp:3:15
DeclRefExpr 0xaec2fb0 'int' lvalue Var 0xaec29e8 'c' 'int'
IntegerLiteral 0xaec2fd8 'int' 4
**************program point***********************
DeclRefExpr 0xaec2fb0 'int' lvalue Var 0xaec29e8 'c' 'int'
****************live info*************************
 b for_blog.cpp:3:9
 c for_blog.cpp:3:12
 d for_blog.cpp:3:15
IntegerLiteral 0xaec2fd8 'int' 4
**************program point***********************
IntegerLiteral 0xaec2fd8 'int' 4
****************live info*************************
 b for_blog.cpp:3:9
 c for_blog.cpp:3:12
 d for_blog.cpp:3:15
/// return b * d + c; <---------------START
/// (1)
**************program point***********************
ReturnStmt 0xaec3130
`-BinaryOperator 0xaec3108 'int' '+'
  |-BinaryOperator 0xaec30a0 'int' '*'
  | |-ImplicitCastExpr 0xaec3070 'int' <LValueToRValue>
  | | `-DeclRefExpr 0xaec3020 'int' lvalue Var 0xaec2970 'b' 'int'
  | `-ImplicitCastExpr 0xaec3088 'int' <LValueToRValue>
  |   `-DeclRefExpr 0xaec3048 'int' lvalue Var 0xaec2a60 'd' 'int'
  `-ImplicitCastExpr 0xaec30f0 'int' <LValueToRValue>
    `-DeclRefExpr 0xaec30c8 'int' lvalue Var 0xaec29e8 'c' 'int'
****************live info*************************
BinaryOperator 0xaec3108 'int' '+'
|-BinaryOperator 0xaec30a0 'int' '*'
| |-ImplicitCastExpr 0xaec3070 'int' <LValueToRValue>
| | `-DeclRefExpr 0xaec3020 'int' lvalue Var 0xaec2970 'b' 'int'
| `-ImplicitCastExpr 0xaec3088 'int' <LValueToRValue>
|   `-DeclRefExpr 0xaec3048 'int' lvalue Var 0xaec2a60 'd' 'int'
`-ImplicitCastExpr 0xaec30f0 'int' <LValueToRValue>
  `-DeclRefExpr 0xaec30c8 'int' lvalue Var 0xaec29e8 'c' 'int'
// (2)
**************program point***********************
DeclRefExpr 0xaec3020 'int' lvalue Var 0xaec2970 'b' 'int'
****************live info*************************
 b for_blog.cpp:3:9
 c for_blog.cpp:3:12
 d for_blog.cpp:3:15
// (3)
**************program point***********************
BinaryOperator 0xaec3108 'int' '+'
|-BinaryOperator 0xaec30a0 'int' '*'
| |-ImplicitCastExpr 0xaec3070 'int' <LValueToRValue>
| | `-DeclRefExpr 0xaec3020 'int' lvalue Var 0xaec2970 'b' 'int'
| `-ImplicitCastExpr 0xaec3088 'int' <LValueToRValue>
|   `-DeclRefExpr 0xaec3048 'int' lvalue Var 0xaec2a60 'd' 'int'
`-ImplicitCastExpr 0xaec30f0 'int' <LValueToRValue>
  `-DeclRefExpr 0xaec30c8 'int' lvalue Var 0xaec29e8 'c' 'int'
****************live info*************************
BinaryOperator 0xaec30a0 'int' '*'
|-ImplicitCastExpr 0xaec3070 'int' <LValueToRValue>
| `-DeclRefExpr 0xaec3020 'int' lvalue Var 0xaec2970 'b' 'int'
`-ImplicitCastExpr 0xaec3088 'int' <LValueToRValue>
  `-DeclRefExpr 0xaec3048 'int' lvalue Var 0xaec2a60 'd' 'int'
ImplicitCastExpr 0xaec30f0 'int' <LValueToRValue>
`-DeclRefExpr 0xaec30c8 'int' lvalue Var 0xaec29e8 'c' 'int'
// (4)
**************program point***********************
BinaryOperator 0xaec30a0 'int' '*'
|-ImplicitCastExpr 0xaec3070 'int' <LValueToRValue>
| `-DeclRefExpr 0xaec3020 'int' lvalue Var 0xaec2970 'b' 'int'
`-ImplicitCastExpr 0xaec3088 'int' <LValueToRValue>
  `-DeclRefExpr 0xaec3048 'int' lvalue Var 0xaec2a60 'd' 'int'
****************live info*************************
 c for_blog.cpp:3:12
ImplicitCastExpr 0xaec3070 'int' <LValueToRValue>
`-DeclRefExpr 0xaec3020 'int' lvalue Var 0xaec2970 'b' 'int'
ImplicitCastExpr 0xaec3088 'int' <LValueToRValue>
`-DeclRefExpr 0xaec3048 'int' lvalue Var 0xaec2a60 'd' 'int'
// (5)
**************program point***********************
ImplicitCastExpr 0xaec3070 'int' <LValueToRValue>
`-DeclRefExpr 0xaec3020 'int' lvalue Var 0xaec2970 'b' 'int'
****************live info*************************
 c for_blog.cpp:3:12
 d for_blog.cpp:3:15
DeclRefExpr 0xaec3020 'int' lvalue Var 0xaec2970 'b' 'int'
// (6)
**************program point***********************
DeclRefExpr 0xaec3048 'int' lvalue Var 0xaec2a60 'd' 'int'
****************live info*************************
 c for_blog.cpp:3:12
 d for_blog.cpp:3:15
ImplicitCastExpr 0xaec3070 'int' <LValueToRValue>
`-DeclRefExpr 0xaec3020 'int' lvalue Var 0xaec2970 'b' 'int'
// (7)
**************program point***********************
ImplicitCastExpr 0xaec3088 'int' <LValueToRValue>
`-DeclRefExpr 0xaec3048 'int' lvalue Var 0xaec2a60 'd' 'int'
****************live info*************************
 c for_blog.cpp:3:12
ImplicitCastExpr 0xaec3070 'int' <LValueToRValue>
`-DeclRefExpr 0xaec3020 'int' lvalue Var 0xaec2970 'b' 'int'
DeclRefExpr 0xaec3048 'int' lvalue Var 0xaec2a60 'd' 'int'
// (8)
**************program point***********************
DeclRefExpr 0xaec30c8 'int' lvalue Var 0xaec29e8 'c' 'int'
****************live info*************************
 c for_blog.cpp:3:12
BinaryOperator 0xaec30a0 'int' '*'
|-ImplicitCastExpr 0xaec3070 'int' <LValueToRValue>
| `-DeclRefExpr 0xaec3020 'int' lvalue Var 0xaec2970 'b' 'int'
`-ImplicitCastExpr 0xaec3088 'int' <LValueToRValue>
  `-DeclRefExpr 0xaec3048 'int' lvalue Var 0xaec2a60 'd' 'int'
// (9)
**************program point***********************
ImplicitCastExpr 0xaec30f0 'int' <LValueToRValue>
`-DeclRefExpr 0xaec30c8 'int' lvalue Var 0xaec29e8 'c' 'int'
****************live info*************************
BinaryOperator 0xaec30a0 'int' '*'
|-ImplicitCastExpr 0xaec3070 'int' <LValueToRValue>
| `-DeclRefExpr 0xaec3020 'int' lvalue Var 0xaec2970 'b' 'int'
`-ImplicitCastExpr 0xaec3088 'int' <LValueToRValue>
  `-DeclRefExpr 0xaec3048 'int' lvalue Var 0xaec2a60 'd' 'int'
DeclRefExpr 0xaec30c8 'int' lvalue Var 0xaec29e8 'c' 'int'
/// return b * d + c; <---------------END
```
参照着**CFG**我们还是从后往前看，***B1*** 块最后一个**CFGElement**是***“12: return [B1.11];”***，也就是说**return**语句会用到***B1.11***，所以说需要将***B1.11***对应的信息添加到此处程序点的数据流值（活跃变量或者活跃子语句），而***B1.11***对应的是***“b*d + c”***。此时的活跃性分析结果对应于上面的**(1)**处。此处对应于源代码[**TransferFunctions::Visit(Stmt *S)**](https://code.woboq.org/llvm/clang/lib/Analysis/LiveVariables.cpp.html#_ZN12_GLOBAL__N_117TransferFunctions5VisitEPN5clang4StmtE)，对应的代码如下：
```
void TransferFunctions::Visit(Stmt *S) {
    // 前面我们提到过，VisitBinaryOperator()，只是针对赋值语句进行特殊处理
    StmtVisitor<TransferFunctions>::Visit(S);
    // 如果分析的当前语句是表达式，说明该表达式有可能在后面会被用到，在活跃值中。
    // 但是数据流分析完当前语句后，处于该语句前面的程序点，此时该语句还没有求值
    // 需要进行kill操作。
    if (isa<Expr>(S)) {
        val.liveStmts = LV.SSetFact.remove(val.liveStmts, S);
    }
    // 将当前语句所有的children添加到活跃结果值中
    for (Stmt *Child : S->children()) {
        if (Child)
            AddLiveStmt(val.liveStmts, LV.SSetFact, Child);
    }
}
```
继续往下分析，**CFG**的第**11**个**CFGElement**是***“[B1.8] + [B1.10]”***，对应于***“b*d+c”***，在分析完第**12**个**CFGElement**时的活跃值是**“[B1.8] + [B1.10]”**。分析***“[B1.8] + [B1.10]”***需要执行***kill*** 操作，所以删除**“[B1.8] + [B1.10]”**。并将其两个**children**，***[B1.8]*** 和 ***[B1.10]*** 添加到活跃值中。
依次类推，数据流分析的值从基本块结尾（**live-out**）不断更新直到基本块入口处（**live-in**）。然后前驱基本块结尾处（**live-out**）的数据流值需要对其后继基本块入口处（**live-in**）数据流值的聚合。我们需要格外关注对基本语句进行活跃性分析时这些语句的**children**，这个**children**都是临时值，在该语句之后是不活跃的，需要清理，而**removeDead()**就是依据这些活跃性分析结果对临时值（**Environment**）进行清理的。
