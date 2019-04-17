# Java类继承中的方法覆写overload - 小灰笔记 - CSDN博客





2017年03月18日 11:28:48[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：216
个人分类：[Java](https://blog.csdn.net/grey_csdn/article/category/6673076)









       方法的覆写与多态在一定程度上相似，都是对一个同名方法的行为进行修改。不一样的是，覆写是出现在不同类的继承中，而且方法不仅仅是同名而且参数都一致。

       示例代码：

**package**MethodOverWrite;



**class** KongFu

{

**void** fight()

    {

        System.***out***.println("Kongfu can fight");

    }

}



**class** TaiJi **extends** KongFu

{

**void** fight()

    {

        System.***out***.println("fight with TaiJiQuan");

    }

}



**class** BaGuaZhang
**extends** KongFu

{

**void** fight()

    {

**super**.fight();

        System.***out***.println("fight with BaGuaZhang");

    }

}



**publicclass** MethodOverWrite {



**publicstaticvoid** main(String[] 
args) {

// **TODO** Auto-generated method stub

        TaiJi yangluchan = **new** TaiJi();

        BaGuaZhang donghaichuan =
**new** BaGuaZhang();

yangluchan.fight();

donghaichuan.fight();

    }



}

       代码运行结果：

fightwith TaiJiQuan

Kongfucan fight

fight with BaGuaZhang

    值得注意的是，在eclipse中进行方法的覆写（overload）的时候，IDE会给出一个提示。或许，这种方法不常用？从上面的程序以及运行结果得出的结论以及知识点：

    1，方法是可以覆写的；

    2，覆写后的类创建的对象调用方法时调用的是覆写后的新方法；

    3，如果需要调用旧的方法，需要使用super。



