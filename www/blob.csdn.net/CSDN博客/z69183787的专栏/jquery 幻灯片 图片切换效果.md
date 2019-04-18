# jquery 幻灯片 图片切换效果 - z69183787的专栏 - CSDN博客
2013年12月06日 11:45:23[OkidoGreen](https://me.csdn.net/z69183787)阅读数：982
js：
```java
(function($){
	$.fn.extend({
		Tabs:function(options){
			// 处理参数
			options = $.extend({
				event : 'mouseover',	//事件类型  
				timeout : 0,			//设置事件延迟
				auto : 0,				//多少秒自动切换一次  
				callback : null			//回调函数
			}, options);
			
			var self = $(this),
				tabBox = self.children( 'div.tab_box' ).children( 'div' ),
				menu = self.find( 'ul.tab_menu' ),
				items = menu.find( 'li' ),
				timer;
				
			var tabHandle = function( elem ){
					elem.siblings( 'li' )
						.removeClass( 'current' )
						.end()
						.addClass( 'current' );
						
					tabBox.siblings( 'div' )
						.addClass( 'hide' )
						.end()
						.eq( elem.index() )
						.removeClass( 'hide' );
				},
					
				delay = function( elem, time ){
					time ? setTimeout(function(){ tabHandle( elem ); }, time) : tabHandle( elem );
				},
				
				start = function(){
					if( !options.auto ) return;
					timer = setInterval( autoRun, options.auto );
				},
				
				autoRun = function(){
					var current = menu.find( 'li.current' ),
						firstItem = items.eq(0),
						len = items.length,
						index = current.index() + 1,
						item = index === len ? firstItem : current.next( 'li' ),
						i = index === len ? 0 : index;
					
					current.removeClass( 'current' );
					item.addClass( 'current' );
					
					tabBox.siblings( 'div' )
						.addClass( 'hide' )
						.end()
						.eq(i)
						.removeClass( 'hide' );
				};
							
			items.bind( options.event, function(){
				delay( $(this), options.timeout );
				if( options.callback ){
					options.callback( self );
				}
			});
			
			if( options.auto ){
				start();
				self.hover(function(){
					clearInterval( timer );
					timer = undefined;
				},function(){
					start();
				});
			}
			
			return this;
		}
	});
})(jQuery);
$(function() {
	$("#imgsider").Tabs({auto : 3000, event : 'click'}); //顶部 图片预览
	}）
```
css：
```
<style>
.backgroundImgWrap{width:100%;height:100%;position:absolute;top:0;left:0;overflow:hidden;}
.bgafill{width:100%;height:100%;}
a{text-decoration:none;}
a:hover{text-decoration:underline;}
.newbox{width:525px;height:286px;padding:11px;background:url(images/news_pic_bg.gif) no-repeat;overflow:hidden;position:relative;}
.new_content{width:100%;}
.hide{display:none;}
.new_list{height:8px;position:absolute;right:22px;bottom:21px;}
.new_list li{list-style-type:none;width:9px;height:8px;margin-left:6px;background:url(images/gray_icon.gif) no-repeat;cursor:pointer;float:left;}
.new_list li.current{list-style-type:none;background:url(images/red_icon2.gif) no-repeat;}
.newData img{width:523px;height:284px;}
.ptitle{width:509px;height:64px;padding-left:14px;overflow:hidden;background:url(images/bs_bg.png);position:absolute;bottom:13px;left:11px;}
.ptitle a{font:18px/48px Microsoft yahei;color:#000;text-decoration:none;}
	</style>
```
页面：
```
<div id="imgsider" class="newbox fL">
      <div class="new_content tab_box">
        <div class="newData">
          <p><img src="images/pic.jpg" /></p>
          <span class="ptitle"><a href="#">上海地铁与艺术人文频道签署战略协议</a></span> 
        </div>
        <div class="newData hide">
          <p><img src="images/pic01.jpg" /></p>
          <span class="ptitle"><a href="#">上海地铁与艺术人文频道签署战略协议3</a></span>
        </div>
        <div class="newData hide">
          <p><img src="images/pic02.jpg" /></p>
          <span class="ptitle"><a href="#">上海地铁与艺术人文频道签署战略协议1</a></span> 
        </div>
        <div class="newData hide">
          <p><img src="images/pic03.jpg" /></p>
          <span class="ptitle"><a href="#">上海地铁与艺术人文频道签署战略协议2</a></span>
        </div>
        <div class="newData hide">
          <p><img src="images/pic04.jpg" /></p>
          <span class="ptitle"><a href="#">上海地铁与艺术人文频道签署战略协议3</a></span>
        </div>
        <div class="newData hide">
          <p><img src="images/pic05.jpg" /></p>
          <span class="ptitle"><a href="#">上海地铁与艺术人文频道签署战略协议2</a></span>
        </div>
     </div>
     <ul class="new_list tab_menu">
      	<li class="current"> </li>
      	<li> </li>
      	<li> </li>
      	<li> </li>
      	<li> </li>
      	<li> </li>
     </ul>
   </div>
```
