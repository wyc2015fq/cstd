# Struts2学习笔记之国际化 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2012年09月06日 22:23:22[boonya](https://me.csdn.net/boonya)阅读数：897标签：[struts																[action																[properties																[jsp																[语言](https://so.csdn.net/so/search/s.do?q=语言&t=blog)
个人分类：[Struts2](https://blog.csdn.net/boonya/article/category/879253)





Struts2学习笔记

第十二记：Struts2国际化

国际化信息作用域包括：全局范围、包范围和Action范围
**1、全局范围的国际化信息**

1-1、编写国际化文件

       新建web项目，在src目录目录下添加properties文件

       如：book_zh_CN.properties和book_en_US.properties

      这两个文件分别表示实现中国大陆和美国英语语言的国际化。

      在properties文件中，信息以key-value的方式编写。

      如：sex=男 from book_zh_CN.properties

       而在book_en_US.properties中sex=male

1-2、配置国际化文件

       在struts.xml文件中配置全局变量，代码如下：

      <constant name="struts.custom.i18n.resources" value="book">

1-3、获取和输出全局国际化信息

   1-3-1、action读取：

       编写Action类继承自ActionSupport,重载execute方法

      在里面获取国际化信息：

       ActionContext.getContext().put("message",this.getText("sex"));

      JSP页面EL表单式输出：${message} #男

       改变计算机系统的语言为英语时显示：male

   1-3-2、struts标签读取

      导入struts标签库：

     <%@ taglib uri="struts-tags" prefix="s" %>

      JSP页面显示：

      <s:text name="book">;

      刷新页面即可看到"male"或者是"男"
**2、输出含占位符国际化信息**

  2-1、分别修改国际化属性配置文件:

     sex={0}男{1}和sex={0}male{1}

  2-2、在JSP页面访问：

     <s:text name="book">

           <s:param>性别：</s:param>

           <s:param>yes</s:param>

     </s:text>

  2-3、Action访问方式：

       ActionContext.getContext().put("message",this.getText("sex",new String[]("性别：","yes")));
**3、包范围的国际化**

    属性文件命名规则：package_language_country.properties

    那么上面的两个文件放在某个包下时使用的名称为:

    package_zh_CN.properties和package_en_US.properties

    注意：包范围的会优先访问，如果找不到再到全局范围去找（小范围到大范围），占位符输出同上。
**4、Action范围的国际化**

     属性文件命名规则：ActionClassName_language_country.properties

     文件路径：在Action类所在的包下面。

     那么上面的两个文件放在某个包下时使用的名称为:

    PersonAction_zh_CN.properties和PersonAction_en_US.properties

    注意：首先访问Action范围，其次访问包范围，如果找不到再到全局范围去找（小范围到大范围），占位符输出同上。
**5、直接访问国际化资源文件**

    5-1、全局范围

    <s:i18n name="book">

         <s:text name="sex"/>

   </s:i18n>

   5-2、包范围

     <s:i18n name="com/struts/action/package">

          <s:param>性别：</s:param>

         <s:param>yes</s:param>

       </s:i18n>

   5-3、Action范围

    <s:i18n name="com/struts/action/PersonAction">

         <s:param>性别：</s:param>

         <s:param>yes</s:param>

    </s:i18n>

      注：这种方式不用做任何配置。](https://so.csdn.net/so/search/s.do?q=jsp&t=blog)](https://so.csdn.net/so/search/s.do?q=properties&t=blog)](https://so.csdn.net/so/search/s.do?q=action&t=blog)](https://so.csdn.net/so/search/s.do?q=struts&t=blog)




