# Oracle密码过期the password has expired - z69183787的专栏 - CSDN博客
2014年12月13日 22:13:01[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1518
调试Web项目的时候出现异常：
**[java]**[view plain](http://blog.csdn.net/android_robot/article/details/7598345#)[copy](http://blog.csdn.net/android_robot/article/details/7598345#)[print](http://blog.csdn.net/android_robot/article/details/7598345#)[?](http://blog.csdn.net/android_robot/article/details/7598345#)
- java.sql.SQLException: ORA-28001: the password has expired  
网上查了一下，是Oracle11g密码过期的原因
连接Oracle，以Oracle用户登陆，输入以下命令
**[sql]**[view plain](http://blog.csdn.net/android_robot/article/details/7598345#)[copy](http://blog.csdn.net/android_robot/article/details/7598345#)[print](http://blog.csdn.net/android_robot/article/details/7598345#)[?](http://blog.csdn.net/android_robot/article/details/7598345#)
- select * from dba_profiles where profile='DEFAULT'and resource_name='PASSWORD_LIFE_TIME';  
结果显示：
**[html]**[view plain](http://blog.csdn.net/android_robot/article/details/7598345#)[copy](http://blog.csdn.net/android_robot/article/details/7598345#)[print](http://blog.csdn.net/android_robot/article/details/7598345#)[?](http://blog.csdn.net/android_robot/article/details/7598345#)
- PROFILE  
- ------------------------------------------------------------  
- RESOURCE_NAME  
- ------------------------------------------------------------  
- RESOURCE_TYPE  
- ----------------  
- LIMIT  
- ------------------------------------------------------------  
- 
- DEFAULT  
- PASSWORD_LIFE_TIME  
- PASSWORD  
- 180  
- 
- ------------------------------------------------------------  
结果显示密码有效期是180天
输入命令：
**[sql]**[view plain](http://blog.csdn.net/android_robot/article/details/7598345#)[copy](http://blog.csdn.net/android_robot/article/details/7598345#)[print](http://blog.csdn.net/android_robot/article/details/7598345#)[?](http://blog.csdn.net/android_robot/article/details/7598345#)
- alter profile default  limit password_life_time unlimited;  
- commit;  
再次执行：
**[sql]**[view plain](http://blog.csdn.net/android_robot/article/details/7598345#)[copy](http://blog.csdn.net/android_robot/article/details/7598345#)[print](http://blog.csdn.net/android_robot/article/details/7598345#)[?](http://blog.csdn.net/android_robot/article/details/7598345#)
- select * from dba_profiles where profile='DEFAULT'and resource_name='PASSWORD_LIFE_TIME';  
结果显示为：
**[sql]**[view plain](http://blog.csdn.net/android_robot/article/details/7598345#)[copy](http://blog.csdn.net/android_robot/article/details/7598345#)[print](http://blog.csdn.net/android_robot/article/details/7598345#)[?](http://blog.csdn.net/android_robot/article/details/7598345#)
- PROFILE  
- ------------------------------------------------------------
- RESOURCE_NAME  
- ------------------------------------------------------------
- RESOURCE_TYPE  
- ----------------
- LIMIT  
- ------------------------------------------------------------
- 
- DEFAULT
- PASSWORD_LIFE_TIME  
- PASSWORD
- UNLIMITED  
- ------------------------------------------------------------
进行以上步骤之后需要改变密码，否则还会出现password has expired异常
改变密码的命令
**[sql]**[view plain](http://blog.csdn.net/android_robot/article/details/7598345#)[copy](http://blog.csdn.net/android_robot/article/details/7598345#)[print](http://blog.csdn.net/android_robot/article/details/7598345#)[?](http://blog.csdn.net/android_robot/article/details/7598345#)
- alteruser XXXUSER identified by Welcome1;  
如果账号被锁住，则需要解锁命令
**[sql]**[view plain](http://blog.csdn.net/android_robot/article/details/7598345#)[copy](http://blog.csdn.net/android_robot/article/details/7598345#)[print](http://blog.csdn.net/android_robot/article/details/7598345#)[?](http://blog.csdn.net/android_robot/article/details/7598345#)
- alteruser XXXUSER identified by oracle account unlock;  
再次调试，问题解决
