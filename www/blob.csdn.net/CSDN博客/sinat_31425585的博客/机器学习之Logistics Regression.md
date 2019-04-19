# 机器学习之Logistics Regression - sinat_31425585的博客 - CSDN博客
2018年10月04日 12:22:07[Mirror_Yu_Chen](https://me.csdn.net/sinat_31425585)阅读数：122
**1、线性分类器**
    在说线性分类器之前，先思考一下我们平时是如何来做决策的。我们做任何决策之前，我们都会考虑很多因素，这里我们以决策树中经常举的相亲例子来进行说明。
    假设，现在有人说要给你介绍个女朋友，你会问他，女孩多大了？性格如何？长得好看吗？我们会考虑三个因素：年龄是否匹配、性格优劣和长相好坏（是否又勾勾又丢丢），这里我们用三个二进制数![x_{1},x_{2},x_{3}](https://private.codecogs.com/gif.latex?x_%7B1%7D%2Cx_%7B2%7D%2Cx_%7B3%7D)表示这三个因素，三个因素对你决定去或者不去相亲的影响程度为![w_{1},w_{2},w_{3}](https://private.codecogs.com/gif.latex?w_%7B1%7D%2Cw_%7B2%7D%2Cw_%7B3%7D)，这时，你会得到一个想去相亲的程度：
![y=w_{1}x_{1}+w_{2}x_{2}+w_{3}x_{3}](https://private.codecogs.com/gif.latex?y%3Dw_%7B1%7Dx_%7B1%7D&plus;w_%7B2%7Dx_%7B2%7D&plus;w_%7B3%7Dx_%7B3%7D)
    那么你到底去不去？我们会设定一个阈值![threshold](https://private.codecogs.com/gif.latex?threshold)，如果我想去的程度大于![threshold](https://private.codecogs.com/gif.latex?threshold)我就去，反之，我不去，因此决策模型就变成了
![y=w_{1}x_{1}+w_{2}x_{2}+w_{3}x_{3}-threshold](https://private.codecogs.com/gif.latex?y%3Dw_%7B1%7Dx_%7B1%7D&plus;w_%7B2%7Dx_%7B2%7D&plus;w_%7B3%7Dx_%7B3%7D-threshold)
    为了表示的简洁性，我们可以将![-threshold](https://private.codecogs.com/gif.latex?-threshold)计为![b](https://private.codecogs.com/gif.latex?b)，那么有：
![y=w_{1}x_{1}+w_{2}x_{2}+w_{3}x_{3}+b](https://private.codecogs.com/gif.latex?y%3Dw_%7B1%7Dx_%7B1%7D&plus;w_%7B2%7Dx_%7B2%7D&plus;w_%7B3%7Dx_%7B3%7D&plus;b)
    将其表示成向量形式，令：
![W=\begin{bmatrix} w_{1}\\ w_{2}\\ w_{3} \end{bmatrix},X=\begin{bmatrix} x_{1}\\ x_{2}\\ x_{3} \end{bmatrix}](https://private.codecogs.com/gif.latex?W%3D%5Cbegin%7Bbmatrix%7D%20w_%7B1%7D%5C%5C%20w_%7B2%7D%5C%5C%20w_%7B3%7D%20%5Cend%7Bbmatrix%7D%2CX%3D%5Cbegin%7Bbmatrix%7D%20x_%7B1%7D%5C%5C%20x_%7B2%7D%5C%5C%20x_%7B3%7D%20%5Cend%7Bbmatrix%7D)
    有：
![y=W^{T}X+b](https://private.codecogs.com/gif.latex?y%3DW%5E%7BT%7DX&plus;b)
    这也即是我们所说的线性分类器，如果y大于0，我们就去，反之我们就不去。
**    2、 Logistics Regression**
    如果权重![W](https://private.codecogs.com/gif.latex?W)和b细小的变化只是轻微影响到输出![y](https://private.codecogs.com/gif.latex?y)，那么我们可以逐步更改权重![W](https://private.codecogs.com/gif.latex?W)和b，让模型按照我们想要的方式发展，这样就能得到一个理想的模型。
   但是，这里存在一个问题，权重![W](https://private.codecogs.com/gif.latex?W)和b中任何一个参数微小变化都能使得模型输出结果彻底翻转，从0到1，因此引入sigmoid函数，来克服上述问题，使用sigmoid函数后，他会将y的值域由  ![[-\infty,+ \infty]](https://private.codecogs.com/gif.latex?%5B-%5Cinfty%2C&plus;%20%5Cinfty%5D)  映射到  ![[0,1]](https://private.codecogs.com/gif.latex?%5B0%2C1%5D)，  权重![W](https://private.codecogs.com/gif.latex?W)和b中任何一个参数微小变化只会微小的影响到输出结果。
    sigmoid函数定义为：
![\sigma (z)=\frac{1}{1+e^{-z}}](https://private.codecogs.com/gif.latex?%5Csigma%20%28z%29%3D%5Cfrac%7B1%7D%7B1&plus;e%5E%7B-z%7D%7D)
**    3、计算图算梯度**
    在Andrew Ng的深度学习课程中，我第一次接触到计算图，然后就被其简单明了的表示方法所吸引，计算图简单明了的描述了在进行计算时，我们需要进行的每一步计算过程，思路十分清晰，且不容易造成错误。
    在说Logistics Regression的计算图之前，我们先看一下一个简单的计算图例子，如图1所示。
![](https://img-blog.csdn.net/20181006122311254?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMxNDI1NTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
                                                                         图1 计算图小例子（摘自参考资料[1]）
    我们已知三个变量a,b,c，而需要求解![J=3*(a+bc)](https://private.codecogs.com/gif.latex?J%3D3*%28a&plus;bc%29)分别对于三个变量a,b,c的偏导，我们首先可以将计算J的步骤表示成计算图的形式（怎么样一步一步的通过对变量a,b,c操作，最后得到计算结果J），然后再基于计算图，一步一步的求解J对三个变量a,b,c的偏导。
    举个例子，这里求J对a的偏导有：
    1) 求J对隐变量v的偏导有：
![\frac{\partial J}{\partial v}=\frac{\partial (3v)}{\partial v}=3](https://private.codecogs.com/gif.latex?%5Cfrac%7B%5Cpartial%20J%7D%7B%5Cpartial%20v%7D%3D%5Cfrac%7B%5Cpartial%20%283v%29%7D%7B%5Cpartial%20v%7D%3D3)
    2) 求v对变量a的偏导有：
![\frac{\partial v}{\partial a}=\frac{\partial (u+a)}{\partial a}=1](https://private.codecogs.com/gif.latex?%5Cfrac%7B%5Cpartial%20v%7D%7B%5Cpartial%20a%7D%3D%5Cfrac%7B%5Cpartial%20%28u&plus;a%29%7D%7B%5Cpartial%20a%7D%3D1)
    所以，最终结果有：
![\frac{\partial J}{\partial a}=\frac{\partial J}{\partial v}\frac{\partial v}{\partial a}=3*1=3](https://private.codecogs.com/gif.latex?%5Cfrac%7B%5Cpartial%20J%7D%7B%5Cpartial%20a%7D%3D%5Cfrac%7B%5Cpartial%20J%7D%7B%5Cpartial%20v%7D%5Cfrac%7B%5Cpartial%20v%7D%7B%5Cpartial%20a%7D%3D3*1%3D3)
    这样，我们就能很清晰的得到需要求解偏导，同时，求解过程很符合我们的编程思路，有利于我们去实现这样一个求解偏导过程，其余两个变量偏导数求解就不做说明了。
    我们再来看一看Logistics Regression的计算图，如图2所示。
![](https://img-blog.csdn.net/20181006123845394?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMxNDI1NTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
                                                          图2 Logistics Regression 的计算图（摘自参考资料[1]）
    那么，我们再基于这个计算图来计算一个L分别对于对于![w_{i},b](https://private.codecogs.com/gif.latex?w_%7Bi%7D%2Cb)的偏导：
    1) 求L对a的偏导数有：
![\frac{\partial L}{\partial a}=-(\frac{y}{a}-\frac{1-y}{1-a})=\frac{1-y}{a(1-a)}](https://private.codecogs.com/gif.latex?%5Cfrac%7B%5Cpartial%20L%7D%7B%5Cpartial%20a%7D%3D-%28%5Cfrac%7By%7D%7Ba%7D-%5Cfrac%7B1-y%7D%7B1-a%7D%29%3D%5Cfrac%7B1-y%7D%7Ba%281-a%29%7D)
    2) 求a对z的偏导数有：
![\frac{\partial a}{\partial z}=a(1-a)](https://private.codecogs.com/gif.latex?%5Cfrac%7B%5Cpartial%20a%7D%7B%5Cpartial%20z%7D%3Da%281-a%29)
    3) 求z对![w_{i}](https://private.codecogs.com/gif.latex?w_%7Bi%7D)的偏导数有：
![\frac{\partial z}{\partial w_{i}}=x_{i}](https://private.codecogs.com/gif.latex?%5Cfrac%7B%5Cpartial%20z%7D%7B%5Cpartial%20w_%7Bi%7D%7D%3Dx_%7Bi%7D)
    同样的，求z对b的偏导数有：
![\frac{\partial z}{\partial b}=1](https://private.codecogs.com/gif.latex?%5Cfrac%7B%5Cpartial%20z%7D%7B%5Cpartial%20b%7D%3D1)
    这样，我们就可以得到L分别对于对于![w_{i},b](https://private.codecogs.com/gif.latex?w_%7Bi%7D%2Cb)的偏导为：
![\frac{\partial L}{\partial w_{i}}=\frac{1-y}{a(1-a)}*a(1-a)*x_{i}=(1-y)x_{i}](https://private.codecogs.com/gif.latex?%5Cfrac%7B%5Cpartial%20L%7D%7B%5Cpartial%20w_%7Bi%7D%7D%3D%5Cfrac%7B1-y%7D%7Ba%281-a%29%7D*a%281-a%29*x_%7Bi%7D%3D%281-y%29x_%7Bi%7D)
![\frac{\partial L}{\partial b}=\frac{1-y}{a(1-a)}*a(1-a)*1=1-y](https://private.codecogs.com/gif.latex?%5Cfrac%7B%5Cpartial%20L%7D%7B%5Cpartial%20b%7D%3D%5Cfrac%7B1-y%7D%7Ba%281-a%29%7D*a%281-a%29*1%3D1-y)
    采用计算图的方法使得求解过程瞬间变得清晰明了了，同时，也十分有利于编程。
**    4、Logistics Regression损失函数**
    这里其实还有一个问题，这里的损失函数L是如何得到的，为什么要采用这种形式的损失函数？其实，很简单，如果采用最常见的平方差损失来作为Logistics Regression的损失函数，在后面优化时，会出现损失函数非凸的情况，使得找到的最优解只是局部最优（局部极小值），而不是全局最优解，因此，Logistics Regression采用了一种等效形式：
![L(a,y)=-(yloga+(1-y)log(1-a))](https://private.codecogs.com/gif.latex?L%28a%2Cy%29%3D-%28yloga&plus;%281-y%29log%281-a%29%29)
    这里，我们来进一步的看一看这个Loss function，当y=1时，有：
![L(a,y)=-loga,a=\sigma (z)](https://private.codecogs.com/gif.latex?L%28a%2Cy%29%3D-loga%2Ca%3D%5Csigma%20%28z%29)
    如果我们让L尽可能小，对应的就是让![a](https://private.codecogs.com/gif.latex?a)尽可能大，而![\sigma](https://private.codecogs.com/gif.latex?%5Csigma)函数的值域为[0,1]，那么其实优化L让其尽可能小的过程，其实就是在y=1时，让预测值尽可能让 ![a=1](https://private.codecogs.com/gif.latex?a%3D1) 的过程。
    当y=0时，有：
![L(a,y)=-log(1-a),a=\sigma (z)](https://private.codecogs.com/gif.latex?L%28a%2Cy%29%3D-log%281-a%29%2Ca%3D%5Csigma%20%28z%29)
    如果我们让L尽可能小，对应的就是让![1-a](https://private.codecogs.com/gif.latex?1-a)尽可能大，等价于让![a](https://private.codecogs.com/gif.latex?a)尽可能小，而![\sigma](https://private.codecogs.com/gif.latex?%5Csigma)函数的值域为[0,1]，那么其实优化L让其尽可能小的过程，其实就是在y=0时，让预测值尽可能让 ![a =0](https://private.codecogs.com/gif.latex?a%20%3D0) 的过程。
    采用这种形式的损失函数还有一个好处，那就是这种损失函数是凸函数，局部极小值等价于全局极小值，求解得到的局部最优解即全局最优解。
**    5、代码实现**
~~~~~未完待续~~~~~
参考资料：
[1] Andrew Ng  deeplearning.ai课程
