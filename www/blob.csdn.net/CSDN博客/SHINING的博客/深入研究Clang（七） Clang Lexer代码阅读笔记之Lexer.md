# 深入研究Clang（七） Clang Lexer代码阅读笔记之Lexer - SHINING的博客 - CSDN博客
2016年08月10日 17:10:50[snsn1984](https://me.csdn.net/snsn1984)阅读数：3922
所属专栏：[深入研究Clang](https://blog.csdn.net/column/details/hacking-clang.html)
作者：[史宁宁（snsn1984）](http://blog.csdn.net/snsn1984)
源码位置：clang/lib/Lexer.cpp
源码网络地址：[http://clang.llvm.org/doxygen/Lexer_8cpp_source.html](http://clang.llvm.org/doxygen/Lexer_8cpp_source.html)
Lexer.cpp这个文件，是Clang这个前端的词法分析器的主要文件，它的内容是对Lexer这个类的具体实现，原文件的注释中：“This file implements the Lexer and Token interfaces.” 这么解释这个文件的，但是Token只有两个简单函数的实现，剩下的都是Lexer的实现。所以要想搞清楚Clang的词法分析器是怎么实现的，那么必须对这个文件有着深入的理解。
从Lexer的初始化函数开始入手：
```cpp
void Lexer::InitLexer(const char *BufStart, const char *BufPtr,
   56                       const char *BufEnd) {
   57   BufferStart = BufStart;
   58   BufferPtr = BufPtr;
   59   BufferEnd = BufEnd;
   60 
   61   assert(BufEnd[0] == 0 &&
   62          "We assume that the input buffer has a null character at the end"
   63          " to simplify lexing!");
   64 
   65   // Check whether we have a BOM in the beginning of the buffer. If yes - act
   66   // accordingly. Right now we support only UTF-8 with and without BOM, so, just
   67   // skip the UTF-8 BOM if it's present.
   68   if (BufferStart == BufferPtr) {
   69     // Determine the size of the BOM.
   70     StringRef Buf(BufferStart, BufferEnd - BufferStart);
   71     size_t BOMLength = llvm::StringSwitch<size_t>(Buf)
   72       .StartsWith("\xEF\xBB\xBF", 3) // UTF-8 BOM
   73       .Default(0);
   74 
   75     // Skip the BOM.
   76     BufferPtr += BOMLength;
   77   }
   78 
   79   Is_PragmaLexer = false;
   80   CurrentConflictMarkerState = CMK_None;
   81 
   82   // Start of the file is a start of line.
   83   IsAtStartOfLine = true;
   84   IsAtPhysicalStartOfLine = true;
   85 
   86   HasLeadingSpace = false;
   87   HasLeadingEmptyMacro = false;
   88 
   89   // We are not after parsing a #.
   90   ParsingPreprocessorDirective = false;
   91 
   92   // We are not after parsing #include.
   93   ParsingFilename = false;
   94 
   95   // We are not in raw mode.  Raw mode disables diagnostics and interpretation
   96   // of tokens (e.g. identifiers, thus disabling macro expansion).  It is used
   97   // to quickly lex the tokens of the buffer, e.g. when handling a "#if 0" block
   98   // or otherwise skipping over tokens.
   99   LexingRawMode = false;
  100 
  101   // Default to not keeping comments.
  102   ExtendedTokenMode = 0;
  103 }
```
这个初始化函数，是在Lexer类的两个构造函数里被调用的，具体代码如下：
```cpp
104 
  105 /// Lexer constructor - Create a new lexer object for the specified buffer
  106 /// with the specified preprocessor managing the lexing process.  This lexer
  107 /// assumes that the associated file buffer and Preprocessor objects will
  108 /// outlive it, so it doesn't take ownership of either of them.
  109 Lexer::Lexer(FileID FID, const llvm::MemoryBuffer *InputFile, Preprocessor &PP)
  110   : PreprocessorLexer(&PP, FID),
  111     FileLoc(PP.getSourceManager().getLocForStartOfFile(FID)),
  112     LangOpts(PP.getLangOpts()) {
  113 
  114   InitLexer(InputFile->getBufferStart(), InputFile->getBufferStart(),
  115             InputFile->getBufferEnd());
  116 
  117   resetExtendedTokenMode();
  118 }
  119 
  120 void Lexer::resetExtendedTokenMode() {
  121   assert(PP && "Cannot reset token mode without a preprocessor");
  122   if (LangOpts.TraditionalCPP)
  123     SetKeepWhitespaceMode(true);
  124   else
  125     SetCommentRetentionState(PP->getCommentRetentionState());
  126 }
  127 
  128 /// Lexer constructor - Create a new raw lexer object.  This object is only
  129 /// suitable for calls to 'LexFromRawLexer'.  This lexer assumes that the text
  130 /// range will outlive it, so it doesn't take ownership of it.
  131 Lexer::Lexer(SourceLocation fileloc, const LangOptions &langOpts,
  132              const char *BufStart, const char *BufPtr, const char *BufEnd)
  133   : FileLoc(fileloc), LangOpts(langOpts) {
  134 
  135   InitLexer(BufStart, BufPtr, BufEnd);
  136 
  137   // We *are* in raw mode.
  138   LexingRawMode = true;
  139 }
```
这两个构造函数各有不同，从输入参数上就可以看出。也有相同的地方，就是对一些参数只是引用的关系，并没有获取这些参数的所有权。
Lexer的构造函数，在自己的类内部，分别被以下的函数所调用：
Create_PragmaLexer: [Lexer](http://clang.llvm.org/doxygen/classclang_1_1Lexer.html) constructor - Create a new lexer object for _Pragma expansion.
http://clang.llvm.org/doxygen/classclang_1_1Lexer.html#ac7f3b1ce4f2eeaec8d787d22bf197cd0
getSpelling - This method is used to get the spelling of a token into a preallocated buffer, instead of as an std::string.
http://clang.llvm.org/doxygen/classclang_1_1Lexer.html#a94f2c5710332ae19d7955c609ac37adb
getRawToken
http://clang.llvm.org/doxygen/classclang_1_1Lexer.html#adac8b8cf001621ec3b109d82a7074f05
 getBeginningOfFileToken
http://clang.llvm.org/doxygen/Lexer_8cpp.html#a4845396d18432c436e605303b057dbb4
findLocationAfterToken
http://clang.llvm.org/doxygen/classclang_1_1Lexer.html#a099b99b2d19ef5cdd8fcb80d8cf4064e

