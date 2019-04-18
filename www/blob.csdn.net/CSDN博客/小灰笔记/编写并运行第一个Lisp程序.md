# 编写并运行第一个Lisp程序 - 小灰笔记 - CSDN博客





2017年11月26日 10:09:05[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：4950








       我觉得接触一门新的编程语言的时候第一个程序的编写至关重要，这能够让我快速了解到简单的语法以及运行方法。而运行方法基本上是我更为关注的，因为这将是后期学习过程中不断打交道的东西。

       为了能够弥补工作中那种浪费时间的懊悔感，我决定学习一个新的编程语言，初步选定了lisp。倒是没有特别的因由，只是随便想到了就选择了。这个纯粹是一种赎罪式消磨时间的学习，倒也不期望自己能够学到什么成果。倒是学习过程中可以顺便练习使用一下Emacs，这个是一个很有趣的事情。

       接下来写自己的第一个程序，我个人感觉比较有代表性的一段代码：

(defun WriteHelloWorld(fileName)

 (with-open-file (out fileName

                     :direction :output

                     :if-exists :supersede)

(with-standard-io-syntax (print "HelloWorld" out))))

       打开lispbox加载并执行如下：

![](https://img-blog.csdn.net/20171126100839394?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

       执行的过程中屏幕显示出“Hello World”，这个其实是有点在我意料之外。我最初对代码的理解是这个函数完成一个写文件的功能。接下来看看写文件的结果吧！查看LispBox启动位置，发现多了一个demo.txt的文件，打开后文件的内容如下：

![](https://img-blog.csdn.net/20171126100849033?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

       有一个空行的输出，还是有些让我意外。不过，目前我比较关注的如何运行的问题暂且是解决了。应该还会有更加简单的运行方法，后面继续了解学习吧！



