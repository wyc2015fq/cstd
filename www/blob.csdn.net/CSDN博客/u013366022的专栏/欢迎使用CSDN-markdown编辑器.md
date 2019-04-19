# 欢迎使用CSDN-markdown编辑器 - u013366022的专栏 - CSDN博客
2015年10月13日 14:13:36[slitaz](https://me.csdn.net/u013366022)阅读数：267标签：[visual-studio																[mfc																[excel](https://so.csdn.net/so/search/s.do?q=excel&t=blog)](https://so.csdn.net/so/search/s.do?q=mfc&t=blog)](https://so.csdn.net/so/search/s.do?q=visual-studio&t=blog)
个人分类：[C++操作EXCEL](https://blog.csdn.net/u013366022/article/category/5874313)
向Excel表中导出&导入数据 
一、  向工程中添加Excel操作接口类 
1、  向项目工程中添加基本的7个类，分别是：CApplication，CMyFont，CRange，CWorkBook，CWorkBooks，CWorkSheet，CWorkSheets 
1.1、打开Class Wizard
1.2、选择MFC Class From TypeLib…，
1.3、在Add Class From 中选中File，在Location中输入或者选择EXCEL.EXE（officer2007为EXCEL.EXE，officer2003为EXCEL9.OLB）的路径。然后在Interface框（即1号框）中选择操作Excel的基本接口，分别是：_Application，_WorkBook，_WorkSheet， Range，Font，WorkBooks，WorkSheets。点击Generated classes（即2号框）中CFont0类，在classes（即3号框）和File（即4号框）中修改其类名为CMyFont，头文件名为CMyFont.h（类名和头文件名，也可以是其他名字，由自己确定）。点击Finish完成添加。
1.4、在Class View中可以看见添加进去的7个类
2、  修改这6个类 
2.1、把CApplication.h，CMyFont.h，CRange.h，CWorkBook.h，CWorkBooks.h，CWorkSheet.h，CWorkSheets.h中的“#import “C:\Program Files\Microsoft Office\Office12\EXCEL.EXE” no_namespace”注释掉，然后添加头文件：#include 
