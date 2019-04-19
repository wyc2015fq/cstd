# android屏幕单位----真机与模拟器获取同一图片显示大小不一样 - xqhrs232的专栏 - CSDN博客
2012年04月05日 18:04:27[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1350
原文地址::[http://blog.csdn.net/danielinbiti/article/details/7310696](http://blog.csdn.net/danielinbiti/article/details/7310696)
这几天做个游戏，设想着场景，不想等分屏幕，最终想到屏幕分辨率的影响，于是找了一下资料，转载同享
转载如下：
==========================================================
px：是屏幕的像素点
in：英寸
mm：毫米
pt：磅，1/72 英寸
dp：一个基于density的抽象单位，如果一个160dpi的屏幕，1dp=1px
dip：等同于dp
sp：同dp相似，但还会根据用户的字体大小偏好来缩放。
建议使用sp作为文本的单位，其它用dip
然后是我自己的理解:
Android的屏幕密度是以160为基准的,
屏幕密度(densityDpi)为160时, 是将一英寸分为160份, 每一份是1像素. 如果屏幕密度(densityDpi)为240时, 是将一英寸分为240份, 每一份是1像素. 1英寸/160(机器x)  =  1英寸/240(机器y)  =  1px
打个比方, 一个三英寸的显示屏幕的机器, 如果屏幕密度(densityDpi)为160, 即密度比(density)1.0时, 画一条160dip和160px的线条, 两个都是1英寸. 可如果还是三英寸的屏幕,如果屏幕密度(densityDpi)变为320, 即密度比(density)2.0时,1英寸有320像素了, 此时160px显示就是半英寸. 160dip显示还是1英寸, 因为 1dip = 1px * density .
可是你有没有注意到, 同样宽的屏幕, 比如3英寸, 如果density 是1.0的话, 分辨率就是480dip*x或480px,*x 如果还是3英寸的, density 是1.5的话 , 分辨率就是480*1.5 = 720px*x或480dip*x. (这里的x代表屏幕的高度). 如果同样是240dip, 在两款机器上显示都是一半的长度, 这样就保证了比例. 这样的话如果是480分辨率和720分辨率的话, 可以采用同一个布局文件了.
又但可是, 我见过的机器只有320*240(0.75) , 480*320(1.0) , 800*480(1.5), 854*480(1.5) ,如果是480 和 800 两个版本, 同一条线480dip , 480显示全屏, 800则显示 480 / (800 / 1.5) 屏 . 其中800/1.5是屏幕的总dip . 我们平时说的分辨率都是以像素px为单位的.  
由此可见, 就算是用dip, 也不能保证不同分辨率的机器的布局比例完全一样.  但还是有好处的. 因为如果是用px的话, 有些时候一些布局直接跑到了屏幕外边, 调试起来很麻烦. 
还有, google代码里边所用的单位都是以px为默认单位的. 
apk的资源包中，当屏幕density=240时使用hdpi标签的资源
当屏幕density=160时，使用mdpi标签的资源
当屏幕density=120时，使用ldpi标签的资源。
在每英寸160点的显示器上，1dp = 1px。
下面是几种不同单位的相互转换.
public static int dip2px(Context context, float dipValue){ 
final float scale = context.getResources().getDisplayMetrics().density; 
return (int)(dipValue * scale + 0.5f); 
} 
public static int px2dip(Context context, float pxValue){ 
final float scale = context.getResource().getDisplayMetrics().density; 
return (int)(pxValue / scale + 0.5f); 
} 
public static int dip2px(Context context, float dipValue){ 
final float scale = context.getResources().getDisplayMetrics().density; 
return (int)(dipValue * scale + 0.5f); 
} 
public static int px2dip(Context context, float pxValue){ 
final float scale = context.getResource().getDisplayMetrics().density; 
return (int)(pxValue / scale + 0.5f); 
} 
下面说下如何获取分辨率:
    在一个Activity的onCreate方法中，写入如下代码：
        DisplayMetrics metric = new DisplayMetrics();
        getWindowManager().getDefaultDisplay().getMetrics(metric);
        int width = metric.widthPixels;  // 屏幕宽度（像素）
        int height = metric.heightPixels;  // 屏幕高度（像素）
        float density = metric.density;  // 屏幕密度（0.75 / 1.0 / 1.5）
        int densityDpi = metric.densityDpi;  // 屏幕密度DPI（120 / 160 / 240）
这还是挺简单的, 可是你有没有在800*480的机器上试过, 是不是得到的宽度是533 ? 因为android刚开始时默认的density是1.0 , 此时你可以再manifest.xml中加入
1.uses-sdk节点, <uses-sdk android:minSdkVersion="4" /> , 表示不sdk1.6以下的机器不能安装你的apk了.
2.supports-screens 节点. 
   <supports-screens
            android:smallScreens="true"
            android:normalScreens="true"
            android:largeScreens="true"
            android:resizeable="true"
            android:anyDensity="true" />
