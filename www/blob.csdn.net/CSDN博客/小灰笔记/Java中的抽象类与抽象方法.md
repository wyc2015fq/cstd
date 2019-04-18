# Java中的抽象类与抽象方法 - 小灰笔记 - CSDN博客





2017年03月19日 23:25:07[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：226








       基础语法与编程代码上值得一说的只有：抽象类或者抽象方法需要用abstract进行修饰。其它的都是抽象类与抽象方法的特殊用法，直接在简单的代码示范后再做总结：

**package**pckAbstrac;



**abstractclass** clsMenPai

{

**void** GongFu()

    {

        System.***out***.println("会功夫");

    }



**abstractvoid** JueJi();

}



**class** clsShaoLin
**extends** clsMenPai

{

**void** JueJi()

    {

        System.***out***.println("龙爪手");

    }

**void** NeiGong()

    {

        System.***out***.println("易筋经");

    }

}



**class** clsWuDang
**extends** clsMenPai

{

**void** JueJi()

    {

        System.***out***.println("太极拳");

    }

**void** NeiGong()

    {

        System.***out***.println("太极功");

    }

}



**publicclass** AbstracDemo {



**publicstaticvoid** main(String[] 
args) {

// **TODO** Auto-generated method stub

        clsMenPai men_pai1 = **new** clsShaoLin();

        clsMenPai men_pai2 = **new** clsWuDang();



men_pai1.GongFu();

men_pai2.GongFu();

    }



}

       代码编译运行结果：

会功夫

会功夫

       从运行结果可以得出结论：

       1，抽象类的方法不一定是抽象方法，再补充一下，抽象方法所在的类一定是抽象类；

       2，抽象类定义的变量可以用于指向子类对象，补充，但是本身不能够实例化；

       3，抽象类定义的变量可以用于访问抽象类中定义过的非抽象方法；

       修改代码如下，再次做测试：

**package**pckAbstrac;



**abstractclass** clsMenPai

{

**void** GongFu()

    {

        System.***out***.println("会功夫");

    }



**abstractvoid** JueJi();

}



**class** clsShaoLin
**extends** clsMenPai

{

**void** JueJi()

    {

        System.***out***.println("龙爪手");

    }

**void** NeiGong()

    {

        System.***out***.println("易筋经");

    }

}



**class** clsWuDang
**extends** clsMenPai

{

**void** JueJi()

    {

        System.***out***.println("太极拳");

    }

**void** NeiGong()

    {

        System.***out***.println("太极功");

    }

}



**publicclass** AbstracDemo {



**publicstaticvoid** main(String[] 
args) {

// **TODO** Auto-generated method stub

        clsMenPai men_pai1 = **new** clsShaoLin();

        clsMenPai men_pai2 = **new** clsWuDang();



men_pai1.GongFu();

men_pai2.GongFu();



men_pai1.JueJi();

men_pai2.JueJi();

    }



}

       代码编译执行结果：

会功夫

会功夫

龙爪手

太极拳

    从结果可以得出除了第一次测试的结论外，还能够得出以下结论：

        1，抽象类定义的变量可以用于访问已经在子类中实现的在父类中的抽象方法；

    再次修改代码如下;

**package**pckAbstrac;



**abstractclass** clsMenPai

{

**void** GongFu()

    {

        System.***out***.println("会功夫");

    }



**abstractvoid** JueJi();

}



**class** clsShaoLin
**extends** clsMenPai

{

**void** JueJi()

    {

        System.***out***.println("龙爪手");

    }

**void** NeiGong()

    {

        System.***out***.println("易筋经");

    }

}



**class** clsWuDang
**extends** clsMenPai

{

**void** JueJi()

    {

        System.***out***.println("太极拳");

    }

**void** NeiGong()

    {

        System.***out***.println("太极功");

    }

}



**publicclass** AbstracDemo {



**publicstaticvoid** main(String[] 
args) {

// **TODO** Auto-generated method stub

        clsMenPai men_pai1 = **new** clsShaoLin();

        clsMenPai men_pai2 = **new** clsWuDang();



men_pai1.GongFu();

men_pai2.GongFu();



men_pai1.JueJi();

men_pai2.JueJi();



men_pai1.NeiGong();

men_pai2.NeiGong();

    }



}

       这段代码直接不能够通过编译，提示相应的方法未定义。其实，这个问题还是可以用之前我自己总结出来的标签模型来理解一下。接下来，对代码修改如下：

**package**pckAbstrac;



**abstractclass** clsMenPai

{

**void** GongFu()

    {

        System.***out***.println("会功夫");

    }



**abstractvoid** JueJi();

}



**class** clsShaoLin
**extends** clsMenPai

{

**void** JueJi()

    {

        System.***out***.println("龙爪手");

    }

**void**
NeiGong()

    {

        System.***out***.println("易筋经");

    }

}



**class** clsWuDang
**extends** clsMenPai

{

**void** JueJi()

    {

        System.***out***.println("太极拳");

    }

**void** NeiGong()

    {

        System.***out***.println("太极功");

    }

}



**publicclass** AbstracDemo {



**publicstaticvoid** main(String[] 
args) {

// **TODO** Auto-generated method stub

        clsMenPai men_pai1 = **new** clsShaoLin();

        clsMenPai men_pai2 = **new** clsWuDang();



men_pai1.GongFu();

men_pai2.GongFu();



men_pai1.JueJi();

men_pai2.JueJi();



        ((clsShaoLin)men_pai1).NeiGong();

        ((clsWuDang)men_pai2).NeiGong();

    }



}

       代码的编译运行结果：

会功夫

会功夫

龙爪手

太极拳

易筋经

太极功

    这部分就比较容易理解了，这在之前的多态与动态绑定的小结博客中已经做了相应的总结了。



