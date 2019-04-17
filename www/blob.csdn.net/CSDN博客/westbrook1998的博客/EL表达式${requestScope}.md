# EL表达式${requestScope} - westbrook1998的博客 - CSDN博客





2018年02月16日 17:51:20[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：235








```xml
<!--这里${requestScope}取不到值，需要改为${param}-->
    <li class="layui-nav-item"><a href="<%=basePath%>student/studentInfo?stuid=${param.stuid}">
```



