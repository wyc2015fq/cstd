# 超微7046GT+Redhat Enterprise 6.1+CUDA5.0的安装说明 - 战斗蜗牛的专栏 - CSDN博客





2013年04月30日 09:56:54[vbskj](https://me.csdn.net/vbskj)阅读数：1264
个人分类：[CUDA](https://blog.csdn.net/vbskj/article/category/643641)









    其实，在Linux服务器下面配置CUDA，本没有什么可说的，学生捣鼓了一天没捣鼓出来，我还以为是Redhat AS6.1的问题呢，早上过来重新配置，成功解决，其中有这样几个小问题。

1、超微准系统7046GT的驱动

    记得5年前，S1070时代，当时超微的准系统用的网卡模块驱动不兼容，当时的AS4.6认不出来，重新编译内核、插入模块，真是麻烦。现在AS6.1对超微的驱动支持挺好，除了C2070，全部成功安装。这里要吐槽一下超微的BIOS版本，真是老啊，连USB drive都不支持，让我等用U盘安装系统的人情何以堪啊。




2、CUDA5.0驱动的新特性

    好久没追新，发现Nvidia从5.0开始推出All-in-one的安装包，还是挺方便的。学生没安装成功，我以为是显示驱动的问题，重新下载linux x64驱动，通过init 3、init 1启动安装，都不行，发现不是这个冲突就是那个错误，问题在什么地方？




3、安装出错的核心问题

    一方面是开启了VNCServer，造成X没有完全退出；

    另一方面就是，显示一直使用7046GT的Slot6 PCIE-x16进行视频输出，因此使用Root用户修改‘/etc/inittab’为"init:3:default"后，果断重启，进入bios，在Advance中的Boot Graphic？？项目中，选择“OnBoard VGA”，F10，回车。把视频线接到7046GT上面的VGA集显接口处。




4、CUDA5.0的安装过程

    在命令行模式下，使用root用户登录，找到5.0的All-in-one安装包（里面挺全的，包含display driver，toolkit，sdk），键入指令“sh nvidia??**.run”:

     （1）很长的说明，直接按“q”退出

     （2）直接“accept”，你不同意人家的协议怎么安装啊；

     （3）让你选择是否安装dispaly driver，键入“y”;

     （4）让你选择是否安装cuda toolkit，键入“y”;

     （5）让你确定cuda toolkit的安装位置，按照默认即可，不用输入任何字符，直接回车;




     （6）让你选择是否安装cuda sdk，键入“y”;

     （7）让你确定cuda sdk的安装位置，按照默认即可，不用输入任何字符，直接回车;

        。。。等待

     （8）安装成功

       ！！注意，至此完成80%，还有20%在什么地方？




       root用户安装CUDA到系统目录，一般用户或者MPI用户编程、运行的时候，最好在自己的一般用户目录下安装。

       过程同上，只是display driver部分，选择“n”；cuda toolkit，选择“y”，然后在询问路径的时候指定“/home/user(你的用户名)/cuda-5.0”,回车；cuda sdk，选择“y”，然后再询问安装路径的时候就会自动修改是“/home/user/cuda-5.0/samples”,不用键入任何字符，回车即可，至此，一般用户的CUDA5.0安装完毕！！

       P.S.：5.0之前，driver、toolkit、sdk分开安装的时候，一般是root用户安装driver、tookit，个人用户安装sdk（能够自动搜索到toolkit的位置）即可；现在5.0的安装，个人用户也要安装toolkit，不然在安装sdk的时候，会出现找不到某些库的错误，造成安装失败。




5、开发环境配置

     安装成功之后，会告诉你安装日志在"/tmp/nvidia_install_1234.log"，把日志文件拷贝到用户目录下，这样按照日志中的说明，配置lib、include即可。









