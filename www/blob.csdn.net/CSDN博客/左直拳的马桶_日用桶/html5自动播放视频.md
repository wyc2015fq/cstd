# html5自动播放视频 - 左直拳的马桶_日用桶 - CSDN博客
2019年03月22日 19:11:47[左直拳](https://me.csdn.net/leftfist)阅读数：54
html5可以直接播放视频。就用标记`<video>`即可，真好。
可是怎么自动播放视频？
不是有"autoplay"吗？但是，无效。任凭你设属性，或是`video.play()`都无济于事。非要你手动点击才能播放。
后来才知道，需要设置静音属性autoplay才起作用！真是太细心了！设想我们无意中打开一个爱情动作片，叫声很大，而且好死不死，是在一个重要的高级会议场合，怎么破？
代码如下：
```
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <title>视频播放</title>
    <style>
        html,body{
            margin: 0;
            width:100%;
            height: 100%;
        }
        #video1{
            width:350px;
            height:280px;
        }
        .hidden{
            display:none;
        }
    </style>
    <script type="text/javascript" src="/static/js/jquery-1.8.3.min.js"></script>
</head>
<body>
    <div>
        <video id="video1" muted="muted" autoplay="autoplay" src="" controls="controls">
        </video>
    </div>
</body>
<script type="text/javascript">
    $(function(){
		var video = document.getElementById("video1");
        video.src = "http://192.168.0.98:20000/v1.mp4";
        video.play();
    });
</script>
</html>
```
```
muted="muted" autoplay="autoplay"
```
muted，静音之意。多么生僻的单词，为啥不是quiet？或者干脆叫“shut up”
