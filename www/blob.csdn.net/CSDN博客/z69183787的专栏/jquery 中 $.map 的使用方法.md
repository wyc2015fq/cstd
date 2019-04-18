# jquery 中 $.map 的使用方法 - z69183787的专栏 - CSDN博客
2013年01月16日 13:38:36[OkidoGreen](https://me.csdn.net/z69183787)阅读数：39048
$.map(data,function(item,index){return XXX})
遍历data数组中的每个元素，并按照return中的计算方式 形成一个新的元素，放入返回的数组中
```java
var b = $.map( [55,1,2], function( item,index ) {  return {  "label": item,  "value": index  }});
                        
                        alert(b[0].label +"  "+ b[0].value);
```
输出为  55 0
```java
var array = [0, 1, 52, 97];
array = $.map(array, function(a, index) {
  return [a - 45, index];
}); 
输出为:
[-45, 0, -44, 1, 7, 2, 52, 3]
```
