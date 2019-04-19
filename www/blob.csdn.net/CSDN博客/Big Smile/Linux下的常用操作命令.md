# Linux下的常用操作命令 - Big Smile - CSDN博客
2017年02月27日 20:06:36[王啸tr1912](https://me.csdn.net/tr1912)阅读数：403
#         最近在弄Linux系统下的软件安装和环境配置，感觉之前用的一些命令还有操作方法都忘记了，所以趁着这次记忆深刻，特别总结了一些常用的命令供参考，运行环境是CentOS。
# 一、最高权限访问文件夹
        在终端中输入命令：sudo nautilus，然后输入当前用户的密码。
        然后得到最高权限访问文件夹，可以访问root文件夹
![](https://img-blog.csdn.net/20170227200152255?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdHIxOTEy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
# 二、获得root权限来执行命令
   su root  然后输入当前用户或者root用户的密码 
![](https://img-blog.csdn.net/20170227200200683?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdHIxOTEy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
# 三、安装jar安装包
       前提要先安装JDK
      Java  -jar  文件名.jar
# 四、解压tar.gz文件
       tar –xzvf 文件路径+文件名.tar.gz
# 五、提升文件夹权限
       Chmod 777 文件或者文件夹名
 说明：此文会随着时间和经验而不断更新的。
