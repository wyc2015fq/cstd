# ORM:ODB安装使用过程 - DoubleLi - 博客园






1.下载odb-2.4.0-i686-windows，是cpp和sql文件生成工具，已经编译好了，如果下odb-2.4.0估计是未编译好的这个项目；



2.将...\odb-2.4.0-i686-windows\bin添加到环境变量Path，以便能使用odb.exe进行命令生成文件；



3.cmd进入到HelloWorld的person.hxx的同目录，输入odb -d mysql --generate-query person.hxx

生成person-odb.cxx、person-odb.hxx、person-odb.ixx三个文件；




3.下载libodb-2.4.0，这个是普通的lib，编译；



4.下载libodb-sqlite-2.4.0,这个是用sqlite数据库用到的，如果需要用mysql数据库，就得下载libodb-mysql-2.4.0版本的，特别注意，在libodb-sqlite-2.4.0项目有一个**LIBODB_SQLITE_DYNAMIC_LIB**预处理宏，如果在编译sqlite.lib时没有添加**SQLITE_ENABLE_UNLOCK_NOTIFY预处理宏，编译**libodb-sqlite-2.4.0就会报错，所找不到connection函数的定义，我是直接下载别人编译好的sqlite.lib，就出现了这问题，估计是没有添加这个预处理宏了，那怎么办，那就将libodb-sqlite-2.4.0项目的LIBODB_SQLITE_DYNAMIC_LIB预处理宏定义去掉，就可以成功编译通过。




5.odb -d mysql --generate-query --generate-schema person.hxx（手册里用这句生成了person.sql，而换成sqlite的话又没有东西生成，估计sqlite不需要吧）。



6.在官网提供的hello例子项目里添加：

...........\odb2.4.0\libodb-2.4.0;

............\odb2.4.0\libodb-sqlite-2.4.0;

.............\sqlite3;
头文件导入目录，其中省略号为你的文件夹对应的路径。



lib目录也是需要的：

..............\odb2.4.0\libodb-2.4.0\lib;

...............\odb2.4.0\libodb-sqlite-2.4.0\lib

同事也得添加依赖：


odb-sqlite-d.lib

odb-d.lib



之后就可以运行了！



下载地址：[http://www.codesynthesis.com/products/odb/download.xhtml](http://www.codesynthesis.com/products/odb/download.xhtml)



![](http://img.blog.csdn.net/20150402231811292?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcGFteHk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

ODB Compiler为ODB生成数据库支持代码文件工具，下windows版本即可；（必须）

Common Runtime Libraries 核心lib库；（必须）

Database Runtime Libraries 数据库lib库；（必须，下自己需要用到的数据库对应的版本即可）

其他的随意。










