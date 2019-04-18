# [javascript]实现登陆界面拖动窗口 - TaigaComplex求职中 - 博客园







# [[javascript]实现登陆界面拖动窗口](https://www.cnblogs.com/TaigaCon/archive/2012/08/09/2630256.html)





本代码实现的是，类似于点击[登陆](javascript:Popup())链接后,在窗口上弹出登陆对话框，并且使得对话框可以移动的方法

代码中有添加代码解释

本代码在chrome下通过测试



```
1 <!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
 2 <html xmlns="http://www.w3.org/1999/xhtml">
 3 <head>
 4     <title></title>
 5     <style>
 6     .Middle{Position:absolute;
 7             top:0;
 8             left:0;
 9             width:100%;
10             height:100%;
11             z-index:100;
12             background-color:RGBA(120,120,120,0.5);
13             }
14    .PopupBox{Position:absolute;
15                       top:200px;
16                       left:400px;
17                       width:450px;
18                       height:300px;
19                       background-color:#999999;
20                       z-index:102;
21                       border-style:solid;
22                       border-width:3px;
23                       }
24     </style>
25     <script type="text/javascript">
26         function Popup() {
27             var PBoxClick = 0;
28             var mid = document.createElement("div");
29             mid.className = "Middle";
30             document.body.appendChild(mid);
31             var PBox = document.createElement("div");
32             PBox.className = "PopupBox";
33             document.body.appendChild(PBox);
34             PBox.setAttribute("onmousedown", "MouseDown(this)");
35             PBox.setAttribute("onmousemove","MouseMove(this)");
36             PBox.setAttribute("onmouseup", "MouseUp(this)");
37             PBox.setAttribute("moving", "0");
38             PBox.style.cursor = "move";
39         }
40         var movex = 0;            //保存上次移动的鼠标X坐标
41         var movey = 0;         //Y坐标
42         function MouseMove(n) {
43 
44             if (n.moving == "down") {                       //如果鼠标down了，执行下面代码
45                 if (movex != 0 && movey != 0) {             //如果是第一次点击，表明movex和movey还没初始化，不执行下面代码
46                     var x = parseInt(event.clientX - event.offsetX, 10) + (event.clientX - movex); 
47                            //可能是出于readonly的原因，div的坐标并不能通过n.style.left获取，所以采用了event.clientX - event.offsetX
                                //本人在这里纠结了很久，原来用n.style.left还以为是其他什么地方写错了

48                     var y = parseInt(event.clientY - event.offsetY, 10) + (event.clientY-movey);
49                     n.style.left = x + "px";                //设置div的位置
50                     n.style.top = y + "px";
51                 }
52                 movex = event.clientX;//保存这次鼠标坐标，供下次做移位使用
53                 movey = event.clientY;
54             }
55 
56         }
57         function MouseDown(n) {
58                 n.moving = "down";
59         }
60         function MouseUp(n) {
61             n.moving="up";
                movex=0;
                movey=0;}
62         
63     </script>
64 </head>
65 <body>
66 <a href="javascript:Popup()">弹出窗口</a>
67 </body>
68 </html>
```














