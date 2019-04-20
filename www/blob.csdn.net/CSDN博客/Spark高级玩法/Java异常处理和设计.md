# Java异常处理和设计 - Spark高级玩法 - CSDN博客
2019年03月27日 21:05:09[Spark高级玩法](https://me.csdn.net/rlnLo2pNEfx9c)阅读数：8
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/adI0ApTVBFW4vy9MpqyEibcNLoJYwKkwktmbbgbAZRgBibTPQuLJUTH3cdSoGx0N4yOsydFMF60iaUQhmbfxp8BvQ/640?wx_fmt=jpeg)
在程序设计中，进行异常处理是非常关键和重要的一部分。一个程序的异常处理框架的好坏直接影响到整个项目的代码质量以及后期维护成本和难度。试想一下，如果一个项目从头到尾没有考虑过异常处理，当程序出错从哪里寻找出错的根源？但是如果一个项目异常处理设计地过多，又会严重影响到代码质量以及程序的性能。因此，如何高效简洁地设计异常处理是一门艺术，本文下面先讲述Java异常机制最基础的知识，然后给出在进行Java异常处理设计时的几个建议。
若有不正之处，请多多谅解和指正，不胜感激。
请尊重作者劳动成果，转载请标明转载地址：http://www.cnblogs.com/dolphin0520/p/3769804.html
以下是本文的目录大纲：
- 
一.什么是异常
- 
Java中如何处理异常
- 
深刻理解try,catch,finally,throws,throw五个关键字
- 
在类继承的时候，方法覆盖时如何进行异常抛出声明
- 
异常处理和设计的几个建议
**一.什么是异常**
异常的英文单词是exception，字面翻译就是“意外、例外”的意思，也就是非正常情况。通过异常机制，我们可以更好地提升程序的健壮性。
在Java中异常被当做对象来处理，根类是java.lang.Throwable类，在Java中定义了很多异常类（如OutOfMemoryError、NullPointerException、IndexOutOfBoundsException等），这些异常类分为两大类：Error和Exception。
Error用于指示合理的应用程序不应该试图捕获的严重问题，这种情况是很大的问题，大到你无法处理，是无法处理的异常，因此可以不用管它。比如OutOfMemoryError，断电、断网、服务器宕机等，一般发生这种异常，JVM会选择终止程序。因此我们编写程序时不需要关心这类异常。
Exception，也就是我们经常见到的一些异常情况，比如NullPointerException、IndexOutOfBoundsException，这些异常是我们可以处理的异常。
Exception类的异常包括checked exception和unchecked exception（unchecked exception也称运行时异常RuntimeException。
**unchecked exception（非检查异常）**，也称运行时异常（RuntimeException），比如常见的NullPointerException、IndexOutOfBoundsException、ArithmeticException(算术异常)、ClassCastException(类型强制转换异常)等。对于运行时异常，java编译器不要求必须进行异常捕获处理或者抛出声明，由程序员自行决定，即也可以使用try..catch...块显示捕获处理，或者显示抛出。
**checked exception（检查异常）**，也称非运行时异常（运行时异常以外的异常就是非运行时异常），java编译器强制程序员必须进行捕获处理或者显示抛出，比如常见的IOExeption和SQLException。对于非运行时异常如果不进行捕获或者抛出声明处理，编译都不会通过。
在Java中，异常类的结构层次图如下图所示：
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFW4vy9MpqyEibcNLoJYwKkwk1aAFadWdiaVoVLjydWGuGxiadvRhLzia9hL4TMlicz8PUP59Jw3oXP8YEw/640?wx_fmt=png)
在Java中，所有异常类的父类是Throwable类（java.lang包下），Error类是error类型异常的父类，Exception类是exception类型异常的父类，RuntimeException类是所有运行时异常的父类，RuntimeException以外的并且继承Exception的类是非运行时异常。
典型的RuntimeException包括NullPointerException、IndexOutOfBoundsException、IllegalArgumentException（非法参数异常）等。
典型的非RuntimeException包括IOException、SQLException等。
**二.Java中如何处理异常**
在Java中如果需要处理异常，必须先对异常进行捕获，然后再对异常情况进行处理。如何对可能发生异常的代码进行异常捕获和处理呢？使用try和catch关键字即可，如下面一段代码所示：
try {
  File file =new File("d:/a.txt");
if(!file.exists())
    file.createNewFile();
} catch(IOException e) {
// TODO: handle exception
}
被try块包围的代码说明这段代码可能会发生异常，一旦发生异常，异常便会被catch捕获到，然后需要在catch块中进行异常处理。
这是一种处理异常的方式。在Java中还提供了另一种异常处理方式即抛出异常，顾名思义，也就是说一旦发生异常，我把这个异常抛出去，让调用者去进行处理，自己不进行具体的处理，此时需要用到throw和throws关键字。　
下面看一个示例：
publicclassMain{
publicstatic voidmain(String[] args){
try{
            createFile();
        }catch (Exception e) {
// TODO: handle exception
        }
    }
publicstatic voidcreateFile()throws IOException{
        File file =new File("d:/a.txt");
if(!file.exists())
            file.createNewFile();
    }
}
这段代码和上面一段代码的区别是，在实际的createFile方法中并没有捕获异常，而是用throws关键字声明抛出异常，即告知这个方法的调用者此方法可能会抛出IOException。那么在main方法中调用createFile方法的时候，采用try...catch块进行了异常捕获处理。
当然还可以采用throw关键字手动来抛出异常对象。下面看一个例子：
publicclassMain {
publicstatic voidmain(String[] args) {
try{
int[] data =newint[]{1,2,3};
            System.out.println(getDataByIndex(-1,data));
        }catch (Exception e) {
            System.out.println(e.getMessage());
        }
    }
publicstatic intgetDataByIndex(int index,int[] data)throws Exception {
if(index<0||index>=data.length)
thrownew ArrayIndexOutOfBoundsException("数组下标越界");
        returndata[index];
    }
}
然后在catch块中进行捕获。
也就说在Java中进行异常处理的话，对于可能会发生异常的代码，可以选择三种方法来进行异常处理：
1）对代码块用try..catch进行异常捕获处理；
2）在 该代码的方法签名处用throws进行抛出声明，告知此方法的调用者这段代码可能会出现这些异常，你需要谨慎处理。此时有两种情况：
如果声明抛出的异常是非运行时异常，此方法的调用者必须显示地用try..catch块进行捕获或者继续向上层抛出异常。
如果声明抛出的异常是运行时异常，此方法可以用try..catch进行异常捕获处理，也可以不捕获，此方法无需使用throws声明抛出；此方法的调用者可以选择地进行异常捕获处理，也可不捕获处理，同样也可以不使用throws声明。
3）在代码块用throw手动抛出一个异常对象，此时也有两种情况，跟2）中的类似：
如果抛出的异常对象是非运行时异常，要么用try..catch进行异常捕获处理，要么放在一个带throws声明抛出的方法中，此方法的调用者必须显示地用try..catch块进行捕获或者继续向上层抛出异常。
如果抛出的异常对象是运行时异常，此方法可以用try..catch进行异常捕获处理，也可以不捕获，此方法无需使用throws声明抛出；此方法的调用者可以选择地进行异常捕获处理也可不捕获处理，同样也可以不使用throws声明。（如果最终将异常抛给main方法，则相当于交给jvm自动处理，此时jvm会简单地打印异常信息）
**三.深刻理解try,catch,finally,throws,throw五个关键字**
下面我们来看一下异常机制中五个关键字的用法以及需要注意的地方。
**1.try,catch,finally**
try关键字用来包围可能会出现异常的逻辑代码，它单独无法使用，必须配合catch或者finally使用。Java编译器允许的组合使用形式只有以下三种形式：
- 
- 
- 
`try...catch...;     ``try....finally......;    ``try....catch...finally...`
当然catch块可以有多个，注意try块只能有一个,finally块是可选的（但是最多只能有一个finally块）。
三个块执行的顺序为 :
- 
` try—>catch—>finally。`
当然如果没有发生异常，则catch块不会执行。但是finally块无论在什么情况下都是会执行的，及时try块和catch块中有return语句或throw语句，也会先跳到finally块执行，然后再跳回执行return或throw语句，（这点要非常注意，因此部分情况下，都会将释放资源的操作放在finally块中进行）。因此尽量避免在finally块中使用return或throw等语句，会导致方法终止，而无法跳回try块或catch块执行retrun或throw语句。           有一种例外不会执行finally块，就是在try或catch块中使用了System.exit(0或1)；语句，则直接退出程序，finally得不到执行。　
finally块常用于回收物理资源，如关闭文件、关闭数据库连接、关闭网络连接等。
在有多个catch块的时候，是按照catch块的先后顺序进行匹配的，一旦异常类型被一个catch块匹配，则不会与后面的catch块进行匹配，因此小范围的异常类型应该放在前面，大异常范围的应该放在后面（后者小异常的catch不会得到执行，导致编译器报错）。
在使用try..catch..finally块的时候，注意千万不要在finally块中使用return，因为finally中的return会覆盖已有的返回值。下面看一个例子：
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
publicclassMain {
publicstatic voidmain(String[] args) {
        String str =new  Main().openFile();
        System.out.println(str);
    }
publicString openFile() {
try{
            FileInputStream inputStream =new FileInputStream("d:/a.txt");
            intch = inputStream.read();
            System.out.println("aaa");
return"step1";
        }catch (FileNotFoundException e) {
            System.out.println("file not found");
return"step2";
        }catch(IOException e) {
            System.out.println("io exception");
return"step3";
        }finally{
            System.out.println("finally block");
//return "finally";
        }
    }
}
这段程序的输出结果为：
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFW4vy9MpqyEibcNLoJYwKkwkEw7UBUtiagN5U64FWgl0yib3ibDThMmOKV26icBkmw2H3cxq1Cx5UnLhdQ/640?wx_fmt=png)
可以看出，在try块中发生FileNotFoundException之后，就跳到第一个catch块，打印"file not found"信息，遇到return语句，跳到finally块执行，执行完后跳回return语句将"step2"赋值给返回值，将返回值返回。
从这个例子说明，无论try块或者catch块中是否包含return语句，都会执行finally块。
如果将这个程序稍微修改一下，将finally块中的return语句注释去掉，运行结果是：
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFW4vy9MpqyEibcNLoJYwKkwkBK2vTHyfpD8d3RFPg0ygffTlRlLWrIXMZcEdHOhv4Xxm1LV0nbtozg/640?wx_fmt=png)
最后打印出的是"finally"，返回值被重新覆盖了。
因此如果方法有返回值，切忌不要再finally中使用return，这样会使得程序结构变得混乱。
**2.throws和thow关键字**
throws抛出的是异常类型（表明方法可能会抛出的异常类型），throw抛出的是异常对象（实例）
1）throws出现在方法的声明中，表示该方法可能会抛出的异常，然后交给上层调用它的方法程序处理，允许throws后面跟着多个异常类型，使用英文逗号隔开；
2）一般会用于程序出现某种逻辑时程序员主动抛出某种特定类型的异常。throw只会出现在方法体中，当方法在执行过程中遇到异常情况时，将异常信息封装为异常对象，然后throw出去。throw关键字的一个非常重要的作用就是异常类型的转换（会在后面阐述道）。
throws表示出现异常的一种可能性，并不一定会发生这些异常；throw则是抛出了异常，执行throw则一定抛出了某种异常对象。两者都是消极处理异常的方式（这里的消极并不是说这种方式不好），只是抛出或者可能抛出异常，但是不会由方法去处理异常，真正的处理异常由此方法的上层调用处理。
**四.在类继承的时候，方法覆盖时如何进行异常抛出声明**
 本小节讨论子类重写父类方法的时候，如何确定异常抛出声明的类型。下面是三点原则：
1）父类的方法没有声明异常，子类在重写该方法的时候不能声明异常；
2）如果父类的方法声明一个异常exception1，则子类在重写该方法的时候声明的异常不能是exception1的父类，即子类重写的方法抛出的异常范围不能比父类大；
3）如果父类的方法声明的异常类型只有非运行时异常（或运行时异常），则子类在重写该方法的时候声明的异常也只能有非运行时异常（或运行时异常），不能含有运行时异常（非运行时异常）。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFW4vy9MpqyEibcNLoJYwKkwkJoJJw7IVr4VZC5FysKw3y2Czno7sNDD61cUAHZ0AepvbFDwic8C3gpw/640?wx_fmt=png)
## **五、Throwable类中的常用方法**
注意：catch关键字后面括号中的Exception类型的参数e。Exception就是try代码块传递给catch代码块的变量类型，e就是变量名。catch代码块中语句"e.getMessage();"用于输出错误性质。通常异常处理常用3个函数来获取异常的有关信息:
getCause()：返回抛出异常的原因。如果 cause 不存在或未知，则返回 null。
getMeage()：返回异常的消息信息。
printStackTrace()：对象的堆栈跟踪输出至错误输出流，作为字段 System.err 的值。
有时为了简单会忽略掉catch语句后的代码，这样try-catch语句就成了一种摆设，一旦程序在运行过程中出现了异常，就会忽略处理异常，而错误发生的原因很难查找。
**六、java常见的异常**
在Java中提供了一些异常用来描述经常发生的错误，对于这些异常，有的需要程序员进行捕获处理或声明抛出，有的是由Java虚拟机自动进行捕获处理。Java中常见的异常类:
### 1. runtimeException子类:
1、 java.lang.ArrayIndexOutOfBoundsException
    数组索引越界异常。当对数组的索引值为负数或大于等于数组大小时抛出。
    2、java.lang.ArithmeticException
    算术条件异常。譬如：整数除零等。
    3、java.lang.NullPointerException
    空指针异常。当应用试图在要求使用对象的地方使用了null时，抛出该异常。譬如：调用null对象的实例方法、访问null对象的属性、计算null对象的长度、使用throw语句抛出null等等
    4、java.lang.ClassNotFoundException
    找不到类异常。当应用试图根据字符串形式的类名构造类，而在遍历CLASSPAH之后找不到对应名称的class文件时，抛出该异常。
   5、java.lang.NegativeArraySizeException  数组长度为负异常
   6、java.lang.ArrayStoreException 数组中包含不兼容的值抛出的异常
   7、java.lang.SecurityException 安全性异常
   8、java.lang.IllegalArgumentException 非法参数异常
2.IOException
IOException：操作输入流和输出流时可能出现的异常。
EOFException   文件已结束异常
FileNotFoundException   文件未找到异常
### 3. 其他
ClassCastException    类型转换异常类
ArrayStoreException  数组中包含不兼容的值抛出的异常
SQLException   操作数据库异常类
NoSuchFieldException   字段未找到异常
NoSuchMethodException   方法未找到抛出的异常
NumberFormatException    字符串转换为数字抛出的异常
StringIndexOutOfBoundsException 字符串索引超出范围抛出的异常
IllegalAccessException  不允许访问某类异常
InstantiationException  当应用程序试图使用Class类中的newInstance()方法创建一个类的实例，而指定的类对象无法被实例化时，抛出该异常
**七、自定义异常**
自定义异常通常是定义一个继承自Exception类的子类。一般情况下我们都会直接继承自Exception类，当然也可以自定义运行时异常，只需继承RuntimeException类即可。
自定义异常时，通常需要提供两个构造器，（1）、一个无参的构造器；（2）带一个字符串参数的构造器，该字符串作为异常对象的描述信息（即异常对象的getMessage()方法的返回值，用于初始化继承自父类中的message属性）
（3）在异常链中还需要一种带一个异常类型参数的构造器，具体介绍见下。
1、创建自定义异常：
publicclassMyExceptionextendsException{
publicMyException(){
super();
    }
publicMyException(String msg){
super(msg);
    }
}
2、在类中使用异常：
publicclassExceptionTest {
publicstaticvoidexecute(String a) throws MyException {
        System.out.println("execute...");
if("true".equals(a)){
thrownew MyException("参数不能为 true");
        }
    }
}
（3）带有一个异常类型参数的构造器
把底层的原始异常直接传给用户是一种不负责任的表现，通常的做法是：程序先捕获原始异常，然后抛出一个新的业务异常，新的业务异常中包含了对用户的提示信息，这种处理方式呗称为异常转译。在异常链中自定义一个异常类通常还需添加一个构造器，构造器带有一个异常类型的参数，该参数用于初始化从Throwable类继承的cause对象，这个cause对象用于表示原始异常，使得抛出新的异常，程序员仍然可以通过这个新的异常到原始异常（即异常最初发生的位置）。
**八、异常链（异常转译）**
把底层的原始异常直接传给用户是一种不负责任的表现，通常的做法是：程序先捕获原始异常，然后抛出一个新的业务异常，新的业务异常中包含了对用户的提示信息，这种处理方式呗称为异常转译。在异常链中自定义一个异常类通常还需添加一个构造器，构造器带有一个异常类型的参数，该参数用于初始化从Throwable类继承的cause对象，这个cause对象用于表示原始异常，使得抛出新的异常，程序员仍然可以通过这个新的异常到原始异常（即异常最初发生的位置）。
这种把捕获一个异常然后抛出另一个异常，并把原始异常信息保存下来是一种典型的链式处理。也被称为“异常链”。
假如程序需要实现工资计算的方法
publiccalSal()throws SalException
{
try
 {
//实现结算工资的业务逻辑
  ...
 }
catch（SQLException sqle）
 {
//把原始异常记录下来，留给管理员
  ... 
//下面异常中的message就是对用户的提示
thrownew SalException("访问底层数据库出现异常")；
 }
catch（Exception e）
 {
//把原始异常记录下来，留给管理员
  ... 
//下面异常中的message就是对用户的提示
thrownew SalException("系统出现未知异常")；
 }
}
上面的程序实现了异常转译，从而把原始异常信息隐藏起来，仅向上提供必要的异常提示信息。但是无法追踪到原始异常了，
下面通过带一个异常类型参数的构造器创建异常对象，从而可以将原始异常封装在新的异常中，便于追踪原始异常。
publiccalSal()throws SalException
{
try
 {
//实现结算工资的业务逻辑
  ...
 }
catch（SQLException sqle）
 {
//把原始异常记录下来，留给管理员
  ... 
//下面异常中的sql就是原始异常
thrownew SalException(sql)；
 }
catch（Exception e）
 {
//把原始异常记录下来，留给管理员
  ... 
//下面异常中的e就是原始异常
thrownew SalException(e)；
 }
}
publicclassSalExceptionextendsException
{
publicSalException();//无参构造器
/*带一个字符串参数的构造器，msg用于初始化从父类中继承的message属性，
 其是getMessage()的返回值，即该异常的详细描述详细*/
publicSalException(String msg)
{
super(msg);
 }
/*带一个异常类型参数构造器，异常对象形参t用于初始化从父类中继承的cause对象，
 该cause对象用于保存原始异常对象*/
publicSalException(Throwable t)
{
super(t);
 }
}
**九.异常处理和设计的几个建议**
以下是根据前人总结的一些异常处理的建议：
**1.只在必要使用异常的地方才使用异常，不要用异常去控制程序的流程**
谨慎地使用异常，异常捕获的代价非常高昂，异常使用过多会严重影响程序的性能。如果在程序中能够用if语句和Boolean变量来进行逻辑判断，那么尽量减少异常的使用，从而避免不必要的异常捕获和处理。比如下面这段经典的程序：
publicvoiduseExceptionsForFlowControl(){   
try{   
while(true) {   
    increaseCount();   
    }   
  }catch (MaximumCountReachedException ex) {   
  }   
//Continue execution   
}   
publicvoidincreaseCount() throwsMaximumCountReachedException {   
if(count >= 5000)   
thrownew MaximumCountReachedException();   
}
上边的useExceptionsForFlowControl()用一个无限循环来增加count直到抛出异常，这种做法并没有说让代码不易读，而是使得程序执行效率降低。
**2.切忌使用空catch块**
在捕获了异常之后什么都不做，相当于忽略了这个异常。千万不要使用空的catch块，空的catch块意味着你在程序中隐藏了错误和异常，并且很可能导致程序出现不可控的执行结果。如果你非常肯定捕获到的异常不会以任何方式对程序造成影响，最好用Log日志将该异常进行记录，以便日后方便更新和维护。
**3.检查异常和非检查异常的选择**
一旦你决定抛出异常，你就要决定抛出什么异常。这里面的主要问题就是抛出检查异常还是非检查异常。
检查异常导致了太多的try…catch代码，可能有很多检查异常对开发人员来说是无法合理地进行处理的，比如SQLException，而开发人员却不得不去进行try…catch，这样就会导致经常出现这样一种情况：逻辑代码只有很少的几行，而进行异常捕获和处理的代码却有很多行。这样不仅导致逻辑代码阅读起来晦涩难懂，而且降低了程序的性能。
我个人建议尽量避免检查异常的使用，如果确实该异常情况的出现很普遍，需要提醒调用者注意处理的话，就使用检查异常；否则使用非检查异常。
因此，在一般情况下，我觉得尽量将检查异常转变为非检查异常交给上层处理。
**4.注意catch块的顺序**
不要把上层类的异常放在最前面的catch块。比如下面这段代码：
try {
        FileInputStream inputStream =new FileInputStream("d:/a.txt");
        intch = inputStream.read();
        System.out.println("aaa");
return"step1";
    }catch (IOException e) {
　　      System.out.println("io exception");　　       
return"step2";
    }catch(FileNotFoundException e) {
        System.out.println("file not found");　　　　       
return"step3";
    }finally{
        System.out.println("finally block");
//return "finally";
    }
第二个catch的FileNotFoundException将永远不会被捕获到，因为FileNotFoundException是IOException的子类。
**5.不要将提供给用户看的信息放在异常信息里**
比如下面这段代码：
publicclassMain {
publicstatic voidmain(String[] args) {
try{
            String user =null;
            String pwd =null;
            login(user,pwd);
        }catch (Exception e) {
            System.out.println(e.getMessage());
        }
    }
publicstatic voidlogin(String user,String pwd) {
if(user==null||pwd==null)
thrownew NullPointerException("用户名或者密码为空");
//...
    }
}
展示给用户错误提示信息最好不要跟程序混淆一起，比较好的方式是将所有错误提示信息放在一个配置文件中统一管理。
**6.避免多次在日志信息中记录同一个异常**
只在异常最开始发生的地方进行日志信息记录。很多情况下异常都是层层向上抛出的，如果在每次向上抛出的时候，都Log到日志系统中，则会导致无从查找异常发生的根源。
**7. 异常处理尽量放在高层进行**
尽量将异常统一抛给上层调用者，由上层调用者统一之时如何进行处理。如果在每个出现异常的地方都直接进行处理，会导致程序异常处理流程混乱，不利于后期维护和异常错误排查。由上层统一进行处理会使得整个程序的流程清晰易懂。
**8. 在finally中释放资源**
如果有使用文件读取、网络操作以及数据库操作等，记得在finally中释放资源。这样不仅会使得程序占用更少的资源，也会避免不必要的由于资源未释放而发生的异常情况。 
推荐阅读
[面试|return 和finally那些事儿](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247486116&idx=1&sn=fc8d97364d064a06d5f4c555ed28d5dd&chksm=9f38e98ca84f609a6fc71bff7303b5324823eb5a073ee5e0e147a93d23c3134dfa5232d77284&scene=21#wechat_redirect)
[一千行 MySQL 学习笔记](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247486607&idx=1&sn=3eac73119f6f8b6fa888eb0edd3a4cc2&chksm=9f38efa7a84f66b1fbcc38444889b970bd2f5cb1b05bbf1e4f911fd8ccfddc9d3df0b7cef6f1&scene=21#wechat_redirect)
[没娱乐、没性生活，中关村程序员要被逼疯了](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247486617&idx=1&sn=0b2fa8024e73ecbdcdb31af3b25e68c9&chksm=9f38efb1a84f66a7e5f86e3d199746567d8f226c7703a6ee2532ba29d87b8c7a1525caf6c3fd&scene=21#wechat_redirect)
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFWF1rkKibTzeA8PicbicYXBsH26a9PXg2HNnlEt1thHBFxUtEjicACeaSlRWictpPziaMdibXmYq34dWfQ9w/640?wx_fmt=png)
