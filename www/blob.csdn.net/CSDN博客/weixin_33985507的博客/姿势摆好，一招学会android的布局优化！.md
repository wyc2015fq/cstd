# 姿势摆好，一招学会android的布局优化！ - weixin_33985507的博客 - CSDN博客
2018年05月29日 01:08:38[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：4
作为android应用来讲，无论应用本身多么美观，功能多么强大，内容多么丰富。但如果App本身打开界面缓慢超过手机16ms刷新一次页面的时间，就会产生卡顿。用户体验都会变得极差，导致用户量减少。所以我们在开发过程中同样要注重布局优化。
### 标签
在Layout布局中如果有你想要引用的布局时，若该布局在不同的布局是公共布局，我们会多次使用到。这时可以使用标签。并且便于统一的修改与查看。
```bash
<-- container为引用布局的布局id -->
    <include layout="@layout/container"/>
复制代码
```
非常简单只要在你所需要放置该布局的布局内部使用标签引入该布局就可以了。
在标签当中，我们是可以覆写所有layout属性的，即include中指定的layout属性将会覆盖掉。如我们想修改它的宽高为wrap_content。
```bash
<include  
        android:layout_width="match_parent"  
        android:layout_height="wrap_content"  
        layout="@layout/container" />  
复制代码
```
除了layout_width与layout_height之外，我们还可以覆写container中的任何一个layout属性，如layout_gravity、layout_margin等，而非layout属性则无法在标签当中进行覆写。另外需要注意的是，如果我们想要在标签当中覆写layout属性，必须要将layout_width和layout_height这两个属性也进行覆写，否则覆写效果将不会生效。
### 标签
标签是作为标签的一种辅助扩展来使用的，它的主要作用是为了防止在引用布局文件时产生多余的布局嵌套。Android解析和展示一个布局需要消耗时间，布局嵌套的越多，那么解析起来也就越耗时，性能也就越差，因此我们在编写布局文件时应该让嵌套的层数越少越好。
```bash
<merge  
        android:layout_width="match_parent"  
        android:layout_height="wrap_content" >
        <View  
            android:layout_width="match_parent"  
            android:layout_height="wrap_content"/>
        <View  
            android:layout_width="match_parent"  
            android:layout_height="wrap_content"/>
    </merge>
复制代码
```
大家可以见到标签的使用方法是直接当做该布局的根布局节点使用，而当在其他位置需要引用该布局时，则使用标签进行引用，同时该节点会同步变成父容器的根节点。比如你使用在LinearLayout中则两个view线性排列，而在RelativeLayout中则标签就相当于相对布局标签。这样就可以省略一些不必要的布局嵌套了。
### 标签
标签实际上是一个轻量级的View，它既没有尺寸，也不会绘制任何东西，所以将它放置在布局当中基本可以认为是完全不会影响性能的。只要在需要的时候显示它，才会进行加载。
```bash
<ViewStub
    android:id="@+id/stub"
    android:inflatedId="@+id/container_layout"
    android:layout="@layout/stub_layout"
    android:layout_width=",match_parent"
    android:layout_height="wrap_content"
    android:layout_gravity="bottom" />
复制代码
```
虽然ViewStub是不占用任何空间的，但是每个布局都必须要指定layout_width和layout_height属性，否则运行就会报错。且ViewStub所要替代的layout文件中不能含有标签，所以使用前需要构思好界面布局，以免不必要的嵌套。一旦ViewStub被显示后，则ViewStub将从视图框架中移除，其id也会失效，此时findViewById()得到的也是空的。
ViewStub使用起来非常简单，只要在需要的时候findViewById()招到它并调用setVisibility(View.VISIBLE)或者inflate()显示它就可以了。
#### 标签小结
|标签|使用原因|优化结果|使用举例|
|----|----|----|----|
||提取公共部分，提高布局复用性|减少测量，绘制时间|App中有多个UI界面需要使用同一布局或部分布局时。如页面标题toolBar复用时使用。|
||布局层级减少|减少绘制工作量|当所需要复用的部分布局与要合并到的布局的根标签一致时使用。（类似加强版include，减少布局层级，但耦合性更强）。|
||无需第一时间展示于界面上，在需要时加载|减少测量，绘制时间|该界面不需要第一时间展示给用户，如网络报错界面，或用户信息下拉界面，在该界面中，但第一时间不需要显示给用户时使用。|
### ConstraintLayout约束布局
AndroidStudio上有一个神奇的功能，就是在Xml布局中我们可以在design标签下进行布局可视化操作。但是该功能并不完善，属于系统自动生成的一个布局，反而会让布局内部凌乱不堪，难以读懂，同时会造成卡顿，所以以前我们都是用该界面进行预览查看；而ConstraintLayout约束布局这一新布局，它反而支持布局可视化操作，可以把它比喻成一个可视化视图操作布局的RelativeLayout，ConstraintLayout是使用约束的方式来指定各个控件的位置和关系的。布局内部不需要嵌套其他布局，就可以完成你想要的界面出现。所以它可以有效的避免布局的嵌套，从而达到优化布局的效果。因为使用太过复杂，想要深入了解使用方法请点击[ConstraintLayout](https://link.juejin.im?target=https%3A%2F%2Fblog.csdn.net%2Fguolin_blog%2Farticle%2Fdetails%2F53122387)。
### 减少视图树层级结构
系统在显示没一个视图的时候，都要经理测量，布局，绘制的过程。如果我们的布局嵌套层数太多，会导致额外的测量、布局等，十分消耗系统资源，使UI卡顿，影响用户体验。所以要尽量减少是图书层级结构，避免不必要的布局嵌套，使用更少嵌套的布局方式。
查看文件的视图树，我们可以使用DDMS来查看。首先运行项目在真机或虚拟机上。而后再到tools中打开DDMS。这里就不展开介绍了。
### 其他
- 嵌套的LinearLayout中，尽量不要使用weight，因为weight会重新测量两次。
- Layout的选择，以尽量减少View树的层级为主，去除不必要的嵌套和View节点。比如如果LinearLayout嵌套过多，建议使用RelativeLayout减少布局嵌套。
- RelativeLayout本身尽量不要嵌套使用。
- View视图的隐藏与现实，尽量使用invisible。因为gone,不占用空间，视图会重新测量绘制；而invisible视图不会重新绘制，但仍然占用空间位置。
- 布局调优工具：[hierarchy viewer](https://link.juejin.im?target=https%3A%2F%2Fdeveloper.android.com%2Fstudio%2Fprofile%2Fhierarchy-viewer)，[Lint tool](https://link.juejin.im?target=https%3A%2F%2Fblog.csdn.net%2Fu011240877%2Farticle%2Fdetails%2F54141714)
链接：https://www.jianshu.com/p/faf57bd030ee
阅读更多
[*react-native技术的优劣**](https://link.juejin.im?target=http%3A%2F%2Fmp.weixin.qq.com%2Fs%3F__biz%3DMzI3OTU0MzI4MQ%3D%3D%26amp%3Bmid%3D2247485690%26amp%3Bidx%3D1%26amp%3Bsn%3D44537ca3fcfb5347df3dde1a388cc4dc%26amp%3Bchksm%3Deb476464dc30ed72a0a9f1cabd86375a0a18bd1478e8ca7e17bb7bcc81bc9ebc553b5f24c1f5%26amp%3Bscene%3D21%23wechat_redirect)
[](https://link.juejin.im?target=http%3A%2F%2Fmp.weixin.qq.com%2Fs%3F__biz%3DMzI3OTU0MzI4MQ%3D%3D%26amp%3Bmid%3D2247485690%26amp%3Bidx%3D1%26amp%3Bsn%3D44537ca3fcfb5347df3dde1a388cc4dc%26amp%3Bchksm%3Deb476464dc30ed72a0a9f1cabd86375a0a18bd1478e8ca7e17bb7bcc81bc9ebc553b5f24c1f5%26amp%3Bscene%3D21%23wechat_redirect)[*学习React Native必看的几个开源项目*](https://link.juejin.im?target=http%3A%2F%2Fmp.weixin.qq.com%2Fs%3F__biz%3DMzI3OTU0MzI4MQ%3D%3D%26amp%3Bmid%3D2247485812%26amp%3Bidx%3D1%26amp%3Bsn%3D4214cefd6a686c614eac9a1e56ce7290%26amp%3Bchksm%3Deb4765eadc30ecfca3c965f2ff8792b6d5f4ac017c7a90cf23ca86d2ec7761f6c624259ffcf8%26amp%3Bscene%3D21%23wechat_redirect)
[*开发了几个小程序后，说说我对小程序的看法*](https://link.juejin.im?target=http%3A%2F%2Fmp.weixin.qq.com%2Fs%3F__biz%3DMzI3OTU0MzI4MQ%3D%3D%26amp%3Bmid%3D2247485806%26amp%3Bidx%3D1%26amp%3Bsn%3D80950a0489ef2145b3dd71b35d00cadc%26amp%3Bchksm%3Deb4765f0dc30ece6dd62910400191c689751a5331737abb41e83b728486b4bc2dc4ba7a6e811%26amp%3Bscene%3D21%23wechat_redirect)
[*NDK项目实战—高仿360手机助手之卸载监听*](https://link.juejin.im?target=http%3A%2F%2Fmp.weixin.qq.com%2Fs%3F__biz%3DMzI3OTU0MzI4MQ%3D%3D%26amp%3Bmid%3D2247485690%26amp%3Bidx%3D1%26amp%3Bsn%3D44537ca3fcfb5347df3dde1a388cc4dc%26amp%3Bchksm%3Deb476464dc30ed72a0a9f1cabd86375a0a18bd1478e8ca7e17bb7bcc81bc9ebc553b5f24c1f5%26amp%3Bscene%3D21%23wechat_redirect)
[*（Android）面试题级答案（精选版）*](https://link.juejin.im?target=http%3A%2F%2Fmp.weixin.qq.com%2Fs%3F__biz%3DMzI3OTU0MzI4MQ%3D%3D%26amp%3Bmid%3D2247485690%26amp%3Bidx%3D1%26amp%3Bsn%3D44537ca3fcfb5347df3dde1a388cc4dc%26amp%3Bchksm%3Deb476464dc30ed72a0a9f1cabd86375a0a18bd1478e8ca7e17bb7bcc81bc9ebc553b5f24c1f5%26amp%3Bscene%3D21%23wechat_redirect)
