# Android 7.0以上版本，如何实现应用内语言切换 - weixin_33985507的博客 - CSDN博客
2018年02月05日 00:29:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：9
### 前言
项目里有个功能，在应用内切换语言，之前上线了大半年，一直都是正常运行，但是最近这次发版以后，在国外的同事反馈（这里面还包括CEO... 尴尬），他们的华为手机上出现了中英文混乱的问题。本文记录了解决问题的过程和方法。
### 正文
首先这部分功能的代码是之前已经离职的同事留下的，一直跑得好好的，我也没去动过，发版前用别的手机测试也没有出现问题，怎么突然就出现中英文混乱了呢？难道是华为的锅？国外同事的手机没法拿来调试，听说他们也都是在国内买了带到国外去的，后来我也在身边找了几个华为手机试了一下，并没有出现问题，我陷入了沉思...
整理了一下线索：
- 国外同事的华为手机出现问题，我身边朋友的华为手机没有问题
- 这部分代码之前一直没有问题，现在突然出现了问题
- 这期间我也看了一下相关代码，确实没有修改过。但是！我发现了一个疑点：代码中使用了一些废弃的方法和属性
![2979284-d7afec05fd37ee7a.png](https://upload-images.jianshu.io/upload_images/2979284-d7afec05fd37ee7a.png)
难道是？
我赶紧去问国外的同事，系统版本是多少，果然不出所料，国外同事的系统版本已经更新到了最新的8.0版本。这下一切都说得通了，华为是国内最早升级8.0系统的厂商，升级的策略一般是先从旗舰机开始，逐步向下辐射，而当下正值部分高端机型已经升级到了8.0系统，而中低端机型还没有开始推送的时间节点。很有可能问题就出在那几个废弃的方法和属性上。
在网上查找了相关资料后，证实了我的猜想。下图是我之前使用的方法：
![2979284-1468a704e5ebf730.png](https://upload-images.jianshu.io/upload_images/2979284-1468a704e5ebf730.png)
废弃的方法和属性
可以看到 configuration.locale 属性和 updateConfiguration 方法都已经被打上了废弃的标记。7.0系统推出后告诉开发者方法已经过时，到了8.0系统上这段代码就已经完全不好使了，所以才出现了前面提到的问题。
那么为什么Google会废弃这段代码呢？其实原因很明显，在7.0系统以后，我们的系统语言选择已经不再是设置一种语言，而是可以设置一组语言：
![2979284-2d74b3af6e66b8ef.png](https://upload-images.jianshu.io/upload_images/2979284-2d74b3af6e66b8ef.png)
7.0版本后的语言设置界面
Google把语言这一块进行了较大的调整，系统会根据这个列表的优先级去适配语言。机制都变了，以前的代码当然会废弃掉了。
既然我们已经找到了问题的原因，那么如何去解决呢？
首先，我们需要实现一个自己的MyContextWrapper，它会根据不同的系统版本采用相应方法来设置 Locale 或者 LocaleList ，并且返回一个ContextWrapper：
```
public class MyContextWrapper extends android.content.ContextWrapper {
    public MyContextWrapper(Context base) {
        super(base);
    }
    public static ContextWrapper wrap(Context context, Locale newLocale) {
        Resources res = context.getResources();
        Configuration configuration = res.getConfiguration();
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.N) {
            configuration.setLocale(newLocale);
            LocaleList localeList = new LocaleList(newLocale);
            LocaleList.setDefault(localeList);
            configuration.setLocales(localeList);
            context = context.createConfigurationContext(configuration);
        } else if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.KITKAT) {
            configuration.setLocale(newLocale);
            context = context.createConfigurationContext(configuration);
        }
        return new ContextWrapper(context);
    }
}
```
接下来，我们只需要在我们的 BaseActivity 里重写 attachBaseContext() 方法即可，代码如下：
```
@Override
    protected void attachBaseContext(Context newBase) {
        Locale newLocale;
        // .. create or get your new Locale object here.
        Context context = MyContextWrapper.wrap(newBase, newLocale);
        super.attachBaseContext(context);
    }
```
大功告成，这样在7.0及以上系统版本就不会出问题了。
### 结语
说实话，这还是我第一次遇到废弃的方法就真的不好使了。以前参与过的项目里或多或少都有一些过时的属性、方法，但是它们都顽强地肩负着自己的使命，看起来好像永远不会真的被废弃...
经过这一次，让我吸取了教训，废弃的方法和属性最好尽早替换掉，它虽然现在没有问题，但是保不齐哪天就不好使了。
参考资料：
- 
[https://stackoverflow.com/questions/39705739/android-n-change-language-programmatically?noredirect=1&lq=1](https://link.jianshu.com?t=https%3A%2F%2Fstackoverflow.com%2Fquestions%2F39705739%2Fandroid-n-change-language-programmatically%3Fnoredirect%3D1%26lq%3D1)
- 
[http://likfe.com/2017/05/10/android-sys-language/](https://link.jianshu.com?t=http%3A%2F%2Flikfe.com%2F2017%2F05%2F10%2Fandroid-sys-language%2F)
