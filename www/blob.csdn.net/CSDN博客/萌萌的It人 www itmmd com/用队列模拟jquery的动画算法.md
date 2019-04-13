
# 用队列模拟jquery的动画算法 - 萌萌的It人 www.itmmd.com - CSDN博客


2015年01月16日 13:20:51[Jlins](https://me.csdn.net/dyllove98)阅读数：1087


Aaron最近疯狂的爱上了算法研究，估计又要死伤不少脑细胞了，我喜欢捡现成的，可以省些力气。发现他写的一段源码，运行一下，还蛮好玩的，于是拿来分析一下，一来吸收下里边的营养，二来加深一下源码学习的功力。话说这源码还真是提高js内功的一大秘决，不信，就和我一起来品味一下吧。
![复制代码](http://common.cnblogs.com/images/copycode.gif)
//立即执行函数，没有什么好说的。看下面演示/**
(function($){
    //此处的$会由后面紧跟的立即执行函数的返回值提供
})(function(){
    //这个函数运行的结果就是$啦
    return aQuery
}())*/(function($) {
    window.$ = $;
})(function() {//用来匹配ID字符串//(?:表示这里不分组) ，参考正则的内容//不过我个人认为把*改成+号会更好，因为\#后至少要一个字符吧varrquickExpr = /^(?:\#([\w-]*))$/;//一看便是jquery的重度患者functionaQuery(selector) {returnnewaQuery.fn.init(selector);
    }/**
     * 动画 
     * @return {[type]} [description]*/varanimation =function() {varself = {};varQueue = [];//动画队列varfireing =false//动画锁varfirst =true;//通过add接口触发vargetStyle =function(obj, attr) {returnobj.currentStyle ? obj.currentStyle[attr] : getComputedStyle(obj,false)[attr];
        }//这里边都是具体的动画效果，没有什么难懂的varmakeAnim =function(element, options, func) {varwidth = options.width//包装了具体的执行算法//css3//setTimeoutelement.style.webkitTransitionDuration = '2000ms';
            element.style.webkitTransform = 'translate3d(' + width + 'px,0,0)';//监听动画完结element.addEventListener('webkitTransitionEnd',function() {
                func()
            });
        }var_fire =function() {//加入动画正在触发if(!fireing) {varonceRun = Queue.shift();if(onceRun) {//防止重复触发fireing =true;//nextonceRun(function() {
                        fireing =false;//这里很巧妙的产生了连环调用的效果_fire();
                    });
                }else{
                    fireing =true;
                }
            }
        }returnself = {//增加队列add:function(element, options) {//这里是整个算法的关键//相当于往数组中添加一个函数//[function(func){},...]//也就是_fire中的onceRun方法，func也就是在那时传进去的。//在Aaron的编程中很喜欢用这种技巧，比如预编译什么的。Queue.push(function(func) {
                    makeAnim(element, options, func);
                });//如果有一个队列立刻触发动画if(first && Queue.length) {//这个开关很好的起到了控制后面添加的元素进行排队的作用first =false;//这里等价于直接运行_fire();//Aaron喜欢装A，故意添加一个self.fire出来，或许他是深谋远虑self.fire();
                }
            },//触发fire:function() {
                _fire();
            }
        }
    }();

    aQuery.fn = aQuery.prototype = {
        run:function(options) {
            animation.add(this.element, options);returnthis;
        }
    }varinit = aQuery.fn.init =function(selector) {varmatch = rquickExpr.exec(selector);varelement = document.getElementById(match[1])this.element = element;returnthis;
    }//差点小看了这一行代码//jquery的样子学的不错//直接aQuery.fn.init = aQuery.fn不是更好?//多一个init变量无非是想减少查询罢了，优化的思想无处不在。init.prototype = aQuery.fn;returnaQuery;
}());//domvaroDiv = document.getElementById('div1');//调用oDiv.onclick =function() {
    $('\#div1').run({
        'width': '500'
    }).run({
        'width': '300'
    }).run({
        'width': '1000'
    });
};![复制代码](http://common.cnblogs.com/images/copycode.gif)
附上html就可以自己调式了。要记得用chrome浏览哦。
<div id="div1" style="width:100px;height:50px;background:red;cursor:pointer;color:\#fff;text-align:center;line-height:50px;" data-mce-style="width: 100px; height: 50px; background: red; cursor: pointer; color: \#fff; text-align: center; line-height: 50px;">点击</div>
演示地址：这个就不演示了。
**其他精彩文章**
## [jQuery教程(19)-jquery ajax操作之序列化表单](http://www.itmmd.com/201501/497.html)
## [jQuery教程(18)-ajax操作之执行POST请求](http://www.itmmd.com/201501/496.html)
## [jQuery教程(20)-jquery ajax + php 操作之为Ajax请求提供不同...](http://www.itmmd.com/201501/499.html)
## [jQuery教程(21)-jquery ajax 回调函数](http://www.itmmd.com/201501/500.html)
## [jQuery教程(22)-ajax操作之错误处理](http://www.itmmd.com/201501/503.html)

## [jQuery教程(24)-ajax操作之Ajax和事件](http://www.itmmd.com/201501/504.html)

更多关于[android开发](http://www.itmmd.com/mobile.html)文章


