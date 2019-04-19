# Eclipse IDE中Android项目打红叉的解决方法 - xqhrs232的专栏 - CSDN博客
2011年11月17日 23:00:56[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1053
其实只是下面帖子的总结帖，没什么新意！
关于android项目总是出现红叉，但是一点错误也没有！----[http://topic.csdn.net/u/20100831/17/71678b5c-1de2-4ff2-b587-6e5a937f79b4.html](http://topic.csdn.net/u/20100831/17/71678b5c-1de2-4ff2-b587-6e5a937f79b4.html)
先可以去看一下几个窗口的输出内容，不同的错误日志要采用不同的方法，要灵活使用各种方法！
1>菜单路径----Window/Show View/Console
2>菜单路径----Window/Show View/Error Log
3>菜单路径----Window/Show View/Problems
排除打红叉的常用方法，只是一个总结性的东西，大家不要太较真!
1>工程在项目列表中删除(不从磁盘删除)并重新导入一次
2>工程先Clean/Refresh一下，然后再重新编译
3>删除工程下的gen目录再重新编译一次，或者把R.java删除掉再重新编译一次
4>看res目录下的资源文件有没错误，图片格式是否支持，main.xml+strings.xml有没语法错误或者书写错误
5>看配置文件AndroidManifest.xml+default.properties+proguard.cfg文件中有没语法错误或者书写错误
6>选中工程/属性使用Android Tools工具菜单中的Fix Project Properties工具修订一下
7>选中工程/属性看看你的工程的配置是否存在问题，比如没有选择SDK包，或者库路径有错误，或者库文件不存在，或者环境变量没配置对等等
8>保存好工程的*.java文件，在这基础之上重新建立一个新的工程
9>Error generating final archive: Debug certificate expired on Date的解决办法
进入：C:\Documents and Settings\Administrator\.android 删除：debug.keystore及 ddms.cfg再重新打开Eclipse就可以了，牵涉一个什么私钥的时间期限问题
10>Eclipse IDE打不开，报.metadata文件夹有看要求去看LOG文件，直接把.metadata文件夹删除再打开就OK了
