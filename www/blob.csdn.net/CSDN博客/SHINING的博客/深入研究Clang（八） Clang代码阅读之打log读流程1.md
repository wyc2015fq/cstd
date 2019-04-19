# 深入研究Clang（八） Clang代码阅读之打log读流程1 - SHINING的博客 - CSDN博客
2016年12月16日 16:30:48[snsn1984](https://me.csdn.net/snsn1984)阅读数：2027
所属专栏：[深入研究Clang](https://blog.csdn.net/column/details/hacking-clang.html)
这个过程简单的分为几步。
第一步：写一个简单的小程序，hello.c。内容如下：
#include<stdio.h>
int main() {
  printf("Hello world!\n");
}
第二步：找出如何在LLVM里面输出信息，最后选择采用llvm::errs()，等于采用了LLVM的错误机制。包括llvm::errs()所需的头文件。具体内容如下：
//shining add begin
#include "llvm/Support/raw_ostream.h"
//shining add end
  //shining add begin
  llvm::errs() << "clang/tools/driver/driver.cpp/main()/shining_add" << '\n';
  //shining add end
注：为了标明代码是我自己添加的，所以在添加的log信息前后都加了注释。
第三步：采用clang3.9.0代码，在我知道的几个点随意添加了几个log信息，信息内容如上一步所示，主要是标明插入log的函数的相关信息，都放在了函数的第一条语句处。编译该clang之后，使用命令clang hello.c -o hello编译hello.c，可以得到如下输出信息：
clang/tools/driver/driver.cpp/main()/shining_add
clang/tools/driver/driver.cpp/main()/shining_add
clang/tools/driver/cc1_main.cpp/cc1_main()/shining_add
clang/lib/CodeGen/CodeGenAction.cpp/CodeGenAction::ExecuteAction()/shining_add
clang/lib/Sema/Sema.cpp/Sema::Sema()/shining_add
clang/lib/Parse/ParseAST.cpp/ParseAST(Sema)/shining_add
clang/lib/Lex/Lexer.cpp/Lexer::InitLexer()/shining_add
clang/lib/Lex/Lexer.cpp/Lexer::InitLexer()/shining_add
clang/lib/Lex/Lexer.cpp/Lexer::InitLexer()/shining_add
clang/lib/Lex/Lexer.cpp/Lexer::InitLexer()/shining_add
clang/lib/Lex/Lexer.cpp/Lexer::InitLexer()/shining_add
clang/lib/Lex/Lexer.cpp/Lexer::InitLexer()/shining_add
clang/lib/Lex/Lexer.cpp/Lexer::InitLexer()/shining_add
clang/lib/Lex/Lexer.cpp/Lexer::InitLexer()/shining_add
clang/lib/Lex/Lexer.cpp/Lexer::InitLexer()/shining_add
clang/lib/Lex/Lexer.cpp/Lexer::InitLexer()/shining_add
clang/lib/Lex/Lexer.cpp/Lexer::InitLexer()/shining_add
clang/lib/Lex/Lexer.cpp/Lexer::InitLexer()/shining_add
clang/lib/Lex/Lexer.cpp/Lexer::InitLexer()/shining_add
clang/lib/Lex/Lexer.cpp/Lexer::InitLexer()/shining_add
clang/lib/Lex/Lexer.cpp/Lexer::InitLexer()/shining_add
clang/lib/Lex/Lexer.cpp/Lexer::InitLexer()/shining_add
clang/lib/Lex/Lexer.cpp/Lexer::InitLexer()/shining_add
clang/lib/Lex/Lexer.cpp/Lexer::InitLexer()/shining_add
clang/lib/Lex/Lexer.cpp/Lexer::InitLexer()/shining_add
clang/lib/Lex/Lexer.cpp/Lexer::InitLexer()/shining_add
基本上我所插入的log信息都打印出来了，这里可以看到clang编译一个简单的程序，所经历的大的点。后续会在此基础之上，再分块进行分析代码和流程。开这么个头，方便自己，也为想读clang源码的朋友们提供一个思路。这个思路简单易行，根据我提供的几个点，也可以继续深入去分析自己感兴趣的地方。
代码和修改后的代码，我也建了一个代码库，地址：[https://github.com/shining1984/clang_code_comment](https://github.com/shining1984/clang_code_comment)
