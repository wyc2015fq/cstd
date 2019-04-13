
# 跨域攻击XSS防御 - 无界 - CSDN博客

2016年09月26日 15:13:22[21aspnet](https://me.csdn.net/21aspnet)阅读数：3451


Java的view层可以使用EL和JSTL
后端的ModelAndView增加
```python
mv.addObject("xss", "<script>alert(\"test\")</script>");
```
View页面
```python
${xss}
<c:out value="${xss}" escapeXml="true"></c:out>
<c:out value="${xss}"></c:out>
```
c:out 有个缺省属性escapeXML="true" 将会对特色字符如 ‘<‘ ‘>‘ ‘&‘ 等进行转义，而EL表达式则不会。
![](https://img-blog.csdn.net/20160926151218530)
![](https://img-blog.csdn.net/20160926151222365)

