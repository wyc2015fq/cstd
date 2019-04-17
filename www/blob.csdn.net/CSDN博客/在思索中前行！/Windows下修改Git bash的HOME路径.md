# Windows下修改Git bash的HOME路径 - 在思索中前行！ - CSDN博客





2014年10月19日 21:02:09[_Tham](https://me.csdn.net/txl16211)阅读数：1716









Windows中使用[http://git-scm.com/](http://git-scm.com/)安装Git bash工具，默认的`HOME`和`~`路径一般都是`C:\Users\用户名`，每次得用命令切换到常用的Repository下，此操作重复而没有意义。更重要的是，若使用`SSH`连接GitHub，其配置文件比如rsa也保存在C盘中，不利于备份。因此，**本文的目的是修改Git bash默认的Home路径。**


打开`Git安装位置\etc\profile`文件，找到


```
# normalize HOME to unix path
HOME="$(cd "$HOME" ; pwd)"
export PATH="$HOME/bin:$PATH"
```



增加两行，修改后结果如下：


```
# normalize HOME to unix path
HOME="你想要修改的HOME路径"
HOME="$(cd "$HOME" ; pwd)"
cd
export PATH="$HOME/bin:$PATH"
```



再次启动Git bash，就会自动进入新修改后的`HOME`路径了，之后新配置的`.SSH`文件夹也为在新路径中。

*Updated on 2013/11/13*


若是Windows系统，为了方便起见可以顺手加上一个变量名用于自定义项目目录，此做法不但便于路径跳转，也可以在启动git时自动进入项目目录。方法极为简单，只需在`profile`文件中加入:


```bash
# set Project Path
proj="你的项目文件目录"
cd $proj
```



如代码所「言」，可使用`cd $proj`命令即可跳转到项目目录，也可以按需定义其他变量。


如果您对bush非常了解，应该能猜到linux的bash的配置方案理应也在这行得通。而事实也的确如此，有profile就有bashrc。


在`HOME`目录下新建文件`.bashrc`写入你最惯用的配置，比如alias、bind、export
 等，执行`source ~/.bashrc`即刻生效。



