
# android 学习-Android Fragment 深度解析 - 萌萌的It人 www.itmmd.com - CSDN博客


2014年11月09日 20:34:17[Jlins](https://me.csdn.net/dyllove98)阅读数：1233


有过一些面试经验的人基本都深有体会，每次面试一般都会问到Fragment的知识，所以，今天我就单独把Fragment拿出来与大家分享一下.
会涉及到Fragment如何产生，什么是Fragment，Fragment生命周期，如何静态和动态使用Fragment，Fragment回退栈，Fragment事务，以及Fragment的一些特殊用途，例如：没有布局的Fragment有何用处？Fragment如何与Activiy交互？Fragment如何创建对话框？Fragment如何与ActionBar集成等等...
1、Fragment的产生与介绍
Android运行在各种各样的设备中，有小屏幕的手机，超大屏的平板甚至电视。针对屏幕尺寸的差距，很多情况下，都是先针对手机开发一套app，然后拷贝一份，修改布局以适应什么超级大屏的。难道无法做到一个app可以同时适应手机和平板吗？答案是，当然有，那就是Fragment.Fragment出现的初衷就是为了解决这样的问题。
你可以把Fragment当成Activity一个界面的一部分，甚至Activity的界面由完全不同的Fragment组成，更帅气的是Fragment有自己的声明周期和接收、处理用户的事件，这样就不必要在一个Activity里面写一堆事件、控件的代码了。更为重要的是，你可以动态的添加、替换、移除某个Fragment。
2、Fragment的生命周期
Fragment必须是依存于Activity而存在的，因此Activity的生命周期会直接影响到Fragment的生命周期。官网这张图很好的说明了俩者的关系：
![](http://images.cnitblog.com/blog/322652/201411/082335144241222.jpg)
可以看到Fragment比Activity多了几个额外的生命周期回调函数：
onAttach(Activity);//当Activity与Fragment发生关联时调用
onCreateView(LayoutInflater,ViewGroup,Bundle);//创建该Fragment的视图
onActivityCreate(bundle);//当Activity的onCreate()；方法返回时调用
onDestoryView();//与onCreateView相对应，当改Fragment被移除时调用
onDetach();//与onAttach()相对应，当Fragment与Activity的关联被取消时调用
注意：除了onCreateView，其他的所有方法如果你重写了，必须调用父类对于该方法的实现。
3、静态的使用Fragment
接下来，就是实践的时候了，要注意了，开始写代码喽~~~~
这是使用Fragment最简单的一种方式，把Fragment当成普通的控件，直接写在Activity的布局文件中，用布局文件调用Fragment。
步骤：
1、继承Fragment，重写onCreateView决定Fragment布局。
2、在Activity中声明此Fragment,就当和普通的View一样。
下面展示一个例子（我使用俩个Fragment作为Activity的布局，一个Fragment用于标题布局，一个Fragment用于内容布局）。
TitleFragment的布局文件，在这里我们可以看出，我们可以每个Fragment当中进行单独的布局：
![复制代码](http://common.cnblogs.com/images/copycode.gif)
<?xml version="1.0" encoding="utf-8"?><RelativeLayoutxmlns:android="http://schemas.android.com/apk/res/android"android:layout_width="fill_parent"android:layout_height="45dp"android:background="@drawable/title_bar"><ImageButtonandroid:id="@+id/id_title_left_btn"android:layout_width="wrap_content"android:layout_height="fill_parent"android:layout_centerVertical="true"android:background="@drawable/showleft_selector"/><TextViewandroid:layout_width="fill_parent"android:layout_height="fill_parent"android:gravity="center"android:text="我不是微信"android:textColor="\#fff"android:textSize="20sp"android:textStyle="bold"/></RelativeLayout>
![复制代码](http://common.cnblogs.com/images/copycode.gif)
TitleFragment.java文件，在这里我们能够看到，可以在各个Fragment当中进行独立的初始化空间并且处理按钮之类的事件，减轻了Activity的负担，我们在Activity中就没有必要写一大推初始化控件和事件响应的代码了，这样就使我们的代码看上去更加的简洁了，可读性大大提高了。
![复制代码](http://common.cnblogs.com/images/copycode.gif)
publicclassTitleFragmentextendsFragment {privateImageButton mButton;
    @SuppressLint("NewApi")
    @OverridepublicView onCreateView(LayoutInflater inflater, ViewGroup container,
            Bundle savedInstanceState) {
        View view= inflater.inflate(R.layout.title_fragment, container,false);
        mButton=(ImageButton)view.findViewById(R.id.id_title_left_btn);
        mButton.setOnClickListener(newOnClickListener() {
            
            @OverridepublicvoidonClick(View v) {
                Toast.makeText(getActivity(),"i am an ImageButton in TitleFragment ! ",  
                        Toast.LENGTH_SHORT).show();  
            }
        });returnview;
    }
}![复制代码](http://common.cnblogs.com/images/copycode.gif)
同理还有ContentFragment的布局文件content_fragment.xml
![复制代码](http://common.cnblogs.com/images/copycode.gif)
<?xml version="1.0" encoding="utf-8"?><LinearLayoutxmlns:android="http://schemas.android.com/apk/res/android"android:layout_width="match_parent"android:layout_height="match_parent"android:orientation="vertical"><TextViewandroid:layout_width="fill_parent"android:layout_height="fill_parent"android:gravity="center"android:text="使用Fragment做主面板"android:textSize="20sp"android:textStyle="bold"/></LinearLayout>![复制代码](http://common.cnblogs.com/images/copycode.gif)
同理还有ContentFragment.java文件
![复制代码](http://common.cnblogs.com/images/copycode.gif)
publicclassContentFragmentextendsFragment {
    @OverridepublicView onCreateView(LayoutInflater inflater, ViewGroup container,
            Bundle savedInstanceState) {returninflater.inflate(R.layout.content_fragment, container,false);
    }
}![复制代码](http://common.cnblogs.com/images/copycode.gif)
下面就是主Activity以及他的布局文件
MainActivity.java文件![复制代码](http://common.cnblogs.com/images/copycode.gif)
publicclassMainActivityextendsActivity {
    @OverrideprotectedvoidonCreate(Bundle savedInstanceState) {super.onCreate(savedInstanceState);  
        requestWindowFeature(Window.FEATURE_NO_TITLE);  
        setContentView(R.layout.activity_main);  
    }
}![复制代码](http://common.cnblogs.com/images/copycode.gif)
activity_main.xml文件，在这里我们可以看到，我们把Fragment就当做普通的控件一样，在xml文件中使用。
![复制代码](http://common.cnblogs.com/images/copycode.gif)
<RelativeLayoutxmlns:android="http://schemas.android.com/apk/res/android"xmlns:tools="http://schemas.android.com/tools"android:layout_width="match_parent"android:layout_height="match_parent"tools:context="com.example.staticfragment.MainActivity"><fragmentandroid:name="com.example.staticfragment.TitleFragment"android:id="@+id/title"android:layout_height="45dp"android:layout_width="match_parent"/><fragmentandroid:layout_below="@id/title"android:name="com.example.staticfragment.ContentFragment"android:id="@+id/content"android:layout_height="fill_parent"android:layout_width="fill_parent"/></RelativeLayout>![复制代码](http://common.cnblogs.com/images/copycode.gif)
运行效果截图如下：
![](http://images.cnitblog.com/blog/322652/201411/090937287993134.jpg)
注：由于时间原因，就没有上传源码，如果需要demo源码的话，可以留言，我单独发你一下...
4、动态的使用Fragment
上面已经演示了最简单的使用Fragment的方式，下面分享一下如何动态的添加、更新、以及删除Fragment。
首先是，MainActivity的布局文件activity_main.xml，该文件布局文件上面的顶部是一个TitleFragment，是一个静态声明的Fragment。
中间也是一个Fragment，但是这个Fragment是动态使用的。
最下面是四个按钮。用include标签包含外部的布局文件进来的。
![复制代码](http://common.cnblogs.com/images/copycode.gif)
<RelativeLayoutxmlns:android="http://schemas.android.com/apk/res/android"xmlns:tools="http://schemas.android.com/tools"android:layout_width="match_parent"android:layout_height="match_parent"><fragmentandroid:id="@+id/id_fragment_title"android:name="com.example.dynamicfragment.TitleFragment"android:layout_width="fill_parent"android:layout_height="45dp"/><includeandroid:id="@+id/id_ly_bottombar"android:layout_width="fill_parent"android:layout_height="55dp"android:layout_alignParentBottom="true"layout="@layout/bottombar"/><FrameLayoutandroid:id="@+id/id_content"android:layout_width="fill_parent"android:layout_height="fill_parent"android:layout_above="@id/id_ly_bottombar"android:layout_below="@id/id_fragment_title"/></RelativeLayout>![复制代码](http://common.cnblogs.com/images/copycode.gif)
然后是，MainActivity.java文件。也是我们这个demo当中最重要的代码文件，首先是将上面的布局文件通过setContentView()加载进来.然后是通过setDefaultFragment()；将默认的ContentFragment动态的加载进来。接下来就是通过我们在最下面防止的四个按钮可以随意的动态切换Fragment。这也是为什么Fragment会有如此火的原因吧~~~^^
![复制代码](http://common.cnblogs.com/images/copycode.gif)
publicclassMainActivityextendsActionBarActivityimplementsOnClickListener {privateImageButton mTabWeixin;privateImageButton mTabFriend;privateImageButton mTabDiscover;privateImageButton mTabMe;privateContentFragment mWeiXinFragment;privateFriendFragment mFriendFragment;
    @OverrideprotectedvoidonCreate(Bundle savedInstanceState) {super.onCreate(savedInstanceState);
        requestWindowFeature(Window.FEATURE_NO_TITLE);
        setContentView(R.layout.activity_main);
        initView();
    }publicvoidinitView() {//初始化控件和声明事件mTabWeixin =(ImageButton) findViewById(R.id.weixin);
        mTabFriend=(ImageButton) findViewById(R.id.friend);
        mTabWeixin.setOnClickListener(this);
        mTabFriend.setOnClickListener(this);//设置默认的FragmentsetDefaultFragment();
    }
    @SuppressLint("NewApi")privatevoidsetDefaultFragment() {
        FragmentManager manager=getFragmentManager();
        FragmentTransaction transaction=manager.beginTransaction();
        mWeiXinFragment=newContentFragment();
        transaction.replace(R.id.id_content, mWeiXinFragment);
        transaction.commit();
    }
    @SuppressLint("NewApi")
    @OverridepublicvoidonClick(View v) {
        FragmentManager fm=getFragmentManager();//开启Fragment事务FragmentTransaction transaction =fm.beginTransaction();switch(v.getId()) {caseR.id.weixin:if(mWeiXinFragment ==null) {
                mWeiXinFragment=newContentFragment();
            }//使用当前Fragment的布局替代id_content的控件transaction.replace(R.id.id_content, mWeiXinFragment);break;caseR.id.friend:if(mFriendFragment ==null) {
                mFriendFragment=newFriendFragment();
            }
            transaction.replace(R.id.id_content, mFriendFragment);break;
        }//transaction.addToBackStack();//事务提交transaction.commit();
    }
}![复制代码](http://common.cnblogs.com/images/copycode.gif)
从上面的代码，我们可以看出，我们可以使用FragmentManager对Fragment进行动态的加载，这里使用的replace方法~~~下一节我们会详细的介绍FragmentManager的常用API。。。。^^
注：如果使用android3.0一下的版本，需要引入v4的包，然后Activity继承FragmentActivity，然后通过getSupportFragmentManager()获得FragmentManager对象,不过还是建议把Menifest文件的uses-sdk的minSdkVersion和targetSdkVersion都改为11以上，这样就不必引入v4的包了。
代码的中间有俩个动态加载进来的Fragment，这个和静态使用ragment的声明方式是一样的，写一个继承Fragment的类，然后设置相应的布局，由于时间的关系，我这里只写了俩个Fragment，现在把这俩个的代码页贴出来：
第一个Fragment和他相应的布局文件：
![复制代码](http://common.cnblogs.com/images/copycode.gif)
publicclassContentFragmentextendsFragment {
    @OverridepublicView onCreateView(LayoutInflater inflater, ViewGroup container,  
            Bundle savedInstanceState)  
    {returninflater.inflate(R.layout.fragment_content, container,false);  
    }  
}![复制代码](http://common.cnblogs.com/images/copycode.gif)
![复制代码](http://common.cnblogs.com/images/copycode.gif)
<?xml version="1.0" encoding="utf-8"?>  
<LinearLayout xmlns:android="http://schemas.android.com/apk/res/android"android:layout_width="match_parent"android:layout_height="match_parent"android:orientation="vertical" >  
  
    <TextView  
        android:layout_width="fill_parent"android:layout_height="fill_parent"android:gravity="center"android:text="weixin"android:textSize="20sp"android:textStyle="bold" />  
  
</LinearLayout>![复制代码](http://common.cnblogs.com/images/copycode.gif)
第二个Fragment和他相应的布局文件：
![复制代码](http://common.cnblogs.com/images/copycode.gif)
publicclassFriendFragmentextendsFragment {
    @OverridepublicView onCreateView(LayoutInflater inflater, ViewGroup container,  
            Bundle savedInstanceState)  
    {returninflater.inflate(R.layout.fragment_friend, container,false);  
    }  
}![复制代码](http://common.cnblogs.com/images/copycode.gif)
![复制代码](http://common.cnblogs.com/images/copycode.gif)
<?xml version="1.0" encoding="utf-8"?>  
<LinearLayout xmlns:android="http://schemas.android.com/apk/res/android"android:layout_width="match_parent"android:layout_height="match_parent"android:orientation="vertical" >  
  
    <TextView  
        android:layout_width="fill_parent"android:layout_height="fill_parent"android:gravity="center"android:text="friend"android:textSize="20sp"android:textStyle="bold" />  
  
</LinearLayout>![复制代码](http://common.cnblogs.com/images/copycode.gif)
好了，现在基本的代码都有了，我们把demo的运行图贴出来给大家分享一下（注：时间原因，没注意布局以及图片的美化，只是功能的实现），这是分别点击下面第一个和第二个按钮的效果图，从而实现了中间用一个Fragment动态的加载这俩个Fragment的显示。
![](http://images.cnitblog.com/blog/322652/201411/091422095038815.jpg)![](http://images.cnitblog.com/blog/322652/201411/091423006432993.jpg)
ps:为了代码的简洁，就不添加按钮的点击变化什么的了，主要讲解功能了~~~
5、Fragment常用的API：
Fragment常用的三个类：
android.app.Fragment 主要用于定义Fragment
android.app.FragmentManager 主要用于在Activity中操作Fragment
android.app.FragmentTransaction 保证一些列Fragment操作的原子性，熟悉事务这个词，一定能明白~
a、获取FragmentManage的方式：
getFragmentManager() // v4中，getSupportFragmentManager
b、主要的操作都是FragmentTransaction的方法
FragmentTransaction transaction = fm.benginTransatcion();//开启一个事务
transaction.add()
往Activity中添加一个Fragment
transaction.remove()
从Activity中移除一个Fragment，如果被移除的Fragment没有添加到回退栈（回退栈后面会详细说），这个Fragment实例将会被销毁。
transaction.replace()
使用另一个Fragment替换当前的，实际上就是remove()然后add()的合体~
transaction.hide()
隐藏当前的Fragment，仅仅是设为不可见，并不会销毁
transaction.show()
显示之前隐藏的Fragment
detach()
将此Fragment从Activity中分离，会销毁其布局，但不会销毁该实例
attach()
将从Activity中分离的Fragment，重新关联到该Activity，重新创建其视图层次
transatcion.commit()//提交一个事务
注意：常用Fragment的哥们，可能会经常遇到这样Activity状态不一致：State loss这样的错误。主要是因为：commit方法一定要在Activity.onSaveInstance()之前调用。
上述，基本是操作Fragment的所有的方式了，在一个事务开启到提交可以进行多个的添加、移除、替换等操作。
值得注意的是：如果你喜欢使用Fragment，一定要清楚这些方法，哪个会销毁视图，哪个会销毁实例，哪个仅仅只是隐藏，这样才能更好的使用它们。
a、比如：我在FragmentA中的EditText填了一些数据，当切换到FragmentB时，如果希望会到A还能看到数据，则适合你的就是hide和show；也就是说，希望保留用户操作的面板，你可以使用hide和show，当然了不要使劲在那new实例，进行下非null判断。
b、再比如：我不希望保留用户操作，你可以使用remove()，然后add()；或者使用replace()这个和remove,add是相同的效果。
c、remove和detach有一点细微的区别，在不考虑回退栈的情况下，remove会销毁整个Fragment实例，而detach则只是销毁其视图结构，实例并不会被销毁。那么二者怎么取舍使用呢？如果你的当前Activity一直存在，那么在不希望保留用户操作的时候，你可以优先使用detach。
上述已经介绍完成了Fragment常用的一些方法，相信看完，大家一定清楚了Fragment的产生理由，以及如何使用Fragment，再根据API的讲解，也能明白，曾经为何觉得Fragment会出现一些列乱七八槽的问题，终究是因为没有弄清楚其生命周期。
由于篇幅原因，剩下的内容留到下一篇了。在下一篇，会介绍：
1、如何管理Fragment回退栈
2、Fragment如何与Activity交互
3、Fragment与Activity交互的最佳实践
4、没有视图的Fragment的用处
5、使用Fragment创建对话框
6、如何与ActionBar，MenuItem集成等~~
原创文章by[萌萌的it人](http://www.itmmd.com)www.itmmd.com

