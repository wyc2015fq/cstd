# Ubuntu 使用 su 切换用户时提示 Authentication failure 解决方法 - shine302的博客 - CSDN博客
2018年04月23日 17:46:42[SmartMap_Joule](https://me.csdn.net/shine302)阅读数：29
### 问题产生的原因
Ubuntu 系统默认是没有激活 root 用户，需要用户手动开启。
### 解决方法
- 按快捷键 Ctrl + Alt + T，打开命令行界面
- 在当前用户下输入 sudo passwd，出现如下命令，输入当前用户的密码 
![Ubuntu 使用 su 切换用户时提示 Authentication failure 解决方法](https://img-blog.csdn.net/20161214095227395?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMjgxNDc3Nzc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
- 如果输入的密码正确，这时候就会出现下面的命令，意思是叫我们设置新的 UNIX 密码。 
![Ubuntu 使用 su 切换用户时提示 Authentication failure 解决方法](https://img-blog.csdn.net/20161214095342928?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMjgxNDc3Nzc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
- 输入想要设置的密码之后，重复确认密码，它就会提示你，密码设置成功。 
![Ubuntu 使用 su 切换用户时提示 Authentication failure 解决方法](https://img-blog.csdn.net/20161214095701745?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMjgxNDc3Nzc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
- 
