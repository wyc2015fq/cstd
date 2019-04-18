# 关于播放器JPlayer的使用及遇到的问题 - z69183787的专栏 - CSDN博客
2012年11月13日 15:12:04[OkidoGreen](https://me.csdn.net/z69183787)阅读数：16099
jPlayer是一个用于控制和播放mp3文件的jQuery插件。它在后台使用Flash来播放mp3文件，前台播放器外观完全可以使用XHML/CSS自定义。支持：
有一点比较好的是，在支持html5的浏览器上会使用html5的标签audio或者video，而不支持的浏览器上使用swf来播放
选择需要播放的Mp3文件。
播放、暂停、停止控制。
音量控制。 
取得播放进度等信息。
官方网站：[http://www.jplayer.org/](http://www.jplayer.org/)
支持格式：
- HTML5: mp3,
 mp4 (AAC/H.264), ogg (Vorbis/Theora), webm (Vorbis/VP8), wav
- Flash: mp3,
 mp4 (AAC/H.264), flv
范例：
```
<!DOCTYPE html>
<html>
<head>
<meta charset=utf-8 />
<!-- Website Design By: www.happyworm.com -->
<title>Demo : jPlayer as a video player</title>
<link href="skin/blue.monday/jplayer.blue.monday.css" rel="stylesheet" type="text/css" />
<script type="text/javascript" src="js/jquery-1.7.1.js"></script>
<script type="text/javascript" src="js/jquery.jplayer.min.js"></script>
<script type="text/javascript">
//<![CDATA[
$(document).ready(function(){
	$("#jquery_jplayer_1").jPlayer({
		ready: function () {
			$(this).jPlayer("setMedia", {
				flv: "/workflowLocal/j/1.flv"
			});
		},
		swfPath: "js",
		supplied: "flv",
		size: {
			width: "640px",
			height: "360px",
			cssClass: "jp-video-360p"
		}
	});
});
function a(){
		$("#jquery_jplayer_1").jPlayer("setMedia", {
				flv: "/workflowLocal/j/2.flv"
			});
	}
//]]>
</script>
</head>
<body>
		<input type="button" onclick="a();">
		<div id="jp_container_1" class="jp-video jp-video-360p">
			<div class="jp-type-single">
				<div id="jquery_jplayer_1" class="jp-jplayer"></div>
				<div class="jp-gui">
					<div class="jp-video-play">
						<a href="javascript:;" class="jp-video-play-icon" tabindex="1">play</a>
					</div>
					<div class="jp-interface">
						<div class="jp-progress">
							<div class="jp-seek-bar">
								<div class="jp-play-bar"></div>
							</div>
						</div>
						<div class="jp-current-time"></div>
						<div class="jp-duration"></div>
						<div class="jp-controls-holder">
							<ul class="jp-controls">
								<li><a href="javascript:;" class="jp-play" tabindex="1">play</a></li>
								<li><a href="javascript:;" class="jp-pause" tabindex="1">pause</a></li>
								<li><a href="javascript:;" class="jp-stop" tabindex="1">stop</a></li>
								<li><a href="javascript:;" class="jp-mute" tabindex="1" title="mute">mute</a></li>
								<li><a href="javascript:;" class="jp-unmute" tabindex="1" title="unmute">unmute</a></li>
								<li><a href="javascript:;" class="jp-volume-max" tabindex="1" title="max volume">max volume</a></li>
							</ul>
							<div class="jp-volume-bar">
								<div class="jp-volume-bar-value"></div>
							</div>
							<ul class="jp-toggles">
								<li><a href="javascript:;" class="jp-full-screen" tabindex="1" title="full screen">full screen</a></li>
								<li><a href="javascript:;" class="jp-restore-screen" tabindex="1" title="restore screen">restore screen</a></li>
								<li><a href="javascript:;" class="jp-repeat" tabindex="1" title="repeat">repeat</a></li>
								<li><a href="javascript:;" class="jp-repeat-off" tabindex="1" title="repeat off">repeat off</a></li>
							</ul>
						</div>
						<div class="jp-title">
							<ul>
								<li>shentong metro</li>
							</ul>
						</div>
					</div>
				</div>
				<div class="jp-no-solution">
					<span>Update Required</span>
					To play the media you will need to either update your browser to a recent version or update your <a href="http://get.adobe.com/flashplayer/" target="_blank">Flash plugin</a>.
				</div>
			</div>
		</div>
</body>
</html>
```
大部分都是官网上的demo，需要引入jquery.jplayer.min.js 以及官网上的2个css包,还有Jplayer.swf
目录结构如下,js文件夹中为官网提供的js文件，skin为官网提供的css皮肤包
![](https://img-my.csdn.net/uploads/201211/13/1352790606_4565.jpg)
这里遇到二个奇怪的问题，在本地时测试demo，只能播放官网说的m4v格式，而用flv格式就不行，但放在服务器上就解决了。
另外就是
```
ready: function () {
			$(this).jPlayer("setMedia", {
				flv: "/workflowLocal/j/1.flv"
			});
		},
```
这里的setMedia设置文件路径时候，本地只能引用url路径 比如:http://xxxxxxxxxxxxxx，而不能使用绝对或者相对路径
而在服务器上时，则可以使用绝对路径，但一定要是以项目根目录开始的绝对路径才可以。
这二个问题不知道怎么解决。。。
因为项目中要用到切换视频的功能，所以尝试了一下 代码如下 ，点击按钮切换
```
function a(){
		$("#jquery_jplayer_1").jPlayer("setMedia", {
				flv: "/workflowLocal/j/2.flv"
			});
	}
```
服务器上播放效果如下：
![](https://img-my.csdn.net/uploads/201211/13/1352790975_6147.jpg)
