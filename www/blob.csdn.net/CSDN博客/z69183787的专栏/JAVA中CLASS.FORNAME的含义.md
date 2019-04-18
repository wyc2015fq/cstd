# JAVA中CLASS.FORNAME的含义 - z69183787的专栏 - CSDN博客
2013年11月21日 20:10:42[OkidoGreen](https://me.csdn.net/z69183787)阅读数：999

Class.forName(xxx.xx.xx) 返回的是一个类, .newInstance() 后才创建一个对象 Class.forName(xxx.xx.xx);的作用是要求JVM查找并加载指定的类，也就是说JVM会执行该类的静态代码段
Class aClass = Class.forName(xxx.xx.xx);
Object anInstance = aClass.newInstance();
Class.forName("").newInstance()返回的是object
but there is some limit for this method to create instance
that is your class constructor should no contain parameters, and you should cast the instance manually.
Class Driver{
protected static Driver current;
public static Driver getDriver(){
return current;
}
}
Class MyDriver extends Driver{
static{
Driver.current=new MyDriver();
}
MyDriver(){}
}
用时:
Class.forName("MyDriver");
Driver d=Driver.getDriver();
有的jdbc连接数据库的写法里是Class.forName(xxx.xx.xx);而有一些：Class.forName(xxx.xx.xx).newInstance()，为什么会有这两种写法呢？
Class.forName(xxx.xx.xx) 返回的是一个类,
.newInstance() 后才创建一个对象
Class.forName(xxx.xx.xx);的作用是要求JVM查找并加载指定的类，也就是说JVM会执行该类的静态代码段
在JDBC规范中明确要求这个Driver类必须向DriverManager注册自己，即任何一个JDBC Driver的Driver类的代码都必须类似如下：
public class MyJDBCDriver implements Driver {
static {
DriverManager.registerDriver(new MyJDBCDriver());
}
}
所以我们在使用JDBC时只需要Class.forName(XXX.XXX);就可以了
下面给一个例子：Class的最大作用就是实现了动态加载类，为多态提供了很好的帮助。
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)class A
![](http://www.cnblogs.com/Images/OutliningIndicators/ExpandedBlockStart.gif){
![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)void aa()
![](http://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif){
![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)        System.out.println ("A里的");    
![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
![](http://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)    }
![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
![](http://www.cnblogs.com/Images/OutliningIndicators/ExpandedBlockEnd.gif)}
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)class B extends A
![](http://www.cnblogs.com/Images/OutliningIndicators/ExpandedBlockStart.gif){
![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)void aa()
![](http://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif){
![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)        System.out.println ("B里的");    
![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
![](http://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)    }
![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
![](http://www.cnblogs.com/Images/OutliningIndicators/ExpandedBlockEnd.gif)}
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)class C extends A
![](http://www.cnblogs.com/Images/OutliningIndicators/ExpandedBlockStart.gif){
![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)void aa()
![](http://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif){
![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)        System.out.println ("C里的");    
![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
![](http://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)    }
![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
![](http://www.cnblogs.com/Images/OutliningIndicators/ExpandedBlockEnd.gif)}
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)publicclass ClassDemo
![](http://www.cnblogs.com/Images/OutliningIndicators/ExpandedBlockStart.gif){
![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)publicstaticvoid main(String[] args)
![](http://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif){
![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)        ClassDemo t=new ClassDemo();
![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)        t.show("C");
![](http://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)    }
![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)void show(String name)
![](http://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif){
![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)try
![](http://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif){
![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)             A show=(A)Class.forName(name).newInstance();
![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)        show.aa();
![](http://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)    }
![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)catch(Exception e)
![](http://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif){
![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)        System.out.println (e);
![](http://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)        }
![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
![](http://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)    }
![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
![](http://www.cnblogs.com/Images/OutliningIndicators/ExpandedBlockEnd.gif)}
