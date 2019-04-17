# 解决浏览器抛出乱码，（HTML、PHP等的乱码问题） - 在思索中前行！ - CSDN博客





2014年08月27日 18:45:19[_Tham](https://me.csdn.net/txl16211)阅读数：3589










在Windows上编写html或php代码的时候，本地编辑器设置的文件编码格式是utf-8保存，但是浏览器打开页面的时候经常出现乱码，而且浏览器自动检测到的页面编码为GBK格式，这时候我就开始纳闷了？

       为什么会这样了，我用记事本打开编写过的文件，然后另存为，在编码方式中选择utf-8，然后保存，再到浏览器中打开，发现正常显示了。这时候我就开始思考，为什么记事本设置编码格式utf-8就能生效而编辑器不行了。经过分析发现，在这些编辑器中设置utf-8无BOM，浏览器都会出现乱码而且检测页面编码为GBK，设置为UTF-8含BOM的时候，页面就能正常显示。

UTF-8 不需要 BOM，尽管 Unicode 标准允许在 UTF-8 中使用 BOM。所以不含
 BOM 的 UTF-8 才是标准形式，在 UTF-8 文件中放置 BOM 主要是微软的习惯（顺便提一下：把带有 BOM 的小端序 UTF-16 称作「Unicode」而又不详细说明，这也是微软的习惯）。BOM（byte order mark）是为 UTF-16
 和 UTF-32 准备的，用于标记字节序（byte order）。微软在 UTF-8 中使用 BOM 是因为这样可以把 UTF-8 和 ASCII 等编码明确区分开，但这样的文件在 Windows 之外的操作系统里会带来问题。
UTF-8」和「带 BOM 的 UTF-8」的区别就是有没有 BOM。即文件开头有没有 U+FEFF。
在其他非windows操作系统上UTF-8 的网页代码不应使用 BOM，否则常常会出错。

## 关于UTF-8 和BOM的分析

BOM是啥。这个就不解释了，Wikipedia上很详细。[http://en.wikipedia.org/wiki/Byte_order_mark](http://en.wikipedia.org/wiki/Byte_order_mark)。
        在网页上使用BOM是个错误。BOM设计出来不是用来支持HTML和XML的。要识别文本编码，HTML有charset属性，XML有encoding属性，没必要拉BOM撑场面。虽然理论上BOM可以用来识别UTF-16编码的HTML页面，但实际工程上很少有人这么干。毕竟UTF-16这种编码连ASCII都双字节，实在不适用于做网页。
       其实说BOM是个坏习惯也不尽然。BOM也是Unicode标准的一部分，有它特定的适用范围。通常BOM是用来标示Unicode纯文本字节流的，用来提供一种方便的方法让文本处理程序识别读入的.txt文件是哪个Unicode编码（UTF-8，UTF-16BE，UTF-16LE）。Windows相对对BOM处理比较好，是因为Windows把Unicode识别代码集成进了API里，主要是CreateFile()。打开文本文件时它会自动识别并剔除BOM。Windows用这个有历史原因，因为它最初脱胎于多代码页的环境。而引入Unicode时Windows的设计者又希望能在用户不注意的情况下同时兼容Unicode和非Unicode（Multiple
 byte）文本文件，就只能借助这种小trick了。相比之下，Linux这样的系统在多locale的环境中浸染的时间比较短，再加上社区本身也有足够的动力轻装前进（吐槽：微软对兼容性的要求确实是到了非常偏执的地步，任何一点破坏兼容性的做法都不允许，以至于很多时候是自己绑住自己的双手），所以干脆一步到位进入UTF-8。当然中间其实有一段过渡期，比如从最初全UTF-8的GTK+2.0发布到基本上所有GTK开发者都弃用多locale的GTK+1.2，我印象中至少经历了三到四年。
BOM不受欢迎主要是在UNIX环境下，因为很多UNIX程序不鸟BOM。主要问题出在UNIX那个所有脚本语言通行的首行#!标示，这东西依赖于shell解析，而很多shell出于兼容的考虑不检测BOM，所以加进BOM时shell会把它解释为某个普通字符输入导致破坏#!标示，这就麻烦了。其实很多现代脚本语言，比如Python，其解释器本身都是能处理BOM的，但是shell卡在这里，没办法，只能躺着也中枪。说起来这也不能怪shell，因为BOM本身违反了一个UNIX设计的常见原则，就是文档中存在的数据必须可见。BOM不能作为可见字符被文本编辑器编辑，就这一条很多UNIX开发者就不满意。
       顺便说一句，即使脚本语言能处理BOM，随处使用BOM也不是推荐的办法。各个脚本语言对Unicode的处理都有自己的一套，Python的
 # -*- coding: utf-8 -*-，Perl的use utf8，都比BOM简单而且可靠。另一个好消息是，即使是必须在Windows和UNIX之间切换的朋友也不会悲催。幸亏在UNIX环境下我们还有VIM这种神器，即使遇到BOM挡道，我们也可以通过 set nobomb; set fileencoding=utf8; w 三条命令解决问题。
最后回头想想，似乎也真就只有Windows坚持用BOM了。


字符编码相信是每个程序员的噩梦，只要是有中文的地方，总是会遇到各种编码的问题，并且这种问题还非常难缠，尤其在linux上，因为上面很多软件都是针对英语国家开发的，是不会考虑其他语种编码问题。在遇到编码的无数大坑之后，我决定仔细研究下编码问题，因为这就像一道坎一直横在你面前，每次到这里你都会跌到，每次爬起来之后，你都若无其事，这样的人被称作战士，真正的战士。可惜是个力量战士，做为新时代的智力战士，当然不能在那跌到然后又在这继续跌到。
       文件的存储方式：
文件都有自己的存储格式，比如最常见的txt，cpp，h，c，xml ，png， rmvb各种格式，还有自定义格式。这些文件不论是什么格式，都是存储在计算机硬盘里的2进制格存储，对应不同文件格式，有不同的软件解析。这篇文章不谈文件是如何存储的，只谈文件是如何解析的。
       文本文件解析：
       文本文件对应于人类可以阅读的文本，如何从2进制转换为文本文件呢？起初由于计算机在美国发明，自然大家考虑的是英语如何表示，英语字母总共26个，加上特殊字符，128个字符，7位既一个byte即可表示出来。这个就是大家所熟知的ascill编码。对应关系很简单，一个字符对应一一个byte。
       但很快发现，其他非英语国家的文字远远超过ascill码，这时候大家当然想统一一下，不同国家出了自己不同的编码方式，中国的gb2312就是自己做出来的编码方式，这样下去每个国家都有自己的编码方式，来回转换太麻烦了。这时候出现了新的编码方式，unicode编码方式，想将编码统一，所以规定了每个字符对应的unicode码。
1、很多文件都是ascii编码，如果用unicode 太浪费。
2、没有标志位说明该几个字节来解析为一个符号。
这时候拯救世界的utf出现了，utf是unicode的一种实现，只不过更聪明了。utf16是占用两字节，或者四字节，utf32是占用四字节。utf8是很聪明的一种表示方式。
1、对于单字节符号，字节第一位为0，后面7位表示字节编码。
2、对于n字节符号，第一字节的前n位都设为1，第n＋1位为0，其余位位编码位置。
对于不同的编码，在文本的最前方有不同的标志，unicode 通常有两位来表示分别是ff fe， 或者feff， fffe表示litte－endian 编码feff表示big－endian编码。utf8是efbbbf来开头的。可以看出来utf-8是自解释的，所以不用带这个标志文件，大多数程序是可以识别的。但有些程序不能识别这个标志，比如php就会直接把这个标志当文本解析，不会忽略。相信很多遇到php输出文本解析乱码或者解析错误的同学都遇到这样的问题。
最后说说如何去掉或者加上bom，如果有vim那最好不过了，去掉命令：
set encoding=utf-8
set nobomb
添加命令:
set encoding=utf-8
set bomb

Windows系统保存的都是有BOM的，所以你可以看到，用记事本保存一个UTF-8的txt，其实是有BOM的，这一点需要注意。另外不同的文本编辑器对于有无BOM的称呼也略有不同，比如EditPlus，有BOM的称为UTF-8+，无BOM的称为UTF-8，而Notepad++中，有BOM的被称为标准UTF-8，而无BOM则被称为UTF-8无BOM。


```
<!DOCTYPE html>
<html>
<head>
<meta http-equiv="Content-Type" content="text/html;charset=utf-8"><title>HTML5的标题</title>
</head>
<body><p>HTML5的内容！Hello</p>
</body>
</html>

![复制代码](http://common.cnblogs.com/images/copycode.gif)
```



用记事本写的，保存后在网页上运行竟然出现了乱码。换成GB2312能正确显示中文。



```
<!DOCTYPE html>
<html>
<head>
<meta http-equiv="Content-Type" content="text/html;charset=gb2312"><title>HTML5的标题</title>
</head>
<body>
<p>HTML5的内容！Hello</p>
</body>
</html>

![复制代码](http://common.cnblogs.com/images/copycode.gif)
```



但是毕竟标准不一样。还是要用Utf-8。最后发现代码没有一点问题，问题就出记事本身上。<meta
 charste="utf-8"> 只是告诉浏览器要用utf-8来解释，而文档的编码，是在你保存时的选择决定的。如果保存ANSI 然后用utf-8解释，肯定是乱码。记事本的话，默认保存的文件格式是ANSI。所以在保存的时候要修改为uif-8。记事本编写时一定要注意了。


用EditPlus写html页面的时候，发现设置为UTF-8的时候仍然出现了乱码，
这是一个很奇怪的问题，而且我完全考虑了浏览器的解析问题，将title放在了了meta标签之后，之后突然想起页面
保存时候的编码问题，在保存页面的时候EditPlus默认的是ANSI的编码方式，而不是UTF-8。所以在保存页面的时候注意选择编码方式为UTF-8即可。（注意浏览器编码必须设置为自动选择模式）
如果要修改EditPlus默认的编码方式，可以在tools---configure user tools，配置为Default encoding为UTF-8即可。（说明在Editplus上设置了文档保存编码UTF-8还是出现乱码，Editplus必须设置为UTF-8+BOM
 才行，但是记事本保存设置UTF-8显示没问题）
如下图配置：






![](http://s15.sinaimg.cn/middle/673bf210xa990e5b8db9e&690)

原因：
在保存文件到计算机硬盘的时候，文件保存的内容已经和本机采用的编码相关，采用了计算机上有的某种编码方式来存储。
在简体中文系统中ANSI代表的就是GB2312，繁体似乎是GBK,而在日文操作系统下，ANSI 编码代表 JIS 编码。
使用 2 个字节来代表一个字符的各种语言字体编码方式，称为 ANSI 编码
所以如果保存时候用的是ANSI, Html页面中的meta定义的是<meta http-equiv="Content-Type" content="text/html; charset=GB2312" />或者
<meta http-equiv="Content-Type" content="text/html; charset=GBK" />，由于GBK是在GB2312基础上演变而来，所以这两者在保存之后都能被浏览器解释出来，而
如果保存时候用的是ANSI而在meta中 用的是<meta http-equiv="Content-Type" content="text/html; charset=UTF-8" />则就会出现乱码，因为在用UTF-8方式去解析这样的ANSI
码的时候会出现不能正常解析也就出现了乱码

解决方式：用什么方式编码，就用什么方式解析




注释：



记事本另存为的四个编码方式。

1.ANSI(默认):中文环境下的ANSI编码方式即我们熟悉的GB2312

2.Unicode:即UTF-16

3.Unicode big endian:弄了半天还是不懂，只知道是和UTF在编码时有顺序区别。

4.UTF-8:大名鼎鼎的国际通用UTF-8，个人觉得应该是趋势。因为在web这个环境，样式可以个性，千变万化，但规则应该做到统一.



测试环境:Firefox，IE，Chrome




首先我新建了六个html页面，全部用notepad来建立。选择GB2312（即ANSI）和UTF-8两种保存方式作为测试.

1.用GB2312保存(ANSI)且声明charset=GB2312。

   结果:正常显示.

2.用GB2312保存(ANSI)且声明charset=UTF-8。

   结果:正常 ，但是浏览器编码依然为GB2312

3.用UTF-8保存且声明charset=UTF-8。

   结果:正常显示.

4.用UTF-8保存且声明charset=GB2312。

   结果:其他正常显示，但是网页编码均为utf-8

5.用GB2312保存(ANSI)，但不声明charset。

   结果:正常显示.

6.用UTF-8保存，但不声明charset。

   结果:正常显示.



总结：我认为charset属性是定义给浏览器接受的，浏览器将直接用从charset接收到的编码方式显示网页，如果不声明，浏览器会检测页面的编码方式。比较**特别的一点是，用UTF-8保存但声明charset=GB2312后，查看浏览器的编码方式均自动改为为UTF-8，且GB2312保存但是声明charset=utf-8后，浏览器的编码方式均自动改为gb2312，没有接收charset的属性值，这一点没有明白是什么回事？ 难道只需设置好文档的编码方式就好了吗？答案显然不是。**




