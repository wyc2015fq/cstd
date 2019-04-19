# linux开机启动服务和chkconfig使用方法 - 零度的博客专栏 - CSDN博客
2016年05月12日 14:07:38[零度anngle](https://me.csdn.net/zmx729618)阅读数：303
个人分类：[Linux](https://blog.csdn.net/zmx729618/article/category/6225640)
- 服务概述
在linux操作系统下，经常需要创建一些服务，这些服务被做成shell脚本，这些服务需要在系统启动的时候自动启动，关闭的时候自动关闭。
将需要自动启动的脚本/etc/rc.d/init.d目录下，然后用命令chkconfig --add filename将自动注册开机启动和关机关闭。实质就是在rc0.d-rc6.d目录下生成一些文件连接，这些连接连接到/etc/rc.d/init.d目录下指定文件的shell脚本。
在文章[linux下Oracle自动启动与停止（一）](http://blog.csdn.net/chenxiaohua/archive/2008/02/26/2121878.aspx)，[linux下Oracle自动启动与停止（二）](http://blog.csdn.net/chenxiaohua/archive/2009/04/07/4054684.aspx)
 对创建服务有一些描述。
- 手工创建服务
在/etc/rc.d/init.d目录下创建shell脚本，文件名auto_run。
设置脚本的运行权限chmod +x auto_run。
然后在rc0.d-rc6.d目录下分别创建文件连接。
ln -s /etc/rc.d/init.d/auto_run /etc/rc.d/rc2.d/S99auto_run
ln -s /etc/rc.d/init.d/auto_run /etc/rc.d/rc3.d/S99auto_run
ln -s /etc/rc.d/init.d/auto_run /etc/rc.d/rc5.d/S99auto_run
ln -s /etc/rc.d/init.d/auto_run /etc/rc.d/rc0.d/K01auto_run
ln -s /etc/rc.d/init.d/auto_run /etc/rc.d/rc6.d/K01auto_run 
这样系统在启动的时候，就会运行auto_run 并加上start参数，等同于执行命令auto_run start。
在系统关闭的时候，就会运行auto_run，并加上stop参数，等同于运行命令auto_run stop。
创建连接的6条命令可以用命令chkconfig --add auto_run来完成，这样就简单多了，还不容易出错。
下面就介绍一下chkconfig命令。
- chkconfig命令用法
语法：
chkconfig --list [name]
chkconfig --add name
chkconfig --del name
chkconfig [--level levels] name <on|off|reset>
chkconfig [--level levels] name
说明:
chkconfig提供一种简单的命令行工具来帮助管理员对/etc/rc[0-6].d目录层次下的众多的符号链接进行直接操作。
此命令使用是由chkconfig命令在IRIX操作系统提供授权。不用在/etc/rc[0-6].d目录下直接维护配置信息，而是直接在/etc/rc[0-6]下管理链接文件。在运行级别的目录下的配置信息通知在将会初始启动哪些服务。
Chkconfig有五个很明确的功能：为管理增加一个新的功能、删除一个功能、列出当前服务的启动信息、改变一个服务的启动信息和检测特殊服务的启动状态。
当chkconfig没有参数运行时，它将显示其使用方法。如果只给出了一个服务名，它将检测这个服务名是否已经被配置到了当前运行级别中。如果已经配置，返回真，否则返回假。--level选项可以被用做查询多个运行级别而不仅仅是一个。
       如果在服务名之后指定了on,、off或reset，chkconfig将改变指定服务的启动信息。On或off标记服务被打开或停止，尤其是在运行级别被改变时。Reset标记重置服务的启动信息。
       默认情况下，on或off选项仅对2、3、4、5有影响，而 reset影响所有的运行级。--level选项可以被用于指定哪个运行级别接收影响。
注意：对于每个服务，每一个运行级都有一个开始角本和一个结束角本。当开或关一个运行级时，init不会重新开始一个已经运行的服务，也不会重新停止一个未运行的服务。
选项：
       --level levels
              指定一个运行级别适合的操作。范围为0-7。
       --add name
              增加一个新的服务。
       --del name
              删除一个服务
       --list name
              显示服务的情况
RUNLEVEL FILES
每个通过chkconfig管理的服务在其init.d目录下的角本中都需要两行或多行的注释。第一行告诉chkconfig在默认情况下什么运行级别的服务可以开始，也就是所说的开始或结束的优先级别。如果服务没有默认的级别，建造将在所有运行级别中启动。a – 将用于代替运行级列表。第二个注释行包括对此服务的描述，可以通过反斜线符号扩展为多行。
示例，auto_run的前三行如下：
#!/bin/sh
#chkconfig: 2345 80 90
#description:auto_run
第一行，告诉系统使用的shell,所以的shell脚本都是这样。
第二行，chkconfig后面有三个参数2345,80和90告诉chkconfig程序，需要在rc2.d~rc5.d目录下，创建名字为S80auto_run的文件连接，连接到/etc/rc.d/init.d目录下的的auto_run脚本。第一个字符是S，系统在启动的时候，运行脚本auto_run，就会添加一个start参数，告诉脚本，现在是启动模式。同时在rc0.d和rc6.d目录下，创建名字为K90auto_run的文件连接，第一个字符为K，个系统在关闭系统的时候，会运行auto_run，添加一个stop，告诉脚本，现在是关闭模式。
注意上面的三行是中，地二，第三行是必须的，否则在运行chkconfig --add auto_run时，会报错。
- 常见的错误
“**服务不支持 chkconfig**”：
请注意检查脚本的前面，是否有完整的两行：
#chkconfig: 2345 80 90
#description:auto_run
在脚本前面这两行是不能少的，否则不能chkconfig命令会报错误。
如果运行chkconfig老是报错，如果脚本没有问题，我建议，直接在rc0.d~rc6.d下面创建到脚本的文件连接来解决，原理都是一样的。
