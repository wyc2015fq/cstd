# JavaScript学习总结(四)——逻辑OR运算符详解 - 孤傲苍狼 - 博客园



# [孤傲苍狼](https://www.cnblogs.com/xdp-gacl/)

只为成功找方法，不为失败找借口！




## [JavaScript学习总结(四)——逻辑OR运算符详解](https://www.cnblogs.com/xdp-gacl/p/3677485.html)



在JavaScript中，逻辑OR运算符用||表示

```
1 var bTrue = true;
2 var bFalse = false;
3 var bResult = bTrue **||** bFalse;
```

下面的真值表描述了逻辑AND运算符的行为:
|运算数 1|运算数 2|结果|
|----|----|----|
|true|true|true|
|true|false|true|
|false|true|true|
|false|false|false|











在JavaScript中，**0，"",false,null,undefined,NaN均表示false，**

可以用如下的代码证明：

```
1   <script type="text/javascript">
 2      var bFalse = false;//bool类型
 3      var strEmpty = "";//空字符串
 4      var iZero = 0;//数是0
 5      var oNull=null;//null
 6      var oUndefined;//undifined
 7      var oNaN=NaN;//NaN
 8     /*
 9     判断JavaScript 变量的 Boolean 值时，也可以使用逻辑 NOT 运算符。这样做需要在一行代码中使用两个 NOT 运算符。
10     无论运算数是什么类型，第一个 NOT 运算符返回 Boolean 值。第二个 NOT 将对该 Boolean 值求负，从而给出变量真正的 Boolean 值。
11     */
12     document.write("<PRE>");
13     document.writeln("布尔数false的逻辑值是 " + (!!bFalse));
14     document.writeln("空字符串(\"\")的逻辑值是 " + (!!strEmpty));
15     document.writeln("数字0的逻辑值是 " + (!!iZero));
16     document.writeln("NaN的逻辑值是 ：" + (!!oNaN));
17     document.writeln("null的逻辑值是 " + (!!oNull));
18     document.writeln("undefined的逻辑值是 " + (!!oUndefined));
19     document.write("</PRE>");
20   </script>
```

运行结果：

![](data:image/png;base64,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)

　　JavaScript的**逻辑 OR 运算也是简便运算，对于逻辑 OR 运算符来说，如果第一个运算数值为 true，就不再计算第二个运算数**，**如果某个运算数不是 Boolean 值，逻辑 OR 运算并不一定返回 Boolean 值**，逻辑||的运算规则如下：
- **如果一个运算数是对象，另一个是 Boolean 值，返回该对象。 **
- **如果两个运算数都是对象，返回第一个对象。 **
- **如果某个运算数是 null，返回 null。 **
- **如果某个运算数是 NaN，返回 NaN。 **
- **如果某个运算数是 undefined，发生错误。**

**　　对于这些运算规则，没有必要死记硬背，因为在JavaScript中，可以使用逻辑Not运算符来判断JavaScript变量的Boolean值，判断的方式就是"!!变量名"，例如：**

**使用逻辑Not运算符来判断JavaScript变量的Boolean值**

```
1 <script type="text/javascript">
 2      var bFalse = false;//运算数是bool类型的数
 3      var sRed = "red";//运算数是字符串
 4      var iZero = 0;//运算数是0
 5      var iThreeFourFive = 345;//运算数是 0 以外的任何数字
 6      var oObject = new Object();//对象
 7      var oNull=null;//运算数是null
 8      var oUndefined;//运算数是undifined
 9      var oNaN=parseInt("abc");//使用parseInt方法把尝试字符串abc转换成整数，因为abc不是数字，因此是转不了的，所以返回的结果就是NaN
10     /*
11     判断JavaScript 变量的 Boolean 值时，也可以使用逻辑 NOT 运算符。这样做需要在一行代码中使用两个 NOT 运算符。
12     无论运算数是什么类型，第一个 NOT 运算符返回 Boolean 值。第二个 NOT 将对该 Boolean 值取反，从而给出变量真正的 Boolean 值。
13     */
14     document.write("<PRE>");
15     document.writeln("布尔数false 的逻辑值是 " + (!!bFalse));
16     document.writeln("字符串sRed 的逻辑值是 " + (!!sRed));
17     document.writeln("数字0 的逻辑值是 " + (!!iZero));
18     document.writeln("数字345 的逻辑值是 " + (!!iThreeFourFive));
19     document.writeln("对象Object 的逻辑值是 " + (!!oObject));
20     document.writeln("NaN的逻辑值是 ：" + (!!oNaN));
21     document.writeln("null 的逻辑值是 " + (!!oNull));
22     document.writeln("undefined 的逻辑值是 " + (!!oUndefined));
23     document.write("</PRE>");
24   </script>
```

判断结果：

![](https://images0.cnblogs.com/i/289233/201404/210041426355620.png)

逻辑||运算符测试脚本：

```
1  <script type="text/javascript">
 2   document.write("<PRE>");
 3   /*JavaScript中的||将返回第一个不为false的那个值(对象亦可)或者最后一个值(如果全部都为false的话)*/
 4     var a=2;
 5     var b=90;
 6     var c=a||b;//在JavaScript中，非0的数字就代表true,0就代表false
 7     document.writeln("a=2,b=90,c=a||b的结果是："+c);//结果为2，返回第一个不为false的那个值，所以值是2
 8 
 9     var bFalse1=false;
10     var bFalse2=false;
11     var num=0;//0代表false
12     var result=bFalse1||bFalse2||num;
13     document.writeln("bFalse1=false,bFalse2=false,num=0,result=bFalse1||bFalse2||num的结果是："+num);//如果全部都为false的话，返回最后一个值，因此结果是0
14 
15     /*如果一个运算数是对象，另一个是 Boolean 值，返回该对象。*/
16     var obj = new Object();
17     var bTrue=true;
18     var bFalse=false;
19     document.writeln("obj||bTrue的结果是："+(obj||bTrue));//返回object
20     document.writeln("bTrue||obj的结果是："+(bTrue||obj));//返回true,逻辑 OR 运算也是简便运算。对于逻辑 OR 运算符来说，如果第一个运算数值为 true，就不再计算第二个运算数。
21     document.writeln("obj||bFalse的结果是："+(obj||bFalse));//返回object
22     document.writeln("bFalse||obj的结果是："+(bFalse||obj));//返回object
23 
24     /*如果两个运算数都是对象，返回第一个对象*/
25     var obj1 = new Object();
26     var obj2 = new Object();
27     document.writeln("obj1==(obj1||obj2)的结果是："+(obj1==(obj1||obj2)));//结果为true
28     document.writeln("obj2==(obj1||obj2)的结果是："+(obj2==(obj1||obj2)));//结果为false
29 
30     /*如果某个运算数是 null，返回 null。*/
31     var c=1;
32     var d=null;
33     document.writeln("d=null,null的布尔值是；"+(!!d));
34     document.writeln("c=1，d=null,c||d的结果是："+(c||d));
35     document.writeln("c=1，d=null,d||c的结果是："+(d||c));
36     
37     var o=new Object();
38     document.writeln("o是一个对象，d=null,o||d的结果是："+(o||d));//返回object
39     document.writeln("o是一个对象，d=null,d||o的结果是："+(d||o));//返回object
40     
41     var zero=0;
42     document.writeln("zero=0，d=null,zero||d的结果是："+(zero||d));//返回null
43     document.writeln("zero=0，d=null,d||zero的结果是："+(d||zero));//返回0
44 
45     var bTrue = true;
46     var bResult = (bTrue || bUnknown);
47     /*变量 bUnknown 是未定义的。不过，由于变量 bTrue 的值为 true，bUnknown 不会被计算，因此输出的是 "true"。*/
48     document.writeln("bUnknown是未定义的一个变量，bResult = (bTrue || bUnknown)的结果是："+bResult);//输出 "true"
49 
50      bFalse = false;
51      bResult = (bFalse || bUnknown);    //发生错误
52     document.writeln(bResult);            //不会执行这一行
53     document.write("</PRE>");
```

运行结果：

![](https://images0.cnblogs.com/i/289233/201404/210019291036753.png)









