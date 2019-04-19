# 万能的RecyclerViewAdapter，支持上拉加载、添加HeadView - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年10月09日 17:06:34[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：925
> 
最近的项目，使用RecyclerView比较多，导致需要写大量的Adapter和ViewHolder。不想偷懒的程序员不是好程序员。于是，查了一些资料，对RecyclerView的Adapter进行了一些封装。使用的时候减少了大量的工作。
先来点效果图
![](http://upload-images.jianshu.io/upload_images/2964446-a4bb746e768823a2.gif?imageMogr2/auto-orient/strip)
1.gif
![](http://upload-images.jianshu.io/upload_images/2964446-bef8fea172a29779.gif?imageMogr2/auto-orient/strip)
2.gif
封装后的Adapter使用：
```
class DragSwipeAdapter extends BaseAdapter<String> {
       public DragSwipeAdapter(Context mContext, List<String> mDatas, int mLayoutId) {
           super(mContext, mDatas, mLayoutId);
       }
       @Override
       protected void convert(Context mContext, BaseViewHolder holder, String s) {
            holder.setText(R.id.tv,s);
       }
   }
```
只要传入数据源和item布局即可，是不是超级简单。
下面看看怎么实现的吧。
# 通用ViewHolder
```java
```java
package org.raphets.demorecyclerview.adapter;
import android.annotation.SuppressLint;
import android.graphics.Bitmap;
import android.graphics.drawable.Drawable;
import android.os.Build;
import android.support.v7.widget.RecyclerView;
import android.text.SpannableStringBuilder;
import android.util.SparseArray;
import android.view.View;
import android.view.animation.AlphaAnimation;
import android.widget.Checkable;
import android.widget.ImageView;
import android.widget.TextView;
public class BaseViewHolder extends RecyclerView.ViewHolder {
    SparseArray<View> mViews;
    View mItemView;
    public BaseViewHolder(View itemView) {
        super(itemView);
        mItemView = itemView;
        mViews = new SparseArray<>();
    }
    public <T extends View> T getView(int viewId) {
        View view = mViews.get(viewId);
        if (view == null) {
            view = mItemView.findViewById(viewId);
            mViews.put(viewId, view);
        }
        return (T) view;
    }
    public BaseViewHolder setText(int viewId, int resId) {
        TextView textView = getView(viewId);
        textView.setText(resId);
        return this;
    }
    public BaseViewHolder setText(int viewId, String text) {
        TextView textView = getView(viewId);
        textView.setText(text);
        return this;
    }
    public BaseViewHolder setText(int viewId, SpannableStringBuilder text) {
        TextView textView = getView(viewId);
        textView.setText(text);
        return this;
    }
    public BaseViewHolder setImageResource(int viewId, int resId) {
        ImageView view = getView(viewId);
        view.setImageResource(resId);
        return this;
    }
    public BaseViewHolder setImageBitmap(int viewId, Bitmap bitmap) {
        ImageView view = getView(viewId);
        view.setImageBitmap(bitmap);
        return this;
    }
    public BaseViewHolder setImageDrawable(int viewId, Drawable drawable) {
        ImageView view = getView(viewId);
        view.setImageDrawable(drawable);
        return this;
    }
    public BaseViewHolder setBackgroundColor(int viewId, int color) {
        View view = getView(viewId);
        view.setBackgroundColor(color);
        return this;
    }
    public BaseViewHolder setBackgroundResource(int viewId, int backgroundRes) {
        View view = getView(viewId);
        view.setBackgroundResource(backgroundRes);
        return this;
    }
    public BaseViewHolder setTextColor(int viewId, int textColor) {
        TextView view = getView(viewId);
        view.setTextColor(textColor);
        return this;
    }
    @SuppressLint("NewApi")
    public BaseViewHolder setAlpha(int viewId, float value) {
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.HONEYCOMB) {
            getView(viewId).setAlpha(value);
        } else {
            // Pre-honeycomb hack to set Alpha value
            AlphaAnimation alpha = new AlphaAnimation(value, value);
            alpha.setDuration(0);
            alpha.setFillAfter(true);
            getView(viewId).startAnimation(alpha);
        }
        return this;
    }
    public BaseViewHolder setVisible(int viewId, boolean visible) {
        View view = getView(viewId);
        view.setVisibility(visible ? View.VISIBLE : View.GONE);
        return this;
    }
    public BaseViewHolder setTag(int viewId, Object tag) {
        View view = getView(viewId);
        view.setTag(tag);
        return this;
    }
    public BaseViewHolder setTag(int viewId, int key, Object tag) {
        View view = getView(viewId);
        view.setTag(key, tag);
        return this;
    }
    public BaseViewHolder setChecked(int viewId, boolean checked) {
        Checkable view = (Checkable) getView(viewId);
        view.setChecked(checked);
        return this;
    }
    /**
     * 关于事件监听
     */
    public BaseViewHolder setOnClickListener(int viewId, View.OnClickListener listener) {
        View view = getView(viewId);
        view.setOnClickListener(listener);
        return this;
    }
    public BaseViewHolder setOnTouchListener(int viewId, View.OnTouchListener listener) {
        View view = getView(viewId);
        view.setOnTouchListener(listener);
        return this;
    }
    public BaseViewHolder setOnLongClickListener(int viewId, View.OnLongClickListener listener) {
        View view = getView(viewId);
        view.setOnLongClickListener(listener);
        return this;
    }
}
```
```
这里对TextView、ImageView等常用控件的一些常用方法进行了封装。有了这个通用ViewHolder，再多的Adapter也只需要这一个ViewHolder，不用每写一个Adapter就要写一个ViewHolder。
# 通用Adapter
```java
```java
package org.raphets.demorecyclerview.adapter;
import android.content.Context;
import android.support.v7.util.DiffUtil;
import android.support.v7.widget.RecyclerView;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import java.util.List;
/**
 * Created by RaphetS on 2016/9/28.
 * 普通的万能Adapter
 * 支持onItemClick
 * 支持onLongItemClick
 */
public abstract class BaseAdapter<T> extends RecyclerView.Adapter<BaseViewHolder> {
    private Context mContext;
    private List<T> mDatas;
    private int mLayoutId;
    private OnItemClickListener mItemClickListener;
    private onLongItemClickListener mLongItemClickListener;
    public BaseAdapter(Context mContext, List<T> mDatas, int mLayoutId) {
        this.mContext = mContext;
        this.mDatas = mDatas;
        this.mLayoutId = mLayoutId;
    }
    public void updateData(List<T> data) {
        mDatas.clear();
        mDatas.addAll(data);
        notifyDataSetChanged();
    }
    public void addAll(List<T> data) {
        mDatas.addAll(data);
        notifyDataSetChanged();
    }
    @Override
    public BaseViewHolder onCreateViewHolder(ViewGroup parent, int viewType) {
        View view = LayoutInflater.from(mContext).inflate(mLayoutId, parent, false);
        BaseViewHolder holder = new BaseViewHolder(view);
        return holder;
    }
    @Override
    public int getItemCount() {
        return mDatas.size();
    }
    @Override
    public void onBindViewHolder(BaseViewHolder holder, final int position) {
        convert(mContext, holder, mDatas.get(position));
        if (mItemClickListener != null) {
            holder.mItemView.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View v) {
                    mItemClickListener.onItemClick(v, position);
                }
            });
        }
        if (mLongItemClickListener != null) {
            holder.itemView.setOnLongClickListener(new View.OnLongClickListener() {
                @Override
                public boolean onLongClick(View v) {
                    mLongItemClickListener.onLongItemClick(v, position);
                    return true;
                }
            });
        }
    }
    protected abstract void convert(Context mContext, BaseViewHolder holder, T t);
    public    interface OnItemClickListener {
        void onItemClick(View view, int position);
    }
    public    interface onLongItemClickListener {
        void onLongItemClick(View view, int postion);
    }
    public void setOnItemClickListener(OnItemClickListener listener) {
        this.mItemClickListener = listener;
    }
    public void setonLongItemClickListener(onLongItemClickListener listener) {
        this.mLongItemClickListener = listener;
    }
}
```
```
这里对RecyclerViewAdapter通用的地方封装了一下，然后封装了一下onItemClick和onLongItemClick事件。
# 如何使用
首先，写一个Adapter继承通用Adapter
```
class DragSwipeAdapter extends BaseAdapter<String> {
       public DragSwipeAdapter(Context mContext, List<String> mDatas, int mLayoutId) {
           super(mContext, mDatas, mLayoutId);
       }
       @Override
       protected void convert(Context mContext, BaseViewHolder holder, String s) {
            holder.setText(R.id.tv,s)
            .setTextColor(R.id.tv, Color.RED)
            .setImageResource(R.id.imageView,R.mipmap.ic_launcher);
           ImageView imageView=holder.getView(R.id.imageView); 
           imageView.setImageResource(R.mipmap.ic_launcher);
       }
   }
```
代码比起原生的是不是少很多？
然后在Activity中使用即可。只要传入Context，数据源，item布局即可。
```
mAdapter = new DragSwipeAdapter(this, mData, R.layout.item);
 mRecyclerView.setLayoutManager(new LinearLayoutManager(this));
 mRecyclerView.setAdapter(mAdapter);
```
是不是少了大量的工作。
> 
但是貌似封装的还不够，我们的RecyclerView经常需要使用的功能还有下拉刷新，上拉加载，加个headview等等。下拉刷新好说，直接使用v4包里的SwipeRefreshLayout即可，上拉加载和HeadView怎么办呢？
# 下面我对上拉加载和HeadView也进行了一下封装。
（目前只支持LinearLayoutManager布局的RecyclerView）
- 
##### 首先ViewHoder不变，依然是上面的ViewHolder。
- 
##### 然后写了一个底部加载中的一个item布局。
```xml
<?xml version="1.0" encoding="utf-8"?>
<LinearLayout xmlns:android="http://schemas.android.com/apk/res/android"
  android:orientation="horizontal" android:layout_width="match_parent"
  android:gravity="center"
  android:padding="20dp"
  android:layout_height="wrap_content">
<ProgressBar
  style="@android:style/Widget.ProgressBar.Small"
  android:id="@+id/pb"
  android:layout_width="wrap_content"
  android:layout_height="wrap_content" />
  <TextView
      android:layout_marginLeft="10dp"
      android:text="正在加载..."
      android:layout_width="wrap_content"
      android:layout_height="wrap_content" />
</LinearLayout>
```
- 
##### 下面是封装的Adapter
```java
```java
package org.raphets.demorecyclerview.adapter;
import android.content.Context;
import android.support.v7.widget.LinearLayoutManager;
import android.support.v7.widget.RecyclerView;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import org.raphets.demorecyclerview.R;
import java.util.List;
/**
 * Created by RaphetS on 2016/10/1.
 *  支持上拉加载
 *  支持添加HeadView
 *  底部有进度条
 */
public abstract class BaseLoadMoreHeaderAdapter<T> extends RecyclerView.Adapter {
    private Context mContext;
    private boolean isLoading=false;
    private OnLoadMoreListener mOnLoadMoreListener;
    private OnItemClickListener mItemClickListener;
    private onLongItemClickListener mLongItemClickListener;
    private List<T> mDatas;
    private int mLayoutId;
    private View mHeadView;
    private final static int TYPE_HEADVIEW=100;
    private final static int TYPE_ITEM=101;
    private final static int TYPE_PROGRESS=102;
    public BaseLoadMoreHeaderAdapter(Context mContext, RecyclerView recyclerView, List<T> mDatas, int mLayoutId) {
        this.mContext = mContext;
        this.mDatas = mDatas;
        this.mLayoutId = mLayoutId;
        init(recyclerView);
    }
    private void init(RecyclerView recyclerView) {
        //mRecyclerView添加滑动事件监听
        recyclerView.addOnScrollListener(new RecyclerView.OnScrollListener() {
            @Override
            public void onScrolled(RecyclerView recyclerView, int dx, int dy) {
                super.onScrolled(recyclerView, dx, dy);
                LinearLayoutManager linearLayoutManager = (LinearLayoutManager) recyclerView.getLayoutManager();
                int    totalItemCount = linearLayoutManager.getItemCount();
                int    lastVisibleItemPosition = linearLayoutManager.findLastVisibleItemPosition();
                if (!isLoading &&dy>0&&lastVisibleItemPosition>=totalItemCount-1) {
                    //此时是刷新状态
                    if (mOnLoadMoreListener != null) {
                        mOnLoadMoreListener.onLoadMore();
                    }
                    isLoading = true;
                }
            }
        });
    }
    public void updateData(List<T> data) {
        mDatas.clear();
        mDatas.addAll(data);
        notifyDataSetChanged();
    }
    public void addAll(List<T> data) {
        mDatas.addAll(data);
        notifyDataSetChanged();
    }
    public void addHeadView(View headView){
        mHeadView=headView;
    }
    @Override
    public RecyclerView.ViewHolder onCreateViewHolder(ViewGroup parent, int viewType) {
        if (viewType==TYPE_ITEM){
            View itemView= LayoutInflater.from(mContext).inflate(mLayoutId,parent,false);
            BaseViewHolder baseViewHolder=new BaseViewHolder(itemView);
            return baseViewHolder;
        }else if (viewType==TYPE_HEADVIEW){
            HeadViewHolder headViewHolder=new HeadViewHolder(mHeadView);
            return headViewHolder;
        } else{
            View progressView=LayoutInflater.from(mContext).inflate(R.layout.progress_item,parent,false);
            ProgressViewHolder progressViewHolder= new ProgressViewHolder(progressView);
            return progressViewHolder;
        }
    }
    @Override
    public void onBindViewHolder(RecyclerView.ViewHolder holder, final int position) {
      if (holder instanceof BaseViewHolder){
          convert(mContext, holder, mDatas.get(position));
          ((BaseViewHolder) holder).mItemView.setOnClickListener(new View.OnClickListener() {
              @Override
              public void onClick(View v) {
                  mItemClickListener.onItemClick(v,position-1);
              }
          });
          ((BaseViewHolder) holder).mItemView.setOnLongClickListener(new View.OnLongClickListener() {
              @Override
              public boolean onLongClick(View v) {
                  mLongItemClickListener.onLongItemClick(v,position-1);
                  return true;
              }
          });
      }
    }
    @Override
    public int getItemViewType(int position) {
        if (mHeadView!=null){
            if (position==getItemCount()-1){
                return TYPE_PROGRESS;
            }else if (position==0){
                return TYPE_HEADVIEW;
            }else {
                return TYPE_ITEM;
           }
        }else {
            if (position==getItemCount()-1){
                return TYPE_PROGRESS;
            }else {
                return TYPE_ITEM;
            }
        }
    }
    public abstract void convert(Context mContext, RecyclerView.ViewHolder holder, T t);
    @Override
    public int getItemCount() {
        return mDatas.size()+1;
    }
    public void setLoading(boolean b){
        isLoading=b;
    }
    public    interface OnItemClickListener {
        void onItemClick(View view, int position);
    }
    public    interface onLongItemClickListener {
        void onLongItemClick(View view, int postion);
    }
    public void setOnItemClickListener(OnItemClickListener listener) {
        this.mItemClickListener = listener;
    }
    public void setonLongItemClickListener(onLongItemClickListener listener) {
        this.mLongItemClickListener = listener;
    }
    public void setOnLoadMoreListener(OnLoadMoreListener listener){
        this.mOnLoadMoreListener=listener;
    }
    public interface OnLoadMoreListener{
        void onLoadMore();
    }
    public class ProgressViewHolder extends RecyclerView.ViewHolder{
        public ProgressViewHolder(View itemView) {
            super(itemView);
        }
    }
    public class HeadViewHolder extends RecyclerView.ViewHolder{
        public HeadViewHolder(View itemView) {
            super(itemView);
        }
    }
}
```
```
上拉加载和HeadView都是通过ItemViewType实现的，通过不同的position得到不同的ItemViewType，然后在onCreateViewHolder()的时候通过不同的ItemViewType加载不同的布局，创建不同的ViewHolder。
- 
##### 使用实例
1、首先写一个Adapter继承BaseLoadMoreHeaderAdapter，注意要判断holder的类型
```
class MyAdapter  extends BaseLoadMoreHeaderAdapter<String> {
      public MyAdapter(Context mContext, RecyclerView recyclerView, List<String> mDatas, int mLayoutId) {
          super(mContext, recyclerView, mDatas, mLayoutId);
      }
      @Override
      public void convert(Context mContext, RecyclerView.ViewHolder holder, String s) {
          if (holder instanceof BaseViewHolder){
              ((BaseViewHolder) holder).setText(R.id.tv,s);
          }
      }
  }
```
2、在Activity中设置Adapter
```java
```java
LinearLayoutManager layoutManager=new LinearLayoutManager(this);
      mAdapter=new MyAdapter(this,mRecyclerView,mDatas,R.layout.item);
      mRecyclerView.setLayoutManager(layoutManager);
      View headView= LayoutInflater.from(this).inflate(R.layout.headview,mRecyclerView,false);
      mAdapter.addHeadView(headView);
      mRecyclerView.setAdapter(mAdapter);
    mAdapter.setOnLoadMoreListener(new BaseLoadMoreHeaderAdapter.OnLoadMoreListener() {
          @Override
          public void onLoadMore() {
              loadMore();
          }
      });
      mAdapter.setOnItemClickListener(new BaseLoadMoreHeaderAdapter.OnItemClickListener() {
          @Override
          public void onItemClick(View view, int position) {
              Toast.makeText(getApplicationContext(),position+"",Toast.LENGTH_SHORT).show();
          }
      });
```
```
注意这里，需要先给RecyclerView设置LayoutManager，在加载HeadView（View headView= LayoutInflater.from(this).inflate(R.layout.headview,mRecyclerView,false);）
否则会报错。
# Demo地址
[https://github.com/RaphetS/RecyclerView](https://github.com/RaphetS/RecyclerView)
文／RaphetS（简书作者）
原文链接：http://www.jianshu.com/p/8ca8bb6705e2
著作权归作者所有，转载请联系作者获得授权，并标注“简书作者”。
