# Gauss–Seidel方法和Jacobi 方法 - 無名黑洞 - CSDN博客
2011年10月20日 12:57:14[wangxiaojun911](https://me.csdn.net/wangxiaojun911)阅读数：10014
**Gauss–Seidelmethod**
对应于形如Ax = b的方程（A为对称正定矩阵或者Diagonally dominant），可求解如下：
![](http://hi.csdn.net/attachment/201110/20/0_1319086707BFb0.gif)
**Jacobi method**
另一种方法是Jacobimethod，它与Gauss–Seidelmethod类相似，但是要求A必须是Diagonally dominant。把A分解成D+U+L，仅求D的逆矩阵。
Dx = b – Ux - Lx
![](http://hi.csdn.net/attachment/201110/20/0_1319086695xGBG.gif)
一般认为，Gauss–Seidel方法更容易Converge。
无论Gauss–Seidel方法或Jacobi方法都涉及矩阵求逆。在Jacobi中对对角阵求逆十分简单。Gauss–Seidel中是三角阵，也有相应的方法。以下是加州大学Fullerton分校数学系提供的一种方法：
![](http://hi.csdn.net/attachment/201110/20/0_13190867401zL4.gif)
![](http://hi.csdn.net/attachment/201110/20/0_1319086747mH7c.gif)
**参考文献**
[http://en.wikipedia.org/wiki/Gauss%E2%80%93Seidel_method](http://en.wikipedia.org/wiki/Gauss%E2%80%93Seidel_method)
[http://en.wikipedia.org/wiki/Jacobi_method](http://en.wikipedia.org/wiki/Jacobi_method)
[http://math.fullerton.edu/mathews/n2003/gaussseidelmod.html](http://math.fullerton.edu/mathews/n2003/gaussseidelmod.html)

