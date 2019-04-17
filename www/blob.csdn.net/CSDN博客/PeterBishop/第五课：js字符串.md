# 第五课：js字符串 - PeterBishop - CSDN博客





2018年12月09日 17:02:28[PeterBishop0](https://me.csdn.net/qq_40061421)阅读数：16
个人分类：[JavaScript](https://blog.csdn.net/qq_40061421/article/category/8510184)









  这节课我们来讲js的字符串



```
//这课写js字符串相关的教学



/**

 * js里字符串可以被""或''括起来

 */



var str1 = "123";

var str2 = '456';



function print(variable) {

    document.write(variable);

}



print(str1);

print(str2);



//在字符串中包含引号

var str3 = "'12300'";

var str4 = '"456"';

print(str3);

print(str4);



print("<br>");

var str5 = '123"4567\'8910\'"';

print(str5);

print("<br>");

//获取字符串长度

print(str5.length);



//字符串是一个对象，内置了很多方法，基本和java里的String类的方法类似，

// 大家也可以查JavaScript API文档
```





