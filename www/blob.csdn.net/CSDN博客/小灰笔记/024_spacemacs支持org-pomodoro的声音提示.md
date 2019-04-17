# 024_spacemacs支持org-pomodoro的声音提示 - 小灰笔记 - CSDN博客





2018年12月03日 21:38:48[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：38
个人分类：[Emacs](https://blog.csdn.net/grey_csdn/article/category/7131378)









         这个简单的小功能配置起初有点让我觉得奇怪，我自己的配置可以通过声音提示，反倒是spacemacs的配置不支持。

         我个人的emacs配置中，这个功能很久之前就实现了，当时印象中只记得配置了一个powershell，不过查看了一下我的spacemacs这个功能也是安装了的。看上去，要么是两个org管理插件不同，要么是我还少了一个插件支持。挨个查看了我自己的配置，还真有一个看着跟声音相关的：sound-wav。

         安装之后，spacemacs测试也支持声音提示了。看起来，这个提示音应该是播放wav格式的声音文件了。

         具体的修改方式：

![](https://img-blog.csdnimg.cn/20181203213825124.png)

         上面着色的地方是配置额外安装包的地方，增加后重启emacs即可。



