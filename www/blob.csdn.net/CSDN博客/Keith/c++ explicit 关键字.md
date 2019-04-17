# c++ explicit 关键字 - Keith - CSDN博客





2014年02月28日 20:59:30[ke1th](https://me.csdn.net/u012436149)阅读数：524










C++中， 一个参数的[构造函数](http://baike.baidu.com/view/411124.htm)(或者除了第一个参数外其余参数都有默认值的多参构造函数)， 承担了两个角色。 1 是个[构造器](http://baike.baidu.com/view/552860.htm) ，2
 是个默认且隐含的类型转换操作符。


所以， 有时候在我们写下如 AAA = XXX， 这样的代码， 且恰好XXX的类型正好是AAA单参数[构造器](http://baike.baidu.com/view/552860.htm)的参数类型， 这时候[编译器](http://baike.baidu.com/view/487018.htm)就自动调用这个构造器，
 创建一个AAA的对象。


这样看起来好象很酷， 很方便。 但在某些情况下（见下面权威的例子）， 却违背了我们（[程序员](http://baike.baidu.com/view/39175.htm)）的本意。 这时候就要在这个构造器前面加上explicit修饰， 指定这个构造器只能被明确的调用，使用， 不能作为类型转换操作符被隐含的使用。 呵呵，
 看来还是光明正大些比较好。


explicit[构造函数](http://baike.baidu.com/view/411124.htm)的作用


解析：


explicit[构造函数](http://baike.baidu.com/view/411124.htm)是用来防止隐式转换的。请看下面的代码：


|123456789101112131415161718192021|`class````Test1````{``public``:``    ``Test1(``int````n) { num = n; }``````//普通构造函数````private``:``    ``int````num;````};``class````Test2````{``public``:``    ``explicit````Test2(````int````n) { num = n; }``````//explicit(显式)构造函数````private``:``    ``int````num;````};``int````main()````{``    ````Test1 t1 = 12;``````//隐式调用其构造函数, 成功````    ````Test2 t2 = 12;``````//编译错误,不能隐式调用其构造函数````    ````Test2 t3(12);``````//显式调用成功````    ``return````0;````}`|
|----|----|




Test1的[构造函数](http://baike.baidu.com/view/411124.htm)带一个int型的参数，代码17行会隐式转换成调用Test1的这个构造函数。而Test2的[构造函数](http://baike.baidu.com/view/411124.htm)被声明为explicit（显式），这表示不能通过隐式转换来调用这个构造函数，因此代码18行会出现编译错误。


普通[构造函数](http://baike.baidu.com/view/411124.htm)能够被[隐式调用](http://baike.baidu.com/view/2221626.htm)。而explicit[构造函数](http://baike.baidu.com/view/411124.htm)只能被显示调用。




