# "MPlayer+TextureView" : 封装一个视频播放器的 "SDK" - DEVELOPER - CSDN博客





2018年08月11日 23:07:41[学术袁](https://me.csdn.net/u012827205)阅读数：436
所属专栏：[Android-开发](https://blog.csdn.net/column/details/20398.html)









用过很多的**SDK**，关于友盟的、高德地图的、腾讯的和支付宝的等；在这些功能上的实现会让我们使用起来特别方便，即拿即用，阅读下文档就好！对这种神奇的效果，我一直有着好奇心，最近在这块儿稍微花了下时间和心思，然后这里封装一个**视频播放器的SDK**来诠释下这方面的封装过程和思路。

# 使用类图展示业务关系

![这里写图片描述](https://img-blog.csdn.net/20180812224256294?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

## SDK封装逻辑

**SDK = 视频播放的业务功能 + 外观模式实现业务功能的封装**

### 简单描述

为了用户快速上手使用，以最少的学习成本完成功能上的实现。比如我们使用过的腾讯、阿里和新浪微博的一些分享的或者地图的**SDK**，往往那些复杂的逻辑功能最终呈现给我们开发者的时候，使用类的实例调用对应功能的**api**就搞定了。而实现的原理肯定能够猜得到，即一层一层的封装。最后最为开发者不用关心实现的过程，只需调用功能性的方法就能实现开发需求。

针对**“MPlayer+TextureView”** 实现的一个简单的视频播放的**SDK ：**

上图中 **CustomVideoView** 继承自 **RelativeLayout** 所实现的一个基本的播放器功能的视频播放内核类，只有视频播放、暂停和停止等功能。而其他业务功能则是使用内部的一个接口 **ADVideoPlayerListener** ，**`其作用有  ：`**
`1）`承接**CustomVideoView**中的一些功能，并暴露到外部。从而实现功能上的层层封装，然后在应用层回调。 
`2）`实现业务功能上的扩展，如点击全屏播放、小平播放、播放窗口划入屏幕播放和滑出暂停等功能。其中**VideoAdShell**是对众多业务上的扩展，比如滑入播放、滑出暂停播放功能； 
`3）`业务层**VideoAdShell**也仿照这种方式继续向上封装。
**VideoFullDialog**是对全屏播放和小屏模式的功能扩展。业务功能上从内到外的通信方式使用的是接口的回调。最后使用一个**类<构建者模式创建实例、外观模式封装业务功能>**作为该**SDK**调用视频播放器**api**的入口，通过简单的方法调用来实现复杂的播放逻辑，并结合接口的回调来获得操作上的结果反馈。

## 视频效果展示

![这里写图片描述](https://img-blog.csdn.net/20180825121920299?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**简单逻辑描述：**该页面是一个`RecycleView`列表，其中一个`item`用来展示**视频播放**；视频的item播放窗口滑入屏幕超过自身的50%则开始静音播放，滑出暂停。点击全屏，则进入全屏的有声播放。点击全屏按钮，item的播放视频的View从item的Layout中remove，然后add到全屏视频的dialog的Layout中继续播放；全屏点击关闭按钮或者播放完毕，则将视频播放的view再次从dialog的Layout中remove，将视频的view再次add到列表的item中的Layout继续执行全屏时候的视频状态。视频内核只有播放、暂停等基本视频功能，复杂业务扩展在上层进行封装，最后通过外观模式进行再次的封装成为方便通用的**SDK** 以供使用。
## 代码介绍

通过代码来描述视频播放功能，从内到外，各种业务逻辑实现的封装过程。直至最后的sdk。

### 做一个视频播放，它的生命周期是要牢记的

![这里写图片描述](https://img-blog.csdn.net/20180813092100327?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

### 内核层逻辑

![这里写图片描述](https://img-blog.csdn.net/20180825124119574?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

自定义一个RelativeLayout，作为视频播放”MPlayer+TextureView”的Layout。这样的设计会让整个视频播放组件的使用灵活性大大提高。
#### 设置视频在列表中的播放窗口大小的宽高比是 16:9

![这里写图片描述](https://img-blog.csdn.net/20180825125607638?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

#### 当自定义RelativeLayout被加载之后执行

![这里写图片描述](https://img-blog.csdn.net/2018082512590239?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

上面74、75、76三行代码，目的和作用是为了防止播放页面切换之后出现黑屏问题； 

接下来就进入到了视频播放前的资源加载逻辑。 
![这里写图片描述](https://img-blog.csdn.net/20180825130110149?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

316：表示显示视频播放之前，开始进入加载资源之后的视频加载动画； 

320：设置静音播放； 

321：加载视频资源； 

319：判断、创建MediaPlayer对象，配置mediaPlayer、vvideoSurface(Surface对象) 
![这里写图片描述](https://img-blog.csdn.net/20180825130416750?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
#### 当视频资源异步加载成功之后，回调

![这里写图片描述](https://img-blog.csdn.net/20180825130735105?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

调用resume();方法进行视频播放。
```
public void resume() {
        if (this.playerState != STATE_PAUSING) {
            return;
        }
        LogUtils.d(TAG, "do resume");
        if (!isPlaying()) {
            entryResumeState();
            mediaPlayer.setOnSeekCompleteListener(null);
            mediaPlayer.start();
            mHandler.sendEmptyMessage(TIME_MSG);
            showPauseView(true);
        } else {
            showPauseView(false);
        }
    }
```

一个视频播放器的基本功能当然还由暂停、播放按钮等按钮的响应逻辑； 
![这里写图片描述](https://img-blog.csdn.net/20180825131032467?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

225：播放器状态处于暂停状态，点击按钮之后，进入播放； 

232：否则，重新加载视频资源，并进行重播； 

234：点击进入大屏播放； 

237：点击视频播放窗口调用对应的操作逻辑； 

以上则是播放内核执行的过程及逻辑。作为一个sdk，除了这些，一定还会有更复杂的业务的。那么怎对内核层进行封装扩展业务？
### 内核层业务的封装扩展——视频播放业务层

对内核层进行业务扩展，就是说把一些视频播放的业务逻辑包裹封装在内核层外面。 
![这里写图片描述](https://img-blog.csdn.net/20180825132612284?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

达到一种什么效果呢？**“视频播放业务层”** 能访问到 **“内核层”** 的内容，内核层同样也能访问视频播放业务层的内容。`也就是说，视频播放业务层和内核层两者互相暴露响应的接口给对方。`这个时候使用什么方式实现呢？**接口回调。**
![这里写图片描述](https://img-blog.csdn.net/20180825133920354?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

使用这种方式，而不是以内部类暴露接口的实现方式，这样能够更好的扩展Class A的业务功能。当然回到视频播放的内核层也是一样，也是为了更好的进行业务的扩展来设计的。并且使用接口进行业务分层也是一种很好的解耦和方式。 

好的，进入内核层源码，定义内核层需要进行扩展的业务，使用接口进行扩展。 
![这里写图片描述](https://img-blog.csdn.net/20180825134359425?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

在视频播放业务层即扩展层进行实现， 
![这里写图片描述](https://img-blog.csdn.net/20180825134517979?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

比如其中一个功能上的扩展，当视频播放结束之后的扩展： 

在内核层 
![这里写图片描述](https://img-blog.csdn.net/20180825134816807?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

在业务扩展层——视频播放的业务层 
![这里写图片描述](https://img-blog.csdn.net/20180825134909225?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

两个方法进行对比可得出结论，内核层直接处理视频播放结束之后的逻辑，并使用回调把视频播放结束的信息反馈到上层进行封装并做相应处理。然后在业务扩展层又继续使用接口，将视频播放结束的信息反馈到更上一层，直到最后的sdk封装并暴露接口对外不的应用层做最后的简单处理，以此来实现作为一个sdk要实现的功能。 
![这里写图片描述](https://img-blog.csdn.net/20180825135301612?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

324：视频播放结束之后的回调； 

视频播放业务层，也使用内核层进行扩展的方式，定义接口进行业务的扩展和对外接口的暴露。 
![这里写图片描述](https://img-blog.csdn.net/20180825135749583?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

在sdk的顶层实现业务层释放的接口进行再次封装，然后使用**外观模式**`*并结合`***构建者模式**来实现一个用户寥寥几行代码就能够实现复杂视频播放的业务功能。
### 视频窗口、小屏和大屏切换的逻辑算法

视频窗口滑入屏幕、滑出屏幕，然后根据视频窗口进入屏幕视野百分比计算的算法。

```
public static int getVisiblePercent(View pView) {
        if (pView != null && pView.isShown()) {
            DisplayMetrics displayMetrics = pView.getContext().getResources().getDisplayMetrics();
            int displayWidth = displayMetrics.widthPixels;
            Rect rect = new Rect();
            pView.getGlobalVisibleRect(rect);
            if ((rect.top > 0) && (rect.left < displayWidth)) {
                double areaVisible = rect.width() * rect.height();
                double areaTotal = pView.getWidth() * pView.getHeight();
                return (int) ((areaVisible / areaTotal) * 100);
            } else {
                return -1;
            }
        }
        return -1;
    }
```

在列表中点击视频播放窗口的全屏按钮，逻辑实现在**视频播放业务层**
![这里写图片描述](https://img-blog.csdn.net/2018082514111572?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
169：记录、反馈用户操作给后台； 

175：从列表的Layout中移除自定义的 RelativeLayout （播放视频的View）； 

176：添加播放视频的View到全屏播放的Dialog中，并继续当前的视频播放；
```
public static Bundle getViewProperty(View view) {
        Bundle bundle = new Bundle();
        int[] screenLocation = new int[2];
        view.getLocationOnScreen(screenLocation); //获取view在整个屏幕中的位置
        bundle.putInt(PROPNAME_SCREENLOCATION_LEFT, screenLocation[0]);
        bundle.putInt(PROPNAME_SCREENLOCATION_TOP, screenLocation[1]);
        bundle.putInt(PROPNAME_WIDTH, view.getWidth());
        bundle.putInt(PROPNAME_HEIGHT, view.getHeight());

        Log.e("Utils", "Left: " + screenLocation[0] + " Top: " + screenLocation[1]
                + " Width: " + view.getWidth() + " Height: " + view.getHeight());
        return bundle;
    }
```

而当视频播放完成或者点击关闭按钮的话，会回调上面185行代码，的下面方法： 
![这里写图片描述](https://img-blog.csdn.net/20180825142217508?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

视频播放完成或者点击关闭按钮，dialog会执行dismiss方法，
```java
@Override
    public void dismiss() {
        LogUtils.e(TAG, "dismiss");
        mParentView.removeView(mVideoView);
        super.dismiss();
    }
```

dialog中remove掉，列表Layout中add。从而实现视频的正常大小屏的切换和播放；













