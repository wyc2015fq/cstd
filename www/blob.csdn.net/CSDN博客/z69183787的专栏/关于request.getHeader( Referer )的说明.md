# 关于request.getHeader("Referer")的说明 - z69183787的专栏 - CSDN博客
2013年11月13日 16:59:25[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2390
request.getHeader("Referer")获取来访者地址。只有通过**链接**访问当前页的时候，才能获取上一页的地址；
否则request.getHeader("Referer")的值为Null，通过window.open打开当前页或者直接输入地址，也为Null。
若从 http://XXXX/A.JSP 上 点击 a标签 至 http://XXXX/b.jsp
b.jsp 获取 request.getHeader("Referer")
 返回  http://XXXX/A.JSP
