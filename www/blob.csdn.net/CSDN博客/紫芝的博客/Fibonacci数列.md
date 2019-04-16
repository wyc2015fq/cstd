# Fibonacci数列 - 紫芝的博客 - CSDN博客





2018年05月07日 17:57:21[紫芝](https://me.csdn.net/qq_40507857)阅读数：25








## [点击打开链接](http://sustoj.com/problem.php?cid=1018&pid=0)

## 问题 A: 一道简单的Fibonacci
时间限制: 1 Sec  内存限制: 32 MB
提交: 17  解决: 2
[[提交](http://sustoj.com/submitpage.php?cid=1018&pid=0&langmask=0)][[状态](http://sustoj.com/problemstatus.php?id=1680)][[讨论版](http://sustoj.com/bbs.php?pid=1680&cid=1018)]
## 题目描述


有如下数列：F(0) = 7, F(1) = 11, F(n) = F(n - 1) + F(n - 2)  (n >= 2)。


## 输入


输入由一系列的行构成，每一行包含一个正数 n (n < 1000000)。


## 输出


如果 F(n) 能被 3 整除，则输出 “Yes”，否则输出 “No”。


## 样例输入
0
1
2
3
4
5

## 样例输出
No
No
Yes
No
No
No

## [点击打开链接](http://sustoj.com/problem.php?cid=1018&pid=1)

## 问题 B: Fibonacci变形1
时间限制: 1 Sec  内存限制: 32 MB
提交: 5  解决: 1
[[提交](http://sustoj.com/submitpage.php?cid=1018&pid=1&langmask=0)][[状态](http://sustoj.com/problemstatus.php?id=1681)][[讨论版](http://sustoj.com/bbs.php?pid=1681&cid=1018)]
## 题目描述


有如下数列：F(1) = 1, F(2) = 1, F(n) = ( a * F(n - 1) + b * F(n - 2) ) mod 7。现给定 a, b, n ，求 F(n)。


## 输入


输入由多组数据构成，每组数据一行，分别为 a, b, n (1 <= a, b <= 1000, 1 <= n <= 100000000) 。当 a，b，n 均为 0 时停止输入。


## 输出


对于每组数据，输出 F(n)。每个输出占一行。


## 样例输入
1 1 3
1 2 10
0 0 0

## 样例输出
2
5




