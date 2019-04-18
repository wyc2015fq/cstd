# js中获取时间new Date()详细介绍 - z69183787的专栏 - CSDN博客
2012年11月18日 19:56:04[OkidoGreen](https://me.csdn.net/z69183787)阅读数：97315
var myDate = new Date();
myDate.getYear(); //获取当前年份(2位)
myDate.getFullYear(); //获取完整的年份(4位,1970-????)
myDate.getMonth(); //获取当前月份(0-11,0代表1月)         // 所以获取当前月份是myDate.getMonth()+1; 
myDate.getDate(); //获取当前日(1-31)
myDate.getDay(); //获取当前星期X(0-6,0代表星期天)
myDate.getTime(); //获取当前时间(从1970.1.1开始的毫秒数)
myDate.getHours(); //获取当前小时数(0-23)
myDate.getMinutes(); //获取当前分钟数(0-59)
myDate.getSeconds(); //获取当前秒数(0-59)
myDate.getMilliseconds(); //获取当前毫秒数(0-999)
myDate.toLocaleDateString(); //获取当前日期
var mytime=myDate.toLocaleTimeString(); //获取当前时间
myDate.toLocaleString( ); //获取日期与时间
==========================================================================
JS获取当前时间戳的方法-JavaScript 获取当前毫秒时间戳有以下三种方法：
var timestamp =Date.parse(new Date());    结果：1280977330000       //不推荐; 毫秒改成了000显示
var timestamp =(new Date()).valueOf();       结果：1280977330748       //推荐; 
var timestamp=new Date().getTime();         结果：1280977330748        //推荐; 
js中单独调用new Date();  显示这种格式  Mar 31 10:10:43 UTC+0800 2012
但是用new Date() 参与计算会自动转换为从1970.1.1开始的毫秒数
--------------------------------------------------------------------------------------------------
**将字符串形式的日期转换成日期对象**
var strTime="2011-04-16";    //字符串日期格式           
var date= new Date(Date.parse(strTime.replace(/-/g,  "/")));      //转换成Data();
--------------------------------------------------------------------------------------------------
new Date() ;     //参数可以为整数; 也可以为字符串; 但格式必须正确
new Date(2009,1,1);       //正确
new Date("2009/1/1");    //正确
new Date("2009-1-1");    //错误
new Date( year, month, date, hrs, min, sec)  按给定的参数创建一日期对象
　　参数说明：
　　year的值为：需设定的年份-1900。例如需设定的年份是1997则year的值应为97，即1997-1900的结果。所以Date中可设定的年份最小为1900；
　　month的值域为0～11，0代表1月，11表代表12月；
　　date的值域在1～31之间；
　　hrs的值域在0～23之间。从午夜到次日凌晨1点间hrs=0，从中午到下午1点间hrs=12；
　　min和sec的值域在0～59之间。
　　例 Date day=new Date(11,3,4);
　　//day中的时间为：04-Apr-11 12:00:00 AM
       另外，还可以给出不正确的参数。
　　例　设定时间为1910年2月30日，它将被解释成3月2日。
　　Date day=new Date(10,1,30,10,12,34);
　　System.out.println("Day's date is:"+day);
　　//打印结果为：Day's date is:Web Mar 02 10:13:34 GMT+08:00 1910
--------------------------------------------------------------------------------------------------
1、当前系统区域设置格式(toLocaleDateString和toLocaleTimeString)
例子:(new Date()).toLocaleDateString() + " " + (new Date()).toLocaleTimeString()
     结果: 2008年1月29日 16:13:11
2.普通字符串(toDateString和toTimeString)
     例子: (new Date()).toDateString() + " " + (new Date()).toTimeString()
     结果:Tue Jan 29 2008 16:13:11 UTC+0800
3.格林威治标准时间(toGMTString)
     例子: (new Date()).toGMTString()
     结果:Tue, 29 Jan 2008 08:13:11 UTC
4.全球标准时间(toUTCString)
     例子: (new Date()).toUTCString()
     结果:Tue, 29 Jan 2008 08:13:11 UTC
5.Date对象字符串(toString)
     例子: (new Date()).toString()
     结果:Tue Jan 29 16:13:11 UTC+0800 2008
--------------------------------------------------------------------------------------------------
日期对象也可用于比较两个日期。
     var myDate=new Date(); myDate.setFullYear(2008,7,9);
     var today = new Date();
     if (myDate>today){
         ......;
      }
