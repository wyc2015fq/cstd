# Java 学习笔记23：关于Java Session使用 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2012年12月14日 11:19:51[initphp](https://me.csdn.net/initphp)阅读数：8729
个人分类：[Java学习笔记系列](https://blog.csdn.net/initphp/article/category/1230072)

所属专栏：[Java学习笔记](https://blog.csdn.net/column/details/javago.html)








- 关于SESSION


度娘说“Session:在[计算机](http://baike.baidu.com/view/3314.htm)中，尤其是在网络应用中，称为“会话”。
- 当一个session第一次被启用时，一个唯一的标识被[存储](http://baike.baidu.com/view/87682.htm)于本地的cookie中。
- 通过这个cookie值，就可以对应存储在服务器端的这个session信息。
- 在访问者从到达某个特定的主页到离开为止的那段时间，每个访问者都会单独获得一个Session。




- Java中的Session使用。


Servlet定义了一个HttpSession接口，实现的Session的功能，在Servlet中使用Session的过程如下：

- 使用HttpServletRequest的getSession方法得到当前存在的session，如果当前没有定义session，则创建一个新的session，还可以使用方法getSession（true）
- 写session变量。可以使用方法HttpSession.setAttribute（name，value）来向Session中存储一个信息。
- 读Session变量。可以使用方法HttpSession.getAttribute（name）来读取Session中的一个变量值，如果name是一个没有定义的变量，那么返回的是null。需要注意的是，从getAttribute读出的变量类型是Object，必须使用强制类型转换。




- 使用实例：

- session对象（在Spring框架中，session,request,response都是非单例模式，这样就可以保证线程安全）：

```java
@Autowired
    private HttpSession httpSession;
```


- 设置和获取Session值：

```java
httpSession.setAttribute("username", "zhuli");
	    String str = (String) httpSession.getAttribute("usernames");
	    
	    if (str == null) {
	        System.out.print("error");
	    } else {
	        if ("zhuli".equals(str)) {
	            System.out.print("YES");
	        }
	    }
```

- 客户端的cookie：
![](https://img-my.csdn.net/uploads/201212/14/1355456285_7425.jpg)





