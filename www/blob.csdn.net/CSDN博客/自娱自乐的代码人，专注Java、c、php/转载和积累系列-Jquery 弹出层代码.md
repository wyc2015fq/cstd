# 转载和积累系列 - Jquery 弹出层代码 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2012年08月15日 20:11:08[initphp](https://me.csdn.net/initphp)阅读数：1199








```java
/*
 *	弹出层 JS
 *	使用方法：
 *	var popups = new popus();
 *	popups.init({
 *		trigger:"popup_open",                       //触发的元素或id,必填参数
 *		popupBlk:"popup",                           //弹出内容层元素或id,必填参数
 *		closeBtn:"popup_close"                      //关闭弹出层的元素或id
 *		moveBtn :'popup_move',						//移动块	
 *	});
 */
var popup = function () {

	var _this = this;

	//配置文件
	this.options = {
		trigger	: null,                            	//触发的元素或id,必填参数
		popupBlk : null,                           	//弹出内容层元素或id,必填参数
		closeBtn : null,                         	//关闭弹出层的元素或id
		moveBtn : null,								//移动按钮
		isShade : true,								//是否开启蒙层
		speed : 'fast',								//动画效果 slow - 慢 normal - 正常 fast - 快
		enventType : 'click',						//绑定事件 click 点击 mouseover 鼠标移动上去
		position : 'mouseclick',					//居中 或者鼠标点击  mouseclick center
	};

	//初始化 设置参数 绑定事件
	this.init = function (options) {
		this.setOptions(options);
		this.bind();
	};

	//设置参数
	this.setOptions = function (options) {
		for (var key in options) {
			if (key in this.options) {
				this.options[key] = options[key];	
			}
		}
	};
	//绑定响应事件
	this.bind = function () {
		$('#' + this.options.trigger).bind(this.options.enventType, function (e) {
			_this.open(e);
		});
		$('#' + this.options.closeBtn).bind('click', function (){
			_this.close();
		});
	};

	//打开弹出层
	this.open = function (e) {
		var obj = $("#" + this.options.popupBlk);
		var cssStyle = {};

		if (this.options.position == 'center') { //弹出框显示位置
			cssStyle.left = ($(document).width() - obj.width()) / 2;
			cssStyle.top = (document.documentElement.clientHeight - obj.height())/2 + $(document).scrollTop();
		} else {
			if (($(document).width() - e.clientX) < obj.width()) {
				cssStyle.left = e.clientX - obj.width() + (($(document).width() - e.clientX) / 2);			} else {
				cssStyle.left = e.clientX;
			}
			cssStyle.top = e.clientY;
		}

		if (this.options.isShade == true) {		//是否开启蒙层
			cssStyle.position = 'absolute';
			cssStyle.zIndex = '100001';
			cssStyle.background = '#FFFFFF';
			this.shade(true);
		}
		obj.css(cssStyle);

		if (this.options.moveBtn != null) {		//是否绑定移动
			this.move(_this, obj);
		}
	
		obj.show(this.options.speed);
	};

	//关闭弹出层
	this.close = function () {
		var obj = $("#" + this.options.popupBlk);

		if (this.options.isShade == true) {		//是否开启蒙层
			this.shade(false);
		}
		
		obj.hide(this.options.speed);
	};

}

//蒙层效果
popup.prototype.shade = function (type) {
	if (type == true) {
		var divHtml = '<div id="popup_shade"></div>';
		$(divHtml).insertAfter('body');
		var cssStyle = {
			position : 'absolute',
			top : '0px',
			background : '#000',
			filter : 'Alpha(opacity=30)',
			opacity : '0.3',
			left : '0',
			width : '100%',
			height : '100%',
			zIndex : '10000'
		};
		$("#popup_shade").css(cssStyle);
	} else {
		$("#popup_shade").remove();
	}
}

//移动效果
popup.prototype.move = function (_this, obj) {
		var moveBtnObj = $('#' + this.options.moveBtn);
		moveBtnObj.css({cursor : 'move'}); 		//添加移动的样式

		_this.moveOptions = {		//数据结构
			moveStart : false
		};

		moveBtnObj.bind('mousedown', function (e) {
			_this.moveOptions.moveStart = true;
			_this.moveOptions.mouseDownX = e.clientX;
			_this.moveOptions.mouseDownY = e.clientY;
			_this.moveOptions.objLeft = parseInt(obj.css('left'));
			_this.moveOptions.objTop  = parseInt(obj.css('top'));
		});
		moveBtnObj.bind('mouseup', function (e) {
			_this.moveOptions.moveStart = false;
		});
		moveBtnObj.bind('mouseover', function (e) {
			_this.moveOptions.moveStart = false;
		});
		moveBtnObj.bind('mousemove', function (e) {
			if (_this.moveOptions.moveStart == true) {
				var cssStyle = {};
				cssStyle.left = _this.moveOptions.objLeft + (e.clientX - _this.moveOptions.mouseDownX);
				cssStyle.top  = _this.moveOptions.objTop + (e.clientY - _this.moveOptions.mouseDownY);
				obj.css(cssStyle);
			}
		});
}
```




```
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>无标题文档</title>
</head>

<body>

<br/><br/><br/><br/><br/><br/><br/>11111111111111111111111111111111111111111111<input name="" type="submit" id="popup_open" />
<div id="popup"  style=" border:1px #000000 solid; width:500px; height:500px; display:none;">11111111

<div id="popup_move" style="cursor:move; width:100px; height:100px; background-color:#000000;" >1111111111111111111</div>
<span id="popup_close">关闭</span>
</div>
<script src="jquery.js" type="text/javascript"></script>
<script src="popup.js" type="text/javascript"></script>
<script type="text/javascript">
$(document).ready(function() {
	var popups = new popup();
	popups.init({
		trigger:"popup_open",                           //触发的元素或id,必填参数
		popupBlk:"popup",                           	//弹出内容层元素或id,必填参数
		closeBtn:"popup_close",                         //关闭弹出层的元素或id
		moveBtn : 'popup_move',							//是否可以移动层
	});
});
</script>
</body>
</html>
```






