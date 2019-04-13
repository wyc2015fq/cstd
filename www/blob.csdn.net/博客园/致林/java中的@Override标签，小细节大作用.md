
# java中的@Override标签，小细节大作用 - 致林 - 博客园






# [java中的@Override标签，小细节大作用](https://www.cnblogs.com/bincoding/p/5725732.html)

转载：http://www.cnblogs.com/octobershiner/archive/2012/03/09/2388370.html
**一般用途**
帮助自己检查是否正确的复写了父类中已有的方法
告诉读代码的人，这是一个复写的方法
比如我们有如下基类
![复制代码](https://common.cnblogs.com/images/copycode.gif)
1 package fruit;
2 /**
3  * @author Octobershiner
4  */
5 public class Fruit {
6
7     public void show_name(int num){
8         System.out.println("Fruit: "+mum);
9     }
10
11     public static void main(String[] args) {
12         // TODO code application logic here
13         Fruit apple  = new Apple(); //generate a kind of new fruit
14         apple.show_name(2);
15     }
16 }![复制代码](https://common.cnblogs.com/images/copycode.gif)
之后我们编写一个Apple子类，继承这个基类。并且复写基类中的show_name()方法。
![复制代码](https://common.cnblogs.com/images/copycode.gif)
1 package fruit;
2
3 public class Apple extends Fruit{
4
5
6     @Override
7     public void show_name(int num){
8         System.out.println("Apple");
9     }
10 }![复制代码](https://common.cnblogs.com/images/copycode.gif)
执行的结果，显而易见就是会打印出Apple:2字样。
其实，在我们手工复写父类的方法时，容易把方法的参数记错，如果此时不加@Override的话，编辑器就不会提示你：例如我们不加这个标签，悄悄的把参数改为float型。
![](https://pic002.cnblogs.com/images/2012/321923/2012030920461828.png)
这个时候，其实我们并没有按照我们的意图成功复写方法，于是一个隐藏的bug就这样诞生了，相反加上Override的效果就是
![](https://pic002.cnblogs.com/images/2012/321923/2012030920485361.png)
IDE给出了错误提示，说明我们复写方法失败。
往往就是我们准备复写方法的时候结果，相反我们是重载了方法。
**Bruce的 一个思考**
《Thinking in java》的作者Bruce在讨论这个问题的时候，提到了一个问题就是override私有的方法的例子：
现在我们向Fruit类中添加一个私有方法，而在Apple中尝试复写
1         @Override
2         private void grow(){
3         }
结果编译器会提示错误，这是一个非常低级的错误，但是有时候恰恰就不会被我们发现：那就是试着复写私有方法，但是当我们去掉Override标签的时候，编译器是不会报错的，而且可以执行。
**其实Apple中的你所谓复写的grow只是一个针对于Apple本身的私有方法。完全是一个新的方法。**
这就引出了一个问题，何为复写？
在面向对象中，只有接口和共有方法，继承方法才有复写，私有方法不可以复习，但是又想了一下，才明白：**不是不可以复习而是，根本就不存在复写私有方法的概念！**
这正是面向对象设计的初衷，私有方法本身就是为了封装在类内部，不希望别人来更改或者外部引用的，看到这里，忽然觉得，java设计的还真是不错，感觉到了思想和实现的统一。
以前总觉得override标签可有可无，但没想到会引出这么多的问题，于是乎得到一个启示：认真思考每一个语法细节的意义，思行合一，文章的最后膜拜一下Bruce.





