# 【水读题】#47 A. Domino piling - CD's Coding - CSDN博客





2014年07月30日 15:05:47[糖果天王](https://me.csdn.net/okcd00)阅读数：608
所属专栏：[CF-A Terminator](https://blog.csdn.net/column/details/cf-amaster.html)


















You are given a rectangular board of *M* × *N* squares. Also you are given an unlimited number of standard domino pieces of 2 × 1squares.
 You are allowed to rotate the pieces. You are asked to place as many dominoes as possible on the board so as to meet the following conditions:


1. Each domino completely covers two squares.


2. No two dominoes overlap.


3. Each domino lies entirely inside the board. It is allowed to touch the edges of the board.


Find the maximum number of dominoes, which can be placed under these restrictions.




Input


In a single line you are given two integers *M* and *N* —
 board sizes in squares (1 ≤ *M* ≤ *N* ≤ 16).




Output


Output one number — the maximal number of dominoes, which can be placed.




Sample test(s)




input
2 4




output
4




input
3 3




output
4












读题读懂了就行了……



N×M的地板，放1×2的多米诺牌，问最多放多少块……

肯定是偶数铺满奇数留一格么……（感觉太水了结果居然还wa了一发，因为忘了写cin就直接cout了 =  =）



```cpp
#include <iostream>
using namespace std;
int main()
{
	int m,n;	
	cin>>m>>n; 
	cout<<m*n/2<<endl;
	return 0;
}
```








