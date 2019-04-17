# foreach使用方法  和   枚举使用方法 - DEVELOPER - CSDN博客





2014年03月26日 18:50:50[学术袁](https://me.csdn.net/u012827205)阅读数：2707








```java
foreach使用方法  和   枚举使用方法

foreach：

这种循环方法的基本语法如下：

for(type variable : 数组或者集合){
   .....
     ........

}


为了更方便理解，写一段代码：

List<String> names=new ArrayList<String>();
names.add("josn");
names.add("tom");
names.add("jerry");

for(String name : names)
System.out.println(name);

枚举：

我们经常会碰到这样一种情景：某些数据必须是一些特定的值，例如：性别只能是“男”或者是“女”；或者参加宴会的只能是“Lucy”或者“Lily”。这个时候，我们就会想到“枚举”。在我们使用类或者接口来实现上面的功能，会比较麻烦。例如：

public class Gender{

public static final String MALE=1;
public static final String FEMALE=2;

}


然后，在需要用到性别的时候使用这个类中的性别来表示。比如：我有个方法，

method(ing sex);

要向里边放入性别男------------>method(Gender.MALE);
但是请注意，方法中的参数是一个int类型的值。那么这样我们可以完全使用这种方法 method(1)来表示。


虽然这个例子在语法上是成立的，但是在逻辑上是不成立的；因为“1”既不表示男性，也不表示“女性”；为了避免这种麻烦，我们只能在method();方法中进行检查，看看接收的参数是否为1或2；显然这种方法虽然可行，但是真的很麻烦。

那么，为了解决这种麻烦在JDK5.0中，引入了一种新的类型：“枚举”即"Enum"，可以很好地解决这个问题。

现在就用枚举来定义上面的性别：

public Enum Gemder{
    MALE;
    FEMALE;
}
然后，定义下面的程序代码来理解一下：
public class TextEnum{
  public vjoid method(Gender sex){
     switch(sex){
       case MALE:
         System.out.println("男");
         break;
       case FEMALE:
         System.out.println("女");
         break;
    }
}

  public static void main(String[] arg0){
     new TextEnum().method(Gander.FEMALE);
  }
}

这个时候，我们就没有办法使用method(1);的方法去调用了。但是，我们可以吧枚举类型的每一个值都映射到protected Enum(String name,int ordinal);枚举类的构造方法中。即

new Enum<Gender>("MALE",1);
new Enum<Gender>("FEMALE",2);
这样就OK了；


当然，更重要的 是：我们也可以自己定制构造方法，而不是使用默认的构造方法；例如：

public enum Gender{
  MALE ("男");
  FEMALE("女");
  String sex;

 Gender(String s){
   sex=s;
 }
}

在自定义枚举类型的构造方法时，注意不能定义public的构造器，这点在进行自定义构造器时候一定要注意！
```





