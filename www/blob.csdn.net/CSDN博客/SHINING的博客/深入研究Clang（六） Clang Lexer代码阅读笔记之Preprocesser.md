# 深入研究Clang（六）  Clang Lexer代码阅读笔记之Preprocesser - SHINING的博客 - CSDN博客
2014年12月01日 17:12:57[snsn1984](https://me.csdn.net/snsn1984)阅读数：4120
所属专栏：[深入研究Clang](https://blog.csdn.net/column/details/hacking-clang.html)
作者：[史宁宁（snsn1984）](http://blog.csdn.net/snsn1984)
clang/include/clang/Lex/Preprocesser.h
这个文件是包含clang::Preprocesser类的定义的头文件。它是类C语言（C、C++、Object C）的预处理的头文件。也就是说，类C语言的预处理都会用到此处的代码。
```cpp
00082 /// \brief Context in which macro name is used.
00083 enum MacroUse {
00084   MU_Other  = 0,  // other than #define or #undef
00085   MU_Define = 1,  // macro name specified in #define
00086   MU_Undef  = 2   // macro name specified in #undef
00087 };
```
这个枚举很简单，就是通过枚举值来确定到底使用了哪些宏。除了define和undef之外，其他的都分类到MU_Other类别下边了。
```cpp
00089 /// \brief Engages in a tight little dance with the lexer to efficiently
00090 /// preprocess tokens.
00091 ///
00092 /// Lexers know only about tokens within a single source file, and don't
00093 /// know anything about preprocessor-level issues like the \#include stack,
00094 /// token expansion, etc.
00095 class Preprocessor : public RefCountedBase<Preprocessor> {
00096   IntrusiveRefCntPtr<PreprocessorOptions> PPOpts;
00097   DiagnosticsEngine        *Diags;
00098   LangOptions       &LangOpts;
00099   const TargetInfo  *Target;
00100   FileManager       &FileMgr;
00101   SourceManager     &SourceMgr;
00102   std::unique_ptr<ScratchBuffer> ScratchBuf;
00103   HeaderSearch      &HeaderInfo;
00104   ModuleLoader      &TheModuleLoader;
```
这里可以看到Preprocessor的类的定义，而它是模板类RefCountedBase<Preprocessor>的子类。同时可以看看这几个类的几个成员变量，DiagnosticsEngine-诊断引擎，LangOptions-接收的编译选项，TargetInfo-存储目标信息，FileManager-文件管理器，SourceManager-源码管理器，ModuleLoader-module加载器。
```cpp
00118   /// Identifiers for builtin macros and other builtins.
00119   IdentifierInfo *Ident__LINE__, *Ident__FILE__;   // __LINE__, __FILE__
00120   IdentifierInfo *Ident__DATE__, *Ident__TIME__;   // __DATE__, __TIME__
00121   IdentifierInfo *Ident__INCLUDE_LEVEL__;          // __INCLUDE_LEVEL__
00122   IdentifierInfo *Ident__BASE_FILE__;              // __BASE_FILE__
00123   IdentifierInfo *Ident__TIMESTAMP__;              // __TIMESTAMP__
00124   IdentifierInfo *Ident__COUNTER__;                // __COUNTER__
00125   IdentifierInfo *Ident_Pragma, *Ident__pragma;    // _Pragma, __pragma
00126   IdentifierInfo *Ident__identifier;               // __identifier
00127   IdentifierInfo *Ident__VA_ARGS__;                // __VA_ARGS__
00128   IdentifierInfo *Ident__has_feature;              // __has_feature
00129   IdentifierInfo *Ident__has_extension;            // __has_extension
00130   IdentifierInfo *Ident__has_builtin;              // __has_builtin
00131   IdentifierInfo *Ident__has_attribute;            // __has_attribute
00132   IdentifierInfo *Ident__has_include;              // __has_include
00133   IdentifierInfo *Ident__has_include_next;         // __has_include_next
00134   IdentifierInfo *Ident__has_warning;              // __has_warning
00135   IdentifierInfo *Ident__is_identifier;            // __is_identifier
00136   IdentifierInfo *Ident__building_module;          // __building_module
00137   IdentifierInfo *Ident__MODULE__;                 // __MODULE__
00138   IdentifierInfo *Ident__has_cpp_attribute;        // __has_cpp_attribute
```
另外Preprocesser类中包含了使用IdentifierInfo指针所保存的内建宏以及其他的内建的标示符。所以如果要修改或者扩展内建宏或者其他内建标示符，这里也是必须要修改的地方。
```cpp
00148   // State that is set before the preprocessor begins.
00149   bool KeepComments : 1;
00150   bool KeepMacroComments : 1;
00151   bool SuppressIncludeNotFoundError : 1;
00152 
00153   // State that changes while the preprocessor runs:
00154   bool InMacroArgs : 1;            // True if parsing fn macro invocation args.
00155 
00156   /// Whether the preprocessor owns the header search object.
00157   bool OwnsHeaderSearch : 1;
00158 
00159   /// True if macro expansion is disabled.
00160   bool DisableMacroExpansion : 1;
00161 
00162   /// Temporarily disables DisableMacroExpansion (i.e. enables expansion)
00163   /// when parsing preprocessor directives.
00164   bool MacroExpansionInDirectivesOverride : 1;
00165 
00166   class ResetMacroExpansionHelper;
00167 
00168   /// \brief Whether we have already loaded macros from the external source.
00169   mutable bool ReadMacrosFromExternalSource : 1;
00170 
00171   /// \brief True if pragmas are enabled.
00172   bool PragmasEnabled : 1;
00173 
00174   /// \brief True if the current build action is a preprocessing action.
00175   bool PreprocessedOutput : 1;
```
另外在Preprocesser类中，还出现了一部分这样的代码，这种给成员变量制定位域的形式还是第一次见到，一度让我以为是要给成员变量赋值。这种使用位域节省空间的方式，在结构体中比在类中稍微常见点，为了节省空间，在这么大的类中使用还是第一次见。：后面的数字是为了指定一定的bit位去保存变量的值。其中，这里里面还有一个更特殊的一行代码：
```cpp
00168   /// \brief Whether we have already loaded macros from the external source.
00169   mutable bool ReadMacrosFromExternalSource : 1;
```
这个成员变量，不仅仅用到了位域，还用到了mutable。一般使用mutable修饰变量，是为了在const成员函数中可以修改这个成员变量。
