# RecyclerView 源码深入解析——绘制流程、缓存机制、动画等 - 我的学习笔记 - CSDN博客





2018年11月10日 16:04:00[zouzhiheng](https://me.csdn.net/u011330638)阅读数：478








# 前言

本文打算对 RecyclerView 做一个详细完整的、重点突出的分析与总结，因为 RecycelrView 源码很长(光 RecyclerView 文件本身就有 13000+ 行)，因此文章也会很长，但一通分析下来后会发现，RecyclerView 虽然是 ListView 的加强版，除了在使用方法上类似之外，关键源码上也是非常类似的。

RecyclerView 的使用可以参考大神的文章：

[Android RecyclerView 使用完全解析 体验艺术般的控件](https://blog.csdn.net/lmj623565791/article/details/45059587)

[RecyclerView 和 ListView 使用对比分析](https://www.jianshu.com/p/f592f3715ae2?utm_campaign=haruki&utm_content=note&utm_medium=reader_share&utm_source=weixin&from=singlemessage&isappinstalled=1)

本文采用“自顶向下”的源码分析法——即把相关的代码按照调用关系自顶向下排列，同一个类的方法放在一起，关键的代码使用注释标注。废话不多说，下面从 RecyclerView 的绘制流程开始分析。

PS：CSDN 的代码编辑器实在不知道怎么设置，觉得丑的可以到[简书](https://www.jianshu.com/p/ece54fc4460a)或[掘金](https://juejin.im/post/5be68d8fe51d45117850e45c)上看

# 绘制流程

先从 onMeasure 方法看起，关键的地方都用注释标出来了：

```
public class RecyclerView extends ViewGroup implements ScrollingView, NestedScrollingChild2 {

    Adapter mAdapter;
    @VisibleForTesting LayoutManager mLayout;

    @Override
    protected void onMeasure(int widthSpec, int heightSpec) {
        if (mLayout == null) {
            defaultOnMeasure(widthSpec, heightSpec);
            return;
        }
        if (mLayout.isAutoMeasureEnabled()) { // 看这里就行了
            final int widthMode = MeasureSpec.getMode(widthSpec);
            final int heightMode = MeasureSpec.getMode(heightSpec);

            mLayout.onMeasure(mRecycler, mState, widthSpec, heightSpec);

			// 关键步骤 1
            // 默认为 start
            if (mState.mLayoutStep == State.STEP_START) {
                dispatchLayoutStep1();
            }
            
            // 关键步骤 2
            dispatchLayoutStep2();

            // now we can get the width and height from the children.
            mLayout.setMeasuredDimensionFromChildren(widthSpec, heightSpec);

            // if RecyclerView has non-exact width and height and if there is at least one child
            // which also has non-exact width & height, we have to re-measure.
            if (mLayout.shouldMeasureTwice()) {
                ...
                dispatchLayoutStep2();
                // now we can get the width and height from the children.
                mLayout.setMeasuredDimensionFromChildren(widthSpec, heightSpec);
            }
        } else {
            if (mHasFixedSize) {
                mLayout.onMeasure(mRecycler, mState, widthSpec, heightSpec);
                return;
            }
            // custom onMeasure
            if (mAdapterUpdateDuringMeasure) {
                ...
            } else if (mState.mRunPredictiveAnimations) {
                setMeasuredDimension(getMeasuredWidth(), getMeasuredHeight());
                return;
            }

            mLayout.onMeasure(mRecycler, mState, widthSpec, heightSpec);
            ...
        }
    }
    
    void defaultOnMeasure(int widthSpec, int heightSpec) {
        // calling LayoutManager here is not pretty but that API is already public and it is better
        // than creating another method since this is internal.
        final int width = LayoutManager.chooseSize(widthSpec,
                getPaddingLeft() + getPaddingRight(),
                ViewCompat.getMinimumWidth(this));
        final int height = LayoutManager.chooseSize(heightSpec,
                getPaddingTop() + getPaddingBottom(),
                ViewCompat.getMinimumHeight(this));

        setMeasuredDimension(width, height);
    }
    
    public abstract static class LayoutManager {
    
        public void onMeasure(Recycler recycler, State state, int widthSpec, int heightSpec) {
            mRecyclerView.defaultOnMeasure(widthSpec, heightSpec);
        }
        
    }
    
}
```

mLayout.isAutoMeasureEnabled() 的返回值默认为 false，但 LinearLayoutManager 和 StaggeredGridLayoutManager 重写了这个方法，且默认为 true：

```
public class LinearLayoutManager extends RecyclerView.LayoutManager implements
        ItemTouchHelper.ViewDropHandler, RecyclerView.SmoothScroller.ScrollVectorProvider {

    @Override
    public boolean isAutoMeasureEnabled() {
        return true;
    }
    
}
```

```
public class StaggeredGridLayoutManager extends RecyclerView.LayoutManager implements
        RecyclerView.SmoothScroller.ScrollVectorProvider {

    public static final int GAP_HANDLING_NONE = 0;
    public static final int GAP_HANDLING_LAZY = 1;
    public static final int GAP_HANDLING_MOVE_ITEMS_BETWEEN_SPANS = 2;

    private int mGapStrategy = GAP_HANDLING_MOVE_ITEMS_BETWEEN_SPANS;
    
    public void setGapStrategy(int gapStrategy) {
        ...
        mGapStrategy = gapStrategy;
        requestLayout();
    }

    @Override
    public boolean isAutoMeasureEnabled() {
        return mGapStrategy != GAP_HANDLING_NONE;
    }
    
}
```

而 GridLayoutManager 继承自 LinearLayoutManager，即 Android 提供的 LayoutManager 的三个子类，都会走 if 分支里的代码，里面最重要的两个调用是 dispatchLayoutStep1 和 dispatchLayoutStep2，在开始分析这两个方法之前，先看一下 onLayout 方法：

```
public class RecyclerView extends ViewGroup implements ScrollingView, NestedScrollingChild2 {

    @Override
    protected void onLayout(boolean changed, int l, int t, int r, int b) {
        dispatchLayout();
        mFirstLayoutComplete = true;
    }
    
    /**
     * Wrapper around layoutChildren() that handles animating changes caused by layout.
     */
    void dispatchLayout() {
        mState.mIsMeasuring = false;
        if (mState.mLayoutStep == State.STEP_START) { 
            // onMeasure 已经执行了 step1 和 step2，正常情况下不会走这个分支
            dispatchLayoutStep1();
            mLayout.setExactMeasureSpecsFrom(this);
            dispatchLayoutStep2();
        } else if (mAdapterHelper.hasUpdates() || mLayout.getWidth() != getWidth()
                || mLayout.getHeight() != getHeight()) {
            // adpter 更新了，或者 width 或 height 改变了，重新走一遍 step2
            // First 2 steps are done in onMeasure but looks like we have to run again due to
            // changed size.
            mLayout.setExactMeasureSpecsFrom(this);
            dispatchLayoutStep2();
        } else {
            // 设置 MeasureSpec 为 EXACTLY
            // always make sure we sync them (to ensure mode is exact)
            mLayout.setExactMeasureSpecsFrom(this);
        }
        // 关键步骤 3
        dispatchLayoutStep3();
    }

    public abstract static class LayoutManager {
    
        void setExactMeasureSpecsFrom(RecyclerView recyclerView) {
            setMeasureSpecs(
                    MeasureSpec.makeMeasureSpec(recyclerView.getWidth(), MeasureSpec.EXACTLY),
                    MeasureSpec.makeMeasureSpec(recyclerView.getHeight(), MeasureSpec.EXACTLY)
            );
        }
    }
    
}
```

可以发现，RecyclerView 的 layout 流程分为三步，关键调用分别为 dispatchLayoutStep1、dispatchLayoutStep2、dispatchLayoutStep3，其中 onMeasure 方法执行前两部，onLayout 方法执行最后一步，下面开始分析这三个方法。

# dispatchLayoutStep

## dispatchLayoutStep1

```
public class RecyclerView extends ViewGroup implements ScrollingView, NestedScrollingChild2 {
    
    /**
     * The first step of a layout where we;
     * - process adapter updates
     * - decide which animation should run
     * - save information about current views
     * - If necessary, run predictive layout and save its information
     */
    private void dispatchLayoutStep1() {
        ...
        processAdapterUpdatesAndSetAnimationFlags();

        if (mState.mRunSimpleAnimations) {
            // Step 0: 找出所有未删除的 item 的位置, 准备执行预布局
            int count = mChildHelper.getChildCount();
            for (int i = 0; i < count; ++i) {
                final ViewHolder holder = getChildViewHolderInt(mChildHelper.getChildAt(i));
                final ItemHolderInfo animationInfo = mItemAnimator.recordPreLayoutInformation(...);
                mViewInfoStore.addToPreLayout(holder, animationInfo);
                ...
            }
        }
        
        if (mState.mRunPredictiveAnimations) {
            // Step 1: 运行预布局

            // Save old positions so that LayoutManager can run its mapping logic.
            saveOldPositions();
            final boolean didStructureChange = mState.mStructureChanged;
            mState.mStructureChanged = false;
            // 借助 LayoutManager 完成 layout 流程
            // temporarily disable flag because we are asking for previous layout
            mLayout.onLayoutChildren(mRecycler, mState);
            mState.mStructureChanged = didStructureChange;

            for (int i = 0; i < mChildHelper.getChildCount(); ++i) {
                final View child = mChildHelper.getChildAt(i);
                final ViewHolder viewHolder = getChildViewHolderInt(child);
                if (!mViewInfoStore.isInPreLayout(viewHolder)) {
                    ...
                    final ItemHolderInfo animationInfo = mItemAnimator.recordPreLayoutInformation(...);
                    if (wasHidden) {
                        recordAnimationInfoIfBouncedHiddenView(viewHolder, animationInfo);
                    } else {
                        mViewInfoStore.addToAppearedInPreLayoutHolders(viewHolder, animationInfo);
                    }
                }
            }
            // we don't process disappearing list because they may re-appear in post layout pass.
            clearOldPositions();
        } else {
            clearOldPositions();
        }
        
        mState.mLayoutStep = State.STEP_LAYOUT;
    }
    
    private void processAdapterUpdatesAndSetAnimationFlags() {
        // 默认为 false，第一次 measure 时不会执行
        if (mDataSetHasChangedAfterLayout) {
            mAdapterHelper.reset();
            if (mDispatchItemsChangedEvent) {
                mLayout.onItemsChanged(this);
            }
        }
        
        // 两个分支里的代码最后调用的都是 AdapterHelper 内部的接口 Callback 的方法
        // Callback 的实现可以查看 RecyclerView 的 initAdapterManager 方法
        // 最后可能会调用 requestLayout 走一遍绘制流程以实现动画的效果
        if (predictiveItemAnimationsEnabled()) {
            mAdapterHelper.preProcess();
        } else {
            mAdapterHelper.consumeUpdatesInOnePass();
        }
        
        mState.mRunSimpleAnimations = mFirstLayoutComplete && ...;
        mState.mRunPredictiveAnimations = mState.mRunSimpleAnimations
                && animationTypeSupported
                && !mDataSetHasChangedAfterLayout
                && predictiveItemAnimationsEnabled();
    }
    
    static ViewHolder getChildViewHolderInt(View child) {
        if (child == null) {
            return null;
        }
        return ((LayoutParams) child.getLayoutParams()).mViewHolder;
    }
    
}
```

根据 dispatchLayoutStep1 的注释及代码，可以得知这个方法主要负责：
- 执行 adapter 更新，最终可能会调用 requestLayout
- 决定哪些动画应该被执行（暂时不会被执行）
- 保存子 View 的相关信息
- 如果有必要，执行预布局

而如果是第一次执行 measure 流程，adapter 正常来说是没有可以更新的元素的，即此时 dispatchLayoutStep1 的作用主要是计算并保存子 View 和动画的相关信息。

## dispatchLayoutStep2

预布局是否执行涉及的变量比较多，先忽略。现在继续看  dispatchLayoutStep2：

```
public class RecyclerView extends ViewGroup implements ScrollingView, NestedScrollingChild2 {

    /**
     * The second layout step where we do the actual layout of the views for the final state.
     * This step might be run multiple times if necessary (e.g. measure).
     */
    private void dispatchLayoutStep2() {
        ...
        // 借助 LayoutManager 完成实际上的布局
        // Step 2: Run layout
        mState.mInPreLayout = false;
        mLayout.onLayoutChildren(mRecycler, mState);
        mState.mStructureChanged = false;
        ...
    }
    
}
```

可以看到，相比 dispatchLayoutStep1，dispatchLayoutStep2 简单得多，作用是执行真正的布局，调用的方法是 mLayout.onLayoutChildren，这个方法在 LayoutManager 中是一个空实现，它的三个子类 LinearLayoutManager、GridLayoutManager、StaggeredGridLayoutManager 都有自己的实现。下面以 LinearLayoutManager 为例（关键部分，比如缓存机制，三个 LayoutManager 都是一样的）：

```
public class LinearLayoutManager extends RecyclerView.LayoutManager implements
        ItemTouchHelper.ViewDropHandler, RecyclerView.SmoothScroller.ScrollVectorProvider {

    @Override
    public void onLayoutChildren(RecyclerView.Recycler recycler, RecyclerView.State state) {
        // layout algorithm:
        // 1) by checking children and other variables, find an anchor coordinate and an anchor item position.
        // 2) fill towards start, stacking from bottom
        // 3) fill towards end, stacking from top
        // 4) scroll to fulfill requirements like stack from bottom.
        // create layout state
        
        if (mPendingSavedState != null || mPendingScrollPosition != NO_POSITION) {
            if (state.getItemCount() == 0) {
                removeAndRecycleAllViews(recycler);
                return;
            }
        }
        
        ...

		// 寻找子 View 的锚点坐标及位置
        final View focused = getFocusedChild();
        if (!mAnchorInfo.mValid || mPendingScrollPosition != NO_POSITION || mPendingSavedState != null) {
            updateAnchorInfoForLayout(recycler, state, mAnchorInfo);
        } else if (...) {
            mAnchorInfo.assignFromViewAndKeepVisibleRect(focused, getPosition(focused));
        }

        if (state.isPreLayout() && mPendingScrollPosition != NO_POSITION
                && mPendingScrollPositionOffset != INVALID_OFFSET) {
            // if the child is visible and we are going to move it around, we should layout
            // extra items in the opposite direction to make sure new items animate nicely
            // instead of just fading in
            ...
        }

        onAnchorReady(recycler, state, mAnchorInfo, firstLayoutDirection);
        // 把子 View detach 掉并缓存到 Recycler 里面
        detachAndScrapAttachedViews(recycler);
        mLayoutState.mIsPreLayout = state.isPreLayout();
        if (mAnchorInfo.mLayoutFromEnd) {
            // fill towards start
            updateLayoutStateToFillStart(mAnchorInfo);
            mLayoutState.mExtra = extraForStart;
            // 填充子 View
            fill(recycler, mLayoutState, state, false);
            startOffset = mLayoutState.mOffset;
            final int firstElement = mLayoutState.mCurrentPosition;
            if (mLayoutState.mAvailable > 0) {
                extraForEnd += mLayoutState.mAvailable;
            }
            
            // fill towards end
            updateLayoutStateToFillEnd(mAnchorInfo);
            mLayoutState.mExtra = extraForEnd;
            mLayoutState.mCurrentPosition += mLayoutState.mItemDirection;
            fill(recycler, mLayoutState, state, false);
            endOffset = mLayoutState.mOffset;

            if (mLayoutState.mAvailable > 0) {
                // end could not consume all. add more items towards start
                extraForStart = mLayoutState.mAvailable;
                updateLayoutStateToFillStart(firstElement, startOffset);
                mLayoutState.mExtra = extraForStart;
                fill(recycler, mLayoutState, state, false);
                startOffset = mLayoutState.mOffset;
            }
        } else {
            // fill towards end
            ...
            
            // fill towards start
            ...
        }

        ...
        layoutForPredictiveAnimations(recycler, state, startOffset, endOffset);
        if (!state.isPreLayout()) {
            mOrientationHelper.onLayoutComplete();
        } else {
            mAnchorInfo.reset();
        }
        ...
    }
    
}
```

结合注释及代码，可以知道 onLayoutChildren 的作用是：寻找到子 View 的锚点坐标及锚点位置后加载子 View。锚点坐标及位置的计算方法忽略，下面重点关注 detachAndScrapAttachedViews 和 fill 这两个方法。

### detachAndScrapAttachedViews

这个方法在第一次 measure 的时候不会产生效果，因为此时 RecyclerView 还没有子 View。而在第二第三次 layout 时，它会把子 View 从 RecyclerView 中 remove 或 detach ，并缓存子 View，以便之后重新 add 回来或 attach 回来，避免重复加载相同的子 View——这点是 ListView 几乎是一致的：

```
public class RecyclerView extends ViewGroup implements ScrollingView, NestedScrollingChild2 {

    public abstract static class LayoutManager {
    
        public void detachAndScrapAttachedViews(Recycler recycler) {
            final int childCount = getChildCount();
            for (int i = childCount - 1; i >= 0; i--) {
                final View v = getChildAt(i);
                scrapOrRecycleView(recycler, i, v);
            }
        }
        
        private void scrapOrRecycleView(Recycler recycler, int index, View view) {
            final ViewHolder viewHolder = getChildViewHolderInt(view);
            if (viewHolder.isInvalid() && !viewHolder.isRemoved()
                    && !mRecyclerView.mAdapter.hasStableIds()) { 
                // 移除子 View 并使用 mCacheVies 或 RecycledViewPool 缓存
                removeViewAt(index);
                recycler.recycleViewHolderInternal(viewHolder);
            } else {
                //  detach 子 View 并使用 mChangedScarp 或 mAttachedScarp 缓存
                detachViewAt(index);
                recycler.scrapView(view);
                mRecyclerView.mViewInfoStore.onViewDetached(viewHolder);
            }
        }
        
        // add / remove 和 attach/ detach 都是通过 mChildHelper 完成的
        // 之后会调用 ChildHelper 内部的接口 Callback 的方法，Callback 的实现可以查看 RecyclerView 的 initChildrenHelper
        // 最终还是通过 ViewGroup 的 add / remove 和 attach / detach 实现的
        public void removeViewAt(int index) {
            final View child = getChildAt(index);
            if (child != null) {
                mChildHelper.removeViewAt(index);
            }
        }
        
        public void detachViewAt(int index) {
            detachViewInternal(index, getChildAt(index));
        }

        private void detachViewInternal(int index, View view) {
            mChildHelper.detachViewFromParent(index);
        }
        
    }
    
    /**
     * A Recycler is responsible for managing scrapped or detached item views for reuse.
     *
     * <p>A "scrapped" view is a view that is still attached to its parent RecyclerView but
     * that has been marked for removal or reuse.</p>
     *
     * <p>Typical use of a Recycler by a {@link LayoutManager} will be to obtain views for
     * an adapter's data set representing the data at a given position or item ID.
     * If the view to be reused is considered "dirty" the adapter will be asked to rebind it.
     * If not, the view can be quickly reused by the LayoutManager with no further work.
     * Clean views that have not {@link android.view.View#isLayoutRequested() requested layout}
     * may be repositioned by a LayoutManager without remeasurement.</p>
     */
    public final class Recycler {
    
        final ArrayList<ViewHolder> mAttachedScrap = new ArrayList<>();
        ArrayList<ViewHolder> mChangedScrap = null;

        final ArrayList<ViewHolder> mCachedViews = new ArrayList<ViewHolder>();

        private final List<ViewHolder>
                mUnmodifiableAttachedScrap = Collections.unmodifiableList(mAttachedScrap);
                
        static final int DEFAULT_CACHE_SIZE = 2;

        private int mRequestedCacheMax = DEFAULT_CACHE_SIZE;
        // mCachedVies 最多放 2 个，这个容量限制会在 mCachedView 调用 add 方法时起作用
        int mViewCacheMax = DEFAULT_CACHE_SIZE;

        RecycledViewPool mRecyclerPool;
        
        private ViewCacheExtension mViewCacheExtension;

        void recycleViewHolderInternal(ViewHolder holder) {
            ...
            
            boolean cached = false;
            boolean recycled = false;
            if (forceRecycle || holder.isRecyclable()) {
                // 如果符合条件，就放到 mCachedViews 里面
                if (mViewCacheMax > 0
                        && !holder.hasAnyOfTheFlags(ViewHolder.FLAG_INVALID
                        | ViewHolder.FLAG_REMOVED
                        | ViewHolder.FLAG_UPDATE
                        | ViewHolder.FLAG_ADAPTER_POSITION_UNKNOWN)) {
                    // Retire oldest cached view
                    int cachedViewSize = mCachedViews.size();
                    // 超过缓存容量限制，把最老的一个 View 放进 RecycledViewPool 里面
                    if (cachedViewSize >= mViewCacheMax && cachedViewSize > 0) {
                        recycleCachedViewAt(0);
                        cachedViewSize--;
                    }

                    ...
                    //  使用 mCachedViews 缓存子 View
                    mCachedViews.add(targetCacheIndex, holder);
                    cached = true;
                }

                // 否则放到 RecycledViewPool 里面
                if (!cached) {
                    addViewHolderToRecycledViewPool(holder, true);
                    recycled = true;
                }
            } else {
                ...
            }
            // even if the holder is not removed, we still call this method so that it is removed
            // from view holder lists.
            mViewInfoStore.removeViewHolder(holder);
        }
        
        // 把一个 View 从 mCachedViews 里删除，并 put 进 RecycledViewPool 里面
        void recycleCachedViewAt(int cachedViewIndex) {
            ViewHolder viewHolder = mCachedViews.get(cachedViewIndex);
            addViewHolderToRecycledViewPool(viewHolder, true);
            mCachedViews.remove(cachedViewIndex);
        }
        
        void addViewHolderToRecycledViewPool(ViewHolder holder, boolean dispatchRecycled) {
            ...
            getRecycledViewPool().putRecycledView(holder);
        }
    
        /**
         * Mark an attached view as scrap.
         */
        void scrapView(View view) {
            final ViewHolder holder = getChildViewHolderInt(view);
            if (holder.hasAnyOfTheFlags(ViewHolder.FLAG_REMOVED | ViewHolder.FLAG_INVALID)
                    || !holder.isUpdated() || canReuseUpdatedViewHolder(holder)) {
                mAttachedScrap.add(holder);
            } else {
                mChangedScrap.add(holder);
            }
        }
        
    }
    
    /**
     * RecycledViewPool lets you share Views between multiple RecyclerViews.
     */
    public static class RecycledViewPool {
        private static final int DEFAULT_MAX_SCRAP = 5;
        
        static class ScrapData {
            // mScrapHeap 最多放 5 个
            final ArrayList<ViewHolder> mScrapHeap = new ArrayList<>();
            int mMaxScrap = DEFAULT_MAX_SCRAP;
            long mCreateRunningAverageNs = 0;
            long mBindRunningAverageNs = 0;
        }
        // 每种类型对应一个 ScrapData，即每种类型最多缓存 5 个 View
        SparseArray<ScrapData> mScrap = new SparseArray<>();

        private int mAttachCount = 0;
        
        public void putRecycledView(ViewHolder scrap) {
            final int viewType = scrap.getItemViewType();
            final ArrayList<ViewHolder> scrapHeap = getScrapDataForType(viewType).mScrapHeap;
            scrapHeap.add(scrap);
        }

        private ScrapData getScrapDataForType(int viewType) {
            ScrapData scrapData = mScrap.get(viewType);
            if (scrapData == null) {
                scrapData = new ScrapData();
                mScrap.put(viewType, scrapData);
            }
            return scrapData;
        }
        
    }
    
}
```

可以看到，detachAndScrapAttachedViews 这个不起眼的方法已经触及 RecyclerView 的核心了，它的作用有可以总结为一句话：remove / detach 所有子 View，并缓存它们。其中，如果根据 ViewHolder 状态得知子 View 可以被移除，则移除子 View，并缓存到 Recycler 的 mCachedViews 数组里面，mCachedViews 默认只缓存 2 个，如果 mCachedViews 容量超出限制，则把 mCachedViews 数组里最老的一个 item 移除，放到 RecycledViewPool 的 mScrapHeap 数组里。如果子 View 无法被移除，那么 detach 掉，并判断子 View 是否发生了改变，改变了的子 View 缓存到 Recycler 的 mChangedScrap 数组，否则缓存到 Recycler 的 mAttachedScrap 数组。

Recycler 和 RecycledViewPool 的区别是，一个 Recycler 对应一个 RecyclerView；一个 RecycledViewPool 可以对应多个 RecyclerView。即 RecycledViewPool 里面的 View 可以提供给多个 RecyclerView 重用。

Scrap View 代表移除了的可以重用的视图。如果即将要重用的 View 标记为 dirty，则需要 Adapter 重新绑定，否则 LayoutManager 可以直接使用。

detachAndScrapAttachedViews 就分析到这里，接下来继续分析 onLayoutChildren。 onLayoutChildren 方法中下一个需要关注的调用是 fill。

### fill

fill 方法特别关键——如果说 detachAndScrapAttachedViews  触及了 RecyclerView 的核心，那么 fill 方法就是真正的核心。它是从 Adapter 中加载子 View、从回收池中复用 View、并显示到 RecyclerView 上的关键调用，源码作者称它为 magic method：

```
public class LinearLayoutManager extends RecyclerView.LayoutManager implements
        ItemTouchHelper.ViewDropHandler, RecyclerView.SmoothScroller.ScrollVectorProvider {
        
    /**
     * The magic functions :). Fills the given layout, defined by the layoutState. This is fairly
     * independent from the rest of the {@link android.support.v7.widget.LinearLayoutManager}
     * and with little change, can be made publicly available as a helper class.
     */
    int fill(RecyclerView.Recycler recycler, LayoutState layoutState,
            RecyclerView.State state, boolean stopOnFocusable) {
        ...
        
        // 这个 while 循环是最关键的部分，它表明了 RecyclerView 和 ListView 一样，只会加载一个手机屏幕能显示的子 View
        while ((layoutState.mInfinite || remainingSpace > 0) && layoutState.hasMore(state)) {
            
            // 加载子 View
            layoutChunk(recycler, state, layoutState, layoutChunkResult);
            
            /**
             * Consume the available space if:
             * * layoutChunk did not request to be ignored
             * * OR we are laying out scrap children
             * * OR we are not doing pre-layout
             */
            if (!layoutChunkResult.mIgnoreConsumed || mLayoutState.mScrapList != null
                    || !state.isPreLayout()) {
                layoutState.mAvailable -= layoutChunkResult.mConsumed;
                // we keep a separate remaining space because mAvailable is important for recycling
                remainingSpace -= layoutChunkResult.mConsumed;
            }

            // 如果当前正在滚动屏幕
            if (layoutState.mScrollingOffset != LayoutState.SCROLLING_OFFSET_NaN) {
                layoutState.mScrollingOffset += layoutChunkResult.mConsumed;
                if (layoutState.mAvailable < 0) {
                    layoutState.mScrollingOffset += layoutState.mAvailable;
                }
                // 把移出屏幕的 View 缓存到 mCachedViews 里面
                recycleByLayoutState(recycler, layoutState);
            }

            if (stopOnFocusable && layoutChunkResult.mFocusable) {
                break;
            }
        }
        return start - layoutState.mAvailable;
    }
    
    void layoutChunk(RecyclerView.Recycler recycler, RecyclerView.State state,
            LayoutState layoutState, LayoutChunkResult result) {
        // 从 Adapter 或回收池中获取一个 View
        View view = layoutState.next(recycler);
        if (view == null) {
            result.mFinished = true;
            return;
        }
        
        LayoutParams params = (LayoutParams) view.getLayoutParams();
        // 添加子 View 到 RecyclerView 上面
        if (layoutState.mScrapList == null) {
            if (mShouldReverseLayout == (layoutState.mLayoutDirection
                    == LayoutState.LAYOUT_START)) {
                addView(view);
            } else {
                addView(view, 0);
            }
        } else {
            if (mShouldReverseLayout == (layoutState.mLayoutDirection
                    == LayoutState.LAYOUT_START)) {
                addDisappearingView(view);
            } else {
                addDisappearingView(view, 0);
            }
        }
        
        // 执行子 View 的 measure 流程
        measureChildWithMargins(view, 0, 0);
        result.mConsumed = mOrientationHelper.getDecoratedMeasurement(view);
        
        ...
        
        // We calculate everything with View's bounding box (which includes decor and margins)
        // To calculate correct layout position, we subtract margins.
        layoutDecoratedWithMargins(view, left, top, right, bottom);
        result.mFocusable = view.hasFocusable();
    }
    
    static class LayoutState {
    
        List<RecyclerView.ViewHolder> mScrapList = null;
    
        /**
         * Gets the view for the next element that we should layout.
         * Also updates current item index to the next item, based on {@link #mItemDirection}
         */
        View next(RecyclerView.Recycler recycler) {
            if (mScrapList != null) { // 只在特定时候才不为空，省略
                return nextViewFromScrapList();
            }
            // 从 Adapter 或回收池中获取一个 View
            final View view = recycler.getViewForPosition(mCurrentPosition);
            mCurrentPosition += mItemDirection;
            return view;
        }
        
    }
    
}
```

可以看到，fill 方法的作用就是循环获取一个个子 View，并加载到屏幕上，直到屏幕没有剩余空间或多余的子 View，具体执行的方法是 layoutChunk，它首先调用 getViewForPosition 尝试获取一个子 View，接着调用 addView 方法把它加载到 RecyclerView 中，最后再执行子 View 的 measure 流程。同时，它会根据 LayoutState 判断 RecyclerView 是否正在滚动中，如果是，则会把移出屏幕之外的子 View 缓存以便之后重用。下面看看它是如何实现的，先从 getViewForPosition 开始分析：

```
public class RecyclerView extends ViewGroup implements ScrollingView, NestedScrollingChild2 {

    public final class Recycler {
    
         /**
         * Obtain a view initialized for the given position.
         *
         * This method should be used by {@link LayoutManager} implementations to obtain
         * views to represent data from an {@link Adapter}.
         * <p>
         * The Recycler may reuse a scrap or detached view from a shared pool if one is
         * available for the correct view type. If the adapter has not indicated that the
         * data at the given position has changed, the Recycler will attempt to hand back
         * a scrap view that was previously initialized for that data without rebinding.
         */
        public View getViewForPosition(int position) {
            return getViewForPosition(position, false);
        }
        
        View getViewForPosition(int position, boolean dryRun) {
            return tryGetViewHolderForPositionByDeadline(position, dryRun, FOREVER_NS).itemView;
        }
        
        /**
         * Attempts to get the ViewHolder for the given position, either from the Recycler scrap,
         * cache, the RecycledViewPool, or creating it directly.
         */
        @Nullable
        ViewHolder tryGetViewHolderForPositionByDeadline(int position,
                boolean dryRun, long deadlineNs) {
            boolean fromScrapOrHiddenOrCache = false;
            ViewHolder holder = null;
            // 0) 如果是预布局，则尝试从 mChangedScrap 中获取
            // 0) If there is a changed scrap, try to find from there
            if (mState.isPreLayout()) {
                holder = getChangedScrapViewForPosition(position);
                fromScrapOrHiddenOrCache = holder != null;
            }
            // 1) 根据 position 依次从 mAttachedScrap, mCachedViews 中获取
            // 1) Find by position from scrap/hidden list/cache
            if (holder == null) {
                holder = getScrapOrHiddenOrCachedHolderForPosition(position, dryRun);
                if (holder != null) {
                    if (!validateViewHolderForOffsetPosition(holder)) {
                        // recycle holder (and unscrap if relevant) since it can't be used
                        // 这个 View 会被重用，从缓存池中移除它
                        if (holder.isScrap()) {
                            removeDetachedView(holder.itemView, false);
                            holder.unScrap();
                        }
                        if (!dryRun) {
                            recycleViewHolderInternal(holder);
                        }
                        holder = null;
                    } else {
                        fromScrapOrHiddenOrCache = true;
                    }
                }
            }
            if (holder == null) {
                final int type = mAdapter.getItemViewType(offsetPosition);
                // 2) 如果上一步没有获取到，则根据 id 依次从 mAttachedScrap, mCachedViews 中获取
                // 2) Find from scrap/cache via stable ids, if exists
                if (mAdapter.hasStableIds()) {
                    // 和 getScrapOrHiddenOrCachedHolderForPosition 类似，省略具体代码
                    holder = getScrapOrCachedViewForId(mAdapter.getItemId(offsetPosition),
                            type, dryRun);
                    if (holder != null) {
                        // update position
                        holder.mPosition = offsetPosition;
                        fromScrapOrHiddenOrCache = true;
                    }
                }

                if (holder == null && mViewCacheExtension != null) {
                    // 从 mViewCacheExtension 中获取
                    // We are NOT sending the offsetPosition because LayoutManager does not know it.
                    final View view = mViewCacheExtension.getViewForPositionAndType(this, position, type);
                    if (view != null) {
                        holder = getChildViewHolder(view);
                        ...
                    }
                }

                if (holder == null) { // fallback to pool
                    // 还没有获取到，则尝试从 RecycledViewPool 中获取
                    holder = getRecycledViewPool().getRecycledView(type);
                    if (holder != null) {
                        holder.resetInternal();
                    }
                }
                if (holder == null) {
                    // 没有可以复用的 View，则回调 mAdapter 的 onCreateViewHolder 方法加载一个子 View
                    holder = mAdapter.createViewHolder(RecyclerView.this, type);
                    ...
                }
            }

            boolean bound = false;
            if (mState.isPreLayout() && holder.isBound()) {
                // 如果是预布局，则设置子 View 的 position 信息即可
                // do not update unless we absolutely have to.
                holder.mPreLayoutPosition = position;
            } else if (!holder.isBound() || holder.needsUpdate() || holder.isInvalid()) {
                // 否则回调 Adapter 的 onBindViewHolder 方法，为子 View 绑定数据
                final int offsetPosition = mAdapterHelper.findPositionOffset(position);
                bound = tryBindViewHolderByDeadline(holder, offsetPosition, position, deadlineNs);
            }

            final ViewGroup.LayoutParams lp = holder.itemView.getLayoutParams();
            final LayoutParams rvLayoutParams;
            if (lp == null) {
                rvLayoutParams = (LayoutParams) generateDefaultLayoutParams();
                holder.itemView.setLayoutParams(rvLayoutParams);
            } else if (!checkLayoutParams(lp)) {
                rvLayoutParams = (LayoutParams) generateLayoutParams(lp);
                holder.itemView.setLayoutParams(rvLayoutParams);
            } else {
                rvLayoutParams = (LayoutParams) lp;
            }
            rvLayoutParams.mViewHolder = holder;
            rvLayoutParams.mPendingInvalidate = fromScrapOrHiddenOrCache && bound;
            return holder;
        }
        
        ViewHolder getChangedScrapViewForPosition(int position) {
            ...
            // find by position
            for (int i = 0; i < changedScrapSize; i++) {
            	// 从 mChangedScrap 中获取缓存的 View
                final ViewHolder holder = mChangedScrap.get(i);
                if (!holder.wasReturnedFromScrap() && holder.getLayoutPosition() == position) {
                    holder.addFlags(ViewHolder.FLAG_RETURNED_FROM_SCRAP);
                    return holder;
                }
            }
            // find by id
            if (mAdapter.hasStableIds()) {
                ...
            }
            return null;
        }
        
        /**
         * Returns a view for the position either from attach scrap, hidden children, or cache.
         *
         * @param dryRun  Does a dry run, finds the ViewHolder but does not remove
         */
        ViewHolder getScrapOrHiddenOrCachedHolderForPosition(int position, boolean dryRun) {
            final int scrapCount = mAttachedScrap.size();

            // Try first for an exact, non-invalid match from scrap.
            for (int i = 0; i < scrapCount; i++) {
                final ViewHolder holder = mAttachedScrap.get(i);
                if (!holder.wasReturnedFromScrap() && holder.getLayoutPosition() == position
                        && !holder.isInvalid() && (mState.mInPreLayout || !holder.isRemoved())) {
                    holder.addFlags(ViewHolder.FLAG_RETURNED_FROM_SCRAP);
                    return holder;
                }
            }

            if (!dryRun) {
                // 作用是把之前隐藏的 View 重新拿出来，这里不把它算作缓存机制的一部分
                View view = mChildHelper.findHiddenNonRemovedView(position);
                if (view != null) {
                    // This View is good to be used. We just need to unhide, detach and move to the
                    // scrap list.
                    final ViewHolder vh = getChildViewHolderInt(view);
                    mChildHelper.unhide(view);
                    ...
                    mChildHelper.detachViewFromParent(layoutIndex);
                    scrapView(view);
                    return vh;
                }
            }

            // Search in our first-level recycled view cache.
            final int cacheSize = mCachedViews.size();
            for (int i = 0; i < cacheSize; i++) {
                final ViewHolder holder = mCachedViews.get(i);
                // invalid view holders may be in cache if adapter has stable ids as they can be
                // retrieved via getScrapOrCachedViewForId
                if (!holder.isInvalid() && holder.getLayoutPosition() == position) {
                    // 从缓存池中移除，getViewForPosition 中传过来的 dryRun 值为 false
                    if (!dryRun) {
                        mCachedViews.remove(i);
                    }
                    return holder;
                }
            }
            return null;
        }
        
        private boolean tryBindViewHolderByDeadline(ViewHolder holder, int offsetPosition,
                int position, long deadlineNs) {
            ...
            mAdapter.bindViewHolder(holder, offsetPosition);
            ...
            return true;
        }
        
    }
    
    public abstract static class Adapter<VH extends ViewHolder> {
        private final AdapterDataObservable mObservable = new AdapterDataObservable();
        
        public final VH createViewHolder(@NonNull ViewGroup parent, int viewType) {
            ...
            	// 从 xml 资源中加载子 View 并创建 ViewHolder，这是我们重写 Adapter 时的关键方法之一
                final VH holder = onCreateViewHolder(parent, viewType);
                holder.mItemViewType = viewType;
                return holder;
            ...
        }
        
        public final void bindViewHolder(@NonNull VH holder, int position) {
            holder.mPosition = position;
            if (hasStableIds()) {
                holder.mItemId = getItemId(position);
            }
            // 绑定数据到 ViewHolder，这是我们重写 Adapter 时的关键方法之一
            onBindViewHolder(holder, position, holder.getUnmodifiedPayloads());
            ...
        }
        
    }
    
}
```

可以看到，getViewForPosition 最终是按照一个固定的顺序从 Recycler 或 RecycledViewPool 中一个个尝试获取的，流程为：mChangedScrap ->  mAttachedScrap -> mCachedViews -> mViewCacheExtension -> RecycledViewPool，如果没有可用的缓存，则会调用  Adapter 的  onCreateViewHolder 方法从 xml 资源中加载子 View。

成功获取到一个 itemView 之后，如果当前属于预布局阶段，则设置 itemView 的位置信息即可；否则调用 Adapter 的 onBindViewHolder 绑定数据到 itemView 中。接着就可以调用 addView 方法把 itemView 加载到 RecyclerView 中了：

```
public class RecyclerView extends ViewGroup implements ScrollingView, NestedScrollingChild2 {

    public abstract static class LayoutManager {
        
        public void addView(View child) {
            addView(child, -1);
        }
        
        public void addView(View child, int index) {
            addViewInt(child, index, false);
        }
        
        public void addDisappearingView(View child) {
            addDisappearingView(child, -1);
        }
        
        public void addDisappearingView(View child, int index) {
            addViewInt(child, index, true);
        }
        
        private void addViewInt(View child, int index, boolean disappearing) {
            final ViewHolder holder = getChildViewHolderInt(child);
            final LayoutParams lp = (LayoutParams) child.getLayoutParams();
            if (holder.wasReturnedFromScrap() || holder.isScrap()) { 
            	// 从缓存池中重新拿出来的子 View， attach 即可
                mChildHelper.attachViewToParent(child, index, child.getLayoutParams(), false);
            } else if (child.getParent() == mRecyclerView) { // it was not a scrap but a valid child
            	// 仍然在屏幕上显示的子 View，移动到新的位置即可
                if (currentIndex != index) {
                    mRecyclerView.mLayout.moveView(currentIndex, index);
                }
            } else { 
            	// 新的子 View，添加到 RecyclerView 上
                mChildHelper.addView(child, index, false);
            }
            if (lp.mPendingInvalidate) {
                holder.itemView.invalidate();
                lp.mPendingInvalidate = false;
            }
        }
        
        /**
         * Moves a View from one position to another.
         */
        public void moveView(int fromIndex, int toIndex) {
            View view = getChildAt(fromIndex);
            detachViewAt(fromIndex);
            attachView(view, toIndex);
        }
        
    }
    
}
```

前面说过，如果当前正在滚动屏幕，那么 fill 方法还会调用 recycleByLayoutState 方法缓存移出屏幕之外的子 View：

```
public class LinearLayoutManager extends RecyclerView.LayoutManager implements
        ItemTouchHelper.ViewDropHandler, RecyclerView.SmoothScroller.ScrollVectorProvider {

    private void recycleByLayoutState(RecyclerView.Recycler recycler, LayoutState layoutState) {
        if (!layoutState.mRecycle || layoutState.mInfinite) {
            return;
        }
        // 根据滑动的距离计算哪些 View 需要被移除
        if (layoutState.mLayoutDirection == LayoutState.LAYOUT_START) {
            recycleViewsFromEnd(recycler, layoutState.mScrollingOffset);
        } else {
            recycleViewsFromStart(recycler, layoutState.mScrollingOffset);
        }
    }
    
    private void recycleViewsFromEnd(RecyclerView.Recycler recycler, int dt) {
        final int childCount = getChildCount();
        final int limit = mOrientationHelper.getEnd() - dt;
        if (mShouldReverseLayout) {
            for (int i = 0; i < childCount; i++) {
                View child = getChildAt(i);
                if (mOrientationHelper.getDecoratedStart(child) < limit
                        || mOrientationHelper.getTransformedStartWithDecoration(child) < limit) {
                    // 缓存
                    // stop here
                    recycleChildren(recycler, 0, i);
                    return;
                }
            }
        } else {
            ...
        }
    }
    
    private void recycleChildren(RecyclerView.Recycler recycler, int startIndex, int endIndex) {
        ...
        // 移除并缓存
        if (endIndex > startIndex) {
            for (int i = endIndex - 1; i >= startIndex; i--) {
                removeAndRecycleViewAt(i, recycler);
            }
        } else {
            for (int i = startIndex; i > endIndex; i--) {
                removeAndRecycleViewAt(i, recycler);
            }
        }
    }
    
}
```

```
public class RecyclerView extends ViewGroup implements ScrollingView, NestedScrollingChild2 {

    public abstract static class LayoutManager {
    
        public void removeAndRecycleViewAt(int index, Recycler recycler) {
            final View view = getChildAt(index);
            // 从 RecyclerView 中移除屏幕之外的子 View
            removeViewAt(index);
            // 移除后缓存
            recycler.recycleView(view);
        }
        
        public void removeViewAt(int index) {
            final View child = getChildAt(index);
            if (child != null) {
                mChildHelper.removeViewAt(index);
            }
        }
        
    }
    
    public final class Recycler {
    
        public void recycleView(View view) {
            // This public recycle method tries to make view recycle-able since layout manager
            // intended to recycle this view (e.g. even if it is in scrap or change cache)
            ViewHolder holder = getChildViewHolderInt(view);
            if (holder.isTmpDetached()) {
                removeDetachedView(view, false);
            }
            if (holder.isScrap()) {
                // 把 ViewHolder 从 mChangedScrap 或 mAttachedScrap 中移除
                holder.unScrap();
            } else if (holder.wasReturnedFromScrap()) {
                holder.clearReturnedFromScrapFlag();
            }
            // 这个方法之前分析过，用于缓存 ViewHolder 到 mCachedViews 中
            recycleViewHolderInternal(holder);
        }
        
        void unscrapView(ViewHolder holder) {
        	// 从 mChangedScrap 或 mAttachedScrap 中移除
            if (holder.mInChangeScrap) {
                mChangedScrap.remove(holder);
            } else {
                mAttachedScrap.remove(holder);
            }
            holder.mScrapContainer = null;
            holder.mInChangeScrap = false;
            holder.clearReturnedFromScrapFlag();
        }
        
    }
    
    public abstract static class ViewHolder {
    
        private Recycler mScrapContainer = null;
    
        void unScrap() {
            mScrapContainer.unscrapView(this);
        }
        
    }
    
}
```

可以看到，fill 最终会把移除屏幕之外的子 View 缓存到 mCahcedViews 里面，如果这个 View 已经缓存到了 mChangedScrap 或 mAttachedScrap 中，那么还会把它移除出来，转移到 mCahcedViews 里面。

对比 RecyclerView 和 ListView：
- 同样在第一次 layout 时回调 Adapter 的相关方法从 xml 资源中加载子 View
- 同样会在第二第三次 layout 的先 remove / detach 所有的子 View，最后再 add / attach 回来，这是为了避免重复加载相同的子 View
- 同样只加载一个手机屏幕能显示的子 View
- 同样使用多个缓存数组/列表，且不同的缓存数组/列表对应的功能不同，RecylerView 的 mAttachedScrap 对应 ListView 的 mActiveViews，mCahcedViews 对应 mScrapViews

区别在于：
- 除了 attach / detach 之外，RecyclerView 还有可能会 remove 子 View 再 add 回来，ListView 只会 detach 再 attach
- RecyclerView 的子 View 缓存列表分工更细，共有 5 级缓存，即 mChangedScrap、mAttachedScrap、mCahcedViews、mViewCacheExtension、RecycledViewPool，其中 mViewCacheExtension 用于提供给用户自定义缓存逻辑，而 RecycledViewPool 甚至可以提供给多个 RecyclerView 共用；ListView 则只分为 mActiveViews 和 mScrapViews 两个级别
- RecyclerView 的缓存单位是 ViewHolder，ListView 的缓存单位是 View，ListView 需要配合 setTag 方法才能实现复用

RecyclerView 的缓存机制在这里就基本分析完成了，下面继续看 dispatchLayoutStep3。

## dispatchLayoutStep3

```
public class RecyclerView extends ViewGroup implements ScrollingView, NestedScrollingChild2 {

    /**
     * The final step of the layout where we save the information about views for animations,
     * trigger animations and do any necessary cleanup.
     */
    private void dispatchLayoutStep3() {
        ...
        // 重置 LayoutState
        mState.mLayoutStep = State.STEP_START;
        if (mState.mRunSimpleAnimations) {
            // Step 3: Find out where things are now, and process change animations.
            for (int i = mChildHelper.getChildCount() - 1; i >= 0; i--) {
                ViewHolder holder = getChildViewHolderInt(mChildHelper.getChildAt(i));
                if (holder.shouldIgnore()) {
                    continue;
                }
                long key = getChangedHolderKey(holder);
                final ItemHolderInfo animationInfo = mItemAnimator
                        .recordPostLayoutInformation(mState, holder);
                ViewHolder oldChangeViewHolder = mViewInfoStore.getFromOldChangeHolders(key);
                if (oldChangeViewHolder != null && !oldChangeViewHolder.shouldIgnore()) {
                    // run a change animation
                    if (oldDisappearing && oldChangeViewHolder == holder) {
                        // run disappear animation instead of change
                        mViewInfoStore.addToPostLayout(holder, animationInfo);
                    } else {
                        final ItemHolderInfo preInfo = mViewInfoStore.popFromPreLayout(
                                oldChangeViewHolder);
                        // we add and remove so that any post info is merged.
                        mViewInfoStore.addToPostLayout(holder, animationInfo);
                        ItemHolderInfo postInfo = mViewInfoStore.popFromPostLayout(holder);
                        if (preInfo == null) {
                            handleMissingPreInfoForChangeError(key, holder, oldChangeViewHolder);
                        } else {
                            // 执行动画
                            animateChange(oldChangeViewHolder, holder, preInfo, postInfo,
                                    oldDisappearing, newDisappearing);
                        }
                    }
                } else {
                    mViewInfoStore.addToPostLayout(holder, animationInfo);
                }
            }

            // 执行 mViewInfoProcessCallback 里的动画
            // Step 4: Process view info lists and trigger animations
            mViewInfoStore.process(mViewInfoProcessCallback);
        }

        // 把 mAttachedScrap 里缓存的 View 移动到 mCachedViews 里面
        mLayout.removeAndRecycleScrapInt(mRecycler);
        if (mRecycler.mChangedScrap != null) {
            mRecycler.mChangedScrap.clear();
        }
        
        if (mLayout.mPrefetchMaxObservedInInitialPrefetch) {
            // 把 mCachedViews 多余的 View 移动到 RecycledViewPool 里面
            mRecycler.updateViewCacheSize();
        }

        mLayout.onLayoutCompleted(mState);
        onExitLayoutOrScroll();
        stopInterceptRequestLayout(false);
        mViewInfoStore.clear();
        if (didChildRangeChange(mMinMaxLayoutPositions[0], mMinMaxLayoutPositions[1])) {
            dispatchOnScrolled(0, 0);
        }
        recoverFocusFromState();
        resetFocusInfo();
    }
    
    private void animateChange(@NonNull ViewHolder oldHolder, @NonNull ViewHolder newHolder,
            @NonNull ItemHolderInfo preInfo, @NonNull ItemHolderInfo postInfo,
            boolean oldHolderDisappearing, boolean newHolderDisappearing) {
        ...
        if (mItemAnimator.animateChange(oldHolder, newHolder, preInfo, postInfo)) {
            postAnimationRunner();
        }
    }
    
    /**
     * The callback to convert view info diffs into animations.
     */
    private final ViewInfoStore.ProcessCallback mViewInfoProcessCallback =
            new ViewInfoStore.ProcessCallback() {
                @Override
                public void processDisappeared(ViewHolder viewHolder, @NonNull ItemHolderInfo info,
                        @Nullable ItemHolderInfo postInfo) {
                    mRecycler.unscrapView(viewHolder);
                    animateDisappearance(viewHolder, info, postInfo);
                }
                @Override
                public void processAppeared(ViewHolder viewHolder,
                        ItemHolderInfo preInfo, ItemHolderInfo info) {
                    animateAppearance(viewHolder, preInfo, info);
                }
                // 各种动画最终都调用了 postAnimationRunner 执行
                ...
            };
    
    void animateDisappearance(@NonNull ViewHolder holder,
            @NonNull ItemHolderInfo preLayoutInfo, @Nullable ItemHolderInfo postLayoutInfo) {
        if (mItemAnimator.animateDisappearance(holder, preLayoutInfo, postLayoutInfo)) {
            postAnimationRunner();
        }
    }
    
     /**
     * Post a runnable to the next frame to run pending item animations. Only the first such
     * request will be posted, governed by the mPostedAnimatorRunner flag.
     */
    void postAnimationRunner() {
        if (!mPostedAnimatorRunner && mIsAttached) {
            ViewCompat.postOnAnimation(this, mItemAnimatorRunner);
            mPostedAnimatorRunner = true;
        }
    }
    
    // DefaultItemAnimator 是 RecyclerView 默认的动画执行者
    ItemAnimator mItemAnimator = new DefaultItemAnimator();
    
    private Runnable mItemAnimatorRunner = new Runnable() {
        @Override
        public void run() {
            if (mItemAnimator != null) {
                mItemAnimator.runPendingAnimations();
            }
            mPostedAnimatorRunner = false;
        }
    };
    
    public abstract static class LayoutManager {
    
        /**
         * Recycles the scrapped views.
         */
        void removeAndRecycleScrapInt(Recycler recycler) {
            final int scrapCount = recycler.getScrapCount();
            // Loop backward, recycler might be changed by removeDetachedView()
            for (int i = scrapCount - 1; i >= 0; i--) {
                final View scrap = recycler.getScrapViewAt(i);
                final ViewHolder vh = getChildViewHolderInt(scrap);
                if (vh.isTmpDetached()) {
                    mRecyclerView.removeDetachedView(scrap, false);
                }
                if (mRecyclerView.mItemAnimator != null) {
                    mRecyclerView.mItemAnimator.endAnimation(vh);
                }
                // 放入 mCachedViews 里面
                recycler.quickRecycleScrapView(scrap);
            }
            recycler.clearScrap();
            if (scrapCount > 0) {
                mRecyclerView.invalidate();
            }
        }
        
        int getScrapCount() {
            return mAttachedScrap.size();
        }
        
        View getScrapViewAt(int index) {
            return mAttachedScrap.get(index).itemView;
        }
        
    }
    
}
```

可以看到，dispatchLayoutStep3 的主要作用就是执行动画和做一些清理工作，默认的动画执行者是 DefaultItemAnimator，清理工作包括把 mAttachedScrap 里缓存的 View 移动到 mCachedViews 里面，如果 mCachedViews 容量超出限制，则移动一部分到 RecycledViewPool 里，同时清理 mChangedScrap 的所有元素。

# 滑动加载更多数据

经过上面的分析，可以基本肯定 RecyclerView 在滑动加载更多数据时，会调用 fill 方法使用 mCachedViews 进行子 View 的缓存和复用，下面来验证一下：

```
public class RecyclerView extends ViewGroup implements ScrollingView, NestedScrollingChild2 {

    @Override
    public boolean onTouchEvent(MotionEvent e) {
        ...
        switch (action) {
            ...
            case MotionEvent.ACTION_MOVE: {
                ...
                if (mScrollState == SCROLL_STATE_DRAGGING) {
                    mLastTouchX = x - mScrollOffset[0];
                    mLastTouchY = y - mScrollOffset[1];

                    if (scrollByInternal(
                            canScrollHorizontally ? dx : 0,
                            canScrollVertically ? dy : 0,
                            vtev)) {
                        getParent().requestDisallowInterceptTouchEvent(true);
                    }
                    if (mGapWorker != null && (dx != 0 || dy != 0)) {
                        mGapWorker.postFromTraversal(this, dx, dy);
                    }
                }
            } break;
            ...
        }

        return true;
    }
    
    boolean scrollByInternal(int x, int y, MotionEvent ev) {
        int unconsumedX = 0, unconsumedY = 0;
        int consumedX = 0, consumedY = 0;

        consumePendingUpdateOperations();
        if (mAdapter != null) {
            ...
            // 滚动的事件处理由 LayoutManager 完成
            if (x != 0) {
                consumedX = mLayout.scrollHorizontallyBy(x, mRecycler, mState);
                unconsumedX = x - consumedX;
            }
            if (y != 0) {
                consumedY = mLayout.scrollVerticallyBy(y, mRecycler, mState);
                unconsumedY = y - consumedY;
            }
        }
        
        if (!mItemDecorations.isEmpty()) {
            invalidate();
        }

        if (dispatchNestedScroll(consumedX, consumedY, unconsumedX, unconsumedY, mScrollOffset,
                TYPE_TOUCH)) {
            ...
        } else if (getOverScrollMode() != View.OVER_SCROLL_NEVER) {
            ...
        }
        return consumedX != 0 || consumedY != 0;
    }
    
    // 如果 Adapter 更新了，则重新走一遍 layout 流程
    void consumePendingUpdateOperations() {
        if (!mFirstLayoutComplete || mDataSetHasChangedAfterLayout) {
            dispatchLayout();
            return;
        }
        if (!mAdapterHelper.hasPendingUpdates()) {
            return;
        }

        // if it is only an item change (no add-remove-notifyDataSetChanged) we can check if any
        // of the visible items is affected and if not, just ignore the change.
        if (mAdapterHelper.hasAnyUpdateTypes(AdapterHelper.UpdateOp.UPDATE) && !mAdapterHelper
                .hasAnyUpdateTypes(AdapterHelper.UpdateOp.ADD | AdapterHelper.UpdateOp.REMOVE
                        | AdapterHelper.UpdateOp.MOVE)) {
            if (!mLayoutWasDefered) {
                if (hasUpdatedView()) {
                    dispatchLayout();
                } else {
                    // no need to layout, clean state
                    mAdapterHelper.consumePostponedUpdates();
                }
            }
        } else if (mAdapterHelper.hasPendingUpdates()) {
            dispatchLayout();
        }
    }
    
}
```

```
public class LinearLayoutManager extends RecyclerView.LayoutManager implements
        ItemTouchHelper.ViewDropHandler, RecyclerView.SmoothScroller.ScrollVectorProvider {

    @Override
    public int scrollHorizontallyBy(int dx, RecyclerView.Recycler recycler,
            RecyclerView.State state) {
        if (mOrientation == VERTICAL) {
            return 0;
        }
        return scrollBy(dx, recycler, state);
    }

    @Override
    public int scrollVerticallyBy(int dy, RecyclerView.Recycler recycler,
            RecyclerView.State state) {
        if (mOrientation == HORIZONTAL) {
            return 0;
        }
        return scrollBy(dy, recycler, state);
    }
    
    int scrollBy(int dy, RecyclerView.Recycler recycler, RecyclerView.State state) {
        if (getChildCount() == 0 || dy == 0) {
            return 0;
        }
        mLayoutState.mRecycle = true;
        ...
        // fill 在上面分析过，即循环获取子 View 直到屏幕填充完毕，或子 View 消耗完毕
        final int consumed = mLayoutState.mScrollingOffset
                + fill(recycler, mLayoutState, state, false);
        ...
        return scrolled;
    }
    
}
```

可以看到，就像上面的推测的那样，RecyclerView 在滑动过程中会调用 fill 方法，使用 mCachedViews 进行子 View 的缓存和复用，如果 Adapter 更新了，则会重新走了一遍 layout 的流程。

# Adapter

上面可以说已经把 RecyclerView最关键的部分都分析完成了，但还有一些问题是没有解决的，即  RecyclerView 是如何得知 Adapter 更新的？下面开始分析这个问题。

从 RecyclerView 的 setAdapter 方法开始看起：

```
public class RecyclerView extends ViewGroup implements ScrollingView, NestedScrollingChild2 {

    private final RecyclerViewDataObserver mObserver = new RecyclerViewDataObserver();
    Adapter mAdapter;
    AdapterHelper mAdapterHelper;

    public void setAdapter(Adapter adapter) {
        // bail out if layout is frozen
        setLayoutFrozen(false);
        setAdapterInternal(adapter, false, true);
        processDataSetCompletelyChanged(false);
        requestLayout();
    }
    
    private void setAdapterInternal(Adapter adapter, boolean compatibleWithPrevious,
            boolean removeAndRecycleViews) {
        if (mAdapter != null) {
            mAdapter.unregisterAdapterDataObserver(mObserver);
            mAdapter.onDetachedFromRecyclerView(this);
        }
        ...
        mAdapter = adapter;
        if (adapter != null) {
            adapter.registerAdapterDataObserver(mObserver);
            adapter.onAttachedToRecyclerView(this);
        }
        ...
    }

    void processDataSetCompletelyChanged(boolean dispatchItemsChanged) {
        mDispatchItemsChangedEvent |= dispatchItemsChanged;
        // 标记 DataSet 改变了，之后会调用 dispatchLayout 走一遍 layout 流程
        mDataSetHasChangedAfterLayout = true;
        markKnownViewsInvalid();
    }

    public abstract static class Adapter<VH extends ViewHolder> {
        // 使用了观察者模式
        private final AdapterDataObservable mObservable = new AdapterDataObservable();

        @NonNull
        public abstract VH onCreateViewHolder(@NonNull ViewGroup parent, int viewType);

        public abstract void onBindViewHolder(@NonNull VH holder, int position);
        
        ...

        // 注册观察者
        public void registerAdapterDataObserver(@NonNull AdapterDataObserver observer) {
            mObservable.registerObserver(observer);
        }

        public void unregisterAdapterDataObserver(@NonNull AdapterDataObserver observer) {
            mObservable.unregisterObserver(observer);
        }

        // 通知观察者，数据发生了改变
        public final void notifyDataSetChanged() {
            mObservable.notifyChanged();
        }

        public final void notifyItemInserted(int position) {
            mObservable.notifyItemRangeInserted(position, 1);
        }

        ...
        
    }
    
    static class AdapterDataObservable extends Observable<AdapterDataObserver> {

		// 通知所有观察者
        public void notifyChanged() {
            for (int i = mObservers.size() - 1; i >= 0; i--) {
                mObservers.get(i).onChanged();
            }
        }

        public void notifyItemRangeInserted(int positionStart, int itemCount) {
            for (int i = mObservers.size() - 1; i >= 0; i--) {
                mObservers.get(i).onItemRangeInserted(positionStart, itemCount);
            }
        }

        ...
    }
    
    private class RecyclerViewDataObserver extends AdapterDataObserver {
        RecyclerViewDataObserver() {
        }

        @Override
        public void onChanged() {
            processDataSetCompletelyChanged(true);
            if (!mAdapterHelper.hasPendingUpdates()) {
                requestLayout();
            }
        }

        @Override
        public void onItemRangeInserted(int positionStart, int itemCount) {
            if (mAdapterHelper.onItemRangeInserted(positionStart, itemCount)) {
                triggerUpdateProcessor();
            }
        }
        
        void triggerUpdateProcessor() {
            // 如果设置了 mHasFixedSize，那么可能会省略一些工作
            if (POST_UPDATES_ON_ANIMATION && mHasFixedSize && mIsAttached) {
                ViewCompat.postOnAnimation(RecyclerView.this, mUpdateChildViewsRunnable);
            } else {
                // 否则需要重新走一遍 View 的三大流程
                mAdapterUpdateDuringMeasure = true;
                requestLayout();
            }
        }

    }
    
    final Runnable mUpdateChildViewsRunnable = new Runnable() {
        @Override
        public void run() {
            if (!mFirstLayoutComplete || isLayoutRequested()) {
                // a layout request will happen, we should not do layout here.
                return;
            }
            if (!mIsAttached) {
                requestLayout();
                // if we are not attached yet, mark us as requiring layout and skip
                return;
            }
            if (mLayoutFrozen) {
                mLayoutWasDefered = true;
                return; //we'll process updates when ice age ends.
            }
            consumePendingUpdateOperations();
        }
    };
    
    void consumePendingUpdateOperations() {
        // 重新执行一遍 dispatchLayoutStep1、dispatchLayoutStep2、dispatchLayoutStep3
        if (!mFirstLayoutComplete || mDataSetHasChangedAfterLayout) {
            dispatchLayout();
            return;
        }
        if (!mAdapterHelper.hasPendingUpdates()) {
            return;
        }

        // if it is only an item change (no add-remove-notifyDataSetChanged) we can check if any
        // of the visible items is affected and if not, just ignore the change.
        if (mAdapterHelper.hasAnyUpdateTypes(AdapterHelper.UpdateOp.UPDATE) && !mAdapterHelper
                .hasAnyUpdateTypes(AdapterHelper.UpdateOp.ADD | AdapterHelper.UpdateOp.REMOVE
                        | AdapterHelper.UpdateOp.MOVE)) { // 局部更新
            // 最终调用了 AdapterHelper 的 Callback 方法，Callback 的实现可以查看 RecyclerView 的 initAdapterManager 方法
            mAdapterHelper.preProcess();
            if (!mLayoutWasDefered) {
                if (hasUpdatedView()) {
                    dispatchLayout();
                } else {
                    // no need to layout, clean state
                    mAdapterHelper.consumePostponedUpdates();
                }
            }
        } else if (mAdapterHelper.hasPendingUpdates()) {
            dispatchLayout();
        }
    }

}
```

根据上面的分析，可以知道：
- RecyclerView 和 Adapter 之间是观察者、被观察者的关系，当 Adapter 调用了 notifyDataSetChanged 等方法时，RecyclerView 能够得知这个变化，并执行 layout 等相应的行为
- 如果 ReccylerView 设置了 mHasFixedSize，那么可以在 Adapter 被修改时执行 RecyclerView 的局部更新，从而避免重新走一遍三大流程，提高效率

# 总结

RecyclerView 的 measure 和 layout 分为三步：
- dispatchLayoutStep1，负责 Adpater 更新、计算并保存子 View 和动画的相关信息、预布局（如果有必要）
- dispatchLayoutStep2，负责实际上的布局，具体工作是交给 LayoutManager 完成的，但基本流程都一样，都有相同的子 View 复用机制
- dispatchLayoutStep3，负责执行动画及清理工作，默认的动画执行者是 DefaultItemAnimator

对比 RecyclerView 和 ListView：
- 同样在第一次 layout 时回调 Adapter 的相关方法从 xml 资源中加载子 View
- 同样会在第二第三次 layout 的先 remove / detach 所有的子 View，最后再 add / attach 回来，这是为了避免重复加载相同的子 View
- 同样只加载一个手机屏幕能显示的子 View
- 同样使用多个缓存数组/列表，且不同的缓存数组/列表对应的功能不同，RecylerView 的 mAttachedScrap 对应 ListView 的 mActiveViews，mCahcedViews 对应 mScrapViews
- 同样使用了观察者模式，在 Adapter 调用了 notifyDataSetChanged 等方法时，能够做出相应的改变

区别在于：
- 除了 attach / detach 之外，RecyclerView 还有可能会 remove 子 View 再 add 回来，ListView 只会 detach 再 attach
- RecyclerView 的子 View 缓存列表分工更细，共有 5 级缓存，即 mChangedScrap、mAttachedScrap、mCahcedViews、mViewCacheExtension、RecycledViewPool，其中 mViewCacheExtension 用于提供给用户自定义缓存逻辑，而 RecycledViewPool 甚至可以提供给多个 RecyclerView 共用；ListView 则只分为 mActiveViews 和 mScrapViews 两个级别
- RecyclerView 的缓存单位是 ViewHolder，ListView 的缓存单位是 View，ListView 需要配合 setTag 方法才能实现复用



