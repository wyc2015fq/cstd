# Java多态中的引用类型转换理解模型 - 小灰笔记 - CSDN博客





2017年02月23日 00:35:28[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：634








       在我已经掌握的知识中，能够拿来类比的只有强制数据类型转换。不过，看完了慕课网的教程之后感觉两个东西很不一样。根据视频教程中的讲解，我得出一个结论：引用类的强制类型转换要么是信息全都减少的转换，要么是全都增加的转换，不能够既增加又减少。这似乎符合课程中给出的代码例子的执行表现。不过，我自己尝试写代码的时候发现这个结论还是错的。没办法，只能够多尝试一下，在探索中前进了。

测试1：

测试代码如下：

**package**exp02;



**publicclass** TestClass {



**publicstaticvoid** main(String[]
args) {


// **TODO** Auto-generated method stub


/* test 1*/

        Person 
person = **new** Person();

        ChinesePerson 
chinese_person = **new** ChinesePerson();


person.Speak();


chinese_person.Speak();


person = chinese_person;


person.Speak();

    }



}

代码编译执行结果：

Personcan speak!

Chineseperson speak Chinese!

Chinese person speak Chinese!

    由以上信息分析，如果仿照Python的解释思想，把创建出来的对象都理解为一个指向内存空间的标签。那么，父类创建的对象可以直接指向子类创建的对象。其实，这中间存在一个转换的问题，在教程中这被叫做向上的转换。其实，这个转换的语法表示不可以按照C语言的强制数据类型转换理解，最好还是按照Python教程中的标签模型来描述。因为标签的指向发生了改变之后，通过标签取对象的信息（不管属性还是方法）取出来的都是新的存储空间的信息。

测试2：

测试代码如下;

**package**exp02;



**publicclass** TestClass {



**publicstaticvoid** main(String[]
args) {


// **TODO** Auto-generated method stub


/* test 2*/

        Person 
person = **new** Person();


ChinesePersonchinese_person = **new**ChinesePerson();


person.Speak();


chinese_person.Speak();


chinese_person = (ChinesePerson)person;


chinese_person.Speak();

    }



}

执行结果：

Personcan speak!

Chineseperson speak Chinese!

Exceptionin thread "main"
java.lang.ClassCastException: exp02.Person cannot be cast to exp02.ChinesePerson

    atexp02.TestClass.main(TestClass.java:12)

    这里与测试1相比，不一样的地方主要是测试父类引用能否被强制转化为子类引用。在代码的编辑阶段，如果使用的是eclipse就可以发现，此时如果不使用强制转换代码本身在编辑阶段就会报错。如果使用了强制转换，编辑阶段到时不报错了，但是编译运行阶段又出现了错误。由于使用的是IDE，中间的很多过程细节我看不太清楚，不过从表现的结果上来看编译应该是通过了，从输出结果来看实际上程序已经执行并且输出了部分程序执行结果。只是，在程序执行到强制转换的时候出现了崩溃。

    尝试从内存数据存储上来思考理解一下，为什么测试1通过而测试2不能通过。假设还是按照Python类似的模型考虑，Person指向的内存中具有的属性在ChinesePerson存储中必然会存在。因为ChinesePerson继承自Person，只能够在Person的基础上修改或者增加信息，但是原来的信息框架必然是存在的。反过来理解的话，这个模型是不可行的。或许，这是程序执行出现错误的原因。

测试3：

本次的测试代码在编辑阶段会出现问题，因此不能够执行，从IDE截图代码如下：

![](https://img-blog.csdn.net/20170223003535544?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




    为什么第10行会出现错误呢？利用前面类比的标签模型是能够说明白的，那就是按照子类定制的标签到父类的存储区中获取信息不一定能够获取成功。

测试4：

为了进一步验证自己猜想的模型理论，修改代码如下：
![](https://img-blog.csdn.net/20170223003610862?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



    这应该说是再一次验证了自己之前的猜想，定制化的标签只能够提取标签上标记的信息。虽说ChinesePerson中我增加了age的属性，但是通过没有标记age的标签来提取ChinesePerson类型区域中的信息是不可实现的。

    通过以上的原理，再对测试2的代码问题进行修改，使其能够运行通过，代码应该如下：

**package**exp02;



**publicclass** TestClass {



**publicstaticvoid** main(String[] 
args) {

// **TODO** Auto-generated method stub

/* test 2*/

        Person person = **new** Person();

        Person chinese_person =
**new** ChinesePerson();

person.Speak();

chinese_person.Speak();

chinese_person = 
person;

chinese_person.Speak();

    }



}

执行结果:

Personcan speak!

Chineseperson speak Chinese!

Person can speak!

    以上是我对类型转换的一种理解方式，而在课程中给出了一个简单的关键字检查方法，那就是isinstaceof关键字。它可以检查一个对象是否是一个类型的实例化对象，如果不是的话说明转换不能够执行。具体的使用语法如下:

    Objectisinstanceof Class

    简单的读上去，相比之前我自己学习过的编程语言来看，这种语法方式跟Python中的某些语法很相似。表达的意义描述起来比较自然，很符合语言的表达逻辑，这是我非常喜欢的。




