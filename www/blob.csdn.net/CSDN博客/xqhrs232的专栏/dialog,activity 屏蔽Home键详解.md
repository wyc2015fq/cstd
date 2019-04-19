# dialog,activity 屏蔽Home键详解 - xqhrs232的专栏 - CSDN博客
2011年11月15日 17:28:10[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：869标签：[dialog																[java																[android																[frameworks																[types																[null](https://so.csdn.net/so/search/s.do?q=null&t=blog)](https://so.csdn.net/so/search/s.do?q=types&t=blog)](https://so.csdn.net/so/search/s.do?q=frameworks&t=blog)](https://so.csdn.net/so/search/s.do?q=android&t=blog)](https://so.csdn.net/so/search/s.do?q=java&t=blog)](https://so.csdn.net/so/search/s.do?q=dialog&t=blog)
个人分类：[Android/Java](https://blog.csdn.net/xqhrs232/article/category/906925)
原文地址::[http://blog.csdn.net/z516518/article/details/6893170](http://blog.csdn.net/z516518/article/details/6893170)
### 相关网页::android如何屏蔽home back power按键----[http://blog.csdn.net/yp120yp/article/details/6954492](http://blog.csdn.net/yp120yp/article/details/6954492)
相信在Android应用上，很多时候逻辑是需要屏蔽Home键的，但这个用户体验是否需要，就看各位的需求了。
    一般的方法屏蔽Home键，大家一定看过不少文章了。我总结一下，先说一下一般情况下Activity的屏蔽按键和Home键吧。
**屏蔽其他键，重写onKeyDown**
Java代码  ![收藏代码](http://www.iteye.com/images/icon_star.png)
- @Override
- publicboolean onKeyDown(int keyCode, KeyEvent event) {  
-     Log.i(TAG,"keycode="+keyCode + "   isBan="+isBan);  
- switch (keyCode) {  
- case KeyEvent.KEYCODE_BACK:  
-         Log.i(TAG,"KEYCODE_BACK");  
- returntrue;  
-     }  
- returnsuper.onKeyDown(keyCode, event);  
- }  
大家会发现，这里屏蔽Home键是捕捉不到的，因为大家的权限一般是User所以是无效的。
而其实android处理Home键等系统级按键是有一定的处理的。
引用
看看源码是怎样处理的 \frameworks\policies\base\phone\com\android\internal\policy\impl\PhoneWindowManager.java #1092
Java代码  ![收藏代码](http://www.iteye.com/images/icon_star.png)
- // First we always handle the home key here, so applications
- // can never break it, although if keyguard is on, we do let
- // it handle it, because that gives us the correct 5 second
- // timeout.
- if (code == KeyEvent.KEYCODE_HOME) {  
- 
- // If a system window has focus, then it doesn't make sense
- // right now to interact with applications.
-     WindowManager.LayoutParams attrs = win != null ? win.getAttrs() : null;  
- if (attrs != null) {  
- finalint type = attrs.type;  
- if (type == WindowManager.LayoutParams.TYPE_KEYGUARD  
-            || type == WindowManager.LayoutParams.TYPE_KEYGUARD_DIALOG) {  
- // the "app" is keyguard, so give it the key
- returnfalse;  
-         }  
- finalint typeCount = WINDOW_TYPES_WHERE_HOME_DOESNT_WORK.length;  
- for (int i=0; i<typeCount; i++) {  
- if (type == WINDOW_TYPES_WHERE_HOME_DOESNT_WORK[i]) {  
- // don't do anything, but also don't pass it to the app
- returntrue;  
-             }  
-         }  
-     }  
通过源码，我们不难发现两个的参数 WindowManager.LayoutParams.TYPE_KEYGUARD和
WindowManager.LayoutParams.TYPE_KEYGUARD_DIALOG
**借鉴于此，重写onAttachedToWindow，以实现屏蔽Home键**
Java代码  ![收藏代码](http://www.iteye.com/images/icon_star.png)
- publicvoid onAttachedToWindow() {  
- this.getWindow().setType(WindowManager.LayoutParams.TYPE_KEYGUARD);  
- super.onAttachedToWindow();  
- }  
- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 华丽的分界线，以下内容更精彩- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
轮到dialog了，如果在Activity弹出dialog，在Activity设置以上2个方法是没办法屏蔽的。
其实，原理是一样的，只是地方不一样而已。
Java代码  ![收藏代码](http://www.iteye.com/images/icon_star.png)
- final Dialog dialog = new Dialog(this);  
- dialog.setContentView(R.layout.mydailog);  
- dialog.getWindow().setType(WindowManager.LayoutParams.TYPE_KEYGUARD);  
- dialog.show();  
- 
- dialog.setOnKeyListener(new android.content.DialogInterface.OnKeyListener(){  
- @Override
- publicboolean onKey(DialogInterface dialog, int keyCode,KeyEvent event) {  
- switch (keyCode) {  
- case KeyEvent.KEYCODE_BACK:  
-             Log.i(TAG,"KEYCODE_BACK");  
- returntrue;  
-         }  
- returnfalse;  
-     }  
- });   
这样运行后，出错如下：
Error代码  ![收藏代码](http://www.iteye.com/images/icon_star.png)
- 10-1813:27:06.380: ERROR/AndroidRuntime(4684): Caused by: android.view.WindowManager$BadTokenException: Unable to add window android.view.ViewRoot$W@2b046d68 -- permission denied for this window type  
其实，只需要把dialog.getWindow().setType的位置放在show后面就可以了
正确代码  ![收藏代码](http://www.iteye.com/images/icon_star.png)
- dialog.show();  
- dialog.getWindow().setType(WindowManager.LayoutParams.TYPE_KEYGUARD);  
这么，就完成了Back键的屏蔽 和Home键盘的屏蔽了！
总结：
1:)在以上用WindowManager.LayoutParams.TYPE_KEYGUARD的地方改用
WindowManager.LayoutParams.TYPE_KEYGUARD_DIALOG 效果一样。至于两者的具体差别，得以后再研究研究。
2:)其实，在源码里是这样调用的。 
Java代码  ![收藏代码](http://www.iteye.com/images/icon_star.png)
- final AlertDialog dialog = new AlertDialog.Builder(mContext)  
-     .setTitle(null)  
-     .setMessage(message)  
-     .setNeutralButton(R.string.ok, null)  
-     .create();  
- dialog.getWindow().setType(WindowManager.LayoutParams.TYPE_KEYGUARD_DIALOG);  
- dialog.show();  
    但我们如果这样调用就会出现之前的那个error：permission denied for this window type 这就显而易见了吧~~
3:)ProgressDialog 默认屏蔽 Back键，Dialog，AlertDialog则需setOnKeyListener
4:)其实屏蔽Home键，在页面的某个地方，例如一个Button的onClick里，去设置setType就可以了,如：
Java代码  ![收藏代码](http://www.iteye.com/images/icon_star.png)
- button.setOnClickListener(new View.OnClickListener() {  
- @Override
- publicvoid onClick(View v) {  
-         getWindow().setType(WindowManager.LayoutParams.TYPE_KEYGUARD);  
-     }  
- });  
但前提是重载Activity的onAttachedToWindow()，哪怕只是一个空实现，然后返回父类方法。
Java代码  ![收藏代码](http://www.iteye.com/images/icon_star.png)
- @Override
- publicvoid onAttachedToWindow() {  
- super.onAttachedToWindow();  
- }  
5:)其实它们，都是常用的~ 
Java代码  
- switch (keyCode) {  
- case KeyEvent.KEYCODE_HOME:  
-         Log.i(TAG,"KEYCODE_HOME");  
- returntrue;  
- case KeyEvent.KEYCODE_BACK:  
-         Log.i(TAG,"KEYCODE_BACK");  
- returntrue;  
- case KeyEvent.KEYCODE_CALL:  
-         Log.i(TAG,"KEYCODE_CALL");  
- returntrue;  
- case KeyEvent.KEYCODE_SYM:  
-         Log.i(TAG,"KEYCODE_SYM");  
- returntrue;  
- case KeyEvent.KEYCODE_VOLUME_DOWN:  
-         Log.i(TAG,"KEYCODE_VOLUME_DOWN");  
- returntrue;  
- case KeyEvent.KEYCODE_VOLUME_UP:  
-         Log.i(TAG,"KEYCODE_VOLUME_UP");  
- returntrue;  
- case KeyEvent.KEYCODE_STAR:  
-         Log.i(TAG,"KEYCODE_STAR");  
- returntrue;  
- }  

