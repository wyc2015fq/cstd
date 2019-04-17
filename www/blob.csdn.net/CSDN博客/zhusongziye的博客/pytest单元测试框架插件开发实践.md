# pytest单元测试框架插件开发实践 - zhusongziye的博客 - CSDN博客





2018年10月20日 16:56:19[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：226








我学习了一下pytest。

按照惯例，检验我学习成果的方法就是造个轮子，于是我根据写单元测试时发现的痛点，写了一个插件

```
https://github.com/ShichaoMa/pytest-apistellar
```

现在分享出来，供大家学习和参考。

既然说到痛点，想必写过单元测试的人都知道，由于我们有时无法直接访问数据库等服务，我们需要mock掉一些方法和属性，但mock是一个很痛苦的事情，而且写起来也相当于不优雅，而我这个插件解决的主要问题就是减轻mock带来的痛苦。通过一个装饰器，mock数据简直不费吹灰之力。

看名字你肯定可以猜的出来，这个插件其实是为我的异步web框架开发的：

```
https://github.com/ShichaoMa/apistellar
```

没错，apistellar前身就是star-builder。我之前写过几篇文章介绍过他。因为同事告诉我star-builder简称SB太过于美妙，于是一气之下我就把这个包重命名了。现在apistellar在我们公司内部已经得到了广泛使用，作为少有的ASGI框架，有兴趣的同学可以学习一下。

言归正传，作为apistellar的测试插件，最初肯定是为apistellar服务的，但后来感觉mock是所有人的痛点，由于我们公司好多服务还是基于python2的，于是我针对mock这个功能做了一下兼容。当然mock只是其功能之一，同时他还支持针对apistellar web服务的接口测试，简而言之，他可以启动一个apistellar的服务来测试某个模块提供的RESTful接口。

当然这节主讲更通用的单元测试mock。

现在我们有一个模块，他的包地址为file.file::File

```
class File(object):

    def __init__(self, filename):
        self.filename = filename

    @classmethod
    def load(cls):
        return cls(filename="这个文件是从数据库获取的")

    @classmethod
    async def load_async(cls):
        return cls(filename="这个文件是从数据库获取的")
```

其中的两个load方法都数据库中获取数据返回一个File对象，现在我们需要mock掉load操作，让其返回我们渴望的数据。

怎么办？

pytest传统的做法应该是这样：

```
from  file.file import File
def test_load(monkeypatch)：
    def load():
        return File("同步返回的文件")
    with monkeypatch.context() as m:
        m.setattr(File, "load", load)
        assert File.load().filename == "同步返回的文件"

    async def load_async():
        return File("异步返回的文件")
    with monkeypatch.context() as m:
        m.setattr(File, "load_async", load_async)
        assert (await File.load_async()).filename == "异步返回的文件"
```

我们需要使用monkeypatch改掉我们想mock的那个方法和属性，这样的实现很不优雅，存在很多多余的函数，让代码显示相当冗余。

现在不同了，我们有了pytest-apistellar，让我们看看他是怎么做的：

```
######为了发现在当前目录下创建的file包手动将当前目录加入环境变量######
import os
import sys
sys.path.insert(0, os.path.dirname(__file__))
#####正常测试我们的项目应该已存在于pythonpath中所以不需要这么写######
import pytest

from file.file import File


# 如果mock掉的方法不需要逻辑判断，直接使用ret_val指定返回值即可
@pytest.mark.usefixtures("mock")
@pytest.mark.prop("file.file.File.load", ret_val=File("直接mock返回值"))
def test_load1():
    assert File.load().filename == "直接mock返回值"


# 无论被mock的方法是异步还是同步，无脑指定返回值
# 不过异步的方法测试需要pytest-asyncio这个包支持，指定一个asyncio的mark才可以
@pytest.mark.asyncio
@pytest.mark.usefixtures("mock")
@pytest.mark.prop("file.file.File.load_async", ret_val=File("异步方法也能mock"))
async def test_load2():
    assert (await File.load_async()).filename == "异步方法也能mock"


# 如果我们用来替代的mock方法有一定的逻辑，我们可以指定一个ret_factory，指向替代方法的包地址
@pytest.mark.asyncio
@pytest.mark.usefixtures("mock")
@pytest.mark.prop("file.file.File.load_async", ret_factory="factories.mock_load")
async def test_load3():
    assert (await File.load_async()).filename == "通过工厂mock"


# 这种就是有业务逻辑的替代方法，他可以通过filename不同返回不同的File对象
@pytest.mark.usefixtures("mock")
@pytest.mark.prop("file.file.File.load", ret_factory="factories.mock_load", filename="还能给工厂传参")
def test_load4():
    assert File.load().filename == "还能给工厂传参"


# 装饰器太长怎么办，取个别名来缩短导包参数
from pytest_apistellar import prop_alias

file = prop_alias("file.file.File", "factories")


@pytest.mark.usefixtures("mock")
@file("load", ret_factory="mock_load", filename="使用别名装饰器，把前缀连起来")
def test_load4():
    assert File.load().filename == "使用别名装饰器，把前缀连起来"


# module作用域的mock
pytestmark = [
        file("load", ret_factory="mock_load", filename="这是一个module作用域的mock")
    ]


@pytest.mark.usefixtures("mock")
def test_load5():
    assert File.load().filename == "这是一个module作用域的mock"


@pytest.mark.usefixtures("mock")
class TestLoad(object):
    # class作用域的mock
    pytestmark = [
        file("load", ret_factory="mock_load", filename="这是一个class作用域的mock")
    ]

    def test_load6(self):
        assert File.load().filename == "这是一个class作用域的mock"

    # funtion作用域的mock
    @file("load", ret_factory="mock_load", filename="这是一个function作用域的mock")
    def test_load7(self):
        assert File.load().filename == "这是一个function作用域的mock"
```

factories模块的代码在这:

```
from file.file import File


def mock_load(filename="通过工厂mock"):
    return File(filename)
```

正如注释所写，pytest-apistellar支持五个pytest作用域的mock，对于简单mock，直接在装饰器加入返回值数据就可以。同时pytest-apistellar还支持mock环境变量。具体的可以自行查看README。

```
https://github.com/ShichaoMa/pytest-apistellar
```

感谢阅读！



