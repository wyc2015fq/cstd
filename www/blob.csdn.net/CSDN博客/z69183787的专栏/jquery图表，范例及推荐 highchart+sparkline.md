# jquery图表，范例及推荐 highchart+sparkline - z69183787的专栏 - CSDN博客
2012年10月22日 10:05:30[OkidoGreen](https://me.csdn.net/z69183787)阅读数：6141标签：[jquery图表																[function																[null																[jquery																[html](https://so.csdn.net/so/search/s.do?q=html&t=blog)](https://so.csdn.net/so/search/s.do?q=jquery&t=blog)](https://so.csdn.net/so/search/s.do?q=null&t=blog)](https://so.csdn.net/so/search/s.do?q=function&t=blog)](https://so.csdn.net/so/search/s.do?q=jquery图表&t=blog)
个人分类：[js图表-Highcharts																[Javascript-Jquery																[js图表-Sparkline](https://blog.csdn.net/z69183787/article/category/2175115)](https://blog.csdn.net/z69183787/article/category/2175111)](https://blog.csdn.net/z69183787/article/category/2175113)
HIGHCHARTS 
官网：[http://www.highcharts.com/](http://www.highcharts.com/)
code：具体操作网管API中都有。
```java
/**
  * 画一个柱状图
  * @param valueList 图表的值 (数组)
  * @param dateList	日期值	(数组)
  * @param renderTo	要显示位置的id
  * @param controlValue 管控值 (数组)
  * @colorA 最后一条数据显示的颜色
  */
 function newChartColumn(valueList,dateList,renderTo,controlValue,colorA){
 	var  chartOption ={
 		chart: {
 		    renderTo: renderTo,
 		    type: 'column',
 		    height: h,
 		    borderWidth:0,
 		    width:w
 		},
 		credits:{					
 			enabled:false
 		},
 		legend: {					
 					enabled: false
 				},
 		title: {
 			text:null
 		},
 		tooltip: {
              formatter: function() {
                 return this.x + '<br>值:'+ this.y +'';
             },
             style: {
                 padding: '10px',
                 fontWeight: 'bold',
                 fontSize:'12px'
             }
         },
 		xAxis: {
 			categories:dateList,
 			minPadding: 0.05,
 			maxPadding: 0.05,
 			labels: {
                 style: {
                     fontSize:'9px'
                 }
             }
 		},
 		yAxis:{
 			title: {
 		    	text: null
 			}, 
 			labels: {
 				align: 'left',
                 style: {
                     fontSize:'9px'
                 }
             }
 		}
 	};
 	
 	var controlList = new Array();
 	if(controlValue!=null && controlValue.length>0){
 		
 		for(var i=0; i<controlValue.length; i++){
 			if(controlValue[i]!=null && !isNaN(controlValue[i]) && controlValue[i]!=""){
 				controlList.push(parseInt(controlValue[i]));
 			}else{
 				//controlList.push(0);	//0能成功成功
 				controlList.push(null);
 			}
 		}
 		
 		
 		var realData = new Array();
 		var innerData;
 		for(var i=0; i<valueList.length; i++){
 			if(i!=valueList.length-1){
 				innerData = {y:valueList[i],color:'#4572A7'};
 			}else{
 				innerData = {y:valueList[i],color:colorA=='' ? '#4572A7' : colorA};
 			}
 			realData.push(innerData);
 		}
 		chartOption.series = [{
 			data:realData},
 	  		{name:	"管控线",
	 			data:controlList,
	 			type:'line',
	 			color:'#80699B'}
 		];
 	}else{
 		chartOption.series = [{
 			name:'值',
 			data:valueList
 		}]
 	  		
 	}
 	new Highcharts.Chart(chartOption); 
 }
```
```java
//画一个柱状图
function newChartColumn(valueList,dateList,renderTo,unit,interval,controlList){
	
	if(interval=='undefined') interval=1;
	var chart1 = new Highcharts.Chart({
		chart: {
		    renderTo: renderTo,
		    type: 'column',
		    borderWidth:0
		},
		credits:{					//右边下标HighCharts.com去除
			enabled:false
		},
		legend: {					//正下方数据线名称，点击可以显示和删除
					enabled: false
				},
		title: {
			text:unit,
			style:{
				fontSize:'24px',
				color:'#4572A7'
			}
		},
		plotOptions: {
            series: {
                marker: {
                    enabled: false,
                    states: {
                        hover: {
                            enabled: true
                        }
                    }
                }
            }
        },
		tooltip: {
            formatter: function() {
            	return this.x + '<br>值:'+ this.y +'';
           },
           style: {
               padding: '10px',
               fontWeight: 'bold',
               fontSize:'12px'
           }
		},
		xAxis: {
			categories:dateList,
			minPadding: 0.05,
			maxPadding: 0.05,
			labels: {
			 	staggerLines: 2,
                style: {
                    fontSize:'10px'
                }
            },
            tickInterval:interval
		},
		yAxis:{
			title: {
		    	text: null
			}, 
			labels: {
                style: {
                    fontSize:'10px'
                }
            }
		},
		series: [
			{
				name: "值",
	  			data: valueList
	  		},{
	  			name:'管控线',
	  			data:controlList,
	  			type:'line',
	  			color:'#80699B'
	  		}]
	});
}
//画一个饼状图
function newChartPie(valueList,renderTo){
	var chart = new Highcharts.Chart({
		chart: {
			renderTo: renderTo,
			plotBackgroundColor: null,
			plotBorderWidth: null,
			plotShadow: false,
       		borderWidth:0
		},
		title: {
			text: ''
		},
		credits: {
			enabled:false
		},
		legend:{
			layout:'vertical',
			align:'right',
			floating: true
		},
		tooltip: {
            formatter: function() {
              return "<b>"+this.point.name + '<b/>：'+ this.y +' 列';
           },
           style: {
               padding: '10px',
               //fontWeight: 'bold',
               fontSize:'14px'
           }
		},
		plotOptions: {
			pie: {
				size:'100%',
				allowPointSelect: true,
				cursor: 'pointer',
				dataLabels: {
					enabled: true,			
					color: 'black',			
					connectorColor: '#333',
					distance: -20,
					style: {		
                        fontSize:'16px'
                    },
					formatter: function() {
						return this.point.name+'：'+this.percentage.toFixed(2)+'%';	//this.percentage 百分比
					}
				},
				showInLegend:true
				
			}
		},
		series: [{
			type: 'pie',
			name: '值',
			data: [
				{	name :"4节编组",
					y : valueList[0],
					color : '#79BB25'
				},
				{	name :"6节编组",
					y : valueList[1],
					color : '#FFC002'
				},
				{	name :"7节编组",
					y : valueList[2],
					color : '#4572A7'
				},
				{	name :"8节编组",
					y : valueList[3],
					color : '#FE5917'
				}
			]
		}]
	});
}
//画一个折线图
function newChartLine(valueList,dateList,renderTo,interval,controlList){
	if(interval=='undefined') interval=1;
	var chart1 = new Highcharts.Chart({
		chart: {
		    renderTo: renderTo,
		    type: 'line',
		    borderWidth:0
		},
		credits:{					//右边下标HighCharts.com去除
			enabled:false
		},
		legend: {					//正下方数据线名称，点击可以显示和删除
					enabled: false
				},
		title: {
			text:null
		},
		tooltip: {
            formatter: function() {
               return this.x + '<br>'+this.series.name+":"+ this.y +'';
           },
           style: {
               padding: '10px',
               fontWeight: 'bold',
               fontSize:'12px'
           }
		},
		plotOptions:{
			dataLables:{
				enabled:true
			},
			series: {
                marker: {
                    enabled: false,
                    states: {
                        hover: {
                            enabled: true
                        }
                    }
                }
            }
		},
		xAxis: {
			categories:dateList,
			minPadding: 0.05,
			maxPadding: 0.05,
			tickInterval:interval
		},
		yAxis:{
			title: {
		    	text: null
			} 
		},
		series: [{
			name:"值",
	  		data: valueList
		},{
  			name:'管控线',
  			data:controlList,
  			type:'line',
  			color:'#80699B'
  		}]
	});
}
```
sample：
![](https://img-my.csdn.net/uploads/201210/22/1350871762_6611.jpg)
![](https://img-my.csdn.net/uploads/201210/22/1350871789_9673.jpg)
![](https://img-my.csdn.net/uploads/201210/22/1350871811_5963.jpg)
jquery sparkline  小型图表
官网 [http://omnipotent.net/jquery.sparkline/#s-about](http://omnipotent.net/jquery.sparkline/#s-about)
code：
```
function newSparkBar(index,dateList,valueList,controlValue,title,measure){
	var data = '';
	var maxData = arrMax(valueList);
	var minData = arrMin(valueList);
	var cmaxData = arrMax(controlValue);
	var cminData = arrMin(controlValue);
	//controlValue = ((maxData+minData)/2).toFixed(4);
	if(controlValue!=null&&controlValue.length>0){
		if(cmaxData>maxData){
			maxData = cmaxData;
		}
		if(cminData<minData&&cminData!=0){
			minData = cminData;
		}
	}
	
	for(var i=0; i<valueList.length; i++){
		data += valueList[i];
		data += ",";
		}
	data = data.substr(0,data.length-1);
	$(index).html(data);
	$(index).sparkline('html',
			{type:'bar',
				barColor:'#0072E3',
				width:150,
				height:40,
				barWidth:7,
				barSpacing:3,
				tooltipChartTitle:'<span style="display:inline;">   </span>'+title,
				tooltipSuffix:measure,
				tooltipFormat: '  <span style="display:inline;color: {{color}}">●</span> 实际值： {{prefix}}{{value}}{{suffix}}',
				chartRangeMin:0,
				chartRangeMax:maxData
			}); 
	
	if(controlValue!=''&&controlValue.length>0&&cmaxData!=0&&cminData!=0){
		var control = controlValue;
		//for(var i=0; i<valueList.length; i++){
		//	control[i] = controlValue;
		//}
		
		$(index).sparkline(control,
				{type:'line',
					spotRadius:2,
					lineWidth:2,
					width:150,height:40,
					tooltipSuffix:measure,
					composite: true, 
					tooltipFormat: '<span style="display:inline;color: {{color}}">●</span> 管控值： {{prefix}}{{y}}{{suffix}}',
					fillColor: false, 
					lineColor: '#FFA042',
					highlightLineColor:'red',
					chartRangeMin:0,
					chartRangeMax:maxData
				}); 		
	}
}
```
sample：
![](https://img-my.csdn.net/uploads/201210/22/1350871693_7320.jpg)
![](https://img-my.csdn.net/uploads/201210/22/1350871717_8922.jpg)
