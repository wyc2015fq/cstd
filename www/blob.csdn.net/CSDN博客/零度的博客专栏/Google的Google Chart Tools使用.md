# Google的Google Chart Tools使用 - 零度的博客专栏 - CSDN博客
2016年07月12日 15:03:50[零度anngle](https://me.csdn.net/zmx729618)阅读数：1858
图形是我们最经常看见和使用的东西。但要自己写代码实现一个图形还不是那么简单。
比如Excel的图形，PDF中的图形，网页上的图形。方式很多，实现方法也不一样。
看看google的网页上面显示的图形，真的很漂亮。看看下面的这个图形的代码。
![](http://hi.csdn.net/attachment/201112/9/0_13234193860aaL.gif)
代码：
```
<html>
  <head>
    <script type="text/javascript" src="https://www.google.com/jsapi"></script>
    <script type="text/javascript">
      google.load("visualization", "1", {packages:["corechart"]});
      google.setOnLoadCallback(drawChart);
      function drawChart() {
        var data = new google.visualization.DataTable();
        data.addColumn('string', 'Task');
        data.addColumn('number', 'Hours per Day');
        data.addRows([
          ['Work',    11],
          ['Eat',      2],
          ['Commute',  2],
          ['Watch TV', 2],
          ['Sleep',    7]
        ]);
        var options = {
          width: 450, height: 300,
          title: 'My Daily Activities'
        };
        var chart = new google.visualization.PieChart(document.getElementById('chart_div'));
        chart.draw(data, options);
      }
    </script>
  </head>
  <body>
    <div id="chart_div"></div>
  </body>
</html>
```
