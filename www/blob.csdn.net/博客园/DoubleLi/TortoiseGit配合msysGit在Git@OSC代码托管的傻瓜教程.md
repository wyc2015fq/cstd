# TortoiseGit配合msysGit在Git@OSC代码托管的傻瓜教程 - DoubleLi - 博客园






命令行太麻烦，肿么破？便便利用睡觉的时间解决了一点效率问题，tortoiseGit处理GitHub，一样可以处理 [Git](mailto:Git@osc)[@osc](http://my.oschina.net/u/1164642) ，虽然说可以用gitk来调出图形界面，but，我就是不想看见黑黑的命令提示符的框框，于是乎，近乎龟毛到变态的便便又开始了新的折腾。。。。 
OK，windows系统，linux和mac勿喷。。。。。

下载msysgit 
[http://msysgit.github.io/](http://msysgit.github.io/)
下载TortoiseGit 
[http://code.google.com/p/tortoisegit/](http://code.google.com/p/tortoisegit/)
先安装msysgit，做如下步骤配置 
点开图标 
![](http://static.oschina.net/uploads/space/2013/0630/024116_sWOW_112406.jpg)
粗线讨厌的黑黑的小框框，不过这个linux字体看上去挺爽的。。。 
![](http://static.oschina.net/uploads/img/201412/07224551_N6TX.jpg)
然后进入到你本地的git工作文件夹下，便便放在G:/git 
![](http://static.oschina.net/uploads/img/201412/07224551_swF3.jpg)
然后初始化配置git init 
![](http://static.oschina.net/uploads/img/201412/07224551_ji4t.jpg)
文件夹下面就会出现这么一个隐藏文件.git 
![](http://static.oschina.net/uploads/img/201412/07224552_Ibw1.jpg)

然后配置用户名 
git config -- global user . name "XXXX" 

git config -- global user . email [XXXX](mailto:XXXX@XXXXX)[@XXXXX](http://my.oschina.net/u/134700)

上面的XXXX是你的名字的代替，表犯二哟 ![](http://static.oschina.net/uploads/img/201412/07224552_GnOX.gif)
然后捏你可以从git上弄份托管代码下来测试一下 



git clone [http://git.oschina.net/oschina/git-osc.git](http://git.oschina.net/oschina/git-osc.git)
介个代码捏就在下面这张图的位置 
![](http://static.oschina.net/uploads/img/201412/07224552_YG1K.jpg)

有兄弟要抓狂了，上面全是命令行啊！！！便便，图形界面捏，好了，接下来便便就要讲解TortoiseGit来处理上面的步骤。 
然后呢就是安装海龟 tortoiseGit 安装应该都看的懂吧，英文不是问题 

如果上面的.git文件夹没有建立，先到G:/git文件夹，右键文件夹，然后Git Create.... 
![](http://static.oschina.net/uploads/img/201412/07224552_f0ux.jpg)
在弹出的对话框勾选make it bare  

![](http://static.oschina.net/uploads/img/201412/07224552_Oiiy.jpg)

进入后会发现有.git文件夹了。 
然后设置，右键空白处 TortoiseGit>Setting 设置用户名和邮箱等信息 

![](http://static.oschina.net/uploads/img/201412/07224552_HqhC.jpg)
在用户工作目录下点击右键,选择git clone,弹出对话框 

![](http://static.oschina.net/uploads/img/201412/07224552_UfIz.jpg)
点击ok后,发现此文件夹下多了一个隐藏的文件夹.git,则clone成功 
如果你新建了一个项目，就建在刚刚的又.git的文件夹下面，便便将要托管的项目mypubuliu文件夹放到G:/git文件夹下。 
然后右键文件夹Tortoisegit-->add, 
然后右键Git Commit -> "master"... 在弹出的对话框,输入message信息后点ok,此时index.jsp变成对勾,提交到了本地服务器 
然后右键Tortoisegit-->push则提交到了远程服务器 
中间会要求输入 [Git](mailto:Git@OSC)[@OSC](http://my.oschina.net/u/1164642) 的用户名和密码，这个应该不会错吧？ 









