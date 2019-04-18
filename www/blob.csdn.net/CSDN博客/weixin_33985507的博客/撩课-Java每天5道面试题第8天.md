# 撩课-Java每天5道面试题第8天 - weixin_33985507的博客 - CSDN博客
2018年11月19日 08:59:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：3
### 71.jsp有哪些内置对象?作用分别是什么?
```
Page，
pageContext，
request，
response，
session，
application，
out，
config，
exception
Page指的是JSP被翻译成Servlet的对象的引用.
pageContext对象可以用来获得其他8个内置对象,
还可以作为JSP的域范围对象使用.
pageContext中存的值是当前的页面的作用范围
request代表的是请求对象,
可以用于获得客户机的信息,
也可以作为域对象来使用，
使用request保存的数据
在一次请求范围内有效。
Session代表的是一次会话，
可以用于保存用户的私有的信息,
也可以作为域对象使用，
使用session保存的数据在一次会话范围有效
Application：代表整个应用范围,
使用这个对象保存的数据
在整个web应用中都有效。
Response是响应对象,
代表的是从服务器向浏览器响应数据.
Out:JSPWriter是用于
向页面输出内容的对象
Config：指的是ServletConfig
用于JSP翻译成Servlet后 
获得Servlet的配置的对象.
Exception:在页面中设置isErrorPage=”true”，
即可使用，
是Throwable的引用.用来获得页面的错误信息。
```
### 72.jsp有哪些动作?作用分别是什么?
```
jsp:include：
在页面被请求的时候引入一个文件。
jsp:useBean：
寻找或者实例化一个JavaBean。
jsp:setProperty：
设置JavaBean的属性。
jsp:getProperty：
输出某个JavaBean的属性。
jsp:plugin：
根据浏览器类型为Java插件生成OBJECT或EMBED标记。
jsp:forward：
把请求转到一个新的页面。
```
### 73.JSP中动态INCLUDE与静态INCLUDE的区别？
```
1. 静态include的结果是
把其他jsp引入当前jsp,
两者合为一体
动态include的结构是两者独立,
直到输出时才合并
看看jsp生成的java文件就可以知道了
2.正是因为这样,
动态include的jsp文件独立性很强,
是一个单独的jsp文件,
需要使用的对象,
页面设置,都必须有自己创建,
当然,还好它和include
它的页面的request范围是一致的.
而静态include纯粹是把代码
写在外面的一种共享方法,
所有的变量都是可以
和include它的主文件共享
,两者高度紧密结合,
不能有变量同名的冲突.
而页面设置也可以借用主文件的.
```
### 74.说一说Servlet的生命周期?
```
servlet 的生命周期是有四个阶段：
实例化 –> 
初始化 –> 
请求处理 –> 
销毁
创建 Servlet 实例。
Web 容器调用 Servlet 的 init() 方法，
对Servlet 进行初始化。
Servlet 初始化后，
将一直存在于容器中，
用于响应客户端请求。
根据客户端的请求方式通过 Servlet 中service() 方法
去相应的 goGet(),或 doPost() 方法；
Web 容器销毁Servlet 时，
调用 Servlet 的 destroy() 方法，
通常在关闭Web容器之时销毁Servlet。
servlet 生命周期的三个方法：
​ init()方法：
Servlet实例化时调用此方法
​ Service()方法：
客户请求和响应调用此方法。
​ Destroy()方法：
释放内存，关闭web服务器调用此方法
```
### 75.说说JSP 的生命周期?
```
浏览器首先要请求一个以.jsp扩展名结尾的页面，
发起JSP请求，
然后，Web服务器读取这个请求，
使用JSP编译器把JSP页面
转化成一个Servlet类。
需要注意的是，
只有当第一次请求页面
或者是JSP文件发生改变的时候
JSP文件才会被编译，
然后服务器调用servlet类，
处理浏览器的请求。
一旦请求执行结束，
servlet会把响应发送给客户端。
jsp 的生命周期分为四个阶段：
编译
初始化
执行
销毁
```
