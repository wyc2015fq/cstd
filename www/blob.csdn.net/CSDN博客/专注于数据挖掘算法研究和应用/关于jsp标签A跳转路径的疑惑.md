# 关于jsp标签A跳转路径的疑惑 - 专注于数据挖掘算法研究和应用 - CSDN博客





2016年02月18日 09:16:40[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：5234








在jsp内通过A链接到另一个jsp，如下代码，无法找到test.jsp资源，其实是都一起放在根目录下。

<a href="/test.jsp?id=<%=id%>"><%=id%></a>


后来加上request.getContextPath()便能跳转到test.jsp页面

<a href="<%=request.getContextPath()%>/test.jsp?id=<%=id%>"><%=id%></a>





很大的疑惑就是有些情况并不需要加根路径，默认就是在根路径寻找资源，那这中间的原因是和具体版本还有运行环境有关系吗？尚未结论。



