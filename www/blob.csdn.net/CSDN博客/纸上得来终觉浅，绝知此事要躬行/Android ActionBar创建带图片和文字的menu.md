# Android ActionBar创建带图片和文字的menu - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2015年01月30日 15:35:50[boonya](https://me.csdn.net/boonya)阅读数：8674








        因为才开始使用到ActionBar功能，所以就很随意的做了一个效果（隐藏item只能显示文字而不能显示图标）；  使用的NavigationDrawerFragment实现的UI框架。         需求是：设置菜单的图标并且带文字。

#### 设置ActionBarMenu菜单style：纯文字方式

![](https://img-blog.csdn.net/20150130152700271?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYm9vbnlh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


文件名称为main.xml




```
<menu xmlns:android="http://schemas.android.com/apk/res/android"
    xmlns:app="http://schemas.android.com/apk/res-auto"
    xmlns:tools="http://schemas.android.com/tools"
    tools:context="com.scengine.escheduler.MainActivity" >

    <item
        android:id="@+id/action_search"
        android:title="@string/action_search"
        android:icon="@drawable/ic_action_search"
        app:showAsAction="always"/>
     <item
        android:id="@+id/action_firstpage"
        android:title="@string/action_firstpage"
        app:showAsAction="never"/>
    <item
        android:id="@+id/action_publish"
        android:title="@string/action_publish"
        app:showAsAction="never"/>
    <item
        android:id="@+id/action_draft"
        android:title="@string/action_draft"
        app:showAsAction="never"/>
     <item
        android:id="@+id/action_settings"
        android:orderInCategory="100"
        android:title="@string/action_settings"
        app:showAsAction="never"/>
    <item
        android:id="@+id/action_notify"
        android:title="@string/action_notify"
        android:icon="@drawable/ic_action_notify"
        app:showAsAction="never"/>
    <item
        android:id="@+id/action_logout"
        android:orderInCategory="100"
        android:title="@string/action_logout"
        app:showAsAction="never"/>


</menu>
```
框架中调用：





```java
@Override
public boolean onCreateOptionsMenu(Menu menu)
{
	if (!mNavigationDrawerFragment.isDrawerOpen())
	{
		getMenuInflater().inflate(R.menu.main, menu);
		restoreActionBar();
		return true;
	}
        return super.onCreateOptionsMenu(menu);
}
```



#### 设置ActionBarMenu菜单style：文字加图片

![](https://img-blog.csdn.net/20150130160013717?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYm9vbnlh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


只需要修改onCreateOptionsMenu方法就可以实现：



```java
@Override
public boolean onCreateOptionsMenu(Menu menu)
{

	SubMenu subMenu = menu.addSubMenu("");

	subMenu.add("探索").setIcon(R.drawable.ic_action_search).setOnMenuItemClickListener(new OnMenuItemClickListener()
	{

		@Override
		public boolean onMenuItemClick(MenuItem item)
		{
			return false;
		}
	});
	subMenu.add("消息").setIcon(R.drawable.ic_action_notify).setOnMenuItemClickListener(new OnMenuItemClickListener()
	{

		@Override
		public boolean onMenuItemClick(MenuItem item)
		{
				return false;
		}
	});

	MenuItem item = subMenu.getItem();
	item.setIcon(R.drawable.ic_menu);
	item.setShowAsAction(MenuItem.SHOW_AS_ACTION_ALWAYS | MenuItem.SHOW_AS_ACTION_WITH_TEXT);
        return super.onCreateOptionsMenu(menu);
}
```
参考文章地址：[http://stackoverflow.com/questions/16709083/how-to-make-popup-menu-like-this-using-actionbarsherlock](http://stackoverflow.com/questions/16709083/how-to-make-popup-menu-like-this-using-actionbarsherlock)









