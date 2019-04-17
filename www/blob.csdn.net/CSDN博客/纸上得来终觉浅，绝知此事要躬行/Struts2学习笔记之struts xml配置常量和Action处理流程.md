# Struts2学习笔记之struts.xml配置常量和Action处理流程 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2012年07月19日 23:45:09[boonya](https://me.csdn.net/boonya)阅读数：3235标签：[struts																[action																[spring																[设计模式																[浏览器](https://so.csdn.net/so/search/s.do?q=浏览器&t=blog)
个人分类：[Struts2](https://blog.csdn.net/boonya/article/category/879253)





    Struts2学习笔记

第五记：struts.xml配置常量和Action处理流程
**（1）、常量设置**

        1-1：修改ation的后缀

              将.action后缀改为.do:

              <constant name="Struts.action.extension"  value="do"/>

              可以修改访问多种后缀： 

              如 <constant name="Struts.action.extension" value="do,html,jsp"/>

       1-2：设置字符编码

              <constant name="Struts.i18n.encoding"  value="UTF-8"/>

             注：这个请求对象是HttpServletRequest，调用的是它的setCharacterEncoding方法。

       1-3：解决浏览器缓存

               <constantname="struts.serve.static.browserCache " value="true" />设置浏览器是否缓存静态内容，默认值为 true。

       1-4：修改配置文件后，系统自动加载该文件

              不重新启动服务器：

             <constant name="Struts.configuration.xml.reload"  value="true"/>,默认值为false。

       1-5：打印出更为详细的错误信息

             <constant name="Struts.devMode"  value="true"/>,默认值为false。

       1-6：设置默认主题

              <constant   name="struts.ui.theme" value="simple"/>

       1-7：与spring集成时，指定由spring负责action对象的创建

              <constant   name="struts.objectFactory" value="spring"/>

       1-8：是否允许动态方法调用

             <constant   name="struts.enable.DynamicMethodInvocation"value="false"/>,默认为true。

        1-9：上传文件的大小限制（多个文件的总大小）

             <constant   name="struts.multipart.maxSize" value="100000"/>

**（2）、Action的处理流程**

        如图所示：

![](https://img-my.csdn.net/uploads/201207/19/1342713192_4039.jpg)


注：Action访问路径必须符合Struts2的路径检索规则才能转发或重定向到该视图。

（3）、Struts2和Struts1Action管理的区别

     struts1：只创建一个Action，并将之放入缓存，待到要用的时候再去取，遵循单例模式。

     struts2：为每一个请求都创建一个Action，保证了线程的安全，符合原型设计模式。](https://so.csdn.net/so/search/s.do?q=设计模式&t=blog)](https://so.csdn.net/so/search/s.do?q=spring&t=blog)](https://so.csdn.net/so/search/s.do?q=action&t=blog)](https://so.csdn.net/so/search/s.do?q=struts&t=blog)




