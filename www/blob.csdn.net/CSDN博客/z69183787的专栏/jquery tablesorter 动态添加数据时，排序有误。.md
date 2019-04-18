# jquery tablesorter 动态添加数据时，排序有误。 - z69183787的专栏 - CSDN博客
2013年12月17日 14:45:50[OkidoGreen](https://me.csdn.net/z69183787)阅读数：3850
，我从后台动态加载表格，然后运用tablesorter，然后动态更改表格中的数据，排序不会按照更改后的数据进行排序，会按照之前的数据进行排序，？
解决方法。在动态修改数据后，使用$("#scroll").trigger("update");就行了。
官网实例：
```java
$(document).ready(function() { 
    $("table").tablesorter(); 
    $("#ajax-append").click(function() { 
         $.get("assets/ajax-content.html", function(html) { 
             // append the "ajax'd" data to the table body 
             $("table tbody").append(html); 
            // let the plugin know that we made a update 
            $("table").trigger("update"); 
            // set sorting column and direction, this will sort on the first and third column 
            var sorting = [[2,1],[0,0]]; 
            // sort on the first column 
            $("table").trigger("sorton",[sorting]); 
        }); 
        return false; 
    }); 
});
```
