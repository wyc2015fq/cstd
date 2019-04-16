# 在Servlet使用getServletContext()获取ServletContext对象出现java.lang.NullPointerException(空指针)异常的解决办法 - 孤傲苍狼 - 博客园



# [孤傲苍狼](https://www.cnblogs.com/xdp-gacl/)

只为成功找方法，不为失败找借口！




## [在Servlet使用getServletContext()获取ServletContext对象出现java.lang.NullPointerException(空指针)异常的解决办法](https://www.cnblogs.com/xdp-gacl/p/4005791.html)



　　今天遇到了一个在servlet的service方法中获取ServletContext对象出现java.lang.NullPointerException(空指针)异常，代码如下：

```
1 //获取ServletContext对象
2 ServletContext servletContext = this.getServletContext();
```

　　这个问题很奇怪，也是第一次遇到，因为以前在servlet的doGet/doPost方法中要获取ServletContext对象时都是这样写的，也没有出现过java.lang.NullPointerException(空指针)异常，上网查了一下出现这个异常的原因：原来是我重写了init(ServletConfig)方法，但重写的init方法内部没有调用super.init(config);就是这导致了错误！父类的 init(ServletConfig)有处理获取ServletContext对象的引用，在doGet/doPost/service方法方法中才能够通过 getServletContext()方法获取到SeverletContext对象！**重写了Servlet的init方法后一定要记得调用父类的init方法，否则在service/doGet/doPost方法中使用getServletContext()方法获取ServletContext对象时就会出现java.lang.NullPointerException异常**


```
1 public void init(ServletConfig config) throws ServletException{
2     //重写了Servlet的init方法后一定要记得调用父类的init方法，否则在service/doGet/doPost方法中使用getServletContext()方法获取ServletContext对象时就会出现java.lang.NullPointerException异常
3 　　super.init(config);
4 }
```











