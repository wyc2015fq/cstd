# Java中匿名类的两种实现方式 - z69183787的专栏 - CSDN博客
2013年12月26日 20:25:20[OkidoGreen](https://me.csdn.net/z69183787)阅读数：860
使用匿名内部类课使代码更加简洁、紧凑，模块化程度更高。内部类能够访问外部内的一切成员变量和方法，包括私有的，而实现接口或继承类做不到。然而这个不是我说的重点，我说的很简单，就是匿名内部类的两种实现方式：第一种，继承一个类，重写其方法；第二种，实现一个接口（可以是多个），实现其方法。下面通过代码来说明：
**[java]**[view
 plain](http://blog.csdn.net/cntanghai/article/details/6094481#)[copy](http://blog.csdn.net/cntanghai/article/details/6094481#)[print](http://blog.csdn.net/cntanghai/article/details/6094481#)[?](http://blog.csdn.net/cntanghai/article/details/6094481#)
- publicclass TestAnonymousInterClass{     
- publicstaticvoid main(String args[]){     
-         TestAnonymousInterClass test=new TestAnonymousInterClass();     
-         test.show();     
-     }     
- //在这个方法中构造了一个匿名内部类   
- privatevoid show(){     
-         Out anonyInter=new Out(){// 获取匿名内部类实例   
- 
- void show(){//重写父类的方法   
-                 System.out.println("this is Anonymous InterClass showing.");     
-             }     
-         };     
-         anonyInter.show();// 调用其方法   
-     }     
- }      
- 
- // 这是一个已经存在的类，匿名内部类通过重写其方法，将会获得另外的实现   
- class Out{     
- void show(){     
-         System.out.println("this is Out showing.");     
-     }     
- }    
程序运行的输出结果为：
this is Anonymous InterClass showing.
   所以在这里看出，匿名内部类有了自己的实现。其实很简单，使用匿名内部类是因为我这地方需要有点什么特殊的实现，所以我就在这地方把具体实现也给了出来了。然后我就在这地方获取它的实例，调用它的方法。
   接口的方式，只要把父类换成接口就行了，没必要给出代码了。
使用匿名内部类时我们不要忘了我们的目的，我们只是在这地方想对某个类有特殊的实现。而不要想得太多，在匿名内部编写其它的方法。在匿名内部类中编写的自己的方法是不可见的。此种做法是毫无意义的，当然一般也不会这么做。在这里只是告诉初学者对于匿名内部类不要想的太多，而要这么想：匿名内部类就是重写父类或接口的方法。
 匿名内部类是没有名字的，所以我们没办法获得其类型，而只能把它当作超类或接口类型来使用。
