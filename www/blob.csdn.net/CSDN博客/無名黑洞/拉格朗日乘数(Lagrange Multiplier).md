# 拉格朗日乘数(Lagrange Multiplier) - 無名黑洞 - CSDN博客
2012年04月09日 18:23:42[wangxiaojun911](https://me.csdn.net/wangxiaojun911)阅读数：6668
                
拉格朗日乘数研究函數在一些限制Constraint下的極限條件。如果一個函數有n個未知數和k個限制，要求其最優解，拉格朗日引入了k個新的變量與函數並立，表面上函數變得更加複雜，其實是簡化問題的方法。
所謂最優化問題就是某個函數的極限狀態。比如求一個斜坡上皮球勢能函數的最大處，未知數是皮球的高度，斜坡就是限制條件，皮球只能沿著斜坡表面運動。抽象來說，要進行最優化的函數為
e(x1, x2, ..., xn)
是n個未知數的函數。限制條件記為：
f(x1, x2, ..., xn)=0
引入拉格朗日乘数lambda，把兩式子連立起來得到：
g=e(x1, x2, ..., xn) + lambda f(x1, x2, ... ,xn) = g(x1, x2, ..., xn, lambda1, lambda2, ...lambdak )
這樣，求解函數在某個限制條件下的最優解X等價於求解上述方程g的極值。下面來看一個具體的例子。
**旋轉向量組的最佳擬合**
已知一系列向量Xn經過某種變換或成為了Yn，現在，要用一個矩陣U來表示這個變換，應當如何選取U。由於Xn的向量是任意變換，有可能剛好把Xn對應到Yn的矩陣根本就沒有，所以需要找一個比較近似的最佳擬合。所謂最佳，就是其residual能量的值最小。
設向量維度為m，則Xn, Yn是m乘1矩陣， U是一個n乘m矩陣。 限制條件是U滿足旋轉矩陣的屬性。
![](https://img-my.csdn.net/uploads/201204/09/1333967179_2420.jpg)
首先分析以上方程組。（1）的未知數是U，一共有n*m個未知數。E也叫能量函數，是一個標量。（2）F的是一個n*m矩陣。所以(3)中的labda必須是一個算子，它引入了n*m個拉格朗日乘数, 把F裡所有元素與一個乘數相乘並求和。對(3)式子求導並令它為零就可以得到需要的U值。
![](https://img-my.csdn.net/uploads/201204/09/1333967190_8486.jpg)
![](https://img-my.csdn.net/uploads/201204/09/1333967198_8031.jpg)
假設Ｕ為旋轉矩陣的情況下，直接求解上述方程不太容易（含大量未知數)，但是可以通過分解R=UP(極分解)來巧妙求解。
![](https://img-my.csdn.net/uploads/201204/09/1333967206_5054.jpg)
但是，假如P含有為零的特征值（比如向量共面的情況），無法求逆的到正確的函數，采取如下方法取而代之：
![](https://img-my.csdn.net/uploads/201204/09/1333967212_1789.jpg)
**參考資料**
http://en.wikipedia.org/wiki/Lagrange_multiplier
WOLFGANG KABSCH, Ma x - P l a n c k - l n s t i t u t f i i r Medizinische Forschun
**逸聞**
Joseph-Louis Lagrange 是十八世紀法國數學家和天文學家，他在數學方面有很多貢獻。與同時期的著名數學家拉普拉斯Laplace、勒让德Legendre並稱數學界三L。
