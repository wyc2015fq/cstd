# Design: cpp11-migrate - SHINING的博客 - CSDN博客
2013年12月08日 19:20:59[snsn1984](https://me.csdn.net/snsn1984)阅读数：1305标签：[c++11																[LLVM																[Migrator																[design																[document](https://so.csdn.net/so/search/s.do?q=document&t=blog)](https://so.csdn.net/so/search/s.do?q=design&t=blog)](https://so.csdn.net/so/search/s.do?q=Migrator&t=blog)](https://so.csdn.net/so/search/s.do?q=LLVM&t=blog)](https://so.csdn.net/so/search/s.do?q=c++11&t=blog)
个人分类：[C++																[编译技术及LLVM](https://blog.csdn.net/snsn1984/article/category/1249418)](https://blog.csdn.net/snsn1984/article/category/488621)
# Design: cpp11-migrate
This document describes the design of the tool cpp11-migrate
 used to automatically migrate legacy C++ code to use features of the new C++11 standard. The tool is to be based on clang’s LibTooling.
## Context
C++11 introduces many new convenience and performance features
 over past standards of C++. For many of the new features, introduction into an existing C++ codebase would be straightforward and would yield more performant and easier to maintain code. The only drawback is the amount of developer time required to convert
 existing code.
Fortunately, automatic conversion for many features is possible
 with clang’s LibTooling. One tool already exists in the clang-tools-extra repository: loop-convert, which attempts to make use of range-based for loops where possible. loop-convert uses AST Matchers and Rewriters. Many other features of C++11 can be addressed
 in the same way.
## Goals
- 
A single tool that applies code transformations for all C++11 features with implemented transformations.
- 
A framework where adding support for new transformations is minimally invasive to the tool and
 other transformation implementations.
- 
Transformations can be independently enabled/disabled via the command-line.
## Non-Goals
- 
Refactoring of code to replace hacks and current best-practices with cleaner implementations made
 available by C++11. e.g. using variadic templates to replace multiple template specializations to support N-argument functions.
## Code Location
This tool is built on top of LibTooling and therefore should
 live in the clang-tools-extra repo.
## Implementation
Each transform, an implementation of the application of a particular
 C++11 feature, is registered with a central framework. Registration happens the same way as for LLVM passes. That is, a global variable of type
RegisterTransform<...>
is instantiated providing the class name for the transform and an identifying name suitable for use on the command line. Transforms
 should be subclasses of a base Transform
 class.
The central framework’s responsibility is the construction
 of the CompilationDatabase,
 parsing command-line arguments, and creating each enabled transform by way of registered factories. Each transform is then applied to source files in turn. Syntax checks happen after the application of each transform to identify transforms that failed and
 abort all changes made so far to failing source files. Transforms are applied in the order they are registered.
## Command-Line Interface
Each transform can be enabled or disabled with a command-line
 switch. e.g.:
cpp11-migrate --loop-convert --nullptr-convert
Transforms requiring arguments, like loop convert, can define
 their own command-line arguments local to the transformation implementation thanks to the design of the LLVM CommandLine Library. Such arguments should always be named and be named so to not conflict with arguments defined by other transforms.
cpp11-migrate --loop-convert --loop-convert-risk=A0
## Future Directions
- 
Rewriting to files in memory instead of on disk to improve performance. This is especially useful
 as the number of times Replacements are applied increases.
- 
During registration, transforms can indicate what sort of transformation they provide (e.g. performance,
 maintenance, etc.). This would enable high-level command-line arguments to enable all transforms of given categories.
- 
Support across all transforms for varying levels of risk. Some transforms may have the opportunity
 to make changes that affect semantics. In such cases a user can specify their tolerance to such changes in a way that affects all transforms that support risk levels. All such transforms no longer need to individually provide command-line arguments for risk
 levels.
- 
Handling dependencies between transformations and controlling the order of transformations.
- 
Saving the state of source files after every successful transform so that when an error occurs
 the work of successful transforms is not lost.
- 
Determining a set of transformations enabled by default.
- 
An interactive mode to afford transforms that require user input on the level of each replacement
 for information on intention, style, etc. (e.g. automatic addition of move constructors, using constexpr, adding noexcept to move constructors, etc.) Simpler transforms with risky replacements can also get user approval on a case-by-case basis.
