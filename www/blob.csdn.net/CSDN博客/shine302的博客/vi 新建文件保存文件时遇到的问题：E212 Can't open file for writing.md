# vi 新建文件保存文件时遇到的问题：E212: Can't open file for writing - shine302的博客 - CSDN博客
2018年04月24日 09:48:39[SmartMap_Joule](https://me.csdn.net/shine302)阅读数：679

用vi 新建一个文件，hello.c 或者hello ,然后 :wq 进行保存退出，退出不了，一直出现这个提示
![](https://img-blog.csdn.net/20161012090602697?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
意思是不能保存。
原因是权限不够，普通用户用vi 进行不了保存，需要使用超级用户才可以
命令：sudo su     转换成超级用户
           vi hello       打开文件 ：wq 即可保存退出
