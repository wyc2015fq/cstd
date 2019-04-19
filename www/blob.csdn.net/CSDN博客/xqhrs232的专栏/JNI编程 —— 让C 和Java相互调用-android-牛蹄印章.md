# JNI编程 —— 让C 和Java相互调用 - android - 牛蹄印章 - xqhrs232的专栏 - CSDN博客
2012年01月13日 15:52:02[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：907
原文地址::[http://www.360doc.com/content/11/0427/10/3700464_112638437.shtml](http://www.360doc.com/content/11/0427/10/3700464_112638437.shtml)
JNI其实是Java Native Interface的简称,也就是java本地接口。它提供了若干的API实现了和Java和其他语言的通信（主要是C&C++）。也许不少人觉 得Java已经足够强大，为什么要需要JNI这种东西呢？我们知道Java是一种平台无关性的语言，平台对于上层的java代码来说是透明的，所以在多数 时间我们是不需要JNI的，但是假如你遇到了如下的三种情况之一呢？
- 你的Java代码，需要得到一个文件的属性。但是你找遍了JDK帮助文档也找不到相关的API。 
- 在本地还有一个别的系统，不过他不是Java语言实现的，这个时候你的老板要求你把两套系统整合到一起。 
- 你的Java代码中需要用到某种算法，不过算法是用C实现并封装在动态链接库文件（DLL）当中的。
对于上述的三种情况，如果没有JNI的话，那就会变得异常棘手了。就算找到解决方案了，也是费时费力。其实说到底还是会增加开发和维护的成本。
说了那么多一通废话，现在进入正题。看过JDK源代码的人肯定会注意到在源码里有很多标记成native的方法。这些个方法只有方法签名但是没有方 法体。其实这些naive方法就是我们说的 java native interface。他提供了一个调用（invoke）的接口，然后用C或者C++去实现。我们首先来编写这个“桥梁”.我自己的开发环境是 j2sdk1.4.2_15 + eclipse 3.2 + VC++ 6.0，先在eclipse里建立一个HelloFore的Java工程，然后编写下面的代码。
Java代码
- package com.chnic.jni;  
- 
- publicclass SayHellotoCPP {  
- 
- public SayHellotoCPP(){  
-     }  
- publicnativevoid sayHello(String name);  
- }  
 一般的第一个程序总是HelloWorld。今天换换口味，把world换成一个名字。我的native本地方法有一个String的参数。会传 递一个name到后台去。本地方法已经完成，现在来介绍下javah这个方法，接下来就要用javah方法来生成一个相对应的.h头文件。
javah是一个专门为JNI生成头文件的一个命令。CMD打开控制台之后输入javah回车就能看到javah的一些参数。在这里就不多介绍 我们要用的是 -jni这个参数，这个参数也是默认的参数，他会生成一个JNI式的.h头文件。在控制台进入到工程的根目录，也就是HelloFore这个目录，然后输 入命令。
Java代码
- javah -jni com.chnic.jni.SayHellotoCPP  
命令执行完之后在工程的根目录就会发现**com_chnic_jni_SayHellotoCPP.h**这个头文件。在这里有必要多句嘴，在执行javah的时候，要输入完整的包名+类名。否则在以后的测试调用过程中会发生java.lang.UnsatisfiedLinkError这个异常。
到这里java部分算是基本完成了，接下来我们来编写后端的C++代码。（用C也可以，只不过cout比printf用起来更快些，所以这里俺偷下 懒用C++）打开VC++首先新建一个Win32 Dynamic-Link library工程，之后选择An empty DLL project空工程。在这里我C++的工程是HelloEnd，把刚刚生成的那个头文件拷贝到这个工程的根目录里。随便用什么文本编辑器打开这个头文 件，发现有一个如下的方法签名。
Cpp代码
- /*
-  * Class:     com_chnic_jni_SayHellotoCPP
-  * Method:    sayHello
-  * Signature: (Ljava/lang/String;)V
-  */
- JNIEXPORT void JNICALL Java_com_chnic_jni_SayHellotoCPP_sayHello  
-   (JNIEnv *, jobject, jstring);  
仔细观察一下这个方法，在注释上标注类名、方法名、签名（Signature），至于这个签名是做什么用的，我们以后再说。在这里最重要的是 Java_com_chnic_jni_SayHellotoCPP_sayHello这个方法签名。在Java端我们执行 sayHello(String name)这个方法之后，JVM就会帮我们唤醒在DLL里的Java_com_chnic_jni_SayHellotoCPP_sayHello这个方 法。因此我们新建一个C++ source file来实现这个方法。
Cpp代码
- #include <iostream.h>
- #include "com_chnic_jni_SayHellotoCPP.h"
- 
- 
- JNIEXPORT void JNICALL Java_com_chnic_jni_SayHellotoCPP_sayHello   
-   (JNIEnv* env, jobject obj, jstring name)  
- {  
- constchar* pname = env->GetStringUTFChars(name, NULL);  
-     cout << "Hello, " << pname << endl;  
- }  
因为我们生成的那个头文件是在C++工程的根目录不是在环境目录，所以我们要把尖括号改成单引号，至于VC++的环境目录可以在 Tools->Options->Directories里设置。F7编译工程发现缺少jni.h这个头文件。这个头文件可以 在%JAVA_HOME%/include目录下找到。把这个文件拷贝到C++工程目录，继续编译发现还是找不到。原来是因为在我们刚刚生成的那个头文件 里，jni.h这个文件是被 #include <jni.h>引用进来的，因此我们把尖括号改成双引号#include "jni.h"，继续编译发现少了jni_md.h文件，接着在%JAVA_HOME%/include/win32下面找到那个头文件，放入到工程根目
 录，F7编译成功。在Debug目录里会发现生成了HelloEnd.dll这个文件。
这个时候后端的C++代码也已经完成，接下来的任务就是怎么把他们连接在一起了，要让前端的java程序“认识并找到”这个动态链接库，就必须把这个DLL放在windows path环境变量下面。有两种方法可以做到：
- 把这个DLL放到windows下面的sysytem32文件夹下面，这个是windows默认的path 
- 复制你工程的Debug目录，我这里是C:/Program Files/Microsoft Visual Studio/MyProjects/HelloEnd/Debug这个目录，把这个目录配置到User variable的Path下面。重启eclipse，让eclipse在启动的时候重新读取这个path变量。
比较起来，第二种方法比较灵活，在开发的时候不用来回copy dll文件了，节省了很多工作量，所以在开发的时候推荐用第二种方法。在这里我们使用的也是第二种，eclipse重启之后打开 SayHellotoCPP这个类。其实我们上面做的那些是不是是让JVM能找到那些DLL文件，接下来我们要让我们自己的java代码“认识”这个动态 链接库。加入System.loadLibrary("HelloEnd");这句到静态初始化块里。
Java代码
- package com.chnic.jni;  
- 
- publicclass SayHellotoCPP {  
- 
- static{  
-         System.loadLibrary("HelloEnd");  
-     }  
- public SayHellotoCPP(){  
-     }  
- publicnativevoid sayHello(String name);  
- 
- }  
这样我们的代码就能认识并加载这个动态链接库文件了。万事俱备，只欠测试代码了，接下来编写测试代码。
Java代码
- SayHellotoCPP shp = new SayHellotoCPP();  
- shp.sayHello("World");  
我们不让他直接Hello，World。我们把World传进去，执行代码。发现控制台打印出来Hello, World这句话。就此一个最简单的JNI程序已经开发完成。也许有朋友会对CPP代码里的
Cpp代码
- constchar* pname = env->GetStringUTFChars(name, NULL);  
 这句有疑问，这个GetStringUTFChars就是JNI给developer提供的API，我们以后再讲。在这里不得不多句嘴。
- 因为JNI有一个Native这个特点，一点有项目用了JNI，也就说明这个项目基本不能跨平台了。 
- JNI调用是相当慢的，在实际使用的之前一定要先想明白是否有这个必要。 
- 因为C++和C这样的语言非常灵活，一不小心就容易出错，比如我刚刚的代码就没有写析构字符串释放内存，对于java developer来说因为有了GC 垃圾回收机制，所以大多数人没有写析构函数这样的概念。所以JNI也会增加程序中的风险，增大程序的不稳定性。
其实在Java代码中，除了对本地方法标注native关键字和加上要加载动态链接库之外，JNI基本上是对上层coder透明的，上层coder调用那些本地方法的时候并不知道这个方法的方法体究竟是在哪里，这个道理就像我们用JDK所提供的API一样。所以在Java中使用JNI还是很简单的，相比之下在C++中调用java，就比前者要复杂的多了。
现在来介绍下JNI里的数据类型。在C++里，编译器会很据所处的平台来为一些基本的 数据类型来分配长度，因此也就造成了平台不一致性，而这个问题在Java中则不存在，因为有JVM的缘故，所以Java中的基本数据类型在所有平台下得到 的都是相同的长度，比如int的宽度永远都是32位。基于这方面的原因，java和c++的基本数据类型就需要实现一些mapping，保持一致性。下面 的表可以概括：
|Java类型|本地类型|JNI中定义的别名|
|----|----|----|
|int|long|jint|
|long|_int64|jlong|
|byte|signed char|jbyte|
|boolean|unsigned char|jboolean|
|char|unsigned short|jchar|
|short|short|jshort|
|float|float|jfloat|
|double|double|jdouble|
|Object|_jobject*|jobject|
上面的表格是我在网上搜的，放上来给大家对比一下。对于每一种映射的数据类型，JNI的设计者其实已经帮我们取好了相应的别名以方便记忆。如果想了解一些更加细致的信息，可以去看一些jni.h这个头文件，各种数据类型的定义以及别名就被定义在这个文件中。
了解了JNI中的数据类型，下面就来看这次的例子。这次我们用Java来实现一个前端的market（以下就用Foreground代替）用CPP来实现一个后端factory（以下用backend代替）。我们首先还是来编写包含本地方法的java类。
Java代码
- package com.chnic.service;  
- 
- import com.chnic.bean.Order;  
- 
- publicclass Business {  
- static{  
-         System.loadLibrary("FruitFactory");  
-     }  
- 
- public Business(){  
- 
-     }  
- 
- publicnativedouble getPrice(String name);  
- publicnative Order getOrder(String name, int amount);  
- publicnative Order getRamdomOrder();  
- publicnativevoid analyzeOrder(Order order);  
- 
- publicvoid notification(){  
-         System.out.println("Got a notification.");  
-     }  
- 
- publicstaticvoid notificationByStatic(){  
-         System.out.println("Got a notification in a static method.");  
-     }  
- }  
这个类里面包含4个本地方法，一个静态初始化块加载将要生成的dll文件。剩下的方法都是很普通的java方法，等会在backend中回调这些方法。这个类需要一个名为Order的JavaBean。
Java代码
- package com.chnic.bean;  
- 
- publicclass Order {  
- 
- private String name = "Fruit";  
- privatedouble price;  
- privateint amount = 30;  
- 
- public Order(){  
- 
-     }  
- 
- publicint getAmount() {  
- return amount;  
-     }  
- 
- publicvoid setAmount(int amount) {  
- this.amount = amount;  
-     }  
- 
- public String getName() {  
- return name;  
-     }  
- 
- publicvoid setName(String name) {  
- this.name = name;  
-     }  
- 
- publicdouble getPrice() {  
- return price;  
-     }  
- 
- publicvoid setPrice(double price) {  
- this.price = price;  
-     }  
- }  
JavaBean中，我们为两个私有属性赋值，方便后面的例子演示。到此为止除了测试代码之外的Java端的代码就全部高调了，接下来进行生成.h 头文件、建立C++工程的工作，在这里就一笔带过，不熟悉的朋友请回头看第一篇。在工程里我们新建一个名为Foctory的C++ source file 文件，去实现那些native方法。具体的代码如下。
Cpp代码
- #include <iostream.h>
- #include <string.h>
- #include "com_chnic_service_Business.h"
- 
- jobject getInstance(JNIEnv* env, jclass obj_class);  
- 
- JNIEXPORT jdouble JNICALL Java_com_chnic_service_Business_getPrice(JNIEnv* env,   
-                                                                    jobject obj,   
-                                                                    jstring name)  
- {  
- constchar* pname = env->GetStringUTFChars(name, NULL);  
-     cout << "Before release: "  << pname << endl;  
- 
- if (strcmp(pname, "Apple") == 0)  
-     {  
-         env->ReleaseStringUTFChars(name, pname);  
-         cout << "After release: " << pname << endl;  
- return 1.2;  
-     }   
- else
-     {  
-         env->ReleaseStringUTFChars(name, pname);  
-         cout << "After release: " << pname << endl;  
- return 2.1;  
-     }     
- }  
- 
- 
- JNIEXPORT jobject JNICALL Java_com_chnic_service_Business_getOrder(JNIEnv* env,   
-                                                                    jobject obj,   
-                                                                    jstring name,   
-                                                                    jint amount)  
- {  
-     jclass order_class = env->FindClass("com/chnic/bean/Order");  
-     jobject order = getInstance(env, order_class);  
- 
-     jmethodID setName_method = env->GetMethodID(order_class, "setName", "(Ljava/lang/String;)V");  
-     env->CallVoidMethod(order, setName_method, name);  
- 
-     jmethodID setAmount_method = env->GetMethodID(order_class, "setAmount", "(I)V");  
-     env->CallVoidMethod(order, setAmount_method, amount);  
- 
- return order;  
- }  
- 
- JNIEXPORT jobject JNICALL Java_com_chnic_service_Business_getRamdomOrder(JNIEnv* env,   
-                                                                          jobject obj)  
- {  
-     jclass business_class = env->GetObjectClass(obj);  
-     jobject business_obj = getInstance(env, business_class);  
- 
-     jmethodID notification_method = env->GetMethodID(business_class, "notification", "()V");  
-     env->CallVoidMethod(obj, notification_method);  
- 
-     jclass order_class = env->FindClass("com/chnic/bean/Order");  
-     jobject order = getInstance(env, order_class);  
-     jfieldID amount_field = env->GetFieldID(order_class, "amount", "I");  
-     jint amount = env->GetIntField(order, amount_field);  
-     cout << "amount: " << amount << endl;  
- return order;  
- }  
- 
- 
- JNIEXPORT void JNICALL Java_com_chnic_service_Business_analyzeOrder (JNIEnv* env,   
-                                                                      jclass cls,   
-                                                                      jobject obj)  
- {  
-     jclass order_class = env->GetObjectClass(obj);  
-     jmethodID getName_method = env->GetMethodID(order_class, "getName", "()Ljava/lang/String;");  
-     jstring name_str = static_cast<jstring>(env->CallObjectMethod(obj, getName_method));  
- constchar* pname = env->GetStringUTFChars(name_str, NULL);  
- 
-     cout << "Name in Java_com_chnic_service_Business_analyzeOrder: " << pname << endl;  
-     jmethodID notification_method_static = env->GetStaticMethodID(cls, "notificationByStatic", "()V");  
-     env->CallStaticVoidMethod(cls, notification_method_static);  
- 
- }  
- 
- jobject getInstance(JNIEnv* env, jclass obj_class)  
- {  
-     jmethodID construction_id = env->GetMethodID(obj_class, "<init>", "()V");  
-     jobject obj = env->NewObject(obj_class, construction_id);  
- return obj;  
- }  
可以看到，在我Java中的四个本地方法在这里全部被实现，接下来针对这四个方法来解释下，一些JNI相关的API的使用方法。先从第一个方法讲起吧：
**1.getPrice(String name)**
这个方法是从foreground传递一个类型为string的参数到backend，然后backend判断返回相应的价格。在cpp的代码中， 我们用GetStringUTFChars这个方法来把传来的jstring变成一个UTF-8编码的char型字符串。因为jstring的实际类型是 jobject，所以无法直接比较。
GetStringUTFChars方法包含两个参数，第一参数是你要处理的jstring对象，第二个参数是否需要在内存中生成一个副本对象。将 jstring转换成为了一个const char*了之后，我们用string.h中带strcmp函数来比较这两个字符串，如果传来的字符串是“Apple”的话我们返回1.2。反之返回 2.1。在这里还要多说一下ReleaseStringUTFChars这个函数，这个函数从字面上不难理解，就是释放内存用的。有点像cpp里的析构函 数，只不过Sun帮我们已经封装好了。由于在JVM中有GC这个东东，所以多数java
 coder并没有写析构的习惯，不过在JNI里是必须的了，否则容易造成内存泄露。我们在这里在release之前和之后分别打出这个字符串来看一下效果。
粗略的解释完一些API之后，我们编写测试代码。
Java代码
- Business b = new Business();          
- System.out.println(b.getPrice("Apple"));  
运行这段测试代码，控制台上打出
Before release: Apple
After release: ��
1.2
在release之前打印出来的是我们“需要”的Apple，release之后就成了乱码了。由于传递的是Apple，所以得到1.2。测试成功。
**2. getOrder(String name, int amount)**
在foreground中可以通过这个方法让backend返回一个你“指定”的Order。所谓“指定”，其实也就是指方法里的两个参数：name和amout，在cpp的代码在中，会根据传递的两个参数来构造一个Order。回到cpp的代码里。
Java代码
- jclass order_class = env->FindClass("com/chnic/bean/Order");  
是不是觉得这句代码似曾相识？没错，这句代码很像我们java里写的Class.forName(className)反射的代码。其实在这里 FindClass的作用和上面的forName是类似的。只不过在forName中要用完整的类名，但是在这里必须用"/"来代替“.”。这个方法会返 回一个jclass的对象，其实也就是我们在Java中说的类对象。
Java代码
- jmethodID construction_id = env->GetMethodID(obj_class, "<init>", "()V");  
- jobject obj = env->NewObject(obj_class, construction_id);  
拿到"类对象"了之后，按照Java RTTI的逻辑我们接下来就要唤醒那个类对象的构造函数了。在JNI中，包括构造函数在内的所有方法都被看成Method。每个method都有一个特定的ID，我们通过GetMethodID这个方法就可以拿到我们想要的某一个java 方法的ID。GetMethodID需要传三个参数，第一个是很显然jclass，第二个参数是java方法名，也就是你想取的method
 ID的那个方法的方法名（有些绕口![](http://chnic.javaeye.com/images/smiles/icon_confused.gif)），第三个参数是方法签名。
 在这里有必要单独来讲一讲这个方法签名，为什么要用这个东东呢？我们知道，在Java里方法是可以被重载的，比如我一个类里有public void a(int arg)和public void a(String arg)这两个方法，在这里用方法名来区分方法显然就是行不通的了。方法签名包括两部分：参数类型和返回值类型；具体的格式：(参数1类型签名 参数2类型签名)返回值类型签名。下面是java类型和年名类型的对照的一个表
|Java类型|对应的签名|
|----|----|
|boolean|Z|
|byte|B|
|char|C|
|shrot|S|
|int|I|
|long|L|
|float|F|
|double|D|
|void|V|
|Object|L用/分割包的完整类名;  Ljava/lang/String;|
|Array|[签名       [I       [Ljava/lang/String;|
其实除了自己对照手写之外，JDK也提供了一个很好用的生成签名的工具javap,cmd进入控制台到你要生成签名的那个类的目录下。在这里用 Order类打比方，敲入： javap -s -private Order。 所有方法签名都会被输出，关于javap的一些参数可以在控制台下面输入 javap -help查看。（做coder的 毕竟还是要认几个单词的）
啰嗦了一大堆，还是回到我们刚刚的getMethodID这个方法上。因为是调用构造函数，JNI规定调用构造函数的时候传递的方法名应该为<init>,通过javap查看 我们要的那个无参的构造函数的方法签是()V。得到方法签名，最后我们调用NewObject方法来生成一个新的对象。
拿到了对象，之后我们开始为对象jobject填充数值，还是首先拿到setXXX方法的Method ID，之后调用Call<Type>Method来调用java方法。这里的<Type>所指的是方法的返回类型，我们刚刚调用 的是set方法的返回值是void，因此这里的方法也就是CallVoidMethod，这个方法的参数除了前两个要传入jobject和 jmethodID之外还要传入要调用的那个方法的参数，而且要顺序必须一致，这点和Java的反射一模一样，在这里就不多解释。（看到这一步是不是对 java 反射又有了自己新的理解？）
终于介绍完了第二个方法，下来就是测试代码测试。
Java代码
- Order o = b.getOrder("Watermelom", 100);  
- System.out.println("java: " + o.getName());  
- System.out.println("java: " + o.getAmount());  
控制台打出
java: Watermelom
java: 100
就此，我们完成了第二个方法的测试。
**3.getRamdomOrder()**
这个方法会从backend得到一个随机的Order对象（抱歉这里“Random”拼错了），然后再调用java中相应的通知方法来通知 foreground。getRamdomOrder方法没有参数，但是所对应的C++方法里却有两个参数，一定有人会不解。其实细心的朋友一定会发 现，JNI里所有对应Java方法的C++ 方法都会比Java方法多两个参数，第一个参数是我们很熟悉的JNIEnv*指针，第二个参数有时是jobject有时是个jclass。针对这第二个参 数在这里有必要多废话两句。
其实第二个参数传递的是包含了native本地方法的对象或者类对象，我们知道非静态的方法是属于某一个对象的，而静态方法是属于类对象的，所以静 态方法可以被所有对象共享。有这个对象/类对象，我们就可以很方便的操作包含了native方法的对象的一些函数了。（这句话有点绕口，没看明白的建议多 读两遍）。
废话完了言归正传，因为getRamdomOrder不是静态的，所以C++相对应的参数中传递来的是一个jobject对象。
Cpp代码
- jclass business_class = env->GetObjectClass(obj);  
这一句不难理解，GetObjectClass方法可以得到一个对象的类对象，这句有点像Java中的Object.class。不熟悉的朋友建议 再去看一下Java反射机制。接下来的几句C++代码应该在之前的方法1和方法2中都解释过。早backend端会发一个“消息”给 foreground，之后new一个新的Order类出来。接下来的三句有必要再废话一下。
Cpp代码
- jfieldID amount_field = env->GetFieldID(order_class, "amount", "I");  
- jint amount = env->GetIntField(order, amount_field);  
- cout << "amount: " << amount << endl;  
之前我为Order这个Javabean的amount的属性设置了一个初始值为30，其实就是为了在这里演示如何在C++中拿一个Java对象的 属性，拿的方法和我们之前说过的调用Java方法的程序差不多，也要先拿到一个jfieldID，之后调用Get<type>Field方法 来取得某一个对象中的某一个属性的数值，最后cout把他打印出来。我们编写测试代码来看一下最终效果。
Java代码
- Business b = new Business();                   
- Order o2 = b.getRamdomOrder();  
- System.out.println(o2.getName());  
运行上述的测试代码之后，控制台上打出了
Got a notification.
amount: 30
Fruit
和我们想要的结果是一样的，测试成功。
**4.analyzeOrder(Order order)**
这是一个静态方法，foreground会通过这个方法传一个Order的对象到backend去，然后再由CPP端进行“analyze”。在这 里我们取出来传递过来的Order对象的name属性，然后打印到控制台上。因为这个方法是静态static方法，所以相对应的C++方法中的第二个参数 也变成了jclass对象，也就是Business.class这个类对象。第三个参数是一个jobject对象，很明显就是我们传递过来的order对 象。
前5句代码应该不难理解，就是调用getName这个方法，然后打印出来。因为JNI的API中并没有提供CallStringMethod这个方 法，所以我们用CallObjectMethod这个方法来取得name这个字符串（String很明显也是一个Object），然后再转型成为 jstring。也就是下面这句代码。
Cpp代码
- jstring name_str = static_cast<jstring>(env->CallObjectMethod(obj, getName_method));  
取到了name这个字符串之后cout打印出来，之后调用Business这个类对象中的静态方法notificationByStatic来通知 foreground。调用的流程以及方法和非静态都是一样的，只不过注意JNI中调用静态方法的API所传递的一个参数是一个jclass而非 jobject（这个也不难理解，因为静态方法是属于class类对象的）
还是编写测试代码测试这个方法
Java代码
- Business b = new Business();          
- Order o = b.getOrder("Watermelom", 100);  
- Business.analyzeOrder(o);  
控制台上打印出
Name in Java_com_chnic_service_Business_analyzeOrder: Watermelom
Got a notification in a static method.
第一句是C++中cout打印出来的，第二句则是Java中的静态方法打印出来的，和我们想要的结果是一致的。
呼~好不容易介绍完了4个方法，最后总结一下吧。
- JNI中所提供的API远远不止这4个方法中所使用的API。上面介绍的都是比较常用的，本人也不可能罗列出所有的API。 
- 了解了JNI编程更加有利于深入了解Java中的反射机制，反之亦然。
因此如果有对JNI编程有兴趣或者有更深入的需要，可以参考一下sun的相关文档。在这里上传sun提供的JNI的API手册，还有上面例子中所用的演示代码给大家参考。
- [Backend.zip](http://dl.javaeye.com/topics/download/3e3620e8-90ff-3c5c-99a8-edc9d8b6ea6c)(10.2 KB)
- 描述: CPP代码 
- 下载次数: 43
- [Foreground.zip](http://dl.javaeye.com/topics/download/20289b84-aa2a-3b1e-a9f5-32f32446f08f)(3.8 KB)
- 描述: Java代码 
- 下载次数: 40
- [JNI_API.zip](http://dl.javaeye.com/topics/download/47ee625b-97d2-3b62-95d8-137a70479a2a)(38.9 KB)
- 描述: JNI的API参考手册 
- 下载次数: 65
