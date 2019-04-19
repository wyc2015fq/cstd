# 使android桌面图标变大 - xqhrs232的专栏 - CSDN博客
2012年03月23日 09:52:36[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1309
原文地址::[http://blog.csdn.net/wzcqr0501/article/details/7301227](http://blog.csdn.net/wzcqr0501/article/details/7301227)
在平板上android系统默认的桌面图标太小了，需要将其放大。之前在launcher中做了简单的图片放大，带
来了图标模糊的问题，重新研究源码寻求解决办法。
（1）解决思路是先找到应用程序的图标等信息是怎么获取的。这就需要学习PackageManager的相关源码。
PackageManger类包含以下功能：
安装，卸载应用
查询permission相干信息
查询Application相干信息（application，activity，receiver，service，provider及响应属性等）从
AndroidManifest.xml获取的这些的信息
查询已安装应用
增长，删除permission
清除用户数据、缓存，代码段
可以通过getPackageManager()方法获得。
此类是一个抽象类。它的实现过程可以参考http://blog.csdn.net/ljsbuct/article/details/6636433中的分析。
PackageItemInfo类
说明： AndroidManifest.xml文件中所有节点的基类，提供了这些节点的基本信息：a label、icon、 meta-data。
常用字段：
public int icon  获得该资源图片在R文件中的值 (对应于android:icon属性)
public int labelRes 获得该label在R文件中的值(对应于android:label属性)
public String name   获得该节点的name值 (对应于android:name属性)
public String packagename   获得该应用程序的包名 (对应于android：packagename属性)
常用方法：
Drawable  loadIcon(PackageManager pm) 获得当前应用程序的图像
CharSequence  loadLabel(PackageManager pm)     获得当前应用程序的label
PackageItemInfo类中的这些方法时利用PackageManager中的方法实现的。
PackageItemInfo类并不直接使用，而是由子类继承然后调用相应方法。例如ActivityInfo类、ServiceInfo类和ApplicationInfo类等。
（2）我们来具体看下Launcher中时如何获取应用程序icon的：
在IconCache.java中的
private CacheEntry cacheLocked(ComponentName componentName, ResolveInfo info) {
       .....
            entry.icon = Utilities.createIconBitmap(
info.activityInfo.loadIcon(mPackageManager), mContext);
        }
也就是调用ActivityInfo类的loadIcon来获取drawable型的icon信息。
接着我们看Utilities中的createIconBitmap方法，其中有
if (sourceWidth > 0 && sourceWidth > 0) {
                // There are intrinsic sizes.
                if (width < sourceWidth || height < sourceHeight) {
                    // It's too big, scale it down.
                    final float ratio = (float) sourceWidth / sourceHeight;
                    if (sourceWidth > sourceHeight) {
                        height = (int) (width / ratio);
                    } else if (sourceHeight > sourceWidth) {
                        width = (int) (height * ratio);
                    }
                } else if (sourceWidth < width && sourceHeight < height) {
                    // It's small, use the size they gave us.
                    width = sourceWidth;
                    height = sourceHeight;
                }
            }
这段代码对icon大小处理，如果icon是固有尺寸大于我们定义的尺寸就进行裁剪，如果小于我们定义的尺寸就采用其固有尺寸。
而尺寸是在这里定义的：
sIconWidth = sIconHeight = (int) resources.getDimension(android.R.dimen.app_icon_size);
定义这个app_icon_size是在\frameworks\base\core\res\res\values中的dimens.xml中。
<!-- The standard size (both width and height) of an application icon that will be displayed in the app launcher and elsewhere. -- >
    <dimen name="app_icon_size">48dip</dimen>
（3）接下来我们在launcher源码中自己定义一个app_icon_size为72dip，编译运行应用程序列表中的图标果然变大了，但是挡住了下面的文字。
在layout/aplication_boxed.xml中把android:layout_height从88dip改成108dip。再次运行可以看到应用程序列表大图标效果是不错的。
