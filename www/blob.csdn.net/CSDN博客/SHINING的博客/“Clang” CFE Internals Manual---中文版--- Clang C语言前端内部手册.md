# “Clang” CFE Internals Manual---中文版---"Clang"C语言前端内部手册 - SHINING的博客 - CSDN博客
2013年12月13日 10:46:07[snsn1984](https://me.csdn.net/snsn1984)阅读数：6758
原文地址：[http://clang.llvm.org/docs/InternalsManual.html](http://clang.llvm.org/docs/InternalsManual.html)
译者：[史宁宁](http://weibo.com/u/1899097901)（[snsn1984](http://blog.csdn.net/snsn1984)）
**"Clang"C语言前端内部手册**
# 简介
这个文档描述了比较重要的API中的一部分API，还表述了Clang C语言前端中的一些内部设计想法。这个文档的目的是既把握住高层次的信息的同时也对这些高层次的信息背后的设计思路进行一些介绍。这篇文档面向的是打算hacking（这个词的具体含义在这里实在不好把握，感觉英文更容易表达具体含义）Clang的人，而不是Clang的最终用户。接下来的描述是按照库分类的，并且没描述任何库的使用者。
# LLVM支持库
LLVM的libSupport库提供了很多相关库和数据结构，包括命令行选项处理，各种容器和一个可以用来接入文件系统的系统抽象层。
# Clang“基本”库
这个库很明显需要一个更好的名字。这个“基本”库包含了跟踪和操作代码缓存，源码缓存区中的定位，诊断，序列，目标抽取，和被编译的编程语言的子集的相关信息这一系列的底层公共操作。
这个库的一部分是特别针对C语言的（比如TargetInfo类），剩下的部分可以被其他的不是基于C的编程语言重用（SourceLocation, SourceManager, Diagnostics, FileManager）。如果未来有需求的话，我们可以指出是否需要介绍一个新库，把这些公用的类移动到别的地方，或者介绍一些其他的解决方案。
我们依据这些类的依赖关系介绍这些类的角色。
## 诊断子系统
Clang的诊断子系统是编译器和人交流的很重要的一部分。诊断信息就是在代码不正确或者有可能不正确的时候，产生的一些警告或者错误提示。在Clang里，每一条诊断信息都有（至少有）一个唯一的一个ID，还有一个与之相关的英文翻译，一个SourceLocation去插入符号，和一个严重性级别（比如：警告或者错误）。它们还有一系列的和诊断相关的源码范围相关的可选择的诊断参数（在字符串的哪个部分插入%0）。
In this section, we’ll be giving examples produced by the Clang command line driver, but diagnostics can be[*rendered in many different ways*](http://clang.llvm.org/docs/InternalsManual.html#diagnosticclient)depending on how theDiagnosticClientinterface
 is implemented. A representative example of a diagnostic is:
在这部分，我们给出的例子都是通过Clang的命令行驱动产生的，但是DiagnosticClient的不同实现方式可以通过不同的方法来给出诊断信息。一个具有代表性的诊断信息的例子：
t.c:38:15: error: invalid operands to binary expression ('int *' and '_Complex float')
P = (P-42) + Gamma*4;
    ~~~~~~ ^ ~~~~~~~
在这个例子里，你可以看到英语翻译，严重性级别（错误），你可以看到源码的位置（插入符号"^"和文件/行/列信息），源码范围"~~~~~~"，对应与诊断信息(“int*” and
 “_Complexfloat”)的参数。你将不得不相信我，每个诊断信息背后都有一个唯一的ID。
让所有的这些发生需要好几个步骤，并且包括了很多moving pieces，这部分将描述它们，并且讨论一下当新加一个新的诊断信息的最好实践办法。
### Diagnostic*Kinds.td文件
诊断信息通过在clang/Basic/Diagnostic*Kinds.td中的某个文件添加一个入口来创建，依赖于即将使用的库。tblgen根据这个文件生成诊断信息唯一的ID，诊断信息的严重程度和英语翻译+格式字符串。
目前唯一的ID的名字很不合理。一些是以err_, warn_, ext_开头的，以此来给名字中加入严重程度的信息。因此使用C++代码中的枚举来生成这些诊断信息，目前因为比较短所以还是有一定的实用之处的。
诊断信息的严重程度来自集合{NOTE, WARNING, EXTENSION, EXTWARN, ERROR}。ERROR严重程度用用于诊断信息表明这个程序是在任何情况下都不能接受的。当一个错误生成的时候，输入代码的AST有可能没有完全构建。EXTENSION 和 EXTWARN严重程度用于Clang可以接受的扩展。这意味着Clang可以完全理解并且可以用AST表示它们（译者注：输入代码），但是我们产生诊断信息告诉用户他们的代码是不可移植的。两者不同的是，前者可以默认被忽略，但是后者默认警告。WARNING严重程度用来构建那些在目前编程语言中是正确的但是却在某种程度上具有可疑的代码。NOTE这个层次的严重程度用来在前面的诊断信息基础上提供更多的信息。
这些严重程度根据诊断系统基于各种配置的选项的子系统，被映射到一个输出层次的更小的集合（the Diagnostic::Level enum, {Ignored, Note, Warning, Error, Fatal}）。Clang内部支持完全的细粒度的映射机制，可以允许你映射几乎所有的诊断信息到你想要的输出层面。不能被映射的严重程度就是NOTE，它总是跟随着前面生成的诊断信息的严重程度；还有ERROR，它只能被映射到Fatal（比如：你不可能把一个错误转化成一个警告）。
诊断信息的映射可以有很多中方法去使用。比如，如果用户指定了-pedantic，那么EXTENSION将被映射到Warning；如果他们指定-pedantic-errors，它（译者注：EXTENSION）将变为Error。这可以用来实现像 -Wunused_macros, -Wundef等类似的选项。
只有在错误十分严重，并且无法恢复的情况下才会把诊断信息映射到Fatal（这时候将有成吨的错误信息喷涌而出）。这类错误的一个例子就是 #include一个文件失败。
#### [The Format String](http://clang.llvm.org/docs/InternalsManual.html#id6)
The format string for the diagnostic is very simple, but it has some power. It takes the form of a string in English with markers that indicate where and how arguments to the diagnostic are inserted and formatted. For example,
 here are some simple format strings:
"binary integer literals are an extension""format string contains '\\0' within the string body""more '%%' conversions than data arguments""invalid operands to binary expression (%0 and %1)""overloaded '%0' must be a %select{unary|binary|unary or binary}2 operator"" (has %1 parameter%s1)"
These examples show some important points of format strings. You can use any plain ASCII character in the diagnostic string except “%”
 without a problem, but these are C strings, so you have to use and be aware of all the C escape sequences (as in the second example). If you want to produce a “%”
 in the output, use the “%%” escape sequence, like the third diagnostic. Finally, Clang uses the “%...[digit]”
 sequences to specify where and how arguments to the diagnostic are formatted.
Arguments to the diagnostic are numbered according to how they are specified by the C++ code that[*produces them*](http://clang.llvm.org/docs/InternalsManual.html#internals-producing-diag), and are referenced by%0..%9.
 If you have more than 10 arguments to your diagnostic, you are doing something wrong :). Unlikeprintf,
 there is no requirement that arguments to the diagnostic end up in the output in the same order as they are specified, you could have a format string with “%1%0”
 that swaps them, for example. The text in between the percent and digit are formatting instructions. If there are no instructions, the argument is just turned into a string and substituted in.
Here are some “best practices” for writing the English format string:
- 
Keep the string short. It should ideally fit in the 80 column limit of theDiagnosticKinds.tdfile.
 This avoids the diagnostic wrapping when printed, and forces you to think about the important point you are conveying with the diagnostic.
- 
Take advantage of location information. The user will be able to see the line and location of the caret, so you don’t need to tell them that the problem is with the 4th argument to the function: just point to it.
- 
Do not capitalize the diagnostic string, and do not end it with a period.
- 
If you need to quote something in the diagnostic string, use single quotes.
Diagnostics should never take random English strings as arguments: you shouldn’t use “youhaveaproblemwith%0”
 and pass in things like “yourargument” or “yourreturnvalue”
 as arguments. Doing this prevents[*translating*](http://clang.llvm.org/docs/InternalsManual.html#internals-diag-translation)the
 Clang diagnostics to other languages (because they’ll get random English words in their otherwise localized diagnostic). The exceptions to this are C/C++ language keywords (e.g.,auto,const,mutable,
 etc) and C/C++ operators (/=). Note that things like “pointer” and “reference” are not keywords. On the other hand, you*can*include
 anything that comes from the user’s source code, including variable names, types, labels, etc. The “select” format can
 be used to achieve this sort of thing in a localizable way, see below.
#### [Formatting a Diagnostic Argument](http://clang.llvm.org/docs/InternalsManual.html#id7)
Arguments to diagnostics are fully typed internally, and come from a couple different classes: integers, types, names, and random strings. Depending on the class of the argument, it can be optionally formatted in different ways.
 This gives theDiagnosticClientinformation about what the argument means without requiring
 it to use a specific presentation (consider this MVC for Clang :).
Here are the different diagnostic argument formats currently supported by Clang:
**“s” format**
- Example:
"requires%1parameter%s1"- Class:
Integers- Description:
This is a simple formatter for integers that is useful when producing English diagnostics. When the integer is 1, it prints as nothing. When the integer is not 1, it prints as “s”.
 This allows some simple grammatical forms to be to be handled correctly, and eliminates the need to use gross things like"requires%1parameter(s)".
**“select” format**
- Example:
"mustbea%select{unary|binary|unaryorbinary}2operator"- Class:
Integers- Description:
This format specifier is used to merge multiple related diagnostics together into one common one, without requiring the difference to be specified as an English string argument. Instead of specifying
 the string, the diagnostic gets an integer argument and the format string selects the numbered option. In this case, the “%2”
 value must be an integer in the range [0..2]. If it is 0, it prints “unary”, if it is 1 it prints “binary” if it is 2, it prints “unary or binary”. This allows other language translations to substitute reasonable words (or entire phrases) based on the semantics
 of the diagnostic instead of having to do things textually. The selected string does undergo formatting.
**“plural” format**
- Example:
"youhave%1%plural{1:mouse|:mice}1connectedtoyourcomputer"- Class:
Integers- Description:
This is a formatter for complex plural forms. It is designed to handle even the requirements of languages with very complex plural forms, as many Baltic languages have. The argument consists
 of a series of expression/form pairs, separated by ”:”, where the first form whose expression evaluates to true is the result of the modifier.
An expression can be empty, in which case it is always true. See the example at the top. Otherwise, it is a series of one or more numeric conditions, separated by ”,”. If any condition matches, the expression matches.
 Each numeric condition can take one of three forms.
- 
number: A simple decimal number matches if the argument is the same as the number. Example:"%plural{1:mouse|:mice}4"
- 
range: A range in square brackets matches if the argument is within the range. Then range is inclusive on both ends. Example:"%plural{0:none|1:one|[2,5]:some|:many}2"
- 
modulo: A modulo operator is followed by a number, and equals sign and either a number or a range. The tests are the same as for plain numbers and ranges, but the argument is taken modulo the number first. Example:"%plural{%100=0:evenhundred|%100=[1,50]:lowerhalf|:everythingelse}1"
The parser is very unforgiving. A syntax error, even whitespace, will abort, as will a failure to match the argument against any expression.
**“ordinal” format**
- Example:
"ambiguityin%ordinal0argument"- Class:
Integers- Description:
This is a formatter which represents the argument number as an ordinal: the value1becomes1st,3becomes3rd,
 and so on. Values less than1are not supported. This formatter is currently hard-coded to
 use English ordinals.
**“objcclass” format**
- Example:
"method%objcclass0notfound"- Class:
DeclarationName- Description:
This is a simple formatter that indicates theDeclarationNamecorresponds
 to an Objective-C class method selector. As such, it prints the selector with a leading “+”.
**“objcinstance” format**
- Example:
"method%objcinstance0notfound"- Class:
DeclarationName- Description:
This is a simple formatter that indicates theDeclarationNamecorresponds
 to an Objective-C instance method selector. As such, it prints the selector with a leading “-”.
**“q” format**
- Example:
"candidatefoundbynamelookupis%q0"- Class:
NamedDecl*- Description:
This formatter indicates that the fully-qualified name of the declaration should be printed, e.g., “std::vector”
 rather than “vector”.
**“diff” format**
- Example:
"noknownconversion%diff{from$to$|fromargumenttypetoparametertype}1,2"- Class:
QualType- Description:
This formatter takes twoQualTypes and attempts
 to print a template difference between the two. If tree printing is off, the text inside the braces before the pipe is printed, with the formatted text replacing the $. If tree printing is on, the text after the pipe is printed and a type tree is printed after
 the diagnostic message.
It is really easy to add format specifiers to the Clang diagnostics system, but they should be discussed before they are added. If you are creating a lot of repetitive diagnostics and/or have an idea for a useful formatter, please
 bring it up on the cfe-dev mailing list.
#### [Producing the Diagnostic](http://clang.llvm.org/docs/InternalsManual.html#id8)
Now that you’ve created the diagnostic in theDiagnostic*Kinds.tdfile,
 you need to write the code that detects the condition in question and emits the new diagnostic. Various components of Clang (e.g., the preprocessor,Sema,
 etc.) provide a helper function named “Diag”. It creates a diagnostic and accepts the arguments, ranges, and other information
 that goes along with it.
For example, the binary expression error comes from code like this:
if(variousthingsthatarebad)Diag(Loc,diag::err_typecheck_invalid_operands)<<lex->getType()<<rex->getType()<<lex->getSourceRange()<<rex->getSourceRange();
This shows that use of theDiagmethod: it takes a location (a[*SourceLocation*](http://clang.llvm.org/docs/InternalsManual.html#sourcelocation)object)
 and a diagnostic enum value (which matches the name fromDiagnostic*Kinds.td). If the diagnostic takes arguments,
 they are specified with the<<operator: the first argument becomes%0,
 the second becomes%1, etc. The diagnostic interface allows you to specify arguments of many different types,
 includingintandunsignedfor
 integer arguments,constchar*andstd::stringfor
 string arguments,DeclarationNameandconstIdentifierInfo*for
 names,QualTypefor types, etc.SourceRanges
 are also specified with the<<operator, but do not have a specific ordering requirement.
As you can see, adding and producing a diagnostic is pretty straightforward. The hard part is deciding exactly what you need to say to help the user, picking a suitable wording, and providing the information needed to format it
 correctly. The good news is that the call site that issues a diagnostic should be completely independent of how the diagnostic is formatted and in what language it is rendered.
#### [Fix-It Hints](http://clang.llvm.org/docs/InternalsManual.html#id9)
In some cases, the front end emits diagnostics when it is clear that some small change to the source code would fix the problem. For example, a missing semicolon at the end of a statement or a use of deprecated syntax that is easily
 rewritten into a more modern form. Clang tries very hard to emit the diagnostic and recover gracefully in these and other cases.
However, for these cases where the fix is obvious, the diagnostic can be annotated with a hint (referred to as a “fix-it hint”) that describes how to change the code referenced by the diagnostic to fix the problem. For example,
 it might add the missing semicolon at the end of the statement or rewrite the use of a deprecated construct into something more palatable. Here is one such example from the C++ front end, where we warn about the right-shift operator changing meaning from C++98
 to C++11:
test.cpp:3:7: warning: use of right-shift operator ('>>') in template argument
                       will require parentheses in C++11
A<100 >> 2> *a;
      ^
  (       )
Here, the fix-it hint is suggesting that parentheses be added, and showing exactly where those parentheses would be inserted into the source code. The fix-it hints themselves describe what changes to make to the source code in
 an abstract manner, which the text diagnostic printer renders as a line of “insertions” below the caret line.[*Other diagnostic clients*](http://clang.llvm.org/docs/InternalsManual.html#diagnosticclient)might choose to render the code differently (e.g., as markup inline) or even give the user the ability to automatically fix the problem.
Fix-it hints on errors and warnings need to obey these rules:
- 
Since they are automatically applied if-Xclang-fixitis
 passed to the driver, they should only be used when it’s very likely they match the user’s intent.
- 
Clang must recover from errors as if the fix-it had been applied.
If a fix-it can’t obey these rules, put the fix-it on a note. Fix-its on notes are not applied automatically.
All fix-it hints are described by theFixItHintclass, instances
 of which should be attached to the diagnostic using the<<operator in the same way that highlighted
 source ranges and arguments are passed to the diagnostic. Fix-it hints can be created with one of three constructors:
- 
FixItHint::CreateInsertion(Loc,Code)
> 
Specifies that the givenCode(a string) should be inserted before
 the source locationLoc.
- 
FixItHint::CreateRemoval(Range)
> 
Specifies that the code in the given sourceRangeshould be removed.
- 
FixItHint::CreateReplacement(Range,Code)
> 
Specifies that the code in the given sourceRangeshould be removed,
 and replaced with the givenCodestring.
#### [TheDiagnosticClientInterface](http://clang.llvm.org/docs/InternalsManual.html#id10)
Once code generates a diagnostic with all of the arguments and the rest of the relevant information, Clang needs to know what to do with it. As previously mentioned, the diagnostic machinery goes through some filtering to map a
 severity onto a diagnostic level, then (assuming the diagnostic is not mapped to “Ignore”) it invokes an object that
 implements theDiagnosticClientinterface with the information.
It is possible to implement this interface in many different ways. For example, the normal ClangDiagnosticClient(namedTextDiagnosticPrinter)
 turns the arguments into strings (according to the various formatting rules), prints out the file/line/column information and the string, then prints out the line of code, the source ranges, and the caret. However, this behavior isn’t required.
Another implementation of theDiagnosticClientinterface is theTextDiagnosticBufferclass,
 which is used when Clang is in-verifymode. Instead of formatting and printing out the diagnostics,
 this implementation just captures and remembers the diagnostics as they fly by. Then-verifycompares
 the list of produced diagnostics to the list of expected ones. If they disagree, it prints out its own output. Full documentation for the-verifymode
 can be found in the Clang API documentation for[VerifyDiagnosticConsumer](http://clang.llvm.org/doxygen/classclang_1_1VerifyDiagnosticConsumer.html#details).
There are many other possible implementations of this interface, and this is why we prefer diagnostics to pass down rich structured information in arguments. For example, an HTML output might want declaration names be linkified
 to where they come from in the source. Another example is that a GUI might let you click on typedefs to expand them. This application would want to pass significantly more information about types through to the GUI than a simple flat string. The interface
 allows this to happen.
#### [Adding Translations to Clang](http://clang.llvm.org/docs/InternalsManual.html#id11)
Not possible yet! Diagnostic strings should be written in UTF-8, the client can translate to the relevant code page if needed. Each translation completely replaces the format string for the diagnostic.
### [TheSourceLocationandSourceManagerclasses](http://clang.llvm.org/docs/InternalsManual.html#id12)
Strangely enough, theSourceLocationclass represents a location
 within the source code of the program. Important design points include:
- sizeof(SourceLocation)must be extremely small, as these are embedded into many
 AST nodes and are passed around often. Currently it is 32 bits.
- SourceLocationmust be a simple value object that can be efficiently copied.
- We should be able to represent a source location for any byte of any input file. This includes in the middle of tokens, in whitespace, in trigraphs, etc.
- ASourceLocationmust encode the current#includestack
 that was active when the location was processed. For example, if the location corresponds to a token, it should contain the set of#includes
 active when the token was lexed. This allows us to print the#includestack for a diagnostic.
- SourceLocationmust be able to describe macro expansions, capturing both the
 ultimate instantiation point and the source of the original character data.
In practice, theSourceLocationworks together with theSourceManagerclass
 to encode two pieces of information about a location: its spelling location and its instantiation location. For most tokens, these will be the same. However, for a macro expansion (or tokens that came from a_Pragmadirective)
 these will describe the location of the characters corresponding to the token and the location where the token was used (i.e., the macro instantiation point or the location of the_Pragmaitself).
The Clang front-end inherently depends on the location of a token being tracked correctly. If it is ever incorrect, the front-end may get confused and die. The reason for this is that the notion of the “spelling” of aTokenin
 Clang depends on being able to find the original input characters for the token. This concept maps directly to the “spelling location” for the token.
### [SourceRangeandCharSourceRange](http://clang.llvm.org/docs/InternalsManual.html#id13)
Clang represents most source ranges by [first, last], where “first” and “last” each point to the beginning of their respective tokens. For example consider theSourceRangeof
 the following statement:
x=foo+bar;^first^last
To map from this representation to a character-based representation, the “last” location needs to be adjusted to point to (or past) the end of that token with eitherLexer::MeasureTokenLength()orLexer::getLocForEndOfToken().
 For the rare cases where character-level source ranges information is needed we use theCharSourceRangeclass.
## [The Driver Library](http://clang.llvm.org/docs/InternalsManual.html#id14)
The clang Driver and library are documented[*here*](http://clang.llvm.org/docs/DriverInternals.html).
## [Precompiled Headers](http://clang.llvm.org/docs/InternalsManual.html#id15)
Clang supports two implementations of precompiled headers. The default implementation, precompiled headers ([*PCH*](http://clang.llvm.org/docs/PCHInternals.html))
 uses a serialized representation of Clang’s internal data structures, encoded with the[LLVM
 bitstream format](http://llvm.org/docs/BitCodeFormat.html). Pretokenized headers ([*PTH*](http://clang.llvm.org/docs/PTHInternals.html)), on the other
 hand, contain a serialized representation of the tokens encountered when preprocessing a header (and anything that header includes).
## [The Frontend Library](http://clang.llvm.org/docs/InternalsManual.html#id16)
The Frontend library contains functionality useful for building tools on top of the Clang libraries, for example several methods for outputting diagnostics.
## [The Lexer and Preprocessor Library](http://clang.llvm.org/docs/InternalsManual.html#id17)
The Lexer library contains several tightly-connected classes that are involved with the nasty process of lexing and preprocessing C source code. The main interface to this library for outside clients is the largePreprocessorclass.
 It contains the various pieces of state that are required to coherently read tokens out of a translation unit.
The core interface to thePreprocessorobject (once it is set
 up) is thePreprocessor::Lexmethod, which returns the next[*Token*](http://clang.llvm.org/docs/InternalsManual.html#token)from
 the preprocessor stream. There are two types of token providers that the preprocessor is capable of reading from: a buffer lexer (provided by the[*Lexer*](http://clang.llvm.org/docs/InternalsManual.html#lexer)class)
 and a buffered token stream (provided by the[*TokenLexer*](http://clang.llvm.org/docs/InternalsManual.html#tokenlexer)class).
### [The Token class](http://clang.llvm.org/docs/InternalsManual.html#id18)
TheTokenclass is used to represent a single lexed token. Tokens
 are intended to be used by the lexer/preprocess and parser libraries, but are not intended to live beyond them (for example, they should not live in the ASTs).
Tokens most often live on the stack (or some other location that is efficient to access) as the parser is running, but occasionally do get buffered up. For example, macro definitions are stored as a series of tokens, and the C++
 front-end periodically needs to buffer tokens up for tentative parsing and various pieces of look-ahead. As such, the size of aTokenmatters.
 On a 32-bit system,sizeof(Token)is currently 16 bytes.
Tokens occur in two forms:[*annotation tokens*](http://clang.llvm.org/docs/InternalsManual.html#annotationtoken)and normal tokens. Normal tokens are those returned by the lexer, annotation tokens represent semantic information and are produced by the parser, replacing normal tokens in the token stream. Normal tokens contain the following
 information:
- **A SourceLocation**— This indicates the location of the start of the token.
- **A length**— This stores the length of the token as stored in theSourceBuffer.
 For tokens that include them, this length includes trigraphs and escaped newlines which are ignored by later phases of the compiler. By pointing into the original source buffer, it is always possible to get the original spelling of a token completely accurately.
- **IdentifierInfo**— If a token takes the form of an identifier, and if identifier lookup was enabled when the token was lexed (e.g., the lexer was not reading in “raw” mode) this contains a pointer to the unique hash value for the
 identifier. Because the lookup happens before keyword identification, this field is set even for language keywords like “for”.
- **TokenKind**— This indicates the kind of token as classified by the lexer. This includes things liketok::starequal(for
 the “*=” operator),tok::ampampfor
 the “&&” token, and keyword values (e.g.,tok::kw_for)
 for identifiers that correspond to keywords. Note that some tokens can be spelled multiple ways. For example, C++ supports “operator keywords”, where things like “and”
 are treated exactly like the “&&” operator. In these cases, the kind value is set totok::ampamp,
 which is good for the parser, which doesn’t have to consider both forms. For something that cares about which form is used (e.g., the preprocessor “stringize” operator) the spelling indicates the original form.
- **Flags**— There are currently four flags tracked by the lexer/preprocessor system on a per-token basis:
- **StartOfLine**— This was the first token that occurred on its input source line.
- **LeadingSpace**— There was a space character either immediately before the token or transitively before the token as it was expanded through a macro. The definition of this flag is very closely defined
 by the stringizing requirements of the preprocessor.
- **DisableExpand**— This flag is used internally to the preprocessor to represent identifier tokens which have macro expansion disabled. This prevents them from being considered as candidates for macro expansion
 ever in the future.
- **NeedsCleaning**— This flag is set if the original spelling for the token includes a trigraph or escaped newline. Since this is uncommon, many pieces of code can fast-path on tokens that did not need cleaning.
One interesting (and somewhat unusual) aspect of normal tokens is that they don’t contain any semantic information about the lexed value. For example, if the token was a pp-number token, we do not represent the value of the number
 that was lexed (this is left for later pieces of code to decide). Additionally, the lexer library has no notion of typedef names vs variable names: both are returned as identifiers, and the parser is left to decide whether a specific identifier is a typedef
 or a variable (tracking this requires scope information among other things). The parser can do this translation by replacing tokens returned by the preprocessor with “Annotation Tokens”.
### [Annotation Tokens](http://clang.llvm.org/docs/InternalsManual.html#id19)
Annotation tokens are tokens that are synthesized by the parser and injected into the preprocessor’s token stream (replacing existing tokens) to record semantic information found by the parser. For example, if “foo”
 is found to be a typedef, the “foo”tok::identifiertoken
 is replaced with antok::annot_typename. This is useful for a couple of reasons: 1) this makes it easy to
 handle qualified type names (e.g., “foo::bar::baz<42>::t”) in C++ as a single “token” in the parser. 2) if the parser
 backtracks, the reparse does not need to redo semantic analysis to determine whether a token sequence is a variable, type, template, etc.
Annotation tokens are created by the parser and reinjected into the parser’s token stream (when backtracking is enabled). Because they can only exist in tokens that the preprocessor-proper is done with, it doesn’t need to keep
 around flags like “start of line” that the preprocessor uses to do its job. Additionally, an annotation token may “cover” a sequence of preprocessor tokens (e.g., “a::b::c”
 is five preprocessor tokens). As such, the valid fields of an annotation token are different than the fields for a normal token (but they are multiplexed into the normalTokenfields):
- **SourceLocation “Location”**— TheSourceLocationfor the annotation
 token indicates the first token replaced by the annotation token. In the example above, it would be the location of the “a”
 identifier.
- **SourceLocation “AnnotationEndLoc”**— This holds the location of the last token replaced with the annotation token. In the example above, it would be the location of the “c”
 identifier.
- **void* “AnnotationValue”**— This contains an opaque object that the parser gets fromSema.
 The parser merely preserves the information forSemato later interpret based on the annotation
 token kind.
- **TokenKind “Kind”**— This indicates the kind of Annotation token this is. See below for the different valid kinds.
Annotation tokens currently come in three kinds:
- **tok::annot_typename**: This annotation token represents a resolved typename token that is potentially qualified. TheAnnotationValuefield
 contains theQualTypereturned bySema::getTypeName(),
 possibly with source location information attached.
- **tok::annot_cxxscope**: This annotation token represents a C++ scope specifier, such as “A::B::”.
 This corresponds to the grammar productions “*::*” and “*:: [opt] nested-name-specifier*”. TheAnnotationValuepointer
 is aNestedNameSpecifier*returned by theSema::ActOnCXXGlobalScopeSpecifierandSema::ActOnCXXNestedNameSpecifiercallbacks.
- **tok::annot_template_id**: This annotation token represents a C++ template-id such as “foo<int,4>”,
 where “foo” is the name of a template. TheAnnotationValuepointer
 is a pointer to amalloc‘dTemplateIdAnnotationobject.
 Depending on the context, a parsed template-id that names a type might become a typename annotation token (if all we care about is the named type, e.g., because it occurs in a type specifier) or might remain a template-id token (if we want to retain more source
 location information or produce a new type, e.g., in a declaration of a class template specialization). template-id annotation tokens that refer to a type can be “upgraded” to typename annotation tokens by the parser.
As mentioned above, annotation tokens are not returned by the preprocessor, they are formed on demand by the parser. This means that the parser has to be aware of cases where an annotation could occur and form it where appropriate.
 This is somewhat similar to how the parser handles Translation Phase 6 of C99: String Concatenation (see C99 5.1.1.2). In the case of string concatenation, the preprocessor just returns distincttok::string_literalandtok::wide_string_literaltokens
 and the parser eats a sequence of them wherever the grammar indicates that a string literal can occur.
In order to do this, whenever the parser expects atok::identifierortok::coloncolon,
 it should call theTryAnnotateTypeOrScopeTokenorTryAnnotateCXXScopeTokenmethods
 to form the annotation token. These methods will maximally form the specified annotation tokens and replace the current token with them, if applicable. If the current tokens is not valid for an annotation token, it will remain an identifier or “::”
 token.
### [TheLexerclass](http://clang.llvm.org/docs/InternalsManual.html#id20)
TheLexerclass provides the mechanics of lexing tokens out of
 a source buffer and deciding what they mean. TheLexeris complicated by the fact that it operates
 on raw buffers that have not had spelling eliminated (this is a necessity to get decent performance), but this is countered with careful coding as well as standard performance techniques (for example, the comment handling code is vectorized on X86 and PowerPC
 hosts).
The lexer has a couple of interesting modal features:
- 
The lexer can operate in “raw” mode. This mode has several features that make it possible to quickly lex the file (e.g., it stops identifier lookup, doesn’t specially handle preprocessor tokens, handles EOF differently, etc). This mode is used for lexing within
 an “#if0” block, for example.
- 
The lexer can capture and return comments as tokens. This is required to support the-Cpreprocessor
 mode, which passes comments through, and is used by the diagnostic checker to identifier expect-error annotations.
- 
The lexer can be inParsingFilenamemode, which happens when preprocessing after reading a#includedirective.
 This mode changes the parsing of “<” to return an “angled string” instead of a bunch of tokens for each thing within
 the filename.
- 
When parsing a preprocessor directive (after “#”) theParsingPreprocessorDirectivemode
 is entered. This changes the parser to return EOD at a newline.
- 
TheLexeruses aLangOptionsobject
 to know whether trigraphs are enabled, whether C++ or ObjC keywords are recognized, etc.
In addition to these modes, the lexer keeps track of a couple of other features that are local to a lexed buffer, which change as the buffer is lexed:
- 
TheLexerusesBufferPtrto
 keep track of the current character being lexed.
- 
TheLexerusesIsAtStartOfLineto
 keep track of whether the next lexed token will start with its “start of line” bit set.
- 
TheLexerkeeps track of the current “#if”
 directives that are active (which can be nested).
- 
TheLexerkeeps track of an[*MultipleIncludeOpt*](http://clang.llvm.org/docs/InternalsManual.html#multipleincludeopt)object,
 which is used to detect whether the buffer uses the standard “#ifndefXX/#defineXX”
 idiom to prevent multiple inclusion. If a buffer does, subsequent includes can be ignored if the “XX” macro is defined.
### [TheTokenLexerclass](http://clang.llvm.org/docs/InternalsManual.html#id21)
TheTokenLexerclass is a token provider that returns tokens from
 a list of tokens that came from somewhere else. It typically used for two things: 1) returning tokens from a macro definition as it is being expanded 2) returning tokens from an arbitrary buffer of tokens. The later use is used by_Pragmaand
 will most likely be used to handle unbounded look-ahead for the C++ parser.
### [TheMultipleIncludeOptclass](http://clang.llvm.org/docs/InternalsManual.html#id22)
TheMultipleIncludeOptclass implements a really simple little
 state machine that is used to detect the standard “#ifndefXX/#defineXX”
 idiom that people typically use to prevent multiple inclusion of headers. If a buffer uses this idiom and is subsequently#include‘d,
 the preprocessor can simply check to see whether the guarding condition is defined or not. If so, the preprocessor can completely ignore the include of the header.
## [The Parser Library](http://clang.llvm.org/docs/InternalsManual.html#id23)
## [The AST Library](http://clang.llvm.org/docs/InternalsManual.html#id24)
### [TheTypeclass and its subclasses](http://clang.llvm.org/docs/InternalsManual.html#id25)
TheTypeclass (and its subclasses) are an important part of the
 AST. Types are accessed through theASTContextclass, which implicitly creates and uniques
 them as they are needed. Types have a couple of non-obvious features: 1) they do not capture type qualifiers likeconstorvolatile(see[*QualType*](http://clang.llvm.org/docs/InternalsManual.html#qualtype)),
 and 2) they implicitly capture typedef information. Once created, types are immutable (unlike decls).
Typedefs in C make semantic analysis a bit more complex than it would be without them. The issue is that we want to capture typedef information and represent it in the AST perfectly, but the semantics of operations need to “see
 through” typedefs. For example, consider this code:
voidfunc(){typedefintfoo;fooX,*Y;typedeffoo*bar;barZ;*X;// error**Y;// error**Z;// error}
The code above is illegal, and thus we expect there to be diagnostics emitted on the annotated lines. In this example, we expect to get:
test.c:6:1: error: indirection requires pointer operand ('foo' invalid)
  *X; // error
  ^~
test.c:7:1: error: indirection requires pointer operand ('foo' invalid)
  **Y; // error
  ^~~
test.c:8:1: error: indirection requires pointer operand ('foo' invalid)
  **Z; // error
  ^~~
While this example is somewhat silly, it illustrates the point: we want to retain typedef information where possible, so that we can emit errors about “std::string”
 instead of “std::basic_string<char,std:...”. Doing this requires properly keeping
 typedef information (for example, the type ofXis “foo”,
 not “int”), and requires properly propagating it through the various operators (for example, the type of*Yis
 “foo”, not “int”).
 In order to retain this information, the type of these expressions is an instance of theTypedefTypeclass,
 which indicates that the type of these expressions is a typedef for “foo”.
Representing types like this is great for diagnostics, because the user-specified type is always immediately available. There are two problems with this: first, various semantic checks need to make judgements about the*actual structure*of a type, ignoring typedefs. Second, we need an efficient way to query whether two types are structurally identical to each other, ignoring typedefs. The solution to both of these problems is the idea of canonical types.
#### [Canonical Types](http://clang.llvm.org/docs/InternalsManual.html#id26)
Every instance of theTypeclass contains a canonical type pointer.
 For simple types with no typedefs involved (e.g., “int”, “int*”,
 “int**”), the type just points to itself. For types that have a typedef somewhere in their structure (e.g., “foo”,
 “foo*”, “foo**”,
 “bar”), the canonical type pointer points to their structurally equivalent type without any typedefs (e.g., “int”,
 “int*”, “int**”,
 and “int*” respectively).
This design provides a constant time operation (dereferencing the canonical type pointer) that gives us access to the structure of types. For example, we can trivially tell that “bar”
 and “foo*” are the same type by dereferencing their canonical type pointers and doing a pointer comparison (they both
 point to the single “int*” type).
Canonical types and typedef types bring up some complexities that must be carefully managed. Specifically, theisa/cast/dyn_castoperators
 generally shouldn’t be used in code that is inspecting the AST. For example, when type checking the indirection operator (unary “*”
 on a pointer), the type checker must verify that the operand has a pointer type. It would not be correct to check that with “isa<PointerType>(SubExpr->getType())”,
 because this predicate would fail if the subexpression had a typedef type.
The solution to this problem are a set of helper methods onType, used to check
 their properties. In this case, it would be correct to use “SubExpr->getType()->isPointerType()” to do the check. This
 predicate will return true if the*canonical type is a pointer*, which is true any time the type is structurally a pointer type. The only hard part here is remembering not to use theisa/cast/dyn_castoperations.
The second problem we face is how to get access to the pointer type once we know it exists. To continue the example, the result type of the indirection operator is the pointee type of the subexpression. In order to determine the
 type, we need to get the instance ofPointerTypethat best captures the typedef information in the program.
 If the type of the expression is literally aPointerType, we can return that, otherwise we have to dig through
 the typedefs to find the pointer type. For example, if the subexpression had type “foo*”, we could return that type as
 the result. If the subexpression had type “bar”, we want to return “foo*”
 (note that we do*not*want “int*”). In order to provide all of this,Typehas
 agetAsPointerType()method that checks whether the type is structurally aPointerTypeand,
 if so, returns the best one. If not, it returns a null pointer.
This structure is somewhat mystical, but after meditating on it, it will make sense to you :).
### [TheQualTypeclass](http://clang.llvm.org/docs/InternalsManual.html#id27)
TheQualTypeclass is designed as a trivial value class that is
 small, passed by-value and is efficient to query. The idea ofQualTypeis that it stores the
 type qualifiers (const,volatile,restrict,
 plus some extended qualifiers required by language extensions) separately from the types themselves.QualTypeis
 conceptually a pair of “Type*” and the bits for these type qualifiers.
By storing the type qualifiers as bits in the conceptual pair, it is extremely efficient to get the set of qualifiers on aQualType(just
 return the field of the pair), add a type qualifier (which is a trivial constant-time operation that sets a bit), and remove one or more type qualifiers (just return aQualTypewith
 the bitfield set to empty).
Further, because the bits are stored outside of the type itself, we do not need to create duplicates of types with different sets of qualifiers (i.e. there is only a single heap allocated “int”
 type: “constint” and “volatileconstint”
 both point to the same heap allocated “int” type). This reduces the heap size used to represent bits and also means we
 do not have to consider qualifiers when uniquing types ([*Type*](http://clang.llvm.org/docs/InternalsManual.html#type)does
 not even contain qualifiers).
In practice, the two most common type qualifiers (constandrestrict)
 are stored in the low bits of the pointer to theTypeobject, together with a flag indicating
 whether extended qualifiers are present (which must be heap-allocated). This means thatQualTypeis
 exactly the same size as a pointer.
### [Declaration names](http://clang.llvm.org/docs/InternalsManual.html#id28)
TheDeclarationNameclass represents the name of a declaration
 in Clang. Declarations in the C family of languages can take several different forms. Most declarations are named by simple identifiers, e.g., “f”
 and “x” in the function declarationf(intx).
 In C++, declaration names can also name class constructors (“Class” instructClass{Class();}),
 class destructors (“~Class”), overloaded operator names (“operator+”),
 and conversion functions (“operatorvoidconst*”).
 In Objective-C, declaration names can refer to the names of Objective-C methods, which involve the method name and the parameters, collectively called a*selector*, e.g., “setWidth:height:”.
 Since all of these kinds of entities — variables, functions, Objective-C methods, C++ constructors, destructors, and operators — are represented as subclasses of Clang’s commonNamedDeclclass,DeclarationNameis
 designed to efficiently represent any kind of name.
Given aDeclarationNameN,N.getNameKind()will
 produce a value that describes what kind of nameNstores. There are 10 options (all of the
 names are inside theDeclarationNameclass).
Identifier
> 
The name is a simple identifier. UseN.getAsIdentifierInfo()to retrieve the correspondingIdentifierInfo*pointing
 to the actual identifier.
ObjCZeroArgSelector,ObjCOneArgSelector,ObjCMultiArgSelector
> 
The name is an Objective-C selector, which can be retrieved as aSelectorinstance viaN.getObjCSelector().
 The three possible name kinds for Objective-C reflect an optimization within theDeclarationNameclass:
 both zero- and one-argument selectors are stored as a maskedIdentifierInfopointer, and therefore
 require very little space, since zero- and one-argument selectors are far more common than multi-argument selectors (which use a different structure).
CXXConstructorName
> 
The name is a C++ constructor name. UseN.getCXXNameType()to retrieve the[*type*](http://clang.llvm.org/docs/InternalsManual.html#qualtype)that
 this constructor is meant to construct. The type is always the canonical type, since all constructors for a given type have the same name.
CXXDestructorName
> 
The name is a C++ destructor name. UseN.getCXXNameType()to retrieve the[*type*](http://clang.llvm.org/docs/InternalsManual.html#qualtype)whose
 destructor is being named. This type is always a canonical type.
CXXConversionFunctionName
> 
The name is a C++ conversion function. Conversion functions are named according to the type they convert to, e.g., “operatorvoidconst*”.
 UseN.getCXXNameType()to retrieve the type that this conversion function converts to. This
 type is always a canonical type.
CXXOperatorName
> 
The name is a C++ overloaded operator name. Overloaded operators are named according to their spelling, e.g., “operator+”
 or “operatornew[]”. UseN.getCXXOverloadedOperator()to
 retrieve the overloaded operator (a value of typeOverloadedOperatorKind).
CXXLiteralOperatorName
> 
The name is a C++11 user defined literal operator. User defined Literal operators are named according to the suffix they define, e.g., “_foo”
 for “operator""_foo”. UseN.getCXXLiteralIdentifier()to
 retrieve the correspondingIdentifierInfo*pointing to the identifier.
CXXUsingDirective
> 
The name is a C++ using directive. Using directives are not really NamedDecls, in that they all have the same name, but they are implemented as such in order to store them in DeclContext effectively.
DeclarationNames are cheap to create, copy, and compare. They require only a single pointer’s
 worth of storage in the common cases (identifiers, zero- and one-argument Objective-C selectors) and use dense, uniqued storage for the other kinds of names. TwoDeclarationNames
 can be compared for equality (==,!=)
 using a simple bitwise comparison, can be ordered with<,>,<=,
 and>=(which provide a lexicographical ordering for normal identifiers but an unspecified
 ordering for other kinds of names), and can be placed into LLVMDenseMaps andDenseSets.
DeclarationNameinstances can be created in different ways depending on what
 kind of name the instance will store. Normal identifiers (IdentifierInfopointers) and Objective-C selectors
 (Selector) can be implicitly converted toDeclarationNames.
 Names for C++ constructors, destructors, conversion functions, and overloaded operators can be retrieved from theDeclarationNameTable,
 an instance of which is available asASTContext::DeclarationNames. The member functionsgetCXXConstructorName,getCXXDestructorName,getCXXConversionFunctionName,
 andgetCXXOperatorName, respectively, returnDeclarationNameinstances
 for the four kinds of C++ special function names.
### [Declaration contexts](http://clang.llvm.org/docs/InternalsManual.html#id29)
Every declaration in a program exists within some*declaration context*, such as a translation unit, namespace, class, or function. Declaration contexts in Clang are represented by theDeclContextclass,
 from which the various declaration-context AST nodes (TranslationUnitDecl,NamespaceDecl,RecordDecl,FunctionDecl,
 etc.) will derive. TheDeclContextclass provides several facilities common to each declaration
 context:
Source-centric vs. Semantics-centric View of Declarations
> 
DeclContextprovides two views of the declarations stored within a declaration context. The source-centric
 view accurately represents the program source code as written, including multiple declarations of entities where present (see the section[*Redeclarations and Overloads*](http://clang.llvm.org/docs/InternalsManual.html#redeclarations)), while the semantics-centric view represents the program semantics. The two views are kept synchronized by semantic analysis while the ASTs are being constructed.
Storage of declarations within that context
> 
Every declaration context can contain some number of declarations. For example, a C++ class (represented byRecordDecl)
 contains various member functions, fields, nested types, and so on. All of these declarations will be stored within theDeclContext,
 and one can iterate over the declarations via [DeclContext::decls_begin(),DeclContext::decls_end()).
 This mechanism provides the source-centric view of declarations in the context.
Lookup of declarations within that context
> 
TheDeclContextstructure provides efficient name lookup for names within that declaration
 context. For example, ifNis a namespace we can look for the nameN::fusingDeclContext::lookup.
 The lookup itself is based on a lazily-constructed array (for declaration contexts with a small number of declarations) or hash table (for declaration contexts with more declarations). The lookup operation provides the semantics-centric view of the declarations
 in the context.
Ownership of declarations
> 
TheDeclContextowns all of the declarations that were declared within its declaration
 context, and is responsible for the management of their memory as well as their (de-)serialization.
All declarations are stored within a declaration context, and one can query information about the context in which each declaration lives. One can retrieve theDeclContextthat
 contains a particularDeclusingDecl::getDeclContext.
 However, see the section[*Lexical and Semantic Contexts*](http://clang.llvm.org/docs/InternalsManual.html#lexicalandsemanticcontexts)for more information about how to interpret this context information.
#### [Redeclarations and Overloads](http://clang.llvm.org/docs/InternalsManual.html#id30)
Within a translation unit, it is common for an entity to be declared several times. For example, we might declare a function “f”
 and then later re-declare it as part of an inlined definition:
voidf(intx,inty,intz=);inlinevoidf(intx,inty,intz){/* ...  */}
The representation of “f” differs in the source-centric and semantics-centric views of a
 declaration context. In the source-centric view, all redeclarations will be present, in the order they occurred in the source code, making this view suitable for clients that wish to see the structure of the source code. In the semantics-centric view, only
 the most recent “f” will be found by the lookup, since it effectively replaces the first declaration of “f”.
In the semantics-centric view, overloading of functions is represented explicitly. For example, given two declarations of a function “g”
 that are overloaded, e.g.,
voidg();voidg(int);
theDeclContext::lookupoperation will return aDeclContext::lookup_resultthat
 contains a range of iterators over declarations of “g”. Clients that perform semantic analysis on a program that is not
 concerned with the actual source code will primarily use this semantics-centric view.
#### [Lexical and Semantic Contexts](http://clang.llvm.org/docs/InternalsManual.html#id31)
Each declaration has two potentially different declaration contexts: a*lexical*context, which corresponds to the source-centric view of the declaration context, and a*semantic*context,
 which corresponds to the semantics-centric view. The lexical context is accessible viaDecl::getLexicalDeclContextwhile
 the semantic context is accessible viaDecl::getDeclContext, both of which returnDeclContextpointers.
 For most declarations, the two contexts are identical. For example:
classX{public:voidf(intx);};
Here, the semantic and lexical contexts ofX::fare theDeclContextassociated
 with the classX(itself stored as aRecordDeclAST
 node). However, we can now defineX::fout-of-line:
voidX::f(intx=){/* ...  */}
This definition of “f” has different lexical and semantic contexts. The lexical context
 corresponds to the declaration context in which the actual declaration occurred in the source code, e.g., the translation unit containingX.
 Thus, this declaration ofX::fcan be found by traversing the declarations provided by [decls_begin(),decls_end())
 in the translation unit.
The semantic context ofX::fcorresponds to the classX,
 since this member function is (semantically) a member ofX. Lookup of the namefinto
 theDeclContextassociated withXwill
 then return the definition ofX::f(including information about the default argument).
#### [Transparent Declaration Contexts](http://clang.llvm.org/docs/InternalsManual.html#id32)
In C and C++, there are several contexts in which names that are logically declared inside another declaration will actually “leak” out into the enclosing scope from the perspective of name lookup. The most obvious instance of
 this behavior is in enumeration types, e.g.,
enumColor{Red,Green,Blue};
Here,Coloris an enumeration, which is a declaration context
 that contains the enumeratorsRed,Green,
 andBlue. Thus, traversing the list of declarations contained in the enumerationColorwill
 yieldRed,Green,
 andBlue. However, outside of the scope ofColorone
 can name the enumeratorRedwithout qualifying the name, e.g.,
Colorc=Red;
There are other entities in C++ that provide similar behavior. For example, linkage specifications that use curly braces:
extern"C"{voidf(int);voidg(int);}// f and g are visible here
For source-level accuracy, we treat the linkage specification and enumeration type as a declaration context in which its enclosed declarations (“Red”,
 “Green”, and “Blue”;
 “f” and “g”)
 are declared. However, these declarations are visible outside of the scope of the declaration context.
These language features (and several others, described below) have roughly the same set of requirements: declarations are declared within a particular lexical context, but the declarations are also found via name lookup in scopes
 enclosing the declaration itself. This feature is implemented via*transparent*declaration contexts (seeDeclContext::isTransparentContext()),
 whose declarations are visible in the nearest enclosing non-transparent declaration context. This means that the lexical context of the declaration (e.g., an enumerator) will be the transparentDeclContextitself,
 as will the semantic context, but the declaration will be visible in every outer context up to and including the first non-transparent declaration context (since transparent declaration contexts can be nested).
The transparentDeclContexts are:
- 
Enumerations (but not C++11 “scoped enumerations”):
enumColor{Red,Green,Blue};// Red, Green, and Blue are in scope
- 
C++ linkage specifications:
extern"C"{voidf(int);voidg(int);}// f and g are in scope
- 
Anonymous unions and structs:
structLookupTable{boolIsVector;union{std::vector<Item>*Vector;std::set<Item>*Set;};};LookupTableLT;LT.Vector=;// Okay: finds Vector inside the unnamed union
- 
C++11 inline namespaces:
namespacemylib{inlinenamespacedebug{classX;}}mylib::X*xp;// okay: mylib::X refers to mylib::debug::X
#### [Multiply-Defined Declaration Contexts](http://clang.llvm.org/docs/InternalsManual.html#id33)
C++ namespaces have the interesting — and, so far, unique — property that the namespace can be defined multiple times, and the declarations provided by each namespace definition are effectively merged (from the semantic point of
 view). For example, the following two code snippets are semantically indistinguishable:
// Snippet #1:namespaceN{voidf();}namespaceN{voidf(int);}// Snippet #2:namespaceN{voidf();voidf(int);}
In Clang’s representation, the source-centric view of declaration contexts will actually have two separateNamespaceDeclnodes
 in Snippet #1, each of which is a declaration context that contains a single declaration of “f”. However, the semantics-centric
 view provided by name lookup into the namespaceNfor “f”
 will return aDeclContext::lookup_resultthat contains a range of iterators over declarations
 of “f”.
DeclContextmanages multiply-defined declaration contexts internally. The functionDeclContext::getPrimaryContextretrieves
 the “primary” context for a givenDeclContextinstance, which is theDeclContextresponsible
 for maintaining the lookup table used for the semantics-centric view. Given the primary context, one can follow the chain ofDeclContextnodes
 that define additional declarations viaDeclContext::getNextContext. Note that these functions are used internally within
 the lookup and insertion methods of theDeclContext, so the vast majority of clients can ignore them.
### [TheCFGclass](http://clang.llvm.org/docs/InternalsManual.html#id34)
TheCFGclass is designed to represent a source-level control-flow
 graph for a single statement (Stmt*). Typically instances ofCFGare
 constructed for function bodies (usually an instance ofCompoundStmt), but can also be instantiated to represent
 the control-flow of any class that subclassesStmt, which includes simple expressions. Control-flow graphs
 are especially useful for performing[flow-
 or path-sensitive](http://en.wikipedia.org/wiki/Data_flow_analysis#Sensitivities)program analyses on a given function.
#### [Basic Blocks](http://clang.llvm.org/docs/InternalsManual.html#id35)
Concretely, an instance ofCFGis a collection of basic blocks.
 Each basic block is an instance ofCFGBlock, which simply contains an ordered sequence ofStmt*(each
 referring to statements in the AST). The ordering of statements within a block indicates unconditional flow of control from one statement to the next.[*Conditional control-flow*](http://clang.llvm.org/docs/InternalsManual.html#conditionalcontrolflow)is represented using edges between basic blocks. The statements within a givenCFGBlockcan
 be traversed using theCFGBlock::*iteratorinterface.
ACFGobject owns the instances ofCFGBlockwithin
 the control-flow graph it represents. EachCFGBlockwithin a CFG is also uniquely numbered
 (accessible viaCFGBlock::getBlockID()). Currently the number is based on the ordering the blocks were created,
 but no assumptions should be made on howCFGBlocksare numbered other than their numbers are
 unique and that they are numbered from 0..N-1 (where N is the number of basic blocks in the CFG).
#### [Entry and Exit Blocks](http://clang.llvm.org/docs/InternalsManual.html#id36)
Each instance ofCFGcontains two special blocks: an*entry*block
 (accessible viaCFG::getEntry()), which has no incoming edges, and an*exit*block (accessible
 viaCFG::getExit()), which has no outgoing edges. Neither block contains any statements, and they serve
 the role of providing a clear entrance and exit for a body of code such as a function body. The presence of these empty blocks greatly simplifies the implementation of many analyses built on top of CFGs.
#### [Conditional Control-Flow](http://clang.llvm.org/docs/InternalsManual.html#id37)
Conditional control-flow (such as those induced by if-statements and loops) is represented as edges betweenCFGBlocks.
 Because different C language constructs can induce control-flow, eachCFGBlockalso records
 an extraStmt*that represents the*terminator*of the block.
 A terminator is simply the statement that caused the control-flow, and is used to identify the nature of the conditional control-flow between blocks. For example, in the case of an if-statement, the terminator refers to theIfStmtobject
 in the AST that represented the given branch.
To illustrate, consider the following code example:
intfoo(intx){x=x+;if(x>)x++;else{x+=;x*=;}returnx;}
After invoking the parser+semantic analyzer on this code fragment, the AST of the body offoois
 referenced by a singleStmt*. We can then construct an instance ofCFGrepresenting
 the control-flow graph of this function body by single call to a static class method:
Stmt*FooBody=...CFG*FooCFG=CFG::buildCFG(FooBody);
It is the responsibility of the caller ofCFG::buildCFGtodeletethe
 returnedCFG*when the CFG is no longer needed.
Along with providing an interface to iterate over itsCFGBlocks, theCFGclass
 also provides methods that are useful for debugging and visualizing CFGs. For example, the methodCFG::dump()dumps
 a pretty-printed version of the CFG to standard error. This is especially useful when one is using a debugger such as gdb. For example, here is the output ofFooCFG->dump():
[B5(ENTRY)]Predecessors():Successors():B4[B4]:x=x+:(x>)T:if[B4.2]Predecessors():B5Successors():B3B2[B3]:x++Predecessors():B4Successors():B1[B2]:x+=:x*=Predecessors():B4Successors():B1[B1]:returnx;Predecessors():B2B3Successors():B0[B0(EXIT)]Predecessors():B1Successors():
For each block, the pretty-printed output displays for each block the number of*predecessor*blocks (blocks that have outgoing control-flow to the given block) and*successor*blocks
 (blocks that have control-flow that have incoming control-flow from the given block). We can also clearly see the special entry and exit blocks at the beginning and end of the pretty-printed output. For the entry block (block B5), the number of predecessor
 blocks is 0, while for the exit block (block B0) the number of successor blocks is 0.
The most interesting block here is B4, whose outgoing control-flow represents the branching caused by the sole if-statement infoo.
 Of particular interest is the second statement in the block,(x>2),
 and the terminator, printed asif[B4.2]. The second statement represents
 the evaluation of the condition of the if-statement, which occurs before the actual branching of control-flow. Within theCFGBlockfor
 B4, theStmt*for the second statement refers to the actual expression in the AST for(x>2).
 Thus pointers to subclasses ofExprcan appear in the list of statements in a block, and not
 just subclasses ofStmtthat refer to proper C statements.
The terminator of block B4 is a pointer to theIfStmtobject in
 the AST. The pretty-printer outputsif[B4.2]because
 the condition expression of the if-statement has an actual place in the basic block, and thus the terminator is essentially*referring*to the expression that is the second statement of block B4 (i.e., B4.2). In this manner,
 conditions for control-flow (which also includes conditions for loops and switch statements) are hoisted into the actual basic block.
### [Constant Folding in the Clang AST](http://clang.llvm.org/docs/InternalsManual.html#id38)
There are several places where constants and constant folding matter a lot to the Clang front-end. First, in general, we prefer the AST to retain the source code as close to how the user wrote it as possible. This means that if
 they wrote “5+4”, we want to keep the addition and two constants in the AST, we don’t want to fold to “9”.
 This means that constant folding in various ways turns into a tree walk that needs to handle the various cases.
However, there are places in both C and C++ that require constants to be folded. For example, the C standard defines what an “integer constant expression” (i-c-e) is with very precise and specific requirements. The language then
 requires i-c-e’s in a lot of places (for example, the size of a bitfield, the value for a case statement, etc). For these, we have to be able to constant fold the constants, to do semantic checks (e.g., verify bitfield size is non-negative and that case statements
 aren’t duplicated). We aim for Clang to be very pedantic about this, diagnosing cases when the code does not use an i-c-e where one is required, but accepting the code unless running with-pedantic-errors.
Things get a little bit more tricky when it comes to compatibility with real-world source code. Specifically, GCC has historically accepted a huge superset of expressions as i-c-e’s, and a lot of real world code depends on this
 unfortuate accident of history (including, e.g., the glibc system headers). GCC accepts anything its “fold” optimizer is capable of reducing to an integer constant, which means that the definition of what it accepts changes as its optimizer does. One example
 is that GCC accepts things like “caseX-X:” even whenXis
 a variable, because it can fold this to 0.
Another issue are how constants interact with the extensions we support, such as__builtin_constant_p,__builtin_inf,__extension__and
 many others. C99 obviously does not specify the semantics of any of these extensions, and the definition of i-c-e does not include them. However, these extensions are often used in real code, and we have to have a way to reason about them.
Finally, this is not just a problem for semantic analysis. The code generator and other clients have to be able to fold constants (e.g., to initialize global variables) and has to handle a superset of what C99 allows. Further,
 these clients can benefit from extended information. For example, we know that “foo()||1”
 always evaluates totrue, but we can’t replace the expression withtruebecause
 it has side effects.
#### [Implementation Approach](http://clang.llvm.org/docs/InternalsManual.html#id39)
After trying several different approaches, we’ve finally converged on a design (Note, at the time of this writing, not all of this has been implemented, consider this a design goal!). Our basic approach is to define a single recursive
 method evaluation method (Expr::Evaluate), which is implemented inAST/ExprConstant.cpp.
 Given an expression with “scalar” type (integer, fp, complex, or pointer) this method returns the following information:
- 
Whether the expression is an integer constant expression, a general constant that was folded but has no side effects, a general constant that was folded but that does have side effects, or an uncomputable/unfoldable value.
- 
If the expression was computable in any way, this method returns theAPValuefor the result
 of the expression.
- 
If the expression is not evaluatable at all, this method returns information on one of the problems with the expression. This includes aSourceLocationfor
 where the problem is, and a diagnostic ID that explains the problem. The diagnostic should haveERRORtype.
- 
If the expression is not an integer constant expression, this method returns information on one of the problems with the expression. This includes aSourceLocationfor
 where the problem is, and a diagnostic ID that explains the problem. The diagnostic should haveEXTENSIONtype.
This information gives various clients the flexibility that they want, and we will eventually have some helper methods for various extensions. For example,Semashould
 have aSema::VerifyIntegerConstantExpressionmethod, which callsEvaluateon
 the expression. If the expression is not foldable, the error is emitted, and it would returntrue. If the
 expression is not an i-c-e, theEXTENSIONdiagnostic is emitted. Finally it would returnfalseto
 indicate that the AST is OK.
Other clients can use the information in other ways, for example, codegen can just use expressions that are foldable in any way.
#### [Extensions](http://clang.llvm.org/docs/InternalsManual.html#id40)
This section describes how some of the various extensions Clang supports interacts with constant evaluation:
- __extension__: The expression form of this extension causes any evaluatable subexpression to be accepted as an integer
 constant expression.
- __builtin_constant_p: This returns true (as an integer constant expression) if the operand evaluates to either a numeric
 value (that is, not a pointer cast to integral type) of integral, enumeration, floating or complex type, or if it evaluates to the address of the first character of a string literal (possibly cast to some other type). As a special case, if__builtin_constant_pis
 the (potentially parenthesized) condition of a conditional operator expression (“?:”), only the true side of the conditional
 operator is considered, and it is evaluated with full constant folding.
- __builtin_choose_expr: The condition is required to be an integer constant expression, but we accept any constant as an
 “extension of an extension”. This only evaluates one operand depending on which way the condition evaluates.
- __builtin_classify_type: This always returns an integer constant expression.
- __builtin_inf,nan,...: These are treated
 just like a floating-point literal.
- __builtin_abs,copysign,...: These are constant
 folded as general constant expressions.
- __builtin_strlenandstrlen:
 These are constant folded as integer constant expressions if the argument is a string literal.
## [How to change Clang](http://clang.llvm.org/docs/InternalsManual.html#id41)
### [How to add an attribute](http://clang.llvm.org/docs/InternalsManual.html#id42)
To add an attribute, you’ll have to add it to the list of attributes, add it to the parsing phase, and look for it in the AST scan.[r124217](http://llvm.org/viewvc/llvm-project?view=rev&revision=124217)has
 a good example of adding a warning attribute.
(Beware that this hasn’t been reviewed/fixed by the people who designed the attributes system yet.)
#### [include/clang/Basic/Attr.td](http://clang.llvm.org/docs/InternalsManual.html#id43)
First, add your attribute to the[include/clang/Basic/Attr.td
 file](http://llvm.org/viewvc/llvm-project/cfe/trunk/include/clang/Basic/Attr.td?view=markup).
Each attribute gets adefinheriting fromAttror
 one of its subclasses.InheritableAttrmeans that the attribute also applies to subsequent
 declarations of the same name.
Spellingslists the strings that can appear in__attribute__((here))or[[here]].
 All such strings will be synonymous. If you want to allow the[[]]C++11 syntax, you have to define a list
 ofNamespaces, which will let users write[[namespace::spelling]].
 Using the empty string for a namespace will allow users to write just the spelling with no “::”. Attributes which g++-4.8
 accepts should also have aCXX11<"gnu","spelling">spelling.
Subjectsrestricts what kinds of AST node to which this attribute can appertain
 (roughly, attach).
Argsnames the arguments the attribute takes, in order. IfArgsis[StringArgument<"Arg1">,IntArgument<"Arg2">]then__attribute__((myattribute("Hello",3)))will
 be a valid use.
#### [Boilerplate](http://clang.llvm.org/docs/InternalsManual.html#id44)
Write a newHandleYourAttr()function in[lib/Sema/SemaDeclAttr.cpp](http://llvm.org/viewvc/llvm-project/cfe/trunk/lib/Sema/SemaDeclAttr.cpp?view=markup),
 and add a case to the switch inProcessNonInheritableDeclAttr()orProcessInheritableDeclAttr()forwarding
 to it.
If your attribute causes extra warnings to fire, define aDiagGroupin[include/clang/Basic/DiagnosticGroups.td](http://llvm.org/viewvc/llvm-project/cfe/trunk/include/clang/Basic/DiagnosticGroups.td?view=markup)named
 after the attribute’sSpellingwith “_”s replaced by “-“s. If you’re only defining one diagnostic, you can
 skipDiagnosticGroups.tdand useInGroup<DiagGroup<"your-attribute">>directly
 in[DiagnosticSemaKinds.td](http://llvm.org/viewvc/llvm-project/cfe/trunk/include/clang/Basic/DiagnosticSemaKinds.td?view=markup)
#### [The meat of your attribute](http://clang.llvm.org/docs/InternalsManual.html#id45)
Find an appropriate place in Clang to do whatever your attribute needs to do. Check for the attribute’s presence usingDecl::getAttr<YourAttr>().
Update the[*Clang Language Extensions*](http://clang.llvm.org/docs/LanguageExtensions.html)document to describe your new attribute.
### [How to add an expression or statement](http://clang.llvm.org/docs/InternalsManual.html#id46)
Expressions and statements are one of the most fundamental constructs within a compiler, because they interact with many different parts of the AST, semantic analysis, and IR generation. Therefore, adding a new expression or statement
 kind into Clang requires some care. The following list details the various places in Clang where an expression or statement needs to be introduced, along with patterns to follow to ensure that the new expression or statement works well across all of the C
 languages. We focus on expressions, but statements are similar.
- Introduce parsing actions into the parser. Recursive-descent parsing is mostly self-explanatory, but there are a few things that are worth keeping in mind:
- 
Keep as much source location information as possible! You’ll want it later to produce great diagnostics and support Clang’s various features that map between source code and the AST.
- 
Write tests for all of the “bad” parsing cases, to make sure your recovery is good. If you have matched delimiters (e.g., parentheses, square brackets, etc.), useParser::BalancedDelimiterTrackerto
 give nice diagnostics when things go wrong.
- Introduce semantic analysis actions intoSema. Semantic analysis should always
 involve two functions: anActOnXXXfunction that will be called directly from the parser, and
 aBuildXXXfunction that performs the actual semantic analysis and will (eventually!) build
 the AST node. It’s fairly common for theActOnCXXfunction to do very little (often just some
 minor translation from the parser’s representation toSema‘s representation of the same thing), but the separation is
 still important: C++ template instantiation, for example, should always call theBuildXXXvariant.
 Several notes on semantic analysis before we get into construction of the AST:
- 
Your expression probably involves some types and some subexpressions. Make sure to fully check that those types, and the types of those subexpressions, meet your expectations. Add implicit conversions where necessary to make sure that all of the types line
 up exactly the way you want them. Write extensive tests to check that you’re getting good diagnostics for mistakes and that you can use various forms of subexpressions with your expression.
- 
When type-checking a type or subexpression, make sure to first check whether the type is “dependent” (Type::isDependentType())
 or whether a subexpression is type-dependent (Expr::isTypeDependent()). If any of these returntrue,
 then you’re inside a template and you can’t do much type-checking now. That’s normal, and your AST node (when you get there) will have to deal with this case. At this point, you can write tests that use your expression within templates, but don’t try to instantiate
 the templates.
- 
For each subexpression, be sure to callSema::CheckPlaceholderExpr()to deal with “weird” expressions
 that don’t behave well as subexpressions. Then, determine whether you need to perform lvalue-to-rvalue conversions (Sema::DefaultLvalueConversions)
 or the usual unary conversions (Sema::UsualUnaryConversions), for places where the subexpression is producing a value
 you intend to use.
- 
YourBuildXXXfunction will probably just returnExprError()at
 this point, since you don’t have an AST. That’s perfectly fine, and shouldn’t impact your testing.
- Introduce an AST node for your new expression. This starts with declaring the node ininclude/Basic/StmtNodes.tdand
 creating a new class for your expression in the appropriateinclude/AST/Expr*.hheader. It’s
 best to look at the class for a similar expression to get ideas, and there are some specific things to watch for:
- 
If you need to allocate memory, use theASTContextallocator to allocate memory. Never use rawmallocornew,
 and never hold any resources in an AST node, because the destructor of an AST node is never called.
- 
Make sure thatgetSourceRange()covers the exact source range of your expression. This is needed
 for diagnostics and for IDE support.
- 
Make sure thatchildren()visits all of the subexpressions. This is important for a number of
 features (e.g., IDE support, C++ variadic templates). If you have sub-types, you’ll also need to visit those sub-types in theRecursiveASTVisitor.
- 
Add printing support (StmtPrinter.cpp) and dumping support (StmtDumper.cpp)
 for your expression.
- 
Add profiling support (StmtProfile.cpp) for your AST node, noting the distinguishing (non-source location) characteristics
 of an instance of your expression. Omitting this step will lead to hard-to-diagnose failures regarding matching of template declarations.
- Teach semantic analysis to build your AST node. At this point, you can wire up yourSema::BuildXXXfunction
 to actually create your AST. A few things to check at this point:
- 
If your expression can construct a new C++ class or return a new Objective-C object, be sure to update and then callSema::MaybeBindToTemporaryfor
 your just-created AST node to be sure that the object gets properly destructed. An easy way to test this is to return a C++ class with a private destructor: semantic analysis should flag an error here with the attempt to call the destructor.
- 
Inspect the generated AST by printing it usingclang-cc1-ast-print,
 to make sure you’re capturing all of the important information about how the AST was written.
- 
Inspect the generated AST underclang-cc1-ast-dumpto
 verify that all of the types in the generated AST line up the way you want them. Remember that clients of the AST should never have to “think” to understand what’s going on. For example, all implicit conversions should show up explicitly in the AST.
- 
Write tests that use your expression as a subexpression of other, well-known expressions. Can you call a function using your expression as an argument? Can you use the ternary operator?
- Teach code generation to create IR to your AST node. This step is the first (and only) that requires knowledge of LLVM IR. There are several things to keep in mind:
- 
Code generation is separated into scalar/aggregate/complex and lvalue/rvalue paths, depending on what kind of result your expression produces. On occasion, this requires some careful factoring of code to avoid duplication.
- CodeGenFunctioncontains functionsConvertTypeandConvertTypeForMemthat
 convert Clang’s types (clang::Type*orclang::QualType)
 to LLVM types. Use the former for values, and the later for memory locations: test with the C++ “bool” type to check
 this. If you find that you are having to use LLVM bitcasts to make the subexpressions of your expression have the type that your expression expects, STOP! Go fix semantic analysis and the AST so that you don’t need these bitcasts.
- 
TheCodeGenFunctionclass has a number of helper functions to make certain operations easy,
 such as generating code to produce an lvalue or an rvalue, or to initialize a memory location with a given value. Prefer to use these functions rather than directly writing loads and stores, because these functions take care of some of the tricky details for
 you (e.g., for exceptions).
- 
If your expression requires some special behavior in the event of an exception, look at thepush*Cleanupfunctions
 inCodeGenFunctionto introduce a cleanup. You shouldn’t have to deal with exception-handling
 directly.
- 
Testing is extremely important in IR generation. Useclang-cc1-emit-llvmand[FileCheck](http://llvm.org/docs/CommandGuide/FileCheck.html)to
 verify that you’re generating the right IR.
- Teach template instantiation how to cope with your AST node, which requires some fairly simple code:
- 
Make sure that your expression’s constructor properly computes the flags for type dependence (i.e., the type your expression produces can change from one instantiation to the next), value dependence (i.e., the constant value your expression produces can change
 from one instantiation to the next), instantiation dependence (i.e., a template parameter occurs anywhere in your expression), and whether your expression contains a parameter pack (for variadic templates). Often, computing these flags just means combining
 the results from the various types and subexpressions.
- 
AddTransformXXXandRebuildXXXfunctions
 to theTreeTransformclass template inSema.TransformXXXshould
 (recursively) transform all of the subexpressions and types within your expression, usinggetDerived().TransformYYY.
 If all of the subexpressions and types transform without error, it will then call theRebuildXXXfunction,
 which will in turn callgetSema().BuildXXXto perform semantic analysis and build your expression.
- 
To test template instantiation, take those tests you wrote to make sure that you were type checking with type-dependent expressions and dependent types (from step #2) and instantiate those templates with various types, some of which type-check and some that
 don’t, and test the error messages in each case.
- There are some “extras” that make other features work better. It’s worth handling these extras to give your expression complete integration into Clang:
- 
Add code completion support for your expression inSemaCodeComplete.cpp.
- 
If your expression has types in it, or has any “interesting” features other than subexpressions, extend libclang’sCursorVisitorto
 provide proper visitation for your expression, enabling various IDE features such as syntax highlighting, cross-referencing, and so on. Thec-index-testhelper
 program can be used to test these features.

