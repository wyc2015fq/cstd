# 小技巧 - 利用 border 实现菜单图标 - weixin_33985507的博客 - CSDN博客
2017年04月19日 20:45:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：9
实现效果如下：
![2849764-5d27deec862011a9.png](https://upload-images.jianshu.io/upload_images/2849764-5d27deec862011a9.png)
demo01.png
实现这个效果，我们可能直接使用图标，可能内嵌一个 `span`标签，也可能会使用 `::before` 和 `::after` 伪元素，不过这其实利用 `border`就可以实现，上述效果的代码如下所示：
```
div {
    width: 38px;
    height: 6px;
    border-top: 18px double #F70776;
    border-bottom: 6px solid #F70776;
  }
```
由于 `border` 的颜色是继承自 `color` 的，所以利用以上代码，我们可以很轻松的实现一个 `hover` 变色效果，改写后的代码如下：
```
div {
    width: 38px;
    height: 6px;
    border-top: 18px double;
    border-bottom: 6px solid;
    color: #F70776;
    cursor: pointer;
    transition: all 1s;
  }
  div:hover {
    color: green;
  }
```
这个技巧是在张鑫旭的教学视屏中看到的，地址如下：[CSS 深入理解之 border](https://link.jianshu.com?t=http://www.imooc.com/video/13373).
