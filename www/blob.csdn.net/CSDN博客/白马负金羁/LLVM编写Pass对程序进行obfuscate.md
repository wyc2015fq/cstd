# LLVM编写Pass对程序进行obfuscate - 白马负金羁 - CSDN博客





2018年06月03日 15:54:30[白马负金羁](https://me.csdn.net/baimafujinji)阅读数：728








对程序进行obfuscate在软件安全领域有诸多应用，其直接的目的就是对那些试图进行逆向工程的努力带来一些困扰。假设你已经阅读了本系列文章中前面的几篇，并已经掌握了在LLVM中编写Pass的基本方法，本文将介绍如何通过在LLVM中加入相关Pass的方法实现对程序的扰乱。本文主要根据文献【1】整理而得，部分英文摘自原文。

##### 〇、一些准备工作

我们所有的实验都是在 mac OS 上进行的，所用的IDE是Xcode。当然你使用cmake或者ninja来build程序，或者在Ubuntu下开展如下实验也是可以的，大体方法都是一致的。首先我们编写一个简单的pass如下，它的作用是遍历每个Basic Block，当它遇到一个Basic Block时，就输出一条信息“I am running a block...”。


```cpp
#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/raw_ostream.h"

#include "llvm/Transforms/IPO/PassManagerBuilder.h"

using namespace llvm;

namespace {

  class MyObfPass01 : public BasicBlockPass {

    public:
      static char ID;
      MyObfPass01() : BasicBlockPass(ID) {}

      bool runOnBasicBlock(BasicBlock &BB) override {
        errs() << "I am running on a block...\n";
        return false;
      }
  };
}

char MyObfPass01::ID = 0;
static RegisterPass<MyObfPass01> X("myobfpass01", "Obfuscates zeroes tutorial 01");
```

因为之前我们已经非常详细地解释过如何执行一个pass了，这里不再赘述。首先你要选择编译刚刚编写的pass，如下图所示。这里因为我们的环境是macOS，所以生成的动态链接库是LLVMMyObfPass01.dylib。


![](https://img-blog.csdn.net/201806030313424)


然后，因为我们这里使用的Xcode环境，所以你需要先为clang配置环境：

-S -emit-llvm /Users/fzuo/Desktop/file.c -o /Users/fzuo/Desktop/file.ll


其中file.c是我们用了做测试的一个源码文件：

```java
#include <stdio.h>

int foo(){return 1;}

int main() {
    puts("Hello world");

    return 0;
}
```
最后，在配置opt命令的执行参数时，你需要使用类似下面的命令，其中myobfpass01是由前面代码中“X("myobfpass01", "Obfuscates zeroes tutorial 01")”里面的第一个参数所指定的：
-load /Users/fzuo/llvm/build/Debug/lib/LLVMMyObfPass01.dylib -myobfpass01 /Users/fzuo/Desktop/file.ll

执行opt程序之后，你会在Xcode的输出窗口中看到如下结果，因为程序中包含有两个Basic Block，所以提示信息被输出了两次。

![](https://img-blog.csdn.net/20180602152340241)


##### 一、扰乱策略

作为一个教学演示的例子，我们这里所使用的obfuscation策略是非常简单的。Our mission is to obfuscate all null literals in the code. It means that we are going to replace (almost) all the zeroes in the code by a non-trivial boolean expression, proved to be always false.

![](https://img-blog.csdn.net/20180603033328711)


其中，
- p1和p2是两个不同的质数；
- a1和a2是两个不同的随机正数；
- x和y是从程序中挑出的两个变量（they have to be reachable from the obfuscation instructions）


要实现上述策略，我们首先要遍历basic block中的每条instruction，并检查其中的操作数是否为null。为此我们在之前的MyObfPass01基础上编写一个新的pass，称其为MyObfPass02：

```cpp
#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Constants.h"
#include "llvm/Support/raw_ostream.h"

#include "llvm/Transforms/IPO/PassManagerBuilder.h"

#include "llvm/IR/Instructions.h"

using namespace llvm;

namespace {

  class MyObfPass02 : public BasicBlockPass {

    public:

    	static char ID;

    	MyObfPass02() : BasicBlockPass(ID) {}

    	bool runOnBasicBlock(BasicBlock &BB) override {

    		// Not iterating from the beginning to avoid obfuscation of Phi instructions
    		// parameters
    		for (typename BasicBlock::iterator I = BB.getFirstInsertionPt(), 
    										   end = BB.end(); I != end; ++I) {
      
      			Instruction &Inst = *I;
      			for (size_t i = 0; i < Inst.getNumOperands(); ++i) {
          			if (Constant *C = isValidCandidateOperand(Inst.getOperand(i))) {
          				errs() << "I've found one sir!\n";
          			}
        		}
      		}
      		return false;
      	}

    private:

    	Constant *isValidCandidateOperand(Value *V) {
    		Constant *C;
    		if (!(C = dyn_cast<Constant>(V))) return nullptr;
    		if (!C->isNullValue()) return nullptr;
    		// We found a NULL constant, lets validate it
    		if(!C->getType()->isIntegerTy()) {
      		//dbgs() << "Ignoring non integer value\n";
      			return nullptr;
    		}
    	return C;
  		}

  };
}

char MyObfPass02::ID = 0;
static RegisterPass<MyObfPass02> X("myobfpass02", "Obfuscates zeroes tutorial 02");
```
The runOnBasicBlock method is going to iterate through all the instructions of the block (for loop) and check if any operand of those instructions is an eligible null literal. If any of the operand is a null literal we print a message on the debug stream and we continue.




此外，你可能注意到了the for loop is initialized with BB.getFirstInsertionPt(). 但另外一种你可能更为熟悉的逐条遍历Basic Block中指令的方法或许应该是下面这样的：


```cpp
for(auto &I : BB) {
}
```

但问题是现在我们不想修改 some of the special instructions located at the beginning of the block (the phi instructions), so we skip them altogether and set the iterator to the first 'normal' instruction.

私有成员方法isValidCandidateOperand被用于检查它的参数是否是一个literal (constant means literal in LLVM, not variable declared const)。它也会检查literal的类型, 并确保它不能是一个 pointer 或者一个浮点值 （原因后面会解释）。类型检查的工作由函数 dyn_cast<> 来负责，它会检查其参数是否能够被转换成给定的模板参数所指定之类型。dyn_cast<> is used in LLVM instead of RTTI（run time type information）because it was deemed too expensive。如果所有这些条件都满足 and the literal is null we return a pointer to the operand (cast as a Constant) else nullptr.





让我们来看一下file.c文件在LLVM中被转换成IR文件之后的样子：


```
; Function Attrs: noinline nounwind optnone ssp uwtable
define i32 @foo() #0 {
entry:
  ret i32 1
}

; Function Attrs: noinline nounwind optnone ssp uwtable
define i32 @main() #0 {
entry:
  %retval = alloca i32, align 4
  store i32 0, i32* %retval, align 4
  %call = call i32 @puts(i8* getelementptr inbounds ([12 x i8], [12 x i8]* @.str, i32 0, i32 0))
  ret i32 0
}
```

如果从源代码的角度来看，你会直觉地认为程序中只包含有一个null值常量，但如果你编译并运行pass来测试你的代码，你会发现其实程序中包含有两个null值常量。这一点在IR文件中也一览无遗。The two 0 that triggered the debug message from our pass are in the store and ret instructions.





下面来测试一下。使用刚刚生成的MyObfPass02，那么在程序执行之后，你就会看到如下结果，这与我们基于IR文件的分析是相一致的：


![](https://img-blog.csdn.net/20180603035538627)


所以到此为止，我们已经可以找到所有的null值常量了，下面就需要替换它们，为此我们需要（注意下面这几条都是本文前面所提到的obfuscation策略所需要用到的）：

- To know the variables reachable from the instruction containing the eligible literal
- To generate the instructions of the arithmetic expression seen earlier
- To insert those expressions back into the code
- (Optional) Generate random prime numbers


接下来，就一步一步地来实现它。





##### 二、Reachable variables




To be sure to have a pool of reachable variable during our obfuscation, we are going to register all the variables with integral type we come across while iterating through the block instructions.


为此，对MyObfPass02这个pass稍作修改，我们将其命名为MyObfPass03，相比较而言，具体的修改包括：
- 增加一个vector类型的类成员，它存储指向 Integer（或values of interest）的指针。We will empty it at the end of every block.

- 增加一个方法，它被用来检查指令的类型，如果该指令是eligible，则将其存储在vector中。
- call the above mentioned method from the main loop.


```cpp
#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Constants.h"
#include "llvm/Support/raw_ostream.h"

#include "llvm/Transforms/IPO/PassManagerBuilder.h"

#include "llvm/IR/Instructions.h"

#include <vector>

using namespace llvm;

namespace {

  class MyObfPass03 : public BasicBlockPass {
    std::vector<Value *> IntegerVect;

    public:

    	static char ID;

    	MyObfPass03() : BasicBlockPass(ID) {}

    	bool runOnBasicBlock(BasicBlock &BB) override {
                IntegerVect.clear();
    		// Not iterating from the beginning to avoid obfuscation of Phi instructions parameters
    		for (typename BasicBlock::iterator I = BB.getFirstInsertionPt(), 
    		        end = BB.end(); I != end; ++I) {
      
      			Instruction &Inst = *I;
      			for (size_t i = 0; i < Inst.getNumOperands(); ++i) {
          			if (Constant *C = isValidCandidateOperand(Inst.getOperand(i))) {
          				errs() << "I've found one sir!\n";
          			}
        		}
            registerInteger(Inst);
      		}
      		return false;
      	}

    private:

      void registerInteger(Value &V) {
        if (V.getType()->isIntegerTy()) {
          IntegerVect.push_back(&V);
          errs() << "Registering an integer!" << V << "\n";
        }
      }

    	Constant *isValidCandidateOperand(Value *V) {
    		Constant *C;
    		if (!(C = dyn_cast<Constant>(V))) return nullptr;
    		if (!C->isNullValue()) return nullptr;
    		// We found a NULL constant, lets validate it
    		if(!C->getType()->isIntegerTy()) {
      		//dbgs() << "Ignoring non integer value\n";
      			return nullptr;
    		}
    	return C;
  		}

  };
}

char MyObfPass03::ID = 0;
static RegisterPass<MyObfPass03> X("myobfpass03", "Obfuscates zeroes tutorial 03");
```
然后更新一下之前的file.c文件，或者新建一个file03.c文件如下：

```cpp
#include <stdio.h>

int foo(){return 1;}

int main() {
    int a = 2;
    puts("Hello world");
    a *= 3;

    return 0;
}
```
我们还是来检视一下它所对应的IR文件。

```
; Function Attrs: noinline nounwind optnone ssp uwtable
define i32 @foo() #0 {
entry:
  ret i32 1
}

; Function Attrs: noinline nounwind optnone ssp uwtable
define i32 @main() #0 {
entry:
  %retval = alloca i32, align 4
  %a = alloca i32, align 4
  store i32 0, i32* %retval, align 4
  store i32 2, i32* %a, align 4
  %call = call i32 @puts(i8* getelementptr inbounds ([12 x i8], [12 x i8]* @.str, i32 0, i32 0))
  %0 = load i32, i32* %a, align 4
  %mul = mul nsw i32 %0, 3
  store i32 %mul, i32* %a, align 4
  ret i32 0
}
```

如果你编译并运行我们的新pass来测试你的代码，你会发现 the pass finds 3 integers to register corresponding to %call, %0 and %mul。实验结果如下：

![](https://img-blog.csdn.net/20180603130244725)


需要注意的是：A variable declaration in your code returns a pointer in the IR not an instance of the type of the variable. This is because Clang translates variable declarations into variables allocated on the stack (through the alloca instruction). A later pass (Mem2reg) takes care of putting them in registers when possible.




##### 三、完成最终的Obfuscation

我们将创建一个新的方法replaceZero，但考虑到代码的篇幅，我们还是一步一步来完成。同样，我们需要在MyObfPass03.cpp的基础上进行修改，第一步还是添加必要的头文件，假设最终的pass文件名为ObfuscateZero.cpp，下面给出的是给文件的最初一段：

```cpp
#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Constants.h"
#include "llvm/Support/raw_ostream.h"

#include "llvm/Transforms/IPO/PassManagerBuilder.h"

#include "llvm/IR/Instructions.h"
#include "llvm/IR/IRBuilder.h"

#include <vector>
#include <random>

using namespace llvm;

namespace {
  using prime_type = uint32_t;

  static const prime_type Prime_array[] = {
     2 ,    3 ,    5 ,    7,     11,     13,     17,     19,     23,     29,
     31,    37,    41,    43,    47,     53,     59,     61,     67,     71,
     73,    79,    83,    89,    97,    101,    103,    107,    109,    113,
    127,   131,   137,   139,   149,    151,    157,    163,    167,    173,
    179,   181,   191,   193,   197,    199,    211,    223,    227,    229,
    233,   239,   241,   251,   257,    263,    269,    271,    277,    281,
    283,   293,   307,   311,   313,    317,    331,    337,    347,    349,
    353,   359,   367,   373,   379,    383,    389,    397,    401,    409,
    419,   421,   431,   433,   439,    443,    449,    457,    461,    463,
    467,   479,   487,   491,   499,    503,    509,    521,    523,    541,
    547,   557,   563,   569,   571,    577,    587,    593,    599,    601,
    607,   613,   617,   619,   631,    641,    643,    647,    653,    659,
    661,   673,   677,   683,   691,    701,    709,    719,    727,    733,
    739,   743,   751,   757,   761,    769,    773,    787,    797,    809,
    811,   821,   823,   827,   829,    839,    853,    857,    859,    863,
    877,   881,   883,   887,   907,    911,    919,    929,    937,    941,
    947,   953,   967,   971,   977,    983,    991,    997};
```

可以看到代码中定义了一个prime_type的自定义类型，本质上它是一个整数型。因为我们的扰乱策略中需要用到质数，因此使用数组Prime_array[]来存储1000以内的全部质数。

下面这个私有函数用于返回一个质数：

```cpp
// Return a random prime number not equal to DifferentFrom
  // If an error occurs returns 0
  prime_type getPrime(prime_type DifferentFrom = 0) {
      static std::uniform_int_distribution<prime_type> Rand(0, std::extent<decltype(Prime_array)>::value);
      size_t MaxLoop = 10;
      prime_type Prime;

      do {
            Prime = Prime_array[Rand(Generator)];
      } while(Prime == DifferentFrom && --MaxLoop);

      if(!MaxLoop) {
          return 0;
      }

      return Prime;
  }
```
我们的 replaceZero 方法 will replace the null operand(s) of an instruction and return a pointer to the new operand(s) (or nullptr if a problem occurs). This gives us the following signature:

```cpp
Value *replaceZero(Instruction &Inst, Value *VReplace) {
    // Replacing 0 by:
    // prime1 * ((x | any1)**2) != prime2 * ((y | any2)**2)
    // with prime1 != prime2 and any1 != 0 and any2 != 0
```
为了产生我们的新公式，我们需要用到两个不同的质数，于是在replaceZero方法中继续编码：

```cpp
prime_type p1 = getPrime(),
               p2 = getPrime(p1);

    if(p2 == 0 || p1 == 0)
        return nullptr;
```
LLVM 的 bytecode （或IR） is strongly typed，因此我需要对数据类型做一点额外的处理。这里，比较重要的类型是那些我们将要替换的操作数之类型，以及那些我们会进行obfuscation操作的表达式中所涉及的类型。对于中间操作，我们将使用刚刚声明的 prime_type 类型（本质上它就是uint32_t）。但我们还是要对类型转换格外小心，以免发生类型溢出（这一点后面还会演示具体的应对措施）。

```cpp
Type *ReplacedType = VReplace->getType(),
          *IntermediaryType = IntegerType::get(Inst.getParent()->getContext(),
                                              sizeof(prime_type) * 8);
```
接下来，我们需要随机挑选两个reachable变量（possibly twice the same）and two random strictly positive integers. For the variables we are going to randomly pick values in IntegerVect.

```cpp
// Abort the obfuscation if we have encontered no integers so far
if (IntegerVect.empty()) {
  return nullptr;
}

// Random distribution to pick variables from IntegerVect
std::uniform_int_distribution<size_t> Rand(0, IntegerVect.size() - 1);
// Random distribution to pick Any1 and Any2 from [1, 10]
std::uniform_int_distribution<size_t> RandAny(1, 10);

// Indexes chosen for x and y
size_t Index1 = Rand(Generator), Index2 = Rand(Generator);
```

如果我们使得新产生指令中的任何一个里的某个中间类型溢出，都可能会导致obfuscating公式中的不等式关系发生改变。也就是说，本来我们想用一个等价于0的值来替换0，现在可能却用了其他什么值来做了替换。因此，这会导致被修改后的代码执行出了同预期不一样的结果，这是我们需要竭力避免的。 To prevent overflowing we have set the maximum for Any1 and Any2 to 10, but this is not enough. We need to make sure that x and y are not too big. The trick is that we have no information on their value at compile time. The solution we chose is to apply a bitmask to x and y in order to obtain a variable of which we know the max value.

```cpp
// Creating the LLVM objects representing literals
Constant *any1 = ConstantInt::get(IntermediaryType, 1 + RandAny(Generator)),
         *any2 = ConstantInt::get(IntermediaryType, 1 + RandAny(Generator)),
         *prime1 = ConstantInt::get(IntermediaryType, p1),
         *prime2 = ConstantInt::get(IntermediaryType, p2),
         // Bitmask to prevent overflow
         *OverflowMask = ConstantInt::get(IntermediaryType, 0x00000007);
```
Now that we have everything we need we will create our new instructions. To insert new instructions before a specific instruction we use an IRBuilder. This object will create instructions and insert them before the instruction given to its constructor. And we need to insert our new instructions before the instruction we are working on. That's why replaceZero takes an Instruction as parameter. We will forward it to the builder.

```cpp
IRBuilder<> Builder(&Inst);

    // lhs
    // To avoid overflow
    Value *LhsCast =
        Builder.CreateZExtOrTrunc(IntegerVect.at(Index1), IntermediaryType);
    registerInteger(*LhsCast);
    Value *LhsAnd = Builder.CreateAnd(LhsCast, OverflowMask);
    registerInteger(*LhsAnd);
    Value *LhsOr = Builder.CreateOr(LhsAnd, any1);
    registerInteger(*LhsOr);
    Value *LhsSquare = Builder.CreateMul(LhsOr, LhsOr);
    registerInteger(*LhsSquare);
    Value *LhsTot = Builder.CreateMul(LhsSquare, prime1);
    registerInteger(*LhsTot);

    // rhs
    Value *RhsCast =
        Builder.CreateZExtOrTrunc(IntegerVect.at(Index2), IntermediaryType);
    registerInteger(*RhsCast);
    Value *RhsAnd = Builder.CreateAnd(RhsCast, OverflowMask);
    registerInteger(*RhsAnd);
    Value *RhsOr = Builder.CreateOr(RhsAnd, any2);
    registerInteger(*RhsOr);
    Value *RhsSquare = Builder.CreateMul(RhsOr, RhsOr);
    registerInteger(*RhsSquare);
    Value *RhsTot = Builder.CreateMul(RhsSquare, prime2);
    registerInteger(*RhsTot);

    // comp
    Value *comp =
        Builder.CreateICmp(CmpInst::Predicate::ICMP_EQ, LhsTot, RhsTot);
    registerInteger(*comp);
    Value *castComp = Builder.CreateZExt(comp, ReplacedType);
    registerInteger(*castComp);

    return castComp;
  }
```

至此，工作已经完成了八九成。最后，我们需要在主循环体中调用我们的新函数，并显式地替换符合条件的操作数。

```
bool runOnBasicBlock(BasicBlock &BB) override {
  IntegerVect.clear();
  bool modified = false;

  for (typename BasicBlock::iterator I = BB.getFirstInsertionPt(),
                                     end = BB.end();
       I != end; ++I) {
    Instruction &Inst = *I;
      for (size_t i = 0; i < Inst.getNumOperands(); ++i) {
        if (Constant *C = isValidCandidateOperand(Inst.getOperand(i))) {
          if (Value *New_val = replaceZero(Inst, C)) {
            Inst.setOperand(i, New_val);
            modified = true;
          } else {
            // If sthg wrong happens during the replacement,
            // almost certainly because IntegerVect is empty
            errs() << "MyPass: could not rand pick a variable for replacement\n";
          }
        }
      }
    registerInteger(Inst);
  }
  return modified;
}
```
完整的代码可以从参考文献【4】中获取，限于篇幅，这里不再详细列出。




不妨来试试看我们的的新pass效果如何。此时，可以使用类似下面的参数来调用新建的pass，并将输入的IR文件转换成扰乱处理之后的IR文件。

-load (路径)/LLVMObfuscateZero.dylib -obfuscatezero (路径)/file03.ll -f -S -o (路径)/file03_obf.ll


由此得到的file03_obf.ll文件如下：


```
; Function Attrs: noinline nounwind optnone ssp uwtable
define i32 @main() #0 {
entry:
  %retval = alloca i32, align 4
  %a = alloca i32, align 4
  store i32 0, i32* %retval, align 4
  store i32 2, i32* %a, align 4
  %call = call i32 @puts(i8* getelementptr inbounds ([12 x i8], [12 x i8]* @.str, i32 0, i32 0))
  %0 = load i32, i32* %a, align 4
  %mul = mul nsw i32 %0, 3
  store i32 %mul, i32* %a, align 4
  %1 = and i32 %call, 7
  %2 = or i32 %1, 4
  %3 = mul i32 %2, %2
  %4 = mul i32 %3, 691
  %5 = and i32 %call, 7
  %6 = or i32 %5, 4
  %7 = mul i32 %6, %6
  %8 = mul i32 %7, 149
  %9 = icmp eq i32 %4, %8
  %10 = zext i1 %9 to i32
  ret i32 %10
}
```

对比没有加obfuscation的file03.ll文件，易见从%1到%10之间的部分其实就是对原IR文件中return 0 指令的等价替换。也就是说我们的扰乱方法已经被成功执行了。这里略去了文献【1】中末尾的讨论部分，但这部分内容仍然值得一读。






【本系列文章目录】
- 在LLVM中编写Pass的详细教程（1）：一个Hello World的Pass
- 在LLVM中编写Pass的详细教程（2）：遍历一个函数中的Basic Blocks
- 在LLVM中编写Pass的详细教程（3）：对程序中的OpCode进行计数
- 在LLVM中编写Pass的详细教程（4）：def use 与 use def
- 在LLVM中编写Backend Pass的详细教程
- 在LLVM中使用Gold Linker作为链接器


【参考文献】
- https://blog.quarkslab.com/turning-regular-code-into-atrocities-with-llvm.html

- http://www.zhupite.com/posts/windows-llvm-use-ollvm-pass.html
- http://llvm.org/docs/CommandGuide/opt.html

- **网盘链接**





**本文完**




