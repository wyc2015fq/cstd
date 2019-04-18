# js window.open 打开页面并居中 - z69183787的专栏 - CSDN博客
2013年12月24日 14:09:38[OkidoGreen](https://me.csdn.net/z69183787)阅读数：10315
1.js
- <script type="text/javascript">  
- 
- function openwindow(url,name,iWidth,iHeight)  
- {  
- // url 转向网页的地址
- // name 网页名称，可为空
- // iWidth 弹出窗口的宽度
- // iHeight 弹出窗口的高度
- //window.screen.height获得屏幕的高，window.screen.width获得屏幕的宽
- var iTop = (window.screen.height-30-iHeight)/2; //获得窗口的垂直位置;
- var iLeft = (window.screen.width-10-iWidth)/2; //获得窗口的水平位置;
- window.open(url,name,'height='+iHeight+',,innerHeight='+iHeight+',width='+iWidth+',innerWidth='+iWidth+',top='+iTop+',left='+iLeft+',toolbar=no,menubar=no,scrollbars=auto,resizeable=no,location=no,status=no');  
- }  
- 
- </script>  

2.调用方法
- <ahref="javascript:void(0);"onclick="javascript:openwindow('a.html','',400,200);">转到a</a>
- 
