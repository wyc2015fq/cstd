
# js循环绑定事件 - wangzhiguo9261的博客 - CSDN博客


2018年03月15日 08:58:55[码里偷闲王果果](https://me.csdn.net/wangzhiguo9261)阅读数：97


首先看这段代码：
![复制代码](https://common.cnblogs.com/images/copycode.gif)
<button id="0">0</button>
<button id="1">1</button>
<button id="2">2</button>
<script>$(function(){for(vari=0; i<=2; i++) {
        $("\#" + i).on("click",function() {
            alert(i);
        });
    };
})</script>![复制代码](https://common.cnblogs.com/images/copycode.gif)

这段代码如果不仔细看的话会误以为三个按钮点击结果分别为0，1，2。但是运行结果却是3，3，3。
我们来分析一下代码执行过程：前三遍循环分别给按钮0，1，2绑定了alert(i)的事件，第四遍循环开始时i=3，不符合i<=2的条件，因此终止循环。这里要注意的是，前三遍循环绑定的是alert(i)事件，而不是alert(0)，alert(1)，alert(2)，因为在绑定的过程中on的事件处理函数里的代码并没有运行，因此在触发click事件之前并不知道i等于几，代码此时只认为i是一个全局变量（实际上i的作用域为最外层的function）。上面分析了，当循环结束时i等于3，因此3个按钮点击均为alert(3)。
但是在实际工作中我们经常会遇到这种循环绑定事件的需求，那该怎么办呢？以下是我总结的几个常用的方法。
1. 数据通过jquery的event.data与事件绑定
for(vari=0; i<=2; i++) {
    $("\#" + i).on("click", {key: i},function(event) {
        alert(event.data.key);
    });
};
2. 数据通过jquery的data方法与dom元素绑定
![复制代码](https://common.cnblogs.com/images/copycode.gif)
for(vari=0; i<=2; i++) {
    $("\#" + i).data("key", i);
    $("\#" + i).on("click",function(){
        alert($(this).data("key"));
    });
};![复制代码](https://common.cnblogs.com/images/copycode.gif)

3. 第三种方法是原生js的方法，在on函数的外层写一个立即执行的函数，其目的是生成作用域，循环三遍即生成三个作用域，i的值由最下面的参数传入，三个作用域互不影响，因此也可以实现循环绑定的效果
![复制代码](https://common.cnblogs.com/images/copycode.gif)
for(vari=0; i<=2; i++) {
    (function(i) {
        $("\#" + i).on("click",function(){
            alert(i);
        });
    })(i);
};![复制代码](https://common.cnblogs.com/images/copycode.gif)


