# Java中的类继承与构造方法初始化顺序 - 小灰笔记 - CSDN博客





2017年03月18日 10:29:57[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：237








       关于继承的概念我的基础一向是相对薄弱的，这部分了解的不多，使用的更少。关于面向对象，我了解的或许更少，现在比较深的概念只有一个对象拥有一些属性和方法。

       重新学习了一下Java中类的继承以及构造函数的的调用时刻，通过代码实现总结一下，代码如下：

**package**DemoConstructor;



**class** A 

{

    A()

    {

        System.***out***.println("fucntion in A");

    }



    A(**int**
par)

    {

        System.***out***.println("parameter function in A with " +
par);

    }

}



**class** B **extends** A

{

    B()

    {

        System.***out***.println("function in B");

    }

    B(**int**
par)

    {

**super**(12);

        System.***out***.println("parameter function in B with "+par);

    }

}



**class** C **extends** B

{

    C()

    {

        System.***out***.println("fucntion in C");

    }

    C(**int**
par)

    {

**super**(123);

        System.***out***.println("parameter function run in C with" +
par);

    }

}



**publicclass** DemoConstuctor {



**publicstaticvoid** main(String[] 
args) {

// **TODO** Auto-generated method stub

        C c = **new** C();

        C C1 = **new** C(345);

    }



}

       上面的代码值得一说的是几个构造方法，代码中的构造方法全都没有返回值，这种使用方法类似于C语言中的函数隐式声明。不过，这里的方法似乎可以省略的是返回为空的方式。

       代码的编译执行结果：

fucntionin A

functionin B

fucntionin C

parameterfunction in A with 12

parameterfunction in B with 123

parameter function run in C with 345

    从上面的结果可以看出：

    1，创建一个对象的时候，无参数构造方法会从最原始的父类开始依次被调用。

    2，如果执行有参数的构造方法，初始化的顺序与此类似。

    3，值得注意的一点：如果在子类中调用父类的有参数构造方法需要使用super并且放在构造方法的第一句。

    再次修改代码如下：

**package**DemoConstructor;



**class** A 

{

    A()

    {

        System.***out***.println("fucntion in A");

    }



    A(**int**
par)

    {

        System.***out***.println("parameter function in A with " +
par);

    }

}



**class** B **extends** A

{

    B()

    {

        System.***out***.println("function in B");

    }

    B(**int**
par)

    {

**super**(12);

        System.***out***.println("parameter function in B with "+par);

    }

}



**class** C **extends** B

{

    C()

    {

        System.***out***.println("fucntion in C");

    }

    C(**int**
par)

    {

//super(123);

        System.***out***.println("parameter function run in C with" +
par);

    }

}



**publicclass** DemoConstuctor {



**publicstaticvoid** main(String[] 
args) {

// **TODO** Auto-generated method stub

        C c = **new** C();

        C C1 = **new** C(345);

    }



}

       代码编译运行结果：

fucntionin A

functionin B

fucntionin C

fucntionin A

functionin B

parameterfunction run in C with 345

       通过上面的结果可以看出一个结论：如果子类没有指明调用父类的有参数构造方法的时候，所有的构造方法都会从最底层的父类开始按照无参数的构造方法进行初始化。



