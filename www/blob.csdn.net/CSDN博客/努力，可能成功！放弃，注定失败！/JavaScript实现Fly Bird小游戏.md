# JavaScript实现Fly Bird小游戏 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年12月15日 09:09:46[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：1420
### 1.分析页面结构，理清需求和功能
游戏有三个界面，分别是开始界面，游戏界面和游戏结束界面。
#### 1.1 开始界面
![](http://upload-images.jianshu.io/upload_images/3772457-af76318ac007d438.gif?imageMogr2/auto-orient/strip)
start.gif
- 游戏的大背景
- 上下移动的游戏标题和翅膀摆动的小鸟
- start 按钮，点击进入游戏界面
- 一直移动的地面
#### 1.2 游戏界面
![](http://upload-images.jianshu.io/upload_images/3772457-937fe47bcf3b38e3.gif?imageMogr2/auto-orient/strip)
play.gif
- 显示越过障碍数量的计分器
- 移动的障碍物，分别是上管道和下管道
- 点击游戏界面，小鸟向上飞起，然后在重力作用下下坠，
- 当小鸟和管道碰撞后，结束界面弹出，同时小鸟落到地面
#### 1.3 结束界面
- GAMEOVER 提示面板
- OK 按钮
### 2. 开发“开始界面”
考虑到草地的移动效果，我们在页面中加入两个草地
#### 2.1 HTML
```xml
<!DOCTYPE html>
<html>
    <head>
        <meta charset="utf-8" />
        <title>Fly Bird</title>
        <link rel="stylesheet" type="text/css" href="css/index.css"/>
    </head>
    <body>
        <div id="wrapBg">  <!--游戏背景-->
            <div id="headTitle"> <!--开始标题-->
                <img id="headBird" src="img/bird0.png" alt="小鸟" /> <!--标题中的小鸟-->
            </div>
            <button id="startBtn" ></button> <!--开始按钮-->
            <div id="grassLand1"></div> <!--草地1-->
            <div id="grassLand2"></div> <!--草地2-->
        </div>
    </body>
</html>
```
#### 2.2 CSS
```
```
#wrapBg{/*游戏背景*/
    width: 343px;height: 480px; 
    margin: 0 auto;
    background-image:url(../img/bg.jpg);
    position: relative;
    top: 100px;
    overflow: hidden;
}
#headTitle{/*开始标题*/
    width: 236px;height: 77px;
    background-image: url(../img/head.jpg);
    position: absolute; left: 53px; top: 100px;
}
#headBird{/*开始标题中的小鸟*/
    float:right;
    margin-top: 25px;
}
#startBtn{/*开始按钮*/
    width: 85px;height: 29px;
    padding: 0;margin: 0;
    background-image: url(../img/start.jpg);
    position: absolute;left: 129px;top: 250px;
}
#grassLand1{/*草地1*/
    height: 14px;width: 343px;
    background-image: url(../img/banner.jpg);
    position: absolute;top: 423px;
}
#grassLand2{/*草地2*/
    height: 14px;width: 343px;
    background-image: url(../img/banner.jpg);
    position: absolute;top: 423px;left: 343px;
}
```
```
将wrapBg中的overflow:hidden 注释掉的页面效果
![](http://upload-images.jianshu.io/upload_images/3772457-3f2623b9b60bdad1.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
开始界面.jpg
#### 2.3 JS
小鸟煽动翅膀的效果需要用到逐帧动画的原理
> 
逐帧动画是一种常见的动画形式（Frame By Frame），其原理是在“连续的关键帧”中分解动画动作，也就是在时间轴的每帧上逐帧绘制不同的内容，使其连续播放而成动画。
![](http://upload-images.jianshu.io/upload_images/3772457-6fc27039303dedb4.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
bird1.png
![](http://upload-images.jianshu.io/upload_images/3772457-e06746a01584016a.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
bird0.png
##### 2.3.1 开始标题的摆动
```java
```java
var jsHeadTitle = document.getElementById("headTitle");// 获取标题
        var jsHeadBird = document.getElementById("headBird"); // 获取标题中小鸟
        var Y = 3;//标题的摆动幅度
        var index = 0;
        var imgArr = ["img/bird0.png","img/bird1.png"] 
        //将小鸟图片路径放入一个数组，利用逐帧动画的原理做出小鸟翅膀摆动的样子
        var headWaveTimer = setInterval(headWave,200); //设置标题上下摆动的定时器
        function headWave() {
            Y *= -1;
            jsHeadTitle.style.top = jsHeadTitle.offsetTop + Y + "px";
            jsHeadBird.src = imgArr[index++];
            if (index == 2) {
                index = 0;
            }
        }
```
```
##### 2.3.2 移动的草地
```java
```java
var jsGrassLand1 = document.getElementById("grassLand1"); //获取草地1
        var jsGrassLand2 = document.getElementById("grassLand2"); //获取草地2
        var landTimer = setInterval(landRun,30); //让草地动起来的定时器
        function landRun() {
            if (jsGrassLand1.offsetLeft <= -343) {
                jsGrassLand1.style.left = "343px";
            }
            if (jsGrassLand2.offsetLeft <= -343) {
                jsGrassLand2.style.left = "343px";
            }
            jsGrassLand1.style.left = jsGrassLand1.offsetLeft - 3 + "px";
            jsGrassLand2.style.left = jsGrassLand2.offsetLeft - 3 + "px";
        }
```
```
### 2.3.3 Start按键
```java
```java
var jsStartBtn = document.getElementById("startBtn");
        jsStartBtn.onclick = function() { //为start按键添加点击事件处理程序
            jsHeadTitle.style.display = "none"; //隐藏标题
            clearInterval(headWaveTimer); //关闭让标题摆动的定时器
            jsStartBtn.style.display = "none"; //隐藏按键
            //待添加功能
            //点击开始按键进入游戏界面
        }
```
```
完成后的效果（注释掉了wrapBg中的overflow:hidden ）
![](http://upload-images.jianshu.io/upload_images/3772457-eb7a9d6f591175b3.gif?imageMogr2/auto-orient/strip)
start01.gif
接下来我们开发“游戏界面”
### 3. “游戏界面”的开发
游戏界面中有三样元素，分别是“小鸟”，“障碍”，和“计分器”，我们依次来创建相应的对象。
#### 3.1 小鸟
首先，创建小鸟的对象，** bird.js **文件。
```java
```java
var bird = {
    flyTimer:null,//小鸟飞翔定时器
    wingTimer:null,//小鸟翅膀摆动定时器
    div:document.createElement("div"),
    showBird:function(parentObj) {
        this.div.style.width = "40px";
        this.div.style.height = "28px";
        this.div.style.backgroundImage = "url(img/bird0.png)";
        this.div.style.backgroundRepeat = "no-repeat";
        this.div.style.position = "absolute";
        this.div.style.left = "50px";
        this.div.style.top = "200px";
        this.div.style.zIndex = "1";
        parentObj.appendChild(this.div);  //将小鸟DIV插入游戏界面中
    },
    fallSpeed: 0, //小鸟下落速度
    flyBird: function(){ //控制小鸟飞翔下落的函数
        bird.flyTimer = setInterval(fly,40);
        function fly() {
            bird.div.style.top = bird.div.offsetTop + bird.fallSpeed++ + "px";
            if (bird.div.offsetTop < 0) {  
                bird.fallSpeed = 2; //这里用于控制小鸟不要飞出界面
            }
            if (bird.div.offsetTop >= 395) {
                bird.fallSpeed = 0;
                clearInterval(bird.flyTimer); //一旦飞到地面，清除定时器
                clearInterval(bird.wingTimer); //清除翅膀摆动定时器
            }
            if (bird.fallSpeed > 12) {
                bird.fallSpeed = 12;  //鸟的最大下落速度控制在12
            }
        }
    },
    wingWave: function() { //控制小鸟煽动翅膀的函数
        var up = ["url(img/up_bird0.png)", "url(img/up_bird1.png)"];
        var down = ["url(img/down_bird0.png)", "url(img/down_bird1.png)"];
        var i = 0, j = 0;
        bird.wingTimer = setInterval(wing,120);//逐帧动画，小鸟煽动翅膀
        function wing() {
            if (bird.fallSpeed > 0) {
                bird.div.style.backgroundImage = down[i++];
                if (i==2) {i = 0}
            }if (bird.fallSpeed < 0) {
                bird.div.style.backgroundImage = up[j++];
                if (j==2) {j = 0}
            }
        }
    },    
};
```
```
下面，实现点击start按钮时，加载小鸟。(在之前的代码基础上添加)
```php
```php
jsStartBtn.onclick = function() { //为start按键添加点击事件处理程序
    jsHeadTitle.style.display = "none"; //隐藏标题
    clearInterval(headWaveTimer); //关闭让标题摆动的定时器
    jsStartBtn.style.display = "none"; //隐藏按键
    bird.showBird(jsWrapBg); //插入小鸟到界面中
    bird.flyBird(); //控制小鸟飞翔下落
    bird.wingWave(); //逐帧动画，小鸟煽动翅膀
    jsWrapBg.onclick = function(){
        bird.fallSpeed = -8;
    };
    //待添加功能
    //点击开始按键进入游戏界面
}
```
```
添加小鸟后的效果
![](http://upload-images.jianshu.io/upload_images/3772457-d42e05e314a9b68c.gif?imageMogr2/auto-orient/strip)
play01.gif
#### 3.2 障碍（上方水管和下方水管）
![](http://upload-images.jianshu.io/upload_images/3772457-3922d474e2558237.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
block示意图.png
障碍分为上方管道和下方管道，如示意图所示结构嵌套，这样就可以通过随机设置DownDiv2的高度和gapHeight的高度，来改变生成障碍的形态
**block.js **
```java
```java
function Block() {
    this.upDivWrap = null;
    this.downDivWrap = null;
    this.downHeight = baseObj.randomNum(0,150);
    this.gapHeight = baseObj.randomNum(150,160);
    this.upHeight = 312 - this.downHeight - this.gapHeight;
    // 用来生成Div的方法
    this.createDiv = function(url, height, positionType, left, top) {
        var newDiv = document.createElement("div");
        newDiv.style.width = "62px";
        newDiv.style.height = height;
        newDiv.style.position = positionType;
        newDiv.style.left = left;
        newDiv.style.top = top;
        newDiv.style.backgroundImage = url;  //"url(/img/0.jpg)"
        return newDiv;
    };
    this.createBlock = function() {
        var upDiv1 = this.createDiv("url(img/up_mod.png)", this.upHeight + "px");
        var upDiv2 = this.createDiv("url(img/up_pipe.png)", "60px");
        this.upDivWrap = this.createDiv(null, null, "absolute", "450px");
        this.upDivWrap.appendChild(upDiv1);
        this.upDivWrap.appendChild(upDiv2);//生成上方管道
        var downDiv1 = this.createDiv("url(img/down_pipe.png)", "60px");
        var downDiv2 = this.createDiv("url(img/down_mod.png)", this.downHeight +"px");
        this.downDivWrap = this.createDiv(null, null, "absolute", "450px", 363 - this.downHeight + "px");
        this.downDivWrap.appendChild(downDiv1);
        this.downDivWrap.appendChild(downDiv2); //生成下方的管道
        jsWrapBg.appendChild(this.upDivWrap);
        jsWrapBg.appendChild(this.downDivWrap);
    };
    this.moveBlock = function() { //控制管道移动的方法
        this.upDivWrap.style.left = this.upDivWrap.offsetLeft - 3 + "px";
        this.downDivWrap.style.left = this.downDivWrap.offsetLeft - 3 + "px";
    };    
}
```
```
公共对象文件 **baseObj.js **，用来提供随机数，和两个矩形div的碰撞检测
```java
```java
var baseObj = {
    //随机数
    randomNum: function(min, max) {
        return parseInt(Math.random() * (max - min + 1) + min);
    },
    //两个矩形元素之间的碰撞检测
    rectangleCrashExamine: function (obj1, obj2) {
            var obj1Left = obj1.offsetLeft;
            var obj1Width = obj1.offsetLeft + obj1.offsetWidth;
            var obj1Top = obj1.offsetTop;
            var obj1Height = obj1.offsetTop + obj1.offsetHeight;
            var obj2Left = obj2.offsetLeft;
            var obj2Width = obj2.offsetLeft + obj2.offsetWidth;
            var obj2Top = obj2.offsetTop;
            var obj2Height = obj2.offsetTop + obj2.offsetHeight;
            if (!(obj1Left > obj2Width || obj1Width < obj2Left || obj1Top > obj2Height || obj1Height < obj2Top)) {
                return true;
            }
            return false;
    },
};
```
```
下面我的想法是在start按钮点击的时候创建一个block，把这个block存储到数组* blocksArr *中，在* landTimer*定时器的方法* landRun *中检查此数组的长度，如果数组不为空数组，那么就让数组中所有的block移动。
检查最后一个block离开的距离，达到一定距离，就重新new 一个block，添加到数组。
检查第一个block，一旦达到一定位置，就在结构中移除downDivWrap 和 upDivWrap，同时在数组中删除。
```cpp
```cpp
var landTimer = setInterval(landRun,30); //让草地动起来的定时器
        function landRun() {
            if (jsGrassLand1.offsetLeft <= -343) {
                jsGrassLand1.style.left = "343px";
            }
            if (jsGrassLand2.offsetLeft <= -343) {
                jsGrassLand2.style.left = "343px";
            }
            jsGrassLand1.style.left = jsGrassLand1.offsetLeft - 3 + "px";
            jsGrassLand2.style.left = jsGrassLand2.offsetLeft - 3 + "px";
            if (blocksArr.length) {
                for (var i = 0; i < blocksArr.length; i++) {
                    blocksArr[i].moveBlock();
                    var x =baseObj.rectangleCrashExamine(blocksArr[i].downDivWrap, bird.div);
                    var y = baseObj.rectangleCrashExamine(blocksArr[i].upDivWrap, bird.div);
                    var z = bird.div.offsetTop >= 390;
                    if (x || y || z) {
                        window.clearInterval(landTimer);//清除landTimer定时器
                        bird.fallSpeed = 0; //小鸟下落
                        jsWrapBg.onclick = null; //消除点击事件
                    }
                }
                if (blocksArr[blocksArr.length - 1].downDivWrap.offsetLeft < (450 - blockDistance)) {
                        blockDistance = baseObj.randomNum(130,250);
                        var newBlock = new Block();
                        newBlock.createBlock();
                        blocksArr.push(newBlock);
                }
                if (blocksArr[0].downDivWrap.offsetLeft < -50) {
                        jsWrapBg.removeChild(blocksArr[0].downDivWrap);
                        jsWrapBg.removeChild(blocksArr[0].upDivWrap);
                        blocksArr.shift(blocksArr[0]);
                }
            }
        }
```
```
当前的游戏效果
![](http://upload-images.jianshu.io/upload_images/3772457-de73cf06a6d67748.gif?imageMogr2/auto-orient/strip)
play02.gif
#### 3.3 计分器
游戏中的计分器相对较好实现
```xml
<div id="score">
                <div id="num1"></div>
                <div id="num2"></div>
                <div id="num3"></div>
            </div>
```
```java
```java
var jsScore = document.getElementById("score");
        var jsNum1 = document.getElementById("num1");
        var jsNum2 = document.getElementById("num2");
        var jsNum3 = document.getElementById("num3");
        var score = 0;
```
```
今天先这样了，改天再写。哈哈
文／Runner_Yang（简书作者）
原文链接：http://www.jianshu.com/p/45d994d04a25
著作权归作者所有，转载请联系作者获得授权，并标注“简书作者”。
