# win7下vs2010编译生成sqlite3.lib库 - L_Andy的专栏 - CSDN博客

2018年01月21日 15:10:40[卡哥](https://me.csdn.net/L_Andy)阅读数：161
个人分类：[第三方库编译](https://blog.csdn.net/L_Andy/article/category/7208429)



                
知识补充：

      sqlite3.dll动态链接库，它允许程序共享执行特殊任务所必需的代码和其他资源。

      动态的sqlite3.lib相当于一个h文件，是对实现部分（.dll文件）的导出部分的声明。编译后只是将导出声明部分编译到宿主程序中，其生成的sqlite3.exe在运行时需要相应的dll文件支持 。

静态的sqlite3.lib将导出声明和实现都放在lib中。编译后所有代码都嵌入到宿主程序，其生成的sqlite3.exe可直接运行。

任务：

     一.使用VS2010编译sqlite3，生成动态的sqlite3.lib和sqlite3.dll（sqlite3.dll在官网中也可下载到）。在VS2010中编写简单程序，使用动态的sqlite3.lib和sqlite3.dll。

     二.使用VS2010编译sqlite3，生成静态的sqlite3.lib。在VS2010中编写简单程序，使用静态的sqlite3.lib。 

任务一：

1.在sqlite官网 http://www.sqlite.org/download.html 上下载sqlite-amalgamation-3071000.zip 和sqlite-dll-win32-x86-3071000.zip（我用的此版本）。

2.分别解压上述两个文件到各自文件夹下（sqlite3.def、sqlite3.dll在同一文件夹sqlite-dll下）。

3.从VS2010的安装目录下Microsoft Visual Studio 10.0\VC\bin找到lib.exe和link.exe，从VS2010的安装目录下G:\Microsoft Visual Studio 10.0\Common7\IDE找到mspdb100.dll。将lib.exe link.exe mspdb100.dll放到步骤2中的sqlite-dll文件夹下。

4.打开cmd窗口，转到上述sqlite-dll文件夹下，我的是E:\SoftwareDesign\SQLite\sqlite-dll

![](https://img-blog.csdn.net/20180121150611486?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvTF9BbmR5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

5.输入命令：LIB /DEF:sqlite3.def /machine:IX86。这时，在sqlite-dll文件夹下会出现sqlite3.lib。（这是动态的sqlite3.lib文件，只有47KB大小。）下边列子为64生成sqlite3.lib

![](https://img-blog.csdn.net/20180121151007928?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvTF9BbmR5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

6.在VS2010中编写简单程序。文件->新建->项目->Win32控制台应用程序，命名为test1 。

7.将test1.cpp中的内容替换为以下代码：
![](https://images.cnblogs.com/OutliningIndicators/ExpandedBlockStart.gif)

![复制代码](http://common.cnblogs.com/images/copycode.gif)
 1// test1.cpp : 定义控制台应用程序的入口点。
 2//
 3 #include "stdafx.h" 4 #include <stdlib.h>
 5 #include "sqlite3.h" 6int _tmain(int argc, _TCHAR* argv[])
 7{
 8int rc;
 9int i, nrows, ncols, tr;
10char *errmsg = NULL;
11char **results;
1213     sqlite3 *db = NULL;
14     rc = sqlite3_open("demodb", &db);
15if (rc)
16    {
17         fprintf(stderr, "can't open db!\n", sqlite3_errmsg(db));
18        sqlite3_close(db);
19         exit(1);
20    }
21else22    {
23         printf("db open successfully!\n");
24    }
25     sqlite3_get_table(db,"select * from clients;",&results,&nrows,&ncols,&errmsg);
26     printf("DB has %d rows and %d cols\n\n",nrows,ncols);
27     tr=(nrows+1)*ncols;
28for(i=0;i<tr;++i) //输出查询结果29    {
30         printf("results[%d]= %s/n",i,results[i]); //此处可以自己定义输出格式，31    }
32     sqlite3_free_table(results); //free 33    sqlite3_close(db);
34int a;
35     scanf("%d",&a);//没有此行代码，运行窗口出现后会马山消失。36return0;
37 }

View Code

8.将sqlite3.h、sqlite3.lib、sqlite3.dll复制到工程所在文件目录中。我的是E:\SoftwareDesign\SQLite\VisualStudio2010Workspace\Projects\test1\test1，如下图所示：

![](https://img-blog.csdn.net/20180121150657700?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvTF9BbmR5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

9.在VS2010中，右键test1工程，添加->现有项，在出现的对话框中选择上一步中的sqlite3.lib。（必须将lib文件再添加到工程中）

10.按F5键，运行成功。

PS：这时，到工程所在文件目录下找到test1.exe（大小为29KB），我的是在E:\SoftwareDesign\SQLite\VisualStudio2010Workspace\Projects\test1\Debug。如果直接双击运行的话，会提示因缺少sqlite3.dll文件而无法运行。这是因为动态的sqlite3.lib只是将声明部分编译到test1.exe中，运行时需要dll文件的支持。将sqlite3.dll文件复制到test1.exe同一目录下，再双击运行test1.exe，就会运行成功。

任务二：

1.在VS2010中，文件->新建->项目->Win32项目，命名为sqlite3(其他名称也行)。在向导设置中，选择DLL(D)、空项目。

2.将下载的sqlite-amalgamation-3071000.zip 中的“sqlite3.h”、“sqlite3.c”、“sqlite3ext.h”三个文件添加到工程中，具体做法：右键sqlite3工程->添加->现有项，在出现的对话框中选择上述三个文件。

3.（release版本比debug版本体积小，运行速度快，所以操作中我发布的是release版本）如下图所示:
![](https://img-blog.csdn.net/20180121150742275?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvTF9BbmR5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

4.按F7键编译（或右键->生成）。在工程所在文件目录中会编译出sqlite3.dll文件。（PS：网上有的讲解需要配置很多的参数等等，试了很多次都不成功，然而按照上述无需多余操作即可成功。可能是VS或sqlite版本不同的原因）

5.编译的同时会在工程所在目录中产生sqlite3.obj文件，我的是在E:\SoftwareDesign\SQLite\VisualStudio2010Workspace\Projects\sqlite3\sqlite3\Release下。接下来要用到VS安装目录下的lib.exe文件。将sqlite3.obj文件复制到lib.exe所在目录下，我的是G:\Microsoft Visual Studio 10.0\VC\bin。将G:\Microsoft Visual Studio 10.0\Common7\IDE下的mspdb100.dll,mspdbcore.dll,mspdbsrv.exe,msobj100.dll复制到lib.exe所在目录下。

6.打开cmd窗口，转到lib.exe所在文件夹下，我的是G:\Microsoft Visual Studio 10.0\VC\bin。输入命令：lib sqlite3.obj。这时会生成静态的sqlite3.lib（大小为2.32MB）。

![](https://img-blog.csdn.net/20180121150823594?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvTF9BbmR5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

7.在VS2010中编写简单程序。文件->新建->项目->Win32控制台应用程序，命名为test2 。

8.test2.cpp中的代码替换为上述的代码。（任务一步骤7中的代码）

9.将sqlite3.h和步骤6中生成的静态sqlite3.lib复制到test2工程所在的文件目录中，我的是E:\SoftwareDesign\SQLite\VisualStudio2010Workspace\Projects\test2\test2。

10.在VS2010中，右键test2工程->添加->现有项。选择sqlite3.lib。

11.按F5键，运行成功。

PS：这时，在E:\SoftwareDesign\SQLite\VisualStudio2010Workspace\Projects\test2\Debug下会生成test2.exe（大小为501KB）。双击可以直接运行。此时不需dll文件支持。因为静态的sqlite3.lib将导出声明和实现都放在lib中，编译后所有代码都嵌入到test2.exe，可直接运行。
            

