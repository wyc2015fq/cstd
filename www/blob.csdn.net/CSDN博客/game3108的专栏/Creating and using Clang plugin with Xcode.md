# Creating and using Clang plugin with Xcode - game3108的专栏 - CSDN博客
2017年08月09日 15:06:37[game3108](https://me.csdn.net/game3108)阅读数：367
个人分类：[iOS																[转载](https://blog.csdn.net/game3108/article/category/2926393)](https://blog.csdn.net/game3108/article/category/2844789)
链接：https://railsware.com/blog/2014/02/28/creation-and-using-clang-plugin-with-xcode/
如何写一个clang plugin，很不错的文章，记录一下。
#### This tutorial describes how to create Clang plugin and covers the next things:
- environment setup
- basic plugin setup
- setup Xcode project for plugin development
- warnings reporting
- errors reporting
- Xcode integration
- interactive hints for errors/warnings riddance
### tl;dr
Clang Rocks!!!
You can find the plugin [here](https://github.com/AlexDenisov/ToyClangPlugin).
### Intro
While working on [BloodMagic](https://github.com/railsware/BloodMagic), I realised that it’d be nice to have a tool for checking semantic errors related to BM usage. For example:
 in the interface property marked as `lazy`, but not defined as `@dynamic` in the implementation, or property marked as `lazy`,
 but class container doesn’t support injections.
I concluded that I need to work with [](http://en.wikipedia.org/wiki/Abstract_syntax_tree)and I need a full-featured parser.
I’ve tried different approaches: [flex](http://en.wikipedia.org/wiki/Flex_lexical_analyser)+[bison](http://en.wikipedia.org/wiki/GNU_bison), [libclang](http://clang.llvm.org/doxygen/group__CINDEX.html),
 but ultimately I decided to write a Clang plugin.
Just for testing purposes I’ve started a simple plugin with the following goals:
- use Xcode for development
- integrate ready plugin into Xcode and use it workaday
- plugin should report warnings, errors and propose interactive hints for fixes (via Xcode UI)
Features of the test plugin:
- report warning in case of class’ name starts with lowercase letter
- report error in case of class’ name contains underscore `_`
- propose hints for fixes
#### Environment setup
For plugin development we need llvm/clang, built from source
```
```
|12345|cd/optsudo mkdirllvmsudo chown`whoami`llvmcdllvmexport LLVM_HOME=`pwd`|
Current `clang` version on my system – 3.3.1, so let’s build respective version:
``
|123456789|gitclone-brelease_33https://github.com/llvm-mirror/llvm.gitllvmgitclone-brelease_33https://github.com/llvm-mirror/clang.gitllvm/tools/clanggitclone-brelease_33https://github.com/llvm-mirror/clang-tools-extra.gitllvm/tools/clang/tools/extragitclone-brelease_33https://github.com/llvm-mirror/compiler-rt.gitllvm/projects/compiler-rtmkdirllvm_buildcdllvm_buildcmake../llvm-DCMAKE_BUILD_TYPE:STRING=Releasemake-j`sysctl-nhw.logicalcpu`|
#### Basic plugin setup
Create directory for plugin
```
```
|12|cd$LLVM_HOMEmkdirtoy_clang_plugin;cdtoy_clang_plugin|
Our plugin based on example from Clang repo and here is it’s structure:
``
|123|ToyClangPlugin.exportsCMakeLists.txtToyClangPlugin.cpp|
We’ll use one source file for simplification
ToyClangPlugin.cpp

C++
```
```
|12345678910111213141516171819202122232425262728293031|#include "clang/Frontend/FrontendPluginRegistry.h"#include "clang/AST/AST.h"#include "clang/AST/ASTConsumer.h"#include "clang/Frontend/CompilerInstance.h"usingnamespaceclang;namespace{classToyConsumer:publicASTConsumer{};classToyASTAction:publicPluginASTAction{public:virtualclang::ASTConsumer*CreateASTConsumer(CompilerInstance&Compiler,llvm::StringRef InFile){returnnewToyConsumer;}boolParseArgs(constCompilerInstance&CI,conststd::vector<std::string>&args){returntrue;}};}staticclang::FrontendPluginRegistry::Add<ToyASTAction>X("ToyClangPlugin","Toy Clang Plugin");|
CMakeLists.txt

``
|123456789101112131415161718192021222324252627282930313233343536373839404142434445464748495051525354555657585960616263646566676869707172737475767778798081828384858687888990919293949596979899100101102103104105106107108109110111112113114115116117118119120121122123124|cmake_minimum_required(VERSION2.6)project(ToyClangPlugin)set(CMAKE_RUNTIME_OUTPUT_DIRECTORY${CMAKE_BINARY_DIR}/bin)set(CMAKE_LIBRARY_OUTPUT_DIRECTORY${CMAKE_BINARY_DIR}/lib)set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY${CMAKE_BINARY_DIR}/lib)set(LLVM_HOME/opt/llvm)set(LLVM_SRC_DIR${LLVM_HOME}/llvm)set(CLANG_SRC_DIR${LLVM_HOME}/llvm/tools/clang)set(LLVM_BUILD_DIR${LLVM_HOME}/llvm_build)set(CLANG_BUILD_DIR${LLVM_HOME}/llvm_build/tools/clang)add_definitions(-D__STDC_LIMIT_MACROS-D__STDC_CONSTANT_MACROS)add_definitions(-D_GNU_SOURCE-DHAVE_CLANG_CONFIG_H)set(CMAKE_CXX_COMPILER"${LLVM_BUILD_DIR}/bin/clang++")set(CMAKE_CC_COMPILER"${LLVM_BUILD_DIR}/bin/clang")set(CMAKE_CXX_FLAGS"${CMAKE_CXX_FLAGS}  -fPIC  -fno-common  -Woverloaded-virtual  -Wcast-qual  -fno-strict-aliasing  -pedantic  -Wno-long-long  -Wall  -Wno-unused-parameter  -Wwrite-strings  -fno-exceptions  -fno-rtti")set(CMAKE_MODULE_LINKER_FLAGS"-Wl,-flat_namespace -Wl,-undefined -Wl,suppress")set(LLVM_LIBSLLVMJITLLVMX86CodeGenLLVMX86AsmParserLLVMX86DisassemblerLLVMExecutionEngineLLVMAsmPrinterLLVMSelectionDAGLLVMX86AsmPrinterLLVMX86InfoLLVMMCParserLLVMCodeGenLLVMX86UtilsLLVMScalarOptsLLVMInstCombineLLVMTransformUtilsLLVMipaLLVMAnalysisLLVMTargetLLVMCoreLLVMMCLLVMSupportLLVMBitReaderLLVMOption)macro(add_clang_pluginname)set(srcs${ARGN})include_directories("${LLVM_SRC_DIR}/include""${CLANG_SRC_DIR}/include""${LLVM_BUILD_DIR}/include""${CLANG_BUILD_DIR}/include")link_directories("${LLVM_BUILD_DIR}/lib")add_library(${name}SHARED${srcs})if(SYMBOL_FILE)set_target_properties(${name}PROPERTIESLINK_FlAGS"-exported_symbols_list ${SYMBOL_FILE}")endif()foreach(clang_lib${CLANG_LIBS})target_link_libraries(${name}${clang_lib})endforeach()foreach(llvm_lib${LLVM_LIBS})target_link_libraries(${name}${llvm_lib})endforeach()foreach(user_lib${USER_LIBS})target_link_libraries(${name}${user_lib})endforeach()endmacro(add_clang_plugin)set(SYMBOL_FILEToyClangPlugin.exports)set(CLANG_LIBSclangclangFrontendclangASTclangAnalysisclangBasicclangCodeGenclangDriverclangFrontendToolclangLexclangParseclangSemaclangEditclangSerializationclangStaticAnalyzerCheckersclangStaticAnalyzerCoreclangStaticAnalyzerFrontend)set(USER_LIBSpthreadcurses)add_clang_plugin(ToyClangPluginToyClangPlugin.cpp)set_target_properties(ToyClangPluginPROPERTIESLINKER_LANGUAGECXXPREFIX"")|
ToyClangPlugin.exports

C++
```
```
|1|__ZN4llvm8Registry*|
Now we’re able to generate Xcode-project, based on `CMakeLists.txt`
``
|123|mkdirbuild;cdbuildcmake-GXcode..openToyClangPlugin.xcodeproj|
Run `ALL_BUILD` target, and you’ll see dynamic library at `lib/Debug/ToyCLangPlugin.dylib`.
#### RecursiveASTVisitor
Clang AST module provides [RecursiveASTVisitor](http://clang.llvm.org/doxygen/classclang_1_1RecursiveASTVisitor.html), which allows us to traverse via AST.
We just need to create a subclass and implement interesting methods.
For test we’ll print all the found class names.
ToyClangPlugin.cpp

C++
```
```
|12345678910111213141516171819|classToyClassVisitor:publicRecursiveASTVisitor<ToyClassVisitor>{public:boolVisitObjCInterfaceDecl(ObjCInterfaceDecl*declaration){printf("ObjClass: %s\n",declaration->getNameAsString().c_str());returntrue;}};classToyConsumer:publicASTConsumer{public:voidHandleTranslationUnit(ASTContext&context){visitor.TraverseDecl(context.getTranslationUnitDecl());}private:ToyClassVisitor visitor;};|
Let’s create test source file and check how plugin works.
test.m

Objective-C
``
|123456789|#import <Foundation/Foundation.h>@interfaceToyObject: NSObject@end@implementationToyObject@end|
Rebuild plugin and run
```
```
|12345|/opt/llvm/toy_clang_plugin/build$$LLVM_HOME/llvm_build/bin/clang../test.m\-Xclang-load\-Xclang lib/Debug/ToyClangPlugin.dylib\-Xclang-plugin\-Xclang ToyClangPlugin|
We’ll see a huge list of classes.
#### Report warnings
Let’s report warning in case if class’ name starts with lowercase letter
Add `ASTContext` to `ToyClassVisitor`
ToyClangPlugin.cpp

C++
``
|123456789101112131415161718|classToyClassVisitor:publicRecursiveASTVisitor<ToyClassVisitor>{private:ASTContext*context;public:voidsetContext(ASTContext&context){this->context=&context;}// ...};// ...voidHandleTranslationUnit(ASTContext&context){visitor.setContext(context);visitor.TraverseDecl(context.getTranslationUnitDecl());}// ...|
Add check
ToyClangPlugin.cpp

C++
```
```
|1234567891011121314151617|boolVisitObjCInterfaceDecl(ObjCInterfaceDecl*declaration){checkForLowercasedName(declaration);returntrue;}//  ...voidcheckForLowercasedName(ObjCInterfaceDecl*declaration){StringRef name=declaration->getName();charc=name[0];if(isLowercase(c)){DiagnosticsEngine&diagEngine =context->getDiagnostics();unsigneddiagID=diagEngine.getCustomDiagID(DiagnosticsEngine::Warning,"Class name should not start with lowercase letter");SourceLocation location=declaration->getLocation();diagEngine.Report(location,diagID);}}|
Add some class with “bad” name
test.m

Objective-C
``
|1234567|@interfacebad_ToyObject: NSObject@end@implementationbad_ToyObject@end|
rebuild and run
```
```
|12345678910|/opt/llvm/toy_clang_plugin/build$$LLVM_HOME/llvm_build/bin/clang../test.m\-Xclang-load\-Xclang lib/Debug/ToyClangPlugin.dylib\-Xclang-plugin\-Xclang ToyClangPlugin../test.m:11:12:warning:Classname should notstart with lowercase letter@interfacebad_ToyObject:NSObject^1warning generated.|
#### Report error
Let’s generate error in case of class’ name contains `_`
ToyClangPlugin.cpp

C++
``
|123456789101112131415161718|voidcheckForUnderscoreInName(ObjCInterfaceDecl*declaration){size_tunderscorePos=declaration->getName().find('_');if(underscorePos!=StringRef::npos){DiagnosticsEngine&diagEngine=context->getDiagnostics();unsigneddiagID=diagEngine.getCustomDiagID(DiagnosticsEngine::Error,"Class name with `_` forbidden");SourceLocationlocation=declaration->getLocation().getLocWithOffset(underscorePos);diagEngine.Report(location,diagID);}}boolVisitObjCInterfaceDecl(ObjCInterfaceDecl*declaration){// disable this check temporary// checkForLowercasedName(declaration);checkForUnderscoreInName(declaration);returntrue;}|
Output after running
```
```
|12345678910|/opt/llvm/toy_clang_plugin/build$$LLVM_HOME/llvm_build/bin/clang../test.m\-Xclang-load\-Xclang lib/Debug/ToyClangPlugin.dylib\-Xclang-plugin\-Xclang ToyClangPlugin../test.m:11:15:error:Classname with`_`forbidden@interfacebad_ToyObject:NSObject^1error generated.|
Uncomment first check `checkForLowercasedName` and you’ll see both error and warning in the output
``
|12345678910111213|/opt/llvm/toy_clang_plugin/build$$LLVM_HOME/llvm_build/bin/clang../test.m\-Xclang-load\-Xclanglib/Debug/ToyClangPlugin.dylib\-Xclang-plugin\-XclangToyClangPlugin../test.m:11:12:warning:Classnameshouldnotstartwithlowercaseletter@interfacebad_ToyObject:NSObject^../test.m:11:15:error:Classnamewith`_`forbidden@interfacebad_ToyObject:NSObject^1warningand1errorgenerated.|
#### Xcode integration
Unfortunately, system (under ‘system’ I mean Xcode’s clang) clang doesn’t support plugins, so we need to hack Xcode a bit, to allow using of custom compiler.
Unzip [this](https://github.com/AlexDenisov/ToyClangPlugin/releases/download/0.0.1/XcodeHacking.zip) archive and run following commands:
```
```
|12|sudo mvHackedClang.xcplugin`xcode-select-print-path`/../PlugIns/Xcode3Core.ideplugin/Contents/SharedSupport/Developer/Library/Xcode/Plug-inssudo mvHackedBuildSystem.xcspec`xcode-select-print-path`/Platforms/iPhoneSimulator.platform/Developer/Library/Xcode/Specifications|
This will enable custom compiler for Xcode.
Reopen Xcode and you’ll see new compiler:
![custom_compiler](https://railsware.com/blog/wp-content/uploads/2014/02/custom_compiler.png)
Create new project and select newly added custom clang in `Build settings`
To enable plugin add following parameters to the `OTHER_CFLAGS` section
``
|1|-Xclang-load-Xclang/opt/llvm/toy_clang_plugin/build/lib/Debug/ToyClangPlugin.dylib-Xclang-add-plugin-XclangToyClangPlugin|
![OTHER_CFLAGS](https://railsware.com/blog/wp-content/uploads/2014/02/other_cflags.png)
Note, that we use `-add-plugin` here, because we want to add our `ASTAction`, not to replace the existing
Also, you should disable modules for this target/build
![disable_modules](https://railsware.com/blog/wp-content/uploads/2014/02/disable_modules.png)
Add `test.m` to this project, or create new one, with class names that match plugin criteria
After build you’ll see error and warnings in a more familiar form
![error_warning](https://railsware.com/blog/wp-content/uploads/2014/02/error_warning.png)
#### Interactive hints
It’s time to add `FixItHints` for both warning and error
ToyClangPlugin.cpp

C++
```
```
|123456789101112131415161718192021222324252627282930313233343536373839404142|voidcheckForLowercasedName(ObjCInterfaceDecl*declaration){StringRefname=declaration->getName();charc=name[0];if(isLowercase(c)){std::stringtempName=name;tempName[0]=toUppercase(c);StringRefreplacement(tempName);SourceLocationnameStart=declaration->getLocation();SourceLocationnameEnd=nameStart.getLocWithOffset(name.size());FixItHintfixItHint=FixItHint::CreateReplacement(SourceRange(nameStart,nameEnd),replacement);DiagnosticsEngine&diagEngine=context->getDiagnostics();unsigneddiagID=diagEngine.getCustomDiagID(DiagnosticsEngine::Warning,"Class name should not start with lowercase letter");SourceLocationlocation=declaration->getLocation();diagEngine.Report(location,diagID).AddFixItHint(fixItHint);}}voidcheckForUnderscoreInName(ObjCInterfaceDecl*declaration){StringRefname=declaration->getName();size_tunderscorePos=name.find('_');if(underscorePos!=StringRef::npos){std::stringtempName=name;std::string::iteratorend_pos=std::remove(tempName.begin(),tempName.end(),'_');tempName.erase(end_pos,tempName.end());StringRefreplacement(tempName);SourceLocationnameStart=declaration->getLocation();SourceLocationnameEnd=nameStart.getLocWithOffset(name.size());FixItHintfixItHint=FixItHint::CreateReplacement(SourceRange(nameStart,nameEnd),replacement);DiagnosticsEngine&diagEngine=context->getDiagnostics();unsigneddiagID=diagEngine.getCustomDiagID(DiagnosticsEngine::Error,"Class name with `_` forbidden");SourceLocationlocation=declaration->getLocation().getLocWithOffset(underscorePos);diagEngine.Report(location,diagID).AddFixItHint(fixItHint);}}|
Rebuild plugin and try to build test project
![warning_fixit_hint](https://railsware.com/blog/wp-content/uploads/2014/02/warning_fixit_hint.png)
![error_fixit_hint](https://railsware.com/blog/wp-content/uploads/2014/02/error_fixit_hint.png)
#### Conclusion
As you can see, creating a clang plugin is relatively simple, but it needs some dirty hacks with Xcode, and you should use custom built clang, so I’d not recommend you to use this clang for building apps for production usage. Apple provides patched
 version, and we can’t know the difference between them. Also, it needs a lot of efforts to make it work, which doesn’t make it widely usable.
Another issue you might face is unstable API, cause it uses internal API which changes continuously.
You still can use it on your system for different diagnostic purposes, but please do not force other people to depend on such heavyweight things.
If you have any comments, questions or suggestions feel free to ask me on [twitter](https://twitter.com/1101_debian), open issue on [GitHub](https://github.com/AlexDenisov/ToyClangPlugin),
 or just leave a comment here.
Happy hacking!
