# Struts2中包的详细介绍 - z69183787的专栏 - CSDN博客
2013年01月31日 10:22:18[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1117
个人分类：[Struts2](https://blog.csdn.net/z69183787/article/category/2175433)

<package name="miyao" namespace="/test" extends="struts-default">
<action name="hello" class="com.miyao.HelloWorldAction" method="execute">
<result name="success">/WEB-INF/page/hello.jsp</result>
</action>
</package>
在strut2框架中使用包来管理Action,这里包的作用和java中的类包作用是非常相似的.它主要用于管理一组业务功能相关的action.在实际应用中,我们应该把一组业务功能相关的Action放在一个包下.
配置包中必须指定name属性,它的名字可以随便取,但是必须是能唯一标识这个包的,他不对应java的类包,如果其它包想要继承它,必须通过该属性进行引用.包的namespace用于指定它的命名空间,其也作为访问包下Action路径的一部分,可以不指定它.
通常每个包都应该继承struts-default包,因为struts2很多核心的功能都是拦截器实现的.如:从请求中把请求参数封装到Action中/文件上传/数据验证等都是通过拦截器实现的.struts-default定义了这些拦截器和Result类型.可以这样说,只有当包继承了struts-default时,才能使用strut2中提供的核心功能.struts-default包是在struts2-core.2.x.x.jar文件中的struts-default.xml中定义.struts-default.xml也是struts2的默认配置文件.当struts2每次启动的时候,都会自动加载struts-default.xml配置文件.
包还可以通过abstract="true"定义抽象包,抽象包中不能包含action.
