# caffe学习笔记之Multilabel classification on PASCAL using python data-layers - AI之路 - CSDN博客





2017年05月02日 18:56:32[AI之路](https://me.csdn.net/u014380165)阅读数：2338








最近在看Caffe的Multilabel classification on PASCAL using python data-layers，是关于在PASCAL数据集上做多标签（multilabel）分类的例子，这里注意多标签和多分类（multiclass）不一样，前者一个样本可能有多个label，而后者不是。

参考地址：http://nbviewer.jupyter.org/github/BVLC/caffe/blob/master/examples/pascal-multilabel-with-datalayer.ipynb




另外本篇博文的参考资料如下：

参考资料1：[http://caffe.berkeleyvision.org/tutorial/layers](http://caffe.berkeleyvision.org/tutorial/layers)这个是关于caffe里面layers的官方文档介绍，非常详细

参考资料2：[http://blog.csdn.net/u011762313/article/details/48213421](http://blog.csdn.net/u011762313/article/details/48213421) 这个是讲用pycaffe定义网络，可以对prototxt文件有更深刻的认识




**接下来主要按照顺序来解读这个例子**

**1、Preliminaries**

WITH_PYTHON_LAYER :=1是在CAFFE根目录下的Makefile.config文件中，表示使用python接口。

sys.path是Python的搜索路径集，是一个列表。可以用sys.path.append(path)添加相关的路径，然后import该路径下的模块，因为tools在pycaffe文件夹中，所以需要先把pycaffe文件夹append到python的搜索路径。


sys.path.append("你的路径/caffe/examples/pycaffe")

import tools

但在退出python环境后这些路径就不包含了



以下两行是关于GPU的，第一行表示用GPU，第二行里面的0表示GUP的ID。

caffe.set_mode_gpu()

caffe.set_device(0)




os.path.join(path,name):连接目录与文件名或目录

os.path.isdir(name):判断name是不是一个目录，name不是目录就返回false




**2、Define network prototxts**

这几个都是辅助函数，基本都是把卷积和relu合并或者全连接层和relu合并，所以只是方便而已，也可以直接用caffe的layer来写。

![](https://img-blog.csdn.net/20170502192751164?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


def caffenet_multilabel()表示定义一个net，在这个net中首先通过n=caffe.NetSpec()获取Caffe的一个Net，然后不断填充这个n，首当其冲就是数据层，因为任何模型的训练都要首先读入数据，这里并不是采用传统的Data层，而是采用Python层。

关于python layer，官方的解释如下：

The Python layer allows users to add customized layers without modifying the Caffe core code.

来看看例子中的python层都做了什么：


![](https://img-blog.csdn.net/20170502191422833?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



这里的module是一个.py文件的文件名，即pascal_multilabel_datalayers.py，这个文件里面定义了一个类PascalMultilabelDataLayerSync，对应这里的layer，这个类直接继承的是caffe.Layer，然后要重写setup()，reshape()，forward()，backward()函数。这里说一句，caffe里面的层比如卷积层，池化层等都有类似setup(),forward()等，意思是初始化，前向传递等，当然部分层还有反向传播。而像数据层的话，就没有反向传播，因此它的backward()函数内容就只是pass。另外这里的data_layer_params传给Python层的param_str，param_str在PascalMultilabelDataLayerSync的setup函数中用到：params=eval(self.param_str)，self.batch_size=params['batch_size']



**3、write nets and solver files**

在caffe/examples/pycaffe 文件夹中有一个 tools.py 文件，这个文件可以生成我们所需要的 solver 的 prototxt 文件。

以下这个图片的几行代码的意思就是新建一个solver.prototxt文件，tools.caffeSolver()括号里面默认为空，该例子中写了两个东西，分别指明训练网络结构和测试网络结构的地址和名字。然后往这个文件里面写入默认的参数以及这两个重新赋值的参数（原来base_lr默认是0.001，display默认是25），如果全部要默认的话，这两行就不用写。最后一句就是写入solver文件。

![](https://img-blog.csdn.net/20170502191959795?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


后面这两行就是写入trainnet.prototxt和test.prototxt，就是前面写的网络结构写到prototxt文件中去，和caffe自带的例子比如mnist的区别主要还是在于用自定义的python层代替了Data层。

![](https://img-blog.csdn.net/20170502192407140?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


这里的SGDSolver是继承Solver，是caffe的一个类，表示采用随机梯度下降优化，当然还有其他Solver比如AdaGradSolver、RMSPropSolver等继承自SGDSolver，是优化版的随机梯度下降。第二行的copy_from表示用另一个模型的参数初始化。

![](https://img-blog.csdn.net/20170502193652366?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


最后一行的solver.step(1)表示一共迭代一次。step(）函数是核心，具体内容查看[点击打开链接](http://alanse7en.github.io/caffedai-ma-jie-xi-4/)




**4、Train a net**

这部分主要是训练，定义准确率计算方式比较容易理解。

主要的训练部分如下：训练六次，每次迭代100次并计算准确率。

![](https://img-blog.csdn.net/20170502203644865?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)







