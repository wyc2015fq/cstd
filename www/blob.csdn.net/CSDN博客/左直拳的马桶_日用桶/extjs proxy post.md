# extjs proxy post - 左直拳的马桶_日用桶 - CSDN博客
2017年10月21日 16:57:48[左直拳](https://me.csdn.net/leftfist)阅读数：491标签：[extjs](https://so.csdn.net/so/search/s.do?q=extjs&t=blog)
个人分类：[JavaScript](https://blog.csdn.net/leftfist/article/category/94500)
extJs store的proxy，里面的read方式，默认是”get”方式。也就是说，用extjs向后台请求数据，默认是”get”方式，有关参数，要附在请求URL后面。
这毕竟不方便，尤其是参数比较多的情况。能不能用post的方式呢？
“可以的”，西蒙说。
上代码：
```
//列表1
var HyStore = new Ext.data.Store({
    autoDestroy: true, pageSize: 20, model: 'HyModel', proxy: {
        type: 'ajax',
        url: urlget,//请求URL
        reader: { root: 'topics', totalProperty: 'totalCount' },
        simpleSortMode: true,
        actionMethods: {
            read: 'POST'//--------------------
        },
        jsonData: true,//post给后台的参数形式？
        extraParams: {"name":"张三","age":43}//post给后台的参数
    },
    remoteSort: true
});
```
提交给后台的参数，存放在extraParams里。我翻遍了整个互联网，古今中外都找遍了，都没人给一个说法，还是直接看extjs的文档试出来的。
之所以叫extarParams，额外的参数，大概是post的时候，extjs会默认将页码、每页的记录数等，总共3个参数一起提交。
后台（C#）代码：
```
public ActionResult Hy_GetList(FormCollection collection)
        {
            var name = collection["name"];//张三
        }
```
