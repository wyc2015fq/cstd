# google的Google Chart Tools太酷了 - z69183787的专栏 - CSDN博客
2014年04月30日 09:22:46[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1507
图形是我们最经常看见和使用的东西。但要自己写代码实现一个图形还不是那么简单。
比如Excel的图形，PDF中的图形，网页上的图形。方式很多，实现方法也不一样。
看看google的网页上面显示的图形，真的很漂亮。看看下面的这个图形的代码。
![](http://hi.csdn.net/attachment/201112/9/0_13234193860aaL.gif)
代码：
**[html]**[view
 plain](http://blog.csdn.net/hantiannan/article/details/7057571#)[copy](http://blog.csdn.net/hantiannan/article/details/7057571#)[print](http://blog.csdn.net/hantiannan/article/details/7057571#)[?](http://blog.csdn.net/hantiannan/article/details/7057571#)
- <html>
- <head>
- <scripttype="text/javascript"src="https://www.google.com/jsapi"></script>
- <scripttype="text/javascript">
-       google.load("visualization", "1", {packages:["corechart"]});  
-       google.setOnLoadCallback(drawChart);  
-       function drawChart() {  
-         var data = new google.visualization.DataTable();  
-         data.addColumn('string', 'Task');  
-         data.addColumn('number', 'Hours per Day');  
-         data.addRows([  
-           ['Work',    11],  
-           ['Eat',      2],  
-           ['Commute',  2],  
-           ['Watch TV', 2],  
-           ['Sleep',    7]  
-         ]);  
- 
-         var options = {  
-           width: 450, height: 300,  
-           title: 'My Daily Activities'  
-         };  
- 
-         var chart = new google.visualization.PieChart(document.getElementById('chart_div'));  
-         chart.draw(data, options);  
-       }  
- </script>
- </head>
- <body>
- <divid="chart_div"></div>
- </body>
- </html>
还有很多类型，直接去官方网站查看比较好。http://code.google.com/intl/ja/apis/chart/interactive/docs/gallery.html
