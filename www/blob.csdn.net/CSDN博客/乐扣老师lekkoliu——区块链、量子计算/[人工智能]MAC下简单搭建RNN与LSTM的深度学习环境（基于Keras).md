
# [人工智能]MAC下简单搭建RNN与LSTM的深度学习环境（基于Keras) - 乐扣老师lekkoliu——区块链、量子计算 - CSDN博客

2018年08月29日 10:59:13[乐扣老师lekkoliu](https://me.csdn.net/lsttoy)阅读数：361


前置环境：Python3.7和编译器IDE PyCharm 搭建———- 1.Python3.7通过如下官网可以安装（练习用）[https://www.python.org/downloads/mac](https://www.python.org/downloads/mac)-osx / 2.PyCharm的IDE安装楼主主要是使用IDEA太多年，因此就选了这个编译器。有其他爱好的可以使用其他编译器.PyCharm简单搜索就可以找到JetBrain官网上的安装地址
## .Keras环境搭建：Python3.7和编译器IDE PyCharm搭建
1、安装anaconda  （后期使用的python都是基于anaconda内置的）
非常好用的工具，这是一个基于Python的数据处理和科学计算平台，它已经内置了很多非常有用的第三方库。
[https://www.anaconda.com/download/\#macos](https://www.anaconda.com/download/#macos)
PS：安装时可以选择不安装微软套件。
2、安装theano
keras的依赖库，在anaconda的environments中选择not installed搜索theano安装即可。
3、安装人工智能系统tensorflow
打开share界面，pip是python包里的一个套件，因此可以在termonal中直接输入命令安装。
pip install tensorflow
4、安装环境Keras
pip install keras
## Keras环境使用
1、 如果使用IDE如PyCharm，那么需要在preferences里面修改project interpreter。
2、 如果使用脚本，则可以在anaconda里面使用自带的python脚本进行作业了。
## 遇到其他的坑
1.keras中的基本问题：报错TypeError：while _ loop（）得到一个意外的关键字参数’maximum _ iterations’ 这种问题反映出是框架版本不容的问题，基本上能够运行的版本可以为稍微旧的版本：pip install keras == 2.1.2 pip install tensorflow == 1.4.1 因此如果报错的话，需要把keras和tensorflow那边的版本调整到对应容错的版本。
2.一般来说，版本可以测试选择相互兼容的版本，需要测试框架的兼容性。
就目前而言，对于tensorflow和keras，兼容的比较好的版本是pip install keras==2.1.6
pip install tensorflow==1.9.0。
以此能解决如下两个问题：
1)RuntimeWarning: compiletime version 3.5 of module ‘tensorflow.python.framework.fast_tensor_util’ does not match runtime version 3.6
2)Your CPU supports instructions that this TensorFlow binary was not compiled to use: AVX AVX2
## 结束语
觉得好的朋友可以给我大赏一杯咖啡钱缓解通宵梳理环境的疲乏〜！
方式一：支付宝
![这里写图片描述](https://img-blog.csdn.net/20180831102425105?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xzdHRveQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)[ ](https://img-blog.csdn.net/20180831102425105?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xzdHRveQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
方式二：以太坊
![这里写图片描述](https://img-blog.csdn.net/2018083110244130?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xzdHRveQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

[
](https://img-blog.csdn.net/20180831102425105?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xzdHRveQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
