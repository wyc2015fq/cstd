# JQuery-跑马灯（文字无缝向上翻动）（自已封装的第三个插件） - z69183787的专栏 - CSDN博客
2013年10月18日 19:20:18[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2613
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
(function($){
    $.fn.extend({
        "slideUp":function(value){
            
            var docthis = this;
            //默认参数
            value=$.extend({
                 "li_h":"30",
                 "time":2000,
                 "movetime":1000
            },value)
            
            //向上滑动动画
            function autoani(){
                $("li:first",docthis).animate({"margin-top":-value.li_h},value.movetime,function(){
                    $(this).css("margin-top",0).appendTo(".line");
                })
            }
            
            //自动间隔时间向上滑动
            var anifun = setInterval(autoani,value.time);
            
            //悬停时停止滑动，离开时继续执行
            $(docthis).hover(function(){
                clearInterval(anifun);            //清除自动滑动动画
            },function(){
                setInterval(autoani,value.time);    //继续执行动画
            })
        }    
    })
})(jQuery)
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
主要思路：
　　滑动动画，就是改变元素的位置，要改变元素的位置有两种方法，一种改变left，top属性(相对定位和绝对定位),还有一种，就是现在这里用到的,改变margin的值。
　　上例中动画过程：
　　　　1.设置要改变margin-top的值;
　　　　2.用animate方法改变第一个LI的margin-top的值为-30（负值会向上移动）;
　　　　3.在动画完成之后，回调函数内，把当前的第一个LI的margin-top改变为"0"
　　　　4.把当前这第一个LI移动到所有LI的最后一个。（实现无缝）
