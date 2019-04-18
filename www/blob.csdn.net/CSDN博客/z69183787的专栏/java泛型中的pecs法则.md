# java泛型中的pecs法则 - z69183787的专栏 - CSDN博客
置顶2014年12月07日 20:17:58[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1634
***? extend***
|`1`|`List<Apple> apples = ``new ``ArrayList<Apple>();`|
|`2`|`List<? ``extends ``Fruit> fruits = apples;`|
|`3`|`fruits.add(``new ``Strawberry());`|
这次，代码就编译不过去了！Java编译器会阻止你往一个Fruit list里加入strawberry。在编译时我们就能检测到错误，在运行时就不需要进行检查来确保往列表里加入不兼容的类型了。即使你往list里加入Fruit对象也不行：
|`1`|`fruits.add(``new ``Fruit());`|
**你没有办法做到这些。事实上你不能够往一个使用了? extends的数据结构里写入任何的值。**
**原因非常的简单，你可以这样想：这个? extends T 通配符告诉编译器我们在处理一个类型T的子类型，但我们不知道这个子类型究竟是什么。因为没法确定，为了保证类型安全，我们就不允许往里面加入任何这种类型的数据。另一方面，因为我们知道，不论它是什么类型，它总是类型T的子类型，当我们在读取数据时，能确保得到的数据是一个T类型的实例：**
|`1`|`Fruit get = fruits.get(``0``);`|
***? super***
使用 ? super 通配符一般是什么情况？让我们先看看这个：
|`1`|`List<Fruit> fruits = ``new ``ArrayList<Fruit>();`|
|`2`|`List<? ``super ``Apple> = fruits;`|
我们看到fruits指向的是一个装有Apple的某种超类(supertype)的List。同样的，我们不知道究竟是什么超类，但我们知道Apple和任何Apple的子类都跟它的类型兼容。既然这个未知的类型即是Apple，也是GreenApple的超类，我们就可以写入：
|`1`|`fruits.add(``new ``Apple());`|
|`2`|`fruits.add(``new ``GreenApple());`|
如果我们想往里面加入Apple的超类，编译器就会警告你：
|`1`|`fruits.add(``new ``Fruit());`|
|`2`|`fruits.add(``new ``Object());`|
因为我们不知道它是怎样的超类，所有这样的实例就不允许加入。
从这种形式的类型里获取数据又是怎么样的呢？结果表明，你只能取出Object实例：因为我们不知道超类究竟是什么，编译器唯一能保证的只是它是个Object，因为Object是任何Java类型的超类。
**存取原则和PECS法则**
总结 ? extends 和 the ? super 通配符的特征，我们可以得出以下结论：
- 如果你想从一个数据类型里获取数据，使用 ? extends 通配符 
- 如果你想把对象写入一个数据结构里，使用 ? super 通配符 
- 如果你既想存，又想取，那就别用通配符。
