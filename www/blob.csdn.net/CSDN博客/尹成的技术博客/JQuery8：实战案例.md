
# JQuery8：实战案例 - 尹成的技术博客 - CSDN博客

2018年11月14日 14:29:59[尹成](https://me.csdn.net/yincheng01)阅读数：68


\#1.手风琴
在页面上呈现手风琴样式得风景图，默认只有一张图片可见，其他不可见，在点击某张图片得标题时显示该张图片，隐藏之前现实的图片。
效果图:
点击之前：
![这里写图片描述](https://img-blog.csdn.net/20180912142656765?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MjI4NjAyMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
点击之后：
![这里写图片描述](https://img-blog.csdn.net/2018091214272853?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MjI4NjAyMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
`<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <title>非洲手风情</title>
    <style type="text/css">
        *{
            margin: 0;
            padding: 0;
        }
        #accordion {
            margin: 20px auto 0;
            /*border: 1px solid black;*/
            width: 727px;
            height: 350px;
            position: relative;
            overflow: hidden;
        }
        ul {
            list-style: none;
        }
        li {
            position: absolute;
            top: 0;
            overflow: hidden;
            width: 643px;
            height: 350px;
        }
        li span, li img {
            float: left;
            display: block;
        }
        li span {
            width: 20px;
            height: 350px;
            display: block;
            padding: 0;
            border-right: 1px solid white;
        }
        li img{
            width: 622px;
            height: 350px;
        }
        .bar01 span {
            background-color: red;
        }
        .bar02 span {
            background-color: orange;
        }
        .bar03 span {
            background-color: yellow;
        }
        .bar04 span {
            background-color: green;
        }
        .bar05 span {
            background-color: lightskyblue;
        }
    </style>
    <script type="text/javascript" src="js/jquery-1.12.4.min.js"></script>
    <script type="text/javascript">
        $(function () {
            
            $("ul li").each(function (i) {
                $(this).css({
                    left:0+21*i,
                })
            });
            
            var current = $("#accordion li").length - 1;
            $('#accordion').delegate("li","click",function () {
                var target = $(this).index();
                $("#accordion li").each(function (i) {
                    if(target < current){
                        if(i>target && i<=current){
                            $(this).animate({
                                left:727-21*(5-i)
                            },300,function () {
                                current = target;
                            })
                        }
                    }else if(target > current){
                        if(i>current && i<=target){
                            $(this).animate({
                                left:0+21*i,
                            },300,function () {
                                current = target;
                            })
                        }
                    }
                })
            })
        });
    </script>
</head>
<body>
<div id="accordion">
    <ul>
        <li class="bar01"><span>非洲景色01</span><img src="images/001.jpg"/></li>
        <li class="bar02"><span>非洲景色02</span><img src="images/002.jpg"/></li>
        <li class="bar03"><span>非洲景色03</span><img src="images/003.jpg"/></li>
        <li class="bar04"><span>非洲景色04</span><img src="images/004.jpg"/></li>
        <li class="bar05"><span>非洲景色05</span><img src="images/005.jpg"/></li>
    </ul>
</div>
</body>
</html>`\#2.无缝滚动
在页面显示滚动效果(向左或向右)，在点击向左时则向左滚动，点击向右滚动时则相右滚动，鼠标在某一个方格中停顿则停止滚动。
效果图：
![这里写图片描述](https://img-blog.csdn.net/20180912143058600?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MjI4NjAyMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
`<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <title>无缝滚动</title>
    <style type="text/css">
        /*常规预设*/
        body, ul, li {
            margin: 0;
            padding: 0
        }
        ul {
            list-style: none;
        }
        /*列表容器*/
        .slide {
            width: 500px;
            height: 100px;
            border: 1px solid #ddd;
            margin: 20px auto 0;
            /*绝对定位参照系*/
            position: relative;
            /*画外部分隐藏*/
            overflow: hidden;
        }
        /*ul的宽度是容器的两倍*/
        .slide ul {
            position: absolute;
            width: 1000px;
            height: 100px;
            /*绝对定位偏移量0*/
            left: 0;
            top: 0;
        }
        /*正常水平排列的li*/
        .slide ul li {
            width: 90px;
            height: 90px;
            margin: 5px;
            background-color: #ccc;
            line-height: 90px;
            text-align: center;
            font-size: 30px;
            float: left;
        }
        /*按钮容器*/
        .btns {
            width: 500px;
            height: 50px;
            margin: 10px auto 0;
            text-align: center;
        }
    </style>
    <script type="text/javascript" src="js/jquery-1.12.4.min.js"></script>
    <script type="text/javascript">
        $(function () {
            var $btn1 = $("#btn1");
            var $btn2 = $("#btn2");
            var $ul = $(".slide ul");
            var offset = 2;
            var left = 0;
            var direction = "right";
            var $timer;
            //拷贝一份
            $ul.html($ul.html() + $ul.html())
            $btn1.click(function () {
                direction = "left";
            });
            $btn2.click(function () {
                direction = "right";
            });
            $ul.hover(
                function () {
                    clearInterval($timer);
                },
                function () {
                    $timer = setInterval(function () {
                        reload();
                    }, 20);
                }
            );
            function reload() {
                if (direction == "right") {
                    left += offset;
                    if(left > 0){
                        left = -500;
                    }
                } else {
                    left -= offset;
                    if(left < -500){
                        left = 0;
                    }
                }
                //改变整个ul的偏移量
                $ul.css({left: left})
            }
            //开始周期性滚动
            $timer = setInterval(function () {
                reload();
            }, 20)
        })
    </script>
</head>
<body>
<div class="btns">
    <input type="button" name="" value="向左" id="btn1">
    <input type="button" name="" value="向右" id="btn2">
</div>
<div class="slide" id="slide">
    <ul>
        <li>1</li>
        <li>2</li>
        <li>3</li>
        <li>4</li>
        <li>5</li>
    </ul>
</div>
</body>
</html>`\#3.美女相册
在页面上显示美女图片，当点击某位美女时显示该美女得大图。
效果图：
![这里写图片描述](https://img-blog.csdn.net/20180912143801568?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MjI4NjAyMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
点击美女A：
![这里写图片描述](https://img-blog.csdn.net/20180912143822737?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MjI4NjAyMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
`<!DOCTYPE html>
<html>
<head lang="en">
  <meta charset="UTF-8">
  <title></title>
  <style type="text/css">
    body {
      font-family: "Helvetica", "Arial", serif;
      color: #333;
      background-color: #ccc;
      margin: 1em 10%;
    }
    
    h1 {
      color: #333;
      background-color: transparent;
    }
    
    a {
      color: #c60;
      background-color: transparent;
      font-weight: bold;
      text-decoration: none;
    }
    
    ul {
      padding: 0;
    }
    
    li {
      float: left;
      padding: 1em;
      list-style: none;
    }
    
    #imagegallery {
      
      list-style: none;
    }
    
    #imagegallery li {
      margin: 0px 20px 20px 0px;
      padding: 0px;
      display: inline;
    }
    
    #imagegallery li a img {
      border: 0;
    }
  </style>
  
  <script src="../jquery-1.12.4.js"></script>
  <script>
    $(function () {
      //1. 给所有的a注册点击事件
      
      
      $("#imagegallery a").click(function () {
        var href = $(this).attr("href");
        $("#image").attr("src", href);
        
        var title = $(this).attr("title");
        $("#des").text(title);
        
        return false;
      });
    });
  </script>
</head>
<body>
<h2>
  美女画廊
</h2>
<ul id="imagegallery">
  <li><a href="images/1.jpg" title="美女A">
    <img src="images/1-small.jpg" width="100" alt="美女1"/>
  </a></li>
  <li><a href="images/2.jpg" title="美女B">
    <img src="images/2-small.jpg" width="100" alt="美女2"/>
  </a></li>
  <li><a href="images/3.jpg" title="美女C">
    <img src="images/3-small.jpg" width="100" alt="美女3"/>
  </a></li>
  <li><a href="images/4.jpg" title="美女D">
    <img src="images/4-small.jpg" width="100" alt="美女4"/>
  </a></li>
</ul>
<div style="clear:both"></div>
<img id="image" src="images/placeholder.png" alt="" width="450px"/>
<p id="des">选择一个图片</p>
</body>
</html>`学院Go语言视频主页
[https://edu.csdn.net/lecturer/1928](https://edu.csdn.net/lecturer/1928)
[清华团队带你实战区块链开发](https://ke.qq.com/course/344443?tuin=3d17195d)
扫码获取海量视频及源码   QQ群：721929980
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181114143613461.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmNoZW5nMDE=,size_16,color_FFFFFF,t_70)

