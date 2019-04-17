# Nginx+jwPlay搭建流媒体服务器，记忆播放 - DoubleLi - 博客园






1.具体的流媒体服务器的搭建参考博客：

http://blog.chinaunix.net/uid-20639775-id-154556.html

具体可能编译的时候有个地方报错



/root/nginx_mod_h264_streaming-2.2.7/src/ngx_http_streaming_module.c: In function ‘ngx_streaming_handler’:

/root/nginx_mod_h264_streaming-2.2.7/src/ngx_http_streaming_module.c:158: error: ‘ngx_http_request_t’ has no member named ‘zero_in_uri’

make[1]: *** [objs/addon/src/ngx_http_h264_streaming_module.o] Error 1

 make[1]: Leaving directory `/root/nginx-0.8.54'

make: *** [build] Error 2

，需要注释或者删除一个地方

将src/ngx_http_streaming_module.c文件中以下代码删除或者是注释掉就可以了：



/* TODO: Win32 */

if (r->zero_in_uri)

{

return NGX_DECLINED;

}

2.然后就是jw_player的使用，我们先了解下jw_Player的预留js的回调函数




 var player = jwplayer("mediaplayer").setup({

            flashplayer: "player.swf",
            file: 'http://localhost/222.mp4',
            autostart: "true",
            bufferlength: '20',
            width: '400',
            height: '400',
            image: '111.jpg',
            events: {
                onComplete: function () { console.log("播放结束!!!"); },
                onVolume: function () { console.log("声音大小改变!!!"); },
                onReady: function () { console.log("准备就绪!!!"); },
                onPlay: function () { console.log("开始播放!!!"); },
                onPause: function () { console.log("暂停!!!"); },
                onBufferChange: function () { console.log("缓冲改变!!!"); },
                onBufferFull: function () { console.log("视频缓冲完成!!!"); },
                onError: function (obj) { console.log("播放器出错!!!" + obj.message); },
                onFullscreen: function (obj) { if (obj.fullscreen) { console.log("全屏"); } else { console.log("非全屏"); } },
                onMute: function (obj) { console.log("静音/取消静音") }
            }
        });

其他js回调函数

player.onBufferChange(function () { alert("缓冲改变!!!"); });
        player.onBufferFull(function () { alert("视频缓冲完成!!!"); });
        player.onError(function (obj) { alert("播放器出错!!!" + obj.message); })
        player.onFullscreen(function (obj) {
            if (obj.fullscreen) { alert("全屏"); }
            else { alert("非全屏"); }
        });
        player.onMute(function (obj) { }) // obj. mute是否静音
        player.onPlaylist(function (obj) { //视频列表载入完成
            alert(obj.playlist[0]['file']);
        });
        var callback = function () { alert("callback");};//定义一个回调方法
        player.onPlaylistItem(callback) // 开始播放一个视频时触发obj.index为视频索引
        player.onReady(callback) //准备就绪
        player.onResize(callback) //obj.width obj.height  宽高
        //进入播放时触发obj.oldstate 值：从BUFFERING PAUSE状态进入播放状态
        player.onPlay(callback)
        //进入暂停obj.oldstate 值1：从PLAYING BUFFERING状态进入暂停
        player.onPause(callback)
        player.onBuffer(callback) //进入加载obj.oldstate 值PLAYING, PAUSED or IDLE
        player.onIdle(callback) //进入空闲obj.oldstate 值PLAYING, PAUSED or BUFFERING.
        player.onComplete(callback) //结束播放
        player.onVolume(callback) //obj.volume
        player.onPlay(function () {
            console.log(this.getPosition());
        });

参考这个大叔的博客:http://www.cnblogs.com/lori/archive/2014/05/05/3709459.html




3.具体将jw_Player嵌套到页面，然后将播放进度存入cookie实现记忆播放。



  
thePlayer = jwplayer( 'container').setup({

                flashplayer: '${pageContext.request.contextPath}/player/player.swf' ,

                 file: "http://192.168.0.11:8888/123456.flv",             

//                    这个是你nginx搭建的流媒体服务器视频播放的地址

                width: 720,

                height: 405,

                dock: false,

                controls: false, //显示控件按钮

                bufferlength:5,

                autostart: true,//是否开始自动播放

                stretching : 'fill', 

                streamer: "start", //这俩行必须要写，这样才能随时跳转，实现伪流的功能,要不不能实现在缓冲数据后面进行拖拽。如果不能随意拖拽进度条，说明你得nginx伪流搭建不成功

                provider: "http"


   });





 //判断状态播放的时候跳转到上次播放地址，然后停止,sectionId是我视频的编号

            var timer1=setInterval( function(){

                 var state = thePlayer.getState();

                      if(state== 'PLAYING'){         

                          thePlayer.seek(thePlayer.getDuration()*parseInt( 
 $.cookie($("#sectionId").text()+"sectionTime"))/100);

                           clearInterval(timer1);

                     }               

            },1000);

//             暂停状态就纪录暂停时间，停止状态就不做任何操作，播放状态就定时获取播放时间，然后存cookie后，关闭浏览器提交cookie中的数据。

            setInterval( function(){

//获取状态

                 var state = thePlayer.getState();

                 switch(state){

                 case 'BUFFERING': //加载

                     break;

                 case 'PLAYING': //正在播放

//thePlayer.getPosition() 获取当年播放时间，thePlayer.getDuration()获取视频的总时长             $.cookie($("#sectionId").text()+"sectionTime" ,parseInt((thePlayer.getPosition()/thePlayer.getDuration())*100));

                     break;

                 case 'PAUSED': //暂停

                      $.cookie($("#sectionId").text()+"sectionTime" ,parseInt((thePlayer.getPosition()/thePlayer.getDuration())*100));

                     break;

                 case 'IDLE': //停止

                     break;

                }

            },1000);




//上面是js轮询的方式获取播放时间，当然你也可以存到数据库，这样换个电脑还能记忆播放。你还可以使用js的

 $(window).bind('beforeunload',function(){

//获取时间存入cookie
            //这个关闭页面或者刷新的时候获取视频的播放进度存入cookie，，如果切换视频，但是无网页刷新的话，是不能记忆的，具体场景具体实现。
            });















