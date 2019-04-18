# Zepto实现滑动核销效果 - z69183787的专栏 - CSDN博客
2017年04月01日 17:13:45[OkidoGreen](https://me.csdn.net/z69183787)阅读数：857
主要事件：touchmove touchstart touchend
0、定义常量值（滑动 横向记录差 distance，手指离开屏幕位置与开始滑动位置；刮开角度 ：angle 当距离差到达时的翻转角度，过程中为0-angle）
1、记录touchstart 的 pageX ：originX
2、绑定touchmove事情，对 实时pageX ：X ，
3、比较X-originX，同时根据常量值计算 偏转角度（角度为-），进行transform rotate的 css3 旋转。
当差值达到常量distance时，滑动页滑落（可设置top值并赋予动画效果 掉落至屏幕底部）
4、若差值未到，则在touchend事件后，重置角度为rotate 0deg
伪代码如下：
```java
var $ = require('zepto'),
    Rive = function(option){
        this.container = $(option.container);
        this.callback = option.callback;
        this.ajax = true;
        var X, reduce, distance = 150, angle= 30,_this = this;
        var body = $('body')
        body.on('touchstart',function(event){
            X = event.touches[0].pageX;
        });
        this.container.on('touchmove',function(event){
            event.preventDefault();
            var x = event.touches[0].pageX;
            reduce = x - X;
            var animateTo = - angle /distance* reduce;
            if(reduce > 0){
                _this.container.css({
                    'transform': 'rotate('+ animateTo +'deg)',
                    '-ms-transform': 'rotate('+ animateTo +'deg)',
                    '-moz-transform': 'rotate('+ animateTo +'deg)',
                    '-webkit-transform': 'rotate('+ animateTo +'deg)',
                    '-o-transform': 'rotate('+ animateTo +'deg)',
                    'transition': 'all 0.5s ease-out'
                });
                if(reduce > distance ){
                    if(_this.ajax) {
                        _this.ajax = false;
                        _this.container.css({
                            'top': '200px',
                            'opacity': '0',
                            'transition': 'all 0.7s ease-out'
                        })
                        _this.callback && _this.callback();
                    };
                }
            }
        });
        body.on('touchend', function(){
            _this.ajax = true;
            if (reduce > distance) {
                _this.container.css({
                    'transition': 'all 0 ease 0'
                });
            } else {
                _this.container.css({
                    'transform': 'rotate(0deg)',
                    '-ms-transform': 'rotate(0deg)',
                    '-moz-transform': 'rotate(0deg)',
                    '-webkit-transform': 'rotate(0deg)',
                    '-o-transform': 'rotate(0deg)',
                    'transition': 'all 0 ease 0',
                    'top': '0'
                });
            };
        });
    }, p = Rive.prototype;
    p.reset = function () {
        this.container.css({
            'transform': 'rotate(0deg)',
            '-ms-transform': 'rotate(0deg)',
            '-moz-transform': 'rotate(0deg)',
            '-webkit-transform': 'rotate(0deg)',
            '-o-transform': 'rotate(0deg)',
            'transition': 'all 0 ease 0',
            'top': '0',
            'opacity': '1'
        });
        this.ajax = true;
    };
    
module.exports = Rive;
```
