# 面试的角度诠释Java工程师（一） - Spark高级玩法 - CSDN博客
2018年04月17日 00:00:00[Spark高级玩法](https://me.csdn.net/rlnLo2pNEfx9c)阅读数：37
前言：
我相信每一个程序员都是为了生活而努力着的。很多人因为兴趣，从此踏上了这条‘烧脑大行动’的金桥；也有很多人因为梦想和执着，奋不顾身融入这个职业；还有很多人因为被现实逼得太无奈，不得不为自己、为家人、为未来谋这么一条坎坷的路。
这里，我不去评价什么，再说我也没有资格去评价啊。言归正传，下面我写这篇文章的定位是各位在场的读者，如果你认为我接下来说的内容会跟书里说的差不多，那么请高抬贵脚移步它处，但是我相信你绝对不会这样觉得的。不知道作为程序员的你们，是否还在抱着面试或者笔试题复习呢？是否还在抱着各种面试书乱翻呢？
我只说一点，面试的目的不是让考官知道你怎么样，而是让你自己清楚自己怎么样。也就是说你会什么、能够做什么！举个例子，如果你去面试Java工程师，你至少要会该职位的职能，而不是只会面面试、做做笔试题。所以，这就是我写这篇文章的初衷！
# 一、基础篇
### **1.面向对象的三大特性**
**继承、封装、多态**
***什么是继承？***
①继承是面向对象程序设计能够提高软件开发效率的重要原因之一。
②继承是具有传递性的，就像现实中孙子不仅长得像爸爸而且还像他爷爷。
③继承来的属性和方法是隐式的，也就是在本类里面是看不见的。
④一个类只能有一个父类，也就是类只能是单继承。
⑤一个接口可以有多个父类，也就是接口可以是多继承。
实际项目开发中，一个类继承于另一个类，那么前者就是后者的子类，反则反之。
***什么是封装？***
对象数据和操作该对象的指令都是对象自身的一部分，能够实现尽可能对外部隐藏数据。
实际项目开发中，使用封装最多的就是实体类，常常和JavaBean（类必须是具体的和公共的，并且具有无参数的构造器）一起使用。
那么，实体类有那些东西呢？
答：私有的成员变量、无参数的构造器、有参数的构造器、setter和getters方法、重写tostring方法、重写hashCode和equals方法。
***什么是多态？***
①多态就是对象拥有多种形态：引用多态和方法多态。
②引用多态：父类的引用可以指向本类对象、父类的引用可以指向子类的对象。
③方法多态：创建本类对象时，调用的方法为本类的方法；创建子类对象时，调用的方法为子类重写的方法或者继承的方法。
④存在多态的必要条件：继承、重写。
⑤多态的作用是消除类型之间的耦合关系。
在实际项目开发中，A类继承B类，如果在A类中不重写B类的方法的时候，输出的仍旧是B类方法里面的信息（B b=new A()）；如果在A类中重写B类的方法的时候，输出的是A类方法里面的信息（B b=new A()）。
### **2.Java集合框架树**
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/QCu849YTaINjkI7R8dWDFFH6SPMIEEFHU0j8qQxRIv1x3wb5tnDOgCAjpibYXzFr3MufPKsz6T2F133ibLaiaVReg/640)
**Collection接口**：它是Java集合框架的一个根接口，也是List、Set和Queue接口的父接口。同时它定义了可用于操作List、Set和Queue的方法—增删改查。
**Map接口**：它提供了一种映射关系，其中的元素是以键值对（key-value）的形式存储的。Map接口有一个重要的实现类HashMap。
①在Map接口中的键值对是以Entry类型的对象实例形式存在。
②在Map接口中键值（Key值）是不可以重复的，value值却可以重复，也就是存在多对一的关系。
③在Map接口中提供了分别返回Key值的集合、value值的集合以及Entry集合的方法。
④Map支持泛型，同时Key值和value值都是可以为null的。
⑤在HashMap中的Entry对象是无序排序的，这点特性和List接口是相反的。
⑥在HashMap中有且只能有一个Key值为null的映射。（注：key值是不能重复的）
**List接口**：List接口有一个重要的实现类ArrayList（数组序列）。
①List是元素有序并且可以重复的集合。
②List可以精确的控制每个元素的插入位置，或删除某个元素的位置。
**Comparator接口**：临时的比较规则。如果某一个类要实现这个接口，那必然要实现它的Compare()方法。
**Comparable接口**：默认的比较规则。当实现了这个接口，则表示这个类的实例可以比较大小，可以进行自然排序。如果某一个类要实现这个接口，那必然要实现它的CompareTo()方法。
### **3.Java中的IO流**
**字节流**：
```
//一个字节一个字节的读写
FileInputStream in=new FileInputStream("源文件");
FileOutputStream out=new FileOutputStream("目标文件");
......
in.close();
out.close();
```
首先要记住，一旦使用了IO流最后一定要记得关闭，这是常识。
```
//利用缓冲区，高效的读取字节
BufferedInputStream in=new BufferedInputStream(new FileInputStream("源文件");
BufferedOutputStream out=new BufferedOutputStream(new FileOutputStream("目标文件");
......
in.close();
out.close();
```
字符流：
```
InputStreamReader isr=new InputStreamReader(new FileInputStream("源文件路径"),"设置编码");
OutputStreamWriter osw=new OutputStreamWriter(new FileOutputStream("目标文件路径"),"设置编码");
......
osw.close();
isr.close();
```
也可以这样写：
```
FileReader fr=new FileReader("源文件路径");
FileWriter fw=new FileWriter("目标文件路径");
......
fr.close();
fw.close();
```
```
//利用缓冲区，高效的读取字符
BufferedReader br=new BufferedReader(new FileReade("源文件路径");
PrintWriter pw=new PrintWriter("目标文件路径");
......
br.close();
pw.close();
```
序列化和反序列化：
```
//对象的序列化
ObjectOutputStream oos=new ObjectOutputStream(new FileOutputStream(file));
```
这里的file指的是`String file="文件在项目中的路径";`
``
```
//对象的反序列化
ObjectInputStream ois=new ObjectInputStream(new FileInputStream(file));
```
### **4.Java的Socket通信（多线程）**
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/QCu849YTaINjkI7R8dWDFFH6SPMIEEFHBvhlyk3CRr9RO6oe7wugHIqBZS6vTOUXCqGuEh1ibjnxmKjVyaD59fw/640)
思路：
①首先创建服务器端Socket，指定并侦听某一个端口，然后循环监听开始等待客户端的连接....
②创建客户端socket，指定服务器地址和端口，然后获取输出流，向服务器端发送请求，并关闭socket输出流。
③服务端接收到客户端的请求后，创建新线程并启动。
④创建线程处理类，执行线程操作，获取输入流，服务端读取客户端用户详情，关闭资源。
⑤执行线程操作，获取输出流，响应客户端请求，客户端接受到服务端的响应，关闭资源。
简单点讲，就相当于我跟你说话（客户端→服务端），你收到我说的话（服务端→线程处理类），大脑进行思考后（线程处理类），做出回答我的话（线程处理类→客户端）。
### **5.关系型数据库**
**数据库的三范式**：
①字段不可分。
②有主键，非主键字段依赖主键。
③非主键字段不能互相依赖。
**T-SQL**：
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/QCu849YTaINjkI7R8dWDFFH6SPMIEEFHCvf8Gr7yNxnVHHxr2TDrxpmKK6JqEfSCic2xYjmAW0he6N9yJhicEQxw/640)
在整个数据库中，查询操作占据增删改查的80%，而说到查询，T-SQL语句自然不能少。如图上示。
**增**：
①插入单行
`insert into <表名> (列名) values (列值)`
②将现有的表数据添加到一个已有表
`insert into <已有的新表> (列名) select <源表列名> from <源表名>`
③直接拿现有表数据创建一个新表并填充
`select <新建表列名> into <新建表名> from <源表名>`
**删**：
①删除满足条件的行
`delete from <表名> where <删除条件>`
②删除整个表
`truncate table <表名>`
注：删除表的所有行，但表的结构、列、约束、索引等不会被删除；不能用语有外建约束引用的表。
**改**：
①更新
`update <表名> set <列名=更新值> where <更新条件>`
**子查询**：
`SELECT * FROM t1 WHERE column1 = (SELECT column1 FROM t2);`
其中：
①SELECT * FROM t1 ...称为外查询。
②SELECT column1 FROM t2 称为子查询。
所以，我们就说子查询是嵌套在外查询内部的。而事实上它也有可能在子查询内部再嵌套一个或者多个子查询。这里要注意，子查询必须出现在圆括号之间哦。
# 二、Web篇
**JavaScript总结：**https://www.jianshu.com/p/cc479554df66
### **JSP：**
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/QCu849YTaINjkI7R8dWDFFH6SPMIEEFHY7XGcWGPicI02fTwghGZIQqXKq9lMb0R5FeRPdiafNEDCYicEORw3PXibg/640)
思路：当用户使用客户端浏览器向服务器发送一个请求访问index.jsp页面，当服务器接收到客户的请求之后，开始判断用户所访问的这个资源是否是第一次访问？
①如果是第一次请求，那么Tomcat的JSP引擎就把这个JSP文件转换成一个Servlet（一个Servlet从本质上而言也是一个Java类），既然是一个Java类就需要通过编译，生成相应的字节码文件，然后执行jspInit这个初始化方法。再生成JSP页面的字节码文件，转到③。
②直接访问所生成的JSP页面的字节码文件，转到③。
③最后解析执行这个Java类当中的jspService方法，这个方法就是专门用来处理用户的请求。
### **JSP九大内置对象：**
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/QCu849YTaINjkI7R8dWDFFH6SPMIEEFHTQPYEHdeqibg8PyEYgqhyrMYLtgIt238oYo339Ewf32XK4k6qicvfotA/640)
**out对象**：它是JspWriter类的实例，是向客户端输出内容常用的对象。
**request对象**：它表示客户端的一次请求，这个请求里面的信息是被封装在request对象中的。同时它也是HttpServletRequest类的实例，并且具有请求域，也就是在完成客户端的请求之前，该对象是一直有效的。
*常用方法*：
`request.setCharacterEncoding("UTF-8");//设置编码，解决中文乱码问题。`
```
request.setAttribute("","");//前者为需要设置的属性，后者为需要的值。
request.getAttribute();
```
`request.getRequestDispatcher("index.jsp").forward(request, response);//请求转发到index.jsp页面，该请求对象是一直有效的。`
**response对象**：它包含了响应客户请求的相关信息，它也是HttpServletResponse类的实例。和request一样，它具有页面的作用域，也就是在访问一个页面的时候，该页面的response对象只对这次访问有效，往后执行是不存在、无效的。
*常用方法*：
`response.setContentType("text/html","charset="utf-8");//设置响应的MIMI类型。`
`response.sendRedirect("index.jsp");//请求重定向，如果存在多次重定向，那么对象数据是不会保存下来的，即为null。`
**请求转发和请求重定向的区别分析：**
请求重定向：客户端行为，response.sendRedirect()，从本质上讲等同于两次请求，前一次的请求对象不会保存，地址栏的URL地址会改变。
请求转发：服务器行为，request.getRequestDispatcher().forward(req, resp);是一次请求，转发后请求对象会保存，地址栏的URL地址不会改变。
**session对象**：当我们第一次访问页面的时候，session对象就会被自动装载和创建。它是HttpSession类的实例对象。
***什么是会话？为什么要用session对象？***
会话：当一个客户端打开浏览器连接到服务器开始，到客户端关闭浏览器服务器结束这个过程，我们称为一个会话。
在实际情况下，客户在访问服务器的过程中，避免不了会在服务器的几个页面之间切换，这时候服务端有必要知道当前操作的客户是不是操作前几个页面的客户，也就是必须要清楚知道是否同一个人在进行操作。所以就引入了session对象这个概念。
**application对象**：它实现了用户间数据的共享，可以存放全局变量，它是ServletContext类的实例。它的生命周期开始于服务器的启动，终止于服务器的关闭。
注：
①在用户的前后连接或不同用户之间的连接中，可以对application对象的同一属性进行操作。
②在任何地方对application对象属性的操作，都将影响到其他用户对此的访问。
### **JSP的指令和动作：**
①三个编译指令：page、include、taglib。
②七个动作指令：jsp:forward、jsp:param、jsp:include、jsp:plugin、jsp:useBean、jsp:setProperty、jsp:getPProperty。
**JSP指令：**
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/QCu849YTaINjkI7R8dWDFFH6SPMIEEFHWh1XIjlVW6bqYRudPiaicNtpxCNJOqeo0xib97lt1QweLgxUasuwaic00Q/640)
**include指令和include动作指令区别**：
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/QCu849YTaINjkI7R8dWDFFH6SPMIEEFHYokpuUqDwTfh2DKwa6Wpw9vyNtNic8v68iaulcWlShiacYcaO5BWWxG7g/640)
### **Session的生命周期**
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/QCu849YTaINjkI7R8dWDFFH6SPMIEEFHJqpn6yicXtMqfEDCibV05jXzCdZibWbGH8uomricysbxEoicdib4rIarO4Sw/640)
Session生命周期的“活动”思路：
①某次会话当中通过超链接打开的新页面属于同一次会话。
②只有当前会话页面没有全部关闭，重新打开新的浏览器窗口访问同一项目资源时属于同一次会话。
③除非本次会话的所有页面都关闭后再重新访问某个Jsp或者Servlet将会创建新的会话。
注：
注意原有会话是还存在的，只是这个旧的SessionId仍然存在于服务端，只不过再也没有客户端会携带它然后交予服务端校验。
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/QCu849YTaINjkI7R8dWDFFH6SPMIEEFHDf1tcgKT6ibm7jkriaYDoMMHPohA99ZvLSjBRKicm9qUK6qA0uVzHf3rw/640)
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/QCu849YTaINjkI7R8dWDFFH6SPMIEEFHXD0FjSPmnFB2ibX416MyxbVTLjacd1RoT3o9GIbXIJqPicV7qBkPzzAA/640)
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/QCu849YTaINjkI7R8dWDFFH6SPMIEEFHlzY6L8KkUQ7OYddRJCw4ic27vZJiaXicSib0EsRCL0x0fuzfoy0OyfAqrA/640)
注：
①当Web容器启动的时候，过滤器就已经加载并启动了。
②实例化只会实例化一次，也就是说在Web容器中只会使用一次。
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/QCu849YTaINjkI7R8dWDFFH6SPMIEEFHjyVe9cYSII2rcAyajOItkk47iacIjicP6ovCL6bqwgiaS6ngqmqhwXp9g/640)
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/QCu849YTaINjkI7R8dWDFFH6SPMIEEFHbgl4cCKfEH8bvfEwR0fnvlJBq4ic9h0MsAiaY4cX7icoKTeqliciaFqyzmw/640)
思路:
①首先判断Servlet实例是否存在，如果存在则跳入第③步。
②初始化阶段，init初始化方法是在创建实例之后完成的。
③响应阶段，响应客户端的请求，调用service()方法。由service()方法根据提交方式选择性执行doGet()方法或者doPost()方法。
④终止阶段，调用Destroy()方法销毁，服务器关闭。
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/QCu849YTaINjkI7R8dWDFFH6SPMIEEFHmOcAcwcuWLpYqTjaSvOYrAl0k0qYb0RHl0ibJvdibtwJtkgiaELUJxrtQ/640)
### **Web监听器**
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/QCu849YTaINjkI7R8dWDFFH6SPMIEEFHPLFh7DleMzhg6FjSq0y7kzQ5tyqHFvnBmVGAP4NUmzjib9LMSIvjTSg/640)
Web监听器是有Servlet规范定义的一种特殊类，它可以监听客户端的请求和服务端的操作，同时它还可以监听，包括ServletContext、HttpSession、ServletRequest等对象。
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/QCu849YTaINjkI7R8dWDFFH6SPMIEEFHBZjQeenwfMkRwFORNcY5T458YibN6RWCobqcaKvLvPhez5wstC2xOEQ/640)
### **监听器的分类**
**HttpSession的事件监听器**
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/QCu849YTaINjkI7R8dWDFFH6SPMIEEFHUCvgiamWiaxEqRpkUxnsC9M46HZIicJd04fezm5c4wTWEYnbKfw8w1bUw/640)
**ServletContext的事件监听器**
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/QCu849YTaINjkI7R8dWDFFH6SPMIEEFH1ZxZqLGOVaopbIq2icFibU8RFOlbdQLScAGoialHvXl6zycXK3uydWvJA/640)
**ServletRequest的事件监听器**
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/QCu849YTaINjkI7R8dWDFFH6SPMIEEFH7HibRr3f3kiavNSVRcLV88w8EiaA5gmKiaj0Wuric8m6XomMkzQWRb0NDaw/640)
注：
这三个监听事件，分别实现对应的接口，也就是图示的1..N。既然实现了接口，即必然要实现其对应的方法，如图所示。主要用途也如图所示。
**绑定到HttpSession域中的对象状态的事件监听器**
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/QCu849YTaINjkI7R8dWDFFH6SPMIEEFHjzuUGRx3T34UCiadcp5cIFaf4HP4GwUVKBDnlFwqTbLU4HhPIXn1ldQ/640)
注：
为什么要用session钝化？
首先，正常情况下，我们的session是存放在服务器的session当中的，但是如果我们的用户很多的时候，仍然把session存放在服务器里，这时服务器的开销是很大的，会直接影响我们Web应用的使用。所以就需要使用到session钝化机制，它的本质就是将服务器未使用到的session对象暂时序列化到系统文件当中，当要使用的时候再反序列化到服务器内存当中。
### **Model1和Model2**
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/QCu849YTaINjkI7R8dWDFFH6SPMIEEFHM9tSRtNAsRLicMecCYXjq5QT9wVyzwS4TFvDw9ZVAlPsibNuCGgF1gaw/640)
注：
Model1是最早的能体现分层思想的一种开发模式，简单来说Model1就是一种Jsp+JavaBean的一种开发模式。
缺点：可维护性和可扩展性是比较差的。
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/QCu849YTaINjkI7R8dWDFFH6SPMIEEFHX2rib3cysDvnewvqEByvYPfPiaBLPJGl9lJErBzRrOCK9iaeJjTLjndtw/640)
思路：
①首先由Jsp页面向Servlet提交一个请求。
②然后在我们的控制层Servlet实例化一个模型层的对象或者调用模型层的一些功能。
③然后由模型层来访问读取我们的数据库层。
④当得到读取到结果后，再将结果返回到我们的控制层。
⑤控制层得到这个结果后，会根据这个结果给用户展示不同的Jsp页面。
- 
**原文：www.jianshu.com/p/e4ba72acffa9**
**推荐阅读：**
1，[Java枚举(enum)七种常见的用法](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484681&idx=1&sn=60939b9e8cca2904a4b237bca867166d&chksm=9f38e621a84f6f37816faaed85b67d536e4cceade7878267f9373f761dcf45eb1aa2317bb048&scene=21#wechat_redirect)
2，[谈谈java的ThreadLocal](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484677&idx=1&sn=d623f7d09a4c69c13e0a5c093979838c&chksm=9f38e62da84f6f3be0f810993e5082d0d02a1a2175981a8269e74f1c77e76791dc430a9f7c54&scene=21#wechat_redirect)
3，[MySQL的索引是什么？怎么优化？](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484680&idx=1&sn=4c123ba1e7202eb65d0788863d29729d&chksm=9f38e620a84f6f3664aedbb1dfa0ede6c35fff825583f332df01b565506f39527bfaf9c0bfad&scene=21#wechat_redirect)
4，[细谈CAS与ABA](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484676&idx=1&sn=dc7896439644bbee8a5f97acecec6858&chksm=9f38e62ca84f6f3ab80f1a4f055799f6db15ba434d0325e0b94ef31c57bd5aa220bd32390994&scene=21#wechat_redirect)
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFWF1rkKibTzeA8PicbicYXBsH26a9PXg2HNnlEt1thHBFxUtEjicACeaSlRWictpPziaMdibXmYq34dWfQ9w/640?wx_fmt=png)
