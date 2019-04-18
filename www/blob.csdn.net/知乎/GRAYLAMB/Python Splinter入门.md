# Python Splinter入门 - 知乎
# 

![](https://pic2.zhimg.com/b2a1abd08800f743ac98a72bfce6f4fd_b.jpg)一直在通过业余时间研究python，并使用python尝试进行一些爬网操作。但有时候觉得通过python的urllib、urllib2库进行web操作真是很麻烦，cookie内容的匹配、浏览器的模拟、post/get模拟，真的是非常麻烦。在网上搜索了一下，开始接触Splinter，通过他可以方便的进行web操作了。因为他是实实在在的进行浏览器操作，所以终于可以不用花太多时间在“模拟”上，而是专注于你的web操作目的，让你事半功倍哦～
**一、安装篇**

安装基础：Mac OS X   python 2.7.10 环境

首先获取到splinter的安装介质：[https://pypi.python.org/pypi/splinter](https://link.zhihu.com/?target=https%3A//pypi.python.org/pypi/splinter)

目前获取到的是splinter-0.7.3版本：

![](https://pic1.zhimg.com/f6e6b4282c90a5b3de1cd2cd95c311f4_b.png)下载并解压这个tar包，会看到这样一个目录：

![](https://pic3.zhimg.com/1dfd537d433752346c77568c26e399b2_b.png)如果你之前安装过python的其它第三方库，比如beautifulsoup等，剩下的安装步骤就比较简单，直接敲命令：

# python setup.py install

如果有类似[Errno 13] Permission denied的错误，说明目前用户的权限不够，需要你使用管理员用户进行安装，可以采用下面的命令：

sudo python setup.py install

在输入管理员用户密码后，开始执行安装动作。

最后：Finished processing dependencies for splinter==0.7.3

安装好啦。

**二、应用篇**

安装成功后，可以进入python的IDE进行测试啦。

```
$ python
Python 2.7.10 (default, Oct 23 2015, 18:05:06) 
[GCC 4.2.1 Compatible Apple LLVM 7.0.0 (clang-700.0.59.5)] on darwin
Type "help", "copyright", "credits" or "license" for more information.
>>> from splinter.browser import Browser
>>> b = Browser()
>>>b.visit("http://www.baidu.com/")
```

理论上，浏览器应该会自己开启弹出，并打开[百度](https://link.zhihu.com/?target=http%3A//www.baidu.com/)。

但有的小伙伴可能会遇到这样的报错：

```
Traceback (most recent call last):
  File "<stdin>", line 1, in <module>
  File "splinter/browser.py", line 63, in Browser
    return driver(*args, **kwargs)
  File "splinter/driver/webdriver/firefox.py", line 39, in __init__
self.driver = Firefox(firefox_profile)
……
```

仔细看一下原因就清楚了，splinter默认支持的是Firefox的浏览器，因此如果你没有安装对相应版本的Firefox浏览器，就会报错。

我自己用的是chrome浏览器，所以自然得解决这个问题。上网查了一下，splinter对于chrome浏览器是可以支持的，但是需要安装chrome的驱动。这在splinter的doc网站上有详细说明：

[Chrome WebDriver](https://link.zhihu.com/?target=http%3A//splinter.readthedocs.org/en/latest/drivers/chrome.html)

好啦，首先找到chrome的驱动，天朝对于google的东西目前还多处于封锁期，因此，我从网上找到了一个国内可以访问的驱动地址：

[http://chromedriver.storage.googleapis.com/index.html](https://link.zhihu.com/?target=http%3A//chromedriver.storage.googleapis.com/index.html)

到这里，先不要急着安装，因为驱动的版本有很多，具体使用哪一个需要分析一下。

先看看本机的chrome版本：

![](https://pic3.zhimg.com/d0623c6fecd030660435d1a4c73df056_b.png)再看看网站的情况：

![](https://pic3.zhimg.com/7757774bb019a9a9cea3e35d94100456_b.png)有没有眼花缭乱的感觉。注意到下面的“最近发布”（Latest_Release），看来跟版本有关，这个一定要看了，里面写着“2.21”，所以2.21是最新的一个版本。

进入2.21的目录：

![](https://pic3.zhimg.com/3d59a76a8a26867fe8117b1d78483aea_b.png)查看notes.txt：

![](https://pic4.zhimg.com/ce6712cc2763c94c4a2cc7b2a8c746a7_b.png)里面写着2.21版本支持chrome v46-50。我的chrome版本是v47，那就是他了。

下载chromedriver_mac32.zip：

![](https://pic3.zhimg.com/01f95c87814c961d8a9f2989f68a8f12_b.png)这个鬼怎么安装呢？

回到splinter chrome webdriver的doc网站：

[Chrome WebDriver](https://link.zhihu.com/?target=http%3A//splinter.readthedocs.org/en/latest/drivers/chrome.html)

里面有不同操作系统的安装办法。对于Mac OS X，方法是先安装homebrew，在通过homebrew安装chromedriver。

![](https://pic1.zhimg.com/2ecbf6bcde49bb3ce789ff0d97ca7380_b.png)顺势点进上面homebrew的链接中：[Homebrew — The missing package manager for OS X](https://link.zhihu.com/?target=http%3A//brew.sh)

![](https://pic2.zhimg.com/4c2951e8528ef39d6e42e3bc864d19e5_b.png)按照安装homebrew的指引，在终端窗口中输入这个语句：

```
/usr/bin/ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"
```

开始安装，终端上会提示你安装时做哪些动作。

```
$ /usr/bin/ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"
==> This script will install:
/usr/local/bin/brew
/usr/local/Library/...
/usr/local/share/man/man1/brew.1
==> The following directories will be made group writable:
/usr/local/.
==> The following directories will have their owner set to fintecher:
/usr/local/.
==> The following directories will have their group set to admin:
/usr/local/.
Press RETURN to continue or any other key to abort
```

安心下一步吧。同时，需要提示一下，安装前需要给Mac安装好Xcode，因为部分安装动作使用到了xcode的命令，否则你就会遇到这样的提示，并中断：

```
==> /usr/bin/sudo /usr/bin/xcode-select --install
xcode-select: note: install requested for command line developer tools
Press any key when the installation has completed.
==> Downloading and installing Homebrew...
xcode-select: note: no developer tools were found at '/Applications/Xcode.app', requesting install. Choose an option in the dialog to download the command line developer tools.
```

安装完毕会提示下面的文字，好极了～

```
==> Installation successful!
==> Next steps
Run `brew help` to get started
```

下面开始安装chromedrive，运行：$ brew install chromedriver

![](https://pic4.zhimg.com/c34df0615392c0bfcba0e24f3cf2d627_b.png)安装完毕，小激动。

再去python IDE中实验一下吧：

```
>>> from splinter.browser import Browser
>>> b = Browser("chrome")
```

![](https://pic1.zhimg.com/807da8e978eaa62db8aad30a29d7e244_b.png)chrome打开啦～～

再试试

`>>>b.visit("http://www.baidu.com/")`

![](https://pic2.zhimg.com/8246b2ff95e4f319769f7e60a275be59_b.png)**三、结语**

本文简单总结了一下splinter的安装过程。对于技术大牛，这篇文章可以忽略掉，主要还是给python splinter初学者的一个入门指导～

后面有时间的话，会结合一些实际场景（如尝试登录网站、抓取信息、点击秒杀等）再写一些入门教程。欢迎关注～

