# Android 中像素px和dp的转化 - weixin_33985507的博客 - CSDN博客
2014年03月25日 21:34:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：4
在Android的布局文件中，往往使用dp作为控件的宽度和高度尺寸，但是在[Java](http://www.2cto.com/kf/ware/Java/)代码中，调用getWidth()方法获得的尺寸单位
却是像素px,这两个单位有明显的区别：dp和屏幕的密度有关，而px与屏幕密度无关，所以使用时经常会涉及到两
者之间的互相转化，代码示例如下：
public int Dp2Px(Context context, float dp) { 
    final float scale = context.getResources().getDisplayMetrics().density; 
    return (int) (dp * scale + 0.5f); 
} 
public int Px2Dp(Context context, float px) { 
    final float scale = context.getResources().getDisplayMetrics().density; 
    return (int) (px / scale + 0.5f); 
} 
