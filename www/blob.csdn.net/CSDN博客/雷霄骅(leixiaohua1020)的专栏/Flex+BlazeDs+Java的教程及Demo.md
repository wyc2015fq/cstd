# Flex+BlazeDs+Java的教程及Demo - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2013年10月06日 20:05:00[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：28
个人分类：[Flash相关](https://blog.csdn.net/leixiaohua1020/article/category/1653525)










这个基础教程是Flex4.1，Blazeds4.0为基础写的。

第一步：新建Web Project，名字为：FlexBlazeDsDemo，点Finish



![](http://dl.iteye.com/upload/attachment/578711/25464d78-8f12-30f8-983e-240684915d1d.png)





第二步：去Adobe公司的网站上下载Blazeds的包，



![](http://dl.iteye.com/upload/attachment/578713/d54d23b6-5e8a-3aa3-9974-26a9c745f803.png)

解压blazeds.war，看到blaeds的文件夹，里面有一个lib的文件夹，将包引入到项目工作中去

同时将blazeds->WEB-INF下的flex文件夹Copy到项目中的WEB-INF下，目录结构如下：



![](http://dl.iteye.com/upload/attachment/578715/0c9a56a3-4cf0-387d-ab75-b45a45cef59a.png)



第三步：在Tomcat中发布项目

第四步：在Web Project中添加Flex项目

Step1：

![](http://dl.iteye.com/upload/attachment/578717/52c9e8d6-9059-3631-b3dd-654343c1aea9.png)

Step2：服务器技术选择J2EE及Blazeds



![](http://dl.iteye.com/upload/attachment/578723/e10bb17a-53a8-3a06-9ab9-bd26ba606415.png)



Step3：



![](http://dl.iteye.com/upload/attachment/578719/a7699e1e-95dd-34bc-bdf3-d4b77554db44.png)



Step4：配置上述图中的信息，根文件夹，选择FlexBlazeDsDemo发布之后的位置，配置之后的信息如下：



![](http://dl.iteye.com/upload/attachment/578721/f01ee433-75b8-3f88-9919-ebc2f91d1d6c.png)



Step5：新建html-template（打开porblem窗口，如果有提示这个错误就新建）和flex-src文件夹，目录结构如下：建立完文件，会提示：DescriptionResourcePathLocationType

missing ; before statementindex.template.htm，这个错误不用处理，接着往下走。





![](http://dl.iteye.com/upload/attachment/578727/c1f8741e-7ed5-3650-9cdb-a37c34b20cfe.png)

Step6：调整目录结构及参数配置，如下图

![](http://dl.iteye.com/upload/attachment/578725/03805779-fd2d-3d6f-862d-46d9bb4bf250.png)



在上图中，将主源文件夹修改成：flex-src

**特别注意：将FlexBlazeDsDemo.mxml文件移动到flex-src目录下，因为我们已经将主源文件夹修改，并且以后所建的Flex文件也都要放到flex-src下。**

Step8：服务参数，也就是Flex编译器

![](http://dl.iteye.com/upload/attachment/578729/80528c9f-4303-33d8-8079-76585cdfeb14.png)

我们也可以打开配置文件去查看参数的（.actionScriptProperties及.flexProperties），如下图：



![](http://dl.iteye.com/upload/attachment/578731/d44b3c7b-2005-3b46-b6be-4ae15c85cf2e.png)



第五步：在FelxBlazeDsDemo输入以下几行代码

![](http://dl.iteye.com/upload/attachment/578733/6e9908b0-4018-329b-865f-fd33e8316d9c.png)



第六步：在浏览器地址栏输入[http://localhost:8080/FlexBlazeDsDemo/FlexBlazeDsDemo.html](http://localhost:8080/FlexBlazeDsDemo/FlexBlazeDsDemo.html)，查看效果，端口号和项目名称改成自己即可。效果如下：

![](http://dl.iteye.com/upload/attachment/578736/6375ef80-95b6-333d-9768-65a9f6f06873.png)



第七步：添加BlazeDs，配置web.xml文件，将我们下载的BlazeDs包下在的web.xml文件覆盖我们项目的文件，web.xml文件内容如下：



![](http://dl.iteye.com/upload/attachment/578739/9f6b5ce0-b0d9-3399-8b5a-941a6f7f6388.png)



第八步：写一个Java类，HelloWorld，代码如下：



![](http://dl.iteye.com/upload/attachment/578738/3a0bca0c-f308-3815-8e3b-df1a9b7bffbc.png)

第九步：配置remote-0bject.xml文件



![](http://dl.iteye.com/upload/attachment/578742/cec6df6b-b8e8-3f63-96cb-d3378088ef0e.png)

第十步：更改FlexBlazeDsDemo代码，内容如下：destinatioin一定要与上一步中的remote-object.xml的destination一样，reusult是调用成功之后处理函数。



![](http://dl.iteye.com/upload/attachment/578744/67620564-8fac-30c6-86aa-0fc033958704.png)

第十一步：测试

![](http://dl.iteye.com/upload/attachment/578748/fc14d6cb-5dbb-34f4-bf7c-184825d048d4.png)

![](http://dl.iteye.com/upload/attachment/578746/1c8e4e99-9ffd-35ff-92f0-908dad3313da.png)



原文地址：[http://wangdongsong.iteye.com/blog/1222435](http://wangdongsong.iteye.com/blog/1222435)




