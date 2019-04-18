# requestAnimationFrame，Web中写动画的另一种选择 - z69183787的专栏 - CSDN博客
2017年01月16日 12:05:26[OkidoGreen](https://me.csdn.net/z69183787)阅读数：852
HTML5/CSS3时代，我们要在web里做动画选择其实已经很多了:
你可以用CSS3的animattion+keyframes;
你也可以用css3的transition;
你还可以用通过在canvas上作图来实现动画，也可以借助jQuery动画相关的API方便地实现;
当然最原始的你还可以使用window.setTimout()或者window.setInterval()通过不断更新元素的状态位置等来实现动画，前提是画面的更新频率要达到每秒60次才能让肉眼看到流畅的动画效果。
现在又多了一种实现动画的方案，那就是还在草案当中的window.requestAnimationFrame()方法。
# 初识requestAnimationFrame
来看[MDN](https://developer.mozilla.org/en/docs/Web/API/window.requestAnimationFrame)上对其给出的诠释：
The window.requestAnimationFrame() method tells the browser that you wish to perform an animation and requests that the browser call a specified function to update an animation before the next repaint. The method takes
 as an argument a callback to be invoked before the repaint.
window.requestAnimationFrame() 将告知浏览器你马上要开始动画效果了，后者需要在下次动画前调用相应方法来更新画面。这个方法就是传递给window.requestAnimationFrame()的回调函数。
也可这个方法原理其实也就跟setTimeout/setInterval差不多，通过递归调用同一方法来不断更新画面以达到动起来的效果，但它优于setTimeout/setInterval的地方在于它是由浏览器专门为动画提供的API，在运行时浏览器会自动优化方法的调用，并且如果页面不是激活状态下的话，动画会自动暂停，有效节省了CPU开销。
## 基本语法
可以直接调用，也可以通过window来调用，接收一个函数作为回调，返回一个ID值，通过把这个ID值传给window.cancelAnimationFrame()可以取消该次动画。
```
requestAnimationFrame(callback)//callback为回调函数
```
## 一个简单的例子
模拟一个进度条动画，初始div宽度为1px,在step函数中将进度加1然后再更新到div宽度上，在进度达到100之前，一直重复这一过程。
为了演示方便加了一个运行按钮（看不到例子请刷新）。
```
<div id="test" style="width:1px;height:17px;background:#0f0;">0%</div>
<input type="button" value="Run" id="run"/>
```
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
window.requestAnimationFrame = window.requestAnimationFrame || window.mozRequestAnimationFrame || window.webkitRequestAnimationFrame || window.msRequestAnimationFrame;
var start = null;
var ele = document.getElementById("test");
var progress = 0;
function step(timestamp) {
    progress += 1;
    ele.style.width = progress + "%";
    ele.innerHTML=progress + "%";
    if (progress < 100) {
        requestAnimationFrame(step);
    }
}
requestAnimationFrame(step);
document.getElementById("run").addEventListener("click", function() {
    ele.style.width = "1px";
    progress = 0;
    requestAnimationFrame(step);
}, false);
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
# 浏览器支持情况
既然还是草案状态下引入的一个功能，在使用全我们就需要关心一下各浏览器对它的支持情况了。就目前来说，主流现代浏览器都对它提供了支持，请看下图：
|![](http://images.cnitblog.com/blog/431064/201402/131716057436728.png)|![](http://images.cnitblog.com/blog/431064/201402/131716525507291.png)|![](http://images.cnitblog.com/blog/431064/201402/131717024884229.png)|![](http://images.cnitblog.com/blog/431064/201402/131717310762164.png)|![](http://images.cnitblog.com/blog/431064/201402/131717469633418.png)|
|----|----|----|----|----|
|31+|26+|10+|19+|6+|
更为具体的浏览器兼容性可以[在这里看到](http://caniuse.com/)。
# Polyfill
[Polyfill](http://remysharp.com/2010/10/08/what-is-a-polyfill/)就是垫片，按发明这个词的人的原话来说，它就是一段这样的代码，让浏览器原生地支持我们期望使用的一些API。
就比如这里的requestAnimationFrame，在看到了上面的浏览器支持情况后，你就知道了比上面列出的浏览器版本老的就不支持该方法，但为了让代码能够有更好的浏览器兼容性在老机器上也能运行不报错，我们可以写一些代码让浏览器在不支持requestAnimationFrame的情况下使用window.setTimeout()，这是一种回退（fallback）到过去的方法。
这样一来，就可以通俗一点的理解polyfill了，它就是备胎。
下面是由Paul Irish及其他贡献者放在GitHub Gist上的代码片段，用于在浏览器不支持requestAnimationFrame情况下的回退，回退到使用setTmeout的情况。当然，如果你确定代码是工作在现代浏览器中，下面的代码是不必的。
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
// http://paulirish.com/2011/requestanimationframe-for-smart-animating/
// http://my.opera.com/emoller/blog/2011/12/20/requestanimationframe-for-smart-er-animating
// requestAnimationFrame polyfill by Erik Möller. fixes from Paul Irish and Tino Zijdel
// MIT license
(function() {
    var lastTime = 0;
    var vendors = ['ms', 'moz', 'webkit', 'o'];
    for (var x = 0; x < vendors.length && !window.requestAnimationFrame; ++x) {
        window.requestAnimationFrame = window[vendors[x] + 'RequestAnimationFrame'];
        window.cancelAnimationFrame = window[vendors[x] + 'CancelAnimationFrame'] || window[vendors[x] + 'CancelRequestAnimationFrame'];
    }
    if (!window.requestAnimationFrame) window.requestAnimationFrame = function(callback, element) {
        var currTime = new Date().getTime();
        var timeToCall = Math.max(0, 16 - (currTime - lastTime));
        var id = window.setTimeout(function() {
            callback(currTime + timeToCall);
        }, timeToCall);
        lastTime = currTime + timeToCall;
        return id;
    };
    if (!window.cancelAnimationFrame) window.cancelAnimationFrame = function(id) {
        clearTimeout(id);
    };
}());
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
上面代码作用有二，一是把各浏览器前缀进行统一，二是在浏览器没有requestAnimationFrame方法时将其指向setTimeout方法。
提到备胎代码呢，这里多说一句，在CSS代码中，我们也经常使用这种回退的技巧，即对同一条CSS规则，编写多条以不同浏览器前缀开头代码，或者编写一条备用样式。
下面是一个CSS中的备胎代码的例子：
```
div {
    background: rgb(0, 0, 0); /* fallback */
    background: rgba(0, 0, 0, 0.5);
}
```
代码中设置div背景为黑色带50%的透明度，但IE9-的浏览器是不支持rbga格式的颜色的，所以浏览器会回退到上一条CSS规则应用rgb颜色。
Reference:
1. article about rAF from css tricks: [http://css-tricks.com/using-requestanimationframe/](http://css-tricks.com/using-requestanimationframe/)
2. article about rAF from Paul Irish:[http://www.paulirish.com/2011/requestanimationframe-for-smart-animating/](http://www.paulirish.com/2011/requestanimationframe-for-smart-animating/)
3. what is polyfill [http://remysharp.com/2010/10/08/what-is-a-polyfill/](http://remysharp.com/2010/10/08/what-is-a-polyfill/)
