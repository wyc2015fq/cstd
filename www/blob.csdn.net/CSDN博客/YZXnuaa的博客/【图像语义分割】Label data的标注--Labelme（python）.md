# 【图像语义分割】Label data的标注--Labelme（python） - YZXnuaa的博客 - CSDN博客
2018年04月20日 12:53:46[YZXnuaa](https://me.csdn.net/YZXnuaa)阅读数：876
图像语义分割是一种pixel-wise级的一种图像分类操作，其目的是在图像中上的同一个类别上打上相同的label，以表示这个类别是同一类。
在训练自己的数据集中，语义分割最重要且最基础的一步便是对图像进行标注，以训练得到自己的模型。
在标注图像中，MIT开源了一款标注软件，Labelme. 
[http://labelme2.csail.mit.edu/Release3.0/index.php?message=1](http://labelme2.csail.mit.edu/Release3.0/index.php?message=1)
个人觉得不是特别好用，需要和Matlab toolbox 联合使用。
在这推荐一个python版的labelme，链接： 
[https://github.com/wkentaro/labelme](https://github.com/wkentaro/labelme)
我在测试该软件的时候，python3环境下，发现有几处bug，已经报给作者了。所以如果最近使用的话，还是推荐在python2环境使用。
（第一个bug，作者已经解决了，应该不久就完美支持python3）
安装的话，作者的readme写的非常详细了，由于作者没有windows环境，因此没有对其进行过测试。我在这篇blog上补充下win10环境的anaconda安装环境， 
其安装步骤完全类似于在其他各个平台上的anaconda环境下的labelme安装：
```
conda create --name=labelme python=2.7
source activate labelme
conda install pyqt
pip install labelme
```
- 1
- 2
- 3
- 4
如果想要安装python3的话，更改第一句为：
`conda create --name=labelme python=3.5`- 1
# 软件的使用
可直接参考作者的readme，我这里仅仅是对其补充
作者已经将这个软件变得非常傻瓜式的操作： 
![这里写图片描述](https://img-blog.csdn.net/20170903161445406?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2h3YW5fbWE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
在View处的Andvanced mode可以切换模式。 
通过open读取文件，选择create polygons对想要的区域进行编辑： 
![这里写图片描述](https://img-blog.csdn.net/20170903161818256?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2h3YW5fbWE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
这里仅选择了一个eye，然后保存即可得到一个json文件。
我们的目的是得到label data，json对我们来说还是要转换为单通道的image
终端输入：
```xml
labelme_json_to_dataset <文件名>.json
```
- 1
即可得到一个文件夹，有四个文件，*.png, info.yaml , label.png, label_viz.png。 
其中label.png即是我们要的label_data.
看起来是全黑的，然而读到像素中，是可以看到对相同类别的文件进行标注了。 
我这里用matlab，进行突出对比度显示为： 
![这里写图片描述](https://img-blog.csdn.net/20170903162412831?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2h3YW5fbWE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
标注已经完成！
可能需要批量转换的话，就需要自己写shell 脚本会方便一些。
