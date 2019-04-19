# Clang AST parsing for automated code generation - SHINING的博客 - CSDN博客
2014年05月15日 10:50:10[snsn1984](https://me.csdn.net/snsn1984)阅读数：3108
原文地址：[http://www.seethroughskin.com/blog/?p=2172](http://www.seethroughskin.com/blog/?p=2172)
Syntax traversal is a powerful tool. With it you can automate repetitive tasks, search for semantic errors, generate wrappers, and so much more.  A few months ago I hit a hump (read: a f***ing mountain) of an issue with some legacy code that has
 been on my plate for awhile now.
Having killed a small forest’s worth of paper I decided that manually tracing paths through code was an inefficient use of my time.  Instead I went in search an automatic method for generating an [abstract
 syntax tree](http://en.wikipedia.org/wiki/Abstract_syntax_tree)(AST) for C++ code.  My idea was that I could use the AST to generate something like a direct graph to better visualize code flow.
There are a few flavors of readable syntax generation out there (and likely more):
- [pycparser](https://bitbucket.org/eliben/pycparser) (Supports on C, I believe)
- [gccxml](http://gccxml.github.io/HTML/Index.html)
- [Clang](http://clang.llvm.org/) (via AST dump)
I’ve been a fan of Clang for awhile now and they have a very robust and active community making it a natural choice for my AST generation needs.  Clang also has decent articles on[getting
 started in both Windows and Linux](http://clang.llvm.org/get_started.html).  If you don’t have Clang installed, I suggest reading that linked article.  You’ll need compiled versions of clang.exe and libclang.dll to follow along with the Python binding below.
## [Caveat]
Clang at revision 183352 (2013-06-05)  has a slight issue in that it won’t identify Linkage specifications (e.g. extern “C” void foo()).  To[fix
 this issue](http://stackoverflow.com/questions/11865486/clang-ast-extern-linkagespec-issue), follow these steps from my SO answer:
```cpp
//Bit of a necroanswer but if you go in to \llvm\tools\clang\lib\Sema\SemaCodeComplete.cpp and add the following line:
 
case Decl::LinkageSpec:  return CXCursor_LinkageSpec;
 
//To the switch in:
CXCursorKind clang::getCursorKindForDecl(const Decl *D)
 
//It should resolve the issue of clang's Python binder 
//returning UNEXPOSED_DECL instead of the correct LINKAGE_SPEC.
//This change was made at revision 183352(2013-06-05).
 
//Example from my version:
CXCursorKind clang::getCursorKindForDecl(const Decl *D) {
if (!D)
    return CXCursor_UnexposedDecl;
 
switch (D-&gt;getKind()) {
    case Decl::Enum:               return CXCursor_EnumDecl; 
    case Decl::LinkageSpec:  return CXCursor_LinkageSpec;
   // ......
```
## [Libclang]
Libclang is Clang’s dynamic binding that is used in conjunction w/ Python to allow for interpreted code evaluation.  Eli Bendersky has a[great
 post](http://eli.thegreenplace.net/2011/07/03/parsing-c-in-python-with-clang/)on using libclang that I referenced frequently while writing code.  Clang documentation can be very lacking in some areas and Eli’s post does a good job of explaining the steps to getting libclang working with Python.  If you follow his
 steps the basic pipeline is:
- Compile libclang
- Add libclang to your PATH environment variable
- On *Nix it’s LD_LIBRARY_PATH
- On Windows it’s the standard PATH
- Or do it in python: os.environ['PATH'] = ‘/path/to/libclang’
- Copy the Clang/Python bindings from /llvm/tools/clang/bindings/python to your python installation or however you’d prefer to install it.
- Verify it works by opening a python console and typing: improt clang.cindex
- Squee when it works
## [Example]
Once libclang is tied to Python it’s time to test your code.  When I got to this step I had trouble finding any good examples.  There are really only 2 and they can be found in your Clang installation folder: llvm\tools\clang\bindings\python\examples\cindex.
  Others can be gleaned from blog posts and StackOverflow.  Here is a simple example I adapted that looks specifically for the LINKAGE_SPEC cursor type. LINKAGE_SPEC refers to code like `extern “C”`
```python
#!/usr/bin/env python
 
import os
import sys
from pprint import pprint
import clang.cindex
os.environ['PATH'] = os.environ['PATH']  + os.getcwd()
 
def get_info(node, depth=0):
	return { 'kind' : node.kind,
             'usr' : node.get_usr(),
             'spelling' : node.spelling,
             'location' : node.location,
             'extent.start' : node.extent.start,
             'extent.end' : node.extent.end,
             'is_definition' : node.is_definition()}
 
def output_cursor_and_children(cursor, level=0):
 
	#LINKAGE_SPEC (http://clang.llvm.org/doxygen/classclang_1_1LinkageSpecDecl.html)
	#Represents code of the type:  extern "C" void foo()
	if cursor.kind == clang.cindex.CursorKind.LINKAGE_SPEC:
		pprint(('nodes', get_info(cursor)))
 
	# Recurse for children of this cursor
	has_children = False;
	for c in cursor.get_children():
		if not has_children:
			has_children = True
		output_cursor_and_children(c, level+1)
 
def main():
	from clang.cindex import Index
	from pprint import pprint
 
	from optparse import OptionParser, OptionGroup
 
	global opts
 
	parser = OptionParser("usage: %prog {filename} [clang-args*]")
	parser.disable_interspersed_args()
	(opts, args) = parser.parse_args()
 
	if len(args) == 0:
		print 'invalid number arguments'
 
	index = Index.create()
	tu = index.parse(None, args)
 
	if not tu:
		print "unable to load input"
 
	output_cursor_and_children(tu.cursor)
 
if __name__ == '__main__':
    main()
```
```cpp
#include "test.h"
 
int main(){
	Foo f;
	return 0;
}
```
```cpp
#ifndef TEST_H
#define TEST_H
 
class Foo 
{
	int data_;
public:
	Foo(){}
 
	void bar(int data){data_ = data;}
};
 
extern "C" __declspec( dllexport )void test1(){}
 
#endif
```
How to run:
python linkage_dump.py test.cpp
![clangarang](http://www.seethroughskin.com/blog/wp-content/uploads/2013/06/clangarang-300x204.png)
## [Conclusion]
There are so many other ways to make use of ASTs and I wish I had more time to include some of them.  Suffice it to say I’ll probably end up posting about ASTs a few more times.  At least until I work through enough examples to meet my immediate
 needs.
