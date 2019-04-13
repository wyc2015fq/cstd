
# caffe+GPU︱AWS.G2+Ubuntu14.04+GPU+CUDA8.0+cudnn8.0 - 素质云笔记-Recorder... - CSDN博客

2017年02月21日 18:51:12[悟乙己](https://me.csdn.net/sinat_26917383)阅读数：2504标签：[ubuntu																](https://so.csdn.net/so/search/s.do?q=ubuntu&t=blog)[gpu																](https://so.csdn.net/so/search/s.do?q=gpu&t=blog)[cuda																](https://so.csdn.net/so/search/s.do?q=cuda&t=blog)[AWS																](https://so.csdn.net/so/search/s.do?q=AWS&t=blog)[caffe																](https://so.csdn.net/so/search/s.do?q=caffe&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=AWS&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=cuda&t=blog)个人分类：[图像︱caffe																](https://blog.csdn.net/sinat_26917383/article/category/6666555)
[
																								](https://so.csdn.net/so/search/s.do?q=cuda&t=blog)
[
				](https://so.csdn.net/so/search/s.do?q=gpu&t=blog)
[
			](https://so.csdn.net/so/search/s.do?q=gpu&t=blog)
[
		](https://so.csdn.net/so/search/s.do?q=ubuntu&t=blog)

国服亚马逊的GPU实例G2.2xlarge的python+caffe的安装过程，被虐…  一周才装出来…
> BVLC/caffe的在AWS安装的官方教程github：

> [https://github.com/BVLC/caffe/wiki/Install-Caffe-on-EC2-from-scratch-(Ubuntu,-CUDA-7,-cuDNN-3)](https://github.com/BVLC/caffe/wiki/Install-Caffe-on-EC2-from-scratch-(Ubuntu,-CUDA-7,-cuDNN-3))

> 如果要在AWS试的话，推荐不要用其他教程了，太坑爹了，被国内的很多教程虐死。

> 当然，AWS也是遍地是坑，教程开篇也直接告诉你，一定要多给自己AWS的实例开EBS的存储量，因为原始实例只有8G，完全不够啊。还有60G的临时实例附送，那么这个60G临时实例，再次启动就会被重新刷新，等于没啥用，所以
> caffe系统安装一定要EBS扩展
> ，你看AWS收费还是高明——设坑收费法?

> 报错：

> 报错一：
dpkg: error: failed
> to
> write available database
> record
> about
> 'krb5
> -locales'
> to
> '/var/lib/dpkg/available': No space left
> on
> device
sh: echo: I/O error
E: Sub-
> process
> /usr/bin/dpkg returned an error code (
> 2
> )
E: Problem executing scripts DPkg::Post-Invoke
> 'if
> [ -d /var/lib/update-notifier ];
> then
> touch /var/lib/update-notifier/dpkg-run-stamp; fi;
> if
> [ -e /var/lib/update-notifier/updates-available ];
> then
> echo > /var/lib/update-notifier/updates-available; fi '
E: Sub-
> process
> returned an error code
报错二：
dpkg: error processing
> package
> initramfs-tools (
> --configure):
> cp: error writing ‘/tmp/mkinitramfs_WnwvEK//lib/modules/
> 3.13
> .0
> -
> 48
> -
> generic
> /kernel/crypto/ghash-
> generic
> .ko’: No space left
> on
> device
> .

> 一、安装NVIDIA Drivers

> 1、Drivers的安装包下载

> AWS中原本就有一套NVIDIA drivers，之前的有点老了，好像是340.46的，那么现在就需要根据实例的GPU配置，重新设置一个drivers，来到
> [NVIDIA Driver的安装网页](http://www.nvidia.com/Download/Find.aspx)
> ，这时候你得挑一下到底是什么driver，那么AWS的G2实例中的配置到底是啥呢？来看看AWS的官方怎么解释G2配置，
> [网址](http://docs.aws.amazon.com/zh_cn/AWSEC2/latest/UserGuide/accelerated-computing-instances.html)
> 。
![这里写图片描述](https://img-blog.csdn.net/20170221161356332?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> 笔者选择了一个375.26的：
> [Linux x64 (AMD64/EM64T) Display Driver BETA](http://www.nvidia.cn/content/DriverDownload-March2009/confirmation.php?url=/XFree86/Linux-x86_64/375.10/NVIDIA-Linux-x86_64-375.10.run&lang=cn&type=GeForce)

> 同时笔者也下载了CUDA 8.0的安装文件：
> [sudo sh cuda_8.0.61_375.26_linux.run](https://developer.nvidia.com/cuda-downloads)

> .

> 2、Drivers的安装

> 接下来就是安装了：

> chmod +x cud
> a_8
> .
> 0.61
> _375.
> 26
> _linux.run
mkdir nvidi
> a_installers
> ./cud
> a_8
> .
> 0.61
> _375.
> 26
> _linux.run -extract=
> `p
> wd
> `/
> nvidi
> a_installers
> 这里的’pwd’要指定到nvidia_installers创建的目录。

> 更新linux的NVIDIA’s drivers：

> sudo apt
> -get
> install linux
> -image
> -extra
> -virtual
> 执行后有可能跳出来：What would you like to do about menu.lst?

> 选择：keep the local version currently installed

> 需要禁用一些有冲突的内核：

> sudo vi /etc/modprobe
> .d
> /blacklist-nouveau
> .conf
> 加上以下内容：

> blacklist nouveau
blacklist lbm-nouveau
options nouveau modeset=
> 0
> alias
> nouveau
> off
> alias
> lbm-nouveau
> off
> 回到shell命令行执行：

> echo
> options nouveau modeset=
> 0
> |
> sudo
> tee
> -a
> /etc/modprobe.d/nouveau-kms.conf
> sudo
> update-initramfs -u
> sudo
> reboot
> 重启之后，需要安装一下这些内容：

> sudo apt
> -get
> install linux
> -source
> sudo apt
> -get
> install linux
> -headers
> -
> `uname -r`
> 正式到了可以安装的时候了：

> cd
> nvidia_installers
> sudo
> ./NVIDIA-Linux-x86_64-
> 375.26
> .run
> 其中会跳到另外的界面：

> 出现“nvidia-installer was forced to guess the X library path ‘/usr/lib’ and X module path ”点ok,

> 出现“The CC version check failed”，点击不理他；

> 出现问你更新 X configuration file，默认选No,

> 安装之后，可以通过

> nvidia
> -smi
> 来看看是否正确，安装，如何正确安装就会出现Nvidia的一些信息。

> 出现报错一：
![这里写图片描述](https://img-blog.csdn.net/20170221175746036?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> [ ](https://img-blog.csdn.net/20170221175746036?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

> 按照教程，你可以更新这个版本，也可以删除这个重新装。

> 要说删除之前的Nvidia的话，AWS也有官方教程：

> [http://docs.aws.amazon.com/zh_cn/AWSEC2/latest/UserGuide/accelerated-computing-instances.html](http://docs.aws.amazon.com/zh_cn/AWSEC2/latest/UserGuide/accelerated-computing-instances.html)

> sudo
> yum erase nvidia cuda
> 出现报错二：
![这里写图片描述](https://img-blog.csdn.net/20170221180042367?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> [ ](https://img-blog.csdn.net/20170221180042367?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

> 按提示安装：

> [

](https://img-blog.csdn.net/20170221180042367?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> apt
> -get
> install linux
> -
> 3.13
> .0
> -
> 108
> -generic
> [
](https://img-blog.csdn.net/20170221180042367?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> .

> [

](https://img-blog.csdn.net/20170221180042367?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> 二、安装CUDA8.0

> [
](https://img-blog.csdn.net/20170221180042367?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> 一般来说，各类教程中不要越级安装，CAFFE教程里面的版本是7.0，而且CUDA7.5比较稳定，一般会选择低一点。不过… 笔者当时并不知道这些，于是直接安装了CUDA8.0，最后也是可以用的。说明，可以兼容。

> [

](https://img-blog.csdn.net/20170221180042367?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> sudo modprobe nvidia
sudo apt
> -get
> install build
> -essential
> sudo
> .
> /cuda
> -linux64
> -rel
> -
> 8.0
> .61
> -
> 19326674.
> run
sudo
> .
> /cuda
> -samples
> -linux
> -
> 8.0
> .61
> -
> 19326674.
> run
> [
](https://img-blog.csdn.net/20170221180042367?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> 出现“Would you like to add desktop menu shortcuts?”随便你；

> 出现“Would you like to create a symbolic link? ”选择yes

> 安装好之后，一定要指定path。

> 打开“~/.bashrc”，输入：

> export
> PATH=
> $PATH
> :/home/usr/local/cuda-
> 8.0
> /bin
> export
> LD_LIBRARY_PATH=/home/usr/local/cuda-
> 8.0
> /lib64
> source
> ~/.bashrc
> sudo
> ldconfig
> 同时需要主要path的路径一定要对应，笔者照着教程抄，没想到一直没有输入正确，然后一直报错“lib”的错误。

> 输入完需要键入，让环境应用起来：

> source
> ~/.bashrc
> 最后：
![这里写图片描述](https://img-blog.csdn.net/20170221183203271?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> [ ](https://img-blog.csdn.net/20170221183203271?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

> 还有：
![这里写图片描述](https://img-blog.csdn.net/20170221183259381?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> [ ](https://img-blog.csdn.net/20170221183259381?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

> .

> .

> 三、cuDNN8.0的安装

> cuDNN这里笔者吃了很多亏…  因为cuDNN和CUDA一定要配套，不然不适配，笔者当时在CUDA8.0的情况，用了cuDNN出现了一堆报错，最后只能改过来。

> 下载链接：
> [https://developer.nvidia.com/rdp/cudnn-download](https://developer.nvidia.com/rdp/cudnn-download)

> 一定选择跟CUDA8.0配套的选项。同时，如果是GTX1080,GTX1070最好是v5以上的版本。

> 其中，该网站需要注册，你注册一下很快就可以用。一个包只有75MB

> tar
> -zxf
> cudnn
> -
> 8.0
> -linux
> -x64
> -v5
> .0
> -prod
> .
> tgz
cd cuda
sudo cp lib64
> /* /usr/local/cuda/lib64/
sudo cp include/cudnn.h /usr/local/cuda/include/
> 至此基本按照完毕了。于是乎，你可以nvidia-smi检查一下，是否ok.

> .

> .

> 四、caffe的安装

> .

> 1、caffe安装

> 相关的lib库安装：

> sudo apt
> -get
> install
> -y
> libprotobuf
> -dev
> libleveldb
> -dev
> libsnappy
> -dev
> libopencv
> -dev
> libboost
> -all
> -dev
> libhdf5
> -serial
> -dev
> protobuf
> -compiler
> gfortran libjpeg62 libfreeimage
> -dev
> libatlas
> -base
> -dev
> git python
> -dev
> python
> -pip
> libgoogle
> -glog
> -dev
> libbz2
> -dev
> libxml2
> -dev
> libxslt
> -dev
> libffi
> -dev
> libssl
> -dev
> libgflags
> -dev
> liblmdb
> -dev
> python
> -yaml
> python
> -numpy
> 需要下载pillow，

> sudo
> easy_install pillow
> 如果报错一：TypeError: ‘NoneType’ object is not callable

> 那么先执行：

> sudo apt
> -get
> install pypy
> -dev
> 如果报错二：

> ImportError:
> No module named packaging
> .version
> 解决：
wget https://bootstrap
> .pypa
> .io
> /get-pip
> .py
> python get-pip
> .py
> [参考教程](http://blog.csdn.net/win_turn/article/details/55052166)

> 然后就是从github把caffe挪下来：

> cd caffe
git
> clone
> https:
> //github.com/BVLC/caffe.git
> 接下里的步骤比较耗时，需要30min左右。

> cd
> caffe
cat python/requirements.txt | xargs -L
> 1
> sudo
> pip install
> .

> 2、配置Makefile文件

> 制作一个Makefile：

> cp
> Makefile
> .config
> .example
> Makefile
> .config
> vi Makefile
> .config
> 需要修改里面的：

> USE_CUDNN := 1，去掉\#，让其激活

> CUDA_DIR 的路径一定要对。

> .

> 3、make过程

> make pycaffe
> -jX
> make
> all
> -jX
> make test
> -jX
> x是你想调用的CPU核心数量，但是笔者之前的疑问。

> .

> 4、pycaffe的使用

> 进入cd caffe/python

> 试试能不能import caffe当然这一步报错就更多了。

> 报错一：No module named caffe

> export
> PYTHONPATH
> =
> /home/username
> /caffe/python
> 报错二：Failed to initialize libdc1394

> sudo
> ln
> /dev/
> null
> /dev/raw1394
> 报错三：

> Gdk-CRITICAL **: gdk_cursor_new_for_display: assertion
> 'GDK_IS_DISPLAY (display)'
> failed
Traceback (most recent call last):
.
.
.
TypeError:
> constructor
> returned
> NULL
> 这个错误的原因在于ssh连接服务器时候，import pyplot因为没有图形界面所导致的故障.两个办法：

> 办法一：直接在ssh连接服务器的时候，添加
> -X
> 的标记
ssh
> 192.168
> .0
> .1
> -X
> 以上参考博客：
> [http://nfeng.cc/2016/03/28/caffe-python-gtk-error/](http://nfeng.cc/2016/03/28/caffe-python-gtk-error/)

> 当然AWS因为需要密钥，这一步骤，比较繁琐。

> 办法二：使用matplotlib
进入Python后：
> import
> matplotlib
matplotlib(
> "Pdf"
> )
> \#matplotlib
> (
> "Agg"
> )都可以
> 报错四：还是报错三的问题

> Gdk-CRITICAL **: gdk_cursor_new_for_display: assertion
> 'GDK_IS_DISPLAY (display)'
> failed
Traceback (most recent call last):
.
.
.
TypeError:
> constructor
> returned
> NULL
> 还是报错三，前面的办法解决不了，那么可以先删除一下matplotlib：

> sudo
> pip uninstall matplotlib
> sudo
> apt-get install python-matplotlib
> 如果还是报错，那么重启一下AWS的shell

> 以上解决办法来源于
> [github](https://github.com/NVIDIA/DIGITS/issues/187)

> 报错五：

> ImportError: libcudart.so
> .8
> .0
> : cannot
> open
> shared object
> file
> : No such
> file
> or
> directory
> 这个问题在tensorflow中出现极多。那么解决方案更多了：

> 如果之前的make all出现该报错：

> （1）ImportError问题，一般是之前的lib库没有安装好；

> （2）TF_BINARY_URL修改，可见
> [lfzCarlosC](https://github.com/tensorflow/tensorflow/issues/5625)
> 这个是因为CUDA7.5和CUDA8.0的binary有很大的不同，那么nvidia、cudnn不适配出现的，github里面没有CUDA8.0,不过可以在以下网址找到：

> [https://github.com/tensorflow/tensorflow/blob/master/tensorflow/g3doc/get_started/os_setup.md](https://github.com/tensorflow/tensorflow/blob/master/tensorflow/g3doc/get_started/os_setup.md)

> TF_BINARY_URL=https://storage
> .googleapis
> .com
> /tensorflow/linux/gpu/tensorflow_gpu-
> 1.0
> .0
> -cp27-none-linux_x86_64
> .whl
> 如果make all可以启动：

> 在import caffe时候单独出现，那么就是你的LD_LIBRARY_PATH没设置好。需要在~/.bashrc修改，然后source：

> LD_LIBRARY_PATH=/home/usr/local/cuda-
> 8.0
> /lib64
> source
> ~/.bashrc
> sudo
> ldconfig
> 报错六：

> make:
> *** [
> .build
> _release/src/caffe/common
> .o
> ] Error
> 1
> make:
> *** [
> .build
> _release/src/caffe/layer_factory
> .o
> ] Error
> 1
> make:
> *** [
> .build
> _release/src/caffe/solver
> .o
> ] Error
> 1
> make:
> *** [
> .build
> _release/src/caffe/internal_thread
> .o
> ] Error
> 1
> 没有正确配置cudnn和CUDA的型号，特别是cudnn的型号进行调整、配置

> 最后：
![这里写图片描述](https://img-blog.csdn.net/20170221183332710?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> [ ](https://img-blog.csdn.net/20170221183332710?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

> .

> [
](https://img-blog.csdn.net/20170221183332710?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> 延伸一：linux很多命令不可用，崩溃

> 因为安装过程中，需要添加环境变量，譬如/etc/profile中，添加：

> PATH=/usr/local/cuda/bin:
> $PATH
> export
> PATH
> source
> /etc/profile
> 真是惨，以上是国产教程让我修改的，那么崩溃了。于是不得不改回来，参考教程：

> [http://xjsunjie.blog.51cto.com/999372/1384468](http://xjsunjie.blog.51cto.com/999372/1384468)

> 第一步：
export
> PATH
> =
> /usr/bin
> :/usr/sbin
> :/bin
> :/sbin
> :/usr/X11R6/bin
> 第二步：
需要把这句保存到/etc/profile里才能永久生效
第三步：
source  /etc/profile
> .

> 延伸二：实时查看NVIDIA

> nvidia-smi

> watch -n 0.1 nvidia-smi（watch实时查看数据）


> [
  ](https://img-blog.csdn.net/20170221183332710?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
