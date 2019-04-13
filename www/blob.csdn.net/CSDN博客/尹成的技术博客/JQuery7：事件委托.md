
# JQuery7：事件委托 - 尹成的技术博客 - CSDN博客

2018年11月14日 15:07:00[尹成](https://me.csdn.net/yincheng01)阅读数：72个人分类：[区块链](https://blog.csdn.net/yincheng01/article/category/7618299)



\#事件委托
事件委托就是利用冒泡的原理，把事件加到父级上，通过判断事件来源的子集，执行相应的操作，事件委托首先可以极大减少事件绑定次数，提高性能；其次可以让新加入的子元素也可以拥有相同的操作。
\#\#\#一般绑定事件的写法
`$(function(){
    $ali = $('#list li');
    $ali.click(function(event) {
        $(this).css({background:'red'});
    });
})
...
<ul id="list">
    <li>1</li>
    <li>2</li>
    <li>3</li>
    <li>4</li>
    <li>5</li>
</ul>`\#\#\#事件委托的写法
`$(function(){
    $list = $('#list');
    $list.delegate('li', 'click', function(event) {
        $(this).css({background:'red'});
    });
})
...
<ul id="list">
    <li>1</li>
    <li>2</li>
    <li>3</li>
    <li>4</li>
    <li>5</li>
</ul>`\#\#\#取消事件委托
`// ev.delegateTarge 委托对象
$(ev.delegateTarge).undelegate();
// 上面的例子可使用 $list.undelegate();`学院Go语言视频主页
[https://edu.csdn.net/lecturer/1928](https://edu.csdn.net/lecturer/1928)
[清华团队带你实战区块链开发](https://ke.qq.com/course/344443?tuin=3d17195d)
扫码获取海量视频及源码   QQ群：721929980
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181114143613461.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmNoZW5nMDE=,size_16,color_FFFFFF,t_70)

