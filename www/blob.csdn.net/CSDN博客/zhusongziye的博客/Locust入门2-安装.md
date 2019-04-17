# Locust入门2-安装 - zhusongziye的博客 - CSDN博客





2018年10月28日 19:44:13[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：70








转自：http://www.testclass.net/

Locust 是基于 Python 语言的一个性能测试库，如果要想使用它来做性能测试必须要先安装 Python 。

Locust 安装

方式一：通过 pip 命令安装

```
> pip install locust

Collecting locust
  Downloading locust-0.8.tar.gz (225kB)    59% |███████████████████             | 133kB 199kB/s eta 0:00:0
    63% |████████████████████▍           | 143kB 304kB/s eta 0:00:    68% |█████████████████████▉          | 153kB 358kB/s eta 0:0
    72% |███████████████████████▎        | 163kB 355kB/s eta 0:    77% |████████████████████████▊       | 174kB 421kB/s eta    81% |██████████████████████████▏     | 184kB 449kB/s eta    86% |███████████████████████████▋    | 194kB 439kB/s e    90% |█████████████████████████████   | 204kB 487kB/s    95% |██████████████████████████████▌ | 215kB 492kB/s    99% |████████████████████████████████| 225kB 487kB    100% |████████████████████████████████| 235kB 417k

    ....
```

方式二：GitHub下载安装

GitHub项目地址：https://github.com/locustio/locust/

将项目克隆下来，通过Python 执行 setup.py 文件

```
...\locust> python setup.py install

running install
running bdist_egg
running egg_info
creating locustio.egg-info
writing locustio.egg-info\PKG-INFO
writing dependency_links to locustio.egg-info\dependency_links.txt
writing entry points to locustio.egg-info\entry_points.txt
writing requirements to locustio.egg-info\requires.txt
writing top-level names to locustio.egg-info\top_level.txt
writing manifest file 'locustio.egg-info\SOURCES.txt'...
```

最后，检查是否安装成功。打开Windows命令提示符，输入 “locust –help” 回车。

```
> locust --help

Usage: locust [options] [LocustClass [LocustClass2 ... ]]

Options:
  -h, --help            show this help message and exit...
```

> 
每个参数的含义，将会放到后面介绍。


安装依赖分析

这里想简单介绍 Locust 都基于了哪些库。打开 Locust 安装目录下的 setup.py 文件。查看安装要求：

install_requires=[“gevent>=1.1.2”, “flask>=0.10.1”, “requests>=2.9.1”, “msgpack-python>=0.4.2”, “six>=1.10.0”, “pyzmq==15.2.0”]

gevent 是在 Python 中实现协程的一个第三方库。协程，又称微线程（Coroutine）。使用gevent可以获得极高的并发性能。

flask 是 Python 的一个 Web 开发框架。

Requests 用来做 HTTP 接口测试。

msgpack-python 是一种快速、紧凑的二进制序列化格式，适用于类似JSON的数据。

six 提供了一些简单的工具用来封装 Python2 和 Python3 之间的差异性。

pyzmq 如果你打算运行 Locust 分布在多个进程/机器，建议你安装pyzmq。

当我们在安装 Locust 时，它会检测我们当前的 Python 环境是否已经安装了这些库，如果没有安装，它会先把这些库一一装上。并且对这些库版本有要求，有些是必须等于某版本，有些是大于某版本。我们也可以事先把这些库全部按要求装好，再安装Locust时就会快上许多。



