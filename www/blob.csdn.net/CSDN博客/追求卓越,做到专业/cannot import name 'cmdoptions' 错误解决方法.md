
# cannot import name 'cmdoptions' 错误解决方法 - 追求卓越,做到专业 - CSDN博客


2018年12月17日 19:34:26[Waldenz](https://me.csdn.net/enter89)阅读数：238


在升级pip的时候，造成了pip损坏且发生错误。执行pip --version 命令仍然报出此错误
![](https://img-blog.csdnimg.cn/20181217192524334.png)
**解决方法如下：**
1. 首先执行命令：  python -m ensurepip --default-pip
![](https://img-blog.csdnimg.cn/20181217192850636.png)
2.下载get-pip.py文件，地址[https://bootstrap.pypa.io/get-pip.py](https://bootstrap.pypa.io/get-pip.py)
3. 从命令行进入到get-pip.py文件所在目录，执行命令： python get-pip.py
![](https://img-blog.csdnimg.cn/20181217192840737.png)
4. 执行 pip --version 查看pip是否安装成功
![](https://img-blog.csdnimg.cn/20181217193332361.png)

