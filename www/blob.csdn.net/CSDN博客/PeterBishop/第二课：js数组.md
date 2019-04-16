# 第二课：js数组 - PeterBishop - CSDN博客





2018年12月07日 20:27:06[PeterBishop0](https://me.csdn.net/qq_40061421)阅读数：13
个人分类：[JavaScript](https://blog.csdn.net/qq_40061421/article/category/8510184)









  这节课来学js的数组，跟着文档学。


//这个用来写数组相关教学



//数组的概念就不说了，说下使用就行

*/** * **数组的创建: * 1. var arr = new Array(); * 2. var arr = new Array(n); //n为整数型 * 3. var arr = new Array(elem0,elem1,elem2,...); * 4. var arr = [elem0,elem1,elem2,...];    //还以为我记错了，js里确实有这种定义数组的语法 * * 数组的使用: * js的数组是不定长的，不过还是可能发生下标越界，因为虽然js的数组是不定长的，但还是有一个长度，只是这个长度可变而已 * 下标依然是从0开始 */***var **arr = **new **Array();

**for **(**var **i = 0; i < 10; i++) {

    arr[i] = i;

}



**var **arr2 = ["a","b","c"];



*/** * **这个是js的foreach写法，java里把in换成了: */***for **(**var **elem **in **arr)

{

    document.write(elem);

}

//js里的foreach还是有点问题。。大家以后就用for就行...

**for **(**var **i = 0; i < arr2.length; i++) {

    document.write(arr2[i]);

}




