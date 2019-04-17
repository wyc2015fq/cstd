# 使用 nginx 和 rtmp 插件搭建视频直播和点播服务器 - DoubleLi - 博客园






使用 nginx 和 [rtmp 模块](https://github.com/arut/nginx-rtmp-module) ，可以很容易地搭建一个视频直播和点播服务器出来。

首先，看一下最经典的参考文献： [How to set up your own private RTMP server using nginx](https://obsproject.com/forum/resources/how-to-set-up-your-own-private-rtmp-server-using-nginx.50/)

1. 安装 nginx 和 rtmp 模块

有关 nginx 的编译和安装比较简单，这里就不介绍了，看参考文献。这里提示以下几点：

(1) 安装好 nginx 后，配置文件在这里：
/usr/local/nginx/conf/nginx.conf
(2) 启动 nginx 的命令：
$ sudo /usr/local/nginx/sbin/nginx -s stop
$ sudo /usr/local/nginx/sbin/nginx
2. 配置 nginx 视频直播和点播服务

先看一下完整的 nginx 配置文件里有关视频点播和直播的配置：
rtmp {
	server {
		listen 1935;
		chunk_size 4096;
		application live {
			live on;
			record off;
		}
		application live2 {
			live on;
			record off;
		}
		# video on demand
		application vod {
			play /var/flvs;
		}
		application vod_http {
			play http://192.168.31.185/vod;
		}
		application hls {
			live on;
			hls on;
			hls_path /tmp/hls;
		}
	}
}
# HTTP can be used for accessing RTMP stats
http {
    server {
	listen      8080;
	# This URL provides RTMP statistics in XML
	location /stat {
	    rtmp_stat all;
	    # Use this stylesheet to view XML as web page
	    # in browser
	    rtmp_stat_stylesheet stat.xsl;
	}
	location /stat.xsl {
	    # XML stylesheet to view RTMP stats.
	    # Copy stat.xsl wherever you want
	    # and put the full directory path here
	    root /path/to/stat.xsl/;
	}
	location /hls {
	    # Serve HLS fragments
	    types {
		application/vnd.apple.mpegurl m3u8;
		video/mp2t ts;
	    }
	    root /tmp;
	    add_header Cache-Control no-cache;
	}
	location /dash {
	    # Serve DASH fragments
	    root /tmp;
	    add_header Cache-Control no-cache;
	}
    }
}

现在来解释一下里面各行代码的含义。对于视频直播服务，如果需要支持多路流输入的话，很简单，在 nginx 配置文件里多配几个 Application 就只可以了，像下面这样：
application live {
	live on;
	record off;
}
application live2 {
	live on;
	record off;
}

这样就可以通过下面的地址来推送直播流，其它观众端也可以通过下面的地址来访问直播流：
rtmp://192.168.31.185/live/test
rtmp://192.168.31.185/live2/test
后面紧跟的 test 关键字，可以随便更换，只要你的推送流和访问流的地址一样就可以了。

[rtmp 模块也可以直接支持 VOD 这种视频点播服务](https://github.com/arut/nginx-rtmp-module/wiki/Directives#video-on-demand) ，只需要在配置文件里添加如下内容即可：
# video on demand
application vod {
    play /var/flvs;
}

application vod_http {
    play http://myserver.com/vod;
}
然后把一个 mp4 或是 flv 文件扔到 /var/flvs 目录下，对于 /var/flvs/dir/file.flv 这个视频文件，就可以通过下面的网址来访问了：
  http://myserver.com/vod//dir/file.flv
这样直接在浏览器里就可以通过网页观看视频。对于 mp4 文件，也可以实现 VOD 服务，不过需要的是采用 H.264 和 AAC 格式编码的 mp4 文件。

3. HLS 直播流的配置

如果需要使用 HLS 来视频直播，可以直接像配置文件那样，写上下面这一段：
application hls {
        live on;
        hls on;
        hls_path /tmp/hls;
}
同时把后面有关 http 访问的内容写上：
# HTTP can be used for accessing RTMP stats
http {
	server {
		listen	  8080;
		# This URL provides RTMP statistics in XML
		location /stat {
			rtmp_stat all;
			# Use this stylesheet to view XML as web page
			# in browser
			rtmp_stat_stylesheet stat.xsl;
		}
		location /stat.xsl {
			# XML stylesheet to view RTMP stats.
			# Copy stat.xsl wherever you want
			# and put the full directory path here
			root /path/to/stat.xsl/;
		}
		location /hls {
			# Serve HLS fragments
			types {
				application/vnd.apple.mpegurl m3u8;
				video/mp2t ts;
			}
			root /tmp;
			add_header Cache-Control no-cache;
		}
		location /dash {
			# Serve DASH fragments
			root /tmp;
			add_header Cache-Control no-cache;
		}
	}
}

配好以后，推流可以使用下面的地址：
rtmp://192.168.31.185/hls/movie
movie 关键字可以任何替换。对于观众端来说，可以有几种播放方式：

(1) 用 rtmp：
rtmp://192.168.31.185/hls/movie
(2) 用 hls 播放：
http://192.168.31.185:8080/hls/movie.m3u8
这样就可以看到主播端推出来的流。注意，如果使用 http 方式，则是监听的 8080 端口，这个是在配置文件里写的。

4. 网页播放器插件

在第二步里，除了可以直接在浏览器里打开网址来观看视频，还可以写一个网页，实现像优酷那样的视频点播业务。通过使用第三方的播放器，在网页里植入该播放器来实现这个功能，比如说使用 [JWPlayer](https://www.jwplayer.com/) 播放器。

下载 JWPlayer 播放器，放到 nginx 网页服务的根目录，比如说，在我这儿是 /usr/local/nginx/html 这个目录，把 JWPlayer 解压后放到这个目录里，然后在这个目录里新建一个 play.html 的文本文件，再写入以下内容：
<html>
  <head>
    <script src="/jwplayer/jwplayer.js"></script>
  </head>

<body>
  <div id='my-video'></div>
  <script type='text/javascript'>
    jwplayer('my-video').setup({
      file:'http://192.168.31.185/vod/North.mp4',
      fallback:'false'
    });
  </script>
</body>
</html>
对于 flv 文件，可以采用如下的形式来实现网页播放：
<html>
  <head>
    <script src="/jwplayer/jwplayer.js"></script>
  </head>

<body>
  <div id='my-video'></div>
  <script type='text/javascript'>
    jwplayer('my-video').setup({
      file:'http://192.168.31.185/vod/1.flv',
      width:'50%',
      aspectratio:'3:2',
      fallback:'false',
      primary:'flash'
    });
  </script>
</body>
</html>
这样，当你打开 [http://192.168.31.185/play.html](http://192.168.31.185/play.html) 这个网页时，就能看到 html 里指定的视频文件。

另外，如果有一些 flv 文件没有办法拖动播放，那是因为没有给 flv 文件添加“关键帧”，可以用工具“yamdi” 和“flvtool”来给 flv 生成关键帧，以 yamdi 为例，下载解压后从 shell 执行:
yamdi –i src.flv –o dst.flv
意思是将 src.flv 添加关键帧，并输出为 dst.flv 。这样 flv 就有关键帧了，即可进行拖动播放操作。









