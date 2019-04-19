# Android自定义控件——ListView的下拉刷新与上拉加载 - 阿瞒--治国之能臣 乱世之奸雄 - CSDN博客
2015年12月28日 22:54:24[baofenny](https://me.csdn.net/jiaminbao)阅读数：1213标签：[android](https://so.csdn.net/so/search/s.do?q=android&t=blog)
个人分类：[学习实例开发](https://blog.csdn.net/jiaminbao/article/category/1158383)
### 1.简介
无疑，在Android开发中，ListView是使用非常频繁的控件之一，ListView提供一个列表的容易，允许我们以列表的形式将数据展示到界面上，但是Google给我们提供的原生ListView的控件，虽然在功能上很强大，但是在用户体验和动态效果上，还是比较差劲的。为了改善用户体验，市面上纷纷出现了各种各样的自定义的ListView，他们功能强大，界面美观，使我们该需要学习的地方。其中，使用最频繁的功能无疑就是ListView的下拉刷新和上拉加载数据了，几乎在没一款内容型的App中都可以找到这种控件的身影，尤其是需要联网获取数据的模块，使用的就更为频繁了，so，我们很有必要了解下这种效果是怎么实现的。
### 2.开源组件PullToRefreshList介绍
既然Android和Java都是开源的，一些常见的功能或者效果就不难被找到。PullToRefresh就是一个典型的例子，PullToRefresh是老外写的一个开源ListView组件，这个项目在ListView的基础上扩展了ListView的功能，增强了Listview的用户体验，功能十分强大，而且很容易被集成到当前的项目中，你只需要调用简单的API，即可省去很多不必要的麻烦，非常棒。以上是项目在Github的链接，有兴趣的可以戳进去down下来，使用一下。这里不是我们的重点，不想废话了。
PullToRefresh的Github项目地址： *[https://github.com/chrisbanes/Android-PullToRefresh](https://github.com/chrisbanes/Android-PullToRefresh)*
### 3.自定义ListView——下拉刷新&上拉加载
本博客的重点讲述一下自定义LisView，实现下拉刷新和上拉加载的功能，实现类似于开源项目PullToRefresh的效果。好，既然如此，先看看我实现后的效果图，再分析：
![](http://img0.tuicool.com/yYBvQf.jpg!web)![](http://img2.tuicool.com/ZbIBNb.jpg!web)![](http://img2.tuicool.com/YniIviu.jpg!web)
好，效果图如上所示，下面逐步讲解下实现的过程。首先，来观察一下，ListView上方的布局，我这里称其为“头布局”，这个所谓的头布局，大致功能是这样的，一个ImageView显示上下拉动方向的状态的，ImageView相同的位置隐藏了一个ProgressBar，用来在数据刷新时给个提示作用的。还有两个TextView，上面用来显示下拉刷新时提醒用户是如何操作的，例如“下拉刷新”“松开刷新”“正在刷新”，另一个是用来显示本次刷新的时间的。比较简单的布局，下面是XML代码：
<?xml version="1.0" encoding="utf-8"?><LinearLayoutxmlns:android="http://schemas.android.com/apk/res/android"android:layout_width="match_parent"android:layout_height="wrap_content"android:orientation="horizontal" ><FrameLayout
android:layout_width="wrap_content"
android:layout_height="wrap_content"
android:layout_margin="10dip" ><ImageView
android:id="@+id/iv_listview_header_arrow"
android:layout_width="wrap_content"android:layout_height="wrap_content"android:layout_gravity="center"android:minWidth="30dip"android:src="@drawable/common_listview_headview_red_arrow" /><ProgressBar
android:id="@+id/pb_listview_header"
android:layout_width="wrap_content"android:layout_height="wrap_content"android:layout_gravity="center"android:indeterminateDrawable="@drawable/common_progressbar"android:visibility="gone" /></FrameLayout><LinearLayout
android:layout_width="fill_parent"
android:layout_height="wrap_content"
android:layout_gravity="center_vertical"
android:gravity="center_horizontal"
android:orientation="vertical" ><TextView
android:id="@+id/tv_listview_header_state"
android:layout_width="wrap_content"android:layout_height="wrap_content"android:text="下拉刷新"android:textColor="#FF0000"android:textSize="18sp" /><TextView
android:id="@+id/tv_listview_header_last_update_time"
android:layout_width="wrap_content"android:layout_height="wrap_content"android:layout_marginTop="5dip"android:text="最后刷新时间: 2014-10-10 12:56:12"android:textColor="@android:color/white"android:textSize="14sp" /></LinearLayout></LinearLayout>
下面讲解一下ListView下方的布局，我称其为“脚布局”，这个脚布局就更简单了，直接看XML代码好了：
<?xml version="1.0" encoding="utf-8"?><LinearLayoutxmlns:android="http://schemas.android.com/apk/res/android"android:layout_width="match_parent"android:layout_height="wrap_content"android:orientation="vertical" ><LinearLayout
android:layout_width="wrap_content"
android:layout_height="wrap_content"
android:layout_gravity="center_horizontal"
android:layout_margin="10dip"
android:gravity="center_vertical"
android:orientation="horizontal" ><ProgressBar
android:layout_width="wrap_content"
android:layout_height="wrap_content"
android:layout_gravity="center"
android:indeterminateDrawable="@drawable/common_progressbar" /><TextView
android:layout_width="wrap_content"
android:layout_height="wrap_content"
android:layout_marginLeft="10dip"
android:text="加载更多..."
android:textColor="#FF0000"
android:textSize="18sp" /></LinearLayout></LinearLayout>
此外，两个布局都用到一个ProgressBar的背景，其XML如下：
<?xml version="1.0" encoding="utf-8"?><rotatexmlns:android="http://schemas.android.com/apk/res/android"android:fromDegrees="0"android:pivotX="50%"android:pivotY="50%"android:toDegrees="360" ><shape
android:innerRadiusRatio="3"
android:shape="ring"
android:useLevel="false" ><gradient
android:centerColor="#FF6666"
android:endColor="#FF0000"
android:startColor="#FFFFFF"
android:type="sweep" /></shape></rotate>
ListView的头布局和脚布局已经做好了，那么接下来该怎么集成到ListView上去呢？首先我们来看看ListView的部分源代码，很轻松能找到这两个方法：**addHeaderView(View v)** ； **addFooterView(View v)** ，通过字面的意思就可以理解，这两个方法分别是向ListView顶部添加一个View和向ListView的底部添加View的，有了这两个方法，那么上面的头布局和脚布局就很容易被添加到ListView上了，并且成为ListView的一体。
其实，再看会发现，简单的使用这两个方法分别往ListView上添加头布局和脚布局是不合理，添加上去的头布局和脚布局会被显示出来，并没有被隐藏掉，为了实现下拉和上拉时能够将头布局和脚布局都“拉出来”并且还可以松开时，再次隐藏起来，我们可以使用View下的一个方法 setPadding(int left, int top, int right, int bottom) ，这个方法设置View的Padding属性，这里，我们不必管left、right、bottom，只要设置top的值为头布局或者脚布局的高度即可“隐藏”这两个布局，而且还可以在手指滑动屏幕的时候，动态的设置这个top的值，来实现头布局和脚布局的显示-隐藏-显示。
还有一个非常重要的话题，就是这个top的值还设定为多少合适？上面说了，我们来头布局来说明一下，隐藏这个头布局需要将top值设置成top=- 头布局高度，那么这个头布局的高度怎么求得呢？很显然，使用 getHeight() 是得不到头布局高度的，因为getHeight() 方法是先控件在屏幕上展示完毕后得到的高度，显然在这里，这个ListView还在构建中，并没有展示到屏幕上。所以注意了，我们先调用View下的measure(int
 widthMeasureSpec, int heightMeasureSpec) 方法，将widthMeasureSpec和heightMeasureSpec分别设置为0，这里的widthMeasureSpec和heightMeasureSpec并不是一个准备的值，而且指定一个规格或者标准让系统帮我们测量View的宽高，当我们指定widthMeasureSpec和heightMeasureSpec分别为0的时候，系统将不采用这个规格去测量，而是根据实际情况去测量。之后，我们可以调用View下的 getMeasuredHeight() 方法获取真实的头布局的高度，然后设置top
 = - 头布局实际高度，实现隐藏头布局。
```java
public class RefreshListView extends ListView implements OnScrollListener {
  private static final String TAG = "RefreshListView";
  private int firstVisibleItemPosition; // 屏幕显示在第一个的item的索引
  private int downY; // 按下时y轴的偏移量
  private int headerViewHeight; // 头布局的高度
  private View headerView; // 头布局的对象
  private final int DOWN_PULL_REFRESH = 0; // 下拉刷新状态
  private final int RELEASE_REFRESH = 1; // 松开刷新
  private final int REFRESHING = 2; // 正在刷新中
  private int currentState = DOWN_PULL_REFRESH; // 头布局的状态: 默认为下拉刷新状态
  private Animation upAnimation; // 向上旋转的动画
  private Animation downAnimation; // 向下旋转的动画
  private ImageView ivArrow; // 头布局的剪头
  private ProgressBar mProgressBar; // 头布局的进度条
  private TextView tvState; // 头布局的状态
  private TextView tvLastUpdateTime; // 头布局的最后更新时间
  private OnRefreshListener mOnRefershListener;
  private boolean isScrollToBottom; // 是否滑动到底部
  private View footerView; // 脚布局的对象
  private int footerViewHeight; // 脚布局的高度
  private boolean isLoadingMore = false; // 是否正在加载更多中
  public RefreshListView(Context context, AttributeSet attrs) {
    super(context, attrs);
    initHeaderView();
    initFooterView();
    this.setOnScrollListener(this);
  }
  /**
   * 初始化脚布局
   */
  private void initFooterView() {
    footerView = View.inflate(getContext(), R.layout.listview_footer, null);
    footerView.measure(0, 0);
    footerViewHeight = footerView.getMeasuredHeight();
    footerView.setPadding(0, -footerViewHeight, 0, 0);
    this.addFooterView(footerView);
  }
  /**
   * 初始化头布局
   */
  private void initHeaderView() {
    headerView = View.inflate(getContext(), R.layout.listview_header, null);
    ivArrow = (ImageView) headerView
        .findViewById(R.id.iv_listview_header_arrow);
    mProgressBar = (ProgressBar) headerView
        .findViewById(R.id.pb_listview_header);
    tvState = (TextView) headerView
        .findViewById(R.id.tv_listview_header_state);
    tvLastUpdateTime = (TextView) headerView
        .findViewById(R.id.tv_listview_header_last_update_time);
    // 设置最后刷新时间
    tvLastUpdateTime.setText("最后刷新时间: " + getLastUpdateTime());
    headerView.measure(0, 0); // 系统会帮我们测量出headerView的高度
    headerViewHeight = headerView.getMeasuredHeight();
    headerView.setPadding(0, -headerViewHeight, 0, 0);
    this.addHeaderView(headerView); // 向ListView的顶部添加一个view对象
    initAnimation();
  }
  /**
   * 获得系统的最新时间
   * 
   * @return
   */
  private String getLastUpdateTime() {
    SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
    return sdf.format(System.currentTimeMillis());
  }
  /**
   * 初始化动画
   */
  private void initAnimation() {
    upAnimation = new RotateAnimation(0f, -180f,
        Animation.RELATIVE_TO_SELF, 0.5f, Animation.RELATIVE_TO_SELF,
        0.5f);
    upAnimation.setDuration(500);
    upAnimation.setFillAfter(true); // 动画结束后, 停留在结束的位置上
    downAnimation = new RotateAnimation(-180f, -360f,
        Animation.RELATIVE_TO_SELF, 0.5f, Animation.RELATIVE_TO_SELF,
        0.5f);
    downAnimation.setDuration(500);
    downAnimation.setFillAfter(true); // 动画结束后, 停留在结束的位置上
  }
  @Override
  public boolean onTouchEvent(MotionEvent ev) {
    switch (ev.getAction()) {
      case MotionEvent.ACTION_DOWN :
        downY = (int) ev.getY();
        break;
      case MotionEvent.ACTION_MOVE :
        int moveY = (int) ev.getY();
        // 移动中的y - 按下的y = 间距.
        int diff = (moveY - downY) / 2;
        // -头布局的高度 + 间距 = paddingTop
        int paddingTop = -headerViewHeight + diff;
        // 如果: -头布局的高度 > paddingTop的值 执行super.onTouchEvent(ev);
        if (firstVisibleItemPosition == 0
            && -headerViewHeight < paddingTop) {
          if (paddingTop > 0 && currentState == DOWN_PULL_REFRESH) { // 完全显示了.
            Log.i(TAG, "松开刷新");
            currentState = RELEASE_REFRESH;
            refreshHeaderView();
          } else if (paddingTop < 0
              && currentState == RELEASE_REFRESH) { // 没有显示完全
            Log.i(TAG, "下拉刷新");
            currentState = DOWN_PULL_REFRESH;
            refreshHeaderView();
          }
          // 下拉头布局
          headerView.setPadding(0, paddingTop, 0, 0);
          return true;
        }
        break;
      case MotionEvent.ACTION_UP :
        // 判断当前的状态是松开刷新还是下拉刷新
        if (currentState == RELEASE_REFRESH) {
          Log.i(TAG, "刷新数据.");
          // 把头布局设置为完全显示状态
          headerView.setPadding(0, 0, 0, 0);
          // 进入到正在刷新中状态
          currentState = REFRESHING;
          refreshHeaderView();
          if (mOnRefershListener != null) {
            mOnRefershListener.onDownPullRefresh(); // 调用使用者的监听方法
          }
        } else if (currentState == DOWN_PULL_REFRESH) {
          // 隐藏头布局
          headerView.setPadding(0, -headerViewHeight, 0, 0);
        }
        break;
      default :
        break;
    }
    return super.onTouchEvent(ev);
  }
  /**
   * 根据currentState刷新头布局的状态
   */
  private void refreshHeaderView() {
    switch (currentState) {
      case DOWN_PULL_REFRESH : // 下拉刷新状态
        tvState.setText("下拉刷新");
        ivArrow.startAnimation(downAnimation); // 执行向下旋转
        break;
      case RELEASE_REFRESH : // 松开刷新状态
        tvState.setText("松开刷新");
        ivArrow.startAnimation(upAnimation); // 执行向上旋转
        break;
      case REFRESHING : // 正在刷新中状态
        ivArrow.clearAnimation();
        ivArrow.setVisibility(View.GONE);
        mProgressBar.setVisibility(View.VISIBLE);
        tvState.setText("正在刷新中...");
        break;
      default :
        break;
    }
  }
  /**
   * 当滚动状态改变时回调
   */
  @Override
  public void onScrollStateChanged(AbsListView view, int scrollState) {
    if (scrollState == SCROLL_STATE_IDLE
        || scrollState == SCROLL_STATE_FLING) {
      // 判断当前是否已经到了底部
      if (isScrollToBottom && !isLoadingMore) {
        isLoadingMore = true;
        // 当前到底部
        Log.i(TAG, "加载更多数据");
        footerView.setPadding(0, 0, 0, 0);
        this.setSelection(this.getCount());
        if (mOnRefershListener != null) {
          mOnRefershListener.onLoadingMore();
        }
      }
    }
  }
  /**
   * 当滚动时调用
   * 
   * @param firstVisibleItem
   *            当前屏幕显示在顶部的item的position
   * @param visibleItemCount
   *            当前屏幕显示了多少个条目的总数
   * @param totalItemCount
   *            ListView的总条目的总数
   */
  @Override
  public void onScroll(AbsListView view, int firstVisibleItem,
      int visibleItemCount, int totalItemCount) {
    firstVisibleItemPosition = firstVisibleItem;
    if (getLastVisiblePosition() == (totalItemCount - 1)) {
      isScrollToBottom = true;
    } else {
      isScrollToBottom = false;
    }
  }
  /**
   * 设置刷新监听事件
   * 
   * @param listener
   */
  public void setOnRefreshListener(OnRefreshListener listener) {
    mOnRefershListener = listener;
  }
  /**
   * 隐藏头布局
   */
  public void hideHeaderView() {
    headerView.setPadding(0, -headerViewHeight, 0, 0);
    ivArrow.setVisibility(View.VISIBLE);
    mProgressBar.setVisibility(View.GONE);
    tvState.setText("下拉刷新");
    tvLastUpdateTime.setText("最后刷新时间: " + getLastUpdateTime());
    currentState = DOWN_PULL_REFRESH;
  }
  /**
   * 隐藏脚布局
   */
  public void hideFooterView() {
    footerView.setPadding(0, -footerViewHeight, 0, 0);
    isLoadingMore = false;
  }
}
```
还有2个重要的问题就是，1.我们如何知道这个头布局的状态什么时候改变？显示，当我们将头布局完全拉下的时候，我们就应该改变头布局上面的一些描述的信息。那么，怎么判断这个头布局什么时候刷新状态呢？提供如下的算法供参考：
1，计算手指在屏幕中滑动的间距。
移动中的Y轴坐标 - 按下的Y轴坐标 = 间距
2，计算头布局距离顶部的距离
-头布局的高度 + 间距 = paddingTop
3，如果: -头布局的高度 > paddingTop的值 执行super.onTouchEvent(ev);手指继续滑动，头布局状态为“松开刷新”；
如果：-头布局的高度 < paddingTop的值，返回true，头布局状态为“下拉刷新”
4，手指抬起的时候，刷新
问题2是如何知道，脚布局滑动到了ListView的底部，即最后一个可见的item。我们可以借助 android.widget.AbsListView.OnScrollListener 接口下的两个方法onScrollStateChanged(AbsListView view, int scrollState) 和onScroll(AbsListView
 view, int firstVisibleItem,int visibleItemCount, int totalItemCount) ，现在ListView滚动时被调用的onScroll()方法中，判断当前的Item是不是最后一个Item，如果是的话就标记一个boolean的状态值，然后在监听ListView滑动状态onScrollStateChanged方法中，判断状态 scrollState == SCROLL_STATE_IDLE||
 scrollState == SCROLL_STATE_FLING ；若是，就说明ListView滑动到了底部了，这是需要上拉加载更多的数据。详细请参考上面贴出的代码，带上注释去看，也很好理解。
### 4.为ListView添加回调函数
上面的效果是实现了，主要就讲解了UI的实现，接下来，我们了解一下逻辑方法的东西。显然，这个ListView还不能动态的“下来刷新”和“上拉加载”，为什么呢？很简单啊，暂时还没有向外界暴露出一个设置数据的方法。为了实现这个能够动态实施数据更新的功能，我们需要写一个回调提供给其它的类使用，首先看一下这个回调的接口：
```java
public interface OnRefreshListener {
  /**
   * 下拉刷新
   */
  void onDownPullRefresh();
  /**
   * 上拉加载更多
   */
  void onLoadingMore();
}
```
这个回调的接口定义了两个方法，分别是“下拉刷新”和“上拉加载”，然后还必须在ListView中暴露一个接口与外面的类链接，最好的方法就暴露公共方法，例如：
publicvoid setOnRefreshListener(OnRefreshListener listener) {
mOnRefershListener = listener;
}
这样这个接口的对象就在ListView中建立了，我们只要拿着这个对象，就可以在相应的位置上调用该对象的“下拉刷新”“上拉加载”的方法了，不必在乎方法体是什么，因为具体实现的方式，具体的数据结构都是其他类中定义的，我们只要提供实现的方式即可。
### 5.使用这个自定义的ListView
使用这个自定义的ListView特别简单的，这里不多说了，看代码：
```java
public class MainActivity extends Activity implements OnRefreshListener {
  private List<String> textList;
  private MyAdapter adapter;
  private RefreshListView rListView;
  @Override
  protected void onCreate(Bundle savedInstanceState) {
    super.onCreate(savedInstanceState);
    setContentView(R.layout.activity_main);
    rListView = (RefreshListView) findViewById(R.id.refreshlistview);
    textList = new ArrayList<String>();
    for (int i = 0; i < 25; i++) {
      textList.add("这是一条ListView的数据" + i);
    }
    adapter = new MyAdapter();
    rListView.setAdapter(adapter);
    rListView.setOnRefreshListener(this);
  }
  private class MyAdapter extends BaseAdapter {
    @Override
    public int getCount() {
      // TODO Auto-generated method stub
      return textList.size();
    }
    @Override
    public Object getItem(int position) {
      // TODO Auto-generated method stub
      return textList.get(position);
    }
    @Override
    public long getItemId(int position) {
      // TODO Auto-generated method stub
      return position;
    }
    @Override
    public View getView(int position, View convertView, ViewGroup parent) {
      // TODO Auto-generated method stub
      TextView textView = new TextView(MainActivity.this);
      textView.setText(textList.get(position));
      textView.setTextColor(Color.WHITE);
      textView.setTextSize(18.0f);
      return textView;
    }
  }
  @Override
  public void onDownPullRefresh() {
    new AsyncTask<Void, Void, Void>() {
      @Override
      protected Void doInBackground(Void... params) {
        SystemClock.sleep(2000);
        for (int i = 0; i < 2; i++) {
          textList.add(0, "这是下拉刷新出来的数据" + i);
        }
        return null;
      }
      @Override
      protected void onPostExecute(Void result) {
        adapter.notifyDataSetChanged();
        rListView.hideHeaderView();
      }
    }.execute(new Void[]{});
  }
  @Override
  public void onLoadingMore() {
    new AsyncTask<Void, Void, Void>() {
      @Override
      protected Void doInBackground(Void... params) {
        SystemClock.sleep(5000);
        textList.add("这是加载更多出来的数据1");
        textList.add("这是加载更多出来的数据2");
        textList.add("这是加载更多出来的数据3");
        return null;
      }
      @Override
      protected void onPostExecute(Void result) {
        adapter.notifyDataSetChanged();
        // 控制脚布局隐藏
        rListView.hideFooterView();
      }
    }.execute(new Void[]{});
  }
}
```
我们模拟的是联网更新数据，所以必须要开启新的线程去获取数据，联网获取数据的方式有很多种，我这里使用的Android为我们提供好的AsyncTask轻量型的框架，关于这个框架，在下面有一些简单的介绍。
### 6.AsyncTask简单介绍
先来看看AsyncTask的定义：
```java
public abstract class AsyncTask<Params, Progress, Result> {
```
三种泛型类型分别代表
Params ：“启动任务执行的输入参数”，Progress：“后台任务执行的进度”，Result：“后台计算结果的类型”。在特定场合下，并不是所有类型都被使用，如果没有被使用，可以用java.lang.Void类型代替。
一个异步任务的执行一般包括以下几个步骤：
1. execute(Params... params) ，执行一个异步任务，需要我们在代码中调用此方法，触发异步任务的执行。
2. onPreExecute() ，在execute(Params... params)被调用后立即执行，一般用来在执行后台任务前对UI做一些标记。
3. doInBackground(Params... params) ，在onPreExecute()完成后立即执行，用于执行较为费时的操作，此方法将接收输入参数和返回计算结果。在执行过程中可以调用publishProgress(Progress... values)来更新进度信息。
4. onProgressUpdate(Progress... values) ，在调用publishProgress(Progress... values)时，此方法被执行，直接将进度信息更新到UI组件上。
5. onPostExecute(Result result) ，当后台操作结束时，此方法将会被调用，计算结果将做为参数传递到此方法中，直接将结果显示到UI组件上。
下面是一个图解：
![](http://img0.tuicool.com/fiqq6z.jpg!web)
通过这个草图，我们可以分析出AsyncTask三个泛型参数的用处，1，第一个泛型参数Params就是execute()方法中的参数，这二者要保持一致，因为这个参数Params会直接传递给doInBackground(Params...params)方法中，作为这个方法的参数存在。2，第二个参数Progress代表任务执行的进度，通常设定为Void类型，不使用它。3，第三个参数Result代表的是doInBackground()方法的返回值类型，这个返回值类型决定该方法在子线程中获取的是什么类型的数据，并且获取到的数据将被传递给onPostExecute()方法中作为参数，就是程序执行的结果，在该方法中拿到这个结果在UI上实现数据更新。
以上是对AsyncTask的简单介绍，没有深入，想要深入了解AsyncTask，请移步到以下这两篇博客中。
