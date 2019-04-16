# JavaScript学习总结(十五)——Function类 - 孤傲苍狼 - 博客园



# [孤傲苍狼](https://www.cnblogs.com/xdp-gacl/)

只为成功找方法，不为失败找借口！




## [JavaScript学习总结(十五)——Function类](https://www.cnblogs.com/xdp-gacl/p/3703776.html)



　　在JavaScript中，函数其实是对象，每个函数都是Function类的实例，既然函数对象，那么就具有自己的属性和方法，因此，函数名实际上也是一个指向函数对象的指针，不会与某个函数绑定。

## 一、函数的声明

方式一：常规方式

```
1 function sum1(num1,num2){
2      return num1+num2   
3 }
```

方式二：函数表达式

```
1 var sum2=function(num1,num2){
2      return num1+num2;         
3 };
```

方式三：动态创建函数(这种方式用得不多)

```
1 var sum3=new Function("num1","num2","return num1+num2");//动态创建函数
```

测试：

```
1  <script type="text/javascript">
 2     function sum1(num1,num2){
 3          return num1+num2   
 4     }
 5     var sum2=function(num1,num2){
 6          return num1+num2;         
 7     };
 8     var sum3=new Function("num1","num2","return num1+num2");
 9     //测试
10     document.write("<pre>");
11     document.writeln("sum1(1,2)="+sum1(1,2));
12     document.writeln("sum2(1,2)="+sum2(1,2));
13     document.writeln("sum3(1,2)="+sum3(1,2));
14     document.write("</pre>");
15 </script>
```

测试结果：

![](data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAF0AAAA5CAIAAADbdrDYAAACNklEQVRoge1Yy3IDIQzj/386vaRbamQh81g4WAcmQ4QwMhCH8kkglNMBXIr0BSN9wUhfMNIXjPQFI33BOOZLKWumXqVjZXeI/qk7Qbf94vLKLwbGesMxU1QcAImgXZgUa8XxPg8Mx2RFcRhwer2TcIa3jMhnX5cKn/9ZNZ3e/lzrC+Eby0hUs+cIJod01q0XfbRTiW1guMLv7xdP7pQvO7YbIIQkjvuiXOT8HInTHThHsF+5BXS1Lm3KF3JvtQl52tovMV0t0/MFRhU6g15IlikqLkc/Y3MnaxI3/j8SU7o3hrPTX4v0BSN9wbj33n1Zx8ruEP1KR8oq/dcHlg7zIVmmqBgFry+UKk7XfLWumwQJQtxB85qiFCbwwaaiNbWs4XiTrfJF1CwN2kX19ZWJazu8zrolC4A9XqcS28BwhZ/vDA4hJBHyRTxWYqC6Jj9H4nS7zhGfuHsQ4HpCRnPalC/k3moT8rT1By9jnqbpISGFFkkmcpmi4nL0M5bvDAZiSvfGcHb6a5G+YKQvGPfeuy/rWNkdol/pSFml//rA0mE+JMsUFaPg9YVSxemar9Z1C6Gkd8YXcXiIv/edQSl2xUA5v62VvahmzxFMDumsW30NvFOJbWC4wt/7zgCDmPRlx3YDhJDEcV88mn6OxOneeGdQtr1yCyimEPCQLJmokHurTcjT1taI6WqZni8wqtAZ9EKyTFFxOfoZy3cGAzGle2M4O/21SF8w0heM9AUjfcFIXzDSF4wfYld2nb+4TCUAAAAASUVORK5CYII=)

　　这里顺便提一下方式一和方式二这两种函数声明方式的区别，对于采用方式一声明的函数，解析器会率先读取函数声明，并使其置于任何代码之前；而对于函数表达式，则必须等到解析器执行到它所在的代码行，才别执行。

可以用如下的代码证明：

```
1     alert("sum1(1,2)="+sum1(1,2));//可以正常运行
2     function sum1(num1,num2){
3          return num1+num2   
4     }
5     alert("sum2(1,2)="+sum2(1,2));//错误：缺少对象
6     var sum2=function(num1,num2){
7          return num1+num2;         
8     };
```

## 二、函数重载

　　JavaScript没有方法重载的说法，如果两个方法名字一样，即使参数个数不一样，那么后面定义的就会覆盖前面定义，调用方法时永远是调用后定义的那个。

**用如下的代码证明JavaScript不支持函数的重载：**

```
1 <script type="text/javascript">
 2     function Test(a){
 3         alert(a);
 4     }
 5     function Test(a,b){
 6         alert("HelloWorld!");
 7     }
 8     function Test(a,b){
 9         alert(a+" "+b);
10     }
11 
12     Test(20);//调用的是最后定义的那个Test方法
13     Test(30,50);//调用的是最后定义的那个Test方法
14 </script>
```

`　　把函数名想像成指针，这例子中声明了三个同名函数，最后声明的一个就会覆盖了前面函数，用下面的写法或许更好理解：`

```
1 /*声明一个变量Test，Test变量中存放了指向函数对象的指针(引用)*/
 2     var Test =function (a){
 3         alert(a);
 4     }
 5 
 6     /*将变量Test指向了另一个函数对象*/
 7     Test = function (a,b){
 8         alert("HelloWorld!");
 9     }
10     /*将变量Test指向了另一个函数对象*/
11     Test = function (a,b){
12         alert(a+" "+b);
13     }
```

## 三、JavaScript函数的特殊性

　　JavaScript中的函数名本身就是变量，所以函数也可以当作普通变量来使用。也就是说，不仅可以像传递参数一样把一个函数传递给另一个函数，而且可以将一个函数作为另一个函数的结果返回。

例如：在使用构造函数给一个对象初始化属性时，指定函数属性

```
1  <script type="text/javascript">
 2     function fn(num1,num2,oper){
 3         var  result=-1;
 4         switch(oper){
 5             case "+":
 6                 result=num1+num2;
 7                 break;
 8             case "-":
 9                 result=num1-num2;
10                 break;
11             case "*":
12                 result=num1*num2;
13                 break;
14             case "/":
15                 result=num1/num2;
16                 break;
17         }
18         return result;
19     }
20     /*创建Person类*/
21     function Person(name,age,fn){
22         this.Name=name;
23         this.Age=age;
24         this.Fn=fn;//使用传递进来的fn函数为函数属性Fn初始化
25     }
26     /*在使用构造函数给一个对象初始化属性时，也可以指定函数属性*/
27     var p1 = new Person("孤傲苍狼",24,fn);
28     var retVal=p1.Fn(1,2,"+");
29     alert(retVal);
30   </script>
```

 　　在函数内部，有两个特殊的对象：arguments和this,arguments是一个数组对象，包含传入的所有参数，arguments的主要作用是保存函数参数，但这个对象还有个叫callee的属性，该属性是一个指向拥有这个arguments对象的函数。

```
1 //非常经典的递归函数
 2 function factoriak(num){
 3      if(num<=1){
 4      return 1;   
 5   }else{
 6      return num * facaorial(num-1);//与函数名factoriak耦合性太高了
 7   }   
 8 }
 9 //上述代码与函数名耦合性太高，一换函数名就不行了，就可以采用以下方法
10 function factoriak(num){
11      if(num<=1){
12      return 1;   
13   }else{
14      return num * arguments.callee(num-1);//这样无论用什么名字都能完成递归调用 
15  }
16 }
```

　　this指的是函数执行时所处的作用域

　　每个函数都包含：length和prototype。length属性表示函数希望接收的参数个数。

```
1     function sayName(name){
 2         alert(name);
 3     }
 4     function sum(num1,num2){
 5         return num1+num2;
 6     }
 7     function sayHi(){
 8         alert("hi");
 9     }
10     alert("sayName函数接收的参数个数是："+sayName.length);//1
11     alert("sum函数接收的参数个数是："+sum.length);    //2
12     alert("sayHi函数接收的参数个数是："+sayHi.length);//0
```

　　每个函数都包含两个非继承而来的方法：apply()和call();这两个方法的用途都是在特定的作用域中调用函数，实际上等于设置函数体内this对象的值

　　apply()方法接收两个参数：一个是在其中运行函数的作用域，另一个是参数数组。其中第二个参数可以是Array的实例，也可以是arguments对象。

```
1 function sum(num1,num2){
 2     return num1+num2;
 3 }
 4 function callSum(num1,num2){
 5     return sum.apply(this,arguments);  //传入aguments对象
 6 }
 7 function callSum2(num1,num2){
 8     return sum.apply(this,[num1,num2]);  //传入数组
 9 }
10 alert(callSum(10,10));  //20
11 alert(callSum2(10,10)); //20
```

　　call()方法接第一个参数是函数运行的作用域，其余的参数就是传递给函数运行时需要的参数(一个或多个)。

```
function sum(num1,num2){
    return num1+num2;
}
function callSum(num1,num2){
    return sum.call(this,num1,num2);
}
alert(callSum(10,10));
```

　　apply()和call()最强大的地方是能够扩充函数赖以运行的作用域

```
1 window.color="red";
2 var o ={color:"blue"};
3 function sayColor(){
4    alert(this.color);
5 }
6 sayColor();              //red
7 sayColor.call(this);     //red
8 sayColor.call(window);   //red
9 sayColor.call(o);        //blue
```











