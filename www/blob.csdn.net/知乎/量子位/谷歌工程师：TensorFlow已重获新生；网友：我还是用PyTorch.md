# 谷歌工程师：TensorFlow已重获新生；网友：我还是用PyTorch - 知乎
# 



> 乾明 发自 凹非寺 
量子位 报道 | 公众号 QbitAI
![](https://pic4.zhimg.com/v2-7161698fb169dc180b442f9fcf9294a7_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='425' height='228'></svg>)



道友留步！TensorFlow已重获新生。

在“PyTorch真香”的潮流中，有人站出来为TensorFlow说话了。

这次来自谷歌的工程师Cassie Kozyrkov。她发表博文称，TensorFlow升级到2.0版本后有了翻天覆地的变化， 对新手更加友好了。

引发这一变化的，是Keras。

在TensorFlow 2.0中，Keras API将成为TensorFlow中构建和训练模型的核心高级API。



![](https://pic3.zhimg.com/v2-883ae33b84dce795d47a79f44c29db32_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='589'></svg>)



在TensorFlow启动项目将变得更简单。

Kozyrkov说， 与TensorFlow 1.X的版本相比，这就是脱胎换骨的变化。

## **难用的TensorFlow1.X**

想要掌握TensorFlow1.X，需要花费不少力气，学习曲线非常陡峭，普通用户只能远远看着。

背后的原因在于，Tensorflow是为从研究到生产的大规模模型而设计的，核心是性能。虽然难学，但只要坚持下去，就能进入机器学习从业者行列了。

然后，就可以用机器学习做一些不可思议的事情了，比如说发现新的恒星，推动医学进步…..

但很可惜，这太难用了，只有少数人能够掌握这种力量。



![](https://pic1.zhimg.com/v2-cb80047172bf0127c8d5c3cff097a0c4_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='742' height='376'></svg>)



很多新的AI研究人员，以及不少Tensorflow用户，都喊着“真香”，转投PyTorch，毕竟隔壁家的不需要花太多力气。

还好有Keras，让不少人感受到了一些安慰。

Keras，是一个用于逐层构建模型的框架，可以与多个机器学习框架一起工作，它从一开始就是Pythonic的，设计灵活，易于学习，吸引了不少人的使用和支持。

虽然它是一个从TensorFlow 内部访问的高级 API，但它跟TF没啥关系。

开发者得在好用的Keras和性能强大的TensorFlow之间做出选择。

这就很容易给人一种“PyTorch”真香的感觉。

在TensorFlow2.0中，解决了这个问题。

## **TensorFlow2.0的变化**

谷歌TF工程负责人Karmel Alliso表示，开发者不应该在简单的 API 和可伸缩的 API 之间做出选择，要有一个高级的API，不论是研究MNIST还是研究行星，都能搞定。

于是，Keras成了TensorFlow的高级 API，而且是可扩展的，可以直接从tf.keras使用 TensorFlow的所有高级功能。

而且Keras的简单性，也带到了TensorFlow上。

这种结合，会大幅度减少学习TensorFlow的门槛，对于初学者来说，是非常友好的。



![](https://pic2.zhimg.com/v2-d7feeaad9dacd83bcb2a647ee0dfd435_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='824' height='520'></svg>)



现在，TensorFlow2.0已经推出了Alpha版，如果你有想法， 可以收好下面的教程传送门：
[https://www.tensorflow.org/alpha​www.tensorflow.org](https://link.zhihu.com/?target=https%3A//www.tensorflow.org/alpha)
教程分成了初学者版和老司机版：

初学者版，用的是Keras Sequential API，最简单的入门大法；

老司机版，展示了怎样用命令式来写正向传播、怎样用GradientTape来写自定义训练loop，以及怎样用tf.function一行代码自动编译。

## **网友：我还是用PyTorch**

这篇博文发表了之后，Medium上获得了3K多的“鼓掌”，在Reddit上有了近百条讨论。

TensorFlow和Keras更好地结合起来，让一些人感觉挺好。但在讨论中，有不少网友依旧心心念着“PyTorch”。

有人评价称，这还是没有PyTorch好用，PyTorch感觉就像NumPy，可以在一天内学会，然后它就能正常工作。而学习TF（甚至2.0）会让人感觉自己很笨。



![](https://pic4.zhimg.com/v2-137eb89fd122ce48f83fffa8da04dafb_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='228'></svg>)



有人说，只要我们有PyTorch，谁会关心TF。




![](https://pic4.zhimg.com/v2-8001a2a491ca5e2fa4249ca47c65eccf_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='891' height='216'></svg>)



甚至还有“脱粉”的现象：


> 好吧，至少现在我有足够的动力离开TensorFlow并切换到PyTorch。



![](https://pic4.zhimg.com/v2-ad70a38c5807c4725e8c2d73937f7ffb_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='207'></svg>)



你呢？

TensorFlow 2.0还是PyTorch？

— **完** —

量子位 · QbitAI

վ'ᴗ' ի 追踪AI技术和产品新动态

戳右上角「+关注」获取最新资讯↗↗

如果喜欢，请分享or点赞吧~比心❤




