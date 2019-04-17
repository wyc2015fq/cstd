# 你还在用 os.path？快来感受一下 pathlib 给你带来的便捷吧！ - zhusongziye的博客 - CSDN博客





2018年12月08日 13:43:24[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：219标签：[你还在用 os.path？快来感受一下 pathlib 给你](https://so.csdn.net/so/search/s.do?q=你还在用 os.path？快来感受一下 pathlib 给你&t=blog)
个人分类：[Python语言](https://blog.csdn.net/zhusongziye/article/category/7131144)









# 概述

pathlib 是Python内置库，Python 文档给它的定义是 Object-oriented filesystem paths（面向对象的文件系统路径）。pathlib 提供表示文件系统路径的类，其语义适用于不同的操作系统。路径类在纯路径之间划分，纯路径提供纯粹的计算操作而没有I / O，以及具体路径，它继承纯路径但也提供I / O操作。

听起来有点绕？那就对了，毕竟这是直译过来的，但这并不影响我们喜爱它。

我们通过几个例子来了解它吧

# 举个栗子

相对于 os 模块的 path 方法，Python3 标准库 pathlib 模块的 Path 对路径的操作会更简单。

## 获取当前文件路径

使用 os 模块时，有两种方法可以直接获取当前文件路径：
`import os value1 = os.path.dirname(__file__) value2 = os.getcwd() print(value1) print(value2)`

![](https://img-blog.csdnimg.cn/20181208133755492.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)

pathlib 获取当前文件路径应该怎么写呢？

官方文档给出了建议 插眼传送

![](https://img-blog.csdnimg.cn/20181208133814837.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)

动手试一试

```
import pathlib

value1 = pathlib.Path.cwd()
print(value1)
```

![](https://img-blog.csdnimg.cn/20181208133838582.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)

### 它是如何实现的

文档中有介绍，它以 os.getcwd() 的形式将路径返回。我们去源码中一探究竟（Pycharm 编辑器快捷键 ctrl+鼠标左键点击即可跟进指定对象）

![](https://img-blog.csdnimg.cn/20181208133856518.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)

原来它是对 os 模块中一些对象进行了封装，看 cwd 的注释：
`Return a new path pointing to the current working directory`

意为：返回指向当前工作目录的新路径。

看起来也没什么特别的，但是为什么官方特意将它推出呢？

### 其他的封装

pathlib 封装了很多的 os path ，文档中有写明，如：

```
# 关系说明
 os.path.expanduser() --> pathlib.Path.home()

 os.path.expanduser() --> pathlib.Path.expanduser()

 os.stat() --> pathlib.Path.stat()

 os.chmod() --> pathlib.Path.chmod()
```

官网文档截图：

![](https://img-blog.csdnimg.cn/20181208133917327.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)

详细请查看官方文档:插眼传送

## 再举几个栗子

刚才的案例并不能说明什么，只是让我们了解到 pathlib 的构成，接下来让我们感受一下它带给我们的便捷。

### 获取上层/上层目录

也就是获取它爷爷的名字

![](https://img-blog.csdnimg.cn/20181208133936745.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)



os 模块的写法为：

```
import os

print(os.path.dirname(os.path.dirname(os.getcwd())))
```

![](https://img-blog.csdnimg.cn/20181208133952866.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)

如果用 pathlib 来实现：

```
import pathlib

print(pathlib.Path.cwd().parent.parent)
```

![](https://img-blog.csdnimg.cn/20181208134010198.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)

parent 就完事了，这是不是更贴近 Pythonic ？ 像写英语一样写代码。

如果你只需要找到它爸爸，那就使用一次：

```
import pathlib

print(pathlib.Path.cwd().parent)
```

你还可以继续往祖辈上找：

```
import pathlib

print(pathlib.Path.cwd().parent.parent.parent)
```

![](https://img-blog.csdnimg.cn/20181208134029131.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)

相对与之前 os 模块使用的多层 os.path.dirname，使用 parent 是不是便捷很多？

### 路径拼接

如果你要在它爷爷辈那里拼接路径，那么你需要写这么长一串代码：

```
import os

print(os.path.join(os.path.dirname(os.path.dirname(os.getcwd())), "关注", "微信公众号", "【进击的", "Coder】"))
```

![](https://img-blog.csdnimg.cn/20181208134052362.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)

当你用 pathlib 的时候，你一定能够感受到快乐：

```
import pathlib

parts = ["关注", "微信公众号", "【进击的", "Coder】"]
print(pathlib.Path.cwd().parent.parent.joinpath(*parts))
```

而且你还可以通过增加或减少 parent 的数量，来实现它祖辈的调节，美哉。

# PurePath

上面的操作大部分都通过 pathlib 中的 Path 实现，其实它还有另一个模块 PurePath。

> 
PurePath 是一个纯路径对象，纯路径对象提供了实际上不访问文件系统的路径处理操作。有三种方法可以访问这些类，我们也称之为flavor。


上面这句话来自于官方文档，听起来还是有点绕，我们还是通过栗子来了解它吧

### PurePath.match

让我们来判断一下，当前文件路径是否有符合 '*.py' 规则的文件

```
import pathlib

print(pathlib.PurePath(__file__).match('*.py'))
```

![](https://img-blog.csdnimg.cn/20181208134118480.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)

很显然，我们编写代码的 coder.py 就符合规则，所以输出是 True。

为什么我要拿这个来举例呢？

再深入想一下 pathlib.PurePath 后面能够跟着 match，那说明它应该是个对象，而不是一个路径字符串。

为了验证这个想法，把代码改一改：

```
import pathlib
import os


os_path = os.path.dirname(__file__)
pure_path = pathlib.PurePath(__file__)
print(os_path, type(os_path))
print(pure_path, type(pure_path))
print(pathlib.PurePath(__file__).match('*.py'))
```

![](https://img-blog.csdnimg.cn/20181208134134514.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)



打印通过 os.path 获取当前路径的结果，得出一个路径字符串；而通过 pathlib.Pure 则获得的是一个 **PurePosixPath** 对象，并且得到的路径包括了当前文件 coder.py。

这就有点悬疑了， PurePosixPath 究竟是什么？

pathlib 可以操作两种文件系统的路径，一种是 Windows 文件系统，另一种称为非 Windows 文件系统，对应的对象是 pathlib.PurePosixPath 和 PureWindowsPath，不过不用担心，这些类并非是指定在某些操作系统上运行才能够使用，无论你运行的是哪个系统，都可以实例化所有这些类，因为它们不提供任何进行系统调用的操作。

不提供任何进行系统调用的操作，这又是什么？真是越听越深了

文档在最开始给出了这么一段描述:

> 
Pure paths are useful in some special cases; for example:

If you want to manipulate Windows paths on a Unix machine (or vice versa). You cannot instantiate a WindowsPath when running on Unix, but you can instantiate PureWindowsPath.

You want to make sure that your code only manipulates paths without actually accessing the OS. In this case, instantiating one of the pure classes may be useful since those simply don’t have any OS-accessing operations.





翻译：纯路径在某些特殊情况下很有用; 例如：

如果要在Unix计算机上操作Windows路径（反之亦然）。WindowsPath在Unix上运行时无法实例化，但可以实例化PureWindowsPath。

您希望确保您的代码仅操作路径而不实际访问操作系统。在这种情况下，实例化其中一个纯类可能很有用，因为那些只是没有任何操作系统访问操作。


还附上了一张图：

![](https://img-blog.csdnimg.cn/20181208134158344.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)

一下子也不是很理解，这是什么意思。不要紧，继续往下看。

# 对应关系

通过以上的例子我们可以感受到，它不仅封装了 os.path 相关常用方法，还集成了 os 的其他模块，比如创建文件夹 Path.mkdir。

如果你担心记不住，没关系的，文档一直都在。并且文档给我们列出了对应关系表

![](https://img-blog.csdnimg.cn/2018120813420976.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)

### 基本用法

Path.iterdir()　　# 遍历目录的子目录或者文件

Path.is_dir()　　# 判断是否是目录

Path.glob()　　# 过滤目录(返回生成器)

Path.resolve()　　# 返回绝对路径

Path.exists()　　# 判断路径是否存在

Path.open()　　# 打开文件(支持with)

Path.unlink()　　# 删除文件或目录(目录非空触发异常)

### 基本属性

Path.parts　　# 分割路径 类似os.path.split(), 不过返回元组

Path.drive　　# 返回驱动器名称

Path.root　　# 返回路径的根目录

Path.anchor　　# 自动判断返回drive或root

Path.parents　　# 返回所有上级目录的列表

### 改变路径

Path.with_name()　　# 更改路径名称, 更改最后一级路径名

Path.with_suffix()　　# 更改路径后缀

### 拼接路径

Path.joinpath()　　# 拼接路径

Path.relative_to()　　# 计算相对路径

### 测试路径

Path.match()　　# 测试路径是否符合pattern

Path.is_dir()　　# 是否是文件

Path.is_absolute()　　# 是否是绝对路径

Path.is_reserved()　　# 是否是预留路径

Path.exists()　　# 判断路径是否真实存在

### 其他方法

Path.cwd()　　# 返回当前目录的路径对象

Path.home()　　# 返回当前用户的home路径对象

Path.stat()　　# 返回路径信息, 同os.stat()

Path.chmod()　　# 更改路径权限, 类似os.chmod()

Path.expanduser()　　# 展开~返回完整路径对象

Path.mkdir()　　# 创建目录

Path.rename()　　# 重命名路径

Path.rglob()　　# 递归遍历所有子目录的文件

# pathlib 回顾

通过上面的几个例子，我们对 pathlib 应该有一个大体的了解，接下来再回顾一下官方给 pathlib 库的定义：

> 
This module offers classes representing filesystem paths with semantics appropriate for different operating systems. Path classes are divided between pure paths, which provide purely computational operations without I/O, and concrete paths, which inherit from pure paths but also provide I/O operations.

释义：pathlib 提供表示文件系统路径的类，其语义适用于不同的操作系统。路径类在纯路径之间划分，纯路径提供纯粹的计算操作而没有I / O，以及具体路径，它继承纯路径但也提供I / O操作。


回顾刚才这张图，重新理解 pathlib

![](https://img-blog.csdnimg.cn/20181208134227403.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)

如果你以前从未使用过这个模块，或者只是不确定哪个类适合您的任务，那么Path很可能就是您所需要的。它为代码运行的平台实例化一个具体路径。

**总结：pathlib 不单纯是对 os  中一些模块或方法进行封装，而是为了兼容不同的操作系统，它为每类操作系统定义了接口。你希望在UNIX机器上操作Windows的路径，然而直接操作是做不到的，所以为你创建了一套接口 PurePath，你可以通过接口来实现你的目的（反之亦然）**



