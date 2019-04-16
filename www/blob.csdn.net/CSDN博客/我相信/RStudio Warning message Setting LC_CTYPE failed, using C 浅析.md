# RStudio: Warning message: Setting LC_CTYPE failed, using "C" 浅析 - 我相信...... - CSDN博客





2016年04月10日 21:39:56[半吊子全栈工匠](https://me.csdn.net/wireless_com)阅读数：4615








周末，一曾经的小伙伴问及数据分析的几个问题，遗憾的是，他没有基于python，而是选择了R。 他告诉我，R在他的mac 上无法显示和输入中文，不明所以。




问题出现了，首先要定位问题所在，还原现场。微信截图，才发现 Rstudio 在启动的时候报如下信息：



During startup - Warning messages:
1: Setting LC_CTYPE failed, using "C"
2: Setting LC_COLLATE failed, using "C"
3: Setting LC_TIME failed, using "C"
4: Setting LC_MESSAGES failed, using "C"
5: Setting LC_PAPER failed, using "C"
[R.app GUI 1.50 (6126) x86_64-apple-darwin9.8.0]

WARNING: You're using a non-UTF8 locale, therefore only ASCII characters will work. 


从告警信息中可以很清晰的知道问题所在，Locale 不是UTF8，所以中文不正常了。




明确了问题，如何解决呢？这里涉及一个概念 Locale，locale 是什么呢？



Mac OS 是一个类Linux系统，在Linux中，locale来设置程序运行的不同语言环境，locale由 ANSI C提供支持。locale的命名规则为_.，如zh_CN.GBK，zh代表中文， CN代表大陆地区，GBK表示字符集。在locale环境中，用一组变量代表国际化环境中的不同设置：

LC_COLLATE，定义该环境的排序和比较规则

LC_CTYPE，用于字符分类和字符串处理，控制所有字符的处理方式，包括字符编码，字符是单字节还是多字节，如何打印等。是最重要的一个环境变量。 

LC_MONETARY，货币格式

LC_NUMERIC，非货币的数字显示格式

LC_TIME，时间和日期格式

LC_MESSAGES，提示信息的语言。

另外还有一个LANGUAGE参数，它与LC_MESSAGES相似，但如果该参数一旦设置，则LC_MESSAGES参数就会失效。 LANGUAGE参数可同时设置多种语言信息，如LANGUAGE="zh_CN.GB18030:zh_CN.GB2312:zh_CN"。

LANG，LC_*的默认值，是最低级别的设置，如果LC_*没有设置，则使用该值。类似于 LC_ALL

LC_ALL，它是一个宏，如果该值设置了，则该值会覆盖所有LC_*的设置值，但LANG的值不受该宏影响



了解了根源，就有解决 的方法了：
1） 修改系统的locale

2）修改R 执行环境的语言设置




对于方法1， export 和source 的命令就和在Linux 的环境设置一样了。

对于方法2， 就Mac OS 而言，涉及到 defaults  的使用。dafaults 是什么？ 详情可以查看apple的相关文档。 简单的说，相当于windows 的注册表。

一条指令解决问题： 

defaults write org.R-project.R force.LANG en_US.UTF-8




这是改系统变量，那么在R 内部是否可以做相关的操作呢？ 当然可以：

`R> Sys.setenv(LANG="en_US.UTF-8")`





出现问题是好事，从问题中挖掘到根本性的知识更重要，最重要的是掌握解决问题的思路和方法。






