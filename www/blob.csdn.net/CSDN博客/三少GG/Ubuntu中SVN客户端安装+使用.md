# Ubuntu中SVN客户端安装+使用 - 三少GG - CSDN博客
2011年09月28日 16:36:24[三少GG](https://me.csdn.net/scut1135)阅读数：58711标签：[svn																[ubuntu																[subversion																[path																[delete](https://so.csdn.net/so/search/s.do?q=delete&t=blog)](https://so.csdn.net/so/search/s.do?q=path&t=blog)](https://so.csdn.net/so/search/s.do?q=subversion&t=blog)](https://so.csdn.net/so/search/s.do?q=ubuntu&t=blog)](https://so.csdn.net/so/search/s.do?q=svn&t=blog)
个人分类：[Ubuntu/OS X系统](https://blog.csdn.net/scut1135/article/category/680808)
                
1、 安装 svn客户端：**apt-get install subversion**，然后根据提示一步一步，就完成了 svn的安装。当然，也可以源码安装 svn，下载 subversion 一个最新版本的源码包，解压之后就可以安装了。 
![](http://hi.csdn.net/attachment/201109/28/0_1317198493tF2Z.gif)
2、 新建一个目录，cd 到新建目录下，将文件 checkout 到本地目录：svn checkout svn://192.168.100.249/server ，按提示输入相应的用户名和密码。 
3、 可以输入：svn help 来查看 svn提供的命令。 
4、 往版本库中添加新的文件，例如: svn add *.c (添加当前目录下所有的 c文件)。 
5、 将改动的文件提交到版本库，例如：svn commit -m “add test file for my test“  test.c 。 
6、 删 除 文 件 ， 例 如 ： svn delete svn://192.168.100.249/server/common/test.c -m “delete test file” 。 
7、 如果哪个命令不会使用可以通过输入：svn help 命令 的形式来查看帮助信息，例如：
      svn help chechout  
> 
输出信息如下： 
checkout (co): Check out a working copy from a repository. 
usage: checkout URL[@REV]... [PATH] 
…………………………………………………………………… 
**[ubuntu SVN命令大全](http://apps.hi.baidu.com/share/detail/21354935)**
**1、**将文件checkout到本地目录
**svn checkout** path（path 是服务器上的目录）
例如：svn checkout svn://**192.168**.**1.1**/pro/domain
简写：svn co
**2、**往版本库中添加新的文件
**svn ****add**file
例如：svn add test.php(添加test.php)
svn add*.php(添加当前目录下所有的php文件)
**3、**将改动的文件提交到版本库
**svn commit** -m “LogMessage“**[**-N**]****[**--no-unlock**]** PATH![](http://hiphotos.baidu.com/yc2266/pic/item/7d1ed21b9d9850ebad6e75ce.jpg)(如果选择了保持锁，就使用–no-
 unlock开关)
例如：svn commit -m “add test file for my test“ test.php
简写：svn ci
**4、**加锁/解锁
**svn lock** -m “LockMessage“**[**--force**]** PATH![](http://hiphotos.baidu.com/yc2266/pic/item/0ef41bd5380296fa51da4bce.jpg)
例如：svn lock -m “lock test file“ test.php
**svn unlock** PATH
**5、**更新到某个版本
**svn update -r **m path
例如：
svn update如果后面没有目录，默认将当前目录以及子目录下的所有文件都更新到最新版本。
svn update -r 200 test.php(将版本库中的文件test.php还原到版本200)
svn update test.php(更新，于版本库同步。如果在提交的时候提示过期的话，是因为**冲突，需要先update，修改文 件**，然后清除**svn resolved**，最后再提交commit)
简写：svn up
**6、**查看文件或者目录状态
1）**svn status** path![](http://hiphotos.baidu.com/yc2266/pic/item/ad6eddc4c6b6caf539db49ce.jpg)（目录下的文件和子目录的状态，正常状态不显示）
【?：不在svn的控制中；M：内容被修改；C：发生冲突；A：预定加入到版本库；K：被锁定】
2）**svn status -v** path![](http://hiphotos.baidu.com/yc2266/pic/item/51da81cba0b7fdc352664fce.jpg)(显示
 文件和子目录状态)
第一列保持相同，第二列显示工作版本号，第三和第四列显示最后一次修改的版本号和修改人。
注：svn status、svn diff和 svn revert这三条命令在没有网络的情况下也可以执行的，原因是svn在本地的.svn中保留了本地版本的原始拷贝。
简写：svn st
**7、**删除文件
**svn delete** path -m “delete test fle“
例如：svn delete svn://192.168.1.1/pro/domain/test.php -m “delete test file”
或者直接svn delete test.php 然后再svn ci -m ‘delete test file‘，推荐使用这种
简写：svn (del, remove, rm)
**8、**查看日志
**svn log** path
例如：svn log test.php 显示这个文件的所有修改记录，及其版本号的变化
**9、**查看文件详细信息
**svn info** path
例如：svn info test.php
**10、**比较差异
**svn diff** path(将修改的文件与基础版本比较)
例如：svn diff test.php
**svn diff** -r m:n path(对版本m和版本n比较差异)
例如：svn diff -r 200:201 test.php
简写：svn di
**11、**将两个版本之间的差异合并到当前文件
**svn merge -r** m:n path
例如：svn merge -r 200:205 test.php（将版本200与205之间的差异合并到当前文件，但是一般都会产生冲突，需要处理一下）
**12、**SVN 帮助
**svn helpsvn help ci**
——————————————————————————
以上是常用命令，下面写几个不经常用的
——————————————————————————
**13、**版本库下的文件和目录列表
**svn list **path
显示path目录下的所有属于版本库的文件和目录
简写：svn ls
**14、**创建纳入版本控制下的新目录
**svn mkdir**: 创建纳入版本控制下的新目录。
用法: 1、mkdir PATH…
2、mkdir URL…
创建版本控制的目录。
1、每一个以工作副本 PATH 指定的目录，都会创建在本地端，并且加入新增
调度，以待下一次的提交。
2、每个以URL指定的目录，都会透过立即提交于仓库中创建。
在这两个情况下，所有的中间目录都必须事先存在。
**15、**恢复本地修改
**svn revert**: 恢复原始未改变的工作副本文件 (恢复大部份的本地修改)。revert:
用法: revert PATH…
注意: 本子命令不会存取网络，并且会解除冲突的状况。但是它不会恢复
被删除的目录
**16、**代码库URL变更
**svn switch (sw):** 更新工作副本至不同的URL。
用法: 1、switch URL [PATH]
2、switch –relocate FROM TO [PATH...]
1、更新你的工作副本，映射到一个新的URL，其行为跟“svn update”很像，也会将
服务器上文件与本地文件合并。这是将工作副本对应到同一仓库中某个分支或者标记的
方法。
2、改写工作副本的URL元数据，以反映单纯的 URL上的改变。当仓库的根URL变动
(比如方案名或是主机名称变动)，但是工作副本仍旧对映到同一仓库的同一目录时使用
这个命令更新工作副本与仓库的对应关系。
**17、**解决冲突
**svn resolved:** 移除工作副本的目录或文件的“冲突”状态。
用法: resolved PATH…
注意: 本子命令不会依语法来解决冲突或是移除冲突标记；它只是移除冲突的
相关文件，然后让 PATH 可以再次提交。
**18、**输出指定文件或URL的内容。
**svn cat** 目标[@版本]…如果指定了版本，将从指定的版本开始查找。
svn cat -r PREV filename > filename (PREV 是上一版本,也可以写具体版本号,这样输出结果是可以提交的)
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
1、首先需要安装SVN。Ubuntu下的SVN安装十分简单，sudo apt-get install subversion，然后根据提示一步一步，就完成了SVN的安装；
2、检出文件（checkout）。
     使用命令：svn co http://{svn repository url} /destination 
     然后系统会用当前的用户名登录，提示输入密码，如果第一次密码输入错误，会提示你输入用户名；
     输入正确后，就可以检出文件了。
3、提交文件（commit）。
      进入需要更新的目录，输入命令：
      svn commit -m path-to-commit，其中path-to-commit可以为空，成功后会提示更新后的版本号。
4、更新文件（update）。
     svn update，在要更新的目录运行这个命令就可以了。
5、查看日志（log）。
     svn log path
6、**为Eclipse配置SVN。**
     http://subclipse.tigris.org/files/documents/906/38385/site-1.2.3.zip，可以从这个地址下载Eclipse的插件，拷贝到plugins目录中以后，重启Eclipse就可以打开SVN的视图了。
    也可以通过官方的 安装页面来进行在线安装：http://subclipse.tigris.org/install.html
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
[http://www.blogjava.net/zhangyuan/archive/2010/07/28/327371.html](http://www.blogjava.net/zhangyuan/archive/2010/07/28/327371.html)
+++++++++++++++++++++++++++
