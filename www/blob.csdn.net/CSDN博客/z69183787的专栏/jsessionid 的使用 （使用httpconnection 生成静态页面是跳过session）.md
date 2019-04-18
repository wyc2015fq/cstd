# jsessionid 的使用 （使用httpconnection 生成静态页面是跳过session） - z69183787的专栏 - CSDN博客
2013年12月18日 20:03:40[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2245
## 在url地址后 增加http://xxxx.action;jsessionid= session.getId() ?a=1&b=2后使用 httpconnect 获取字节流并 生成静态页面保存在本地时，即可越过过滤器与session 获得实际内容，而不会被过滤器拦截。
## 示例：[点击打开链接](http://blog.csdn.net/z69183787/article/details/8124841)
