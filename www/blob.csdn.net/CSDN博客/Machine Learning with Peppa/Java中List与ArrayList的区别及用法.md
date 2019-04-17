# Java中List与ArrayList的区别及用法 - Machine Learning with Peppa - CSDN博客





2018年07月10日 21:34:37[刺客五六柒](https://me.csdn.net/qq_39521554)阅读数：691
所属专栏：[JavaSE](https://blog.csdn.net/column/details/21699.html)









                 List是一个接口，而ArrayList是List接口的一个实现类。 ArrayList类继承并实现了List接口。 因此，List接口不能被构造，也就是我们说的不能创建实例对象，但是我们可以像下面那样为List接口创建一个指向自己的对象引用，而ArrayList实现类的实例对象就在这充当了这个指向List接口的对象引用。 


要是你已经学过了OOP，上面的部分是不难理解的，这是面向对象重要的知识点，面向对象最重要的就是多态，我们都知道接口和抽象不能被实例化，但是它们可以创建一个指向自己的对象引用，它们的实现类或子类就在充当这样的角色，我想这就是面向对象编程中多态的优势。

前些日子在学习UML建模语言和Java设计模式的时候，深深地的体会到了面向对象编程的好处，Java集合框架中用到也不足为奇，Java本身就是面向对象的编程语言。上面的理解可能有点难度，但是我们找一个具体的实例，就会理解起来比较容易。我们定义一个动物的抽象类Animal，再定义一个继承自Animal基类的Dog类，看下面的代码就会理解抽象类和接口不能被实例化：

```java
public class Test{  
    public static void main(String[] args){  
        Animal a1 = new Animal();//编译出错  
        Animal a2 = new Dog();  
    }  
}  
  
abstract class Animal{  
    //动物名字  
    String name;  
      
    //动物叫声  
    public void shout(){  
        System.out.println("叫声...");  
    }  
}  
  
class Dog extends Animal{  
    //狗类独有的方法  
    public void guard(){  
        System.out.println("狗有看门的独特本领！");  
    }
```




![](https://img-blog.csdn.net/20180710213347569?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM5NTIxNTU0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)






List list;//正确，list = null; 
List list = new List();//是错误的用法


List list = new ArrayList();

这句创建了一个ArrayList实现类的对象后把它上溯到了List接口。此时它就是一个List对象了，它有些ArrayList类具有的，但是List接口没有的属性和方法，它就不能再用了。 而ArrayList list=newArrayList();创建一对象则保留了ArrayList的所有属性和方法。 



