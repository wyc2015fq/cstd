# javascript下获取guid或者UTC时间作为唯一值 - 左直拳的马桶_日用桶 - CSDN博客
2015年01月07日 10:06:58[左直拳](https://me.csdn.net/leftfist)阅读数：5821
javascript下，有时出于需要，可以利用guid或UTC时间作为当前页面中的唯一值。
什么场景需要弄这个唯一值？
比如说，用easyUI的treegrid，添加新节点。在treegrid里面 ，节点要对应唯一的主键，比如ID。那么新建节点的时候，通常只是在界面操作，然后再批量提交到后台，保存到数据库。这样在界面操作的时候，新建节点的唯一ID怎么来呢？我想到的一个思路就是，利用guid或者UTC时间临时作为ID值，等到后台处理时，再忽略或者做别的处理。
没代码没J8：
```java
function guid() {
    return 'xxxxxxxx-xxxx-4xxx-yxxx-xxxxxxxxxxxx'.replace(/[xy]/g, function(c) {
        var r = Math.random()*16|0, v = c == 'x' ? r : (r&0x3|0x8);
        return v.toString(16);
    });
}                              
function getUtc() {
    var d = new Date();
    return Date.UTC(d.getFullYear()
        , d.getMonth()
        , d.getDate()
        , d.getHours()
        , d.getMinutes()
        , d.getSeconds()
        , d.getMilliseconds());
}
```
=================================
在循环语句中产生UTC值，会有重复。想想不奇怪，它只精确到毫秒而已。
