# PLSQL中文乱码问题 - z69183787的专栏 - CSDN博客
2013年12月21日 12:33:16[OkidoGreen](https://me.csdn.net/z69183787)阅读数：917
机器 中病毒 后，plsql数据库中的字段成了乱码 
重装 系统后，plsql中查询出来的字段仍为 乱码
*plsql* 中显示的数据和数据库的中文字段全是*乱码*
服务器查询出来的数据，如果中文显示的就是*乱码* 。
### [*plsql乱码* 如何解决？](http://topic.csdn.net/u/20071017/18/46cfeb0b-fccf-4d7f-a6db-eaaf8511a2ca.html)
设置环境变量：
NLS_LANG SIMPLIFIED CHINESE_CHINA.ZHS16GBK
解决PL/SQL和TOAD中文乱码问题： 
regedit->hkey_local_machine->software->oracle->home0->NLS_LANG value:AMERICAN_AMERICA.WE8ISO8859P1” 
那位朋友遇见过同样的问题，请指教，谢谢了！
创建一个名为“NLS_LANG”的系统环境变量，设置其值为“SIMPLIFIED CHINESE_CHINA.ZHS16GBK”，然后重新启动 pl/sql developer，这样检索出来的中文内容就不会是乱码了。如果想转换为UTF8字符集，可以赋予“NLS_LANG”为 “AMERICAN_AMERICA.UTF8”，然后重新启动 pl/sql developer。其它字符集设置同上。
