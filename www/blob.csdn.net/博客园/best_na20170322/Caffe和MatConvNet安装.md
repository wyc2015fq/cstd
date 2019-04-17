# Caffe和MatConvNet安装 - best_na20170322 - 博客园




- [博客园](https://www.cnblogs.com/)
- [首页](https://www.cnblogs.com/believe-in-me/)
- [新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)
- [联系](https://msg.cnblogs.com/send/best_na20170322)
- [管理](https://i.cnblogs.com/)
- [订阅](https://www.cnblogs.com/believe-in-me/rss)![订阅](https://www.cnblogs.com/images/xml.gif)





# [Caffe和MatConvNet安装](https://www.cnblogs.com/believe-in-me/p/6655115.html)





转载：http://www.linuxdiyf.com/linux/22847.html

一、Caffe（CPU Only）+Ubuntu14.04

要求：**Ubuntu系统，且必须是14.04版本，其他版本在配置过程中，可能会报错。如果不想出错，用14.04版本（32位、64位均可，推荐64位）。

我配置时，完全是参考Caffe官网上的流程，上面的流程十分简单明了，一遍成功。下面是网址：http://caffe.berkeleyvision.org/install_apt.html



1、安装依赖库

sudo apt-get install libprotobuf-dev libleveldb-dev libsnappy-dev libopencv-dev libhdf5-serial-dev protobuf-compiler

sudo apt-get install --no-install-recommends libboost-all-dev



2、BLAS

sudo apt-get install libatlas-base-dev



3、依赖库

sudo apt-get install libgflags-dev libgoogle-glog-dev liblmdb-dev



4:（可选，不配置也是可以跑caffe训练 的），安装OpenCV，切记，Ubuntu14.04系统，一定要下载OpenCV2.4.9，否则配置时很容易出错。



5、下载Caffe并编译。这里没有配置Python和Matlab接口，如想配置这两个借口，用Python和Matlab调用Caffe，请参考官网。（个人感觉在服务器上没必要配，Python和Matlab接口，Caffe本身的.cpp和.prototxt代码写的十分清晰明了)。

首先：

我是在用户主目录下执行的下面操作（非root用户），不要告诉我不会切到用户的主目录下面嗷~（命令：cd ~）

git clone https://github.com/BVLC/caffe.git

cd caffe

cp Makefile.config.example Makefile.config

然后：修改Makefile.config文件内容，将“# CPU_ONLY := 1”中的#号去掉，即可，其他的都不用改。

最后：

make all

（也可以 make all -j4 开启4个进程，开启进程数不要过多，最好看看自己CPU是几核的）

make test （或 make test -j4

make runtest （或make runtest -j4)



6、测试

如果安装、编译不出错，那么配置成功，跑一个例子看看。

首先进入Caffe目录

cd ~/caffe

./data/mnist/get_mnist.sh (或 sh data/mnist/get_mnist.sh）

./examples/mnist/create_mnist.sh(或 sh examples/mnist/create_mnist.sh)

vi examples/mnist/lenet_solver.prototxt

%修改里面的solver_mode为CPU

./examples/mnist/train_lenet.sh（或 sh /examples/mnist/train_lenet.sh）

尽情的跑吧。



二、MatConvNet+Windows+Matlab2014b+VC2013

MatConvNet配置GPU还是相对简单的，CPU更加简单了。这里还是CPU版本的，去官网下载最新版。

http://www.vlfeat.org/matconvnet/

或者，用我上传的这个压缩包（一样是在官网下的）

http://download.csdn.net/detail/zxc024000/9589881

首先，请配置Matlab2014b+VS2013编译环境，使用 mex -setup，设置编译器为VS2013

下载后，解压，我这里文件夹名叫“matconvnet-master”

打开Matlab2014b，工作目录切换到该文件夹下，新建 “Compile.m”,内容如下：

addpath matlab

vl_compilenn

运行该文件，即可编译CPU版的MatConvNet，编译好后。运行

matconvnet-master\examples\mnist 中的cnn_mnist.m，

该程序会自动从网上下载训练数据，下载好后，自动进行训练。训练后，会得到一个训练好的网络，利用Matlab的save命令，将该结果保存为.mat文件，以后就可以用利用MatConvNet提供的函数，调用这个.mat文件，进行分类了。下图是在训练中，全部样本训练第二次迭代中。

![](http://www.linuxdiyf.com/linux/uploads/allimg/160730/2-160I0102T5606.jpg)



一个更简单的例子，

在该网址上下载，imagenet-vgg-f.mat。

http://www.vlfeat.org/matconvnet/models/

切换到matconvnet-master目录下，新建demo.m,内容如下：

run matlab/vl_setupnn

net=load('../models/imagenet-vgg-f.mat');%换成模型存储的位置

im=imread('peppers.jpg');%图片自己搜，推荐百度一张企鹅图片，测试一下

im_=single(im);

im_=imresize(im_,net.meta.normalization.imageSize(1:2));

im_=im_-net.meta.normalization.averageImage;

res=vl_simplenn(net,im_);

y=res(end).x;

x=gather(res(end).x);

scores=squeeze(gather(res(end).x));

[bestScore,best]=max(scores);

figure(1);

clf;

imshow(im);

title(sprintf('%s %d,%.3f',...

net.meta.classes.description{best},best,bestScore));

![](http://www.linuxdiyf.com/linux/uploads/allimg/160730/2-160I0102UK05.jpg)

上图将目标正确 分类，类别为946，得分为0.704.



Caffe和MatConvNet包含许多知识，最好深入学习一下，才能更好的使用。












