# jquery sparkline中在未加载的DOM元素中无法显示图标 - z69183787的专栏 - CSDN博客
2015年01月27日 13:15:55[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1217
#### How do I draw a sparkline on an element that is not yet in the DOM?
You need to know two things:
- The element you create must have some initial content, even if it's just  
- After the element is inserted into the DOM (and visible), you must call `$.sparkline_display_visible()` to actually display it
Eg.
// create an element and draw a sparklinevar barchart = $('<span> </span>');
barchart.sparkline([1,2,3,2],{type:'bar'});// insert the element somewhere into the DOM
$('#mycharts').append(barchart);// actually render any undrawn sparklines that are now visible in the DOM
$.sparkline_display_visible();
