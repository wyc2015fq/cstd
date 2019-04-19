# extjs grid 列顺序紊乱问题 - 左直拳的马桶_日用桶 - CSDN博客
2016年08月19日 20:45:52[左直拳](https://me.csdn.net/leftfist)阅读数：2019
这个问题描述类似
[关于extjs表格列展示顺序问题](http://www.iteye.com/problems/77962)
![这里写图片描述](https://img-blog.csdn.net/20160819202417355)
明明在columns定义好了，理应按照里面的顺序输出嘛，但偏不，原本应该列在第一位的，结果忽而在最后，忽而在中间，忽忽何所似，天地一狗屎。
在谷歌里寻寻觅觅，找不到。给出的答案，大部分都是列排序问题，我靠，牛头不对马嘴。
后来尝试着将一个叫stateful的属性值改为false，就正常了：
```java
////grid
    var DaipiGrid = Ext.create('Ext.grid.Panel', {
        id: 'DaipiGrid',
        renderTo: 'grid',
        layout: 'fit',
        autoScroll: true,
        store: DaipiStore,
        stateful: false,//<---------------------为true时紊乱；设为fasle或干脆屏蔽都正常
        stateId: 'taskGroup-Grid',
        viewConfig: {
            enableTextSelection: true //列表数据可复制
        },
        columns: [
           {
               xtype: 'rownumberer',
               text: "序号",
               width: '3%',
               align: 'center'               
           },
           { xtype: 'gridcolumn', dataIndex: 'ID', text: '流水号', width: '5%', align: 'center', sortable: false, style: 'text-align:center;' },
           { xtype: 'gridcolumn', dataIndex: 'YEARLY', text: '计划年度', width: '7%', align: 'center', sortable: false },
            {
```
stateful，这是什么鬼？
