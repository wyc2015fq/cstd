# Android activity 阻止屏幕休眠 - xqhrs232的专栏 - CSDN博客
2011年07月20日 10:48:23[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1195
原文地址::[http://blog.csdn.net/ultrapro/article/details/6618918](http://blog.csdn.net/ultrapro/article/details/6618918)
在Activity 的onCreate 方法中添加 getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
当这个窗口处于前台时,屏幕不会进入休眠状态.
@Override
    protected void onCreate(Bundle icicle) {
        super.onCreate(icicle);
        getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
    }
