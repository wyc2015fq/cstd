# 混合开发经验之谈-1TH - DEVELOPER - CSDN博客





2017年12月27日 18:02:17[学术袁](https://me.csdn.net/u012827205)阅读数：397
所属专栏：[Android-开发](https://blog.csdn.net/column/details/20398.html)









![这里写图片描述](https://img-blog.csdn.net/20171227114642678?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

### 偶遇

进来公司需要开发新的项目，要求不要纯原生开发，依照混合方式 Hybrid，使用Native+H5的方式。然后就使用原生代码来完成类似微信的主页布局和逻辑框架，只是在四个页卡中嵌入了H5的内容。而作为原生的内容只是作为一个“壳”、“基础框架”，来承接H5的上层建筑。特别声明，不要给我讲什么使用WebView这种混合方式，流畅性差，渲染不怎么地！请起开，我今天就是在聊这块儿。

### 设计项目结构、封装

然后，就开始了项目的开发。使用比较普遍使用的 MVP 设计方式(Rxjava+Retrofit+Okhttp)，然后内部要进行嵌用H5的并不是安卓原生的 WetbView，而是使用了github 上比较 star 的依赖库 —— [AgentWeb](https://github.com/Justson/AgentWeb)   。这些确定之后，就要进行封装了！

### 结构介绍

在项目上，起初，主页面逻辑设计结构类似新浪微博——4个tab点击进行切换，而tab控件是由自己进行组织，非常的纯粹。并不是使用网上的一些封装控件。4个tab分别各对应一个Fragment，Fragment的Layout中嵌入AgentWeb。页面切换的方式是通过方法 [show()和hide()](http://blog.csdn.net/harvic880925/article/details/44927375)；看下代码就会明白，大致的代码是这样：

![这里写图片描述](https://img-blog.csdn.net/20171228100639813?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

![这里写图片描述](https://img-blog.csdn.net/20171228100656545?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

![这里写图片描述](https://img-blog.csdn.net/20171228100707340?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

good，基础工作完成之后， 就是要写入 HTML 的地址然后运行项目检阅效果了。但是意外的事情发生了，4个tab页面（都嵌入了AgentWeb）只有第一个显示出来，其他的3个页面都是空白，白的很！

### 第一次bug 处理

**首先声明一点，html的地址只在内部局域网中有效，第一个也卡首页tab，显示的是一个地图展示。地图是公司自己开发的，这个阶段公司开发的地图是刚起步，应用起来特别的耗手机资源，有时直接 render error 。**

当时一脸懵逼，因为这个AgentWeb不是第一次使用了，代码都是惯用的。然后仔细检查了一遍，并进行各种尝试。起初想，难道是地图太耗资源了？？然后就使用第4个页面的地址url替换了地图页面的，这时第一个页面还能显示，而其他页面还是空白。这就说明了，html的url是没有问题的，也不是地图的问题。那就是代码的问题！然后经过各种尝试，查阅无数的博客，官网。在一次无意的尝试中产生了一个效果，就是第1个页面和第3个能显示了，第3个只有背景图无数据文字显示，其他仍然是空白！

**然后萌生出了一个设想：**此时的这种结构设计，类似通过把Fragment添加到一个栈中，依照add的顺序层叠式的叠放在一起，然后通过show()和hide()进行显示和隐藏。而实现相同的这种结构形式，还有一种实现，那就是**ViewPager + Fragment** 。这种就是类似于把Fragment 依照add的顺序平铺在一张画布上，当我们进行切换的时候，指定显示对应的页面内容。

然后围绕这个设想开始进行实施，最后的结果正明我当时的设想是正确的，使用ViewPager+Fragment就能实现4个页面都能正确显示。 

简单实现代码展示：

```java
/**
 * Created by YJH on 2017/12/19.
 * 功能：项目的首页Tabs-UI页面
 */

public class HomeTabsAty extends ActivityPresenter<HomeTabsProxy> implements TabLayout.OnTabSelectedListener {

    private TabLayoutFragmentAdapter mAdapter;
    private List<String> mTabList = new ArrayList<>();
    private List<Fragment> mFragments = new ArrayList<>();
    private String[] lonlat = new String[2];
    private int[] mTabImgs = new int[]{R.mipmap.ic_home_home_normal,
            R.mipmap.ic_home_travel_normal, R.mipmap.ic_home_ticket_normal,
            R.mipmap.ic_home_mine_normal};

    public static int mCurrentPager = 0;//当前默认的页面tab


    @Override
    protected Class<HomeTabsProxy> getPresenterClass() {
        return HomeTabsProxy.class;
    }

    @Override
    protected void initData() {

    }

    @Override
    protected void onResume() {
        super.onResume();
        //这里接收 从二级原生页面跳转到首页的pos-tab
        if (mCurrentPager != 0)
            atyPresenter.getViewPager().setCurrentItem(mCurrentPager);

    }

    @Override
    protected void initView() {

        initTabList();
        mAdapter = new TabLayoutFragmentAdapter(getSupportFragmentManager(), mTabList, this, mFragments, mTabImgs);
        atyPresenter.getViewPager().setAdapter(mAdapter);
        atyPresenter.getViewPager().setCurrentItem(mCurrentPager);
        atyPresenter.getViewPager().setOffscreenPageLimit(3);
        atyPresenter.getTabLayout().setupWithViewPager(atyPresenter.getViewPager());
        atyPresenter.getTabLayout().setTabMode(TabLayout.MODE_FIXED);
        for (int i = 0; i < atyPresenter.getTabLayout().getTabCount(); i++) {
            atyPresenter.getTabLayout().getTabAt(i).setCustomView(mAdapter.getTabView(i));
        }
        atyPresenter.getTabLayout().addOnTabSelectedListener(this);

    }

    @Override
    protected void receiveRxBuxCommanders(BaseRxBusEvent baseRxBusEvent) {
        //比如，这个时候，从某个页面发来指令，让改变一下某个TextView显示的内容变化
        // BaseRxBusEvent rxCommander = new BaseRxBusEvent();
        //rxCommander.data = "aty-SelfInfoActivity";
        //rxCommander.type = 1;
        //RxBusCommander.getInstance().postCommander(rxCommander);

        //这个是接受方法
        if (baseRxBusEvent.from.equals("JPush_sos") && 1 == baseRxBusEvent.type) {
            SosNoticeBean noticeBean = Constant.gsonToBean(SosNoticeBean.class, baseRxBusEvent.json);
            tvSOSPhone.setText(noticeBean.getMobile());
            tvSOSContent.setText(noticeBean.getMsg());
            lonlat[0] = noticeBean.getLon() + "";
            lonlat[1] = noticeBean.getLat() + "";
            mMaterialDialog.show();
        }
    }

    MaterialDialog mMaterialDialog;
    TextView tvSOSPhone, tvSOSContent, tvBtSure;

    private void initTabList() {
        View dialogView = LayoutInflater.from(this).inflate(R.layout.dialog_home_sos, null);
        mMaterialDialog = new MaterialDialog(this).setContentView(dialogView);
        mMaterialDialog.setCanceledOnTouchOutside(true);
        tvSOSPhone = (TextView) dialogView.findViewById(R.id.tv_sos_telephone);
        tvSOSContent = (TextView) dialogView.findViewById(R.id.tv_sos_content);
        tvBtSure = (TextView) dialogView.findViewById(R.id.tv_sure);
        tvBtSure.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                //Android 端调用js
                TabHomeFrag tabHomeFrag = (TabHomeFrag) mFragments.get(0);
                tabHomeFrag.noticeJs(lonlat);
                mMaterialDialog.dismiss();
            }
        });


        mTabList.clear();
        mTabList.add(getString(R.string.item_home));
        mTabList.add(getString(R.string.item_travel));
        mTabList.add(getString(R.string.item_ticket));
        mTabList.add(getString(R.string.item_mine));


        mFragments.clear();
        mFragments.add(new TabHomeFrag());
        mFragments.add(new TabTravelFrag());
        mFragments.add(new TabTicketFrag());
        mFragments.add(new TabMineFrag());
    }


    private long time;

    @Override
    public boolean onKeyDown(int keyCode, KeyEvent event) {
        // 连续点击回退键，退出程序
        if (keyCode == KeyEvent.KEYCODE_BACK) {
            if (currentTimeMillis() - time > 2000) {
                ToastPop.showLong("再按一次退出");
                time = currentTimeMillis();
            } else {
                finish();
            }
            return true;
        }
        return super.onKeyDown(keyCode, event);
    }

    @Override
    public void onTabSelected(TabLayout.Tab tab) {
        View customView = tab.getCustomView();
        TextView tabText = (TextView) customView.findViewById(R.id.tv_tab_text);
        ImageView tabIcon = (ImageView) customView.findViewById(R.id.iv_tab_icon);
        tabText.setTextColor(ContextCompat.getColor(this, R.color.color3baae1));
        String s = tabText.getText().toString();
        if (getString(R.string.item_home).equals(s)) {
            tabIcon.setImageResource(R.mipmap.ic_home_home_pressed);
        } else if (getString(R.string.item_travel).equals(s)) {
            tabIcon.setImageResource(R.mipmap.ic_home_travel_pressed);
        } else if (getString(R.string.item_ticket).equals(s)) {
            tabIcon.setImageResource(R.mipmap.ic_home_ticket_pressed);
        } else if (getString(R.string.item_mine).equals(s)) {
            tabIcon.setImageResource(R.mipmap.ic_home_mine_pressed);
        }
    }

    @Override
    public void onTabUnselected(TabLayout.Tab tab) {
        View customView = tab.getCustomView();
        TextView tabText = (TextView) customView.findViewById(R.id.tv_tab_text);
        ImageView tabIcon = (ImageView) customView.findViewById(R.id.iv_tab_icon);
        tabText.setTextColor(ContextCompat.getColor(this, R.color.color292929));
        String s = tabText.getText().toString();
        if (getString(R.string.item_home).equals(s)) {
            tabIcon.setImageResource(R.mipmap.ic_home_home_normal);
        } else if (getString(R.string.item_travel).equals(s)) {
            tabIcon.setImageResource(R.mipmap.ic_home_travel_normal);
        } else if (getString(R.string.item_ticket).equals(s)) {
            tabIcon.setImageResource(R.mipmap.ic_home_ticket_normal);
        } else if (getString(R.string.item_mine).equals(s)) {
            tabIcon.setImageResource(R.mipmap.ic_home_mine_normal);
        }
    }

    @Override
    public void onTabReselected(TabLayout.Tab tab) {

    }
}
```

[后面继续。。。](http://blog.csdn.net/junhuahouse/article/details/78921153)



