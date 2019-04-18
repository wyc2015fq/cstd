# SVN版本库无损迁移与自动备份（一） - z69183787的专栏 - CSDN博客
2015年01月16日 13:11:39[OkidoGreen](https://me.csdn.net/z69183787)阅读数：748
引：最近正在做版本库迁移和自动备份，在网上找过一些相关资料，但都比较凌乱，让人很纠结，相信很多网友会遇到相同的问题，笔者根据自己的整理和实践结果总结了一套可操作（经过实际验证）的方案，打算用两篇博客与大家分享一下，供大家参考。
**一、业务目标**
1、在不改变原来版本库的内容和版本号的前提下，把原来分散在多个服务器上的各个版本库统一迁移到一台服务器上的新版本库上。
2、实现新版本库的定时自动备份。
**二、相关指令**
1、svnadmin dump命令语法
svnadmin dump Repository_Path [-r LOWER[:UPPER]] [--incremental]
（1）svnadmindump命令用于导出整个Repository或Repository下的某个范围的修订版本。
（2）参数说明：
Repository_Path是版本库的路径，
[-rLOWER[:UPPER]]用于指定导出的修订版本范围，由参数-r和两个用:号隔开阿拉伯数字组成。
例如：-r 0:100表示导出才版本0到版本100之间的所有修订版，-r是revision的缩写。
--incremental，它使用增量方式来导出版本，即每次都只导出自上一个版本以来的修改。这样的好处是第一：可以把一个大的文件切分成若干个小的文件。第二：在版本库已经存在的情况下，我们只需要每次导出修改的部分，不需要每次都导出整个版本库的内容。甚至可以通过hook脚本，每天晚上自动将当天的修改dump出来做备份用。
2、svnadmin load命令语法
svnadmin load Repository_Path
（1）svnadminload命令用于从标准输入流/其它流中导入版本库，
（2）参数说明：
Repository_Path是要导入的目标版本库。
3、dump和load的输出/入重定向
svnadmin dump oldRepository> dumpfile    
svnadminload newRepository < dumpfile
默认情况下dump和load命令分别输出到默认输出设备(屏幕)和从默认输入设备(键盘)导入。但我们也可以把输出流/输入流重定向。例如上面的第一个命令，使用重定向符>把屏幕的输出定向当前目录下的dumpfile，而第二个命令从当前目录下的dumpfile文件导入。
4、把导出和导入合并。
svnadmin dump oldRepository| svnadmin load newRepository
5、过滤器svndumpfilter
用于指定只包括那些项目，不会包括其它的项目
**二、迁移版本库（解决方案示例）**
**方案1、一次全部迁移。**
首先新建三个批处理文档（新建记事本，后缀改为.bat）
①导出.bat
svnadmin dump oldRepository > dumpfile
②新建版本库.bat
         svnadmin create newRepostitory
③导入.bat
        svnadmin load newRepository < dumpfile
    步骤：
如果你的SVN装在C：\program files下，那么：
a、将“导出.bat”放在原库目录下，即oldRepository所在目录下（例如D:\Repositories），双击执行，导出版本库！
b、将“新建版本库.bat”放在新库目录下，即newRepostitory要放的位置（例如E:\Repositories）,双击执行，新建版本库！
c、将“导入.bat”放到新库目录下（例如刚才的E:\Repositories）,双击执行，导入版本库！
如果你的SVN不是装在上述位置，那么：
这三个批处理文件，要全部放在SVN安装目录的Bin目录下，而且也不能单纯的写文件名就可以了，要写完整的文件名。
例如svnadmin dump D:\SVN版本库\oldrepository > D:\dumpfile
说明：上述步骤即实现将oldRepository版本库无损迁移到newRepository。这里是采用批处理文件的形式，完全可以在命令提示符窗口下，以命令的形式完成上述操作，注意必须在相应的目录下执行。
**方案2、分批增量迁移版本库。**
①查看当前旧版本库最新的版本号是多少
在命令提示符窗口，打开库所在目录，例如：cd D:\Repositories。
执行svnlook youngest oldRepositories
例如返回版本为281
②分批增量导出版本库内容
D:\Repositories\svnadmin dump oldRepository -r 0:100 > dumpfile1
导出第一个文件，版本号从0到100的修订版本
D:\Repositories\svnadmin dump oldRepository -r 101:200 --incremental > dumpfile2
导出第二个文件，版本号从101到200的修订版本
D:\Repositories\svnadmin dump oldRepository -r 201:281 --incremental > dumpfile3
导出第三个文件，版本号从201到281的修订版本
注：三个命令中第2，3个命令多了一个--incremental的参数，使其采用了增量的方式导出，
③分批导入版本库文件
注：打开要导入的版本库所在目录，例如cd E:\Repositories。
首先导入dumpfile1，然后是dumpfile2，dumpfile3
依次执行
E:\Repositories\svnadmin load newRepository < dumpfile1
E:\Repositories\svnadmin load newRepository < dumpfile2
E:\Repositories\svnadmin load newRepository < dumpfile3
可能会出现的问题，提示错误：版本库文件已经存在。请确认前边导出时，是否使用了--incremental参数。
说明：这里我们是在命令提示符窗口下进行的。同样的，我们也可以按照方案1，采用写批处理文件的方式。
注：要根据自己的svn安装目录，和库目录写命令，例如：
C:\Program Files\VisualSVN Server\bin\svnadmin load D:\Repositories\newRepository < E:\dumpfile1
**方案3、导出后，在导入时对库做分库处理或其它处理操作过滤版本库历史。**
假设有一个包含三个项目的版本库oldRepository：Project1，Project2，和 Project3。它们在版本库中的布局如下：
/
  Project1/
      trunk/
      branches/
      tags/
   Project2/
      trunk/
      branches/
      tags/
   Project3/
      trunk/
      branches/
      tags/
现在要把这三个项目转移到三个独立的版本库中。
①利用上面介绍的方案1导出整个版本库：
svnadmin dump oldRepository> dumpfile
②将转储文件三次送入过滤器，每次仅保留一个顶级目录，就可以得到三个转储文件：
cat dumpfile | svndumpfilter include Project1> 1-dumpfile
cat dumpfile | svndumpfilter include Project2 > 2-dumpfile
cat dumpfile | svndumpfilter include project3 > 3-dumpfile
注：cat是subversion的文档中，关于svndumpfilter介绍给出的命令，在windows下并没有，与cat类似的命令是type，可以采用typedumpfile | svndumpfilter include Project1> 1-dumpfile
③这三个转储文件中，每个都可以用来创建一个可用的版本库，不过它们保留了原版本库的精确路径结构。
也就是说，虽然项目Project1现在独占了一个版本库，但版本库中还保留着名为Project1的顶级目录。如果希望trunk、tags和branches这三个目录直接位于版本库的根路径下，你可能需要编辑转储文件，调整Node-path和Copyfrom-path头参数，将路径Project1/删除。
同时删除转储数据中创建Project1目录的部分。一般为如下的一些内容：
 Node-path: Project1
 Node-action: add
 Node-kind: dir
 Content-length: 0
注:手工编辑转储文件来移除一个顶级目录时，不要让编辑器将换行符转换为本地格式（比如将\r\n转换为\n），很容易造成转储文件失效。
④最后，我们可以采用方案1提供的方法，将三个转储文件分别导入：
svnadmin create Project1
svnadmin load Project1< 1-dumpfile
svnadmin create Project2
svnadmin load Project2< 2-dumpfile
svnadmin create Project3
svnadmin load Project3< 3-dumpfile
**迁移版本库的解决方案就先写到这，下篇博客中，我们将介绍定时自动备份版本库的解决方案。**
