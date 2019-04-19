# highcharts获取点击事件及其点击点的值 - 零度的博客专栏 - CSDN博客
2016年04月06日 10:16:28[零度anngle](https://me.csdn.net/zmx729618)阅读数：16643

直接上代码[javascript]：
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
