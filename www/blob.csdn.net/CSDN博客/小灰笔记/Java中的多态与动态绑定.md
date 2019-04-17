# Java中的多态与动态绑定 - 小灰笔记 - CSDN博客





2017年03月18日 13:19:03[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：222
个人分类：[Java](https://blog.csdn.net/grey_csdn/article/category/6673076)









       关于Java的多态在慕课网上的教程中已经学到了，但是当时似乎没有看到有动态绑定这么个主题。虽说，动态绑定的相关功能再教程中确实是提到了。

       而Java中的多态一般是跟动态绑定放到一块儿说的，我个人的理解是这更加有助于理解Java类继承的相关技术。通过这种方法，可以更好地把多态与重载两个概念给区分开。接下来写一段代码总结一下：

**package**pkgWulin;



**class** JiangHuRen

{

    String Name;

**int**
age;

    String MenPai;



**void** KongFu()

    {

        System.***out***.println("江湖人会功夫");

    }

}



**class** ShaoLinDiZi
**extends** JiangHuRen



{

**void** KongFu()

    {

        System.***out***.println("会少林功夫");

    }

}



**class** WuDangDiZi
**extends** JiangHuRen

{

**void** KongFu()

    {

        System.***out***.println("会武当功夫");

    }

}



**class** EMeiDiZi
**extends** JiangHuRen

{

**void** KongFu()

    {

        System.***out***.println("会峨眉功夫");

    }

}



**publicclass** JiangHu {



**publicstaticvoid** main(String[] 
args) {

// **TODO** Auto-generated method stub

        JiangHuRen[] jiang_hu_ren_shi =
**new** JiangHuRen[3];



jiang_hu_ren_shi[0] = **new** ShaoLinDiZi();

jiang_hu_ren_shi[1] = **new** WuDangDiZi();

jiang_hu_ren_shi[2] = **new** EMeiDiZi();



jiang_hu_ren_shi[0].KongFu(); 


jiang_hu_ren_shi[1].KongFu(); 


jiang_hu_ren_shi[2].KongFu(); 


    }



}

       代码编译运行结果：

会少林功夫

会武当功夫

会峨眉功夫

    通过上面的运行结果可以得出结论：

    1，通过父类声明的对象课可以存储根据子类创建的对象；

    2，1中的对象拥有父类中覆写的方法（其他方法暂时看不出来）。

    修改代码如下：

**package**pkgWulin;



**class** JiangHuRen

{

    String Name;

**int**
age;

    String MenPai;



**void** KongFu()

    {

        System.***out***.println("江湖人会功夫");

    }

}



**class** ShaoLinDiZi
**extends** JiangHuRen



{

**void** KongFu()

    {

        System.***out***.println("会少林功夫");

    }



**void** YiJinJing()

    {

        System.***out***.println("会用易筋经");

    }

}



**class** WuDangDiZi
**extends** JiangHuRen

{

**void** KongFu()

    {

        System.***out***.println("会武当功夫");

    }

}



**class** EMeiDiZi
**extends** JiangHuRen

{

**void** KongFu()

    {

        System.***out***.println("会峨眉功夫");

    }

}



**publicclass** JiangHu {



**publicstaticvoid** main(String[] 
args) {

// **TODO** Auto-generated method stub

        JiangHuRen[] jiang_hu_ren_shi =
**new** JiangHuRen[3];



jiang_hu_ren_shi[0] = **new** ShaoLinDiZi();

jiang_hu_ren_shi[1] = **new** WuDangDiZi();

jiang_hu_ren_shi[2] = **new** EMeiDiZi();



jiang_hu_ren_shi[0].KongFu(); 


jiang_hu_ren_shi[1].KongFu(); 


jiang_hu_ren_shi[2].KongFu();



jiang_hu_ren_shi[0].YiJinJing(); 


    }



}

       这段代码在eclipse中会提示有错误，无法进行到编译以及运行的阶段。由此可以看出，利用父类声明的元素虽然可以用于存储子类，但是却无法访问子类的方法。我之前的博客中对此进行过模型化的理解思维，类就像是一个标签，在父类的标签上并没有写出子类的索引，因此无法找到相应的属性或者方法。而使用强制数据类型转换的意义在在于——根据箱子里面的内容增加标签上的标记。如此，修改代码如下：

**package**pkgWulin;



**class** JiangHuRen

{

    String Name;

**int**
age;

    String MenPai;



**void** KongFu()

    {

        System.***out***.println("江湖人会功夫");

    }

}



**class** ShaoLinDiZi
**extends** JiangHuRen



{

**void** KongFu()

    {

        System.***out***.println("会少林功夫");

    }



**void** YiJinJing()

    {

        System.***out***.println("会用易筋经");

    }

}



**class** WuDangDiZi
**extends** JiangHuRen

{

**void** KongFu()

    {

        System.***out***.println("会武当功夫");

    }

}



**class** EMeiDiZi
**extends** JiangHuRen

{

**void** KongFu()

    {

        System.***out***.println("会峨眉功夫");

    }

}



**publicclass** JiangHu {



**publicstaticvoid** main(String[] 
args) {

// **TODO** Auto-generated method stub

        JiangHuRen[] jiang_hu_ren_shi =
**new** JiangHuRen[3];



jiang_hu_ren_shi[0] = **new** ShaoLinDiZi();

jiang_hu_ren_shi[1] = **new** WuDangDiZi();

jiang_hu_ren_shi[2] = **new** EMeiDiZi();



jiang_hu_ren_shi[0].KongFu(); 


jiang_hu_ren_shi[1].KongFu(); 


jiang_hu_ren_shi[2].KongFu();



        ((ShaoLinDiZi)jiang_hu_ren_shi[0]).YiJinJing();  


    }



}

       代码编译运行结果如下：

会少林功夫

会武当功夫

会峨眉功夫

会用易筋经

       进一步总结知识点：

       1，父类声明而根据子类创建的对象访问子类中专有属性或者方法时需要强制数据类型转换；

       2，强制数据类型转换的时候，访问属性或者方法需要加括号以示其为一个对象整体。




上面提到的理解模型需要参考我之前的博客：http://blog.csdn.net/grey_csdn/article/details/56558021



