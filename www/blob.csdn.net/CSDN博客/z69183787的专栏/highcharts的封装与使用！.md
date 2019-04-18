# highcharts的封装与使用！ - z69183787的专栏 - CSDN博客
2014年12月15日 21:10:07[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1881
使用环境：struts2 json
使用目的：统计图
流程：只需要在jsp页面配置，就能实现数据的线状图，柱状图，饼状图的统计
一个例子：
1.JSP页面：
```java
<%@ page contentType="text/html; charset=utf-8" %>
<%@ taglib prefix="s" uri="/struts-tags"%>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>浙江省食品药品监督管理局公众服务平台</title>
<link href="/css/main.css" rel="stylesheet" type="text/css" />
<link href="/css/column.css" rel="stylesheet" type="text/css" />
<link href="/css/myChart.css" rel="stylesheet" type="text/css" />
<script language="javascript" type="text/javascript" src="/js/jquery-1.5.2.min.js" ></script>
<script language="javascript" type="text/javascript" src="/jqPlugIn/highcharts/highcharts.js"></script>
<script language="javascript" type="text/javascript" src="/js/myChart.js" ></script>
<script language="javascript" type="text/javascript" src="/js/myChartTheme.js" ></script>
<script language="javascript" type="text/javascript" src="/My97DatePicker/WdatePicker.js"></script>
<script type="text/javascript">
$(function(){
  var sql = "select count(*) from tb_interac_consultative where time_treatment between ? and ? ";
  
  // 初始化线状图对象
  var line = new MyChart(0);
  line.xAppend("已回复",sql+"and ct_treatment_status=?",new Array("1"));
  line.xAppend("回复中",sql+"and ct_treatment_status=?",new Array("2"));
  line.xAppend("未回复",sql+"and ct_treatment_status=?",new Array("0"));
  line.setTime("timeStart","timeEnd","timetype");
    
  // 初始化柱状图对象
  var column = new MyChart(1).cloneAttr(line);
  
  // 初始化饼状图对象
  var pie = new MyChart(2);
  pie.fAppend("已回复",sql+"and ct_treatment_status=?",new Array("1"));
  pie.fAppend("回复中",sql+"and ct_treatment_status=?",new Array("2"));
  pie.fAppend("未回复",sql+"and ct_treatment_status=?",new Array("0"));
  pie.setTime("timeStart","timeEnd","timetype");
  
  var myHighcharts = new MyHighcharts({
    title:"科大公司在线咨询信息统计",
    subTitle:"Source:http://www.zjda.com",
    xTitle:"人数",
    yTitle:"信息条数",
    line:line,
    column:column,
    pie:pie
  });
  
  myHighcharts.draw(0);	
});
</script>
</head>
<body>
<jsp:include page="/header.jsp" flush="true"/>
<div class="tenHeight"></div>
<table width="960" border="0" align="center" cellpadding="0" cellspacing="0">
  <tr>
    <td width="706" align="left" valign="top"><table width="100%" border="0" cellpadding="0" cellspacing="0" class="positionArea">
      <tr>
        <td>当前位置：<a href="/">首页</a>>> <a href="/onlineInteract.jsp">互动信息</a>>> <a href="/zjfda/statistic/consultative.jsp">咨询信息统计</a></td>
      </tr>
    </table>
<div class="tenHeight"></div>
<!-- 数据统计图 -->
<div id="myChart">
  <!-- 导航 -->
  <ul id="navigation">
    <li><a href="#" class="current">线状图</a></li>
    <li><a href="#">柱状图</a></li>
    <li><a href="#">饼状图</a></li>
  </ul>
  
  <!-- 统计图 -->
  <div id="container"></div>
  
  <!-- 如果没有用到时间动态统计则删除 -->
  <div id="timeselect">
    时间类型：
    <select id="timetype">
      <!-- <option value="3">年份</option> -->
      <option value="2" selected="selected">月份</option>
      <!-- <option value="1">天数</option> -->
    </select>
    时间段：<input type="text" class="Wdate" id="timeStart" size="12" onclick="WdatePicker()" value="2012-01-01"/>
                至<input type="text" class="Wdate" id="timeEnd" size="12" onclick="WdatePicker()" value="2012-12-30"/>
  </div>
</div>
<!-- 数据统计图 -->
<div class="tenHeight"></div>
<jsp:include page="/footer.jsp" flush="true"/>
</body>
</html>
```
2.myChart.js
```java
// 封装请求参数对象
var Emtoy = function(f,name,sql,params){
  this.f = -1;
  this.name = name;
  this.sql = sql;
  if(params!=null){
    this.params = params.concat();
  }else{
    this.params = null
  }
}
/**
 * 统计图对象对应的JAVA类MyChart.java
 * @param : typechart 
 * 			0表示线状图对象 
 * 			1表示柱状图对象 
 * 			2表示单饼状图对象 
 * 			3表示内嵌饼图对象
 */
var MyChart = function(typechart){
  
  this.title;//统计图标题
  this.subtitle;//统计图副标题
  this.xTitle;//x轴标题
  this.yTitle;//主轴标题
  
  this.typedb;//服务器端的数据源
  this.typechart = typechart;//统计图类类型
  this.typetime = 0;//统计的时间类型
  this.emtoys = new Array();//需要统计的参数
  this.smtoys	= new Array();//需要统计的参数,当统计图是内嵌饼图的时候用到
  this.categories = new Array();//发送到服务器时间分段
  this.categoriesLocal = new Array();//本地轴分段名称
  this.timeAry = new Array();//保存从页面取得的时间的ID
  /**
   * x轴统计内容
   * @param : name   系列名称
   * @param : sql    查询语句
   * @param : params 查询参数
   */
  this.xAppend = function(name,sql,params){
    this.emtoys.push(new Emtoy(null,name,sql,params));
  }
  /**
   * y轴系列内容,时间段查询用
   * @param : name   时间段名称
   * @param : time   时间
   */
  this.yAppend = function(name,time){
    this.categories.push(time);
    this.categoriesLocal.push(name);
  }
  /**
   * 饼状图外层
   * @param : name   系列名称
   * @param : sql    查询语句
   * @param : params 查询参数
   */
  this.fAppend = function(name,sql,params){
    this.emtoys.push(new Emtoy(null,name,sql,params));
  }
  /**
   * 饼状图内层
   * @param : f      外层饼状图的标志
   * @param : name   系列名称
   * @param : sql    查询语句
   * @param : params 查询参数
   */
  this.sAppend = function(f,name,sql,params){
    this.smtoys.push(new Emtoy(f,name,sql,params));
  }
  /**
   * 保存y轴系列时间段，从页面读取
   * @param : timeStart   页面开始时间的ID
   * @param : timeEnd     页面结束时间的ID
   * @param : timetype    页面时间的类型，年或月或日
   */
  this.setTime = function(timeStart,timeEnd,timetype){
    this.timeAry.push(timeStart);
    this.timeAry.push(timeEnd);
    this.timeAry.push(timetype);
  }
  /**
   * 设置y轴系列时间段，从页面读取
   * @param : timeStart   页面开始时间的ID
   * @param : timeEnd     页面结束时间的ID
   * @param : timetype    页面时间的类型，年或月或日
   */
  this.getPageTime = function(){
    if(this.timeAry.length!=0){			
      this.categories = new Array();
      this.categories.push($("#"+this.timeAry[0]).val());
      this.categories.push($("#"+this.timeAry[1]).val());
      this.typetime = $("#"+this.timeAry[2]).val();
      this.xTitle =  $("#"+this.timeAry[2]).find("option:selected").text();
    }else{
      this.categories = null;
    }
  }
  /**
   * 复制一个对象
   * @param : chart     目标对象
   * @param : typechart 指定类型
   */
  this.cloneAttr = function(chart){
    this.title = chart.title;
    this.subtitle = chart.subtitle;
    this.xTitle = chart.xTitle;
    this.yTitle = chart.yTitle;
    this.typedb = chart.typedb;
    this.typetime  = chart.typetime;
    this.emtoys  = chart.emtoys;
    this.smtoys = chart.smtoys;
    this.categories = chart.categories;
    this.categoriesLocal = chart.categoriesLocal;
    this.timeAry = chart.timeAry;
    return this;
  }
}
// 统计图的触发绑定与整理
var MyHighcharts = function(options){
  tempHighcharts = this;
  var defaults = {
      typedb:0,
      title:"这是默认标题",
      subTitle:"这是默认副标题",
      xTitle:"x轴说明",
      yTitle:"y轴说明",
      line:null,
      column:null,
      pie:null
    };
  var options = $.extend(defaults, options);
  
  /** ajax请求，这里用POST提交，因为参数可能拼接的较长 */
  this.draw = function(i){
    // 显示等待信息
    $("#container").empty();
    $("#container").append("<p style=\"text-align: center\"><img src=\"/images/loading.gif\" alt=\"加载中，请稍候...\" /></p>");
    this.initLocalData(i,options);
    $.post("/stat/chart!draw.do",this.initParams(tempChart),this.callBackChart);
  }
  
  /** 数据本地化请求*/	
  this.initLocalData = function(i,options){
    switch (i) {
    case 0:
      tempChart = options.line;
      break;
    case 1:
      tempChart = options.column;
      break;
    default:
      tempChart = options.pie;
      break;
    }
    tempChart.title = options.title;
    tempChart.subtitle = options.subtitle;
    tempChart.xTitle = options.xTitle;
    tempChart.yTitle = options.yTitle;
    tempChart.typedb = options.typedb;
  }
  
  /** 参数处理 */
  this.initParams = function(myChart){
    var param = new Object();
    var timeStr = "1950#1950";
    
    if(myChart.time != 0){
      myChart.getPageTime();
    }
    
    param["myChart.typedb"] = myChart.typedb;
    param["myChart.typechart"] = myChart.typechart;
    param["myChart.typetime"] = myChart.typetime;
    
    if(myChart.categories!=undefined && myChart.categories!=null){
      param["myChart.categoriesStr"] = this.getFztoStr(myChart.categories);
    }else{
      timeStr = "";
    }
    
    if(myChart.emtoys!=undefined && myChart.emtoys!=null){			
      for(var i=0; i<myChart.emtoys.length; i++){
        param["myChart.emtoys["+i+"].name"] = myChart.emtoys[i].name;
        param["myChart.emtoys["+i+"].sql"] = myChart.emtoys[i].sql;
        
        if(myChart.emtoys[i].params!=null && myChart.emtoys[i].params!=""){
          param["myChart.emtoys["+i+"].params"] = (timeStr==""?timeStr:timeStr+"#")+this.getFztoStr(myChart.emtoys[i].params);
        }else{
          if(timeStr != ""){
            param["myChart.emtoys["+i+"].params"] = timeStr;
          }
        }
      }
      for(var k=0; k<myChart.smtoys.length; k++){
        param["myChart.smtoys["+k+"].f"] = myChart.smtoys[k].f;
        param["myChart.smtoys["+k+"].name"] = myChart.smtoys[k].name;
        param["myChart.smtoys["+k+"].sql"] = myChart.smtoys[k].sql;
        param["myChart.smtoys["+k+"].params"] = "2010#2050"+myChart.smtoys[k].params
        if(myChart.smtoys[k].params!=null && myChart.smtoys[k].params!=""){
          param["myChart.smtoys["+k+"].params"] = timeStr==""?timeStr:(timeStr+"#")+this.getFztoStr(myChart.smtoys[k].params)
        }else{
          if(timeStr != ""){
            param["myChart.smtoys["+k+"].params"] = timeStr;
          }
        }
      }
    }	
    
    return param;
  }
  
  this.getFztoStr = function(array){
    var str = "";
    for(var i=0; i<array.length; i++){
      if(i == 0){
        str = str+array[i];
      }else{
        str = str+"#"+array[i];
      }
    }
    return str;
  }
  
  /** 返回数据处理 */
  this.callBackChart = function(data){
    if(tempChart.timetype != 0){
      tempChart.categoriesLocal = data.myChart.categoriesLocal;
    }
    switch (data.myChart.typechart) {
    case 0://线状图
      tempHighcharts.setClass(0);
      tempHighcharts.callBackLine(data);
      break;
    case 1://柱状图
      tempHighcharts.setClass(1);
      tempHighcharts.callBackColumn(data);
      break;
    case 2://单饼图
      tempHighcharts.setClass(2);
      tempHighcharts.callBackPie(data);
      break;
    default://内嵌饼图
      tempHighcharts.setClass(2);
      tempHighcharts.callBackDonutPie(data);
      break;
    }
  }
  
  // line请求返回函数的处理
  this.callBackLine = function(data){
    new Highcharts.Chart({
            chart: {
                renderTo: 'container',
                type: 'line'
            },
            title: {
                text: tempHighcharts.getTimeTitle(tempChart.categoriesLocal)
            },
            subtitle: {
                text: tempChart.subtitle
            },
            xAxis: {
            	title: {
                  text: tempChart.xTitle,
                  align: 'high'
              },
                categories: tempChart.categoriesLocal
            },
            yAxis: {
                title: {
                    align: 'high',
                    offset: 0,
                    text: tempChart.yTitle,
                    rotation: 0,
                    y: -10
                },
                plotLines: [{
    				value: 0,
    				width: 1,
    				color: '#808080'
    			}]
            },
            plotOptions: {
                spline: {
                    marker: {
                        radius: 4,
                        lineColor: '#666666',
                        lineWidth: 1
                    }
                }
            },
            legend:{
            	borderWidth:0
            },
            tooltip: {
            	crosshairs: true,
                shared: true
                
            },
            series: data.myChart.series
        });
  }
  // column请求返回函数的处理
  this.callBackColumn = function(data){
    new Highcharts.Chart({
            chart: {
                renderTo: 'container',
                type: 'column'
            },
            title: {
                text: tempHighcharts.getTimeTitle(tempChart.categoriesLocal)
            },
            subtitle: {
                text: tempChart.subtitle
            },
            xAxis: {
            	title: {
                  text: tempChart.xTitle,
                  align: 'high'
              },
                categories: tempChart.categoriesLocal
            },
            yAxis: {
                title: {
                    align: 'high',
                    offset: 0,
                    text: tempChart.yTitle,
                    rotation: 0,
                    y: -10
                },
                plotLines: [{
    				value: 0,
    				width: 1,
    				color: '#808080'
    			}]
            },
            plotOptions: {
                spline: {
                    marker: {
                        radius: 4,
                        lineColor: '#666666',
                        lineWidth: 1
                    }
                }
            },
            legend:{
            	borderWidth:0
            },
            tooltip: {
            	formatter: function() {
            		return ''+this.x+'<br/>'+
            		this.series.name +': '+ this.y;
        }   
            },
            series: data.myChart.series
        });
  }
  // 单饼状图
  this.callBackPie = function(data){
    new Highcharts.Chart({
      chart: {
        renderTo: 'container',
        plotBackgroundColor: null,
        plotBorderWidth: null,
        plotShadow: false,
        type:'pie'
      },
      title: {
                text: tempHighcharts.getTimeTitle(tempChart.categoriesLocal)
            },
            subtitle: {
                text: tempChart.subtitle
            },
      tooltip: {
        formatter: function() {
          return '<b>'+ this.point.name +'</b>: '+ Highcharts.numberFormat(this.percentage, 2) +' %';
        }
      },
      plotOptions: {
        pie: {
          allowPointSelect: true,
          cursor: 'pointer',
          dataLabels: {
            enabled: true,
            color: '#000000',
            connectorColor: '#000000',
            formatter: function() {
              return '<b>'+ this.point.name +'</b>: '+ Highcharts.numberFormat(this.percentage, 2) +' %';
            }
          }
        }
      },
      series: data.myChart.series_pie
    });
  }
  // 内嵌饼状图
  this.callBackDonutPie = function(data){
    var dt = tempChart.comb(data);
    new Highcharts.Chart({
      chart: {
        renderTo: 'container',
        type: 'pie'
      },
      title: {
                text: tempHighcharts.getTimeTitle(data.myChart.categories)
            },
            subtitle: {
                text: tempChart.subtitle
            },
      yAxis: {
        title: {
          text: 'Total percent market share'
        }
      },
      plotOptions: {
        pie: {
          shadow: false
        }
      },
      tooltip: {
        formatter: function() {
          return '<b>'+ this.point.name +'</b>: '+ this.y +' %';
        }
      },
      series: dt
    });
  }
  // 对内嵌饼状图异步请求产生的数据进行整理然后展示到JSP页面上
  this.comb = function(data){
    var colors = Highcharts.getOptions().colors;
    var pie1 = data.myChart.series_pie[0];
    var pie2 = data.myChart.series_pie[1];
    var firstData = [];
    var secondData = [];
    for (var i = 0; i < pie1.data.length; i++) {
      firstData.push({
        name: pie1.data[i][0],
        y: pie1.data[i][1],
        color: colors[i]
      });
    }
    for (var i = 0; i < pie2.data.length; i++) {
      secondData.push({
        name: pie2.data[i][0],
        y: pie2.data[i][1],
        color:this.getColor(colors,pie2,pie2.data[i])
      });
    }
    
    var dt = [];
    dt.push({
      name: 'first',
      data: firstData,
      size: '60%',
      dataLabels: {
        formatter: function() {
          return this.y > -1 ? this.point.name : null;
        },
        color: 'white',
        distance: -30
      }
    });
    dt.push({
      name: 'second',
      data: secondData,
      innerSize: '60%',
      dataLabels: {
        formatter: function() {
          return this.y > -1 ? '<b>'+ this.point.name +':</b> '+ this.y +'%'  : null;
        }
      }
    });	
    return dt;
  }
  // 内嵌饼状图-子类的颜色
  this.getColor = function(colors,pie2,dt){
    var one = 0;
    var all = 0;
    var tempAy = [];
    for (var i = 0; i < pie2.data.length; i++) {
      if(pie2.data[i][2] == dt[2]){
        tempAy.push(pie2.data[i][0]); 
      }
    }
    all  =tempAy.length;
    for (var i = 0; i < all; i++) {
      if(tempAy[i]== dt[0]){
        one = i;
        continue;
      }
    }
    //alert(dt[0]+":"+one+"/"+all+":"+dt[2]);
    var brightness = 0.2 - (one / all) / 5 ;
    return Highcharts.Color(colors[dt[2]]).brighten(brightness).get();
  }
  this.setClass = function(i){
    var obj = $("#navigation a");
    obj.attr("class","");
    obj.eq(i).attr("class","current");
  }
  this.getTimeTitle = function(categories){
    if(categories == null){
      return tempChart.title;
    }
    var lgh = categories.length;
    return this.pmt(categories[0],0)+"~"+this.pmt(categories[lgh-1],1)+tempChart.title;
  }
  this.pmt = function(str,i){//时间格式化		
    if(str.indexOf("#") != -1){
      str = str.split("#")[i];
    }
    if(str.length==10){
      str = str.substring(0,4)+"年"+str.substring(5,7)+"月"+str.substring(8)+"号";
    }else if(str.length==7){
      str = str.substring(0,4)+"年"+str.substring(5)+"月";
    }else{
      str = str + "年";
    }
    return str;
  }
}
$(function(){
  $("#navigation a").click(function(){
    var i = $("#navigation a").index($(this));
    tempHighcharts.draw(i);
  });	
});
```
3.myChartTheme.js可以在官网下载，这里我作了少量改动，统计饼图的颜色作了调整
```java
/**
 * Grid theme for Highcharts JS
 * @author Torstein Hønsi
 */
Highcharts.theme = {
   colors: ['#058DC7', '#50B432', '#ED561B', '#DDDF00', '#24CBE5', '#64E572', '#FF9655', '#FFF263', '#6AF9C4'],
   chart: {
      backgroundColor: {
         linearGradient: [0, 0, 500, 500],
         stops: [
            [0, 'rgb(255, 255, 255)'],
            [1, 'rgb(240, 240, 255)']
         ]
      },
      borderWidth: 2,
      plotBackgroundColor: 'rgba(255, 255, 255, .9)',
      plotShadow: true,
      plotBorderWidth: 1,
      spacingBottom:25
   },
   title: {
      style: {
         color: '#000',
         font: 'bold 14px "Trebuchet MS", Verdana, sans-serif'
      }
   },
   subtitle: {
      style: {
         color: '#666666',
         font: 'bold 12px "Trebuchet MS", Verdana, sans-serif'
      }
   },
   xAxis: {
      gridLineWidth: 1,
      lineColor: '#000',
      tickColor: '#000',
      labels: {
         style: {
            color: '#000',
            font: '11px Trebuchet MS, Verdana, sans-serif'
         }
      },
      title: {
         style: {
            color: '#333',
            fontWeight: 'bold',
            fontSize: '12px',
            fontFamily: 'Trebuchet MS, Verdana, sans-serif'
         }
      }
   },
   yAxis: {
      minorTickInterval: 'auto',
      lineColor: '#000',
      lineWidth: 1,
      tickWidth: 1,
      tickColor: '#000',
      labels: {
         style: {
            color: '#000',
            font: '11px Trebuchet MS, Verdana, sans-serif'
         }
      },
      title: {
         style: {
            color: '#333',
            fontWeight: 'bold',
            fontSize: '12px',
            fontFamily: 'Trebuchet MS, Verdana, sans-serif'
         }
      }
   },
   legend: {
      itemStyle: {
         font: '9pt Trebuchet MS, Verdana, sans-serif',
         color: 'black'
      },
      itemHoverStyle: {
         color: '#039'
      },
      itemHiddenStyle: {
         color: 'gray'
      }
   },
   labels: {
      style: {
         color: '#99b'
      }
   }
};
// Apply the theme
var highchartsOptions = Highcharts.setOptions(Highcharts.theme);
```
4.sturts.xml
<!-- 统计图ajax --><packagename="fda.bak"namespace="/stat"extends="common-ajax"><actionname="chart"class="com.zjfda.action.MyChartAction"><resulttype="json">chart\.categoriesLocal\[\d+\],
chart\.series\[\d+\]\.name,
chart\.series\[\d+\]\.data\[\d+\]
chart\.series_pie\[\d+\]\.name,
chart\.series_pie\[\d+\]\.data\[\d+\]\[\d+\]
</result></action></package>
4.MyChartAction.java
```java
package com.zjfda.action;
import javax.annotation.Resource;
import com.common.ssh.action.BaseAction;
import com.zjfda.bean.MyChart;
import com.zjfda.service.MyChartService;
/**
 * 线状、柱状、饼状(单饼与内嵌饼)统计图
 * @author LQ.
 *
 */
@SuppressWarnings("serial")
public class MyChartAction extends BaseAction{
  public String draw(){
    myChart = myChartService.drawMyChart(myChart);
    return SUCCESS;
  }
  
  @Resource MyChartService myChartService;
  private MyChart myChart;
  public MyChart getMyChart() {
    return myChart;
  }
  public void setMyChart(MyChart myChart) {
    this.myChart = myChart;
  }
  
}
```
5.MyChartService.java
package com.zjfda.service;
import java.util.ArrayList;
import java.util.List;
import com.common.ssh.dao.BaseJDBCDao;
import com.common.ssh.service.BaseService;
import com.common.util.Tools;
import com.zjfda.bean.Emtoy;
import com.zjfda.bean.MyChart;
import com.zjfda.bean.Serie;
import com.zjfda.bean.Serie_pie;
publicclass MyChartService extends BaseService {
public MyChart drawMyChart(MyChart myChart) {
BaseJDBCDao dao;
/* TODO 如果是多数据源，在这里通过myChart.getTypedb()判断选择哪个数据源*/switch (myChart.getTypedb()) {
case1:
dao = spDao;
break;
default:
dao = fdaDao;
break;
}
switch (myChart.getTypechart()) {
case3:
myChart = getPieInline(dao,myChart);
break;
case2:
myChart = getPie(dao,myChart);
break;
case1:
myChart = getColumn(dao,myChart);
break;
default:
myChart = getLine(dao,myChart);
break;
}
return myChart;
}
// 内嵌饼图private MyChart getPieInline(BaseJDBCDao dao, MyChart myChart) {
List<Serie_pie> series_pie = new ArrayList<Serie_pie>();
String[] categories = getCombCate(myChart);
myChart.setCategoriesLocal(categories);
series_pie.add(getPieSeries(dao,myChart.getEmtoys(),categories));
series_pie.add(getPieSeries(dao,myChart.getSmtoys(),categories));
myChart.setSeries_pie(series_pie);
return myChart;
}
// 单饼图private MyChart getPie(BaseJDBCDao dao, MyChart myChart) {
List<Serie_pie> series_pie = new ArrayList<Serie_pie>();
String[] categories = getCombCate(myChart);
myChart.setCategoriesLocal(categories);
series_pie.add(getPieSeries(dao,myChart.getEmtoys(),categories));
myChart.setSeries_pie(series_pie);
return myChart;
}
// 柱状图private MyChart getColumn(BaseJDBCDao dao, MyChart myChart) {
return getLine(dao,myChart);
}
// 线状图private MyChart getLine(BaseJDBCDao dao, MyChart myChart) {
List<Emtoy> emtoys = myChart.getEmtoys();
String[] categories = getCombCate(myChart);
myChart.setCategoriesLocal(categories);
List<Serie> series = new ArrayList<Serie>();
int length = categories.length;
int line[];
Serie serie;
for (Emtoy emtoy : emtoys) {
line = newint[length];
for (int i = 0; i < length; i++) {
line[i] = dao.count(emtoy.getSql(), getRealParams(emtoy.getParams().split("#"),categories,i));
}
serie = new Serie();
serie.setName(emtoy.getName());
serie.setData(line);
series.add(serie);
}
myChart.setSeries(series);
return myChart;
}
// 饼图的数据处理private Serie_pie getPieSeries(BaseJDBCDao dao, List<Emtoy> emtoys,String[] categories) {
int length = emtoys.size();
Object pie[][] = new Object[length][3];
Emtoy emtoy;
double temp;
for (int i = 0; i < length; i++) {
emtoy = emtoys.get(i);
temp = dao.count(emtoy.getSql(),getRealParamsPie(emtoy.getParams(),categories));	
pie[i][0] = emtoy.getName();
pie[i][1] = temp;
pie[i][2] = emtoy.getF();
}
Serie_pie serie_pie = new Serie_pie();
serie_pie.setData(pie);
return serie_pie;
}
private Object[] getRealParamsPie(String paramsStr, String[] categories) {
String[] params;
if(paramsStr==null && categories==null){
params = new String[]{};
}elseif(paramsStr!=null&&categories==null){
params = paramsStr.split("#");
}elseif(paramsStr==null&&categories!=null){
params = new String[2];
int i = categories.length;
params = combTimeStart(params,categories[0]);
params = combTimeEnd(params,categories[i-1]);
}else{
params = paramsStr.split("#");
int i = categories.length;
params = combTimeStart(params,categories[0]);
params = combTimeEnd(params,categories[i-1]);
}
returnparams;
}
private String[] combTimeEnd(String[] params, String timeStr) {
int i = timeStr.indexOf("#");
if(i == -1){
params[1] = Tools.getTimeEnd(timeStr);
}else{
params[1] = Tools.getTimeEnd(timeStr.substring(i+1));
}
returnparams;
}
private String[] combTimeStart(String[] params, String timeStr) {
int i = timeStr.indexOf("#");
if(i == -1){
params[0] = Tools.getTimeStart(timeStr);
}else{
params[0] = Tools.getTimeStart(timeStr.substring(0,i));
}
returnparams;
}
// 取得查询参数private Object[] getRealParams(Object[] params, String[] categories,int w) {
if(categories == null){
returnparams;
}
String timeStr;
if(categories.length == 1){
timeStr = categories[0];
}else{
timeStr = categories[w];
}
int i = timeStr.indexOf("#");
if(i == -1){
params[0] = Tools.getTimeStart(timeStr);
params[1] = Tools.getTimeEnd(timeStr);
}else{
params[0] = Tools.getTimeStart(timeStr.substring(0,i));
params[1] = Tools.getTimeEnd(timeStr.substring(i+1));
}
returnparams;
}
// 取得时间段private String[] getCombCate(MyChart myChart) {
String[] newCates;
String[] cates = myChart.getCategories();
switch (myChart.getTypetime()) {
case3:
newCates = Tools.getYearList(cates[0], cates[1]);
break;
case2:
newCates = Tools.getMonthList(cates[0], cates[1]);
break;
case1:
newCates = Tools.getDayList(cates[0], cates[1]);
break;
default:
newCates = cates;
break;
}
return newCates;
}
}
6.统计图
![](http://img0.tuicool.com/BvQNjm.png)
![](http://img1.tuicool.com/vqMzaa.png)
![](http://img2.tuicool.com/vmmyia.png)
