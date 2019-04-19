# web.xml文件中配置 - 零度的博客专栏 - CSDN博客
2016年05月13日 15:37:45[零度anngle](https://me.csdn.net/zmx729618)阅读数：6768
       项目中发现使用IE9下载文件（doc，docx等）时，浏览器有时直接打开文件显示乱码有时可以下载但下载后是一文件夹是里面一堆以xml文件（微软的office存储文件信息主要以xml文件存储），后来跟踪代码发现后台默认是以text形式输出文件，然后打开web.xml文件发现
**[html]**[view
 plain](http://blog.csdn.net/sccemstanford/article/details/9064155#)[copy](http://blog.csdn.net/sccemstanford/article/details/9064155#)
- <mime-mapping>
- <extension>doc</extension>
- <mime-type>application/zip</mime-type>
- </mime-mapping>
        原来当我下载doc文件时，默认以ZIP文档的形式下载，难怪有时下载后的文件为一堆XML文件，于是重新配置了一些常用的文档下载MIME格式，并查看了<mime-mapping>的作用--[mime-mapping元素将mime类型映射到扩展名, 
 用于规定下载格式](http://baike.baidu.com/view/1002818.htm)，在此记录：
**[html]**[view
 plain](http://blog.csdn.net/sccemstanford/article/details/9064155#)[copy](http://blog.csdn.net/sccemstanford/article/details/9064155#)
- <!-- 为下载的文件自动装配相应的mime后缀 -->
- <mime-mapping>
- <extension>doc</extension>
- <mime-type>application/vnd.ms-word</mime-type>
- </mime-mapping>
- 
- <mime-mapping>
- <extension>xls</extension>
- <mime-type>application/msexcel</mime-type>
- </mime-mapping>
- 
- <mime-mapping>
- <extension>ppt</extension>
- <mime-type>application/vnd.ms-powerpoint</mime-type>
- </mime-mapping>
- 
- <mime-mapping>
- <extension>docx</extension>
- <mime-type>application/vnd.openxmlformats-officedocument.wordprocessingml.document</mime-type>
- </mime-mapping>
- 
- <mime-mapping>
- <extension>pptx</extension>
- <mime-type>application/vnd.openxmlformats-officedocument.presentationml.presentation</mime-type>
- </mime-mapping>
- 
- <mime-mapping>
- <extension>xlsx</extension>
- <mime-type>application/vnd.openxmlformats-officedocument.spreadsheetml.sheet</mime-type>
- </mime-mapping>
