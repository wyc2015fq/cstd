# 80老翁谈人生（285）：沃森电脑与Peolog编程语言 - 袁萌专栏 - CSDN博客
2016年12月12日 21:22:51[yuanmeng001](https://me.csdn.net/yuanmeng001)阅读数：369
**80老翁谈人生（285）：沃森电脑与Peolog编程语言**
**    说句实在话，沃森电脑运行在Suse Linux操作系统之上，使用Prolog语言编程，进行自然语言处理。在当今沃森电脑大行其道的今天，回顾一下Prolog编程语言是有益的。**
**国内关于Prolog语言的教材，不是原汁原味，味道不纯。本文转载Prolog Tutorial如下：（注：请从下面的引言部分开始阅读）**
**PrologTutorial**
J. A. Robinson: Aprogram is a theory (in some logic) and computation is deduction from thetheory.
   N. Wirth: Program = data structure + algorithm
    R. Kowalski: Algorithm = logic +control 
**Introduction to Prolog**
[Introduction](http://cs.wwc.edu/~cs_dept/KU/PR/#intro)
[The Structure of Prolog Program](http://cs.wwc.edu/~cs_dept/KU/PR/#struc)
[Syntax](http://cs.wwc.edu/~cs_dept/KU/PR/#syntax)
[Types](http://cs.wwc.edu/~cs_dept/KU/PR/#types)
[Simple](http://cs.wwc.edu/~cs_dept/KU/PR/#simple)
[Composite](http://cs.wwc.edu/~cs_dept/KU/PR/#composite)
[Expressions](http://cs.wwc.edu/~cs_dept/KU/PR/#expr)
[Unification and Pattern Matchine](http://cs.wwc.edu/~cs_dept/KU/PR/#pat)
[Functions](http://cs.wwc.edu/~cs_dept/KU/PR/#fun)
[Lists](http://cs.wwc.edu/~cs_dept/KU/PR/#list)
[Iteration](http://cs.wwc.edu/~cs_dept/KU/PR/#iteration)
[Iterators, Generators andBacktracking](http://cs.wwc.edu/~cs_dept/KU/PR/#itgenback)
[Tuples](http://cs.wwc.edu/~cs_dept/KU/PR/#tuples)
[Extra-Logical Predicates](http://cs.wwc.edu/~cs_dept/KU/PR/#extralogical)
[Input/Output](http://cs.wwc.edu/~cs_dept/KU/PR/#io)
[Style and Layout](http://cs.wwc.edu/~cs_dept/KU/PR/#style)
**Applications & AdvancedProgramming Techniques**
[Negation and Cuts](http://cs.wwc.edu/~cs_dept/KU/PR/#negation)
[Definite Clause Grammars](http://cs.wwc.edu/~cs_dept/KU/PR/#dcg)
[Incomplete Data Structures](http://cs.wwc.edu/~cs_dept/KU/PR/#inc)
[Meta Level Programming](http://cs.wwc.edu/~cs_dept/KU/PR/#meta)
[Second-Order Programming](http://cs.wwc.edu/~cs_dept/KU/PR/#2ndOrder)
[Database](http://cs.wwc.edu/~cs_dept/KU/PR/#database)
[Expert Systems](http://cs.wwc.edu/~cs_dept/KU/PR/#expert)
[Object-Oriented Programming](http://cs.wwc.edu/~cs_dept/KU/PR/#oop)
[Appendix](http://cs.wwc.edu/~cs_dept/KU/PR/#appendix)
[References](http://cs.wwc.edu/~cs_dept/KU/PR/#ref)
**Introduction**
Prolog, whichstands for PROgramming in LOGic, is the most widely available language in thelogic programming paradigm. Logic and therefore Prolog is based themathematical notions of relations and logical inference. Prolog is adeclarative language
 meaning that rather than describing how to compute asolution, a program consists of a data base of facts and logical relationships(rules) which describe the relationships which hold for the given application.Rather then running a program to obtain a solution,
 the user asks a question.When asked a question, the run time system searches through the data base offacts and rules to determine (by logical deduction) the answer.
Among the features of Prolog are `logical variables'meaning that they behave like mathematical variables, a powerfulpattern-matching facility (unification), a backtracking strategy to search forproofs, uniform data structures, and input and
 output are interchangeable. 
Often there will be more than one way to deduce theanswer or there will be more than one solution, in such cases the run timesystem may be asked find other solutions. backtracking to generate alternativesolutions. Prolog is a weakly typed language
 with dynamic type checking andstatic scope rules. 
Prolog is used in artificial intelligence applicationssuch as natural language interfaces, automated reasoning systems and expertsystems. Expert systems usually consist of a data base of facts and rules andan inference engine, the run time system
 of Prolog provides much of theservices of an inference engine. 
**TheStructure of Prolog Programs**
- A Prolog program consists of a database of facts and rules, and queries (questions).
- Fact: ... . 
- Rule: ... :- ... . 
- Query: ?- ... . 
- Variables: must begin with an upper case letter.
- Constants: numbers, begin with lowercase letter, or enclosed in single quotes.
- Inductive definitions: base and inductive cases
- Towers of Hanoi: move N disks from pin a to pin b using pin c.
|hanoi(N)|:-|hanoi(N, a, b, c).|
|----|----|----|
|hanoi(0,_,_,_).| | |
|hanoi(N,FromPin,ToPin,UsingPin)|:-|M is N-1,|
|hanoi(M,FromPin,UsingPin,ToPin),| | |
|move(FromPin,ToPin),| | |
|hanoi(M,UsingPin,ToPin,FromPin).| | |
|move(From,To)|:-|write([move, disk from, pin, From, to, pin, ToPin]),|
|nl.| | |
- - Lists: append, member 
member(X,L)
:-
concat(_,[X|_],L).
- - Ancestor 
|ancestor(A,D)|:-|parent(A,B).|
|----|----|----|
|ancestor(A,D)|:-|parent(A,C),ancestor(C,D).|
|*but not*| | |
|ancestor(A,D)|:-|ancestor(A,P), parent(P,D).|
*since infinite recursion mayresult.*
- Depth-first search: Maze/Graph traversal
A database of arcs (we will assume they are directed arcs) of the form: 
a(node_i,node_j).
Rules for searching the graph: 
visited(A,T)
:-
member(A,T).
- I/O: terms, characters, files, lexical analyzer/scanner
- read(T), write(T), nl. 
- get0(N), put(N): ascii value of character 
- name(Name,Ascii_list). 
- see(F), seeing(F), seen, tell(F), telling(F), told.
- Natural language processing: Context-free grammars may be represented as Prolog rules. For example, the rule
|sentence|::=|noun_clause verb_clause|
*can be implemented in Prolog as*
|sentence(S)|:-|append(NC,VC,S), noun_clause(NC), verb_clause(VC).|
|----|----|----|
|*or in DCG as:*| | |
|sentence|->|noun_clause, verb_clause.|
|?- sentence(S,[]).| | |
Note that two arguments appear inthe query. Both are lists and the first is the sentence to be parsed, thesecond the remaining elements of the list which in this case is empty.
A Prolog program consists of a data base of facts andrules. There is no structure imposed on a Prolog program, there is no mainprocedure, and there is no nesting of definitions. All facts and rules areglobal in scope and the scope of a variable
 is the fact or rule in which itappears. The readability of a Prolog program is left up to the programmer.
A Prolog program is executed by asking a question. Thequestion is called a query. Facts, rules, and queries are called
*clauses*.
**Syntax**
**Facts**
A **fact** is just what it appears to be --- afact. A fact in everyday language is often a proposition like ``It is sunny.''or ``It is summer.'' In Prolog such facts could be represented as follows:
'It is sunny'.
'It is summer'.
**Queries**
A **query** in Prolog is the action of asking theprogram about information contained within its data base. Thus, queries usuallyoccur in the interactive mode. After a program is loaded, you will receive thequery prompt,
  ?- 
at which time you can ask the run time system aboutinformation in the data base. Using the simple data base above, you can ask theprogram a question such as
  ?- 'It is sunny'.
and it will respond with the answer 
  Yes
  ?- 
A yes means that the information in the data base isconsistent with the subject of the query. Another way to express this is thatthe program is capable of proving the query true with the available informationin the data base. If a fact is not
 deducible from the data base the systemreplys with a no, which indicates that based on the information available (theclosed world assumption) the fact is not deducible.
If the data base does not contain sufficientinformation to answer a query, then it answers the query with a no.
  ?- 'It is cold'.
  no
  ?-
**Rules**
**Rules** extend the capabilities of a logic program. They arewhat give Prolog the ability to pursue its decision-making process. Thefollowing program contains two rules for temperature. The first rule is read asfollows: ``It is
 hot if it is summer and it is sunny.'' The second rule is readas follows: ``It is cold if it is winter and it is snowing.''
'It is sunny'.
'It is summer'.
'It is hot' :- 'It is summer','It is sunny'.
'It is cold' :- 'It is winter','It is snowing'.
The query, 
  ?- 'It is hot'.
  Yes
  ?- 
is answered in the affirmative since both 'It issummer' and 'It is sunny' are in the data base while a query ``?- 'It is cold.''' will produce a negative response.
The previous program is an example of propositionallogic. Facts and rules may be parameterized to produce programs in predicatelogic. The parameters may be variables, atoms, numbers, or terms.Parameterization permits the definition of more complex
 relationships. Thefollowing program contains a number of predicates that describe a family'sgenelogical relationships.
  female(amy).
  female(johnette).
  male(anthony).
  male(bruce).
  male(ogden).
  parentof(amy,johnette).
  parentof(amy,anthony).
  parentof(amy,bruce).
  parentof(ogden,johnette).
  parentof(ogden,anthony).
  parentof(ogden,bruce).
The above program contains the three simplepredicates: female; male; and parentof. They are parameterized with what arecalled `atoms.' There are other family relationships which could also bewritten as facts, but this is a tedious process. Assuming
 traditional marriageand child-bearing practices, we could write a few rules which would relieve thetedium of identifying and listing all the possible family relations. Forexample, say you wanted to know if johnette had any siblings, the first questionyou must
 ask is ``what does it mean to be a sibling?'' To be someone's siblingyou must have the same parent. This last sentence can be written in Prolog as
siblingof(X,Y) :- 
            parentof(Z,X),
            parentof(Z,Y).
A translation of the above Prolog rule into Englishwould be ``X is the sibling of Y provided that Z is a parent of X, and Z is aparent of Y.'' X, Y, and Z are variables. This rule however, also defines achild to be its own sibling. To correct
 this we must add that X and Y are notthe same. The corrected version is: 
siblingof(X,Y) :- 
            parentof(Z,X),
            parentof(Z,Y),
            X Y.
The relation brotherof is similar but adds thecondition that X must be a male.
brotherof(X,Y) :- 
            parentof(Z,X),
            male(X),
            parentof(Z,Y),
            X Y.
From these examples we see how to construct facts,rules and queries and that strings are enclosed in single quotes, variablesbegin with a capital letter, constants are either enclosed in single quotes orbegin with a small letter.
**Types**
Prolog provides for numbers, atoms, lists, tuples, andpatterns. The types of objects that can be passed as arguments are defined inthis section.
**SimpleTypes**
Simple types are implementation dependent in Prologhowever, most implementations provide the simple types summarized in thefollowing table.
|TYPE|VALUES|
|----|----|
|boolean|true, fail|
|integer|integers|
|real|floating point numbers|
|variable|variables|
|atom|character sequences|
The boolean constants are not usually passed asparameters but are propositions. The constant fail is useful in forcing thegeneration of all solutions. Variables are character strings beginning with acapital letter. Atoms are either quoted character
 strings or unquoted stringsbeginning with a small letter. 
**CompositeTypes**
In Prolog the distinction between programs and dataare blurred. Facts and rules are used as data and data is often passed in thearguments to the predicates. Lists are the most common data structure inProlog. They are much like the array in that
 they are a sequential list ofelements, and much like the stack in that you can only access the list ofelements sequentially, that is, from one end only and not in random order. Inaddition to lists Prolog permits arbitrary patterns as data. The patterns canbe
 used to represent tuples. Prolog does not provide an array type. But arraysmay be represented as a list and multidimensional arrays as a list(s) of lists.An alternate representation is to represent an array as a set of facts in a thedata base.
REPRESENTATION 
list 
[ *comma separated sequence of items* ] 
pattern 
*sequence of items*
A list is designated in Prolog by square brackets ([]+). An example of a list is
[dog,cat,mouse]
This says that the list contains the elements dog,{\tt cat, and mouse, in that order. Elements in a Prolog list are ordered, eventhough there are no indexes. Records or tuples are represented as patterns.Here is an example.
book(author(aaby,anthony),title(labmanual),data(1991))
The elements of a tuple are accessed by patternmatching. 
book(Title,Author,Publisher,Date).
author(LastName,FirstName,MI).
publisher(Company,City).
book(T,A,publisher(C,rome),Date)
**Type Predicates**
Since Prolog is a weakly typed language, it isimportant for the user to be able to determine the type of a parameter. Thefollowing built in predicates are used to determine the type of a parameter.
|PREDICATE|CHECKS IF|
|----|----|
|var(V)|V is a variable|
|nonvar(NV)|NV is not a variable|
|atom(A)|A is an atom|
|integer(I)|I is an integer|
|real(R)|R is a floating point number|
|number(N)|N is an integer or real|
|atomic(A)|A is an atom or a number|
|functor(T,F,A)|T is a term with functor F and arity A|
|T =..L|T is a term, L is a list (see example below).|
|clause(H,T)|H :- T is a rule in the program|
The last three are useful in program manipulation(metalogical or meta-programming) and require additional explanation.clause(H,T) is used to check the contents of the data base. functor(T,F,A) andT=..L are used to manipulate terms. The predicate,
 functor is used as follows. 
functor(T,F,A)
T is a term, F is its functor, and A is its arity. Forexample, 
?- functor(t(a,b,c),F,A).
F = t
A = 3
yes
t is the functor of the term t(a,b,c), and 3 is thearity (number of arguments) of the term. The predicate =.. (univ) is used tocompose and decompose terms. For example:
?- t(a,b,c) =..L.
L = [t,a,b,c]
yes
?- T =..[t,a,b,c].
T = t(a,b,c)
yes
**Expressions**
Arithmetic expressions are evaluated with the built inpredicate is which is used as an infix operator in the following form.
*variable* is *expression*
For example, 
?- X is 3*4.
X = 12
yes
**Arithmetic Operators**
Prolog provides the standard arithmetic operations assummarized in the following table.
|SYMBOL|OPERATION|
|----|----|
|+|addition|
|-|subtraction|
|*|multiplication|
|/|real division|
|//|integer division|
|mod|modulus|
|**|power|
**Boolean Predicates**
Besides the usual boolean predicates, Prolog providesmore general comparison operators which compare terms and predicates to testfor unifiability and whether terms are identical.
|SYMBOL|OPERATION|ACTION| |
|----|----|----|----|
|A ?= B|unifiable|A and B are unifiable but|does not unify A and B|
|A = B|unify|unifys A and B if possible| |
|A \+= B|not unifiable| | |
|A == B|identical|does not unify A and B| |
|A \+== B|not identical| | |
|A =:= B|equal (value)|evaluates A and B to|determine if equal|
|A =\+= B|not equal (value)| | |
|A < B|less than (numeric)| | |
|A =< B|less or equal (numeric)| | |
|A > B|greater than (numeric)| | |
|A >= B|greater or equal (numeric)| | |
|A @< B|less than (terms)| | |
|A @=< B|less or equal (terms)| | |
|A @> B|greater than (terms)| | |
|A @>= B|greater or equal (terms)| | |
For example, the following are all true. 
3 @< 4
3 @< a
a @< abc6
abc6 @< t(c,d)
t(c,d) @< t(c,d,X)
Logic programming definition of natural number. 
% natural_number(N) <- N is anatural number.
natural_number(0).
natural_number(s(N)) :-natural_number(N).
Prolog definition of natural number. 
natural_number(N) :- integer(N),N >= 0.
Logic programming definition of inequalities 
% less_than(M,N) <- M is lessthan M
less_than(0,s(M)) :-natural_number(M).
less_than(s(M),s(N)) :-less_than(M,N).
% less_than_or_equal(M,N) <- Mis less than or equal to M
less_than_or_equal(0,N) :-natural_number(N).
less_than_or_equal(s(M),s(N)) :-less_than_or_equal(M,N).
Prolog definition of inequality. 
M =< N.
Logic programming definition of addition/substraction 
% plus(X,Y,Z) <- Z is X + Y
plus(0,N,N) :- natural_number(N).
plus(s(M),N,s(Z)) :- plus(M,N,Z).
Prolog definition of addition 
plus(M,N,Sum) :- Sum is M+N.
This does not define substration. Logic programmingdefinition of multiplication/division
% times(X,Y,Z) <- Z is X*Y 
times(0,N,0) :-natural_number(N).
times(s(M),N,Z) :- times(M,N,W),plus(W,N,Z).
Prolog definition of multiplication. 
times(M,N,Product) :- Product isM*N.
This does not define substration. Logic programmingdefinition of Exponentiation
% exp(N,X,Z) <- Z is X**N
exp(s(M),0,0) :-natural_number(M).
exp(0,s(M),s(0)) :-natural_number(M).
exp(s(N),X,Z) :- exp(N,X,Y),times(X,Y,Z).
Prolog definition of exponentiation is implementationdependent. 
**Logical Operators**
Predicates are functions which return a boolean value.Thus the logical operators are built in to the language. The comma on the righthand side of a rule is logical conjunction. The symbol :- is logicalimplication. In addition Prolog provides
 negation and disjunction operators.The logical operators are used in the definition of rules. Thus,
a :- b -> c;d. % a if (if b then c else d) 
This table summarizes the logical operators. 
|SYMBOL|OPERATION|
|----|----|
|not|negation|
|\+|not provable|
|,|logical conjunction|
|;|logical disjunction|
|:-|logical implication|
|->|if-then-else|
**Unificationand Pattern Matching**
The arguments in a query are matched (or unified inProlog terminology) to select the appropriate rule. Here is an example whichmakes extensive use of pattern matching. The rules for computing thederivatives of polynomial expressions can be written
 as Prolog rules. A givenpolynomial expression is matched against the first argument of the rule and thecorresponding derivative is returned.
% deriv(*Polynomial*, *variable*,*derivative*)
% dc/dx = 0
deriv(C,X,0) :- number(C). 
% dx/dx} = 1
deriv(X,X,1).
% d(cv)/dx = c(dv/dx)
deriv(C*U,X,C*DU) :- number(C),deriv(U,X,DU).
% d(u v)/dx = u(dv/dx) + v(du/dx)
deriv(U*V,X,U*DV + V*DU) :-deriv(U,X,DU), deriv(V,X,DV).
% d(u ± v)/dx = du/dx ± dv/dx
deriv(U+V,X,DU+DV) :-deriv(U,X,DU), deriv(V,X,DV).
deriv(U-V,X,DU-DV) :-deriv(U,X,DU), deriv(V,X,DV).
% du^n/dx = nu^{n-1}(du/dx)
deriv(U^+N,X,N*U^+N1*DU) :- N1 isN-1, deriv(U,X,DU).
Prolog code is often bidirectional. In bidirectionalcode, the arguments may be use either for input or output. For example, thiscode may be used for both differentiation and integration with queries of theform:
?- deriv(*Integral*,*X*,*Derivative*).
where either *Integral* or *Derivative* maybe instantiated to a formula.
**Functions**
Prolog does not provide for a function type therefore,functions must be defined as relations. That is, both the arguments to thefunction and the result of the function must be parameters to the relation.This means that composition of two functions
 cannot be constructed. As anexample, here is the factorial function defined as relation in Prolog. Notethat the definition requires two rules, one for the base case and one for theinductive case.
fac(0,1).
fac(N,F) :- N > 0, M is N - 1,
       fac(M,Fm), F is N * Fm.
The second rule states that if N > 0, M = N - 1, Fmis (N-1)!, and F = N * Fm, then F is N!. Notice how `is' is used. In thisexample it resembles an assignment operator however, it may not be used toreassign a variable to a new value. I the logical
 sense, the order of theclauses in the body of a rule are irrelevant however, the order may matter in apractical sense. M must not be a variable in the recursive call otherwise aninfinite loop will result. Much of the clumsiness of this definition comes fromthe
 fact that fac is defined as a relation and thus it cannot be used in anexpression. Relations are commonly defined using multiple rules and the order ofthe rules may determine the result. In this case the rule order is irrelevantsince, for each value of N only
 one rule is applicable. Here are the Prologequivalent of the definitions of the gcd function, Fibonacci function andackerman's function.
gcd(A,B,GCD) :- A = B, GCD = A.
gcd(A,B,GCD) :- A < B, NB is B- A, gcd(A,NB,GCD).
gcd(A,B,GCD) :- A > B, NA is A- B, gcd(NA,B,GCD).
fib(0,1).
fib(1,1).
fib(N,F) :- N > 1, N1 is N -1, N2 is N - 2,
    fib(N1,F1), fib(N2,F2), F is F1 + F2.
ack(0,N,A) :- A is N + 1.
ack(M1,0,A) :- M > 0, M is M -1, ack(M,1,A).
ack(M1,N1,A) :- M1 > 0, N1> 0, M is M - 1, N is N - 1,
  ack(M1,N,A1), ack(M,A1,A).
Notice that the definition of ackerman's function isclumsier than the corresponding functional definition since the functionalcomposition is not available. Logic programming definition of the factorialfunction.
% factorial(N,F) <- F is N!
factorial(0,s(0)).
factorial(s(N),F) :-factorial(N,F1), times(s(N),F1,F).
Prolog definition of factorial function. 
factorial(0,1).
factorial(N,F) :- N1 is N-1,factorial(N1,F1), F is N*F1.
Logic programming definition of the minimum. 
% minimum(M,N,Min) <- Min isthe minimum of {M, N}
minimum(M,N,M) :-less_than_or_equal(M,N).
minimum(M,N,N) :-less_than_or_equal(N,M).
Prolog programming definition of the minimum. 
minimum(M,N,M) :- M =< N.
minimum(M,N,N) :- N =< M.
Logic programming definition of the modulus. 
% mod(M,N,Mod) <- Mod is theremainder of the integer division of M by N.
mod(X,Y,Z) :- less_than(Z,Y),times(Y,Q,W), plus(W,Z,X).
% or
mod(X,Y,X) :- less_than(X,Y).
mod(X,Y,X) :- plus(X1,Y,X),mod(X1,Y,Z).
Logic programming definition of Ackermann's function. 
ack(0,N,s(N)).
ack(s(M),0,Val) :-ack(M,s(0),Val).
ack(s(M),s(N),Val) :-ack(s(M),N,Val1), ack(M,Val1,Val).
Prolog definition of Ackermann's function. 
ack(0,N,Val) :- Val is N + 1.
ack(M,0,Val) :- M > 0, M1 isM-1, ack(M1,1,Val).
ack(M,N,Val) :- M > 0, N >0, M1 is M-1, N1 is N-1, 
                ack(M,N1,Val1),ack(M1,Val1,Val).
Logic programming definition of the Euclidianalgorithm. 
gcd(X,0,X) :- X > 0.
gcd(X,Y,Gcd) :- mod(X,Y,Z),gcd(Y,Z,Gcd).
Logic programming definition of the Euclidianalgorithm. 
gcd(X,0,X) :- X > 0.
gcd(X,Y,Gcd) :- mod(X,Y,Z),gcd(Y,Z,Gcd).
**Lists**
Objective
Outline 
- Lists 
- Composition of Recursive Programs 
- Iteration 
Lists are the basic data structure used in logic (andfunctional) programming. Lists are a recursive data structure so recursionoccurs naturally in the definitions of various list operations. When definingoperations on recursive data structures,
 the definition most often naturallyfollows the recursive definition of the data structure. In the case of lists,the empty list is the base case. So operations on lists must consider the emptylist as a case. The other cases involve a list which is composed
 of an elementand a list. 
Here is a recursive definition of the list datastructure as found in Prolog.
List --> [ ]
List --> [Element|List]
Here are some examples of list representation, thefirst is the empty list.
Pair Syntax                Element Syntax
 [ ]            [ ] 
 [a|[ ]]        [a] 
 [a|b|[ ]]      [a,b] 
 [a|X]          [a|X] 
 [a|b|X]        [a,b|X] 
Predicates on lists are often written using multiplerules. One rule for the empty list (the base case) and a second rule for nonempty lists. For example, here is the definition of the predicate for thelength of a list.
% length(List,Number) <-Number is lenght of List
length([],0).
length([H|T],N) :- length(T,M), Nis M+1.
Element of a list. 
% member(Element,List) <-Element is an element of the list List
member(X,[X|List).
member(X,[Element|List]) :-member(X,List).
Prefix of a list. 
% prefix(Prefix,List) <-Prefix is a prefix of list List
prefix([],List).
prefix([X|Prefix],[X|List]) :-prefix(Prefix,List).
Suffix of a list. 
% suffix(Suffix,List) <-Suffix is a suffix of list List
suffix(Suffix,Suffix).
prefix(Suffix,[X|List]) :-suffix(Suffix,List).
Append (concatenate) two lists. 
% append(List1,List2,List1List2)<- 
%   List1List2 is the result of concatenatingList1 and List2.
append([],List,List).
append([Element|List1],List2,[Element|List1List2]):- 
append(List1,List2,List1List2).
Compare this code with the code for plus. sublist --define using 
- Suffix of a prefix 
- Prefix of a suffix 
- Recursive definition of sublist using prefix 
- Suffix of a prefix using append 
- Prefix of a suffix using append 
member, prefix and suffix -- defined using appendreverse, delete, select, sort, permutation, ordered, insert, quicksort.
**Iteration**
Iterative version of Length 
% length(List,Number) <-Number is lenght of List
% Iterative version.
length(List,LenghtofList) :-length(List,0,LengthofList).
%length(SufixList,LengthofPrefix,LengthofList) <- 
%         LengthofList is LengthofPrefix +length of SufixList
length([],LenghtofPrefix,LengthofPrefix).
length([Element|List],LengthofPrefix,LengthofList):- 
PrefixPlus1 is LengthofPrefix +1, 
length(List,PrefixPlus1,LengthofList).
Iterative version of Reverse 
% reverse(List,ReversedList)<- ReversedList is List reversed.
% Iterative version.
reverse(List,RList) :-reverse(List,[],RList).
%length(SufixList,LengthofPrefix,LengthofList) <- 
%         LengthofList is LengthofPrefix +length of SufixList
reverse([],RL,RL).
reverse([Element|List],RevPrefix,RL):- 
reverse(List,[Element|RevPrefix],RL).
Here are some simple examples of common listoperations defined by pattern matching. The first sums the elements of a listand the second forms the product of the elements of a list.
sum([ ],0).
sum([X|L],Sum) :- sum(L,SL), Sumis X + SL.
product([ ],1).
product([X|L],Prod) :-product(L,PL), Prod is X * PL.
Another example common list operation is that ofappending or the concatenation of two lists to form a third list. Append may bedescribed as the relation between three lists, L1, L2, L3, where L1 = [x1,...,xm],L2 = [y1,...,yn]
 and L3 = [x1,...,xm,y1,...,yn].In Prolog, an inductive style definition is required.
append([ ],L,L). 
append([X1|L1],L2, [X1|L3]) :-append(L1,L2,L3).
The first rule is the base case. The second rule isthe inductive case. In effect the second rule says that
if L1 = [x2,...,xm],
      L2 = [y1,...,yn]and 
      L3 = [x2,...,xm,y1,...,yn],
then [x1,x2,...,xm,y1,...,yn],is the result of
appending  [x1,x2,...,xm]and L2.
The append relation is quite flexible. It can be usedto determine if an object is an element of a list, if a list is a prefix of alist and if a list is a suffix of a list.
member(X,L) :- append(_,[X|_],L).
prefix(Pre,L) :-append(Prefix,_,L).
suffix(L,Suf) :- append(_,Suf,L).
The underscore (_+) in the definitions denotes ananonymous variable (or don`t care) whose value in immaterial to the definition.The member relation can be used to derive other useful relations.
vowel(X) :-member(X,[a,e,i,o,u]).
digit(D) :- member(D,['0','1','2','3','4','5','6','7','8','9']).
A predicate defining a list and its reversal can bedefined using pattern matching and the append relation as follows.
reverse([ ],[ ]).
reverse([X|L],Rev) :-reverse(L,RL), append(RL,[X],Rev).
Here is a more efficient (iterative/tail recursive)version. 
reverse([ ],[ ]).
reverse(L,RL) :- reverse(L,[],RL).
reverse([ ],RL,RL).
reverse([X|L],PRL,RL) :-reverse(L,[X|PRL],RL).
To conclude this section, here is a definition ofinsertion sort. 
isort([ ],[ ]).
isort([X|UnSorted],AllSorted) :-isort(UnSorted,Sorted),
                                insert(X,Sorted,AllSorted).
insert(X,[ ],[X]).
insert(X,[Y|L],[X,Y|L]) :- X=< Y.
insert(X,[Y|L],[Y|IL]) :-  X > Y, insert(X,L,IL).
**Iteration**
Recursion is the only iterative method available inProlog. However, tail recursion can often be implemented as iteration. Thefollowing definition of the factorial function is an `iterative' definitionbecause it is `tail recursive.' It corresponds
 to an implementation using a while-loopin an imperative programming language. 
fac(0,1).
fac(N,F) :- N > 0, fac(N,1,F).
fac(1,F,F).
fac(N,PP,F) :- N > 1, NPp isN*PP, M is N-1, 
       fac(M,NPp,F). 
Note that the second argument functions as an *accumulator*.The accumulator is used to store the partial product much as might be done is aprocedural language. For example, in Pascal an iterative factorial functionmight be written as
 follows. 
function fac(N:integer) :integer;
var i : integer;
begin
  if N >= 0 then begin
       fac := 1
      for I := 1 to N do
           fac := fac * I
  end
end;
In the Pascal solution fac acts as an accumulator tostore the partial product. The Prolog solution also illustrates the fact thatProlog permits different relations to be defined by the same name provided thenumber of arguments is different.
 In this example the relations are fac/2 andfac/3 where fac is the ``functor" and the number refers to the arity ofthe predicate. As an additional example of the use of accumulators, here is aniterative (tail recursive version) of the Fibonacci function.
fib(0,1).
fib(1,1).
fib(N,F) :- N > 1,fib(N,1,1,F)
fib(2,F1,F2,F) :- F is F1 + F2.
fib(N,F1,F2,F) :- N > 2, N1 isN - 1, NF1 is F1 + F2,
    fib(N1,NF1,F1,F).
**Iterators,Generators and Backtracking**
The following fact and rule can be used to generatethe natural numbers. % Natural Numbers
nat(0).
nat(N) :- nat(M), N is M + 1.
The successive numbers are generated by backtracking.For example, when the following query is executed successive natural numbersare printed.
?- nat(N), write(N), nl, fail.
The first natural number is generated and printed,then fail forces backtracking to occur and the second rule is used to generatethe successive natural numbers. The following code generates successive prefixesof an infinite list beginning with
 N. 
natlist(N,[N]).
natlist(N,[N|L]) :- N1 is N+1,natlist(N1,L).
As a final example, here is the code for generatingsuccessive prefixes of the list of prime numbers.
primes(PL) :- natlist(2,L2),sieve(L2,PL).
sieve([ ],[ ]).
sieve([P|L],[P|IDL]) :-sieveP(P,L,PL), sieve(PL,IDL).
sieveP(P,[ ],[ ]). 
sieveP(P,[N|L],[N|IDL]) :- N modP  > 0, sieveP(P,L,IDL).
sieveP(P,[N|L],   IDL) :- N mod P =:= 0, sieveP(P,L,IDL).
Occasionally, backtracking and multiple answers areannoying. Prolog provides the cut symbol (!) to control backtracking. Thefollowing code defines a predicate where the third argument is the maximum ofthe first two.
max(A,B,M) :- A <  B, M = B.
max(A,B,M) :- A >= B, M = A.
The code may be simplified by dropping the conditionson the second rule.
max(A,B,B) :- A <  B.
max(A,B,A).
However, in the presence of backtracking, incorrectanswers can result as is shown here.
?- max(3,4,M).
M = 4;
M = 3
To prevent backtracking to the second rule the cutsymbol is inserted into the first rule.
max(A,B,B) :- A < B.!.
max(A,B,A).
Now the erroneous answer will not be generated. A wordof caution: cuts are similar to gotos in that they tend to increase thecomplexity of the code rather than to simplify it. In general the use of cutsshould be avoided.
**Tuples ( orRecords)**
We illustrate the data type of tuples with the codefor the abstract data type of a binary search tree. The binary search tree isrepresented as either nil for the empty tree or as the tuple btree(Item,L_Tree,R_Tree).Here is the Prolog code for
 the creation of an empty tree, insertion of anelement into the tree, and an in-order traversal of the tree.
create_tree(niltree).
inserted_in_is(Item,niltree,btree(Item,niltree,niltree)).
inserted_in_is(Item,btree(ItemI,L_T,R_T),Result_Tree):- 
    Item @< ItemI,
    inserted_in_is(Item,L_Tree,Result_Tree).
inserted_in_is(Item,btree(ItemI,L_T,R_T),Result_Tree):- 
    Item @> ItemI,
    inserted_in_is(Item,R_Tree,Result_Tree).
inorder(niltree,[ ]).
inorder(btree(Item,L_T,R_T),Inorder):- 
          inorder(L_T,Left),
          inorder(R_T,Right),
          append(Left,[Item|Right],Inorder).
The membership relation is a trivial modification ofthe insert relation. Since Prolog access to the elements of a tuple are bypattern matching, a variety of patterns can be employed to represent the tree.Here are some alternatives.
[Item,LeftTree,RightTree]
Item/LeftTree/RightTree
(Item,LeftTree,RightTree)
**Extra-LogicalPredicates**
Objective
Outline 
- Input/Output 
- Assert/Retract 
- System Access 
The class of predicates in Prolog that lie outside thelogic programming model are called
*extra-logical* predicates. Thesepredicates achieve a side effect in the course of being satisfied as a logicalgoal. There are three types of extra-logical predicates, predicates forhandling I/O, predicates for manipulating the program, and predicates
 foraccessing the underlying operating system. 
**Input/Output**
Most Prolog implementations provide the predicatesread and write. Both take one argument, read unifies its argument with the nextterm (terminated with a period) on the standard input and write prints itsargument to the standard output. As an
 illustration of input and output as wellas a more extended example, here is the code for a checkbook balancing program.The section beginning with the comment ``Prompts" handles the I/0.
% Check Book Balancing Program.
checkbook :-initialbalance(Balance),
  newbalance(Balance).
% Recursively compute newbalances 
newbalance(OldBalance) :-transaction(Transaction),
  action(OldBalance,Transaction).
% If transaction amount is 0 thenfinished.
action(OldBalance,Transaction) :-Transaction = 0, 
  finalbalance(OldBalance). %
%
% If transaction amount is not 0then compute new balance.
action(OldBalance,Transaction) :-Transaction \+= 0, 
  NewBalance is OldBalance + Transaction,
  newbalance(NewBalance).  
%
% Prompts 
initialbalance(Balance) :-write('Enter initial balance: \'),
  read(Balance).
transaction(Transaction) :- 
  write('Enter Transaction, '),
    write('- for withdrawal, 0 to terminate):'),
  read(Transaction).
finalbalance(Balance) :-write('Your final balance is: \'),
  write(Balance), nl.
**Files**
see(File)
Current input file is now File. 
seeing(File)
File is unified with the name of the current input file. 
seen
Closes the current input file. 
tell(File)
Current output file is now File. 
telling(File)
File is unified with the name of the current output file. 
told
Closes the current output file. 
**Term I/O**
read(Term)
Reads next full-stop (period) delimited term from the current inputstream, if eof then returns the atom 'end_of_file'.
write(Term)
Writes a term to the current output stream. 
print(Term)
Writes a term to the current output stream. Uses a user defined predicateportray/1 to write the term, otherwise uses write.
writeq(Term)
Writes a term to the current output stream in a form aceptable as input toread.
**Character I/O**
get(N)
N is the ASCII code of the next non-blank printable character on thecurrent input stream. If end of file, then a -1 is returned.
put(N)
Puts the character corresponding to ASCII code N on the current outputstream.
nl
Causes the next output to be on a new line. 
tab(N)
N spaces are output to the current output stream. 
**Program Access**
consult(SourceFile)
Loads SourceFile into the interpreter but, if a predicate is definedaccross two or more files, consulting them will result in only the clauses inthe file last consulted being used.
reconsult(File)
available in some systems. 
**Other**
name(Atom,ASCII_List)
the conversion routine between lists of ASCII codes and atoms. 
display, prompt 
% Read a sentence and return alist of words.
read_in([W|Ws]) :- get0(C),read_word(C,W,C1), rest_sent(W,C1,Ws).
% Given a word and the next character,read in the rest of the sentence
rest_sent(W,_,[]) :- lastword(W).
rest_sent(W,C,[W1|Ws]) :-read_word(C,W1,C1), rest_sent(W1,C1,Ws).
read_word(C,W,C1) :-single_character(C),!,name(W,[C]), get0(C1).
read_word(C,W,C2) :-in_word(C,NewC), get0(C1),
                    rest_word(C1,Cs,C2),name(W,[NewC|Cs]).
read_word(C,W,C2) :- get0(C1),read_word(C1,W,C2).
rest_word(C,[NewC|Cs],C2) :-in_word(C,NewC), !, get0(C1), 
rest_word(C1,Cs,C2).
rest_word(C,[],C).
% These are single characterwords.
single_character(33).      % !
single_character(44).      % ,
single_character(46).      % .
single_character(58).      % :
single_character(59).      % ;
single_character(63).      % ?
% These characters can appearwithin a word.
in_word(C,C) :- C > 96, C <123.               % a,b,...,z
in_word(C,L) :- C > 64, C <91, L is C + 32.   % A,B,...,Z
in_word(C,C) :- C > 47, C <58.                % 0,1,...,9
in_word(39,39).                                % '
in_word(45,45).                                % -
% These words terminate asentence.
lastword('.').
lastword('!').
lastword('?').
**Program Access and Manipulation**
clause(Head,Body)
assert(Clause)
adds clause to the end of the database 
asserta(Clause)
retract(Clause_Head)
consult(File_Name)
**System Access**
system(Command)
Execute Command in the operating system 
**Style andLayout**
Objective 
Outline 
- Style and Layout 
- Debugging 
Some conventions for comments. 
- Long comments should precede the code they refer to while short comments should be interspersed with the code itself.
- Program comments should describe what the program does, how it is used (goal predicate and expected results), limitations, system dependent features, performance, and examples of using the program.
- Predicate comments explain the purpose of the predicate, the meaning and relationship among the arguments, and any restrictions as to argument type.
- Clause comments add to the description of the case the particular clause deals with and is usefull for documenting cuts.
Some conventions for program layout 
- Group clauses belonging to a relation or ADT together.
- Clauses should be short. Their body should contain no more than a few goals.
- Make use of indentation to improve the readability of the body of a clause.
- Mnemonic names for relations and variables should be used. Names should indicate the meaning of relations and the role of data objects.
- Clearly separate the clauses defining different relations.
- The cut operator should be used with care. The use of `red' cuts should be limited to clearly defined mutually exclusive alternatives.
Illustration 
merge( List1, List2, List3 ) :-
 ( List1 = [], !, List3 = List2 );
 ( List2 = [], !, List3 = List1 );
 ( List1 = [X|L1], List2 = [Y|L2 ),
((X < Y, ! Z = X, merge( L1,List2, L3 ) );
( Z = Y, merge( List1, L2, L3 ))),
 List3 = [Z|L3].
A better version 
merge( [], List2, List2 ). 
merge( List1, [], List1 ). 
merge( [X|List1], [Y|List2],[X|List3] ) :-
X < Y, !,  merge( List1, List2, List3 ).  \% Red Cut
merge( List1, [Y|List2],[Y|List3] ) :-
 merge( List1, List2, List3 ).
**Debugging**
trace/notrace, spy/nospy, programmer inserteddebugging aids -- write predicates and p :- write, fail.
**Negationand Cuts**
Objective
Outline 
- Negation as failure 
- Green Cuts 
- Red Cuts 
**Negation**
**Cuts**
**Green cuts: Determinism**
Selection among mutually exclusive clauses. 
**Tail Recursion Optimization**
Prevention of backtracking when only one solutionexists. 
A :- B1,...,Bn,Bn1.
A :- B1,...,Bn,!,Bn1. % preventsbacktracking
**Red cuts: omitting explicitconditions**
**DefiniteClause Grammars**
Objective:
Outline
- The parsing problem: Context-free grammars; Construct a parse tree for a sentence given the context-free grammar.
- Representing the Parsing Problem in Prolog 
- The Grammar Rule Notation] (Definite Clause Grammars -- DCG)
- Adding Extra Arguments 
- Adding Extra Tests 
Prolog originated from attempts to use logic toexpress grammar rules and formalize the parsing process. Prolog has specialsyntax rules which are called
*definite clause grammars* (DCG). DCGs are ageneralization of context free grammars.
**Context Free Grammars**
A context free grammar is a set of rules of the form: 
 -> 
where nonterminal is a nonterminal and body is asequence of one or more items. Each item is either a nonterminal symbol or asequence of terminal symbols. The meaning of the rule is that the body is apossible form for an object of type nonterminal.
S --> a b
S --> a S b
**DCG**
Nonterminals are written as Prolog atoms, the items inthe body are separated with commas and sequences of terminal symbols are writtenas lists of atoms. For each nonterminal symbol, S, a grammar defines a languagewhich is obtained by repeated
 nondeterministic application of the grammarrules, starting from S. 
s --> [a],[b].
s --> [a],s,[b].
As an illustration of how DCG are used, the string[a,a,b,b] is given to the grammar to be parsed.
?- s([a,a,b,b],[]).  
  yes
Here is a natural language example. 
% DCGrammar
sentence --> noun_phrase,verb_phrase.
noun_phrase --> determiner,noun.
noun_phrase --> noun.
verb_phrase --> verb.
verb_phrase --> verb,noun_phrase.
% Vocabulary
determiner --> [the].
determiner --> [a].
noun --> [cat].
noun --> [cats].
noun --> [mouse].
noun --> [mice].
verb --> [scare].
verb --> [scares].
verb --> [hate].
verb --> [hates].
Context free grammars cannot define the requiredagreement in number between the noun phrase and the verb phrase. Thatinformation is context dependent (sensitive). However, DCG are more generalNumber agreement
% DCGrammar - with numberagreement between noun phrase and verb phrase
sentence -->noun_phrase(Number), verb_phrase(Number).
noun_phrase(Number) -->determiner(Number), noun(Number).
noun_phrase(Number) -->noun(Number).
verb_phrase(Number) -->verb(Number).
verb_phrase(Number) -->verb(Number), noun_phrase(Number1).
% Vocabulary
determiner(Number) --> [the].
determiner(singular) --> [a].
noun(singular) --> [cat].
noun(plural) --> [cats].
noun(singular) --> [mouse].
noun(plural) --> [mice].
verb(plural) --> [scare].
verb(singular) --> [scares].
verb(plural) --> [hate].
verb(singular) --> [hates].
**Parse Trees**
% DCGrammar -- with parse tree asa result
sentence(sentence(NP,VP)) -->noun_phrase(NP), verb_phrase(VP).
noun_phrase(noun_phrase(D,NP))--> determiner(D), noun(NP).
noun_phrase(NP) --> noun(NP).
verb_phrase(verb_phrase(V))--> verb(V).
verb_phrase(verb_phrase(V,NP))--> verb(V), noun_phrase(NP).
% Vocabulary
determiner(determiner(the))--> [the].
determiner(determiner(a)) -->[a].
noun(noun(cat)) --> [cat].
noun(noun(cats)) --> [cats].
noun(noun(mouse)) --> [mouse].
noun(noun(mice)) --> [mice].
verb(verb(scare)) --> [scare].
verb(verb(scares)) -->[scares].
verb(verb(hate)) --> [hate].
verb(verb(hates)) --> [hates].
**Simple Semantics for NaturalLanguage Sentences**
**Transitive and intransitive verbs**
% DCGrammar -- Transitive andintransitive verbs
sentence(VP) -->noun_phrase(Actor), verb_phrase(Actor,VP).
noun_phrase(Actor) -->proper_noun(Actor).
verb_phrase(Actor,VP) -->intrans_verb(Actor,VP).
verb_phrase(Actor,VP) -->transitive_verb(Actor,Something,VP),
                            noun_phrase(Something).
% Vocabulary
proper_noun(john)  --> [john].
proper_noun(annie) -->[annie].
intrans_verb(Actor,paints(Actor))  --> [paints].
transitive_verb(Somebody,Something,likes(Somebody,Something))--> [likes].
**Determiners -- `a' and `every'**
:- op( 100, xfy, and).
:- op( 150, xfy, =>).
% DCGrammar -- Transitive andintransitive verbs
sentence(S) -->noun_phrase(X,Assn,S), verb_phrase(X,Assn).
noun_phrase(X,Assn,S) -->determiner(X,Prop,Assn,S), noun(X,Prop).
verb_phrase(X,Assn) -->intrans_verb(X,Assn).
% Vocabulary
determiner(X,Prop,Assn,exists(X,Propand Assn)) --> [a].
determiner(X,Prop,Assn,    all(X,Prop => Assn)) --> [every].
noun(X,man(X))  --> [man].
noun(X,woman(X))  --> [woman].
intrans_verb(X,paints(X))  --> [paints].
intrans_verb(X,dances(X))  --> [dances].
**Relative Clauses**
**Interleaving syntax and semanticsin DCG**
% Word level
sentence --> word(W),rest_sent(W).
rest_sent(W) -->{last_word(W)}.
rest_sent(_) --> word(W),rest_sent(W).
% Character level
word(W) -->{single_char_word(W)},   [W].
word(W) -->{multiple_char_word(W)}, [W].
% Read a sentence and return alist of words.
sentence --> {get0(C)},word(C,W,C1), rest_sent(C1,W).
% Given the next character andthe previous word,
% read the rest of the sentence
rest_sent(C,W) -->{lastword(W)}.                  % empty
rest_sent(C,_) -->word(C,W,C1), rest_sent(C1,W).
word(C,W,C1) -->{single_character(C),!,name(W,[C]), get0(C1)}, [W].  % !,.:;?
word(C,W,C2) -->{in_word(C,Cp), get0(C1), rest_word(C1,Cs,C2),
                     name(W,[Cp|Cs])},[W].                        
word(C,W,C2) --> {get0(C1)},word(C1,W,C2).                   %consume blanks
% These words terminate asentence.
lastword('.').
lastword('!').
lastword('?').
% This reads the rest of the wordplus the next character.
rest_word(C,[Cp|Cs],C2) :-in_word(C,Cp), get0(C1), rest_word(C1,Cs,C2).
rest_word(C,[],C).
% These are single characterwords.
single_character(33).      % !
single_character(44).      % ,
single_character(46).      % .
single_character(58).      % :
single_character(59).      % ;
single_character(63).      % ?
% These characters can appearwithin a word.
in_word(C,C) :- C > 96, C <123.               % a,b,...,z
in_word(C,L) :- C > 64, C <91, L is C + 32.   % A,B,...,Z
in_word(C,C) :- C > 47, C <58.                % 0,1,...,9
in_word(39,39).                                % '
in_word(45,45).                                % -
a calculator!! 
**IncompleteData Structures**
Objective
Outline 
- Difference Lists 
- Dictionaries 
- Queue 
- QuickSort 
An incomplete data structure is a data structurecontaining a variable. Such a data structure is said to be `partiallyinstantiated' or `incomplete.' We illustrate the programming with incompletedata structures by modifying the code for a binary
 search tree. The resultingcode permits the relation inserted_in_is to define both the insertion andmembership relations. The empty tree is represented as a variable while apartially instantiated tree is represented as a tuple.
create_tree(Niltree) :-var(Niltree). % Note: Nil is a variable
inserted_in_is(Item,btree(Item,L_T,R_T)).
inserted_in_is(Item,btree(ItemI,L_T,R_T)):- 
    Item @< ItemI,
    inserted_in_is(Item,L_T).
inserted_in_is(Item, btree(ItemI,L_T,R_T)):- 
    Item @> ItemI,
    inserted_in_is(Item,R_T).
inorder(Niltree,[ ]) :-var(Niltree).
inorder(btree(Item,L_T,R_T),Inorder):- 
          inorder(L_T,Left),
          inorder(R_T,Right),
          append(Left,[Item|Right],Inorder).
**Meta LevelProgramming**
Meta-programs treat other programs as data. Theyanalyze, transform, and simulate other programs. Prolog clauses may be passedas arguments, added and deleted from the Prolog data base, and may beconstructed and then executed by a Prolog program.
 Implementations may requirethat the functor and arity of the clause be previously declared to be a dynamictype.
Objective
Outline 
- Meta-logical Type Predicates 
- Assert/Retract 
- System Access 
**Meta-Logical Type Predicates**
var(V)
Tests whether V is a variable. 
nonvar(NV)
Tests whether NV is a non-variable term. 
atom(A)
Tests whether A is an atom (non-variable term of arity 0 other than anumber).
integer(I)
Tests whether I is an integer. 
number(N)
Tests whether N is a number. 
**Term Comparison**
X = Y
X == Y
X =:= Y
**The Meta-Variable Facility**
call(X)
this 
**Assert/Retract**
Here is an example illustrating how clauses may beadded and deleted from the Prolog data base. The example shows how to simulatean assignment statement by using assert and retract to modify the associationbetween a variable and a value.
:- dynamic x/1 .% this may berequired in some Prologs  
x(0).  % An initial value is required in thisexample
assign(X,V) :- Old =..[X,_],retract(Old),
                  New =..[X,V], assert(New).
Here is an example using the assign predicate. 
?- x(N).
N = 0
yes
?- assign(x,5).
yes
?- x(N).
N = 5
Here are three programs illustrating Prolog's metaprogramming capability. This first program is a simple interpreter for pureProlog programs.
% Meta Interpreter for pureProlog
prove(true).
prove((A,B)) :- prove(A),prove(B).
prove(A) :- clause(A,B),prove(B).
Here is an execution of an append using theinterpreter. 
 ?- prove(append([a,b,c],[d,e],F)).
F = [a,b,c,d,e]
It is no different from what we get from using theusual run time system. The second program is a modification of the interpreter,in addition to interpreting pure Prolog programs it returns the sequence ofdeductions required to satisfy the query.
% Proofs for pure Prolog programs
proof(true,true).
proof((A,B),(ProofA,ProofB)) :-proof(A,ProofA), proof(B,ProofB).
proof(A,(A:-Proof)) :-clause(A,B), proof(B,Proof).
Here is a proof an append. 
?-proof(append([a,b,c],[d,e],F),Proof).
F = [a,b,c,d,e]
Proof = (append([a,b,c],[d,e],[a,b,c,d,e]):-
           (append([b,c],[d,e],[b,c,d,e]) :-
           (append([c],[d,e],[c,d,e]) :- 
           (append([ ],[d,e],[d,e]) :- true))))
The third program is also a modification of theinterpreter. In addition to interpreting pure Prolog programs, is a tracefacility for pure Prolog programs. It prints each goal twice, before and aftersatisfying the goal so that the programmer
 can see the parameters before andafter the satisfaction of the goal. 
% Trace facility for pure Prolog
trace(true).
trace((A,B)) :- trace(A),trace(B).
trace(A) :- clause(A,B),downprint(A), trace(B), upprint(A).
downprint(G) :- write('>'),write(G), nl.
upprint(G)   :- write('<'), write(G), nl.
Here is a trace of an append. 
?-trace(append([a,b,c],[d,e],F)).
>append([a,b,c],[d,e],[a|1427104])
>append([b,c],[d,e],[b|1429384])
>append([c],[d,e],[c|1431664])
>append([ ],[d,e],[d,e])
<append([ ],[d,e],[d,e])
<append([c],[d,e],[c,d,e])
<append([b,c],[d,e],[b,c,d,e])
<append([a,b,c],[d,e],[a,b,c,d,e])
F = [a,b,c,d,e]
**Predictates for programmanipulation**
- consult(*file name*) 
- var(*term*), nonvar(*term*), atom(*term*), integer(*term*), atomic(*term*)
- functor(*Term*,*Functor*,*arity*), arg(*N*,*term*,*N-th arg*),
*Term* =..*List*
- call(*Term*) 
- clause(*Head*,*Body*), assertz(*Clause*), retract(*Clause*)
**Second-OrderProgramming**
Objective:
Second-Order Programming 
Outline: 
- Setof, Bagof, Findall 
- Other second-order predicates 
- Applications 
**Setof, Bagof and Findall**
**Other second-order predicates**
has_property, map_list, filter, foldr etc 
- Variable predicate names 
p(P,X,Y) :- P(X,Y).
p(P,X,Y) :- R =..[P,X,Y],call(R).
For the following functions let S be the list[S_1,...,S_n]. 
1. The function map where map(f,S)is [f(S_1),...,f(S_n)]. 
2. The function filter wherefilter(P,S) is the list of elements of S that satisfy the predicate P.
3. The function foldl wherefoldl(Op,In,S) which folds up S, using the given binary operator Op and startvalue In, in a left associative way, ie, foldl(op, r,[a,b,c]) = (((r op a) opb) op c).
4. The function foldr wherefoldr(Op,In,S) which folds up S, using the given binary operator Op and startvalue In, in a right associative way, ie, foldr(op,r,[a,b,c]) = a op (b op (cop r)).
5. The function map2 is similar tomap, but takes a function of two arguments, and maps it along two argumentlists.
6. The function scan where scan(op,r, S) applies foldl op r) to every initial segment of a list. For example scan(+) 0 x) computes running sums.
7. The function dropwhile wheredropwhile(P,S) which returns the suffix of S where each element of the prefexsatisfies the predicate P.
8. The function takewhile wheretakewhile(P,S) returns the list of initial element of S which satisfy P.
9. The function until whereuntil(P,F,V) returns the result of applying the function F to the value thesmallest number of times necessary to satisfy the predicate. Example until(>1000) (2*) 1 = 1024
10. The function iterate whereiterate(f,x) returns the infinite list [x, f x, f(f x), ... ]
11. Use the function foldr to definethe functions, sum, product and reverse.
12. Write a generic sort program, itshould take a comparison function as a parameter.
13. Write a generic transitive closerprogram, it should take a binary relation as a parameter.
**Applications**
Generalized sort, transitive closure ... 
transitive_closure(Relation,Item1,Item2):- Predicate =..[Relation,Item1,Item2],
                                           call(Predicate).
transitive_closure(Relation,Item1,Item2):- Predicate =..[Relation,Item1,Link],
                                           call(Predicate),
transitive_closure(Relation,Link,Item2).
**DatabaseProgramming**
Objective:
Logic Programming as Database Programming 
Outline 
- Simple Family Database 
- Recursive Rules 
- Logic Programming and the Relational Database Model (relational algebra)
**Simple Databases**
Basic predicates: father/2,mother/2, male/1, female/1.
father(Father,Child).
mother(Mother,Child).
male(Person). 
female(Person).
son(Son,Parent).
daughter(Daughter,Parent).
parent(Parent,Child).
grandparent(Grandparent,Grandchild).
Question: Which should be facts and which should berules? Example: if parent, male and female are facts then father and mothercould be rules.
father(Parent,Child) :-parent(Parent,Child), male(Parent).
mother(Parent,Child) :-parent(Parent,Child), female(Parent).
Some other relations that could be defined are. 
mother(Woman) :-mother(Woman,Child).
parents(Father,Mother) :-father(Father,Child), mother(Mother,Child).
brother(Brother,Sibling) :-parent(P,Brother), parent(P,Sibling),
    male(Brother), Brother  Sibling.
uncle(Uncle,Person) :-brother(Uncle,Parent), parent(Parent,Person).
sibling(Sib1,Sib2) :-parent(P,Sib1), parent(P,Sib2), Sib1 =\= Sib2.
cousin(Cousin1,Cousin2) :-parent(P1,Cousin1), parent(P2,Cousin2),
  sibling(P1,P2).
What about: sister, niece, full_ sibling, mother_in_law,etc. 
**Recursive Rules**
ancestor(Ancestor,Descendent) :-parent(Ancestor,Descendent).
ancestor(Ancestor,Descendent) :-parent(Ancestor,Person),
  ancestor(Persion,Descendent).
The ancestor relation is an example of the moregeneral relation of transitive closure. Here is an example of the transitiveclosure for graphs. Transitive closure: connected
edge(Node1,Node2).
...
connected(Node1,Node2) :-edge(Node1,Node2).
connected(Node1,Node2) :-edge(Node1,Link), connected(Link,Node2).
**Logic programs and the relationaldatabase model**
The mathematical concept underlying the relationaldatabase model is the set-theoretic
*relation*, which is a subset of theCartesian product of a list of domains. A domain is a set of values. A
*relation*is any subset of the Cartesian product of one or more domains. The members of arelation are called
*tuples*. In relational databases, a relation isviewed as a table. The Prolog view of a relation is that of a set of namedtuples. For example, in Prolog form, here are some unexpected entries in acity-state-population relation.
city_state_population('SanDiego','Texas',4490).
city_state_population('Miami','Oklahoma',13880).
city_state_population('Pittsburg','Iowa',509).
In addition to defining relations as a set of tuples,a relational database management system (DBMS) permits new relations to bedefined via a query language. In Prolog form this means defining a rule. Forexample, the sub-relation consisting of
 those entries where the population isless than 1000 can be defined as follows: 
smalltown(Town,State,Pop) :-city_state_pop(Town,State,Pop), Pop < 1000.
One of the query languages for relational databases isthe Relational Algebra. Its operations are union, set difference, Cartesianproduct, projection, and selection. They may be defined for two relations r ands as follows.
% Union of relations r/n and s/n 
r_union_s(X1,...,Xn) :-r(X1,...,Xn).
r_union_s(X1,...,Xn) :-s(X1,...,Xn).
% Set Difference  r/n $\setminus$ s/n
r_diff_s(X1,...,Xn) :-r(X1,...,Xn), not s(X1,...,Xn).
r_diff_s(X1,...,Xn) :-s(X1,...,Xn), not r(X1,...,Xn).
% Cartesian product r/m, s/n
r_x_s(X1,...,Xm,Y1,...,Yn) :-r(X1,...,Xm), s(Y1,...,Yn).
% Projection
r_p_i_j(Xi,Xj) :- r(X1,...,Xn).
% Selection
r_c(X1,...,Xn) :- r(X1,...,Xn),c(X1,...,Xn).
% Meet
r_m_s(X1,...,Xn) :- r(X1,...,Xn),s(X1,...,Xn).
% Join
r_j_s(X'1,...,X'j,Y'1,...,Y'k) :-r(X1,...,Xn), s(Y1,...,Yn).
The difference between Prolog and a Relational DBMS isthat the in Prolog the relations are stored in main memory along with theprogram whereas in a Relational DBMS the relations are stored in files and theprogram extracts the information from
 the files. 
**Expertsystems**
Expert systems may be programmed in one of two ways inProlog. One is to construct a knowledge base using Prolog facts and rules anduse the built-in inference engine to answer queries. The other is to build amore powerful inference engine in
 Prolog and use it to implement an expertsystem. 
Pattern matching: Symbolic differentiation 
|d(X,X,1)|:-|!.|
|----|----|----|
|d(C,X,0)|:-|atomic(C).|
|d(-U,X,-A)|:-|d(U,X,A).|
|d(U+V,X,A+B)|:-|d(U,X,A), d(V,X,B).|
|d(U-V,X,A-B)|:-|d(U,X,A), d(V,X,B).|
|d(C*U,X,C*A)|:-|atomic(C), CX, d(U,X,A),!.|
|d(U*V,X,B*U+A*V)|:-|d(U,X,A), D(V,X,B).|
|d(U/V,X,A)|:-|d(U*V^-1,X,A)|
|d(U^C,X,C*U^(C-1)*W)|:-|atomic(C), CX, d(U,X,W).|
|d(log(U),X,A*U^(-1))|:-|d(U,X,A).|
**Object-OrientedProgramming**
object( Object, Methods )
/******************************************************************************
                                    OOP
******************************************************************************/
/*=============================================================================
                        Interpreter for OOP
=============================================================================*/
send( Object, Message ) :-get_methods( Object, Methods ),
                           process( Message,Methods ).
get_methods( Object, Methods ) :-object( Object, Methods ).
get_methods( Object, Methods ) :-isa( Object, SuperObject ),
                                  get_methods(SuperObject, Methods ).
process( Message, [Message|_] ).
process( Message, [(Message :-Body)|_] ) :- call( Body ).
process( Message, [_|Methods] ):- process( Message, Methods ).
/*=============================================================================
                        Geometric Shapes
=============================================================================*/
object( polygon( Sides ), [(perimeter( P ) :- sum( Sides, P )) ] ).
object( reg_polygon( Side, N ), [((perimeter( P ) :- P is N*Side)),
                                  (describe :-write('Regular polygon')) ] ).
object( rectangle( Length, Width), 
                 [ (area( A ) :- A is Length *Width ),
                   (describe  :- write('Rectangle of size ' ),
                                 write(Length*Width)) ] ).
object( square( Side ), [(describe :- write( 'Square with side ' ),
                                       write(Side )) ] ).
object( pentagon( Side ), [(describe :- write('Pentagon')) ] ).
isa( square( Side ), rectangle(Side, Side ) ).
isa( square( Side ), reg_polygon(Side, 4 ) ).
isa( rectange( Length, Width ),polygon([Length, Width, Length, Width]) ).
isa( pentagon( Side ),reg_polygon( Side, 5 ) ).
isa( reg_polygon( Side, N ),polygon( L ) ) :- makelist( Side, N, L ).
**Appendix**
The entries in this appendix have the form: pred/ndefinition where pred is the name of the built in predicate, n is its arity(the number of arguments it takes), and definition is a short explanation ofthe function of the predicate.
ARITHMETIC EXPRESSIONS 
+, -, *, /, sin, cos, tan, atan,sqrt, pow, exp, log
I/O 
see/1
the current input stream becomes arg1 
seeing/1
arg1 unifies with the name of the current input stream. 
seen/0
close the current input stream 
tell/1
the current output stream becomes arg1 
telling/1
arg1 unifies with the name of the current output stream. 
told/0
close current output stream 
read/1
arg1 is unified with the next term delimited with a period from thecurrent input stream.
get/1
arg1 is unified with the ASCII code of the next printable character in thecurrent input stream.
write/1
arg1 is written to the current output stream. 
writeq/1
arg1 is written to the current output stream so that it can be read withread.
nl/0
an end-of-line character is written to the current output stream.
spaces/1
arg1 number of spaces is written to the current output stream. 
PROGRAM STATE 
listing/0
all the clauses in the Prolog data base are written to the current outputstream
listing/1
all the clauses in the Prolog data base whose functor name is equal toarg1 are written to the current output stream
clause(H,B)
succeeds if H is a fact or the head of some rule in the data base and B isits body (true in case H is a fact).
PROGRAM MANIPULATION 
consult/1
the file with name arg1 is consulted (loaded into the Prolog data base)
reconsult/1
the file with name arg1 is reconsulted 
assert/1
arg1 is interpreted as a clause and is added to the Prolog data base(functor must be dynamic)
retract/1
the first clause which is unifiable with arg1 is retracted from the Prologdata base (functor must be dynamic)
META-LOGICAL 
ground/1
succeeds if arg1 is completely instantiated (BIM) 
functor/3
succeeds if arg1 is a term, arg2 is the functor, and arg3 is the arity ofthe term.
T =..L
succeeds if T is a term and L is a list whose head is the principlefunctor of T and whose tail is the list of the arguments of T.
name/2
succeeds if arg1 is an atom and arg2 is a list of the ASCII codes of thecharacters comprising the name of arg1.
call/1
succeeds if arg1 is a term in the program. 
setof/3
arg3 is a set (list) of all instances of arg1 for which arg2 holds. Arg3must be of the form X^T where X is an unbound variables in T other than arg1.
bagof/3
arg3 is a list of all instances of arg1 for which arg2 holds. See setof.
\+/1
succeeds if arg1 is not provable (Required instead of **not** in somePrologs if arg1 contains variables.
not/1
same as \+ but may requires arg1 to be completely instantiated 
SYSTEM CONTROL 
halt/0, C-d
exit from Prolog 
DIRECTIVES 
:- dynamic pred/n .
the predicate pred of order n is dynamic 
**References**
Clocksin & Mellish, *Programmingin Prolog* 4th ed. Springer-Verlag 1994.
Hill, P. & Lloyd, J. W., *TheGödel Programming Language* MIT Press 1994.
Hogger, C. J., *Introduction toLogic Programming* Academic Press 1984.
Lloyd, J. W., *Foundations ofLogic Programming* 2nd ed. Springer-Verlag 1987.
Nerode, A. & Shore, R. A., *Logicfor Applications* Springer-Verlag 1993.
Robinson, J. A., *Logic: Formand Function* North-Holland 1979.
Sterling and Shapiro, *The Artof Prolog*. MIT Press, Cambridge, Mass. 1986.
© 1996 by [A. Aaby](http://cs.wwc.edu/~cs_dept/KU/PR/Notices.html)Last Updated:Fri May 2 23:24:37 1997Send comments to: webmaster@cs.wwc.edu
