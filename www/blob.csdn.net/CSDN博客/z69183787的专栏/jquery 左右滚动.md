# jquery 左右滚动 - z69183787的专栏 - CSDN博客
2012年10月22日 14:48:06[OkidoGreen](https://me.csdn.net/z69183787)阅读数：975
```java
$(function(){
    //@Mr.Think***变量
    var $cur = 1;//初始化显示的版面
    var $i = 4;//每版显示数
    var $len = $('.showbox>ul>li').length;//计算列表总长度(个数)
    var $pages = Math.ceil($len / $i);//计算展示版面数量
    var $w = $('.ibox').width();//取得展示区外围宽度
    var $showbox = $('.showbox');
    var $num = $('span.num li')
    var $pre = $('span.pre')
    var $next = $('span.next');
 	//@Mr.Think***向前滚动
    $pre.click(function(){
        if (!$showbox.is(':animated')) {  //判断展示区是否动画
            if ($cur == 1) {   //在第一个版面时,再向前滚动到最后一个版面
               
            }
            else {
            	 $cur--; //版面数累加
            				$showbox.animate({ left : -$w*($cur-1)}, 800);
            				
                	}     
            }
    });
    //@Mr.Think***向后滚动
    $next.click(function(){
    	
        if (!$showbox.is(':animated')) { //判断展示区是否动画
            if ($cur == $pages) {  //在最后一个版面时,再向后滚动到第一个版面
                
            }
            else {
            	 $cur++; //版面数累加
            		$showbox.animate({ left : -$w*($cur-1) }, 800);
               
            }
           
        }
    });
})
```
```java
$(function(){
    //@Mr.Think***变量
    var $cur = 1;//初始化显示的版面
    var $i = 4;//每版显示数
    var $len = $('.showbox>ul>li').length;//计算列表总长度(个数)
    var $pages = Math.ceil($len / $i);//计算展示版面数量
    var $w = $('.ibox').width();//取得展示区外围宽度
    var $showbox = $('.showbox');
    var $num = $('span.num li')
    var $pre = $('span.pre')
    var $next = $('span.next');
 	//@Mr.Think***向前滚动
    $pre.click(function(){
        if (!$showbox.is(':animated')) {  //判断展示区是否动画
            if ($cur == 1) {   //在第一个版面时,再向前滚动到最后一个版面
               
            }
            else {
            					$("li").hide();
            				 for(var i=($cur-2)*4;i<($cur-1)*4;i++){
            				 	
                		$("li:eq("+i+")").show();
                	}
            				
               
                $cur--; //版面数累加
            }
           
        }
    });
    //@Mr.Think***向后滚动
    $next.click(function(){
    	
        if (!$showbox.is(':animated')) { //判断展示区是否动画
            if ($cur == $pages) {  //在最后一个版面时,再向后滚动到第一个版面
                
            }
            else {
            		if($cur==$pages-1){
            				$("li").hide();
            				 for(var i=($cur)*4;i<$len;i++){
            				 
                		$("li:eq("+i+")").show();
                	}
            			}else{
            					$("li").hide();
            				 for(var i=($cur)*4;i<($cur+1)*4;i++){
            				 	
                		$("li:eq("+i+")").show();
                	}
            				
            				}
               
                $cur++; //版面数累加
            }
           
        }
    });
})
```
```
<style>
/*demo css*/
.ibox{width:720px; height:210px; background:#ddd; margin:1em auto; border:1em solid #ddd; position:relative; overflow:hidden}
.ibox .showbox{position:absolute;left:0px;width:2550px; height:170px; background:#fff}
.ibox .showbox ul li{float:left;margin:7px}
.ibox .showbox ul li img{width:150px; height:150px; border:3px solid #a40000}
.ibox span{position:absolute;padding:2px 5px; background:#fff; cursor:pointer;}
.ibox span.pre{left:10px; bottom:0}
.ibox span.next{right:10px; bottom:0}
.ibox span.num{right:45%; bottom:0; background:#ddd;line-height:18px;}
.ibox span.num li{display:inline;margin:0 5px; padding:1px 5px; line-height:18px}
.ibox span.num .numcur{background:#a40000; color:#fff}
</style>
```
```java
<!--DEMO start-->
<div id="demo" class="ibox">
<div class="showbox">
<ul>
<li><img src="http://img.ffffound.com/static-data/assets/6/f8a9dd53720bf31b5917915e8222c27370a3681c_m.jpg" alt="demo" title="demo" /></li>
<li><img src="http://img.ffffound.com/static-data/assets/6/15a1d7982c49b0e638ae3340cf86dc4469f955cc_m.jpg" alt="demo" title="demo" /></li>
<li><img src="http://img.ffffound.com/static-data/assets/6/68e9a7a8476e26a1022fa5e39ccdda669a5d4f08_m.jpg" alt="demo" title="demo" /></li>
<li><img src="http://img.ffffound.com/static-data/assets/6/56d1b9aaff5b424f1cdb08b80544d5150839b4de_m.jpg" alt="demo" title="demo" /></li>
<li><img src="http://img.ffffound.com/static-data/assets/6/56d1b9aaff5b424f1cdb08b80544d5150839b4de_m.jpg" alt="demo" title="demo" /></li>
<li><img src="http://img.ffffound.com/static-data/assets/6/20417dfc945bae6c720a660625e9652a154c2c94_m.jpg" alt="demo" title="demo" /></li>
<li><img src="http://img.ffffound.com/static-data/assets/6/f4a9f29299f3dba6e7af0ac3c5aff93e01f874d3_m.jpg" alt="demo" title="demo" /></li>
<li><img src="http://img.ffffound.com/static-data/assets/6/ca6f0c9ac0c6e3bfce827cd92833e1db620b6377_m.jpg" alt="demo" title="demo" /></li>
<li><img src="http://img.ffffound.com/static-data/assets/6/809fcfbb8a1f58d1d47a760fd4283556cce201ef_m.jpg" alt="demo" title="demo" /></li>
</ul>
</div>
    <span class="pre">«向左滚动</span><span class="next">向右滚动»</span></div>
<!--DEMO end-->
<!--@Mr.Think的统计及广告代码,使用样例请勿复制:)-->
<div class="clear"></div>
<div id="adsense"></div>
```
