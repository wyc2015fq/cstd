## [JavaScript学习总结(十)——this关键字](https://www.cnblogs.com/xdp-gacl/p/3699398.html)

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
 1 <script type="text/javascript">
 2     function Person(){
 3         /*使用var 属性名定义的属性是类的私有属性，外界无法访问，要想被外界访问，就要使用公共方法*/
 4         var QQ="123456"; 
 5         var Email="123456@qq.com";
 6 
 7         /*使用this.属性名定义的属性就是类的公共属性，是可以被外界访问的*/
 8         this.Name="孤傲苍狼";
 9         this.Age=24;
10 
11         /*定义Person类的公共方法，凡是定义类的公共属性和公共方法都要使用this*/
12         this.Show=function(){//定义Person类的公共方法(特权方法)
13             alert("QQ="+QQ+"\tEmail="+Email);
14 
15         }
16         
17         /*定义Person类的私有方法*/
18         function Show2(){//这是Person类的一个私有方法，只能在Person类内部使用
19             
20         }
21     }
22 
23     var p1 = new Person();
24     alert("私有属性p1.QQ="+p1.QQ+"\t"+"p1.Email="+p1.Email);
25     alert("公共属性p1.Name="+p1.Name+"\t"+"p1.Age="+p1.Age);
26     p1.Show();
27     //p1.Show2();//这里会报错，无法调用Person类的私有方法，
28     
29     /*
30     Window 对象描述
31     Window 对象表示一个浏览器窗口或一个框架。在客户端 JavaScript 中，Window 对象是全局对象，所有的表达式都在当前的环境中计算。
32     也就是说，要引用当前窗口根本不需要特殊的语法，可以把那个窗口的属性作为全局变量来使用。例如，可以只写 document，而不必写 window.document。
33 
34     同样，可以把当前窗口对象的方法当作函数来使用，如只写 alert()，而不必写 Window.alert()。
35     */
36     function Test(){
37         alert(this.v);
38     }
39     v=90;
40     Test();
41     window.Test();//记住一句话：哪个对象调用this所在的函数，那么this代表的就是哪个对象实例，这里是window对象调用Test()方法，因此此时this指的就是window对象
42 
43     function Test2(){
44         this.fnTest=function(){
45             alert(this.v2);
46         }
47     }
48 
49     var t = new Test2();
50     t.v2="记住一句话：哪个对象调用this所在的函数，那么this代表的就是哪个对象实例";
51     t.fnTest();//这里是t对象调用Test()方法，因此此时this指的就是t对象
52   </script>
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

**JavaScript的this总结：**

 　　1、this不能在类定义的外部使用，只能在类定义的内部使用。

　　 2、哪个对象调用this所在的函数，那么this代表的就是哪个对象实例。



分类: [JavaScript](https://www.cnblogs.com/xdp-gacl/category/431982.html)