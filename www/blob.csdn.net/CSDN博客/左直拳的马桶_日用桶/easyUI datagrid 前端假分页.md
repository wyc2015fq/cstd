# easyUI datagrid 前端假分页 - 左直拳的马桶_日用桶 - CSDN博客
2015年01月26日 23:05:40[左直拳](https://me.csdn.net/leftfist)阅读数：10104
datagrid有两种分页方式，真分页和假分页。
所谓真分页，就是真的每次只获取一张分页的数据。
所谓假分页，就是将所有数据全部获取下来，然后利用其分页控件进行分页。
下面具体说说假分页：
1、datagrid有些参数需要设置：
```java
$("#dg").datagrid({
    pageNumber:1,
    pagination: true,//分页控件 
    rownumbers: true,//显示行号
    pageSize: 10,
    pageList: [10, 15, 50, 100],
    loader: myLoader, //前端分页加载函数
    ……
```
2、加载函数：
```java
function myLoader(param, success, error) {
    var that = $(this);
    var opts = that.datagrid("options");
    if (!opts.url) {
        return false;
    }
    var cache = that.data().datagrid.cache;
    if (!cache) {
        $.ajax({
            type: opts.method,
            url: opts.url,
            data: param,
            dataType: "json",
            success: function (data) {
                that.data().datagrid['cache'] = data;
                success(bulidData(data));
            },
            error: function () {
                error.apply(this, arguments);
            }
        });
    } else {
        success(bulidData(cache));
    }
    function bulidData(data) {
        debugger;
        var temp = $.extend({}, data);
        var tempRows = [];
        var start = (param.page - 1) * parseInt(param.rows);
        var end = start + parseInt(param.rows);
        var rows = data.rows;
        for (var i = start; i < end; i++) {
            if (rows[i]) {
                tempRows.push(rows[i]);
            } else {
                break;
            }
        }
        temp.rows = tempRows;
        return temp;
    }
}
```
照抄可也，只要 jQuery easyui 1.2.6+
3、服务器端获取数据，格式一定要是：
｛"total":106,"rows":[{},{},{}.....]｝，且rows.length的长度应该等于total的值。显然，rows就是具体的数据。
如此，分页可成矣，只不过，它是假的。
4、如果需要刷新数据，务必清除缓存：
```java
$("#dg).data().datagrid.cache = null;
$("#dg).datagrid("reload");
```
