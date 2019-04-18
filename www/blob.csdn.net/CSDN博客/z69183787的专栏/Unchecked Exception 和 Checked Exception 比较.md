# Unchecked Exception 和 Checked Exception 比较 - z69183787的专栏 - CSDN博客
2017年03月13日 18:00:47[OkidoGreen](https://me.csdn.net/z69183787)阅读数：597
Throwable类是所有异常的始祖，它有两个直接子类Error / Exception： 
  Error仅在Java虚拟机中发生动态连接失败或其它的定位失败的时候抛出一个Error对象。一般程序不用捕捉或抛出Error对象。 
Unchecked Exception:
a. 指的是程序的瑕疵或逻辑错误，并且在运行时无法恢复。 
b. 包括Error与RuntimeException及其子类，如：OutOfMemoryError, UndeclaredThrowableException, IllegalArgumentException, IllegalMonitorStateException, NullPointerException, IllegalStateException, IndexOutOfBoundsException等。 
c. 语法上不需要声明抛出异常。 
Checked Exception:
a. 代表程序不能直接控制的无效外界情况（如用户输入，数据库问题，网络异常，文件丢失等） 
b. 除了Error和RuntimeException及其子类之外，如：ClassNotFoundException, NamingException, ServletException, SQLException, IOException等。 
c. 需要try catch处理或throws声明抛出异常。 
有点困惑的是：RuntimeException (Unchecked)是Exception (Checked)的子类。 
示例： 
Java代码  ![收藏代码](http://czj4451.iteye.com/images/icon_star.png)
- publicclass GenericException extends Exception {  
- public GenericException() {  
-     }  
- 
- public GenericException(String message) {  
- super(message);  
-     }  
- }  
Java代码  ![收藏代码](http://czj4451.iteye.com/images/icon_star.png)
- publicclass TestException {  
- publicvoid first() throws GenericException {  
- thrownew GenericException("Generic exception"); // Checked Exception需要显式声明抛出异常或者try catch处理
-     }  
- 
- publicvoid second(String msg) {  
- if (msg == null)  
- thrownew NullPointerException("Msg is null"); // Unchecked Exception语法上不需要处理
-     }  
- 
- publicvoid third() throws GenericException {  
-         first(); // 调用有Checked Exception抛出的方法也需要try catch或声明抛出异常
-     }  
- 
- publicstaticvoid main(String[] args) {  
-         TestException test = new TestException();  
- try {  
-             test.first();  
-         } catch (GenericException e) {  
-             e.printStackTrace();  
-         }  
- 
-         test.second(null);  
-     }  
- }  
- 

一、
[Java](http://lib.csdn.net/base/javase) 中定义了两类异常： 
1) Checked exception: 这类异常都是Exception的子类 。异常的向上抛出机制进行处理，如果子类可能产生A异常，那么在父类中也必须throws A异常。可能导致的问题：代码效率低，耦合度过高。C#中就没有使用这种异常机制。 
2) Unchecked exception: 这类异常都是RuntimeException的子类，虽然RuntimeException同样也是Exception的子类，但是它们是特殊的，它们不能通过client code来试图解决，所以称为Unchecked exception 。
另外：
error 表示恢复不是不可能但很困难的情况下的一种严重问题。比如说内存溢出。不可能指望程序能处理这样的情况。
exception 表示一种设计或实现问题。也就是说，它表示如果程序运行正常，从不会发生的情况
二、
checked exception是需要强制catch的异常，你在调用这个方法的时候，你如果不catch这个异常，那么编译器就会报错，比如说我们读写文件的时候会catch IOException，执行[数据库](http://lib.csdn.net/base/mysql)操作会有SQLException等 
UnChecked Exception是RuntimeException，也就是说运行时的异常，这种异常不是必须需要catch的，你是无法预料的，比如说你在调用一个list.szie()的时候，如果这个list为null，那么就会报NUllPointerException，而这个异常就是RuntimeException，也就是UnChecked Exception
三、
Error和RuntimeException及其子类是unchecked exception.其他exception是checked exception. 
checked exception可以出现在throws子句中，unchecked exception不可以。 
Error是java自己的错误或者诸如内存耗尽等严重错误，是不可抗拒的，显然没有捕捉的必要，而且也没有办法捕捉。 
RuntimeException是你的程序有逻辑错误，是程序员应该积极避免其出现的异常。比如NullPointerException等，完全是程序员马虎出的错。当遇到这种错误时，java将这个错误自动捕捉到，比如显示到concole里，然后继续运行。而checked exception如果不捕捉则会导致程序终止。
四、error和excption的区别
Error的继承关系：
java.lang.Object
--java.lang.Throwable
--java.lang.Error
Exception的继承关系：
java.lang.Object
--java.lang.Throwable
--java.lang.Exception
二者的不同之处：
Exception：
1．可以是可被控制(checked) 或不可控制的(unchecked)
2．表示一个由程序员导致的错误
3．应该在应用程序级被处理
Error：
1．总是不可控制的(unchecked)
2．经常用来用于表示系统错误或低层资源的错误
3．如何可能的话，应该在系统级被捕捉
