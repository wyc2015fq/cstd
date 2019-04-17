# coursera-斯坦福-机器学习-吴恩达-第5周笔记-反向传播 - 博客堂 - CSDN博客





2017年12月03日 22:42:19[最小森林](https://me.csdn.net/u012052268)阅读数：5450
所属专栏：[斯坦福-吴恩达《机器学习》之路](https://blog.csdn.net/column/details/18191.html)









# coursera-斯坦福-机器学习-吴恩达-第5周笔记-反向传播



- [coursera-斯坦福-机器学习-吴恩达-第5周笔记-反向传播](#coursera-斯坦福-机器学习-吴恩达-第5周笔记-反向传播)- [1代价函数and反向传播](#1代价函数and反向传播)- [1代价函数](#11代价函数)
- [2误差反向传播](#12误差反向传播)
- [3直观感受反向传播](#13直观感受反向传播)

- [2神经网络的技巧](#2神经网络的技巧)- [1参数的展开](#21参数的展开)
- [2梯度检验 Gradient Checking](#22梯度检验-gradient-checking)
- [3随机初始化参数](#23随机初始化参数)
- [4总结神经网络](#24总结神经网络)

- [3神经网络的应用](#3神经网络的应用)
- [4测验quiz](#4测验quiz)
- [5编程题](#5编程题)





## 1代价函数and反向传播

### 1.1代价函数

首先定义一些我们需要使用的变量： 

- L =网络中的总层数 

- $s_l$ =第l层中的单位数量（不包括偏差单位） 

- K =输出单元/类的数量
首先，回想一下“逻辑回归”正则化的成本函数是：

$J(\theta) = - \frac{1}{m} \sum_{i=1}^m [ y^{(i)}\ \log (h_\theta (x^{(i)})) + (1 - y^{(i)})\ \log (1 - h_\theta(x^{(i)}))] + \frac{\lambda}{2m}\sum_{j=1}^n \theta_j^2$

再回想一下，在神经网络中，我们可能有许多输出节点。我们把 $h_\Theta(x)_k$表示为导致第k个输出的假设。我们的神经网络的成本函数将会是我们用于逻辑回归的一个综合泛化。神经网络的代价函数为：

$ J(\Theta) = - \frac{1}{m} \sum_{i=1}^m \sum_{k=1}^K \left[y^{(i)}_k \log ((h_\Theta (x^{(i)}))_k) + (1 - y^{(i)}_k)\log (1 - (h_\Theta(x^{(i)}))_k)\right] + \frac{\lambda}{2m}\sum_{l=1}^{L-1} \sum_{i=1}^{s_l} \sum_{j=1}^{s_{l+1}} ( \Theta_{j,i}^{(l)})^2$

这个式子看着复杂，但其实并不难理解：我们只是添加了几个嵌套的求和，来解释我们的多个输出节点。
- 在方程的第一部分，在方括号之前，我们有一个额外的嵌套总和，表示输出节点的数量。
- 在正则化部分，在方括号后面，我们必须考虑多个theta矩阵。当前theta矩阵中的列数等于当前图层中的节点数（包括偏置单元）。在我们当前theta矩阵中的行数等于下一层中的节点数（不包括偏置单元）。与之前的逻辑回归一样，我们对每一项进行平方。

### 1.2误差反向传播

就像我们在逻辑回归和线性回归中使用梯度下降所做的一样。 

我们的目标是：$\min_\Theta J(\Theta)$

所以我们希望使用theta中的一组最优参数来最小化我们的成本函数J。寻找最小的参数，需要使用梯度下降法；而梯度下降法最重要的是计算梯度$\dfrac{\partial}{\partial \Theta_{i,j}^{(l)}}J(\Theta)$

为了计算这个偏导数，我们使用反向传播算法： 
![](http://oqy7bjehk.bkt.clouddn.com/17-12-3/87838602.jpg)
我们现在来讲一下反向传播算法，它是**计算偏导数的**一种有效方法。
- 设$a^{(1)} := x^{(t)}$
- 
执行前向传播以计算$a^{(l)}$ for l=2,3,…,L 
![](http://oqy7bjehk.bkt.clouddn.com/17-12-3/23499165.jpg)- 
使用$y^{(t)}$计算最后一层的误差：$\delta^{(L)} = a^{(L)} - y^{(t)}$


其中L是我们的总层数，a（L）是最后一层激活单元的输出向量。 

所以我们最后一层的“误差值”就是我们最后一层的实际结果和y中的正确输出的差别。 

为了得到最后一层之前的图层的增量值，我们可以使用一个从右到左的方程式：- 计算之前层次的误差：$\delta^{(L-1)}$ , $\delta^{(L-2)}$ ,… ,$\delta^{(2)}$

使用公式：$\delta^{(l)} = ((\Theta^{(l)})^T \delta^{(l+1)})\ .*\ a^{(l)}\ .*\ (1 - a^{(l)})$

其中，我们用一个称为g’的函数或者g-prime来元素乘法，g是用z（l）给出的输入值评估的激活函数g的导数：$g'(z^{(l)}) = a^{(l)}\ .*\ (1 - a^{(l)})$
- 进一步，把每个样本的误差合计$\Delta^{(l)}_{i,j} := \Delta^{(l)}_{i,j} + a_j^{(l)} \delta_i^{(l+1)}$

然后计算正则化: 

- $D^{(l)}_{i,j} := \dfrac{1}{m}\left(\Delta^{(l)}_{i,j} + \lambda\Theta^{(l)}_{i,j}\right)$

- $D^{(l)}_{i,j} := \dfrac{1}{m}\Delta^{(l)}_{i,j}$
误差-增量 矩阵D被用作“累加器”来累加我们的误差，并最终计算得到我们所需要的偏导数：$\frac \partial {\partial \Theta_{ij}^{(l)}} J(\Theta)=D_{ij}^{(l)}$

### 1.3直观感受反向传播

吴老师生怕我们不能“理解感受”反向传播，所以在这一节，他讲了一种反向传播的直观理解方式。

首先回忆神经网络的代价函数是：

$J(\Theta) = - \frac{1}{m} \sum_{t=1}^m\sum_{k=1}^K \left[ y^{(t)}_k \ \log (h_\Theta (x^{(t)}))_k + (1 - y^{(t)}_k)\ \log (1 - h_\Theta(x^{(t)})_k)\right] + \frac{\lambda}{2m}\sum_{l=1}^{L-1} \sum_{i=1}^{s_l} \sum_{j=1}^{s_l+1} ( \Theta_{j,i}^{(l)})^2$

如果我们考虑简单的非多类分类（k = 1）并且忽视正则化，则成本计算如下：

$cost(t) =y^{(t)} \ \log (h_\Theta (x^{(t)})) + (1 - y^{(t)})\ \log (1 - h_\Theta(x^{(t)}))$

那么直观上，$\delta_j^{(l)}$是$a^{(l)}_j$（第1层中的单元j）的“误差”。 

更正式地说，δ值实际上是成本函数的导数，即： 
$\delta_j^{(l)} = \dfrac{\partial}{\partial z_j^{(l)}} cost(t)$
![](http://oqy7bjehk.bkt.clouddn.com/17-12-1/63003101.jpg)

每一个神经元的误差都与后面连接的神经元有关。比如： 
$\delta_2^{(2)}=\Theta_{12}^{(2)}*\delta_1^{(3)}+\Theta_{22}^{(2)}*\delta_2^{(3)}$

再比如：$\delta_2^{(3)}=\Theta_{12}^{(3)}*\delta_1^{(4)}$

tips：这是一种表达方法，并不意味着使用这么简单的方法就能求出δ。这只是一种解释，直观的展示不同层次误差之间的关系。

> 
我对反向传播的一种形象的理解： 

- 由于参数选的不好，导致网络最后输出的结果与真实值相差甚远。我们很自然的求出了最后一层的误差（真实-输出）。
- 但是，最后一层的神经元不服气：明明数值都是前面的神经元传递过来的，出了误差凭什么都赖我啊？我要兴师问罪！
- 于是，最后一层的神经元就去找前面一层的神经元问罪，问罪的证据就是参数Θ。谁的Θ大，就说明谁传过来的误差大，责任也就越多。靠这个方法，最后一层的神经元，把误差的责任按照比例“推卸”给了前一层。
- 前一层也按照这个方式，一层层往前“推诿责任”。这样，误差就反向传播给了每一层。

## 2神经网络的技巧

### 2.1参数的展开

使用神经网络时，我们处理的一组矩阵： 
$\Theta^{(1)},\Theta^{(2)}\Theta^{(3)}...$

为了使用诸如“fminunc（）”这样的优化函数，我们将要“展开”所有元素，并将它们放入一个长向量中，我们可以使用命令[]：
`thetaVector = [ Theta1(:); Theta2(:); Theta3(:); ]`
如果Theta1的尺寸是10x11，Theta2是10x11，Theta3是1x11，那么我们可以从“展开”版本中取回我们的原始矩阵，如下所示：

```
Theta1 = reshape(thetaVector(1:110),10,11)
Theta2 = reshape(thetaVector(111:220),10,11)
Theta3 = reshape(thetaVector(221:231),1,11)
```

![](http://oqy7bjehk.bkt.clouddn.com/17-12-3/98417041.jpg)

像这样的函数需要一个参数向量。
### 2.2梯度检验 Gradient Checking

渐变检查将确保我们的反向传播按预期工作。我们可以用下式近似我们的成本函数的导数：

$\dfrac{\partial}{\partial\Theta}J(\Theta) \approx \dfrac{J(\Theta + \epsilon) - J(\Theta - \epsilon)}{2\epsilon}$

使用多个theta矩阵，我们可以如下近似于关于Θj的导数： 
$\dfrac{\partial}{\partial\Theta_j}J(\Theta) \approx \dfrac{J(\Theta_1,\Theta_j + \epsilon, \Theta_n) - J(\Theta_1 , \Theta_j - \epsilon,  \Theta_n)}{2\epsilon}$

为保证数学运算正确ε（ε）的一个很小的值，比如ε=${\epsilon = 10^{-4}}$。 

但如果ε的值太小，我们最终会出现数值问题。

因此，我们只是在Θj矩阵中加上或减去ε。在octave我们可以做到这一点，如下所示：

```matlab
epsilon = 1e-4;
for i = 1:n,
  thetaPlus = theta;
  thetaPlus(i) += epsilon;
  thetaMinus = theta;
  thetaMinus(i) -= epsilon;
  gradApprox(i) = (J(thetaPlus) - J(thetaMinus))/(2*epsilon)
end;
```

记住一旦验证了反向传播算法是正确的，就不需要再次计算gradApprox。因为 

计算gradApprox的代码可能非常慢。

### 2.3随机初始化参数

将所有的权重初始化为0不适用于神经网络。那样反向传播时，所有节点将重复更新为相同的值。

相反，我们可以使用以下方法随机初始化我们的Θ矩阵的权重： 
![](http://oqy7bjehk.bkt.clouddn.com/17-12-3/93655022.jpg)

因此，我们将每个$\Theta^{(l)}_{ij}$初始化为[-ε，ε]之间的随机值。使用上面的公式保证我们得到所需的界限。相同的程序适用于所有的Θ。以下是您可以用来进行实验的一些工作代码。
```
Theta1 = rand(10,11) * (2 * INIT_EPSILON) - INIT_EPSILON;
Theta2 = rand(10,11) * (2 * INIT_EPSILON) - INIT_EPSILON;
Theta3 = rand(1,11) * (2 * INIT_EPSILON) - INIT_EPSILON;
```

rand（x，y）只是一个八度函数，它将初始化一个0到1之间的随机实数矩阵。

### 2.4总结神经网络
- 网络体系结构

首先，选择一个网络体系结构;选择你的神经网络的布局，包括每层中有多少个隐藏单元，以及你想要的总共有多少层。一些经验： 

- 输入单元的数量=特征的维度x（i） 

- 输出单元的数量=类的数量y 

- 每层隐藏单元的数量=通常越多越好（必须与计算成本平衡，因为随着更多隐藏单元的增加而增加） 

- 默认值：1个隐藏层。如果您有多个隐藏层，则建议您在每个隐藏层中具有相同数量的单元。- 模型的训练

模型的训练遵照以下步骤： 

1. 随机初始化权重 

2. 实现向前传播以获得任何x（i）的hΘ（x（i））， 

3. 实施成本函数 

4. 实施反向传播以计算偏导数 

    - 循环每个训练的例子for i = 1:m 

5. 使用梯度检查来确认您的反向传播的作品。然后禁用梯度检查。 

6. 使用梯度下降或内置的优化功能，以theta中的权重最小化成本函数。
下面的图像让我们直观地了解当我们实施我们的神经网络时发生了什么：

![](http://oqy7bjehk.bkt.clouddn.com/17-12-3/48392701.jpg)

理想情况下，你需要$h_\Theta(x^{(i)})$≈$y^{(i)}$。这将使我们的成本函数最小化。但是，请记住，J（Θ）不是凸的，因此我们可以最终找到局部最小值。

## 3神经网络的应用

吴老师举了一个自动驾驶的例子： 
![](http://oqy7bjehk.bkt.clouddn.com/17-12-3/80554627.jpg)
训练一个人驾驶汽车 然后让ALVINN观看 。 

ALVINN每两秒  将前方的路况图生成一张数字化图片 并且记录驾驶者的驾驶方向 。 

得到的训练集图片 被压缩为30x32像素 并且作为输入 提供给ALVINN的三层  

神经网络。 通过使用反向传播学习算法， ALVINN会训练得到一个 与人类驾驶员操纵方向 基本相近的结果。
一开始 我们的网络选择出的方向是随机的 ， 

大约经过两分钟的训练后 ，我们的神经网络 便能够准确地模拟 人类驾驶者的  

驾驶方向 对其他道路类型
## 4测验quiz
- 
You are training a three layer neural network and would like to use backpropagation to compute the gradient of the cost function. In the backpropagation algorithm, one of the steps is to update。答案A 
![](http://oqy7bjehk.bkt.clouddn.com/17-12-3/50567110.jpg)- 
Suppose Theta1 is a 5x3 matrix, and Theta2 is a 4x6 matrix. You set thetaVec=[Theta1(:);Theta2(:)]. Which of the following correctly recovers Theta2?答案A 
![](http://oqy7bjehk.bkt.clouddn.com/17-12-3/96288279.jpg)- 
Let J(θ)=3θ3+2. Let θ=1, and ϵ=0.01. Use the formula J(θ+ϵ)−J(θ−ϵ)2ϵ to numerically compute an approximation to the derivative at θ=1. What value do you get? (When θ=1, the true/exact derivative is dJ(θ)dθ=9.)答案：A 
![](http://oqy7bjehk.bkt.clouddn.com/17-12-3/47393977.jpg)- 
Which of the following statements are true? Check all that apply.答案AD 
![](http://oqy7bjehk.bkt.clouddn.com/17-12-3/96772320.jpg)- 
Which of the following statements are true? Check all that apply.答案AD


If we are training a neural network using gradient descent, one reasonable “debugging” step to make sure it is working is to plot J(Θ) as a function of the number of iterations, and make sure it is decreasing (or at least non-increasing) after each iteration. 
![](http://oqy7bjehk.bkt.clouddn.com/17-12-3/83689737.jpg)
## 5编程题

在前面的练习中，你实现了神经网络的前馈传播，并用它来预测我们提供的权重的手写数字。 在这个练习中，您将实施反向传播算法来学习神经网络的参数。 所提供的脚本ex4.m将帮助您逐步完成这个练习。
- Part 1: Loading and Visualizing Data 

自动画图，不用修改。
- Part 2: Loading Parameters

自动加载
- Part 3: Compute Cost (Feedforward)
- Part 4: Implement Regularization 

这两个都是修改nnCostFunction.m

```matlab
%计算每层的结果，记得要把bias unit加上，第一次写把a1 写成了 [ones(m,1); X];  
a1 = [ones(m,1) X];  
z2 = Theta1 * a1';  
a2 = sigmoid(z2);  
a2 = [ones(1,m); a2]; %这里 a2 和 a1 的格式已经不一样了，a1是行排列，a2是列排列  
z3 = Theta2 * a2;  
a3 = sigmoid(z3);  

% 首先把原先label表示的y变成向量模式的output，下面用了循环  
y_vect = zeros(num_labels, m);  
for i = 1:m,  
    y_vect(y(i),i) = 1;  
end;  

%每一training example的cost function是使用的向量计算，然后for loop累加所有m个training example  
%的cost function  
for i=1:m,  
    J =J+ sum(-1*y_vect(:,i).*log(a3(:,i))-(1-y_vect(:,i)).*log(1-a3(:,i)));  
end;  
J = J/m;  

%增加regularization，一开始只写了一个sum，但其实theta1 2 分别都是矩阵，一个sum只能按列累加，bias unit的theta不参与regularization  
J = J + lambda*(sum(sum(Theta1(:,2:end).^2))+sum(sum(Theta2(:,2:end).^2)))/2/m;  

%backward propagation  
%Δ的元素个数应该和对应的theta中的元素的个数相同  
Delta1 = zeros(size(Theta1));  
Delta2 = zeros(size(Theta2));  
for i=1:m,  
    delta3 = a3(:,i) - y_vect(:,i);  
    T=(Theta2'*delta3);
    %注意这里的δ是不包含bias unit的delta的，毕竟bias unit永远是1，  
    %不需要计算delta, 下面的2:end,: 过滤掉了bias unit相关值  
    delta2 = T(2:end,:).*sigmoidGradient(z2(:,i));  
    %移除bias unit上的delta2，但是由于上面sigmoidGradient式子中  
    %的z，本身不包含bias unit，所以下面的过滤不必要，注释掉。  
    %delta2 = delta2(2:end);  
    Delta2 =Delta2+ delta3 *a2(:,i)';

    %第一层的input是一行一行的，和后面的结构不一样，后面是一列作为一个example  
    Delta1 =Delta1+ delta2 * a1(i,:);  
end;  

%总结一下，δ不包含bias unit的偏差值，Δ对跟θ对应的，用来计算每个θ  
%后面的偏导数的，所以Δ包含bias unit的θ  
Theta2_grad = Delta2/m;  
Theta1_grad = Delta1/m;  

%regularization gradient  

Theta2_grad(:,2:end) = Theta2_grad(:,2:end) + lambda * Theta2(:,2:end) / m;  
Theta1_grad(:,2:end) = Theta1_grad(:,2:end) + lambda * Theta1(:,2:end) / m;
```
- Part 5: Sigmoid Gradient

在开始实施神经网络之前，您将首先实现sigmoid函数的渐变。 您应该完成sigmoidGradient.m文件中的代码。
`g = 1.0 ./ (1.0 + exp(-z));`- Part 6: Initializing Pameters

在这部分的练习中，你将开始实现分类数字的双层神经网络。 您将开始执行一个函数来初始化神经网络的权重（randInitializeWeights.m）

```
epsilon = 0.12;
W = rand(L_out, 1+L_in)*2*epsilon - epsilon;
```


























