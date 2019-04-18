# LLVM与C++代码的相互调用（全注释） - Likes的博客 - CSDN博客
2018年12月12日 19:08:40[Lailikes](https://me.csdn.net/songchuwang1868)阅读数：143
# 一、在C++中调用LLVM编写的IR函数
```cpp
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/ExecutionEngine/JIT.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/CodeGen/MachineCodeInfo.h"
#include "llvm/Analysis/Verifier.h"
#include "llvm/ADT/ArrayRef.h"
#include "llvm/IR/DerivedTypes.h"
#include <iostream>
/*
      务必使用llvm-3.3
*/
using namespace llvm;
using namespace std;
int main()
{
    int x = 9;
    // 获取LLVM上下文
    LLVMContext &context = llvm::getGlobalContext();
    //获取LLVM Module，Module类似于一个完整的.cc文件
    Module *module = new Module("test", context);
    //获取一个函数，如果这个函数已经声明，获取相应的函数指针，如果没有，声明
    Function *f = cast<Function>(module->getOrInsertFunction("foo", Type::getInt32Ty(context),
                                                             Type::getInt32Ty(context), NULL));
    //创建一个代码块，这个代码块是之前声明的函数foo的。代码块就是C++中的 {}
    BasicBlock *BB = BasicBlock::Create(context, "entryBar", f);
    //创建LLVM Builder，Builder的作用就是辅助完成IR代码，这个builder用于在代码块BB中完成IR
    IRBuilder<> builder(BB);
    //下面是foo中的主要逻辑：int ret=x;return ret;
    Value *ret = builder.getInt32(x);
    builder.CreateRet(ret);
    //这行代码的作用是引入初始化native，JIT会将IR编译成当前主机可以运行的二进制代码，而native中包含了当前主机的操作系统信息
    InitializeNativeTarget();
    //创建执行JIT引擎，并将IR代码放入，也就是放入module（因为IR代码都在其中）
    ExecutionEngine *ee = EngineBuilder(module).setEngineKind(EngineKind::JIT).create();
    //从执行引擎中获取相应的函数指针，JIT即时编译器后我们之前写的IR函数可以通过函数指针获取
    void *barAddr = ee->getPointerToFunction(f);
    //将函数指针进行强转
    typedef int32_t (*FuncType)(int32_t);
    FuncType barFunc = (FuncType)barAddr;
    //正式调用经由JIT编译后的IR函数
    int64_t n = 9;
    cout << barFunc(n) << endl;
    return 0;
}
```
# 二、在LLVM编写的IR函数中调用C++代码
```cpp
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/ExecutionEngine/JIT.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/CodeGen/MachineCodeInfo.h"
#include "llvm/Analysis/Verifier.h"
#include <iostream>
using namespace llvm;
/*
    务必使用llvm-3.3
*/
int foo(int x)
{
    return 2 * x;
}
int value = 10;
int main()
{
    LLVMContext &context = llvm::getGlobalContext();
    Module *module = new Module("test", context);
    //在IR代码中声明一个全局变量
    GlobalVariable *v = cast<GlobalVariable>(module->getOrInsertGlobal("value", Type::getInt32Ty(context)));
    //在IR中声明一个函数，注意我们并不会在IR中定义foo，我们会将这个IR中声明的函数映射到C++代码中的函数
    Function *f = cast<Function>(module->getOrInsertFunction("foo", Type::getInt32Ty(context),
                                                             Type::getInt32Ty(context), NULL));
    //在IR中声明一个函数bar，我们会用IR定义这个函数
    Function *bar = cast<Function>(module->getOrInsertFunction("bar", Type::getInt32Ty(context), NULL));
    //创建函数bar的代码块
    BasicBlock *entry = BasicBlock::Create(context, "entry", bar);
    IRBuilder<> builder(entry);
    //用一个局部变量获取全局变量v的值
    Value *v_IR = builder.CreateLoad(v);
    //调用函数foo
    Value *ret = builder.CreateCall(f, v_IR);
    //返回值
    builder.CreateRet(ret);
    InitializeNativeTarget();
    ExecutionEngine *ee = EngineBuilder(module).setEngineKind(EngineKind::JIT).create();
    //将外部的C++代码中的全局变量映射到IR代码中，IR代码中只有声明
    ee->addGlobalMapping(v, &value);
    //将外部的C++代码中的全局函数映射到IR代码中，IR代码中只有声明
    ee->addGlobalMapping(f, (void *)foo);
    void *barAddr = ee->getPointerToFunction(bar);
    typedef int (*FuncType)();
    FuncType barFunc = (FuncType)barAddr;
    std::cout << barFunc() << std::endl;
    return 0;
}
```
