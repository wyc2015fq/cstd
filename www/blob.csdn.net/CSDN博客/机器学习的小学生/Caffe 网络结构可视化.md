
# Caffe 网络结构可视化 - 机器学习的小学生 - CSDN博客


置顶2017年03月15日 15:36:29[机器学习的小学生](https://me.csdn.net/xuluhui123)阅读数：1371


**Pooling 层输出数据大小：**
\begin{equation}
\begin{aligned}
W_2 &= (W_1-F)/S+1 \
H_2 &= (H_1 -F)/S+1  \
D_2 &= D_1
\end{aligned}
\end{equation}
其中$W_1 \cdot H_1 \cdot D_1$是输入数据的大小。
**Conv层输出数据大小：**
\begin{equation}
\begin{aligned}
W_2 &= (W_1-F+2P)/S+1 \
H_2 &= (H_1 -F+2P)/S+1  \
D_2 &= K
\end{aligned}
\end{equation}
其中$W_1 \cdot H_1 \cdot D_1$是输入数据的大小。
各种可视化工具博客:
[https://handong1587.github.io/deep_learning/2015/10/09/visulize-cnn.html](https://handong1587.github.io/deep_learning/2015/10/09/visulize-cnn.html)
**1.在线可视化Caffe:**
主入口：
[http://ethereon.github.io/netscope/quickstart.html](http://ethereon.github.io/netscope/quickstart.html)
快捷入口：
[http://ethereon.github.io/netscope/\#/editor](http://ethereon.github.io/netscope/#/editor)
**2.利用 python/draw_net.py 可视化。**
1.下载 protobuf-3.1.0-py2.py3-none-any.whl
`pip install protobuf-3.1.0-py2.py3-none-any.whl`2.下载pydot-1.2.3.tar.gz ,解压后:
`python setup.py install`3.下载 graphviz-2.38.zip ,解压后，将下面的目录添加到环境变量即可：
`E:\caffe-windows\graphviz\release\bin`4.配置eclipse run configuration ,程序参数输入：
`train_val.prototxt result.jpg --rankdir=TB`然后运行，生成的结果为result.jpg
安装过程中碰到的问题，很奇怪的是我已经安装了protobuf，并且在
`D:\python\python2.7.12\Lib\site-packages`目录下能够找到google\protobuf\text_format.py文件，但是却通过下面指令，导入时提示错误：
![这里写图片描述](https://img-blog.csdn.net/20170323102239834?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcmFieV9neWw=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
然后，我将google下的protobuf也添加到库的包含目录中，通过下面的形式引入：
`from protobuf import text_format`问题就解决了，很纳闷。
**参考文献**
1[http://blog.csdn.net/solomon1558/article/details/53144606](http://blog.csdn.net/solomon1558/article/details/53144606)
[Caffe学习系列——工具篇：神经网络模型结构可视化]
2[http://blog.csdn.net/tina_ttl/article/details/51800635](http://blog.csdn.net/tina_ttl/article/details/51800635)[python错误锦集及各种packages安装] 很不错！
3.[https://cuijiahua.com/convdraw/](https://cuijiahua.com/convdraw/)[简单的画图参考可视化 for convs]

