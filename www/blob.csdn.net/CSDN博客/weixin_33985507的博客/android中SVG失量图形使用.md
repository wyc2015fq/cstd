# android中SVG失量图形使用 - weixin_33985507的博客 - CSDN博客
2017年04月08日 22:45:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：10
偶然间 看到一个类库 关于svg 动画，可以下载demo 运行看源码哦
- 
GitHub最详细的SvgDemo：
[https://github.com/jrummyapps/AnimatedSvgView](https://link.jianshu.com?t=https://github.com/jrummyapps/AnimatedSvgView)
- 
首先 大家还是要知道 该去哪里下载svg图 这点阿里做的很
矢量图片库---
[http://iconfont.cn/collections](https://link.jianshu.com?t=http://iconfont.cn/collections)
阿里矢量图形库---
[http://iconfont.cn/plus/collections/detail?cid=3057](https://link.jianshu.com?t=http://iconfont.cn/plus/collections/detail?cid=3057)
[](https://link.jianshu.com?t=http://iconfont.cn/plus/collections/detail?cid=3057)
那接下来 我们 说一下使用的步骤
- 
![2516867-4a89a5a0cb02209f.png](https://upload-images.jianshu.io/upload_images/2516867-4a89a5a0cb02209f.png)
下载一个svg文件.png
- ![导入svg
- 
![2516867-63307fefaab6de7e.png](https://upload-images.jianshu.io/upload_images/2516867-63307fefaab6de7e.png)
点击ok.png
.png]([http://upload-images.jianshu.io/upload_images/2516867-88953d08121f085f.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240](https://link.jianshu.com?t=http://upload-images.jianshu.io/upload_images/2516867-88953d08121f085f.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240))
- 
![2516867-b35ae0c142519b25.png](https://upload-images.jianshu.io/upload_images/2516867-b35ae0c142519b25.png)
这就是生成的xml文件.png
根据上图的方式可以获得drawable类型的xml文件，里面有path，color，height，weigh
*接下来要说的就是正儿八经的使用方法了you
```
<com.jrummyapps.android.widget.AnimatedSvgView
        android:id="@+id/animated_svg_view"
        android:layout_width="180dp"
        android:layout_height="180dp"
        android:layout_gravity="center"
        android:layout_marginBottom="25dp"
        app:animatedSvgFillColors="@array/didi_glyph_colors"
        app:animatedSvgFillStart="1200"
        app:animatedSvgFillTime="1000"
        app:animatedSvgGlyphStrings="@array/didi_glyph_strings"
        app:animatedSvgImageSizeX="1792"
        app:animatedSvgImageSizeY="1792"
        app:animatedSvgTraceMarkerLength="50"
        app:animatedSvgTraceTime="2000"
        app:animatedSvgTraceTimePerGlyph="1000" />
```
我们会看到这比较显眼的array引用  大家有会想array文件是哪里冒出来的呢哈哈 自己写的呗
- 这里的string array 里的item 其实对应的就是生成xml文件里的pathdata
同样 integer array 对应的就是fillcolor 说到这里 真相大白了。至于其他的属性 我再继续研究一下 安喽
![2516867-338dfce73eed07b7.png](https://upload-images.jianshu.io/upload_images/2516867-338dfce73eed07b7.png)
3F5EB311-4BC9-495D-87D7-49981284C474.png
