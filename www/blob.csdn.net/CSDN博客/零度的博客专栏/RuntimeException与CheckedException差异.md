# RuntimeException与CheckedException差异 - 零度的博客专栏 - CSDN博客
2016年08月08日 15:15:00[零度anngle](https://me.csdn.net/zmx729618)阅读数：589
[RuntimeException与CheckedException](http://www.cnblogs.com/perfy/archive/2013/06/15/3138192.html)
![](http://images.cnitblog.com/blog/367379/201306/15233816-b0fb36dcf6de4d14af4a0f8206439959.jpg)
Java的异常被分为两大类：Checked异常和Runtime异常（运行时异常）。所有RuntimeException类及其子类实例被称为Runtime异常；不是RuntimeException类及其子类的异常实例则称为Checked异常。
只有Java语言提供了Checked异常，其他语言都没有提供Checked异常。Java认为Checked异常都是可以被处理（修复）的异常，所以Java程序必须显式处理Checked异常。
       对于Checked异常的处理方式有两种：
（1）当前方法明确知道如何处理该异常，程序应该使用try...catch块来捕获该异常，然后对应的catch块中修补该异常。
（2）当前方法不知道如何处理这种异常，应该在定义该方法时声明抛出该异常。
       Runtime异常则更加灵活，Runtime异常无须显式声明抛出，如果程序需要捕捉Runtime异常，也可以使用try...catch块来捕捉Runtime异常。
 当使用throw语句自行抛出异常，如果throw语句抛出的异常是Checked异常，则该throw语句要么处于try块里，显式捕获该异常，要么放在一个带throws声明的方法中，即把该异常交给该方法的调用者处理；也就是说当出现了（不管是自行抛出的，还是系统抛出的）Checked异常，就要想办法去处理它，不能不理会它，要么显式地在try...catch块里捕获，处理它；要么把它放在一个带throws声明的方法中，把异常交给该方法的调用者处理。
       如果throw语句抛出的异常是Runtime异常，则该语句无须放在try块里，也无须放在带throws声明抛出的方法中；程序既可以显式使用try...catch来捕获，并处理异常，也可以完全不处理该异常，把异常交给该方法调用者处理。
ArithmeticException（除数为0的异常）, BufferOverflowException（缓冲区上溢异常）, BufferUnderflowException（缓冲区下溢异常）, IndexOutOfBoundsException（出界异常）, NullPointerException（空指针异常）, EmptyStackException（空栈异常）,
 IllegalArgumentException（不合法的参数异常）,  NegativeArraySizeException, NoSuchElementException, SecurityException, SystemException, UndeclaredThrowableException
1. java.lang.NullPointerException
　　异常的解释是"程序遇上了空指针"，简单地说就是调用了未经初始化的对象或者是不存在的对象，即把数组的初始化和数组元素的初始化混淆起来了。数组的初始化是对数组分配需要的空间，而初始化后的数组，其中的元素并没有实例化，依然是空的，所以还需要对每个元素都进行初始化（如果要调用的话）
　　2. java.lang.ClassNotFoundException　　异常的解释是"指定的类不存在"。
　　3. java.lang.ArithmeticException　　这个异常的解释是"数学运算异常"，比如程序中出现了除以零这样的运算就会出这样的异常。
　　4. java.lang.ArrayIndexOutOfBoundsException
　　异常的解释是"数组下标越界"，现在程序中大多都有对数组的操作，因此在调用数组的时候一定要认真检查，看自己调用的下标是不是超出了数组的范围，一般来说，显示（即直接用常数当下标）调用不太容易出这样的错，但隐式（即用变量表示下标）调用就经常出错了，还有一种情况，是程序中定义的数组的长度是通过某些特定方法决定的，不是事先声明的，这个时候，最好先查看一下数组的length，以免出现这个异常。
　　5. java.lang.IllegalArgumentException
　　这个异常的解释是"方法的参数错误"，比如g.setColor(int red,int green,int blue)这个方法中的三个值，如果有超过２５５的也会出现这个异常，因此一旦发现这个异常，我们要做的，就是赶紧去检查一下方法调用中的参数传递是不是出现了错误。
　　6. java.lang.IllegalAccessException
　　这个异常的解释是"没有访问权限"，当应用程序要调用一个类，但当前的方法即没有对该类的访问权限便会出现这个异常。对程序中用了Package的情况下要注意这个异常。
　　它们的不同之处表现在两方面:机制上和逻辑上。 
　　一、机制上 
　　它们在机制上的不同表现有两点:1.如何定义方法;   2. 如何处理抛出的异常。
请看下面CheckedException的定义: 
　　public class CheckedException extends Exception 
　　{ 
　　public CheckedException() {} 
　　public CheckedException( String message ) 
　　{ 
　　super( message ); 
　　} 
　　} 
　　以及一个使用exception的例子: 
　　public class ExceptionalClass 
　　{ 
　　public void method1() 
　　throws CheckedException 
　　{ 
　　　// ... throw new CheckedException( “...出错了“ ); 
　　} 
　　public void method2( String arg ) 
　　{ 
　　　if( arg == null ) 
　　　{ 
　　　　throw new NullPointerException( “method2的参数arg是null!” ); 
　　　} 
　　} 
　　public void method3() throws CheckedException 
　　{ 
　　　method1(); 
　　} 
　　} 
　　你可能已经注意到了，两个方法method1()和method2()都会抛出exception，可是只有method1()做了声明。另外，method3()本身并不会抛出exception，可是它却声明会抛出CheckedException。在向你解释之前，让我们先来看看这个类的main()方法: 
　　public static void main( String[] args ) 
　　{ 
　　ExceptionalClass example = new ExceptionalClass(); 
　　try 
　　{ 
　　example.method1(); 
　　example.method3(); 
　　} 
　　catch( CheckedException ex ) { } example.method2( null ); 
　　} 
　　在main()方法中，如果要调用method1()，你必须把这个调用放在try/catch程序块当中，因为它会抛出Checked exception。 
　　相比之下，当你调用method2()时，则不需要把它放在try/catch程序块当中，因为它会抛出的exception不是checked exception，而是runtime exception。会抛出runtime exception的方法在定义时不必声明它会抛出exception。 
　　现在，让我们再来看看method3()。它调用了method1()却没有把这个调用放在try/catch程序块当中。它是通过声明它会抛出method1()会抛出的exception来避免这样做的。它没有捕获这个exception，而是把它传递下去。实际上main()方法也可以这样做，通过声明它会抛出Checked exception来避免使用try/catch程序块(当然我们反对这种做法)。 
　　小结一下: 
　　* Runtime exceptions: 
　　在定义方法时不需要声明会抛出runtime exception; 
　　在调用这个方法时不需要捕获这个runtime exception; 
　　runtime exception是从java.lang.RuntimeException或java.lang.Error类衍生出来的。 
　　* Checked exceptions: 
　　定义方法时必须声明所有可能会抛出的checked exception; 
　　在调用这个方法时，必须捕获它的checked exception，不然就得把它的exception传递下去; 
　　checked exception是从java.lang.Exception类衍生出来的。 
　　二、逻辑上 
　　从逻辑的角度来说，checked exceptions和runtime exception是有不同的使用目的的。checked exception用来指示一种调用方能够直接处理的异常情况。而runtime exception则用来指示一种调用方本身无法处理或恢复的程序错误。 
　　checked exception迫使你捕获它并处理这种异常情况。以java.net.URL类的构建器(constructor)为例，它的每一个构建器都会抛出MalformedURLException。MalformedURLException就是一种checked exception。设想一下，你有一个简单的程序，用来提示用户输入一个URL，然后通过这个URL去下载一个网页。如果用户输入的URL有错误，构建器就会抛出一个exception。既然这个exception是checked exception，你的程序就可以捕获它并正确处理:比如说提示用户重新输入。 
　　再看下面这个例子: 
　　public void method() 
　　{ 
　　int [] numbers = { 1, 2, 3 }; 
　　int sum = numbers[0] numbers[3]; 
　　} 
　　在运行方法method()时会遇到ArrayIndexOutOfBoundsException(因为数组numbers的成员是从0到2)。对于这个异常，调用方无法处理/纠正。这个方法method()和上面的method2()一样，都是runtime exception的情形。上面我已经提到，runtime exception用来指示一种调用方本身无法处理/恢复的程序错误。而程序错误通常是无法在运行过程中处理的，必须改正程序代码。 
　　总而言之，在程序的运行过程中一个checked exception被抛出的时候，只有能够适当处理这个异常的调用方才应该用try/catch来捕获它。而对于runtime exception，则不应当在程序中捕获它。如果你要捕获它的话，你就会冒这样一个风险:程序代码的错误(bug)被掩盖在运行当中无法被察觉。因为在程序测试过程中，系统打印出来的调用堆栈路径(StackTrace)往往使你更快找到并修改代码中的错误。有些程序员建议捕获runtime exception并纪录在log中，我反对这样做。这样做的坏处是你必须通过浏览log来找出问题，而用来测试程序的测试系统(比如Unit
 Test)却无法直接捕获问题并报告出来。 
　　在程序中捕获runtime exception还会带来更多的问题:要捕获哪些runtime exception?什么时候捕获?runtime exception是不需要声明的，你怎样知道有没有runtime exception要捕获?你想看到在程序中每一次调用方法时，都使用try/catch程序块吗?
