# 一个小栗子聊聊 JAVA 泛型基础 - 文章 - 伯乐在线
本文作者： [伯乐在线](http://blog.jobbole.com) - [Float_Lu](http://www.jobbole.com/members/5334670325) 。未经作者许可，禁止转载！
欢迎加入伯乐在线 [专栏作者](http://blog.jobbole.com/99322)。
# 背景
周五本该是愉快的，可是花了一个早上查问题，为什么要花一个早上？我把原因总结为两点：
- 日志信息严重丢失，茫茫代码毫无头绪。
- 对泛型的认识不够，导致代码出现了BUG。
第一个原因可以通过以后编码谨慎的打日志来解决，我们今天主要来一起回顾下 JAVA 泛型基础。
# 一个小栗子
先看下面一个例子，test1实例化一个List容器的时候没有指定泛型参数，那么我们可以往这个容器里面放入任何类型的对象，这样是不是很爽？但是**当我们从容器中取出容器中的对象的时候我们必须小心翼翼，因为容器中的对象具有运行时的类型信息，这意味着你不能够将一个带有运行时类型信息的对象赋值给另一个类型，否则ClassCastException**。


```
@ Test
public void test1() throws Exception {
    List list = new ArrayList();
    list.add("float.lu");
    list.add(1);
	
    String name = (String) list.get(0);
    int num = (Integer) list.get(1);
    System.out.println(String.format("name[%s], num[%s]", name, num));
}
```
上面的代码没问题，可以很好地编译和运行通过，问题是我必须要事先很清楚地知道容器中的索引为0的对象是什么类型，索引为1的对象是什么类型，很显然，这在实际应用中是不切实际的，也是一种很不靠谱的做法，那么这个问题如何解决呢？泛型。
# 引入泛型
为了解决这个问题，我们引入泛型，下面代码可以看出与上面不同的是我们在实例化容器的时候加了<String>这个东西，这个东西的学名叫做**泛型参数**，就像普通方法带有参数一样，interface List<E>中的E为形式参数、而String为实参。


```
@ Test1
public void test2() throws Exception {
    List<String> list = new ArrayList<String>();
    list.add("a");
    list.add(1)//1
}
```
引入泛型后，我们规定这个容器中只能存放类型为字符串类型的对象，好的，编译器可以识别泛型并帮我们检查编译错误，上面的代码中1处会出现编译错误。注意：**泛型信息仅仅存在于编译期间，编译器可以通过泛型信息来对代码是否存在违规行为（编译错误）来进行检查，当编译器将代码编译为字节码之后，泛型信息将不复存在，然而对象的运行时信息仍然是有的，这就解释了为什么会出现ClassCastException。**
# 别高兴太早
有了泛型我们可以让代码安全地通过编译，并且我们认为他是安全的了，嘿嘿，是否就真的安全了呢？是否就能和ClassCastException说拜拜了呢？答案是：NO。看看下面这段代码：


```
@ Test
public void test3() throws Exception {
    List<String> list = new ArrayList<String>();
    list.add("a");
    list.add("b");
    List _list = list;
    List<Integer> integerList = _list;
    for (Integer item : integerList) {
        System.out.println(String.format("item[%s]", item));
    }
}
```
上面这段代码编译没有问题，我们没有直接将泛型参数为String的容器赋值给泛型参数为Integer的容器，而是花了点点小心思，我们现将list赋值给_list，_list生命为可以存储任何类型，也就相当于无特定类型，而后我们又把_list赋值给integerList容器，integerList容器被声明为只能存储类型为Integer的对象。悲催的是这段代码在运行的时候报了ClassCastException，很明显，我们知道在迭代integerList容器中的对象的时候，这些对象是有运行时类型信息的，当带有String类型信息的对象赋值给Integer的时候显然就报错了。这一切看起来似乎没问题，符合逻辑，但是有一个问题我们还没有问：为什么会没有编译错误？
# 泛型术语
在学习数学的时候我们往往会对一个证明题进行论证，而论证之前我们手上往往会有一些不需要证明的已知定理，下面这些“定理”将被用来直接回答上一节中遗留的问题。
- List<E>被称作泛型类型。
- List<E>中的E被称为类型变量或类型参数。
- List<String>被称为参数化类型。
- List<String>中的String被称为实际类型参数。
- List<E>中的<>年typeof。
- List被称为原始类型。
- 参数化类型可以引用一个原始类型对象，编译报告警告。
- 原始类型可以引用一个参数化类型对象，编译报告警告。
由上可知，List<Integer> integerList = _list;可以通过编译。
# 看清本质
经过上面的一些小波折，我们了解一些关于泛型的本质：泛型是给javac编译器使用的，javac是JAVA的编译器，而泛型可以让代码在编译期间确定类型安全，比如我们告诉编译器某个容器只能存储某种类型的对象，那么编译器会为我们好好地检查，确保类型安全，但是安全是相对的，只要我们逃过编译器，我们就有一百种方法让代码ClassCastException（比如反射）。同时编译之后参数化类型在运行时没有任何泛型信息，也就是为什么List.class和List<String>.class是同一个东西。除了参数化类型之外，容器中的对象在运行的时候是有类型信息的，也就是为什么会ClassCastExcetion。关于泛型还有很多内容，这里不做多讲，文中有误也欢迎留言讨论。
为学日益，为道日损，损之又损，以至于无为。无为而无不为，取天下常事以无事；及其有事，不足以取天下。
> 
**打赏支持我写出更多好文章，谢谢！**
[打赏作者](#rewardbox)
#### 打赏支持我写出更多好文章，谢谢！
任选一种支付方式
![](http://jbcdn2.b0.upaiyun.com/2016/05/7cb05a1a5b9ef06cc9d40cc79a096f293.png)![](http://jbcdn2.b0.upaiyun.com/2016/05/12e72c4df391cc981614cc68aedd44085.jpg)
