# javascript数组的排序及查找 - 左直拳的马桶_日用桶 - CSDN博客
2017年11月29日 19:32:50[左直拳](https://me.csdn.net/leftfist)阅读数：214
基本上这两个操作都要写函数作为参数传进去。 
假设有数组：
```
var arrg = [
{date:1472601600000,name:'监测4'}
{date:1374105600000,name:'监测1'}
{date:1464912000000,name:'监测3'}
{date:1487289600000,name:'监测6'}
{date:1492473600000,name:'监测7'}
{date:1384819200000,name:'监测2'}
{date:1479772800000,name:'监测5'}
]
```
**1、排序**
```java
arrg.sort(function(a,b) {//a,b应该分别是前后两个元素
    return a.date - b.date;
});//无须返回值，直接将arrg排好序
```
参考文章 
[JavaScript sort() 方法](http://www.w3school.com.cn/jsref/jsref_sort.asp)
**2、查找**
返回元素
```
_timeSliders = [{id:'a'},{id:'b'}];
var obj = _timeSliders.find((t) => t.id == 'b');
alert(typeof obj != 'undefined');
```
返回下标
```java
var i = arrg.findIndex(function (g, index, arr) {
    return g.date == 1464912000000;
});//返回date == 1464912000000 在数组中的下标。如果无匹配，则返回-1
```
参考文章： 
[JavaScript 数组中查找符合条件的值](https://www.cnblogs.com/neverleave/p/6134177.html)
相关文章： 
[javascript 数组与json的混合怪物](http://blog.csdn.net/leftfist/article/details/78648541)
我靠，IE不支持这两个方法，真无语。所以要检测下
```php
if(array.find){
    //用find
} else {
    //自己再想办法
}
```
IE里面，数组查找可以用indexOf方法，像查找字符串一样：
```php
var list = ["bar", "baz", "foo", "qux"];
alert(list.indexOf('foo'));//2
alert(list.indexOf('FOO'));//-1
```
