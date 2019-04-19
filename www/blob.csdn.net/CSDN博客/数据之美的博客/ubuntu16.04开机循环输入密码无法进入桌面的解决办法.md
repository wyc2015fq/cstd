# ubuntu16.04开机循环输入密码无法进入桌面的解决办法 - 数据之美的博客 - CSDN博客
2017年03月04日 17:01:57[看穿数据之美](https://me.csdn.net/oppo62258801)阅读数：1122
前些天碰到一个头疼的问题，启动我的ubuntu之后，输入密码闪屏一下，又需要输入密码！！！于是再输还要再输！！！！！
经过百度一翻后终于找到原因和解决办法。
原来是我之前在profile文件里配置了一次[Java](http://lib.csdn.net/base/javase)环境变量造成的
这是我之前的配置：
**[plain]**[view
 plain](http://blog.csdn.net/lj779323436/article/details/52649068#)[copy](http://blog.csdn.net/lj779323436/article/details/52649068#)
- export JAVA_HOME=/usr/jdk1.8.0_101  
- export PATH=$JAVA_HOME/bin:$JAVA_HOME/jre/bin  
- export CLASSPATH=.:$JAVA_HOME/lib:$JAVA_HOME/jre/lib  
至于原因，大家参考这里[http://m.blog.csdn.net/article/details?id=6378212](http://m.blog.csdn.net/article/details?id=6378212)
而解决办法就是重新配置环境变量，但是系统都进不去，很懵逼！然而我们是可以在非图形界面下进系统的：
1.进入非图形化界面：在登录界面同时按下ctrl+alt+f1(有的需要同时按下ctrl+alt+f1+fn)
2：输入你的账户名回车     *注意;这里是帐户名，而不是密码
![](https://img-blog.csdn.net/20160926164915645?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
3：输入你的密码回车ok,此时就已经进入系统了。
![](https://img-blog.csdn.net/20160926164943146?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
4：进入系统后需要修改/etc/profile文件 命令：sudo vi /ect/profile注意：vi和/etc之间有一个空格
如果你的回车后没有让你输入密码，而是说sudo这个命令找不到，这个时候你可以有2种方法
1、/usr/bin/sudo vi /etc/profile 
2、比较笨，就是先切换到/usr/bin目录下，键入cd  /回车再使用sudo vi /ect/profile
   接下来就是输入你的密码，然后就可以修改profile文件了。
5：修改profile文件，将自己添加的都给删了![](https://img-blog.csdn.net/20160926172122219?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
vi删除操作代码：　
                        x 一个字符　　#x 删除几个字符，#表示数字，比如3x
                 　　dw 删除一个单词　　#dw 删除几个单词，#用数字表示，比如3dw表示删除三个单词　
                     　dd 删除一行；　　#dd 删除多个行，#代表数字，比如3dd 表示删除光标行及光标的下两行　
                     　d$ 删除光标到行尾的内容
                        J 清除光标所处的行与上一行之间的空格，把光标行和上一行接在一起 
vi退出保存代码：
shift + :进入尾行命令模式
:x 应该是保存并退出  　　
vi撤销操作代码：　
u命令取消最近一次的操作，可以使用多次来恢复原有的操作　
U取消所有操作
   具体参照[http://yunhuazhiyue.blog.163.com/blog/static/217621154201341043126366/](http://yunhuazhiyue.blog.163.com/blog/static/217621154201341043126366/)
   而我直接用的del键删除光标前面的字符这样一个个删的（一开始不会呀，而且backspace键根本没用）。
6：修改完成后保存退出，键入:wq回车然后就可以看到回到命令行状态了。
7：重启，键入sudo reboot回车
OK大功告成，问题已完美解决。
