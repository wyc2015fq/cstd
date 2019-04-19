# The lexer hack - SHINING的博客 - CSDN博客
2014年02月24日 16:26:52[snsn1984](https://me.csdn.net/snsn1984)阅读数：1487标签：[Lexer																[Hack																[User-defined type																[identifier																[the lexer hack](https://so.csdn.net/so/search/s.do?q=the lexer hack&t=blog)](https://so.csdn.net/so/search/s.do?q=identifier&t=blog)](https://so.csdn.net/so/search/s.do?q=User-defined type&t=blog)](https://so.csdn.net/so/search/s.do?q=Hack&t=blog)](https://so.csdn.net/so/search/s.do?q=Lexer&t=blog)
个人分类：[编译技术及LLVM](https://blog.csdn.net/snsn1984/article/category/1249418)
# The lexer hack
From Wikipedia, the free encyclopedia
In[computer programming](http://en.wikipedia.org/wiki/Computer_programming),**the lexer hack**(as opposed to "a lexer hack") describes a common solution to the problems in[parsing](http://en.wikipedia.org/wiki/Parsing)[ANSI
 C](http://en.wikipedia.org/wiki/ANSI_C), due to the reference[lexical grammar](http://en.wikipedia.org/wiki/Lexical_grammar)being context-sensitive. In C, classifying
 a sequence of characters as a variable name or a type name requires contextual information of the phrase structure, which prevents one from having a context-free[lexer](http://en.wikipedia.org/wiki/Lexical_analysis)based
 only on the[regular grammar](http://en.wikipedia.org/wiki/Regular_grammar)of token classes.
## Contents
 [[hide](http://en.wikipedia.org/wiki/The_lexer_hack#)] 
- [1Problem](http://en.wikipedia.org/wiki/The_lexer_hack#Problem)
- [2The
 hack solution](http://en.wikipedia.org/wiki/The_lexer_hack#The_hack_solution)
- [3Alternative
 solutions](http://en.wikipedia.org/wiki/The_lexer_hack#Alternative_solutions)
- [4See
 also](http://en.wikipedia.org/wiki/The_lexer_hack#See_also)
- [5References](http://en.wikipedia.org/wiki/The_lexer_hack#References)
- [6Citations](http://en.wikipedia.org/wiki/The_lexer_hack#Citations)
## Problem[[edit](http://en.wikipedia.org/w/index.php?title=The_lexer_hack&action=edit&section=1)]
The problem is that in the following code, the lexical class of`A`cannot be determined without
 further contextual information:
(A)*B
This code could be multiplication of two variables, in which case`A`is a`variable`;
 written unambiguously:
A * B
Alternatively, it could be casting the dereferenced value of`B`to the type`A`,
 in which case`A`is a`typedef-name`; written in
 usual human-readable form, but still ambiguously from the point of view of the grammar:
(A)*B
In more detail, in a[compiler](http://en.wikipedia.org/wiki/Compiler), the lexer performs one
 of the earliest stages of converting the[source code](http://en.wikipedia.org/wiki/Source_code)to a program. It scans the text to
 extract meaningful*tokens*, such as words, numbers, and strings. The parser analyzes sequences of tokens attempting to match them to syntax rules representing language structures, such as loops and variable declarations. A problem occurs
 here if a single sequence of tokens can ambiguously match more than one syntax rule.
This ambiguity can happen in C if the lexer does not distinguish between variable and[typedef](http://en.wikipedia.org/wiki/Typedef)identifiers.[[1]](http://en.wikipedia.org/wiki/The_lexer_hack#cite_note-Roskind_91-1)For
 example, in the C expression:
(A)* B
the lexer may find these tokens:
- left parenthesis
- identifier 'A'
- right parenthesis
- operator '*'
- identifier 'B'
The problem is precisely that the lexical class of*A*cannot be determined without further context: the parser can interpret this as variable*A*multiplied
 by*B*or as type*A*casting the dereferenced value of*B*. This is known as the "typedef-name: identifier" problem, due to the name of the problematic[production
 rule](http://en.wikipedia.org/wiki/Production_(computer_science)).[[2]](http://en.wikipedia.org/wiki/The_lexer_hack#cite_note-Bendersky_07-2)
## The hack solution[[edit](http://en.wikipedia.org/w/index.php?title=The_lexer_hack&action=edit&section=2)]
The solution generally consists of feeding information from the parser's[symbol
 table](http://en.wikipedia.org/wiki/Symbol_table)back into the lexer. That is, rather than functioning as a pure one-way[pipeline](http://en.wikipedia.org/wiki/Pipeline_(software))from
 the lexer to the parser, there is a[backchannel](http://en.wikipedia.org/wiki/Return_channel)from the parser back to the lexer.
 This mixing of the lexer and parser is generally regarded as inelegant, which is why it is called a "hack".
Without added context, the lexer cannot distinguish type identifiers from other identifiers without extra context because all identifiers have the same format. With the hack in the above example, when the
 lexer finds the identifier*A*it should be able to classify the token as a type identifier. The rules of the language would be clarified by specifying that typecasts require a type identifier and the ambiguity disappears.
The problem also exists in[C++](http://en.wikipedia.org/wiki/C%2B%2B)and parsers can
 use the same hack.[[1]](http://en.wikipedia.org/wiki/The_lexer_hack#cite_note-Roskind_91-1)
## Alternative solutions[[edit](http://en.wikipedia.org/w/index.php?title=The_lexer_hack&action=edit&section=3)]
This problem does not arise (and hence needs no "hack" in order to solve) when using[lexerless
 parsing](http://en.wikipedia.org/wiki/Lexerless_parsing)techniques, as these are intrinsically contextual. These are generally seen as less elegant designs, however, because they lack the modularity of having a[concurrent](http://en.wikipedia.org/wiki/Concurrent_computation)lexer
 and parser in a pipeline.
Some parser generators, such as the[yacc](http://en.wikipedia.org/wiki/Yacc)-derived BtYacc ("Backtracking
 Yacc"), give the generated parser the ability to try multiple attempts to parse the tokens. In the problem described here, if an attempt fails because of semantic information about the identifier, it can backtrack and attempt other rules.[[3]](http://en.wikipedia.org/wiki/The_lexer_hack#cite_note-3)
The relatively new[LLVM](http://en.wikipedia.org/wiki/LLVM)-based parser[Clang](http://en.wikipedia.org/wiki/Clang)handles
 the situation a completely different way, namely by using a non-reference lexical grammar. Clang's lexer does not attempt to differentiate between type names and variable names: it simply reports the current token as an identifier. The parser then uses Clang's[semantic
 analysis](http://en.wikipedia.org/wiki/Semantic_analysis_(compilers))library to determine the nature of the identifier. This allows a much cleaner[separation
 of concerns](http://en.wikipedia.org/wiki/Separation_of_concerns)and[encapsulation](http://en.wikipedia.org/wiki/Encapsulation)of the lexer and parser, and is therefore
 considered a much more elegant solution than The Lexer Hack by most modern software design metrics.[[4]](http://en.wikipedia.org/wiki/The_lexer_hack#cite_note-4)This
 is also the approach used in most other modern languages, which do not distinguish different classes of identifiers in the lexical grammar, but instead defer them to the parsing or semantic analysis phase, when sufficient information is available.
## See also[[edit](http://en.wikipedia.org/w/index.php?title=The_lexer_hack&action=edit&section=4)]
- [Dangling else](http://en.wikipedia.org/wiki/Dangling_else)
## References[[edit](http://en.wikipedia.org/w/index.php?title=The_lexer_hack&action=edit&section=5)]
- ^[Jump
 up to:**a**](http://en.wikipedia.org/wiki/The_lexer_hack#cite_ref-Roskind_91_1-0)[**b**](http://en.wikipedia.org/wiki/The_lexer_hack#cite_ref-Roskind_91_1-1)Roskind,
 James A. (1991-07-11).["A YACC-able C++
 2.1 GRAMMAR, AND THE RESULTING AMBIGUITIES"](http://www.cs.utah.edu/research/projects/mso/goofie/grammar5.txt).
- **[Jump up^](http://en.wikipedia.org/wiki/The_lexer_hack#cite_ref-Bendersky_07_2-0)**Bendersky, Eli (2007-11-24).["The
 context sensitivity of C's grammar"](http://eli.thegreenplace.net/2007/11/24/the-context-sensitivity-of-cs-grammar/).
- **[Jump up^](http://en.wikipedia.org/wiki/The_lexer_hack#cite_ref-3)**["BtYacc
 3.0"](http://www.siber.com/btyacc/).Based
 on yacc with modifications by Chris Dodd and Vadim Maslov.
- **[Jump up^](http://en.wikipedia.org/wiki/The_lexer_hack#cite_ref-4)**Bendersky, Eli.["How
 Clang handles the type / variable name ambiguity of C/C++"](http://eli.thegreenplace.net/2012/07/05/how-clang-handles-the-type-variable-name-ambiguity-of-cc/).
## Citations[[edit](http://en.wikipedia.org/w/index.php?title=The_lexer_hack&action=edit&section=6)]
- [http://www.cs.berkeley.edu/~smcpeak/elkhound/sources/elkhound/index.html](http://www.cs.berkeley.edu/~smcpeak/elkhound/sources/elkhound/index.html)
- [http://cs.nyu.edu/rgrimm/papers/pldi06.pdf](http://cs.nyu.edu/rgrimm/papers/pldi06.pdf)
- [http://cens.ioc.ee/local/man/CompaqCompilers/ladebug/ladebug-manual-details.html](http://cens.ioc.ee/local/man/CompaqCompilers/ladebug/ladebug-manual-details.html)
- [http://www.springerlink.com/index/YN4GQ2YMNQUY693L.pdf](http://www.springerlink.com/index/YN4GQ2YMNQUY693L.pdf)
- [http://news.gmane.org/find-root.php?group=gmane.comp.lang.groovy.jsr&article=843&type=blog](http://news.gmane.org/find-root.php?group=gmane.comp.lang.groovy.jsr&article=843&type=blog)
- [http://groups.google.com/group/comp.compilers/browse_frm/thread/db7f68e9d8b49002/fa20bf5de9c73472?lnk=st&q=%2B%22the+lexer+hack%22&rnum=1&hl=en#fa20bf5de9c73472](http://groups.google.com/group/comp.compilers/browse_frm/thread/db7f68e9d8b49002/fa20bf5de9c73472?lnk=st&q=%2B%22the+lexer+hack%22&rnum=1&hl=en#fa20bf5de9c73472)
