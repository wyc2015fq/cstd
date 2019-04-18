# 基础微积分后记（Epilogue) - 袁萌专栏 - CSDN博客
2013年07月10日 01:27:47[yuanmeng001](https://me.csdn.net/yuanmeng001)阅读数：1166
EPILOGUE
How does the infinitesimal calculus as developed in this book relate to the traditional (or
*ε,δ*) calculus? To get the proper perspective we shall sketch the history of the calculus. 
Many problems involving slopes, areas and volumes, which we would today call calculus problems, were solved by the ancient Greek mathematicians. The greatest
 of them was Archimedes(287-212 B.C.). Archimedes anticipated both the infinitesimal and the* ε,δ* approach to calculus. He sometimes discovered his results by reasoning with infinitesimals, but always published
 his proofs using the “method of exhaustion,” which is similar to the* ε,δ* approach.
Calculus problems became important in the early 1600’s with the development of physics and astronomy. The basic rules for differentiation and integration were
 discovered in that period by informal reasoning with infinitesimals. Kepler, Galileo, Fermat, and Barrow were among the contributors.
In the 1660’s and 1670’s Sir Isaac Newton and Gottfried Wilhelm Leibniz independently “invented” the calculus. They took the major step of recognizing the importance
 of a collection of isolated results and organizing them into a whole.
Newton,atdifferent times, described the derivative of*γ*(which he calledthe“fluxion”of*γ*)in three different ways, roughly
(1)Theratioof an infinitesimal change in*y*to an infinitesimal
 change in*x*(The infinitesimal method.)
(2)Thelimitof the ratio of the change in*y*to the changein*x,*Δ*y*/Δ*x,*asΔ*x*approacheszero.
 (The limit method.)
(3)Thevelocityof*y* where*x* denotestime. (The velocity
 method.)
Inhislater writings Newton sought to avoid infinitesimals andemphasizedthe methods (2) and (3).
Leibnizratherconsistently favored the infinitesimal method but believed(correctly)that the same results could be obtained using only realnumbers. Heregarded
 the infinitesimals as “ideal”numbers like the imaginarynumbers. Tojustify them he proposed his law of continuity:“Inanysupposed transition, ending in any terminus, it is permissibletoinstitute a general reasoning, in which the terminus may alsobeincluded.”This
 “law”is far too imprecise by presentstandards.But it was a remarkable forerunner of the TransferPrinciple on whichmodern infinitesimal calculus is based. Leibnizwas on the righttrack, but 300 years too soon!
Thenotationdeveloped by Leibniz is still in general use today, eventhough it wasmeant to suggest the infinitesimal method:*dy*/*dx*for thederivative
 (to suggest aninfinitesimal change in *y*divided byan
infinitesimalchangein*x* ),and∫baƒ(*x*)*dx*forthe
 integral (to suggest the sum ofinfinitely many infinitesimalquantities ƒ(*x*)*dx*)*.*
Allthreeapproaches had serious inconsistencies which were criticizedmosteffectively by Bishop Berkeley in 1734. However, a precisetreatmentof the calculus was
 beyond the state of the art at thetime, and thethree intuitive descriptions(1)-(3) of the derivativecompeted witheach other for the next two hundred years. Untilsometime after 1820,the infinitesimal method (1) of Leibniz wasdominant on the Europeancontinent,
 because of its intuitive appealand the convenience of theLeibniz notation. In England the velocitymethod (3) predominated; italso has intuitive appeal but cannot bemade rigorous.
In1821A. L. Cauchy published a forerunner of the modern treatment ofthecalculus based on the limit method (2). He defined the integralaswell as the derivative
 in terms of limits, namely
∫baƒ(χ)*d*χ=limΣƒ(χ)Δχ
Hestillused infinitesimals, regarding them as variables which approachzero.From that time on, the limit method gradually became thedominantapproach to calculus,
 while infinitesimals and appeals tovelocitysurvived only as a manner of speaking. There were twoimportant pointswhich still had to be cleared up in Cauchy’swork, however. First,Cauchy’sdefinition of limit was not sufficiently clear; it stillrelied onthe intuitive
 use of infinitesimals. Second, a precisedefinition ofthe real number system was not yet available. Such adefinitionrequired a better understanding of the concepts of set andfunctionwhich were then evolving.
Acompletelyrigorous treatment of the calculus was finally formulatedby KarlWeierstrass in the 1870’s.He introduced the*ε,δ*conditionasthe definition
 of limit. At about the same time a numberofmathematicians, including Weierstrass, succeeded in constructingthereal number system from the positive integers. The problemofconstructing the real number system also led to development ofsettheory by Georg Cantor
 in the 1870’s.Weierstrass’approachhasbecome the traditional or “standard”treatment of calculus asit is usuallypresented today. It begins with the*ε,δ*conditionasthe definition of limit and goes on to develop the calculusentirelyin terms of the real
 number system(with no mention ofinfinitesimals).However, even when calculus is presented in thestandard way, it iscustomary to argue informally in terms ofinfinitesimals, and to usethe Leibniz notation which suggestsinfinitesimals.
Fromthetime of Weierstrass until very recently, it appeared that thelimitmethod(2) had finally won out and the history of elementarycalculuswas closed. But
 in 1934, Thoralf Skolem constructed what wehere callthe hyperintegers and proved that the analogue of theTransferPrinciple holds for them. Skolem’sconstruction (now called theultraproduct construction) was laterextended to a wide class ofstructures, including
 the construction ofthe hyperreal numbers fromthe real numbers.
'SeeKline,p.385. Boyer, p. 217.
Thename“hyperreal”wasfirst used by E. Hewitt in a paper in 1948. The hyperrealnumberswere known for over a decade before they were applied tothecalculus.
Finallyin1961 Abraham Robinson discovered that the hyperreal numbers couldbeused to give a rigorous treatment of the calculus withinfinitesimals.The presentation
 of the calculus which was given inthis book is basedon Robinson’streatment (but modified to make it suitable for afirst course).
Robinson’scalculusis in the spirit of Leibniz’oldmethod of infinitesimals. Therearemajor differences in detail. For instance, Leibniz definedthederivative as
 the ratioΔ*y*/Δ*x*whereΔ*x*isinfinitesimal,
 while Robinson definesthe derivative as the*standardpart*of the ratio Δ*y*/Δ*x*whereΔ*x*isinfinitesimal.
 This is how Robinsonavoids the inconsistencies intheold infinitesimal approach. Also, Leibniz’ vaguelaw ofcontinuity is replaced by the precisely formulated TransferPrinciple.
ThereasonRobinson’s work was notdone sooner is that theTransferPrinciple forthe hyperreal numbers is a type of axiom that was notfamiliar inmathematics until
 recently. It arose in the subject ofmodel theory,which studies the relationship between axioms andmathematicalstructures. The pioneering developments in model theorywere not madeuntil the 1930’s,by GÖdel,Malcev, Skolem, and Tarski; and thesubject hardly existed
 until the1950’s.
Lookingbackwe see that the method of infinitesimals was generally preferredoverthe method of limits for over 150 years after Newton andLeibnizinvented the calculus,
 because infinitesimals have greaterintuitiveappeal. But the method of limits was finally adopted around1870because it was the first mathematically precise treatment ofthecalculus. Now it is also possible to use infinitesimals inamathematically precise way.
 Infinitesimals in Robinson’ssense havebeen applied not only to the calculus but to the muchbroader subjectof analysis. They have led to new results andproblems in mathematicalresearch. Since Skolem’sinfinite hyperintegers are usually callednonstandard integers.Robinson
 called the newsubject“nonstandardanalysis.” (he called the realnumbers“standard”andthe other hyperreal numbers “nonstandard.”Thisis the origin of the name“standardpart.”)
Thestartingpoint for this course was a pair of intuitive pictures ofthe real andhyperreal number systems. These intuitive pictures arereally onlyrough sketches
 that are not completely trustworthy. Inorder to besure that the results are correct, the calculus must bebased onmathematically precise descriptions of these number systems,whichfill in the gaps in the intuitive pictures. There are two waysto dothis. The quickest
 way isto list the mathematical properties of thereal and hyperrealnumbers. These properties are to be accepted asbasic and are calledaxioms. The second way of mathematicallydescribing the real andhyperreal numbers is to start with thepositive integers and,
 step bystep, construct the integers, therational numbers, the real numbers,and the hyperreal numbers. Thissecond method is better because itshows that there really is astructure with the desired properties.At the end of this epilogue weshall briefly outline
 the constructionof the real and hyperrealnumbers and give some examples ofinfinitesimals.
Wenowturn to the first way of mathematically describing the realandhyperreal numbers. We shall list two groups of axioms intheepilogue, one for the real numbers
 and one for the hyperrealnumbers.The axioms for the hyperreal numbers will just be morecarefulstatements of the Extension Principle and Transfer PrincipleofChapter 1. The axioms for the real numbers come in three sets:theAlgebraic Axioms, the Order Axioms,
 and the Completeness Axiom.Allthe familiar facts about the real numbers can be proved usingonlythese axioms.
1.ALGEBRAICAXIOMSFOR THE REAL NUMBERS
A   Closure laws0*and*1*arerealnumbers. If a and b are real numbers, then so are a + b, ab and-a.If a is a real number and a*≠*0,then*1*/aisa real number.*
B   Commutativelaws    *a+b= b+a  ab = ba*
C   Associativelaws     *a+(b+c)= (a+b) +c    a(bc) = (ab) c.*
DIdentitylaws0*+a=a               *1·*a= a .*
E   Inverselaws*a+ (-a)=0     ifa*≠0,    a.1
F   Distributivelaw        a·(b + c)= ab + ac
**DEFINITION**
The ***positiveintegers***arethereal
 numbers 1,2 = 1+1, 3 = 1+1+1 ,
4= 1+1+1+1,and so on.
║. ORDERAXIOMS FOR REAL NUMBERS
A   0<1.
B   Transitivelaw  if a< b and b<c then a< c.
C   Trichotomylaw Exactly one of the relations a<b, a=b , b<a,holds.
D   Sumlaw  If a< b , then a+c < b+c.
E   productlaw  If a <b and o<c, then ac< bc.
F   Rootaxiom   For every real number a> 0 and everypositiveinteger n, there is a real number b>0 such that_____=a
Ⅲ.  COMPLETENESS AXIOM
Let Abea set of real numbers such that whenever x and y are in A, anyrealnumber between x and y is in A. Then A is an interval.
THEOREM     
Anincreasingsequence <___> either converges ordiverges to ∞.
PROOF letT be the set of all real number x such that x ≤____forsome n. T is obviously nonempty.
Case1 T is the whole real line. If H is infinite we have x ≤____forall real numbers x. So__is
 positiveinfinite and <__> diverges to ∞
.
Case2  T is not the whole real line. By the CompletenessAxiom, T is aninterval (－∞,b]or
 (－∞,b).For each real x < b, we have
for somen.It follows that for infinite H,__≤band__≈b.Therefore<__>convergesto
 b.
Wenowtake up the second group of axioms, which give the properties ofthehyperreal numbers. There will be two axioms, called theExtensionAxiom and the Transfer
 Axiom, which correspond to theExtensionPrinciple and Transfer Principle of Section 1.5. We firststate theExtension Axiom.
1*. EXTENSION AXIOM
(a ) The set R of real numbers is a subset of the set R* of hyperreal numbers.
(b) There is given a relation <* on R*, such that the order relation < on R is a subset of <*, <* is transitive (a <* b and b <*c implies
 a <* c), and <* satisfies the Trichotomy Law: for all a,b in R*, exactly one of a <* b, a = b, b <* a holds.
   (c) There is a hyperreal number ε such that 0 <* ε and ε < * r for each positive real number r.
(d) For each real function f, there is given a hyperreal function f* with the same number of variables, called the
 natural extension of f.
Part (c) of the Extension Axiom states that there is at least one positive infinitesimal. Part (d) gives us the natural extension for each real function. The
 Transfer Axiom will say that this natural extension has the same properties as the original function.
Recall that the Transfer Principle of Section 1.5 made use of the intuitive idea of a real statement. Before we can state the Transfer Axiom, we must give an
 exact mathematical explanation of the notion of a real statement. This will be done in several steps, first introducing the concepts of a real expression and a formula.
We begin with the concept of a real expression, or term, built up from variables and real constants using real functions. Real expressionscan be built up as
 follows:
(1) A real constantstanding alone isa real expression.
A variable standing alone is a real expression.
Ifeis a real expression, and f is a real function of one variable,thenf(e) is a real expression. Similarly, if____________arereal
 expressions, andg is a real function of n variables, theng(_________)isa real expression.
Step(3)canbe used repeatedly to build up longer expressions. Here aresomeexamples of real expressions, where x and y are variables:
Byaformula, we mean a statement of one of the following kinds, wheredand e are real expressions:
(1)Anequationbetween
 two real expressions, d = e.
(2)Aninequalitybetween
 two real expressions, d< e, d≤e, d> e,d≥e, or d≠e.
（3）Astatementof
 the form “ *e*isdefined”or“*e*isundefined.”
Herearesome examples of formulas:
*x*+*y*=5,
*f(x)= *1－x²
1+x²
*g(x,y)<**f(t),*
*f (x,x)*isundefined.
Ifeachvariable in a formula is replaced by a real number, the formulawillbe
 either true or false. Ordinarily, a formula will be true forsomevalues of the variables and false for others. For example,theformula*x*+*y*=5willbe
 true when (*x*,*y*)=(4,1)andfalse
 when (*x,y*)=(7,－2).
**DEFINITION**
*Arealstatement is either a nonempty finite set of formulas Tor acombination involving twononempty finite sets of formulas S andTthat states that*“*whenever every formulain Sis true,everyformula in Tis true.*”
Weshallgive several comments and examples to help make this definitionclear.Sometimes, instead
 of writing “whenevereveryformula in S is true, every formula in Tistrue”weusethe
 shorter form“ifSthenT”forareal statements.Eachof the Algebraic Axioms for the Real Numbers is areal statement. Thecommutative laws, associative
 laws, identity laws,and distributivelaws are real statements.
Forexample,the commutative laws are the pair of formulas
*a+b=b+a,**ab=ba,*
Whichinvolvethe two variables*a*and*b*.Theclosure
 laws may be expressed as four real statements:
*a+b*isdefined,
*ab*isdefined,
-*a*isdefined,
If*a*≠0,then1*/a*isdefined.
Theinverselaws consist of two more real statements. The Trichotomy Lawis partof the Extension Axiom, and all of the other Order
 Axioms forthe RealNumbers are real statements. However, the CompletenessAxioms is not areal statement, because it is not built up fromequations andinequalities between terms.
Atypicalexample of a real statement is the inequality for exponentsdiscussedin Section 8.1:
if a≥0,andq≥1,then(a+1)q≥aq+1
Thisstatementis true for all real numbers*a*and*q.*
Aformulacan be given a meaning in the hyperreal number system as wellas inthe real number
 system. Consider a formula with thetwovariables*x*and*y*.When*x*and*y*arereplacedby
 particular real numbers, the formula in the hyperrealnumbersystem. To give the formula a meaning in the hyperreal numbersystem,we replace each real function by its natural extension andreplace thereal order relation < by the hyperreal relation<*.When*x*and*y*arereplacedby
 hyperreal numbers, each real function *f*isreplacedby its natural extension*f**,andthe
 real order relation <isreplaced by<*, the formula will beeither true or false in thehypereal number system.
Forexample,the formula*x+y*=5istrue
 in the hyperreal number system when(*x,y*)=(2－*ε,3+**ε),*
butfalsewhen (*x,y*)=(2+*ε,3+ε),*if*ε*isnonzero.
Wearenow ready to state the Transfer Axiom.
║*. TRANSFER  AXIOM
*Every real statement that holds for all real numbers holds for all hyperreal numbers.*
It is possible to develop the whole calculus course as presented in this book from these axioms for the real and hyperreal numbers.
 By the Transfer Axiom, all the Algebraic Axioms for the Real Numbers also hold true for the hyperreal numbers. In other words, we can transfer every Algebraic Axiom for the real numbers to the hyperreal numbers.We can also transfer every Order Axiom for the
 real numbers to the hyperreal numbers. The Trichotomy Law is part of the Extension Axiom.Each of the other Order Axioms is a real statement and thus carries over to the hyperreal numbers by the Transfer Axiom. Thus we can make computations with the hyperreal
 numbers in the same way as we do for the real numbers.
There is one fact of basic importance that we state now as a theorem.
**THEOREM (Standard Part  Principle)**
*For every finite hyperreal number b, there is exactly one real number r that is infinitely close to b.*
*PROOF*We first show that there cannot be more
 than one real number infinitely close to*b.*Suppose* r*and
*s *are real numbers such that*r*≈*b*
 and*s*≈*b*. Then*r*≈*s*,
 and since*r *and*s*are real,*r*must be equal to*s*.Thus
 there is at most one real number infinitely close to* b.*
Wenowshow that there is a real number infinitely close to*b*.Let*A*betheset
 of all real numbers less than*b*.Thenany real number between two elements of*A*isanelement
 of*A*.Bythe Completeness Axiom for the real numbers,*A*isaninterval.
 Since the hyperreal number *b*isfinite.*A*mustbean
 interval of the form(*－*∝*,r*)or(*－*∝*,r*]forsomereal
 number*r.*Everyrealnumber*s <r*belongsto*A,*so*s< b. *Also,everyreal number *t>r*doesnotbelong
 to*A*,so*t*≥*b*.Thisshowsthat*r*isinfinitelyclose
 to*b.*
Itwaspointed out earlier that the Completeness Axiom does not qualifyas areal statement. For
 this reason, the Transfer Principle cannotbe usedto transfer the Completeness Axiom to the hyperreal numbers.In fact,the Completeness Axiom is not true for the hyperrealnumbers. By aclosed hyperreal interval, we mean a set of hyperrealnumbers of theform [*a,b*],theset
 of all hyperreal numbers *x*forwhich*a*≤*x*≤*b*,where*a*and*b*arehyperrealconstants.
 Open and half-open hyperreal intervals aredefined in asimilar way. When we say that the Completeness Axiom isnot true forthe hyperreal numbers, we mean that there actually aresets A ofhyperreal numbers such that:
(a)whenever*x*and*y*arein*A*,anyhyperreal
 number between*x*and*y*isin*A*.
(b)*A*isnota
 hyperreal interval.
**Herearetwoquitefamiliar examples.**
**EXAMPLE1**Theset*A*ofallinfinitesimals
 has property (a ) above but is not ahyperrealinterval. It has property (a ) because any hyperreal numberthat isbetween two infinitesimals is itself infinitesimal. We showthat*A*isnota
 hyperreal interval.*A*cannotbeof the form [*a,*∞]or(*a,*∞)becauseevery
 infinitesimal is less than 1.*A*cannotbeof the form [*a,b*]or(*a, b*],becauseif*b*ispositiveinfinitesimal, then 2*·b*isalarger
 infinitesimal. *A*cannotbeof the form [*a,b*)or(*a,b*),becauseif*b*ispositiveand
 not infinitesimal, then*b*/2isless than*b*butstillpositive and not infinitesimal.
The
Calculusproblems became important in the early 1600’swith the developmentof physics and astronomy. The basic rules fordifferentiation andintegration were discovered
 in that period byinformal reasoning withinfinitesimals. Kepler, Galileo, Fermat, andBarrow were among thecontributors.
setBofall finite hyperreal numbers is another example of a set thathasproperty (a ) above but is not an interval.
Herearesome examples that may help to illustrate the nature of thehyperrealnumber system and the use of the Transfer Axiom.
**EXAMPLE2**let*f*bethereal
 function given by the equation.
Itsgraphis the unit semicircle with center at the origin. The followingtworeal statements
 hold for all real numbers*x*:
BytheTransfer Axiom, these real statements also hold for allhyperrealnumbers*x*.Thereforethe
 natural extension *f**of*f*isgivenby the
 same equation 
Thedomainof*f**istheset
 of all hyperreal numbers between－1and1.
 The hyperreal graph of*f**,shownin Figure E.1, can be drawn on paper by drawing the realgraphof*f(x)*andtrainingan
 infinitesimal microscope on certain key points.
**EXAMPLE3**let*f*betheidentity
 function on the real numbers, *f*(*x*)=*x*.Bythe
 Transfer Axiom, the equation *f*(*x*)=*x*istruefor
 all hyperreal x. Thus the natural extension*f**of*f*isdefined,and*f*(x)=x*forallhyperreal*x*.FigureE.2
 shows the hyperreal graph of*f**.Undera microscope, it has a 45°slope.
Hereisan example of a hyperreal function that is not the naturalextensionof a real function.
**FigureE.1**
**FigureE.2**
**EXAMPLE4 **Onehyperrealfunction,which
 we have already studied in some detail, is the*standardpart function st(x*).Forreal numbers the standard part function has the same values astheidentity function,
*st(x)=x*forallreal*x*.
However,thehyperreal graph of*st(x)*,shownin
 Figure E.3, is very different from the hyperreal graph oftheidentity function *f**.Thedomain of the standard part function is the set of all finitenumbers,while*f**hasdomain*R**.Thusfor
 infinite*x*,*f*(x)=x*,but*st(x)*isundefined.If*x*isfinitebut
 not real,*f*(x)=x*but*st(x)*≠*x*.Underthe
 microscope, an infinitesimal piece of the graph of thestandardpart function is horizontal, while the identity function hasa45°slope.
Thestandardpart function is not the natural extension of the identityfunction,and hence is not the natural extension of any realfunction.
**FigureE.3**
Thestandardpart function is the only hyperreal function used in thiscourseexcept for natural extensions of real functions.
Weconcludewith a few words about the construction of the real and thehyperrealnumbers. Before
 Weierstrass, the rational numbers were onsolid groundbut the real numbersweresomething new. Before one could safely usethe axioms for the realnumbers, it had to be shown that the axiomsdid not lead to acontradiction. This was done by starting with therational
 numbersand constructing a structure which satisfied all theaxioms for thereal numbers. Since anything proved from the axioms istrue in thisstructure, the axioms cannot lead to a contradiction.
Theideais to construct real numbers out of Cauchy sequences ofrationalnumbers.
**DEFINITION**
*ACauchySequence is a sequence <**________**>ofnumbers such that for every real ε >0 there is an integer**____**such that the numbers*
*areallwithin εof eachother.*
TwoCauchysequences
ofrationalnumbers are called Cauchy equivalent, in symbols <_____>=<____>,ifthe
 difference sequence
convergestozero. (Intuitively this means that the two sequences have thesamelimit.)
**PROPERTIESOFCAUCHY EQUIVALENCE**
(1)If<_______>=<______>and<______>=<_______>
thenthesum sequences are equivalent.
(2)Underthesame
 hypotheses, the product sequences are equivalent,
(3)if__=__forallbut
 finitely many*n,*then
Thesetof real numbers is then defined as the set of all equivalenceclassesof Cauchy sequences
 of rational numbers. A rationalnumber*r*correspondstothe equivalence class of the constant sequence <*r,r,r,…*>.Thesum
 of the equivalence class of <___>andthe equivalence class of
 < ___>isdefined as the equivalence class of the sum sequence
Theproductis defined in a similar way. It can be shown that all theaxioms forthe real numbers hold for this structure.
Todaythereal numbers are on solid ground and the hyperreal numbers are anewidea. Robinson
 used the ultraproduct construction of Skolem toshowthat the axioms for the hyperreal numbers (for example, as usedinthis book) do not lead to a contradiction. The method is much liketheconstruction of the real numbers from the rationals. But thistime thereal
 number system is the starting point. Weconstruct*hyperreal*numbersoutof arbitrary (not just Cauchy) sequences of real numbers.
Byan*ultraproductequivalence*wemeanan
 equivalence relation=ontheset of all sequences of real numbers
 which have the propertiesofCauchy equivalence (1)-(3) and also
(4)Ifeach__belongstothe
 set {0,1} the <__>isequivalent to exactly one of the constant
 sequences <0,0,0,…>or<1,1,1,…>.
Givenanultraproduct equivalence relation, the set of hyperreal numbersisdefined as the set
 of all equivalence classes of sequences ofrealnumbers. A real number*r*correspondstothe equivalence class of the constant sequence <*r*,*r,r…*>.Sumsand
 products are defined as for Cauchy sequences. Thenaturalextension *f**ofareal function*f(x)*isdefinedso
 that the image of the equivalence class of <__>isthe equivalence
 class of <___>.Itcan be proved that ultraproduct equivalence
 relations exist, andthatall the axioms for the real and hyperreal numbers hold forthestructure defined in this way.
Whenhyperrealnumbers are constructed as equivalence classes of sequencesof realnumbers, we can give specific examples of infinite
 hyperrealnumbers.The equivalence class of 
<1,2,3,……*n,*……>
isapositive infinite hyperreal number. The equivalence class of
<1,4,9……*n²*……>
islarger,and the equivalence class of
<1,2,4……__……>
isastill larger infinite hyperreal number.
Wecanalso give examples of nonzero infinitesimals. The equivalenceclassesof
<1, 1/2,1/3……1/n,……>,
<1,1/4,1/9……n–²,……>,
and                 <1,1/2,1/4……___________,……>,
areprogressivelysmaller positive infinitesimals.
Themistakeof Leibniz and his contemporaries was to identify alltheinfinitesimals with zero.
 This leads to an immediatecontradictionbecause*dy/dx*becomes0/0.In the present treatment the equivalence classes of
<1, 1/2,1/3,……, 1/n, ……>
and     <0,0, ……0, ……>
aredifferenthyperreal numbers. They are not equal but merely have thesamestandard part, zero. This avoids the contradiction and
 onceagainmakes infinitesimals a mathematically sound method.
Formoreinformation about the ideas touched on in this epilogue, seetheinstructor’ssupplement,*FoundationsofInfinitesimal Calculus,*whichhasa self-contained treatment of ultraproducts and the hyperrealnumbers.
**FORFURTHERREADING ON THE HISTORY OF THE CALCULUS SEE:**
*TheHistoryof the Calculus and its Conceptual Development: Carl c.Boyer, Dover,New York, 1959.*
*MathematicalThoughtfrom Ancient to Modern Times; Morris Kline, Oxford Univ.*
*Press,NewYork, 1972.*
*No-standardAnalysis:Abraham Robinson, North-Holland, Amsterdam, London, 1966.*
**FOR ADVANCED READING ON INFINITESIMAL ANALYSIS SEE NON-STANDARD ANALYSIS BY ABRAHAM ROBINSON AND:**
*LecturesonNon-standard Analysis; M. Machover and J. Hirschfeld,Springer-Verlag,Berlin, Heidelberg, New York, 1969.*
VictoriaSymposiumon Nonstandard Analysis; A. Hurd and p. Loeb,Springer-Verlag, Berlin,Heidelberg, New York,
 1973.
StudiesinModel Theory; M. Morley, Editor, Mathematical Association ofAmerica,Providence, 1973.
AppliedNonstandardAnalysis: M. Davis, Wiley, New York, 1977.
Introductiontothe Theory of Infinitesimals: K.D Stroyan and W.A. J.Luxemburg,Academic Press, New York and London,
 1976.
FoundationsofInfinitesimal Stochastic Analysis: K.D. Stroyan and J.M..Bayod,North-Holland Publ.Co., in press.
