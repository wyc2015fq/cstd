# jeasyUI treegrid 的 reload 和 getChanges - 左直拳的马桶_日用桶 - CSDN博客
2015年01月07日 22:23:09[左直拳](https://me.csdn.net/leftfist)阅读数：4424
看上去，treegrid继承自datagrid，因此，datagrid有的，treegrid也会有。
比如说，getChanges，翻看那些网络教程，绝大多数都没提treegrid有这个东东。但是，treegrid确确实实是有。
有了getChanges，我们再不用担心如何记录修改。
问题是，当我们reload数据的时候，这些getChanges居然还在，雷打不动。
网上疯狂搜索，找不到现成的答案。怎么办呢？
想起getChanges是返回上次acceptChange以来的变动，那么reload的时候，我紧接着acceptChange不就行啦？
没代码我说个J8：
```java
//var _grid = $("treegridDemo");
function () {
    _grid.treegrid('unselectAll');
    _grid.treegrid('reload');
    _grid.treegrid('acceptChanges');
}
```
果然OK！我真是智多星。            
