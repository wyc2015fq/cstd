# 找不到syntax.vim文件 - weixin_33985507的博客 - CSDN博客
2017年06月26日 11:01:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：32
新拿到一个centos 6.8 主机，没有vim，觉得不方便，就
> 
yum install vim
没想到装好后用vim打开文件报错，
E484: Can't open file /usr/share/vim/vim74/syntax/syntax.vim
查找发现在/usr/share下没有vim文件，yum reisntall vim也不行。
最后
> 
yum remove vim*
yum install vim
yum install vim*
然后就可以了，估计是以前装vim的时候出错了，或者误删了文件，重新安装一下就好了
yum install vim安装了三个包，如下图
![3824151-2e6467ad23dcb6cc.png](https://upload-images.jianshu.io/upload_images/3824151-2e6467ad23dcb6cc.png)
yum install vim*安装了4个包，如下图
![3824151-a256558ccd7656f0.png](https://upload-images.jianshu.io/upload_images/3824151-a256558ccd7656f0.png)
安装完成后用vim打开文件不再报错，查看上面的报错路径现在也有了，
#### 小技巧
编辑root用户家目录下的隐藏文件 
> 
vi  .bashrc
添加：
alias vi='vim',这样以后用vim就方便了
![3824151-5e9c95187a6c7e71.png](https://upload-images.jianshu.io/upload_images/3824151-5e9c95187a6c7e71.png)
