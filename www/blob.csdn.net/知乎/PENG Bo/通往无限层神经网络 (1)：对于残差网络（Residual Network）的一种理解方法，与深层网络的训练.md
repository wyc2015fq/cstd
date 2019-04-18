# 通往无限层神经网络 (1)：对于残差网络（Residual Network）的一种理解方法，与深层网络的训练 - 知乎
# 


目前我们在训练极深层网络时，最有效的方法是使用残差网络（Residual Network）结构。关于残差网络，一种常见的理解方法是认为它等价于多个浅层网络的 ensemble（见 1605.06431），但这不足以解释几个现象，例如：- **ReLU 和卷积的排列顺序，为什么按照 1603.05027 的方法比较好？**
- **为什么要旁路掉 2 层网络才能得到不错的结果？如果只旁路掉 1 层，为什么效果并不好？**


最近我在考虑其它问题时发现，**其实可以用一个极其简单的模型探讨残差结构的有效性**。不知道之前有没有人说过。**如果您有什么想法，欢迎讨论。**

我们都知道，用 2 层网络，配合足够多的神经元，就可以拟合任何函数。那么考虑这个问题：**如果我们在每层只用 1 个神经元，但是用足够深的网络，以及合理的网络架构，是否也可以拟合任何函数？答案也是肯定的，而且需要用残差结构才能得到较好的结果**（需要指出，理论能拟合，不代表实际能拟合。实际的拟合情况是这里的关键）。


不妨看个具体例子，因为一般情况的证明稍后会看到是比较明显的。只需看一个最简单的回归问题，使用 MSE 损失。我们的目标是在 (-3, 3) 区间拟合 sin(x)：
![](https://pic2.zhimg.com/v2-226185ddb0be2a5d2d44cb5cbb721219_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='360' height='223'></svg>)
## 方法 1 ：

如果用直接的方法：
![input => Max(0, w_1x+b_1) => Max(0, w_2x+b_2) => ... => w_nx+b_n => output](https://www.zhihu.com/equation?tex=input+%3D%3E+Max%280%2C+w_1x%2Bb_1%29+%3D%3E+Max%280%2C+w_2x%2Bb_2%29+%3D%3E+...+%3D%3E+w_nx%2Bb_n+%3D%3E+output)

我们会发现多加几层后就根本无法训练，全部清零。原因很简单，ReLU 会不断丢失信息。当 ReLU 给出 0 之后，后面就全死了。如果网络的层数少，还可以得到这样的结果：

![](https://pic4.zhimg.com/v2-15c57849aa64d5bd4948109bc6b3f77b_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='360' height='223'></svg>)如果层数一多，就肯定全部清零。

## 方法 2 ：

那么加上残差结构吧，比如旁路掉 1 层：
![input => x + Max(0, w_1x+b_1) => x + Max(0, w_2x+b_2) => .... => x + Max(0, w_nx+b_n) => output](https://www.zhihu.com/equation?tex=input+%3D%3E+x+%2B+Max%280%2C+w_1x%2Bb_1%29+%3D%3E+x+%2B+Max%280%2C+w_2x%2Bb_2%29+%3D%3E+....+%3D%3E+x+%2B+Max%280%2C+w_nx%2Bb_n%29+%3D%3E+output)

这个肯定也不行。因为 x 只会变大，我们会得到类似这样的结果：
![](https://pic3.zhimg.com/v2-cfc5ee616e706f882e6abe37d1e2e53e_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='360' height='223'></svg>)
可以用一些办法改善，比如把最后一层的残差和 ReLU 都去掉，但这无疑不是好的办法。


## 方法 3 ：

最简单的解决方法就是旁路掉 2 层。因为如果熟悉 ReLU 就会知道，ReLU 是可以给出负值的，只需要 2 层即可。例如：-Max(0, -x) = Min(0, x)。


于是，最简单的满足我们要求的 2 层结构，就是 Convolution => ReLU => Convolution（这其实就是 1610.02915 PyramidNet 的结构了）：
![input => x + w_{1b} \cdot Max(0, w_{1a}x+b_{1a}) + b_{1b} => .... => x + w_{nb} \cdot Max(0, w_{na}x+b_{na}) + b_{nb} => output](https://www.zhihu.com/equation?tex=input+%3D%3E+x+%2B+w_%7B1b%7D+%5Ccdot+Max%280%2C+w_%7B1a%7Dx%2Bb_%7B1a%7D%29+%2B+b_%7B1b%7D+%3D%3E+....+%3D%3E+x+%2B+w_%7Bnb%7D+%5Ccdot+Max%280%2C+w_%7Bna%7Dx%2Bb_%7Bna%7D%29+%2B+b_%7Bnb%7D+%3D%3E+output)

可以得到类似这样的结果：

![](https://pic3.zhimg.com/v2-c28b107c23fbc51fe5aa0f42c83172d6_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='360' height='223'></svg>)易证这种方法可以拟合任何函数，**因为它其实就是在不断"折"一条线**。不过，如果层数很多，仍然可能死掉，拟合效果卡住。

## 方法 4 ：
把 ReLU 再换成 leaky 的，例如 PReLU。这就基本解决了死亡问题，可以保证多层训练的效果，例如：
![](https://pic3.zhimg.com/v2-3c4870a5dd61d32880bf847f586896aa_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='360' height='223'></svg>)最后，如果我们把方法 1 中的 ReLU 换成 PReLU，是否可以解决死亡问题？遗憾的是不行。这里涉及到数值稳定性。最好的方法仍然是方法 4 的旁路2层残差+PReLU。


## 总结：

下一步自然的问题是做数值稳定性的分析。未来写写吧。深度网络的奥秘在于深度，因此在后文也会继续探讨训练这种极窄极深的网络的办法。

此外，在数学上容易看到残差结构的必要性：假设网络的层数很多，那么每一层就最好接近于 identity map，否则本征值很容易发散或消亡（严格说，不一定如此，因为还可以用更精细的方法控制本征值，不过这也是后话了）。

其实我真正在想的问题是如何构造和训练无限深的网络。因此，本系列的下一篇是：[通往无限层神经网络：一个小实验 - 知乎专栏](https://zhuanlan.zhihu.com/p/26613796)

如果本文对你有启发，请点个赞，谢谢！~ 如果您有什么想法，也很欢迎讨论。


