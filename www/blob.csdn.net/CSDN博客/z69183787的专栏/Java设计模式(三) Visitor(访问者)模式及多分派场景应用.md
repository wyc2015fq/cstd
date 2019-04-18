# Java设计模式(三) Visitor(访问者)模式及多分派场景应用 - z69183787的专栏 - CSDN博客
2017年10月18日 10:51:16[OkidoGreen](https://me.csdn.net/z69183787)阅读数：370
## 基本概念
### Visitor
- 封装一些作用于数据结构中的各元素的操作，不同的操作可以借助新的visitor实现，降低了操作间的耦合性
- 访问者可以将数据结构和对数据的操作解耦，使得增加对数据结构的操作不需要取修改数据结构，也不必去修改原有的操作，而执行时再定义新的Visitor时闲着就行了(在操作添加上易拓展)
### 模式中角色分工
- Visitor:抽象访问者，在重载的visit函数中声明访问者可以访问的对象。
- Concrete Visitor:实现一个访问者对于一个具体的元素的操作
- Element:抽象元素，声明具有访问该类型元素权限的访问者的类型(一般是抽象类型)，提供重载的accept函数赋予权限
- Concrete Element:实现accept方法，基本上是模板化的visitor.visit(this)
- Object Structure:容纳多种类型或许不同，接口或者不同的元素的集合。
## 例讲Visitor的实现
#### 先是一个简单的例子，展现一个最基本的简陋的Visitor
既然在春招季，我们举个简历筛选的例子，投简历的都是写本科生、专科生，还有硕士生、高职啊…为了简单就先取前两者。求职者的简历作为Element实现如下：
```java
abstract class Student {
　　 //提供对于数据域基本操作的函数
    private String name;
    private String university;
    private String rating;
    //让指定的visitor获得操作该对象的权限
    public abstract void accept(Visitor visitor);
    public String getName() {
        return name;
    }
    public void setName(String name) {
        this.name = name;
    }
    public String getUniversity() {
        return university;
    }
    public void setUniversity(String university) {
        this.university = university;
    }
    public String getRating() {
        return rating;
    }
    public void setRating(String rating) {
        this.rating = rating;
    }
}
class Bachelor extends Student{
    @Override
    public void accept( Visitor visitor ) {
        visitor.visit( this );
    }
}
class College extends Student{
    @Override
    public void accept(Visitor visitor) {
        visitor.visit(this);
    }
}
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
- 25
- 26
- 27
- 28
- 29
- 30
- 31
- 32
- 33
- 34
- 35
- 36
- 37
- 38
- 39
- 40
- 41
- 42
- 43
- 44
- 45
- 46
- 47
- 48
因为我们只定义了两种学生,所以接口提供了对于两种Element访问
```java
interface Visitor{
    public void visit ( Bachelor bachelor );
    public void visit ( College college );
}
```
- 1
- 2
- 3
- 4
首先筛选简历我们看一下大家的简历都什么样子，那么需要一个ShowVisitor:
```java
class ShowVisitor implements Visitor {
    @Override
    public void visit(Bachelor bachelor) {
        System.out.println("A bachelor\n");
        //TODO 可能会有一些特异的操作，我们为了简单就省略了
        this.printMessage( bachelor );
    }
    @Override
    public void visit(College college) {
        System.out.println(" a college student!\n");
        //TODO 同上
        this.printMessage( college );
    }
    public void printMessage ( Student student ){
        System.out.println( "Name : " + student.getName()+"\n"
                + "University : " + student.getUniversity()+"\n"
                + "Rating : " + student.getRating() + "\n"
        );
    }
}
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
要进行测试，我们首先要构造一个数据集合，也就是角色中对应的ObjectStructure,为了简单我们直接用ArrayList了
```java
public class VisitorEg {
    public static void main ( String [] args ){
        ArrayList<Student> list = new ArrayList<Student>();
        Bachelor bachelor = new Bachelor();
        bachelor.setName("llin");
        bachelor.setRating("100");
        bachelor.setUniversity("Tianjin University");
        College college = new College();
        college.setUniversity("Tianjin college");
        college.setRating("1");
        college.setName("dalinge");
        list.add ( bachelor );
        list.add ( college );
        Visitor visitor = new ShowVisitor();
        for ( Student student: list ){
            student.accept( visitor );
        }
    }
}
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
那么好像看不出访问者模式有什么优势啊!!!而且好费事啊，但是因为你将数据结构和对数据的操作分离了(解耦),所以当我想添加新的操作时，不需要修改原有的类，只需要重新实现一个visitor就可以了。
所以，我们回到这个例子，这么多人报名，那么到底有多少本科生呢(如果人数够了，可能直接偷懒只面试本科生了)，-_-万恶的这种HR，所以我们需要一个统计的Visitor:
```java
class SumVisitor implements Visitor{
    private int totalBachelor;
    SumVisitor(){
        super();
        totalBachelor = 0;
    }
    @Override
    public void visit(Bachelor bachelor) {
        totalBachelor++;
    }
    @Override
    public void visit(College college) {
    }
    public int getTotal_bachelor() {
        return totalBachelor;
    }
}
public class VisitorEg {
    public static void main ( String [] args ){
        ArrayList<Student> list = new ArrayList<Student>();
        Bachelor bachelor = new Bachelor();
        bachelor.setName("llin");
        bachelor.setRating("100");
        bachelor.setUniversity("Tianjin University");
        College college = new College();
        college.setUniversity("Tianjin college");
        college.setRating("1");
        college.setName("dalinge");
        list.add ( bachelor );
        list.add ( college );
        Visitor visitor = new ShowVisitor();
        Visitor visitor1 = new SumVisitor();
        for ( Student student: list ){
            student.accept( visitor );
            student.accept( visitor1);
        }
        System.out.println( "The total sum of bachelors : "+ ((SumVisitor)visitor1).getTotal_bachelor() );
    }
}
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
- 25
- 26
- 27
- 28
- 29
- 30
- 31
- 32
- 33
- 34
- 35
- 36
- 37
- 38
- 39
- 40
- 41
- 42
- 43
- 44
- 45
- 46
- 47
- 48
达到了要求，却没有修改一行代码，开心！
## Visitor应用场景
### 一定会有的疑问:visitor和iterator的区别:
- visitor可以访问不同的对象(只需要在Element定义对应的accept)，但是Iterator只能访问相同的对象，最起码要有相同的接口
- iterator是不依赖具体实现的，而visitor是依赖具体实现的，因为Visitor会根据访问的具体的对象来采取对应的操作，而iterator最多只是基于相同的接口的泛化实现。
- iterator访问的数据结构的操作和数据并未分离，所以拓展功能起来需要修改，违反了开闭原则和单一职责原则。但是因为访问者依赖具体实现，而不是依赖抽象，所以违反了依赖倒置原则
### 优缺点决定的应用场景
- 符合单一职责原则，功能上具有良好的拓展性，但是因为依赖具体实现违背了具体实现，所以为类的修改带了麻烦。
- 具有优良的拓展性，只需要实现新的Visitor来满足新的访问要求。因为数据和操作的分离，防止了添加新的操作污染原来的数据结构。
#### 综上
访问者是一种集中规整模式,特别适合用于大规模重构的项目，在这一个阶段的需求已经非常清晰，原系统的功能点也已经明确，通过访问者模式可以很容易把一些功能进行梳理，达到最终目的功能集中化
### 双分派
#### 首先介绍以下单分派
单分派:一个操作是根据请求者的名称和接收到的参数决定的，在Java有静态绑定和动态绑定，分别是通过重载和覆写实现的。 
双分派:双分派意味着得到执行的操作决定于请求的种类和接收者的类型。正对应于访问者模式。
#### Javac在构建、优化、解析语法树的时候就是采用的是Visitor模式(语法、语义分析阶段)
