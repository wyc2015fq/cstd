# SVN版本库无损迁移与自动备份（二） - z69183787的专栏 - CSDN博客
2015年01月16日 13:13:30[OkidoGreen](https://me.csdn.net/z69183787)阅读数：778

**三、定时自动备份版本库解决方案**
**1、业务目标**
①版本库的远程自动备份，将版本库备份到另一台机器上。
假设我们要同步的源版本库为 http://192.168.1.210/svn/svnprojec位于机器A，具体路径我们不必理会，因为我们使用http协议
目标库在机器B, file:///F:/Repositories/svnproject，这个为了简单和安全，我们使用file://协议
②实现版本库的本地备份，只需要将上述目标库的位置，改成本地位置即可。
**2、相关指令**
达到备份版本库的目的要用到两个命令：
①svnsync init
初始化，建立目标库和源库之间的同步关系
命令格式: svnsync init 目标库URL 源库URL
（两个URL之间有空格）
② svnsync sync
真正的同步
命令格式： svnsync sync 目标库URL
**3、过程示例**
**（1）备份**
①在要备份的机器上建立版本库（如果是本地备份，则在本地建立版本库）：svnadmin create test1BackUp
②进入源版本库的hooks目录，例如cdD:\Repositories\TestRepostitory\hooks
③创建pre-revprop-change.bat文件:复制pre-revprop-change.tmpl，将扩展名改为pre-revprop-change.bat,并且清空原有的所有内容，保存。
④修改文件：修改pre-revprop-change.tmpl文件，用记事本打开该文件，把文件最后的exit 1改为exit 0
（原脚本的意思是如果修改的是svn:log属性，将允许修改，返回0；否则，不允许，返回1，我们要将它改为允许修改所有的属性，在脚本中直接返回0）
⑤同步初步：          
在目标机器上，打开命令提示符窗口，打开SVN服务器Bin目录，运行
svnsync init file:///D:/Repositories/test1BackUp  https://192.168.0.110/svn/Test1
（会提示输入用户名和密码，这里提供的用户名和密码是可以完全读取于https://192.168.0.110/svn/Test1的计算机密码，用户名和密码）
⑥实现同步：
在目标机器上，打开命令提示符窗口，打开SVN服务器Bin目录，运行
svnsyncsync file:///D:/Repositories/test1BackUp
（如果提示输入用户名和密码，你可以在这个命令之后加上 username 、password参数
即svnsync sync file:///D:/Repositories/test1BackUp --username username --password password）
**注：**
第⑤⑥两步可以直接放在一起，写入到一个批处理文件(新建记事本，将⑤⑥中的两句话放入，改记事本后缀为.bat)，将该批处理文件放入SVN服务器Bin目录，双击运行即可。**一会我们设定执行备份会用到这个批处理文件。**
如果是本地备份，则只需将目标URL改为本地库位置即可。（针对VisualSVN，因为一台机器上只能有一个VisualSVN服务器，所以所有版本库只能在一个目录下，才能被服务器识别，这样，备份只能备在相同目录，似乎意义不大。）
如果版本库较大时，备份的时间会有点慢，花费几个小时或者一天也是有可能的，
备份完毕，你可以打开目标库看看，和源库是一样的。
**（2）定时执行备份。**
这里我们用到了windows自带的任务计划程序
①在控制面板\所有控制面板项\管理工具下，打开任务计划程序
 主界面：点击右侧操作的创建任务
![](http://hi.csdn.net/attachment/201111/30/0_1322638310bXky.gif)
②开始创建任务，常规选项卡下，主要设置任务的基本信息，这里我们一般给任务起一个名字就可以了，例如SVN同步
![](http://hi.csdn.net/attachment/201111/30/0_13226383184Zai.gif)
③操作选项卡下，点击新建，这里可以设定我们要执行的操作。我们备份SVN版本库，需要执行，刚才设定好的备份批处理文件。这个文件，我们实现应该放在SVN服务器安装目录的bin目录下（不要放错地方哦）。我们点击浏览，找到这个文件。
![](http://hi.csdn.net/attachment/201111/30/0_13226383271lj5.gif)
④在触发器选项卡下，我们可以新建触发器，这里我们可以设定执行刚才设定的操作的条件。让其自动执行。
![](http://hi.csdn.net/attachment/201111/30/0_1322638332lSTh.gif)
⑤条件和设置选项卡下，可以设定执行该任务的其他条件，根据我们自己的情况选择即可。
![](http://hi.csdn.net/attachment/201111/30/0_1322638338n1Qp.gif)
![](http://hi.csdn.net/attachment/201111/30/0_1322638343h55e.gif)
最后确定，大功告成，我们的定时自动备份SVN版本库的任务就创建成功了。
**提醒：如果版本库不是很大，小于15G，完全可以把它建在金山快盘目录下，使其自动备份到云端**
**注：我使用的win7旗舰版，在winXP环境下创建任务，大同小异，相信大家都能轻松搞定。**
**三、定时自动备份版本库解决方案**
**1、业务目标**
①版本库的远程自动备份，将版本库备份到另一台机器上。
假设我们要同步的源版本库为 http://192.168.1.210/svn/svnprojec位于机器A，具体路径我们不必理会，因为我们使用http协议
目标库在机器B, file:///F:/Repositories/svnproject，这个为了简单和安全，我们使用file://协议
②实现版本库的本地备份，只需要将上述目标库的位置，改成本地位置即可。
**2、相关指令**
达到备份版本库的目的要用到两个命令：
①svnsync init
初始化，建立目标库和源库之间的同步关系
命令格式: svnsync init 目标库URL 源库URL
（两个URL之间有空格）
② svnsync sync
真正的同步
命令格式： svnsync sync 目标库URL
**3、过程示例**
**（1）备份**
①在要备份的机器上建立版本库（如果是本地备份，则在本地建立版本库）：svnadmin create test1BackUp
②进入源版本库的hooks目录，例如cdD:\Repositories\TestRepostitory\hooks
③创建pre-revprop-change.bat文件:复制pre-revprop-change.tmpl，将扩展名改为pre-revprop-change.bat,并且清空原有的所有内容，保存。
④修改文件：修改pre-revprop-change.tmpl文件，用记事本打开该文件，把文件最后的exit 1改为exit 0
（原脚本的意思是如果修改的是svn:log属性，将允许修改，返回0；否则，不允许，返回1，我们要将它改为允许修改所有的属性，在脚本中直接返回0）
⑤同步初步：          
在目标机器上，打开命令提示符窗口，打开SVN服务器Bin目录，运行
svnsync init file:///D:/Repositories/test1BackUp  https://192.168.0.110/svn/Test1
（会提示输入用户名和密码，这里提供的用户名和密码是可以完全读取于https://192.168.0.110/svn/Test1的计算机密码，用户名和密码）
⑥实现同步：
在目标机器上，打开命令提示符窗口，打开SVN服务器Bin目录，运行
svnsyncsync file:///D:/Repositories/test1BackUp
（如果提示输入用户名和密码，你可以在这个命令之后加上 username 、password参数
即svnsync sync file:///D:/Repositories/test1BackUp --username username --password password）
**注：**
第⑤⑥两步可以直接放在一起，写入到一个批处理文件(新建记事本，将⑤⑥中的两句话放入，改记事本后缀为.bat)，将该批处理文件放入SVN服务器Bin目录，双击运行即可。**一会我们设定执行备份会用到这个批处理文件。**
如果是本地备份，则只需将目标URL改为本地库位置即可。（针对VisualSVN，因为一台机器上只能有一个VisualSVN服务器，所以所有版本库只能在一个目录下，才能被服务器识别，这样，备份只能备在相同目录，似乎意义不大。）
如果版本库较大时，备份的时间会有点慢，花费几个小时或者一天也是有可能的，
备份完毕，你可以打开目标库看看，和源库是一样的。
**（2）定时执行备份。**
这里我们用到了windows自带的任务计划程序
①在控制面板\所有控制面板项\管理工具下，打开任务计划程序
 主界面：点击右侧操作的创建任务
![](http://hi.csdn.net/attachment/201111/30/0_1322638310bXky.gif)
②开始创建任务，常规选项卡下，主要设置任务的基本信息，这里我们一般给任务起一个名字就可以了，例如SVN同步
![](http://hi.csdn.net/attachment/201111/30/0_13226383184Zai.gif)
③操作选项卡下，点击新建，这里可以设定我们要执行的操作。我们备份SVN版本库，需要执行，刚才设定好的备份批处理文件。这个文件，我们实现应该放在SVN服务器安装目录的bin目录下（不要放错地方哦）。我们点击浏览，找到这个文件。
![](http://hi.csdn.net/attachment/201111/30/0_13226383271lj5.gif)
④在触发器选项卡下，我们可以新建触发器，这里我们可以设定执行刚才设定的操作的条件。让其自动执行。
![](http://hi.csdn.net/attachment/201111/30/0_1322638332lSTh.gif)
⑤条件和设置选项卡下，可以设定执行该任务的其他条件，根据我们自己的情况选择即可。
![](http://hi.csdn.net/attachment/201111/30/0_1322638338n1Qp.gif)
![](http://hi.csdn.net/attachment/201111/30/0_1322638343h55e.gif)
最后确定，大功告成，我们的定时自动备份SVN版本库的任务就创建成功了。
**提醒：如果版本库不是很大，小于15G，完全可以把它建在金山快盘目录下，使其自动备份到云端**
**注：我使用的win7旗舰版，在winXP环境下创建任务，大同小异，相信大家都能轻松搞定。**
