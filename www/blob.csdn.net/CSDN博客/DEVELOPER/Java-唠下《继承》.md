# Java-唠下《继承》 - DEVELOPER - CSDN博客





2018年03月14日 15:25:43[学术袁](https://me.csdn.net/u012827205)阅读数：83
所属专栏：[JAVA-语言](https://blog.csdn.net/column/details/20397.html)









> 
继承是复用程序代码的有力手段。它有两个重要的概念：方法重载和方法覆盖。 

  继承关系最大的弱点就是——打破封装；而组合关系与之相比，她的优势是不会破坏封装。但是组合关系却会比继承关系要创建更多的对象。


### 方法重载（Overload）要点
- **重载的行为发生在同一个类中。**
- **方法名相同。**
- **方法的参数类型、个数、顺序（前提是参数签名不相同）至少有一项不同。**
- **方法的返回类型可以不同。（不是重载的必要条件）**
- **方法的修饰符可以不同。（不是重载的必要条件）**

**解释一下参数签名是什么？**
![这里写图片描述](https://img-blog.csdn.net/20180314130739254?watermark/2/text/Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
> 
在图片中看到，方法getGold(String xx, String xx)的参数**goldNum** 和 **goldWeight** 无论怎么交换顺序，都会报错，因为他们的参数签名是没有变化的。然而我们看方法getMoney(int xx, String xx)，改变了参数顺序之后，就是ok的，因为随着参数的变化，参数签名也发生了变化。


### 方法覆盖（Override）要点
- **覆盖的行为发生在子类与父类之间。**
- **子类方法的访问权限（访问控制修饰符 public protected private）、名称、参数签名、返回值类型和抛出的异常类型、异常数量必须一致且一样。**
- **子类的静态方法不能覆盖父类的静态方法，若存在子类和父类具有相同的静态方法，子类中便会隐藏父类的静态方法。**
- **父类的私有方法不能被子类覆盖。**
- **父类的非抽象方法可以被覆盖为抽象方法。**
- **父类的抽象方法可以被子类通过两种方式覆盖：子类实现父类的抽象方法和子类重新生命父类的抽象方法。**

**静态方法不存在覆盖详解**
![这里写图片描述](https://img-blog.csdn.net/20180314134304269?watermark/2/text/Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![这里写图片描述](https://img-blog.csdn.net/20180314134250994?watermark/2/text/Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
> 
图中看到运行的结果，证明了在子类和父类中同名的静态方法不存在覆盖。对这种情况，可共存，不属覆盖，目的为了便于在子类中隐藏父类的静态方法。解释为，Java 虚拟机把静态方法和所属的类绑定，而把实例方法和所属的实例进行绑定。


**父类的非抽象方法怎么被覆盖为抽象方法代码解释**

![这里写图片描述](https://img-blog.csdn.net/20180314135850542?watermark/2/text/Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

![这里写图片描述](https://img-blog.csdn.net/20180314135858332?watermark/2/text/Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

**Super 和 This 关键字的使用**
![这里写图片描述](https://img-blog.csdn.net/20180314142743631?watermark/2/text/Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![这里写图片描述](https://img-blog.csdn.net/20180314143100804?watermark/2/text/Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![这里写图片描述](https://img-blog.csdn.net/2018031414310866?watermark/2/text/Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
> 
从图中代码的打印结果可以看出 （1）当子类的某个方法覆盖了父类的方法时，在子类范围内，父类的方法不可见；（2）当子类中定义了和父类同名的成员变量时，在子类范围内，父类的成员变量不可见；但针对最后两行代码，第一行出现的结果是覆盖的结果；第二行的成员变量的结果要注意：对于一个引用类型的变量，Java 编译器按照它被声明的类型进行处理和操作；这是多态的属性。


**深刻解析Java语言多态的含义**

![这里写图片描述](https://img-blog.csdn.net/20180314144800993?watermark/2/text/Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

> 
多态：同一消息可以根据发送对象的不同而采取多种不同的行为方式。 

  多态存在的三个必要条件 ：有继承、有重写、父类引用指向子类对象。


```java
//饲养员
public class Feeder {

    public void feed(Animal animal, Food food) {
        animal.eatFood(food);
    }
}

//禽兽
public class Animal {

    public void eatFood(Food food) {
        System.out.println("禽兽也要进食...");
    }
}

//食物
public class Food {
public Food() {
    // TODO Auto-generated constructor stub
    System.out.println("我拥有各种食物...");
}
}


//猫
public class Cat extends Animal{
    public void eatFood(Food food) {
        System.out.println("喂 猫---吃-鱼");

    }
}
//食物-鱼
public class Fish extends Food{

    public Fish() {
        // TODO Auto-generated constructor stub
        super();
        System.out.println("挑选食物--鱼肉");
    }
}


//狗
public class Dog extends Animal{

    public void eatFood(Food food) {
        System.out.println("喂 狗---吃-骨头");

    }
}

//食物-骨头
public class Bone extends Food{

    public Bone() {
        // TODO Auto-generated constructor stub
        super();
        System.out.println("挑选食物--骨头");
    }
}

//测试类
public class JavaTest{

    public static void main(String[] args) {
        Feeder feeder = new Feeder();

        Animal cat = new Cat();
        Food catFood = new Fish();
        feeder.feed(cat, catFood);

        System.out.println("-------------------");

        Animal dog = new Dog();
        Food dogFood = new Bone();
        feeder.feed(dog, dogFood);
    }
}
```

![这里写图片描述](https://img-blog.csdn.net/20180314151630702?watermark/2/text/Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

> 
从以上内容来看，以Animal作为一个类型，它可能引用Dog和Cat的实例。在类Feeder的方法feed()中调用了 animal.eatFood()方法。此时，Java虚拟机会执行animal变量所引用的实例Cat或者Dog的方法 eatFood()。由此可见， animal 的变量有多种状态，可变 猫、狗。这就是多态字面上的含义。而多态的实现是依靠——继承。


**多态的属性研究**

```java
public class Father {
    String var = "父类-var";// 实例变量
    static String staticVar = "父类-staticVar";// 静态变量

    void method() {
        System.out.println("父类实例方法-method");
    }

    static void staticMethod() {
        System.out.println("父类的静态方法-staticMehod");
    }
}

public class Son extends Father{

    String var = "子类-var";// 实例变量
    static String staticVar = "子类-staticVar";// 静态变量

    void method() {
        System.out.println("子类实例方法-method");
    }

    static void staticMethod() {
        System.out.println("子类的静态方法-staticMehod");
    }

    String sonVar = "该实例-变量-只属于子类";
    void sonMethod() {
        System.out.println("该实例-方法-只属于子类");
    }

    public static void main(String[] args) {
        Father who = new Son();
        System.out.println("who.var="+who.var);
        System.out.println("who.staticVar="+who.staticVar);
        who.method();
        who.staticMethod();

        //System.out.println("who.sonVar="+who.sonVar);//表示不存在，调用错误
        //System.out.println("who.sonMethod="+who.sonMethod);//表示不存在，调用错误

    }
}
```

![这里写图片描述](https://img-blog.csdn.net/20180314154944858?watermark/2/text/Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

> 
从上面的代码逻辑以及运行的结果来看，我只想总结一下。对于一个引用类型的变量，Java 编译器按照它被声明的类型进行处理和操作，”重写方法”除外。就比如上面代码 `Father who = new Son();` 其中 `who` 就是一个引用类型变量，而它声明的类型就是`Father` 。然后我们就看到了这样的结果，上面代码中最后被注释掉的两行`System.out.println` 都会出现调用失败的错误。


**组合关系和继承关系使用代码逻辑进行比较**

> 
继承关系最大的弱点就是——打破封装，因为子类可以继承父类的开放属性，并可以根据自身需要进行扩展；而组合关系与之相比，她的优势是不会破坏封装。但是组合关系却会比继承关系要创建更多的对象。


```
//玫瑰花
public class Rose {

    protected void raise() {
        System.out.println("仰望-生长");
    }

    protected void fade() {
        System.out.println("低头-凋谢");
    }

    public void die() {
        fade();
        System.out.println("枯萎");
    }

    public void openRose() {
        raise();
        System.out.println("玫瑰花开");
    }
}


//牡丹花
public class Peony {
    protected void raise() {
        System.out.println("仰望-生长");
    }

    protected void fade() {
        System.out.println("低头-凋谢");
    }

    public void die() {
        fade();
        System.out.println("枯萎");
    }

    public void openPeony() {
        raise();
        System.out.println("牡丹花开");
    }
}
```

使用继承 

此时需要从牡丹和玫瑰花中抽象出他们的父类——花（Flower），并且具备牡丹和玫瑰的基本功能：生长、凋谢和枯萎。

```java
//花
public class Flower {
    protected void raise() {
        System.out.println("仰望-生长");
    }

    protected void fade() {
        System.out.println("低头-凋谢");
    }

    public void die() {
        System.out.println("枯萎");
    }
}

//牡丹花
public class Peony extends Flower{

    public void die() {
        fade();
        System.out.println("牡丹花枯萎了");
    }

    public void openPeony() {
        raise();
        System.out.println("牡丹花开");
    }
}

//玫瑰花
public class Rose extends Flower{

    public void die() {
        fade();
        System.out.println("玫瑰花枯萎了");
    }

    public void openRose() {
        raise();
        System.out.println("玫瑰花开");
    }
}
```

使用组合 

此时需要从牡丹和玫瑰花中抽象出他们具备相同的功能到一个类中，如花（Flower）。而此类要为牡丹和玫瑰提供接口。

```java
//花
public class Flower {
    protected void raise() {
        System.out.println("仰望-生长");
    }

    protected void fade() {
        System.out.println("低头-凋谢");
    }
}

//玫瑰花
public class Rose{
    Flower flower;
    public Rose(Flower flower) {
        this.flower = flower;
    }

    public void die() {
        flower.fade();
        System.out.println("玫瑰花枯萎了");
    }

    public void openRose() {
        flower.raise();
        System.out.println("玫瑰花开");
    }
}

//牡丹花
public class Peony{
    Flower flower;
    public Peony(Flower flower) {
        this.flower = flower;
    }
    public void die() {
        flower.fade();
        System.out.println("牡丹花枯萎了");
    }

    public void openPeony() {
        flower.raise();
        System.out.println("牡丹花开");
    }
}
```






