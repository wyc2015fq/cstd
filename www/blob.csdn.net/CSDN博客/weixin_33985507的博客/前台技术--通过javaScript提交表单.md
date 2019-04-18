# 前台技术--通过javaScript提交表单 - weixin_33985507的博客 - CSDN博客
2015年12月31日 10:31:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：2

```java
window.location=pp+"?username="+getCookie("username")+"&userid="+getCookie("userid");
```
替换
```java
document.write("<form action="+pp+" method=post name=formx1 style='display:none'>");
document.write("<input type=hidden name=username value='"+getCookie("username")+"' />");
document.write("<input type=hidden name=userid value='"+getCookie("userid")+"' />");
document.write("</form>");
document.formx1.submit();
```
