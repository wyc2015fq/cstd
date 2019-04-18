# Win10 64位系统ADO方式操作数据库失败解决方法 - 深之JohnChen的专栏 - CSDN博客

2017年07月30日 21:44:10[byxdaz](https://me.csdn.net/byxdaz)阅读数：3818


       VC操作Access数据库一般通过ODBC、ADO、DAO等方式，但在我的Win10 64位操作系统中，通过ADO方式操作数据库会失败，无法读取数据。
解决方法：
1、首先确保Win10操作系统ado目录下是否安装了msado15.dll文件。如果没有就下载Win10版本的ADO组件包。
 Win10版本的ADO组件包，文件版本号是10.0.14393.0，以10.0开头的或含有 win10 信息的一般是windows10的文件。
   下载地址：[http://download.csdn.net/detail/byxdaz/9915729](http://download.csdn.net/detail/byxdaz/9915729)
   将解压的msado15.dll文件剪切放在Win10操作系统ado目录下，Win10 64位操作系统ado目录为： C:\Program Files (x86)\Common Files\System\ado
2、注册ADO
1)、单击“开始”->“运行”
2)、输入如下信息： 
    regsvr32 "C:\Program Files (x86)\Common Files\System\ado\msado15.dll"
3)、单击“确定”按钮，就完成注册。

