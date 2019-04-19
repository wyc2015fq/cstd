# caffe学习笔记3.2--前向传播和反向传播 - 数据之美的博客 - CSDN博客
2017年03月07日 22:25:51[看穿数据之美](https://me.csdn.net/oppo62258801)阅读数：694
caffe学习笔记3从3.1开始主要翻译一下caffe的官方文档，写的非常好，忍不住要作一下。本篇翻译来自caffe官网的：[http://caffe.berkeleyvision.org/tutorial/forward_backward.html](http://caffe.berkeleyvision.org/tutorial/forward_backward.html)
前向传播和反向传播是计算神经网络非常重要的部分。
![](https://img-blog.csdn.net/20160425181709805?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
考虑一个简单的逻辑回归分类问题
前向传播：通过网络的输入计算输出结果的过程，在前向传播中，caffe整合每一层的计算得到整个模型的计算函数，这个过程是个自底向上的过程，如图：
![](https://img-blog.csdn.net/20160425182408751?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
数据x通过通过内积层得到g(x),然后通过softmax得到h(g(x))和损失函数（softmax loss）fw(x).
反向传播网络：根据损失计算梯度，在反向传播过程中，Caffe通过自动求导计算逆向组合每一层的梯度得到整个模型的梯度，在反向传播中，这个过程是自顶向下的。如图：
![](https://img-blog.csdn.net/20160425183812304?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
反向过程由loss开始，根据输出计算梯度dfw/dh (fw对h求导)， 模型中其余各层的梯度根据链式法则计算。每层的参数：如`INNER_PRODUCT层，在反馈过程中对参数计算梯度dfw/dWip.`
``
`你只需要定义好模型，这些计算就可以立即执行，caffe已经定义好的前向传播和反向传播的执行方法。`
- `Net::Forward()和Net::Backward()实现整个网络的前向与后向的传播，Layer::Forward和Layer::Backward()计算每一层的前后向的传播`
- `每一层都有backward_{cpu,gpu}和backward{cpu,gpu}方法适应不同的计算模式。但是，一个层仅能使用其中的一种模式`
`Solver优化一个模型，首先通过forward计算输出和损失，然后通过backward生成模型的梯度，让后根据梯度和权值更新最小话损失函数，Solver，Net和Layer间的分离保证了caffe的模块化设计和开源。`
``
``
参考资料：
http://caffe.berkeleyvision.org/tutorial/forward_backward.html
            
