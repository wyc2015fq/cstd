# 错误总结之播放器（vitamio）音量实体键与触摸手势控制，音量调节冲突 - DEVELOPER - CSDN博客





2014年08月18日 15:21:14[学术袁](https://me.csdn.net/u012827205)阅读数：1958








这个可是独家心得；经过几天的网上资料查询未果，在群里遇到一同行。然后让他帮我看了看，最终解决了该冲突。此时，谨以此来感谢那位同僚的热情帮助；

说说这个问题吧：

目前我在做一款影视方面的项目，在该项目中肯定是要用到视频播放这一块，视频播放是引用了vitamio类库，播放页面使用的是SurfaceView控件。然后在视频播放的期间，有这些要求：手势滑动调节音量。当我费劲千辛万苦搞定之后，没有想到却遇到了按音量实体键的时候播放页面退出的冲突问题。那么怎么解决该问题呢？很简单，几行代码的事情。

返回键监听这里改一下，就好了：

```java
@Override
	public boolean onKeyDown(int keyCode, KeyEvent event) {
		// TODO Auto-generated method stub
		if (keyCode == KeyEvent.KEYCODE_BACK && event.getRepeatCount() == 0) {// 当单击键盘的返回键时
			if (event.getAction() == KeyEvent.ACTION_DOWN) {
				......
                                       ...... 
                             return true;
			}
		}
```


至于原因，我不是很清楚。我猜想：此时音量键应该被充当返回键的功能了，而上面是消除这种冲突。






