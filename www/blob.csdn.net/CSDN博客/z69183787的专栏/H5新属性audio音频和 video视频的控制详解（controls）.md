# H5新属性audio音频和 video视频的控制详解（controls） - z69183787的专栏 - CSDN博客
2016年10月11日 18:23:19[OkidoGreen](https://me.csdn.net/z69183787)阅读数：8351
 1.音频（audio）
<audio controls="controls">
<source src="这里面放入音频文件路径"></source>
</audio>
2.视频（video）
 <video controls="controls" loop="loop" autoplay="autoplay" id="video">
<source src="这里面放入视频文件路径"></source>
</video>
<button>静音</button>
 <button>打开声音</button>
 <button>播放</button>
 <button>停止播放</button>
 <button>全屏</button> 
下面是对于视频文件的控制；
javascript引出。
<script>
var myVideo=document.getElementById("video");
var btn=document.getElementById("button");
btn[0].click=function(){
    myVideo.muted=true;(是否静音：是)
}
btn[1].click=function(){
    myVideo.muted=true;(是否静音：否)
}
btn[2].click=function(){
    myVideo.play();(播放)
}
btn[3].click=function(){
    myVideo.pause();(停止播放)
} 
btn[4].click=function(){
    myVideo.webkitrequestFullscreen();(全屏显示)
}  
</script> 
3.如何设置进度条和视频的播放时长
同步进行。
如图所示:
这里说一下，首先
1、需要获取视频的总时长(duration)赋值给进度条的最大值，progress.max=video.duration;
2、需要获取当前视频播放的当前时间位置(currentTime)赋值给当前进度条的长度，progress.value=video.currentTime;
然后在视频播放的同时，要保证进度条的值能够及时获取到视频的时长和当前播放时间位置。
需要开一个定时器setInterval(pro，100);：就是说在1毫秒获取一次视频的数值赋值给progress进度条，这样就能保证及时性。
这样进度条就能和视频的准确的同步了。
4.如何用表单元素 range属性控制视频的音量大小。
1、首先需要获取到range的value值，赋给视频的音量上去，才能控制视频的音量大小，
<input type="range" min="0" value="50" max="100" id="range" />
var ran=document.getElementById("range");
获取range.value,
赋值给video的音频属性:video.volume=range.value/100;
这时候就能实现简单拖动range而控制视频的音量了。
然后还需要进去前面的声音关闭判断，两则是独立的事件，所以，需要在拖动事件中进行判断是否是静音muted，然后在进行muted设置为false。
具体代码请详看我的博客:
![](http://images2015.cnblogs.com/blog/970806/201607/970806-20160719194547451-1233996524.png)
最终实现的代码如下；
<!DOCTYPE html> 
<html> 
<body> 
<video id="video1" controls="controls" width="400px" height="400px">
<source src="img/1.mp4">
</video>
<div>
<button onclick="enableMute()" type="button">关闭声音</button>
<button onclick="disableMute()" type="button">打开声音</button>
<button onclick="playVid()" type="button">播放视频</button>
<button onclick="pauseVid()" type="button">暂停视频</button>
<button onclick="showFull()" type="button">全屏</button><br />
<span>进度条:</span>
<progress value="0" max="0" id="pro"></progress>
<span>音量:</span>
<input type="range" min="0" max="100" value="50" onchange="setvalue()" id="ran"/>
</div>
<script>
var btn=document.getElementsByTagName("button");
var myvideo=document.getElementById("video1");
var pro=document.getElementById("pro");
var ran=document.getElementById("ran");
//关闭声音
function enableMute(){
myvideo.muted=true;
btn[0].disabled=true;
btn[1].disabled=false;
}
//打开声音
function disableMute(){
myvideo.muted=false;
btn[0].disabled=false;
btn[1].disabled=true;
}
//播放视频
function playVid(){
myvideo.play();
setInterval(pro1,1000);
}
//暂停视频
function pauseVid(){
myvideo.pause();
}
//全屏
function showFull(){
myvideo.webkitrequestFullscreen();
}
//进度条展示
function pro1(){
pro.max=myvideo.duration;
pro.value=myvideo.currentTime;
}
//拖动range进行调音量大小
function setvalue(){
myvideo.volume=ran.value/100;
myvideo.muted=false;
}
</script>
</body> 
</html>
