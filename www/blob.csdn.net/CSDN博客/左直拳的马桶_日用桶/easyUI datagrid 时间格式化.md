# easyUI datagrid 时间格式化 - 左直拳的马桶_日用桶 - CSDN博客
2015年03月10日 19:01:30[左直拳](https://me.csdn.net/leftfist)阅读数：28264
从后台传过来的数据，其中含有日期字段，那么在前端的easyUI这里显示的话，会显得比较怪异，一大串，中间是个T，后面一大堆零，不知道是什么意思。
看来要进行格式化。
问题是，在哪里格式化？
如果在后端，转换成想要的字符串，易如反掌。但细想，逻辑上比较合适在前端处理：时间数据从后台过来，然后如何显示，是前端的事。
那么在前端这里，如何处理？
1、扩展Date的功能
```java
Date.prototype.format = function (format) {
    var o = {
        "M+": this.getMonth() + 1, // month
        "d+": this.getDate(), // day
        "h+": this.getHours(), // hour
        "m+": this.getMinutes(), // minute
        "s+": this.getSeconds(), // second
        "q+": Math.floor((this.getMonth() + 3) / 3), // quarter
        "S": this.getMilliseconds()
        // millisecond
    }
    if (/(y+)/.test(format))
        format = format.replace(RegExp.$1, (this.getFullYear() + "")
            .substr(4 - RegExp.$1.length));
    for (var k in o)
        if (new RegExp("(" + k + ")").test(format))
            format = format.replace(RegExp.$1, RegExp.$1.length == 1 ? o[k] : ("00" + o[k]).substr(("" + o[k]).length));
    return format;
}
function formatDatebox(value) {
    if (value == null || value == '') {
        return '';
    }
    var dt;
    if (value instanceof Date) {
        dt = value;
    } else {
        dt = new Date(value);
    }
    return dt.format("yyyy-MM-dd"); //扩展的Date的format方法(上述插件实现)
}
```
2、easyUI datagrid 中调用
```java
$('#datagridDemo1').datagrid({
    ……
    columns: [[
            { field: 'CreateDate', title: '创建日期', width: 120,align: 'center',formatter: formatDatebox},
            ……
        ]]    
});
```
如此，可显示格式化日期矣。
参考文章：
[http://blog.csdn.net/walkerjong/article/details/7514026](http://blog.csdn.net/walkerjong/article/details/7514026)
