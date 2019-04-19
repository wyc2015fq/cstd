# WinCE MUI的实现----本人亲自实践 - xqhrs232的专栏 - CSDN博客
2013年08月24日 23:16:45[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：879
原文地址::[http://blog.csdn.net/nanjianhui/article/details/4394397](http://blog.csdn.net/nanjianhui/article/details/4394397)
作者：ARM-WinCE
MUI是指多语言的支持，可以在WinCE系统中支持多种语言，并根据需要进行切换。下面介绍一下具体的实现：
**1. 添加多种语言：**
创建一个WinCE工程，然后在”Solution Explorer”中右击工程，选择”Properties”，会弹出一个对话框。在左侧的列表中选择”Local”，在右侧选择所要支持的语言，这里我选择了French，Japanese，Chinese(PRC)和English四种语言。在”Default Locale:”中选择系统的默认语言，这里选择的是英语。如图：
![](https://p-blog.csdn.net/images/p_blog_csdn_net/nanjianhui/EntryImages/20090730/mui_1.JPG)
**2. 添加MUI组件**
在”Catalog Item View”中选择支持MUI的组件，选择”Core OS”->”CEBASE”->”International”->”Multilingual User Interface(MUI)”，如图：
**![](https://p-blog.csdn.net/images/p_blog_csdn_net/nanjianhui/EntryImages/20090730/mui_2.JPG)**
在上面选择了French，Japanese，English和Chinese四种语言，如果要想WinCE能够正确显示，还要选择相应的字体。在这里只需为Chinese和Japanese添加字体。选择”Core OS”->”CEBASE”->”International”->”Locale Specify Support”->”Chinese(Simplified)”->”Fonts”->”SimSun & NSimSun”->”SimSun & NSimSun”来添加中文字体，同样的方法在”Locale
 Specify Support”下找到”Japanese”然后添加日语字体。
**3. 编译MUI工程**
重新编译工程。编译成功后，打开工程的release目录，调用createmui命令来创建多种语言的资源文件，每种语言对应一个LCID，可以在WinCE的帮助文档中查到，如下：
**English：0409**
**Chinese(PRC)：0804**
**French：040C**
**Japanese：0411**
命令如下：
**createmui 0409,0804,040C,0411**
运行成功后会生成一个MultiUI.bib文件，打开该文件，拷贝所有内容到common.bib文件的末尾。MutiUI.bib文件中可能包含k.coredll.dll.xxx，在common.bib文件中删除所有的k.coredll.dll.xxx项。
最后重新Makeimg就可以了。
**4. 运行测试**
下载并运行WinCE，可以看到默认的语言是英文的。选择”start”->”setting”->”Control Panel”弹出控制面板，然后选择”Regional Settings”，再选择”User Interface Language”，选择要切换的语言，比如Chinese(PRC)，如图：
![](https://p-blog.csdn.net/images/p_blog_csdn_net/nanjianhui/EntryImages/20090730/mui_3.JPG)
重新启动目标板，如果你用WinCE模拟器的话，在菜单中选择File，然后选择Reset，选择Soft来重新启动。重起以后，界面就会变成中文的WinCE系统了。
//=================================================================================================================
备注::
1>老早就知道怎么去弄但一直没弄，今天弄了一下，不成功啊！看来做事情必须事必恭亲才能有自己的真实的收获！！！----总算可以了，原来选择了区域去了，没改到语言的那个地方----User  Interface Language.再一次论证了技术在于细节的道理！！！
2>下面是实现这个功能的几个要点
  A、运行成功后会生成一个MultiUI.bib文件，打开该文件，拷贝所有内容到common.bib文件的末尾----不这样弄NK里面就没语言资源文件----MUI文件
 B、MutiUI.bib文件中可能包含k.coredll.dll.xxx，在common.bib文件中删除所有的k.coredll.dll.xxx项----不去掉编译会报错，RELEASE文件夹下确实没有
k.coredll.dll.DLL.mui这一项，不过有k.coredll.DLL.0409.mui++++k.coredll.DLL.0804.mui，修改为这两个应该更好！！！----不同的DLL对应不同的语言！！！这样修改也可以编译成功，并且运行情况也是正确正常的！
3>createmui 0409,0804后会产生一个multiui.bib文件于Release目录下，然后在platform.bib文件里面包含这个文件，再makeimg就可以了
  #include "$(_TARGETPLATROOT)\FILES\multiui.bib"---------注意文件COPY放置的路径
