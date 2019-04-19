# easyUI的getChanges中的inserted - 左直拳的马桶_日用桶 - CSDN博客
2015年01月13日 21:25:19[左直拳](https://me.csdn.net/leftfist)阅读数：6028
easyUI中的datagrid、treegrid编辑了数据，可以用getChanges命令来获取更新情况。按道理，有"inserted"、"updated"、"deleted"三种类型，增改删嘛，但不知为何，"inserted"我从来没有获取到过。
deleted比较没有问题，只要有remove，就可以获取；
updated的话，有修改也可以。
对于inserted，有个变通的办法是，新增的时候，选中新增行，然后beginEdit，再endEdit；这样该行即可进入updated矣。有一个问题要注意：datagrid、treegrid（其实treegrid继承自datagrid）每行会有个主键，比如id，用于识别行。那么我们只要在新增行的时候，将此id赋一个唯一值，再乘以-1，提交到后台，就可知该行是insert而不是update，从而不会与其他update行混淆。
还有一个注意的问题，就是这些新增行的数据不能齐全，要至少有一个字段没有值，这样才能进入到update里，真奇怪。
```java
function InsertItem(nodes,groupAId, node) {
    var _id = getUtc() * -1;
    var _data = {
        'id': _id
        , 'name': node.text
        //, 'seq': 0 //空出一个值不赋，这样才能进入update。原因不明
        , 'isValid': true
        , 'pId': node.pId
        , 'itemId': node.itemId
        , 'isGroup': false
    };
    _grid.treegrid('append', {
        parent: groupAId,
        data: [_data]
    });
    _grid.treegrid('select', _data.id);
    _grid.treegrid('beginEdit', _data.id);
    _grid.treegrid('endEdit', _data.id);
    return _id;
}
```
猜想：
通常我们用数据库的主键id来做datagrid的主键，其实可以分开来，多设一个字段给easyUI，比如设一个eid，用eid来做datagrid的行主键；从后台过来的时候，eid == id。而新增的时候，新增行的eid被赋予唯一值，而id则不赋值。这样的好处是
1）有可能在getChanges的时候，可以获取到inserted；
2）不用将 id 赋予一个负值这么折腾
但不想专门去试这个东西。不值得。
