# ubuntu的root默认密码问题 - 三少GG - CSDN博客
2010年04月27日 18:11:00[三少GG](https://me.csdn.net/scut1135)阅读数：1442标签：[ubuntu																[debian																[login																[unix																[system																[security](https://so.csdn.net/so/search/s.do?q=security&t=blog)](https://so.csdn.net/so/search/s.do?q=system&t=blog)](https://so.csdn.net/so/search/s.do?q=unix&t=blog)](https://so.csdn.net/so/search/s.do?q=login&t=blog)](https://so.csdn.net/so/search/s.do?q=debian&t=blog)](https://so.csdn.net/so/search/s.do?q=ubuntu&t=blog)
个人分类：[Ubuntu/OS X系统](https://blog.csdn.net/scut1135/article/category/680808)
### 1.[ubuntu默认root密码问题](blog/634944)
**文章分类:[操作系统](http://www.javaeye.com/blogs/category/os)**
ubuntu默认root密码 为空 
用sudo - i 进入# 提示符，然后输入 passwd  root  输入新的密码就OK!
[http://fengyaodong.javaeye.com/blog/634944](http://fengyaodong.javaeye.com/blog/634944)
**2.Ubuntu下如何修改root默认密码**
Ubuntu安装后root是没有默认的密码的，因为你还没给root设置密码，你第一个 user 是在 admin 组，他可以给 root 设置密码，所以大家不要费脑筋想当初自己设的密码是多少了，因为根本就不存在。
端口输入如下命令：
sudo passwd root
[sudo] password for you ：---> 输入你的密码，不回显
Enter new UNIX password: --- > 设置root 密码
Retype new UNIX password: --> 重复
就OK了。
[http://www.linuxdiyf.com/viewarticle.php?id=111102](http://www.linuxdiyf.com/viewarticle.php?id=111102)
3.
让大家知道如何简易的激活root帐号，不理解的话，我可以在这里帮助大家理解下Ubuntu root的问题，然后全面介绍Ubuntu root，希望对大家有用。Ubuntu 包含了非常好的翻译和容易使用的架构，这些由自由软件社团将不断的提供, 将尽可能的使大多数人方便使用 Ubuntu 。
**激活Ubuntu的root帐号**
Ubuntu root 帐号默认是被禁用了的。在这种情况之下，如果你想要使用 Ubuntu root 的权限来干些事情，就需要 sudo 指令。对某些朋友来说，他们可能需要激活 Ubuntu 中的 root 帐号。
在 Ubuntu 中激活 root 帐号的过程如下：
1. 在终端执行 sudo passwd root 指令后，系统将会提示你设置一个新的 Ubuntu root 帐号密码。在你没经过当前用户密码验证的时候，还要求你先输入当前用户密码，然后才能设置新的Ubuntu root密码。
2. 点击 System -> Preferences -> Login Window 菜单，并切换到 Security 选项页，然后选中其下的“Allow local system administrator login”选项。
执行上述两步后，你便可以使用 root 帐号登录 Ubuntu 系统了。如果要再次禁用 Ubuntu root 帐号，那么可以执行 sudo passwd -l root。新接触ubuntu的人（base on debian），大多会因为安装中没有提示root密码而不太清楚为什么会这样。根据ubuntu的中文文档，总结了本文。
关键词：如何设定/改变/启用 Ubuntu root 使用者的密码?
sudo passwd root 
为了启用 Ubuntu root 帐号 (也就是 设置一个口令) 使用:
sudo passwd root
当你使用完毕后屏蔽 Ubuntu root 帐号 使用:
sudo passwd -l root
这个将锁住 root 帐号.
[http://os.51cto.com/art/201001/176089.htm](http://os.51cto.com/art/201001/176089.htm)
4.在Ubuntu下如何切换到超级用户 (superuser) 
由于 Ubuntu 是基于 Debian 的 linux 操作系统，在默认的情况下，是没有超级用户(superuser, root)的，但有些系统操作必须有超级用户的权限才能进行，如
手动释放内存
等。
在其他 linux 操作系统 (如 fedora) 下，可以使用 
su
切换到超级用户。
当输入 su 命令后， 系统会要求输入 root 的密码。
可是，在 Ubuntu 下我们根本不知道 root 的密码是什么。
这样，在 Ubuntu 下切换到超级用户需要使用其他方法，主要有两种：
1) sudo -i 
sudo
是 su 的加强版，意思是 do something as the supervisor。
   不需要密码就可以得到 root 的权限。
   但是它也有很多限制，比如，在默认的情况下，只能在 5 分钟之内使用 root 权限。
**2) 如果想一直使用 root 权限，还是要使用 su， 还是要得到 root 密码的。   用 sudo passwd root可以设置 root 的密码。    之后就可以自由使用 su 命令啦。**
[http://linux.chinaunix.net/techdoc/system/2009/12/01/1146941.shtml](http://linux.chinaunix.net/techdoc/system/2009/12/01/1146941.shtml)
