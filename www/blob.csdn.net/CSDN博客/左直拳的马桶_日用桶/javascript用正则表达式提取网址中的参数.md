# javascript用正则表达式提取网址中的参数 - 左直拳的马桶_日用桶 - CSDN博客
2013年01月08日 23:40:44[左直拳](https://me.csdn.net/leftfist)阅读数：1635
个人分类：[JavaScript](https://blog.csdn.net/leftfist/article/category/94500)
直接上代码
```java
<script type="text/javascript">
    var url = "http://www.***.com/n/2013/01/08/1037962.shtml";
    //url = "http://www.***.com/mine/mine_show.aspx?id=1037962";
    var reg = /\/\d+.shtml|\?id\=\d+/gi;
    var result;
    if((result = reg.exec(url)) != null){
        result += "";
        var id = result.match(/\d+/g);
        alert(id);
    }
</script>
```
参考文章
http://www.w3school.com.cn/js/jsref_obj_regexp.asp
------------------------------------------------------------
下面的两条语句是等价的： 
re = new RegExp("\\w+","gi") 
re = /\w+/gi
