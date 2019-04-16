# JSP取得绝对路径 - 孤傲苍狼 - 博客园



# [孤傲苍狼](https://www.cnblogs.com/xdp-gacl/)

只为成功找方法，不为失败找借口！




## [JSP取得绝对路径](https://www.cnblogs.com/xdp-gacl/p/3707243.html)



在JavaWeb开发中，常使用绝对路径的方式来引入JavaScript和CSS文件，这样可以避免因为目录变动导致引入文件找不到的情况，常用的做法如下：

一、使用${pageContext.request.contextPath}

　　代码” ${pageContext.request.contextPath}”的作用是取出部署的应用程序名，这样不管如何部署，所用路径都是正确的。

例如：

```
1 <!--使用绝对路径的方式引入CSS文件-->
2 <link rel="stylesheet" href="${pageContext.request.contextPath}/themes/default/css/ueditor.css" type="text/css"/>
3 <!--使用绝对路径的方式引入JavaScript脚本-->
4 <script type="text/javascript" src="${pageContext.request.contextPath}/ueditor1_3_6-gbk-jsp/ueditor.config.js"></script>
```

使用<%=request.getContextPath()%>和使用${pageContext.request.contextPath}达到同样的效果

```
1 <script type="text/javascript" src="<%=request.getContextPath()%>/ueditor1_3_6-gbk-jsp/ueditor.all.js"></script>
```











