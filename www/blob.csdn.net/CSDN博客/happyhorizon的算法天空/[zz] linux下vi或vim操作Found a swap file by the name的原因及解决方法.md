# [zz] linux下vi或vim操作Found a swap file by the name的原因及解决方法 - happyhorizon的算法天空 - CSDN博客
2016年01月05日 22:23:22[lxy_Alex](https://me.csdn.net/happyhorizion)阅读数：553标签：[linux																[ubuntu																[shell																[vim](https://so.csdn.net/so/search/s.do?q=vim&t=blog)](https://so.csdn.net/so/search/s.do?q=shell&t=blog)](https://so.csdn.net/so/search/s.do?q=ubuntu&t=blog)](https://so.csdn.net/so/search/s.do?q=linux&t=blog)
个人分类：[linux](https://blog.csdn.net/happyhorizion/article/category/3079971)
                
在linux下用vi或vim打开Test.java文件时
[root@localhost tmp]# vi Test.java
出现了如下信息：
E325: ATTENTION  
  Found a swap file by the name ".Test.java.swp"  
       owned by: root   dated: Wed Dec  7 13:52:56 2011  
       file name: /var/tmp/Test.java  
       modified: YES  
       user name: root   host name: localhost  
       process ID: 26153 (still running)  
  While opening file "Test.java"  
         dated: Wed Dec  7 14:00:46 2011  
      NEWER than swap file!  
  (1) Another program may be editing the same file.  
    If this is the case, be careful not to end up with two  
    different instances of the same file when making changes.  
    Quit, or continue with caution.  
  (2) An edit session for this file crashed.  
    If this is the case, use ":recover" or "vim -r Test.java"  
    to recover the changes (see ":help recovery").  
    If you did this already, delete the swap file ".Test.java.swp"  
    to avoid this message.  
  "Test.java" 11L, 237C  
  Press ENTER or type command to continue
原因是在此次vi或vim操作前有过一次使用vi或vim 操作Test.java文件时出现了异常中断，所以在当前目录下产生了一个.Test.java.swp文件，这个文件使用ls命令查看不能发现，使用ls -a命令查看可以知道Test.java.swp是一个隐藏文件。
注：以.开头的文件就是隐藏文件
想要不再有上面的警告提示，把该文件删除即可：
[root@localhost tmp]# rm .Test.java.swp
rm: remove regular file `.Test.java.swp'? y
[root@localhost tmp]#
使用vi或vim编辑其他文件出现类似情况的处理方法一样可以。
            
