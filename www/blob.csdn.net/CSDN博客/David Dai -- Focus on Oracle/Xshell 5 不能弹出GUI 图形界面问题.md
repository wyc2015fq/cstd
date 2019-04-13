
# Xshell 5 不能弹出GUI 图形界面问题 - David Dai -- Focus on Oracle - CSDN博客


2015年03月24日 16:59:25[Dave](https://me.csdn.net/tianlesoftware)阅读数：19308




刚从Xshell 4 换成了Xshell5，界面是高大上了很多，结果执行asmca的时候，不能弹出GUI界面。

开始按照Xshell 4的习惯，直接su切换到用户，然后执行，如下：
[root@rac1 ~]\# su - grid
[grid@rac1 ~]$ asmca
[grid@rac1 ~]$ asmca
[grid@rac1 ~]$

没有结果， 不得已，重新开了一个session，直接用grid用户登陆，尝试，ok了，随笔小记。
![](https://img-blog.csdn.net/20150324165842499?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdGlhbmxlc29mdHdhcmU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



--------------------------------------------------------------------------------------------
版权所有，文章禁止转载，否则追究法律责任!
**AboutDave:**
--------------------------------------------------------------------------------------------
QQ:492913789
Email:ahdba@qq.com
Blog:http://www.cndba.cn/dave
Weibo:[http://weibo.com/tianlesoftware](http://weibo.com/tianlesoftware)
Twitter:[http://twitter.com/tianlesoftware](http://twitter.com/tianlesoftware)
Facebook:[http://www.facebook.com/tianlesoftware](http://www.facebook.com/tianlesoftware)
Linkedin:[http://cn.linkedin.com/in/tianlesoftware](http://cn.linkedin.com/in/tianlesoftware)
**Dave 的QQ群：**
--------------------------------------------------------------------------------------------
注意:加群必须注明表空间和数据文件关系 | 不要重复加群
CNDBA_1: 62697850 (空)    CNDBA_2: 62697716 (满)  CNDBA_3: 283816689
CNDBA_4: 391125754   CNDBA_5:104207940    CNDBA_6: 62697977   CNDBA_7: 142216823（满）

