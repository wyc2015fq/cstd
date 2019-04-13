
# JQuery3：动画和特效 - 尹成的技术博客 - CSDN博客

2018年11月14日 15:57:10[尹成](https://me.csdn.net/yincheng01)阅读数：35


\#jquery动画
通过animate方法可以设置元素某属性值上的动画，可以设置一个或多个属性值，动画执行完成后会执行一个函数。
`$('#div1').animate({
    width:300,
    height:300
},1000,swing,function(){
    alert('done!');
});`参数可以写成数字表达式：
`$('#div1').animate({
    width:'+=100',
    height:300
},1000,swing,function(){
    alert('done!');
});`\#jquery特殊效果
`fadeIn() 淡入
    $btn.click(function(){
        $('#div1').fadeIn(1000,'swing',function(){
            alert('done!');
        });
    });
fadeOut() 淡出
fadeToggle() 切换淡入淡出
hide() 隐藏元素
show() 显示元素
toggle() 依次展示或隐藏某个元素
slideDown() 向下展开
slideUp() 向上卷起
slideToggle() 依次展开或卷起某个元素`学院Go语言视频主页
[https://edu.csdn.net/lecturer/1928](https://edu.csdn.net/lecturer/1928)
[清华团队带你实战区块链开发](https://ke.qq.com/course/344443?tuin=3d17195d)
扫码获取海量视频及源码   QQ群：721929980
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181114143613461.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmNoZW5nMDE=,size_16,color_FFFFFF,t_70)

