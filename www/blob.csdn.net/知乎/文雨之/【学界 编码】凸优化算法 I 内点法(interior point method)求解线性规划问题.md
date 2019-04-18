# 【学界/编码】凸优化算法 I: 内点法(interior point method)求解线性规划问题 - 知乎
# 

> 作者： [@李恩志](https://www.zhihu.com/people/f7c07b0802f86160f31ca46180a75ddd)   Louisiana State University  计算物理学博士。

[『运筹OR帷幄』](https://zhuanlan.zhihu.com/operations-research)责任编辑： [@文雨之](https://www.zhihu.com/people/186282e45de0e1040b7882c9be8f5de8)（东北大学系统工程博士生）

本篇文章是由以上作者在知乎上的优秀文章（原文链接:[凸优化算法 I: 内点法(interior point method)求解线性规划问题](https://zhuanlan.zhihu.com/p/32685234)），通过[『运筹OR帷幄』](https://zhuanlan.zhihu.com/operations-research)责任编辑整理修改而成的。
*欢迎原链接转发，付费转载请前往*[@留德华叫兽](https://www.zhihu.com/people/961e8cc4f7512fda1ea6626ce9a05e8e)*的主页获取信息，盗版必究。*
敬请关注和扩散本专栏及同名公众号，会邀请**全球知名学者**陆续发布运筹学、人工智能中优化理论等相关干货、[知乎Live](https://www.zhihu.com/lives/users/961e8cc4f7512fda1ea6626ce9a05e8e)及行业动态：
[『运筹OR帷幄』大数据人工智能时代的运筹学](https://zhuanlan.zhihu.com/operations-research)

## **0前言**

内点法是一种处理带约束优化问题的方法，其在线性规划，二次规划，非线性规划等问题上都有着很好的表现。在线性规划的问题上，相对于鼎鼎大名的单纯形法，内点法是多项式算法，而单纯形法并非多项式算法。从实际应用的效果来说，内点法也达到了足以和单纯形法分庭抗衡的地步，尤其针对大规模的线性规划问题内点法有着更大的发展潜力。
- 线性规划单纯形法：通过一系列迭代达到最优解，迭代点沿着可行多面体的边界从一个顶点到另一个顶点，直到得到最优解。一般而言单纯形法每次迭代的开销相对内点法来说较小，但所需迭代次数较多。
- 线性规划内点法：同样是通过一系列迭代达到最优解，但其是从多面体内部逐渐收敛到最优解。一般而言内点法每次迭代的开销相对单纯形法来说较大，但所需迭代次数较少。

内点法并不仅仅用于线性规划的求解，值得一提的是内点法的很多思想有着更广泛的应用，例如障碍函数法的思想。

## **1导论**

线性规划问题的一般形式为

![\text{min } c^{T}x \\ \text{s.t. } Ax \preceq b](https://www.zhihu.com/equation?tex=%5Ctext%7Bmin+%7D+c%5E%7BT%7Dx+%5C%5C+%5Ctext%7Bs.t.+%7D+Ax+%5Cpreceq+b)

这里，目标函数为线性函数 ![c^{T}x: \mathbb{R}^{n} \rightarrow \mathbb{R}](https://www.zhihu.com/equation?tex=c%5E%7BT%7Dx%3A+%5Cmathbb%7BR%7D%5E%7Bn%7D+%5Crightarrow+%5Cmathbb%7BR%7D) ，约束条件为 ![A_{ij}x_{j} \le b_{i}, i = 1, 2, ..., m. ](https://www.zhihu.com/equation?tex=A_%7Bij%7Dx_%7Bj%7D+%5Cle+b_%7Bi%7D%2C+i+%3D+1%2C+2%2C+...%2C+m.+) 矩阵 ![A](https://www.zhihu.com/equation?tex=A) 为 ![m\times n](https://www.zhihu.com/equation?tex=m%5Ctimes+n) 的满秩矩阵，其中 ![m](https://www.zhihu.com/equation?tex=m) 为约束条件的个数， ![n](https://www.zhihu.com/equation?tex=n) 为变量的个数。通常约束条件的个数大于变量的个数，所以有 ![m > n](https://www.zhihu.com/equation?tex=m+%3E+n) .

## **2线性规划问题的等价(近似)表述**

这个线性规划问题可以重新表述为计算 ![\text{min } f(x)](https://www.zhihu.com/equation?tex=%5Ctext%7Bmin+%7D+f%28x%29) ，其中

![f(x) = c^{T}x + \sum_{i = 1}^{m}I(A_{ij}x_{j} - b_{i})](https://www.zhihu.com/equation?tex=f%28x%29+%3D+c%5E%7BT%7Dx+%2B+%5Csum_%7Bi+%3D+1%7D%5E%7Bm%7DI%28A_%7Bij%7Dx_%7Bj%7D+-+b_%7Bi%7D%29)

这里，我们使用了一个indicator函数，定义为

![\[ I(u) = \begin{cases} 0 & \text{if } x \le 0 \\ \infty & \text{if } x > 0 \end{cases} \]](https://www.zhihu.com/equation?tex=%5C%5B+I%28u%29+%3D+%5Cbegin%7Bcases%7D+0+%26+%5Ctext%7Bif+%7D+x+%5Cle+0+%5C%5C+%5Cinfty+%26+%5Ctext%7Bif+%7D+x+%3E+0+%5Cend%7Bcases%7D+%5C%5D)

引入这个函数的意义在于可以将约束条件直接写入到目标函数里面，这样我们直接求新的函数的极小值就可以了，而不必借助于未知乘子。 但是这里有一个问题，那就是indicator函数存在不可求导的点，因此在求函数极小值的时候我们没法通过普通的微分法来确定函数的极小值。为了规避这个问题，我们可以用一个光滑的函数来近似这个indicator函数。一个不错的选择是用 ![I_{t}(u) = -\frac{1}{t}\log (-u)](https://www.zhihu.com/equation?tex=I_%7Bt%7D%28u%29+%3D+-%5Cfrac%7B1%7D%7Bt%7D%5Clog+%28-u%29) 来代替indicator函数。 ![I_{t}(u)](https://www.zhihu.com/equation?tex=I_%7Bt%7D%28u%29) 只有在 ![u < 0](https://www.zhihu.com/equation?tex=u+%3C+0) 的时候有定义，我们规定当 ![u > 0](https://www.zhihu.com/equation?tex=u+%3E+0) 的时候 ![I_{t}(u) = \infty](https://www.zhihu.com/equation?tex=I_%7Bt%7D%28u%29+%3D+%5Cinfty) . 而且参数 ![t > 0](https://www.zhihu.com/equation?tex=t+%3E+0) 越大，函数 ![I_{t}(u)](https://www.zhihu.com/equation?tex=I_%7Bt%7D%28u%29) 就越接近于 ![I(u)](https://www.zhihu.com/equation?tex=I%28u%29) . 所以我们可以通过调节 ![t](https://www.zhihu.com/equation?tex=t) 的值来调节这个函数的近似程度。使用这个近似的indicator函数，我们新的的目标函数可以写作

![f(x) =t c^{T}x - \sum_{i = 1}^{m} \log(-A_{ij} x_j + b_i)](https://www.zhihu.com/equation?tex=f%28x%29+%3Dt+c%5E%7BT%7Dx+-+%5Csum_%7Bi+%3D+1%7D%5E%7Bm%7D+%5Clog%28-A_%7Bij%7D+x_j+%2B+b_i%29) .

因为线性函数是凸函数，并且 ![I_{t}(u)](https://www.zhihu.com/equation?tex=I_%7Bt%7D%28u%29) 也是凸函数，所以 ![f(x)](https://www.zhihu.com/equation?tex=f%28x%29) 是凸函数，因此我们可以很容易用凸优化的经典方法得到该函数的极小值。

## **3计算函数的梯度和Hessian矩阵**

为了求函数的极小值，根据微积分的经典结果，我们只需令函数的梯度等于零，然后计算梯度为零时对应的解 ![x^{\star}](https://www.zhihu.com/equation?tex=x%5E%7B%5Cstar%7D) .

函数的梯度为

![\frac{\partial f}{\partial x_{k}} = t c_{k} + \sum_{i = 1}^{m}\frac{-A_{ik}}{A_{ij}x_{j} - b_{i}}](https://www.zhihu.com/equation?tex=%5Cfrac%7B%5Cpartial+f%7D%7B%5Cpartial+x_%7Bk%7D%7D+%3D+t+c_%7Bk%7D+%2B+%5Csum_%7Bi+%3D+1%7D%5E%7Bm%7D%5Cfrac%7B-A_%7Bik%7D%7D%7BA_%7Bij%7Dx_%7Bj%7D+-+b_%7Bi%7D%7D)

Hessian 矩阵为

![\frac{\partial^2 f}{\partial x_{k}\partial x_{l}} = \sum_{i = 1}^{m}\frac{A_{ik} A_{il}}{\Big(A_{ij} x_{j} - b_{i} \Big)^2}](https://www.zhihu.com/equation?tex=%5Cfrac%7B%5Cpartial%5E2+f%7D%7B%5Cpartial+x_%7Bk%7D%5Cpartial+x_%7Bl%7D%7D+%3D+%5Csum_%7Bi+%3D+1%7D%5E%7Bm%7D%5Cfrac%7BA_%7Bik%7D+A_%7Bil%7D%7D%7B%5CBig%28A_%7Bij%7D+x_%7Bj%7D+-+b_%7Bi%7D+%5CBig%29%5E2%7D)

定义对角型矩阵为

![D_{ij} = \delta_{ij} \frac{1}{\Big(A_{ik}x_{k} - b_{i}\Big)^2}](https://www.zhihu.com/equation?tex=D_%7Bij%7D+%3D+%5Cdelta_%7Bij%7D+%5Cfrac%7B1%7D%7B%5CBig%28A_%7Bik%7Dx_%7Bk%7D+-+b_%7Bi%7D%5CBig%29%5E2%7D)

于是Hessian矩阵可以写作

![H_{f} = A^{T} D A](https://www.zhihu.com/equation?tex=H_%7Bf%7D+%3D+A%5E%7BT%7D+D+A)

因为 ![D](https://www.zhihu.com/equation?tex=D) 为正定矩阵，所以Hessian矩阵至少为半正定矩阵。所以函数 ![f(x)](https://www.zhihu.com/equation?tex=f%28x%29) 是一个凸函数。而且矩阵 ![D](https://www.zhihu.com/equation?tex=D) 为可逆矩阵，矩阵 ![A](https://www.zhihu.com/equation?tex=A) 满秩，所以Hessian矩阵为可逆矩阵。于是函数 ![f(x)](https://www.zhihu.com/equation?tex=f%28x%29) 为强凸函数。所以，要计算 ![\nabla f = 0](https://www.zhihu.com/equation?tex=%5Cnabla+f+%3D+0) 的根，我们可以用高效的牛顿迭代法。

## **4牛顿迭代法**

我们现在的目标是计算 ![\nabla f = 0](https://www.zhihu.com/equation?tex=%5Cnabla+f+%3D+0) 的根。因为Hessian矩阵可逆，所以我们可以用牛顿迭代法求解。牛顿迭代法为

![x^{(n+1)} = x^{(n)} - H^{-1}\nabla f](https://www.zhihu.com/equation?tex=x%5E%7B%28n%2B1%29%7D+%3D+x%5E%7B%28n%29%7D+-+H%5E%7B-1%7D%5Cnabla+f)

在这个迭代过程中，参数 ![t](https://www.zhihu.com/equation?tex=t) 为固定的。每对应一个 ![t](https://www.zhihu.com/equation?tex=t) ，我们都可以得到一个解 ![x_{t}^{\star}](https://www.zhihu.com/equation?tex=x_%7Bt%7D%5E%7B%5Cstar%7D) . 如果我们扫描参数 ![t](https://www.zhihu.com/equation?tex=t) ，我们就可以得到一系列的解。 其中最大的 ![t](https://www.zhihu.com/equation?tex=t) 的对应的解应该最精确。

一个更好的算法是选取一个比较小的初始参数 ![t_{0}](https://www.zhihu.com/equation?tex=t_%7B0%7D) ，求出这个参数对应的解 ![x_{t_0}^{\star}](https://www.zhihu.com/equation?tex=x_%7Bt_0%7D%5E%7B%5Cstar%7D) . 然后增加 ![t](https://www.zhihu.com/equation?tex=t) ，用之前得到的解 ![x_{t_0}^{\star}](https://www.zhihu.com/equation?tex=x_%7Bt_0%7D%5E%7B%5Cstar%7D) 来初始化当前 ![t](https://www.zhihu.com/equation?tex=t) 所对应的牛顿迭代法的试解。这样算出来的解应该比直接计算 ![t](https://www.zhihu.com/equation?tex=t) 所对应的解更加精确。这样逐步迭代从小 ![t_{0}](https://www.zhihu.com/equation?tex=t_%7B0%7D) 到大 ![t](https://www.zhihu.com/equation?tex=t) 可以得到一系列的解，最后得到的解 ![x_{t}^{\star}](https://www.zhihu.com/equation?tex=x_%7Bt%7D%5E%7B%5Cstar%7D) 称作是淬火解。这个解应该可以满足我们的精度需求。

## **5一个例子**

为了展示该算法的功效，我们举一个简单的例子。这个例子可以用简单的几何方法求解出来。我们要展示的是，我们可以用数值方法得到同样的结果（在一定的精度范围之内）。之所以要用数值方法求解这个简单的问题，是因为数值方法对更复杂的问题同样有效，而简单的几何方法对复杂问题却已经不适用了。

现在要研究的例子为

![\text{min } x + y \\ \text{s.t. }\\ x + 2y \le 1 \\ 2x + y \le 1 \\ x \ge 0 \\ y \ge 0](https://www.zhihu.com/equation?tex=%5Ctext%7Bmin+%7D+x+%2B+y+%5C%5C+%5Ctext%7Bs.t.+%7D%5C%5C+x+%2B+2y+%5Cle+1+%5C%5C+2x+%2B+y+%5Cle+1+%5C%5C+x+%5Cge+0+%5C%5C+y+%5Cge+0)

也就是

![c = \begin{pmatrix} 1 \\ 1 \end{pmatrix}](https://www.zhihu.com/equation?tex=c+%3D+%5Cbegin%7Bpmatrix%7D+1+%5C%5C+1+%5Cend%7Bpmatrix%7D) , ![A = \begin{pmatrix} 1 & 2 \\ 2 & 1 \\ -1 & 0 \\ 0 & -1 \end{pmatrix}](https://www.zhihu.com/equation?tex=A+%3D+%5Cbegin%7Bpmatrix%7D+1+%26+2+%5C%5C+2+%26+1+%5C%5C+-1+%26+0+%5C%5C+0+%26+-1+%5Cend%7Bpmatrix%7D) , ![b = \begin{pmatrix} 1 \\ 1 \\ 0 \\ 0 \end{pmatrix}](https://www.zhihu.com/equation?tex=b+%3D+%5Cbegin%7Bpmatrix%7D+1+%5C%5C+1+%5C%5C+0+%5C%5C+0+%5Cend%7Bpmatrix%7D) .

梯度为

![\nabla f(x) = tc - A^{T} \begin{pmatrix} \frac{1}{x + 2y -1}\\ \frac{1}{2x + y -1} \\ \frac{1}{-x} \\ \frac{1}{-y} \end{pmatrix}](https://www.zhihu.com/equation?tex=%5Cnabla+f%28x%29+%3D+tc+-+A%5E%7BT%7D+%5Cbegin%7Bpmatrix%7D+%5Cfrac%7B1%7D%7Bx+%2B+2y+-1%7D%5C%5C+%5Cfrac%7B1%7D%7B2x+%2B+y+-1%7D+%5C%5C+%5Cfrac%7B1%7D%7B-x%7D+%5C%5C+%5Cfrac%7B1%7D%7B-y%7D+%5Cend%7Bpmatrix%7D)

Hessian矩阵为

![H_{f}(x) = A^{T}DA](https://www.zhihu.com/equation?tex=H_%7Bf%7D%28x%29+%3D+A%5E%7BT%7DDA)

其中，对角型矩阵 ![D](https://www.zhihu.com/equation?tex=D) 为

![D = \begin{pmatrix} \frac{1}{(x + 2y - 1)^2 } & 0 & 0 & 0 \\ 0 & \frac{1}{(2x + y - 1)^2 } & 0 & 0 \\ 0& 0 & \frac{1}{x^2} & 0 \\ 0 & 0 & 0 & \frac{1}{y^2} \end{pmatrix}](https://www.zhihu.com/equation?tex=D+%3D+%5Cbegin%7Bpmatrix%7D+%5Cfrac%7B1%7D%7B%28x+%2B+2y+-+1%29%5E2+%7D+%26+0+%26+0+%26+0+%5C%5C+0+%26+%5Cfrac%7B1%7D%7B%282x+%2B+y+-+1%29%5E2+%7D+%26+0+%26+0+%5C%5C+0%26+0+%26+%5Cfrac%7B1%7D%7Bx%5E2%7D+%26+0+%5C%5C+0+%26+0+%26+0+%26+%5Cfrac%7B1%7D%7By%5E2%7D+%5Cend%7Bpmatrix%7D)

对于一个固定的参数 ![t](https://www.zhihu.com/equation?tex=t) ，选择一个恰当的初始解 ![x^{(0)}](https://www.zhihu.com/equation?tex=x%5E%7B%280%29%7D) ，代入牛顿迭代公式

![x^{(n+1)} = x^{(n)} - H_{f}(x^{(n)})^{-1}\nabla f(x^{(n)}), n \ge 0](https://www.zhihu.com/equation?tex=x%5E%7B%28n%2B1%29%7D+%3D+x%5E%7B%28n%29%7D+-+H_%7Bf%7D%28x%5E%7B%28n%29%7D%29%5E%7B-1%7D%5Cnabla+f%28x%5E%7B%28n%29%7D%29%2C+n+%5Cge+0)

可以得到一个依赖于参数 ![t](https://www.zhihu.com/equation?tex=t) 的解 ![x^{\star}_{t}](https://www.zhihu.com/equation?tex=x%5E%7B%5Cstar%7D_%7Bt%7D) .

用几何方法很容易求得这个例子的解为 ![(x^{\star}, y^{\star} ) = (0, 0)](https://www.zhihu.com/equation?tex=%28x%5E%7B%5Cstar%7D%2C+y%5E%7B%5Cstar%7D+%29+%3D+%280%2C+0%29) . 所以我们期待，当 ![\lim_{t \rightarrow \infty} (x_{t}^{\star}, y_{t}^{\star}) = (0, 0)](https://www.zhihu.com/equation?tex=%5Clim_%7Bt+%5Crightarrow+%5Cinfty%7D+%28x_%7Bt%7D%5E%7B%5Cstar%7D%2C+y_%7Bt%7D%5E%7B%5Cstar%7D%29+%3D+%280%2C+0%29) .

我需要用程序求这个例子的数值解。我写了一个Python程序实现这个算法，程序地址为[PrimerLi/linear-programming](https://link.zhihu.com/?target=https%3A//github.com/PrimerLi/linear-programming).

程序结果如下：
![](https://pic1.zhimg.com/v2-e195ecd42390c3cfe31a43f97c1a0f50_b.jpg)
这里用了四个不同的初始点来初始化程序，最终得到解都收敛到了 ![(0, 0)](https://www.zhihu.com/equation?tex=%280%2C+0%29) . 箭头方向为参数 ![t](https://www.zhihu.com/equation?tex=t) 增加时 ![(x_t^{\star}, y_t^{\star} )](https://www.zhihu.com/equation?tex=%28x_t%5E%7B%5Cstar%7D%2C+y_t%5E%7B%5Cstar%7D+%29) 移动的方向。这正是我们期待的结果。

在此感谢[『运筹OR帷幄』](https://zhuanlan.zhihu.com/operations-research)审稿人对本文提出了宝贵的意见。

[『运筹OR帷幄』](https://zhuanlan.zhihu.com/operations-research)审稿人 [@覃含章](https://www.zhihu.com/people/866e63341ae3873b7a4ce0390767dc74)，美国麻省理工学院（MIT）计算科学与工程方向博士在读，清华大学工业工程及数学与应用数学（第二学位）本科。研究兴趣主要为优化理论，机器学习算法在运营管理中的应用。

以上[『运筹OR帷幄』](https://zhuanlan.zhihu.com/operations-research)专栏所有文章都会同步发送至 [留德华叫兽的头条主页](https://link.zhihu.com/?target=https%3A//www.toutiao.com/c/user/62215957061/%23mid%3D1570625219325954)， 以及同名微信公众号，目前预计受众**10w +**

如果你是运筹学/人工智能硕博或在读，请在下图的公众号后台留言：**“加微信群”**。系统会自动辨认你的关键字，并提示您进一步的加群要求和步骤，邀请您进全球运筹或AI学者群（群内学界、业界大佬云集）。

运筹学|控制论|优化理论爱好者，欢迎加qq群：686387574

人工智能爱好者，欢迎加qq群： 685839321

数据科学|分析爱好者，欢迎加qq群：130414574

最后敬请**关注和扩散**本专栏及同名公众号，会陆续发布运筹学、人工智能中优化理论相关干货及行业动态：[『运筹OR帷幄』](https://zhuanlan.zhihu.com/operations-research)[大数据和人工智能时代下的运筹学 - 知乎专栏](https://zhuanlan.zhihu.com/operations-research)
![](https://pic3.zhimg.com/v2-487cabb2fb70b18d0a8aebbe14ce359a_b.jpg)

