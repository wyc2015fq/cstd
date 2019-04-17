# Android字母导航示例 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2016年09月18日 14:43:01[boonya](https://me.csdn.net/boonya)阅读数：2674








文章转自:[http://blog.csdn.net/qq_26787115/article/details/52502787](http://blog.csdn.net/qq_26787115/article/details/52502787)


论字母导航的重要性，我们来实现一个联系人字母导航列表吧！

> 
说起这个字母导航，我相信大家都不陌生,不论是联系人列表还是城市列表，基本上都是需要字母导航，那我们就有必要来研究一下这个思路的探索了，毕竟这是一个非常常用的功能，如果现在把轮子造好，那以后也可以节省很多的时间，同时，我们把思路理清楚了，对我们以后的帮助也是很大的，那好，既然如此，我们就一起来探索一下吧！


![这里写图片描述](https://img-blog.csdn.net/20160917155947230)

> 
我们首选新建一个项目——LettersNavigation


![这里写图片描述](https://img-blog.csdn.net/20160917160231113)

> 
OK,工程建立好之后我们来思考一下这个功能的一个实现逻辑


![这里写图片描述](https://img-blog.csdn.net/20160917160258093)

> 
逻辑不是很难，那我们首先要去做的就是把大致的框架搭好，也就是布局，我们来看


## activity_main.xml



```
<?xml version="1.0" encoding="utf-8"?>
<LinearLayout
    xmlns:android="http://schemas.android.com/apk/res/android"
    android:layout_width="match_parent"
    android:layout_height="match_parent"
    android:orientation="vertical">

    <!--搜索框-->
    <RelativeLayout
        android:layout_width="match_parent"
        android:layout_height="wrap_content"
        android:layout_marginLeft="5dp"
        android:layout_marginRight="5dp"
        android:layout_marginTop="5dp"
        android:alpha="0.5"
        android:background="@drawable/search_bg">

        <ImageView
            android:id="@+id/iv_search"
            android:layout_width="40dp"
            android:layout_height="40dp"
            android:src="@drawable/ic_search"/>

        <EditText
            android:id="@+id/et_search"
            android:layout_width="match_parent"
            android:layout_height="40dp"
            android:layout_alignParentTop="true"
            android:layout_marginRight="10dp"
            android:layout_toRightOf="@+id/iv_search"
            android:background="@null"
            android:hint="请输入联系人"
            android:singleLine="true"
            android:textColor="@android:color/white"
            android:textColorHint="@android:color/white"/>

        <ImageView
            android:id="@+id/tv_cancel"
            android:layout_width="30dp"
            android:layout_height="30dp"
            android:layout_alignParentRight="true"
            android:layout_centerVertical="true"
            android:layout_marginRight="10dp"
            android:src="@drawable/titlebar_cancel"
            android:visibility="gone"/>
    </RelativeLayout>

    <!--列表-->
    <FrameLayout
        android:layout_width="match_parent"
        android:layout_height="match_parent">

        <ListView
            android:id="@+id/mListView"
            android:layout_width="match_parent"
            android:layout_height="match_parent"
            android:divider="@null"/>

        <!--字母提示-->
        <TextView
            android:id="@+id/tvToast"
            android:layout_width="100dp"
            android:layout_height="100dp"
            android:layout_gravity="center"
            android:alpha="0.5"
            android:background="@drawable/toast_bg"
            android:gravity="center"
            android:text="A"
            android:textColor="@android:color/white"
            android:textSize="25sp"
            android:visibility="gone"/>

        <!--字母导航-->
        <com.liuguilin.lettersnavigation.view.LettersView
            android:id="@+id/mLettersView"
            android:layout_width="30dp"
            android:layout_height="match_parent"
            android:layout_gravity="right"/>

    </FrameLayout>

</LinearLayout>
```
            这里就比较好理解了，首选是最上面的一个搜索框，我们监听里面的内容，有内容的haunted我就显示清除按钮，下面是一个FrameLayout，他包裹着一个ListView显示联系人，一个TextView是作为字母提示的，最右边就是我们的自定义字母导航栏LettersView了，同样的，因为主页有输入框，他只要一进去就会获取到焦点弹起输入法，这样的交互式不友好的，我们在清单文件中的Activity根节点加一个属性:


`android:windowSoftInputMode="adjustUnspecified|stateHidden"`
> 
还有我们搜索的输入框的背景，因为我没有找到合适的.9图片，PS也不是很懂，就自己绘制了一个圆角的背景


## search_bg.xml



```
<?xml version="1.0" encoding="utf-8"?>
<shape xmlns:android="http://schemas.android.com/apk/res/android">
    <solid android:color="@color/colorPrimary"/>
    <corners android:radius="20dp"/>

</shape>
```

> 
同理，还有我们字母提示TextView的背景


## toast_bg.xml



```
<?xml version="1.0" encoding="utf-8"?>
<shape xmlns:android="http://schemas.android.com/apk/res/android">
    <solid android:color="@android:color/black"/>
    <corners android:radius="10dp"/>

</shape>
```
         在我看来，xml绘制动画也好，显示效果也好，都是十分的方便的，这个一定要掌握以下哦！，好的，前期的架子搭完了，我们现在可以重点来看一下这个字母View了，其实他就两个步骤，在我看来，首先绘制这个View，然后通过手势监听去逐帧他的选中字母，拿这样的话，我们的绘制应该是这个样子的：



```java
/**
     * 绘制
     *
     * @param canvas
     */
    @Override
    protected void onDraw(Canvas canvas) {
        super.onDraw(canvas);
        /**
         * 为了排列26个字母，我们可以用坐标点来计算，X居中，Y为 1/27 的递加计算
         * 首先获取到我们View的宽高
         */
        int viewWidth = getWidth();
        int viewHeight = getHeight();
        //计算一个字母的高度
        int singleHeight = viewHeight / strChars.length;
        //循环绘制字母
        for (int i = 0; i < strChars.length; i++) {
            //设置选中字母的颜色
            if (i == checkIndex) {
                mPaint.setColor(Color.WHITE);
                mPaint.setTextSize(50);
            } else {
                mPaint.setColor(Color.BLACK);
                //设置字体大小
                mPaint.setTextSize(40);
            }
            /**
             * 绘制字母
             * x: （view的宽度 - 文本的宽度）/ 2
             * y:  singleHeight * x + singleHeight  //单个字母的高度 + 最上面的字幕空白高度
             */
            float lettersX = (viewWidth - mPaint.measureText(strChars[i])) / 2;
            float lettersY = singleHeight * i + singleHeight;
            //绘制
            canvas.drawText(strChars[i], lettersX, lettersY, mPaint);
            //重绘
            mPaint.reset();
        }
    }
```

> 
然后我们就可以来预览以下效果了


![这里写图片描述](https://img-blog.csdn.net/20160917160627050)


感觉还是有点挫呀，不过先不管，我们仔细看下这个onDraw方法，其实我们主要还是计算x,y的坐标然后去绘制，在绘制的时候我们定义一个选中的下标，，默认为0，所以默认选中A，好的，那我们再来实现手势的监听吧！这里我们用事件分发来做是最合适的了



```java
/**
     * 事件分发
     *
     * @param event
     * @return
     */
    @Override
    public boolean dispatchTouchEvent(MotionEvent event) {
        //判断手势
        switch (event.getAction()) {
            case MotionEvent.ACTION_DOWN:
            case MotionEvent.ACTION_MOVE:
                setBackgroundResource(R.color.colorAccent);
                //获取点击的Y坐标，以此来判断选中的字母
                float y = event.getY();
                Log.i(TAG, "y:" + y);
                //第一次被选中的下标
                int oldCheckIndex = checkIndex;
                /**
                 * 计算选中的字母
                 * strChars[当前Y / View的高度 * 字母个数]
                 */
                int c = (int) (y / getHeight() * strChars.length);
                Log.i(TAG, "c:" + c);
                //判断移动
                if (oldCheckIndex != c) {
                    //不能越界
                    if (c >= 0 && c < strChars.length) {
                        //效果联动
                        if (onLettersListViewListener != null) {
                            onLettersListViewListener.onLettersListener(strChars[c]);
                        }
                        if (mTextView != null) {
                            mTextView.setVisibility(View.VISIBLE);
                            mTextView.setText(strChars[c]);
                        }
                    }
                    checkIndex = c;
                    invalidate();
                }
                break;
            case MotionEvent.ACTION_UP:
                //设置透明背景
                setBackgroundResource(android.R.color.transparent);
                //恢复不选中
                checkIndex = -1;
                invalidate();
                //是否显示
                if (mTextView != null) {
                    mTextView.setVisibility(View.INVISIBLE);
                }
                break;
        }
        return true;
    }
```

> 
其实这里也是比较简单的，不过这里有一个set/get的TextView，需要在MainActivity里面去设置一下



```java
mLettersView = (LettersView) findViewById(R.id.mLettersView);
mLettersView.setmTextView(tvToast);
```

这样我们就可以看到实际的效果了


![这里写图片描述](https://img-blog.csdn.net/20160917160743272)


这个View当然不止做这么一点点事情了，我们等下还需要他与ListView去联动呢，那既然如此，我们先把ListView给撸出来，这个ListView其实很简单，不要看我们的不规则排列，其实他的原理就是item上有两个TextView，上面是字母，下面是姓名，当然，我们要去转换判断一下并且计算出谁该显示字母，谁有需要去隐藏，所以我们的item应该是

## list_item.xml



```
<?xml version="1.0" encoding="utf-8"?>
<LinearLayout
    xmlns:android="http://schemas.android.com/apk/res/android"
    android:layout_width="match_parent"
    android:layout_height="match_parent"
    android:orientation="vertical">

    <TextView
        android:layout_marginLeft="10dp"
        android:id="@+id/tvLetters"
        android:layout_width="wrap_content"
        android:layout_height="wrap_content"
        android:text="A"
        android:textColor="@color/colorAccent"
        android:textSize="20sp"/>

    <TextView
        android:layout_marginLeft="10dp"
        android:id="@+id/tvName"
        android:layout_width="wrap_content"
        android:layout_height="wrap_content"
        android:layout_marginTop="5dp"
        android:text="阿三"
        android:textSize="16sp"/>

</LinearLayout>
```
然后就是我们的实体类了

## LettersModel



```java
package com.liuguilin.lettersnavigation.entity;

/*
 *  项目名：  LettersNavigation 
 *  包名：    com.liuguilin.lettersnavigation.entity
 *  文件名:   LettersModel
 *  创建者:   LGL
 *  创建时间:  2016/9/11 16:55
 *  描述：    联系人实体
 */

public class LettersModel {

    //字母
    private String letter;
    //联系人
    private String name;

    public String getLetter() {
        return letter;
    }

    public void setLetter(String letter) {
        this.letter = letter;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }
}
```
当然，我们又两个重要的环节，一个是数据，还有一个适配器，我们先来看下数据怎么去用，首先我们模拟一些数据



```java
//联系人数据模拟
    private String[] strName = {"张三", "李四", "李七", "刘某人", "王五", "Android", "IOS", "王寡妇","阿三", "爸爸", "妈妈", "CoCo", "弟弟", "尔康", "紫薇", "小燕子", "皇阿玛", "福尔康", "哥哥", "Hi", "I", "杰克", "克星", "乐乐", "你好", "Oppo", "皮特", "曲奇饼","日啊", "思思", "缇娜", "U", "V", "王大叔", "嘻嘻", "一小伙子", "撒贝宁", "吱吱", "舅舅", "老总", "隔壁老王", "许仙"};
```
然后我们写个方法



```java
/**
     * 联系人数组转换实体数据
     *
     * @return
     */
    private List<LettersModel> parsingData() {
        List<LettersModel> listModel = new ArrayList<>();
        Log.i(TAG, " strName.length:" + strName.length);
        for (int i = 0; i < strName.length; i++) {
            LettersModel model = new LettersModel();
            model.setName(strName[i]);
            Log.i(TAG, strName[i]);
            //转换拼音截取首字母并且大写
            String pinyin = Trans2PinYin.trans2PinYin(strName[i]);
            Log.i(TAG, "pinyin:" + pinyin);
            String letter = pinyin.substring(0, 1).toUpperCase();
            Log.i(TAG, "letter:" + letter);
            model.setLetter(letter);
            listModel.add(model);
        }
        return listModel;
    }
```


这个方法主要是把数据填充到我们的数据集里面，然后我们就要进行排序了，我们排序的规则是A-Z #，那我们就得自定义一个排序规则

## LettersSorting



```java
package com.liuguilin.lettersnavigation.utils;

/*
 *  项目名：  LettersNavigation 
 *  包名：    com.liuguilin.lettersnavigation.utils
 *  文件名:   LettersSorting
 *  创建者:   LGL
 *  创建时间:  2016/9/11 18:04
 *  描述：    字母排序算法
 */

import com.liuguilin.lettersnavigation.entity.LettersModel;

import java.util.Comparator;

public class LettersSorting implements Comparator<LettersModel> {

    @Override
    public int compare(LettersModel lettersModel, LettersModel t1) {
        //给我两个对象，我只比较他的字母
        return lettersModel.getLetter().compareTo(t1.getLetter());
    }
}
```


这样我们就可以调通了，我把MainActivity的代码全部贴上你就能看的十分的清晰了

## MainActivity



```java
package com.liuguilin.lettersnavigation;

import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.text.Editable;
import android.text.TextWatcher;
import android.util.Log;
import android.view.View;
import android.widget.EditText;
import android.widget.ImageView;
import android.widget.ListView;
import android.widget.TextView;

import com.liuguilin.lettersnavigation.adapter.LettersAdapter;
import com.liuguilin.lettersnavigation.entity.LettersModel;
import com.liuguilin.lettersnavigation.utils.LettersSorting;
import com.liuguilin.lettersnavigation.utils.Trans2PinYin;
import com.liuguilin.lettersnavigation.view.LettersView;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

public class MainActivity extends AppCompatActivity implements LettersView.OnLettersListViewListener, View.OnClickListener {

    //TAG
    private static final String TAG = "Letters";

    //联系人列表
    private ListView mListView;
    //字母提示
    private TextView tvToast;
    //字母列表
    private LettersView mLettersView;
    //清除按钮
    private ImageView tv_cancel;
    //搜索框
    private EditText et_search;
    //列表数据
    private List<LettersModel> mList = new ArrayList<>();
    //数据源
    private LettersAdapter adapter;
    //联系人数据模拟
    private String[] strName = {"张三", "李四", "李七", "刘某人", "王五", "Android", "IOS", "王寡妇",
            "阿三", "爸爸", "妈妈", "CoCo", "弟弟", "尔康", "紫薇", "小燕子", "皇阿玛", "福尔康", "哥哥", "Hi", "I", "杰克", "克星", "乐乐", "你好", "Oppo", "皮特", "曲奇饼",
            "日啊", "思思", "缇娜", "U", "V", "王大叔", "嘻嘻", "一小伙子", "撒贝宁", "吱吱", "舅舅", "老总", "隔壁老王", "许仙"};

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        initView();
    }

    /**
     * 初始化View
     */
    private void initView() {
        tv_cancel = (ImageView) findViewById(R.id.tv_cancel);
        tv_cancel.setOnClickListener(this);
        mListView = (ListView) findViewById(R.id.mListView);
        tvToast = (TextView) findViewById(R.id.tvToast);
        et_search = (EditText) findViewById(R.id.et_search);
        et_search.addTextChangedListener(new TextWatcher() {
            @Override
            public void beforeTextChanged(CharSequence charSequence, int i, int i1, int i2) {

            }

            @Override
            public void onTextChanged(CharSequence charSequence, int i, int i1, int i2) {

            }

            @Override
            public void afterTextChanged(Editable editable) {
                if (editable.length() > 0) {
                    tv_cancel.setVisibility(View.VISIBLE);
                } else {
                    tv_cancel.setVisibility(View.GONE);
                }
            }
        });
        mLettersView = (LettersView) findViewById(R.id.mLettersView);
        mLettersView.setmTextView(tvToast);
        //绑定
        mLettersView.setOnLettersListViewListener(this);

        //加载联系人的模拟数据
        mList = parsingData();
        //对字母进行排序A-Z #
        Collections.sort(mList, new LettersSorting());
        //加载适配器
        adapter = new LettersAdapter(this, mList);
        //设置数据
        mListView.setAdapter(adapter);
    }

    /**
     * 联系人数组转换实体数据
     *
     * @return
     */
    private List<LettersModel> parsingData() {
        List<LettersModel> listModel = new ArrayList<>();
        Log.i(TAG, " strName.length:" + strName.length);
        for (int i = 0; i < strName.length; i++) {
            LettersModel model = new LettersModel();
            model.setName(strName[i]);
            Log.i(TAG, strName[i]);
            //转换拼音截取首字母并且大写
            String pinyin = Trans2PinYin.trans2PinYin(strName[i]);
            Log.i(TAG, "pinyin:" + pinyin);
            String letter = pinyin.substring(0, 1).toUpperCase();
            Log.i(TAG, "letter:" + letter);
            model.setLetter(letter);
            listModel.add(model);
        }
        return listModel;
    }

    /**
     * ListView与字母导航联动
     *
     * @param s
     */
    @Override
    public void onLettersListener(String s) {
        //对应的位置
        int position = adapter.getPositionForNmae(s.charAt(0));
        //移动
        mListView.setSelection(position);
    }

    /**
     * 点击事件
     *
     * @param view
     */
    @Override
    public void onClick(View view) {
        switch (view.getId()) {
            case R.id.tv_cancel:
                et_search.setText("");
                tv_cancel.setVisibility(View.GONE);
                break;
        }
    }
}
```


OK，正如你所见，我们最重要的是LettersAdapter，我们先来分析一下这个adapter,其实他要做的就两件事情，首选把字母的第一个排出来，然后依次显示数据，我们直接看代码



```java
package com.liuguilin.lettersnavigation.adapter;

/*
 *  项目名：  LettersNavigation 
 *  包名：    com.liuguilin.lettersnavigation.adapter
 *  文件名:   LettersAdapter
 *  创建者:   LGL
 *  创建时间:  2016/9/11 18:12
 *  描述：    联系人适配器
 */

import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.TextView;

import com.liuguilin.lettersnavigation.R;
import com.liuguilin.lettersnavigation.entity.LettersModel;

import java.util.List;

public class LettersAdapter extends BaseAdapter {

    //上下文
    private Context mContext;
    //数据集
    private List<LettersModel> mList;
    //布局加载器
    private LayoutInflater mInflater;
    //实体类
    private LettersModel model;

    public LettersAdapter(Context mContext, List<LettersModel> mList) {
        this.mContext = mContext;
        this.mList = mList;

        //获取系统服务
        mInflater = (LayoutInflater) mContext.getSystemService(Context.LAYOUT_INFLATER_SERVICE);
    }

    @Override
    public int getCount() {
        return mList.size();
    }

    @Override
    public Object getItem(int i) {
        return mList.get(i);
    }

    @Override
    public long getItemId(int i) {
        return i;
    }

    @Override
    public View getView(int i, View view, ViewGroup viewGroup) {
        ViewHolder vHolder = null;
        if (view == null) {
            vHolder = new ViewHolder();
            //加载布局
            view = mInflater.inflate(R.layout.list_item, null);
            vHolder.tvLetters = (TextView) view.findViewById(R.id.tvLetters);
            vHolder.tvName = (TextView) view.findViewById(R.id.tvName);
            view.setTag(vHolder);
        } else {
            vHolder = (ViewHolder) view.getTag();
        }
        //选中下标
        model = mList.get(i);
        //获取首字母显示人
        int firstPosition = getNmaeForPosition(i);
        //第一个
        int index = getPositionForNmae(firstPosition);
        //需要显示字母
        if (index == i) {
            vHolder.tvLetters.setVisibility(View.VISIBLE);
            vHolder.tvLetters.setText(model.getLetter());
        } else {
            vHolder.tvLetters.setVisibility(View.GONE);
        }
        vHolder.tvName.setText(model.getName());
        return view;
    }

    /**
     * 缓存优化
     */
    class ViewHolder {
        private TextView tvLetters;
        private TextView tvName;
    }

    /**
     * 通过首字母获取该首字母要显示的第一个人的下标
     *
     * @param position
     * @return
     */
    public int getPositionForNmae(int position) {
        for (int i = 0; i < getCount(); i++) {
            String letter = mList.get(i).getLetter();
            //首字母显示
            char firstChar = letter.toUpperCase().charAt(0);
            if (firstChar == position) {
                return i;
            }
        }
        return -1;
    }

    /**
     * 根据名称拿到下标
     *
     * @param position
     * @return
     */
    private int getNmaeForPosition(int position) {
        return mList.get(position).getLetter().charAt(0);
    }

}
```


这里最关键的也就在于getPositionForNmae方法他直接计算出了我们一堆同类型的联系人中的第一个数据并且返回出来，然后我们再利用getNmaeForPosition拿到需要显示的position,OK，到这里，我们基本上就可以看到很大一部分效果了

![这里写图片描述](https://img-blog.csdn.net/20160917161138961)


现在这个大体的架构就已经搭建出来了，我们现在最后的难点就是如何字母导航和ListView的联动了，我们只能回到我们的LettersView，其实这个说难也不难，主要还是一个思想，我们可以用接口的形式去做，首先我们定义一个接口



```java
/**
     * 接口回调/ListView联动
     */
    public interface OnLettersListViewListener {
        public void onLettersListener(String s);
    }
```
并且实现它的set/get方法，然后我们在选中的手势事件中，我们去直接设置



```java
//效果联动
 if (onLettersListViewListener != null) {
      onLettersListViewListener.onLettersListener(strChars[c]);
  }
```


这里我们回传回去的是我们选中的字母，我们再回到我们的MainActivity，然后直接



```java
implements LettersView.OnLettersListViewListener
```


设置了我们的监听之后，我们可以直接移动



```java
/**
     * ListView与字母导航联动
     *
     * @param s
     */
    @Override
    public void onLettersListener(String s) {
        //对应的位置
        int position = adapter.getPositionForNmae(s.charAt(0));
        //移动
        mListView.setSelection(position);
    }
```


我们可以来看一下最终的效果

![这里写图片描述](https://img-blog.csdn.net/20160917161408478)

> 
现在思路是不是一场的清晰？这里我把这个View的代码贴上


## LettersView



```java
package com.liuguilin.lettersnavigation.view;

/*
 *  项目名：  LettersNavigation 
 *  包名：    com.liuguilin.lettersnavigation.view
 *  文件名:   LettersView
 *  创建者:   LGL
 *  创建时间:  2016/9/11 11:58
 *  描述：    字母导航
 */

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Typeface;
import android.util.AttributeSet;
import android.util.Log;
import android.view.MotionEvent;
import android.view.View;
import android.widget.TextView;

import com.liuguilin.lettersnavigation.R;

public class LettersView extends View {

    //TAG
    private static final String TAG = "LettersView";

    //字母数组,#代表未知，比如数字开头
    private String[] strChars = {"A", "B", "C", "D", "E", "F", "G", "H", "I", "J",
            "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z", "#"};
    //画笔
    private Paint mPaint;
    //选中字母的下标
    private int checkIndex;
    //字母提示的TextView,需要set/get动态设置显示内容
    private TextView mTextView;
    //接口回调
    private OnLettersListViewListener onLettersListViewListener;

    public LettersView(Context context) {
        super(context);
        init();
    }

    public LettersView(Context context, AttributeSet attrs) {
        super(context, attrs);
        init();
    }

    public LettersView(Context context, AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);
        init();
    }

    /**
     * 初始化
     */
    private void init() {
        //实例化画笔
        mPaint = new Paint();
        //设置style
        mPaint.setTypeface(Typeface.DEFAULT_BOLD);
        //设置抗锯齿
        mPaint.setAntiAlias(true);
    }

    /**
     * 绘制
     *
     * @param canvas
     */
    @Override
    protected void onDraw(Canvas canvas) {
        super.onDraw(canvas);
        /**
         * 为了排列26个字母，我们可以用坐标点来计算，X居中，Y为 1/27 的递加计算
         * 首先获取到我们View的宽高
         */
        int viewWidth = getWidth();
        int viewHeight = getHeight();
        //计算一个字母的高度
        int singleHeight = viewHeight / strChars.length;
        //循环绘制字母
        for (int i = 0; i < strChars.length; i++) {
            //设置选中字母的颜色
            if (i == checkIndex) {
                mPaint.setColor(Color.WHITE);
                mPaint.setTextSize(50);
            } else {
                mPaint.setColor(Color.BLACK);
                //设置字体大小
                mPaint.setTextSize(40);
            }
            /**
             * 绘制字母
             * x: （view的宽度 - 文本的宽度）/ 2
             * y:  singleHeight * x + singleHeight  //单个字母的高度 + 最上面的字幕空白高度
             */
            float lettersX = (viewWidth - mPaint.measureText(strChars[i])) / 2;
            float lettersY = singleHeight * i + singleHeight;
            //绘制
            canvas.drawText(strChars[i], lettersX, lettersY, mPaint);
            //重绘
            mPaint.reset();
        }
    }

    public TextView getmTextView() {
        return mTextView;
    }

    public void setmTextView(TextView mTextView) {
        this.mTextView = mTextView;
    }

    /**
     * 事件分发
     *
     * @param event
     * @return
     */
    @Override
    public boolean dispatchTouchEvent(MotionEvent event) {
        //判断手势
        switch (event.getAction()) {
            case MotionEvent.ACTION_DOWN:
            case MotionEvent.ACTION_MOVE:
                setBackgroundResource(R.color.colorAccent);
                //获取点击的Y坐标，以此来判断选中的字母
                float y = event.getY();
                Log.i(TAG, "y:" + y);
                //第一次被选中的下标
                int oldCheckIndex = checkIndex;
                /**
                 * 计算选中的字母
                 * strChars[当前Y / View的高度 * 字母个数]
                 */
                int c = (int) (y / getHeight() * strChars.length);
                Log.i(TAG, "c:" + c);
                //判断移动
                if (oldCheckIndex != c) {
                    //不能越界
                    if (c >= 0 && c < strChars.length) {
                        //效果联动
                        if (onLettersListViewListener != null) {
                            onLettersListViewListener.onLettersListener(strChars[c]);
                        }
                        if (mTextView != null) {
                            mTextView.setVisibility(View.VISIBLE);
                            mTextView.setText(strChars[c]);
                        }
                    }
                    checkIndex = c;
                    invalidate();
                }
                break;
            case MotionEvent.ACTION_UP:
                //设置透明背景
                setBackgroundResource(android.R.color.transparent);
                //恢复不选中
                checkIndex = -1;
                invalidate();
                //是否显示
                if (mTextView != null) {
                    mTextView.setVisibility(View.INVISIBLE);
                }
                break;
        }
        return true;
    }

    public OnLettersListViewListener getOnLettersListViewListener() {
        return onLettersListViewListener;
    }

    public void setOnLettersListViewListener(OnLettersListViewListener onLettersListViewListener) {
        this.onLettersListViewListener = onLettersListViewListener;
    }

    /**
     * 接口回调/ListView联动
     */
    public interface OnLettersListViewListener {
        public void onLettersListener(String s);
    }
}
```



ok,这里最终小伙伴是不是还有一个疑问，就是我们使用到的Trans2PinYin这个类是什么？事实上，这个类的唯一功能就是把汉子转化成拼音的工具类，在android官方的通讯录中就有这个类，有兴趣的可以去看一下源码，最后我把这个类的源码贴上：

## Trans2PinYin



```java
package com.liuguilin.lettersnavigation.utils;

/*
 *  项目名：  HanZiToPinYinSample 
 *  包名：    com.liuguilin.hanzitopinyinsample
 *  文件名:   Trans2PinYin
 *  创建者:   LGL
 *  创建时间:  2016/8/23 17:05
 *  描述：    汉字转拼音
 */

public class Trans2PinYin {

    private static int[] pyvalue = new int[]{-20319, -20317, -20304, -20295,
            -20292, -20283, -20265, -20257, -20242, -20230, -20051, -20036,
            -20032, -20026, -20002, -19990, -19986, -19982, -19976, -19805,
            -19784, -19775, -19774, -19763, -19756, -19751, -19746, -19741,
            -19739, -19728, -19725, -19715, -19540, -19531, -19525, -19515,
            -19500, -19484, -19479, -19467, -19289, -19288, -19281, -19275,
            -19270, -19263, -19261, -19249, -19243, -19242, -19238, -19235,
            -19227, -19224, -19218, -19212, -19038, -19023, -19018, -19006,
            -19003, -18996, -18977, -18961, -18952, -18783, -18774, -18773,
            -18763, -18756, -18741, -18735, -18731, -18722, -18710, -18697,
            -18696, -18526, -18518, -18501, -18490, -18478, -18463, -18448,
            -18447, -18446, -18239, -18237, -18231, -18220, -18211, -18201,
            -18184, -18183, -18181, -18012, -17997, -17988, -17970, -17964,
            -17961, -17950, -17947, -17931, -17928, -17922, -17759, -17752,
            -17733, -17730, -17721, -17703, -17701, -17697, -17692, -17683,
            -17676, -17496, -17487, -17482, -17468, -17454, -17433, -17427,
            -17417, -17202, -17185, -16983, -16970, -16942, -16915, -16733,
            -16708, -16706, -16689, -16664, -16657, -16647, -16474, -16470,
            -16465, -16459, -16452, -16448, -16433, -16429, -16427, -16423,
            -16419, -16412, -16407, -16403, -16401, -16393, -16220, -16216,
            -16212, -16205, -16202, -16187, -16180, -16171, -16169, -16158,
            -16155, -15959, -15958, -15944, -15933, -15920, -15915, -15903,
            -15889, -15878, -15707, -15701, -15681, -15667, -15661, -15659,
            -15652, -15640, -15631, -15625, -15454, -15448, -15436, -15435,
            -15419, -15416, -15408, -15394, -15385, -15377, -15375, -15369,
            -15363, -15362, -15183, -15180, -15165, -15158, -15153, -15150,
            -15149, -15144, -15143, -15141, -15140, -15139, -15128, -15121,
            -15119, -15117, -15110, -15109, -14941, -14937, -14933, -14930,
            -14929, -14928, -14926, -14922, -14921, -14914, -14908, -14902,
            -14894, -14889, -14882, -14873, -14871, -14857, -14678, -14674,
            -14670, -14668, -14663, -14654, -14645, -14630, -14594, -14429,
            -14407, -14399, -14384, -14379, -14368, -14355, -14353, -14345,
            -14170, -14159, -14151, -14149, -14145, -14140, -14137, -14135,
            -14125, -14123, -14122, -14112, -14109, -14099, -14097, -14094,
            -14092, -14090, -14087, -14083, -13917, -13914, -13910, -13907,
            -13906, -13905, -13896, -13894, -13878, -13870, -13859, -13847,
            -13831, -13658, -13611, -13601, -13406, -13404, -13400, -13398,
            -13395, -13391, -13387, -13383, -13367, -13359, -13356, -13343,
            -13340, -13329, -13326, -13318, -13147, -13138, -13120, -13107,
            -13096, -13095, -13091, -13076, -13068, -13063, -13060, -12888,
            -12875, -12871, -12860, -12858, -12852, -12849, -12838, -12831,
            -12829, -12812, -12802, -12607, -12597, -12594, -12585, -12556,
            -12359, -12346, -12320, -12300, -12120, -12099, -12089, -12074,
            -12067, -12058, -12039, -11867, -11861, -11847, -11831, -11798,
            -11781, -11604, -11589, -11536, -11358, -11340, -11339, -11324,
            -11303, -11097, -11077, -11067, -11055, -11052, -11045, -11041,
            -11038, -11024, -11020, -11019, -11018, -11014, -10838, -10832,
            -10815, -10800, -10790, -10780, -10764, -10587, -10544, -10533,
            -10519, -10331, -10329, -10328, -10322, -10315, -10309, -10307,
            -10296, -10281, -10274, -10270, -10262, -10260, -10256, -10254};
    private static String[] pystr = new String[]{"a", "ai", "an", "ang",
            "ao", "ba", "bai", "ban", "bang", "bao", "bei", "ben", "beng",
            "bi", "bian", "biao", "bie", "bin", "bing", "bo", "bu", "ca",
            "cai", "can", "cang", "cao", "ce", "ceng", "cha", "chai", "chan",
            "chang", "chao", "che", "chen", "cheng", "chi", "chong", "chou",
            "chu", "chuai", "chuan", "chuang", "chui", "chun", "chuo", "ci",
            "cong", "cou", "cu", "cuan", "cui", "cun", "cuo", "da", "dai",
            "dan", "dang", "dao", "de", "deng", "di", "dian", "diao", "die",
            "ding", "diu", "dong", "dou", "du", "duan", "dui", "dun", "duo",
            "e", "en", "er", "fa", "fan", "fang", "fei", "fen", "feng", "fo",
            "fou", "fu", "ga", "gai", "gan", "gang", "gao", "ge", "gei", "gen",
            "geng", "gong", "gou", "gu", "gua", "guai", "guan", "guang", "gui",
            "gun", "guo", "ha", "hai", "han", "hang", "hao", "he", "hei",
            "hen", "heng", "hong", "hou", "hu", "hua", "huai", "huan", "huang",
            "hui", "hun", "huo", "ji", "jia", "jian", "jiang", "jiao", "jie",
            "jin", "jing", "jiong", "jiu", "ju", "juan", "jue", "jun", "ka",
            "kai", "kan", "kang", "kao", "ke", "ken", "keng", "kong", "kou",
            "ku", "kua", "kuai", "kuan", "kuang", "kui", "kun", "kuo", "la",
            "lai", "lan", "lang", "lao", "le", "lei", "leng", "li", "lia",
            "lian", "liang", "liao", "lie", "lin", "ling", "liu", "long",
            "lou", "lu", "lv", "luan", "lue", "lun", "luo", "ma", "mai", "man",
            "mang", "mao", "me", "mei", "men", "meng", "mi", "mian", "miao",
            "mie", "min", "ming", "miu", "mo", "mou", "mu", "na", "nai", "nan",
            "nang", "nao", "ne", "nei", "nen", "neng", "ni", "nian", "niang",
            "niao", "nie", "nin", "ning", "niu", "nong", "nu", "nv", "nuan",
            "nue", "nuo", "o", "ou", "pa", "pai", "pan", "pang", "pao", "pei",
            "pen", "peng", "pi", "pian", "piao", "pie", "pin", "ping", "po",
            "pu", "qi", "qia", "qian", "qiang", "qiao", "qie", "qin", "qing",
            "qiong", "qiu", "qu", "quan", "que", "qun", "ran", "rang", "rao",
            "re", "ren", "reng", "ri", "rong", "rou", "ru", "ruan", "rui",
            "run", "ruo", "sa", "sai", "san", "sang", "sao", "se", "sen",
            "seng", "sha", "shai", "shan", "shang", "shao", "she", "shen",
            "sheng", "shi", "shou", "shu", "shua", "shuai", "shuan", "shuang",
            "shui", "shun", "shuo", "si", "song", "sou", "su", "suan", "sui",
            "sun", "suo", "ta", "tai", "tan", "tang", "tao", "te", "teng",
            "ti", "tian", "tiao", "tie", "ting", "tong", "tou", "tu", "tuan",
            "tui", "tun", "tuo", "wa", "wai", "wan", "wang", "wei", "wen",
            "weng", "wo", "wu", "xi", "xia", "xian", "xiang", "xiao", "xie",
            "xin", "xing", "xiong", "xiu", "xu", "xuan", "xue", "xun", "ya",
            "yan", "yang", "yao", "ye", "yi", "yin", "ying", "yo", "yong",
            "you", "yu", "yuan", "yue", "yun", "za", "zai", "zan", "zang",
            "zao", "ze", "zei", "zen", "zeng", "zha", "zhai", "zhan", "zhang",
            "zhao", "zhe", "zhen", "zheng", "zhi", "zhong", "zhou", "zhu",
            "zhua", "zhuai", "zhuan", "zhuang", "zhui", "zhun", "zhuo", "zi",
            "zong", "zou", "zu", "zuan", "zui", "zun", "zuo"};
    private StringBuilder buffer;
    private String resource;
    private static Trans2PinYin chineseSpelling = new Trans2PinYin();

    public static Trans2PinYin getInstance() {
        return chineseSpelling;
    }

    public String getResource() {
        return resource;
    }

    public void setResource(String resource) {
        this.resource = resource;
    }

    private int getChsAscii(String chs) {
        int asc = 0;
        try {
            byte[] bytes = chs.getBytes("gb2312");
            if (bytes == null || bytes.length > 2 || bytes.length <= 0) { // 错误
                // log
                throw new RuntimeException("illegal resource string");
                // System.out.println("error");
            }
            if (bytes.length == 1) { // 英文字符
                asc = bytes[0];
            }
            if (bytes.length == 2) { // 中文字符
                int hightByte = 256 + bytes[0];
                int lowByte = 256 + bytes[1];
                asc = (256 * hightByte + lowByte) - 256 * 256;
            }
        } catch (Exception e) {
            System.out
                    .println("ERROR:ChineseSpelling.class-getChsAscii(String chs)"
                            + e);
            // e.printStackTrace();
        }
        return asc;
    }

    /**
     * 转换单个汉字
     *
     * @param str
     * @return
     */
    public String convert(String str) {
        String result = null;
        int ascii = getChsAscii(str);
        if (ascii > 0 && ascii < 160) {
            result = String.valueOf((char) ascii);
        } else {
            for (int i = (pyvalue.length - 1); i >= 0; i--) {
                if (pyvalue[i] <= ascii) {
                    result = pystr[i];
                    break;
                }
            }
        }
        return result;
    }

    /**
     * 转换一个或多个汉字
     *
     * @param str
     * @return
     */
    public String convertAll(String str) {
        String result = "";
        String strTemp = null;
        for (int j = 0; j < str.length(); j++) {
            strTemp = str.substring(j, j + 1);
            int ascii = getChsAscii(strTemp);
            if (ascii > 0 && ascii < 160) {
                result += String.valueOf((char) ascii);
            } else {
                for (int i = (pyvalue.length - 1); i >= 0; i--) {
                    if (pyvalue[i] <= ascii) {
                        result += pystr[i];
                        break;
                    }
                }
            }
        }
        return result;
    }

    public String getSelling(String chs) {
        String key, value;
        buffer = new StringBuilder();
        for (int i = 0; i < chs.length(); i++) {
            key = chs.substring(i, i + 1);
            if (key.getBytes().length == 2) {
                value = (String) convert(key);
                if (value == null) {
                    value = "unknown";
                }
            } else {
                value = key;
            }
            buffer.append(value);
        }
        return buffer.toString();
    }

    public String getSpelling() {
        return this.getSelling(this.getResource());
    }

    /**
     * 转换为拼音
     *
     * @param str
     * @return
     */
    public static String trans2PinYin(String str) {
        return Trans2PinYin.getInstance().convertAll(str);
    }

}
```



这些小细节大家等下自行下载我上传的源码看吧！最后，这篇博客到这里也就结束了，字母导航的思路也借鉴了很多前人，自己摸索了一天多才摸索出来，感觉这个扩展性还可以强一点，无奈精力有限，篇幅也有限了，等我下次再继续研究，这么多，能看到这里也是不容易，我写的也不容易，给我点个赞吧！！！

> 
我们最后看一下最终的效果


![这里写图片描述](https://img-blog.csdn.net/20160917161607782)

> 
CSDN太坑了，博客再一次没有了，又要重新写


### LettersNavigation:[http://download.csdn.net/detail/qq_26787115/9627880](http://download.csdn.net/detail/qq_26787115/9627880)



