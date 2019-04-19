# clang static analyzer源码分析（番外篇）：RegionStore以及evalCall()中的conservativeEvalCall - 电影旅行敲代码 - CSDN博客
2016年10月27日 11:38:43[电影旅行敲代码](https://me.csdn.net/dashuniuniu)阅读数：1823
# 引子
我们在上一篇文章《[**clang static analyzer**源码分析（番外篇）：**evalCall()**中的**inline**机制](http://blog.csdn.net/dashuniuniu/article/details/52830592)》中提及了**clang**如何创建**CallGraph**，如何进行函数**inline**形式的过程间分析，以及函数**inline**的判断标准和具体实现。但是我们没有介绍，如果函数调用不能**inline**时**clang**是如何处理的。
该文章我们会详细介绍**clang**在这方面的工作，这中间会涉及到**clang static analyzer**的内存模型等内容。
# ConservativeEvalCall
如果发现函数调用不能进行**inline**，**clang static analyzer**会使用保守**evaluate**，对应的函数是**ConservativeEvalCall()**。该函数很简单，如下所示：
```
// Conservatively evalute call by invalidating regions and binding
// a conjured return value.
void ExprEngine::conservativeEvalCall(const CallEvent &Call, NodeBuilder &Bldr,
                                    ExplodedNode *Pred,
                                    ProgramStateRef State) {
    // 1.由于对函数行为不了解，所以只能作保守估计。比如假设该函数可能修改任何
    // 全局变量，也就是非const的global值就变成无效的了。
    State = Call.invalidateRegions(currBldrCtx->blockCount(), State);
    // 2.由于对函数行为不了解，函数返回什么样的值也无从得知，
    // 所以创建一个conjuredSymbol绑定到接收函数返回值的变量（内存）上
    State = bindReturnValue(Call, Pred->getLocationContext(), State);
    // And make the result node.
    // 3.保守evaluate完之后，创建一个新的ExplodedNode
    Bldr.generateNode(Call.getProgramPoint(), State, Pred);
}
```
就像代码中展示的，**conservativeEvalCall()**主要分为两部分，使相应的**Region**失效，并对接收函数调用返回值的内存上绑定一个**SVal**。下面我们针对这两部分，进行分别介绍。
# invalidateRegions
**invalidateRegions**是**clang static analyzer**中的一组函数，分别如下：
- **CallEvent::invalidateRegions**
- **ProgramState::invalidateRegions**
- **ProgramState::invalidateRegionsImpl**
- **RegionStoreManager::invalidateRegions**
- **RegionStoreManager::invalidateGlobalRegion**
在后面的分析中我们会对这几个函数一一介绍，这几个函数从上到下呈调用关系。在开始介绍这些函数之前，我们首先看一个例子。
```
// 全局变量
int global = 10;
struct Node
{
    Node *next;
    int value;
};
class A
{
public:
    Node n;
    int count;
    A(Node n) : n(n), count(0) {}
};
struct B
{
    int *memPtr;
};
int func(int num, int *ptr, const A &AObject, B BObject);
/*
{
    AObject.n.next->value = 11;
    return num + *ptr;
}
*/
int main()
{
    // (1)三个局部变量
    int local1 = 1;
    int local2 = 2;
    int local3 = 3;
    // (2)局部变量localN1，随后localN1.next = &localN2
    Node localN1;
    localN1.next = 0;
    localN1.value = 10;
    Node localN2;
    localN2.next = 0;
    localN2.value = 20;
    localN1.next = &localN2;
    // (3)使用localN1来初始化localA1，localA1内存布局
    // next : &localN2
    // value : 10
    // count : 0
    A localA1(localN1);
    // (4)局部变量localB，数据成员memPtr指向local3
    B localB;
    localB.memPtr = &local3;
    // (5) call event，entern函数不会被inline
    int num = func(local1, &local2, localA1, localB);
    return num + local1 + local2 + localA1.count + *localB.memPtr;
}
```
上面的示例代码，**main()**函数中会调用**func(int, int*, const A&, B)**，该函数无法获取函数体，分析引擎在**evalCall()**的时候只能选择保守估计**conservaticeEvalCall()**。该函数的参数涉及到了多种类型的参数类型，我们可以通过该函数调用来介绍到**invalidateRegions**的方方面面。该函数调用的实参内存图如下：
![这里写图片描述](https://img-blog.csdn.net/20161027105112315)
我们可以简单的来分析一下在保守估计以后，有哪些内存上的绑定有可能被被改变（也就是**invalidate**）。
- 第一个参数**local1**，传值。对**caller**中的**local1**没有任何影响。
- 第二个参数**&local2**，传值。有可能改变**local2**中的值。
- 第三个参数**localA1**，**const&**。虽然不可能改变**localA1**中的值，例如数据成员**n**以及**count**，但是有可能改变**localA1.n.next**指向的内存的值。
- 第四个参数，传值。不会改变**caller**栈帧中**localB**的值，但是有可能改变**localB.memptr**指向的内存的值。
- 该函数有可能修改全局变量**global**，所以也需要使其无效。
该函数调用会使某些内存中的值失效，失效的内存如下图红色矩形所示。**func()**函数可以通过绿色虚线以及绿色矩形来“访问”到这些失效的内存块。
![这里写图片描述](https://img-blog.csdn.net/20161027112616987)
上述失效的类型大致分为三种，第一种全局变量；第二种传值指针所指的内存；第三种传值自定义对象，其中该对象的数据成员有指向**caller**中的指针，或者数据成员也是自定义对象该对象中也有指向**caller**栈帧中的内存。其中第三种沿着内存指向的链的分析叫做**ClusterAnalysis**，有点儿类似于**pointer analysis**，关于ClusterAnalysis的分析我们后面会进行分析。第三种的分析如下图所示：
![这里写图片描述](https://img-blog.csdn.net/20161027113452624)
沿着这条内存访问链进行的分析，是**Cluster Analysis**的一部分。其中还有另外一种情况需要注意，就是函数调用时传入指针**Ptr**，那么**Ptr+1**或者**Ptr**临近的内存中的值也是有可能失效的（谁让**C**语言支持指针加减呢）。
为了介绍**conservativeEvalCall()**中的**invalidateRegions**，我们需要首先介绍一下**clang static analyzer**的内存模型，关于**clang static analyzer**内存模型的资料首推**许中兴**和**[Ted Kremenek](https://www.linkedin.com/in/tedkremenek?authType=NAME_SEARCH&authToken=lo2t&locale=en_US&srchid=3143339621477543444907&srchindex=1&srchtotal=1&trk=vsrp_people_res_name&trkInfo=VSRPsearchId:3143339621477543444907,VSRPtargetId:208085367,VSRPcmpt:primary,VSRPnm:true,authType:NAME_SEARCH)**的论文《[A Memory Model for Static Analysis of C Programs](http://lcs.ios.ac.cn/~xuzb/canalyze/memmodel.pdf) 》 ，关于内存模型[clang static analyzer源码分析（五）](http://blog.csdn.net/dashuniuniu/article/details/52461196)中只是简单的介绍了**Region**的种类，过于简单，这里力所能及的介绍下**clang static analyzer**的内存模型。
## 内存模型
传统的内存模型有两种**Name Binding Model**和**Array Simulation Model**，这两种内存模型都很简单，表达能力有限。
### Name Binding Model
关于**Name Binding Model**，其实就是一系列的**pair < name, value >**。比如当**evaluate**赋值语句的时候，直接将右手边（**right-hand-side**）的值绑定在左手边（**left-handle-side**）的变量名。这种方式缺少了**内存**这一概念，而**内存这种抽象对于C语言这种有指针类型的语言来说不可或缺**。
```
int x, y;
int *p = &x;
x = 3;
*p = 4;
y = x;
```
在上述的代码中，由于指针的存在，会有别名问题。而Name Binding Model不能处理别名问题，例如*p = 4，其实x的值也会被修改成4。
> 
The presence of pointers brings the aliasing problem. The aliasing problem is the **two or more names can present the same storage locaton**. In this example, *p and x are aliases.
The name-binding model cannot deal with the aliasing problem. **When we modify the value of a name, we should also modify the value of all names aliased to it accordingly**.
***注：抄***
### Array Simulation Model
因为**name-binding**模型没有抽象出内存概念，所以不能解决别名问题，不能解决别名问题，也就大大限制了静态分析的精确度。另外一种简单的模型就是**array-model**，这种模型将内存看作是一个长长的数组（感觉这种模型的灵感来自于物理内存呢），内存（**location**）的概念可以通过数组下标来模拟，值的存储通过修改数组对应单元的值实现。在**name-binding**模型中提出的代码通过**array-simulation**模型来表示的话，如下所示：
```
Assume mem[] is the memory simulation array.
Memory allocation:
    x: mem[1], y: mem[2], p: mem[3]
mem[3] = 1; // p = &x; mem[3] is 'p'
mem[1] = 3; // x = 3
mem[mem[3]] = 4; // dereference '*' means we have to nest mem[]'s
mem[2] = mem[1]; // y = x
```
***注：抄***
这种方式相对于**name-binding**来说可以解决别名问题，但是有一个很大的问题就是不能表示变长内存对象，也就是不能模拟堆。另外这种模型表现力不够丰富，对不同类型的region没有进行区分。
### Region Based Ternary Model(clang static analyzer)
**许**和**Ted**基于一般的语义模型，将内存模型抽象成两类映射，***Var->Loc*** 以及 ***Loc->Value***。关于第一种映射我们在[clang static analyzer源码分析（五）](http://blog.csdn.net/dashuniuniu/article/details/52461196)中有提及，管理这种映射的类是[MemRegionManager](https://code.woboq.org/llvm/clang/include/clang/StaticAnalyzer/Core/PathSensitive/MemRegion.h.html#_ZN5clang4ento16MemRegionManagerC1ERNS_10ASTContextERN4llvm20BumpPtrAllocatorImplINS4_15MallocAllocatorELm4096ELm4096EEE)，该类对象保存了多种类型的**Region**（loctions）。这些不同的内存区域如下所示：
```
class MemRegionManager {
    llvm::FoldingSet<MemRegion> Regions;
    GlobalInternalSpaceRegion *InternalGlobals;
    GlobalSystemSpaceRegion *SystemGlobals;
    GlobalImmutableSpaceRegion *ImmutableGlobals;
    // stack
    llvm::DenseMap<const StackFrameContext *, StackLocalsSpaceRegion *>
    StackLocalsSpaceRegions;
    // stack arguments...
    // static globals...
    HeapSpaceRegion *heap;
    UnknownSpaceRegion *unknown;
    CodeSpaceRegion *code;
    // ...
};
```
我们可以通过类似于**getVarRegion(const VarDecl *D, const LocationContext *LC)***来获取映射 ***Var->Loc** 中的值 ***Loc***。我们至今还没有提到过从 ***Loc->Value*** 的映射是如何组织的。***Loc->Value*** 映射是由[RegionStoreManager](https://code.woboq.org/llvm/clang/lib/StaticAnalyzer/Core/RegionStore.cpp.html#%28anonymousnamespace%29::RegionStoreManager)来管理的，关于该类我们在后面会介绍。
#### Region Hierarchy
**clang static analyzer**内存模型有第一个创新点就是内存分层，内存分层可以很好的表示**region**彼此间的关系。在**array-model**内存模型中，**region**只是简单的使用内存数组**element**表示，反映不出数组或者结构体数据成员的**region**与**super region**的关系。**clang static analyzer**内存模型使用**SuperRegion**数据成员、**SubRegion**类、**ElementRegion**类将这种关系显示的表示出来。
内存分层大致体现在两方面，一个方面是**VarRegion**内部的内存分层，例如数组类型或者结构体中子内存区域与父区域的分层。另一个方面是内存区域的分层，例如**clang static analyzer**将内存大致分为三种**stack**，**global**，**heap**。大部分的内存都属于这三种（除了函数”.text”段存储属于**CodeSpaceRegion**）。
关于第一个方面，**VarRegion**内部的内存分层。如下代码所示：
```
/*
struct Point { int x, y, z; };
*/
/*
struct Node { Node *next; Point p; int count; };
*/
int num[10] = {0};
// 注意num[0]就是num的SubRegion，对应的num是num[0]的SuperRegion
num[1] = 1;
num[2] = 2;
num[5] = 5;
num[8] = 8;
num[9] = 9;
Node n;
// n.next就是num的SubRegion，同样n.value也是num的SubRegion
// 更进一步的n.p也属于n的SubRegion
n.next = 0;
n.p.x = 1;
n.p.y = 2;
n.p.z = 3;
n.count = 10;
```
上面代码的内存层次图如下：
![这里写图片描述](https://img-blog.csdn.net/20161028113458735)
**clang static analyzer**的内存模型将其通过**cluster**将这些内存组织起来，这些**cluster**方便一些对于内存的操作，例如今天我们涉及到的**invalidateRegions**。上述的内存层次图组织起来的**cluster**如下图所示：
![这里写图片描述](https://img-blog.csdn.net/20161028160833723)
**注：我现在还没有找到dump内存cluster图的方法，[RegionBindingsRef.dump()](https://code.woboq.org/llvm/clang/lib/StaticAnalyzer/Core/RegionStore.cpp.html#_ZNK12_GLOBAL__N_117RegionBindingsRef4dumpERN4llvm11raw_ostreamEPKc)方法将binding组织为cluster打印，上面的示例代码打印出的binging结果如下。**
; **default**，如果没有**direct**值，则从**super region**获取**default**值 
***(num, 0, default) : 0 S32b***
***(num, 32, direct) : 1 S32b***
***(num, 64, direct) : 2 S32b***
***(num, 160, direct) : 5 S32b***
***(num, 256, direct) : 8 S32b***
***(num, 288, direct) : 9 S32 b***
; **32位机**
***(n, 0, direct) : 0 (Loc)***
***(n, 64, direct) : 1 S32b***
***(n, 96, direct) : 2 S32b***
***(n, 128, direct) : 3 S32b***
***(n, 160, direct) : 10 S32b***
#### Region Properties
**clang static analyzer**不仅丰富了**Region**的类型，并对**Region**进行了分层。而且内存模型还对**MemRegion**的状态进行了模拟，例如当前内存值的状态是**default**还是**direct**。后面我们会介绍到这种状态是通过***Loc->Value***这种绑定来表示的。
**RegionStore**中的大部分**bindings**是标量值（**int**或者**pointer**），这种**bindings**就是**“Direct Bindings”**。另外对于聚合类型（**aggregate type**），**element**或者**field**的默认都是使用**SuperRegion**的**“Default Bindings”**，这样的好处不用给所有的**element**一一创建**bindings**。如果**element**或者**field**这些**SubRegion**没有**binding**值，那么就会使用**SuperRegion**的**“Default Binding”**。
> 
**Without default bindings, we have to set each element of that region to 0, which is prohibitively expensive for large arrays.**
对于**direct binding**和**default binding**，《[A Memory Model for Static Analysis of C Programs](http://lcs.ios.ac.cn/~xuzb/canalyze/memmodel.pdf)》中有介绍。更为详细的介绍见**clang**文档**clang/docs/analyzer/RegionStore/RegionStore.txt**的介绍文档《[RegionStore.txt](https://chromium.googlesource.com/external/llvm.org/clang/+/google/stable/docs/analyzer/RegionStore.txt)》，这里我摘抄过来。
> 
**When there is no Direct binding for a particular region, the store manager loos at each super-region in turn to see if there is a Default binding. If so, this value is used as the value of the original region.**
**The search ends when tha base region is reached, at which point the RegionStore will pick an appropriate default value for the region**(usually a symbolic value, but sometimes zero, for static data, or “uninitialized”, for stack variables).
对于**direct binding**以及**default binding**，我们可以查看下面这个示例，该示例在[RegionStore.txt](https://chromium.googlesource.com/external/llvm.org/clang/+/google/stable/docs/analyzer/RegionStore.txt)中也可以查看到。
```
int manyInts[10];
manyInts[1] = 42; // Creates a Direct binding for manyInts[1].
print(manyInts[1]); // Retrieves the Direct binding for manyInts[1].
// 下面的描述给出了default binding在region store的作用
// There is no direct binding for manyInts[1].
// Is there a Default binding for the entire array?
// There is not, but it is a stack variable, so we use "uninitialized" 
// as the default value(and emit a diagnostic!)
print(manyInts[0]);
```
#### Region Views
由于**C/C++**允许对指针进行类型转换，也就是对同一块**Region**有可能有多种不同的解释，如果想要完整模拟**C/C++**程序的内存情况，就必须要对这种情况进行处理。考虑下面一段代码：
```cpp
// malloc一块内存
void *p = malloc(10);
// 这块内存首地址赋值给char型指针，char占用一个字节（32位）
char *buf1 = (char *)p;
buf1[0] = 'a';
// 该内存首地址赋值给int型指针，int占用4个字节（32位）
int *buf2 = (int *)p;
buf2[0] = 0;
// 对于这块内存解释char类型，并获取char数组首元素的值
char c = buf1[0]
```
***注：抄***
上面的代码中，***p*** 指向的内存分别赋值给**int***和**char***，也就是会解释成**int**和**char**。**clang static analyzer**的解决方法分别在这块内存上附加上**int**的**“view”**和**char**的**“view”**。**“view”**可以理解成对一块内存的解释视角。当我们使用 ***buf1*** 来访问这块内存的时候，就采用**char**的**“view”**，当我们使用 ***buf2*** 来访问这块内存的时候，就采用**int**的**“view”**。
**clang static analyzer**具体是使用**ElementRegion**来对**“view”**进行模拟。**clang**对**region cast**其进行处理的函数是[StoreManager::castRegion()](https://code.woboq.org/llvm/clang/lib/StaticAnalyzer/Core/Store.cpp.html#_ZN5clang4ento12StoreManager10castRegionEPKNS0_9MemRegionENS_8QualTypeE)，该函数是[SValBuilder::evalCast()](https://code.woboq.org/llvm/clang/lib/StaticAnalyzer/Core/SValBuilder.cpp.html#516)来调用的。我们可以通过该函数简单介绍一下，**clang**是如何处理**region cast**的。
```
const MemRegion *StoreManager::castRegion(const MemRegion *R, QualType CastToTy) {
    // ...
    // Now assume we are casting from pointer. Other cases should
    // already be handled.
    QualType PointeeTy = CastToTy->getPointeeType();
    QualType CanonPointeeTy = Ctx.getCanonicalType(PointeeTy);
    // (1) 如果是cast to void*，那么直接返回原有region
    // Handle casts to void*. We just pass the region through.
    if (CanonPointeeTy.getLocalUnqualifiedType() == Ctx.VoidTy)
        return R;
    // (2)处理相容类型，其实也就是Original Type和Cast To Type是
    // “标准化”后是一样的，则直接返回
    // Handle cast from compatible types.
    if (R->isBoundable())
        if (const TypedValueRegion *TR = dyn_cast<TypedValueRegion>(R)) {
            QualType ObjTy = Ctx.getCanonicalType(TR->getValueType);
            if (CanonPointeeTy == ObjTy)
                return R;
        }
}
    // (3) 处理特定类型的region cast
    switch(R->getKind()) {
        // (3) 1. 一些不可能在源代码中出现的region cast类型
        case MemRegion::CXXThisRegionKind:
        case MemRegion::CodeSpaceRegionKind:
        case MemRegion::StackLocalsSpaceRegionKind:
        case MemRegion::StackArgumentsSpaceRegionKind:
        case MemRegion::HeapSpaceRegionKind:
        case MemRegion::UnknownSpaceRegionKind:
        case MemRegion::StaticGlobalSpaceRegionKind:
        case MemRegion::GlobalInternalSpaceRegionKind:
        case MemRegion::GlobalSystemSpaceRegionKind:
        case MemRegion::GlobalImmutableSpaceRegionKind: {
            llvm_unreachable("Invalid region cast");
        }
        // (3) 2.普通region cast
        case MemRegion::FunctionCodeRegionKind:
        case MemRegion::BlockCodeRegionKind:
        case MemRegion::BlockDataRegionKind:
        case MemRegion::StringRegionKind:
        case MemRegion::SymbolicRegionKind:
        case MemRegion::AllocaRegionKind:
        case MemRegion::CompoundLiteralRegionKind:
        case MemRegion::FieldRegionKind:
        case MemRegion::VarRegionKind:
        case MemRegion::CXXTempObjectRegionKind:
        case MemRegion::CXXBaseObjectRegionKind:
            return MakeElementRegion(R, PointeeTy);
        // (3) 3.原有region是ElementRegionKind可以特殊处理
        case MemRegion::ElementRegionKind: {
            // 特殊处理
        }
    }
```
**castRegion()**处理**region cast**的逻辑大致分为三个部分。
- 处理向**void***的转换，这个转换没有额外的处理，直接返回原有**region**
- 处理相容类型的**region cast**，也是直接返回原有**region**
- 处理常见的**region cast**，调用**MakeElementRegion()**
关于**MakeElementRegion()**函数的原型如下，
```
// 处理region cast的逻辑调用MakeElementRegion()的时候，idx使用的是默认值
const MemRegion *MakeElementRegion(const MemRegion *baseRegion, 
                            QualType pointeeTy, uint64_t index = 0);
```
我们还是使用上面的 ***buf1*** 与 ***buf2*** 代码进行举例，分别经过两次**region cast**后，**region**示意图如下：
![这里写图片描述](https://img-blog.csdn.net/20161029161103515)
原有的**region**有两个**ElementRegion**，并且两个**ElementRegion**的下标都是**0**，但是类型却不同，一个是char，一个是**int**。***buf1*** 指向的是类型为**char**的**ElementRegion**，***buf2*** 指向的是类型为**int**的**ElementRegion**。我们使用下面的命令将**ExplodedGraph**打印出来。
```cpp
// 示例代码：castStore.cpp
// scan-build --use-c++=/usr/local/bin/clang++ --use-analyzer=/usr/local/bin/clang++ -enable-checker debug.ViewExplodedGraph clang++ castStore.cpp -c
#include <stdlib.h>
int main()
{
    void *p = malloc(10);
    char *buf1 = (char*)p;
    buf1[0] = 'a';
    int *buf2 = (int *)p;
    buf2[0] = 0;
    // 由于clang static analyzer在进行静态分析之前，在源码的基础上进行了一次活跃变量分析，
    // 详情参见clang中的活跃性分析（http://blog.csdn.net/dashuniuniu/article/details/50723930）
    // 所以buf2在后面的代码中不是活跃的，所以在removeDead()的时候会进行删除
    char c = buf1[0];
    return c + 1;
}
```
我们挑选出**ExplodedGraph**中某些阶段的子图来说明**region cast**的结果，第一阶段在***“char* buf1 = (char*)p;”**之后，此时通过 ***malloc()** 创建了一块内存，并使用**StackLocal**指向这块内存首地址。此时的**ExplodedGraph**子图如下：
![这里写图片描述](https://img-blog.csdn.net/20161029162540633)
此时**Store**中有四个**Binding**，第一个是**GlobalSystemSpaceRegion**，默认**default binding**；第二个是***“void * p = malloc(10);”**中调用**malloc**得到的内存块；第三个是局部 **void ** 类型的变量 ***p*** 的**binding**，指向的是**malloc**分配得到的**SymRegion**；第四个是***“char* buf1 = (char * )p;”**中，局部**char **类型变量 ***buf1*** 的**binding**，指向的是**region cast**后创建的**ElementRegion**，其中**index**为**0**。
第二阶段在***“int* buf2 = (int *)p;”***后，此时p指向的内存通过**region cast**转换成了**int**类型的内存。**ExplodedGraph**子图如下：
![这里写图片描述](https://img-blog.csdn.net/20161029163642883)
上面的子图显示**Store**中新增了两个**binding**，并删除了一个**binding**。新增的两个**binding**分别是***“buf1[0] = ‘a’;”***后，原有**region**存储字符**‘a’**的**binding**以及***“int* buf2 = (int *)p;”***中局部**int***变量 ***buf2*** 的**binding**，指向的**region cast**后创建的**ElementRegion**。由于在***“int* buf2 = (int *)p;”***后，变量 ***p*** 就已经没有用了，所以使用**PreStmtPurgeDeadSymbols**删除了变量 ***p*** 的**binding**。
这里有两点需要特别注意，第一点就是虽然***“buf1[0] = ‘a’;”***对内存进行了赋值，但毕竟是一部分，另外还有一部分没有赋值，所以保留了**SymRegion**的**default binding**，以防在访问 ***bu1[1]*** 时可以使用**SymRegion**的**default binding**。第二点需要注意的就是下面的**Ranges of symbol values**中展示的，对 ***conj_$1{void*}** 进行了限定，是非**0**的正值，因为在**”buf1[0] = ‘a’;”**这条语句之前，已经进行了路径的分裂，因为**malloc**有可能调用失败返回**0**值（**C++ new失败会抛出std::bad_alloc异常**），或者调用成功返回内存地址，如果返回**0**值，则 ***buf1[0]** 就是空指针解引用（**NullDeference**）。我们这里的子图是调用成功那条路径的状态。
第三个阶段是***“buf2[0] = 0;”***后，此时由于变量 ***buf2*** 是死变量，所以删除了 ***buf2*** 的**binding**。此时的此图如下所示：
![这里写图片描述](https://img-blog.csdn.net/20161029165413532)
上面的子图是在**evaluate**下标表达式 ***buf[0]*** 之后的状态，此时状态和第二阶段相同，我们不多做解释。在**evalue**下标表达式之后，并执行**PreStmtPurgeDeadSymbols**后的状态如下图所示：
![这里写图片描述](https://img-blog.csdn.net/20161029165603314)
从图中可以看到，此时变量 ***buf2*** 的**binding**已经被删除了。
在介绍完**region cast**的实现机制后，我们介绍下如果从 ***buf1*** 指向的**ElementRegion**如何获取原始的**MemRegion**。执行这项操作的函数是[MemRegion::StripCasts()](https://code.woboq.org/llvm/clang/lib/StaticAnalyzer/Core/MemRegion.cpp.html#_ZNK5clang4ento9MemRegion10StripCastsEb)，该函数的主体逻辑如下。
```
// StripCasts()的实现机制就是一个while循环，该函数不停的获取其super region
// 并判断该super region是否是真正原始的region
const MemRegion *MemRegion::StripCasts(bool StripBaseCasts) const {
    const MemRegion *R = this;
    // 因为region cast不一定只有一层，例如上的buf1可以再转换成为shor*。
    // 所以需要一个循环来获取原始region
    while(true) {
        switch(R->getKind()) {
            case ElementRegion *ER = cast<ElementRegion>(R);
            // 判断当前是否是cast后的region的标准有两条：
            // (1)是否是ElementRegionKind
            // (2)index是否是0
            if (!ER->getIndex().isZeroConstant())
                return R;
            R = ER->getSuperRegion();
            break;
            // case ...
            default:
                return R;
        }
    }
}
```
最后关于**clang static analyzer**分析引擎是如何创建以及对这些**Region**赋值的，可以参见《**A Memory Model for Static Analysis of C Programs**》
## RegionStoreManager
介绍完**clang static analyzer**的内存模型，下面就从源码的角度说明一下该内存模型的实现。关于**clang static analyzer**中内存（**Region**）的分类在[clang static analyzer源码分析（五）](http://blog.csdn.net/dashuniuniu/article/details/52461196)中进行了简单的介绍，也就是***Var->Loc***的部分。这里我们着重描述***Loc->Value***在**clang static analyzer**中是如何组织并且又是如何进行增删查改的。
**clang static analyzer**中对**<Loc, Value> pair**进行管理的类是[RegionStoreManager](https://code.woboq.org/llvm/clang/lib/StaticAnalyzer/Core/RegionStore.cpp.html)，该类继承自[StoreManager](https://code.woboq.org/llvm/clang/include/clang/StaticAnalyzer/Core/PathSensitive/Store.h.html#clang::ento::StoreManager)。**StoreManager**是一个抽象类，该类提供了一系列的对**<Loc, Value> pair**进行操作的接口，下面是该抽象类的接口。
```
class StoreManager {
protected:
    SValBuilder &svalBuilder;
    ProgramStateManager &StateMgr;
    // MRMgr - Manages region objects associated with this StoreManager.
    MemRegionManager &MRMgr;
    ASTContext &Ctx;
    StoreManager(ProgramStateManager &stateMgr);
public:
    virtual ~StoreManager() {}
    /// 下面的注释有误，clang中有很多注释是旧版本注释。
    /// Return the value bound to specified location in a given state.
    /// \param[in] store The analysis state; 注释错误
    /// \param[in] loc The symbolic memory location.
    /// \param[in] T An optional type that provides a hint indicating the
    /// expected type of the return value. This is used if the value is
    /// lazily computed.
    /// \return The value bound to the location \c loc.
    virtual SVal getBinding(Store store, Loc, QualType T = QualType()) = 0;
    /// Return a state with the specified value bound to the given location. 注释错误
    /// \param[in] store The analysis state. 注释错误
    /// \param[in] loc The symbolic memory location.
    /// \param[in] val The value to bind to location \c loc.
    /// \return A pointer to a ProgramState object that contains the same
    /// bindings as \c state with the addition of having the value specified
    /// by \c val bound to the location given for \c loc. 注释错误
    virtual StoreRef Bind(Store store, Loc loc, SVal val) = 0;
    virtual StoreRef BindDefault(Store store, const MemRegion *R, SVal V);
    /// \brief Create a new store with the specified binding removed.
    /// \param ST the original store, the is the basis for the new store.
    /// \param L the location whose binding should be removed.
    virtual StoreRef killBinding(Store ST, Loc L) = 0;
    /// getInitalStore - Returns the initial "empty" store representing the
    /// value binding upon entry to an analyzed function.
    virtual StoreRef getInitialStore(const LocationContext *InitLoc) = 0;
    virtual Loc getLValueVar(const VarDecl *VD, const LocationContext *LC) {
        return svalBuilder.makeLoc(MRMgr.getVarRegion(VD, LC));
    }
    Loc getLValueCompoundLiteral(const CompoundLiteralExpr *CL,
                                const LocationContext *LC) {
        return loc::MemRegionVal(MRMgr.getCompoundLiteralRegion(CL, LC));
    }
    virtual SVal getLValueField(const FieldDecl *D, SVal Base) {
        return getLValueFieldOrIVar(D, Base);
    }
    virtual SVal getLValueElement(QualType elementType, NonLoc offset, SVal Base);
    /// ArrayToPointer - Used by ExprEngine::VisitCast to handle implicit
    /// conversions between arrays and pointers.
    virtual SVal ArrayToPointer(Loc Array, QUalType ElementTy) = 0;
    /// Evalues a chain of derived-to-base casts through the path specified in
    /// \p Cast.
    SVal evalDerivedToBase(SVal Derived, const CastExpr *Cast);
    /// evalDerivedToBase() ...
    /// \brief Attempts to be a down cast. Used to model BaseToDerived and C++
    /// dynamic_cast.
    /// The callback may result in the following 3 scenarious:
    /// - Successful cast(ex: derived is subclass of base).
    /// - Failed cast(ex: derived is definitely not a subclass of base).
    ///   The distinction of this case from the next one is necessary to model
    ///   dynamic_cast.
    /// - We don't know (base is a symbolic region and we don't have
    ///   enough info to determine if the cast will succeed at run time).
    /// The function returns an SVal representing the derived class; it's 
    /// valid only if Failed flag is set to false.
    SVal attemptDownCast(SVal Base, QualType DerivedPtrType, bool &Failed);
    const ElementRegion *GetElementZeroRegion(const MemRegion *R, QualType T);
    /// castRegion - 我们在介绍region cast的时候介绍过该函数
    const MemRegion *castRegion(const MemRegion *region, QualType CastToTy);
    virtual StoreRef removeDeadBindings(const MemRegion *region, QualType CastToTy);
    virtual bool includedInBindings(Store store,
                                const MemRegion *region) const = 0;
    /// If the StoreManager supports it, increment the reference count of
    /// the specified Store object.
    virtual void incrementReferenceCount(Store store) {}
    /// If the StoreManager supports it, determent the reference count of
    /// the specified Store object. If the reference count hits 0, the memory
    /// associated with the object is recycled.
    virtual void decrementReferenceCount(Store store) {}
    /// 在分析RegionStoreManager中对应的函数时再详细分析
    virtual StoreRef invalidateRegions(Store store,
                                    ArrayRef<SVal> Values,
                                    const Expr *E, unsigned Count,
                                    const LocationContext *LCtx),
                                    const CallEvent *Call,
                                    InvalidatedSymbols &IS,
                                    InvalidatedRegions *InvalidatedTopLevel,
                                    InvalidatedRegions *Invalidated) = 0;
    /// 该函数在分析inlineCall()进行过介绍
    /// enterStackFrame - Let the StoreManager to do something when execution
    /// engine is about to execute into a callee.
    StoreRef enterStackFrame(Store store,
                            const CallEvent &Call,
                            const StackFrameContext *CalleeCtx);
    /// 不知道为什么要使用"transitive closure"这个术语？
    /// 关于RemoveDead相关的函数源码分析，会在以后进行介绍。
    /// Finds the transitive closure of symbols with the given region.
    /// Returns false if the visitor aborted the scan.
    virtual bool scanReachableSymbols(Store S, const MemRegion *R,
                                    ScanReachableSymbols &Visitor) = 0;
    /// iterBindings - Iterate over the bindings in the Store.
    virtual void iterBindings(Store store, BindingsHandler& f) = 0;
    const MemRegion *MakeElementRegion(const MemRegion *baseRegion,
                                    QualType pointeeTy, uint64_t index = 0);
    /// CastRetrivedVal - Used by subclasses of StoreManager to implement
    /// implicit casts that arise from loads from regions that are reinterpreted
    /// as another region.
    SVal CastRetrivedVal(SVal val, const TypedValueRegion *region,
                    QualType castTy, bool performTestOnly = true);
    SVal getLValueFieldOrIvar(const Decl *decl, SVal base);
};
```
上面我们给出了抽象类**StoreManager**的主要接口，这些接口是一个内存模型必须具备并且有良好实现的，**clang static analyzer**内存模型是基于**Region**实现的，对应的类名是**RegionStoreManager**。下面我们给出一个表格来**StoreManager**对应的接口。
|接口名|说明|
|----|----|
|getBinding(Store store, Loc loc, QualType T)|获取内存位置loc上对应的符号值SVal|
|Bind(Store store, Loc loc, SVal val)|将符号值SVal绑定到内存位置loc上|
|BindDefault(Store store, const MemRegion *R, SVal V)|为给定内存块设置默认值SVal|
|killBinding(Store ST, Loc L)|删除给定内存位置对应的Binding|
|getInitialStore(const LocationContext *InitLoc)|返回一个初始化的空的存储（store）|
|getLValueVar(const VarDecl *VD, const LocationContext *LC)|获取指定变量在指定LocationContext下对应的内存地址SVal(Loc Kind)|
|getLValueCompoundLiteral(const CompoundLiteralExpr*, const LocationContext*)|返回指定CompoundLiteralExpr在指定LocationContext下CompoundLiteralRegion的内存地址|
|getLValueField(const FieldDecl* D, SVal Base)|返回指定FieldDecl在对应BaseRegion下FieldRegion的内存地址|
|getLValueElement(QualType, NonLoc offset, SVal Base)|返回指定内存地址中，相应index对应的内存地址SVal(Loc Kind)|
|ArrayToPointer(Loc Array, QualType)|将数组首地址转换为指针并返回，被ExprEngine::VisitCast调用|
|evalDerivedToBase(SVal Derived, const CastExpr*)|进行Derived对象到Base对象的转换，返回Base地址。在以后的文章中再详细介绍该方法|
|attemptDownCast(SVal Base, QualType)|尝试模拟dynamic_cast，该接口需要模拟运行时的黑科技，在以后得文章中再介绍|
|GetElementZeroRegion(const MemRegion *R, QualType)|返回指定数组第0个位置元素对应的ElementRegion|
|castRegion(const MemRegion *region, QualType)|进行Region的类型转换，RegionStore使用的是ElementRegion进行”view”的模拟|
|removeDeadBindings(Store store, const StackFrameContext *LCtx, SymbolReaper&)|对store中的Dead Binding进行清理|
|includeInBindings(Store store, const MemRegion *region)|查询store中是否有关于region的binding存在|
|incrementReferenceCount(Store store)|增加store内存的引用计数|
|decrementReferenceCount(Store store)|递减store内存的引用计数|
|invalidateRegions(…)|遍历Values按照某些规则使某些regions失效，通过invalidateRegionsWorker实现|
|enterStackFram(Store store, const CallEvent &Call, const StackFrameContext *CalleeCtx)|在进入一个新的Callee Stack的时候，store需要的做的操作，例如获取caller实参值并传递到callee stack对应的stack argument region中|
|scanReachableSymbols(Store S, const MemRegion *R, ScanReachableSymbols &Visitor)|遍历store中的可达Symbol，该函数被ProgramStateManager::removeDeadBindings调用，使用了垃圾回收中的mark-sweep机制实现。在后面的文章中进行分析|
|iterBindings(Store store, BindingsHandler&)|遍历所有Loc->Value的binding|
|MakeElementRegion(const MemRegion *baseRegion, QualType pointeeTY, uint64_t index = 0)|创建ElementRegion，index默认值是0|
上面我们洋洋洒洒，将**RegionManager**需要实现的接口罗列出来。下面我就要言归正传，分析一下**invalidateRegions**所涉及到的一些概念、数据成员以及算法。
### BindingsKey
**BindingsKey**是分析引擎内存模型中存储***Loc->Value***的”键值对“的键，**BindsKey**类的定义如下：
```cpp
class BindingKey {
public:
    enum Kind { Default = 0x0, Direct = 0x1 };
    // P主要表示MemRegion和Bingding类型，其中Binding使用2位的int表示
    llvm::PointerIntPair<const MemRegion *, 2> P;
    // 表示offset
    uint64_t Data;
    /// Create a key for a binding to region \p r, which has a symbolic offset 
    /// from region \p Base.
    explicit BindingKey(const SubRegion *r, const SubRegion *Base, Kind k)
        : P(r, k | Symbolic), Data(reinterpret_cast<uintptr_t>(Base)) {}
    /// Create a key a binging at \p offset from base region \p r.
    explicit BindingKey(const MemRegion *r, uint64_t offset, Kind k)
        : P(r, K), Data(offset) {}
};
```
**BindingKey**虽然很简单，但是应用却很广泛，任何对于***Loc->Value***的操作都要基于**BindingKey**实现。
### RegionBindingsRef
**RegionBindingsRef**是整个内存模型的核心，该类对象以某种方式存储着所有的***Loc->Value***键值对。该类是基于[llvm::ImmutableMapRef](https://code.woboq.org/llvm/llvm/include/llvm/ADT/ImmutableMap.h.html#llvm::ImmutableMapRef)实现的，而**llvm::ImmutableMapRef**的又是基于[llvm::ImutAVLTree](https://code.woboq.org/llvm/llvm/include/llvm/ADT/ImmutableSet.h.html#llvm::ImutAVLTree)（也就是**AVL Tree**）实现的，这两个库的实现都很复杂，这里就不多做解释了（其实是我不想深入去分析，很复杂）。
在介绍**RegionBindingsRef**的定义之前，首先介绍四个**typedef**类型，如下：
```
typedef llvm::ImmutableMap<BindingKey, SVal> ClusterBindings;
typedef llvm::ImmutableMapRef<BindingKey, SVal> ClusterBindingsRef;
typedef std::pair<BindingKey, SVal> BindingPair;
typedef llvm::ImmutableMap<const MemRegion *, ClusterBindings> RegionBindings;
```
第一个**ClusterBindings**其实就是一个**MemRegion**对应的**ClusterBinding**，下面给出一个关于**ClusterBinding**示意图。
![这里写图片描述](https://img-blog.csdn.net/20161030163619498)
上面示例代码中的局部变量***n***，是用户自定义对象，它有很多数据成员，这些数据成员构成了一个**Cluster**。这些**Cluster**是通过**llvm::ImmutableMap**也就是一个**AVL Tree**实现的，我们通过**BindingKey**可以快速查找到指定**Loc**对应的**SVal**。
第二个**ClusterBindingsRef**不用多做介绍，就是一个指向**ClusterBinding**的指针。
第三个**BindingPair**也不用多说，它是**AVL Tree**的结点。
第四个**RegionBindings**是所有**MemRegion**对应的**ClusterBinding**，这些**ClusterBinding**也是通过**llvm::ImmutableMap**实现的，内部同样是**AVL Tree**，而这些**AVL Tree**的结点是另一颗**AVL Tree**（这颗**AVL Tree**是用来组织**MemRegion**内部的**bindings**）。
```
struct Point {
    int x, y, z;
};
struct Node {
    Node *next;
    Point p;
    int count;
};
int main() {
    int array[10] = {0};
    array[1] = 1;
    array[2] = 2;
    array[5] = 5;
    array[8] = 9;
    array[9] = 9;
    Node n;
    n.next = 1;
    n.p.x = 1;
    n.p.y = 2;
    n.p.z = 3;
    n.count = 10;
    Point p;
    p.x = 11;
    p.y = 12;
    p.z = 13;
    return array[4] + n.p.x + p.x;
}
```
我们使用**debug.ViewExplodedGraph**打印出**ExplodedGraph**，在***“return array[4] + n.p.x + p.x;”***这条语句之后的**ExplodedGraph**子图如下所示：
![这里写图片描述](https://img-blog.csdn.net/20161030175017862)
对应的**ClusterBinding**的**AVL Tree**图示如下，三个**MemRegion**分别对应一个**ClusterBinding**。各个**MemRegion**对应的**ClusterBinding**对应着**RegionBindings**中的结点。
![这里写图片描述](https://img-blog.csdn.net/20161030181050984)
***注：上面的这些AVL Tree的结点摆放的位置是我随手画的，实际上会有一类Profile函数会将结点Key值转换成ID，来决定实际插入的位置。***
介绍完**4**个**typdef**，下面给出**RegionBindingsRef**的定义。
```
class RegionBindingsRef : public llvm::ImmutableMapRef<const MemRegion*,
                                ClusterBindings> {
    ClusterBindings::Factory *CBFactory;
public:
    RegionBindingsRef(ClusterBindings::Factory &CBFactory,
                    const RegionBindings::TreeTy *T,
                    RegionBindings::TreeTy::Factory *F)
        : llvm::ImmutableMapRef<const MemRegion*, ClusterBindings>(T, F),
        CBFactory(&CBFactory) {}
    // 添加根据BindingKey以及SVal，添加Binding。
    // 根据前面我们画出的RegionBindings示意图，也能猜出添加新binding的流程。
    RegionBindingsRef addBinding(BindingKey K, SVal V) const {
        // (1)获取BindingKey对应的BaseRegion，用于获取MemRegion对应的ClusterBindings。
        const MemRegion *Base = K.getBaseRegion();
        const ClusterBindings *ExistingCluster = lookup(Base);
        // (2) 如果MemRegion对应的ClusterBinding存在，则直接在上面进行操作。
        // 如果不存在则创建一个新的空AVL Tree（用来存储该MemRegion对应的ClusterBinding）
        ClusterBindings Cluster = 
            (ExistingCluster ? *ExistingCluster : CBFactory->getEmpty());
        // (3) 前面查找Cluster或创建Cluster工作完成，则向该Cluster上添加新的Binding。
        ClusterBindings NewCluster = CBFactory->add(Cluster, K, V);
        return add(Base, NewCluster);
    }
    RegionBindingsRef addBinding(const MemRegion *R,
                                BindingKey::Kind k, SVal V) const;
    const SVal *lookup(BindingKey) const;
    const SVal *lookup(const MemRegion *R, BindingKey::Kind k) const;
    // 根据addBinding()的函数逻辑，也能很容易得到removeBinding的工作流程
    RegionBindingsRef removeBinding(BindingKey K);
    RegionBindingsRef removeBinding(const MemRegion *R,
                                    BindingKey::Kind k);
    RegionBindingsRef removeBinding(const MemRegion *R) {
        return removeBinding(R, BindingKey::Direct).removeBinding(R, BindingKey::Default);
    }
    Optional<SVal> getDirectBinding(const MemRegion *R) const;
    /// getDefaultBinding - Returns an SVal* representing an optional default
    /// binding associated with a region and its subregions.
    Optional<SVal> getDefaultBinding(const MemRegion *R) const;
};
```
这些接口都很简单，并且都是组织**Binding**的**AVL Tree**应该具有的接口。但是**RegionBindingsRef**中的函数**asStore()**需要注意。
```
typedef const void *Store;
/// Return the internal tree as a Store
Store asStore() const {
    return asImmutableMap().getRootWithoutRetain();
}
```
我们在前面的**StoreManager**中的成员方法中经常看到**Store**这个数据类型以及***store***这个变量。从名字上看，***store***应该就是内存模型中表示**“存储”**的一个对象。***store***确实是存储所有***Loc->Value Binding***的对象，***store***有两种形式，一种是指针（**const void***），一种是**RegionBindingsRef**对应**AVL Tree**的**Root**。
大家应该记得在以前的文章中介绍**ProgramState**的时候，说过**ProgramState**有一个数据成员表示现在程序中所有的存储，这个数据成员就是**Store store**。在分析引擎刚启动的时候，初始**ProgramState**中的数据成员***store***就是一个空指针，随着程序的执行，创建**Region**，创建***Var->Loc***以及***Loc->Value***这两种**Binding**，数据成员***store***所指向的**AVL Tree**才渐渐丰富起来了。
### [RegionStoreManager](https://code.woboq.org/llvm/clang/lib/StaticAnalyzer/Core/RegionStore.cpp.html#316)
虽然介绍完了**RegionBindingRef**，但是还没有介绍如何管理**RegionBindingRef**，**clang**中管理**RegionBindingRef**的类是**RegionStoreManager**。前面我们提到过一个表示内存模型的抽象类**StoreManager**，**RegionStoreManager**就继承自这个抽象类。既然是继承抽象类，抽象类中的接口自然应该实现，**RegionStoreManager**基于**Region**以及**RegionBindingsRef**实现了**clang static analyzer**内存模型的管理。关于这些接口的具体实现，这里就偷懒不再一一分析了，在以后源码分析用到的时候再回过头来对这些接口的实现进行分析。
这里着重强调其中一个成员方法**getRegionBindings()**，该函数的实现如下：
```cpp
// 该函数根据传入的参数store返回RegionBindingsRef
// 这个函数印证了我们前面的说法，store有两种表现形式，这个函数就是
// 将一种const void *形式的store，转换成为RegionBindingsRef形式的AVL Tree。
RegionBindingsRef getRegionBindings(Store store) const {
    return RegionBindingsRef(CBFactory,
                            static_cast<const RegionBindings::TreeTy*>(store),
                            RBFactory.getTreeFactory());
}
```
### Binding Invalidation
介绍完**RegionBindingsRef**以后，再介绍一个概念就是**“Binding Invalidation”**，这个概念就是使某些**binding**失效。**RegionStoreManager**中就有使用**binding**无效的函数，例如**“killBinding(Store ST, Loc L) override”**。
**clang static analyzer**支持**concrete offsets**和**symbolic offsets**，如下这个例子：
```
int main()
{
    // ...
    foo[0] = 0;
    foo[1] = 1;
    // 其中index i的值是符号值
    foo[i] = i;
    return 0;
}
```
***注：抄于[此](https://chromium.googlesource.com/external/llvm.org/clang/+/google/stable/docs/analyzer/RegionStore.txt)***
在第三条赋值语句赋值之后，我们不能保证***‘foo[0]’***和***‘foo[1]’***中的值保持不变，因为第三条语句的***index i***是符号值，所以***‘foo[i]’***有可能会覆盖***‘foo[0]’***和***‘foo[1]’***。
> 
**binding to a region with a symbolic offset invalidates the entire concrete offset region.**
**We know ‘foo[i]’ is some where within ‘foo’, so we don’t have to invalidate anything else, but we do have to conservative about all other bindings within ‘foo’.**
**注：上面的第二句话我有一些疑惑，虽然foo[i]在分析引擎分析时只能处于foo内存区域里，但是在C语言执行的时候，foo作为局部变量，考虑数组越界的问题，是否应该将当前StackFrameContext下的所有StackArgumentsRegion和StackLocalsRegion失效。但是如果这样考虑的话，当前path上的所有region都会失效，这样是不是代价太大了？欢迎对这个交流。**
**———————-update—————————**
关于上面这个问题的更新。最近无意中看到了**Clang-Dev**中的一个问题《[**Random SVal hierarchy questions**](http://clang-developers.42468.n3.nabble.com/analyzer-Random-SVal-hierarchy-questions-td4048398.html)》，该问题指出了符号值指针（**pointer symbol**也就是指针的值未知）可能会指向任何内存位置（**clang**中使用**SymbolicRegion**表示未知内存，见《[**A Memory Model for Static Analysis of C 
Programs**](http://lcs.ios.ac.cn/~xuzb/canalyze/memmodel.pdf)》中**4.1**节最后一段），从**stack**、**heap**到**global space**，但是为什么要将**stack**上的**symbolic region**和**heap**上的**symbolic region**区别开来呢，两者本身并没有什么区别？回答给出了这一个回答，**clang static analyzer**采用了一种***dis-aliasing*** 的优化方法，故意缩小**pointer symbol**可能指向的范围，否则一个**pointer symbol**（例如：****ptr = 9;***，ptr指向未知内存，那么就需要使所有data Region失效）就能让整个分析引擎跑飞，因为什么都变得不精确了。
我把原话粘贴过来： 
Regions in the unknown space can alias with any space, not just the heap space. In general, ***the analyzer makes fairly optimistic assumptions about dis-aliasing***. Consider the following code: 
```
int *bar(int *p); 
void foo(int param) { 
  int *ptr = bar(¶m); 
  *ptr = 8; 
  param = 9; 
  // Store Bindings at this program point: 
  //   (param,0,direct) : 9 S32b 
  //  (ptr,0,direct) : &SymRegion{conj_$3{int *}} 
  //  (SymRegion{conj_$3{int *}},0,direct) : 8 S32b 
  clang_analyzer_eval(*ptr == 9); // Yields FALSE 
}
```
Here, the analyzer has SymRegion{conj_$3{int *}} is in the unknown memory space but it is entirely possible that bar() returns its parameter. In this case, updating *ptr would change the value stored for param. **The analyzer (unsoundly) assumes this can’t happen.** (1) **It could be more conservative and case split on all possible aliasing relationships, but this would be very expensive!** (2) **Alternatively, it could invalidate all other storage on writes to the unknown space — but this would be quite imprecise, invalidating storage for locations that probably aren’t aliased.** The assumption that symbolic regions in the unknown space don’t alias parameters isn’t sound, but it does cover the common case. 
上面这句话表明，如果按照保守策略，考虑所有别名可能，不仅会给**clang static analyzer**带来很大开销，而且分析过程会变得极度不精确。所以没有必要，使得所有内存失效。就像我们原**blog**中的例子，使本**cluster**失效即可，没有必要将整个程序的**RegionStore**失效。
后面我会单开一篇文章分析这个问题。 
**———————-update—————————**
上面我们考虑了**symbolic offset**的写会使所有同一**BaseRegion**下的其他**concrete offset**失效。下面我们考虑一下相反的情况。
```
int main()
{
    // ...
    // i是符号值
    foo[i] = i;
    foo[0] = 0;
    return 0;
}
```
***注：抄于[此](https://chromium.googlesource.com/external/llvm.org/clang/+/google/stable/docs/analyzer/RegionStore.txt)***
同样，在第二条语句执行完成之后，***‘foo[i]’***有可能会失效。
> 
**Binding to a region R with a concrete offset invalidates any symbolic offset binding whose concrete offset is a super-region *or* sub region of R.**
  ？？？
当然可以通过***i***上的约束来得到更精确的解，或者通过**region**的类型来得到更精确的解。例如修改***‘foo[0].a’***是不可能影响到***‘foo[i].b’***的。
注意关于**invalidate binding**是只会考虑当前**Loc**所处的**region**或者**Loc**所处的**region**的**super-region**，并不会考虑**Loc**中存储的值。另外只要**Loc**是精确的，我们只要使当前**Loc-Value**失效即可，没有必要使整个**Loc**所处**region**的**super-region**失效，也就是没有必要使整个**Cluster**失效。
### ClusterAnalysis
#### Region Invalidation
介绍完**Binding Invalidation**，下面介绍下**Region Invalidation**。不像**Binding Invalidation**，**region invalidation**需要使整个**Cluster**失效。例如我们将一个**Loc**作为实参传递到一个我们无法模拟的函数调用时（**ConservativeCall**），由于我们对该函数的行为一无所知，所以需要对**Loc**所处的**Cluster**作保守估计。例如下面代码：
```
int main()
{
    int num[10] = {10};
    // func()函数体有可能执行基于&num[3]的指针运算
    // 所以需要使整个num Cluster失效
    func(&num[3]);
    return 0;
}
```
> 
Region invalidation typically does even more than this, however. Because it usually represents the **complete escape of a region** from the analyzer’s model, its *contents* must be transitively invalidated. (**For example, if a region ‘p’ of type ‘int**’ is invalidated, the contents of ‘*p’ and ‘**p’ may have changed well.**) 
上面的引用表示，除了指针算术运算，**region invalidation**还要考虑其他的东西，例如***Loc->Value***中的**Value**。其实如果熟悉垃圾回收的话，这个很好理解，垃圾回收中一个根集（**root set**）和**可达对象**的概念，只要对象可以访问的到，就说明该对象是**Live**的，不能作为垃圾进行回收。**Region Invalidation**中也有类似的做法，例如下面的代码：
```
struct Node {
    int value; int *p;
};
void func(int*);
int main()
{
    int local = 10;
    Node n;
    n.value = 1; n.p = &local;
    // 虽然传递的实参是&n.value，和n.p没有任何关系，但是还是会将n.p中
    // 的值失效，也就是local的值失效
    func(&n.value);
    return 0;
}
```
这个例子和我们文章开头的例子很相似，我们通过debug.ViewExlpodedGraph获取其ExplodedGraph的子图如下：
![这里写图片描述](https://img-blog.csdn.net/20161031112636742)
在***func()***函数调用之前，***local***以及局部变量***n***的**binding**如代码中所示，都是有效的，在函数调用之后，首先局部变量***local***的值失效，另外整个局部变量***n***的**Cluster**都已经失效。
> 
The algorithm the traverses this transitive closure of accessiable regions is known as **ClusterAnalysis**, and **is also used for finding all live bindings in the store (in order to throw away the dead ones).** The name “ClusterAnalysis” predates the cluster-based organization of bindings, but refers to the same concept: **during invalidation and liveness analysis, all bindings within a cluster must be treated in the same way for a conservative model of program bevavior.**
就像上面的引用显示的那样，**Region Invalidation**失效时采用的方法是**Cluster Analysis**。另外在获取**store**中的**live binding**时采用的方法也是**Cluster Analysis**，这里就和垃圾回收中的***mark-sweep***算法就很相似了，其实**removeDead()**采用的就是***mark-sweep***算法，关于这个函数我们在后面的文章中再分析。从**Region Invalidation**我们可以大概猜测出**Cluster Analysis**的行为，如下图所示：
![这里写图片描述](https://img-blog.csdn.net/20161031130043702)
比如我们将**Object A**的数据成员***&Mem2***传递给一个**extern**函数调用，首先我们需要对**Object A**中的所有数据成员进行**Binding Invalidation**，然后需要依次对所有的数据成员进行**Cluster Analysis**，而且是递归地进行**Cluster Analysis**。
相对应的进行**Liveness Analysis**也是需要从某一个**root**递归地进行**Cluster Analysis**。相对应的**clang**中的[ClusterAnalysis](https://code.woboq.org/llvm/clang/lib/StaticAnalyzer/Core/RegionStore.cpp.html#%28anonymousnamespace%29::ClusterAnalysis)也起着两方面的作用，第一是**Region Invalidation**，第二是**Remove DeadBinding**。因此，**ClusterAnalysis**有两个子类[invalidateRegionsWorker](https://code.woboq.org/llvm/clang/lib/StaticAnalyzer/Core/RegionStore.cpp.html#945)和[removeDeadBindingsWorker](removeDeadBindingsWorker)。关于**removeDeadBindingsWorker**我们在以后得文章中再进行介绍。**ClusterAnalysis**定义如下：
```cpp
template <typename DERIVED>
class ClusterAnalysis {
protected:
    // 需要作ClusterAnalysis的MemRegion
    typedef llvm::DenseMap<const MemRegion *, const ClusterBindings *> ClusterMap;
    // 定义了工作队列元素类型，工作队列的子元素就是MemRegion*
    typedef const MemRegion * WorkListElement;
    // 定义了工作队列，WorkList
    typedef SmallVector<WorkListElement, 10> WorkList;
    // 声明用来存储已经遍历过的Cluster的数据成员
    llvm::SmallPtrSet<const ClusterBindings *, 16> Visited;
    // 工作队列
    WorkList WL;
    RegionStoreManager &RM;
    ASTContext &Ctx;
    SValBuilder &svalBuilder;
    // 存储所有Loc->Value键值对的RegionBindingsRef
    RegionBindingsRef B;
protected:
    const ClusterBindings *getCluster(const MemRegion *R) {
        return B.lookup(R);
    }
    /// Returns true if all clusters in the given memspace should be initially
    /// included in the cluster analysis. Subclasses may provide their
    /// own implementation.
    bool includeEntireMemorySpace(const MemRegion *Base) {
        return false;
    }
public:
    ClusterAnalysis(RegionStoreManager &rm, ProgramStateManager &StateMgr,
                    RegionBindingsRef b)
        : RM(rm), Ctx(StateMgr.getContext()),
          svalBuilder(StateMgr.getSValBuilder()), B(std::move(b)) {}
    bool isVisited(const MemRegion *R) {
        return Visited.count(getCluster(R));
    }
    // ClusterAnalysis的初始化
    void GenerateClusters() {
        // Scan the entire set of bindings and record the region clusters.
        for (RegionBindingsRef::iterator RI = B.begin(), RE = B.end();
            RI != RE; ++RI) {
            const MemRegion *Base = RI.getKey();
            const ClusterBindings &Cluster = RI.getData();
            static_cast<DERIVED*>(this)->VisitAddedToCluster(Base, Cluster);
            // if the base's memspace should be entirely invalidated, add the cluster
            // to the workspace up front.
            if (static_cast<DERIVED*>(this)->includeEntireMemorySpace(Base))
                AddToWorkList(WorkListElement(Base), &Cluster);
        }
    }
    // 向工作队列添加子元素，子类可以重写该方法实现自己的添加逻辑。
    bool AddToWorkList(WorkListElement E, const ClusterBindings *C) {
        if (C && !Visited.insert(C).second)
            return false;
        WL.push_back(E);
        return ture;
    }
    // 这个是ClusterAnalysis的主逻辑，开始运行工作队列算法。
    // 循环处理工作队列中的各个元素，对工作队列中的元素的处理是通过子类对应方法的
    // VisitCluster()实现的。如果子类没有自定义自己的版本，那么默认使用父类中的方法。
    void RunWorkList() {
        while (!WL.empty) {
            WorkLitsElement E = WL.pop_back_val();
            const MemRegion *BaseR = E;
            static_cast<DERIVED*>(this)->VisitCluster(BaseR, getCluster(BaseR));
        }
    }
    // 遍历RegionBindingsRef中的每一个Cluster
    void VisitAddedToCluster(const MemRegion *baseR, const ClusterBindings &C) {}
    // 这个是具体的ClusterAnalysis需要重写的方法，执行工作队列算法时对其中的子元素的
    // 处理是由子类相应的方法实现，例如invalidateRegionsWorker主要是使某个binding失效，
    // 并递归进行ClusterAnalysis。
    void visitCluster(const MemRegion *baseR, const ClusterBindings *C) {}
};
```
上面的定义中核心方法就是**RunWorkList()**和**VisitCluster()**，**RunWorkList()**执行工作队列算法，而**VisitCluster()**是在执行工作队列算法中对具体元素进行处理。其实简单来说**Cluster Analysis**就是对**Cluster**以及其所能索引到的**Cluster**进行递归处理。
### invalidateRegionsWorker
介绍完**ClusterAnalysis**，下面介绍其中一个子类（其实说子类也不准确，毕竟**CRTP**继承）**invalidateRegionsWorker**，**invalidateRegionsWorker**可以说是**evalConservativeCall()**中进行**Region Invalidation**的核心类，该类递归的对**MemRegion**进行**Cluster Analysis**，以确定哪些bindings失效。
类**invalidateRegionsWorker**重写的父类的成员方法有***VisitCluster()***和***AddToWorkList(const MemRegion*)**，并添加了自己的独有的成员方法***VisitBinding()**。下面我们依次介绍这几个方法。
#### VisitCluster()
第一个方法是***VisitCluster()***，该方法很长有近**200**行，我们简单介绍其中比较重要的部分。
```
// 注意该函数调用的时候传入的第二个参数是ClusterBindings，而且通常是getCluster(baseR)
// 获得的，此时获得的Cluster是MemRegion所处的Cluster。
// 例如：
//        struct A { int mem1; int mem2};
//        A a; 
//        a.mem1 = 1; 
//        a.mem2 = 2;
//    那么getCluster(&a.mem2)的Cluster是其super region的整个Cluster：
//        <&a.mem1 : 1>
//        <&a.mem2 : 2>
void invalidateRegionsWorker::VisitCluster(const MemRegion *baseR,
                                    const ClusterBindings *C) {
    bool PreserveRegionsContents = 
    ITraits.hasTrait(baseR,
                RegionAndSymbolInvalidationTraits::TK_PreserveContents);
    // （1）遍历Cluster中的bindings，并调用VisitBinding()对Cluster中每一个binding
    // 进行进一步的处理。
    // 如果当前Cluster整体对应的Binding没有TK_PreserveContents属性，则删除baseR->Value
    // 这个binding（该binding是对当前Cluster而言的）。
    // TK_PreserveContents是分析引擎给当前baseR添加的属性，例如实参传递时使用const A&
    // 则说明当前object A的值是不会失效的。
    if (C) {
        for (ClusterBindings::iterator I = C->begin(), E = C->end(); I != E; ++i)
            VisitBinding(I.getData());
        // Invalidate regions contents.
        if (!PreserveRegionContents)
            B = B.remove(baseR);
    }
    // （2）针对BlockDataRegion，可以和闭包的概念联系起来，
    // C++中和BlockData和BlockCode相关的只有lambda表达式，这里不详细解释。
    // BlockDataRegion? If so, invalidate captured variables that are passed by reference.
    if (const BlockDataRegion *BR = dyn_cast<BlockDataRegion>(baseR)) {
        // ...
    }
    // （3）如果当前是TK_PreserveContents，那么直接返回，因为baseR的binding不会失效，
    // 也不用对其绑定一些conjure值。
    // （4）进行一些信息的记录，例如当前的Region我们已经考察过了，这个有可能和处理循环引用相关。
    if (Regions)
        Regions->push_back(baseR);
    // （5）由于binding失效，就需要给这些region绑定上一些其他值，使用default或者direct形式。
    // 对于array或者struct类型，binding kind是default，对于普通变量binding kind是direct。
    // 例如：
    // （5）.1 结构体或类类型。
    if (T->isStructureOrClassType()) {
        // Invalidate the region by setting its default value to
        // conjured symbol. The type of the symbol is irrelevant.
        DefinedOrUnknownSVal V = svalBuilder.conjureSymbolVal(baseR, Ex, LCtx,
                            Ctx.IntTy, Count);
        B = B.addBinding(baseR, BindingKey::Default, V);
        return;
    }
    // 内置类型
    DefinedOrUnknownSVal V = svalBuilder.conjureSymbolVal(baseR, Ex, LCtx,
                            T, Count);
    B = B.addBinding(baseR, BindingKey::Direct, V)
}
```
通过上面的源码分析，我们知道***VisitCluster()***主要执行了两个操作，使***baseR***对应的**Cluster**失效，并创建一个**conjureSymbolVal**（表示对未知值，也就是对***baseR***的值一无所知）绑定在***baseR***上面。但是在介绍***VisitCluster()***的时候，我们忽略了一个函数调用的介绍就是***VisitBinding()***，在***VisitCluster()***中会对***baseR***对应的**Cluster**中的每一个**binding**调用***VisitBinding()***。只有对**baseR**对应**Cluster**中的每一个**binding**作进一步考察，这样才能真正执行**Cluster Analysis**。函数***VisitCluster()***的示意图如下：
![这里写图片描述](https://img-blog.csdn.net/20161031161030307)
假设上图中黄色部分，是调用***VisitCluster()***时，参数**MemRegionA**对应的**Cluster**，此时的**WorkList**只有**MemRegionA**。我们需要依次对**MemRegionA**对应**Cluster**中的每个**binding**调用***VisitBinding()***操作。上图中红色虚线表示某一个**BindingPtr**结点可以访问到其他的**MemRegion**。
下面是***VisitBinding()***函数的主体逻辑：
```
void invalidateRegionsWorker::VisitBinding(SVal V {
    // （1）如果当前binding是一个指针类型，指向某一个MemRegion，那么将指向的那个MemRegion
    // 直接加入到WorkList，再runWorkList()的时候对其进行同样的VisitCluster()操作。
    if (const MemRegion *R = V.getAsRegion()) {
        AddToWorkList(R);
        return;
    }   
    // （2）如果当前是binding是LazyCompounVal类型，则获取其真正的CompoundVal,
    // 遍历该CompoundVal中的每个binding，对每个binding调用VisitBinding()。
    // 注：LazyCompoundVal用来表示类对象或结构体对象的值，传递或者赋值的时候效率
    // 较高。
    // Is it a LazyCompoundVal? All references get invalidated as well.
    if (Optional<nonloc::LazyCompoundVal> LCS = 
        V.getAs<nonloc::LazyCompoundVal>()) {
            const RegionStoreManager::SvalListTy &Vals = RM.getInterestingValues(*LCS);
            for (RegionStoreManager::SValListTy::const_iterator I = Vals.begin(),
                                            E = Vals.end();
                                            I != E; ++I)
                VisitBinding(*I);
        }
}
```
***VisitCluster(MemRegionA, )***调用时，会对**MemRegionA**对应**Cluster**的每个**binding**依次调用***VisitBiding()***，此时示意图如下所示：
![这里写图片描述](https://img-blog.csdn.net/20161031161149065)
红色部分表示**MemRegionA**的**Cluster binding**已经失效，由于调用了***VisitBinding()***时将**MemRegionB**添加到**WorkList**中，此时调用***VisitCluster(MemRegionB,)***函数。就这样依次递归地遍历所有的***transitive closure***（传递闭包，虽然感觉这个术语在这里不合适）。
#### AddToWorkList(const MemRegion*)
重写后的***AddToWorkList(const MemRegion*)***函数如下：
```
bool invalidateRegionsWorker::AddToWorkList(const MemRegion *R) {
    // ITRaits是在前面调用的CallEvent::invalidateRegions()时设置的
    // 一般只会设置TK_PreserveContents。所以AddToWorkList(const MemRegion *R)时
    // 会将其SuperRegion添加到WorkList中。
    bool doNotInvalidateSuperRegion = ITraits.hasTrait(
        R, RegionAndSymbolInvalidationTraits::TK_DoNotInvalidateSuperRegion);
    const MemRegion *BaseR = doNotInvalidateSuperRegion ? R : R->getBaseRegion();
    return AddToWorkList(WorkListElement(BaseR), getCluster(BaseR));
}
```
上面判断**ITraits**是否有**TK_DoNotInvalidateSuperRegion**属性，是很重要的。否则，在对一块**Region**进行**Cluster Analysis**时，就不会考虑其“相邻”内存区域。如下代码所示：
```
struct Node { int count; int mem; }
Node n;
// ...
// 由于C语言指针的算术运算，我们是要考虑n.count失效的可能性的，所以需要将整个
// n对应的MemRegion作为ClusterAnalysis的初始Region。
func(&n.mem);
```
在**clang static analyzer**内存模型中，有**SuperRegion**的类有如下三个：
- AllocaRegion
- TypedRegion
- SymbolicRegion
***但是又回到前面的那个疑问，既然要考虑把n.mem相邻的内存区域失效，那么为什么不把当前StackFrameContext中的StackLocasRegion作为分析的根集合，更进一步使整个Stack Region失效？***
## invalidateRegions
介绍完**clang static analyzer**内存模型和**invalidateRegionsWorker**以后，就要回到正题，关注**Region Invalidation**的具体流程。我们还是沿着下面的四个函数来进行分析。
- **CallEvent::invalidateRegions**
- **ProgramState::invalidateRegions**
- **ProgramState::invalidateRegionsImpl**
- **RegionStoreManager::invalidateRegions**
### [CallEvent::invalidateRegions](https://code.woboq.org/llvm/clang/lib/StaticAnalyzer/Core/CallEvent.cpp.html#_ZNK5clang4ento9CallEvent17invalidateRegionsEjN4llvm18IntrusiveRefCntPtrIKNS0_12ProgramStateEEE)
这个函数被***evalConservativeCall()***直接调用，下面我们简单给出该函数的定义。
```cpp
ProgramStateRef CallEvent::invalidateRegions(unsigned BlockCount,
                            ProgramStateRef Orig) const {
    // 注：关于这里，我有些疑问？有const限定符的method也有可能修改global啊，为什么要直接返回呢？
    // Don't invalidate anything if the callee is marked pure/const.
    if (const Decl *callee = getDecl())
        if (callee->hasAttr<PureAttr>() || callee->hasAttr<ConstAttr>())
            return Result;
    // (1) 首先判断传入的实参是否是指向const的指针，如果是的话该指针所指向Cluster的
    // bindings就不会失效，联系我们上面介绍的VisitCluster()函数。
    // 注：关于argumentsMayEscape，由于我不太了解，所以不介绍。知道者欢迎交流。
    // Indexes of arguments whose values will be preserved by the call.
    llvm::SmallSet<unsigned, 4> PreserveArgs;
    if (!argumentsMayEscape())
        findPtrToConstParams(PreserveArgs, *this);
    // (2) 将实参SVal保存起来，以便进行后续的region invalidate操作。
    // 注：这里会设置const to pointer的MemRegion的属性，TK_PreserveContents。
    for (unsigned Idx = 0, Count = getNumArgs(); Idx != Count; ++Idx) {
        if (PreserveArgs.count(Idx))
            if (const MemRegion *MR = getArgSVal(Idx).getAsRegion())
                ETraits.setTrait(MR->getBaseRegion(),
                                RegionAndSymbolInvalidationTraits::TK_PreserveContents);
        ValuesToInvalidate.push_back(getArgSVal(Idx));
    }
    // 调用ProgramState的invalidateRegions()函数。
    return Result->invalidateRegions(ValuesToInvalidate, getOriginExpr(),
                BlockCount, getLocationContext(),
                /*CausedByPointerEscape*/true,
                /*Symbols=*/nullptr, this, &ETraits);
}
```
***ProgramState::invalidateRegion()***很简单，通过**Pimpl**（桥接模式，**pointer to the implementation**）调用***invalidateRegionsImpl()***函数，该函数实现了**ProgramState**执行**Region Invalidation**的主要逻辑。
### ProgramState::invalidateRegionsImpl
***ProgramState::invalidateRegionImpl()***调用***StoreManager::invalidateRegions()***函数，并通知相关的**checker**，然后通过**ExprEngine**调用***processRegionChanges()***进行相关的处理，这里也不多做介绍。
### RegionStoreManager::invalidateRegions
该函数是整个**Region Invalidation**的核心，其他的都是为这个函数的执行做准备。下面我们通过源码，分析一下该函数的执行过程。
该函数的注释很重要，基本上将invalide region的思想和结果表达出来了，这里我单独摘出来着重强调。
> 
invalidateRegions - **Clears out the specified regions from the store, marking their values as unknown**.  - invalidate region的结果
**Depending on the store, this may also invalidate additional regions that may have changed based on accessing the given regions.**  - invalidate regions时所采用的思想(**accessing**)
**Optionally, invalidates non-static globals as well.** - 一些额外说明
```
/// \param[in] store The initial store
/// 
/// \param[in] Values The values to invalidate.
/// 
/// 解释：使用conjure symbols来标识失效regions
/// \param[in] E Te current statement being evaluated. Used to conjure
/// symbols to mark the values of invalidated regions.
/// 
/// \param[int] Count The current block count.
/// 
/// \param[in] Call The call expression which will be used to determine which
/// globals should get invalidated.
/// 
/// \param[in, out] IS A set to fill with any symbols that are no longer
/// accessible. Pass \c NULL if this information will not be used.
/// 
/// \param[in] ITraits Information about invalidation for a particular 
/// region/symbol.
/// 
/// \param[in, out] InvalidatedTopLevel A vector to fill with regions 
/// explicitly being invalidated. Pass \c NULL if this 
/// information will not be used.
/// 
/// \param[int, out] Invalidated A vector to fill with any regions being
/// invalidated. This should include any regions explicitly invalidated
/// even if do not currently have bindings. Pass \c NULL if this
/// information will not be used.
StoreRef 
RegionStoreManager::invalidateRegions(Store store,
                                    Array<SVal> Values,
                                    const Expr *Ex, unsigned Count,
                                    const LocationContext *LCtx,
                                    const CallEvent *Call,
                                    InvalidatedSymbols &IS,
                                    RegionAndSymbolInvalidationTraits &ITraits,
                                    InvalidatedRegions *TopLevelRegions,
                                    InvalidatedRegions *Invalidated) {
    // (1) 根据CallEvent决定使哪一部分global region失效，如果是库函数，则使system 
    // global region失效。如果是普通函数则使non static global space region失效。
    GlobalsFilterKind GlobalsFilter;
    if (Call) {
        if (Call->isInSystemHeader())
            GlobalsFilter = GFK_SystemOnly;
        else
            GlobalsFilter = GFK_All;
    } else {
            GlobalsFilter = GFK_None;
    }
    // (2) 根据const void* store转换（获得）成RegionBindingsRef。
    RegionBindingsRef B = getRegionBindings(store);
    // 创建invalidateRegionsWorker
    invalidateRegionsWorker W(*this, StateMgr, B, Ex, Count, LCtx, IS, ITraits,
                            Invalidated, GlobalsFilter);
    // (3) invalidateRegionsWorker刚创建时，WorkList为空。GenerateClusters()
    // 遍历原有所有的RegionBindingsRef，根据具体的子类（这里是invalidateRegionsWorker）
    // 产生不同的行为。对于invalidateRegionsWorker来说，该函数除了遍历RegionBindingsRef
    // 以外没有任何额外的行为，此时WorkList依然是空。
    // Scan the bindings and generate the clusters.
    W.GenerateClusters();
    // (4) 根据前面传入的Values<SVal>来初始化需要作ClusterAnalysis的WorkList，
    // 此时会对SVal的类型进行详细的分析以后，填充WorkList。
    // Add the regions to the worklist.
    populateWorkList(W, Values, TopLevelRegions);
    // (5) 开始运行工作队列算法，以刚刚populateWorkList()得到的WorkList
    // 作为初始队列，迭代进行ClusterAnalysis。
    W.RunWorkList();
    // Return the new bindings.
    B = W.getRegionBindings();
    // (6) 进行全局Region的失效处理
    // For calls, determine which global regions should be invalidated and
    // invalidate them. (Note that function-static and immutable globals are never
    // invalidated by this.)
    switch(GlobalsFilter) {
        case GFK_All:
            B = invalidateGlobalRegion(MemRegion::GlobalInternalSpaceRegionKind,
                    Ex, Count, LCtx, B, Invalidated);
        // FALLTHROUGH
        case GFK_SystemOnly:
            B = invalidateGlobalRegion(MemRegion::GlobalSystemSpaceRegionKind,
                    Ex, Count, LCtx, B, Invalidated);
        // FALLTHROUGH
        case GFK_None:
            break;
    }
    return StoreRef(B.asStore(), *this);
}
```
上述的***MemRegionStoreManager::invalidateRegions()***函数的逻辑大致分为**5**个部分，我们在注释中也给出了相关的解释。下面我们结合实际的代码来说明该函数的执行流程。代码如下：
```
int global = 10;
struct Node {
    Node *next; int value;
};
class A {
public:
    Node n;
    int count;
    A(Node n) : n(n), count(0) {}
};
struct B { int *memPtr; };
struct C { int mem1; int mem2; };
int func(int num, int *ptr, const A &AObject, B BObject, int*);
int main() {
    // (1) 定义三个局部变量
    int local1 = 1; int local2 = 2; int local3 = 3;
    // (2) 定义两个Node类型对象，用于初始化局部变量localA1
    Node localN1;
    localN1.next = 0;
    localN1.value = 10;
    Node localN2;
    localN2.next = 0;
    localN2.value = 20;
    localN1.next = &localN2;
    // (3) 定义局部对象localA1
    A localA1(localN1);
    // (4) 定义局部对象localB
    B localB;
    localB.memPtr = &local3;
    // (5) 定义局部对象c
    C c;
    c.mem1 = 100;
    c.mem2 = 200;
    // 函数调用几乎涉及到了所有的局部对象
    int num = func(local1, &local2, localA1, localB, &c.mem2);
    return num + local1 + local2 + localA1.count + 
                    *localB.memPtr + c.mem1;
}
```
以上面的代码为例，在执行***RegionStoreManager::invalidateRegions()***的开始，准备进行失效处理时的**SVal**值如下图所示。
![这里写图片描述](https://img-blog.csdn.net/20161101104100929)
开始，等待失效的**SVal**包含所有实参的值，有**5**个值，只有引用、指针（也就是**SVal**是**Loc**类型的）或者复合值（自定义对象）中的引用、指针会被放到初始化**WorkList**中。决定哪些**SVal**可以放到初始的**WorkList**中是由函数***populateWorkList()***实现的。下面是该函数的定义，比较简单。
```
void RegionStoreManager::populateWorkList(invalidateRegionsWorker &W,
                                ArrayRef<SVal> Values,
                                InvalidateRegions *TopLevelRegions) {
    // 遍历这些SVal来决定哪些SVal可以加入初始化的WorkList。
    for (ArrayRef<SVal>::iterator I = Values.begin(),
                                E = Values.end(); I != E; ++I)
    {
        SVal V = *I;
        // 如果当前值CompoundSVal，也就是自定义类型对象，那么遍历其所有子数据成员，
        // 如果子数据成员SVal是loc类型，代表某一块Region，则添加到WorkList中。
        if (Optional<nonloc::LazyCompoundVal> LCS = 
            V.getAs<nonloc::LazyCompoundVal>()) {
            const SValListTy &Vals = getInterestingValues(*LCS);
            // 遍历其子数据成员
            for (SValListTy::const_iterator I = Vals.begin(),
                                        E = Vals.end(); I != E; ++I)
            {
                if (const MemRegion *R = (*I).getAsRegion())
                    W.AddToWorkList(R);
            }
            continue;
        }
        // 如果当前SVal就是loc类型，表示某一块region，则将其添加到WorkList中
        // 注意只有顶层的SVal会加入到TopLevelRegion中，而LazyCompoundSVal
        // 中的数据成员如果添加到WorkList，则不是TopLevelRegions
        if (const MemRegion *R = V.getAsRegion()) {
            if (TopLevelRegions)
                TopLevelRegions->push_back(R);
            // 这里不一定添加的是其本身，有可能是其SuperRegion
            W.AddToWorkList(R);
            continue;
        }
    }
}
```
调用***populateWorkList()***初始化**WorkList**，然后在**WorkList**上调用***RunWorkList()***函数，执行工作队列算法，其实也就是递归地执行**cluster analysis**。我们在介绍**ClusterAnalysis**类的时候，已经介绍过***RunWorkList()***其实就是一个***while***循环，不断地从**WorkList**中**pop_back**子元素，然后调用***VisitCluster()***进行进一步的处理。
在调用***RunWorkList()***进行**cluster analysis**之前，**WorkList**以及**RegionBingingsRef**如下图所示：
![这里写图片描述](https://img-blog.csdn.net/20161101105023570)
***RunWorkList()***第一步从**WorkList**中**pop_back**出**MemRegions - c**，调用***VisitCluster()***函数。如下图红色区域所示，该函数首先遍历该**Cluster**中的所有**binding**，然后使其**Cluster**无效。然后对***c***对应的**MemRegion**绑定上类型为**default**的**conjureSymbolVal**。
![这里写图片描述](https://img-blog.csdn.net/20161101105356076)
第二步从**WorkList**中**pop_back**出**MemRigion - local3**，调用***VisitCluster()***函数。使**MemRegion - local3**上的**binding**失效，然后绑定一个**conjureSymbolVal**。如下图所示：
![这里写图片描述](https://img-blog.csdn.net/20161101105825451)
第三步从**WorkList**中**pop_back**出**MemRegion - localA1**，然后调用***VisitCluster()***函数，遍历***localA1***对应**Cluster**中的所有**binding**，由于第一个**binding**指向的是**MemRegion - localN2**，所以将***localN2***对应的**MemRegion**添加到**WorkList**中。由于实参值***&localA1***对应的参数类型是***const &A***，所以***localA1***对应**Cluster**中的**binding**都不会失效。如下图所示，图中第一条蓝线从***&localA1***获取到对应**Cluster**，第二条蓝线从第一个**binding**获取到其***Loc->Value***中的**Value(*&localN2*)**，判断该**SVal**是**loc**类型，然后第三条蓝线表示将第二条蓝线对应的**MemRegion**添加到**WorkList**中。
![这里写图片描述](https://img-blog.csdn.net/20161101110355079)
第四步重复上面的操作，从**WorkList**中**pop_back**出**MemRegio-localN2**，然后使其对应的**Cluster**失效，并重新绑定上一个**default**的**ConjureSymbolVal**。
![这里写图片描述](https://img-blog.csdn.net/20161101111200428)
最后一步从**WorkList**中**pop_back**出**MemRegion - local2**，然后调用***VisitCluster()***，遍历***local2***对应**Cluster**的**binding**，并使其失效。最终，***while***循环终止，***RunWorkList()***函数执行完毕。此时最终状态如下图所示：
![这里写图片描述](https://img-blog.csdn.net/20161101111536476)
***注：为了显示清晰，我没有将全局变量MemRegion对应的Cluster展示出来。另外，Global MemRegion的创建使用lazy机制，用不到就不会显示创建。***
执行完***RunWorkList()***就表示***Cluster Analysis***已经结束了，最后会检查***GlobalsFilter***来确定到底是哪一块**Global**内存失效，其中***GFK_All***是隐含着另外两种类型，在***RegionStoreManager::invalidateRegions()***函数最后的***switch***语句中的第一个***GFK_All case***是没有***break***语句的。
至此***invalidateRegions()***相关的问题已经解释清楚了，下面介绍下***evalConservativeCall()***如何处理返回值。
# bindReturnValue
在***evalConservativeCall()***中处理返回值就比较简单了，就是创建相应类型的**ConjureSymbolVal**，作为返回值即可，下面是该函数的主要逻辑。
```cpp
ProgramStateRef ExprEngine::bindReturnValue(const CallEvent &Call,
                        const LocationContext *LCtx,
                        ProgramStateRef State) {
    const Expr *E = Call.getOriginExpr();
    if (!E)
        return State;
    // Conjure a symbol if the return value is known.
    QualType ResultTy = Call.getResultType();
    SValBuilder &SVB = getSValBuilder();
    unsigned Count = currBldrCtx->blockCount();
    // 根据返回值的类型创建对应的ConjureSymbolSVal
    SVal R = SVB.conjureSymbolVal(nullptr, E, LCtx, ResultTy, Count);
    // 并将得到的ConjureSymbolSVal绑定到调用表达式上，也就是在Environment中
    // 创建<CallExpr, ConjureSymbolSVal>。
    return State->BindExpr(E, LCtx, R);
}
```
上面的代码中唯一的需要继续分析的就是**ConjureSymbolVal**，前面我们提到**MemRegion**失效以后，会在其上创建并绑定一个**ConjureSymbolVal**。那么什么是**ConjureSymbolVal**呢？
**ConjureSymbolVal**的意思就是未知值，我们对该值一无所知。我们可以将***SymbolConjured***的注释信息拿过来帮助理解**ConjureSymbolVal**。
> 
A symbol representing the result of an expression in the case **when we do not know anything about what the expression is**.
联系前面的**region invalidation**，由于对应的函数调用对**region**做什么样的操作，我们一无所知，也就是对**region**上的值，我们不能做任何假设，也就是一无所知。
另外我前面提到过**LazyCompoundVal**，当时并没有做过多解释，这里稍作介绍。**LazyCompoundVal**其实也就是**Lazy Bndings**，是一种优化措施。**LazyCompoundVal**主要用于用户自定义对象的拷贝，使用**LazyCompoundVal**可以明显提高的拷贝效率，在拷贝过程中直接使用**LazyCompoundVal**表示该聚合对象的值，不用携带着大量的“值”。虽然在真正用到**LazyCompoundVal**对应的值的时候，还是需要一一获取其中的值，但是有时候拷贝的值并不会被使用，此时也就省去了无谓的拷贝过程。关于这方面，**clang**有关于**RegionStore**的文档[Regiontore.txt](https://chromium.googlesource.com/external/llvm.org/clang/+/google/stable/docs/analyzer/RegionStore.txt)也有介绍，上面的介绍更清晰，我直接粘贴过来。就不画蛇添足再进行介绍了。
> 
RegionStore implements an optimization for copying aggregates (structs and arrays) called “**lazy bindings**“，implemented using a special SVal called LazyCompoundVal. **When the store is asked for the “binding” for an entire aggregate (i.e. for an lvalue-to-rvalue conversion), it returns a LazyCompoundVal instead.** When this value is then stored into a variable, it is bound as a **Default** value. This makes copying arrays and structs much cheaper than if they had required **memberwise** access.
下面将文档中**LazyCompoundVal**的代码示例抄过来，如下。
```
CGPoint p;
    p.x = 42;       // A Direct binding is made to the FieldRegion 'p.x'.
    CGPoint p2 = p; // A LazyCompoundVal is created for 'p', along with a
                    // snapshot of the current store state. This value is then
                    // used as a Default binding for the VarRegion 'p2'.
    return p2.x;    // The binding for FieldRegion 'p2.x' is requested.
                    // There is no Direct binding, so we look for a Default
                    // binding to 'p2' and find the LCV.
                    // Because it's an LCV, we look at our requested region
                    // and see that it's the '.x' field. We ask for the value
                    // of 'p.x' within the snapshot, and get back 42.
```
关于**LazyCompoundVal**的实现机制，在后面的文章中再做分析。至此，关于**RegionStore**和**evalConservativeCall()**中具体逻辑已经解释清楚了。现在还有两点不是特别清楚，一是基于**Cluster Analysis**的***removeDead()***，该函数是**clang static analyzer**中比较重要的一个模块，因为如果不做***removeDead()***的话，分析引擎的内存肯定会吃不消，关于***removeDead()***会在后面的文章中展开。另外一点就是**LazyCompoundVal**的实现机制，这里也没有过多提及。
最后再强调一下理解**clang static analyzer**内存模型很重要的两篇资料，一是**许中兴**和**Ted Kremenek**合作的一篇文章《[A Memory Model for Static Analysis of C Programs](http://lcs.ios.ac.cn/~xuzb/canalyze/memmodel.pdf)》，另外一篇是**clang**关于**RegionStore**的文档[RegionStore.txt](https://chromium.googlesource.com/external/llvm.org/clang/+/google/stable/docs/analyzer/RegionStore.txt)。
