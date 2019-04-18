# Laplace算子和Laplacian矩陣 - 無名黑洞 - CSDN博客
2012年04月02日 07:03:42[wangxiaojun911](https://me.csdn.net/wangxiaojun911)阅读数：14993
**1 Laplace算子的物理意義**
Laplace算子的定義為梯度的散度。
![](https://img-my.csdn.net/uploads/201204/02/1333321353_8208.jpg)
在Cartesian坐標系下也可表示為：
![](https://img-my.csdn.net/uploads/201204/02/1333321356_1477.jpg)
或者，它是Hessian矩陣的跡：
![](https://img-my.csdn.net/uploads/201204/02/1333321359_2597.jpg)
以熱傳導方程為例，因為熱流與溫度的梯度成正比，那麼溫度的梯度的散度就是熱量的損失率。
![](https://img-my.csdn.net/uploads/201204/02/1333321362_4434.jpg)
由此可見，Laplace算子可用於表現由於物質分佈不均引起的物質輸送。
**2 Laplace算子的數學意義**
現在，在一維空間中簡單分析上面的式子：
![](https://img-my.csdn.net/uploads/201204/02/1333321414_3408.jpg)
也可以寫作：
![](https://img-my.csdn.net/uploads/201204/02/1333321419_6740.jpg)
把分子第一項和第二項分別按泰勒展開：
![](https://img-my.csdn.net/uploads/201204/02/1333321423_2227.jpg)
可以看出Laplace算子實際上是一個使函數取平均的算子。多維空間相似。
**3 Laplace方程**
若Laplace算子右邊為零，稱為Laplace方程。Laplace方程的解稱為調和函數。若右邊是一個函數，稱為泊松方程。
**4 Laplace算子在圖像處理的運用**
圖像處理是以像素作為基礎離散化，如下：
![](https://img-my.csdn.net/uploads/201204/02/1333321506_1570.jpg)
**5 Laplacian 矩陣**
是一種用于表示圖的矩陣。 它的維度是 |V|-by-|V| ( |V| 是節點的數目 )。 James Demmel提供了一種由Incidence matrix轉化為Laplacian矩陣的方法。
In(G)是一個 |V|-by-|E| 矩陣( |E| 是邊的數目 ), 設邊e=(i,j)，這一列除了第i行（為+1）和第j行(為-1)外都為零。 需要说明的是，根据这个定义，对于无向图 e=(i,j) 和 e=(j,i) 是等价的， 看似会生成很多不同的In图（根据每条边不同的取向）。但是实际上可以证明，无论边的方向怎么取，由In图生成的L图都是唯一的。 也就是说， e=(i,j) 和 e=(j,i) 怎么取是无关紧要的。 如何使用In图生成L图：
![](https://img-my.csdn.net/uploads/201204/02/1333321544_8174.jpg)
![](https://img-my.csdn.net/uploads/201204/02/1333321561_4773.jpg)
可得知Laplacian矩陣的两个重要性质：一是为对称阵。二是存在一个为零的特征值（秩为|V|-1）。三是一個半正定矩陣。 注意Laplace算子是负定的。
在求解含Laplacian矩陣的方程組時，常常要求為正定矩陣。觀察發現這是因為Laplacian矩陣每列相加等于零。這時只需要手動更改第一行和第一列（比如第一個元素設為1，其余設為零），破壞其結構，令秩等于|V|就可以了。
对于非正定矩阵，左乘个transpose of the matrix, 推导如下： 
Ax – b = 0 
最小化 ||Ax – b||^2，展開后對x求導數：
![](https://img-my.csdn.net/uploads/201204/02/1333321587_8579.jpg)
可轉化為正定方程組。
**6 Laplace算子和Laplacian矩陣的關系**
Laplace算子可以推廣到多維情況計算。Laplacian矩陣主要用于三維以下的圖形學計算，可以表現復雜的幾何結構。而Lapace方程使用了Laplace算子來表示Laplacian矩陣。
**參考文獻：**
丘成桐，幾何三十載，香港中文大學
James Demmel, related lectures, University of California, Berkeley
