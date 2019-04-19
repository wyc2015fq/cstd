# typeof、undefined 和 null - 左直拳的马桶_日用桶 - CSDN博客
2012年09月12日 15:15:47[左直拳](https://me.csdn.net/leftfist)阅读数：4766
typeof 是运算符，注意不是函数，是运算符，其作用，是考察变量究竟是什么类型。或曰，是变量是否定义或是否初始化的照妖镜。返回值是字符串。
undefined 表示一个对象没有被定义或者没有被初始化。
null 表示一个尚未存在的对象的占位符。
测试一，变量没有定义的时候，只能使用 typeof
```java
<input type="button" id="btnTest" onclick="Test()" value="测试" />
    <script type="text/javascript">
        function Test() {
            alert(i == null);               //报错
            alert(i == undefined);          //报错
            alert(typeof i == "undefined"); //true
        }
    </script>
```
测试二，变量有定义，但未初始化，typeof,undefined,null都可以使用 
```java
<script type="text/javascript">
        function Test() {
            var i;
            alert(i == null);               //true
            alert(i == undefined);          //true
            alert(typeof i == "undefined"); //true
            i = 0;
            alert(i == null);               //false
            alert(i == undefined);          //false
            alert(typeof i == "undefined"); //false
        }
    </script>
```
测试三，函数参数跟变量类似
1、没有参数
```java
<input type="button" id="btnTest" onclick="Test()" value="测试" />
    <script type="text/javascript">
        function Test(i) {
            alert(i == null);               //true
            alert(i == undefined);          //true
            alert(typeof i == "undefined"); //true
        }
    </script>
```
2、有参数
```java
<input type="button" id="btnTest" onclick="Test(0)" value="测试" />
    <script type="text/javascript">
        function Test(i) {
            alert(i == null);               //false
            alert(i == undefined);          //false
            alert(typeof i == "undefined"); //false
        }
    </script>
```
参考文章：[点击打开链接](http://www.cnblogs.com/ttltry-air/archive/2011/03/24/1993433.html)
