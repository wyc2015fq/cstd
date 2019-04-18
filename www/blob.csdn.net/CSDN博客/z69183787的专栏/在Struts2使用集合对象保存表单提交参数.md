# 在Struts2使用集合对象保存表单提交参数 - z69183787的专栏 - CSDN博客
2015年07月22日 18:23:59[OkidoGreen](https://me.csdn.net/z69183787)阅读数：5441
先上图，有这样的业务场景，一次表单提交需要批量添加n本书籍的信息
![](http://hi.csdn.net/attachment/201109/18/0_1316331850NO5z.gif)
我们来看下如何用Struts2的特性来一步搞定它!
1.创建model类Book
**[java]**[view
 plain](http://blog.csdn.net/crazycoder2010/article/details/6787313#)[copy](http://blog.csdn.net/crazycoder2010/article/details/6787313#)[print](http://blog.csdn.net/crazycoder2010/article/details/6787313#)[?](http://blog.csdn.net/crazycoder2010/article/details/6787313#)
- package com.crazycoder2010.struts2.bean;  
- 
- publicclass Book {  
- private String name;  
- privateint price;  
- public String getName() {  
- return name;  
-     }  
- publicvoid setName(String name) {  
- this.name = name;  
-     }  
- publicint getPrice() {  
- return price;  
-     }  
- publicvoid setPrice(int price) {  
- this.price = price;  
-     }  
- }  
2.编写业务处理Action类AddBookAction
**[java]**[view
 plain](http://blog.csdn.net/crazycoder2010/article/details/6787313#)[copy](http://blog.csdn.net/crazycoder2010/article/details/6787313#)[print](http://blog.csdn.net/crazycoder2010/article/details/6787313#)[?](http://blog.csdn.net/crazycoder2010/article/details/6787313#)
- package com.crazycoder2010.struts2.action;  
- 
- import java.util.ArrayList;  
- import java.util.List;  
- 
- import com.crazycoder2010.struts2.bean.Book;  
- import com.opensymphony.xwork2.ActionSupport;  
- 
- publicclass AddBookAction extends ActionSupport {  
- privatestaticfinallong serialVersionUID = 8136233165580827673L;  
- private List<Book> books = new ArrayList<Book>();  
- 
- public List<Book> getBooks() {  
- return books;  
-     }  
- 
- publicvoid setBooks(List<Book> books) {  
- this.books = books;  
-     }  
- 
- @Override
- public String execute() throws Exception {  
- for(Book book : books){  
-             System.out.println(book.getName()+":"+book.getPrice());  
-         }  
- return SUCCESS;  
-     }  
- }  
3.编写页面add_book.jsp
**[html]**[view
 plain](http://blog.csdn.net/crazycoder2010/article/details/6787313#)[copy](http://blog.csdn.net/crazycoder2010/article/details/6787313#)[print](http://blog.csdn.net/crazycoder2010/article/details/6787313#)[?](http://blog.csdn.net/crazycoder2010/article/details/6787313#)
- <%@ taglib prefix="s"uri="/struts-tags"%>
- <html>
- <head>
- </head>
- <body>
- <s:formaction="addBook"method="post">
- <s:iteratorvalue="new int[3]"var="book"status="status">
- <label>Name:</label>
- <s:textfieldname="%{'books['+#status.index+'].name'}"theme="simple"></s:textfield>
- <label>Price:</label>
- <s:textfieldname="%{'books['+#status.index+'].price'}"theme="simple"></s:textfield>
- <br/>
- </s:iterator>
- <s:submitvalue="Add"></s:submit>
- </s:form>
- </body>
- </html>
这个表单中我们模拟创建3个book信息，每个book有name和price属性，这里需要特别注意的是表单元素的命名需要和我们AddBookAction中的List<Book> books 保持一直，名字分别为books[0].name,books[0].price;books[1].name,books[1].price，依次类推
4.在AddBookAction类同目录下建立一个AddBookAction-conversion.properties文件，内容如下
**[html]**[view
 plain](http://blog.csdn.net/crazycoder2010/article/details/6787313#)[copy](http://blog.csdn.net/crazycoder2010/article/details/6787313#)[print](http://blog.csdn.net/crazycoder2010/article/details/6787313#)[?](http://blog.csdn.net/crazycoder2010/article/details/6787313#)
- Element_books=com.crazycoder2010.struts2.bean.Book  
其中Element_作为前缀books就是这个AddBookAction中用来接受list参数的参数名
5.Run it!
![](http://hi.csdn.net/attachment/201109/18/0_1316332456hBn7.gif)
结果展示出我们的action已经可以接受页面提交过来的批量表单参数，这是struts2的一个非常有用的功能
