# Intellij Idea导入非maven Web项目时遇到lib目录冲突的问题 - 零度的博客专栏 - CSDN博客
2016年08月08日 14:31:01[零度anngle](https://me.csdn.net/zmx729618)阅读数：888
                
Maven项目的目录结构为：
project
    Webroot
        WEB-INF
            classes
             lib
在idea中 会默认将webroot目录下的所有目录均copy至 war包中，若希望只使用lib目录下的部分jar包，
需做如下设置，
![](https://img-blog.csdn.net/20141123213026340?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvejY5MTgzNzg3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
将lib目录 exclude在 content root下。
在library菜单栏下设置需要的lib jar包。
