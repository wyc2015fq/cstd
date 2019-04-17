# 华硕ASUS电脑 Ubuntu下  安装NVIDIA驱动（循环登陆+开机显示the system is running in low graphic mode问题 ） - Snoopy_Dream - CSDN博客





2018年01月28日 16:23:49[Snoopy_Dream](https://me.csdn.net/e01528)阅读数：522








**环境：**ASUS FL5900U，双显卡驱动HD620&GeoForce 940MX。






**1.安装NVIDIA**



    直接终端下输出以下命令，不需要禁用开源驱动和关闭lightdm。

```
sudo add-apt-repository ppa:graphics-drivers/ppa   
sudo apt-get update   
sudo apt-get upgrade
sudo apt-get install nvidia-384
```

    如果你选择安装CUDA（本人主要是用于caffe），可以选择直接安装CUDA，不需要安装NVIDIA驱动（CUDA中自带NVIDIA驱动）。[*但是要选择直接deb安装*](https://developer.nvidia.com/cuda-90-download-archive?target_os=Linux&target_arch=x86_64&target_distro=Ubuntu&target_version=1604&target_type=deblocal)，不要选择run安装，否则会出现无限登陆的问题。



**    注意：****不要选择**在软件和更新--附加驱动选项中，直接将驱动从默认的第三方nouveau驱动，修改为NVIDIA驱动，否则重启会导致the system is running in low graphic mode的问题！（我自己双显卡的电脑会出现这个问题）网上关于这个问题所有能看的，我都试过，结果全都失败了！！最后重装系统，改用以上正确的方法，最终才搞定！

    过来人的建议，如果你是用双系统，一般的东西都在windows下，如果遇到以上的问题，重装系统换种“打开方式”，解决问题会更快 !




![](https://img-blog.csdn.net/20180128161610276?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZTAxNTI4/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)





![](https://img-blog.csdn.net/20180128161918193?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZTAxNTI4/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)




**2.关闭安全模式**



    开机长按ESC，之后进bios选项，选择secure里面的securty boot修改为disable。 然后选择ubuntu系统进入之后，在终端下输出：nvidia-smi会显示你的显卡相关信息，证明你的NVIDIA驱动已经安装好了。也可以下搜索下搜索 详细信息，可以看到 图形下会显示你的NVIDIA的显卡型号，我的是GeoForce 940MX。

![](https://img-blog.csdn.net/20180128161747594?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZTAxNTI4/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


![](https://img-blog.csdn.net/20180128161842678?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZTAxNTI4/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)







