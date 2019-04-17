# 通过重写ScrollView和Listview完成上下滑动选中不同位置标题的效果 - DEVELOPER - CSDN博客





2014年09月25日 17:04:40[学术袁](https://me.csdn.net/u012827205)阅读数：3466








![](https://img-blog.csdn.net/20140925164944734?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)![](https://img-blog.csdn.net/20140925174054494?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)![](https://img-blog.csdn.net/20140925174055687?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)![](https://img-blog.csdn.net/20140925165022828?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



[*点击这里源码下载*](http://download.csdn.net/detail/u012827205/7973927)

看到了吗？就是这种效果：我再跟大家简单的叙述一下；

头部标题有三个：剧本梗概、剧本正文、剧本介绍。

当ScrollView滑到剧本中的无论哪一个内容标题的时候头部的大标题将被选中并呈现橙色；而三个标题内容下面是ListView的item，嵌在其父控件Scrollview中的。

下面让我们分析一下代码：

布局：activity_main.xml

```java
<RelativeLayout xmlns:android="http://schemas.android.com/apk/res/android"
    xmlns:tools="http://schemas.android.com/tools"
    android:layout_width="match_parent"
    android:layout_height="wrap_content"
    android:background="#f8f8f8"
    android:gravity="top"
    tools:context=".MainActivity" >

    <LinearLayout
        android:id="@+id/liLay_tabs"
        android:layout_width="fill_parent"
        android:layout_height="35dip"
        android:background="#fff"
        android:gravity="center_vertical"
        android:orientation="horizontal" >

        <LinearLayout
            android:id="@+id/liLay_tabsLeft"
            android:layout_width="fill_parent"
            android:layout_height="45dip"
            android:layout_weight="1.0"
            android:orientation="horizontal" >

            <TextView
                android:id="@+id/tabs_tvMovieDetail"
                android:layout_width="fill_parent"
                android:layout_height="45dp"
                android:gravity="center"
                android:text="故事梗概"
                android:textColor="#eb8037"
                android:textSize="16sp" />
        </LinearLayout>

        <LinearLayout
            android:id="@+id/liLay_tabsMiddle"
            android:layout_width="fill_parent"
            android:layout_height="45dip"
            android:layout_weight="1.0"
            android:orientation="horizontal" >

            <TextView
                android:id="@+id/tabs_tvMovieComment"
                android:layout_width="fill_parent"
                android:layout_height="45dp"
                android:gravity="center"
                android:text="剧本正文"
                android:textSize="16sp" />
        </LinearLayout>

        <LinearLayout
            android:id="@+id/liLay_tabsRight"
            android:layout_width="fill_parent"
            android:layout_height="45dip"
            android:layout_weight="1.0"
            android:orientation="horizontal" >

            <TextView
                android:id="@+id/tabs_tvMovieRavent"
                android:layout_width="fill_parent"
                android:layout_height="45dp"
                android:gravity="center"
                android:text="人物介绍"
                android:textSize="16sp" />
        </LinearLayout>
    </LinearLayout>

    <LinearLayout
        android:id="@+id/re_tabLine"
        android:layout_width="fill_parent"
        android:layout_height="2dp"
        android:layout_below="@+id/liLay_tabs"
        android:background="#c8c7cc" >

        <ImageView
            android:id="@+id/tabLine"
            android:layout_width="200dp"
            android:layout_height="2.5dp"
            android:background="#eb8037" />
    </LinearLayout>

    <com.junhuaceo.customscrollview.custom.CustomScrollView
        android:id="@+id/customScrollview"
        android:layout_width="match_parent"
        android:layout_height="match_parent"
        android:layout_below="@+id/re_tabLine"
        android:fadeScrollbars="true"
        android:paddingLeft="15dp"
        android:paddingRight="15dp"
        android:scrollbarStyle="insideOverlay"
        android:scrollbars="none" >

        <RelativeLayout
            android:layout_width="fill_parent"
            android:layout_height="wrap_content" >

            <TextView
                android:id="@+id/line1"
                android:layout_width="match_parent"
                android:layout_height="0.5dp"
                android:background="#C8C7CC" />

            <TextView
                android:id="@+id/text1"
                android:layout_width="match_parent"
                android:layout_height="wrap_content"
                android:layout_below="@+id/text1"
                android:layout_marginTop="15dp"
                android:text="@string/text1"
                android:textSize="16sp" />

            <TextView
                android:id="@+id/line2"
                android:layout_width="match_parent"
                android:layout_height="0.5dp"
                android:layout_below="@+id/text1"
                android:layout_marginTop="15dp"
                android:background="#C8C7CC" />

            <TextView
                android:id="@+id/text2"
                android:layout_width="match_parent"
                android:layout_height="wrap_content"
                android:layout_below="@+id/line2"
                android:layout_marginTop="15dp"
                android:text="@string/text2"
                android:textSize="16sp" />

            <TextView
                android:id="@+id/line3"
                android:layout_width="match_parent"
                android:layout_height="0.5dp"
                android:layout_below="@+id/text2"
                android:layout_marginTop="15dp"
                android:background="#C8C7CC" />

            <TextView
                android:id="@+id/text3"
                android:layout_width="match_parent"
                android:layout_height="wrap_content"
                android:layout_below="@+id/line3"
                android:layout_marginTop="15dp"
                android:text="@string/text3"
                android:textSize="16sp" />

            <com.junhuaceo.customscrollview.custom.CustomListView
                android:id="@+id/listview"
                android:layout_width="fill_parent"
                android:layout_height="fill_parent"
                android:layout_below="@+id/text3"
                android:layout_marginTop="25dp"
                android:divider="#C8C7CC"
                android:dividerHeight="0.3dp" >
            </com.junhuaceo.customscrollview.custom.CustomListView>
        </RelativeLayout>
    </com.junhuaceo.customscrollview.custom.CustomScrollView>

    <include
        android:id="@+id/commentMessageView"
        android:layout_alignParentBottom="true"
        layout="@layout/send_commentmessage_insert"
        android:visibility="gone" />

</RelativeLayout>
```



重写的ScrollView控件代码CustomScrollView.java：

```java
/**
 * 
 * 项目：CustomScrollView
 * 包名：com.junhuaceo.customscrollview.custom
 * @author Yuanjunhua
 *
 * 2014-9-25下午1:08:08
 */
public class CustomScrollView extends ScrollView {

	
	public CustomScrollView(Context context, AttributeSet attrs, int defStyle) {
		super(context, attrs, defStyle);
		// TODO Auto-generated constructor stub
	}


	public CustomScrollView(Context context, AttributeSet attrs) {
		super(context, attrs);
		// TODO Auto-generated constructor stub
	}

	public CustomScrollView(Context context) {
		super(context);
		// TODO Auto-generated constructor stub
	}
	
	
	private OnScrollListener listener;
	public interface OnScrollListener{
		void onScrollListener(int scrollY);
	}
	public void setOnScrollListener(OnScrollListener listener){
		this.listener = listener;
	}
	
	
	private int lastScrollY;
	private Handler mHandler = new Handler(){
		public void handleMessage(android.os.Message msg) {
			int scrollY = getScrollY();
			if(lastScrollY != scrollY){
				lastScrollY = scrollY;
				mHandler.sendMessageDelayed(mHandler.obtainMessage(), 10);
			}
		};
	};
	
	@Override
	public boolean onTouchEvent(android.view.MotionEvent ev) {
		if(null != listener ){
			listener.onScrollListener(lastScrollY = getScrollY());
		}
		switch (ev.getAction()) {
		case MotionEvent.ACTION_UP:
			mHandler.sendMessageDelayed(mHandler.obtainMessage(), 10);
			break;
		}
		return super.onTouchEvent(ev);
	};
	
	
	 @Override
	protected void onScrollChanged(int l, int t, int oldl, int oldt) {
		// TODO Auto-generated method stub
		super.onScrollChanged(l, t, oldl, oldt);
		if(null != listener){
			listener.onScrollListener(t);
			mHandler.sendMessageDelayed(mHandler.obtainMessage(), 10);
		}
	}
}
```



重写的Listview代码CustomListView.java：

```java
/**
 * 
 * 项目：CustomScrollView
 * 包名：com.junhuaceo.customscrollview.custom
 * @author Yuanjunhua
 *
 * 2014-9-25下午3:12:28
 */
public class CustomListView extends ListView {

	public CustomListView(Context context, AttributeSet attrs, int defStyle) {
		super(context, attrs, defStyle);
		// TODO Auto-generated constructor stub
	}

	public CustomListView(Context context, AttributeSet attrs) {
		super(context, attrs);
		// TODO Auto-generated constructor stub
	}

	public CustomListView(Context context) {
		super(context);
		// TODO Auto-generated constructor stub
	}
	
	
	@Override
	protected void onMeasure(int widthMeasureSpec, int heightMeasureSpec) {
		// TODO Auto-generated method stub
		
		int expandSpec = MeasureSpec.makeMeasureSpec(Integer.MAX_VALUE >> 2, MeasureSpec.AT_MOST);
		super.onMeasure(widthMeasureSpec, expandSpec);
		
		
	}
}
```




最后是MainActivity.java：
```java
/**
 * 
 * 项目：CustomScrollView
 * 包名：com.junhuaceo.customscrollview
 * @author Yuanjunhua
 *
 * 2014-9-25上午11:03:09
 */
public class MainActivity extends Activity implements OnScrollListener, OnClickListener{

	private TextView line1;
	private TextView line2;
	private TextView line3;
	
	private CustomScrollView cs;
	/** 屏幕的宽度  */
	private int screenWidth, tabWidth;
	/** scroll 距离父控件的距离  */
	private int scrollToParentDistance;
	/** line 距离父控件的距离  */
	private int lineToParentDistance1,lineToParentDistance2,lineToParentDistance3;
	/** line 高度  */
    private int lineHight;	
    
    private LinearLayout liLeft,liMid,liRight;
    private TextView tvLeft,tvMid,tvRight;
    private TextView text1,text2,text3;
    private ImageView tabLine;
	
    private ListView listView;
    private LinkedList<CheckCommentsBean> listBean;
    private DramaInsidePageAdapter adapter;
	private Handler mHandler = new Handler(){
		public void handleMessage(android.os.Message msg) {
			View edtPopView = (View) msg.obj;
			edtPopView.setBackgroundResource(R.drawable.bg_black_color);
		};
	};
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		tvLeft=(TextView) findViewById(R.id.tabs_tvMovieDetail);
		tvMid=(TextView) findViewById(R.id.tabs_tvMovieComment);
		tvRight=(TextView) findViewById(R.id.tabs_tvMovieRavent);
		
		
		text1 = (TextView) findViewById(R.id.text1);
		text2 = (TextView) findViewById(R.id.text2);
		text3 = (TextView) findViewById(R.id.text3);
		setTextApointWordsColor();
		
		liLeft=(LinearLayout) findViewById(R.id.liLay_tabsLeft);
		liMid=(LinearLayout) findViewById(R.id.liLay_tabsMiddle);
		liRight=(LinearLayout) findViewById(R.id.liLay_tabsRight);
		liLeft.setOnClickListener(this);
		liMid.setOnClickListener(this);
		liRight.setOnClickListener(this);
		
		tabLine=(ImageView) findViewById(R.id.tabLine);
		
		line1 = (TextView) findViewById(R.id.line1);
		line2 = (TextView) findViewById(R.id.line2);
		line3 = (TextView) findViewById(R.id.line3);
		
		SendCommentMessageInsertListViewUtil.getInstance().preInitViews(this, mHandler);
		listView = (ListView) findViewById(R.id.listview);
		
		cs = (CustomScrollView) findViewById(R.id.customScrollview);
		cs.setOnScrollListener(this);
		getScreen();
		
		String heandUrl = "http://www.jxvdy.com/file/upload/201409/02/11-17-52-65-3.jpg";
		String content = "故事梗概：聚美优品百米冲刺奔向纽交所，给一批化妆品电商敲响警钟。美妆电商平台PBA正在寻找新的突破点，他们的首款智能硬件成为爆款。";
		String name = "樱桃小丸子";
		int age = 99;
		listBean = new LinkedList<CheckCommentsBean>();
		for (int i = 0; i < 15; i++) {
			listBean.add(new CheckCommentsBean(heandUrl, name, age, content));
		}

		adapter = new DramaInsidePageAdapter(this, listBean);
		listView.setAdapter(adapter);
		
	}
	
	/** 窗口有焦点的时候，即所有的布局绘制完毕的时候，我们来获取购买布局的高度和myScrollView距离父类布局的顶部位置   */
	@Override
	public void onWindowFocusChanged(boolean hasFocus) {
		// TODO Auto-generated method stub
		super.onWindowFocusChanged(hasFocus);
		
		if(hasFocus){
			lineHight = line1.getHeight();
			lineToParentDistance1 = line1.getTop();
			lineToParentDistance2 = line2.getTop();
			lineToParentDistance3 = line3.getTop();
			scrollToParentDistance = cs.getTop();
		}
		
	}


	/**  滚动的回调方法，当滚动的Y距离大于或者等于 标题内容距离父类布局顶部的位置，就选中其对应的标题 */
	@Override
	public void onScrollListener(int scrollY) {
		// TODO Auto-generated method stub
		if(scrollY >= (lineToParentDistance1 + lineHight) & scrollY < (lineToParentDistance2 + lineHight)){
			reSetTextColor();
			LayoutParams lp = (LayoutParams) tabLine.getLayoutParams();
			lp.leftMargin = 0;
			tabLine.setLayoutParams(lp);
			tvLeft.setTextColor(Color.parseColor("#eb8037"));
		}else if(scrollY >= (lineToParentDistance2 + lineHight) & scrollY < (lineToParentDistance3 + lineHight)){
			reSetTextColor();
			LayoutParams lp = (LayoutParams) tabLine.getLayoutParams();
			lp.leftMargin = screenWidth / 3;
			tabLine.setLayoutParams(lp);
			tvMid.setTextColor(Color.parseColor("#eb8037"));
			
		}else if(scrollY >= (lineToParentDistance3 + lineHight)){
			reSetTextColor();
			LayoutParams lp = (LayoutParams) tabLine.getLayoutParams();
			lp.leftMargin = 2 * screenWidth / 3;
			tabLine.setLayoutParams(lp);
			tvRight.setTextColor(Color.parseColor("#eb8037"));
			
		}
	}

	
	/** 重新设置字体颜色 */
	private void reSetTextColor(){
		tvLeft.setTextColor(Color.parseColor("#000000"));
		tvMid.setTextColor(Color.parseColor("#000000"));
		tvRight.setTextColor(Color.parseColor("#000000"));
	}
	
	/** 获取屏幕的宽度 */
	private void getScreen(){
		DisplayMetrics outMetrics=new DisplayMetrics();
		getWindowManager().getDefaultDisplay().getMetrics(outMetrics);
		screenWidth = outMetrics.widthPixels;
		tabWidth = (int) (1.0 * screenWidth / 3);
		LayoutParams lp=(LayoutParams) tabLine.getLayoutParams();
		lp.width=tabWidth;
		tabLine.setLayoutParams(lp);
	}
	
	/**
	 * 设置文字中被选中字的颜色
	 */
	private void setTextApointWordsColor(){
		String txt1 = getResources().getString(R.string.text1);
		SpannableStringBuilder style1 = new SpannableStringBuilder(txt1);
//		style.setSpan(new BackgroundColorSpan(Color.RED), 0, 5,	Spannable.SPAN_EXCLUSIVE_INCLUSIVE); // 设置指定位置textview的背景颜色
		style1.setSpan(new ForegroundColorSpan(Color.GRAY),0,5,Spannable.SPAN_EXCLUSIVE_INCLUSIVE);     //设置指定位置文字的颜色  
		String txt2 = getResources().getString(R.string.text2);
		SpannableStringBuilder style2 = new SpannableStringBuilder(txt2);
//		style.setSpan(new BackgroundColorSpan(Color.RED), 0, 5,	Spannable.SPAN_EXCLUSIVE_INCLUSIVE); // 设置指定位置textview的背景颜色
		style2.setSpan(new ForegroundColorSpan(Color.GRAY),0,5,Spannable.SPAN_EXCLUSIVE_INCLUSIVE);     //设置指定位置文字的颜色  
		String txt3 = getResources().getString(R.string.text3);
		SpannableStringBuilder style3 = new SpannableStringBuilder(txt3);
//		style.setSpan(new BackgroundColorSpan(Color.RED), 0, 5,	Spannable.SPAN_EXCLUSIVE_INCLUSIVE); // 设置指定位置textview的背景颜色
		style3.setSpan(new ForegroundColorSpan(Color.GRAY),0,5,Spannable.SPAN_EXCLUSIVE_INCLUSIVE);     //设置指定位置文字的颜色  
		text1.setText(style1);
		text2.setText(style2);
		text3.setText(style3);
	}


	@Override
	public void onClick(View v) {
		// TODO Auto-generated method stub
		switch (v.getId()) {
		case R.id.liLay_tabsLeft:
			cs.scrollTo(0, lineToParentDistance1);
			break;
		case R.id.liLay_tabsMiddle:
			cs.scrollTo(0, lineToParentDistance2);
			break;
		case R.id.liLay_tabsRight:
			cs.scrollTo(0, lineToParentDistance3);
			
			break;

		default:
			break;
		}
	}
	
}
```



接下来让我们分析代码：

 首先分析CustomScrollView.java:

 重写onScrollChanged方法，是为了在ScrollView上下滑动的时候进行滑动位置的监听，并通过自定义的监听接口在外部调用。在滑动监听的过程中，每隔10ms发送handler一次，记录下当前距离父控件顶部的距离scrollY；

重写onTouchEvent方法，是为了在通过触摸使ScrollView上下滑动时候，在自定义的接口外部调用过程中每当手指离开屏幕后10ms后发送handler一次，并记录下当前距离父控件顶部的距离scrollY；

 然后分析CustomListView.java:

重写onMeasure方法，是为了解决冲突<当ListView嵌套在ScrollView中的时候，ListView无法显示所有item或者只能显示一条item>；冲写onMeasure方法改变heightMeasureSpec大小属性使ListView中的所有item都能完全显示出来；

最后分享MainActivity.java:

这里改变指定字体颜色，即改变任意textView中的某些文字的颜色；程序使用这几句代码：

```java
String txt1 = getResources().getString(R.string.text1);
		SpannableStringBuilder style1 = new SpannableStringBuilder(txt1);
//		style.setSpan(new BackgroundColorSpan(Color.RED), 0, 5,	Spannable.SPAN_EXCLUSIVE_INCLUSIVE); // 设置指定位置textview的背景颜色
		style1.setSpan(new ForegroundColorSpan(Color.GRAY),0,5,Spannable.SPAN_EXCLUSIVE_INCLUSIVE);     //设置指定位置文字的颜色
text1.setText(style1);
```

MainActivity中控制ScrollView滑动过程根据滑动距离来选中标题文字思路是这样的：首先在重写的ScrollView中通过onTouchEvent方法和onScrollChanged方法，是的在屏幕中页面滑动的过程中能记录下当前ScrollView页面滑动的距离位置，也就是说：把ScrollView看做一张A4的纸的话，在一个A4纸大小的页面，记录下这张A4纸的顶部距离其所在页面顶部的距离大小的绝对值。然后在MainActivity中回调的过程拿ScrollY与那三个标题对应的文章TextView<本身是控件>所获得的距离页面顶部距离大小的绝对值进行比较，并选中页面内容对应的标题。




建一个群，方便大家交流：蹦蹦哒Android <群号：423923313>




