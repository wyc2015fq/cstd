# 深入Java核心 Java内存分配原理精讲 - 博客堂 - CSDN博客





2016年04月02日 14:47:08[最小森林](https://me.csdn.net/u012052268)阅读数：280









Java内存分配与管理是Java的核心技术之一，之前我们曾介绍过[Java的内存管理与内存泄露](http://developer.51cto.com/art/200906/129353.htm)以及[Java垃圾回收](http://developer.51cto.com/art/200607/29278.htm)方面的知识，今天我们再次深入Java核心，详细介绍一下Java在内存分配方面的知识。一般Java在内存分配时会涉及到以下区域：


◆寄存器：我们在程序中无法控制


◆栈：存放基本类型的数据和对象的引用，但对象本身不存放在栈中，而是存放在堆中


◆堆：存放用new产生的数据


◆静态域：存放在对象中用static定义的静态成员


◆常量池：存放常量


◆非RAM存储：硬盘等永久存储空间

Java内存分配中的栈


在函数中定义的一些基本类型的变量数据和对象的引用变量都在函数的栈内存中分配。


当在一段代码块定义一个变量时，Java就在栈中 为这个变量分配内存空间，当该变量退出该作用域后，Java会自动释放掉为该变量所分配的内存空间，该内存空间可以立即被另作他用。

Java内存分配中的堆


堆内存用来存放由new创建的对象和数组。 在堆中分配的内存，由Java虚拟机的自动垃圾回收器来管理。


在堆中产生了一个数组或对象后，还可以 在栈中定义一个特殊的变量，让栈中这个变量的取值等于数组或对象在堆内存中的首地址，栈中的这个变量就成了数组或对象的引用变量。  引用变量就相当于是 为数组或对象起的一个名称，以后就可以在程序中使用栈中的引用变量来访问堆中的数组或对象。引用变量就相当于是为数组或者对象起的一个名称。


引用变量是普通的变量，定义时在栈中分配，引用变量在程序运行到其作用域之外后被释放。而数组和对象本身在堆中分配，即使程序 运行到使用 new 产生数组或者对象的语句所在的代码块之外，数组和对象本身占据的内存不会被释放，数组和对象在没有引用变量指向它的时候，才变为垃圾，不能在被使用，但仍 然占据内存空间不放，在随后的一个不确定的时间被垃圾回收器收走（释放掉）。这也是 Java 比较占内存的原因。

实际上，栈中的变量指向堆内存中的变量，这就是Java中的指针！

常量池 (constant pool)


常量池指的是在编译期被确定，并被保存在已编译的.class文件中的一些数据。除了包含代码中所定义的各种基本类型（如int、long等等）和对象型（如String及数组）的常量值(final)还包含一些以文本形式出现的符号引用，比如：


◆类和接口的全限定名；


◆字段的名称和描述符；


◆方法和名称和描述符。


虚拟机必须为每个被装载的类型维护一个常量池。常量池就是该类型所用到常量的一个有序集和，包括直接常量（string,integer和 floating point常量）和对其他类型，字段和方法的符号引用。


对于String常量，它的值是在常量池中的。而JVM中的常量池在内存当中是以表的形式存在的， 对于String类型，有一张固定长度的CONSTANT_String_info表用来存储文字字符串值，注意：该表只存储文字字符串值，不存储符号引 用。说到这里，对常量池中的字符串值的存储位置应该有一个比较明了的理解了。

在程序执行的时候,常量池 会储存在Method Area,而不是堆中。

堆与栈


Java的堆是一个运行时数据区,类的(对象从中分配空间。这些对象通过new、newarray、 anewarray和multianewarray等指令建立，它们不需要程序代码来显式的释放。堆是由垃圾回收来负责的，堆的优势是可以动态地分配内存 大小，生存期也不必事先告诉编译器，因为它是在运行时动态分配内存的，Java的垃圾收集器会自动收走这些不再使用的数据。但缺点是，由于要在运行时动态 分配内存，存取速度较慢。


栈的优势是，存取速度比堆要快，仅次于寄存器，栈数据可以共享。但缺点是，存在栈中的数据大小与生存期必须是 确定的，缺乏灵活性。栈中主要存放一些基本类型的变量数据（int, short, long, byte, float, double, boolean, char）和对象句柄(引用)。


栈有一个很重要的特殊性，就是存在栈中的数据可以共享。假设我们同时定义：


- int a = 3; 
  
- int b = 3；  




编译器先处理int a = 3；首先它会在栈中创建一个变量为a的引用，然后查找栈中是否有3这个值，如果没找到，就将3存放进来，然后将a指向3。接着处理int b = 3；在创建完b的引用变量后，因为在栈中已经有3这个值，便将b直接指向3。这样，就出现了a与b同时均指向3的情况。


这时，如果再令 a=4；那么编译器会重新搜索栈中是否有4值，如果没有，则将4存放进来，并令a指向4；如果已经有了，则直接将a指向这个地址。因此a值的改变不会影响 到b的值。


要注意这种数据的共享与两个对象的引用同时指向一个对象的这种共享是不同的，因为这种情况a的修改并不会影响到b, 它是由编译器完成的，它有利于节省空间。而一个对象引用变量修改了这个对象的内部状态，会影响到另一个对象引用变量。


String是一个特殊的包装类数据。可以用：
- String str = new String("abc");   
- String str = "abc";  


两种的形式来创建，第一种是用new()来新建对象的，它会在存放于堆中。每调用一次就会创建一个新的对象。而第二种是先在栈中创建一个对String类的对象引用变量str，然后通过符号引用去字符串常量池 里找有没有"abc",如果没有，则将"abc"存放进字符串常量池 ，并令str指向”abc”，如果已经有”abc” 则直接令str指向“abc”。


比较类里面的数值是否相等时，用equals()方法；当测试两个包装类的引用是否指向同一个对象时，用==，下面用例子说明上面的理论。
- String str1 = "abc";   
- String str2 = "abc";   
- System.out.println(str1==str2); //true 


可以看出str1和str2是指向同一个对象的。
- String str1 =new String ("abc");   
- String str2 =new String ("abc");   
- System.out.println(str1==str2); // false 


用new的方式是生成不同的对象。每一次生成一个。


因此用第二种方式创建多个”abc”字符串,在内存中 其实只存在一个对象而已. 这种写法有利与节省内存空间. 同时它可以在一定程度上提高程序的运行速度，因为JVM会自动根据栈中数据的实际情况来决定是否有必要创建新对象。而对于String str = new String("abc")；的代码，则一概在堆中创建新对象，而不管其字符串值是否相等，是否有必要创建新对象，从而加重了程序的负担。


另 一方面, 要注意: 我们在使用诸如String str = "abc"；的格式定义类时，总是想当然地认为，创建了String类的对象str。担心陷阱！对象可能并没有被创建！而可能只是指向一个先前已经创建的 对象。只有通过new()方法才能保证每次都创建一个新的对象。


由于String类的immutable性质，当String变量需要经常变换 其值时，应该考虑使用StringBuffer类，以提高程序效率。


1. 首先String不属于8种基本数据类型，String是一个对象。因为对象的默认值是null，所以String的默认值也是null；但它又是一种特殊的对象，有其它对象没有的一些特性。


2. new String()和new String(”")都是申明一个新的空字符串，是空串不是null；


3. String str=”kvill”；String str=new String (”kvill”)的区别


示例：
- String s0="kvill";   
- String s1="kvill";   
- String s2="kv" + "ill";   
- System.out.println( s0==s1 );   
- System.out.println( s0==s2 );  


结果为：


true 

true


首先，我们要知结果为道Java 会确保一个字符串常量只有一个拷贝。


因为例子中的 s0和s1中的”kvill”都是字符串常量，它们在编译期就被确定了，所以s0==s1为true；而”kv”和”ill”也都是字符串常量，当一个字 符串由多个字符串常量连接而成时，它自己肯定也是字符串常量，所以s2也同样在编译期就被解析为一个字符串常量，所以s2也是常量池中” kvill”的一个引用。所以我们得出s0==s1==s2；用new String() 创建的字符串不是常量，不能在编译期就确定，所以new String() 创建的字符串不放入常量池中，它们有自己的地址空间。


示例：
- String s0="kvill";   
- String s1=new String("kvill");   
- String s2="kv" + new String("ill");   
- System.out.println( s0==s1 );   
- System.out.println( s0==s2 );   
- System.out.println( s1==s2 );  


结果为：


false 

false 

false


例2中s0还是常量池 中"kvill”的应用，s1因为无法在编译期确定，所以是运行时创建的新对象”kvill”的引用，s2因为有后半部分 new String(”ill”)所以也无法在编译期确定，所以也是一个新创建对象”kvill”的应用;明白了这些也就知道为何得出此结果了。


4. String.intern()：


再补充介绍一点：存在于.class文件中的常量池，在运行期被JVM装载，并且可以扩充。String的 intern()方法就是扩充常量池的 一个方法；当一个String实例str调用intern()方法时，Java 查找常量池中 是否有相同Unicode的字符串常量，如果有，则返回其的引用，如果没有，则在常 量池中增加一个Unicode等于str的字符串并返回它的引用；看示例就清楚了


示例：
- String s0= "kvill";   
- String s1=new String("kvill");   
- String s2=new String("kvill");   
- System.out.println( s0==s1 );   
- System.out.println( "**********" );   
- s1.intern();   
- s2=s2.intern(); //把常量池中"kvill"的引用赋给s2  
- System.out.println( s0==s1);   
- System.out.println( s0==s1.intern() );   
- System.out.println( s0==s2 );  


结果为：


false 

false //虽然执行了s1.intern(),但它的返回值没有赋给s1 

true //说明s1.intern()返回的是常量池中"kvill"的引用 

true


最后我再破除一个错误的理解：有人说，“使用 String.intern() 方法则可以将一个 String 类的保存到一个全局 String 表中 ，如果具有相同值的 Unicode 字符串已经在这个表中，那么该方法返回表中已有字符串的地址，如果在表中没有相同值的字符串，则将自己的地址注册到表中”如果我把他说的这个全局的 String 表理解为常量池的话，他的最后一句话，”如果在表中没有相同值的字符串，则将自己的地址注册到表中”是错的：


示例：
- String s1=new String("kvill");   
- String s2=s1.intern();   
- System.out.println( s1==s1.intern() );   
- System.out.println( s1+" "+s2 );   
- System.out.println( s2==s1.intern() );  


结果：


false 

kvill kvill 

true


在这个类中我们没有声名一个”kvill”常量，所以常量池中一开始是没有”kvill”的，当我们调用s1.intern()后就在常量池中新添加了一 个”kvill”常量，原来的不在常量池中的”kvill”仍然存在，也就不是“将自己的地址注册到常量池中”了。


s1==s1.intern() 为false说明原来的”kvill”仍然存在；s2现在为常量池中”kvill”的地址，所以有s2==s1.intern()为true。


5. 关于equals()和==:


这个对于String简单来说就是比较两字符串的Unicode序列是否相当，如果相等返回true;而==是 比较两字符串的地址是否相同，也就是是否是同一个字符串的引用。


6. 关于String是不可变的


这一说又要说很多，大家只 要知道String的实例一旦生成就不会再改变了，比如说：String str=”kv”+”ill”+” “+”ans”; 就是有4个字符串常量，首先”kv”和”ill”生成了”kvill”存在内存中，然后”kvill”又和” ” 生成 “kvill “存在内存中，最后又和生成了”kvill ans”;并把这个字符串的地址赋给了str,就是因为String的”不可变”产生了很多临时变量，这也就是为什么建议用StringBuffer的原 因了，因为StringBuffer是可改变的。


下面是一些String相关的常见问题：


String中的final用法和理解


final StringBuffer a = new StringBuffer("111");

final StringBuffer b = new StringBuffer("222");

a=b;//此句编译不通过

final StringBuffer a = new StringBuffer("111");

a.append("222");// 编译通过


可见，final只对引用的"值"(即内存地址)有效，它迫使引用只能指向初始指向的那个对象，改变它的指向会导致编译期错误。至于它所指向的对象 的变化，final是不负责的。


String常量池问题的几个例子


下面是几个常见例子的比较分析和理解：
- String a = "a1";   
- String b = "a" + 1;   
- System.out.println((a == b)); //result = true 
- String a = "atrue";   
- String b = "a" + "true";   
- System.out.println((a == b)); //result = true 
- String a = "a3.4";   
- String b = "a" + 3.4;   
- System.out.println((a == b)); //result = true


分析：JVM对于字符串常量的"+"号连接，将程序编译期，JVM就将常量字符串的"+"连接优化为连接后的值，拿"a" + 1来说，经编译器优化后在class中就已经是a1。在编译期其字符串常量的值就确定下来，故上面程序最终的结果都为true。
- String a = "ab";   
- String bb = "b";   
- String b = "a" + bb;   
- System.out.println((a == b)); //result = false


分析：JVM对于字符串引用，由于在字符串的"+"连接中，有字符串引用存在，而引用的值在程序编译期是无法确定的，即"a" + bb无法被编译器优化，只有在程序运行期来动态分配并将连接后的新地址赋给b。所以上面程序的结果也就为false。
- String a = "ab";   
- final String bb = "b";   
- String b = "a" + bb;   
- System.out.println((a == b)); //result = true


分析：和[3]中唯一不同的是bb字符串加了final修饰，对于final修饰的变量，它在编译时被解析为常量值的一个本地拷贝存储到自己的常量 池中或嵌入到它的字节码流中。所以此时的"a" + bb和"a" + "b"效果是一样的。故上面程序的结果为true。
- String a = "ab";   
- final String bb = getBB();   
- String b = "a" + bb;   
- System.out.println((a == b)); //result = false  
- privatestatic String getBB() {  
- return"b";   
- } 


分析：JVM对于字符串引用bb，它的值在编译期无法确定，只有在程序运行期调用方法后，将方法的返回值和"a"来动态连接并分配地址为b，故上面 程序的结果为false。


通过上面4个例子可以得出得知：


String  s  =  "a" + "b" + "c"; 


就等价于String s = "abc";  


String  a  =  "a";   

String  b  =  "b";   

String  c  =  "c";   

String  s  =   a  +  b  +  c; 


这个就不一样了，最终结果等于： 

- StringBuffer temp = new StringBuffer();     
- temp.append(a).append(b).append(c);     
- String s = temp.toString(); 


由上面的分析结果，可就不难推断出String 采用连接运算符（+）效率低下原因分析，形如这样的代码：
- publicclass Test {  
- publicstaticvoid main(String args[]) {  
- String s = null;  
- for(int i = 0; i < 100; i++) {  
- s += "a";  
- }  
- }  
- } 


每做一次 + 就产生个StringBuilder对象，然后append后就扔掉。下次循环再到达时重新产生个StringBuilder对象，然后 append 字符串，如此循环直至结束。如果我们直接采用 StringBuilder 对象进行 append 的话，我们可以节省 N - 1 次创建和销毁对象的时间。所以对于在循环中要进行字符串连接的应用，一般都是用StringBuffer或StringBulider对象来进行 append操作。


String对象的intern方法理解和分析：
- publicclass Test4 {  
- privatestatic String a = "ab";   
- publicstaticvoid main(String[] args){  
- String s1 = "a";  
- String s2 = "b";  
- String s = s1 + s2;  
- System.out.println(s == a);//false 
- System.out.println(s.intern() == a);//true   
- }  
- } 


这里用到Java里面是一个常量池的问题。对于s1+s2操作，其实是在堆里面重新创建了一个新的对象,s保存的是这个新对象在堆空间的的内容，所 以s与a的值是不相等的。而当调用s.intern()方法，却可以返回s在常量池中的地址值，因为a的值存储在常量池中，故s.intern和a的值相等。

总结


栈中用来存放一些原始数据类型的局部变量数据和对象的引用(String,数组.对象等等)但不存放对象内容


堆中存放使用new关键字创建的对象.


字符串是一个特殊包装类,其引用是存放在栈里的,而对象内容必须根据创建方式不同定(常量池和堆).有的是编译期就已经创建好，存放在字符串常 量池中，而有的是运行时才被创建.使用new关键字，存放在堆中。




