# 教你从头到尾利用DL学梵高作画：GTX 1070 cuda 8.0 tensorflow gpu版 - 结构之法 算法之道 - CSDN博客





2016年09月25日 11:58:11[v_JULY_v](https://me.csdn.net/v_JULY_v)阅读数：46995
个人分类：[30.Machine L & Deep Learning](https://blog.csdn.net/v_JULY_v/article/category/1061301)

所属专栏：[机器学习十大算法系列](https://blog.csdn.net/column/details/datamining.html)









> 
> 
> 
> 
## 教你从头到尾利用DL学梵高作画











作者：七月在线开发/市场团队中的三人，骁哲、李伟、July。本教程主要由骁哲撰写，李伟负责Linux命令部分。
时间：二零一六年九月二十五日
配置：GTX 1070 cuda 8.0 Ubuntu 14.04 cudnn 5.1 tensorflow gpu
交流：TensorFlow实战交流Q群 472899334，有问题可以加此群共同交流。另探究实验背后原理，请参看我们的深度学习在线课程：[**深度学习在线班**](https://www.julyedu.com/category/index/8/21)。








## 一、前言

    12年本博客推出SVM三层境界，14年July团队开始做机器学习线下班，也因此写了一系列ML博客。15年，July团队正式创业，开始做[七月在线](https://www.julyedu.com/)—— 国内领先的人工智能教育平台，最初涵盖算法、机器学习、深度学习三大重心。16年，七月在线从上半年的5个课程扩展到下半年的30个课程，讲师团队也从上半年的5人扩大至现有的19人（且到18年上半年，讲师团队已超过60人）。

    与此同时，机器学习越来越火，且随着计算机计算能力的提高深度学习则更趋火热。公司在做深度学习的课程时，开发/市场同事也会顺带着学习学习，但从始至终都尚未跟着课程上的实验实际动手做过。

    直到最近，我们团队人手渐渐齐了之后，发现可以动手做一些实验了。为何呢？学一个东西，懂它理论之后，最重要的是付诸实践，动手实验一把，不然永远只是纸上谈兵。**我们三虽然都不讲课（一市场一PHP一打杂），但可以辅助讲师团队带动更多学员一起做实验**、一起搞。

    激情澎湃，心血来潮。
- 21号下午5点，同事李、许开始搞DL：用DL学梵高作画。恩，是搞DL 不是搞ML。
- 接近6点的时候，开始卷积计算，等待的过程就像生孩子一样。
- 晚上8点，CPU搞了一个半小时搞出来了，然后开始装cuda，好用GPU搞几副梵高笔下的画。
- 0点，同事还在装，因为GTX 1070下装cuda8.0实在是费劲呐（1070+8.0之所以难装，一个是网上教程很少，一个是1070 8.0都是刚出来的，特别是1070目前只能用cuda 8.0。可能960/970/980搭cuda 7.5会轻松太多）。
- 接下来，两同事在黑暗中前行了一夜，趟遍无数坑。
- 22号早上6点半，cuda、TensorFlow终于编译好，我..
- 22号早上8点，用DL学梵高作画，GTX 1070 cuda 8.0 Ubuntu 14.04 cudnn 5.1 tensorflow gpu，折腾一整夜，趟遍无数坑，两位同事一PHP一市场终于搞定。（恩，有人说，PHP是世界上最好的语言）。


    我们搭建好之后，兴高采烈的在微博上发布（因为两同事此前从未搞过DL，可以想象我们心情之兴奋），发现咱5月DL班里一学员之前也搭过这个配置，说：“装cuda 和驱动花了几天，经常出现黑屏”。也有朋友反馈：“配置这玩意确实很麻烦。”、以及“是真的很麻烦，我自己尝试搭建就一直没成功”。所以，很多朋友求教程。

    事实上，GPU搭好后，最后跑下demo就几分钟的事，主要是整个搭建过程 很折腾。不信？你可以先不看本教程，然后自个从头到尾搭一把1070+8.0，没搭过1070+8.0之前就不要来轻易下结论了。真心话。

    综上，特此推出本份教程，是网上极少有的1070+8.0教程里面最详尽的一份（部分图都是由骁哲用手机对着电脑拍下），希望让更多人少走弯路。此外，我们更会在[深度学习课程](https://www.julyedu.com/category/index/8/21)上带领更多人跟我们一块做DL实验，一起玩。







## 二、配置



我们的配置为
- 硬件配置：酷睿i7-6700+GTX1070+500G固态硬盘+8G内存+主板
- 软件配置：Ubuntu14.04 +GTX1070显卡驱动+CUDA8.0+cudnn5.1+Tensorflow







## 三、下载



考虑到有些同学Linux操作不是很熟练，以下步骤尽量详细，以让每一个人都能玩一把。另，操作系统为全新安装的Ubuntu14.04英文桌面版，如果已安装其它版本或非全新安装，步骤可相应调整。

### **3.1、 Ubuntu14.04**

[http://www.ubuntu.com/download/alternative-downloads](http://www.ubuntu.com/download/alternative-downloads)（下载地址，选择64位下载）

> 
> 
> 
![](https://img-blog.csdn.net/20160925150305399)




说明：


- 我们自己下载的时候是在官方英文版页面下载，中文版网页面似乎没有Ubuntu14.04的下载页。
- Ubuntu14.04版本选择上建议使用英文原版，Kylin（麒麟：给中国用户特别定制）版本不建议使用。原因：呵呵，外来和尚会念经
- Ubuntu16.04版本我们测试后发现进入桌面会屏幕闪动，分析原因似乎跟我们的显卡是1070版本有关（其实只要把GTX1070显卡驱动装上就行），而Ubuntu14.04未出现以上状况。所以考虑为广大人民群众减少麻烦，当您的显卡与我们类似（GTX1070\GTX1080），请参考本条建议。



UltraISO软碟通：

[http://cn.ultraiso.net/xiazai.html](http://cn.ultraiso.net/xiazai.html)（下载地址）

说明：


- 这个软件是将你U盘制作成Ubuntu14.04安装盘的工具。
- 使用方法请查看百度文库相关说明：[http://wenku.baidu.com/link?url=XIitpKr9kKSXLLBzhrO7DzCOgGtrqpvxyfnI8tt3ugnt59dEWzMwUAUzMy-mIyY1gDeqaOPkKMB5EwlWYCwWZjaq2CaLiZzWpENTpgk04SG](http://wenku.baidu.com/link?url=XIitpKr9kKSXLLBzhrO7DzCOgGtrqpvxyfnI8tt3ugnt59dEWzMwUAUzMy-mIyY1gDeqaOPkKMB5EwlWYCwWZjaq2CaLiZzWpENTpgk04SG)
- 如果下载地址链接失效，自行百度：UltraISO软碟通。
- 安装是在Windows下安装的。



### **3.2、 GTX1070显卡驱动**

[http://www.geforce.cn/drivers](http://www.geforce.cn/drivers)（下载地址）

说明：

（1）选项图解

![](https://img-blog.csdn.net/20160925123324266)![](https://img-blog.csdn.net/20160925123351503)

（2）大家可以根据自己电脑配置进行选择。

### **3.3、 CUDA**

[https://developer.nvidia.com/cuda-toolkit](https://developer.nvidia.com/cuda-toolkit)（下载地址）

说明：

（1）在NVIDIA的CUDA下载页面下，选择要使用的CUDA版本进行下载。

（2）我们这里使用CUDA8.0（页面有提示GTX1070、GTX1080支持8.0版本），学员如果没有使用以上两个版本的GPU，可以下载CUDA7.5。DOWNLOAD（下载）。

（3）下载需要注册。

（4）图解选择

![](https://img-blog.csdn.net/20160925123706270)![](https://img-blog.csdn.net/20160925123720864)



### 3.4、 Cudnn

[https://developer.nvidia.com/cudnn](https://developer.nvidia.com/cudnn)（下载地址）

说明：

（1）下载需要填写一个调查问卷，就三个选项，建议认真填写，毕竟人家免费给咱使用。

（2）填写完毕点击**I Agree To**前面的小方框，出现如下：

![](https://img-blog.csdn.net/20160925123840755)

    点击下载后出现很多个选择，勾选3个选项：Images、Image Classification、Tensorflow。




### 3.5、 Tensorflow

tensorflow github上面提到 4 种安装方式，本教程使用 第四种 源码安装

- Virtualenv installation
- Anaconda installation
- Docker installation
- Installing from sources

[https://github.com/tensorflow/tensorflow](https://github.com/tensorflow/tensorflow)（下载地址）

说明：

（1）打开下载页面，往下翻，直到下图这个位置：

![](https://img-blog.csdn.net/20160925123911677)

（2） 点击Python 2开始下载。

最后，将所有下载文件全部存放至自己的移动硬盘/U盘内，等待安装时候使用。







## 四、 安装

### 4.1、 安装Ubuntu14.04



安装Ubuntu14.04：

[http://jingyan.baidu.com/article/eb9f7b6d8536a8869364e813.html](http://jingyan.baidu.com/article/eb9f7b6d8536a8869364e813.html)

说明：

（1）我们直接安装的英文原版系统，语言也是选择英文的。

（2）上述链接在--第三步：安装类型上选择的是--自定义。我们选择的是--清除整个磁盘并且安装，如果你有Windows系统，还会提示安装Ubuntu14.04与Windows并存模式。这个自行选择**，切记！**这个地方谨慎选择。

（3）感谢百度经验上传者！

### 4.2、 安装GTX1070显卡驱动及CUDA8.0



注：安装驱动需要在字符界面下安装



步骤：

（**1**）进入Ubuntu界面

（**2**）插入U盘，将内容拷贝至Desktop文件下。熟悉Ubuntu的同学，这步可以按照自己习惯放置在其他文件夹下。

（**3**）如图搜索Terminal

![](https://img-blog.csdn.net/20160925124336280)


（**4**） 将命令框图标拖动到左边栏里或者桌面，以便使用。

（**5**） 打开命令框，输入 sudo  –i

输入开机密码

输入  sudo apt-get install vim


           sudo  vi   /etc/default/grub

![](https://img-blog.csdn.net/20160925124430947)


（**6**） 此时系统会进入一个文本页面。在第20行前后会出现GRUB_CMDLINE_LINUX_DEFAULT="quiet"。将光标移动到这个命令最前端，然后键盘点击Esc键，再点击y键两次，点一下p键。就会将本条指令复制一个放到下一行。然后点击i键，就能进行编辑了，在本行命令前面添加#号，然后移动到复制出来的那条，修改quiet为text，按Esc键。Shift+：输入wq。

![](https://img-blog.csdn.net/20160925124506892)

				此时会调回命令框界面，输入sudo update-grub2

输入shutdown  –r  now重启

（**7**） 重启后进入字符界面，按照提示输入你的用户名及密码

![](https://img-blog.csdn.net/20160925124544963)


输入sudo  -i

输入密码

输入cd  /home/***(这个按Tab键自动匹配)/Desktop

输入 ./NVIDIA-LIN…(这个按Tab键自动匹配)或  /bin/bash 文件名

				不懂的看图如下

![](https://img-blog.csdn.net/20160925124702191)


按回车键后出现

…………………………………………………………………………………………………………………………………………………………………………………………………………………………….

说明开始安装

然后选择Accept等等同意字样

期间遇到协议的时候一直按空格键就行

协议完成后提示如图

![](https://img-blog.csdn.net/20160925124608010)


输入accept

然后一路同意就行

（**8**） 安装NVIDIA结束后，同样操作安装CUDA8.0，这里需要注意其中一个地方需要填写N才行，不能写Y。如图

![](https://img-blog.csdn.net/20160925124744011)


				其余操作如图

![](https://img-blog.csdn.net/20160925124805364)


然后回车等待就行

（**9**） 类似刚刚第（5）步的执行

输入sudo  vi   /etc/default/grub此时系统会进入一个文本页面。在第20行前后会出现GRUB_CMDLINE_LINUX_DEFAULT="quiet"。将光标移动到这个命令最前端，然后键盘点击Esc键，再点击y键两次，点一下p键。就会将本条指令复制一个放到下一行。然后点击i键，就能进行编辑了，在本行命令前面删除#号，然后移动到复制出来的那条，命令前面添加#号，按Esc键。Shift+：输入wq。

				此时会调回命令框界面，输入sudo update-grub

输入shutdown  –r  now重启

### 4.3、 Cudnn安装

cd  /home/***(自己的用户名)/Desktop/###(这个命令意思是找到刚刚我们用U盘传过来的文件)

tar xvzf cudnn-8.0-linux-x64-v5.1-ga.tgz###（解压这个文件）

sudo cp cuda/include/cudnn.h /usr/local/cuda/include###（复制）

sudo cp cuda/lib64/libcudnn* /usr/local/cuda/lib64###（复制）

sudochmoda+r/usr/local/cuda/include/cudnn.h/usr/local/cuda/lib64/libcudnn*



### 4.4、 其他依赖安装：

[https://github.com/tensorflow/tensorflow/blob/master/tensorflow/g3doc/get_started/os_setup.md](https://github.com/tensorflow/tensorflow/blob/master/tensorflow/g3doc/get_started/os_setup.md)

我们是在github的Tensorflow官方网页上，根据提示安装，地址如上。

按步骤截图如下

sudo apt-get install python-pip python-dev（这个不完整，完整版如下）

sudo apt-get install python-pip python-dev Python-scipy Pythoy-numpy git





![](https://img-blog.csdn.net/20160925125329199)


![](https://img-blog.csdn.net/20160925125351465)


![](https://img-blog.csdn.net/20160925125423294)


### 4.5、 Bazel安装

由于本教程使用tensorflow源码编译/安装，所以需要使用 bazel build。

链接：[https://www.bazel.io/versions/master/docs/install.html](https://www.bazel.io/versions/master/docs/install.html)

![](https://img-blog.csdn.net/20160925125611796)


![](https://img-blog.csdn.net/20160925125638545)

自动跳转

![](https://img-blog.csdn.net/20160925125656594)


![](https://img-blog.csdn.net/20160925125719030)


之后回到之前的Tensorflow安装教程页面：[https://github.com/tensorflow/tensorflow/blob/master/tensorflow/g3doc/get_started/os_setup.md](https://github.com/tensorflow/tensorflow/blob/master/tensorflow/g3doc/get_started/os_setup.md)

![](https://img-blog.csdn.net/20160925125803108)


### 4.6、 numpy安装

[http://www.scipy.org/scipylib/download.html](http://www.scipy.org/scipylib/download.html)

![](https://img-blog.csdn.net/20160925125819093)


git clone git://github.com/numpy/numpy.git numpy




### 4.7、 Tensorflow安装

还是刚刚的网址

[https://github.com/tensorflow/tensorflow/blob/master/tensorflow/g3doc/get_started/os_setup.md](https://github.com/tensorflow/tensorflow/blob/master/tensorflow/g3doc/get_started/os_setup.md)

![](https://img-blog.csdn.net/20160925125918738)


./configure



如果configure 失败 尝试执行一下两个命令
export LD_LIBRARY_PATH="$LD_LIBRARY_PATH:/usr/local/cuda/lib64:/usr/local/cuda/extras/CUPTI/lib64"
export CUDA_HOME=/usr/local/cuda

![](https://img-blog.csdn.net/20160925125943379)


bazel build -c opt //tensorflow/tools/pip_package:build_pip_package

bazel build -c opt --config=cuda //tensorflow/tools/pip_package:build_pip_package

bazel-bin/tensorflow/tools/pip_package/build_pip_package /tmp/tensorflow_pkg

sudo pip install /home/***（你自己的用户名）/Desktop/tensorflow-0.10.0-cp2-none-any.whl

![](https://img-blog.csdn.net/20160925130037037)


bazel build -c opt //tensorflow/tools/pip_package:build_pip_package



# To build with GPU support:

bazel build -c opt --config=cuda //tensorflow/tools/pip_package:build_pip_package



mkdir _python_build

cd _python_build

ln -s ../bazel-bin/tensorflow/tools/pip_package/build_pip_package.runfiles/org_tensorflow/*.

ln -s ../tensorflow/tools/pip_package/*.

python setup.py develop









## **五、 测试Tensorflow**

![](https://img-blog.csdn.net/20160925130116266)![](https://img-blog.csdn.net/20160925131516729)







## 六、 实验：模仿梵高绘画

neural-style下载
在这个github网站下载相应代码：[https://github.com/anishathalye/neural-style](https://github.com/anishathalye/neural-style)


![](https://img-blog.csdn.net/20160925130136773)


接下来，下载vgg19：
http://www.vlfeat.org/matconvnet/models/beta16/imagenet-vgg-verydeep-19.mat


然后，把vgg放到neural-style的文件夹根目录下


> 
> ![](https://img-blog.csdn.net/20160926103414723)




现在，在命令窗口cd  /home/***(你自己的用户名)/Desktop/

		   git clone  https://github.com/anishathalye/neural-style.git 

接着你就能在桌面打开这个文件夹了

![](https://img-blog.csdn.net/20160925130203375)


把你想修改的照片放入example文件夹内，然后在刚才的窗口命令接着输入：

python   neural_style.py   --content  ./example/***.jpg(此括号内不要复制：***代表你想要使用的图片名称)   --styles  ./example/ 1-style.jpg(此括号内不要复制：1-style.jpg是梵高星空图片在文件夹内名称，你也可以换其他的风格，改名称就行了) --output  ./example/$$$.jpg(此括号内不要复制：$$$代表你想要生成的图片名称)

例如我的输入如下python neural_style.py –content ./example/1-content.jpg --styles./example/1-style.jpg--output ./example/1-output.jpg




接下来，就是见证奇迹的时刻。

在计算机学到梵高作品星空的风格之后，让计算机对指定图片进行梵高式处理，如下图所示

![](https://img-blog.csdn.net/20160925131943564)![](https://img-blog.csdn.net/20160925132006509)


之前用CPU得一个半小时才能出来结果的，现在用GPU几分钟即可出结果。不枉费劲心思折腾GPU。








## 后记

    两位同事此前从未做过任何DL实验，仅拼着一腔热情、兴趣、热爱做这个实验，刚做完时觉得很麻烦，因为很多坑网上都没提过、全凭在黑暗中摸石头过河，且前行整整一夜。但相信有此教程后，1070+8.0则不再麻烦，可能一两个小时便能搞定（因为后来我们一下午连搞三遍，一遍比一遍快）。动嘴容易、动手方知没那么容易，但真正推动社会前行的永远是实干家，而非评论家。

    这个实验只是开始，我们开发/市场团队还会辅助讲师团队带动更多学员朋友做一系列更多实验，比如char-rnn、WaveNet（用卷积神经网络模拟人的声音）等等，哪怕实验再简单我们也会去做，因为：学计算机/ML/DL，第一要领就是动手、就是实验，否则说再多都是纸上谈兵。

    创造价值、帮助众人。贴两张图，是为念。最后探究实验背后原理，请参看此课程：[深度学习班](https://www.julyedu.com/category/index/8/21)。

![](https://img-blog.csdn.net/20160926235646881)![](https://img-blog.csdn.net/20160928231654406)




    七月在线开发/市场团队骁哲、李伟、July，二零一六年九月二十五日。



