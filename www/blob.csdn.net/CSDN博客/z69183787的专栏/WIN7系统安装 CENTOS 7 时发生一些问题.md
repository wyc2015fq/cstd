# WIN7系统安装 CENTOS 7 时发生一些问题 - z69183787的专栏 - CSDN博客
2014年09月06日 15:27:07[OkidoGreen](https://me.csdn.net/z69183787)阅读数：9831
                
U盘安装CentOS 7提示 “Warning: /dev/root does not exist, could not boot” 解决办法
1、把U盘的Lable（即标签）修改成centos
2、在安装界面上按TAB键，修改启动路径，把”CENTOS\x207\x20x86_64″改成 “centos”

安装完成后 win7 启动项丢失：
在装完CentOS7.0之后，会发现以前装好的win7亦或者是其它系统没有出现在启动项中（6.5有更新多系统grub的步骤，7.0 LZ暂时没发现），这个时候也不要心急，我们只需要到grub2的配置文件里添加一点script就好啦..
![](http://imgsrc.baidu.com/forum/w%3D580/sign=29de4f6b013b5bb5bed720f606d1d523/8bf756da81cb39dbb7172996d2160924a91830c9.jpg)
![](http://imgsrc.baidu.com/forum/w%3D580/sign=367178cb59b5c9ea62f303ebe53bb622/07e986cb39dbb6fd508cabe60b24ab18952b37c9.jpg)
7.0和6.5相比变化还是挺大的，我们去/boot/目录下查看就会发现很大的不同，LZ经过了一会的摸索，在/boot/grub2/里找到了grub的配置文件 grub.cfg ..
![](http://imgsrc.baidu.com/forum/w%3D580/sign=2c8f1da9272dd42a5f0901a333395b2f/842809f41bd5ad6e26612c2a83cb39dbb7fd3c0d.jpg)
那我们就用vim打开文件来看看吧..原来里面是这个样子..大致看了一下脚本，成功找到了修改启动菜单选项的地方..
![](http://imgsrc.baidu.com/forum/w%3D580/sign=9cfc58ddd62a283443a636036bb7c92e/58c71cd5ad6eddc4ad7ffc3b3bdbb6fd5366330d.jpg)
![](http://imgsrc.baidu.com/forum/w%3D580/sign=d888a9da2a34349b74066e8df9e81521/4de6aa6eddc451da7c6e442bb4fd5266d116320d.jpg)

