# highcharts点击事件获取点击点的值 - z69183787的专栏 - CSDN博客
2013年03月06日 13:03:45[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2255
还是直接来段代码吧。
[javascript]
chart = new Highcharts.Chart({ 
    plotOptions: { 
        series: { 
            cursor: 'pointer', 
            events: { 
                click: function(e) { 
                    alert(e.point.category); 
                } 
            } 
        } 
    }, 
chart = new Highcharts.Chart({
 plotOptions: {
  series: {
   cursor: 'pointer',
   events: {
    click: function(e) {
     alert(e.point.category);
    }
   }
  }
 }, 从代码中可以看出e.point就是点击事件所获得的点击点的对象。category是对应的x轴的值，也可以获得y轴对应的值等。
