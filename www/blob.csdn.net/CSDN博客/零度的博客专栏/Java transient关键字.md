# Java transient关键字 - 零度的博客专栏 - CSDN博客
2016年10月13日 17:34:33[零度anngle](https://me.csdn.net/zmx729618)阅读数：157
个人分类：[Java](https://blog.csdn.net/zmx729618/article/category/3108443)
     Java的serialization提供了一种持久化对象实例的机制。当持久化对象时，可能有一个特殊的对象数据成员，我们不想 用serialization机制来保存它。为了在一个特定对象的一个域上关闭serialization，可以在这个域前加上关键字transient。   transient是Java语言的关键字，用来表示一个域不是该对象串行化的一部分。当一个对象被串行化的时候，transient型变量的值不包括在串行化的表示中，然而非transient型的变量是被包括进去的。  
**     注意：static变量也是可以串行化的 **![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)首先，让我们看一些Java serialization的代码：   
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)publicclass LoggingInfo implements java.io.Serializable   
{   
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
{   
![](http://www.blogjava.net/Images/OutliningIndicators/InBlock.gif)   ObjectOutputStream o =new ObjectOutputStream(   
![](http://www.blogjava.net/Images/OutliningIndicators/InBlock.gif)new FileOutputStream("logInfo.out"));   
![](http://www.blogjava.net/Images/OutliningIndicators/InBlock.gif)   o.writeObject(logInfo);   
![](http://www.blogjava.net/Images/OutliningIndicators/InBlock.gif)   o.close();   
![](http://www.blogjava.net/Images/OutliningIndicators/ExpandedBlockEnd.gif)}
catch(Exception e) {//deal with exception}   
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
**       现在，如果我们穿行化GuestLoggingInfo的一个实例，将它写入磁盘，并且再将它从磁盘中读出，我们仍然看到读回的对象打印**
**password 为 ****"NOT SET"****。当从磁盘中读出某个类的实例时，实际上并不会执行这个类的构造函数，   而是载入了一个该类对象的持久化状态，并将这个状态赋值给该类的另一个对象。 **
