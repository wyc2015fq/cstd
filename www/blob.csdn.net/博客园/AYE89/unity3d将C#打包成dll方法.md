
# unity3d将C\#打包成dll方法 - AYE89 - 博客园|[AYE89](https://www.cnblogs.com/eniac1946/)
|coding & learning|
|posts - 153, comments - 3, trackbacks - 0, articles - 2

|
|导航
|[博客园](https://www.cnblogs.com/)|[首页](https://www.cnblogs.com/eniac1946/)|[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)|[联系](https://msg.cnblogs.com/send/AYE89)![订阅](//www.cnblogs.com/images/xml.gif)|[订阅](https://www.cnblogs.com/eniac1946/rss)|[管理](https://i.cnblogs.com/)
|公告


|[unity3d将C\#打包成dll方法](https://www.cnblogs.com/eniac1946/p/7243173.html)
|Posted on|2017-07-27 09:43|[AYE89](https://www.cnblogs.com/eniac1946/)|阅读(|...|) 评论(|...|)|[编辑](https://i.cnblogs.com/EditPosts.aspx?postid=7243173)|[收藏](#)
|方法一|：用vs新建工程-C\#库，添加UnityEngine.dll引用，注意.netframwork选3.5，编译C\#脚本得到dll；
|方法二|：使用mono的mcs，具体如下
|c\#提供了dll打包，但是unity中由于涉及到unity的自身dll，如UnityEngine.dll,我们一般写的很多代码都会涉及到对这个dll的引用，在unity中建代码文件都会默认添加对此dll命名空间的使用，using UnityEngine;
|因此在涉及到使用此dll在打包dll的时候就要有点特殊处理。
|项目中常见的是对Debug的二次封装，由于调试的需求，在项目开发周期debug是个很重要且常用的类，但是debug的效率实在是不忍直视，所以在打包apk时出于效率考虑就要屏蔽掉debug的调试，这时要做的可能是删除掉所有对应的代码，这基本上是不可能的，不说这些调试代码可能以后会用到，每次打包都删一次也不实际。还有一种是设定bool值，每次调用Debug类的时候，判定此bool值，每次都要判定，这本书就代码冗余，设计的不合理。这里大家想到的可能都是二次封装了。在封装一个类，调用Debug的时候调用封装的类，但是这又设计到一个问题，Debug调试的一个重要功能就是显示在控制台的信息双击可以跳到指定代码，如果这样二次封装的话就每次跳到封装的代码里，而不是我们想要的地方。有人说可以看堆栈调用信息，可是每次都看，很累的。对于这个问题，便是将二次封装类进行dll打包。
|啰嗦了一大堆，只是说明为什么要对Debug进行二次封装和为什么要用dll的原因，具体方法如下，unity官方提供了相应的教程。
|[Unity官方文档教程链接地址](http://docs.unity3d.com/Manual/UsingDLL.html)
|文档是英文的，但是不难，分别讲解了在Windows条件下和mac条件下的操作方法。
|这里以Windows为例，大概讲解一下。
|流程：
|1.|先进入unity的安装目录，找到你引用的dll，地址类似于C:\Program Files\Unity\Editor\Data\Managed
|如你引用了UnityEngine.dll，那么久复制对应的文件。
|2.|将复制的文件拷贝到硬盘根目录，根目录的缘故是为了接下来方便操作，也可以在根目录下创建一个文件夹，将文件放入这个文件夹。这里示例为F盘根目录 。|注意：不建议放在桌面
|3.|将你要打包dll的文件复制到同个地方（示例为F盘根目录）
|4.|开cmd界面，进入执行文件的目录，路径大概如下
|C:\Program Files (x86)\Unity4.6\Editor\Data\MonoBleedingEdge\lib\mono\2.0
|根据自己的安装目录进行参考
|5.|执行命令，命令大概如下
|mcs -r:F:\UnityEngine.dll -target:library F:\DebugCustom.cs
|其中mcs是命令是要执行的命令，F:\UnityEngine.dll是引用的dll，如果你引用了多个dll，可以加个空格后继续添加，-target:library是指将文件打包成dll，F:\DebugCustom.cs是你要打包成dll的文件，如果文件有多个，同样加空格然后在后面添加
|上面就是流程了，看完了后不知道有没有看出问什么会有上面那个注意了，第五条中空格很关键，而我们桌面的路径有的会有空格，win7的没有，好像是Xp有，所以只是不建议，而不是不能。大家根据自己的情况调整就行。
|引申：混淆器可以用来安全保护编译后的dll，具体原理是去除代码中所有的symbol信息。







|[刷新评论](javascript:void(0);)|[刷新页面](#)|[返回顶部](#top)






|
|Powered by:
|博客园
|Copyright © AYE89
|
