# highChart另类使用及一些属性 - z69183787的专栏 - CSDN博客
2013年11月26日 20:00:18[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1481
效果图：
![](https://img-blog.csdn.net/20131126195904921?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvejY5MTgzNzg3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
具体代码：
包括背景透明，图表中载入图片等；
```java
function showPie(name){
	var a =  [
                		{name:'A',y:100,href:'http://www.baidu.com',color:'#D587CE'},
                    {name:'B',y:30,href:'http://www.baidu.com',color:'#34E3FF'},
                    {name:'C',y:10,href:'http://www.baidu.com',color:'#8EF58B'},
                    {name:'D',y:10,href:'http://www.baidu.com',color:'#F1FE19'},
                    {name:'E',y:5,href:'http://www.baidu.com',color:'#F8E104'},
                    {name:'F',y:5,href:'http://www.baidu.com',color:'#F68101'}
          ]
          
	var  chartOption = {
            chart: {
                renderTo: name,
                backgroundColor: 'rgba(255, 255, 255, 0)',
                plotBorderColor : null,
                plotBackgroundColor: null,
                plotBackgroundImage:null,
                plotBorderWidth: null,
                plotShadow: false,  
                borderWidth : 0,
                events: {
                load: function() {
                    this.renderer.image('cg.png', 41, 38, 40, 43) //位置参数 left top right bottom
                        .attr({
                            zIndex: 7
                        })
                        .add();
                }
            }
            },
						credits :{
								enabled:false
						},
            title: {
                text: ''
            },
            tooltip: {
                formatter: function() {
                    //alert(1);
                    //return '<b>'+ this.point.name +'</b>: '+ this.percentage +' %';
                     return '<b>'+ this.point.name +'</b>: '+ this.y;
                },
                style: {
                    color: '#333333',
                    fontSize: '10pt',
                    padding: 5,
                    zIndex:9999
                }
            },
            plotOptions: {
                pie: {
                    allowPointSelect: true,
                    borderWidth : 0,
                    cursor: 'pointer',
	                  dataLabels: {
	                  enabled: false
	                },
	              showInLegend: false,
	              point: {  
	                    events : {  
	                         click: function(event){
	                           alert(this.y);      
	                             window.open(this.href);                            
	                         }
	                    }
	                  },
               dataLabels: {
                  enabled: false,
                  color: '#000000',
                  //distance: -20,
                  connectorColor: '#000000',
                  formatter: function() {
                      return '<b>'+ this.point.name +'</b>: '+ this.percentage +' %';
                  }
              },
               showInLegend: false,
               size:68
	              }
            },
            
            series: [{
                type: 'pie',
                name: 'Browser share',
                data: a
                }]
         }	
         
          new Highcharts.Chart(chartOption); 
}
```
