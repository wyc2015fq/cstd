# 第三课：js函数 - PeterBishop - CSDN博客





2018年12月07日 20:28:48[PeterBishop0](https://me.csdn.net/qq_40061421)阅读数：12
个人分类：[JavaScript](https://blog.csdn.net/qq_40061421/article/category/8510184)









  这节课开始我们来学jsa的函数


//这个用来写js函数相关的教学



*/** * js**函数的定义语法: * 语法1: * function 函数名(参数名1,参数名2,参数名3,...) * { *      函数体 * } * * 语法2: * var 函数名 = function(参数名1,参数名2,参数名3,...){ *      函数体 * } */***function **fun1(text) {

    //在浏览器的控制台输出

    console.log(text);

}



fun1(123);



**var **fun2 = **function **(i) {

    **return **i>0?1:0;

}



fun1(fun2(2));



//js中函数里还可以再定义函数，函数的作用域为函数定义的外一层大括号

**function **fun3() {

    **function **fun4() {

        **function **fun5() {

            fun1("这是fun5");

        }

        fun5()

        fun1("这是fun4");

    }

    //可以看到在这一层是调用不了fun5()的

    // fun5();

    fun4();

    fun1("这是fun3");

}

fun3();

fun1("这是最外层");



//注意： java里不允许在方法里继续定义方法，只允许在类里继续定义类




