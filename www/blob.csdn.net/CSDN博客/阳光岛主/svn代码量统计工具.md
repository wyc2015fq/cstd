
# svn代码量统计工具 - 阳光岛主 - CSDN博客

2014年07月15日 19:50:48[阳光岛主](https://me.csdn.net/sunboy_2050)阅读数：16303个人分类：[Script																](https://blog.csdn.net/sunboy_2050/article/category/694817)



StatSVN介绍
StatSVN是一个Java写的开源代码统计程序，从statCVS移植而来，能够从Subversion版本库中取得信息，然后生成描述项目开发的各种表格和图表。比如：代码行数的时间线；针对每个开发者的代码行数；开发者的活跃程度；开发者最近所提交的；文件数量；平均文件大小；最大文件；哪个文件是修改最多次数的；目录大小；带有文件数量和代码行数的Repository tree。StatSVN当前版本能够生成一组包括表格与图表的静态HTML文档。

**StatSVN下载**
StartSVN官网地址为：[http://www.statsvn.org/index.html](http://www.statsvn.org/downloads.html)
StartSVN的下载页面为：[http://www.statsvn.org/downloads.html](http://www.statsvn.org/downloads.html)
现在官网上最新的版本为：[statsvn-0.7.0](http://sourceforge.net/projects/statsvn/files/statsvn/0.7.0/statsvn-0.7.0.zip)

**StatSVN使用**
**使用须知**
StatSVN的运行需要Java的运行环境支持，所以大家需要安装Java的运行环境（Java Runtime Environment）。JRE可以从Sun的网站上下载。
Statsvn在使用中需要使用SVN的客户端，因此需要确保机器上可以访问到SVN的客户端命令
**Checkout工作拷贝**
首先从SVN仓库中checkout一个需要统计的路径（如果在工作目录下进行统计，首先请更新，保证工作区中的版本是最新的版本，确保统计结果的准确性），例如我把我的某个路径下的工程checkout在我的电脑上的 D:\MyProjects 路径下。
**生成svn log文件**
首先通过命令行进入工作目录：D:\MyProjects ，再使用svn log -v --xml > logfile.log的命令，其中 logfile.log为log文件的名称，可以根据需要自行定义。这样就在工作拷贝的目录下生成一个名称为logfile.log的文件。
注：要在命令行中使用svn命令，在安装TortoiseSVN时必须选择安装commend组件，可以在cmd命令行里输入svn help测试一下该组件是否安装，如果未安装是无法使用svn log命令的。如果能够操作svn server的话也可以直接在服务器上生成svn log然后下载到本地来使用
**调用StatSVN进行统计**
首先我们把从官网上下载的statsvn-0.7.0.zip包解压缩到D:\statsvn-0.7.0目录下
通过命令行进入D:\statsvn-0.7.0目录
调用命令java -jar statsvn.jar D:\MyProjects\logfile.log D:\MyProjects，命令运行成功即完成了统计工作。
该命令的格式是java -jar statsvn.jar [options] <logfile> <checked-out-module>
参数<logfile>为前一步中生成的svn log文件，<checked-out-module>为checkout工作拷贝目录，注意两个参数都要列出正确的全路径，否则会提示错误如logfile.log找不到等等
<logfile>          path to the svn logfile of the module
<directory>        path to the directory of the checked out module
[options]为可选参数，该参数格式及用法如下：
Some options:
-version            print the version information and exit
-output-dir <dir>         directory where HTML suite will be saved
-include <pattern>        include only files matching pattern, e.g. **/*.c;**/*.h
-exclude <pattern>    exclude matching files, e.g. tests/**;docs/**
-tags <regexp>        show matching tags in lines of code chart, e.g. version-.*
-title <title>            Project title to be used in reports
-viewvc <url>         integrate with ViewVC installation at <url>
-trac <url>           integrate with Trac at <url>
-bugzilla <url>           integrate with Bugzilla installation at <url>
-username <svnusername> username to pass to svn
-password <svnpassword> password to pass to svn
-verbose            print extra progress information
-xdoc                   optionalswitchoutput to xdoc
-xml                    optionalswitchoutput to xml
-threads <int>            how many threadsforsvn diff (default:25)
-concurrency-threshold <millisec>switchto concurrent svn diffif1st call>threshol
-dump               dump the Repository content on console
-charset <charset>        specify the charset to useforhtml/xdoc
-tags-dir <directory>     optional, specifies the directorfortags (default'/tags/')
Full options list: http://www.statsvn.org

**1. 先导出svn log**
svn log -v --xml -rStartrevision:Endrevision > svn.log local_project
其中Startrevision和Endrevision用来导出一个revision段的svn日志.local_project是svn上的project checkout到本地的结果.
**2. 通过statsvn工具做分析**
java -jar statsvn.jar svn.log local_project
运行完成后,就会在$PWD(unix下)或者%CD%(windows下)下生成对应的分析文件,在index.html文件中就有代码量统计.
![](https://img-blog.csdn.net/20130708201519531)


```python
#!/bin/bash
svn_dir='/home/homer/work/code_svn/weiguan'
statsvn_dir='/home/homer/work/tool-server/statsvn-0.7.0/statsvn.jar'
log_dir=svnstat
log_file="$log_dir/svnstat.log"
log_day="$log_dir/2014-01-01_00:00:00"
version_start=4150
version_end=4159
function statsvn() {
    cd $svn_dir
    svn up
    if [ ! -d $log_dir ];then
        mkdir $log_dir
    fi
    date=$(date "+%Y-%m-%d_%H:%M:%S")
    echo "$date"
    
    lines=`find . -name *.java | xargs wc -l | sort -n`
    echo "total code lines : $lines"
    version_end=`svn log -l1 | sed -n 2p | awk '{print $1}' | cut -d "r" -f2`
    echo "version_start : $version_start; version_end : $version_end"
    svn log -v --xml -r$version_start:$version_end > $log_file
    log_day="$log_dir/$date"
    java -jar $statsvn_dir $log_file . -output-dir $log_day > /dev/null 2>&1

    google-chrome $log_day/index.html &
}
statsvn
```


**参考推荐：**
[statsvn统计svn中的代码量](http://chenzhou123520.iteye.com/blog/1436653)
[统计svn上代码量的方法--使用statsvn工具](http://www.cnblogs.com/leipei2352/archive/2011/04/26/2029594.html)
[统计分析svn用户每天提交的代码数](http://tech.lezi.com/archives/268)
[一个基于SVN 的代码提交量统计工具](http://blog.csdn.net/hacking_in_my_mind/article/details/7662845)
[StatSVN](http://sourceforge.net/projects/statsvn/?source=dlp)



