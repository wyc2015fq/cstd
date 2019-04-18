# Java的成员隐藏与属性封装 - 小灰笔记 - CSDN博客





2017年03月21日 23:38:34[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：839








       关于这个功能的优点我现在体会还不深，只是从书本中找到了面向对象编程的一个原则——尽量让数据私有。如果需要对数据的信息进行修改或者获取，不会直接修改成员信息，而是通过方法的调用来实现。

       写如下代码：

**package**fengzhuang;



**class** HuaShanPai

{

**private** String
zhangmen;

**privateint**dizishu;

**private** String
gongfu;



**public** String getZhangmen() {

**return**zhangmen;

    }

**publicvoid** setZhangmen(String zhangmen) {

**this**.zhangmen =
zhangmen;

    }

**publicint** getDizishu() {

**return**dizishu;

    }

**publicvoid** setDizishu(**int**dizishu) {

**this**.dizishu =
dizishu;

    }

**public** String getGongfu() {

**return**gongfu;

    }

**publicvoid** setGongfu(String gongfu) {

**this**.gongfu =
gongfu;

    }



}



**publicclass** FengZhuang {



**publicstaticvoid** main(String[] 
args) {

// **TODO** Auto-generated method stub

        HuaShanPai huashandizi =
**new** HuaShanPai();

huashandizi.setDizishu(123);

huashandizi.setGongfu("紫霞神功");

huashandizi.setZhangmen("岳不群");



        System.***out***.println("华山派弟子人数： " +
huashandizi.getDizishu());

        System.***out***.println("华山派功夫： " +
huashandizi.getGongfu());

        System.***out***.println("华山派掌门： " +
huashandizi.getZhangmen());

    }



}

       首先，关于如上代码值得一说的是方法的实现。其实，在eclipse中，实现get以及set的功能是集成了一个自动生成功能的。而代码中相应方法的实现使用的全都是这个功能。具体的功能再source的菜单栏中，选择生成getter以及setter选项即可弹出一个对话框。在对话框中勾选自己需要处理的属性即可。

       通过main方法中的代码可以看出，通过set函数实现了对私有属性的修改，同样，通过get函数实现了对修改后的属性的获取。当然，关于属性的获取应该是什么时候都是可以的。

       目前，暂时还不是很清楚这种功能在软件构件上有什么优点或者优势。如果有优势的话，其实在C语言中也可以仿出一些类似的功能，这种技术倒是可以变相拓展使用到现在工作的软件构建中去。

       至于把数据私有化，代码中使用了private关键字对属性定义进行了修饰。这样，直接通过一个对象的属性访问时无法对属性进行获取或者修改的。通过这种方式，对象所拥有的数据表现为一种私有状态。



