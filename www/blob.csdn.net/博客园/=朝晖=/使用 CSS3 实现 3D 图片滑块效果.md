# 使用 CSS3 实现 3D 图片滑块效果 - =朝晖= - 博客园
# [使用 CSS3 实现 3D 图片滑块效果](https://www.cnblogs.com/dhcn/p/7120749.html)
Slicebox – A fresh 3D image slider with graceful fallback
英文原文地址：http://tympanus[.NET](http://lib.csdn.net/base/dotnet)/codrops/2011/09/05/slicebox-3d-image-slider/
项目地址：https://github.com/codrops/Slicebox
demo地址：http://tympanus[.net](http://lib.csdn.net/base/dotnet)/Development/Slicebox/index.html
使用 [CSS3](http://lib.csdn.net/base/css3) 的3D变换特性，我们可以通过让元素在三维空间中变换来实现一些新奇的效果。 这篇文章分享的这款 [jQuery](http://lib.csdn.net/base/jquery) 立体图片滑块插件，利用了 3D transforms（变换）属性来实现多种不同的效果。
　　实现的基本思路是创建三维图像切片，作为三维物体的另一侧，旋转并显示下一个图像。若浏览器不支持3D变换，一个简单的滑块将作为后备方案。要调用这个插件，首先把图片放在无序列表中，然后添加 CSS 类——"sb-slider" 即可，下面是 HTML 代码示例：
```
<ul id="sb-slider" class="sb-slider">  
   
    <li>  
        <a href="#" target="_blank">  
            <img src="images/1.jpg" alt="image1"/>  
        </a>  
        <div class="sb-description">  
            <h3>Creative Lifesaver</h3>  
        </div>  
    </li>  
   
    <li>  
        <img src="images/2.jpg" alt="image2"/>  
        <div class="sb-description">  
            <h3>...</h3>  
        </div>  
    </li>  
   
    <li><!-- ... --></li>  
       
    <!-- ... -->  
       
</ul>
```
另外也还可以使用带有 class 为 “sb-description” 的DIV来为图片添加描述。如本例所示，您还可以在图像周围添加锚点。然后调用插件就可以了：
```
$('#sb-slider').slicebox();
```
Slicebox 配备了一组选项，你可以调整以实现不同类型的效果，各个选项和作用如
```
$.Slicebox.defaults = {  
    // 方向(v)ertical, (h)orizontal or (r)andom  
    orientation : 'v',  
    // 元素距离视图的距离，以像素计  
    perspective : 1200,  
    // 切片，长方体的数量  
    cuboidsCount : 5,  
    // 是否随机  
    cuboidsRandom : false,  
    // 长方体最大数量  
    maxCuboidsCount : 5,  
    disperseFactor : 0,  
    // 隐藏滑块的颜色  
    colorHiddenSides : '#',  
    sequentialFactor : 150,  
    // 动画速度  
    speed : 600,  
    // 过渡效果  
    easing : 'ease',  
    // 自动播放  
    autoplay : false,  
    // 旋转间隔  
    interval: 3,  
    // 淡入淡出速度  
    fallbackFadeSpeed : 300,  
    // 回调函数  
    onBeforeChange : function( position ) { return false; },  
    onAfterChange : function( position ) { return false; }  
};
```

