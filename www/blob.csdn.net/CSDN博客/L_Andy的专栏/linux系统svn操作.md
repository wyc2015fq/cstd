# linux系统svn操作 - L_Andy的专栏 - CSDN博客

2017年06月12日 08:56:07[卡哥](https://me.csdn.net/L_Andy)阅读数：1312


1、将文件checkout到本地目录

svn checkout path（path是服务器上的目录）

例如：svncheckout svn://192.168.1.1/pro/domain

简写：svn co

2、往版本库中添加新的文件

svn add file

例如：svn addtest.php(添加test.php)

svn add *.php(添加当前目录下所有的php文件) 

3、将改动的文件提交到版本库

svn commit -m “LogMessage“ [-N] [--no-unlock] PATH(如果选择了保持锁，就使用–no-unlock开关)

例如：svn commit-m “add test filefor my test“ test.php

简写：svn ci

4、加锁/解锁

svn lock -m “LockMessage“ [--force] PATH

例如：svn lock -m“lock testfile“ test.php

svn unlock PATH 

5、更新到某个版本

svn update -r m path

例如：

svn update如果后面没有目录，默认将当前目录以及子目录下的所有文件都更新到最新版本。

svn update -r 200 test.php(将版本库中的文件test.php还原到版本200)

svn update test.php(更新，于版本库同步。如果在提交的时候提示过期的话，是因为冲突，需要先update，修改文件，然后清除svn resolved，最后再提交commit)

简写：svn up 

6、查看文件或者目录状态

1）svn status path（目录下的文件和子目录的状态，正常状态不显示）

【?：不在svn的控制中；M：内容被修改；C：发生冲突；A：预定加入到版本库；K：被锁定】M状态一般比较多

2）svn status -v path(显示文件和子目录状态)

第一列保持相同，第二列显示工作版本号，第三和第四列显示最后一次修改的版本号和修改人。

注：svn status、svn diff和 svn revert这三条命令在没有网络的情况下也可以执行的，原因是svn在本地的.svn中保留了本地版本的原始拷贝。

简写：svn st 

7、删除文件

svn delete path -m “delete test fle“

例如：svn deletesvn://192.168.1.1/pro/domain/test.php -m “delete test file”

或者直接svn deletetest.php 然后再svn ci -m ‘delete test file‘，推荐使用这种

简写：svn (del,remove, rm)

8、查看日志

svn log path

例如：svn logtest.php 显示这个文件的所有修改记录，及其版本号的变化

9、查看文件详细信息

svn info path

例如：svn infotest.php

10、比较差异

svn diff path(将修改的文件与基础版本比较)

例如：svn difftest.php

svn diff -r m:n path(对版本m和版本n比较差异)

例如：svn diff -r200:201 test.php

简写：svn di 

11、将两个版本之间的差异合并到当前文件

svn merge -r m:n path

例如：svn merge-r 200:205 test.php（将版本200与205之间的差异合并到当前文件，但是一般都会产生冲突，需要处理一下）

12、SVN
帮助

svn help

svn help ci

——————————————————————————

以上是常用命令，下面写几个不经常用的

——————————————————————————

13、版本库下的文件和目录列表

svn list path

显示path目录下的所有属于版本库的文件和目录

简写：svn ls

14、创建纳入版本控制下的新目录

svn mkdir: 创建纳入版本控制下的新目录。

用法: 1、mkdir PATH…

2、mkdir URL…

创建版本控制的目录。

1、每一个以工作副本 PATH 指定的目录，都会创建在本地端，并且加入新增

调度，以待下一次的提交。

2、每个以URL指定的目录，都会透过立即提交于仓库中创建。

在这两个情况下，所有的中间目录都必须事先存在。 

15、恢复本地修改

svn revert: 恢复原始未改变的工作副本文件 (恢复大部份的本地修改)。revert:

用法: revertPATH…

注意: 本子命令不会存取网络，并且会解除冲突的状况。但是它不会恢复

被删除的目录 

16、代码库URL变更

svn switch (sw): 更新工作副本至不同的URL。

用法: 1、switch URL [PATH]

     2、switch –relocate FROM TO [PATH...] 

  1、更新你的工作副本，映射到一个新的URL，其行为跟“svn update”很像，也会将服务器上文件与本地文件合并。这是将工作副本对应到同一仓库中某个分支或者标记的方法。

  2、改写工作副本的URL元数据，以反映单纯的URL上的改变。当仓库的根URL变动

(比如方案名或是主机名称变动)，但是工作副本仍旧对映到同一仓库的同一目录时使用

这个命令更新工作副本与仓库的对应关系。

17、解决冲突

svn resolved: 移除工作副本的目录或文件的“冲突”状态。

用法: resolvedPATH…

注意: 本子命令不会依语法来解决冲突或是移除冲突标记；它只是移除冲突的

相关文件，然后让 PATH 可以再次提交。 

18、输出指定文件或URL的内容。

svn cat 目标[@版本]…如果指定了版本，将从指定的版本开始查找。

svn cat -r PREV filename > filename(PREV 是上一版本,也可以写具体版本号,这样输出结果是可以提交的)

19、查看版本提交修改文件情況。

svn log -r200 - v或者 svn log -l1 -v 比较最新提交版本与当前版本文件修改情况。

注意：-r200为版本号，-l1 为last one最后一个。

20、删除

svn delete－删除文件和目录的实例，

delete (del, remove, rm): 从版本库中删除文件和目录。

用法: 1、delete PATH...

2、delete URL...

1、每个 PATH 指定的项目会被调度到下次提交时从版本库删除。除非给出

--keep-local 参数，工作副本中没有提交的文件和目录会被立即删除。

如果 PATH 是未版本控制或者已修改的项目，或者包含这些项目，那么仅当

给出 --force 参数时这些项目才会被删除。

2、每个 URL 指定的项目会通过立即提交从版本库中删除。

有效选项:

--force: 强制操作运行

-q [--quiet] : 不打印信息，或只打印概要信息

--targets ARG: 传递文件 ARG 内容为附件参数

-m [--message] ARG : 指定日志信息ARG

-F [--file] ARG: 从文件ARG读取日志信息

--force-log: 强制校验日志信息资源

--editor-cmd ARG : 使用 ARG 作为外部编辑器

--encoding ARG : 将ARG的值视为字符编码

--with-revprop ARG : 在新版本设置版本属性 ARG

使用格式name[=value]

--keep-local : 在工作副本中保留路径

全局选项:

--username ARG : 指定用户名称 ARG

--password ARG : 指定密码 ARG

--no-auth-cache: 不要缓存用户认证令牌

--non-interactive: 不要交互提示

--trust-server-cert: 不提示的接受未知的 SSL 服务器证书(只用于选项“--non-interactive”)

--config-dir ARG : 从目录 ARG 读取用户配置文件

--config-option ARG: 以下属格式设置用户配置选项：

 FILE:SECTION:OPTION=[VALUE]

例如：

 servers:global:http-library=serf

例子

Using svn to delete a file from yourworking copy deletes your local copy of the file, but it merely schedules thefile to be deleted from the repository. When you commit, the file is deleted inthe repository.

$ svn delete myfile

D        myfile

$ svn commit -m "Deleted file'myfile'."

Deleting       myfile

Transmitting file data .

Committed revision 14.

www.111cn.net

然而直接删除一个URL，你需要提供一个日志信息：

$ svn delete -m "Deleting file'yourfile'" file:///var/svn/repos/test/yourfile

Committed revision 15.

如下是强制删除本地已修改文件的例子：

$ svn delete over-there 

svn: Attempting restricted operation formodified resource

svn: Use --force to override thisrestriction

svn: 'over-there' has local modifications

$ svn delete --force over-there 

D        over-there

常用操作

1.使用svn从工作拷贝删除文件只是预定要删除，当你提交，文件才会从版本库删除。

$ svn delete myfile

2.然而直接删除一个URL，你需要提供一个日志信息：

$ svn delete -m “Deleting file ‘yourfile’”file:///tmp/repos/test/yourfile

3.强制删除本地已修改文件的例子：

$ svn delete –force over-there

