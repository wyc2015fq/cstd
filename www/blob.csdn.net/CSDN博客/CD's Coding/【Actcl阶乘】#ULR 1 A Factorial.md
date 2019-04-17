# 【Actcl阶乘】#ULR 1 A. Factorial - CD's Coding - CSDN博客





2014年08月20日 12:03:03[糖果天王](https://me.csdn.net/okcd00)阅读数：560








这道题没有太大意义……主要是也在A题= =

是考语言的……ActiveTCL限定……无视掉就好













Print the factorial of the given integer number *n*. The factorial of *n* is
 equal to 1·2·...·*n*.




Input


The only line contains *n* (1 ≤ *n* ≤ 10).




Output


Print the factorial of *n*.




Sample test(s)




input
3




output
6




input
5




output
120






















set A [gets stdin]set fact 1for{set i 0}{$i<$A}{incr i}{set fact [expr{$fact*($i+1)}]}puts$fact








