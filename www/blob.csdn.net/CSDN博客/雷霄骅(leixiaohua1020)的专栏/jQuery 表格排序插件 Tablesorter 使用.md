# jQuery 表格排序插件 Tablesorter 使用 - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2013年10月10日 17:49:00[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：24
个人分类：[HTML和Javascript](https://blog.csdn.net/leixiaohua1020/article/category/1678189)










jQuery 表格排序插件 Tablesorter 使用方式如下：

1.引入头文件（注意一定要把jQuery放在前面）：



```
<script src="lib/jquery-1.8.3.min.js"></script>
<!--tablesorter-->
<link href="css/css_tablesorter_green/style.css" rel="stylesheet" type="text/css" />
<script type="text/javascript" src="lib/jquery.tablesorter.js"></script>
<script type="text/javascript">
		$(document).ready(function(){
		$("#alltable").tablesorter();     
	}); 
</script>
```


 2.在需要使用排序的<Table>上无需做任何设定，很方便：





```
<table id="alltable" class="tablesorter"  width="584"> 
<thead> 
<tr> 
    <th>Last Name</th> 
    <th>First Name</th> 
    <th>Email</th> 
    <th>Due</th> 
    <th>Web Site</th> 
</tr> 
</thead> 
<tbody> 
<tr> 
    <td>Smith</td> 
    <td>John</td> 
    <td>jsmith@gmail.com</td> 
    <td>$50.00</td> 
    <td>http://www.jsmith.com</td> 
</tr> 
<tr> 
    <td>Bach</td> 
    <td>Frank</td> 
    <td>fbach@yahoo.com</td> 
    <td>$50.00</td> 
    <td>http://www.frank.com</td> 
</tr> 
<tr> 
    <td>Doe</td> 
    <td>Jason</td> 
    <td>jdoe@hotmail.com</td> 
    <td>$100.00</td> 
    <td>http://www.jdoe.com</td> 
</tr> 
<tr> 
    <td>Conway</td> 
    <td>Tim</td> 
    <td>tconway@earthlink.net</td> 
    <td>$50.00</td> 
    <td>http://www.timconway.com</td> 
</tr> 
</tbody> 
</table>
```



在<thead>上就会有两个小三角符号，点击可以用于排序。

![](https://img-blog.csdn.net/20131010175221703?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



Tablesorter下载地址：[http://download.csdn.net/detail/leixiaohua1020/6377187](http://download.csdn.net/detail/leixiaohua1020/6377187)






