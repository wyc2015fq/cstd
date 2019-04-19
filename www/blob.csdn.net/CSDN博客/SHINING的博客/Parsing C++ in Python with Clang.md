# Parsing C++ in Python with Clang - SHINING的博客 - CSDN博客
2014年05月16日 11:21:56[snsn1984](https://me.csdn.net/snsn1984)阅读数：6561标签：[clang																[binding																[python																[parse																[AST](https://so.csdn.net/so/search/s.do?q=AST&t=blog)](https://so.csdn.net/so/search/s.do?q=parse&t=blog)](https://so.csdn.net/so/search/s.do?q=python&t=blog)](https://so.csdn.net/so/search/s.do?q=binding&t=blog)](https://so.csdn.net/so/search/s.do?q=clang&t=blog)
个人分类：[编译技术及LLVM](https://blog.csdn.net/snsn1984/article/category/1249418)
## 原文地址：http://eli.thegreenplace.net/2011/07/03/parsing-c-in-python-with-clang/
## [Parsing C++ in Python with Clang](http://eli.thegreenplace.net/2011/07/03/parsing-c-in-python-with-clang/)
July 3rd, 2011 at 5:15 am 
People that need to parse and analyze C code in Python are usually really excited to run into
[pycparser](http://code.google.com/p/pycparser/). However, when the task is to parse C++, 
pycparser is not the solution. When I get asked about plans to support C++ in
pycparser, my usual answer is – there are no such plans
[[1]](http://eli.thegreenplace.net/2011/07/03/parsing-c-in-python-with-clang/comment-page-1/#id6), you should look elsewhere. Specifically, at [Clang](http://clang.llvm.org/).
Clang is a front-end compiler for C, C++ and Objective C. It’s a liberally licensed open-source project backed by Apple, which uses it for its own tools. Along with its parent project – the LLVM compiler backend, Clang starts to become a formidable alternative
 to gcc itself these days. The dev team behind Clang (and LLVM) is top-notch and its source is one of the best designed bodies of C++ code in the wild. Clang’s development is very active, closely following the latest C++ standards.
So what I point people to when I’m asked about C++ parsing is Clang. There’s a slight problem with that, however. People like
pycparser*because it’s Python*, and Clang’s API is C++ – which is not the most high-level hacking friendly language out there, to say the least.
### libclang
Enter libclang. Not so long ago, the Clang team wisely recognized that Clang can be used not only as a compiler proper, but also as a tool for analyzing C/C++/ObjC code. In fact, Apple’s own Xcode development tools use Clang
 as a library under the hood for code completion, cross-referencing, and so on.
The component through which Clang enables such usage is called 
libclang. It’s a C API [[2]](http://eli.thegreenplace.net/2011/07/03/parsing-c-in-python-with-clang/comment-page-1/#id7) that the Clang team vows to keep relatively stable, allowing the user to examine parsed code at the level of an abstract syntax tree (AST)
[[3]](http://eli.thegreenplace.net/2011/07/03/parsing-c-in-python-with-clang/comment-page-1/#id8).
More technically, libclang is a shared library that packages Clang with a public-facing API defined in a single C header file:
clang/include/clang-c/Index.h.
### Python bindings to libclang
libclang comes with Python bindings, which reside in
clang/bindings/python, in module 
clang.cindex. This module relies on ctypes to load the dynamic
libclang library and tries to wrap as much of 
libclang as possible with a Pythonic API.
### Documentation?
Unfortunately, the state of documentation for libclang and its Python bindings is dire. The official documentation according to the devs is the source (and auto-generated Doxygen HTML). In addition, all I could find online
 is a [presentation](http://llvm.org/devmtg/2010-11/Gregor-libclang.pdf) and a couple of outdated email messages from the Clang dev mailing list.
On the bright side, if you just skim the Index.h header file keeping in mind what it’s trying to achieve, the API isn’t hard to understand (and neither is the implementation, especially if you’re a bit familiar with Clang’s
 internals). Another place to look things up is the clang/tools/c-index-test tool, which is used to test the API and demonstrates its usage.
For the Python bindings, there is absolutely no documentation as well, except the source plus a couple of examples that are distributed alongside it. So I hope this article will be helpful!
### Setting up
Setting up usage of the Python bindings is very easy:
- Your script needs to be able to find the clang.cindex module. So either copy it appropriately or set up
PYTHONPATH to point to it [[4]](http://eli.thegreenplace.net/2011/07/03/parsing-c-in-python-with-clang/comment-page-1/#id9).
- clang.cindex needs to be able to find the 
libclang.so shared library. Depending on how you build/install Clang, you will need to copy it appropriately or set up
LD_LIBRARY_PATH to point to its location. On Windows, this is
libclang.dll and it should be on 
PATH.
That arranged, you’re ready to import clang.cindex and start rolling.
### Simple example
Let’s start with a simple example. The following script uses the Python bindings of
libclang to find all references to some type in a given file:
#!/usr/bin/env python""" Usage: call with <filename> <typename>"""importsysimportclang.cindexdeffind_typerefs(node, typename):
    """ Find all references to the type named 'typename'    """if node.kind.is_reference():
        ref_node = clang.cindex.Cursor_ref(node)
        if ref_node.spelling == typename:
            print'Found %s [line=%s, col=%s]' % (
                typename, node.location.line, node.location.column)
    # Recurse for children of this nodefor c in node.get_children():
        find_typerefs(c, typename)
index = clang.cindex.Index.create()
tu = index.parse(sys.argv[1])
print'Translation unit:', tu.spelling
find_typerefs(tu.cursor, sys.argv[2])
Suppose we invoke it on this dummy C++ code:
classPerson {
};
classRoom {
public:
    void add_person(Person person)
    {
        // do stuff
    }
private:
    Person* people_in_room;
};
template <classT, int N>
classBag<T, N> {
};
int main()
{
    Person* p = new Person();
    Bag<Person, 42> bagofpersons;
    return0;
}
Executing to find referenced to type Person, we get:
Translation unit: simple_demo_src.cpp
Found Person [line=7, col=21]
Found Person [line=13, col=5]
Found Person [line=24, col=5]
Found Person [line=24, col=21]
Found Person [line=25, col=9]
### Understanding how it works
To see what the example does, we need to understand its inner workings on 3 levels:
- Conceptual level – what is the information we’re trying to pull from the parsed source and how it’s stored
- libclang level – the formal C API of 
libclang, since it’s much better documented (albeit only in comments in the source) than the Python bindings
- The Python bindings, since this is what we directly invoke
#### Creating the index and parsing the source
We’ll start at the beginning, with these lines:
index = clang.cindex.Index.create()
tu = index.parse(sys.argv[1])
An "index" represents a set of translation units compiled and linked together. We need some way of grouping several translation units if we want to reason across them. For example, we may want to find references to some type defined in a header file, in
 a set of other source files. Index.create() invokes the C API function
clang_createIndex.
Next, we use Index‘s 
parse method to parse a single translation unit from a file. This invokes 
clang_parseTranslationUnit, which is a key function in the C API. Its comment says:
> 
This routine is the main entry point for the Clang C API, providing the ability to parse a source file into a translation unit that can then be queried by other functions in the API.
This is a powerful function – it can optionally accept the full set of flags normally passed to the command-line compiler. It returns an opaque
CXTranslationUnit object, which is encapsulated in the Python bindings as
TranslationUnit. This 
TranslationUnit can be queried, for example the name of the translation unit is available in the
spelling property:
print'Translation unit:', tu.spelling
Its most important property is, however, cursor. A
*cursor* is a key abstraction in libclang, it represents some node in the AST of a parsed translation unit. The cursor unifies the different kinds of entities in a program under a single abstraction, providing a common
 set of operations, such as getting its location and children cursors. 
TranslationUnit.cursor returns the top-level cursor of the translation unit, which serves as the stating point for exploring its AST. I will use the terms
*cursor* and *node* interchangeably from this point on.
#### Working with cursors
The Python bindings encapsulate the libclang cursor in the
Cursor object. It has many attributes, the most interesting of which are:
- kind – an enumeration specifying the kind of AST node this cursor points at
- spelling – the source-code name of the node
- location – the source-code location from which the node was parsed
- get_children – its children nodes
get_children requires special explanation, because this is a particular point at which the C and Python APIs diverge.
The libclang C API is based on the idea of *visitors*. To walk the AST from a given cursor, the user code provides a callback function to
clang_visitChildren. This function is then invoked on all descendants of a given AST node.
The Python bindings, on the other hand, encapsulate visiting internally, and provide a more Pythonic iteration API via
Cursor.get_children, which returns the children nodes (cursors) of a given cursor. It’s still possible to access the original visitation APIs directly through Python, but using
get_children is much more convenient. In our example, we use
get_children to recursively visit all the children of a given node:
for c in node.get_children():
    find_typerefs(c, typename)
### Some limitations of the Python bindings
Unfortunately, the Python bindings aren’t complete and still have some bugs, because it is a work in progress. As an example, suppose we want to find and report all the function calls in this file:
bool foo()
{
    returntrue;
}
void bar()
{
    foo();
    for (int i = 0; i < 10; ++i)
        foo();
}
int main()
{
    bar();
    if (foo())
        bar();
}
Let’s write this code:
importsysimportclang.cindexdefcallexpr_visitor(node, parent, userdata):
    if node.kind == clang.cindex.CursorKind.CALL_EXPR:
        print'Found %s [line=%s, col=%s]' % (
                node.spelling, node.location.line, node.location.column)
    return2# means continue visiting recursively
index = clang.cindex.Index.create()
tu = index.parse(sys.argv[1])
clang.cindex.Cursor_visit(
        tu.cursor,
        clang.cindex.Cursor_visit_callback(callexpr_visitor),
        None)
This time we’re using the libclang visitation API directly. The result is:
Found None [line=8, col=5]
Found None [line=10, col=9]
Found None [line=15, col=5]
Found None [line=16, col=9]
Found None [line=17, col=9]
While the reported locations are fine, why is the node name 
None? After some perusal of libclang‘s code, it turns out that for expressions, we shouldn’t be printing the
*spelling*, but rather the *display name*. In the C API it means 
clang_getCursorDisplayName and not clang_getCursorSpelling. But, alas, the Python bindings don’t have
clang_getCursorDisplayName exposed!
We won’t let this stop us, however. The source code of the Python bindings is quite straightforward, and simply uses
ctypes to expose additional functions from the C API. Adding these lines to
bindings/python/clang/cindex.py:
Cursor_displayname = lib.clang_getCursorDisplayName
Cursor_displayname.argtypes = [Cursor]
Cursor_displayname.restype = _CXString
Cursor_displayname.errcheck = _CXString.from_result
And we can now use Cursor_displayname. Replacing
node.spelling by clang.cindex.Cursor_displayname(node) in the script, we now get the desired output:
Found foo [line=8, col=5]
Found foo [line=10, col=9]
Found bar [line=15, col=5]
Found foo [line=16, col=9]
Found bar [line=17, col=9]
*Update (06.07.2011):* Inspired by this article, I submitted a patch to the Clang project to expose
Cursor_displayname, as well as to fix a few other problems with the Python bindings. It was committed by Clang’s core devs in revision 134460 and should now be available from trunk.
### Some limitations of libclang
As we have seen above, limitations in the Python bindings are relatively easy to overcome. Since
libclang provides a straightforward C API, it’s just a matter of exposing additional functionality with appropriate
ctypes constructs. To anyone even moderately experienced with Python, this isn’t a big problem.
Some limitations are in libclang itself, however. For example, suppose we wanted to find all the return statements in a chunk of code. Turns out this isn’t possible through the current API of
libclang. A cursory look at the 
Index.h header file reveals why.
enum CXCursorKind enumerates the kinds of cursors (nodes) we may encounter via
libclang. This is the portion related to statements:
/* Statements */
CXCursor_FirstStmt                     = 200,
/** * \brief A statement whose specific kind is not exposed via this * interface. * * Unexposed statements have the same operations as any other kind of * statement; one can extract their location information, spelling, * children, etc. However, the specific kind of the statement is not * reported. */
CXCursor_UnexposedStmt                 = 200,
/** \brief A labelled statement in a function. * * This cursor kind is used to describe the "start_over:" label statement in * the following example: * * \code *   start_over: *     ++counter; * \endcode * */
CXCursor_LabelStmt                     = 201,
CXCursor_LastStmt                      = CXCursor_LabelStmt,
Ignoring the placeholders CXCursor_FirstStmt and
CXCursor_LastStmt which are used for validity testing, the only statement recognized here is the label statement. All other statements are going to be represented with
CXCursor_UnexposedStmt.
To understand the reason for this limitation, it’s constructive to ponder the main goal of
libclang. Currently, this API’s main use is in IDEs, where we want to know everything about types and references to symbols, but don’t particularly care what kind of statement or expression we see
[[5]](http://eli.thegreenplace.net/2011/07/03/parsing-c-in-python-with-clang/comment-page-1/#id10).
Forgunately, from discussions in the Clang dev mailing lists it can be gathered that these limitations aren’t really intentional. Things get added to
libclang on a per-need basis. Apparently no one needed to discern different statement kinds through
libclang yet, so no one added this feature. If it’s important enough for someone, he can feel free to suggest a patch to the mailing list. In particular, this specific limitation (lack of statement kinds) is especially easy
 to overcome. Looking at cxcursor::MakeCXCursor in
libclang/CXCursor.cpp, it’s obvious how these "kinds" are generated (comments are mine):
CXCursor cxcursor::MakeCXCursor(Stmt *S, Decl *Parent,
                                CXTranslationUnit TU) {
  assert(S && TU && "Invalid arguments!");
  CXCursorKind K = CXCursor_NotImplemented;
  switch (S->getStmtClass()) {
  case Stmt::NoStmtClass:
    break;
  case Stmt::NullStmtClass:
  case Stmt::CompoundStmtClass:
  case Stmt::CaseStmtClass:
  ... // many other statement classescase Stmt::MaterializeTemporaryExprClass:
    K = CXCursor_UnexposedStmt;
    break;
  case Stmt::LabelStmtClass:
    K = CXCursor_LabelStmt;
    break;
  case Stmt::PredefinedExprClass:
  .. //  many other statement classescase Stmt::AsTypeExprClass:
    K = CXCursor_UnexposedExpr;
    break;
  .. // more code
This is simply a mega-switch on Stmt.getStmtClass() (which is Clang’s
*internal* statement class), and only for Stmt::LabelStmtClass there is a kind that isn’t
CXCursor_UnexposedStmt. So recognizing additional "kinds" is trivial:
- Add another enum value to CXCursorKind, between
CXCursor_FirstStmt and 
CXCursor_LastStmt
- Add another case to the switch in cxcursor::MakeCXCursor to recognize the appropriate class and return this kind
- Expose the enumeration value in (1) to the Python bindings
### Conclusion
Hopefully this article has been a useful introduction to 
libclang‘s Python bindings (and libclang itself along the way). Although there is a dearth of external documentation for these components, they are well written and commented, and their source code is thus straightforward
 enough to be reasonably self-documenting.
It’s very important to keep in mind that these APIs wrap an extremely powerful C/C++/ObjC parser engine that is being very actively developed. In my personal opinion, Clang is one’s best bet for an up-to-date open-source C++ parsing library these days. Nothing
 else comes even close.
A small fly in the ointment is some limitations in 
libclang itself and its Python bindings. These are a by-product of 
libclang being a relatively recent addition to Clang, which itself is a very young project.
Fortunately, as I hope this article demonstrated, these limitations aren’t terribly difficult to work around. Only a small amount of Python and C expertise is required to extend the Python bindings, while a bit of understanding of Clang lays the path to
 enhancements to libclang itself. In addition, since
libclang is still being actively developed, I’m quite confident that this API will keep improving over time, so it will have less and less limitations and omissions as time goes by.
![http://eli.thegreenplace.net/wp-content/uploads/hline.jpg](http://eli.thegreenplace.net/wp-content/uploads/hline.jpg)
|[[1]](http://eli.thegreenplace.net/2011/07/03/parsing-c-in-python-with-clang/comment-page-1/#id1)|For me, there are a few reasons for not wanting to get into C++ parsing. First, I like my projects being born from a need. I needed to parse C, sopycparser was created. I have no need parsing C++. Second,[as hard](http://eli.thegreenplace.net/2011/05/02/the-context-sensitivity-of-c%E2%80%99s-grammar-revisited/) as C is to parse, C++ is much harder since its grammar is even more ambiguous. Third, a great tool for parsing C++ already exists – Clang.|
|[[2]](http://eli.thegreenplace.net/2011/07/03/parsing-c-in-python-with-clang/comment-page-1/#id2)|C for better interoperability with non C/C++ based languages and tools. For example, the Python bindings would be much harder to implement on top of a C++ API.|
|[[3]](http://eli.thegreenplace.net/2011/07/03/parsing-c-in-python-with-clang/comment-page-1/#id3)|The key word here is *stable*. While Clang as a whole is designed in a library-based approach and its parts can be used directly, these are internal APIs the development team isn’t obliged to keep stable between releases.|
|[[4]](http://eli.thegreenplace.net/2011/07/03/parsing-c-in-python-with-clang/comment-page-1/#id4)|Note that the Python bindings are part of the *source distribution* of Clang.|
|[[5]](http://eli.thegreenplace.net/2011/07/03/parsing-c-in-python-with-clang/comment-page-1/#id5)|Expression kinds are also severely limited in libclang.|
Related posts:
- [Adventures in parsing C: ASTs for switch statements](http://eli.thegreenplace.net/2012/02/03/adventures-in-parsing-c-asts-for-switch-statements/)
- [Basic source-to-source transformation with Clang](http://eli.thegreenplace.net/2012/06/08/basic-source-to-source-transformation-with-clang/)
- [How Clang handles the type / variable name ambiguity
 of C/C++](http://eli.thegreenplace.net/2012/07/05/how-clang-handles-the-type-variable-name-ambiguity-of-cc/)
- [Python internals: adding a new statement to Python](http://eli.thegreenplace.net/2010/06/30/python-internals-adding-a-new-statement-to-python/)
- [Parsing VHDL is [very] hard](http://eli.thegreenplace.net/2009/05/19/parsing-vhdl-is-very-hard/)
