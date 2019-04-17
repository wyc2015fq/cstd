# 重拾C#教程：结构体 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年04月10日 12:56:36[boonya](https://me.csdn.net/boonya)阅读数：70








原文地址：[http://www.runoob.com/csharp/csharp-struct.html](http://www.runoob.com/csharp/csharp-struct.html)




在 C# 中，结构体是值类型数据结构。它使得一个单一变量可以存储各种数据类型的相关数据。**struct** 关键字用于创建结构体。

结构体是用来代表一个记录。假设您想跟踪图书馆中书的动态。您可能想跟踪每本书的以下属性：
- Title
- Author
- Subject
- Book ID


## 定义结构体

为了定义一个结构体，您必须使用 struct 语句。struct 语句为程序定义了一个带有多个成员的新的数据类型。

例如，您可以按照如下的方式声明 Book 结构：
structBooks{publicstring title;publicstring author;publicstring subject;publicint book_id;};
下面的程序演示了结构的用法：
usingSystem;usingSystem.Text;structBooks{publicstring title;publicstring author;publicstring subject;publicint book_id;};publicclass testStructure
{publicstaticvoidMain(string[] args){BooksBook1;/* 声明 Book1，类型为 Book */BooksBook2;/* 声明 Book2，类型为 Book *//* book 1 详述 */Book1.title ="C Programming";Book1.author ="Nuha Ali";Book1.subject ="C Programming Tutorial";Book1.book_id =6495407;/* book 2 详述 */Book2.title ="Telecom Billing";Book2.author ="Zara Ali";Book2.subject ="Telecom Billing Tutorial";Book2.book_id =6495700;/* 打印 Book1 信息 */Console.WriteLine("Book 1 title : {0}",Book1.title);Console.WriteLine("Book 1 author : {0}",Book1.author);Console.WriteLine("Book 1 subject : {0}",Book1.subject);Console.WriteLine("Book 1 book_id :{0}",Book1.book_id);/* 打印 Book2 信息 */Console.WriteLine("Book 2 title : {0}",Book2.title);Console.WriteLine("Book 2 author : {0}",Book2.author);Console.WriteLine("Book 2 subject : {0}",Book2.subject);Console.WriteLine("Book 2 book_id : {0}",Book2.book_id);Console.ReadKey();}}
当上面的代码被编译和执行时，它会产生下列结果：
Book1 title : C ProgrammingBook1 author :NuhaAliBook1 subject : C ProgrammingTutorialBook1 book_id :6495407Book2 title :TelecomBillingBook2 author :ZaraAliBook2 subject :TelecomBillingTutorialBook2 book_id :6495700
## C# 结构的特点

您已经用了一个简单的名为 Books 的结构。在 C# 中的结构与传统的 C 或 C++ 中的结构不同。C# 中的结构有以下特点：
- 结构可带有方法、字段、索引、属性、运算符方法和事件。
- 结构可定义构造函数，但不能定义析构函数。但是，您不能为结构定义默认的构造函数。默认的构造函数是自动定义的，且不能被改变。
- 与类不同，结构不能继承其他的结构或类。
- 结构不能作为其他结构或类的基础结构。
- 结构可实现一个或多个接口。
- 结构成员不能指定为 abstract、virtual 或 protected。
- 当您使用 **New** 操作符创建一个结构对象时，会调用适当的构造函数来创建结构。与类不同，结构可以不使用 New 操作符即可被实例化。
- 如果不使用 New 操作符，只有在所有的字段都被初始化之后，字段才被赋值，对象才被使用。

## 类 vs 结构

类和结构有以下几个基本的不同点：
- 类是引用类型，结构是值类型。
- 结构不支持继承。
- 结构不能声明默认的构造函数。

针对上述讨论，让我们重写前面的实例：
usingSystem;usingSystem.Text;structBooks{privatestring title;privatestring author;privatestring subject;privateint book_id;publicvoid getValues(string t,string a,string s,int id){
      title = t;
      author = a;
      subject = s;
      book_id =id;}publicvoid display(){Console.WriteLine("Title : {0}", title);Console.WriteLine("Author : {0}", author);Console.WriteLine("Subject : {0}", subject);Console.WriteLine("Book_id :{0}", book_id);}};publicclass testStructure
{publicstaticvoidMain(string[] args){BooksBook1=newBooks();/* 声明 Book1，类型为 Book */BooksBook2=newBooks();/* 声明 Book2，类型为 Book *//* book 1 详述 */Book1.getValues("C Programming","Nuha Ali","C Programming Tutorial",6495407);/* book 2 详述 */Book2.getValues("Telecom Billing","Zara Ali","Telecom Billing Tutorial",6495700);/* 打印 Book1 信息 */Book1.display();/* 打印 Book2 信息 */Book2.display();Console.ReadKey();}}
当上面的代码被编译和执行时，它会产生下列结果：
Title: C ProgrammingAuthor:NuhaAliSubject: C ProgrammingTutorialBook_id:6495407Title:TelecomBillingAuthor:ZaraAliSubject:TelecomBillingTutorialBook_id:6495700



