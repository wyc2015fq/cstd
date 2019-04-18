# 《Training：Adding an Easy Share Action》 - 一世豁然的专栏 - CSDN博客





2017年04月21日 17:05:40[一世豁然](https://me.csdn.net/Explorer_day)阅读数：253标签：[android](https://so.csdn.net/so/search/s.do?q=android&t=blog)
个人分类：[Android](https://blog.csdn.net/Explorer_day/article/category/6289318)









注：本人非专业翻译人员，编写此博客只是为了自学使用，如有疑问，请参考官方文档。







在Android 4.0（API第14级）中引入的ActionProvider，使在ActionBar中实现有效和用户友好的共享操作变得更加容易。 ActionProvider一旦附加到操作栏中的菜单项，就处理该项目的外观和行为。 （但是）在ShareActionProvider的情况下，您提供一个共享意图，其就处理剩下的。





注意：从API Level 14及更高版本开始可以使用ShareActionProvider。








一、更新菜单声明

要开始使用ShareActionProviders，请在菜单资源文件中定义相应的<item>的android：actionProviderClass属性。




```
<menu xmlns:android="http://schemas.android.com/apk/res/android">
    <item
            android:id="@+id/menu_item_share"
            android:showAsAction="ifRoom"
            android:title="Share"
            android:actionProviderClass=
                "android.widget.ShareActionProvider" />
    ...
</menu>
```


将该项目的外观和功能的责任委托给ShareActionProvider。 但是，您需要告诉提供者您想要分享的内容。







二、设置分享意图

为了使ShareActionProvider运行，您必须提供一个共享意图。 此共享意图应与“发送简单数据到其他应用程序”课程中描述的相同，其中包含动作ACTION_SEND和通过EXTRA_TEXT和EXTRA_STREAM等附加设置的附加数据。 要分配一个共享意图，首先找到相应的MenuItem，同时增加你的Activity或Fragment中的菜单资源。 接下来，调用MenuItem.getActionProvider（）来检索ShareActionProvider的一个实例。 使用setShareIntent（）更新与该操作项相关联的共享意图。
 以下是一个例子：




```java
private ShareActionProvider mShareActionProvider;
...

@Override
public boolean onCreateOptionsMenu(Menu menu) {
    // Inflate menu resource file.
    getMenuInflater().inflate(R.menu.share_menu, menu);

    // Locate MenuItem with ShareActionProvider
    MenuItem item = menu.findItem(R.id.menu_item_share);

    // Fetch and store ShareActionProvider
    mShareActionProvider = (ShareActionProvider) item.getActionProvider();

    // Return true to display menu
    return true;
}

// Call to update the share intent
private void setShareIntent(Intent shareIntent) {
    if (mShareActionProvider != null) {
        mShareActionProvider.setShareIntent(shareIntent);
    }
}
```


您可能只需要在创建菜单时设置一次分享意图，或者您可能需要设置它，然后在UI更改时进行更新。 例如，当您在“图库”应用程序中全屏查看照片时，分享意图将在照片之间切换。



