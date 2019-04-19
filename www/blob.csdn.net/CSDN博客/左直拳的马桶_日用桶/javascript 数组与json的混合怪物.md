# javascript 数组与json的混合怪物 - 左直拳的马桶_日用桶 - CSDN博客
2017年11月27日 20:11:02[左直拳](https://me.csdn.net/leftfist)阅读数：194
众所周知，javascript的变量是可变类型，说变就变。这是优点，有很大的灵活性，但有时一不小心，又会让人因为未反应过来，而导致一些困惑。
比方说，有个数组：
```
var arr = ['a','b','c'];
alert(arr.length);//3
arr['a'] = 'A';
alert(arr.length);//数组长度没变，还是3
alert(arr['a'] + "," + arr.a);//A,A。说明arr['a'] == arr.a
alert(arr['a'] === arr.a);//true
```
原本arr只是一个单纯的数组，但有赋值语句`arr['a'] = 'A'`之后，arr变成了一个怪物：数组与json的混合体：
`["a", "b", "c", a: "A"]`
其中a不知为何，并不是数组的元素，数组的长度还是3。
```php
arr.forEach(function(a){
    alert("foreach:" + a);
});//分别提示a,b,c
```
由于forEach只遍历数组元素，所以上述语句，arr.a并不能被检视出来。所以，为保险起见，最好还是用以下语句，避免疏漏。
```
for(var i in arr){
    alert("for:" + arr[i]);
}//分别提示a,b,c,A
```
