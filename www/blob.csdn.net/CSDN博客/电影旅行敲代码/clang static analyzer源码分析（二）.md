# clang static analyzer源码分析（二） - 电影旅行敲代码 - CSDN博客
2016年09月05日 10:12:54[电影旅行敲代码](https://me.csdn.net/dashuniuniu)阅读数：3223
# 引子
在[clang static analyzer源码分析（一）](http://blog.csdn.net/dashuniuniu/article/details/50773316)中我们简单介绍了 **[AnalysisConsumer](https://code.woboq.org/llvm/clang/lib/StaticAnalyzer/Frontend/AnalysisConsumer.cpp.html)** 这个类以及基于AST树的语法层级的代码检查，今天简单介绍下 [PathSensitiveChecks](https://en.wikipedia.org/wiki/Data-flow_analysis) 的概念，以及如何对clang static analyzer进行调试。
## path-sensitive analysis
在编译技术中接触到的[数据流分析](https://en.wikipedia.org/wiki/Data-flow_analysis#Sensitivities)都是路径不敏感的分析，因为在分析的时候并没有对各个路径区别开来，只是在汇聚节点对各个路径的信息进行简单的聚合。在传统的数据流分析中，**Join Point**处的数据流方程如下图所示：
第二行的数据流方程只是简单的将各个 **Pred** 结点（**BasicBlock** 结点）得到的信息进行了简单的聚合（依据数据流问题的不同，聚合的具体实现方式也不相同）。如下图所示源码，在 **func(a)** 处到达的定值有 2、3、4，但是传统的**Reaching-Definition** 分析并不会区分这些定值分别是从哪条路径传递过来并且在什么条件下传递过来的。
![这里写图片描述](https://img-blog.csdn.net/20160904212212214)
路径敏感的数据流分析是将路径信息以及相应的谓词信息纳入到分析过程中。
> 
A **path-sentitive analysis** computes different pieces of analysis information dependent on the predicates at conditional branch instructions.  
  For instance, if a branch contains a condition ***(x > 0)***, then on the ***fall-through*** path, the analysis would assume that **(x <= 0)** on the target of the branch it would assume that indeed **(x > 0)** holds.
按照上面的英文描述，路径敏感的分析应该是如下图所示：
![这里写图片描述](https://img-blog.csdn.net/20160904213605958)
在路径敏感的数据流分析过程中对于得到的每一份数据流分析结果都要维护沿途的“路径信息”，在 **path-sensitive analysis** 的分析中，常常使用 **ExplodedGraph** 来表示分析结果。
另外在**clang static analyzer**的分析也是一种**flow-sensitive analysis**，由于传统的数据流分析在**CFG**迭代求解时可以说的每一个**ProgramPoint**上的数据流信息，所以也属于**flow-sensitive analysis**，这里我们就不细说了。
> 
A **flow-sensitive analysis** takes into account the order of statement in a program. For example, a flow-insensitive pointer alias analysis may determine “variable *x* and *y* may refer to the same location”, while a flow-sensitive analysis may determine “**after statement 20, variable *x* and *y* may refer to the same location**“.
## Exploded Graph
***注：这一小节的内容选自[《C程序的静态-许中兴》](http://xueshu.baidu.com/s?wd=paperuri:%283b87d920576a849b9cc034c88d443316%29&filter=sc_long_sign&sc_ks_para=q=C%E7%A8%8B%E5%BA%8F%E7%9A%84%E9%9D%99%E6%80%81%E5%88%86%E6%9E%90&tn=SE_baiduxueshu_c1gjeupa&ie=utf-8&sc_us=3380701477227140199)，其中许中兴是clang static analyzer的贡献者之一。侵删***
在**flow-sensitive analysis**中，分析完之后产生的包含状态的**CFG**和原先的**CFG**结构是一样的，只是在每个程序点处关联上了一个**State**。
在**flow-sensitive analysis**中，如果把不同的（**ProgramPoint, State**）对看成不同的节点，也就是说即便程序点相同，状态不同的结点也看成不同的节点（*如上图中调用func(a)之前的程序点，在**ExplodedGraph**中却是两个完全不同的节点*），那么分析完之后产生的图的节点比原先的**CFG**的节点要多。把这个新的图称为扩展图（**expoded graph**）。由于不同的路径到达exit的状态不同，所以EOP中也包含了多个节点。
**CFG**，带状态的**CFG**和**Exploded Graph**的对比如下所示：
![这里写图片描述](https://img-blog.csdn.net/20160904220329401)
***注：截屏图，渣画质谅解***
上图中需要特别注意的是，带状态的**CFG**中节点是各个状态节点，其中的边则是各个**BasicBlock**（当然可以细化到语句）。而在**Exploded Graph**中的各个节点则是[ **ProgramPoint, State** ] 这样的状态对。
### Program Point
**Basic Block**包含常规语句（**normal statement**）和结束语句（**terminator**），结束语句包括：复合条件语句（**&&**，**||**），选择语句，**if**语句，**while**语句，**do-while**语句，**for**语句等等。一般来说**Basic Block**会包含一个结束语句，但是也有可能不包含结束语句，即所有语句都是常规语句。
在设计中区分3种程序点：
- **BlockEdge**，这是在两个block之间的一个点，记录了刚刚处理完的一个block和将要进入的一个block。
- **BlockEntrance**，这是在刚刚进入一个block的点，还没有处理该block的第一条语句。
- **PostStmt**，这是最多的一种程序点，在基本块的每一条非结束语句的后面都会有这样一个点。
## clang static analyzer调试及相关命令
熟悉clang static analyzer最好的方法莫过于dump出CFG和AST，然后单步调试clang static analyzer。
### 打印clang static analyzer可用checker
`$user:clang -cc1 -analyzer-checker-help`
打印出的信息很多，我就不放出来了，但是其中有几个checker是和调试息息相关的，我们会需要这些命令来获取相关的信息。
```
// ...
// 打印出static analyzer配置信息
debug.ConfigDumper              Dump config table
// 打印出CFG，clang static analyzer是在CFG上进行符号执行的
debug.DumpCFG                   Display Control-Flow Graphs
// 打印出CallGraph，我们在前面提到过，虽然在单个函数中是在CFG上进行符号执行的。
// 但是在多个函数的时候，path-sensitive analysis是按照CallGraph的拓扑序进行的
debug.DumpCallGraph             Display Call Graph
// 打印出引擎遍历function call的顺序
debug.DumpCalls                 Print calls as they are traversed by the engine
// 打印出CFG的支配性信息，对于数据流分析是极为重要的
debug.DumpDominators            Print the dominance tree for a given CFG
// 打印出活跃性信息，前面我们提到过为了减小内存占用，静态分析引擎会在合适的时候
// 删减Dead Variable（注：clang static analyzer是在AST级别上进行的）
debug.DumpLiveVars              Print results of live variable analysis
// 打印出引擎遍历的BranchCondition信息
debug.DumpTraversal             Print branch conditions as they are traversed by the engine
debug.ExprInspection            Check the analyzer's understanding of expressions
debug.Stats                     Emit warnings with analyzer statistics
debug.TaintTest                 Mark tainted symbols as such.
// 与DumpCFG相同，只是这个会使用graphviz将图形展示出来
debug.ViewCFG                   View Control-Flow Graphs using GraphViz
// 与DumpCallGraph相同
debug.ViewCallGraph             View Call Graph using GraphViz
// 打印出ExplodedGraph，这个是我们使用的很重要的命令，因为调试过程中有很多信
// 息，结合着CFG和ExplodedGraph能够很方便的定位到引擎现今执行到何处
debug.ViewExplodedGraph         View Exploded Graphs using GraphViz
```
例如我们需要在如下代码上进行clang static analyzer的调试，我们首先需要三个信息，CFG、AST和ExplodedGraph。
***note:下面这个代码是递归形式，引擎默认对递归和循环只进行上限为4次的分析，这个上限是可以手工设置的。***
```
int fac(int num)
{
    if (num == 0)
        return 1;
    else
        return num * fac(num - 1);
}
```
dump CFG如下：
```
$user:clang -cc1 -analyze -analyzer-checker=debug.DumpCFG fac.c
// ----------------- 如下为CFG---------------------
int fac(int num)
 [B4 (ENTRY)]
   Succs (1): B3
 [B1]
   1: num
   2: [B1.1] (ImplicitCastExpr, LValueToRValue, int)
   3: fac
   4: [B1.3] (ImplicitCastExpr, FunctionToPointerDecay, int (*)(int))
   5: num
   6: [B1.5] (ImplicitCastExpr, LValueToRValue, int)
   7: 1
   8: [B1.6] - [B1.7]
   9: [B1.4]([B1.8])
  10: [B1.2] * [B1.9]
  11: return [B1.10];
   Preds (1): B3
   Succs (1): B0
 [B2]
   1: 1
   2: return [B2.1];
   Preds (1): B3
   Succs (1): B0
 [B3]
   1: num
   2: [B3.1] (ImplicitCastExpr, LValueToRValue, int)
   3: 0
   4: [B3.2] == [B3.3]
   T: if [B3.4]
   Preds (1): B4
   Succs (2): B2 B1
 [B0 (EXIT)]
   Preds (2): B1 B2
```
dump AST如下所示：
```
// -Xclang 可以打印头文件并且彩色显示打印结果，-cc1 不能打印头文件，字符显示打印结果
$user:clang -Xclang -cc1 -ast-dump fac_test.c -c
// -------------- 下面为AST-----------------
TranslationUnitDecl 0x4025a60 <<invalid sloc>> <invalid sloc>
|-TypedefDecl 0x4025f60 <<invalid sloc>> <invalid sloc> implicit __int128_t '__int128'
|-TypedefDecl 0x4025fc0 <<invalid sloc>> <invalid sloc> implicit __uint128_t 'unsigned __int128'
|-TypedefDecl 0x4026310 <<invalid sloc>> <invalid sloc> implicit __builtin_va_list '__va_list_tag [1]'
`-FunctionDecl 0x4026430 <fac_test.c:1:1, line:7:1> line:1:5 referenced fac 'int (int)'
  |-ParmVarDecl 0x4026370 <col:9, col:13> col:13 used num 'int'
  `-CompoundStmt 0x4026780 <line:2:1, line:7:1>
    `-IfStmt 0x4026750 <line:3:2, line:6:27>
      |-<<<NULL>>>
      |-BinaryOperator 0x4026538 <line:3:6, col:13> 'int' '=='
      | |-ImplicitCastExpr 0x4026520 <col:6> 'int' <LValueToRValue>
      | | `-DeclRefExpr 0x40264d8 <col:6> 'int' lvalue ParmVar 0x4026370 'num' 'int'
      | `-IntegerLiteral 0x4026500 <col:13> 'int' 0
      |-ReturnStmt 0x4026580 <line:4:3, col:10>
      | `-IntegerLiteral 0x4026560 <col:10> 'int' 1
      `-ReturnStmt 0x4026730 <line:6:3, col:27>
        `-BinaryOperator 0x4026708 <col:10, col:27> 'int' '*'
          |-ImplicitCastExpr 0x40266f0 <col:10> 'int' <LValueToRValue>
          | `-DeclRefExpr 0x40265a0 <col:10> 'int' lvalue ParmVar 0x4026370 'num' 'int'
          `-CallExpr 0x40266c0 <col:16, col:27> 'int'
            |-ImplicitCastExpr 0x40266a8 <col:16> 'int (*)(int)' <FunctionToPointerDecay>
            | `-DeclRefExpr 0x40265c8 <col:16> 'int (int)' Function 0x4026430 'fac' 'int (int)'
            `-BinaryOperator 0x4026650 <col:20, col:26> 'int' '-'
              |-ImplicitCastExpr 0x4026638 <col:20> 'int' <LValueToRValue>
              | `-DeclRefExpr 0x40265f0 <col:20> 'int' lvalue ParmVar 0x4026370 'num' 'int'
              `-IntegerLiteral 0x4026618 <col:26> 'int' 1
```
由于我在本机上安装的Graphviz不是很成功，所以只能先获得.ps文件，然后使用GSview打开。命令以及结果如下所示：
`$user:./clang -cc1 -analyze -analyzer-checker=debug.ViewExplodedGraph ../../../../fac.c`
得到.ps文件然后使用GSview打开如下，由于得到的ExplodedGraph太大，使用GSView打开需要刷新很长时间。
![这里写图片描述](https://img-blog.csdn.net/20160905100047849)
直接使用GSview打开内存会不够，我是先转成pdf，然后使用阅读器打开，ExplodedGraph的部分区域如下所示：
![这里写图片描述](https://img-blog.csdn.net/20160905100242761)
可以看到dump出的ExplodedGraph的内容很丰富，结合着CFG，在调试的过程中可很方便的获知当前处于哪一个ProgramPoint。我是在Ubuntu下使用gdbtui调试的clang static analyzer，命令如下：
```
$user: gdbtui --args ./clang -cc1 -analyze -analyzer-checker=debug.ViewExplodedGraph ../../../../fac.c
// -------------- 调试窗口如下-------------------
lqq/home/cilabsev1/file/For_WiuLiuShuai_moses/llvm/tools/clang/lib/StaticAnalyzer/Core/SimpleConstraintManager.cppqqqqqqqqqqqqqqqqqqqqqqqk
   x244                                                                                                                                     x
   x245       case BO_EQ:                                                                                                                   x
   x246         return assumeSymEQ(state, Sym, ConvertedInt, Adjustment);                                                                   x
   x247                                                                                                                                     x
   x248       case BO_NE:                                                                                                                   x
   x249         return assumeSymNE(state, Sym, ConvertedInt, Adjustment);                                                                   x
   x250                                                                                                                                     x
   x251       case BO_GT:                                                                                                                   x
   x252         return assumeSymGT(state, Sym, ConvertedInt, Adjustment);                                                                   x
   x253                                                                                                                                     x
   x254       case BO_GE:                                                                                                                   x
   x255         return assumeSymGE(state, Sym, ConvertedInt, Adjustment);                                                                   x
   x256                                                                                                                                     x
   x257       case BO_LT:                                                                                                                   x
   x258         return assumeSymLT(state, Sym, ConvertedInt, Adjustment);                                                                   x
   x259                                                                                                                                     x
   x260       case BO_LE:                                                                                                                   x
   x261         return assumeSymLE(state, Sym, ConvertedInt, Adjustment);                                                                   x
  >x262       } // end switch                                                                                                               x
   x263     }                                                                                                                               x
   x264                                                                                                                                     x
   x265     } // end of namespace ento                                                                                                      x
   x266                                                                                                                                     x
   mqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqj
multi-thre Thread 0x7ffff In: clang::ento::SimpleConstraintManager::assumeSymRel                                    Line: 262  PC: 0x21d3106 
    at /home/cilabsev1/file/For_WiuLiuShuai_moses/llvm/tools/clang/lib/StaticAnalyzer/Core/RangeConstraintManager.cpp:391
Breakpoint 62, (anonymous namespace)::RangeConstraintManager::GetRange (this=0x6eb87a0, state=..., sym=0x6eba4d0)
    at /home/cilabsev1/file/For_WiuLiuShuai_moses/llvm/tools/clang/lib/StaticAnalyzer/Core/RangeConstraintManager.cpp:394
(anonymous namespace)::RangeConstraintManager::assumeSymLE (this=0x6eb87a0, St=..., Sym=0x6eba4d0, Int=..., Adjustment=...)
    at /home/cilabsev1/file/For_WiuLiuShuai_moses/llvm/tools/clang/lib/StaticAnalyzer/Core/RangeConstraintManager.cpp:566
(gdb) p New.isEmpty()
$40 = false
(gdb) n
clang::ento::SimpleConstraintManager::assumeSymRel (this=0x6eb87a0, state=..., LHS=0x6eba6b8, op=clang::BO_LE, Int=...)
    at /home/cilabsev1/file/For_WiuLiuShuai_moses/llvm/tools/clang/lib/StaticAnalyzer/Core/SimpleConstraintManager.cpp:234
(gdb)
```
我调试的过程中保存了很多断点信息，断点信息文件内容如下可以作为参考。 
***note:我调试的环境是Ubuntu 14.04-64，gcc-4.8.4，clang3.6***
```
break AnalysisConsumer.cpp:506
break /home/cilabsev1/file/For_WiuLiuShuai_moses/llvm/tools/clang/lib/StaticAnalyzer/Frontend/AnalysisConsumer.cpp:521
break /home/cilabsev1/file/For_WiuLiuShuai_moses/llvm/tools/clang/lib/StaticAnalyzer/Frontend/AnalysisConsumer.cpp:521
break AnalysisConsumer.cpp:506
break /home/cilabsev1/file/For_WiuLiuShuai_moses/llvm/tools/clang/lib/StaticAnalyzer/Frontend/AnalysisConsumer.cpp:521
break /home/cilabsev1/file/For_WiuLiuShuai_moses/llvm/tools/clang/lib/StaticAnalyzer/Frontend/AnalysisConsumer.cpp:539
break /home/cilabsev1/file/For_WiuLiuShuai_moses/llvm/tools/clang/lib/StaticAnalyzer/Frontend/AnalysisConsumer.cpp:346
break /home/cilabsev1/file/For_WiuLiuShuai_moses/llvm/tools/clang/lib/StaticAnalyzer/Frontend/AnalysisConsumer.cpp:605
break /home/cilabsev1/file/For_WiuLiuShuai_moses/llvm/tools/clang/lib/StaticAnalyzer/Frontend/AnalysisConsumer.cpp:623
break CheckerManager.cpp:172
break /home/cilabsev1/file/For_WiuLiuShuai_moses/llvm/tools/clang/lib/StaticAnalyzer/Frontend/AnalysisConsumer.cpp:543
break /home/cilabsev1/file/For_WiuLiuShuai_moses/llvm/tools/clang/lib/StaticAnalyzer/Frontend/AnalysisConsumer.cpp:625
break /home/cilabsev1/file/For_WiuLiuShuai_moses/llvm/tools/clang/lib/StaticAnalyzer/Frontend/AnalysisConsumer.cpp:678
break /home/cilabsev1/file/For_WiuLiuShuai_moses/llvm/tools/clang/include/clang/Analysis/Analyses/LiveVariables.h:112
break *0x1ee3f99
break /home/cilabsev1/file/For_WiuLiuShuai_moses/llvm/tools/clang/lib/StaticAnalyzer/Frontend/AnalysisConsumer.cpp:657
break *0x1ee1cd2
break /home/cilabsev1/file/For_WiuLiuShuai_moses/llvm/tools/clang/lib/StaticAnalyzer/Core/CoreEngine.cpp:168
break *0x213b719
break /home/cilabsev1/file/For_WiuLiuShuai_moses/llvm/tools/clang/lib/StaticAnalyzer/Core/CoreEngine.cpp:223
break *0x213bf98
break *0x213c044
break *0x21240f3
break /home/cilabsev1/file/For_WiuLiuShuai_moses/llvm/tools/clang/include/clang/StaticAnalyzer/Core/CheckerManager.h:240
break /home/cilabsev1/file/For_WiuLiuShuai_moses/llvm/tools/clang/lib/StaticAnalyzer/Core/ExprEngine.cpp:240
break *0x214e86a
break *0x213bce3
break *0x214daaa
break *0x214e675
break *0x215079d
break *0x216b503
break *0x216b6e2
break *0x20a139e
break /home/cilabsev1/file/For_WiuLiuShuai_moses/llvm/tools/clang/lib/StaticAnalyzer/Core/ExprEngine.cpp:2115
break *0x1ee33f5
break /home/cilabsev1/file/For_WiuLiuShuai_moses/llvm/tools/clang/lib/Analysis/LiveVariables.cpp:566
break /home/cilabsev1/file/For_WiuLiuShuai_moses/llvm/tools/clang/lib/StaticAnalyzer/Core/CoreEngine.cpp:232
break /home/cilabsev1/file/For_WiuLiuShuai_moses/llvm/tools/clang/lib/StaticAnalyzer/Core/CoreEngine.cpp:204
break /home/cilabsev1/file/For_WiuLiuShuai_moses/llvm/tools/clang/lib/StaticAnalyzer/Core/ExprEngine.cpp:2022
break *0x213c10e
break *0x214dc8c
break *0x2155107
break *0x216a32a
break *0x214c5fe
break *0x21d4cb5
break *0x21d4cb5
break *0x213c2a1
break *0x2153a56
break *0x21537f4
break *0x1f7720f
break *0x1f76eba
break *0x21d25d2
break /home/cilabsev1/file/For_WiuLiuShuai_moses/llvm/tools/clang/lib/StaticAnalyzer/Core/SimpleConstraintManager.cpp:28
break *0x21d2d2f
break *0x21d2d99
break *0x21d2bdd
break *0x21ac61e
break *0x21abba8
break *0x21d210b
break *0x21d232f
break *0x21abbe2
```
后面文章会根据具体的示例源码，在调试过程中分析clang static analyzer分析引擎的内容。
