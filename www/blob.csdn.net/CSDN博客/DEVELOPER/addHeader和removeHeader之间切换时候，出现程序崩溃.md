# addHeader和removeHeader之间切换时候，出现程序崩溃 - DEVELOPER - CSDN博客





2014年11月21日 10:10:11[学术袁](https://me.csdn.net/u012827205)阅读数：1201标签：[addHeader和removeHead																[出现程序崩溃](https://so.csdn.net/so/search/s.do?q=出现程序崩溃&t=blog)](https://so.csdn.net/so/search/s.do?q=addHeader和removeHead&t=blog)
个人分类：[出现程序崩溃																[addHeader和removeHeader之间切换时候](https://blog.csdn.net/u012827205/article/category/2728817)](https://blog.csdn.net/u012827205/article/category/2728819)







![](https://img-blog.csdn.net/20141121095101515?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

![](https://img-blog.csdn.net/20141121095508406?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





































在做项目的时候，遇到了一个问题。问题过程我在此叙述一遍：在未登录的时候，在好友动态的页面（整个动态的列表使用的是listview）会显示一个头部布局用于提示用于进行登陆；当用户登陆过后，该头部布局则会消失。

然后，在我进行调试的时候，出现了崩溃现象！我是这样进行调试的：首先进入该页面，点击登陆按钮进行登陆。登陆之后，到注销账号页面注销登陆账号。然后再回到这个好友动态页面。然后，问题出现了！报："Cannot add header view to list -- setAdapter has already been called . " 那该怎么解决这个问题呢？先了解一下listview的setadapter源码是怎么运行的。

```java
public void addHeaderView(View v, Object data, boolean isSelectable) {
 
  if (mAdapter != null && ! (mAdapter instanceof HeaderViewListAdapter)) {
      throw new IllegalStateException(
        "Cannot add header view to list -- setAdapter has already been called.");
  }
 
  FixedViewInfo info = new FixedViewInfo();
  info.view = v;
  info.data = data;
  info.isSelectable = isSelectable;
  mHeaderViewInfos.add(info);
 
  // in the case of re-adding a header view, or adding one later on,
  // we need to notify the observer
  if (mAdapter != null && mDataSetObserver != null) {
      mDataSetObserver.onChanged();
  }
}
```
有上面的代码，我们大致可以理解。当listview已经setadapter之后（或者之前已经setadapter，第二次操作addheader），再给listview  addheader的时候。根据上面的源码，mAdapter ！= null  了已经，因此会报错！所有，我们可以重置mAdapter对象。在listview调用addheader方法之前我们就先使用“listView.setAdapter(null);”之后对listview添加头部就行了。我的代码是这样写的：
```java
<pre name="code" class="java">@Override
	public void onStart() {
		super.onStart();
		listView.setAdapter(null);//解决冲突bug
		if(TextUtils.isEmpty(SharedPreferenceMemoryUtil.getToken(getActivity()))){
			if(!hasHeader){
				listView.addHeaderView(headView);
			}
			hasHeader = true;
		}else{
			if(hasHeader){
				listView.removeHeaderView(headView);
				hasHeader = false;
			}
		}
		requestMethod();
		initBroadCastMethod();
	}
```


[然后，链接一下类似问题朋友的解决方法！](http://www.cnblogs.com/nuliniaoboke/archive/2012/10/25/2738965.html)









