# 怎么使用侧滑菜单SlidingMenu？ - DEVELOPER - CSDN博客





2014年08月26日 19:02:07[学术袁](https://me.csdn.net/u012827205)阅读数：2110










![](https://img-blog.csdn.net/20140826181624708?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

左侧滑：


![](https://img-blog.csdn.net/20140826181718091?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

右侧滑：


![](https://img-blog.csdn.net/20140826181803752?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)




















[***点击这里下载源码***](http://download.csdn.net/detail/u012827205/7823567)

在此之前，我使用网上朋友自定义的侧滑菜单。利用重写布局来控制和实现，但是到最后项目写80%的时候。突然发现跟我的功能产生了冲突。我去，这么凌乱的代码，我也搞不定。所以没办法，就在网上狂搜索，三天之后。搞定了，并成功嵌入到了我的项目中。其中的测试demo如上图！还是别带着好奇去使用那些自定义的侧滑菜单，当你项目马上要结项，你使用该侧滑菜单跟你 的项目功能产生了冲突，你又不能解决，这时候你才叫一个抓狂。

所以，经久的更耐用，人家都是用，当你项目出现问题，才能快熟解决；

好了，看一下怎么使用吧！



```java
/** SlidingMenu 常用属性介绍: */

menu.setMode（SlidingMenu.LEFT）;//设置左滑菜单 SlidingMenu.RIGHT  SlidingMenu.LEFT_RIGHT 右侧滑/左右侧滑

menu.setTouchModeAbove（SlidingMenu.TOUCHMODE_FULLSCREEN）;//设置滑动的屏幕局限，该设置为全屏区域都可以滑动

menu.setShadowDrawable（R.drawable.shadow）;//设置暗影

menu.setShadowWidthRes（R.dimen.shadow_width）;//设置暗影的宽度

menu.setBehindOffsetRes（R.dimen.slidingmenu_offset）;//SlidingMenu划出时主页面显示的残剩宽度

menu.setBehindWidth（400）;//设置SlidingMenu菜单的宽度

menu.setFadeDegree（0.35f）;//SlidingMenu滑动时的渐变程度

menu.attachToActivity（this， SlidingMenu.SLIDING_CONTENT）;//使SlidingMenu附加在Activity上

menu.setMenu（R.layout.menu_layout）;//设置menu的布局文件

menu.toggle（）;//动态断定主动封闭或开启SlidingMenu

menu.showMenu（）;//显示SlidingMenu

menu.showContent（）;//显示内容

menu.setOnOpenListener（onOpenListener）;//slidingmenu打开关于封闭menu有两个，简单的来说，对于menu close事务，一个是when，一个是after 

menu.OnClosedListener（OnClosedListener）;//slidingmenu封闭时事务

menu.OnClosedListener（OnClosedListener）;//slidingmenu封闭后事务
```


简单设置左方向菜单侧滑的使用方法：
```java
public class SlidingExample extends Activity {

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setTitle(R.string.attach);
        // set the content view
        setContentView(R.layout.content);
        // configure the SlidingMenu
        SlidingMenu menu = new SlidingMenu(this);
        menu.setMode(SlidingMenu.LEFT);
        menu.setTouchModeAbove(SlidingMenu.TOUCHMODE_FULLSCREEN);
        menu.setShadowWidthRes(R.dimen.shadow_width);
        menu.setShadowDrawable(R.drawable.shadow);
        menu.setBehindOffsetRes(R.dimen.slidingmenu_offset);
        menu.setFadeDegree(0.35f);
        menu.attachToActivity(this, SlidingMenu.SLIDING_CONTENT);
        menu.setMenu(R.layout.menu);
    }

}
```


如果你要设置左右菜单侧滑，看这里：
```java
public class MainActivity extends FragmentActivity {

	public SlidingMenu mSlidingMenu;
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		
		mSlidingMenu = new SlidingMenu(this);
		mSlidingMenu.setMode(SlidingMenu.LEFT_RIGHT);
		mSlidingMenu.setBehindOffsetRes(R.dimen.slidingmenu_offset);
		mSlidingMenu.setTouchModeAbove(SlidingMenu.TOUCHMODE_FULLSCREEN);
		mSlidingMenu.attachToActivity(this, SlidingMenu.SLIDING_CONTENT);
		mSlidingMenu.setMenu(R.layout.slidemenu_primary);//左侧滑
		mSlidingMenu.setSecondaryMenu(R.layout.slidemenu_secondery);//右侧滑
		
		
		 getSupportFragmentManager().beginTransaction().
		 replace(R.id.aty_main_framLayout, new ViewPagerFragment()).commit();
		
		
		
		 getSupportFragmentManager().beginTransaction().
		 replace(R.id.framLayout_fragment, new TranslationAnimationFragment()).commit();
		
		
	}
	
	
	
	/**  重写返回键和menu键，控制菜单侧滑关闭  */
	@Override
	public boolean onKeyDown(int keyCode, KeyEvent event) {
		// TODO Auto-generated method stub
		switch (keyCode) {
		case KeyEvent.KEYCODE_BACK:
			if(mSlidingMenu.isMenuShowing())
			mSlidingMenu.showContent();
			else finish();
			return true;
		case KeyEvent.KEYCODE_MENU:
			mSlidingMenu.showSecondaryMenu();
			return true;
		}
		
		
		
		
		return super.onKeyDown(keyCode, event);
	}


}
```


下面看一下如何解决侧滑跟ViewPager左右滑动的冲突：《当然，我的源码上展示了怎么使用》





```java
mSlidingMenu.addIgnoredView(viewPager);
```
在你的Viewpager内容显示之前，调用上面的方法，就Ok了、





```java
SlidingMenu.setTouchModeAbove().其中一共包含三中手势模式：
TOUCHMODE_FULLSCREEN 全屏模式，在正文布局中通过手势也可以打开SlidingMenu
TOUCHMODE_MARGIN 边缘模式，在正文布局的边缘处通过手势可以找开SlidingMenu
TOUCHMODE_NONE 自然是不能通过手势打开SlidingMenu了
```


当你在设置暗影时，在res/drawable/shadow.xml
```java
<?xml version="1.0" encoding="utf-8"?>
<shape xmlns:android="http://schemas.android.com/apk/res/android" >

    <gradient
        android:centerColor="#11000000"
        android:endColor="#00000000"
        android:startColor="#33000000" />

</shape>
```

*续写：*

其一：在导入工程之后，如果使用到了Actionbar，要把actionbarSherLockLibrary这个类库、SlideMenu类库以及所建立的工程，必须要使用同一个android-support-v4.jar包，(使用自己的工程中的，然后把其他的给替换掉)；否则，会报错哦！

其二：当我们导入完成侧滑的效果之后，是不是发现我们所做的侧滑菜单的左右两侧划出来的距离是一样的？答案是必须的，因为源码就是这么做的，并没有给我们写出单独设置某边侧滑——左或者右的距离。那么，这样的话，就要我们自己来设置了。自己看了看源码以及在网上看了看小伙伴们对侧滑的自定义的理解，总结到了，只需要更改几个方法和属性就能完成该功能。让我们自己能够任意设置左右侧滑的宽度！

![](https://img-blog.csdn.net/20140903183220193?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)




![](https://img-blog.csdn.net/20140903183321688?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)







![](https://img-blog.csdn.net/20140903183401546?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

[](http://download.csdn.net/detail/u012827205/7860093)

[](http://download.csdn.net/detail/u012827205/7860093)

[*点击这里源码下载*](http://download.csdn.net/detail/u012827205/7860093)


























首先，在源码中的SlidingMenu.java中添加几个方法：

```java
//*********************************自定义扩展侧滑宽度方法********************************************

	// 设置右侧边栏菜单展开时距离左边界的偏移量
    public void setRightMenuOffset(int offset) {
            mViewBehind.setRightWidthOffset(offset);
    }

    // 设置右侧边栏菜单展开时距离左边界的偏移量
    public void setRightMenuOffsetRes(int resId) {
            int i = (int) getContext().getResources().getDimension(resId);
            setRightMenuOffset(i);
    }

    // 设置右侧边栏的宽度
    @SuppressWarnings("deprecation")
    public void setRightBehindWidth(int i) {
            int width;
            Display display = ((WindowManager) getContext().getSystemService(
                            Context.WINDOW_SERVICE)).getDefaultDisplay();
            try {
                    Class<?> cls = Display.class;
                    Class<?>[] parameterTypes = { Point.class };
                    Point parameter = new Point();
                    Method method = cls.getMethod("getSize", parameterTypes);
                    method.invoke(display, parameter);
                    width = parameter.x;
            } catch (Exception e) {
                    width = display.getWidth();
            }
            setRightMenuOffset(width - i);
    }

    // 设置右侧边栏的宽度
    public void setRightBehindWidthRes(int res) {
            int i = (int) getContext().getResources().getDimension(res);
            setRightBehindWidth(i);
    }
```


然后，修改CustomViewBehind.java类



即：1.在源码CustomViewBehind.java类中加入必要的方法：

```java
//	----------------------自定义扩展添加方法----------------------------------------
	private int mRightWidthOffset; // 右边宽度偏移量

	public void setRightWidthOffset(int i) {
		mRightWidthOffset = i;
		requestLayout();
	}

	private int getRightBehindWidth() {
		return mSecondaryContent.getWidth();
	}
```



2.修改方法：scrollBehindT( View content , int x , int y );

```java
public void scrollBehindTo(View content, int x, int y) {
        int vis = View.VISIBLE;
        if (mMode == SlidingMenu.LEFT) {
                if (x >= content.getLeft())
                        vis = View.INVISIBLE;
                scrollTo((int) ((x + getBehindWidth()) * mScrollScale), y);
        } else if (mMode == SlidingMenu.RIGHT) {
                if (x <= content.getLeft())
                        vis = View.INVISIBLE;
                scrollTo(
                                (int) (getRightBehindWidth() - getWidth() + (x - getRightBehindWidth())
                                                * mScrollScale), y);
        } else if (mMode == SlidingMenu.LEFT_RIGHT) {
                mContent.setVisibility(x >= content.getLeft() ? View.INVISIBLE
                                : View.VISIBLE);
                mSecondaryContent
                                .setVisibility(x <= content.getLeft() ? View.INVISIBLE
                                                : View.VISIBLE);
                vis = x == 0 ? View.INVISIBLE : View.VISIBLE;
                if (x <= content.getLeft()) {
                        scrollTo((int) ((x + getBehindWidth()) * mScrollScale), y);
                } else {
                        scrollTo(
                                        (int) (getRightBehindWidth() - getWidth() + (x - getRightBehindWidth())
                                                        * mScrollScale), y);
                }
        }
        if (vis == View.INVISIBLE)
                Log.v(TAG, "behind INVISIBLE");
        setVisibility(vis);
}
```
3.修改方法：getMenuLeft( View content , int page );
```java
public int getMenuLeft(View content, int page) {
        if (mMode == SlidingMenu.LEFT) {
                switch (page) {
                case 0:
                        return content.getLeft() - getBehindWidth();
                case 2:
                        return content.getLeft();
                }
        } else if (mMode == SlidingMenu.RIGHT) {
                switch (page) {
                case 0:
                        return content.getLeft();
                case 2:
                        return content.getLeft() + getRightBehindWidth();
                }
        } else if (mMode == SlidingMenu.LEFT_RIGHT) {
                switch (page) {
                case 0:
                        return content.getLeft() - getBehindWidth();
                case 2:
                        return content.getLeft() + getRightBehindWidth();
                }
        }
        return content.getLeft();
}
```
4.修改方法：getAbsRightBound( View content );
```java
public int getAbsRightBound(View content) {
        if (mMode == SlidingMenu.LEFT) {
                return content.getLeft();
        } else if (mMode == SlidingMenu.RIGHT
                        || mMode == SlidingMenu.LEFT_RIGHT) {
                return content.getLeft() + getRightBehindWidth();
        }
        return 0;
}
```


最后，使用 属性变量mRightWidthOffset 和方法 getRightBehindWidth()；替换在源码CustomViewBehind.java类中的方法
```java
onLayout(boolean changed, int l, int t, int r, int b);
 onMeasure(int widthMeasureSpec, int heightMeasureSpec);
```
中的变量属性 mWidthOffset ；和中的方法
```java
drawFade(View content, Canvas canvas, float openPercent)
```
中的 getBehindWidth();具体代码，如下所示：
```java
@Override
	protected void onLayout(boolean changed, int l, int t, int r, int b) {
		final int width = r - l;
		final int height = b - t;
		mContent.layout(0, 0, width - mWidthOffset, height);
		if (mSecondaryContent != null)
//			这里做了修改
//		mSecondaryContent.layout(0, 0, width - mWidthOffset, height);
			mSecondaryContent.layout(0, 0, width - mRightWidthOffset, height);
	}

	@Override
	protected void onMeasure(int widthMeasureSpec, int heightMeasureSpec) {
		int width = getDefaultSize(0, widthMeasureSpec);
		int height = getDefaultSize(0, heightMeasureSpec);
		setMeasuredDimension(width, height);
//		这里做了修改
//		final int contentWidth = getChildMeasureSpec(widthMeasureSpec, 0, width
//				- mWidthOffset);
		final int contentWidth = getChildMeasureSpec(widthMeasureSpec, 0, width
				- mRightWidthOffset);
		final int contentHeight = getChildMeasureSpec(heightMeasureSpec, 0,
				height);
		mContent.measure(contentWidth, contentHeight);
		if (mSecondaryContent != null)
			mSecondaryContent.measure(contentWidth, contentHeight);
	}
```


```java
public void drawFade(View content, Canvas canvas, float openPercent) {
		if (!mFadeEnabled)
			return;
		final int alpha = (int) (mFadeDegree * 255 * Math.abs(1 - openPercent));
		mFadePaint.setColor(Color.argb(alpha, 0, 0, 0));
		int left = 0;
		int right = 0;
		if (mMode == SlidingMenu.LEFT) {
			left = content.getLeft() - getBehindWidth();
			right = content.getLeft();
		} else if (mMode == SlidingMenu.RIGHT) {
			left = content.getRight();
			right = content.getRight() + getBehindWidth();
		} else if (mMode == SlidingMenu.LEFT_RIGHT) {
			left = content.getLeft() - getRightBehindWidth();//替换原先的getBehindWidth();
			right = content.getLeft();
			canvas.drawRect(left, 0, right, getHeight(), mFadePaint);
			left = content.getRight();
			right = content.getRight() + getRightBehindWidth();//替换原先的getBehindWidth();
		}
		canvas.drawRect(left, 0, right, getHeight(), mFadePaint);
	}
```


最后的最后，在控制侧滑菜单的Activity类中添加几句代码就搞定了：
```java
mSlidingMenu = new SlidingMenu(this);
		mSlidingMenu.setMode(SlidingMenu.LEFT_RIGHT);
		
		mSlidingMenu.setBehindOffsetRes(R.dimen.slidingmenu_offset);//左右侧滑距离
		mSlidingMenu.setRightMenuOffsetRes(R.dimen.slidingmenu_right_offset);//重写右边侧滑的距离
		
		mSlidingMenu.setTouchModeAbove(SlidingMenu.TOUCHMODE_FULLSCREEN);
		mSlidingMenu.attachToActivity(this, SlidingMenu.SLIDING_CONTENT);
		mSlidingMenu.setMenu(R.layout.slidemenu_primary);
		mSlidingMenu.setSecondaryMenu(R.layout.slidemenu_secondery);
		
		getSupportFragmentManager().beginTransaction().
		replace(R.id.aty_main_framLayout, new ViewPagerFragment()).commit();
		getSupportFragmentManager().beginTransaction().
		replace(R.id.framLayout_fragment, new TranslationAnimationFragment()).commit();
```















