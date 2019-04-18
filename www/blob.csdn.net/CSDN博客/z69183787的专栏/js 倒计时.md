# js 倒计时 - z69183787的专栏 - CSDN博客
2012年10月26日 16:32:34[OkidoGreen](https://me.csdn.net/z69183787)阅读数：644
个人分类：[Javascript-概述](https://blog.csdn.net/z69183787/article/category/2175033)
```
<!--倒计时-->    
<meta http-equiv="Content-Type" content="text/html; charset=utf-8">  
<script language="JavaScript">    
function Dclock()    
{     
var symbol="3月31日";  
var deadline= new Date("03/31/2013 11:25:00"); //标准时间格式  
var now = new Date();  
var diff = -480 - now.getTimezoneOffset(); //是北京时间和当地时间的时间差   
//alert(diff); 
var leave = (deadline.getTime() - now.getTime()) + diff*60000; //getTime()可以取得1970之后的毫秒数，1970年前为负数  
var day = Math.floor(leave / (1000 * 60 * 60 * 24));  //天数  
var hour = Math.floor(leave / (1000*3600)) - (day * 24);    
var minute = Math.floor(leave / (1000*60)) - (day * 24 *60) - (hour * 60);    
var second = Math.floor(leave / (1000)) - (day * 24 *60*60) - (hour * 60 * 60) - (minute*60);    
  
if (now.getTime()<deadline.getTime())  
{  
    LiveClock.innerHTML = "现在离"+symbol+"还有"+day+"天"+hour+"小时"+minute+"分"+second +"秒"  ;  
    test.innerHTML= "getTime:"+now.getTime()+"Deadline:"+deadline.getTime();  
    setTimeout("Dclock()",1000); //调用自身每隔1秒   
}    
else if (now.getTime()>deadline.getTime())  
{  
    LiveClock.innerHTML = "END";  
    setTimeout("Dclock()",1000);  
}  
}    
</script>    
<!--倒计时-->    
<body onload=Dclock()>    
<div id= LiveClock></div>    
<div id=test></div>  
</body>
```
