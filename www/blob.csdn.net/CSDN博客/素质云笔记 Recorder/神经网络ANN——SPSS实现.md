# 神经网络ANN——SPSS实现 - 素质云笔记/Recorder... - CSDN博客





2015年08月22日 09:54:04[悟乙己](https://me.csdn.net/sinat_26917383)阅读数：13523标签：[神经网络																[SPSS径向基																[多层感知器MLP](https://so.csdn.net/so/search/s.do?q=多层感知器MLP&t=blog)
个人分类：[SPSS Modeler&SPSS](https://blog.csdn.net/sinat_26917383/article/category/5754503)





神经网络[]

一、起源与历史

1、与传统统计方法的区别

传统线性回归模型可通过最小平方方法获取知识并在回归系数存储知识。在此意义下，其为神经网络。实际上，您可以证明线性回归为特定神经网络的特殊个案。但是，线性回归具有严格模型结构和在学习数据之前施加的一组假设。

神经网络可以接近多种统计模型，并无需您预先假设因变量和自变量间的特定关系。

若因变量和自变量间实际为线性关系，神经网络结果应接近线性回归模型的结果；

若两者为非线性关系，神经网络将自动接近“正确”模型结构。

但是如果您正试图解释生成因变量和自变量间关系的基础过程，最好使用更传统的统计模型。但是，如果模型的可解释性并不重要，您可以使用神经网络更快获取良好模型结果。[]

2、多层感知器MLP的应用

两大重要的应用：特征选择（变量选择）；预测。

因变量的取值范围很广。
||||||
|----|----|----|----|----|

如何提高训练速度：编码问题（多数“压缩”编码方法通常导致较差的拟合神经网络。如果您的网络培训进行很慢，尝试通过将类似的类别组合起来或删除具有极少见类别的个案以减少分类预测变量中的类别数目）；

3、非线性分析的应用

有以下几种应用在非线性分析的方法：多层感知（MLP）、径向基函数（RBF）、SVM、广义回归神经网络（GRNN）和广义神经网络（GNN）。[]












四、径向基神经网络（RBFN）

全局逼近神经网络（BP）多网络所有隐含层、输出层变量进行赋权、认定阀值，学习速度慢，在实时预测中很难做到；

而径向基神经网络，是局部逼近，局部赋值与认定阀值，实际应用能力较强。

1、原理

径向基函数是一种类似母函数（简单函数），通过基函数来映射高维空间函数特征。就像是多项式可以通过x与x次方的方式，逼近某一函数一样。低维空间非线性可分的问题总可以映射高维空间（输入——隐含层是径向基层），使其在高维空间线性可分（隐含层——输出是线性函数层）。

输入——隐含层是径向基层（非线性），隐含层——输出是线性函数层。径向基层， 径向基神经元权重与输入层权重对比，相近的权重设定趋于1，偏离的权重设定趋于0（不起作用）。从而相近权重的输入变量激活了“隐含层——输出层”的权重。

RBFN看上去网络是全连接的，但实质上只有几个输入变量对指定的径向基层有贡献，所以是一个局部逼近的过程，训练速度比BP要快2-3个数量级。

RBFN比BP隐含层神经元要多，可以构成高维隐单元空间，只要隐含层神经元的数目足够多，就可以使输出层空间线性可分。







五、SPSS的R组件安装

使用PASW Statistics-R Essentials插件作为接口， 自动安装。

同时excel也支持了R语言的插件接入。[]

1、安装R组件[]

关于SPSS插件安装。

安装流程：

先安装   SPSS Statistics 21.0

再安装   SPSS Statistics Python Essentials 21.0（注意版本 2.7.x）

再安装   Scipy py 2.7 （务必对应 Python Essentials 版本 2.7.x） x64位必须用x64位的Scipy py 2.7 官方没有，第三方的可以。官方有第三方的链接。

再安装   Numpy py 2.7 （务必对应 Python Essentials 版本 2.7.x）x64位必须用x64位的Scipy py 2.7 官方没有，第三方的可以。官方有第三方的链接。

Scipy/Numpy官方地址： [http://www.scipy.org/Download](http://www.scipy.org/Download)

再安装   PLS Extension Module 拷贝到位，将PLS.py和plscommand.xml放入SPSS安装文件夹下\extensions；或将PLS.py放入Python文件夹下Lib\site-packages，plscommand.xml放入\extensions

再安装   R 注意版本 2.14 （千万注意下载 R-2.14.2-win.zip ）

后安装   SPSS Statistics R Essentials 安装时要找 R。



![](https://img-blog.csdn.net/20150822095554065)


![](https://img-blog.csdn.net/20150822095656573)


![](https://img-blog.csdn.net/20150822095700183)


![](https://img-blog.csdn.net/20150822095704554)


![](https://img-blog.csdn.net/20150822095708974)


![](https://img-blog.csdn.net/20150822095712005)


![](https://img-blog.csdn.net/20150822095716207)


![](https://img-blog.csdn.net/20150822095719535)


![](https://img-blog.csdn.net/20150822095725639)


![](https://img-blog.csdn.net/20150822095736607)


![](https://img-blog.csdn.net/20150822095741469)


![](https://img-blog.csdn.net/20150822095745245)


![](https://img-blog.csdn.net/20150822095749045)


![](https://img-blog.csdn.net/20150822095755475)](https://so.csdn.net/so/search/s.do?q=SPSS径向基&t=blog)](https://so.csdn.net/so/search/s.do?q=神经网络&t=blog)




