# Python在测试中的用途 - zhusongziye的博客 - CSDN博客





2017年11月29日 21:16:50[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：1339









Python语言是一种面向对象的用途广泛的变成语言，语法清晰，可以完成许多任务，由于海量的第三方库的存在，实现的功能非常多也非常的容易。








Python是一种动态的脚本语言，学习成本大大低于Java等编程语言，因此对于我们测试人员来说，如果需要掌握一门语言，当然


Python是最好的选择；




Python丰富的第三方库，几乎想要的功能，都能找到对应的库，而我们应用这些库的时候，只考虑怎么去使用这些库提供的API而不用考虑怎么去实现，极少出现需要二次开发的情况；




Python开发效率很高，同样的任务大约是java的10倍，C/C++的10-20倍，当然这个统计来源于一些编程人员的感受，并不是精确的数值，但是也可以看出其开发效率之高。我们测试人员一般都会主要以手工测试为主，因此不可能全职来运用某种语言实现，因此肯定需要选择效率高的，人生苦短，我用Python；




Python支持多种系统，linux、mac上都默认安装了Python，windows更不用说，当我们测试需要在各种平台运行脚本的时候，几乎不用考虑跨平台。





那如果我们学会了Python，哪些方面可以应用Python来辅助我们的测试呢？显而易见并应用最广泛的当属自动化测试了。




Python + Selenium实现web端的UI自动化：





Selenium是一个用于Web应用程序测试的工具。Selenium测试直接运行在浏览器中，就像真正的用户在操作一样。支持的浏览器包括IE（7, 8, 9, 10, 11），Mozilla Firefox，Safari，Google Chrome，Opera等。这个工具的主要功能包括：测试与浏览器的兼容性——测试你的应用程序看是否能够很好得工作在不同浏览器和操作系统之上。测试系统功能——创建回归测试检验软件功能和用户需求。
![](https://img-blog.csdn.net/20171129211601406?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


Python与Selenium结合能够很好的实现web端的UI级别的自动化。Selenium几乎能解决目前遇到的web页面中所有的定位难题，当然有些特殊的可以借助JavaScripts也可以实现。




Python + Requests实现接口测试：




进入python requests官网看到的的第一句话是：Requests 唯一的一个非转基因的Python HTTP 库，人类可以安全享用。




通过requests库提供的各种API可以非常方便的模拟HTTP请求，实现接口自动化测试。




Python + Appium实现app自动化测试：





Appium是一个自动化测试开源工具，支持 iOS 平台和Android 平台上的原生应用，web 应用和混合应用。




所谓的 移动原生应用 是指那些用iOS 或者 Android SDK 写的应用。所谓的 移动 web 应用 是指使用移动浏览器访问的应用（Appium 支持 iOS 上的Safari 和 Android 上的 Chrome）。所谓的“混合应用”是指原生代码封装网页视图——原生代码和web 内容交互。比如，像 Phonegap，可以帮助开发者使用网页技术开发应用，然后用原生代码封装，这些就是混合应用。




重要的是，Appium 是一个跨平台的工具：它允许测试人员在不同的平台（iOS，Android）使用同一套API来写自动化测试脚本，这样大大增加了iOS 和 Android 测试套件间代码的复用性。





Appium也是多语言支持，当然也是支持python的，运用python + Appium可以实现跨平台的app UI自动化测试。




Python编写工具辅助测试：


Python 提供了多个图形开发界面的库，几个常用 Python GUI 库如下：



Tkinter：


Tkinter 模块(Tk 接口)是Python 的标准 Tk GUI 工具包的接口。Tk 和 Tkinter 可以在大多数的Unix 平台下使用，同样可以应用在Windows 和 Macintosh 系统里。Tk8.0 的后续版本可以实现本地窗口风格,并良好地运行在绝大多数平台中。



wxPython：


wxPython 是一款开源软件，是 Python 语言的一套优秀的GUI 图形库，允许 Python 程序员很方便的创建完整的、功能键全的GUI 用户界面。




Python还有很多快速开发的web框架，比如Django、falsk、webpy等，通过这些框架也是能很容易快速的开发出web应用。




通过这些GUI库、web框架，可以写出很多小工具辅助测试。



结语：


一旦你学会一门语言，对于日常工作中，都可以自己写点工具来解放那些繁琐重复的工作，让你有更多的时间放在更有意义的测试上。同时你也学到了很多的技术，也能让你更好的完成测试工作。






