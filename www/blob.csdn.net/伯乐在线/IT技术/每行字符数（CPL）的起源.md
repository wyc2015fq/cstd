# 每行字符数（CPL）的起源 - 文章 - 伯乐在线
原文出处： [阮一峰](http://www.ruanyifeng.com/blog/2011/10/characters_per_line.html)
前几天，我收到网友小龙的Email。
他想与我讨论一个问题：
> 
**“各种计算机语言的编码风格，有的建议源码每行的字符数（characters per line）不超过72个，还有的建议不超过80个，这是为什么？区别在哪里？怎么来的？”**
我一下子就被问住了。
![bg2011102301](http://jbcdn2.b0.upaiyun.com/2016/11/af0ec234c6eda07d4f1f471bd0bcb1a2.png)
命令行状态下，终端窗口的显示宽度，默认是80个字符，这个我早就知道，但是并不清楚原因；至于72个字符，更是从未注意过。
幸好，世界上还有Wikipedia，我在里面找到了[答案](http://en.wikipedia.org/wiki/Characters_per_line)。
![bg2011102302](http://jbcdn2.b0.upaiyun.com/2016/11/033e222fbf5d8a6a22733c00f7e338c3.jpg)
每行72个字符的限制，来源于打字机。上图是20世纪60年代初，非常流行的IBM公司生产的[Selectric](http://en.wikipedia.org/wiki/IBM_Selectric)电动打字机。
![bg2011102303](http://jbcdn2.b0.upaiyun.com/2016/11/583d9a38764881847a9a6ba719be2652.png)
当时，美国最通用的信笺大小是8.5英寸x11英寸（215.9 mm × 279.4 mm），叫做[US Letter](http://en.wikipedia.org/wiki/Letter_(paper_size))。打字的时候，左右两边至少要留出1英寸的页边距，因此每行的长度实际为6英寸。打字机使用等宽字体（monospaced）的情况下，每英寸可以打12个字符，就相当于一行72个字符。
![bg2011102304](http://jbcdn2.b0.upaiyun.com/2016/11/c3cbe83a345f45e56a0a23d666a15d3c.png)
早期，源码必须用打字机打出来阅读，所以有些语言就规定，每行不得超过72个字符。直到今天，[RFC](http://www.ietf.org/download/rfc-index.txt)文档依然采用这个规定，因为它从诞生起就采用打字稿的形式。
![bg2011102308](http://jbcdn2.b0.upaiyun.com/2016/11/8c7cba356520f679557c4589c6afec03.jpg)
20世纪70年代，显示器出现了。它的主要用途之一，是将[打孔卡](http://en.wikipedia.org/wiki/Punched_card)（punched card）的输入显示出来。当时，最流行的打孔卡是IBM公司生产的80栏打孔卡，每栏为一个字符，80栏就是80个字符。
![bg2011102305](http://jbcdn2.b0.upaiyun.com/2016/11/1813ce7f63fcea818441a671ca20ad74.png)
上图是一张Fortran语言的源码填写单，一共有80栏，程序员在每一栏选择想要输入的字符，最多为80个字符。
![bg2011102307](http://jbcdn2.b0.upaiyun.com/2016/11/8988bec2fa962105ea725f5c9d4a886e.jpg)
然后，用机器自动生成打孔卡，在每栏选定的位置打一个孔。
计算机读取打孔卡以后，把每个孔转换为相应的字符。如果显示器每行显示80个字符，就正好与打孔卡一一对应，终端窗口的每行字符数（CPL）就这样确定下来了。
