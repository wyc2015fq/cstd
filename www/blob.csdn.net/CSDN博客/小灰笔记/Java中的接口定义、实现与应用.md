# Java中的接口定义、实现与应用 - 小灰笔记 - CSDN博客





2017年03月20日 23:03:28[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：169








       通过简单的学习，我觉得可以这么理解：接口实际上是一个类的框架，而类则是实例的模子。接口中定义的全都是最基本的框架，而且所定义的方法全都是抽象的、未实现的。简单写一段代码：

**package**pkgInterface;



**interface**MenPai

{

    String ***zu_zhi*** =
"五岳剑派";



**publicvoid** ZhangMen();

**publicvoid** JianFa();

}



**class** clsHuaShanPai
**implements** MenPai

{

**publicvoid** ZhangMen()

    {

        System.***out***.println("岳不群");

    }



**publicvoid** JianFa()

    {

        System.***out***.println("华山剑法");

    }

}



**class** clsHengShanPai**implements** MenPai

{

**publicvoid** ZhangMen()

    {

        System.***out***.println("莫大");

    }



**publicvoid** JianFa()

    {

        System.***out***.println("衡山剑法");

    }

}



**publicclass** clsInterface {



**publicstaticvoid** main(String[] 
args) {

// **TODO** Auto-generated method stub

        clsHuaShanPai linghuchong =
**new** clsHuaShanPai();

        clsHengShanPai liuzhengfeng =
**new** clsHengShanPai();



linghuchong.ZhangMen();

linghuchong.JianFa();



liuzhengfeng.ZhangMen();

liuzhengfeng.JianFa();

    }



}

       代码的编译运行结果：

岳不群

华山剑法

莫大

衡山剑法

    由此看，接口的使用也很简单。代码中数据描述层次合理性暂且不考虑，大致结论：

    1，接口中的方法只有public修饰和缺省两种情况，前者对全局可见，而后者包内可见；

    2，非抽象类中必须将接口中的全部方法实现（抽象类最终要在子类中实现）；

    在可实例化上，接口与抽象类的功能类似。一段相应的示例代码如下：

**package**pkgInterface;



**interface**MenPai

{

    String ***zu_zhi*** =
"五岳剑派";



**publicvoid** ZhangMen();

**publicvoid** JianFa();

}



**class** clsHuaShanPai
**implements** MenPai

{

**publicvoid** ZhangMen()

    {

        System.***out***.println("岳不群");

    }



**publicvoid** JianFa()

    {

        System.***out***.println("华山剑法");

    }

}



**class**
clsHengShanPai**implements** MenPai

{

**publicvoid** ZhangMen()

    {

        System.***out***.println("莫大");

    }



**publicvoid** JianFa()

    {

        System.***out***.println("衡山剑法");

    }

}



**publicclass** clsInterface {



**publicstaticvoid** main(String[] 
args) {

// **TODO** Auto-generated method stub

        MenPai linghuchong = **new** clsHuaShanPai();

        MenPai liuzhengfeng = **new**clsHengShanPai();



        ((clsHuaShanPai)linghuchong).ZhangMen();

        ((clsHuaShanPai)linghuchong).JianFa();



        ((clsHengShanPai)liuzhengfeng).ZhangMen();

        ((clsHengShanPai)liuzhengfeng).JianFa();

    }



}

       代码的编译运行结果如下：

岳不群

华山剑法

莫大

衡山剑法

    以上大致是接口的定义、实现以及简单的使用，算是最基本的。当然，接口中还有一个类对多接口的继承等更加复杂的功能，这里暂且不做总结。



