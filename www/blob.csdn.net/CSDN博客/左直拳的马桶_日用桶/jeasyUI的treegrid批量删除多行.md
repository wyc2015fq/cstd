# jeasyUI的treegrid批量删除多行 - 左直拳的马桶_日用桶 - CSDN博客
2015年01月07日 18:02:44[左直拳](https://me.csdn.net/leftfist)阅读数：3135
看上去，javascript的变量类型，也可以分为值类型和引用类型。赋值操作中，值类型，各自独立，互不干涉；引用类型，指针而已，大家指向同一个对象。
为什么这样说呢？
我是从jeasyUI的treegrid批量删除多行操作中知道的。
treegrid中批量删除多行，首先要获得选中的那些行：
var rows = _grid.treegrid('getSelections');
然后遍历之，删除之
for (var i = 0; i < rows.length; i++) {
    var id = rows[i].id;
    _grid.treegrid('remove', id);
}
结果总剩下一些没删。
究其原因，就是因为rows是个引用类型，它指向了那些行，结果删掉一行，rows.length就减1，最后循环很快就结束了。这跟以前C#下，遍历删除DataTable里的记录一个样。
知道原因后，修正也简单：先将rows里的id取出来，存到数组里，数组是值类型，然后遍历该数组，删除：
```java
//var _grid = $("treegridDemo");
var rows = _grid.treegrid('getSelections');
var ids = new Array();
for (var i = 0; i < rows.length; i++) {
    ids[i] = rows[i].id;
}
for (var i = 0; i < ids.length; i++) {
    _grid.treegrid('remove', ids[i]);
}
_grid.treegrid('reloadFooter');
```
