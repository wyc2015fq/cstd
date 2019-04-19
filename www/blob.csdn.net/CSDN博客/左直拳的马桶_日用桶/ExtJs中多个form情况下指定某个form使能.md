# ExtJs中多个form情况下指定某个form使能 - 左直拳的马桶_日用桶 - CSDN博客
2015年06月16日 14:46:03[左直拳](https://me.csdn.net/leftfist)阅读数：1685
采用extjs的时候，如果一个页面存在多个，那么提交之时，究竟是哪个form使能，就要指明。我今天就遇到了这种情况：明明页面已经有提交，为啥没有提交到内容？一查才知道，我的页面是有2个form，我本意是提交form1的，结果提交了form2。
那么应该怎么指明哪个form使能？
这样写：
```java
listStore.on('beforeload', function () {
    var str;
    if (panel == 0) {
        str = Ext.getCmp('formSimple').form.getValues();//简单查询窗口使能
    } else {
        str = Ext.getCmp('formPower').form.getValues();//高级查询窗口
    }
    listStore.proxy.extraParams = [];
    Ext.apply(listStore.proxy.extraParams, str);
});
listStore.on('load', function (store, records, successful, eOpts) {
    try {
        if (store.proxy.reader.rawData.topics[0].timeout == "timeout") {
            alert('登录超时，请重新登录！');
        }
    }
    catch (e) { }
});
//listStore是表格的数据源
var listStore = new Ext.data.Store({
    autoDestroy: true, 
    pageSize: 20,
    model: 'listModel',
    proxy: {
        type: 'ajax',
        url: 'list.aspx?action=GetList',
        reader: {
                root: 'topics',
                totalProperty: 'totalCount'
            },
            simpleSortMode: true
        },
        remoteSort: true
    });
```
如此，问题解决矣
