# 编码自动识别工具 uchardet - 在思索中前行！ - CSDN博客





2014年08月17日 14:01:22[_Tham](https://me.csdn.net/txl16211)阅读数：1545








        本文转自网络


        最近在给[OpenCC](http://opencc.byvoid.com)做图形界面，遇到一个问题：OpenCC默认只能转换utf-8文本，其他编码像GB18030，BIG5只能转换成utf-8以后，才能用OpenCC转换。这个问题说大不大，说小也不小。我完全可以增加一个选项，在打开的时候让用户选择文本编码，然后再转换就行了，但这却给用户非常糟糕的体验，因为很多非专业用户根本不知道什么是文本编码，更别说辨别了。GB18030/BIG5硬要用utf-8打开的话，肯定会遇到乱码。由于Windows默认是GB18030/BIG5编码，一般情况下文本会被保存成默认编码，这样更大大增加了用户遇到乱码的概率。为了提高体验，我计划实现文本编码的自动检测。

最早接触到编码是从做网站开始的，记得如果忘了在head中显式地向浏览器指定编码，就经常会出现乱码，但乱码也并不总是出现，这是怎么回事呢？浏览器还是有自动识别的能力的。发现Firefox浏览器中有一个编码选项，里面有「自动检测」，使用它绝大多数时候都能正确识别。

事实上纯文本的编码检测是一个非常复杂的问题，甚至理论上根本不可能实现。确切地说，「检测」应该叫「探测」或者「推测」才更恰当。自动编码探测的实现原理主要是统计学的方法，每个编码会有一定的特征，首先检测特征是否符合，再使用常用的匹配，类似于蒙特卡罗法。具体方法可以参考[Mozilla](http://www.mozilla.org/projects/intl/UniversalCharsetDetection.html)。

mozilla在很多年前就做了一个非常优秀的编码检测工具，叫chardet，后来有发布了算法更加优秀的universalchardet，用于Firefox的自动编码识别。我想，这么出名的一个工具，应该肯定已经有不少人在用了。有意思的是，我在网上找到了chardet和universalchardet的各种移植：
- [python-chardet](http://chardet.feedparser.org/) Python 移植
- [ruby-rchardet](http://rubyforge.org/projects/chardet/) Ruby 移植
- [juniversalchardet](http://code.google.com/p/juniversalchardet/) Java 移植 universalchardet
- [jchardet](http://jchardet.sourceforge.net/) Java 移植 chardet
- [nuniversalchardet](http://code.google.com/p/nuniversalchardet/) C# 移植 universalchardet
- [nchardet](http://www.conceptdevelopment.net/Localization/NCharDet/) C# 移植 chardet

惟一没有的，竟然是C/C++的接口封装。debian更是收录了python-chardet和ruby-rchardet，却没有libchardet或者libuniversalchardet。莫非没有C/C++的应用在使用chardet吗？用强大的Google代码搜索，发现的确有，但几乎都是把chardet的代码内嵌到了项目中，耦合十分紧密。更有直接调用python-chardet的，实现不够纯净。

总觉不该是这样，但经过反复确认，真的没有一个独立的universalchardet的C函数库封装。还是自己动手好了，我从mozilla上面取下来了代码，做了一点点补丁，写了一个接口和命令行界面，取名uchardet，大功告成。测试了一些GB18030和UTF8的文本，感觉准确率非常高，而且速度很快。但是当我试图识别几个字节的短文本的时候，却出现了识别错误，开始以为是我的错，后来发现我用Firefox直接打开，也是无法识别的，而且错误识别的编码一样。看来是上游的问题，应该是算法本身的缺陷吧。想想看，毕竟文本越短歧义的可能性越强。不过既然能达到和Firefox同样的水平，一般应用也就够了。

项目主页在Google code上：

[http://code.google.com/p/uchardet/](http://code.google.com/p/uchardet/)

代码在github上：

[https://github.com/BYVoid/uchardet](https://github.com/BYVoid/uchardet)

我为什么用universalchardet？其实编码自动识别的解决方案不止一个，有[icu](http://icu-project.org/)提供的解决方案，IE也有API，还有已经在很多Linux发行版中的[enca](http://freshmeat.net/projects/enca/)。我之所以用universalchardet，是因为它是最合适的。IE的API不能跨平台，icu实现太庞大，enca是GPL（注意不是LGPL），使用它意味著我也要让我的所有源码使用GPL，而不是更加开放的Apache。universalchardet是MPL的，和LGPL差不多宽松，使用它是没有问题的。我非常不喜欢以GPL发布的函数库，这给开发者的限制太大了。



