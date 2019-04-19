# 【Android】保存Fragment切换状态 - =朝晖= - 博客园
# [【Android】保存Fragment切换状态](https://www.cnblogs.com/dhcn/p/7130746.html)
http://www.cnblogs.com/over140/p/3362047.html
**前言**
一般频繁切换Fragment会导致频繁的释放和创建，如果Fragment比较臃肿体验就非常不好了，这里分享一个方法。
声明
**正文**
一、应用场景
1、不使用ViewPager
2、不能用replace来切换Fragment，会导致Fragment释放（调用onDestroyView）
二、实现
1、xml
```
<LinearLayout xmlns:android="http://schemas.android.com/apk/res/android"
    android:layout_width="match_parent"
    android:layout_height="match_parent"
    android:orientation="vertical" >
    <FrameLayout
        android:id="@+id/container"
        android:layout_width="match_parent"
        android:layout_height="0dip"
        android:layout_weight="1.0" >
    </FrameLayout>
    <RadioGroup
        android:id="@+id/main_radio"
        android:layout_width="fill_parent"
        android:layout_height="wrap_content"
        android:layout_gravity="bottom"
        android:gravity="bottom"
        android:layout_marginBottom="-6dp"
        android:orientation="horizontal" >
        <RadioButton
            android:id="@+id/radio_button0"
            style="@style/main_tab_bottom"
            android:drawableTop="@drawable/bottom_1" />
        <RadioButton
            android:id="@+id/radio_button1"
            style="@style/main_tab_bottom"
            android:drawableTop="@drawable/bottom_2" />
        <RadioButton
            android:id="@+id/radio_button2"
            style="@style/main_tab_bottom"
            android:drawableTop="@drawable/bottom_3" />
        <RadioButton
            android:id="@+id/radio_button3"
            style="@style/main_tab_bottom"
            android:drawableTop="@drawable/bottom_4" />
        <RadioButton
            android:id="@+id/radio_button4"
            style="@style/main_tab_bottom"
            android:drawableTop="@drawable/bottom_5" />
    </RadioGroup>
</LinearLayout>
```
代码说明：
非常常见的底部放5个RadioButton，点击切换不同的Fragment。
2、Activity
为RadioButton设置setOnCheckedChangeListener事件，其他代码：
```
@Override
    public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
        if (isChecked) {
            Fragment fragment = (Fragment) mFragmentPagerAdapter.instantiateItem(mContainer, buttonView.getId());
            mFragmentPagerAdapter.setPrimaryItem(mContainer, 0, fragment);
            mFragmentPagerAdapter.finishUpdate(mContainer);
        } 
    }
    private FragmentPagerAdapter mFragmentPagerAdapter = new FragmentPagerAdapter(getSupportFragmentManager()) {
        @Override
        public Fragment getItem(int position) {
            switch (position) {
            case R.id.radio_button1:
                return new Fragment1();
            case R.id.radio_button2:
                return new Fragment2();
            case R.id.radio_button3:
                return new Fragment3();
            case R.id.radio_button4:
                return new Fragment4();
            case R.id.radio_button0:
            default:
                return new Fragment0();
            }
        }
        @Override
        public int getCount() {
            return 5;
        }
    };
```
代码说明：
instantiateItem从FragmentManager中查找Fragment，找不到就getItem新建一个，setPrimaryItem设置隐藏和显示，最后finishUpdate提交事务。
mContainer就是xml中的FrameLayout。 
三、FragmentPagerAdapter核心代码
```
@Override
    public Object instantiateItem(ViewGroup container, int position) {
        if (mCurTransaction == null) {
            mCurTransaction = mFragmentManager.beginTransaction();
        }
        final long itemId = getItemId(position);
        // Do we already have this fragment?
        String name = makeFragmentName(container.getId(), itemId);
        Fragment fragment = mFragmentManager.findFragmentByTag(name);
        if (fragment != null) {
            if (DEBUG) Log.v(TAG, "Attaching item #" + itemId + ": f=" + fragment);
            mCurTransaction.attach(fragment);
        } else {
            fragment = getItem(position);
            if (DEBUG) Log.v(TAG, "Adding item #" + itemId + ": f=" + fragment);
            mCurTransaction.add(container.getId(), fragment,
                    makeFragmentName(container.getId(), itemId));
        }
        if (fragment != mCurrentPrimaryItem) {
            fragment.setMenuVisibility(false);
            fragment.setUserVisibleHint(false);
        }
        return fragment;
    }
    @Override
    public void destroyItem(ViewGroup container, int position, Object object) {
        if (mCurTransaction == null) {
            mCurTransaction = mFragmentManager.beginTransaction();
        }
        if (DEBUG) Log.v(TAG, "Detaching item #" + getItemId(position) + ": f=" + object
                + " v=" + ((Fragment)object).getView());
        mCurTransaction.detach((Fragment)object);
    }
    @Override
    public void setPrimaryItem(ViewGroup container, int position, Object object) {
        Fragment fragment = (Fragment)object;
        if (fragment != mCurrentPrimaryItem) {
            if (mCurrentPrimaryItem != null) {
                mCurrentPrimaryItem.setMenuVisibility(false);
                mCurrentPrimaryItem.setUserVisibleHint(false);
            }
            if (fragment != null) {
                fragment.setMenuVisibility(true);
                fragment.setUserVisibleHint(true);
            }
            mCurrentPrimaryItem = fragment;
        }
    }
    @Override
    public void finishUpdate(ViewGroup container) {
        if (mCurTransaction != null) {
            mCurTransaction.commitAllowingStateLoss();
            mCurTransaction = null;
            mFragmentManager.executePendingTransactions();
        }
    }
```
 FragmentPagerAdapter是support包自带的类。
四、注意 
之前自己模拟ViewPager用attach、setMenuVisibility、setUserVisibleHint来控制Fragment的显示隐藏，经常会出现Fragment重叠现象，非常头疼，换了这个之后目前没有发现重叠现象。
五、文章后期维护
2013-12-01 上传示例代码：[http://files.cnblogs.com/over140/SampleFragmentSwitch.zip](http://files.cnblogs.com/over140/SampleFragmentSwitch.zip)
```
@Override
    public void setMenuVisibility(boolean menuVisible) {
        super.setMenuVisibility(menuVisible);
        if (this.getView() != null)
            this.getView().setVisibility(menuVisible ? View.VISIBLE : View.GONE);
    }
```
重新做例子时发现自己也出不来效果了，后来发现少了这段代码。
2014-01-08想实现本文的效果还是推荐直接使用ViewPager，通过自定义ViewPager禁用掉左右滑动和自动销毁即可，根据评论来看非正常情况下重影现象还是挺严重的。 
**结束**
需要多看看源码，才能很好的解决问题。 

