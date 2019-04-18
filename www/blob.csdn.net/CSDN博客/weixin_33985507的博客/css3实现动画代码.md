# css3实现动画代码 - weixin_33985507的博客 - CSDN博客
2017年08月29日 17:34:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：10
# 一、效果图
![6166110-67066978ed53e58f.gif](https://upload-images.jianshu.io/upload_images/6166110-67066978ed53e58f.gif)
sungif.gif
本效果由H5+CSS3实现，将雪碧图制成一个炫酷有特色的动画。该方法通常用于加载动画、H5小动画的制作，可以代替gif图的使用，并且能简单实现控制动画播放的速度、方向、循环次数等，减少美工的工作量，减少图片大小，加快加载速度。
适用浏览器：360、FireFox、Chrome、Safari、Opera、傲游、搜狗、世界之窗. 不支持IE8及以下浏览器。
# 二、开始制作
1.准备一张雪碧图，每一帧相同间隔排列。如下图：
![6166110-4c3af5bb44339e84.jpg](https://upload-images.jianshu.io/upload_images/6166110-4c3af5bb44339e84.jpg)
sungif.jpg
2.在html页面中建一个div容器，如下：
```
<div class="sungif"></div>
```
3.编写css3代码
```
.sungif {
    /*Our sprite is 4200px x 280px, then each frame is 280px x 280px*/
    width: 280px;
    height: 280px;
    background-image: url("../img/sungif.jpg");
    -webkit-animation: play 3s steps(15) infinite;
    -moz-animation: play 3s steps(15) infinite;
    -ms-animation: play 3s steps(15) infinite;
    -o-animation: play 3s steps(15) infinite;
    animation: play 3s steps(15) infinite;
}
@-webkit-keyframes play {
    from {
        background-position: 0px;
    }
    to {
        background-position: -4200px;
    }
}
@-moz-keyframes play {
    from {
        background-position: 0px;
    }
    to {
        background-position: -4200px;
    }
}
@-ms-keyframes play {
    from {
        background-position: 0px;
    }
    to {
        background-position: -4200px;
    }
}
@-o-keyframes play {
    from {
        background-position: 0px;
    }
    to {
        background-position: -4200px;
    }
}
@keyframes play {
    from {
        background-position: 0px;
    }
    to {
        background-position: -4200px;
    }
}
```
![6166110-20a9b1edeebf9260.png](https://upload-images.jianshu.io/upload_images/6166110-20a9b1edeebf9260.png)
做一个心里有光的女孩。
danhuan  创建于2017年8月29日
