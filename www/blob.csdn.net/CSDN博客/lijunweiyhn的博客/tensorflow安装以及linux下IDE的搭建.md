# tensorflow安装以及linux下IDE的搭建 - lijunweiyhn的博客 - CSDN博客





置顶2016年05月30日 15:10:33[_Junwei](https://me.csdn.net/lijunweiyhn)阅读数：8367








TensorFlow是谷歌基于DistBelief进行研发的第二代人工智能学习系统，其命名来源于本身的运行原理。Tensor（张量）意味着N维数组，Flow（流）意味着基于数据流图的计算，TensorFlow为张量从图象的一端流动到另一端计算过程。TensorFlow是将复杂的数据结构传输至人工智能神经网中进行分析和处理过程的系统。

TensorFlow可被用于语音识别或图像识别等多项机器深度学习领域，对2011年开发的深度学习基础架构DistBelief进行了各方面的改进，它可在小到一部智能手机、大到数千台数据中心服务器的各种设备上运行。TensorFlow将完全开源，任何人都可以用。



TensorFlow 表达了高层次的机器学习计算，大幅简化了第一代系统，并且具备更好的灵活性和可延展性。TensorFlow一大亮点是支持异构设备分布式计算，它能够在各个平台上自动运行模型，从电话、单个CPU / GPU到成百上千GPU卡组成的分布式系统。

从目前的文档看，TensorFlow支持CNN、RNN和LSTM算法，这都是目前在Image，Speech和NLP最流行的深度神经网络模型。

--------------------------------------------------------------

经过2天的捣鼓，终于把这个并不怎么复杂的东西装上了。

我给出安装服务器的电脑配置：

CPU：E5-2620@2.10GHz 

显卡：Telsa K20C 两块

服务器内存：64G

操作系统：Ubuntu15.04

python版本：2.7.9

JDK:   1.7

-----------------------------------------------------------------

先说说tensorflow，最新的tensorflow版本为0.8.0，这个是要在英文TensorFlow社区才能看到的，中文社区网站更新是写0.5.0，在这里建议安装0.8.0，因为0.5.0很多函数用不了。

安装的方式有很多，建议使用的是pip或则Virtualenv安装。前者很方便，后者用的独立的依赖，不会跟一些东西发生冲突。

先给出英文社区网站地址：

https://www.tensorflow.org/versions/r0.8/get_started/os_setup.html#download-and-setup




  我使用的是pip安装。因为python2版本有许多库函数，相对于之后的版本来说有着更方便的函数调用，所以在版本选择时候，我选择的python2的pip安装。这里分为CPU和GPU的版本，如果电脑较差可以选择CPU，如果是实验室服务器或则比较壕家用服务器，可以选择GPU版本。在这里要注意的是，如果选择的是GPU版本，需要安装CUDA toolkit7.5 和CuDNN v4（在英文网站里这两个可以直接下载，而且安装方法在官网里有注明）。

在联网状态下，在服务器中输入：sudo pip install --upgrade https://storage.googleapis.com/tensorflow/linux/gpu/tensorflow-0.8.0-cp27-none-linux_x86_64.whl

开始下载。再次提醒要安装CUDA toolkit7.5和CuDNN v4，CuDNN用最新的，CUDA用7以上版本。




----------------------------------------------------------------

下面说下IDE搭建。对于python来说有许多开发平台供人们选择使用，虽然记事本可以进行程序编写工作，但是有个开发平台提供错误检查和编译，并且有更舒服的视觉外观更能让人接受。我用的是Eclipse下配置的python环境。首先需要搞到的是Eclipse客户端，这个官网即可下载：https://www.eclipse.org/

我使用的是Eclipse IDE for Java EE Developers，200多MB，速度也很快一会就下完了，Eclipse不需要安装，在Ubuntu环境下解压即可使用，建好自己的工作空间进入编写个Java语言试试jdk是否安装成功。或则在命令中输入：java -version即可。

接下来是配置python环境，Eclipse下并不支持python，所以需要配置。配置python只需要安装一个Pydev插件即可。

网上搜索“Linux环境下Eclipse配置python”会发现有很多方法，大致是这样的，打开Eclipse，然后照下面两图进行在线下载pydev。

![](https://img-blog.csdn.net/20160530154152987?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


![](https://img-blog.csdn.net/20160530154215799?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





  然后你发现当你连上网速度很慢装了半天之后，你以为你成功了，可以配置路径然后开始写python代码开始deep learning的时候，发现配置路径时，在Window->Preferences设置里面左边栏里找不到Pydev这一项。我以为是Eclipse问题，结果下了3,4个不同的Eclipse，结果都有这问题，在这里说一下，3.8以上版本即可。这个问题发生的原因是因为pydev版本太高，而我们的python版本太低导致的，也就是说版本不匹配，就无法显示。

建议照着http://www.linuxidc.com/Linux/2015-02/113883.htm先试试，我试了，但是并没有什么用。如果你的eclipse可以照着这方法解决，那是最好了。如果不行，我们需要手动下载pydev。

pydev下载地址:https://sourceforge.net/projects/pydev/files/pydev/

我使用的python版本是2.7.9，我下载的Pydev版本是Pydev2.7.5，下载回来后，解压，发现有两个包：1.features  2. plugins  ，这两个包eclipse根目录下也有，把这两个包里面的文件分别拷贝到eclipse目录下覆盖。重启eclipse就会发现pydev有了，然后按照http://www.linuxidc.com/Linux/2014-08/105073.htm后续操作设置路径。这里再提醒一点好了，系统本来的python也许也是2.7.x，但是我们装好tensorflow后发现直接输入python进行编辑会报错，这里直接输入python是调用的系统的python，我们安装tensorflow时安在后续安装的python里的，调用python时，记得输入/usr/bin/python，这个是我安装的路径。当然了，配置eclipse用可视化的编程方法就没有这个问题了。




配置完成后，在官网上随便找一段代码敲进去，编译试试就好。这个安装配置的网站也很多，任何问题只需baidu一下就能找到，这里只是做一个总结方便以后自己安装。









