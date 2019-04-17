# cookie及session - 专注于数据挖掘算法研究和应用 - CSDN博客





2009年03月25日 10:11:00[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：2102标签：[session																[servlet																[浏览器																[服务器																[存储																[url](https://so.csdn.net/so/search/s.do?q=url&t=blog)




1， web中为何要引入cookie和session 
    我知道对于动态网页，有在页面之间传递参数的需求，常见的有表单传递，URL传递，引入cookie和session也是用来在页面中传递参数的吧 
2， cookie,session是如何传递参数的： 
      所有的变量都是在服务端产生的，我说的变量较笼统，一般指： “名称=值” 
      对于cookie，服务器的某个servlet根据需要把产生的变量保存在cookie对象中，然后发给客户端的浏览器保存，当下次请求该servlet时，浏览器也会把相关的cookie信息发过来， servlet根据cookie对象的名称可以获取到其值 

      对于session，同样服务器的某个servlet根据需要把产生的变量保存在session对象中, 然后返回给客户端该session的一个id（身份表示码），当下次请求时该id(session标识码)也会传递给服务器(任何一个servlet都可获取到)，服务器根据该id从而可以判定是哪个用户的session 

3， cookie,session保存信息的方式及技术支持等： 
      cookie信息保存在“客户端”的浏览器中，具体是依txt格式存在硬盘中，它是由浏览器提供的一种机制，是对http协议的扩展 ，一个用户可分配多个cookie对象，每一个cookie存储一个“名称=值; 属性1； 属性2；。。。” 
      session信息保存在“服务端”,是http协议提供的一种机制，一个用户只分配一个session对象，里面存储该用户相关的属性值 

4， cookie,session的生存期： 
    cookie，其生存期从产生到给定的时间结束（时间可由用户设定），如果时间值大于0，则浏览器关闭也没影响 
    session，从产生到给定的时间结束，如果用户不设置，则按默认时间,一旦超出这个时间还没登陆服务器，则session失效 

5， cookie,session其它的，请朋友们补充吧，我也不是很清楚了： 
    比如二者适用范围，与url传递相比有什么好处，](https://so.csdn.net/so/search/s.do?q=存储&t=blog)](https://so.csdn.net/so/search/s.do?q=服务器&t=blog)](https://so.csdn.net/so/search/s.do?q=浏览器&t=blog)](https://so.csdn.net/so/search/s.do?q=servlet&t=blog)](https://so.csdn.net/so/search/s.do?q=session&t=blog)




