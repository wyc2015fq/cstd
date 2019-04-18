# 精简版ORACLE客户端程序  - ljx0305的专栏 - CSDN博客
2008年03月11日 21:17:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：1789标签：[oracle																[network																[网络																[文档																[工具																[测试](https://so.csdn.net/so/search/s.do?q=测试&t=blog)](https://so.csdn.net/so/search/s.do?q=工具&t=blog)](https://so.csdn.net/so/search/s.do?q=文档&t=blog)](https://so.csdn.net/so/search/s.do?q=网络&t=blog)](https://so.csdn.net/so/search/s.do?q=network&t=blog)](https://so.csdn.net/so/search/s.do?q=oracle&t=blog)
   昨天本想在异地公司的电脑上用TNSPING测试一下网络状况，可是却发现如果不装
ORACLE客户端，TNSPING根本就无法执行。而要在异地装个ORACLE客户端，又谈何容
易，几乎就是恶梦，安装程序压缩后是453MB，安装过程也是费时费力，我仅仅想测
试一下网络，需要这么大费周张吗？以后这种需求还多，所就决定把客户端程序重新
打包。
   1、找一台没装过ORACLE客户端的电脑自定义安装10GR2的CLIENT，仅选择Oracle Net
和其它必选的组件安装。（晕啊，这样装好后居然也还要两百多MB）。
   2、通过查阅文档和利用filemon和regmon观察，rip出以下目录的文件：
        ├─BIN
        ├─NETWORK
        │  ├─ADMIN
        │  └─mesg
        ├─nls
        │  ├─data
        │  └─mesg
        ├─oracore
        │  ├─mesg
        │  └─zoneinfo
        ├─rdbms
        │  ├─mesg
        └─sqlplus
            └─mesg
   3、将注册表HKLM/softwar/oracle导出，存为ora.reg
        ora.reg内容：
        Windows Registry Editor Version 5.00
        [HKEY_LOCAL_MACHINE/SOFTWARE/ORACLE]
        [HKEY_LOCAL_MACHINE/SOFTWARE/ORACLE/KEY_OraClient10g_home1]
        "ORACLE_HOME"="D://oracle"
        "ORACLE_HOME_NAME"="OraClient10g_home1"
        "ORACLE_GROUP_NAME"="Oracle - OraClient10g_home1"
        "ORACLE_BUNDLE_NAME"="Enterprise"
        "NLS_LANG"="SIMPLIFIED CHINESE_CHINA.ZHS16GBK"
        "ORACLE_HOME_KEY"="SOFTWARE//ORACLE//KEY_OraClient10g_home1"
   4、将以上文件除NLS和SQLPLUS目录外的文件加上ora.reg打包成oracle_1.rar
（约11MB），NLS和SQLPLUS打包成oracle_2.rar（约8.5M），另外将PLSQL Developer
打包成plsqldev.rar（约8.7MB）。
   如果仅仅是需要TNSPING，将Oracle_1.rar解压到D:/，然后将Ora.reg(如果解压后
的目录不是D:/oracle，则需先修改注册表文件"ORACLE_HOME"值为解压目录)导入注册
表即可。
   如果需要运行SQLPLUS，还得再将Oracle_2.rar 解压到D:/，然后运行
/oracle/bin/sqlplusw.exe即可。
   如果需要运行PLSQL Developer，还得解压缩plsqldev.rar到任一目录，运行
plsqldev.exe即可。
  5、ORACLE_HOME还支持UNC路径，如果在公司局域网内部使用，那还可以直接以上文件
放在一个公共目录下，修改ORACLE_HOME为相应的UNC路径。为方便使用，可建立相应
批处理文件来导入和删除注册表键值。
   例子：
   我将文件放在公共路径：//files/pub/Oracle
   a.更改ora.reg:
   "ORACLE_HOME"="////files//pub//oracle"
   b.复制WINXP的控制台注册表工具%SystemRoot%/system32/reg.exe 到bin目录。
（WIN2000系统没有该文件）
   c.建立批处理文件sqlplus.bat
      @echo off
      if exist %SystemRoot%/bk_HKLM_SOFT_Oracle.reg copy %SystemRoot%/bk_HKLM_SOFT_Oracle.reg *.reg.bak >nul
      //files/pub/oracle/bin/reg export HKLM/software/oracle %SystemRoot%/bk_HKLM_SOFT_Oracle.reg >nul
      //files/pub/oracle/bin/reg delete HKLM/software/oracle /f >nul
      //files/pub/oracle/bin/reg import //files/pub//oracle/reg/ora.reg >nul
      //files/pub/oracle/bin/sqlplusw.exe >nul
      //files/pub/oracle/bin/reg delete HKLM/software/oracle /f >nul
      //files/pub/oracle/bin/reg import %SystemRoot%/bk_HKLM_SOFT_Oracle.reg >nul
      echo on
   6、软件版本：10.2.0.1.0
      客户端操作系统 WINXP+SP2,WIN2000+SP4.
   7、如果有自己写的应用程序需要这些运行环境，也可将这些文件包含进安装包一起打包发行。
引用:http://www.cnblogs.com/xulei/archive/2006/11/16/562092.html
