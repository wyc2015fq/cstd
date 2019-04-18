# k阶齐次线性递推式 - 知乎
# 

> 给定k阶齐次线性递推式 ![a_n=\sum_{i=1}^kf_ia_{n-i}](https://www.zhihu.com/equation?tex=a_n%3D%5Csum_%7Bi%3D1%7D%5Ekf_ia_%7Bn-i%7D) ，以及数列的前k项，求 ![a_n](https://www.zhihu.com/equation?tex=a_n) 。
其中 ![k\le10^5,n\leq10^{18}](https://www.zhihu.com/equation?tex=k%5Cle10%5E5%2Cn%5Cleq10%5E%7B18%7D) 。

## **朴素算法**

![O(nk)](https://www.zhihu.com/equation?tex=O%28nk%29) 。

## **矩阵优化**

考虑构造如下 ![k](https://www.zhihu.com/equation?tex=k) 阶方阵：

![\begin{pmatrix} f_1&f_2&f_3&\cdots&f_k\\ 1&0&0&\cdots&0\\ 0&1&0&\cdots&0\\  \vdots&\vdots&\vdots&\ddots&\vdots\\ 0&0&0&\cdots&0 \end{pmatrix}\\](https://www.zhihu.com/equation?tex=%5Cbegin%7Bpmatrix%7D+f_1%26f_2%26f_3%26%5Ccdots%26f_k%5C%5C+1%260%260%26%5Ccdots%260%5C%5C+0%261%260%26%5Ccdots%260%5C%5C++%5Cvdots%26%5Cvdots%26%5Cvdots%26%5Cddots%26%5Cvdots%5C%5C+0%260%260%26%5Ccdots%260+%5Cend%7Bpmatrix%7D%5C%5C) 以及初始向量：
![\begin{pmatrix} a_k\\a_{k-1}\\a_{k-2}\\\vdots\\a_1 \end{pmatrix}\\](https://www.zhihu.com/equation?tex=%5Cbegin%7Bpmatrix%7D+a_k%5C%5Ca_%7Bk-1%7D%5C%5Ca_%7Bk-2%7D%5C%5C%5Cvdots%5C%5Ca_1+%5Cend%7Bpmatrix%7D%5C%5C)

可以得到答案：

![\begin{pmatrix} a_n\\a_{n-1}\\a_{n-2}\\\vdots\\a_{n-k+1} \end{pmatrix} = \begin{pmatrix} f_1&f_2&f_3&\cdots&f_k\\ 1&0&0&\cdots&0\\ 0&1&0&\cdots&0\\  \vdots&\vdots&\vdots&\ddots&\vdots\\ 0&0&0&\cdots&0 \end{pmatrix}^{n-1} \begin{pmatrix} a_k\\a_{k-1}\\a_{k-2}\\\vdots\\a_1 \end{pmatrix}\\](https://www.zhihu.com/equation?tex=%5Cbegin%7Bpmatrix%7D+a_n%5C%5Ca_%7Bn-1%7D%5C%5Ca_%7Bn-2%7D%5C%5C%5Cvdots%5C%5Ca_%7Bn-k%2B1%7D+%5Cend%7Bpmatrix%7D+%3D+%5Cbegin%7Bpmatrix%7D+f_1%26f_2%26f_3%26%5Ccdots%26f_k%5C%5C+1%260%260%26%5Ccdots%260%5C%5C+0%261%260%26%5Ccdots%260%5C%5C++%5Cvdots%26%5Cvdots%26%5Cvdots%26%5Cddots%26%5Cvdots%5C%5C+0%260%260%26%5Ccdots%260+%5Cend%7Bpmatrix%7D%5E%7Bn-1%7D+%5Cbegin%7Bpmatrix%7D+a_k%5C%5Ca_%7Bk-1%7D%5C%5Ca_%7Bk-2%7D%5C%5C%5Cvdots%5C%5Ca_1+%5Cend%7Bpmatrix%7D%5C%5C)

利用快速幂计算矩阵乘法，时间复杂度 ![O(k^3\log n)](https://www.zhihu.com/equation?tex=O%28k%5E3%5Clog+n%29) 。

## **多项式优化**

设生成函数 ![G(x)=\sum a_ix^i](https://www.zhihu.com/equation?tex=G%28x%29%3D%5Csum+a_ix%5Ei)，问题转化为求解 ![[x^n]G(x)](https://www.zhihu.com/equation?tex=%5Bx%5En%5DG%28x%29) 。

定理： 由![k](https://www.zhihu.com/equation?tex=k) 阶齐次线性递推式所确定的数列的每一项都可以由该数列前 ![k](https://www.zhihu.com/equation?tex=k) 项线性组合得到。

证明：

当 ![n\le k](https://www.zhihu.com/equation?tex=n%5Cle+k) 时显然成立。

当 ![n\gt k](https://www.zhihu.com/equation?tex=n%5Cgt+k) 时，由递推式可得：![a_nx^n=\sum_{i=1}^kf_ia_{n-i}x^{n-i}\\](https://www.zhihu.com/equation?tex=a_nx%5En%3D%5Csum_%7Bi%3D1%7D%5Ekf_ia_%7Bn-i%7Dx%5E%7Bn-i%7D%5C%5C)

所以定理成立。

上式代入 ![n=k+1](https://www.zhihu.com/equation?tex=n%3Dk%2B1) ，移项，得：

![a_{k+1}x^{k+1}-\sum_{i=1}^kf_{k-i+1}a_{i}x^{i}=0\\](https://www.zhihu.com/equation?tex=a_%7Bk%2B1%7Dx%5E%7Bk%2B1%7D-%5Csum_%7Bi%3D1%7D%5Ekf_%7Bk-i%2B1%7Da_%7Bi%7Dx%5E%7Bi%7D%3D0%5C%5C)

设 ![P(x)=a_{k+1}x^{k+1}-\sum_{i=1}^kf_{k-i+1}a_{i}x^{i}](https://www.zhihu.com/equation?tex=P%28x%29%3Da_%7Bk%2B1%7Dx%5E%7Bk%2B1%7D-%5Csum_%7Bi%3D1%7D%5Ekf_%7Bk-i%2B1%7Da_%7Bi%7Dx%5E%7Bi%7D),那么 ![P(x)=0](https://www.zhihu.com/equation?tex=P%28x%29%3D0) 且 ![P(x)](https://www.zhihu.com/equation?tex=P%28x%29) 是一个 ![k+1](https://www.zhihu.com/equation?tex=k%2B1) 阶多项式。

另设多项式 ![Q(x),R(x)](https://www.zhihu.com/equation?tex=Q%28x%29%2CR%28x%29) 且 ![R(n)](https://www.zhihu.com/equation?tex=R%28n%29) 的阶小于 ![k+1](https://www.zhihu.com/equation?tex=k%2B1)。则 ![x^n](https://www.zhihu.com/equation?tex=x%5En)可以表示成![x^n=P(x)Q(x)+R(x)](https://www.zhihu.com/equation?tex=x%5En%3DP%28x%29Q%28x%29%2BR%28x%29) 。其中 ![Q(x)](https://www.zhihu.com/equation?tex=Q%28x%29) 的值以及 ![R(x)](https://www.zhihu.com/equation?tex=R%28x%29) 的值都可以惟一确定。

这同样是多项式带余除法的定义，于是有 ![R(x)\equiv x^n(\text{mod } P(x))](https://www.zhihu.com/equation?tex=R%28x%29%5Cequiv+x%5En%28%5Ctext%7Bmod+%7D+P%28x%29%29) 。又因为有 ![P(x)=0](https://www.zhihu.com/equation?tex=P%28x%29%3D0) ，所以得 ![x^n=R(x)](https://www.zhihu.com/equation?tex=x%5En%3DR%28x%29) 。 ![R(x)](https://www.zhihu.com/equation?tex=R%28x%29) 的阶小于 ![k+1](https://www.zhihu.com/equation?tex=k%2B1) ，所以 ![R(x)](https://www.zhihu.com/equation?tex=R%28x%29) 其实就是 ![a_n](https://www.zhihu.com/equation?tex=a_n) 关于数列前 ![k](https://www.zhihu.com/equation?tex=k) 项的线性表达式。

利用快速幂计算出 ![R(x)](https://www.zhihu.com/equation?tex=R%28x%29) ，时间复杂度 ![O(k^2\log n)](https://www.zhihu.com/equation?tex=O%28k%5E2%5Clog+n%29) 。已经可以满足大部分神仙题的要求。

利用FFT计算多项式乘法以及取模，时间复杂度 ![O(k\log k \log n)](https://www.zhihu.com/equation?tex=O%28k%5Clog+k+%5Clog+n%29) 。

另一种思路：

由于已经知道 ![a_n](https://www.zhihu.com/equation?tex=a_n) 可以由 ![a_{1\text{~}k}](https://www.zhihu.com/equation?tex=a_%7B1%5Ctext%7B~%7Dk%7D) 线性表达，那么就可以根据递推式替换 ![a_n](https://www.zhihu.com/equation?tex=a_n) 。

举一个具体的例子，给定递推式 ![a_n=a_{n-1}+a_{n-2}+a_{n-3}](https://www.zhihu.com/equation?tex=a_n%3Da_%7Bn-1%7D%2Ba_%7Bn-2%7D%2Ba_%7Bn-3%7D) ，那么 ![\begin{align} a_6&=a_5+a_4+a_3\\ &=(a_4+a_3+a_2)+a_4+a_3=2a_4+2a_3+a_2\\ &=2(a_3+a_2+a_1)+2a_3+a_2=4a_3+3a_2+2a_1 \end{align}\\](https://www.zhihu.com/equation?tex=%5Cbegin%7Balign%7D+a_6%26%3Da_5%2Ba_4%2Ba_3%5C%5C+%26%3D%28a_4%2Ba_3%2Ba_2%29%2Ba_4%2Ba_3%3D2a_4%2B2a_3%2Ba_2%5C%5C+%26%3D2%28a_3%2Ba_2%2Ba_1%29%2B2a_3%2Ba_2%3D4a_3%2B3a_2%2B2a_1+%5Cend%7Balign%7D%5C%5C)

 如果以多项式大除法来理解：![\begin{align} x^2+x+2 \\ x^4-x^3-x^2-x)\overline{x^6\space\space\space\space\space\space\space\space\space\space\space\space\space\space\space\space\space\space\space\space\space\space\space\space\space\space\space\space\space\space\space\space\space\space\space\space\space\space\space\space\space\space\space\space\space\space\space\space\space}\\ \underline{x^6-x^5-x^4-x^3\space\space\space\space\space\space\space\space\space\space\space\space\space\space\space\space\space\space\space\space\space\space}\\ \space\space\space\space\space\space\space\space\space x^5+x^4+x^3\space\space\space\space\space\space\space\space\space\space\space\space\space\space\space\space\space\space\space\space\space\space\\ \underline{x^5-x^4-x^3-x^2\space\space\space\space\space\space\space\space\space\space\space\space\space}\\ \underline{2x^4+2x^3+x^2\space\space\space\space\space\space\space\space\space\space\space\\ 2x^4-2x^3-2x^2-2x}\\ 4x^3+3x^2+2x \end{align}\\](https://www.zhihu.com/equation?tex=%5Cbegin%7Balign%7D+x%5E2%2Bx%2B2+%5C%5C+x%5E4-x%5E3-x%5E2-x%29%5Coverline%7Bx%5E6%5Cspace%5Cspace%5Cspace%5Cspace%5Cspace%5Cspace%5Cspace%5Cspace%5Cspace%5Cspace%5Cspace%5Cspace%5Cspace%5Cspace%5Cspace%5Cspace%5Cspace%5Cspace%5Cspace%5Cspace%5Cspace%5Cspace%5Cspace%5Cspace%5Cspace%5Cspace%5Cspace%5Cspace%5Cspace%5Cspace%5Cspace%5Cspace%5Cspace%5Cspace%5Cspace%5Cspace%5Cspace%5Cspace%5Cspace%5Cspace%5Cspace%5Cspace%5Cspace%5Cspace%5Cspace%5Cspace%5Cspace%5Cspace%5Cspace%7D%5C%5C+%5Cunderline%7Bx%5E6-x%5E5-x%5E4-x%5E3%5Cspace%5Cspace%5Cspace%5Cspace%5Cspace%5Cspace%5Cspace%5Cspace%5Cspace%5Cspace%5Cspace%5Cspace%5Cspace%5Cspace%5Cspace%5Cspace%5Cspace%5Cspace%5Cspace%5Cspace%5Cspace%5Cspace%7D%5C%5C+%5Cspace%5Cspace%5Cspace%5Cspace%5Cspace%5Cspace%5Cspace%5Cspace%5Cspace+x%5E5%2Bx%5E4%2Bx%5E3%5Cspace%5Cspace%5Cspace%5Cspace%5Cspace%5Cspace%5Cspace%5Cspace%5Cspace%5Cspace%5Cspace%5Cspace%5Cspace%5Cspace%5Cspace%5Cspace%5Cspace%5Cspace%5Cspace%5Cspace%5Cspace%5Cspace%5C%5C+%5Cunderline%7Bx%5E5-x%5E4-x%5E3-x%5E2%5Cspace%5Cspace%5Cspace%5Cspace%5Cspace%5Cspace%5Cspace%5Cspace%5Cspace%5Cspace%5Cspace%5Cspace%5Cspace%7D%5C%5C+%5Cunderline%7B2x%5E4%2B2x%5E3%2Bx%5E2%5Cspace%5Cspace%5Cspace%5Cspace%5Cspace%5Cspace%5Cspace%5Cspace%5Cspace%5Cspace%5Cspace%5C%5C+2x%5E4-2x%5E3-2x%5E2-2x%7D%5C%5C+4x%5E3%2B3x%5E2%2B2x+%5Cend%7Balign%7D%5C%5C)

每做一次减法就相当于做一次替换。又因为 ![x^4-x^3-x^2-x](https://www.zhihu.com/equation?tex=x%5E4-x%5E3-x%5E2-x) 本身为0所以乘上 ![x](https://www.zhihu.com/equation?tex=x) 或者常数仍然成立，所以商的每一位相当于对递推式做了变换。

