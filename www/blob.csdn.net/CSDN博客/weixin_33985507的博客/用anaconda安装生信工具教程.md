# 用anaconda安装生信工具教程 - weixin_33985507的博客 - CSDN博客
2018年10月02日 21:22:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：17
> 
本贴出自徐州更老师的《生信必修课之软件安装》[https://ke.qq.com/course/310838](https://ke.qq.com/course/310838)。仅做学习交流，不做商业用途，侵删。
- 打开终端
- 转到src文件夹下
> 
cd ~/src
- 打开清华大学开源镜像网站 [https://mirrors.tuna.tsinghua.edu.cn](https://mirrors.tuna.tsinghua.edu.cn) -> 获取下载链接
![9640232-6d4a5e70b52799a8.png](https://upload-images.jianshu.io/upload_images/9640232-6d4a5e70b52799a8.png)

![9640232-f3c155867f8ba721.png](https://upload-images.jianshu.io/upload_images/9640232-f3c155867f8ba721.png)
右键复制链接[https://mirrors.tuna.tsinghua.edu.cn/anaconda/miniconda/Miniconda3-4.5.11-MacOSX-x86_64.sh](https://mirrors.tuna.tsinghua.edu.cn/anaconda/miniconda/Miniconda3-4.5.11-MacOSX-x86_64.sh)
注：miniconda就可以满足我们的需求，anaconda略大
- 在终端中输入
> 
wget -m [https://mirrors.tuna.tsinghua.edu.cn/anaconda/archive/Anaconda3-5.3.0-Linux-x86.sh](https://mirrors.tuna.tsinghua.edu.cn/anaconda/archive/Anaconda3-5.3.0-Linux-x86.sh)
此时可能会出现报错：Miniconda3-4.5.11-MacOSX-x86_64.sh: Permission denied
因为src需要更高的管理权限，如何解决呢？
> 
sudo chmod -R 777 ～/src
777是最高管理权限了。
- 安装下载的sh文件
> 
bash Miniconda3-4.5.11-MacOSX-x86_64.sh
- 激活环境
> 
source ~/miniconda3/bin/activate
- 还需要安装的仓库镜像
> 
conda config --add channels [https://mirrors.tuna.tsinghua.edu.cn/anaconda/pkgs/free/](https://mirrors.tuna.tsinghua.edu.cn/anaconda/pkgs/free/)
conda config --add channels [https://mirrors.tuna.tsinghua.edu.cn/anaconda/pkgs/main/](https://mirrors.tuna.tsinghua.edu.cn/anaconda/pkgs/main/)
conda config --set show_channel_urls yes
conda config --add channels [https://mirrors.tuna.tsinghua.edu.cn/anaconda/cloud/conda-forge/](https://mirrors.tuna.tsinghua.edu.cn/anaconda/cloud/conda-forge/)
conda config --add channels [https://mirrors.tuna.tsinghua.edu.cn/anaconda/cloud/bioconda/](https://mirrors.tuna.tsinghua.edu.cn/anaconda/cloud/bioconda/)
安完后，通过vim ~/.condarc来查看有哪些仓库镜像
- 根据jimmy老师的帖子，常用的工具有：macs2, bwa, samtools, fastqc, htseq, sratools
使用
> 
conda search macs2
来查看该包是否存在，以及版本情况。
如果需要依赖python2，则需要在python2的环境下进行安装，否则会将3强制降级成2
> 
conda create -n python2 python=2
conda activate python2
若想离开，则
> 
source deactivate
安装：
> 
conda install macs2
