# Ubuntu16.04＋Cuda8.0(gtx1060)+OpenCV2.4.13+anaconda2+caffe详细配置 - 数据之美的博客 - CSDN博客
2017年03月04日 17:18:44[看穿数据之美](https://me.csdn.net/oppo62258801)阅读数：1561
**[一]硬件以及系统介绍：**
1.1.显卡（getforce 10 系列－－GTX10ｘｘ，如GTX1060）
　　本解决方案使用的显卡类型为GTX1060，当然所有的getforce 10 系列－－－GTX10ｘｘ都可以。cuda7.5不支持getforce 10 系列以上的显卡，所以，那些安装有getforce 10 系列－－－GTX10以上的显卡的同学必须安装Cuda8.0，那些含有cuda7.5以及以下的教程将不再适用。
系统（主要是[Linux](http://lib.csdn.net/base/linux)）
　　我们先来看一看，Cuda8.0支持的系统和软件，如下图，Ubuntu16.04需要的gcc版本为5.3.1，而当Ubuntu16.04安装好之后，系统自带的gcc版本为5.4,所以gcc要重新安装，下面会有安装教程。
![](https://img-blog.csdn.net/20161218122706739?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpamllWmhhbmc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
      也就是说不论你是那个版本的Linux系统只要在上面表里即可使用Cuda8.0，从上表中可以看出Ubuntu14.04和Ubuntu16.04都可以使用Cuda8.0。
1.2.[OpenCV](http://lib.csdn.net/base/opencv)版本选择
　　　之前装了好几OpenCV3.1.0都失败了，找了半天原因，原来是OpenCV3.1.0对Cuda8.0兼容性不好。
![](https://img-blog.csdn.net/20161218123819243?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpamllWmhhbmc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![](https://img-blog.csdn.net/20161218123831785?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpamllWmhhbmc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![](https://img-blog.csdn.net/20161218123843086?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpamllWmhhbmc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
　　由上图可知Cuda8.0出来的时间是２０１６年９月，而OpenCV3.1.0却是去年，OpenCV2.4.13是今年５月，兼容性比OpenCV3.1.0要好，所以我选择的是OpenCV2.4.13。
****[二]**NVIDIA显卡驱动安装**
　　2.1有两种安装方式，第一种如下：
![](https://img-blog.csdn.net/20161216093504038?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpamllWmhhbmc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
        选择附加驱动，点击后会有如下窗口弹出
![](https://img-blog.csdn.net/20161218124523311?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpamllWmhhbmc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
        上面一项选择使用英伟达驱动，下面一项未知驱动为ｃｐｕ带的集成显卡选择下面的选项：不用这个设备，选完以后一定要点击"Apply Changes"也就是“应用改变”的按钮（右边的按钮）。
        安装完成以后要重启一下系统，然后在ｂａｓｈ里面输入：
**[html]**[view
 plain](http://blog.csdn.net/leijiezhang/article/details/53688157#)[copy](http://blog.csdn.net/leijiezhang/article/details/53688157#)
- nvidia-settings  
       显示如下结果表示安装成功，你可以在下面查看你的驱动版本
![](https://img-blog.csdn.net/20161216095924978?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpamllWmhhbmc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
        2.2方案二：如果方案一行不通，可以自行安装驱动，从NVIDIA官网上下载驱动：
[http://www.nvidia.cn/Download/index.aspx?lang=cn](http://www.nvidia.cn/Download/index.aspx?lang=cn)　如下图
![](https://img-blog.csdn.net/20161216101752660?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpamllWmhhbmc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
         选择自己需要的版本和驱动，然后点击旁边绿色按钮“搜索”：
![](https://img-blog.csdn.net/20161216101849577?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpamllWmhhbmc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
        点击绿色按钮“下载”
![](https://img-blog.csdn.net/20161216101944936?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpamllWmhhbmc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
        2.2.2下载完成，打开终端，先删除旧的驱动：
**[html]**[view
 plain](http://blog.csdn.net/leijiezhang/article/details/53688157#)[copy](http://blog.csdn.net/leijiezhang/article/details/53688157#)
- sudo apt-get purge nvidia*  
2.2.3禁用自带的
 nouveau nvidia驱动 （很重要！）
              通过如下命令创建一个文件
**[html]**[view
 plain](http://blog.csdn.net/leijiezhang/article/details/53688157#)[copy](http://blog.csdn.net/leijiezhang/article/details/53688157#)
- sudo vim /etc/modprobe.d/blacklist-nouveau.conf  
              在文件内添加如下内容：
**[html]**[view
 plain](http://blog.csdn.net/leijiezhang/article/details/53688157#)[copy](http://blog.csdn.net/leijiezhang/article/details/53688157#)
- blacklist nouveau  
**[html]**[view
 plain](http://blog.csdn.net/leijiezhang/article/details/53688157#)[copy](http://blog.csdn.net/leijiezhang/article/details/53688157#)
- options nouveau modeset=0
              更新一下
**[html]**[view
 plain](http://blog.csdn.net/leijiezhang/article/details/53688157#)[copy](http://blog.csdn.net/leijiezhang/article/details/53688157#)
- sudo update-initramfs -u  
 2.2.4重启系统至init 3（文本模式），也可先进入图形桌面再运行init 3进入文本模式，再安装下载的驱动就无问题，首先我们需要结束x-window的服务，否则驱动将无法正常安装
              关闭`X-Window`
``
**[html]**[view
 plain](http://blog.csdn.net/leijiezhang/article/details/53688157#)[copy](http://blog.csdn.net/leijiezhang/article/details/53688157#)
- sudo service lightdm stop  
切换到tty1控制台
**[html]**[view
 plain](http://blog.csdn.net/leijiezhang/article/details/53688157#)[copy](http://blog.csdn.net/leijiezhang/article/details/53688157#)
- Ctrl+Alt+F1  
　2.2.5.执行如下命令开始安装
**[html]**[view
 plain](http://blog.csdn.net/leijiezhang/article/details/53688157#)[copy](http://blog.csdn.net/leijiezhang/article/details/53688157#)
- sudo ./NVIDIA.run  
安装完成后重新启动`X-Window`
``
**[html]**[view
 plain](http://blog.csdn.net/leijiezhang/article/details/53688157#)[copy](http://blog.csdn.net/leijiezhang/article/details/53688157#)
- sudo service lightdm start  
然后`Ctrl+Alt+F7`进入图形界面
如果安装后驱动程序工作不正常，使用下面的命令进行卸载：
**[html]**[view
 plain](http://blog.csdn.net/leijiezhang/article/details/53688157#)[copy](http://blog.csdn.net/leijiezhang/article/details/53688157#)
- sudo sh ~/NVIDIA-Linux-x86_64-367.44.run --uninstall  
2.2.6检查是否安装成功，过程跟第一种方案一样，输入
**[html]**[view
 plain](http://blog.csdn.net/leijiezhang/article/details/53688157#)[copy](http://blog.csdn.net/leijiezhang/article/details/53688157#)
- nvidia-smi  
**[html]**[view
 plain](http://blog.csdn.net/leijiezhang/article/details/53688157#)[copy](http://blog.csdn.net/leijiezhang/article/details/53688157#)
- nvidia-settings  
****[三]**系统软件源设置并安装依赖包**
流程,分别操作：系统设置(system
 settings)---->软件与更新(software & update)----->选择Ubuntu软件（Ubuntu software)选项卡。     来改变系统软件源为阿里的源。
**![](https://img-blog.csdn.net/20161218131355882?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpamllWmhhbmc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)**
更新列表，打开终端输入如下命令
**[html]**[view
 plain](http://blog.csdn.net/leijiezhang/article/details/53688157#)[copy](http://blog.csdn.net/leijiezhang/article/details/53688157#)
- sudo apt-get update  
        软件源更新完毕后，安装一些必要的依赖包，命令如下
**[html]**[view
 plain](http://blog.csdn.net/leijiezhang/article/details/53688157#)[copy](http://blog.csdn.net/leijiezhang/article/details/53688157#)
- sudo apt-get install libprotobuf-dev libleveldb-dev libsnappy-dev libopencv-dev libhdf5-serial-dev protobuf-compiler libboost-all-dev libopenblas-dev liblapack-dev libatlas-base-dev libgflags-dev libgoogle-glog-dev liblmdb-dev  
![](https://img-blog.csdn.net/20161218132100714?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpamllWmhhbmc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
        如果执行过程报出有一些包没有找到，如下图
![](https://img-blog.csdn.net/20161218133037593?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpamllWmhhbmc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
        则更新列表，打开终端输入如下命令
**[html]**[view
 plain](http://blog.csdn.net/leijiezhang/article/details/53688157#)[copy](http://blog.csdn.net/leijiezhang/article/details/53688157#)
- sudo apt-get update  
        软件源更新完毕后，重新安装这些必要的依赖包，命令如下
**[html]**[view
 plain](http://blog.csdn.net/leijiezhang/article/details/53688157#)[copy](http://blog.csdn.net/leijiezhang/article/details/53688157#)
- sudo apt-get install libprotobuf-dev libleveldb-dev libsnappy-dev libopencv-dev libhdf5-serial-dev protobuf-compiler libboost-all-dev libopenblas-dev liblapack-dev libatlas-base-dev libgflags-dev libgoogle-glog-dev liblmdb-dev  
             此时就会安装成功！
****[四]**安装Anaconda**
anaconda里面集成了很多关于[Python](http://lib.csdn.net/base/python)科学计算的第三方库，主要是安装方便，而python是一个编译器，如果不使用anaconda，那么安装起来会比较痛苦，各个库之间的依赖性就很难连接的很好
      4.1.下载：anaconda的下载地址为：  [https://www.continuum.io/downloads#linux](https://www.continuum.io/downloads#linux)
![](https://img-blog.csdn.net/20161218133737651?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpamllWmhhbmc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
根据自己的需要选择不同的版本，但是推荐选择Python2.7的64位的。
![](https://img-blog.csdn.net/20161218134048374?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpamllWmhhbmc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
4.2.安装
在终端中进入存放这你刚刚下载好的文件的目录（我的是/下载),然后执行如下命令进行安装：
**[html]**[view
 plain](http://blog.csdn.net/leijiezhang/article/details/53688157#)[copy](http://blog.csdn.net/leijiezhang/article/details/53688157#)
- sudo ./Anaconda2-4.2.0-Linux-x86_64.sh  
![](https://img-blog.csdn.net/20161218135343511?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpamllWmhhbmc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
输入回车键之后会有一段长长的须知，一直按Ctr+F直到须知展示完毕，然后输入yes继续执行
![](https://img-blog.csdn.net/20161218135456324?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpamllWmhhbmc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
           这个时候，我们在家目录下新建一个/Software文件夹用存放以后自己安装的软件，当然包含anaconda，执行如下命令
**[html]**[view
 plain](http://blog.csdn.net/leijiezhang/article/details/53688157#)[copy](http://blog.csdn.net/leijiezhang/article/details/53688157#)
- cd ~  
**[html]**[view
 plain](http://blog.csdn.net/leijiezhang/article/details/53688157#)[copy](http://blog.csdn.net/leijiezhang/article/details/53688157#)
- mkdir Software  
              此时终端会提示你输入安装位置，你输入：/home/leijie/Software/anaconda2，然后继续安装
![](https://img-blog.csdn.net/20161218140646376?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpamllWmhhbmc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
           直到最后安装成功，到这里anaconda就安装完成了，[测试](http://lib.csdn.net/base/softwaretest)一下
**[html]**[view
 plain](http://blog.csdn.net/leijiezhang/article/details/53688157#)[copy](http://blog.csdn.net/leijiezhang/article/details/53688157#)
- anaconda -V  
![](https://img-blog.csdn.net/20161218141000349?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpamllWmhhbmc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
**[五]**cuda安装
     前言：一定要搞清楚你自己的显卡要装什么版本的驱动，以及支持的cuda版本。我的显卡支持cuda8.0
**５.1.下载**： 下载地址为：[https://developer.nvidia.com/cuda-downloads](https://developer.nvidia.com/cuda-downloads),然后根据自己的需要选择下载文件
![](https://img-blog.csdn.net/20161218142333304?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpamllWmhhbmc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
               我下载的是.run文件，也推荐大家下载这种格式的。
![](https://img-blog.csdn.net/20161218142729369?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpamllWmhhbmc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
** ５.2.GCC降级**------->由文章的最开始，我们可以知道cuda8.0目前仅支持gcc5.3而ubuntu16.04自带的gcc版本为gcc5.4,所以要降级。
        ５.2.1.先查看你是否需要降级GCC
**[html]**[view
 plain](http://blog.csdn.net/leijiezhang/article/details/53688157#)[copy](http://blog.csdn.net/leijiezhang/article/details/53688157#)
- gcc -v  
![](https://img-blog.csdn.net/20161218144042859?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpamllWmhhbmc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
          如果你的版本是5.3.1以下的就不用降级了，否则要降级gcc。
         ５.2.2.下载gcc5.3.0,下载地址为：[https://pan.baidu.com/s/1cL2v62](https://pan.baidu.com/s/1cL2v62)，下载好后，进入到该文件目录，执行如下操作：
**[html]**[view
 plain](http://blog.csdn.net/leijiezhang/article/details/53688157#)[copy](http://blog.csdn.net/leijiezhang/article/details/53688157#)
- sudo tar -zxvf gcc-5.3.0.tar.gz  
          ５.2.3.进入到gcc目录：
**[html]**[view
 plain](http://blog.csdn.net/leijiezhang/article/details/53688157#)[copy](http://blog.csdn.net/leijiezhang/article/details/53688157#)
- cd gcc-5.3.0/  
          ５.2.4.下载依赖项(网络不好的话可能慢点，慢慢等吧)：
**[html]**[view
 plain](http://blog.csdn.net/leijiezhang/article/details/53688157#)[copy](http://blog.csdn.net/leijiezhang/article/details/53688157#)
- sudo ./contrib/download_prerequisites  
          ５.2.5. 新建一个编译的文件夹，并进入该文件夹：
**[html]**[view
 plain](http://blog.csdn.net/leijiezhang/article/details/53688157#)[copy](http://blog.csdn.net/leijiezhang/article/details/53688157#)
- sudo mkdir build  
**[html]**[view
 plain](http://blog.csdn.net/leijiezhang/article/details/53688157#)[copy](http://blog.csdn.net/leijiezhang/article/details/53688157#)
- cd build/  
          ５.2.6.生成makefile文件：
**[html]**[view
 plain](http://blog.csdn.net/leijiezhang/article/details/53688157#)[copy](http://blog.csdn.net/leijiezhang/article/details/53688157#)
- sudo ../configure --enable-checking=release--enable-languages=c,c++ --disable-multilib  
          ５.2.7.编译，编译过程有点长，请耐心等待：
**[html]**[view
 plain](http://blog.csdn.net/leijiezhang/article/details/53688157#)[copy](http://blog.csdn.net/leijiezhang/article/details/53688157#)
- sudo make -j4  
          ５.2.8.安装：
          编译结束以后，我们就可以执行安装了：
**[html]**[view
 plain](http://blog.csdn.net/leijiezhang/article/details/53688157#)[copy](http://blog.csdn.net/leijiezhang/article/details/53688157#)
- sudo make install  
           ５.2.9.查看是否成功
**[html]**[view
 plain](http://blog.csdn.net/leijiezhang/article/details/53688157#)[copy](http://blog.csdn.net/leijiezhang/article/details/53688157#)
- gcc --version  
![](https://img-blog.csdn.net/20161218161208681?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpamllWmhhbmc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
**[html]**[view
 plain](http://blog.csdn.net/leijiezhang/article/details/53688157#)[copy](http://blog.csdn.net/leijiezhang/article/details/53688157#)
- g++ --version  
![](https://img-blog.csdn.net/20161218161315439?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpamllWmhhbmc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
**５.3安装cuda**
      gcc降到5.3之后，可以直接来安装了cuda了，进到你下载好的cuda-8.0的run文件目录，运行如下命令：
**[html]**[view
 plain](http://blog.csdn.net/leijiezhang/article/details/53688157#)[copy](http://blog.csdn.net/leijiezhang/article/details/53688157#)
- sudo ./cuda_8.0.44_linux.run  
     这时便进入的声明的界面，你可以连续按Ctr+f向下阅读直到结尾，也可以按Ctr+C跳过阅读，直接到选择接受与否的位置
![](https://img-blog.csdn.net/20161218162012588?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpamllWmhhbmc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
                输入接受：accept;  然后回车。具体应该怎么输入请按照如下图片的指示来操作，原因：第一次不安装的是英伟达驱动，因为我们之前已经安装过了，第二次不安装是更新，也不用安，安装了麻烦。
![](https://img-blog.csdn.net/20161218163326558?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpamllWmhhbmc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![](https://img-blog.csdn.net/20161218163429981?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpamllWmhhbmc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
               由于我第一遍输错了，所以我又重新安装了一遍，这一次的操作如下图：
![](https://img-blog.csdn.net/20161218163527456?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpamllWmhhbmc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
                 如果出现下图的结果，表明你已经安装成功了，虽然有一个警告和一个特别注意但是不影响结果
![](https://img-blog.csdn.net/20161218163932270?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpamllWmhhbmc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
          从上图可是，下载的例子里面是缺少一些支持库的，现在我们安装这些库：
**[html]**[view
 plain](http://blog.csdn.net/leijiezhang/article/details/53688157#)[copy](http://blog.csdn.net/leijiezhang/article/details/53688157#)
- sudo apt-get install freeglut3-dev build-essential libx11-dev libxmu-dev libxi-dev libgl1-mesa-glx libglu1-mesa libglu1-mesa-dev  
          从上图可以看出来，有一个特别注意，所以我们做如下配置：打开.bashrc来进行配置，命令如下
**[html]**[view
 plain](http://blog.csdn.net/leijiezhang/article/details/53688157#)[copy](http://blog.csdn.net/leijiezhang/article/details/53688157#)
- sudo vim ~/.bashrc  
         并将如下的几行内容复制的到.bashrc文件的最下面
**[html]**[view
 plain](http://blog.csdn.net/leijiezhang/article/details/53688157#)[copy](http://blog.csdn.net/leijiezhang/article/details/53688157#)
- export PATH=/usr/local/cuda-8.0/bin${PATH:+:${PATH}}  
**[html]**[view
 plain](http://blog.csdn.net/leijiezhang/article/details/53688157#)[copy](http://blog.csdn.net/leijiezhang/article/details/53688157#)
- export LD_LIBRARY_PATH=/usr/local/cuda8.0/lib64${LD_LIBRARY_PATH:+:${LD_LIBRARY_PATH}}  
**[html]**[view
 plain](http://blog.csdn.net/leijiezhang/article/details/53688157#)[copy](http://blog.csdn.net/leijiezhang/article/details/53688157#)
- export CUDA_HOME=/usr/local/cuda  
![](https://img-blog.csdn.net/20161218164755016?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpamllWmhhbmc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
          然后设置环境变量和动态链接库，在命令行输入：
**[html]**[view
 plain](http://blog.csdn.net/leijiezhang/article/details/53688157#)[copy](http://blog.csdn.net/leijiezhang/article/details/53688157#)
- sudo vim /etc/profile  
          在打开的文件末尾加入：
**[html]**[view
 plain](http://blog.csdn.net/leijiezhang/article/details/53688157#)[copy](http://blog.csdn.net/leijiezhang/article/details/53688157#)
- export PATH=/usr/local/cuda/bin:$PATH  
          保存之后，创建链接文件：
**[html]**[view
 plain](http://blog.csdn.net/leijiezhang/article/details/53688157#)[copy](http://blog.csdn.net/leijiezhang/article/details/53688157#)
- sudo gedit /etc/ld.so.conf.d/cuda.conf  
          在打开的文件中添加如下语句：
**[html]**[view
 plain](http://blog.csdn.net/leijiezhang/article/details/53688157#)[copy](http://blog.csdn.net/leijiezhang/article/details/53688157#)
- /usr/local/cuda/lib64  
          然后执行如下命令使得链接立即生效：
**[html]**[view
 plain](http://blog.csdn.net/leijiezhang/article/details/53688157#)[copy](http://blog.csdn.net/leijiezhang/article/details/53688157#)
- sudo ldconfig  
         现在重启电脑！
         现在就开始测试一下看看cuda是否安装成功吧！输入如下命令打开测试的代码位置：
**[html]**[view
 plain](http://blog.csdn.net/leijiezhang/article/details/53688157#)[copy](http://blog.csdn.net/leijiezhang/article/details/53688157#)
- cd /usr/local/cuda-8.0/samples/1_Utilities/deviceQuery  
         编译这些代码：
**[html]**[view
 plain](http://blog.csdn.net/leijiezhang/article/details/53688157#)[copy](http://blog.csdn.net/leijiezhang/article/details/53688157#)
- sudo make -j4  
         执行代码
**[html]**[view
 plain](http://blog.csdn.net/leijiezhang/article/details/53688157#)[copy](http://blog.csdn.net/leijiezhang/article/details/53688157#)
- ./deviceQuery  
         如果出现如下结果---显卡的配置信息，恭喜你，这就证明你成功的安装了cuda
![](https://img-blog.csdn.net/20161219002057389?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpamllWmhhbmc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
**[六]**cuDNN安装
cuDNN[深度学习](http://lib.csdn.net/base/deeplearning)加速的一些库。下载地址为：[https://developer.nvidia.com/cudnn](https://developer.nvidia.com/cudnn)
**６.1.下载**
        打开网站，如下图，需要注册一个账户，因为下载的时候需要,然后下载，流程如下图：
![](https://img-blog.csdn.net/20161219003323641?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpamllWmhhbmc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
![](https://img-blog.csdn.net/20161219003333734?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpamllWmhhbmc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
![](https://img-blog.csdn.net/20161219003339687?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpamllWmhhbmc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
       ６.2下载后解压
**[html]**[view
 plain](http://blog.csdn.net/leijiezhang/article/details/53688157#)[copy](http://blog.csdn.net/leijiezhang/article/details/53688157#)
- tar -xvf cudnn-8.0-linux-x64-v5.1.tgz  
       执行如下操作：
**[html]**[view
 plain](http://blog.csdn.net/leijiezhang/article/details/53688157#)[copy](http://blog.csdn.net/leijiezhang/article/details/53688157#)
- cd cuda/include/  
**[html]**[view
 plain](http://blog.csdn.net/leijiezhang/article/details/53688157#)[copy](http://blog.csdn.net/leijiezhang/article/details/53688157#)
- sudo cp cudnn.h /usr/local/cuda/include/  
**[html]**[view
 plain](http://blog.csdn.net/leijiezhang/article/details/53688157#)[copy](http://blog.csdn.net/leijiezhang/article/details/53688157#)
- cd ../lib64/  
**[html]**[view
 plain](http://blog.csdn.net/leijiezhang/article/details/53688157#)[copy](http://blog.csdn.net/leijiezhang/article/details/53688157#)
- sudo cp lib* /usr/local/cuda/lib64/  
**[html]**[view
 plain](http://blog.csdn.net/leijiezhang/article/details/53688157#)[copy](http://blog.csdn.net/leijiezhang/article/details/53688157#)
- cd /usr/local/cuda/lib64/  
**[html]**[view
 plain](http://blog.csdn.net/leijiezhang/article/details/53688157#)[copy](http://blog.csdn.net/leijiezhang/article/details/53688157#)
- sudo rm -rf libcudnn.so libcudnn.so.5  
**[html]**[view
 plain](http://blog.csdn.net/leijiezhang/article/details/53688157#)[copy](http://blog.csdn.net/leijiezhang/article/details/53688157#)
- sudo ln -s libcudnn.so.5.1.5 libcudnn.so.5  
**[html]**[view
 plain](http://blog.csdn.net/leijiezhang/article/details/53688157#)[copy](http://blog.csdn.net/leijiezhang/article/details/53688157#)
- sudo ln -s libcudnn.so.5 libcudnn.so  
      执行完之后，cuDNN算是安装完成了
**[七]**OpenCV2.4.13安装
**７.1下载**
        下载地址：[http://opencv.org/](http://opencv.org/) 也可以从网盘下载：[https://pan.baidu.com/s/1qXT7is0](https://pan.baidu.com/s/1qXT7is0)
![](https://img-blog.csdn.net/20161219005400635?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpamllWmhhbmc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
**７.2安装相关依赖项：**
**[html]**[view
 plain](http://blog.csdn.net/leijiezhang/article/details/53688157#)[copy](http://blog.csdn.net/leijiezhang/article/details/53688157#)
- sudo apt-get install build-essential cmake git libgtk2.0-dev pkg-config libavcodec-dev libavformat-dev libswscale-dev python-dev python-numpy libtbb2 libtbb-dev libjpeg-dev libpng12-dev libtiff5-dev libjasper-dev libdc1394-22-dev libatlas-base-dev gfortran  
**７.3配置编译opencv的环境**
     配置opencv，在这里使用cmake-gui进行配置，，后面会介绍。
     先安装cmake-gui，直接在终端输入cmake-gui会有提示的：
**[html]**[view
 plain](http://blog.csdn.net/leijiezhang/article/details/53688157#)[copy](http://blog.csdn.net/leijiezhang/article/details/53688157#)
- sudo apt-get install cmake-qt-gui  
**７.4解压这个下载的文件，在解压后的文件夹下新建一个opencv-debug文件夹：**
**[html]**[view
 plain](http://blog.csdn.net/leijiezhang/article/details/53688157#)[copy](http://blog.csdn.net/leijiezhang/article/details/53688157#)
- unzip opencv-2.4.13.zip  
**[html]**[view
 plain](http://blog.csdn.net/leijiezhang/article/details/53688157#)[copy](http://blog.csdn.net/leijiezhang/article/details/53688157#)
- cd opencv-2.4.13/  
**[html]**[view
 plain](http://blog.csdn.net/leijiezhang/article/details/53688157#)[copy](http://blog.csdn.net/leijiezhang/article/details/53688157#)
- mkdir opencv-debug  
**[html]**[view
 plain](http://blog.csdn.net/leijiezhang/article/details/53688157#)[copy](http://blog.csdn.net/leijiezhang/article/details/53688157#)
- cd opencv-debug/  
**７.5配置cmake**
      执行如下命令
**[html]**[view
 plain](http://blog.csdn.net/leijiezhang/article/details/53688157#)[copy](http://blog.csdn.net/leijiezhang/article/details/53688157#)
- cmake-gui  
     点击configure出现下面的界面，cmake中configure的作用就是能够读出系统已经安装的相关文件，然后进行generate：
![](https://img-blog.csdn.net/20161219012823839?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpamllWmhhbmc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
**７.6编译安装**，
           编译时间可能有点长，耐心等待。还是在刚刚的cd的那个文件夹opencv2-4-13-debug下执行：
**[html]**[view
 plain](http://blog.csdn.net/leijiezhang/article/details/53688157#)[copy](http://blog.csdn.net/leijiezhang/article/details/53688157#)
- make -j8  
![](https://img-blog.csdn.net/20161219013241252?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpamllWmhhbmc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
         要是中途出现了一些问题是与cuda有关的，打开opencv下面那个cmakelist文件把with_cuda设置为OFF，如下图，之后再cmake，再编译。 
![](https://img-blog.csdn.net/20161219015244661?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpamllWmhhbmc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
        安装opencv,安装时间可能有点长，耐心等待。
**[html]**[view
 plain](http://blog.csdn.net/leijiezhang/article/details/53688157#)[copy](http://blog.csdn.net/leijiezhang/article/details/53688157#)
- sudo make install  
![](https://img-blog.csdn.net/20161219092914114?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpamllWmhhbmc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
**７.7环境变量配置**
        安装成功后还需要设置opencv的环境变量,打开文件
**[html]**[view
 plain](http://blog.csdn.net/leijiezhang/article/details/53688157#)[copy](http://blog.csdn.net/leijiezhang/article/details/53688157#)
- sudo gedit /etc/profile  
　　在最后一行输入如下内容：
**[html]**[view
 plain](http://blog.csdn.net/leijiezhang/article/details/53688157#)[copy](http://blog.csdn.net/leijiezhang/article/details/53688157#)
- export PKG_CONFIG_PATH=/usr/local/lib/pkgconfig:$PKG_CONFIG_PATH  
![](https://img-blog.csdn.net/20161219094001202?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpamllWmhhbmc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
　　执行下列命令, 使环境变量立即生效
**[html]**[view
 plain](http://blog.csdn.net/leijiezhang/article/details/53688157#)[copy](http://blog.csdn.net/leijiezhang/article/details/53688157#)
- source /etc/profile  
　　设置lib库路径，打开文件
**[html]**[view
 plain](http://blog.csdn.net/leijiezhang/article/details/53688157#)[copy](http://blog.csdn.net/leijiezhang/article/details/53688157#)
- sudo gedit /etc/ld.so.conf.d/opencv.conf  
       opencv的库一般安装在’/usr/local/lib’文件夹下，在文件内添加
**[html]**[view
 plain](http://blog.csdn.net/leijiezhang/article/details/53688157#)[copy](http://blog.csdn.net/leijiezhang/article/details/53688157#)
- /usr/local/lib  
       执行下列命令使之立刻生效
**[html]**[view
 plain](http://blog.csdn.net/leijiezhang/article/details/53688157#)[copy](http://blog.csdn.net/leijiezhang/article/details/53688157#)
- sudo ldconfig  
       经过上面的流程，这样就可以在eclipse里或者qtcreator里用opencv了。 不过要配置号相应的路径和lib文件。
**７．８验证是否安装成功**
       对opencv的sample进行编译并运行，依次执行如下代码
**[html]**[view
 plain](http://blog.csdn.net/leijiezhang/article/details/53688157#)[copy](http://blog.csdn.net/leijiezhang/article/details/53688157#)
- cd opencv-2.4.13/samples/c  
**[html]**[view
 plain](http://blog.csdn.net/leijiezhang/article/details/53688157#)[copy](http://blog.csdn.net/leijiezhang/article/details/53688157#)
- ./build_all.sh  
**[html]**[view
 plain](http://blog.csdn.net/leijiezhang/article/details/53688157#)[copy](http://blog.csdn.net/leijiezhang/article/details/53688157#)
- ./facedetect --cascade="/usr/local/share/OpenCV/haarcascades/haarcascade_frontalface_alt.xml"--scale=1.5 lena.jpg  
　　如果出现lena的图片就证明成功了！
![](https://img-blog.csdn.net/20161219095340255?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpamllWmhhbmc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
**[八]**Caffe安装配置
**８．１下载**
　　　github下载地址：[https://github.com/BVLC/caffe](https://github.com/BVLC/caffe)  网盘下载地址：[https://pan.baidu.com/s/1bpqsnZD](https://pan.baidu.com/s/1bpqsnZD)
　　　下载好以后解压，如下图
![](https://img-blog.csdn.net/20161219104421313?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpamllWmhhbmc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
**８.2Makefile.config配置**
　　你会看到其中有一个文件叫Makefile.config.example.这个文件是官方给出了编译的“模板”，我们可以直接拿过来小小的修改一下就行。复制下面的命令把名字改为Makefile.config
**[html]**[view
 plain](http://blog.csdn.net/leijiezhang/article/details/53688157#)[copy](http://blog.csdn.net/leijiezhang/article/details/53688157#)
- sudo cp Makefile.config.example Makefile.config  
　你会发现下面多了一个Makefile.config的文件
![](https://img-blog.csdn.net/20161219111751155?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpamllWmhhbmc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
　　打开这个文件并且修改(我这里用的是sublime，你可以换成其他的编辑器比如vim)：
　　如果你没有安装sublime，可以参考我写的教程安装：[http://blog.csdn.net/leijiezhang/article/details/53725710](http://blog.csdn.net/leijiezhang/article/details/53725710)
**[html]**[view
 plain](http://blog.csdn.net/leijiezhang/article/details/53688157#)[copy](http://blog.csdn.net/leijiezhang/article/details/53688157#)
- sudo subl Makefile.config  
　　你能够看到如下编辑窗口：
![](https://img-blog.csdn.net/20161219112022386?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpamllWmhhbmc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
然后根据个人情况修改文件： 
　　　　　1.若使用cudnn，则将`#USE_CUDNN := 1`修改成： `USE_CUDNN := 1` （就是去掉注释的‘#’号）
　　　　　2.若使用的opencv版本是２的，则将`#OPENCV_VERSION := ３` 修改为：`OPENCV_VERSION := ２`
　　　　　3.若要使用python来编写layer，则将`#WITH_PYTHON_LAYER := 1`修改为`WITH_PYTHON_LAYER := 1`
　　　　　4.要是你是用的anaconda的话，还需要改一些地方
![](https://img-blog.csdn.net/20161219112143746?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpamllWmhhbmc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
　　　把之前的`PYTHON_INCLUDE`加上注释，把`ANACONDA_HOME`以及下面的`PYTHON_INCLUDE` 都去掉，并且改为如下图最终的样子。注意：`ANACONDA_HOME`后面是anaconda的安装位置，根据自己的情况不同而不同。其中$(HOME)就是你的家目录
![](https://img-blog.csdn.net/20161219112220724?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpamllWmhhbmc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
　　　5.重要 : 
　　　　将`# Whatever else you find you need goes here.`下面的
**[html]**[view
 plain](http://blog.csdn.net/leijiezhang/article/details/53688157#)[copy](http://blog.csdn.net/leijiezhang/article/details/53688157#)
- INCLUDE_DIRS := $(PYTHON_INCLUDE) /usr/local/include  
**[html]**[view
 plain](http://blog.csdn.net/leijiezhang/article/details/53688157#)[copy](http://blog.csdn.net/leijiezhang/article/details/53688157#)
- LIBRARY_DIRS := $(PYTHON_LIB) /usr/local/lib /usr/lib  
　　　　修改为：
**[html]**[view
 plain](http://blog.csdn.net/leijiezhang/article/details/53688157#)[copy](http://blog.csdn.net/leijiezhang/article/details/53688157#)
- INCLUDE_DIRS := $(PYTHON_INCLUDE) /usr/local/include /usr/include/hdf5/serial  
**[html]**[view
 plain](http://blog.csdn.net/leijiezhang/article/details/53688157#)[copy](http://blog.csdn.net/leijiezhang/article/details/53688157#)
- LIBRARY_DIRS := $(PYTHON_LIB) /usr/local/lib /usr/lib /usr/lib/x86_64-linux-gnu /usr/lib/x86_64-linux-gnu/hdf5/serial  
　　　(因为ubuntu16.04的文件包含位置发生了变化)
**８．３修改makefile文件**
打开makefile文件
**[html]**[view
 plain](http://blog.csdn.net/leijiezhang/article/details/53688157#)[copy](http://blog.csdn.net/leijiezhang/article/details/53688157#)
- sudo subl Makefile  
　　将(大概409行的样子)：
**[html]**[view
 plain](http://blog.csdn.net/leijiezhang/article/details/53688157#)[copy](http://blog.csdn.net/leijiezhang/article/details/53688157#)
- NVCCFLAGS +=-ccbin=$(CXX) -Xcompiler-fPIC $(COMMON_FLAGS)  
　　替换为：
**[html]**[view
 plain](http://blog.csdn.net/leijiezhang/article/details/53688157#)[copy](http://blog.csdn.net/leijiezhang/article/details/53688157#)
- NVCCFLAGS += -D_FORCE_INLINES -ccbin=$(CXX) -Xcompiler -fPIC $(COMMON_FLAGS)  
![](https://img-blog.csdn.net/20161219113308705?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpamllWmhhbmc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
　　在Makefile文件的第大概181行，把 `hdf5_hl`和`hdf5`修改为`hdf5_serial_hl` 和`hdf5_serial`，即
将：
**[html]**[view
 plain](http://blog.csdn.net/leijiezhang/article/details/53688157#)[copy](http://blog.csdn.net/leijiezhang/article/details/53688157#)
- LIBRARIES += glog gflags protobuf boost_system boost_filesystem m hdf5_hl hdf5  
　　改为：
**[html]**[view
 plain](http://blog.csdn.net/leijiezhang/article/details/53688157#)[copy](http://blog.csdn.net/leijiezhang/article/details/53688157#)
- LIBRARIES += glog gflags protobuf boost_system boost_filesystem m hdf5_serial_hl hdf5_serial  
**８．４编辑/usr/local/cuda/include/host_config.h**
　　执行如下命令，开始编辑host_config.h
**[html]**[view
 plain](http://blog.csdn.net/leijiezhang/article/details/53688157#)[copy](http://blog.csdn.net/leijiezhang/article/details/53688157#)
- sudo subl /usr/local/cuda/include/host_config.h  
　　将其中的第115行注释掉：
**[html]**[view
 plain](http://blog.csdn.net/leijiezhang/article/details/53688157#)[copy](http://blog.csdn.net/leijiezhang/article/details/53688157#)
- #error-- unsupported GNU version! gcc versions later than 5 are not supported!  
　　改为：
**[html]**[view
 plain](http://blog.csdn.net/leijiezhang/article/details/53688157#)[copy](http://blog.csdn.net/leijiezhang/article/details/53688157#)
- //#error-- unsupported GNU version! gcc versions later than 5 are not supported!  
![](https://img-blog.csdn.net/20161219114322570?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpamllWmhhbmc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
**８．４编译**编译之前，先安装编译需要的库：
**[html]**[view
 plain](http://blog.csdn.net/leijiezhang/article/details/53688157#)[copy](http://blog.csdn.net/leijiezhang/article/details/53688157#)
- sudo apt-get install libatlas-base-dev  
**[html]**[view
 plain](http://blog.csdn.net/leijiezhang/article/details/53688157#)[copy](http://blog.csdn.net/leijiezhang/article/details/53688157#)
- sudo apt-get install libprotobuf-dev  
**[html]**[view
 plain](http://blog.csdn.net/leijiezhang/article/details/53688157#)[copy](http://blog.csdn.net/leijiezhang/article/details/53688157#)
- sudo apt-get install libleveldb-dev  
**[html]**[view
 plain](http://blog.csdn.net/leijiezhang/article/details/53688157#)[copy](http://blog.csdn.net/leijiezhang/article/details/53688157#)
- sudo apt-get install libsnappy-dev  
**[html]**[view
 plain](http://blog.csdn.net/leijiezhang/article/details/53688157#)[copy](http://blog.csdn.net/leijiezhang/article/details/53688157#)
- sudo apt-get install libopencv-dev  
**[html]**[view
 plain](http://blog.csdn.net/leijiezhang/article/details/53688157#)[copy](http://blog.csdn.net/leijiezhang/article/details/53688157#)
- sudo apt-get install libboost-all-dev  
**[html]**[view
 plain](http://blog.csdn.net/leijiezhang/article/details/53688157#)[copy](http://blog.csdn.net/leijiezhang/article/details/53688157#)
- sudo apt-get install libhdf5-serial-dev  
**[html]**[view
 plain](http://blog.csdn.net/leijiezhang/article/details/53688157#)[copy](http://blog.csdn.net/leijiezhang/article/details/53688157#)
- sudo apt-get install libgflags-dev  
**[html]**[view
 plain](http://blog.csdn.net/leijiezhang/article/details/53688157#)[copy](http://blog.csdn.net/leijiezhang/article/details/53688157#)
- sudo apt-get install libgoogle-glog-dev  
**[html]**[view
 plain](http://blog.csdn.net/leijiezhang/article/details/53688157#)[copy](http://blog.csdn.net/leijiezhang/article/details/53688157#)
- sudo apt-get install liblmdb-dev  
**[html]**[view
 plain](http://blog.csdn.net/leijiezhang/article/details/53688157#)[copy](http://blog.csdn.net/leijiezhang/article/details/53688157#)
- sudo apt-get install protobuf-compiler  
**[html]**[view
 plain](http://blog.csdn.net/leijiezhang/article/details/53688157#)[copy](http://blog.csdn.net/leijiezhang/article/details/53688157#)
- sudo apt-get install cmake  
接下来进行编译（在解压的文件夹下)：
**[html]**[view
 plain](http://blog.csdn.net/leijiezhang/article/details/53688157#)[copy](http://blog.csdn.net/leijiezhang/article/details/53688157#)
- sudo make all -j8  
然后就编译好了
![](https://img-blog.csdn.net/20161219134205840?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpamllWmhhbmc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
再测试一下：
**[html]**[view
 plain](http://blog.csdn.net/leijiezhang/article/details/53688157#)[copy](http://blog.csdn.net/leijiezhang/article/details/53688157#)
- sudo make test  
可能有个缓慢的编译过程，然后出现如下如所示的界面
![](https://img-blog.csdn.net/20161219135636254?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpamllWmhhbmc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
到此，所有需要安装的都已经安装好了！关键是遇到问题去解决问题，不要慌，越努力越幸运。
**参考网站：**
[http://blog.csdn.net/xierhacker/article/details/53035989](http://blog.csdn.net/xierhacker/article/details/53035989)
[http://blog.csdn.net/autocyz/article/details/52299889](http://blog.csdn.net/autocyz/article/details/52299889)
[http://www.cnblogs.com/jinggege/p/5877326.html](http://www.cnblogs.com/jinggege/p/5877326.html)
[http://blog.csdn.net/sunxianliang1/article/details/50723086](http://www.cnblogs.com/jinggege/p/5877326.html)
另外补充几个网址，方便各位查看
cuda的官方文档：[http://docs.nvidia.com/cuda/](http://docs.nvidia.com/cuda/)
ｎｖｃｃ介绍的官方文档：[http://docs.nvidia.com/cuda/cuda-compiler-driver-nvcc/index.html#axzz4SsuDPfdq](http://docs.nvidia.com/cuda/cuda-compiler-driver-nvcc/index.html#axzz4SsuDPfdq)
ｃｕｄａ开发包介绍的官方文档：[http://docs.nvidia.com/cuda/#axzz4SsuDPfdq](http://docs.nvidia.com/cuda/#axzz4SsuDPfdq)
还有ｃｕｄａ８．０安装的官方文档：[http://blog.csdn.net/u012581999/article/details/52433609](http://blog.csdn.net/u012581999/article/details/52433609)[http://docs.nvidia.com/cuda/cuda-installation-guide-linux/index.html#axzz4SsuDPfdq](http://docs.nvidia.com/cuda/cuda-installation-guide-linux/index.html#axzz4SsuDPfdq)
