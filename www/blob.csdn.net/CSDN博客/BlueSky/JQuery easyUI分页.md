# JQuery easyUI分页 - BlueSky - CSDN博客
2016年01月14日 19:37:14[ASIA_kobe](https://me.csdn.net/ASIA_kobe)阅读数：315
个人分类：[Java Spring](https://blog.csdn.net/ASIA_kobe/article/category/6039238)
下面记录下项目中关于分页的部分内容，
![](https://img-blog.csdn.net/20160114194111794?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![](https://img-blog.csdn.net/20160114194135310?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![](https://img-blog.csdn.net/20160114194150341?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
以上为关于分页插件的代码的总体需要设置的地方还有一个地方需要注意的是Dao层中对应方法以“Page”结尾，因为过滤器中规则是对以Page结尾的sql进行分页操作。
相关的内容还包括JS文件、sql语句和Dao方法对应。
