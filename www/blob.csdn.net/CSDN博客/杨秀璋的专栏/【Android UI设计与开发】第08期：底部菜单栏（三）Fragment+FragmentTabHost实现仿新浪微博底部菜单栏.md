
# 【Android UI设计与开发】第08期：底部菜单栏（三）Fragment+FragmentTabHost实现仿新浪微博底部菜单栏 - 杨秀璋的专栏 - CSDN博客

2014年11月04日 00:28:14[Eastmount](https://me.csdn.net/Eastmount)阅读数：2528标签：[android																](https://so.csdn.net/so/search/s.do?q=android&t=blog)[碎片																](https://so.csdn.net/so/search/s.do?q=碎片&t=blog)[Fragment																](https://so.csdn.net/so/search/s.do?q=Fragment&t=blog)[FragmentTabHost																](https://so.csdn.net/so/search/s.do?q=FragmentTabHost&t=blog)[底部菜单栏																](https://so.csdn.net/so/search/s.do?q=底部菜单栏&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=FragmentTabHost&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=Fragment&t=blog)个人分类：[Android																](https://blog.csdn.net/Eastmount/article/category/2645447)
[
																								](https://so.csdn.net/so/search/s.do?q=Fragment&t=blog)
[
				](https://so.csdn.net/so/search/s.do?q=碎片&t=blog)
[
			](https://so.csdn.net/so/search/s.do?q=碎片&t=blog)
[
		](https://so.csdn.net/so/search/s.do?q=android&t=blog)
PS:这是转载CSDN作者[yangyu20121224](http://my.csdn.net/yangyu20121224)的文章,真的非常不错.它主要讲述如何通过Fragment碎片实现点击TabHost切换页面.可惜作者很长时间没有更新了,他写的那系列文章都很实用.以后在做Android布局时会用到该篇文章内容,所以转载作为在线笔记.希望对大家也有所帮助吧!
转载源地址：http://blog.csdn.net/yangyu20121224/article/details/9016223
在上一篇文章中，我们花了大量的篇幅来讲解Fragment这个新引进类的使用，目的就是为了让大家能够牢牢的掌握它的使用方法，以便读者在今后的开发中能够熟练的使用它。
**一、实现效果图**
![](https://img-blog.csdn.net/20130603204331671)![](https://img-blog.csdn.net/20130603204337781)
**二、项目工程结构**
![](https://img-blog.csdn.net/20130603205005046)
![](https://img-blog.csdn.net/20130603205011437)
**三、详细代码编写**
1、主tab布局界面，main_tab_layout：
```python
<?xml version="1.0" encoding="utf-8"?>
<LinearLayout xmlns:android="http://schemas.android.com/apk/res/android"
    android:layout_width="fill_parent"
    android:layout_height="fill_parent"
    android:orientation="vertical" >
 
    <FrameLayout
        android:id="@+id/realtabcontent"
        android:layout_width="fill_parent"
        android:layout_height="0dip"
        android:layout_weight="1" />
 
    <android.support.v4.app.FragmentTabHost
        android:id="@android:id/tabhost"
        android:layout_width="fill_parent"
        android:layout_height="wrap_content" 
        android:background="@drawable/maintab_toolbar_bg">
 
        <FrameLayout
            android:id="@android:id/tabcontent"
            android:layout_width="0dp"
            android:layout_height="0dp"
            android:layout_weight="0" />            
    </android.support.v4.app.FragmentTabHost>
 
</LinearLayout>
```
2、Tab按钮选项布局，tab_item_view.xml：
```python
<?xml version="1.0" encoding="utf-8"?>
<LinearLayout xmlns:android="http://schemas.android.com/apk/res/android"
    android:layout_width="wrap_content"
    android:layout_height="wrap_content"
    android:gravity="center"
    android:orientation="vertical" >
 
    <ImageView
        android:id="@+id/imageview"
        android:layout_width="wrap_content"
        android:layout_height="wrap_content"
        android:focusable="false"
        android:padding="3dp" 
        android:src="@drawable/tab_home_btn">
    </ImageView>
 
    <TextView
        android:id="@+id/textview"       
        android:layout_width="wrap_content"
        android:layout_height="wrap_content" 
        android:text="首页"
        android:textSize="10sp"
        android:textColor="#ffffff">
    </TextView>
 
</LinearLayout>
```
3、fragment布局界面，这里只列出一个，fragment_1.xml：
```python
<?xml version="1.0" encoding="utf-8"?>
<LinearLayout xmlns:android="http://schemas.android.com/apk/res/android"
    android:layout_width="fill_parent"
    android:layout_height="fill_parent" >
 
    <ImageView
        android:id="@+id/imageview"
        android:layout_width="fill_parent"
        android:layout_height="fill_parent"
        android:scaleType="fitCenter"
        android:src="@drawable/xianjian01" >
    </ImageView>
 
</LinearLayout>
```
4、Tab选项的自定义按钮资源文件，列出其中一个按钮，tab_home_btn：
```python
<?xml version="1.0" encoding="utf-8"?>
<selector xmlns:android="http://schemas.android.com/apk/res/android">
 
    <item android:drawable="@drawable/icon_home_sel" android:state_selected="true"/>
    <item android:drawable="@drawable/icon_home_nor"/>
 
</selector>
```
5、Tab选项按钮背景资源文件，selector_tab_background.xml：
```python
<?xml version="1.0" encoding="utf-8"?>
<selector xmlns:android="http://schemas.android.com/apk/res/android">
 
    <item android:drawable="@drawable/home_btn_bg" android:state_pressed="true"/>
    <item android:drawable="@drawable/home_btn_bg" android:state_selected="true"/>
 
</selector>
```
6、主Activity类，MainTabActivity.java：
```python
package com.yangyu.mycustomtab02;
 
import android.os.Bundle;
import android.support.v4.app.FragmentActivity;
import android.support.v4.app.FragmentTabHost;
import android.view.LayoutInflater;
import android.view.View;
import android.widget.ImageView;
import android.widget.TabHost.TabSpec;
import android.widget.TextView;
 
/**
 * @author yangyu
 *	功能描述：自定义TabHost
 */
public class MainTabActivity extends FragmentActivity{	
	//定义FragmentTabHost对象
	private FragmentTabHost mTabHost;
	
	//定义一个布局
	private LayoutInflater layoutInflater;
		
	//定义数组来存放Fragment界面
	private Class fragmentArray[] = {FragmentPage1.class,FragmentPage2.class,
                FragmentPage3.class,FragmentPage4.class,FragmentPage5.class};
	
	//定义数组来存放按钮图片
	private int mImageViewArray[] = {R.drawable.tab_home_btn,R.drawable.tab_message_btn,
                R.drawable.tab_selfinfo_btn,R.drawable.tab_square_btn,R.drawable.tab_more_btn};
	
	//Tab选项卡的文字
	private String mTextviewArray[] = {"首页","消息","好友","广场","更多"};
	
	public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main_tab_layout);
        
        initView();
    }
	 
	/**
	 * 初始化组件
	 */
	private void initView(){
		//实例化布局对象
		layoutInflater = LayoutInflater.from(this);
				
		//实例化TabHost对象，得到TabHost
		mTabHost = (FragmentTabHost)findViewById(android.R.id.tabhost);
		mTabHost.setup(this, getSupportFragmentManager(), R.id.realtabcontent);	
		
		//得到fragment的个数
		int count = fragmentArray.length;	
				
		for(int i = 0; i < count; i++){	
			//为每一个Tab按钮设置图标、文字和内容
			TabSpec tabSpec = mTabHost.newTabSpec(mTextviewArray[i]).setIndicator(getTabItemView(i));
			//将Tab按钮添加进Tab选项卡中
			mTabHost.addTab(tabSpec, fragmentArray[i], null);
			//设置Tab按钮的背景
			mTabHost.getTabWidget().getChildAt(i).setBackgroundResource(
                                R.drawable.selector_tab_background);
		}
	}
				
	/**
	 * 给Tab按钮设置图标和文字
	 */
	private View getTabItemView(int index){
		View view = layoutInflater.inflate(R.layout.tab_item_view, null);
	
		ImageView imageView = (ImageView) view.findViewById(R.id.imageview);
		imageView.setImageResource(mImageViewArray[index]);
		
		TextView textView = (TextView) view.findViewById(R.id.textview);		
		textView.setText(mTextviewArray[index]);
	
		return view;
	}
}
```
7、Fragment页面，FragmentPage1.java：
```python
package com.yangyu.mycustomtab02;
 
import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
 
public class FragmentPage1 extends Fragment{
 
	@Override
	public View onCreateView(LayoutInflater inflater, ViewGroup container,Bundle savedInstanceState) {		
		return inflater.inflate(R.layout.fragment_1, null);		
	}	
}
```
[源码下载地址](http://download.csdn.net/detail/yangyu20121224/5511157)
PS：该文章主要是在做Android随手拍时转载的,下面也是一些相关优秀的文章。
[[Android UI设计与开发]第09期：底部菜单栏（四）Fragment+PopupWindow仿QQ空间最新版底部菜单栏](http://blog.csdn.net/yangyu20121224/article/details/9023451)
[android底部菜单栏demo](http://blog.csdn.net/wangkuifeng0118/article/details/7745109)
[Android开发技巧——实现底部图标文字的导航栏(已更新)](http://blog.csdn.net/maosidiaoxian/article/details/38864679)
[Android提高十八篇之自定义Menu(TabMenu)](http://blog.csdn.net/hellogv/article/details/6168439)


