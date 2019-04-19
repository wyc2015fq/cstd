# How Clang handles the type / variable name ambiguity of C/C++ - SHINING的博客 - CSDN博客
2014年02月24日 16:08:51[snsn1984](https://me.csdn.net/snsn1984)阅读数：1545标签：[文档																[clang																[词法分析器																[Lexer																[Type](https://so.csdn.net/so/search/s.do?q=Type&t=blog)](https://so.csdn.net/so/search/s.do?q=Lexer&t=blog)](https://so.csdn.net/so/search/s.do?q=词法分析器&t=blog)](https://so.csdn.net/so/search/s.do?q=clang&t=blog)](https://so.csdn.net/so/search/s.do?q=文档&t=blog)
个人分类：[编译技术及LLVM](https://blog.csdn.net/snsn1984/article/category/1249418)
## 原文地址： [http://eli.thegreenplace.net/2012/07/05/how-clang-handles-the-type-variable-name-ambiguity-of-cc/](http://eli.thegreenplace.net/2012/07/05/how-clang-handles-the-type-variable-name-ambiguity-of-cc/)
## [How Clang handles the type / variable name ambiguity of C/C++](http://eli.thegreenplace.net/2012/07/05/how-clang-handles-the-type-variable-name-ambiguity-of-cc/)
July 5th, 2012 at 7:35 pm
My previous articles on the context sensitivity and ambiguity of the C/C++ grammar ([one](http://eli.thegreenplace.net/2007/11/24/the-context-sensitivity-of-cs-grammar/),[two](http://eli.thegreenplace.net/2011/05/02/the-context-sensitivity-of-c%E2%80%99s-grammar-revisited/),[three](http://eli.thegreenplace.net/2012/06/28/the-type-variable-name-ambiguity-in-c/))
 can probably make me sound pessimistic about the prospect of correctly parsing C/C++, which couldn’t be farther from the truth. My gripe is not with the grammar itself (although I admit it’s needlessly complex), it’s with the inability of Yacc-generated LALR(1)
 parsers to parse it without considerable hacks. As I’ve mentioned numerous times before, industrial-strength compilers for C/C++ exist after all, so they do manage to somehow parse these languages.
One of the newest, and in my eyes the most exciting of C/C++ compilers is[Clang](http://clang.llvm.org/). Originally
 developed by Apple as a front-end to LLVM, it’s been a vibrant open-source project for the past couple of years with participation from many companies and individuals (although Apple remains the main driving force in the community). Clang, similarly to LLVM,
 features a modular library-based design and a very clean C++ code-base. Clang’s parser is hand-written, based on a standard recursive-descent parsing algorithm.
In this post I want to explain how Clang manages to overcome the ambiguities I mentioned in the previous articles.
### No lexer hack
There is no "lexer hack" in Clang. Information flows in a single direction – from the lexer to the parser, not back. How is this managed?
The thing is that the Clang lexer doesn’t distinguish between user-defined types and other identifiers. All are marked with theidentifiertoken.
For this code:
typedefint mytype;
mytype bb;
The Clang parser encounters the following tokens (-dump-tokens):
typedef 'typedef'   [StartOfLine]   Loc=<z.c:1:1>
int 'int'           [LeadingSpace]  Loc=<z.c:1:9>
identifier 'mytype' [LeadingSpace]  Loc=<z.c:1:13>
semi ';'                            Loc=<z.c:1:19>
identifier 'mytype' [StartOfLine]   Loc=<z.c:2:1>
identifier 'bb'     [LeadingSpace]  Loc=<z.c:2:8>
semi ';'                            Loc=<z.c:2:10>
eof ''                              Loc=<z.c:4:1>
Note howmytypeis always reported
 as an identifier, both before and after Clang figures out it’s actually a user-defined type.
### Figuring out what’s a type
So if the Clang lexer always reportsmytypeas
 an identifier, how does the parser figure out when it is actually a type? By keeping a symbol table.
Well, actually it’s not the parser that keeps the symbol table, it’sSema.Semais
 the Clang module responsible for semantic analysis and AST construction. It gets invoked from the parser through a generic "actions" interface, which in theory could serve a different client. Although conceptually the parser andSemaare
 coupled, the actions interface provides a clean separation in the code. The parser is responsible for driving the parsing process, andSemais
 responsible for handling semantic information. In this particular case, the symbol table*is*semantic information, so it’s handled bySema.
To follow this process through, we’ll start inParser::ParseDeclarationSpecifiers[[1]](http://eli.thegreenplace.net/2012/07/05/how-clang-handles-the-type-variable-name-ambiguity-of-cc/#id5).
 In the C/C++ grammar, type names are part of the "specifiers" in a declaration (that also include things likeexternorinline),
 and following the "recursive-descent protocol", Clang will usually feature a parsing method per grammar rule. When this method encounters an identifier (tok::identifier),
 it asksSemawhether it’s actually a type by callingActions.getTypeName[[2]](http://eli.thegreenplace.net/2012/07/05/how-clang-handles-the-type-variable-name-ambiguity-of-cc/#id6).
Sema::getTypeNamecallsSema::LookupNameto
 do the actual name lookup. For C, name lookup rules are relatively simple – you just climb the lexical scope stack the code belongs to, trying to find a scope that defines the name as a type. I’ve[mentioned
 before](http://eli.thegreenplace.net/2011/05/02/the-context-sensitivity-of-c%E2%80%99s-grammar-revisited/)that all names in C (including type names) obey lexical scoping rules. With this mechanism, Clang implements the required nested symbol table. Note that this symbol table is queried by Clang in places where a type is actually expected
 and allowed, not only in declarations. For example, it’s also done to disambiguate function calls from casts in some cases.
How does a type actually get into this table, though?
When the parser is done parsing atypedef(and
 any declarator, for that matter), it callsSema::ActOnDeclarator.
 When the latter notices a newtypedefand makes sure
 everything about it is kosher (e.g. it does not re-define a name in the same scope), it adds the new name to the symbol table at the current scope.
In Clang’s code this whole process looks very clean and intuitive, but in a generated LALR(1) parser it would be utterly impossible, because leaving out the special token for type names and merging it withidentifierwould
 create a tons of unresolvable reduce-reduce conflicts in the grammar. This is why Yacc-based parsers require a lexer hack to handle this issue.
### Class-wide declarations in C++
In the[previous post](http://eli.thegreenplace.net/2012/06/28/the-type-variable-name-ambiguity-in-c/)I
 mentioned how C++ makes this type lookup problem much more difficult by forcing declarations inside a class to be visible throughout the class, even in code that appears before them. Here’s a short reminder:
int aa(int arg) {
    return arg;
}
classC {
    int foo(int bb) {
        return (aa)(bb);
    }
    typedefint aa;
};
In this code, even though thetypedefappears
 afterfoo, the parser must figure out that(aa)(bb)is
 a cast ofbbto typeaa,
 and not the function callaa(bb).
We’ve seen how Clang can manage to figure out thataais
 a type. However, when it parsesfooit hasn’t even*seen*thetypedefyet,
 so how does that work?
### Delayed parsing of inline method bodies
To solve the problem described above, Clang employs a clever technique. When parsing an inline member function declaration/definition, it does full parsing and semantic analysis of the*declaration*, leaving the*definition*for
 later.
Specifically, the body of an inline method definition is*lexed*and the tokens are kept in a special buffer for later (this is done byParser::ParseCXXInlineMethodDef).
 Once the parser has finished parsing the class, it callsParser::ParseLexedMethodDefsthat
 does the actual parsing and semantic analysis of the saved method bodies. At this point, all the types declared inside the class are available, so the parser can correctly disambiguate wherever required.
### Annotation tokens
Although the above is enough to understand how Clang approaches the problem, I want to mention another trick it uses to make parsing more efficient in some cases.
TheSema::getTypeNamemethod
 mentioned earlier can be costly. It performs a lookup in a set of nested scopes, which may be expensive if the scopes are deeply nested and a name is*not*actually a type (which is probably most often the case). It’s alright
 (and inevitable!) to do this lookup once, but Clang would like to avoid repeating it for the same token when it*backtracks*trying to parse a statement in a different way.
A word on what "backtracks" means in this context.[Recursive
 descent parsers](http://eli.thegreenplace.net/2008/09/26/recursive-descent-ll-and-predictive-parsers/)are naturally (by their very structure) backtracking. That is, they may try a number of different ways to parse a single grammatical production (be that a statement, an expression, a declaration, or whatever), before finding
 an approach that succeeds. In this process, the same token may need to be queried more than once.
To avoid this, Clang has special "annotation tokens" it inserts into the token stream. The mechanism is used for other things as well, but in our case we’re interested in thetok::annot_typenametoken.
 What happens is that the first time the parser encounters atok::identifierand
 figures out it’s a type, this token gets replaced bytok::annot_typename.
 The next time the parser encounters this token, it won’t have to lookup whether it’s a type once again, because it’s no longer a generictok::identifier[[3]](http://eli.thegreenplace.net/2012/07/05/how-clang-handles-the-type-variable-name-ambiguity-of-cc/#id7).
### Disclaimer and conclusion
It’s important to keep in mind that the cases examined in this post do not represent the full complexity of the C++ grammar. In C++, constructs like qualified names (foo::bar::baz)
 and templates complicate matters considerably. However, I just wanted to focus on the cases I specifically discussed in previous posts, explaining how Clang addresses them.
To conclude, we’ve seen how Clang’s recursive descent parser manages some of the ambiguities of the C/C++ grammar. For a task that complex, it’s inevitable for the code to become non-trivial[[4]](http://eli.thegreenplace.net/2012/07/05/how-clang-handles-the-type-variable-name-ambiguity-of-cc/#id8).
 That said, Clang has actually managed to keep its code-base relatively clean and logically structured, while at the same time sticking to its aggressive performance goals. Someone with a general understanding of how front-ends work shouldn’t require more than
 a few hours of immersion in the Clang code-base to be able to answer questions about "how does it do*that*".
![http://eli.thegreenplace.net/wp-content/uploads/hline.jpg](http://eli.thegreenplace.net/wp-content/uploads/hline.jpg)
|[[1]](http://eli.thegreenplace.net/2012/07/05/how-clang-handles-the-type-variable-name-ambiguity-of-cc/#id1)|As a rule, allParsercode lives inlib/Parsein the Clang source tree.Semacode lives inlib/Sema.|
|[[2]](http://eli.thegreenplace.net/2012/07/05/how-clang-handles-the-type-variable-name-ambiguity-of-cc/#id2)|Here and later I’ll skip a lot of details and variations, focusing only on the path I want to use in the example.|
|[[3]](http://eli.thegreenplace.net/2012/07/05/how-clang-handles-the-type-variable-name-ambiguity-of-cc/#id3)|It’s very important to note that only*this instance*of the token in the token stream is replaced. The next instance may have already become a type (or we may have even changed the scope), so it wouldn’t be semantically correct to reason about it.|
|[[4]](http://eli.thegreenplace.net/2012/07/05/how-clang-handles-the-type-variable-name-ambiguity-of-cc/#id4)|That Clang parses Objective-C and various extensions like CUDA or OpenCL in the same code-base doesn’t help in this respect.|
Related posts:
- [The
 type / variable name ambiguity in C++](http://eli.thegreenplace.net/2012/06/28/the-type-variable-name-ambiguity-in-c/)
- [The
 context sensitivity of C’s grammar](http://eli.thegreenplace.net/2007/11/24/the-context-sensitivity-of-cs-grammar/)
- [The
 context sensitivity of C’s grammar, revisited](http://eli.thegreenplace.net/2011/05/02/the-context-sensitivity-of-c%E2%80%99s-grammar-revisited/)
- [Parsing
 C++ in Python with Clang](http://eli.thegreenplace.net/2011/07/03/parsing-c-in-python-with-clang/)
- [Reading
 C type declarations](http://eli.thegreenplace.net/2008/07/18/reading-c-type-declarations/)
