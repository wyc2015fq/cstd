# Dev-C++,Time to say goodbye ?（更新2012-02-04） - tkokof1的专栏 - CSDN博客

2011年10月04日 16:21:54[tkokof1](https://me.csdn.net/tkokof1)阅读数：1932标签：[工具																[下载工具																[delphi																[编译器																[ide																[windows](https://so.csdn.net/so/search/s.do?q=windows&t=blog)](https://so.csdn.net/so/search/s.do?q=ide&t=blog)](https://so.csdn.net/so/search/s.do?q=编译器&t=blog)](https://so.csdn.net/so/search/s.do?q=delphi&t=blog)](https://so.csdn.net/so/search/s.do?q=下载工具&t=blog)](https://so.csdn.net/so/search/s.do?q=工具&t=blog)
个人分类：[随性](https://blog.csdn.net/tkokof1/article/category/642708)


一. 那段使用Dev-C++的日子

  记得在那段很久以前的学生时代，自己曾经很反感Visual Studio的过度臃肿，再加上当时学校的实验PC陈腐老旧：经常假死的Windows 2000，外加日渐古稀的Visual-C++ 6.0，以及一些不知何处而来的过时病毒，这般情景一度令当时我痛苦难耐，心想：写一个

  std::cout << "Hello,world\n";

真有这么费时费力吗？

  直到那一天，我与Dev-C++偶遇的那一天……

  我是如何得知Dev-C++的，我已记不太清，只记得当时非常惊讶于他的小巧：整个安装包竟然总计只有10M字节，而与之对应的VC++6.0却有近1G的身材，光这点便着实令当时的我费解不已，心里不住的琢磨：这玩意到底行是不行……？

  一开始自己尝试使用时确实不是非常舒服，譬如关于main函数的写法Dev-C++（准确的说应该是gcc）会强制你写成:

  int main( void ) 

  {

      ......

      return 0;

  }

  或者

   int main( int argc, char *argv[] ) 

   {

       ......

       return 0;

   }

  而VC++6.0则相对宽松，main函数有无返回值都没有什么警告错误，当然类似的差异还有很多，不一而足，所以Dev-C++曾令当时散漫的我颇不适应，不过还好，随着时间的推移自己慢慢适应了规范的写法，也渐渐发现其实Dev-C++才是一款正确的学习规范C/C++语言的工具（VC++6.0对C/C++标准的支持实在不是很好……），以致到后来我基本上忘却了VC的存在，一心一意用起了Dev-C++……

  记得当时我的所有练手程序（大部分基于Console）都是使用Dev-C++，偶尔的一些图形界面程序也是基本使用Win32 API来完成（当时还不甚了解Qt之类的界面库），虽然效率效果都比较差劲，但是却也乐于自己得到的那份自由舒畅~~~~~~

  可惜精致小巧终归会带来一些弊端，一方面由于后期Dev-C++已基本不再更新，导致很多遗留的BUG都不能及时修复，二来由于Dev-C++开发的年代较早，一些目前已经算作普通功能的特性（譬如语法自动补全），Dev-C++对他们的支持也是非常有限，三者也因为Dev-C++的小巧，其基本只提供了一些普通的代码编辑功能（有些还不是特别有用），有时确实比较影响开发效率……

  因为种种的原因，自己自工作以后，便也渐渐的与Dev-C++疏远，Win32平台上用的最多的、也渐渐觉得最方便的还是VS（继VC++6.0之后，VisualStudio的确在C/C++标准支持上面做了不少改进），如果非要使用其他的一些较小型的IDE，我也基本不会选Dev-C++了，最重要的一条理由便是Dev-C++现存的很多调试功能Bug，就这点确实令人避其不及……

  近来听说Dev-C++终于又放出了更新的版本（目前好像已经到5.0.0.4），虽然不是原作者发布的，但是也着实令人兴奋了，本想马上下一份安装试上一试，可惜登陆官网直接被墙，简单尝试了一下翻墙，未果，比较残念（有途径的朋友可以下一份来共享[orwellengine.blogspot.com/2011/09/dev-c-5004-released.html](orwellengine.blogspot.com/2011/09/dev-c-5004-released.html) ，万分感谢）

  想来Dev-C++的上次更新似乎已是6年前了，期间时过进迁，很多优秀的IDE愈来愈好，自然亦有另一些IDE慢慢的退出舞台，虽然Dev-C++又迎来了自己的一次更新之春，但是可能除了一些死忠以外，大部分人还是不会轻易放掉目前自己手上惯用的IDE了（譬如我），所以至少对于我来说，时不时已经到时间向Dev-C++ Say Goodbye了呢？

  无论如何，还是让我再来简单回顾一下Dev-C++，这位曾经跟我亲密无间的战友吧 ：）

（Dev-C++的更多的一些评价[信息](http://blog.ez2learn.com/2011/07/08/please-do-not-use-dev-c/)。）

  二. Dev-C++纵览

  Dev-C++是一套用于C/C++开发的集成开发环境，他是自由的，并且遵守GPL协议。其内建的编译环境基于MinGW GCC，调试环境则基于GDB，而其本身则使用Delphi作为开发语言。

  Dev-C++目前只能运行于Windows平台之上，其最初的开发者为Colin Laplace，至今你仍然可以在SourceForge上找到该[项目](http://sourceforge.net/projects/dev-cpp/)，可惜自2005年2月以来，该项目已经没有更新（目前更新的Dev-C++属于非官方版本），据Colin Laplace在2006年的一则说法，不再更新的原因是由于其一直忙于自己的生活而没有时间继续Dev-C++的开发。

![](http://hi.csdn.net/attachment/201110/4/0_1317715040fJUF.gif)

  A下载与安装

  目前网络上可以正常下载到的Dev-C++版本便是6年前的4.9.9.2(或者有些网站也称其为5.0 beta)，你可以在很多地方得到他的安装包拷贝（譬如[这里](http://www.skycn.com/soft/4639.html)），其中汉化版居多，安装过程也一贯的傻瓜，不过由于各类安装包并不都很纯粹，有时在自定义安装界面时会自动选择一些第三方的插件程序（譬如百度工具条，唉……），在意的朋友可以注意一下：

![](http://hi.csdn.net/attachment/201110/4/0_1317715949ZQ6j.gif)

 一切OK之后，便可以运行Dev-C++了：
![](http://hi.csdn.net/attachment/201110/4/0_1317715957OK0p.gif)

B. Dev-C++的使用

  关于Dev-C++的使用，网上已经有很多不错的教程，在此我便不再赘述，有兴趣的朋友可以看看：

  1. [建立新项目、编译、执](http://www.cs.pu.edu.tw/~tsay/course/objprog/slides/newproj.html)行

  2. [开启旧项目](http://www.cs.pu.edu.tw/~tsay/course/objprog/slides/oldproj.html)

  3. [向项目中添加文件](http://www.cs.pu.edu.tw/~tsay/course/objprog/slides/addproj.html)

  4. [调试](http://www.cs.pu.edu.tw/~tsay/course/objprog/slides/debugger.html)

  当然，以上只是Dev-C++的基本使用，另外的一些功能（譬如自定义功能）我也觉得值得一说：

  a. 编译设置

![](http://hi.csdn.net/attachment/201110/4/0_13177150458Mzm.gif)
![](http://hi.csdn.net/attachment/201110/4/0_13177150512UqR.gif)

  在这里，你可以设置全局的编译器命令以及链接器命令，还有很多预设的编译链接选项（譬如编译优化），以及一些默认的文件包含目录，还有一些预设的程序（一般会在配置不同的编译器时会用到）。

  b. 环境设置

![](http://hi.csdn.net/attachment/201110/4/0_13177159341lb4.gif)
![](http://hi.csdn.net/attachment/201110/4/0_1317715942Nzx4.gif)

  在此你可以设置很多环境相关的参数，一般而言使用默认值即可，我一般可能会修改的便是部分，在此我可以设置语言、主题以及一些个性化的参数，在此建议勾选“No splash screen on startup”，以消除Dev-C++的启动界面。

  c. 编辑器设置

![](http://hi.csdn.net/attachment/201110/4/0_1317715890uLIw.gif)
![](http://hi.csdn.net/attachment/201110/4/0_1317715900uH2l.gif)

  在此可以设置很多编辑器相关的参数，如各类语法高亮，字体大小等等，有兴趣的朋友可以仔细对照修改，与我而言，默认设置基本符合要求。（[这里](http://www.slyar.com/blog/devcpp.html)有一些更细节的内容）

  d 另外的一些配置

![](http://hi.csdn.net/attachment/201110/4/0_1317715056nES2.gif)

  这个用来设置自定义快捷键。

![](http://hi.csdn.net/attachment/201110/4/0_13177158755EH2.gif)

  这个似乎是用来设置第三方工具，但是本人并没有实际使用过……

  Dev-C++内建了一个开发包的管理工具，其基本由两部分组成：

  一个是更新下载工具：

![](http://hi.csdn.net/attachment/201110/4/0_13177160638w8a.gif)
![](http://hi.csdn.net/attachment/201110/4/0_1317716070Z5t4.gif)

  注意，上面的需要选择第二项：（第一项貌似早已不可访问），链接并更新状态之后，便会在下方显示可以下载的更新包，你可以勾选并点击下载。（注意：开发包并不都可以顺利下载，请做好心理准备：））

  第二个开发包工具便是Package Manager：

![](http://hi.csdn.net/attachment/201110/4/0_1317716028Hi92.gif)
![](http://hi.csdn.net/attachment/201110/4/0_13177160415vlT.gif)

  当你下载使用更新工具下载了对应的开发包，你便可以使用Package Manager的Install Package功能来安装开发包：

![](http://hi.csdn.net/attachment/201110/4/0_13177160470f54.gif)

  安装成功之后，对应的开发包会在右侧显示，点选之后你可以在左侧查看开发包的基本信息以及所涉及的文件：
![](http://hi.csdn.net/attachment/201110/4/0_1317716053264Z.gif)

e. CVS

Dev-C++ 内部集成了对于[CVS](http://www.nongnu.org/cvs/)（[wiki](http://en.wikipedia.org/wiki/Concurrent_Versions_System)）的支持：
![](http://hi.csdn.net/attachment/201110/4/0_1317715885W59h.gif)

可惜CVS的版本控制功能有着一些天生的缺陷，目前普遍的观点都认为可以使用[SVN](http://subversion.apache.org/)（[wiki](http://en.wikipedia.org/wiki/Apache_Subversion)）来进行代替，基于此，在此也就省略介绍Dev-C++的这项功能，当然有兴趣的朋友可以仔细的评鉴一下 ：）

C. 一些有用的快捷键

  在此罗列一些Dev-C++中自认为很有用的快捷键，有兴趣的朋友可以看一看：

1. Ctrl+N ： 新建一个空白文件

2. Ctrl+O ： 打开工程（文件）

3. Ctrl+S ： 保存当前文件

4. Ctrl+F12 ：另存文件

5. Ctrl+F4 ： 关闭当前文件

6. Ctrl+Z ： 撤销

7. Shift+Ctrl+Z ： 重做（Redo）

8. Ctrl+X ：剪切

9. Ctrl+C ：复制

10. Ctrl+V ：黏贴

11. Ctrl+1~9 ：设置书签（1~9）

12. Alt+1~9：跳至书签（1~9）

13. Ctrl+A ：全选

14. Ctrl+. ：注释当前选择代码

15. Ctrl+M：取消注释

16. Shift+Ctrl+I：缩进当前选择代码

17. Shift+Ctrl+U：取消缩进

18. Ctrl+F ：查询

19. Ctrl+Alt+F：在文件中查找

20. F3：继续查找

21. Ctrl+R：替换

22. Shift+Ctrl+G：跳至函数

23. Ctrl+G：跳至某行

24. Alt+P：项目属性

25. F9：编译并运行

26. Ctrl+F9：编译

27. Ctrl+F10：运行

28. Ctrl+F11：重新编译

29. F8：开始调试

30. Ctrl+Alt+F2：停止调试

31. Ctrl+F5：设置/取消断点

32. F7：下一步

33. Shift+F7：进入函数

34. Ctrl+F7：继续运行

35. Shift+F4：运行到光标所在处

36. F4：添加查看

37. Ctrl+W：查看变量

38. F12：开启/取消全屏模式

39. F6：下一个窗口

40. F5：上一个窗口

三. 最后的一些话

  时间真的过的很快，曾经那么熟悉的Dev-C++，今天看来竟然已经有些陌生了，诚然，Dev-C++有着这样那样的不足，但是他曾经也成为了那么多人的朋友，也许今后我很少再会拿起他来做一些正儿八经的东西，但是我仍然真心希望随着新版本的发布，他能够再次开花结果，并且愈加沉甸，到时，说不定我仍然会跨起这位老战友的手臂再次启程 ：）

  现在，也不知是永远还是暂时，就让Dev-C++成为我心中的一份默默的留念，并让我平静的对他说一声：Goodbye ~~~ 

  再见了，我的Dev-C++ ~~~ 

**（ Dev-C++的最新版本目前已经上传，有兴趣的朋友可以[看看](http://download.csdn.net/detail/tkokof1/4044294)：） ）**

  部分参考：

  1. [http://en.wikipedia.org/wiki/Dev-C%2B%2B](http://en.wikipedia.org/wiki/Dev-C%2B%2B)


