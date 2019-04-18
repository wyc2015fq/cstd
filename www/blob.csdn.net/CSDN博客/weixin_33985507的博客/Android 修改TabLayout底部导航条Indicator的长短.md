# Android 修改TabLayout底部导航条Indicator的长短 - weixin_33985507的博客 - CSDN博客
2018年08月09日 18:24:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：4
关于Tablayout，使用的应该很频繁了，但是底部导航条长短是固定死的，需要自己来改动长短，找了半天没找着方法，看了下官方建议，可以通过映射来修改自己想要的长短，其实也就几行代码的问题，看代码：
```
public static void setIndicator(Context context, TabLayout tabs, int leftDip, int rightDip) {
        Class<?> tabLayout = tabs.getClass();
        Field tabStrip = null;
        try {
            tabStrip = tabLayout.getDeclaredField("mTabStrip");
        } catch (NoSuchFieldException e) {
            e.printStackTrace();
        }
        tabStrip.setAccessible(true);
        LinearLayout ll_tab = null;
        try {
            ll_tab = (LinearLayout) tabStrip.get(tabs);
        } catch (IllegalAccessException e) {
            e.printStackTrace();
        }
        int left = (int) (getDisplayMetrics(context).density * leftDip);
        int right = (int) (getDisplayMetrics(context).density * rightDip);
        for (int i = 0; i < ll_tab.getChildCount(); i++) {
            View child = ll_tab.getChildAt(i);
            child.setPadding(0, 0, 0, 0);
            LinearLayout.LayoutParams params = new LinearLayout.LayoutParams(0, LinearLayout.LayoutParams.MATCH_PARENT, 1);
            params.leftMargin = left;
            params.rightMargin = right;
            child.setLayoutParams(params);
            child.invalidate();
        }
    }
```
leftDip和rightDip是左右间距，根据自己的需要来设置，直接调用即可。
```
很多同学都遇到问题，这里附上源码，大家可以看看：
package com.example.donghe.myrefreshview;
import android.app.Activity;
import android.content.Context;
import android.support.design.widget.TabLayout;
import android.support.v4.app.Fragment;
import android.support.v4.app.FragmentManager;
import android.support.v4.app.FragmentStatePagerAdapter;
import android.support.v4.view.ViewPager;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.DisplayMetrics;
import android.util.TypedValue;
import android.view.View;
import android.widget.LinearLayout;
import android.widget.TableLayout;
import java.lang.reflect.Field;
public class MainActivity extends AppCompatActivity {
    private static int TAB_MARGIN_DIP = 11;
    TabLayout tableLayout;
    ViewPager viewPager;
    RankingPagerAdapter adapter;
    String [] strings = {"皇帝","丞相","尚书","太监"};
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        tableLayout = (TabLayout) findViewById(R.id.tabs_ranking);
        viewPager = (ViewPager) findViewById(R.id.container_ranking);
        adapter = new RankingPagerAdapter(getSupportFragmentManager());
        viewPager.setAdapter(adapter);
        tableLayout.setupWithViewPager(viewPager);
        tableLayout.setTabTextColors(getResources().getColor(R.color.black), getResources().getColor(R.color.red_pre));
        tableLayout.setTabsFromPagerAdapter(adapter);
        setIndicator(this, tableLayout, TAB_MARGIN_DIP, TAB_MARGIN_DIP);
    }
    private class RankingPagerAdapter extends FragmentStatePagerAdapter {
        public RankingPagerAdapter(FragmentManager fm) {
            super(fm);
        }
        @Override
        public Fragment getItem(int position) {
            return new FuelFragment().newInstance(position, strings[position]);
        }
        @Override
        public int getCount() {
            return strings.length;
        }
        @Override
        public CharSequence getPageTitle(int position) {
            return strings[position];
        }
    }
    public static void setIndicator(Context context, TabLayout tabs, int leftDip, int rightDip) {
        Class<?> tabLayout = tabs.getClass();
        Field tabStrip = null;
        try {
            tabStrip = tabLayout.getDeclaredField("mTabStrip");
        } catch (NoSuchFieldException e) {
            e.printStackTrace();
        }
        tabStrip.setAccessible(true);
        LinearLayout ll_tab = null;
        try {
            ll_tab = (LinearLayout) tabStrip.get(tabs);
        } catch (IllegalAccessException e) {
            e.printStackTrace();
        }
        int left = (int) (getDisplayMetrics(context).density * leftDip);
        int right = (int) (getDisplayMetrics(context).density * rightDip);
        for (int i = 0; i < ll_tab.getChildCount(); i++) {
            View child = ll_tab.getChildAt(i);
            child.setPadding(0, 0, 0, 0);
            LinearLayout.LayoutParams params = new LinearLayout.LayoutParams(0, LinearLayout.LayoutParams.MATCH_PARENT, 1);
            params.leftMargin = left;
            params.rightMargin = right;
            child.setLayoutParams(params);
            child.invalidate();
        }
    }
    public static DisplayMetrics getDisplayMetrics(Context context) {
        DisplayMetrics metric = new DisplayMetrics();
        ((Activity) context).getWindowManager().getDefaultDisplay().getMetrics(metric);
        return metric;
    }
    public static float getPXfromDP(float value, Context context) {
        return TypedValue.applyDimension(TypedValue.COMPLEX_UNIT_DIP, value,
                context.getResources().getDisplayMetrics());
    }
}
```
Fragment：
```
package com.example.donghe.myrefreshview;
import android.os.Bundle;
import android.support.annotation.Nullable;
import android.support.v4.app.Fragment;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.TextView;
/**
 * Created by dong.he on 2016/12/9.
 */
public class FuelFragment extends Fragment {
    public static String TAB_POSITION;
    public static String TAB_NAME;
    private String tabId;
    private int tabPosition;
    public FuelFragment() {
    }
    public static Fragment newInstance(int tabPosition, String tabName) {
        FuelFragment fragment = new FuelFragment();
        Bundle bundle = new Bundle();
        bundle.putInt(TAB_POSITION, tabPosition);
        bundle.putString(TAB_NAME, tabName);
        fragment.setArguments(bundle);
        return fragment;
    }
    @Nullable
    @Override
    public View onCreateView(LayoutInflater inflater, @Nullable ViewGroup container, @Nullable Bundle savedInstanceState) {
        return inflater.inflate(R.layout.layout, container, false);
    }
    @Override
    public void onActivityCreated(@Nullable Bundle savedInstanceState) {
        super.onActivityCreated(savedInstanceState);
        tabPosition = getArguments().getInt(TAB_POSITION, 0);
        tabId = getArguments().getString(TAB_NAME);
        ((TextView) getView().findViewById(R.id.text)).setText(tabId);
    }
}
```
布局：
```
<?xml version="1.0" encoding="utf-8"?>
<LinearLayout xmlns:android="http://schemas.android.com/apk/res/android"
    xmlns:app="http://schemas.android.com/apk/res-auto"
    android:layout_width="match_parent"
    android:layout_height="match_parent"
    android:orientation="vertical">
    <android.support.design.widget.TabLayout
        android:id="@+id/tabs_ranking"
        android:layout_width="match_parent"
        android:layout_height="wrap_content"
        app:tabGravity="fill"
        app:tabIndicatorColor="@color/red_pre"
        app:tabMode="scrollable" />
    <android.support.v4.view.ViewPager
        android:id="@+id/container_ranking"
        android:layout_width="match_parent"
        android:layout_height="match_parent" />
</LinearLayout>
```
有问题的欢迎垂询。 
![这里写图片描述](https://img-blog.csdn.net/20161209105145200?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaGVkb25nXzc3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
