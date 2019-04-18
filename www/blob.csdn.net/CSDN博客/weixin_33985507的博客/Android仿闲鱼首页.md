# Android仿闲鱼首页 - weixin_33985507的博客 - CSDN博客
2017年04月22日 20:50:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：38
#### 先放一个效果图
![5320369-8d8028176b601173.gif](https://upload-images.jianshu.io/upload_images/5320369-8d8028176b601173.gif)
demo.gif
> 
Demo是基于MVVM模式来编写的，欢迎大家给予批评和指正。
其中Banner的无限轮播用了PageSnapHelper，后续RecycleView也可以实现更多类似ViewPage的效果了
项目链接：[https://github.com/ly85206559/demo4Fish](https://link.jianshu.com?t=https://github.com/ly85206559/demo4Fish)
如果能对你有帮助那就最好了
#### 可以看出页面大概可以分为这几个部分
> 
1.最上面是一个轮播的Banner
2.中间可能有些其他的功能列表
3.最后是Tab页(这里是新鲜的和附近的两个列表)
#### OK，看到这样的布局需求的时候可能有两种思路
> - 整体是一个RefreshLayout布局，内嵌RecycleView，而Banner页，其他功能列表以及TabLayout都当成RecycleView的头加入到RecycleView中，TabLayout下面是真正的列表项
> - 整体还是一个RefreshLayout布局，内部是一个NestScrollView，Banner页，其他功能列表，TabLayout依次布局在NestScrollView中，然后最下面布局一个FrameLayout，TabLayout切换的时候切换不同的Fragment
> 
###### Demo中使用的是第一种方式，第二种方式考虑到SwipeRefreshLayout和内部FrameLayout的滑动会有冲突，后续再尝试编写
#### 接下来考虑需要考虑的问题
> - TabLayout需要固定到顶部
- 第一次加载数据的时候需要有个Loading提示，Demo中就是一个小鱼的空白等待页
- 因为使用一个数据集，在TabLayout来回切换的时候需要保证数据集合所在的位置是正确的(比如新鲜的这个列表当前在Position1的位置，切换到附近的列表我滑到了Position2的位置，当我再切回新鲜的时候需要回到Position1的位置)
#### 下面就一些核心的代码和思路讲解一下
首先是布局，布局很简单，SwipeRefreshLayout中包了一个FrameLayout，然后在FrameLayout中包含了一个RecycleView
```
<android.support.v4.widget.SwipeRefreshLayout
        android:id="@+id/layout_refresh"
        android:layout_width="match_parent"
        android:layout_height="match_parent">
        <FrameLayout
            android:id="@+id/container"
            android:layout_width="match_parent"
            android:layout_height="match_parent">
            <android.support.v7.widget.RecyclerView
                android:id="@+id/list"
                android:layout_width="match_parent"
                android:layout_height="match_parent" />
        </FrameLayout>
    </android.support.v4.widget.SwipeRefreshLayout>
```
接下来看下StickyHead如何实现
```
//正常的TabLayout布局
private TabLayout mTabLayout;
//粘性 TabLayout布局(用于固定在顶部)
private TabLayout mStickyTabLayout;
//粘性布局的Y坐标(用户判断粘性布局是否显示)
private int mStickyPositionY;
//主列表布局
private RecyclerView mHomeList;
```
这是变量的定义，下面的这个类是我将一些页面逻辑涉及的变量抽离出来
```
public class HomeEntity extends BaseObservable {
//列表类型 0:新鲜的 1:附近的
public static final int LIST_TYPE_FRESH = 0;
public static final int LIST_TYPE_NEAR = 1;
private int bannerCount;
private int listType = LIST_TYPE_FRESH;
//新鲜的和附近的首次加载的loading状态
private boolean refreshLoading;
private boolean nearLoading;
//首页是否正在下拉刷新
private boolean refreshing;
//新鲜的和附近的 获取更多的View的状态值(用户记录TabLayout切换的时候，LoadingMore的状态)
private int refreshMoreStatus;
private int nearMoreStatus;
//首页的活动更多的状态
private int loadingMoreStatus;
}
```
这是变量的定义，然后初始化两个TabLayout，主要在于需要监听TabLayout的切换
```
mTabLayout.addOnTabSelectedListener(new TabLayout.OnTabSelectedListener() {
        @Override
        public void onTabSelected(TabLayout.Tab tab) {
            int position = tab.getPosition();
            //设置粘贴TabLayout的选中Tab
            if (!mStickyTabLayout.getTabAt(position).isSelected()) {
                mStickyTabLayout.getTabAt(position).select();
                mViewModel.changeHomeData(position);
            }
        }
        @Override
        public void onTabUnselected(TabLayout.Tab tab) {
        }
        @Override
        public void onTabReselected(TabLayout.Tab tab) {
        }
    });
mStickyTabLayout.addOnTabSelectedListener(new TabLayout.OnTabSelectedListener() {
        @Override
        public void onTabSelected(TabLayout.Tab tab) {
            int position = tab.getPosition();
            if (!mTabLayout.getTabAt(position).isSelected()) {
                mTabLayout.getTabAt(position).select();
                mHomeList.stopScroll();
                //mAdapter.setEnableLoadMore(false);
                mViewModel.changeHomeData(position);
                ......
              }
        }
        @Override
        public void onTabUnselected(TabLayout.Tab tab) {
        }
        @Override
        public void onTabReselected(TabLayout.Tab tab) {
        }
    });
```
> 
这段的逻辑比较简单，就是实现了保持TabLayout切换状态的统一，当TabLayout切换的时候，需要将StickyTabLayout所选中的Tab也设置一下，mViewModel.changeHomeData(position)这句话是为了切换数据，下面会分析到
接下来是StickyHead重要的代码
```
mHomeList.addOnScrollListener(new RecyclerView.OnScrollListener() {
        @Override
        public void onScrolled(RecyclerView recyclerView, int dx, int dy) {
            int[] location = new int[2];
            mTabLayout.getLocationInWindow(location);
            int count = mViewContainer.getChildCount();
            if (location[1] <= mStickyPositionY) {
                if (count == 1) {
                    mViewContainer.addView(mStickyTabLayout);
                    mBinding.layoutRefresh.setEnabled(false);
                }
            } else {
                if (count > 1) {
                    mViewContainer.removeView(mStickyTabLayout);
                    //mOffsetY = DisplayUtil.dip2px(mContainer.getContext(), 46);
                    //mRefreshPosition = mAdapter.getHeaderLayoutCount();
                    //mNearPosition = mAdapter.getHeaderLayoutCount();
                    mBinding.layoutRefresh.setEnabled(true);
                }
            }
            //if (mInitPositionY == -1) {
                //mInitPositionY = location[1];
            //}
            //mHomeListPositionY = location[1];
        }
    });
```
> 
主要逻辑就是先获取TabLayout在窗口的位置，如果Y坐标小于粘贴头部的Y坐标，则将粘贴头部加入到布局中来并显示，否则，将粘贴头部布局从布局中移除。判断count这个值是为了防止重复添加和重复移除粘贴头布局。mBinding.layoutRefresh.setEnabled(true/false)是为了在粘贴头部固定在顶上的时候消除掉外层SwipeRefreshLayout的下拉刷新错误。注释掉的代码会在下面再讲
###### 只需要上面的这么多代码一个StickyHead就实现了，在测试的时候遇到点小问题，就是焦点重置导致的RecycleView重新回到初始位置的一个错误，下面是暂时的解决方案
```
LinearLayoutManager manager = new LinearLayoutManager(mContainer.getContext()) {
        @Override
        public boolean onRequestChildFocus(RecyclerView parent, RecyclerView.State state, View child, View focused) {
            //TODO 暂时处理View焦点问题
            return true;
        }
    };
```
#### 下面简单说下如何实现首次加载新鲜的或者附近的数据的时候出现的一个等待页面
> 
主要思路是这样的
- 
这个等待的LoadingView是当成RecycleView的头加在TabLayout后面的，当数据加载完成这个LoadingView设置为不可见
- 
因为有TabLayout会切换，导致RecycleView的数据会重新绘制，进而导致RecyView会回到初始位置，所以需要记录下RecycleView所在的位置，然后手动滑动到记录的位置
具体的我们还是来看代码吧
private int mHomeListPositionY;//用来标识当前RecycleView的位置
private int mInitPositionY = -1;//初始状态下RecycleView的Y坐标
//这里是RecycleView的滑动监听，用来记录RecycleView的位置，这里其实是记录了mTabLayout的位置
mHomeList.addOnScrollListener(new RecyclerView.OnScrollListener() {
@Override
public void onScrolled(RecyclerView recyclerView, int dx, int dy) {
int[] location = new int[2];
mTabLayout.getLocationInWindow(location);
int count = mViewContainer.getChildCount();
```
if (mInitPositionY == -1) {
             mInitPositionY = location[1];
         }
         mHomeListPositionY = location[1];
     }
 });
```
//这个函数就是用来手动将RecycleView滑动到正确的位置
private void setLoadingView(boolean visible, int type) {
int position;
if (type == HomeEntity.LIST_TYPE_FRESH) {
position = mRefreshPosition;
} else {
position = mNearPosition;
}
if (visible) {
mLoadingView.setVisibility(View.VISIBLE);
if (mStickyTabLayout.getVisibility() == View.VISIBLE) {
LinearLayoutManager layoutManager = (LinearLayoutManager) mHomeList.getLayoutManager();
layoutManager.scrollToPositionWithOffset(0, mHomeListPositionY - mInitPositionY);
}
} else {
mLoadingView.setVisibility(View.GONE);
LinearLayoutManager layoutManager = (LinearLayoutManager) mHomeList.getLayoutManager();
if (mViewContainer.getChildCount() > 1) {
layoutManager.scrollToPositionWithOffset(position, mStickyTabLayout.getHeight());
} else {
layoutManager.scrollToPositionWithOffset(0, mHomeListPositionY - mInitPositionY);
}
}
}
#### 最后来看下新鲜的和附近的加载更多时页面的实现
> 
这里Adapter使用了第三方BRVAH，所以相对LoadingMore的状态BRVAH帮我封了一下，因为虽然是一个List，但其实是两个列表复用一个List的，所以这里的LoadingMore状态我们需要记录两个，方便切换的时候列表的LoadingMore状态是正确的，下面看下主要代码
```
if (propertyId == BR.refreshLoading) {
            if (HomeEntity.LIST_TYPE_FRESH != entity.getListType()) {
                return;
            }
            if (mLoadingView.getVisibility() == View.GONE) {
                mAdapter.setEnableLoadMore(true);
            }
        } else if (propertyId == BR.nearLoading) {
            if (HomeEntity.LIST_TYPE_NEAR != entity.getListType()) {
                return;
            }
            if (mLoadingView.getVisibility() == View.GONE) {
                mAdapter.setEnableLoadMore(true);
            }
        } else if (propertyId == BR.loadingMoreStatus) {
            int status = entity.getLoadingMoreStatus();
            mAdapter.setEnableLoadMore(true);
            if (LoadMoreView.STATUS_DEFAULT == status) {
                mAdapter.loadMoreComplete();
            } else if (LoadMoreView.STATUS_END == status) {
                mAdapter.loadMoreEnd();
            } else if (LoadMoreView.STATUS_FAIL == status) {
                mAdapter.loadMoreFail();
            }
        }
```
> 
BR.refreshLoading和BR.nearLoading 都是监听首次加载，这里
if (mLoadingView.getVisibility() == View.GONE) {
mAdapter.setEnableLoadMore(true);
}
这个是为了防止首次加载显示Loading页面的时候又显示了LoadingMore布局
BR.loadingMoreStatus这个就是监听LoadingMore的状态来更新List的Adapter
#### 其他的主要ViewModel代码在HomeViewModel中。主要的几个点
- 粘贴头布局的逻辑
- TabLayout切换导致数据集变化以及位置的变化
- 加载更多的时候需要考虑TabLayout切换的问题
