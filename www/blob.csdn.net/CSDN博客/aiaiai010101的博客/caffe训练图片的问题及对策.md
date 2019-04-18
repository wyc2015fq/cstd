# caffe训练图片的问题及对策 - aiaiai010101的博客 - CSDN博客

2017年03月27日 22:09:51[aiaiai010101](https://me.csdn.net/aiaiai010101)阅读数：230
个人分类：[计算机视觉																[机器学习](https://blog.csdn.net/aiaiai010101/article/category/6764564)](https://blog.csdn.net/aiaiai010101/article/category/6764555)


这段时间一直在试图用caffe训练图像，今天终于成功了一次，虽然结果很差，不过毕竟能跑起来了。

特此记录一下caffe训练过程中的种种问题，及解决办法。

1、caffe安装中遇到的问题

(1)安装caffe的过程中最初libcaffe和testcaffe总是加载失败，网上百度一搜，给出的解决方案居然是重启电脑，卧槽，重启之后，果然。。。没有鸟用。。。

这个问题我已经忘记是怎么解决的了，但是让我发现了遇到问题，还是google靠得住，连接VPN，上了google搜索了解决方案，然后成功了。

(2)编译caffe的过程中老是出现各种warnning，当然也有少量error，前者可以在项目设置中进行设置，即不将warnning视作error来解决，后者。。。嗯。。。继续google。。。

error中很可能碰到的一个问题是电脑是否支持GPU，开始我以为我的电脑支持，结果频频报错，后来上了NVIDA官网，里边可以查询各种型号的GPU支持哪些技术，嗯，我这一台很显然属于最差的那款GPU，虽然官网上显示支持CUDNN，但是在别处查到CUDNN需要3.0以上计算能力的GPU(这个可能与CUDA的版本有关)，我这台计算能力只有渣到不行的2.1，因此在caffe的配置文件中关闭了CUDNN，果然不再报错。

在安装过程中，务必要注意配置文件中的CUDA等要与自己安装的版本一致，不一致的要在配置文件中改动。

2、caffe训练图片中遇到的问题

caffe训练图片的网上教程很多，这里不再赘述，我只说一下其中可能遇到的问题。

**训练过程中遇到的问题极大可能是因为路径设置不对！！！**

**训练过程中遇到的问题极大可能是因为路径设置不对！！！**

**训练过程中遇到的问题极大可能是因为路径设置不对！！！**

(1)把图片制作成lmdb或者leveldb过程中遇到的问题

开始以为数据制作很简单，写个bat，调用convert_imageset.exe就可以了。没错，确实很简单，我也很注意在bat中的路径了，但是依然一直报错，说“A of 0 images”，很明显，没有找到图片。我改了很久，甚至重新编译了convert_imageset.exe，依然不行，最后很偶然的解决了。

我来说一下，convert_imageset.exe需要的参数中，其中有两个参数是图片路径和label.txt，注意是label.txt，不是label.txt的路径，为什么，因为label.txt要和convert_imageset.exe放在同一个目录下，而不是放在train文件夹中。

正确的文件放置位置是:caffe-master(caffe的根目录)下放置convert_imageset.exe和label.txt，然后在data文件夹(也在caffe-master下)中放置train文件夹，train文件夹中放置4个文件夹，对应4类图像，比如设置cat、dog、frog、bird四个文件夹，每个文件夹中放置一类图片数据。

然后是bat，图片路径写data\train\，然后写label.txt(直接label.txt，不用再加任何路径)，label.txt中格式应该是:dog\000001.jpg，cat\00012.jpg...也就是图片名字前要加文件夹名字，因为这样前边的data\train\才能和dog\000001.jpg连接起来，构成图片的路径全写，正确读取图片。

之前我把label.txt放在train文件夹中，然后在bat文件中写了label.txt的全路径，结果总是显示“A of 0 images”。

(2)caffe训练过程中遇到的问题

写完solver.prototxt和train_val.prototxt之后，开始训练，结果老是报错，读取solver.prototxt失败，主要是解析字符失败。

还是路径问题，为了防止路径错误，我在solver.prototxt和train_val.prototxt中每次都写了绝对路径，结果还是不行。最后解决方法出人意料地简单，把路径中的‘\’改成'/'或者‘\\’，因为C++中，读取字符串的时候，‘\’是作为转义字符读取的，所以路径中的‘\’必须写成'/'或者‘\\’。

之前在bat中写路径的时候，网上教程有很多说要写反斜杠，而不是斜杠，但是我在实践中发现，没什么区别，所以一直不太重视斜杠与反斜杠。但是C++不一样，涉及到转义字符，在写路径的时候必须注意这个问题。

