# ubuntu14.04+cuda8.0+cudnn5.1+torch7配置整理 - 工作笔记 - CSDN博客





2017年02月14日 15:40:26[Eason.wxd](https://me.csdn.net/App_12062011)阅读数：7858








实际上教程网上有很多，这里说下自己遇到的几个坑。

很少有人提到（包括torch7官方）CUDA与torch的安装顺序，我这里测试发现先安装torch再cuda，总是各种问题，所以，先cuda,cudnn，再torch,一路顺利。这应该跟opencv与cuda关系类似。跟torch相关的CUDA实现，要重新编译，因此，先CUDA比较保险。

另外就是网络要好，否则经常断！感谢同事提供的VPN。。

还有一个，双显卡笔记本一定要注意，安装CUDA时，提示的OpenGL库安装问题。

**一.NVIDIA 驱动安装 **

**(之前以为按网上方法安装成功就没事啦，结果重启发现进不了桌面系统，所以，这里在更新一份CUDA7.5的文章，我自己按照此方式安装8。0 OK)**

**转自：http://blog.csdn.net/masa_fish/article/details/51882183 非常感谢博主。一定要认真读完。**



本篇文章是基于之前安装CUDA 7.5的经验写的，但因为最近更新TensorFlow 到了r0.12版本，官方提示该版本TensorFlow 和CUDA 8.0 & cudnnv5.1 配合的最好，所以又卸了CUDA 7.5，重新装了一遍CUDA 8.0，安装的过程中发现了之前文档里的些许不足，于是做了稍许修改。

PS：本来想在7.5的基础上更新到8.0，但是失败了。还把系统搞崩了，又重装了一遍Ubuntu，唉，说多了都是泪啊

CUDA7.5和CUDA8.0的安装过程是一毛一样的。所以如果安装CUDA8.0的同学，直接将下文中的所有7.5替换为8.0即可。

折腾了好多天，前后重装了大概六、七次Ubuntu，终于上把CUDA安装好了，被坑了好多次，也走了不少弯路。

第一次发帖，还请多指教。

【环境】

笔记本：ThinkPad T450 **X86_64**

显卡：主显 Intel HD Graphics 5500 ； 独显 NVIDIA GT 940M

系统环境：Ubuntu14.04 64位、Windows7 64位 双系统

CUDA 版本： 7.5

其他：Ubuntu中不存在已经安装的CUDA

先说说自己遇到的大坑吧，安装完CUDA之后总是遇到登录界面循环问题：**输入密码后又跳回密码输入界面**。尝试了网上的多种说法无解，最终发现我的问题是出在**双显卡**这里。最后是在官方安装文档中找到了答案。

**问题解决方案：**

在安装cuda时，会出现提示，询问你是否需要安装 **openGL Libraries**。如果你的电脑是双显，而且用来显示的那块GPU不是NVIDIA，则OpenGL Libraries就不应该安装，否则不是NVIDIA的那块GPU使用的OpenGL Libraries会被覆盖，然后GUI就无法工作了。

## **先谈谈关于安装CUDA的几点感受吧**

关于 CUDA的安装，网上有特别多的安装帖子，我在安装的时候也参考了很多。你会发现网上很多的安装帖子，一般只做十几个操作就完事了。但是CUDA的官方安装文档却非常冗长，有四十多页，别觉得它是废话，其实都是干货。

网上的安装帖子之所以精炼，是因为它只涵盖了必须的安装步骤，跳过了许多的检查操作（例如：**检查电脑中是否已安装CUDA需要依赖的软件、是否已经卸载会与CUDA产生冲突的软件**）等。如果你足够幸运，电脑里上恰好该有的有，不该有的没有，那这么做没问题，但通常情况下你并没有那么走运。

CUDA这个东西需要依赖的、牵着的东西特别多，如果没有做好检查，则在安装时候特别容易出现各种各样的冲突，且一旦冲突发生，即使卸载CUDA也不能解决问题，只能重装系统。

我最后就是参考CUDA的官方文档安装成功的。

## **总结一下，我的安装建议是**：

1) **一定要下一份CUDA官方的安装文档，按照它的步骤一步步慢慢来，不可偷懒**。
[CUDA 7.5 官方安装文档下载](http://developer.download.nvidia.com/compute/cuda/7.5/Prod/docs/sidebar/CUDA_Installation_Guide_Linux.pdf)

2) **在安装之前一定要详细检查自己的系统环境、软件是否符合CUDA的安装要求。不要存有侥幸心理，跳过检查步骤**。

3) **每进行一项操作，都检查一下该项操作是否成功**。

下面分享一下我的安装步骤。基本跟CUDA的官方安装文档的操作是一致的，不同的地方都有特别注明。

**安装CUDA主要分三大环节。**

一、安装前的环境准备和检查

二、安装CUDA

三、安装完的校验。

## **1、安装Ubuntu系统14.04**

新安装完的14.04系统会提示很多更新项。

网上有很多帖子说不建议更新，说更新过后会导致安装CUDA发生系统界面循环登录的问题。

但是我更新后，并没有遇到任何问题。

## **2、检查自己的电脑环境是否具备安装CUDA的条件**

**a) 检查自己的GPU是否是CUDA-capable**

在终端中输入： `$ lspci | grep -i nvidia` ，会显示自己的NVIDIA GPU版本信息

去CUDA的官网查看自己的GPU版本是否在CUDA的支持列表中

**b) 检查自己的[Linux](http://lib.csdn.net/base/linux)版本是否支持 CUDA(Ubuntu 14.04没问题)**

**c) 检查自己的系统中是否装了gcc**

在终端中输入： `$gcc –version` 可以查看自己的gcc版本信息

**d) 检查是否安装了kernel header和 package development**

在终端中输入： `$uname –r` 可以查看自己的kernel版本信息

在终端中输入：`$ sudo apt-get install linux-headers-$(uname -r)`

可以安装对应kernel版本的kernel header和package development

以上检查我的电脑系统都满足要求，如果没有满足要求的话，可以参考cuda的官方文档，里面有详细的针对每个问题的解决方案。

## **3、 选择安装方式**

CUDA提供两种安装方式：**package manager**安装和**runfile**安装

我本来选的是 package manager 安装，这种方法相对简单，但尝试了几次都失败。后来是转换到runfile安装才成功的。**因此此处只介绍runfile安装方式。**

下载cuda安装包：cuda官网下载，根据系统信息选择对应的版本，runfile安装的话最后一项要选择 runfile文件

[CUDA 7.5 下载链接](https://developer.nvidia.com/cuda-downloads)

## **4、runfile安装cuda**

**a) 禁用 nouveau**

终端中运行：`$ lsmod | grep nouveau`，如果有输出则代表nouveau正在加载。

Ubuntu的nouveau禁用方法：

在`/etc/modprobe.d`中创建文件`blacklist-nouveau.conf`，在文件中输入一下内容

```
blacklist nouveau
options nouveau modeset=0
```

打开终端，运行
`$ sudo update-initramfs –u`
设置完毕可以再次运行 `$ lsmod | grep nouveau` 检查是否禁用成功，如果运行后没有任何输出，则代表禁用成功。

**b) 重启电脑，到达登录界面时，alt+ctrl+f1，进入text mode，登录账户**

**c) 输入 `$ sudo service lightdm stop` 关闭图形化界面**

**d) 切换到cuda安装文件的路径，运行`$ sudo sh cuda_7.5.18_linux.run`**

按照提示一步步操作

**遇到提示是否安装openGL ，选择no（如果你的电脑跟我一样是双显，且主显是非NVIDIA的GPU需要选择no，否则可以yes）**

其他都选择yes或者默认

安装成功后，会显示**installed**，否则会显示**failed**。

**e) 输入 `$ sudo service lightdm start` 重新启动图形化界面。**

**Alt + ctrl +F7**，返回到图形化登录界面，输入密码登录。

如果能够成功登录，则表示不会遇到循环登录的问题，基本说明CUDA的安装成功了。

**f) 重启电脑。检查Device Node Verification**。

检查路径`/dev`下 有无存在名为`nvidia*`（以nvidia开头）的多个文件(device files)

如果没有的话，可以参考官方文档里的指导步骤，进行添加。

**g) 设置环境变量。**

终端中输入 `$ sudo gedit /etc/profile`

在打开的文件末尾，添加以下两行。

```
$ export PATH=/usr/local/cuda-7.5/bin:$PATH
$ export LD_LIBRARY_PATH=/usr/local/cuda7.5/lib64
```

**保存文件。**

**这里有点与官方安装文档稍有不同，需要说明：**

官方文档里说只需在终端中运行上述两条export语句即可，但如果不将它们不写入`/etc/profile`文件的话，这样的环境变量在你退出终端后就消失了，不起作用了，所以写入才是永久的做法。

**h) 重启电脑，检查上述的环境变量是否设置成功。**

终端中输入 : `$ env`

在输出的环境变量中检查有无上述 g) 中设置的变量，如果有则代表设置成功。

到此为止，CUDA的安装算是告一段落了。为了保险起见，建议进行下述的检查工作，确保真正的安装成功。

## **5、 安装完毕后的检查工作。**

**a) 检查 NVIDIA Driver是否安装成功**

终端输入 ：`$ cat /proc/driver/nvidia/version` 会输出NVIDIA Driver的版本号

**b) 检查 CUDA Toolkit是否安装成功**

终端输入 ： `$ nvcc –V` 会输出CUDA的版本信息

**c) 尝试编译cuda提供的例子**

切换到例子存放的路径，默认路径是 `~/NVIDIA_CUDA-7.5_Samples`

（即 `/home/xxx/ NVIDIA_CUDA-7.5_Samples`， xxx是你自己的用户名）

然后终端输入：`$ make`

如果出现错误的话，则会立即报错停止，否则会开始进入编译阶段。

我的第一次运行时出现了报错，提示的错误信息是系统中没有gcc

然后在终端运行 `$ sudo apt-get install gcc` 安装完gcc后 再make就正常了

整个编译的时间持续比较长，耐心等待，大概十几分钟是需要的。

**d) 运行编译生成的二进制文件。**

编译后的二进制文件 默认存放在`~/NVIDIA_CUDA-7.5_Samples/bin中。`

切换路径 ： `cd ~/NVIDIA_CUDA-7.5_Samples/bin/x86_64/linux/release`

终端输入 ：`$ ./deviceQuery`

看到类似如下图片中的显示，则代表CUDA安装且配置成功（congratulation！！）

![这里写图片描述](https://img-blog.csdn.net/20160711165507583)

再检查一下系统和CUDA-Capable device的连接情况

终端输入 ： `$ ./bandwidthTest`

看到类似如下图片中的显示，则代表成功
![这里写图片描述](https://img-blog.csdn.net/20160711165558838)



**二.安装cudnn5.0（5.1也一样）**

安装cuDNN比较简单，解压后把相应的文件拷贝到对应的CUDA目录下即可：

tar -zxvf cudnn-8.0-linux-x64-v5.0-ga.tgz



显示以下信息：

*cuda/include/cudnn.h

cuda/lib64/libcudnn.so

cuda/lib64/libcudnn.so.5

cuda/lib64/libcudnn.so.5.0.5

cuda/lib64/libcudnn_static.a*

继续执行以下指令：

sudo cp cuda/include/cudnn.h /usr/local/cuda/include/

sudo cp cuda/lib64/libcudnn* /usr/local/cuda/lib64/

sudo chmod a+r /usr/local/cuda/include/cudnn.h

sudo chmod a+r /usr/local/cuda/lib64/libcudnn*

8) 验证一下，哈哈哈哈哈啊哈哈哈哈哈

cuda的samples 里面有个deviceQuery 运行之后会显示信息，最后一行出行pass说明成功啦～～～～

下面是这位同学的：http://blog.csdn.net/hungryof/article/details/51557666,主要是安装Torch7,cuda，cudnn部分可以不看。



三个前提：

1. ubuntu别太老，最好14.04或以上吧，本人采用14.04

2. cuda别太老，本人试过cuda7.5和cuda8.0。都完美运行。

3. cudnn一定要和cuda版本对应。这是最关键的。比如我以前cuda是7.5的，即cuda-repo-ubuntu1404-7-5-local_7.5-18_amd64，采用的cudnn就要cudnn-7.5-[Linux](http://lib.csdn.net/base/linux)-x64-v5.0-ga.tgz，是为cuda7.5准备的cudnn5.0版本。 现在装的是cuda8.0，即cuda-repo-ubuntu1404-8-0-local_8.0.44-1_amd64.deb。采用的是

cudnn-8.0-linux-x64-v5.1.tgz。

安装与测试

用一个例子来跑，从而验证torch以及cuda和cudnn是否安装成功。

我推荐这个。

参考：链接：[https://github.com/jcjohnson/neural-style/blob/master/INSTALL.md](https://github.com/jcjohnson/neural-style/blob/master/INSTALL.md)
[neural-style](http://blog.csdn.net/hungryof/article/details/16:58:45.254)

其实这篇已经讲的很清楚怎么安装了，但是安装时还是碰到了不少坑。

稍微翻译一下吧，顺便加点小解释：

**三.安装torch7**

```bash
cd ~/
curl -s https://raw.githubusercontent.com/torch/ezinstall/master/install-deps | bash
git clone https://github.com/torch/distro.git ~/torch --recursive
cd ~/torch; ./install.sh
```

这个稍微注意一下，这里的

```bash
curl -s https://raw.githubusercontent.com/torch/ezinstall/master/install-deps | bash
```

是安装torch的依赖库，和官方的版本不是一样的。如果你运行这个出现**版本依赖问题**从而没法安装的话，这时候采用下面的官方版本

```bash
git clone https://github.com/torch/distro.git ~/torch --recursive
cd ~/torch; bash install-deps;
./install.sh
```

这里可能会出现的问题，在 **坑一**中。

接下来，它会提示是否吧torch加入bashrc中，有”….(yes|no)”提示，输入yes，即可。

为了保险，可以看看bashrc文件

```bash
vim ~/.bashrc
```

查看文档末尾是不是有类似

```bash
./home/xxx/torch/install/bin/torch-activate
```

上面是你torch安装的路径。

然后跟新一下环境变量。

```bash
source ~/.bashrc
```

现在来说，基本的torch就安装好了！！！就是这么简单。但是你可能还想装cuda，其实也很简单。

## 下载配置neural style（这个只是进行用代码测试torch是否安装好了。）

安装其他依赖库

```bash
sudo apt-get install libprotobuf-dev protobuf-compiler
luarocks install loadcaffe
```

下载neural style代码

```bash
cd ~/
git clone https://github.com/jcjohnson/neural-style.git
cd neural-style
```

安装VGG模型

```bash
sh models/download_models.sh

CPU版本的测试
th neural_style.lua -gpu -1 -print_iter 1
```

若出现

```bash
[libprotobuf WARNING google/protobuf/io/coded_stream.cc:505] Reading dangerously large protocol message.  If the message turns out to be larger than 1073741824 bytes, parsing will be halted for security reasons.  To increase the limit (or to disable these warnings), see CodedInputStream::SetTotalBytesLimit() in google/protobuf/io/coded_stream.h.
[libprotobuf WARNING google/protobuf/io/coded_stream.cc:78] The total number of bytes read was 574671192
Successfully loaded models/VGG_ILSVRC_19_layers.caffemodel
conv1_1: 64 3 3 3
conv1_2: 64 64 3 3
conv2_1: 128 64 3 3
conv2_2: 128 128 3 3
conv3_1: 256 128 3 3
conv3_2: 256 256 3 3
conv3_3: 256 256 3 3
conv3_4: 256 256 3 3
conv4_1: 512 256 3 3
conv4_2: 512 512 3 3
conv4_3: 512 512 3 3
conv4_4: 512 512 3 3
conv5_1: 512 512 3 3
conv5_2: 512 512 3 3
conv5_3: 512 512 3 3
conv5_4: 512 512 3 3
fc6: 1 1 25088 4096
fc7: 1 1 4096 4096
fc8: 1 1 4096 1000
WARNING: Skipping content loss  
Iteration 1 / 1000  
  Content 1 loss: 2091178.593750    
  Style 1 loss: 30021.292114    
  Style 2 loss: 700349.560547   
  Style 3 loss: 153033.203125
```

则安装成功。表示如果这一步都到不了的话，难度挺大。

## 安装cuda

这个如果你以前装过就不用再装了，其实就是个环境嘛，默认是安装在/usr/local/cuda-8.0的。安装的时候它会创建一个软链接，类似windows的快捷方式。

直接从官网上下载对应版本的[cuda](https://developer.nvidia.com/cuda-downloads) 。然后deb文件直接双击安装。你别以为这个deb安装完了，cuda就安装好了，其实还有附加的库，也是要安装的。这些要通过命令行来进行。

```bash
sudo apt-get update
sudo apt-get install cuda
```

上面的apt-get安装的cuda会根据你的deb的cuda的版本的不同，而安装相应的库。比如你如果是采用cuda8.0的deb，那么此时上面会出现一堆cuda8.0-之类的文件名的库。

最后再试试是否安装好了cuda

```bash
nvidia-smi
```

出现你的显卡配置信息的话，就ok。

## 安装torch的cuda支持

刚才装的cuda是通用的，是所有的程序框架都可以用的啊。但是你让torch用cuda的话，还要安装2个库cutorch和cunn。其实torch安装相应的其他库还是很简单的，直接一条命令搞定。

```bash
luarocks install cutorch
luarocks install cunn
```

[测试](http://lib.csdn.net/base/softwaretest)一下

```bash
th -e "require 'cutorch'; require 'cunn'; print(cutorch)"
```

此时到这里应该还是妥妥的.

## 安装cudnn

其实cudnn就是一些链接库，怎么安装呢。把cudnn的头文件放入和相应的链接库放入cuda路径的相应位置就行了。显然头文件是放入include文件夹中，而链接库是放入lib64文件夹中。因此有

```bash
tar -xzvf  cudnn-8.0-linux-x64-v5.1.tgz
sudo cp cuda/lib64/libcudnn* /usr/local/cuda-8.0/lib64/
sudo cp cuda/include/cudnn.h /usr/local/cuda-8.0/include/
```

最后在安装torch的cudnn支持

```bash
luarocks install cudnn
```

测试一下：

```bash
th neural_style.lua -gpu 0 -backend cudnn
```

几乎是百分百成功的，妥妥的，可能会出现**坑二**。

# 坑1：torch的依赖库很多！！
`curl -s https://raw.githubusercontent.com/torch/ezinstall/master/install-deps | bash`
运行这个时，**一定会经过较长时间的安装！！！！**由于我这里的网很差，所以如果你的也有类似的情况，那么可能会出现：“xxx 校验和不符”。这时说明完全没有安装依赖库好吧！！我以前以为已经装好了，直接下完neural-style，然后./install.sh。我擦，结果出现什么cmake not found之类的。然后我还傻乎乎的去 sudo apt-get install cmake。结果又出现其他乱七八糟的，现在就是一句话：**curl -s[https://raw.githubusercontent.com/torch/ezinstall/master/install-deps](https://raw.githubusercontent.com/torch/ezinstall/master/install-deps) | bash**是把所有的依赖库都会安装好！！并且安装完之后会有类似提示：“torch dependencies have already installed.”

# 坑2 可能出现’libcudnn not found in library path’的情况

截取其中一段错误信息：

```
Please install CuDNN from https://developer.nvidia.com/cuDNN
Then make sure files named as libcudnn.so.5 or libcudnn.5.dylib are placed in your library load path (for example /usr/local/lib , or manually add a path to LD_LIBRARY_PATH)
```

LD_LIBRARY_PATH是该环境变量，主要用于指定查找共享库（动态链接库）时除了默认路径之外的其他路径。由于刚才已经将

“libcudnn*”复制到了/usr/local/cuda-8.0/lib64/下面，因此需要
- sudo gedit /etc/ld.so.conf.d/cudnn.conf 就是新建一个conf文件。名字随便
- 加入刚才的路径/usr/local/cuda-8.0/lib64/
- 反正我还添加了/usr/local/cuda-8.0/include/,这个估计不要也行。
- 保存后，再sudo ldconfig来更新缓存。（可能会出现libcudnn.so.5不是符号连接的问题，不过无所谓了！！）

此时运行
`th neural_style.lua -gpu 0 -backend cudnn`
成功了！！！！
![](https://img-blog.csdn.net/20160601173426091)

发现用cudnn时，变成50个50个一显示了，速度快了些。刚才但存用cuda只是1个1个显示的。不说了，歇会儿。

# 总结

一定要版本对应！！cuda和cudnn的版本一定要对应，对应！！！！

# 四.HDF5安装

torch中如果直接

```bash
luarocks install hdf5
```

是会出错的。要这样：

```bash
sudo apt-get install libhdf5-serial-dev hdf5-tools
git clone https://github.com/deepmind/torch-hdf5
cd torch-hdf5
luarocks make hdf5-0-0.rockspec LIBHDF5_LIBDIR="/usr/lib/x86_64-linux-gnu/"
```

具体的一些用法参照
[DeepMind](https://github.com/deepmind/torch-hdf5/blob/master/doc/usage.md)

**补充Torch IDE**

**五.安装Torch能用的IDE**

Torch能用的IDE是基于ZeroBrane Studio 和 MobDebug开发的一个zbs-torch IDE，github的网址是：https://github.com/soumith/zbs-torch。

按照github中提供的方法是：



**[python]**[view plain](http://blog.csdn.net/linmingan/article/details/51039017#)[copy](http://blog.csdn.net/linmingan/article/details/51039017#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)
- luarocks install mobdebug #安装调试工具  

这个工具torch提供的工具包是没有的，请自己网上下载。安装的过程会提示需要依赖库，请根据提示自行下载。

**[plain]**[view plain](http://blog.csdn.net/linmingan/article/details/51039017#)[copy](http://blog.csdn.net/linmingan/article/details/51039017#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)
- $ git clone https://github.com/soumith/zbs-torch.git  
- $ cd zbs-torch  
- $ ./zbstudio.sh  

这样就安装完zbs-torch了。接下来是把torch加入zbs-torch中去。

首先是在IDE的菜单栏中进行如下操作：Project->Start Debugger Server，然后是：Project->Lua Interpreter->Torch-7。

最后要调试的时候在代码中加入：require('mobdebug').start()，点击绿色的运行按钮开始调试程序。

**调试操作说明：**

**![](https://img-blog.csdn.net/20160401194722470?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)**

图中左边第一个按钮是进入函数单步调试（F10），第三个按钮是不进入函数单步调试（shift+F10），第四个是运行到光标处。

更新：

有时候在interpreter中找不到torch7选项，只要在zbs中的packages目录加入torch7.lua文件（在此下载：[https://github.com/pkulchenko/ZeroBranePackage/blob/master/torch7.lua](https://github.com/pkulchenko/ZeroBranePackage/blob/master/torch7.lua)）





