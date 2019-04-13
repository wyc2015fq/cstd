
# android 开发实现悬浮窗体 - 萌萌的It人 www.itmmd.com - CSDN博客


2015年02月09日 16:59:45[Jlins](https://me.csdn.net/dyllove98)阅读数：2449


突然对悬浮窗体感兴趣，查资料做了个小Demo，效果是点击按钮后，关闭当前Activity，显示悬浮窗口，窗口可以拖动，双击后消失。效果图如下：
![](http://images.cnitblog.com/blog/359646/201502/091217441838541.gif)
它的使用原理很简单，就是借用了WindowManager这个管理类来实现的。
1.首先在AndroidManifest.xml中添加使用权限：
<uses-permissionandroid:name="android.permission.SYSTEM_ALERT_WINDOW"/>
2.悬浮窗口布局实现
![复制代码](http://common.cnblogs.com/images/copycode.gif)
publicclassDesktopLayoutextendsLinearLayout {publicDesktopLayout(Context context) {super(context);
        setOrientation(LinearLayout.VERTICAL);//水平排列//设置宽高this.setLayoutParams(newLayoutParams(LayoutParams.WRAP_CONTENT,
                LayoutParams.WRAP_CONTENT));
        
        View view = LayoutInflater.from(context).inflate(  
                R.layout.desklayout,null);this.addView(view);
    }![复制代码](http://common.cnblogs.com/images/copycode.gif)
3.在activity中让它显示出来。
![复制代码](http://common.cnblogs.com/images/copycode.gif)
//取得系统窗体mWindowManager = (WindowManager) getApplicationContext()
                .getSystemService("window");//窗体的布局样式mLayout =newWindowManager.LayoutParams();//设置窗体显示类型——TYPE_SYSTEM_ALERT(系统提示)mLayout.type = WindowManager.LayoutParams.TYPE_SYSTEM_ALERT;//设置窗体焦点及触摸：//FLAG_NOT_FOCUSABLE(不能获得按键输入焦点)mLayout.flags = WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE;//设置显示的模式mLayout.format = PixelFormat.RGBA_8888;//设置对齐的方法mLayout.gravity = Gravity.TOP | Gravity.LEFT;//设置窗体宽度和高度mLayout.width = WindowManager.LayoutParams.WRAP_CONTENT;
        mLayout.height = WindowManager.LayoutParams.WRAP_CONTENT;![复制代码](http://common.cnblogs.com/images/copycode.gif)
详细 MainActivity 代码如下：
![](http://images.cnblogs.com/OutliningIndicators/ContractedBlock.gif)![](http://images.cnblogs.com/OutliningIndicators/ExpandedBlockStart.gif)![复制代码](http://common.cnblogs.com/images/copycode.gif)
packagecom.yc.yc_suspendingform;importandroid.app.Activity;importandroid.graphics.PixelFormat;importandroid.graphics.Rect;importandroid.os.Bundle;importandroid.util.Log;importandroid.view.Gravity;importandroid.view.MotionEvent;importandroid.view.View;importandroid.view.View.OnClickListener;importandroid.view.View.OnTouchListener;importandroid.view.WindowManager;importandroid.widget.Button;importcom.yc.yc_floatingform.R;publicclassMainActivityextendsActivity {privateWindowManager mWindowManager;privateWindowManager.LayoutParams mLayout;privateDesktopLayout mDesktopLayout;privatelongstartTime;//声明屏幕的宽高floatx, y;inttop;
    @OverrideprotectedvoidonCreate(Bundle savedInstanceState) {super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);        
        createWindowManager();
        createDesktopLayout();
        Button btn = (Button) findViewById(R.id.btn);
        btn.setOnClickListener(newOnClickListener() {publicvoidonClick(View v) {
                showDesk();
            }
        });
    }/*** 创建悬浮窗体*/privatevoidcreateDesktopLayout() {
        mDesktopLayout =newDesktopLayout(this);
        mDesktopLayout.setOnTouchListener(newOnTouchListener() {floatmTouchStartX;floatmTouchStartY;
            @OverridepublicbooleanonTouch(View v, MotionEvent event) {//获取相对屏幕的坐标，即以屏幕左上角为原点x = event.getRawX();
                y = event.getRawY() - top;//25是系统状态栏的高度Log.i("startP", "startX" + mTouchStartX + "====startY"
                        + mTouchStartY);switch(event.getAction()) {caseMotionEvent.ACTION_DOWN://获取相对View的坐标，即以此View左上角为原点mTouchStartX = event.getX();
                    mTouchStartY = event.getY();
                    Log.i("startP", "startX" + mTouchStartX + "====startY"
                            + mTouchStartY);longend = System.currentTimeMillis() - startTime;//双击的间隔在 300ms以下if(end < 300) {
                        closeDesk();
                    }
                    startTime = System.currentTimeMillis();break;caseMotionEvent.ACTION_MOVE://更新浮动窗口位置参数mLayout.x = (int) (x - mTouchStartX);
                    mLayout.y = (int) (y - mTouchStartY);
                    mWindowManager.updateViewLayout(v, mLayout);break;caseMotionEvent.ACTION_UP://更新浮动窗口位置参数mLayout.x = (int) (x - mTouchStartX);
                    mLayout.y = (int) (y - mTouchStartY);
                    mWindowManager.updateViewLayout(v, mLayout);//可以在此记录最后一次的位置mTouchStartX = mTouchStartY = 0;break;
                }returntrue;
            }
        });
    }
    @OverridepublicvoidonWindowFocusChanged(booleanhasFocus) {super.onWindowFocusChanged(hasFocus);
        Rect rect =newRect();///取得整个视图部分,注意，如果你要设置标题样式，这个必须出现在标题样式之后，否则会出错getWindow().getDecorView().getWindowVisibleDisplayFrame(rect);
        top = rect.top;//状态栏的高度，所以rect.height,rect.width分别是系统的高度的宽度Log.i("top",""+top);
    }/*** 显示DesktopLayout*/privatevoidshowDesk() {
        mWindowManager.addView(mDesktopLayout, mLayout);
        finish();
    }/*** 关闭DesktopLayout*/privatevoidcloseDesk() {
        mWindowManager.removeView(mDesktopLayout);
        finish();
    }/*** 设置WindowManager*/privatevoidcreateWindowManager() {//取得系统窗体mWindowManager = (WindowManager) getApplicationContext()
                .getSystemService("window");//窗体的布局样式mLayout =newWindowManager.LayoutParams();//设置窗体显示类型——TYPE_SYSTEM_ALERT(系统提示)mLayout.type = WindowManager.LayoutParams.TYPE_SYSTEM_ALERT;//设置窗体焦点及触摸：//FLAG_NOT_FOCUSABLE(不能获得按键输入焦点)mLayout.flags = WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE;//设置显示的模式mLayout.format = PixelFormat.RGBA_8888;//设置对齐的方法mLayout.gravity = Gravity.TOP | Gravity.LEFT;//设置窗体宽度和高度mLayout.width = WindowManager.LayoutParams.WRAP_CONTENT;
        mLayout.height = WindowManager.LayoutParams.WRAP_CONTENT;
    }
}![复制代码](http://common.cnblogs.com/images/copycode.gif)
View Code
源代码地址：https://github.com/YeXiaoChao/Yc_suspendingform

参考资料：


## android学习笔记（60）使用Bundle在Activity之间文换教接
## android学习笔记（59） android开发启动和关闭activity

## [android学习笔记（58）Activity学习过程](http://www.itmmd.com/201502/597.html)
## 更多
## [android开发](http://www.itmmd.com/)
## 教程



