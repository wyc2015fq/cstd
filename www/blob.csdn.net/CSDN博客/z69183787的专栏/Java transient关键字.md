# Java transient关键字 - z69183787的专栏 - CSDN博客
2014年05月29日 14:23:26[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1136
Volatile修饰的成员变量在每次被线程访问时，都强迫从主内存中重读该成员变量的值。而且，当成员变量发生变化时，强迫线程将变化值回写到主内存。这样在任何时刻，两个不同的线程总是看到某个成员变量的同一个值。   
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)Java语言规范中指出：为了获得最佳速度，允许线程保存共享成员变量的私有拷贝，而且只当线程进入或者离开同步代码块时才与共享成员变量的原始值对比。   
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)这样当多个线程同时与某个对象交互时，就必须要注意到要让线程及时的得到共享成员变量的变化。   
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)而volatile关键字就是提示VM：对于这个成员变量不能保存它的私有拷贝，而应直接与共享成员变量交互。   
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)使用建议：在两个或者更多的线程访问的成员变量上使用volatile。当要访问的变量已在synchronized代码块中，或者为常量时，不必使用。   
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)由于使用volatile屏蔽掉了VM中必要的代码优化，所以在效率上比较低，因此一定在必要时才使用此关键字。   
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)
****![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)java关键字Transient****
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)转自http://horst.sun.blog.163.com/blog/static/348849612007614494492/   
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)翻译自http://www.devx.com/tips/Tip/13726。   
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)Java的serialization提供了一种持久化对象实例的机制。当持久化对象时，可能有一个特殊的对象数据成员，我们不想   
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)用serialization机制来保存它。为了在一个特定对象的一个域上关闭serialization，可以在这个域前加上关键字transient。   
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)transient是Java语言的关键字，用来表示一个域不是该对象串行化的一部分。当一个对象被串行化的时候，transient型变量的值不包括在串行化的表示中，然而非transient型的变量是被包括进去的。  
****注意static变量也是可以串行化的 ****![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)首先，让我们看一些Java serialization的代码：   
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)publicclass LoggingInfo implements java.io.Serializable   
![](http://www.blogjava.net/Images/OutliningIndicators/ExpandedBlockStart.gif){   
![](http://www.blogjava.net/Images/OutliningIndicators/InBlock.gif)private Date loggingDate =new Date();   
![](http://www.blogjava.net/Images/OutliningIndicators/InBlock.gif)private String uid;   
![](http://www.blogjava.net/Images/OutliningIndicators/InBlock.gif)privatetransient String pwd;   
![](http://www.blogjava.net/Images/OutliningIndicators/InBlock.gif)
![](http://www.blogjava.net/Images/OutliningIndicators/InBlock.gif)    LoggingInfo(String user, String password)   
![](http://www.blogjava.net/Images/OutliningIndicators/ExpandedSubBlockStart.gif){   
![](http://www.blogjava.net/Images/OutliningIndicators/InBlock.gif)        uid = user;   
![](http://www.blogjava.net/Images/OutliningIndicators/InBlock.gif)        pwd = password;   
![](http://www.blogjava.net/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)    }
![](http://www.blogjava.net/Images/OutliningIndicators/InBlock.gif)public String toString()   
![](http://www.blogjava.net/Images/OutliningIndicators/ExpandedSubBlockStart.gif){   
![](http://www.blogjava.net/Images/OutliningIndicators/InBlock.gif)        String password=null;   
![](http://www.blogjava.net/Images/OutliningIndicators/InBlock.gif)if(pwd ==null)   
![](http://www.blogjava.net/Images/OutliningIndicators/ExpandedSubBlockStart.gif){   
![](http://www.blogjava.net/Images/OutliningIndicators/InBlock.gif)        password ="NOT SET";   
![](http://www.blogjava.net/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)        }
![](http://www.blogjava.net/Images/OutliningIndicators/InBlock.gif)else
![](http://www.blogjava.net/Images/OutliningIndicators/ExpandedSubBlockStart.gif){   
![](http://www.blogjava.net/Images/OutliningIndicators/InBlock.gif)            password = pwd;   
![](http://www.blogjava.net/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)        }
![](http://www.blogjava.net/Images/OutliningIndicators/InBlock.gif)return"logon info: \n   "+"user: "+ uid +
![](http://www.blogjava.net/Images/OutliningIndicators/InBlock.gif)"\n   logging date : "+ loggingDate.toString() +
![](http://www.blogjava.net/Images/OutliningIndicators/InBlock.gif)"\n   password: "+ password;   
![](http://www.blogjava.net/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)    }
![](http://www.blogjava.net/Images/OutliningIndicators/ExpandedBlockEnd.gif)}
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)现在我们创建一个这个类的实例，并且串行化(serialize)它 ,然后将这个串行化对象写如磁盘。   
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)LoggingInfo logInfo =new LoggingInfo("MIKE", "MECHANICS");   
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)System.out.println(logInfo.toString());   
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)try
![](http://www.blogjava.net/Images/OutliningIndicators/ExpandedBlockStart.gif){   
![](http://www.blogjava.net/Images/OutliningIndicators/InBlock.gif)   ObjectOutputStream o =new ObjectOutputStream(   
![](http://www.blogjava.net/Images/OutliningIndicators/InBlock.gif)new FileOutputStream("logInfo.out"));   
![](http://www.blogjava.net/Images/OutliningIndicators/InBlock.gif)   o.writeObject(logInfo);   
![](http://www.blogjava.net/Images/OutliningIndicators/InBlock.gif)   o.close();   
![](http://www.blogjava.net/Images/OutliningIndicators/ExpandedBlockEnd.gif)}
![](http://www.blogjava.net/Images/OutliningIndicators/ExpandedBlockStart.gif)catch(Exception e) {//deal with exception}   
![](http://www.blogjava.net/Images/OutliningIndicators/InBlock.gif)
![](http://www.blogjava.net/Images/OutliningIndicators/InBlock.gif)To read the object back, we can write   
![](http://www.blogjava.net/Images/OutliningIndicators/InBlock.gif)
![](http://www.blogjava.net/Images/OutliningIndicators/InBlock.gif)try
![](http://www.blogjava.net/Images/OutliningIndicators/ExpandedSubBlockStart.gif){   
![](http://www.blogjava.net/Images/OutliningIndicators/InBlock.gif)   ObjectInputStream in =new ObjectInputStream(   
![](http://www.blogjava.net/Images/OutliningIndicators/InBlock.gif)new FileInputStream("logInfo.out"));   
![](http://www.blogjava.net/Images/OutliningIndicators/InBlock.gif)   LoggingInfo logInfo = (LoggingInfo)in.readObject();   
![](http://www.blogjava.net/Images/OutliningIndicators/InBlock.gif)   System.out.println(logInfo.toString());   
![](http://www.blogjava.net/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)}
![](http://www.blogjava.net/Images/OutliningIndicators/ExpandedSubBlockStart.gif)catch(Exception e) {//deal with exception}   
![](http://www.blogjava.net/Images/OutliningIndicators/InBlock.gif)
![](http://www.blogjava.net/Images/OutliningIndicators/InBlock.gif)如果我们运行这段代码，我们会注意到从磁盘中读回(read——back (de-serializing))的对象打印password为"NOT SET"。这是当我们定义pwd域为transient时，所期望的正确结果。   
![](http://www.blogjava.net/Images/OutliningIndicators/InBlock.gif)现在，让我们来看一下粗心对待transient域可能引起的潜在问题。假设我们修改了类定义，提供给transient域一个默认值，   
![](http://www.blogjava.net/Images/OutliningIndicators/InBlock.gif)代码如下：   
![](http://www.blogjava.net/Images/OutliningIndicators/InBlock.gif)
![](http://www.blogjava.net/Images/OutliningIndicators/InBlock.gif)publicclass GuestLoggingInfo implements java.io.Serializable   
![](http://www.blogjava.net/Images/OutliningIndicators/ExpandedSubBlockStart.gif){   
![](http://www.blogjava.net/Images/OutliningIndicators/InBlock.gif)private Date loggingDate =new Date();   
![](http://www.blogjava.net/Images/OutliningIndicators/InBlock.gif)private String uid;   
![](http://www.blogjava.net/Images/OutliningIndicators/InBlock.gif)privatetransient String pwd;   
![](http://www.blogjava.net/Images/OutliningIndicators/InBlock.gif)
![](http://www.blogjava.net/Images/OutliningIndicators/InBlock.gif)    GuestLoggingInfo()   
![](http://www.blogjava.net/Images/OutliningIndicators/ExpandedSubBlockStart.gif){   
![](http://www.blogjava.net/Images/OutliningIndicators/InBlock.gif)        uid ="guest";   
![](http://www.blogjava.net/Images/OutliningIndicators/InBlock.gif)        pwd ="guest";   
![](http://www.blogjava.net/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)    }
![](http://www.blogjava.net/Images/OutliningIndicators/InBlock.gif)public String toString()   
![](http://www.blogjava.net/Images/OutliningIndicators/ExpandedSubBlockStart.gif){   
![](http://www.blogjava.net/Images/OutliningIndicators/InBlock.gif)//same as above   
![](http://www.blogjava.net/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)     }
![](http://www.blogjava.net/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)}
![](http://www.blogjava.net/Images/OutliningIndicators/InBlock.gif)**现在，如果我们穿行化GuestLoggingInfo的一个实例，将它写入磁盘，并且再将它从磁盘中读出，我们仍然看到读回的对象打印password 为 ****"NOT SET"****。当从磁盘中读出某个类的实例时，实际上并不会执行这个类的构造函数，   ![](http://www.blogjava.net/Images/OutliningIndicators/InBlock.gif)而是载入了一个该类对象的持久化状态，并将这个状态赋值给该类的另一个对象。 **
