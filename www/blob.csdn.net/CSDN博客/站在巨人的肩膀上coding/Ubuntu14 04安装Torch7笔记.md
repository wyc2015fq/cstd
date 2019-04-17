# Ubuntu14.04安装Torch7笔记 - 站在巨人的肩膀上coding - CSDN博客





2018年04月27日 12:55:43[coding思想](https://me.csdn.net/LOVE1055259415)阅读数：87











利用快捷键Ctrl+Alt+T打开Ubuntu终端



第一步：

获取安装LuauJIT（C语言编写的Lua的解释器）和Torch所必需的依赖包。

代码如下：

`1 git clone https://github.com/torch/distro.git ~/torch --recursive`

如果提示git还未安装，在终端输入命令：

`sudo apt-get install git`

安装git

![](https://images2017.cnblogs.com/blog/743623/201708/743623-20170809172214714-1585792611.png)



![](https://images2017.cnblogs.com/blog/743623/201708/743623-20170809172313370-1374001040.png)

漫长的等待中。。。。。。。。。。。。。。。。。。。。。。。。。。。。。。。。



第二步：

刚才默认将依赖包下载在当前路径下的torch文件夹，进入torch文件夹，打开并执行install-deps中的命令。

代码如下：

```
cd ~/torch
bash install-deps
```

又得等待好久，可以去喝杯咖啡~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~





第三步：

执行install.sh文件

输入命令：
`./install.sh`
![](https://images2017.cnblogs.com/blog/743623/201708/743623-20170809172929339-132451180.png)

![](https://images2017.cnblogs.com/blog/743623/201708/743623-20170809172936902-880616778.png)

至此，安装torch需要的依赖已经搞定！！！！！！！！！！！！！！！！！！！！！！！！





第四步：

将路径添加到PATH变量中。

```
source ~/.bashrc
source ~/.zshrc
```

使用上面两个命令中任意一个即可。





第五步：

检测安装是否成功。在终端输入th命令，若出现下图，表明安装成功。

![](https://images2017.cnblogs.com/blog/743623/201708/743623-20170809173328667-1002484365.png)



附加：

利用粗暴删除来卸载torch：
`rm -rf ~/torch`


