# javascript数组元素的增加与删除 - 左直拳的马桶_日用桶 - CSDN博客
2018年04月25日 17:57:15[左直拳](https://me.csdn.net/leftfist)阅读数：118
一、数组添加元素（至末端） 
push(元素)
```
var arr = [];
arr.push(100);
arr.push(99);
arr.push(101);
alert(arr);//100,99,101
```
二、弹出末端元素 
pop()
```
var arr = [];
arr.push(100);
arr.push(99);
arr.push(101);
alert(arr);//100,99,101
arr.pop();
alert(arr);//100,99
```
三、弹出头部元素 
shift()
```
var arr = [];
arr.push(100);
arr.push(99);
arr.push(101);
alert(arr);//100,99,101
arr.shift();
alert(arr);//99,101
```
四、弹出指定元素 
splice(起始位置，长度)
```
var arr = [];
arr.push(100);
arr.push(99);
arr.push(101);
alert(arr);//100,99,101
arr.splice(1,1);
alert(arr);//100,101
```
```
var arr = [];
arr.push(100);
arr.push(99);
arr.push(101);
alert(arr);//100,99,101
arr.splice(1,2);
alert(arr);//100
```
与查找结合
```
var arr = [];
arr.push(100);
arr.push(99);
arr.push(101);
alert(arr);//100,99,101
arr.splice(arr.indexOf(99),1);
alert(arr);//100,101
```
