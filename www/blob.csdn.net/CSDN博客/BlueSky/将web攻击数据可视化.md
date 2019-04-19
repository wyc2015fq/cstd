# 将web攻击数据可视化 - BlueSky - CSDN博客
2015年11月25日 09:29:51[ASIA_kobe](https://me.csdn.net/ASIA_kobe)阅读数：621
转自：http://www.freebuf.com/articles/web/25476.html
去年一直在做日志分析的工作，略有心得，对于恶意攻击我们需要知道啥时候有人攻击我们了，攻击了多少次，这个有利于领导知道你做了哪些工作。从防护者的角度来说，我不止关心payload、或者request url还关心谁经常来攻击我们，在哪些时间段比较集中，这些都需要数据化，从程序员的角度思考，能否用程序来实现呢，答案当然是可以，因为只是工作而不是做产品，我搜索了下，发现highcharts比较适合这个case，于是研究了几天做了个demo，主要是对攻击数据的展现。
这里需要说下attack != 成功攻击，但对于恶意扫描也是需要关注的，试想一个0day来打你的网站只打了一次那可能是0day，如果我们关注日志分析就可以发现，而如果这个是nday，在你的多个网站上都出现了，那我们就判断这个是xday，诸如此类。
这个部分涉及到一些前端页面展示的技能，读者可自信脑补，程序实现设计到php+mysql+html+highcharts，因为只是个demo，本着keep simple的原则，能用就好。
1、首先建表：
`+----------------+----------------------------------------------------------------------------------------------------------------------------------------------+| Table          | Create Table                                                                                                                                 |+----------------+----------------------------------------------------------------------------------------------------------------------------------------------+| highcharts_php | CREATE TABLE `highcharts_php` (  `timespan` varchar(200) DEFAULT NULL,  `visits` int(11) DEFAULT NULL) ENGINE=MyISAM DEFAULT CHARSET=utf8 |+----------------+----------------------------------------------------------------------------------------------------------------------------------------------+1 row in set (0.00 sec)`
2、编写Php页面，来回显json，这里需要说下，官方的highcharts demo里实现是用jsonp，我这里实现也是jsonp，jsonp是通过javascript callback实现调用的，当然这里也可能存在安全问题，本文主要讨论如何实现将咱们获取到的攻击数据可视化。
```php
<?php
header("Content-type: text/json");   //header输出成json格式
$callback = isset($_GET['callback']) ? $_GET['callback'] : '';
$json = '';
$con = mysql_connect("localhost","root","");
if (!$con) {
  die('Could not connect: ' . mysql_error());
}
mysql_select_db("audit", $con);
$result = mysql_query("SELECT * FROM highcharts_php");
while($row = mysql_fetch_array($result)) {
  $the_date = gmdate(strtotime($row['timespan'])) * 1000; // highcharts需要格式化时间成highcharts需要的格式，例如：February 11, 2014, 08:02:03 
  $results[] = array($the_date,intval($row['visits']));   //攻击次数
}
   if (isset($_GET['callback'])) {
   $json = json_encode($results);
   echo $callback . '(' . $json . ')'; 
}   else  {
   echo "please enter yor pass key!";
}
mysql_close($con);
?>
```
上边的调用方式主要是使用http://127.0.0.1/test/test.php?callback来实现回调json，如果使用http://127.0.0.1/test/test.php来调用会返回please enter yor pass key!
3、实现页面展现，在这里不麻烦，但相对来说，需要多琢磨琢磨，按照官方demo copy下来，然后改下数据。
```
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN"
"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head> 
<meta http-equiv="Content-Type" content="text/html; charset=utf-8"/> 
 
<title>24小时web攻击趋势展现</title>
<script type="text/javascript" src="js/jquery-1.7.1.min.js" ></script>
<script type="java/javascript" src="js/exporting.js"></script>
<script type="text/javascript" src="js/themes/grid.js"></script>
<script type="text/javascript">
$(function() {
         
        Highcharts.setOptions({
                global : {
                useUTC : false
                }
        });
        $.getJSON('http://127.0.0.1/test/test.php?callback=?', function(data) { 
                // Create the chart
                $('#container').highcharts('StockChart', {
                        rangeSelector : {
                                selected : 1
                        },
                        credits: { 
                           enabled: false   //右下角不显示LOGO 
                        },
                        title : {     // 标题
                               text : '网络攻击趋势图'
                        },
                        subtitle: {   //副标题
                            text: 'web攻击趋势',
                            x: -20   //居中
                        },
                        yAxis: {   //坐标Y轴
                            title: {
                                text: '受到网络攻击次数'
                                },
                                plotLines: [{
                                   value: 0,
                                   width: 1,
                                   color: '#808080'
                               }]
                        },
                        series : [{
                                name : '攻击数',
                                data : data,
                                marker : {
                                       enabled : true,
                                       radius : 3
                                },
                                shadow : true,
                                tooltip: {
                                valueDecimals: 2
                                }
                        }]
                });
        });
});
                </script>
        </head>
        <body>
<script src="js/highstock.js"></script>
<script src="js/exporting.js"></script>
<div id="container" style="height: 500px; min-width: 500px"></div>
```
上边的数据比较好理解，div里参数container实现调用highcharts来画图，highcharts里的参数主要是使用javascript jsonp来实现获取数据。
4、既然有了demo，那就需要有图，demo图如下：
![](http://image.3001.net/images/20140211/13920875354818.jpg!small)
EOF
