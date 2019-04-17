# HTTP协议下可拖动时间轴播放FLV的实现（伪流媒体） - DoubleLi - 博客园






HTTP协议下实现FLV的播放其实并不复杂，当初实现的原理是使用了flowPlayer插件实现的，效果还不错。但仍有两大问题影响着客户的访问情绪：

1.预加载时页面卡死，似乎没有边下边播。

2.偶尔边下边播，却无法拖动时间轴至未下载的部分。相信很多人也遇到该问题。

一度想采用专门的媒体服务器如Adobe的FMS去实现该功能，后多方查找资料，发现采用媒体服务器成本较高，且效率并不是很好，各大视频网站也未采用该方式。而实现HTTP协议下播放flv并可拖动时间轴并非没有可能，关键在于以下几点：
- Flv视频文件包含metadata信息，大多数转码工具生成的FLV不包含该信息。可用工具增加（flvtool2，yamdi[速度很快，效率高]）。
- Web端播放器需支持拖动时间轴时发送请求的连接中带有字节参数，或时间参数。
- 服务器端实现对flv文件的读取和流式输出。

一、给FLV文件加入metadata信息

flvtool2和yamdi都可以实现该功能，但yamdi工具的效率要高出很多，400M左右的FLV处理时间大概2分钟，推荐使用。实现方式是在cmd命令窗口下执行如下命令：

yamdi -i 源文件名 -o 新文件名

二、flowPlayer的使用与配置

flowPlayer是一款web端播放flv等视频的利器，功能比较强大，采用的版本3.2.2，可支持多种插件，此次实现可拖动时间轴的功能也是使用了它的一款插件， 该插件名为：flowplayer.pseudostreaming-byterange-3.2.9.swf，采用的版本是3.2.9，3.2.10不可作为flowPlayer3.2.2的插件使用， [测试](http://lib.csdn.net/base/softwaretest)未有图像显示。页面中的编写方式是，红色标出的是重要部分：

> 
<%@ page language=“[Java](http://lib.csdn.net/base/java)” import=“java.util.*;” pageEncoding=“UTF-8″%>

<%@ taglib prefix=“c” uri=“http://java.sun.com/jstl/core”%>

<!DOCTYPE HTML PUBLIC “-//W3C//DTD HTML 4.01 Transitional//EN”>

<html>

    <head>

       <title>FLV</title>

       <script type=”text/[JavaScript](http://lib.csdn.net/base/javascript)”

           src=”<c:url value=“/script/[jQuery](http://lib.csdn.net/base/jquery)-1.5.2.min.[js](http://lib.csdn.net/base/javascript)” />”></script>

       <script type=“text/[javascript](http://lib.csdn.net/base/javascript)”

           src=”<c:url value=“/_flowplayer/flowplayer-3.2.4.min.js”/>”></script>



    </head>

    <body>



       <script type=“text/javascript”>

       <!–

           var p;

           $(function(){

              p = $(“.player”).flowplayer(

                  {

                     src:’<c:url value=”/_flowplayer/flowplayer.commercial.swf”/>’,

                     wmode: “opaque”

                  },

                  {

                     clip:{

                         //scaling: ‘orig’,   设置播放器读取原始视频高宽比

                         autoPlay: true,

                         autoBuffering: true,

                         bufferLength: 1,

                         provider: ‘lighttpd’

                  },

                  plugins: {

                     controls: {

                         url: ‘<c:url value=”/_flowplayer/flowplayer.controls-air-3.2.2.swf”/>’,

                         opacity: 0.8,

                         backgroundColor: ‘#000′,

                         scrubber : true,



                         buttonColor: ‘#000′,

                         buttonOverColor: ‘#4c4c4c’,



                         autoHide: {

                            enabled: true,

                            fullscreenOnly: false,

                            hideDelay: 1000,

                            mouseOutDelay: 2000,

                            hideStyle: ‘fade’

                         }  

                     },

                     lighttpd: {

                          url: “<c:url value=”/_flowplayer/flowplayer.pseudostreaming-byterange-3.2.9.swf”/>”

                         ,queryString: escape(‘?target=${“${start}”}&secretToken=1235oh8qewr5uweynkc’)

                            // queryString配置了拖动时间轴后发送到后台的参数。${start}为固定格式。

                      }                                                

                  },

                  play: { replayLabel : “再次播放”, width:120 , height: 50}

              })

           })

       //à

       </script>

       <!—视频展示区域à

       <div class=”left_video_areaBg clearWrap”>

           <!—视频限制高宽 W:451px H:252pxà

           <a class=”player”

              href=”<c:url value=”/movie/131201174437530567C.flv”/>”

              style=”display: block; width: 429px; height: 252px;” id=”player1”>

           </a>

       </div>

        <div class=”left_video_dotLine”></div>

       <div class=”blank8”></div>



       <button onclick=”$f().seek(60);”>1分钟</button>

       <button onclick=”$f().seek(180);”>3分钟</button>

       <button onclick=”alert($f().getTime());”>获取当前时间点</button>

    </body>

</html>


三、实现流式输出的Servlet的编写

package flv.laukin[.NET](http://lib.csdn.net/base/dotnet);



import java.io.IOException;

import java.io.RandomAccessFile;



import javax.servlet.ServletException;

import javax.servlet.http.HttpServlet;

import javax.servlet.http.HttpServletRequest;

import javax.servlet.http.HttpServletResponse;



public class FlvStreamServlet extends HttpServlet{



    protected void doGet(HttpServletRequest req, HttpServletResponse resp)

            throws ServletException, IOException {

        // TODO Auto-generated method stub



        resp.reset();

        resp.setContentType(“Video/x-flv”);



        String target = req.getParameter(“target”);  //接收参数，为字节数

        int targetInt = 0;



        System.out.println(“Target:” + target);

        System.out.println(“Target:” + req.getServletPath());

        String flvPath = req.getSession().getServletContext().getRealPath(req.getServletPath());

        System.out.println(flvPath);



        RandomAccessFile raf = null;

        int totalByte = 0;

        try{

            raf = new RandomAccessFile(flvPath, “r”);

            totalByte = (int)raf.length();



            if (target != null && !”".equals(target)) {

                try {

                    targetInt = Integer.parseInt(target);

            byte[] headData = new byte[]{‘F’,'L’,'V’,1,1,0,0,0,9,0,0,0,9}; //拖动时间轴后的response中头信息需写入该字节 

                    resp.getOutputStream().write(headData);

                    resp.setContentLength(totalByte – targetInt + 13);

                } catch (NumberFormatException e) {

                    targetInt = 0;

                }

            } else {

                resp.setContentLength(totalByte – targetInt);

            }



            raf.skipBytes(targetInt);//跳过时间轴前面的字节;



            byte[] b = new byte[4096];

            while(raf.read(b) != -1) {

                resp.getOutputStream().write(b);

            }

            resp.getOutputStream().flush();



        } catch (Exception e) {

            String simplename = e.getClass().getSimpleName();

            if(!”ClientAbortException”.equals(simplename)){

                e.printStackTrace();

            }//web端拖动时间轴总有连接被重置的异常，暂时还不知如何解决，可以此方式不输出异常

        } finally {

            if(raf != null){

                raf.close();

            }

        }

    }

}

web.xml中增加配置：

    <servlet>

       <servlet-name>FlvStream</servlet-name>

       <servlet-class>flv.laukin.net.FlvStreamServlet</servlet-class>

       <load-on-startup>1</load-on-startup>

    </servlet>

    <servlet-mapping>

       <servlet-name>FlvStream</servlet-name>

       <url-pattern>*.flv</url-pattern>

</servlet-mapping>

至此Tomcat下的FLV播放就可实现任意拖动了。

下面的连接为项目代码，可下载交流，测试可自己制作 flv 放到 movie目录下。

[FLVstreaming](http://www.laukin.net/wordpress/wp-content/uploads/2012/08/FLVstreaming.zip)



来源：[http://www.laukin.net/wordpress/archives/191](http://www.laukin.net/wordpress/archives/191)









