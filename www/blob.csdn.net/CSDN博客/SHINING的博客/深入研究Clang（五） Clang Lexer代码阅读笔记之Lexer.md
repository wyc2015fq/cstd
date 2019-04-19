# 深入研究Clang（五）  Clang Lexer代码阅读笔记之Lexer - SHINING的博客 - CSDN博客
2014年11月20日 14:53:55[snsn1984](https://me.csdn.net/snsn1984)阅读数：5741
所属专栏：[深入研究Clang](https://blog.csdn.net/column/details/hacking-clang.html)

作者：[史宁宁（snsn1984）](http://blog.csdn.net/snsn1984)
Clang的Lexer（词法分析器）的源码的主要位置如下：
clang/lib/Lex    这里是主要的Lexer的代码；
clang/include/clang/Lex   这里是Lexer的头文件代码的位置；
同时，Lexer还使用了clangBasic库，所以要分析Lexer的代码，clangBasic（clang/lib/Basic）的一些代码也会用到。
首先从Lexer入手。
**clang/include/clang/Lex/Lexer.h**
clang::Lexer:
```cpp
00057   //===--------------------------------------------------------------------===//
00058   // Context-specific lexing flags set by the preprocessor.
00059   //
00060
00061   /// ExtendedTokenMode - The lexer can optionally keep comments and whitespace
00062   /// and return them as tokens.  This is used for -C and -CC modes, and
00063   /// whitespace preservation can be useful for some clients that want to lex
00064   /// the file in raw mode and get every character from the file.
00065   ///
00066   /// When this is set to 2 it returns comments and whitespace.  When set to 1
00067   /// it returns comments, when it is set to 0 it returns normal tokens only.
00068   unsigned char ExtendedTokenMode;
00069
00070   //===--------------------------------------------------------------------===//
```
这个成员变量保存词法分析的一个状态，根据它的值的不同：0、1、2，分别对应只返回正常的token，返回comments
和正常的token，返回空格、comments和正常的token。
下面是几个操作这个成员变量的函数，基本上都是获取值、设置值和重设值。代码不复杂，
```cpp
00162   /// isKeepWhitespaceMode - Return true if the lexer should return tokens for
00163   /// every character in the file, including whitespace and comments.  This
00164   /// should only be used in raw mode, as the preprocessor is not prepared to
00165   /// deal with the excess tokens.
00166   bool isKeepWhitespaceMode() const {
00167     return ExtendedTokenMode > 1;
00168   }
00169
00170   /// SetKeepWhitespaceMode - This method lets clients enable or disable
00171   /// whitespace retention mode.
00172   void SetKeepWhitespaceMode(bool Val) {
00173     assert((!Val || LexingRawMode || LangOpts.TraditionalCPP) &&
00174            "Can only retain whitespace in raw mode or -traditional-cpp");
00175     ExtendedTokenMode = Val ? 2 : 0;
00176   }
00177
00178   /// inKeepCommentMode - Return true if the lexer should return comments as
00179   /// tokens.
00180   bool inKeepCommentMode() const {
00181     return ExtendedTokenMode > 0;
00182   }
00183
00184   /// SetCommentRetentionMode - Change the comment retention mode of the lexer
00185   /// to the specified mode.  This is really only useful when lexing in raw
00186   /// mode, because otherwise the lexer needs to manage this.
00187   void SetCommentRetentionState(bool Mode) {
00188     assert(!isKeepWhitespaceMode() &&
00189            "Can't play with comment retention state when retaining whitespace");
00190     ExtendedTokenMode = Mode ? 1 : 0;
00191   }
00192
00193   /// Sets the extended token mode back to its initial value, according to the
00194   /// language options and preprocessor. This controls whether the lexer
00195   /// produces comment and whitespace tokens.
00196   ///
00197   /// This requires the lexer to have an associated preprocessor. A standalone
00198   /// lexer has nothing to reset to.
00199   void resetExtendedTokenMode();
```
关于raw mode:
raw mode的时候，ExtendedTokenMode = 2，Lexer会输出包含空格、comments和正常tokens在内的所有
字符。在Lexer的父类：clang::PreprocessorLexer类中（），有一个成员变量：
```cpp
00049   /// \brief True if in raw mode.
00050   ///
00051   /// Raw mode disables interpretation of tokens and is a far faster mode to
00052   /// lex in than non-raw-mode.  This flag:
00053   ///  1. If EOF of the current lexer is found, the include stack isn't popped.
00054   ///  2. Identifier information is not looked up for identifier tokens.  As an
00055   ///     effect of this, implicit macro expansion is naturally disabled.
00056   ///  3. "#" tokens at the start of a line are treated as normal tokens, not
00057   ///     implicitly transformed by the lexer.
00058   ///  4. All diagnostic messages are disabled.
00059   ///  5. No callbacks are made into the preprocessor.
00060   ///
00061   /// Note that in raw mode that the PP pointer may be null.
00062   bool LexingRawMode;
```
它可以表明Lexer是否在raw mode下。同时，这里的注释也说明了raw model的作用。
从clang::Lexer的定义可以看出，它是clang::PreprocessorLexer的子类，上面raw model的部分也引用了clang::PreprocessorLexer类的代码，下面看下clang::PreprocessorLexer的代码。
**clang/include/clang/Lex/PreprocessorLexer.h**
```cpp
00022 namespace clang {
00023 
00024 class FileEntry;
00025 class Preprocessor;
```
从这里可以看出clang::PreprocessorLexer使用了上面两个类，而在头文件中的具体位置就是：
```cpp
00027 class PreprocessorLexer {
00028   virtual void anchor();
00029 protected:
00030   Preprocessor *PP;              // Preprocessor object controlling lexing.
```
以及
```cpp
00164   /// getFileEntry - Return the FileEntry corresponding to this FileID.  Like
00165   /// getFileID(), this only works for lexers with attached preprocessors.
00166   const FileEntry *getFileEntry() const;
```
从代码中可以看出，这两个类，一个是作为成员变量，一个是作为了一个成员函数的返回类型来使用的。我们跟踪代码去看下这两个类的具体实现。这两个类的具体实现，FileEntry较为简单，很容易看出到底内容；而Preprocessor类较为复杂，牵涉内容较多，在这里暂且不作分析。后续继续分析。

