# Google Summer of Code: C++ Modernizer Improvements----Monday, November 18, 2013 - SHINING的博客 - CSDN博客
2013年12月09日 13:54:53[snsn1984](https://me.csdn.net/snsn1984)阅读数：1357标签：[c++																[c++11																[compiler																[Clang																[Modernizer](https://so.csdn.net/so/search/s.do?q=Modernizer&t=blog)](https://so.csdn.net/so/search/s.do?q=Clang&t=blog)](https://so.csdn.net/so/search/s.do?q=compiler&t=blog)](https://so.csdn.net/so/search/s.do?q=c++11&t=blog)](https://so.csdn.net/so/search/s.do?q=c++&t=blog)
个人分类：[C++																[编译技术及LLVM](https://blog.csdn.net/snsn1984/article/category/1249418)](https://blog.csdn.net/snsn1984/article/category/488621)
原文地址：[http://blog.llvm.org/2013/11/google-summer-of-code-c-modernizer.html](http://blog.llvm.org/2013/11/google-summer-of-code-c-modernizer.html)
[Google
 Summer of Code](http://all.yumingyemaibuqila.info/browse.php?u=819ac3aa7bcd7ebOi8vd3d3Lmdvb2dsZS1tZWxhbmdlLmNvbS9nc29jL2hvbWVwYWdlL2dvb2dsZS9nc29jMjAxMw%3D%3D&b=13) (GSoC) offers students stipends to participate in open source projects during the summer. This year, I was accepted to work on the [Clang
 C++ Modernizer](http://all.yumingyemaibuqila.info/browse.php?u=819ac3aa7bcd7ebOi8vY2xhbmcubGx2bS5vcmcvZXh0cmEvY2xhbmctbW9kZXJuaXplLmh0bWw%3D&b=13), a project formerly known as the *C++11 Migrator*, driven by a team at Intel. The goals of the tool are to modernize C++ code by using the new features of new C++ standards in order to improve maintainability, readability and compile
 time and runtime performance. The project was featured in the April blog post “[Status
 of the C++11 Migrator](http://all.yumingyemaibuqila.info/browse.php?u=819ac3aa7bcd7ebOi8vYmxvZy5sbHZtLm9yZy8yMDEzLzA0L3N0YXR1cy1vZi1jMTEtbWlncmF0b3IuaHRtbA%3D%3D&b=13)” and has been evolving since, both in terms of architecture and features.
This article presents the improvements made to the tool in the last few months, which include my work from this summer for GSoC. For a complete overview of the tool and how to install it, please visit the documentation: [http://clang.llvm.org/extra/clang-modernize.html#getting-started](http://all.yumingyemaibuqila.info/browse.php?u=819ac3aa7bcd7ebOi8vY2xhbmcubGx2bS5vcmcvZXh0cmEvY2xhbmctbW9kZXJuaXplLmh0bWw%3D&b=13#getting-started).
 For a demonstration of the tool you can take a look at the Going Native 2013 talk given by Chandler Carruth: [The
 Care and Feeding of C++'s Dragons](http://all.yumingyemaibuqila.info/browse.php?u=819ac3aa7bcd7ebOi8vY2hhbm5lbDkubXNkbi5jb20vRXZlbnRzL0dvaW5nTmF0aXZlLzIwMTMvVGhlLUNhcmUtYW5kLUZlZWRpbmctb2YtQy1zLURyYWdvbnM%3D&b=13). clang-modernize is featured starting at ~33min.
## Transform all Files That Make up a Translation Unit
A major improvement since the last version is the ability to transform every file that composes a translation unit not only the main source file. This means headers also get transformed if they need to be which makes the modernizer more useful.
To avoid changing files that shouldn’t be changed, e.g. system headers or headers for third-party libraries, there are a few options to control which files should be transformed:
- [-include](http://all.yumingyemaibuqila.info/browse.php?u=819ac3aa7bcd7ebOi8vY2xhbmcubGx2bS5vcmcvZXh0cmEvTW9kZXJuaXplclVzYWdlLmh0bWw%3D&b=13#cmdoption-include) Takes a comma-separated
 list of paths allowed to be transformed. All files within the entire directory tree rooted at each given path are marked as modifiable. For safety, the default behaviour is that no extra files will be transformed.
- [-exclude](http://all.yumingyemaibuqila.info/browse.php?u=819ac3aa7bcd7ebOi8vY2xhbmcubGx2bS5vcmcvZXh0cmEvTW9kZXJuaXplclVzYWdlLmh0bWw%3D&b=13#cmdoption-exclude) Takes a comma-separated
 list of paths forbidden to be transformed. Can be used to prune out subtrees from included directory trees.
- [-include-from](http://all.yumingyemaibuqila.info/browse.php?u=819ac3aa7bcd7ebOi8vY2xhbmcubGx2bS5vcmcvZXh0cmEvTW9kZXJuaXplclVzYWdlLmh0bWw%3D&b=13#cmdoption-include-from) and [-exclude-from](http://all.yumingyemaibuqila.info/browse.php?u=819ac3aa7bcd7ebOi8vY2xhbmcubGx2bS5vcmcvZXh0cmEvTW9kZXJuaXplclVzYWdlLmh0bWw%3D&b=13#cmdoption-exclude-from) Respectively
 equivalent to -include and -exclude but takes a filename as argument instead of a comma-separated list of paths. The file should contain one path per line.
Example, assuming a directory hierarchy of:
- src/foo.cpp
- include/foo.h
- lib/third-party.h
to transform both *foo.cpp* and *foo.h* but leave *third-party.h* as is, you can use one of the following commands:
clang-modernize -include=include/ src/foo.cpp -- -std=c++11 -I include/ -I lib/clang-modernize -include=. -exclude=lib/ src/foo.cpp -- -std=c++11 -I include/ -I lib/
## The Transforms
Right now there is a total of 6 transforms, two of which are new:
- [Add-Override Transform](http://all.yumingyemaibuqila.info/browse.php?u=819ac3aa7bcd7ebOi8vY2xhbmcubGx2bS5vcmcvZXh0cmEvQWRkT3ZlcnJpZGVUcmFuc2Zvcm0uaHRtbA%3D%3D&b=13)
Adds the ‘override’ specifier to overriden member functions.
- [Loop Convert Transform](http://all.yumingyemaibuqila.info/browse.php?u=819ac3aa7bcd7ebOi8vY2xhbmcubGx2bS5vcmcvZXh0cmEvTG9vcENvbnZlcnRUcmFuc2Zvcm0uaHRtbA%3D%3D&b=13)
Makes use of for-ranged based loop.
- [Pass-By-Value Transform](http://all.yumingyemaibuqila.info/browse.php?u=819ac3aa7bcd7ebOi8vY2xhbmcubGx2bS5vcmcvZXh0cmEvUGFzc0J5VmFsdWVUcmFuc2Zvcm0uaHRtbA%3D%3D&b=13) [new]
Replaces const-ref parameters that would benefit from using the pass-by-value idiom.
- [Replace Auto-Ptr Transform](http://all.yumingyemaibuqila.info/browse.php?u=819ac3aa7bcd7ebOi8vY2xhbmcubGx2bS5vcmcvZXh0cmEvUmVwbGFjZUF1dG9QdHJUcmFuc2Zvcm0uaHRtbA%3D%3D&b=13) [new]
Replaces uses of the deprecated std::auto_ptr by std::unique_ptr.
- [Use-Auto Transform](http://all.yumingyemaibuqila.info/browse.php?u=819ac3aa7bcd7ebOi8vY2xhbmcubGx2bS5vcmcvZXh0cmEvVXNlQXV0b1RyYW5zZm9ybS5odG1s&b=13)
Makes use of the auto type specifier in variable declarations.
- [Use-Nullptr Transform](http://all.yumingyemaibuqila.info/browse.php?u=819ac3aa7bcd7ebOi8vY2xhbmcubGx2bS5vcmcvZXh0cmEvVXNlTnVsbHB0clRyYW5zZm9ybS5odG1s&b=13)
Replaces null literals and macros by nullptr where applicable.
### Improvement to Add-Override
Since the last article in April, the Add-Override Transform has been improved to handle user-defined macros. Some projects, like LLVM, use a macro that expands to the ‘override’ specifier for backward compatibility with non-C++11-compliant compilers. clang-modernize
 can detect those macros and use them instead of the ‘override’ identifier.
The command line switch to enable this functionality is **-override-macros**.
Example:
clang-modernize **-override-macros** foo.cpp
|**Before**|**After**|
|----|----|
|#define LLVM_OVERRIDE override**struct** A {**virtual**void foo();};**struct** B : A {**virtual**void foo();};|#define LLVM_OVERRIDE override**struct** A {**virtual**void foo();};**struct** B : A {**virtual**void foo()**LLVM_OVERRIDE**;};|
### Improvement to Use-Nullptr
This transform has also been improved to handle user-defined macros that behave like NULL. The user specifies which macros can be replaced by nullptr by using the command line switch [-user-null-macros=<string>](http://all.yumingyemaibuqila.info/browse.php?u=819ac3aa7bcd7ebOi8vY2xhbmcubGx2bS5vcmcvZXh0cmEvTW9kZXJuaXplclVzYWdlLmh0bWw%3D&b=13#cmdoption-user-null-macros).
Example:
clang-modernize **-user-null-macros=MY_NULL** bar.cpp
|**Before**|**After**|
|----|----|
|#define MY_NULL 0voidbar() {int*p= MY_NULL;}|#define MY_NULL 0voidbar() {int*p=**nullptr**;}|
### New Transform: Replace Auto-Ptr
This transform was a result of GSoC work. The transform replaces uses of std::auto_ptr by std::unique_ptr. It also inserts calls to std::move()when needed.
|**Before**|**After**|
|----|----|
|#include <memory>voidsteal(std::auto_ptr<int> x);voidfoo(int i) { std::auto_ptr<int> p(**new**int(i)); steal(p);}|#include <memory>voidsteal(std::**unique_ptr**<int> x);voidfoo(int i) { std::**unique_ptr**<int> p(**new**int(i)); steal(**std****::****move(**p**)**);}|
### New Transform: Pass-By-Value
Also a product of GSoC this transform makes use of move semantics added in C++11 to avoid a copy for functions that accept types that have move constructors by const reference. By changing to pass-by-value semantics, a copy can be avoided if an rvalue argument
 is provided. For lvalue arguments, the number of copies remains unchanged.
The transform is currently limited to constructor parameters that are copied into class fields.
Example:
clang-modernize pass-by-value.cpp
|**Before**|**After**|
|----|----|
|#include <string>**class****A** {**public:** A(**const** std::string &Copied,**const** std::string&ReadOnly): Copied(Copied),ReadOnly(ReadOnly) {}**private:** std::string Copied;**const** std::string&ReadOnly;};|#include <string>#include <utility>**class****A** {**public:** A(std::string Copied,**const** std::string&ReadOnly): Copied(std::move(Copied)),ReadOnly(ReadOnly) {}**private:** std::string Copied;**const** std::string&ReadOnly;};|
std::move() is a library function declared in <utility>. If need be, this header is added by clang-modernize.
There is a lot of room for improvement in this transform. Other situations that are safe to transform likely exist. Contributions are most welcomed in this area! 
## Usability Improvements
We also worked hard on improving the overall usability of the modernizer. Invoking the modernizer now requires fewer arguments since most of the time the arguments can be inferred. 
- If no compilation database or flags are provided, -std=c++11 is assumed.
- All transforms are enabled by default.
- Files don’t need to be explicitly listed if a compilation database is provided. The modernizer will get files from the compilation database. Use -include to choose which ones.
Two new features were also added.
- Automatically reformat code affected by transforms using [LibFormat](http://all.yumingyemaibuqila.info/browse.php?u=819ac3aa7bcd7ebOi8vY2xhbmcubGx2bS5vcmcvZG9jcy9MaWJGb3JtYXQuaHRtbA%3D%3D&b=13).
- A new command line switch to choose transforms to apply based on compiler support.
### Reformatting Transformed Code
[LibFormat](http://all.yumingyemaibuqila.info/browse.php?u=819ac3aa7bcd7ebOi8vY2xhbmcubGx2bS5vcmcvZG9jcy9MaWJGb3JtYXQuaHRtbA%3D%3D&b=13) is the library used behind the scenes by [clang-format](http://all.yumingyemaibuqila.info/browse.php?u=819ac3aa7bcd7ebOi8vY2xhbmcubGx2bS5vcmcvZG9jcy9DbGFuZ0Zvcm1hdC5odG1s&b=13),
 a tool to format C, C++ and Obj-C code. *clang-modernize* uses this library as well to reformat transformed code. When enabled with -format, the default style is LLVM. The -style option can control the style in a way identical to clang-format.
Example:
**format.cpp**
#include
 <iostream>
#include <vector>
voidf(**const**
 std::vector<int>&my_container) {
**for** (std::vector<int>::const_iterator
 I = my_container.begin(),
                                       E = my_container.end();
      I != E;
++I) {
   std::cout
<<*I
<< std::endl;
 }
}
**Without reformatting**
$ clang-modernize -use-auto format.cpp
#include
 <iostream>
#include <vector>
voidf(**const**
 std::vector<int>&my_container) {
**for** (**auto**
 I = my_container.begin(),
                                       E = my_container.end();
      I != E;
++I) {
   std::cout
<<*I
<< std::endl;
 }
}
**With reformatting**
$ clang-modernize -format -style=LLVM -use-auto format.cpp
#include
 <iostream>
#include <vector>
voidf(**const**
 std::vector<int>&my_container) {
**for** (**auto**
 I = my_container.begin(), E
= my_container.end(); I
!= E;
++I) {
   std::cout
<<*I
<< std::endl;
 }
}
For more information about this option, take a look at the documentation: [Formatting
 Command Line Options](http://all.yumingyemaibuqila.info/browse.php?u=819ac3aa7bcd7ebOi8vY2xhbmcubGx2bS5vcmcvZXh0cmEvTW9kZXJuaXplclVzYWdlLmh0bWw%3D&b=13#formatting-command-line-options).
### Choosing Transforms based on Compiler Support
Another useful command-line switch is: [-for-compilers](http://all.yumingyemaibuqila.info/browse.php?u=819ac3aa7bcd7ebOi8vY2xhbmcubGx2bS5vcmcvZXh0cmEvTW9kZXJuaXplclVzYWdlLmh0bWw%3D&b=13#for-compilers-option).
 This option enables all transforms the given compilers support.
As an example, imagine that your project dropped a dependency to a “legacy” version of a compiler. You can automagically modernize your code to the new minimum versions of the compilers you want to support:
To support Clang >= 3.1, GCC >= 4.6 and MSVC 11:
clang-modernize -format -for-compilers=clang-3.1,gcc-4.6,msvc-11 foo.cpp
For more information about this option and to see which transforms are available for each compilers, please read [the
 documentation](http://all.yumingyemaibuqila.info/browse.php?u=819ac3aa7bcd7ebOi8vY2xhbmcubGx2bS5vcmcvZXh0cmEvTW9kZXJuaXplclVzYWdlLmh0bWw%3D&b=13#cmdoption-for-compilers).
## What’s next?
The ability to transform many translation units in parallel will arrive very soon. Think of *clang-modernize -j* as in make and ninja. Modernization of large code bases will become much faster as a result.
More transforms are coming down the pipe as well as improvements to existing transforms such as the pass-by-value transform.
We will continue fixing bugs and adding new features. Our backlog is publically available: [https://cpp11-migrate.atlassian.net/secure/RapidBoard.jspa?rapidView=1&view=planning](http://all.yumingyemaibuqila.info/browse.php?u=819ac3aa7bcd7ebczovL2NwcDExLW1pZ3JhdGUuYXRsYXNzaWFuLm5ldC9zZWN1cmUvUmFwaWRCb2FyZC5qc3BhP3JhcGlkVmlldz0xJnZpZXc9cGxhbm5pbmc%3D&b=13)
## Get involved!
Interested by the tool? Found a bug? Have an idea of a transform that can be useful to others? The project is Open Source and contributions are most welcomed!
The modernizer has its own bug and project tracker. If you want to file or fix a bug just go to: [https://cpp11-migrate.atlassian.net](http://all.yumingyemaibuqila.info/browse.php?u=819ac3aa7bcd7ebczovL2NwcDExLW1pZ3JhdGUuYXRsYXNzaWFuLm5ldC8%3D&b=13)
A few other addresses to keep in mind: 
- [Clang C++ Modernizer User’s Manual](http://all.yumingyemaibuqila.info/browse.php?u=819ac3aa7bcd7ebOi8vY2xhbmcubGx2bS5vcmcvZXh0cmEvY2xhbmctbW9kZXJuaXplLmh0bWw%3D&b=13)
- IRC channel: #llvm on irc.oftc.net
- Mailing lists:
- [cfe-dev](http://all.yumingyemaibuqila.info/browse.php?u=819ac3aa7bcd7ebOi8vbGlzdHMuY3MudWl1Yy5lZHUvbWFpbG1hbi9saXN0aW5mby9jZmUtZGV2&b=13) for questions and general discussions
- [cfe-commits](http://all.yumingyemaibuqila.info/browse.php?u=819ac3aa7bcd7ebOi8vbGlzdHMuY3MudWl1Yy5lZHUvbWFpbG1hbi9saXN0aW5mby9jZmUtY29tbWl0cw%3D%3D&b=13) for patches
- [Phabricator](http://all.yumingyemaibuqila.info/browse.php?u=819ac3aa7bcd7ebOi8vbGx2bS1yZXZpZXdzLmNoYW5kbGVyYy5jb20v&b=13) to submit patches
## Final word
Finally I want to thank my mentor Edwin Vane and his team at Intel, Tareq Siraj and Ariel Bernal, for the great support they provided me. Also thanks to the LLVM community and Google Summer of Code team for giving me this opportunity to work on the C++ Modernizer
 this summer.
-- Guillaume Papin
Posted by Guillaume Papinat [4:12
 AM](http://all.yumingyemaibuqila.info/browse.php?u=819ac3aa7bcd7ebOi8vYmxvZy5sbHZtLm9yZy8yMDEzLzExL2dvb2dsZS1zdW1tZXItb2YtY29kZS1jLW1vZGVybml6ZXIuaHRtbA%3D%3D&b=13)![](http://all.yumingyemaibuqila.info/browse.php?u=819ac3aa7bcd7ebOi8vaW1nMi5ibG9nYmxvZy5jb20vaW1nL2ljb24xOF9lZGl0X2FsbGJrZy5naWY%3D&b=13)
Labels: [C++](http://all.yumingyemaibuqila.info/browse.php?u=819ac3aa7bcd7ebOi8vYmxvZy5sbHZtLm9yZy9zZWFyY2gvbGFiZWwvQyUyQiUyQg%3D%3D&b=13), [Clang](http://all.yumingyemaibuqila.info/browse.php?u=819ac3aa7bcd7ebOi8vYmxvZy5sbHZtLm9yZy9zZWFyY2gvbGFiZWwvQ2xhbmc%3D&b=13), [GSoC](http://all.yumingyemaibuqila.info/browse.php?u=819ac3aa7bcd7ebOi8vYmxvZy5sbHZtLm9yZy9zZWFyY2gvbGFiZWwvR1NvQw%3D%3D&b=13), [modernizer](http://all.yumingyemaibuqila.info/browse.php?u=819ac3aa7bcd7ebOi8vYmxvZy5sbHZtLm9yZy9zZWFyY2gvbGFiZWwvbW9kZXJuaXplcg%3D%3D&b=13)
