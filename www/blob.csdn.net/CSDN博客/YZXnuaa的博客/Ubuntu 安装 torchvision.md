# Ubuntu 安装 torchvision - YZXnuaa的博客 - CSDN博客
2018年06月06日 23:28:36[YZXnuaa](https://me.csdn.net/YZXnuaa)阅读数：1166
pytorch是torch移植到python上的版本，作为python3的第三方库，我们首先想到的就是利用pip来进行安装，但是直接使用
pip3 install pytorch  （因为我使用的是python3，因此此处为pip3），会报如下错误：
![Ubuntu 17.04安装pytorch and pytorchvision（采用源码安装）](http://www.linuxdiyf.com/linux/uploads/allimg/170612/2-1F612151J5S7.jpg)
提示我们要去pytorch的官网进行下载，打开官网，可以按照安装方式（pip ,conda,source）和python版本，cuda版本来进行安装，由于本国特殊的网络国情，一般是打不开，或者是无法查看 run the command 之后的指令的。
![Ubuntu 17.04安装pytorch and pytorchvision（采用源码安装）](http://www.linuxdiyf.com/linux/uploads/allimg/170612/2-1F612151K5609.jpg)
在题主进行科学上网以后，得到了利用pip进行安装的相关的指令，（该指令为linux,pip,python3.6,none-cuda）
pip install http://download.pytorch.org/whl/cu75/torch-0.1.12.post2-cp35-cp35m-linux_x86_64.whl （linux下，即使cuda选择none，还是默认安装cuda75版本）
pip install torchvision
却发现torchvision已经不再支持pip，pytorch可以正常使用
![Ubuntu 17.04安装pytorch and pytorchvision（采用源码安装）](http://www.linuxdiyf.com/linux/uploads/allimg/170612/2-1F612151PA95.jpg)
看提示，建议我们选择miniconda进行安装，于是乎转战conda，由于我们学校提供了官方的镜像库，所以我选择直接安装了anaconda，如何在ubuntu上安装anaconda有很多文章可以参考，可以之前前往  https://www.continuum.io/downloads/  进行安装。
利用conda进行安装的指令如下：
cuda7.5 
conda install pytorch torchvision -c soumith
cuda8.0
conda install pytorch torchvision cuda80 -c soumith
然而！利用conda进行安装时会报condaHttpError错误，起初我认为是我国特殊的网络环境造成的，因此，将conda的源切换为我们学校的镜像源，仍然会报HttoError,但是安装别的第三方库可以成功。此时我是绝望的。这2个第三方包管理器太不靠谱，我在starkoverflow和github上找了一下，有很多人碰到了类似的问题，也有一些回答，但经过我的尝试，没有一个能解决我的问题。
![Ubuntu 17.04安装pytorch and pytorchvision（采用源码安装）](http://www.linuxdiyf.com/linux/uploads/allimg/170612/2-1F612151QT58.jpg)
最终解决方法----源码编译
源码包在github上的托管地址为
https://github.com/pytorch/vision
https://github.com/pytorch/pytorch
分别克隆2个包的源代码进行安装：
git clone git@github.com:pytorch/pytorch.git
git clone git@github.com:pytorch/vision.git
然后，cd到克隆的目录下
cd pytorch
python3 setup.py install
cd vision
python3 setup.py install
注意，若安装时提示找不到cmake，直接sudo apt install cmake 即可
安装完成之后，进入ipython3，查看是否安装成功（直接在python命令行查看也可）
![Ubuntu 17.04安装pytorch and pytorchvision（采用源码安装）](http://www.linuxdiyf.com/linux/uploads/allimg/170612/2-1F612151Ra13.jpg)
之后就可以畅游pytorch和pytorchvision了。
