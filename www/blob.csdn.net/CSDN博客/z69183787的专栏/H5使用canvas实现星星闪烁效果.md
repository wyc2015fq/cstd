# H5使用canvas实现星星闪烁效果 - z69183787的专栏 - CSDN博客
2017年01月16日 13:13:09[OkidoGreen](https://me.csdn.net/z69183787)阅读数：3759
html
**[html]**[view
 plain](http://blog.csdn.net/u014041540/article/details/52194544#)[copy](http://blog.csdn.net/u014041540/article/details/52194544#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1826545/fork)
- <!DOCTYPE html>
- <html>
- <head>
- <metacharset="utf-8"/>
- <title>star</title>
- </head>
- <body>
- <div>
- <canvasid="canvas"width = "800"height="600"></canvas>
- </div>
- <scriptsrc="js/main.js"></script>
- <scripttype="text/javascript"src="js/commonFunctions.js"></script>
- <scripttype="text/javascript"src="js/starts.js"></script>
- </body>
- </html>
JS
main.js
**[javascript]**[view
 plain](http://blog.csdn.net/u014041540/article/details/52194544#)[copy](http://blog.csdn.net/u014041540/article/details/52194544#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1826545/fork)
- var can;  
- var ctx;  
- var w,h;  
- var girlPic = new Image();  
- var starPic = new Image();  
- var num = 60;  
- var stars  = [];  
- var lastTime,deltaTime;  
- var switchy;  
- var life = 0;  
- function init(){  
-     can  = document.getElementById("canvas");  
-     ctx = can.getContext("2d");  
-     w = can.width;  
-     h = can.height;  
-     document.addEventListener("mousemove",mousemove,false);  
-     girlPic.src = "img/girl.jpg";  
-     starPic.src = "img/star.png";  
- 
- for(var i=0;i<num;i++){  
- var obj = new starObj();  
-         stars.push(obj);  
-         stars[i].init();  
- 
-     }  
-     lastTime = Date.now();  
-     gameloop();  
- 
- }  
- document.body.onload = init;  
- function gameloop(){  
- 
-     window.requestAnimationFrame(gameloop);  
- var now = Date.now();  
-     deltaTime = now - lastTime;  
-     lastTime = now;  
-     drawBackground();  
-     drawGril();  
-     drawStars();  
-     aliveUpdate();  
- 
- }  
- function drawBackground(){  
-     ctx.fillStyle ="#393550";  
-     ctx.fillRect(0,0,w,h);  
- 
- }  
- function drawGril(){  
- //drawImage(img,x,y,width,height)
-     ctx.drawImage(girlPic,100,150,600,300);  
- 
- }  
- function  mousemove(e){  
- 
- if(e.offsetX||e.layerX){  
- var px = e.offsetX == undefined?e.layerX:e.offsetX;  
- var py = e.offsetY == undefined?e.layerY:e.offsetY;  
- //判断px py在范围内
- if(px>100&&px<700&&py>150&&py<450){  
-             switchy =true;  
- 
-         }  
- else{  
-             switchy =false;  
-         }  
- //      console.log(switchy);
-     }  
- }  
commonFunctions.js
**[javascript]**[view
 plain](http://blog.csdn.net/u014041540/article/details/52194544#)[copy](http://blog.csdn.net/u014041540/article/details/52194544#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1826545/fork)
- window.requestAnimFrame = (function() {  
- return window.requestAnimationFrame || window.webkitRequestAnimationFrame || window.mozRequestAnimationFrame || window.oRequestAnimationFrame || window.msRequestAnimationFrame ||  
- function( /* function FrameRequestCallback */ callback, /* DOMElement Element */ element) {  
- return window.setTimeout(callback, 1000 / 60);  
-         };  
- })();  
stars.js
**[javascript]**[view
 plain](http://blog.csdn.net/u014041540/article/details/52194544#)[copy](http://blog.csdn.net/u014041540/article/details/52194544#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1826545/fork)
- var starObj = function(){  
- 
- this.x;  
- this.y;  
- this.picNo;  
- this.timer;  
- this.xSpd;  
- this.ySpd;  
- }  
- starObj.prototype.init = function(){  
- this.x = Math.random()*600+100;  
- this.y = Math.random()*300+150;  
- 
- this.picNo =Math.floor(Math.random()*7);  
- this.timer = 0;  
- 
- this.xSpd = Math.random()*3-1.5;  
- this.ySpd = Math.random()*3-1.5;  
- }  
- starObj.prototype.update = function(){  
- 
- this.x += this.xSpd*deltaTime*0.01;  
- this.y += this.ySpd*deltaTime*0.01;  
- //this.x 超过范围
- if(this.x<100||this.x>693){  
- this.init();  
- return;  
-     }  
- //this.y超出范围 重生
- if(this.y<150||this.y>443){  
- this.init();  
- return;  
-     }  
- this.timer +=deltaTime;  
- if(this.timer>60){  
- this.picNo += 1;  
- this.picNo %= 7;  
- this.timer = 0;  
-     }  
- 
- 
- }  
- starObj.prototype.draw = function(){  
- 
- // save()
-     ctx.save();  
- 
- //globalAlpha 全局透明度
-     ctx.globalAlpha = life;  
- //drawImage(img,sx,sy,swidth,sheight,x,y,width,height);
-     ctx.drawImage(starPic,this.picNo*7,0,7,7,this.x,this.y,7,7);  
- //restore()
- 
-     ctx.restore();  
- }  
- function drawStars(){  
- for(var i = 0;i<num;i++){  
-         stars[i].update();  
-         stars[i].draw();  
-     }  
- 
- }  
- function aliveUpdate(){  
- if(switchy){  //in area
- //show stars
-         life +=0.3*deltaTime*0.05;  
- if(life>1){  
-             life = 1;  
-         }  
- 
-     }else{   //out area
- //hide stars
-         life -=0.3*deltaTime*0.05;  
- if(life<0){  
-             life=0;  
-         }  
效果图
![](https://img-blog.csdn.net/20160812191049245)
素材图片
1![](https://img-blog.csdn.net/20160812191853502)
2
![](https://img-blog.csdn.net/20160812191906518)
