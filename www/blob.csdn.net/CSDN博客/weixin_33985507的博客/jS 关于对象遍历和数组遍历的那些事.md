# jS:关于对象遍历和数组遍历的那些事 - weixin_33985507的博客 - CSDN博客
2017年07月25日 05:20:26[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：1
可能是由于职业的关系，下班之后完全不想Open PC，但很多知识点有必要自己做个小小总结。本人之前对原生Array和Object完全没概念。
- 
遍历对象的方法：
Object.keys(Object) | for(item in Object)| object.getOwnPropertyNames(o)
- 
遍历数组的方法：
Array.forEach() | Array.map() | Array.filter() | Array.some() | Array.every()
下面一一来说说每种遍历方法都是干啥子用的
## 遍历对象,枚举一个对象的所有属性：
从 ECMAScript 5 开始，有三种原生的方法用于列出或枚举对象的属性：
- 
for…in 循环
该方法依次访问一个对象及其原型链中所有可枚举的属性。
*Object.keys(o)*
![图片描述](https://image-static.segmentfault.com/270/983/2709838760-597696b72ecb0_articlex)
该方法返回一个对象 o 自身包含（不包括原型中）的所有属性的名称的数组。
*Object.getOwnPropertyNames(o)*
该方法返回一个数组，它包含了对象 o 所有拥有的属性（无论是否可枚举）的名称。
pay attention:Object.keys(object)返回的是一个数组。如图：
![图片描述](https://image-static.segmentfault.com/192/620/1926204864-597697f8a62d4_articlex)
## 遍历数组(interating over array)
遍历数组元素并以某种方式处理每个元素是一个常见的操作。以下是最简单的方式：
```
var colors = ['red', 'green', 'blue'];
for (var i = 0; i < colors.length; i++) {
  console.log(colors[i]);
}
array.forEach()
```
- 
forEach()
对数组中的而每一项运行传入的函数，没有返回值，本质是与for循环迭代数组一样，一般的用法：
```
var numbers =[111,11,2,3];
numbers.forEach(function(item,index,array){
    //console.log(item,index,array)
})
或者：
var arr = [3, 5, 7];
arr.forEach(item => {
    console.log(item);
})
```
由于 arrow function 的特性，自动绑定父 scope 的 this， 会更加简洁，而且少了个function关键字，可读性更好。
- 
array.some() 和array.every()
```
var arr=[1,2,3,4];
var everyresult =arr.every((item,index,array) => item>2);
console.log(everyresult)//false
```
两者用法比较相似，用于查询数组中的项是否满足某个条件，两者都有返回值，且返回值是boolean,true or false,every()只有每一项都满足才能返回true，否则返回false,some()则是相反的。切记返回值是true或者false;
- 
array.map()和array.filter()
两个方法比较相似，都有返回值，且返回的还都是数组，不是布尔类型。filter()，利用指定的函数确定是否在返回的数组中包含某一项。用来数组去重比较灵活
```
var arr1=[11,22,223,24,3,42,3,11,11,2,3333,3333333,2];
var arr2 = arr1.filter((k,index) => arr1.indexOf(k) === index).sort((a,b) => a-b > 1);
Result: [3, 2, 11, 22, 24, 42, 223, 3333, 3333333]
```
又或者用法某一项值返回true的情况下：
![图片描述](https://image-static.segmentfault.com/645/070/645070213-5976981f14345_articlex)
map() 也返回一个数组，不过，map有一点不好的地方在于不能控制循环的流程，如果不能完成，就返回undefined继续下一次迭代。所以遇到可能会返回undefined的情况应该用forEach或者for循环遍历。
如图：
![图片描述](https://image-static.segmentfault.com/216/380/2163807764-59769875a876c_articlex)
不过，JQ里的$.map()功能比较强大，可以遍历而且返回值是undefined的则被省略掉
![图片描述](https://image-static.segmentfault.com/110/330/1103309001-597698ac1b48e_articlex)
```
var obj={"send_message":{"1":false,"2":true},"audit_type":{"1":true,"2":false,"3":true}};
var res={};
for(item in obj){
    var value =obj[item];
    var arr =Object.keys(value);
    var arr2 = $.map(arr,function(kk,vv){
    if(vv){
    return kk
}
})
    res[item]=arr2.join();}
console.log(res,'res');
```
## 总结
1.遍历对象，Object.keys(object)遍历出来的是键名，而不是键值，参数必须是对象。
2.遍历数组，前提必须是数组，才能用filter/map/every/some/forEach等这几个方法，且注意map()和filter()是有返回值的，map()有自己的缺陷，有可能返回undefined，而filter()返回的则是为true的某一项
3.join（）则是使用不同的分隔符来构建字符串，数组默认情况下以逗号分隔的形式返回数组项。
4.虽然数组也是对象的一种，但确是Array类型，检测数组的几种方法：
```
if(arr instanceof Array){console.log('shuzu')};
or:
if(Array.isArray (arr)){console.log('shuzu')}
or:
if(Object.prototype.toString.call(arr) == '[object Array]'){console.log('我是数组')}
```
第三种办法比较万能，推荐使用！！
