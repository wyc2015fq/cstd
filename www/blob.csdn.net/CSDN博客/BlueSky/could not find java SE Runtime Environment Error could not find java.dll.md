# could not find java SE Runtime Environment/Error: could not find java.dll - BlueSky - CSDN博客
2015年12月23日 10:33:26[ASIA_kobe](https://me.csdn.net/ASIA_kobe)阅读数：1741
发生这种错误的原因是什么呢，正常情况下都是你的系统中装了两个版本的JDK，比如装了1个1.6版本的JDK，然后再安装1个1.7版本的JDK，这个时候你打开eclipse的时候就会弹出一个对话框，提示：Error:could not find java SE Runtime Environment。
这个时候你也可以在命令行中输入”java -version“，可以看到相关的错误提示：
![](https://img-blog.csdn.net/20140709095622009)
解决方式有三种：
1、卸载掉其中一个版本，如果卸载掉的版本是已经配置了环境变量的，记得重新配置。
2、C:\Windows\System32这个路径下搜索java.exe，然后将java.exe名字改成其他名字。
这个时候就ok了，但是有可能这个时候你去打开Eclipse，还是会弹错误框，原因请看方式3：
![](https://img-blog.csdn.net/20140709095531796)
3、直接改注册表，因为你的Eclipse引用的还是1.6版本的，但是你安装1.7版本的时候，注册表的CurrentVersion字段被更新成1.7了，所以导致你无法打开，把框起来的两个CurrentVersion都改下，改成1.6就ok了。当然改成多少关键取决于你原来Eclipse使用的是哪个版本的JDK。
![](https://img-blog.csdn.net/20140709095737718)
注意：方法1,2解决的是JDK不能正常使用问题，如果要解决Eclipse不能打开问题，请使用方式3。有可能有时候你发现1,2可以解决Eclipse不能打开问题，这是因为你Eclipse使用的JDK刚好是注册表中注册版本的。
