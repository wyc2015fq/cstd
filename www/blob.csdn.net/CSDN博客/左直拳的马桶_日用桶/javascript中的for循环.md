# javascript中的for循环 - 左直拳的马桶_日用桶 - CSDN博客
2017年10月17日 09:18:21[左直拳](https://me.csdn.net/leftfist)阅读数：381
假设有个数组:
```php
var array = [];
```
那么在javascript中，要用for循环语句来遍历处理这个数组，有多少种方式呢？3种：
**1、for**
```matlab
for(var i = 0;i < array.length;i++){
    var a = array[i];
    ……
}
```
众所周知，应用最广，无需多言。
**2、for升级版**
```php
for(var i in array){
    var a = array[i];
    ……
}
```
for的改进版，与1相比，简洁了不少。要注意在这里 i 只是一个下标，而不是一个数组元素。
**3、foreach**
```php
array.forEach(function(a){
    ……
});
```
这种方式，每个步骤都是一个函数，封装性最好。也因为如此，我想不通如果要中途退出循环该咋写？怎么样break呢？
