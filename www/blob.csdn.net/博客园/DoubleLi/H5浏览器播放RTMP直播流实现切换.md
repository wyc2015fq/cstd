# H5浏览器播放RTMP直播流实现切换 - DoubleLi - 博客园






如果有两个视频流同时切换的话，推荐直接移动这个直播源，并改变大小实现切换 
下面提供的是通过videoJs提供的方法src替换直播源路径（高低清流切换，更换播放视频流）

```xml
<!DOCTYPE html>
<html>
<head>
<meta charset="UTF-8">
<title>视频流切换</title>
<style>
.fl {
    float: left;
}
.bgc_pink {
    background-color: pink;
}
.bgc_green {
    background-color: green;
}
.my-player0-dimensions {
    width: 0;
    height: 0;
}
.pos_rel {
    position: relative;
}
</style>
<link href="http://vjs.zencdn.net/5.19/video-js.min.css"
    rel="stylesheet">
<script src="http://vjs.zencdn.net/5.19/video.min.js"></script>
</head>
<body>
    <div id="firstShow" class="fl bgc_green pos_rel"
        style="width: 600px; height: 400px; margin-right: 100px;">
        <!-- 111 -->
    </div>
    <div class="asideVideos fl"
        style="width: 300px; height: 1000px; ">
        <div id="secondShow" style="width: 300px; height: 300px;"
            class="asidePlay bgc_pink pos_rel">
            <!-- 222 -->
        </div>
    </div>
    <button id="buu">切换</button>
    <script src="/js/jquery.min.js"></script>
    <script type="text/javascript">
        window.onload=function(){
            var player1="rtmp://live.hkstv.hk.lxdns.com/live/hks";//建议换另一个直播源 以便区分
            var player2="rtmp://live.hkstv.hk.lxdns.com/live/hks";
            var LiveplayerArr=[player1,player2];
            $("#firstShow").html('<video id="my-player1" class="video-js" preload="auto" data-setup="{}"><source src="'+LiveplayerArr[0]+'" type="rtmp/flv"/></video>')
            .attr("data-playId",1);
            $("#secondShow").html('<video id="my-player2" class="video-js" preload="auto" data-setup="{}"><source src="'+LiveplayerArr[1]+'" type="rtmp/flv"/></video>')
            .attr("data-playId",2);

            $(".video-js").css({
            "width":"100%",
            "height":"100%"
            });
             <!-- start-->
               function startPlay(id){
               var myPlayer = videojs(id);
               videojs(id,{},function onPlayerReady() {
                 videojs.log('Your player is ready!');
                 this.play();
                 this.on('ended', function() {
                   videojs.log('Awww...over so soon?!');
                 });
                    });
               }
               startPlay("my-player1");
               startPlay("my-player2");
               function rePlay(id,src){
                   var myPlayer = videojs(id);
                   videojs(id,{},function onPlayerReady() {
                        videojs.log('Your player is ready!');
                        if(src!=""&&src!=null){
                             $(id+" source").attr("src", src);
                             myPlayer.src(src);
                             myPlayer.load(src);
                             this.play();
                        }
                        this.on('ended', function() {
                             videojs.log('Awww...over so soon?!');
                        });
                    });
               }
               <!-- end--> 
               $("#buu").click(function(){
                    alert("000")
                    var index=$(this).index()+2;
                    var centerPlayId=$("#firstShow").attr("data-playId");
                    var asidePlayId=$(this).attr("data-playId");
                    var centerId=centerPlayId,asideId=asidePlayId;
                    rePlay("my-player1",LiveplayerArr[asidePlayId-1]);
                    rePlay("my-player"+index,LiveplayerArr[centerPlayId-1]);
                    $("#firstShow").attr("data-playId",asidePlayId);
                    $(this).attr("data-playId",centerPlayId);
                });
            }
        </script>
</body>
</html>
```









