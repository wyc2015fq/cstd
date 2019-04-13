
# Android使用ViewPager实现左右循环滑动及轮播效果 - 萌萌的It人 www.itmmd.com - CSDN博客


2014年11月11日 17:03:47[Jlins](https://me.csdn.net/dyllove98)阅读数：89660


ViewPager是一个常用的android组件，不过通常我们使用ViewPager的时候不能实现左右无限循环滑动，在滑到边界的时候会看到一个不能翻页的动画，可能影响用户体验。此外，某些区域性的ViewPager（例如展示广告或者公告之类的ViewPager），可能需要自动轮播的效果，即用户在不用滑动的情况下就能够看到其他页面的信息。
为此我查阅了网络上现有的一些关于实现这样效果的例子，但都不是很满意，经过反复实验，在这里总结并分享给大家，希望能有所帮助。

循环滑动效果的实现：PagerAdapter
我们知道ViewPager自带的滑动效果非常出色，因此我们基本不需要处理这个滑动，只处理内容的显示。而内容的显示是由Adapter控制的，因此这里重点就是这个Adapter了。为简单起见，本例的每个View直接是一张图片。下面是Adapter的代码：

```python
private class ImageAdapter extends PagerAdapter{
         
        private ArrayList<ImageView> viewlist;
 
        public ImageAdapter(ArrayList<ImageView> viewlist) {
            this.viewlist = viewlist;
        }
 
        @Override
        public int getCount() {
            //设置成最大，使用户看不到边界
            return Integer.MAX_VALUE;
        }
 
        @Override
        public boolean isViewFromObject(View arg0, Object arg1) {
            return arg0==arg1;
        }
         @Override  
         public void destroyItem(ViewGroup container, int position,  
                 Object object) {  
             //Warning：不要在这里调用removeView
         }  
         @Override  
         public Object instantiateItem(ViewGroup container, int position) {
             //对ViewPager页号求模取出View列表中要显示的项
             position %= viewlist.size();
             if (position<0){
                 position = viewlist.size()+position;
             }
             ImageView view = viewlist.get(position);
             //如果View已经在之前添加到了一个父组件，则必须先remove，否则会抛出IllegalStateException。
             ViewParent vp =view.getParent();
             if (vp!=null){
                 ViewGroup parent = (ViewGroup)vp;
                 parent.removeView(view);
             }
             container.addView(view);  
             //add listeners here if necessary
             return view;  
         }  
    }
```

这里有几个地方需要注意：
getCount()方法的返回值：这个值直接关系到ViewPager的“边界”，因此当我们把它设置为Integer.MAX_VALUE之后，用户基本就看不到这个边界了（估计滑到这里的时候电池已经挂了吧o_O）。当然，通常情况下设置为100倍实际内容个数也是可以的，之前看的某个实现就是这么干的。
instantiateItem()方法position的处理：由于我们设置了count为 Integer.MAX_VALUE，因此这个position的取值范围很大很大，但我们实际要显示的内容肯定没这么多（往往只有几项），所以这里肯定会有求模操作。但是，简单的求模会出现问题：考虑用户向左滑的情形，则position可能会出现负值。所以我们需要对负值再处理一次，使其落在正确的区间内。
instantiateItem()方法父组件的处理：通常我们会直接addView，但这里如果直接这样写，则会抛出IllegalStateException。假设一共有三个view，则当用户滑到第四个的时候就会触发这个异常，原因是我们试图把一个有父组件的View添加到另一个组件。但是，如果直接写成下面这样：

```python
(ViewGroup)view.getParent().removeView(view);
```

则又会因为一开始的时候组件并没有父组件而抛出NullPointerException。因此，需要进行一次判断。也就是上面的代码。
destroyItem()方法：由于我们在instantiateItem()方法中已经处理了remove的逻辑，因此这里并不需要处理。实际上，实验表明这里如果加上了remove的调用，则会出现ViewPager的内容为空的情况。

轮播效果的实现：使用Handler进行更新
这里我定义了一个Handler来处理ViewPager的轮播。所谓的“轮播”效果实现起来是这样的：每隔一定时间（这里是3秒）切换一次显示的页面。通过控制各页面以一定顺序循环播放，就达到了轮播的效果。为此，我们可以使用Handler的sendEmptyMessageDelayed()方法来实现定时更新，并
注意用户也可能会对带有轮播效果的ViewPager手动进行滑动操作，因此我认为用户这时候是希望查看指定页面的，这时候应该取消轮播。下面是这个Handler的实现：

```python
private static class ImageHandler extends Handler{
         
        /**
         * 请求更新显示的View。
         */
        protected static final int MSG_UPDATE_IMAGE  = 1;
        /**
         * 请求暂停轮播。
         */
        protected static final int MSG_KEEP_SILENT   = 2;
        /**
         * 请求恢复轮播。
         */
        protected static final int MSG_BREAK_SILENT  = 3;
        /**
         * 记录最新的页号，当用户手动滑动时需要记录新页号，否则会使轮播的页面出错。
         * 例如当前如果在第一页，本来准备播放的是第二页，而这时候用户滑动到了末页，
         * 则应该播放的是第一页，如果继续按照原来的第二页播放，则逻辑上有问题。
         */
        protected static final int MSG_PAGE_CHANGED  = 4;
         
        //轮播间隔时间
        protected static final long MSG_DELAY = 3000;
         
        //使用弱引用避免Handler泄露.这里的泛型参数可以不是Activity，也可以是Fragment等
        private WeakReference<MainActivity> weakReference;
        private int currentItem = 0;
         
        protected ImageHandler(WeakReference<MainActivity> wk){
            weakReference = wk;
        }
         
        @Override
        public void handleMessage(Message msg) {
            super.handleMessage(msg);
            Log.d(LOG_TAG, "receive message " + msg.what);
            MainActivity activity = weakReference.get();
            if (activity==null){
                //Activity已经回收，无需再处理UI了
                return ;
            }
            //检查消息队列并移除未发送的消息，这主要是避免在复杂环境下消息出现重复等问题。
            if (activity.handler.hasMessages(MSG_UPDATE_IMAGE)){
                activity.handler.removeMessages(MSG_UPDATE_IMAGE);
            }
            switch (msg.what) {
            case MSG_UPDATE_IMAGE:
                currentItem++;
                activity.viewPager.setCurrentItem(currentItem);
                //准备下次播放
                activity.handler.sendEmptyMessageDelayed(MSG_UPDATE_IMAGE, MSG_DELAY);
                break;
            case MSG_KEEP_SILENT:
                //只要不发送消息就暂停了
                break;
            case MSG_BREAK_SILENT:
                activity.handler.sendEmptyMessageDelayed(MSG_UPDATE_IMAGE, MSG_DELAY);
                break;
            case MSG_PAGE_CHANGED:
                //记录当前的页号，避免播放的时候页面显示不正确。
                currentItem = msg.arg1;
                break;
            default:
                break;
            } 
        }
    }
```


集成代码：MainActivity
下面是MainActivity的代码，主要是加载View和对ViewPager进行初始化设置。因为代码量比较少，重要的部分已经加了注释，就不赘述了

```python
public class MainActivity extends Activity {
     
    private static final String LOG_TAG = "MainActivity";
    private ImageHandler handler = new ImageHandler(new WeakReference<MainActivity>(this));
    private ViewPager viewPager;
 
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        //初始化iewPager的内容
        viewPager = (ViewPager) findViewById(R.id.main_viewpager);
        LayoutInflater inflater = LayoutInflater.from(this);
        ImageView view1 = (ImageView) inflater.inflate(R.layout.item, null);
        ImageView view2 = (ImageView) inflater.inflate(R.layout.item, null);
        ImageView view3 = (ImageView) inflater.inflate(R.layout.item, null);
        view1.setImageResource(R.drawable.ics);
        view2.setImageResource(R.drawable.jellybean);
        view3.setImageResource(R.drawable.kitkat);
        ArrayList<ImageView> views = new ArrayList<ImageView>();
        views.add(view1);
        views.add(view2);
        views.add(view3);
        viewPager.setAdapter(new ImageAdapter(views));
        viewPager.setOnPageChangeListener(new ViewPager.OnPageChangeListener() {
             
            //配合Adapter的currentItem字段进行设置。
            @Override
            public void onPageSelected(int arg0) {
                handler.sendMessage(Message.obtain(handler, ImageHandler.MSG_PAGE_CHANGED, arg0, 0));
            }
             
            @Override
            public void onPageScrolled(int arg0, float arg1, int arg2) {
            }
             
            //覆写该方法实现轮播效果的暂停和恢复
            @Override
            public void onPageScrollStateChanged(int arg0) {
                switch (arg0) {
                case ViewPager.SCROLL_STATE_DRAGGING:
                    handler.sendEmptyMessage(ImageHandler.MSG_KEEP_SILENT);
                    break;
                case ViewPager.SCROLL_STATE_IDLE:
                    handler.sendEmptyMessageDelayed(ImageHandler.MSG_UPDATE_IMAGE, ImageHandler.MSG_DELAY);
                    break;
                default:
                    break;
                }
            }
        });
        viewPager.setCurrentItem(Integer.MAX_VALUE/2);//默认在中间，使用户看不到边界
        //开始轮播效果
        handler.sendEmptyMessageDelayed(ImageHandler.MSG_UPDATE_IMAGE, ImageHandler.MSG_DELAY);
    }//end of onCreate
}//end of MainActivity
```


![](http://images.cnitblog.com/blog/437282/201411/041405283617635.gif)
欢迎大家访问我的个人网站[萌萌的IT人](http://www.itmmd.com)

