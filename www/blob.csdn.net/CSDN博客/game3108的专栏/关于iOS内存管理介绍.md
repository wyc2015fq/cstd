# 关于iOS内存管理介绍 - game3108的专栏 - CSDN博客
2015年04月17日 14:39:17[game3108](https://me.csdn.net/game3108)阅读数：590
个人分类：[转载																[iOS](https://blog.csdn.net/game3108/article/category/2844789)](https://blog.csdn.net/game3108/article/category/2926393)

[http://www.cnblogs.com/tangbinblog/archive/2012/12/31/2840690.html](http://www.cnblogs.com/tangbinblog/archive/2012/12/31/2840690.html)**ARC ****和**** retain cycle**
还有之前总结的一片文章
**1****概要**
由于在测试过程中手机的测试项目越来越多，作者要学习一下IOS环境手机开发的平台的一些相关知识，同时也需要对测试项目做一些代码走读工作。本文是从白盒代码检测的角度介绍一下IOS平台内存检查的方法。并举工作中实际应用的实例来介绍如下。相关原理介绍部分的内容可以在官网上查找。
**2****IOS****内存原理介绍**
iOS平台的内存使用引用计数的机制，并且引入了半自动释放机制；这种使用上的多样性，导致开发者在内存使用上非常容易出现内存泄漏和内存莫名的增长情况；下面会介绍iOS平台的内存使用原则与使用陷阱；深度剖析autorelease机制；低内存报警后的处理流程；并结合自身实例介绍内存暴增的问题追查记录以及相关工具的使用情况；
[http://www.weste.net/2011/12-3/78383.html](http://www.weste.net/2011/12-3/78383.html)
2.1 IOS平台内存管理介绍
   iOS平台的内存管理采用引用计数的机制；当创建一个对象时使用alloc或者allWithZone方法时，引用计数就会+1；当释放对象使用release方法时，引用计数就是-1；这就意味着每一个对象都会跟踪有多少其他对象引用它，一旦引用计数为0，该对象的内存就会被释放掉；另外，iOS也提供了一种延时释放的机制AutoRelease，以这种方式申请的内存，开发者无需手动释放，系统会在某一时机释放该内存；由于iOS平台的这种内存管理的多样性，导致开发者在内存使用上很容易出现内存泄漏或者程序莫名崩溃的情况；
下图是内存从申请到释放的一个完整示例：
![](https://partner.outlook.cn/owa/service.svc/s/GetFileAttachment?id=AAMkAGM4ODIyYTRhLTBiMjgtNDUzYy04OTE3LTQ0NmMyNzA5NTA4MgBGAAAAAACta1CHqa5IQbrw4%2FmjvMNQBwDNMi%2BLWfmtQ4yfkI7LtrwIAAAAAAEMAADNMi%2BLWfmtQ4yfkI7LtrwIAAAzKxa%2BAAABEgAQADZiKav6U2FFsoQHarul%2Bfg%3D&X-OWA-CANARY=QMGUaDwG4USXnnyLQDn6CVB_22rwRtIIUqxcE_oB1-QZaG-5YmL1D5TMJGeCj4Ccbadh06VWGMk.)
![](https://partner.outlook.cn/owa/service.svc/s/GetFileAttachment?id=AAMkAGM4ODIyYTRhLTBiMjgtNDUzYy04OTE3LTQ0NmMyNzA5NTA4MgBGAAAAAACta1CHqa5IQbrw4%2FmjvMNQBwDNMi%2BLWfmtQ4yfkI7LtrwIAAAAAAEMAADNMi%2BLWfmtQ4yfkI7LtrwIAAAzKxa%2BAAABEgAQAJtet5BsbYBIki2vA2Augm4%3D&X-OWA-CANARY=QMGUaDwG4USXnnyLQDn6CVB_22rwRtIIUqxcE_oB1-QZaG-5YmL1D5TMJGeCj4Ccbadh06VWGMk.)
2.2 IOS平台内存使用原则
2.2.1 对象的所有权与销毁
（1）谁创建，谁释放；
如果是以alloc，new或者copy，mutableCopy创建的对象，则必须调用release或者autorelease方法释放内存；
如果没有释放，则导致内存泄漏！
（2）谁retain，谁释放；
如果对一个对象发送retain消息，其引用计数会+1，则使用完必须发送release或者autorelease方法释放内存或恢复引用计数；
如果没有释放，则导致内存泄漏！
（3）没创建且没retain，别释放；
不要释放那些不是自己alloc或者retain的对象，否则程序会crash；
不要释放autorelease的对象，否则程序会crash；
2.2.2 对象的深拷贝与浅拷贝
一般来说，复制一个对象包括创建一个新的实例，并以原始对象中的值初始化这个新的实例。复制非指针型实例变量的值很简单，比如布尔，整数和浮点数。复制指针型实例变量有两种方法。一种方法称为浅拷贝，即将原始对象的指针值复制到副本中。因此，原始对象和副本共享引用数据。另一种方法称为深拷贝，即复制指针所引用的数据，并将其赋给副本的实例变量。
(1) 深拷贝
深拷贝的流程是先创建一个新的对象且引用计数为1，并用旧对象的值初始化这个新对象；
ClassA* objA = [[ClassA alloc] init];
ClassA* objB = [objA copy];
objB是一个新对象，引用计数为1，且objB的数据等同objA的数据；
注意：objB需要释放，否则会引起内存泄漏！
(2) 浅拷贝
浅拷贝的流程是，无需引入新的对象，把原有对象的引用计数+1即可
ClassA* objA = [[ClassA alloc] init];
ClassA* objB = [objA retain];
注意：objB需要释放，恢复objA的引用计数，否则会引起内存泄漏！
2.2.3对象的存取方法
(1)属性声明和实现
变量声明的常用属性类型包括readonly，assign，retain和copy；且系统会自动为声明了属性的变量生成set和get函数；
readonly属性：只能读，不能写；
assign属性：是默认属性，直接赋值，没有任何保留与释放问题；
retain属性：会增加原有对象的引用计数并且在赋值前会释放原有对象，然后在进行赋值；
copy属性：会复制原有对象，并在赋值前释放原有对象，然后在进行赋值；
(2)使用属性声明可能带来的隐患
当一个非指针变量使用retain（或者copy）这个属性时，尽量不要显性的release这个变量；直接给这个变量置空即可；否则容易产生过度释放，导致程序crash；例如：
ClassA类的strName是NSString* 类型的变量且声明的属性为retain；
ClassA.strName = nil; /* 释放原有对象且对此对象赋值为空*/
[ClassA.strName release]; /* strName内存可能已经被释放过了，将导致程序crash
 */
Assign这个属性一般是非指针变量（布尔类型，整形等）时用这个类型；属于直接赋值型，不需要考虑内存的保留与释放；
如果一个指针类型的变量使用assign类型的属性，有可能引用已经释放的变量；导致程序crash；例如：
ClassB* obj =[[[ClassB alloc] init] autorelease];
……
ClassA.strName = obj; /* strName 指向obj的内存地址*/
后续在使用ClassA.strName的时候，因为obj是autorelease的，可能obj的内存已经被回收；导致引用无效内存，程序crash；
2.3 IOS平台AutoRelease机制
2.3.1 自动释放池的常见问题
大家在开发iOS程序的时候，是否遇到过在列表滑动的情况内存莫名的增长，频繁访问图片的时候内存莫名的增长，频繁的打开和关闭数据库的时候内存莫名的增长……这些都是拜iOS的autorelease机制所赐；具体分析如下：
(1) 滑动列表的时候，内存出现莫名的增长，原因可能有如下可能：
a：没有使用UITableView的reuse机制；导致每显示一个cell都用autorelease的方式重新alloc一次；导致cell的内存不断的增加；
b：每个cell会显示一个单独的UIView，在UIView发生内存泄漏，导致cell的内存不断增长；
(2) 频繁访问图片的时候，内存莫名的增长；
频繁的访问网络图片，导致iOS内部API，会不断的分配autorelease方式的buffer来处理图片的解码与显示；利用图片cache可以缓解一下此问题；
(3) 频繁打开和关闭SQLite，导致内存不断的增长；
在进行SQLite频繁打开和关闭操作，而且读写的数据buffer较大，那么SQLite在每次打开与关闭的时候，都会利用autorelease的方式分配51K的内存；如果访问次数很多，内存马上就会顶到几十兆，甚至上百兆！所以针对频繁的读写数据库且数据buffer较大的情况，可以设置SQLite的长连接方式；避免频繁的打开和关闭数据库；
2.3.2 自动释放池的概念
NSAutoreleasePool内部包含一个数组（NSMutableArray），用来保存声名为autorelease的所有对象。如果一个对象声明为autorelease，系统所做的工作就是把这个对象加入到这个数组中去。
ClassA *obj1 = [[[ClassA alloc] init] autorelease]; //retain count = 1，把此对象加入autorelease
 pool中
NSAutoreleasePool自身在销毁的时候，会遍历一遍这个数组，release数组中的每个成员。如果此时数组中成员的retain
 count为1，那么release之后，retain
 count为0，对象正式被销毁。如果此时数组中成员的retain
 count大于1，那么release之后，retain
 count大于0，此对象依然没有被销毁，内存泄露。
2.3.3 自动释放池的作用域与嵌套
    AutoreleasePool是可以嵌套使用的！池是被嵌套的，嵌套的结果是个栈，同一线程只有当前栈顶pool实例是可用的：
![](https://partner.outlook.cn/owa/service.svc/s/GetFileAttachment?id=AAMkAGM4ODIyYTRhLTBiMjgtNDUzYy04OTE3LTQ0NmMyNzA5NTA4MgBGAAAAAACta1CHqa5IQbrw4%2FmjvMNQBwDNMi%2BLWfmtQ4yfkI7LtrwIAAAAAAEMAADNMi%2BLWfmtQ4yfkI7LtrwIAAAzKxa%2BAAABEgAQAGQYwHVvbKBMhiXTwvS45jQ%3D&X-OWA-CANARY=QMGUaDwG4USXnnyLQDn6CVB_22rwRtIIUqxcE_oB1-QZaG-5YmL1D5TMJGeCj4Ccbadh06VWGMk.)
当短生命周期内，比如一个循环中，会产生大量的临时内存，可以创建一个临时的autorelease
 pool，这样可以达到快速回收内存的目的；
2.3.4 自动施放池的手动创建与自动创建
(1) 需要手动创建自动释放池
●如果你正在编写一个不是基于Application
 Kit的程序，比如命令行工具，则没有对自动释放池的内置支持；你必须自己创建它们。
●如果你生成了一个从属线程，则一旦该线程开始执行，你必须立即创建你自己的自动释放池；否则，你将会泄漏对象。
●如果你编写了一个循环，其中创建了许多临时对象，你可以在循环内部创建一个自动释放池，以便在下次迭代之前销毁这些对象。这可以帮助减少应用程序的最大内存占用量。
(2) 系统自动创建自动释放池
Application Kit会在一个事件周期（或事件循环迭代）的开端—比如鼠标按下事件—自动创建一个自动释放池，并且在事件周期的结尾释放它.
2.4 iOS平台内存使用陷阱
2.4.1 重复释放
在前文已经提到，不要释放不是自己创建的对象；
释放自己的autorelease对象，app会crash；
释放系统的autorelease对象，app会crash；
2.4.2 循环引用
![](https://partner.outlook.cn/owa/service.svc/s/GetFileAttachment?id=AAMkAGM4ODIyYTRhLTBiMjgtNDUzYy04OTE3LTQ0NmMyNzA5NTA4MgBGAAAAAACta1CHqa5IQbrw4%2FmjvMNQBwDNMi%2BLWfmtQ4yfkI7LtrwIAAAAAAEMAADNMi%2BLWfmtQ4yfkI7LtrwIAAAzKxa%2BAAABEgAQACxmpZYoikpLmfb2%2BMHRXRY%3D&X-OWA-CANARY=QMGUaDwG4USXnnyLQDn6CVB_22rwRtIIUqxcE_oB1-QZaG-5YmL1D5TMJGeCj4Ccbadh06VWGMk.)
循环引用，容易产生野引用，内存无法回收，最终导致内存泄漏！可以通过弱引用的方式来打破循环引用链；所谓的弱引用就是不需要retain，直接赋值的方式，这样的话，可以避免循环引用的问题，但是需要注意的是，避免重复释放的问题；
2.5 iOS平台内存报警机制
由于iOS平台的内存管理机制，不支持虚拟内存，所以在内存不足的情况，不会去Ram上创建虚拟内存；所以一旦出现内存不足的情况，iOS平台会通知所有已经运行的app，不论是前台app还是后台挂起的app，都会收到memory
 warning的notice；一旦app收到memory
 warning的notice，就应该回收占用内存较大的变量；
2.5.1 内存报警处理流程
(1) app收到系统发过来的memory
 warning的notice；
(2) app释放占用较大的内存；
(3)系统回收此app所创建的autorelease的对象；
(4) app返回到已经打开的页面时，系统重新调用viewdidload方法，view重新加载页面数据；重新显示；
2.5.2 内存报警测试方法
在Simulate上可以模拟低内存报警消息；
iOS模拟器-> 硬件-> 模拟内存警告；
开发者可以在模拟器上来模拟手机上的低内存报警情况，可以避免由于低内存报警引出的app的莫名crash问题；
**3****IOS****内存原理****ARC****技术介绍**
    Automatic Reference Counting (ARC) 是一个编译期的技术，利用此技术可以简化Objective-C编程在内存管理方面的工作量。
这里我把此技术翻译为自动内存计数器管理技术，下图是使用和不使用此技术的Objective-C代码的区别。
![](https://partner.outlook.cn/owa/service.svc/s/GetFileAttachment?id=AAMkAGM4ODIyYTRhLTBiMjgtNDUzYy04OTE3LTQ0NmMyNzA5NTA4MgBGAAAAAACta1CHqa5IQbrw4%2FmjvMNQBwDNMi%2BLWfmtQ4yfkI7LtrwIAAAAAAEMAADNMi%2BLWfmtQ4yfkI7LtrwIAAAzKxa%2BAAABEgAQALNMZOf2%2B7hEnBvgqZl1zcU%3D&X-OWA-CANARY=QMGUaDwG4USXnnyLQDn6CVB_22rwRtIIUqxcE_oB1-QZaG-5YmL1D5TMJGeCj4Ccbadh06VWGMk.)
     ARC技术是随着XCode4.2一起发布的，在缺省工程模板中，你可以指定你的工程是否支持ARC技术，如果你不指定工程支持ARC技术，在代码中你必须使用管理内存的代码来管理内存。
本章节内容详见IOS ARC技术官网：
[http://blog.csdn.net/nicktang/article/details/6792972](http://blog.csdn.net/nicktang/article/details/6792972)
备注：本章节ARC技术比较新，但是第二章节也有开发人员在使用，所以一些问题还是存在。
**4****内存检测工具****----****编译和分析工具****Analyze**
iOS的分析工具可以发现编译中的warning，内存泄漏隐患，甚至还可以检查出logic上的问题；所以在自测阶段一定要解决Analyze发现的问题，可以避免出现严重的bug;
常见提示如下：
    (1)内存泄漏隐患提示：Potential
 Leak of an object allocated on line ……
    (2)数据赋值隐患提示：The
 left operand of …… is a garbage value;
    (3)对象引用隐患提示：Reference-Counted
 object is used after it is released;
以上提示均比较严重，可能会引起严重问题，需要开发者密切关注！
如下方式进入：xcode---product----Analyze, 如下显示，这个是以酒店返现IOS版本为例：
![](https://partner.outlook.cn/owa/service.svc/s/GetFileAttachment?id=AAMkAGM4ODIyYTRhLTBiMjgtNDUzYy04OTE3LTQ0NmMyNzA5NTA4MgBGAAAAAACta1CHqa5IQbrw4%2FmjvMNQBwDNMi%2BLWfmtQ4yfkI7LtrwIAAAAAAEMAADNMi%2BLWfmtQ4yfkI7LtrwIAAAzKxa%2BAAABEgAQABUWZK26i7VAs9oGB0nvUkw%3D&X-OWA-CANARY=QMGUaDwG4USXnnyLQDn6CVB_22rwRtIIUqxcE_oB1-QZaG-5YmL1D5TMJGeCj4Ccbadh06VWGMk.)
**5****内存泄漏检测工具****----Leak**
    Leak工具可以很容易的统计所有内存泄漏的点，而且还可以显示在哪个文件，哪行代码有内存泄漏，这样定位问题比较容易，也比较方便；但是Leak在统计内存泄漏的时候会把autorelease方式的内存也统计进来；所以我们在查找内存泄漏情况的时候，可以autorelease的情况忽略掉；
5.1 使用Leak工具
 1，如下方式进入leak：
xcode---product----Profile----Leak
 2，示意顺序图
如下选择图示：
![](https://partner.outlook.cn/owa/service.svc/s/GetFileAttachment?id=AAMkAGM4ODIyYTRhLTBiMjgtNDUzYy04OTE3LTQ0NmMyNzA5NTA4MgBGAAAAAACta1CHqa5IQbrw4%2FmjvMNQBwDNMi%2BLWfmtQ4yfkI7LtrwIAAAAAAEMAADNMi%2BLWfmtQ4yfkI7LtrwIAAAzKxa%2BAAABEgAQADjSR%2Fhwb%2FxHhkbwu7I4els%3D&X-OWA-CANARY=QMGUaDwG4USXnnyLQDn6CVB_22rwRtIIUqxcE_oB1-QZaG-5YmL1D5TMJGeCj4Ccbadh06VWGMk.)
点击leaks, 然后点击Profile开始检测，通过Leak工具可以很快发现代码中的内存泄漏，通过工具也可以很快找到发生内存泄漏的代码段。示意图如下：这个是以酒店返现IOS版本为例：
![](https://partner.outlook.cn/owa/service.svc/s/GetFileAttachment?id=AAMkAGM4ODIyYTRhLTBiMjgtNDUzYy04OTE3LTQ0NmMyNzA5NTA4MgBGAAAAAACta1CHqa5IQbrw4%2FmjvMNQBwDNMi%2BLWfmtQ4yfkI7LtrwIAAAAAAEMAADNMi%2BLWfmtQ4yfkI7LtrwIAAAzKxa%2BAAABEgAQAKYI47owtBBEne9b%2BolHmIU%3D&X-OWA-CANARY=QMGUaDwG4USXnnyLQDn6CVB_22rwRtIIUqxcE_oB1-QZaG-5YmL1D5TMJGeCj4Ccbadh06VWGMk.)
