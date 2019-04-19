# 【js】用js定时循环执行语句和定时延缓执行 - Big Smile - CSDN博客
2018年01月28日 16:44:18[王啸tr1912](https://me.csdn.net/tr1912)阅读数：4438
        最近在项目中，需要使用到站内信的消息推送方式在网站中给用户推送消息，就是在页面有下角推送一个弹窗，这里需要我们定时去后台查询是否有消息推送过来，所以需要在JS层面进行定时执行查询的任务。
这里用到了JS的两个函数方法，一个是setInterval，一个是setTimeout，这两个函数在使用上要如何处理呢，下面分别讲一下：
## 1、setInterval
setInterval这个函数有很多的参数，大家可以自行查询一下w3school，这里不再赘述，主要讲一下两种模式使用这个函数。
**一、是直接写我们要执行的函数在里面，不带外部参数：**
```java
setInterval(function(){alert("Hello")}, 3000);
```
     第一个参数是我们要执行的js代码，也可以调用其他的function，比如：
```java
function myFunction() {
    setInterval(alertFunc, 3000);
}
 
function alertFunc() {
    alert("Hello!");
}
```
     第二个参数是间隔的时间，单位是毫秒，3000就代表间隔是3秒的时间。
**二、可以带外部参**
```java
setInterval(function, milliseconds, param1, param2, ...)
```
例如：
```java
var str1='Hello';
var str2='World';
setInterval(function(){alert({0}+' '+{1})}, milliseconds, str1, str2)
```
怎么样，是不是感觉这个函数非常简单？
## 2、setTimeout
        这个函数，主要是用于延迟执行某段js的时候使用的，参数比较少，使用比较简单，和setIterval的第一种方法一样，它也是有两个参数：
```java
setTimeout(function(){alert("Hello")},1000);
```
同样第一个是执行对象，第二个是延时时长。但是这里有需要注意的地方：
1、在setTimeout这条语句执行完成之后会立即进入倒计时状态，所以我们想要设置间隔输出的话，需要在延时时间上面做手脚。
2、在延时时间到了之后，执行的第一个参数中的js语句，所取的是执行时间到了之后的参数所代表的数值，比如我之前写过一个代码，用i代表数组内的元素，结果只是执行的时候只是输出最后一个，例如：
```java
Array[] strList={'tip1','tip2','tip3','tip4','tip5','tip6'}
for(var i=0;i<strList.length;i++){
     setTimeout(function(){alert(strList[i])},3000);
}
```
如果我们的代码这样写的话，只会输出6遍tip6，并且是在3秒到了以后一块输出。正确的方式是这样的
```java
Array[] strList={'tip1','tip2','tip3','tip4','tip5','tip6'}
var j=0;
for(var i=0;i<strList.length;i++){
     setTimeout(function(){alert(strList[j]);j++;},1000*i);
}
```
这样就可以了。

