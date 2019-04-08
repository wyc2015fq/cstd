# SVN版本库备份和恢复



# 1.版本库备份

## 1.1.svnadmin dump方式备份

​        此方法借助的是Subversion官网推荐的svnadmin dump备份方式，它的优点是比较灵活，既可以进行全量备份又可以进行增量备份，并提供了版本恢复机制。

缺点是如果版本过大，如版本数增加到数万、数十万条时，则dump的过程很慢，备份时耗时，恢复备份库时更耗时，不利于快速进行灾难恢复，此方法建议在版本库较小的情况下采用。

### **a)** **全量备份：**

命令格式为：svnadmin dump 版本库路径及名称 -r 导出的版本号 > 备份版本库存放路径及名称（-r是revision的缩写，也可以--revision代替）

​                 或 svnadmin dump 版本库路径及名称> 备份版本库存放路径及名称（不添加版本号时默认备份最高版本号）

例如：

\1. 对project版本库的前10个版本进行备份，将备份文件保存到H盘的svn_back/dump文件夹中，进入cmd命令行，输入以下命令：

svnadmin dump E:\Repository\project -r 10 > H:\svn_backup\dump\project.dump

点击回车后，如图所示，已经将project版本库中的前10个版本进行备份。

![img](https://images2017.cnblogs.com/blog/1205757/201708/1205757-20170802162505583-1597612706.png)

\2. 对project版本库的所有版本进行备份，将备份文件保存到H盘的svn_backup文件夹中，

进入cmd命令行，输入以下命令：

svnadmin dump F:\Repository\project>H:\svn_backup\project.dump

点击回车后，如下图所示，已经将project版本库中的12个版本全部备份。

![img](https://images2017.cnblogs.com/blog/1205757/201708/1205757-20170802162612568-134831695.png)

### **b)****增量备份：**

增量备份主要使用svnadmin dump的 --incremental选项来实现，在对版本进行管理时，采用增量备份的方法，只需要对版本库从上次备份后的版本号开始进行备份即可，节省了对版本库备份时的时间及存放版本的硬盘资源。

**命令格式：**

**svnadmin dump 版本库路径及名称 -r 上次备份的版本号:到本次要备份到的版本号 --incremental > 导出的版本库存放的路径及命名**

例如：project版本库此时的最高版本号为16，分三次进行增量备份，每次备份后的文件名称分别为project1.dump、project2.dump、project3.dump，将备份文件都保存到H盘的svn_backup/dump文件夹下。

首先对project版本库的版本号为0至10的版本进行备份，备份文件名称为project1.dump

进入cmd命令行，输入如下命令：

svnadmin dump E:\Repository\project -r 0:10 > H:\svn_backup\dump\project1.dump

![img](https://images2017.cnblogs.com/blog/1205757/201708/1205757-20170802162744240-187572428.png)

其次对版本号从11至15的版本进行备份，备份文件名称为project2.dump

输入命令：svnadmin dump E:\Repository\project -r 11:15 --incremental > H:\svn_backup\dump\project2.dump

![img](https://images2017.cnblogs.com/blog/1205757/201708/1205757-20170802162808380-460443270.png)

最后对版本号从16至16的版本进行备份，备份文件名称为project3.dump

输入命令：svnadmin dump E:\Repository\project -r 16:16 --incremental > H:\svn_backup\dump\project3.dump

![img](https://images2017.cnblogs.com/blog/1205757/201708/1205757-20170802162831036-1906602654.png)

请注意：上面三个命令中第2，3个命令多了一个--incremental的参数，这就是采用了增量的方式导出版本库。

## 1.2.svnadmin hotcopy方式备份

​        此方法只能进行全量备份，不能进行增量备份，优点是备份较快，灾难恢复也很快，如果备份机上已经搭建了svn服务，甚至不需要恢复，只需要进行简单配置即可切换到备份机上。缺点是比较耗费硬盘空间，需要较大的硬盘支持。

**命令格式为：**

**svnadmin hotcopy** **版本库路径及名称****备份版本库存放路径及名称**

比如：对project版本库进行热备份，将备份文件保存到H盘的svn_backup\hotcopy文件夹中，进入cmd命令行，输入以下命令：

svnadmin hotcopy E:\Repository\project  H:\svn_backup\hotcopy\project_bak

或svnadmin hotcopy E:\Repository\project  H:\svn_backup\hotcopy\project_bak --clean-logs

点击回车后，进入hotcopy文件夹下便可发现已将project库保存下来。

​        这个命令会制作一个版本库的完全“热”拷贝，包括所有的钩子、配置文件，当然还有数据库文件。如果命令后面传递--clean-logs选项，svnadmin会执行热拷贝操作，然后删除不用的Berkeley DB日志文件，可以在任何时候运行这个命令得到一个版本库的安全拷贝，不管其它进程是否使用这个版本库。

## 1.3. svnsync方式备份

​        svnsync是Subversion的一个远程版本库镜像工具，它允许把一个版本库的内容录入到另一个。在任何镜像场景中，有两个版本库：源版本库，镜像(或“sink”)版本库。源版本库就是**svnsync**获取修订版本的库，镜像版本库是源版本库修订版本的目标，两个版本库可以是在本地或远程，它们只是通过URL跟踪。

​        此方法同样只能进行全量备份，它实际上是制作了两个镜像库，当一个坏了的时候可以迅速切换到另一个，它必须是svn1.4版本以上才支持此功能。优点是当制作成两个镜像库的时候可以起到双机实时备份的作用。缺点是当作为两个镜像库使用时，无法做到“想完全抛弃今天的修改恢复到昨天的样子”，而当作为普通备份机制每天备份时，操作又较前两种方法麻烦。具体使用方法见下面的例子。

 

例如：采用svnsync方式同步备份两台机器的SVN，同步的版本库名称为project，SVN版本库在1.4以上

源版本库目录为：svn://10.1.1.156/project

同步版本库目标目录为：http://10.1.1.224/project

\1. 首先在目标目录中创建一个空的版本库，如果目标目录中存在相同的项目，先清除再创建一个新的版本库

命令为：svnadmin create E:\Repository\project

\2. 在目标目录的project版本库的hooks目录下，新建pre-revprop-change.bat

内容只有：exit 0

也可以直接将pre-revprop-change.tmpl更改为pre-revprop-change.bat，然后将其中的内容的最后三行注释掉，或者清空内容只添加一句exit 0

\3. 执行初始化

在目标目录机器中进入cmd命令行，输入如下命令：

svnsync init http://10.1.1.224/project svn://10.1.1.156/project

即svnsunc init 目标库路径 源库路径

注：执行这一步一般会出现问题：svnsync认证失败，解决方法是将源版本库中的conf拷贝到目标版本库的conf中替换。

如果成功则会首先提示输入目标机器的密码，此项输与不输无关紧要，然后提示输入登陆版本库的用户名和密码，建议源版本库和目标版本库设置相同的用户名和密码，输入正确后会提示Copied properties for revision 0.

![img](https://images2017.cnblogs.com/blog/1205757/201708/1205757-20170802164623583-1641474809.png)

\4. 执行同步

在目标目录机器中的cmd命令行，输入如下命令：

svnsync sync http://10.1.1.224/project

即svnsunc sync 目标库路径

​       点击回车后会显示复制的版本号和提交数据等信息。

![img](https://images2017.cnblogs.com/blog/1205757/201708/1205757-20170802164735490-1383935716.jpg)

如果同步时出现“Failed to get lock on destination repos, currently held by 'svn.test.com:0e4e0d98-631d-0410-9a00-9330a90920b3'  

... 

Failed to get lock on destination repos, currently held by 'svn.test.com:0e4e0d98-631d-0410-9a00-9330a90920b3'  

svnsync: Couldn't get lock on destination repos after 10 attempts ”的错误：

是因为版本库属性被锁，删掉属性即可：

svn propdel svn:sync-lock --revprop -r0 http://10.1.1.224/project

\5. 更新同步

1) Windows环境下：

**a. 采用全量备份：**

首先在源版本库project的根目录下添加commit.bat脚本，内容如下：

echo off

set SVN_HOME="C:\Program Files\Subversion"

set SVN_ROOT=E:\Repository

set BACKUP_SVN_ROOT=E:\Repositorybak

set BACKUP_DIRECTORY=%BACKUP_SVN_ROOT%\%date:~0,10%

if exist %BACKUP_DIRECTORY% goto checkBack

echo 建立备份目录%BACKUP_DIRECTORY%>>%SVN_ROOT%/backup.log

mkdir %BACKUP_DIRECTORY%

for /r %SVN_ROOT% %%I in (.) do @if exist "%%I\conf\svnserve.conf" %SVN_ROOT%\simpleBackup.bat "%%~fI" %%~nI

goto end

:checkBack

echo 备份目录%BACKUP_DIRECTORY%已经存在，请清空。

goto end

:end

 

其次同样在project版本库根目录下添加simpleBackup.bat脚本

@echo 正在备份版本库%1……

@%SVN_HOME%\bin\svnadmin hotcopy %1 %BACKUP_DIRECTORY%\%2

@echo 版本库%1成功备份到了%BACKUP_DIRECTORY%\%2！

 

**b.** **采用增量备份：**

首先在源版本库project下的hooks中添加commit.bat脚本，内容如下：

echo off

set SVN_HOME="C:\Program Files\CollabNet\Subversion Server"

set SVN_ROOT=E:\Repository

set DELTA_BACKUP_SVN_ROOT=E:\Repositorybak\delta

set LOG_FILE=%1\backup.log

echo backup revision %2 >> %LOG_FILE%

for /r %SVN_ROOT% %%I in (.) do if /i %SVN_ROOT%\%%~nI == %1 %SVN_ROOT%\%%~nI\hooks\deltaBackup.bat %%~nI %2

goto end

:end

 

同样在hooks中再添加deltaBackup.bat脚本，内容如下

@echo 正在备份版本库%2......

%SVN_HOME%\svnadmin dump %SVN_ROOT%\%1 --incremental --revision %2 >> %DELTA_BACKUP_SVN_ROOT%\%1.dump

%SVN_HOME%\svnsync sync --non-interactive svn://10.1.1.238/%1 --source-username caocan --source-password cc

@echo 版本库%2成功备份到了%3！

2) Linux环境下

同样可以按照Windows环境下的方法进行。

最后需要把post-commit.bat 的权限设置为755权限。如果post-commit脚本出错，或者无可执行权限，在提交更新时会报错。

# 2.备份库恢复

## 2.1svnadmin dump方式的备份库

### a) 全量备份方式备份库的恢复

将备份在H盘的svn_backup文件夹中的project.dump恢复到版本库中。

进入cmd命令行，输入以下命令：

svnadmin load F:\Repository\project<H:\svn_backup\project.dump

点击回车后，如下图所示，已经将备份的project.dump还原到project版本库中。

注：导入备份库到新的SVN库下时，一定要新建库！

![img](https://images2017.cnblogs.com/blog/1205757/201708/1205757-20170802165135193-1294330524.jpg)

### b) 增量备份方法备份库的恢复

首先创建一个新的版本库，名称为project_dump，将前面以增量备份方式备份的三个文件依次导入此版本库中。

首先导入project1.dump文件，进入cmd命令行，输入命令：

svnadmin load E:\Repository\project_dump<H:\svn_backup\dump\project1.dump

点击回车后命令行窗口如下：

![img](https://images2017.cnblogs.com/blog/1205757/201708/1205757-20170802165217208-1743103546.png)

![img](https://images2017.cnblogs.com/blog/1205757/201708/1205757-20170802165330708-76352609.png)

然后再导入project2.dump文件，输入命令：

svnadmin load E:\Repository\project_dump<H:\svn_backup\dump\project2.dump

![img](https://images2017.cnblogs.com/blog/1205757/201708/1205757-20170802165427052-1004417180.png)

最后导入project3.dump文件，输入命令：

svnadmin load E:\Repository\project_dump<H:\svn_backup\dump\project3.dump

![img](https://images2017.cnblogs.com/blog/1205757/201708/1205757-20170802165506708-818621292.png)

至此已将所有的备份文件都导入新版本库后则整个版本库都已恢复，此种方法只能针对备份时采用--incremental方式进行备份的文件进行，否则会提示错误：版本库文件已经存在。

## 2.2.svnadmin hotcopy方式的备份库

利用hotcopy方式备份的备份库是一个功能完全的版本库，当发生严重错误时可以作为活动版本库直接进行替换。

## 2.3.svnsync方式的备份库

利用svnsync方式备份的备份库同样也是一个完整的版本库，当源版本库发生严重错误时可以利用此备份库作为活动版本库进行操作。



分类: [配置管理](https://www.cnblogs.com/bumengru/category/1047152.html)