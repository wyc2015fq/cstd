# Lucas定理：线性求所有逆元的方法 - wishchinYang的专栏 - CSDN博客
2018年07月05日 14:25:24[wishchin](https://me.csdn.net/wishchin)阅读数：1347
*[Miskcoo's Space](http://blog.miskcoo.com)，版权所有丨如未注明，均为原创转载请注明转自：[http://blog.miskcoo.com/2014/09/linear-find-all-invert](http://blog.miskcoo.com/2014/09/linear-find-all-invert)*
主要绕过费马小定理来证明lucas定理，挺有意思..此外设置多进制计算机可以通过移位运算直接加快多进制运算的速度。
1.Lucas定理
# [Lucas定理详解](https://www.cnblogs.com/scx2015noip-as-php/p/lucas.html)
Lucas定理解决的问题是组合数取模。数学上来说，就是求 
。
这里
可能很大，比如达到，而在
以内。显然运用常规的阶乘方法无法直接求解，所以引入Lucas定理。
## Lucas定理
把
和写成进制数的样子（如果长度不一样把短的补成长的那个的长度）：
那么：
证明
如果把Lucas定理从递归的角度理解，它其实是这样的：
设
这个定理的一个很巧妙的证法是通过二项式定理来说明上面的式子是成立的。
首先，对于任意质数pp，有：
其证明可以由费马小定理
直接得出：
所以
（当然同样也有
，具体为什么你可以拆开前面的式子，将其除  和  项外的所有项的系数好好研究一下（其实就是杨辉三角的第p层），可以发现把对称项系数分别合并后都能整除
）
利用这个性质，我们证明Lucas定理：
考察等式左右两边xmxm的系数，可以发现：
所以上面的式子成立，证明完毕。
如果不算预处理什么的，算法时间复杂度为
。如果能够支持预处理，那么就加一个，要不就用快速幂，乘上。
**2. 线性求所有逆元的方法**
前几天在看 lucas 定理的时候发现要求 121 的逆元，然后就看到了一个 
 的做法发现太神了，虽然想起来是挺简单的
这个做法实际上是这样的，首先 111
然后我们设 1
再将这个式子放到
 意义下就会得到
0
两边同时乘上 11
 就会得到
1111011
于是就可以从前面推出当前的逆元了，代码也就一行
```python
A[i] = -(p / i) * A[p % i];
```
Related posts:
- [[数论]Miller-Rabin素性测试](http://blog.miskcoo.com/2014/07/miller-rabin-primality-test)
- [[数论]二次剩余及计算方法](http://blog.miskcoo.com/2014/08/quadratic-residue)
- [扩展欧几里得算法与中国剩余定理](http://blog.miskcoo.com/2014/09/chinese-remainder-theorem)
- [BZOJ-3157. 国王奇遇记](http://blog.miskcoo.com/2014/06/bzoj-3157)
- [BZOJ-3771. Triple](http://blog.miskcoo.com/2015/04/bzoj-3771)
