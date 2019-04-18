# struts2标签之 - z69183787的专栏 - CSDN博客
2014年06月26日 21:25:22[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1583
**1.s:fielderror输出指定错误信息，而不是输出全部错误信息**
在struts2框架中，例如在完成数据类型转换、数据校验时如果出现错误信息，我们会利用ActionSupport类中addFieldError方法来添加错误信息内容（如：在Action中使用this.addFieldError("username" , "用户名已存在!");
）。在JSP页面中利用<s:fielderror/>标签在相应的字段处输出错误信息。（如：JSP页面：<s:textfield name="username" label="用户名"/>，在该处后面使用<s:fielderror/>标签， 那么上面的错误信息也就会显示到username这个输入框的旁边。）
但是，在实际开发中，<s:fielderror/>它会输出全部的错误信息内容。而如果想选择性地输出指定错误信息。我们可以使用如下代码解决：
第一种方法：
- <s:fielderror>  
-   <s:param>username</s:param> <!--显示指定的 username字段的 错误消息-->  
-   <s:param>username2</s:param> <!--显示指定的 username2字段的 错误消息-->  
- <s:fielderror/>  
第二种方法：
- <s:fielderror fieldName="username"/> <!--显示指定的 username字段的 错误消息-->  
- <s:fielderror fieldName="username2"/> <!--显示指定的 username2字段的 错误消息-->  
