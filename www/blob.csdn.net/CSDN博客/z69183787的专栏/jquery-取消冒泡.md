# jquery-取消冒泡 - z69183787的专栏 - CSDN博客
2013年12月17日 15:37:04[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1200
1、通过返回false来取消默认的行为并阻止事件起泡。
jQuery 代码:
$("form").bind(
　　"submit", 
function() { 
return false;
 }
);
**2、通过使用 preventDefault() 方法只取消默认的行为。**
jQuery 代码:
$("form").bind(
"submit", 
function(event){
event.preventDefault();
}
);
**3、通过使用 stopPropagation() 方法只阻止一个事件起泡。**
jQuery 代码:
$("form").bind(
"submit", 
function(event){
event.stopPropagation();
}
);
### （4）总结
**1.一个事件起泡对应触发的是上层的同一事件**
　　特殊：如果two设置成双击事件，那么在你单击two的时候就会起泡触发one单击的事件
　　（双击包含单击）。
**2.如果在click事件中，在你要处理的事件之前加上e.preventDefault();　那么就取消了行为（通俗理解：相当于做了个return操作），不执行之后的语句了。3.e.stopPropagation()只要在click事件中，就不会触发上层click事件。**
//如果提供了事件对象，则这是一个非IE浏览器
**if** ( e && e.stopPropagation )
　　// 因此它支持W3C的stopPropagation()方法
　　e.stopPropagation();
**else**
　　//否则，我们需要使用IE的方式来取消事件冒泡
　　window.event.cancelBubble = **true**;
**return false**;
