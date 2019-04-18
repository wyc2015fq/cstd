# jquery datepicker 插件使用 - z69183787的专栏 - CSDN博客
2013年01月23日 10:21:52[OkidoGreen](https://me.csdn.net/z69183787)阅读数：3640
首先子啊 jqueryui 的官网下载datepicker 控件的 rar包
css:
```java
<link type="text/css" href="../../css/flick/jquery-ui-1.8.18.custom.css" rel="stylesheet" />
```
```
<style type="text/css">
        .ui-datepicker-title span {display:inline;}
        button.ui-datepicker-current { display: none; }
		</style>
```
设置currentButton  今天按钮 不显示。
js:
```java
<script src="../js/datepicker/js/jquery-ui-1.8.18.custom.min.js"></script>
<script type="text/javascript" src="../ui/i18n/jquery.ui.datepicker-zh-CN.js"></script>
```
第一个为核心代码，第二个为文字显示，此例中为中文
```java
$('#indicatorDate').datepicker({
				inline: true,
				changeYear: true,
				changeMonth: true
				showButtonPanel:true,	
				closeText:'清除',	
				currentText:'inDate'//仅作为“清除”按钮的判断条件
				
			});
			
			$("#indicatorDate").datepicker('option', 'maxDate', inDate); 若设置maxDate小于今天 则无currentButton 按钮
			$(".ui-datepicker-close").live("click", function (){     
			         alert($(this).parent("div").children("button:eq(0)").text());
              if($(this).parent("div").children("button:eq(0)").text()=="inDate")
              { 
              	alert(1);
              	$("#indicatorDate").val("");    
              }       
            });
```
上例中由于增加了 changeMonth 与 changeYear ，上述css文件需要修改 ，width需要短一些。
```
.ui-datepicker select.ui-datepicker-month-year {width: 100%;}
.ui-datepicker select.ui-datepicker-month, 
.ui-datepicker select.ui-datepicker-year { width: 45%;}
```
![](https://img-my.csdn.net/uploads/201301/23/1358907637_4465.jpg)
最后，该控件还提供了很多 事件及方法，具体可在官网api及范例中查到。
```java
$("input[name=contractStartDate]").datepicker({
		inline: true,
		changeYear:true,
		changeMonth:true,
		showOtherMonths: true,
		selectOtherMonths: true,
		onSelect: function( selectedDate ) {
			$( "input[name=contractEndDate]" ).datepicker( "option", "minDate", selectedDate );
		}
	});
```
选择日期后，控制另一日期控件最小值。用于时间段的选择。
