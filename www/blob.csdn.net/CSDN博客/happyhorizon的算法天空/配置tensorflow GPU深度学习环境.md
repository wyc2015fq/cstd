# 配置tensorflow GPU深度学习环境 - happyhorizon的算法天空 - CSDN博客
2017年09月08日 14:09:42[lxy_Alex](https://me.csdn.net/happyhorizion)阅读数：545

# Cuda安装
安装cuda是比较麻烦的一步，以下安装说明来自cuda的安装说明文件。遇到问题找官方解决方案，最便捷有效。
Perform the following steps to install CUDA and verify the installation.
Disable the Nouveau drivers:首先需要屏蔽ubuntu默认的显卡驱动
- Create a file at /etc/modprobe.d/blacklist-nouveau.conf with the
following contents:
```
blacklist nouveau
options nouveau modeset=0
```
- Regenerate the kernel initramfs: 更新一下
`$ sudo update-initramfs -u`
- Reboot into runlevel 3 by temporarily adding the number "3" and the word
"nomodeset" to the end of the system's kernel boot parameters.
重新开机，输入 sudo init 3 切换到文本模式
- Run the installer silently to install with the default selections (implies acceptance of
the EULA):然后按ctrl+alt+F1打开tty1.将当前路径切换到NVIDA驱动安装文件的文件夹下，输入：
`sudo sh cuda_<version>_linux.run --silent`
具体来说，为了保证驱动安装成功，需要首先关闭桌面图形环境：
sudo service lightdm stop
之后安装cuda驱动：sudo sh cuda_8.0.61_375.26_linux.run
4.Create an xorg.conf file to use the NVIDIA GPU for display:
等安装完成后输入，创建NVIDIA GPU的配置文件：
`$ sudo nvidia-xconfig`
5.Reboot the system to load the graphical interface.
输入reboot重新启动，进入到图形用户界面，也就是默认桌面
- Set up the development environment by modifying the PATH and
LD_LIBRARY_PATH variables: 在启动配置文件（/etc/profile）中加入如下内容
```
$ export PATH=/usr/local/cuda-8.0/bin${PATH:+:${PATH}}
$ export LD_LIBRARY_PATH=/usr/local/cuda-8.0/lib64\
${LD_LIBRARY_PATH:+:${LD_LIBRARY_PATH}}
```
- Install a writable copy of the samples then build and run the nbody sample:
测试显卡是否安装成功，在终端输入如下内容，跑一个例子。
```
cuda-install-samples-8.0.sh ~
cd ~/NVIDIA_CUDA-8.0_Samples/5_Simulations/nbody
make
./nbody
```
# tensorflow安装
1、首先安装Anacoda，下载，然后按照提示安装。
2、在安装tensorflow GPU版本之前，需要安装cudnn。在NVIDIA网站可以找到cudnn，这个工具包没有包括在CUDA中，下载后解压。解压后打开，里面有两个文件夹：include和lib64.在终端输入：
```
sudo cp include/cudnn.h /usr/local/cuda_8.0/include
sudo cp lib64/lib* /usr/local/cuda_8.0/lib64
sudo ldconfig -v
```
3、安装tensorflow GPU版本。在终端输入
```
conda create -n tensorflow 
source activate tensorflow
pip install --ignore-installed --upgrade https://storage.googleis.com/tensorflow/linux/[tensorflow版本].whl
```
安装完后输入ipython
加载tensorflow包，即可。
