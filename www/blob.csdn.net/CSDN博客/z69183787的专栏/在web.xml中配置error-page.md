# 在web.xml中配置error-page - z69183787的专栏 - CSDN博客
2013年11月13日 13:39:17[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2866
  知道在web.xml中有两种配置error-page的方法，一是通过错误码来配置，而是通过异常的类型来配置，分别举例如下： 
一．   通过错误码来配置error-page 
Eg. 
Xml代码 
<error-page>   
        <error-code>500</error-code>   
        <location>/error.jsp</location>   
  </error-page>  
上面配置了当系统发生 
500错误（即服务器内部错误）时，跳转到错误处理页面error.jsp。 
二．   通过异常的类型配置error-page 
Eg. 
Xml代码 
<error-page>   
        <exception-type>java.lang.NullException</exception-type>   
        <location>/error.jsp</location>   
</error-page>  
上面配置了当系统发生 
java.lang.NullException（即空指针异常）时，跳转到错误处理页面error.jsp 
<error-code>和 <exception-type>二选一，不能在同一个<error-page>上同时出现
