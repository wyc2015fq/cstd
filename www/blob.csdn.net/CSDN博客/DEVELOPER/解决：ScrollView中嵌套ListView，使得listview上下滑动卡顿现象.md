# 解决：ScrollView中嵌套ListView，使得listview上下滑动卡顿现象 - DEVELOPER - CSDN博客





2014年10月14日 12:01:49[学术袁](https://me.csdn.net/u012827205)阅读数：15617








![](https://img-blog.csdn.net/20141014105945845?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)





效果图就是上面的那样！实现方式：上面是一个ViewPager实现的左右无限循环并带有自动轮播的banner。在banner图的下面是一个listview实现上拉加载的功能；但是在我实现的过程中我遇到了一个问题，就是整个页面的展示效果都已经实现，但是banner图连带下面的listview在上下滑动的时候出现卡顿的现象；当然之前也遇到了一个问题，就是我的banner图和listview一起放在了父控件ScrollView中，是的listview不能完全显示；那么就来解决这两个问题。

解决问题一：<listview置放在ScrollView中不能完全显示>

调用下面这个方法在listview.setAdapter();或者adapter.notifyDataSetChanged();之后，可以让该不能完全显示的listview正确的显示在其父控件ScrollView中。但是要注意的是，对于listview中的每个Item的填充布局，布局的根必须是Linearlayout控件；否则，listview就不能通过onMeasure()正确的计算到每个Item的高度并显示出来，并会抛出异常！要调用方法如下：抛出异常的展示如图：![](https://img-blog.csdn.net/20141014114745111?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

```java
/**
	 * 动态测量listview-Item的高度
	 * @param listView
	 */
	public static void setListViewHeightBasedOnChildren(ListView listView) {  
        ListAdapter listAdapter = listView.getAdapter();   
        if (listAdapter == null) {  
            // pre-condition  
            return;  
        }  
  
        int totalHeight = 0;  
        for (int i = 0; i < listAdapter.getCount(); i++) {  
            View listItem = listAdapter.getView(i, null, listView);  
            listItem.measure(0, 0);  
            totalHeight += listItem.getMeasuredHeight();  
        }  
  
        ViewGroup.LayoutParams params = listView.getLayoutParams();  
        params.height = totalHeight + (listView.getDividerHeight() * (listAdapter.getCount() - 1));  
        listView.setLayoutParams(params);  
    }
```

还有一种解决方式：不再重新计算listview高度，禁止滑动，直接加载显示所有的Item数据：<重写listview，重写onMesure方法>

```java
/**
    * 设置不滚动
    */
    public void onMeasure(int widthMeasureSpec, int heightMeasureSpec)
    {
    int expandSpec = MeasureSpec.makeMeasureSpec(Integer.MAX_VALUE >> 2,
    MeasureSpec.AT_MOST);
    super.onMeasure(widthMeasureSpec, expandSpec);
    }
```




解决问题二：<listview上下滑动卡顿>

只要是listview上下滑动卡顿，其主要原因是因为在listview填充适配器的时候。在重写的类BaseAdapter方法getView();被无限的循环的调用；这样消耗了大量的内存导致listview上下滑动的时候，出现了卡顿。原因就在于measure过程，ListView一般都会根据屏幕加载多个Item，而且也会同时显示这些已经加载过的若干组Item，而这些Item的父元素都是这个ListView。

通过Google的解释：View在Draw的时候分成两个阶段：measure和layout，在measure阶段时主要就是为了计算两个参数：height和width。而且要注意的是，这是个递归的过程，从顶向下，DecorView开始依次调用自己子元素的measure。计算完成这两个参数后就开始layout，最后再是draw的调用。对于ListView，当然每一个Item都会被调用Measure方法，而在这个过程中getView和getCount会被调用，而且看用户的需求，可能会有很多次调用。问题就在于在layout中的决定ListView或者它的父元素的height和width属性的定义了。根据我的经验，height和width最好都设置成fill_parent，这样能避免listview对getView的疯狂调用。解决方法就是尽量避免自适应，除非是万不得已，固定大小或者填充的效果会比较好一些。还有一点请注意，如果使用了上面的动态测量来设置listview的显示高度，并使用了fill_parent来设置了listview的属性，但是仍然不能停止adapter中getView()方法的疯狂调用；或者还伴有listview内容显示仍然不完全现象；根据我自己的实践经验来讲，你可以这样做：把listview放置在LinearLayout中，<LinearLayout中只存在一个listview>;我推断是，当你通过动态测量加载的时候，ta需要一个独立的空间。

如此下来，基本上就解决了，放在ScrollView中时候listview滑动卡顿，显示不完全的现象。




 建一个群，方便大家交流：蹦蹦哒Android <群号：423923313>



