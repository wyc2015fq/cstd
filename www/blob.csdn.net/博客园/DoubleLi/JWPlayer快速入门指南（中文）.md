# JWPlayer快速入门指南（中文） - DoubleLi - 博客园






将JW Player嵌入到网页中非常的简单，只需要进行如下3个步骤：

1、解压mediaplayer-viral.zip文件，将jwplayer.js和player.swf文件拷贝到工程中；

2、在页面引入jwplayer.js文件：

   <script type="text/javascript" src="/jwplayer/jwplayer.js"></script>

3、将下面代码粘贴在body标签内，如下所示：

   <div id="container">Loading the player ...</div>

    <script type="text/javascript">

       jwplayer("container").setup({

                          flashplayer: "/jwplayer/player.swf",

                          file: "/uploads/video.mp4",

                          height: 270,

                          width: 480

      });

   </script>

   上面代码的含义不言自明，上面script中的flashplayer只不过是JWPlayer众多配置中的一个，上面例子使用了div标签，下面给出使用video标签：

    <video

        src="/videos/video.mp4"

        height="270"

        id="container"

        poster="/thumbs/image.jpg"

        width="480">

    </video>

    <script type="text/javascript">

        jwplayer("container").setup({

            flashplayer: "/jwplayer/player.swf"

        });

    </script>

在这种情况下，JW Player使用video标签及其属性加载配置选项。

快速嵌入

将jwplayer.js和player.swf拷贝到jwplayer文件夹，你可以通过下面两行简单的代码将JW Player嵌入到HTML网页中，代码如下：

<script type="text/javascript" src="/jwplayer/jwplayer.js"></script>

<video class="jwplayer" src="/uploads/video.mp4"poster="/uploads/image.jpg"></video>

页面中class属性值为jwplayer的所有video标签将更换为JW Player。

JWPlayer语法

语法基本结构：

jwplayer(container).setup({options});

“container”是加载JW Player的DOM元素，例如video、div、p等等，如果是video标签，标签的属性（比如width和src)）将被加载到JW Player中；“options”是JW Player配置选项列表，配置选项的指南包含完整的概述，比如下面的例子：

<div id="container">Loadingthe player ...</div>

<script type="text/javascript">

    jwplayer("container").setup({

        autostart: true,

        controlbar: "none",

        file: "/videos/video.mp4",

        duration: 57,

        flashplayer: "/jwplayer/player.swf",

        volume: 80,

        width: 720

    });

</script>

“options”的选项并不仅限于此，它还有如下选项：

skin：设置JW Player皮肤；

playlist: 设置JW Player播放列表；

levels：通过设置比特率来设定不同视频的播放质量等级；

plugins：设置JW Player插件，包括它们的配置选项；

events： 给JW Player添加事件，营造执行js脚本的环境；

modes：指定为了渲染播放器JW Player 所使用的不同浏览器技术的顺序；

接下来详细介绍这些选项：

skin

JW Player有各种各样的可用来修改播放器外观和感觉的皮肤。为了嵌入JWPlayer 5的皮肤，只需将压缩文件拷贝到Web服务器上并使用skin选项指定皮肤压缩文件路径即可，代码如下：

<div id="container">Loadingthe player ...</div>

<script type="text/javascript">

    jwplayer("container").setup({

        flashplayer: "/jwplayer/player.swf",

        file: "/uploads/video.mp4",

        height: 270,

        width: 480,

        skin: "/skins/modieus/modieus.zip"

    });

</script>

注意：如果您正在配置主要在HTML5 mode中运行的JW Player，你的皮肤的文件夹结构看起来应该像这样：

/skins/modieus/modieus.zip

/skins/modieus/modieus.xml

/skins/modieus/controlbar/

/skins/modieus/playlist/

etc.



更多皮肤下载：[http://developer.longtailvideo.com/trac/browser/skins?rev=1035&order=name](http://developer.longtailvideo.com/trac/browser/skins?rev=1035&order=name)

playlist

该选项用于设置JW Player播放列表；例子如下：

<div id="container">Loadingthe player...</div>

<script type="text/javascript">

    jwplayer("container").setup({

        flashplayer: "/jwplayer/player.swf",

        playlist: [

                 { duration: 32, file: "/uploads/video.mp4", image:"/uploads/video.jpg"},

                 { duration: 124, file: "/uploads/bbb.mp4", image:"/uploads/bbb.jpg" },

                 { duration: 542, file: "/uploads/ed.mp4", image:"/uploads/ed.jpg" }

        ],

        "playlist.position": "right",

        "playlist.size": 360,

        height: 270,

        width: 720

    });

</script>

一个播放列表可以包含一个或多个视频文件，每一个文件都支持如下属性：

file：如果没有设置levels选项，则该项为必选项）。该项用来指定媒体的位置。如果未设置此属性，playlist项将被忽略；

image：用来设置视频海报图片，是播放列表的一部分，该图片显示在视频播放前和视频播放完成后；

duration：视频的持续时间，单位为秒。该播放器用该选项将视频持续时间显示在控制栏和图片列表中；

start：视频播放时间点。当用户播放该视频文件时，视频将不会从头播放，而是从该选项设定的播放时间点开始播放；

title：视频标题，显示在图形播放列表中；

description：视频描述信息，显示在图形播放列表中；

streamer：设定视频流应用。仅用于RTMP或HTTP流；

provider：设置用于回放播放列表视频的特定媒体回放API（如HTTP，RTMP或YouTube）；

levels：设定视频播放的质量等级。

Levels

levels配置项允许将一个视频的多个质量等级加载到播放器中。Flash播放器使用这些设定的质量等级实现RTMP或HTTP码率切换。比特率转换是一种播放器为每一位视频观看者自动显示最佳视频质量的机制。

下面展示使用RTMP码率切换（又称动态流）的一个例子。注意这里使用了“streamer”配置项，该配置项用来告诉播放器RTMP服务器的位置：

<div id="container">Loadingthe player...</div>

<script type="text/javascript">

    jwplayer("container").setup({

        flashplayer: "/jwplayer/player.swf",

        height: 270,

        width: 480,

        image: "/thumbs/video.jpg",

        levels: [

               { bitrate: 300, file: "videos/video_300k.mp4", width: 320 },

               { bitrate: 600, file: "videos/video_600k.mp4", width: 480 },

               { bitrate: 900, file: "videos/video_900k.mp4", width: 720 }

               ],

        provider: "rtmp",

        streamer: "rtmp://rtmp.example.com/application/"

    });

</script>

下面这个例子使用了HTTP码率切换。通过为http设定“provider”配置项来启用HTTP码率切换：

<div id="container">Loadingthe player...</div>

<script type="text/javascript">

    jwplayer("container").setup({

        flashplayer: "/jwplayer/player.swf",

        height: 270,

        width: 480,

        image: "/thumbs/video.jpg",

        levels: [

               { bitrate: 300, file: "http://example.com/videos/video_300k.mp4",width: 320 },

               { bitrate: 600, file: "http://example.com/videos/video_600k.mp4",width: 480 },

               { bitrate: 900, file: "http://example.com/videos/video_900k.mp4",width: 720 }

        ],

        provider: "http","http.startparam":"starttime"

    });

</script>

在HTML5模式中使用levels选项

在HTML5模式中为了兼容各种浏览器，也可以使用levels选项指定交替的视频格式。如果浏览器不支持levels选项中第一个视频文件，那么播放器将使用第二个，以此类推。如下代码：

<div id="container">Loadingthe player...</div>

<script type="text/javascript">

    jwplayer("container").setup({

        flashplayer: "/jwplayer/player.swf",

        height: 270,

        width: 480,

        image: "/thumbs/video.jpg",

        levels: [

               { file: "/videos/video.mp4" }, // H.264 version

               { file: "/videos/video.webm" }, // WebM version

               { file: "/videos/video.ogv" } // Ogg Theroa version

        ]

    });

</script>

Plugins

插件被用于JWPlayer顶部的堆栈。很多插件在我们的库中是可用的，例如转发，分析或广告。下面给出一个使用了HD 插件和GoogleAnalytics Pro插件的例子：

<div id="container">Loadingthe player...</div>

<script type="text/javascript">

    jwplayer("container").setup({

        flashplayer: "/jwplayer/player.swf",

        file: "/videos/video.mp4",

        height: 270,

        plugins: {

                 hd: { file: "/videos/video_high.mp4", fullscreen: true },

                 gapro: { accountid: "UKsi93X940-24" }

        },

        image: "/thumbs/video.jpg",

        width: 480

    });

</script>

下面再给出一个使用了sharing插件的例子，在该例子中插件的参数被包括在了playlist块内：

<div id="container">Loadingthe player...</div>

<script type="text/javascript">

      jwplayer("container").setup({

            flashplayer: "/jwplayer/player.swf",

            playlist: [

                     { file: "/videos/bunny.mp4", "sharing.link":"http://bigbuckbunny.org"},

                     { file: "/videos/ed.mp4", "sharing.link":"http://orange.blender.org" }

            ],

            plugins: {

                     sharing: { link: true }

            },

            height: 270,

            width: 720

      });

</script>

Events

events块允许你向播放器事件上添加JavaScript脚本。这是一个增强网页交互性的强大方法。下面给出一个简答的例子：

<div id="container">Loadingthe player ...</div>

<script type="text/javascript">

    jwplayer("container").setup({

           flashplayer: "/jwplayer/player.swf",

           file: "/videos/video.mp4",

           height: 270,

           width: 480,

           events: {

                  onComplete: function() { alert("the video is finished!"); }

           }

    });

</script>

下面再给出一个含有两个事件处理的例子。注意：onReady()方法使用this声明自动启动播放器，onVolume()方法处理正在处理的事件：

<div id="container">Loadingthe player ...</div>

<script type="text/javascript">

    jwplayer("container").setup({

          flashplayer: "/jwplayer/player.swf",

          file: "/videos/video.mp4",

          height: 270,

          width: 480,

          events: {

                  onReady: function() { this.play(); },

                  onVolume: function(event) { alert("the new volume is"+event.volume); }

          }

    });

</script>

要查看所有事件及其属性，请参考API，其网址为：[http://www.longtailvideo.com/support/jw-player/28850/using-the-javascript-api](http://www.longtailvideo.com/support/jw-player/28850/using-the-javascript-api)，

也可参考API文件夹中的word文档。

Modes

modes配置项被用来指定渲染播放器JW Player所使用不同浏览器技术的顺序，JW Player使用的默认顺序为：

1、Flash 插件；

2、HTML5中的video标签；

3、一个源文件的下载链接。

下面给出一段代码，让html5播放器放在最前面；

<div id="container">Loadingthe player ...</div>

<script type="text/javascript">

      jwplayer("container").setup({

           file: "/videos/video.mp4",

           height: 270,

           width: 480,

           modes: [

                   { type: "html5" },

                   { type: "flash", src: "/jwplayer/player.swf" },

                   { type: "download" }

           ]

      });

</script>



modes的具体配置（5.5版本）

可以为每个模式指定可替换的播放器配置，如果播放器嵌入在一个特定的模式中，那么它将覆盖默认的配置。

<div id="container">Loading the player ...</div>

<script type="text/javascript">

    jwplayer("container").setup({

        height: 270,

        width: 480,

        image: "http://server.com/images/thumbnail.jpg",

        modes: [

                { type: "flash",src: "/jwplayer/player.swf",config: {

                                                                file: "video.mp4",

                                                                 streamer: "rtmp://rtmp.server.com/videos",

                                                                provider: "rtmp"

                                                      }

                },

               { type: "html5",config: {

                                    file: "http://server.com/videos/video.mp4"

                              }

               },

              { type: "download" }

         ]

    });

</script>

Player Removal

使用如下代码可以去除播放器

jwplayer("container").remove();

remove方法将使播放器停止播放，DOM重新设置为原来的状态，所有的事件侦听器被清除。

获取更多关于详细，请访问[http://www.jwplayer.com/](http://www.jwplayer.com/) 网址。









