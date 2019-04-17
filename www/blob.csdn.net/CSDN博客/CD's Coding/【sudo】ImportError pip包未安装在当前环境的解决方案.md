# 【sudo】ImportError: pip包未安装在当前环境的解决方案 - CD's Coding - CSDN博客





2019年01月16日 21:58:04[糖果天王](https://me.csdn.net/okcd00)阅读数：135标签：[sudo																[pip																[install																[root																[import error](https://so.csdn.net/so/search/s.do?q=import error&t=blog)
个人分类：[DIY																[开发备忘](https://blog.csdn.net/okcd00/article/category/2611293)](https://blog.csdn.net/okcd00/article/category/1775037)





## 0x00 前言

通常在使用 pip 指令安装 Python 需求的包时，会需要使用 `sudo pip install` 获取权限，

或者，在 Jupyter Notebook 里可以采用 `!` 的语法调用 pip 指令直接安装需要的包，例如：

```
! pip install xlrd
! sudo pip install xlrd
```

但有时安装后发现当前环境或虚拟环境（conda-env）中，

其实是因为 sudo 指令后的指令实际上不是在当前环境的原因。

## 0x01 实例测试

> 
以实际操作情况举例，目标为在 `tf1.10` 环境下安装 `xlrd` 包


**情境重现**

```
(tf1.10) [chend@cx ~] $ python -c "import xlrd"  # ImportError，缺少xlrd包
(tf1.10) [chend@cx ~] $ pip install xlrd  # 安装失败，权限不够，需要sudo提权
(tf1.10) [chend@cx ~] $ sudo pip install xlrd  # 成功安装
(tf1.10) [chend@cx ~] $ python -c "import xlrd"  # ImportError，缺少xlrd包
```

**解决方案**

```
(tf1.10) [chend@cx ~] $ source deactivate  # 退出当前虚拟环境
[chend@cx ~] $ sudo su  # 提权到root
[root@cx ~] # which python  # /bin/python
[root@cx ~] # source /etc/profile  # root 重载环境
[root@cx ~] # which python  # /usr/local/anaconda2/...
[root@cx ~] # source activate tf1.10  # 进入需要安装的虚拟环境
(tf1.10) [root@cx ~] # pip install xlrd  # 在虚拟环境中安装需求的包
(tf1.10) [root@cx ~] # su chend  # 退出root，回到一般用户
(tf1.10) [chend@cx ~] $ python -c "import xlrd"  # 成功 import
```

## 0x02 如何重载环境变量

环境变量在 `/etc/profile` 文件中，通常是在系统启动时会重载，

那么在不重启系统的时候需要重载系统变量（如上述中为 root 用户重载 python 位置）

方法1：

让/etc/profile文件修改后立即生效 ,可以使用如下命令:

# .  /etc/profile
方法2：

让/etc/profile文件修改后立即生效 ,可以使用如下命令:

# source /etc/profile
附：Linux中source命令的用法

source命令：

source命令也称为“点命令”，也就是一个点符号（.）。

source命令通常用于重新执行刚修改的初始化文件，使之立即生效，而不必注销并重新登录。

用法：

source filename 或 . filename （`.` 后有空格）](https://so.csdn.net/so/search/s.do?q=root&t=blog)](https://so.csdn.net/so/search/s.do?q=install&t=blog)](https://so.csdn.net/so/search/s.do?q=pip&t=blog)](https://so.csdn.net/so/search/s.do?q=sudo&t=blog)




