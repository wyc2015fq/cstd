# jquery 联动 年月日 - weixin_33985507的博客 - CSDN博客
2017年07月11日 20:29:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：7

```java
<html xmlns="http://www.w3.org/1999/xhtml">  
<head>  
    <title>JQuery实例 - 生成年月日</title>  
    <meta http-equiv="content-type" content="text/html; charset=gbk">  
    <script type="text/javascript" src="jquery.js"></script>  
    <script type="text/javascript">   
        $(function () {  
            var $day = $("#day"),  
              $month = $("#month"),  
              $year = $("#year");  
  
            <!--初始化年-->  
            var dDate = new Date(),  
              dCurYear = dDate.getFullYear(),  
              str = "";  
            for (var i = dCurYear - 70; i < dCurYear + 70; i++) {  
                if (i == dCurYear) {  
                    str = "<option value=" + i + " selected=true>" + i + "</option>";  
                } else {  
                    str = "<option value=" + i + ">" + i + "</option>";  
                }  
                $year.append(str);  
            }  
  
            <!--初始化月-->  
            for (var i = 1; i <= 12; i++) {  
  
                if (i == (dDate.getMonth() + 1)) {  
                    str = "<option value=" + i + " selected=true>" + i + "</option>";  
                } else {  
                    str = "<option value=" + i + ">" + i + "</option>";  
                }  
                $month.append(str);  
            }  
            <!--调用函数出始化日-->  
            TUpdateCal($year.val(), $month.val());  
            $("#year,#month").bind("change", function(){  
                TUpdateCal($year.val(),$month.val());  
            });  
        });  
  
        <!--依据年月获取当月最大天数-->  
        function TGetDaysInMonth(iMonth, iYear) {  
            var dPrevDate = new Date(iYear, iMonth, 0);  
            return dPrevDate.getDate();  
        }  
  
        function TUpdateCal(iYear, iMonth) {  
            var dDate = new Date(),  
                daysInMonth = TGetDaysInMonth(iMonth, iYear),  
                str = "";  
  
            $("#day").empty();  
  
            for (var d = 1; d <= parseInt(daysInMonth); d++) {  
  
                if (d == dDate.getDate()) {  
                    str = "<option value=" + d + " selected=true>" + d + "</option>";  
                } else {  
                    str = "<option value=" + d + ">" + d + "</option>";  
                }  
                $("#day").append(str);  
            }  
        }  
  
    </script>  
  
</head>  
<body>  
<form name="ymd">  
    <select id="year"></select>年  
    <select id="month"></select>月  
    <select id="day"></select>日  
</form>  
</body>  
</html>
```
