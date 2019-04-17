# 【2008IO】#ULR 2 A. Goshtasp, Vishtasp and Eidi - CD's Coding - CSDN博客





2014年10月10日 11:37:33[糖果天王](https://me.csdn.net/okcd00)阅读数：472标签：[Codeforces](https://so.csdn.net/so/search/s.do?q=Codeforces&t=blog)
个人分类：[CF-A题终结者](https://blog.csdn.net/okcd00/article/category/2156945)


















Goshtasp was known to be a good programmer in his school. One day Vishtasp, Goshtasp's friend, asked him to solve this task:

Given a positive integer *n*, you should determine whether *n* is
 rich.


The positive integer *x* is rich, if there exists some set of distinct numbers *a*1, *a*2, ..., *a**m* such
 that ![](http://espresso.codeforces.com/e87f6ed8dbb5ba5a37fce497d51dab80d56e0fc2.png).
 In addition: every *a**i* should
 be either a prime number, or equal to 1.


Vishtasp said that he would share his Eidi 50 / 50 with Goshtasp, if he could solve the task. Eidi is money given to children for Noruz by their parents and/or relatives.


Goshtasp needs to solve this problem to get money, you need to solve it to get score!




Input


Input contains a single positive integer *n* (1 ≤ *n* ≤ 10000).




Output


If the number is not rich print 0. Otherwise print the numbers *a*1, ..., *a**m*.
 If several solutions exist print the lexicographically latestsolution. Answers are compared as sequences of numbers, not as strings.


For comparing two sequences *a*1, ..., *a**m* and *b*1, ..., *b**n* we
 first find the first index *i* such that *a**i* ≠ *b**i*,
 if *a**i* < *b**i* then *a* is
 lexicographically earlier and if *b**i* < *a**i* then *b* is
 lexicographically earlier. If *m* ≠ *n* we add zeroes at the end of the smaller sequence (only for the moment of comparison) and then perform the comparison.


You do not need to minimize the number of elements in sequence (i.e. *m*). You just need to print the lexicographically latestsolution.


See samples to find out how to print the sequence.




Sample test(s)




input
11




output
11=11




input
545




output
541+3+1=545















又是一场Unknown Language Round……没办法 A题终结者的No.是按照Round编号来的（啊那个加入专栏的不是按Round号而是按照#号顺序的哦~）

强迫症表示，没什么学习意义，不过是也贴一下完成强迫症的心情：






```
n :=File standardInput readLine asNumber
p :=0
m :=n
while(n>0,for(x,n,1,-1,
q :=0
for(i,2,x-1,if(x%i==0,q :=1;break;))
if(q==0,
if(p==1,"+" print)
p :=1
n :=n-x
x print
break
)
))
"=" print
m println
```





