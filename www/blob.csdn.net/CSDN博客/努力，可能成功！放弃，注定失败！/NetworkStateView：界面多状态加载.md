# NetworkStateView：界面多状态加载 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2017年04月17日 09:58:49[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：410
本文来自简书，原文地址:[http://www.jianshu.com/p/858d41972d15](http://www.jianshu.com/p/858d41972d15)
在项目中经常需要进行不同状态的加载，例如在网络请求时的加载中状态，加载失败状态，没有网络状态和没有数据的状态等，之前在项目中的做法是把几个不同的状态布局都添加到需要进行状态切换的Activity或Fragment的布局文件当中，接着再对每一个状态界面进行相应的隐藏显示，但是在界面一多的情况下，重复操作就会显得很繁琐。
在进行了无数次这样繁琐的操作后，有些受不了了，就想着能不能把这几种状态都封装到同一个View中，在需要显示不同的状态时只需要调用相应的状态方法就可以进行切换，这样可比上一种方法简便得多，哈哈，这当然是可以的，接下来就介绍一下NetworkStateView。
NetworkStateView继承自LinearLayout，在里面定义了加载成功，加载中，加载出错(这里只统一定义为网络出错，当然了用在哪种出错方式上可以由你自己决定)，没有网络，没有数据五种状态，其中加载成功表示用来显示Activity或Fragment的界面，并用变量mCurrentState来记住当前显示的状态，相应的变量值如下：
```
//当前的加载状态
private int mCurrentState;
private static final int STATE_SUCCESS = 0;
private static final int STATE_LOADING = 1;
private static final int STATE_NETWORK_ERROR = 2;
private static final int STATE_NO_NETWORK = 3;
private static final int STATE_EMPTY = 4;
```
接着需要自定义属性，用于传入对应的状态布局文件，在同一种状态中如果需要有不同的界面显示，便可以对应的传入layout文件，这样可以方便扩展
```xml
<declare-styleable name="NetworkStateView">
    <!-- 加载中的布局id -->
    <attr name="loadingView" format="reference" />
    <!-- 加载错误的布局id -->
    <attr name="errorView" format="reference" />
    <!-- 加载错误的布局图片 -->
    <attr name="nsvErrorImage" format="reference" />
    <!-- 加载错误的布局文字 -->
    <attr name="nsvErrorText" format="string" />
    <!-- 没有数据的布局id -->
    <attr name="emptyView" format="reference" />
    <!-- 没有数据的布局图片 -->
    <attr name="nsvEmptyImage" format="reference" />
    <!-- 没有数据的布局文字 -->
    <attr name="nsvEmptyText" format="string" />
    <!-- 没有网络的布局id -->
    <attr name="noNetworkView" format="reference" />
    <!-- 没有数据的布局图片 -->
    <attr name="nsvNoNetworkImage" format="reference" />
    <!-- 没有数据的布局文字 -->
    <attr name="nsvNoNetworkText" format="string" />
    <!-- 刷新的ImageView图片id -->
    <attr name="nsvRefreshImage" format="reference"/>
    <!-- 文字大小 -->
    <attr name="nsvTextSize" format="dimension" />
    <!-- 文字颜色 -->
    <attr name="nsvTextColor" format="color" />
</declare-styleable>
```
定义了属性之后，需要在NetworkStateView的构造函数中使用TypedArray进行相应属性值的查找，注意，这里查找得到时布局文件的id，最后在显示的时候需要对布局文件id进行相应的inflate，查找之后可以进行一些基本属性的设置，例如LayoutParams和BackgroundColor等
```
public NetworkStateView(@NonNull Context context, @Nullable AttributeSet attrs, @AttrRes int defStyleAttr) {
    super(context, attrs, defStyleAttr);
    TypedArray typedArray = context.obtainStyledAttributes(attrs, R.styleable.NetworkStateView, defStyleAttr, 0);
    mLoadingViewId = typedArray.getResourceId(R.styleable.NetworkStateView_loadingView, R.layout.view_loading);
    mErrorViewId = typedArray.getResourceId(R.styleable.NetworkStateView_errorView, R.layout.view_network_error);
    mNoNetworkViewId = typedArray.getResourceId(R.styleable.NetworkStateView_noNetworkView, R.layout.view_no_network);
    mEmptyViewId = typedArray.getResourceId(R.styleable.NetworkStateView_emptyView, R.layout.view_empty);
   ....
    typedArray.recycle();
    mInflater = LayoutInflater.from(context);
    params = new ViewGroup.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.MATCH_PARENT);
    setBackgroundColor(getResources().getColor(R.color.white));
}
```
在使用属性时，可以直接在NetworkStateView的布局文件中进行设置，又或者在styles文件中进行设置
- 
直接在布局文件中声明
```
<com.zht.networkstateview.ui.widget.NetworkStateView xmlns:android="http://schemas.android.com/apk/res/android"
      xmlns:app="http://schemas.android.com/apk/res-auto"
      android:id="@+id/nsv_state_view"
      android:layout_width="match_parent"
      android:layout_height="match_parent"
      android:layout_centerInParent="true"
      android:orientation="vertical"
      android:visibility="visible"
      app:emptyView="@layout/view_empty"
      app:errorView="@layout/view_network_error"
      app:loadingView="@layout/view_loading"
      app:noNetworkView="@layout/view_no_network"
      app:nsvTextColor="@color/gray_text_default"
      app:nsvTextSize="16sp">
  </com.zht.networkstateview.ui.widget.NetworkStateView>
```
- 
在styles文件进行设置
```xml
<style name="AppTheme" parent="Theme.AppCompat.Light.DarkActionBar">
      <!-- Customize your theme here. -->
      <item name="styleNetworkStateView">@style/NetworkStateViewTheme</item>
      ...
  </style>
  <style name="NetworkStateViewTheme" parent="NetworkStateView.Style">
      <item name="nsvTextSize">16sp</item>
      <item name="nsvTextColor">#ffffff</item>
      ...
  </style>
```
进行上面的步骤后，就可以加载相应的布局文件了，以加载失败(网络出错)为例，先定义一个showError方法，在一开始需要将当前状态置为STATE_NETWORK_ERROR，接着inflate布局，注意，inflate之后需要进行addView将加载失败(网络出错)状态的View添加到NetworkStateView中，这样才可以进行相应的显示隐藏操作
```
public void showError() {
    mCurrentState = STATE_NETWORK_ERROR;
    if (null == mErrorView) {
        mErrorView = mInflater.inflate(mErrorViewId, null);
        addView(mErrorView, 0, params);
    }
    showViewByState(mCurrentState);
}
```
showViewByState方法就是根据当前的状态来进行相应的View的切换
```
private void showViewByState(int state) {
    //如果当前状态为加载成功，隐藏此View，反之显示
    this.setVisibility(state == STATE_SUCCESS ? View.GONE : View.VISIBLE);
    if (null != mLoadingView) {
        mLoadingView.setVisibility(state == STATE_LOADING ? View.VISIBLE : View.GONE);
    }
    if (null != mErrorView) {
        mErrorView.setVisibility(state == STATE_NETWORK_ERROR ? View.VISIBLE : View.GONE);
    }
    if (null != mNoNetworkView) {
        mNoNetworkView.setVisibility(state == STATE_NO_NETWORK ? View.VISIBLE : View.GONE);
    }
    if (null != mEmptyView) {
        mEmptyView.setVisibility(state == STATE_EMPTY ? View.VISIBLE : View.GONE);
    }
}
```
嗯...到这里其实也差不多了，不过还有一个问题，就是在加载失败之后需要进行刷新重新请求网络怎么办？哈哈，这当然也是可以解决的，我们只需要在定义一个刷新按钮，并对外提供一个接口进行调用就可以了，相应的接口及方法为
```
public void setOnRefreshListener(OnRefreshListener listener) {
    mRefreshListener = listener;
}
public interface OnRefreshListener {
    void onRefresh();
}
```
那么showError可以改造如下
```
public void showError() {
    mCurrentState = STATE_NETWORK_ERROR;
    if (null == mErrorView) {
        mErrorView = mInflater.inflate(mErrorViewId, null);
        View errorRefreshView = mErrorView.findViewById(R.id.error_refresh_view);
        if (null != errorRefreshView) {
            errorRefreshView.setOnClickListener(new OnClickListener() {
                @Override
                public void onClick(View view) {
                    if (null != mRefreshListener) {
                        mRefreshListener.onRefresh();
                    }
                }
            });
        }
        addView(mErrorView, 0, params);
    }
    showViewByState(mCurrentState);
}
```
嗯，这样就可以把多种状态的View统一封装在同一个View当中，我们可以在Activity的布局文件中通过`include`标签加入`NetworkStateView`，接着我们只要调用`NetworkStateView`的相关方法就可以进行多种状态的切换了
详细的代码以及Sample可以去[我的github](https://github.com/AlarmZeng/NetworkStateView)查看，觉得还可以的不妨Star或follow
古语有云：不懒的程序员不是好程序员，有人会觉得这样也只是比第一种方法稍微简便一些，但也是需要在每个界面的布局文件中通过`include`标签进行添加，并且进行`findViewById`的操作然后才能调用相关方法，能不能进行统一的设置呢？可以不需要在每个界面中进行`include`，统一设置之后就可以调用相关方法，这当然也是可以的，我们可以在BaseActivity进行相应的设置，这样子类Activity只需要调用BaseActivity的方法就可以，至于怎么进行设置，请等下篇讲解。。。
