# Linux下切换python版本 - 别说话写代码的博客 - CSDN博客





2017年07月18日 09:46:04[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：1880







转自：http://www.cnblogs.com/rhjeans/p/5499193.htmlhttp://www.cnblogs.com/tigerm/p/5794386.html
当你安装 Debian Linux 时，安装过程有可能同时为你提供多个可用的 Python 版本，因此系统中会存在多个 Python 的可执行二进制文件。你可以按照以下方法使用 ls 命令来查看你的系统中都有那些 Python 的二进制文件可供使用。

$ ls/usr/bin/python*
/usr/bin/python /usr/bin/python2 /usr/bin/python2.7/usr/bin/python3 /usr/bin/python3.4/usr/bin/python3.4m/usr/bin/python3m

执行如下命令查看默认的 Python 版本信息:

$ python --version
Python2.7.8

1、基于用户修改 Python 版本：

想要为某个特定用户修改 Python 版本，只需要在其 home 目录下创建一个 alias(别名) 即可。打开该用户的 ~/.bashrc 文件，添加新的别名信息来修改默认使用的 Python 版本。

alias python='/usr/bin/python3.4'

一旦完成以上操作，重新登录或者重新加载 .bashrc 文件，使操作生效。

$ .~/.bashrc

检查当前的 Python 版本。

$ python --version
Python3.4.2

2、 在系统级修改 Python 版本

我们可以使用 update-alternatives 来为整个系统更改 Python 版本。以 root 身份登录，首先罗列出所有可用的 python 替代版本信息：

# update-alternatives --list python
update-alternatives: error:no alternatives for python

如果出现以上所示的错误信息，则表示 Python 的替代版本尚未被 update-alternatives 命令识别。想解决这个问题，我们需要更新一下替代列表，将 python2.7 和 python3.4 放入其中。

# update-alternatives --install /usr/bin/python python /usr/bin/python2.7 1
update-alternatives:using/usr/bin/python2.7 to provide /usr/bin/python (python)inauto mode
# update-alternatives --install /usr/bin/python python /usr/bin/python3.4 2
update-alternatives:using/usr/bin/python3.4 to provide /usr/bin/python (python)inauto mode

--install 选项使用了多个参数用于创建符号链接。最后一个参数指定了此选项的优先级，如果我们没有手动来设置替代选项，那么具有最高优先级的选项就会被选中。这个例子中，我们为 /usr/bin/python3.4 设置的优先级为2，所以 update-alternatives 命令会自动将它设置为默认 Python 版本。

# python --version
Python3.4.2

接下来，我们再次列出可用的 Python 替代版本。

# update-alternatives --list python
/usr/bin/python2.7
/usr/bin/python3.4

现在开始，我们就可以使用下方的命令随时在列出的 Python 替代版本中任意切换了。

# update-alternatives --config python

# python --version
Python2.7.8

3、移除替代版本

一旦我们的系统中不再存在某个 Python 的替代版本时，我们可以将其从 update-alternatives 列表中删除掉。例如，我们可以将列表中的 python2.7 版本移除掉。

# update-alternatives --remove python /usr/bin/python2.7
update-alternatives: removing manually selected alternative - switching python to auto mode
update-alternatives:using/usr/bin/python3.4 to provide /usr/bin/python (python)inauto mode
- 源码安装新的python版本，我的安装路径： /usr/self/Python3.5.2
- 修改软链接到你所安装的python版本中：- 默认python命令是在/usr/bin/目录下
```
1 sudo mv /usr/bin/python /usr/bin/python2.7.0                  
2 sudo ln -s /usr/self/Python3.5.2/bin/python3 /usr/bin/python  #把系统默认python命令改成python3
```


- 在终端输入“python --version”即可查看python版本更改为3.5.2







