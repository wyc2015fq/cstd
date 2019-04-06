## [JavaScript学习总结(十三)——极简主义法编写JavaScript类](https://www.cnblogs.com/xdp-gacl/p/3701921.html)

　　前两天在网上无意中发现了一篇使用极简主义法定义JavaScript类的文章，[原文链接](http://www.ruanyifeng.com/blog/2012/07/three_ways_to_define_a_javascript_class.html)，这个所谓的"极简主义法"我还是第一次听说，是荷兰程序员Gabor  de  Mooij提出来的，这种方法不使用this和prototype，代码部署起来非常简单，这大概也是它被叫做"极简主义法"的原因。下面就介绍如何使用极简主义法完成JavaScript的封装和继承

## **1. 封装**

　　首先，它也是用一个对象模拟"类"。在这个类里面，定义一个构造函数createNew()，用来生成实例。

```
1 var Cat = {
2 　　　　createNew: function(){
3 　　　　　　// some code here
4 　　　　}
5 };
```

　　然后，在createNew()里面，定义一个实例对象，把这个实例对象作为返回值。

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
1 var Cat = {
2 　　　　createNew: function(){
3 　　　　　　var cat = {};
4 　　　　　　cat.name = "大毛";
5 　　　　　　cat.makeSound = function(){ alert("喵喵喵"); };
6 　　　　　　return cat;
7 　　　　}
8 };
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

　　使用的时候，调用createNew()方法，就可以得到实例对象。

```
1 var cat1 = Cat.createNew();
2 cat1.makeSound(); // 喵喵喵
```

　　这种方法的好处是，容易理解，结构清晰优雅，符合传统的"面向对象编程"的构造，因此可以方便地部署下面的特性。

## **2. 继承**

　　让一个类继承另一个类，实现起来很方便。只要在前者的createNew()方法中，调用后者的createNew()方法即可。

　　先定义一个Animal类：

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
1 var Animal = {
2 　　　　createNew: function(){
3 　　　　　　var animal = {};
4 　　　　　　animal.sleep = function(){ alert("睡懒觉"); };
5 　　　　　　return animal;
6 　　　　}
7 };
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

　　然后，在Cat的createNew()方法中，调用Animal的createNew()方法

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
1 var Cat = {
2 　　　　createNew: function(){
3 　　　　　　var cat = Animal.createNew();
4 　　　　　　cat.name = "大毛";
5 　　　　　　cat.makeSound = function(){ alert("喵喵喵"); };
6 　　　　　　return cat;
7 　　　　}
8 };
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

　　这样得到的Cat实例，就会继承Animal类。

```
1 var cat1 = Cat.createNew();
2 cat1.sleep(); // 睡懒觉
```

## **3. 私有属性和私有方法**

　　**在createNew()方法中，只要不是定义在cat对象上的方法和属性，都是私有的。**

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
 1 var Cat = {
 2 　　　　createNew: function(){
 3 　　　　　　var cat = {};
 4 　　　　　　var sound = "喵喵喵";//私有属性
 5 　　　　　　cat.makeSound = function(){ 
 6                     alert(sound); 
 7                 };
 8 　　　　　　return cat;
 9 　　　　}
10 };    
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

　　上例的内部变量sound，外部无法读取，只有通过cat的公有方法makeSound()来读取。

```
1 var cat1 = Cat.createNew();
2 alert(cat1.sound); // undefined
```

## **4. 数据共享**

　　有时候，我们需要所有实例对象，能够读写同一项内部数据。这个时候，只要把这个内部数据，封装在类对象的里面、createNew()方法的外面即可。

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
1 var Cat = {
2 　　　　sound : "喵喵喵",
3 　　　　createNew: function(){
4 　　　　　　var cat = {};
5 　　　　　　cat.makeSound = function(){ alert(Cat.sound); };
6 　　　　　　cat.changeSound = function(x){ Cat.sound = x; };
7 　　　　　　return cat;
8 　　　　}
9 };
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

　　然后，生成两个实例对象：

```
1 var cat1 = Cat.createNew();
2 var cat2 = Cat.createNew();
3 cat1.makeSound(); // 喵喵喵
```

这时，如果有一个实例对象，修改了共享的数据，另一个实例对象也会受到影响。

```
1 cat2.changeSound("啦啦啦");
2 cat1.makeSound(); // 啦啦啦
```

 　　极简主义，看起来很美好，但是也有缺点，首先是不能使用instanceof 判断对象所属的类，"cat1 instanceof  Cat"无法通过，另外，极简主义虽然摆脱了使用原型链的缺点（属性不能私有、创建、继承对象不够直观），但也暴露了没用原型链的弊端：每一次生成一个实例，都必须为重复的内容，多占用一些内存。



分类: [JavaScript](https://www.cnblogs.com/xdp-gacl/category/431982.html)