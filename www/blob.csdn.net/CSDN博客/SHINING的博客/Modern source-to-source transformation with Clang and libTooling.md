# Modern source-to-source transformation with Clang and libTooling - SHINING的博客 - CSDN博客
2014年05月07日 11:43:13[snsn1984](https://me.csdn.net/snsn1984)阅读数：1952
## [Modern source-to-source transformation with Clang and libTooling](http://eli.thegreenplace.net/2014/05/01/modern-source-to-source-transformation-with-clang-and-libtooling/)
May 1st, 2014 at 7:08 pm
I couple of years ago I published a blog post named[Basic
 source-to-source transformation with Clang](http://eli.thegreenplace.net/2012/06/08/basic-source-to-source-transformation-with-clang/), in which I presented a small but complete sample of a tool that performs C++ source-to-source rewriting using Clang. That post was (and still is) quite popular, but also severely out of date; it’s time to fix
 that.
Clang is moving forward very fast. Today, to develop tools similar to the one presented in that post one should use the "tooling" library (libTooling). I hinted at its existence even in that post, but it was still at its infancy then. Now libTooling
 is a fairly mature layer through which it’s possible to interact with Clang. Moreover, there are reasons to be optimistic about its stability, since tools using it pop up all the time, both in and outside the LLVM/Clang source tree[[1]](http://eli.thegreenplace.net/2014/05/01/modern-source-to-source-transformation-with-clang-and-libtooling/#id4).
So I rewrote the tool presented in the[previous
 post](http://eli.thegreenplace.net/2012/06/08/basic-source-to-source-transformation-with-clang/)using libTooling. The tool is very basic, but it demonstrates a lot of the machinery. It takes C++ code like:
voidfoo(int* a, int *b) {
  if (a[0] > 1) {
    b[0] = 2;
  }
}
And adds some descriptive comments:
// Begin function foo returning voidvoidfoo(int* a, int *b) {
  if (a[0] > 1) // the 'if' part
  {
    b[0] = 2;
  }
}
// End function foo
Non-trivial code samples pasted into blog posts tend to go stale, so this time I’ll just directly link to my[Github
 repository for LLVM and Clang samples](https://github.com/eliben/llvm-clang-samples), which I’m keeping functional fairly close to the LLVM trunk (as well as with released versions of LLVM, in branches). The direct path to the sample described here is[src_clang/tooling_sample.cpp](https://github.com/eliben/llvm-clang-samples/blob/master/src_clang/tooling_sample.cpp).
The good thing about this repository is that it comes complete with a makefile which builds the tools & samples*out of tree*, meaning that they can reside anywhere as long as the paths to LLVM & Clang headers and libraries are
 set up correctly. If you’ve ever tried to build an out-of-tree tool using LLVM or Clang as a library, you will not underestimate the utility of having this makefile available and continuously updated![;-)](http://eli.thegreenplace.net/wp-includes/images/smilies/icon_wink.gif)
Anyway, it’s time to explain the code.
### Implementing the transformation itself
The low-level details of the sample haven’t changed much. An implementation of theASTConsumerinterface
 chooses which AST parsing events to handle.MyASTConsumerimplementsHandleTopLevelDecl,
 so it will be invoked on each top-level declaration in the AST. It uses an implementation ofRecursiveASTVisitorto
 do the actual traversal over declaration AST nodes.MyASTVisitorimplementsVisitFunctionDeclto
 be able to emit comments before and after function definitions, andVisitStmtto
 detectifstatements and analyze them.
Finally, aRewriteris used
 to manage the textual changes to the source code. This is how Clang-based source to source transformations work – analysis of the AST lets us find out where to apply changes, and aRewriteris
 used to actually apply those changes[[2]](http://eli.thegreenplace.net/2014/05/01/modern-source-to-source-transformation-with-clang-and-libtooling/#id5).
 The link between the two is the very accurate source location information managed by the AST for each node.
### Becoming a tool via libTooling
What’s really new about this sample is how it turns this low-level transformation logic into an actual tool. With libTooling, we no longer need to laboriously set up an instance of the compiler front-end manually. TheClangToolclass
 does it all for us, whileCommonOptionsParsersimplifies
 the command-line interface of the tool. All we need is to implement aFrontendAction–
 a very central abstraction within Clang for entities that produce stuff from the parsed AST[[3]](http://eli.thegreenplace.net/2014/05/01/modern-source-to-source-transformation-with-clang-and-libtooling/#id6).
MyFrontendActionis very simple. All it
 does is provide our custom AST consumer inCreateASTConsumerand
 sets up theRewriter. Note thatClangToolwill
 create a newMyFrontendActioninstance per file, so
 it’s correct to have a newRewriterin each instance.
 This is enabled by adding another layer of abstraction – theFrontendActionFactory.
 The factory pattern here is used to decouple the process of creating objects implementingFrontendActionfrom
 the concreteFrontendActionsubclasses used by our
 tool. It also lets us customize the creation process to a greater extent – for example passing additional information into eachMyFrontendActionobject.
 In our sample, advanced features aren’t really needed so we use the conveniencenewFrontendActionFactorytemplate
 which creates a basic factory behind the scenes.
### Invoking the tool
It’s important to spend a few minutes to discuss how to invoke the tool. libTooling has the concept of a*compilation database*, which tells tools about the compiler options used to build the sources under consideration.
 Describing compilation databases in detail is out of scope here, and I may devote a separate post to it in the future, but in the meantime it helps knowing that a compilation database isn’t strictly required to run tools.
The libTooling command-line parser (CommonOptionsParser)
 supports providing compiler flags on the command line, following the special flag--.
 Think of it as a simplified, ad-hoc version of a compilation database.
If you clone and build the[llvm-clang-samples](https://github.com/eliben/llvm-clang-samples)repository,
 you can invoke the sample discussed here as follows:
$ build/tooling_sample inputs/cfunc_with_if.c --
Note the--at
 the end: this invocation means there are no special flags the tool needs to know about while analyzing the file.
### What next
This post provides an introduction into building a source-to-source transformation tool based on Clang’s libTooling. It should provide a foundation for building custom tools, and nicely complements the existing Clang libTooling documentation
 and examples. Check them out, by the way. Clang actually has[decent documentation](http://clang.llvm.org/docs/index.html)on
 using libTooling these days.
![http://eli.thegreenplace.net/wp-content/uploads/hline.jpg](http://eli.thegreenplace.net/wp-content/uploads/hline.jpg)
|[[1]](http://eli.thegreenplace.net/2014/05/01/modern-source-to-source-transformation-with-clang-and-libtooling/#id1)|The[Euro LLVM 2014 conference](http://llvm.org/devmtg/2014-04/), held just a couple of weeks ago, has a talk and a tutorial about writing Clang tools based on libTooling.|
|[[2]](http://eli.thegreenplace.net/2014/05/01/modern-source-to-source-transformation-with-clang-and-libtooling/#id2)|Note that the code presented here uses low-level APIs. libTooling has some more convenient higher-level APIs which I hope to talk about in future posts.|
|[[3]](http://eli.thegreenplace.net/2014/05/01/modern-source-to-source-transformation-with-clang-and-libtooling/#id3)|AFrontendActionis used throughout the main Clang compiler as well. For example, the Clang code generator – the component responsible for emitting LLVM IR from the AST – defines a subclass namedCodeGenActionwhich is its main entry point.|
Related posts:
- [Basic
 source-to-source transformation with Clang](http://eli.thegreenplace.net/2012/06/08/basic-source-to-source-transformation-with-clang/)
- [Building
 gcc 4.8 from source on Ubunu 12.04](http://eli.thegreenplace.net/2014/01/16/building-gcc-4-8-from-source-on-ubunu-12-04/)
- [Parsing
 C++ in Python with Clang](http://eli.thegreenplace.net/2011/07/03/parsing-c-in-python-with-clang/)
- [How
 Clang handles the type / variable name ambiguity of C/C++](http://eli.thegreenplace.net/2012/07/05/how-clang-handles-the-type-variable-name-ambiguity-of-cc/)
- [Dumping
 a C++ object’s memory layout with Clang](http://eli.thegreenplace.net/2012/12/17/dumping-a-c-objects-memory-layout-with-clang/)
