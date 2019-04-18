# jquery 找到当前可见的元素并切换显示 - z69183787的专栏 - CSDN博客
2013年12月05日 20:11:42[OkidoGreen](https://me.csdn.net/z69183787)阅读数：4043
```
<script src="./jquery.js"></script>
<script>
$(function(){
$("li").hover(function(){clearInterval(t)}, function(){t = setInterval("a()", 3000);}); 
})
var t ;
function a(){
var listTmp = $("li").filter(function(){return $(this).css('display')!='none';});
			var infoTmp = $("#play_info li").filter(function(){return $(this).css('display')!='none';});
			
			if(listTmp.next().length==0){
				$("li").eq(0).fadeIn(1000); 
				
			}else{
				listTmp.fadeOut(500).next().fadeIn(1000); 
			}
			}
			t = setInterval("a()", 3000); 
</script>
<div style="height:50px;overflow:hidden;">
<ul>
<li style="height:50px;">111111111111111111111</li>
<li style="height:50px;">222222222222222222222222</li>
<li style="height:50px;">3333333333333333333</li>
</ul>
</div>
```
