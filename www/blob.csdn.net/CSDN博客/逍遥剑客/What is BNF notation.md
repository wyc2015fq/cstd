# What is BNF notation? - 逍遥剑客 - CSDN博客
2006年12月01日 00:28:00[xoyojank](https://me.csdn.net/xoyojank)阅读数：1988

BNF is an acronym for "Backus Naur Form". John Backus and
Peter Naur introduced for the first time a formal notation
to describe the syntax of a given language (This was for the description
of the ALGOL 60
  programming language, see [Naur 60]). 
To be precise,  most of BNF was introduced by Backus in a report presented at 
an earlier UNESCO conference on ALGOL 58.  Few read the report, but when 
Peter Naur read it he was surprised at some of the differences he found 
between his and Backus's interpretation of ALGOL 58. He decided that for the 
successor to ALGOL, all participants of the first design had come to 
recognize some weaknesses, should be given in a similar form so that all 
participants should be aware of what they were agreeing to.  He made a 
few modificiations that are almost universally used and drew up on his 
own the BNF for ALGOL 60 at the meeting where it was designed.  Depending 
on how you attribute presenting it to the world, it was either by Backus 
in 59 or Naur in 60. (For more details on this period of 
programming languages history, see the
introduction to Backus's Turing award article in Communications of the 
ACM, Vol. 21, No. 8, august 1978. This note was suggested by 
William B. Clodius
from Los Alamos Natl. Lab).
Since then, almost every author of books on new programming languages
used it to specify the syntax rules of the language. See [Jensen 74] and [Wirth 82] for examples.
 The following is taken from [Marcotty 86]:
--------------------------------------------------------------------------------
The meta-symbols of BNF are:
::=meaning "is defined as"
|  meaning "or"
< >angle brackets used to surround category names.
The angle brackets distinguish syntax rules names (also called
non-terminal symbols) from terminal symbols which are written exactly
as they are to be represented. A BNF rule defining a nonterminal has the form:
nonterminal ::= sequence_of_alternatives consisting of strings of
                terminals or nonterminals separated by the meta-symbol |
For example, the BNF production for a mini-language is:
 ::=  program
               begin
               end ;
This shows that a mini-language program consists of the keyword "program"
followed by the declaration sequence, then the keyword "begin" and the
statements sequence, finally the keyword "end" and a semicolon. 
(end of quotation)--------------------------------------------------------------------------------
In fact, many authors have introduced some slight extensions of BNF for the
ease of use:
 optional items are enclosed in meta symbols [ and ], example:
 ::=  if  then
                    [ else
                          ]
                    end if ;
 repetitive items (zero or more times) are enclosed in meta 
symbols { and }, example:
 ::=  {  |  }
 this rule is equivalent to the recursive rule:
 ::=  |
                  [  |  ]
terminals of only one character are surrounded by quotes (") to
     distinguish them from meta-symbols, example:
 ::=  { ";"  }
in recent text books, terminal and non-terminal symbols are distingued
   by using bold faces for terminals and suppressing  < and > around
   non-terminals. This improves greatly the readability. The example then becomes:
if_statement ::= if boolean_expression then
                     statement_sequence
                 [ else
                     statement_sequence ]
                 end if ";"
Now as a last example (maybe not the easiest to read !), here is the
definition of BNF expressed in BNF:
syntax     ::=  { rule }
rule       ::=  identifier  "::="  expression
expression ::=  term { "|" term }
term       ::=  factor { factor }
factor     ::=  identifier |
                quoted_symbol |
                "("  expression  ")" |
                "["  expression  "]" |
                "{"  expression  "}"
identifier ::=  letter { letter | digit }
quoted_symbol ::= """ { any_character } """
BNF is not ony important to describe syntax rules in books, but it is
very commonly used (with variants) by syntactic tools. See for example
any book on LEX and YACC, 
the standard UNIX parser generators. If you
have access to any Unix machine, you will probably find a chapter of
the documentation on these tools. 
--------------------------------------------------------------------------------
Some references:
[Naur 60]
 NAUR, Peter (ed.), "Revised Report on the Algorithmic
     Language ALGOL 60.", Communications of the ACM, Vol. 3 No.5, pp. 
     299-314, May 1960.
[Jensen 74]
 JENSEN, Kathleen, WIRTH, Niklaus, 
     "PASCAL user manual and report", 
      Lecture notes in computer science ; vol. 18.,
      Berlin [etc.] : Springer, 1974., 1974.
[Johnson 75]
 S.C. Johnson, 
     "Yacc: Yet Another Compiler Compiler", 
     Computer Science Technical Report #32, 
     Bell Laboratories, 
     Murray Hill, NJ, 1975. 
[Wirth 82]
WIRTH, Niklaus., 
    Programming in Modula-2,
    Berlin, Heidelberg: Springer, 1982.
[Marcotty 86] 
M. Marcotty & H. Ledgard, 
The World of Programming Languages, 
Springer-Verlag, 
Berlin 1986., pages 41 and following.
