# 双击BACK键退出程序 - DEVELOPER - CSDN博客





2014年04月02日 19:57:34[学术袁](https://me.csdn.net/u012827205)阅读数：1596标签：[android																[TabHost效果下  BACK键退出](https://so.csdn.net/so/search/s.do?q=TabHost效果下  BACK键退出&t=blog)](https://so.csdn.net/so/search/s.do?q=android&t=blog)
个人分类：[交互设计](https://blog.csdn.net/u012827205/article/category/2851663)








今天就要结项目了，所以就额外添加了一个功能——双击BACK键退出程序。我做的项目是基于TabHost的，通过页卡显示了四块内容。就跟新浪4.0版本微博客户端一样的效果。

本来我也不会的，我只是知道要重写BACK键的功能。所以，就在网上搜索了好一会儿，并使用了好几种网上的版本<可恶的是有些还要CSDN积分才能下>。更可可恨的是，不能用。没别的办法，就得读一读代码，综合一下他们。最后，“黄天不负有心人”，OK了。所以我就要拿出来和大家分享一下；

首先，应该注意的是：重写BACK键的功能方法要放在TabHost所在的类。

然后，就只剩下代码实现了，看代码：



```java
private long exitTime;
	@Override
	public boolean dispatchKeyEvent(KeyEvent event) {// back退出应用程序
		if (event.getKeyCode() == KeyEvent.KEYCODE_BACK
				&& event.getAction() == KeyEvent.ACTION_DOWN) {

			// 处理连按退出
			// System.currentTimeMillis()无论何时调用，肯定大于2000
			if ((System.currentTimeMillis() - exitTime) > 2000) 
			{
				Toast.makeText(getApplicationContext(), "再按一次,退出",
						Toast.LENGTH_SHORT).show();
				exitTime = System.currentTimeMillis();
			} else {
				finish();
				System.exit(0);
			}
			return true;
		}
		return super.dispatchKeyEvent(event);
	}
```





建一个群，方便大家交流：蹦蹦哒Android <群号：423923313>







