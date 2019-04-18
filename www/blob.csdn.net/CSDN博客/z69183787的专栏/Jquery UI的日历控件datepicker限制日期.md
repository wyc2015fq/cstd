# Jquery UI的日历控件datepicker限制日期 - z69183787的专栏 - CSDN博客
2013年12月30日 22:55:10[OkidoGreen](https://me.csdn.net/z69183787)阅读数：14938
JQuery.ui.Datepicker是一个很强大的日期选择控件，定制性很强。朱最近在做一个旅馆预订的小功能，要求用户在填写订单选择入住日期时，要将已经被预订出的日期置为不可用，查些资料，看看JS，原来可以使用beforeshowday来控制每一天的格式。主要代码如下：
- var orderDays = [[4,30,2010],[5,1,2010],[5,14,2010]];  
- function setOrdereDays(date) {  
- var isOrdered = false;  
- var scheduleStatus = "";  
- for (i = 0; i < orderDays.length; i++) {  
- if (date.getMonth() == orderDays[i][0] - 1 && 
      date.getDate() == orderDays[i][1] && date.getFullYear() == orderDays[i][2])  
-       {  
-         isOrdered = true;break;  
-       }  
-     }  
- if(isOrdered){  
- return [false, 'CLOSED'];  
-     }else{  
- return [true, ''];  
-     }  
- }  
“CLOSED”貌似是Datepicker内置的一个什么值，设置为"CLOSED"则当天的日期是不可选的。
ui.Datepicker.js里关于beforeShowDay的一些注释：
beforeShowDay: null, // Function that takes a date and returns an array with
// [0] = true if selectable, false if not, [1] = custom CSS class name(s) or '',
// [2] = cell title (optional), e.g. $.datepicker.noWeekends
**A function that takes a date as a parameter and must return an array with:**
- **`[0]`: `true`/`false` indicating whether or not this date is selectable**
- **`[1]`: a CSS class name to add to the date's cell or `""` for the default presentation**
- **`[2]`: an optional popup tooltip for this date**
**第三个参数可以在该日期上显示title，提示节假日名称。（比如国庆）**
**return [false, 'customCss','国庆'];  **
**[plain]**[view
 plain](http://blog.csdn.net/donthan/article/details/7587555#)[copy](http://blog.csdn.net/donthan/article/details/7587555#)[print](http://blog.csdn.net/donthan/article/details/7587555#)[?](http://blog.csdn.net/donthan/article/details/7587555#)
- 现在jquery用的很广泛，一方面是其强大的库，另一方面是它的插件很丰富。最近用了jquery的datepicker插件，感觉很棒，界面很漂亮，功能齐全，且扩展性很好。强烈推荐使用。  
- 
- 项目中有一个需求，要禁用某些日期，比如周末，节假日。看了datepicker的源码，发现里面有一个noWeekends方法可非常简单地实现禁用周末，请看代码：  
- 
- $("#datepicker").datepicker({  
- beforeShowDay: $.datepicker.noWeekends  
- });  
- 
- 如果要禁用某些特定的日期，可定义一个函数实现，如下：  
- 
- natDays = [  
- [7, 23，2009], [7, 24，2009], [8, 13,2009], [8, 14,2009],  
- ];  
- 
- function nationalDays(date) {  
- for (i = 0; i < natDays.length; i++) {  
- if (date.getFullYear() == natDays[i][2] && date.getMonth() == natDays[i][0] - 1  
- && date.getDate() == natDays[i][1]) {  
- return [false, natDays[i][2] + '_day'];  
- }  
- }  
- return [true, ''];  
- }  
- 
- $("#datepicker").datepicker({  
- beforeShowDay: nationalDays  
- });  
- 
- 上面函数的返回值是以个数组，第一个布尔值表示是否禁用该日期，true为不禁用，false为禁用。第二个参数是以个样式名，可自定义被禁用时的样式。  
- 
- 如果只要求某个时间段的日期可以选择，通过设置两个参数可以实现：  
- 
- $("#datepicker").datepicker({  
- minDate: new Date(2009, 6, 6),  
- maxDate: new Date(2009, 7, 14),  
- });  
- 
- 以上设置表示在2009年5月6号到2009年6月14号是可选的。  
现在考虑我自己的应用，我现在要做淡旺季的价格表，如果选择旺季，日期就不能选择非旺季的日期，淡季和平季也是如此。
这样乍看起来需要界定一个日期范围，但是研究楼上的源代码后看到，只需要限定月份就行了。因为每个淡旺季的开头和结尾都是以自然月的1号和30号（31，28号）为结束的，所以数组那里只需要一个值，而不需要三个值来判断。对于一般的应用，最多也是需要两个，也就是日期和月份，年份几乎每年都一样。
另外一个变通点，起始日期的选择。如果旺季开始于7月，当前日期是5月，那么日历的起始日期应该是7月1日。如果当前日期正好处于旺季，那么就从当天开始作为日期的起始点。
第三，我必须要从一个隐藏元素中获取当前的季节特征，淡季，平季还是旺季，不同的季节有不同的价格。那么因为各个浏览器对于浏览器加载元素的时机不同，所以决定放在jquery的$(function(){});里面。
于是就有了如下的js
**[javascript]**[view
 plain](http://blog.csdn.net/donthan/article/details/7587555#)[copy](http://blog.csdn.net/donthan/article/details/7587555#)[print](http://blog.csdn.net/donthan/article/details/7587555#)[?](http://blog.csdn.net/donthan/article/details/7587555#)
- var d=new Date();  
- var y=d.getFullYear();  
- var m=d.getMonth()+1;  
- var day=d.getDate();  
- 
- 
- function nationalDays(date) {  
- for (i = 0; i < natDays.length; i++) {  
- if (date.getMonth() == natDays[i][0] - 1) {  
- return [false, natDays[i][0] + '_day'];  
-         }  
-     }  
- return [true, ''];  
- }  
- 
- $(function(){  
- var season=document.getElementById('season').value;  
- if(season=='busy'){  
-         document.getElementById('seasonTips').innerHTML='旺季的月份选择只能是4月5月9月10月11月'
- if(m>1 && m<4){  
-             mindate=y+'-4-1';  
-         }elseif(m>5&&m<9){  
-             mindate=y+'-9-1';  
-         }elseif(m>11){  
-             mindate=y+1+'-4-1';  
-         }else{  
-             mindate=y+'-'+m+'-'+day;  
-         }  
- //旺季4,5,9,10,11
-         natDays=[[1],[2],[3],[6], [7], [8],[12],];  
-     }elseif(season=='common'){  
-         document.getElementById('seasonTips').innerHTML='平季的月份选择只能是3月6月7月8月';  
- if(m<3){  
-             mindate=y+'-3-1';  
-         }elseif(m>3 && m<6){  
-             mindate=y+'-6-1';  
-         }elseif(m>8){  
-             mindate=y+1+'-3-1'
-         }else{  
-             mindate=y+'-'+m+'-'+day;  
-         }  
- //平3678
-         natDays=[[1],[2],[4],[5],[9],[10],[11],[12],];  
-     }elseif(season=='dull'){  
-         document.getElementById('seasonTips').innerHTML='淡季的月份选择只能是1月2月和12月';  
- if(m>2 && m<12){  
-             mindate=y+'-12-1';  
-         }else{  
-             mindate=y+'-'+m+'-'+day;  
-         }  
- //淡季12.1.2,禁用345678910
-         natDays=[[3],[4],[5],[6],[7],[8],[9],[10],[11],];  
-     }  
- // Datepicker
-         $('.Wdate').datepicker({  
-             numberOfMonths:2,  
-             showButtonPanel:false,  
-             dateFormat: 'yy-mm-dd',  
-             minDate: mindate,  
-             beforeShowDay: nationalDays,  
-         });  
- });  
Example：
```java
/* create an array of days which need to be disabled */
var disabledDays = ["2-21-2010","2-24-2010","2-27-2010","2-28-2010","3-3-2010","3-17-2010","4-2-2010","4-3-2010","4-4-2010","4-5-2010"];
/* utility functions */
function nationalDays(date) {
	var m = date.getMonth(), d = date.getDate(), y = date.getFullYear();
	//console.log('Checking (raw): ' + m + '-' + d + '-' + y);
	for (i = 0; i < disabledDays.length; i++) {
		if(ArrayContains(disabledDays,(m+1) + '-' + d + '-' + y) || new Date() > date) {
			//console.log('bad:  ' + (m+1) + '-' + d + '-' + y + ' / ' + disabledDays[i]);
			return [false];
		}
	}
	//console.log('good:  ' + (m+1) + '-' + d + '-' + y);
	return [true];
}
function noWeekendsOrHolidays(date) {
	var noWeekend = jQuery.datepicker.noWeekends(date);
	return noWeekend[0] ? nationalDays(date) : noWeekend;
}
/* taken from mootools */
function ArrayIndexOf(array,item,from){
	var len = array.length;
	for (var i = (from < 0) ? Math.max(0, len + from) : from || 0; i < len; i++){
		if (array[i] === item) return i;
	}
	return -1;
}
/* taken from mootools */
function ArrayContains(array,item,from){
	return ArrayIndexOf(array,item,from) != -1;
}
/* create datepicker */
jQuery(document).ready(function() {
	jQuery('#date').datepicker({
		minDate: new Date(2010, 0, 1),
		maxDate: new Date(2010, 5, 31),
		dateFormat: 'DD, MM, d, yy',
		constrainInput: true,
		beforeShowDay: noWeekendsOrHolidays
	});
});
```
