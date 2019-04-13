
# 哈工大开源项目LTP系统的首次运行 - forever1dreamsxx--NLP - CSDN博客


2012年07月26日 08:35:12[forever1dreamsxx](https://me.csdn.net/forever1dreamsxx)阅读数：1980


原文地址：[http://www.mrluoyi.com/blog/2011/07/tutorial-of-ltp/](http://www.mrluoyi.com/blog/2011/07/tutorial-of-ltp/)
（环境：Windows, VS2008)
chubby_roro@qq.com
0.    说明
辛勤原创，转载请注明出处。新手之作，请各位朋友斧正。为了方便，[
](http://www.mrluoyi.com/material/%E5%93%88%E5%B7%A5%E5%A4%A7%E5%BC%80%E6%BA%90%E9%A1%B9%E7%9B%AELTP%E7%B3%BB%E7%BB%9F%E7%9A%84%E9%A6%96%E6%AC%A1%E9%85%8D%E7%BD%AE.pdf)[这里]提供了PDF版本的，欢迎下载：）
1.       必须的材料：
1）  VS2008的安装略去。（由于该项目的开发比较早，所以官方建议2008而不是2010）
2）  LTP系统包：HIT-SCIR-ltp-83ddff2，记得跟哈工大申请，下载地址：http://ir.hit.edu.cn/ltp/
3）  Boost库（LTP用到了boost.regex库），此案例使用boost版本1.37.0，下载地址：http://www.boost.org/users/history/version_1_37_0.html
2.       Boost库中regex库的编译
Regex库是博大精深的boost库的一个组成部分，它必须要专门地进行一次编译操作才能正确被创建。然后，系统包中有三个模块（_svmtagger, _srl, _ner）用到了boost库中的regex库，所以我们必须先编译它。**[1]**
1）  把下载好的boost压缩包解压到某个盘，我是解压到了d盘。
![](http://www.mrluoyi.com/blog/wp-content/uploads/2011/11/LTP_1.jpg)
2）  然后运行CMD，进入Visual Studio 2009\VC路径下，运行vcvarsall.bat，得到以下提示。
![](http://www.mrluoyi.com/blog/wp-content/uploads/2011/11/LTP_2.jpg)
3）  接着直接把CMD转入路径D:\boost_1_37_0\libs\regex\build，在这个路径下有针对VC各个不同版本的Makefile文件，Visual Studio 2008对应的VC版本是VC9。
4）  依次执行以下命令：
执行命令nmake -f vc9.mak
![](http://www.mrluoyi.com/blog/wp-content/uploads/2011/11/LTP_3.bmp)
执行命令nmake -f vc9.mak install
执行命令nmake -f vc9.mak clean
5）  这时，在C:\Program Files\Microsoft Visual Studio 9.0\VC\lib下生成了7个Boost.Regex库文件，Regex库的编译告一段落。
![](http://www.mrluoyi.com/blog/wp-content/uploads/2011/11/LTP_4.jpg)
6）  Boost库还有很多内容，完全的编译需要很长时间，由于我们只用到了regex，就不再赘述了，大家有兴趣查阅**[1]**或者boost的官方网站（http://www.boost.org/）。
3.       把LTP代码跑起来
1)       先把HIT-SCIR-ltp-83ddff2包解压到本地某个位置，我放在了E:\LTP，并把其中的ltp_data压缩包在同一层解压。（ltp_data里装的是各个模块的模型文件，以待加载）
![](http://www.mrluoyi.com/blog/wp-content/uploads/2011/11/LTP_5.jpg)
2)       在VS2008中打开windows\win_project\vs2008\__ltp.sln工程，可以看到一共有8个项目文件。
3)       由于_ner，_srl与_svmtagger用到了boost库，我们需要把他们编译时的引用指向第三方库，具体操作我们以_ ner为例：
a.       右键点击_ner，选择属性，左边的配置属性->C/C++，在“附加包含目录”中，把boost库的路径添加进去，即D:\boost_1_37_0。
![](http://www.mrluoyi.com/blog/wp-content/uploads/2011/11/LTP_6.jpg)
b.       同理把_srl与_svmtagger项目也配置好。
4)       由于模块之间存在依赖关系，我们需要分清楚顺序。实际的操作中，我们发现_svmtagger模块儿需要最先编译，所以请首先编译该项目，右键->生成。
5)       接下来，把官方提供的_test_suit项目改为“启动项目”，右键->设为启动项目。（这一步不改的话，会出现让你选择执行文件的对话框，但是我因为这个迷茫了好久……）你注意看_test_suit的main函数可以发现，哈工大已经为我们提供好了各个测试例子，我们只需要顺藤摸瓜就可以自己完善了：）
6)       按道理讲，现在可以对整个工程直接编译了！然后出现以下对话框：
![](http://www.mrluoyi.com/blog/wp-content/uploads/2011/11/LTP_7.jpg)
开始各种读取和加载模型。
4.       输入和输出在哪里
直接编译的话，输入是E:\LTP\windows\win_project\vs2008里面的text.txt。你可以自己放些文本测试测试。
输出就是该路径下的xml文件。
5.       引用文献
[1]正则化库Boost.Regex的编译与使用（Visual Studio 2008）http://hi.baidu.com/%D3%F3%C4%E0%C4%EA%B8%E2/blog/item/c4058512fa451147f819b88c.html
6.       特别感谢
之所以写这篇文档，是为了帮助那些和我一样在入门道路上苦无援助的同学们。都是我自己的实践过程，由于是初学者，难免很多步骤显得冗长和幼稚，请大家包涵指正，希望我们共同进步！
特别感谢LTP官方微群http://q.weibo.com/849045中帮助我的学长们，感谢郑学长。感谢吴老师提供实验室，感谢淼的键盘，感谢Quan的指导和Kong的帮助……
最重要的是感谢哈工大无私地开源！LTP是个强大的东西。


