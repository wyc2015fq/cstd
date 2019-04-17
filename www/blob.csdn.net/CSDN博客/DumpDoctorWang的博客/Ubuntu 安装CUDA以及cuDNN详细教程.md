# Ubuntu 安装CUDA以及cuDNN详细教程 - DumpDoctorWang的博客 - CSDN博客





2018年09月02日 19:11:59[原我归来是少年](https://me.csdn.net/DumpDoctorWang)阅读数：341
个人分类：[OpenCV与CUDA混合编程																[CUDA](https://blog.csdn.net/DumpDoctorWang/article/category/7308303)](https://blog.csdn.net/DumpDoctorWang/article/category/7812660)








如果要安装带CUDA的OpenCV，先按照本篇博客安装CUDA，然后参考我另一篇博客[Ubuntu 编译OpenCV](https://blog.csdn.net/DumpDoctorWang/article/details/82259357)安装OpenCV。

本文以Ubuntu 18.04+CUDA 9.0+cuDNN 7.4.2为例，说明安装过程。

# 一、安装驱动
- 添加驱动的源。打开终端，输入：

```bash
sudo add-apt-repository ppa:graphics-drivers/ppa
sudo apt-get update
```

 这一步的目的是添加Ubuntu显卡驱动的源，然后才能从“软件和更新”中看新的到显卡驱动。如果是20系显卡或者要安装CUDA 10，必须执行上面的步骤。
- 打开“系统设置(System Settings)”-->“软件和更新(Software & Updates)”

![](https://img-blog.csdn.net/20180902173142935?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0R1bXBEb2N0b3JXYW5n/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
- 选择“附加驱动(Addtional Drivers)”，等待几分钟，出现如下界面

![](https://img-blog.csdnimg.cn/20190411215411985.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0R1bXBEb2N0b3JXYW5n,size_16,color_FFFFFF,t_70)
- 然后勾选含有“nvidia-driver-390”那一项，点击“应用更改（Apply Changes）”，然后输入密码，等待半小时左右就可以完成驱动的安装。如果要安装CUDA 10，要选含有“nvidia-driver-418”那一项。

> 
带有“open source（开源）”字样的就是开源显卡驱动，都是在执行完第一步添加驱动的源之后才有的；带有“proprietary（专有）”字样的是专有显卡驱动，对于10系显卡，不管是否执行第一步都是有的，对于20系显卡，是没有的。“390”和“418”等字样是显卡驱动版本号。

- 驱动安装完成之后重启电脑，就会自动使用NVIDIA驱动。

# 二、下载CUDA安装文件

**对于10系显卡，搞深度学习的话推荐安装CUDA 9.0+cuDNN 7.4.2。**
- 如何确认CUDA需要的最低显卡驱动版本号

> 
本文通过.run文件来安装CUDA。下载的CUDA文件命名方式都类似于“cuda_CUDA版本号_显卡驱动版本号_linux.run”；比如CUDA 9.0的文件名就是“cuda_9.0.176_384.81_linux.run”，其中“9.0.176”就是CUDA的版本号，“384.81”就是显卡驱动的版本号。因此“384.81”就是CUDA 9需要的显卡驱动的最低版本。


> 
10系显卡在ubuntu 16.04上的专有驱动版本号是384，在ubuntu 18.04上的专有显卡驱动的版本号是390；故Ubuntu 16.04安装384版本的显卡驱动就能支持CUDA 9.0，ubuntu 18.04的专有显卡版本是390，虽然比384.81高，但也是兼容CUDA 9.0的。对于20系显卡，一般选择418版本的开源显卡驱动就行。

- CUDA下载汇总地址

[https://developer.nvidia.com/cuda-toolkit-archive](https://developer.nvidia.com/cuda-toolkit-archive)
- CUDA 9.0下载地址

[https://developer.nvidia.com/cuda-90-download-archive](https://developer.nvidia.com/cuda-90-download-archive)

进入下载页面后，做如下选择
![](https://img-blog.csdnimg.cn/20190331171157264.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0R1bXBEb2N0b3JXYW5n,size_16,color_FFFFFF,t_70)

需要说明的是，虽然CUDA 9.0没有为ubuntu 18.04编译，但是也是可以用的，只需要把gcc的版本降级到5.5就行。然后把“Base Installer”和“Parch1”～“Parch4”下载下来：

![](https://img-blog.csdnimg.cn/20190331171304140.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0R1bXBEb2N0b3JXYW5n,size_16,color_FFFFFF,t_70)

# 三、安装CUDA
- 降级gcc为gcc-5（Ubuntu 16.04不需要降级）

安装gcc-5以及g++-5

```bash
sudo apt-get install -y gcc-5 g++-5
```

通过update-alternatives管理gcc版本：

```bash
sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-5 1 --slave /usr/bin/g++ g++ /usr/bin/g++-5
sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-7 0 --slave /usr/bin/g++ g++ /usr/bin/g++-7
```

 选择gcc版本：

```bash
sudo update-alternatives --config gcc
```

运行上面的命令之后，出现如下选择界面：

![](https://img-blog.csdnimg.cn/20190412010921580.png)

 在“selection number”后面输入usr/bin/gcc-5前面的序号2，再敲回车即可。检查gcc以及g++版本是否是5.5：

```bash
gcc --version
g++ --version
```
- 安装依赖项

```bash
sudo apt-get install mesa-common-dev freeglut3-dev
```
- 添加执行权限

```
chmod +x cuda_9.0.176_384.81_linux.run
chmod +x cuda_9.0.176.1_linux.run
chmod +x cuda_9.0.176.2_linux.run
chmod +x cuda_9.0.176.3_linux.run
chmod +x cuda_9.0.176.4_linux.run
```
-  安装CUDA 9.0

打开终端，输入
`sudo ./cuda_9.0.176_384.81_linux.run`
然后按Ctrl+C， 输入“accept”接受协议；**若提示"**You are attempting to install on an unsupported configuration. Do you wish to continue?**"，输入“y”就行；当询问你是否安装驱动的时候，要输入"n"，因为之前就安装过驱动了**；主要的选项如下图

![](https://img-blog.csdnimg.cn/20190331175902963.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0R1bXBEb2N0b3JXYW5n,size_16,color_FFFFFF,t_70)

然后依次安装补丁包

```
sudo ./cuda_9.0.176.1_linux.run
sudo ./cuda_9.0.176.2_linux.run
sudo ./cuda_9.0.176.3_linux.run
sudo ./cuda_9.0.176.4_linux.run
```
- 添加环境变量

打开终端，输入”gedit ~/.bashrc“，然后再弹出的编辑器的最后，添加如下两行

```
export PATH=/usr/local/cuda/bin:$PATH
export LD_LIBRARY_PATH=/usr/local/cuda/lib64:$LD_LIBRARY_PATH
```

保存退出。
- 测试CUDA

进入你的CUDA Example所在目录，默认是主目录，找到“NVIDIA_CUDA-8.0_Samples”或者“NVIDIA_CUDA-9.0_Samples”。依次打开“1_Utilities”-->"deviceQuery"，然后打开终端输入

```
# 使.bashrc的环境变量生效，重启之后或者打开新终端就不用再执行这一行了
source ~/.bashrc
# 修改文件权限
sudo chmod -R 777 *
# 编译
make
# 运行
./deviceQuery
```

然后出现”Result = PASS“字样时，说明安装成功了。



# 四、安装cuDNN
- 下载地址

[https://developer.nvidia.com/rdp/cudnn-archive](https://developer.nvidia.com/rdp/cudnn-archive)

**下载的时候要选择和安装的CUDA版本匹配的cuDNN。**

![](https://img-blog.csdnimg.cn/20190331164739847.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0R1bXBEb2N0b3JXYW5n,size_16,color_FFFFFF,t_70)

然后下载“cuDNN Library for Linux”那一项：

![](https://img-blog.csdnimg.cn/20190331165412184.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0R1bXBEb2N0b3JXYW5n,size_16,color_FFFFFF,t_70)
- 安装方法

以CUDA 9.0+cuDNN 7.4.2为例：

```bash
# 解压
tar zxvf cudnn-9.0-linux-x64-v7.4.2.24.tgz
# 复制cudnn头文件
sudo cp cuda/include/* /usr/local/cuda-9.0/include/
# 复制cudnn的库
sudo cp cuda/lib64/* /usr/local/cuda-9.0/lib64/

# 添加可执行权限
sudo chmod +x /usr/local/cuda-9.0/include/cudnn.h
sudo chmod +x /usr/local/cuda-9.0/lib64/libcudnn*
```

安装过程实际上是把cudnn的头文件复制到CUDA的头文件目录里面去；把cuDNN的库复制到CUDA的库目录里面去。

# 五、安装NCCL（可选）

NCCL是NVDIA多显卡通信库，是可选安装的。
- 下载地址

[https://developer.nvidia.com/nccl/nccl-legacy-downloads](https://developer.nvidia.com/nccl/nccl-legacy-downloads)

下载"[Download NCCL v2.3.7, for CUDA 9.0, Nov 8 & Dec 14, 2018](https://developer.nvidia.com/nccl/nccl-legacy-downloads#a-collapse237-90)"->“O/S agnostic local installer”

![](https://img-blog.csdnimg.cn/20190417100832303.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0R1bXBEb2N0b3JXYW5n,size_16,color_FFFFFF,t_70)
- 安装方法

以CUDA 9.0+NCCL 2.3.7为例：

```bash
# 解压
tar Jxvf nccl_2.3.7-1+cuda9.0_x86_64.txz
cd nccl_2.3.7-1+cuda9.0_x86_64/
# 复制到CUDA目录
sudo cp include/nccl.h /usr/local/cuda-9.0/include/
sudo cp lib/* /usr/local/cuda-9.0/lib64/
# 添加执行权限
sudo chmod +x /usr/local/cuda-9.0/include/nccl.h 
sudo chmod +x /usr/local/cuda-9.0/lib64/libnccl*
```

就是把NCCL的头文件和库文件复制到对应的目录里面去。




