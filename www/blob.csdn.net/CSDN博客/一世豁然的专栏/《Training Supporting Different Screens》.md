# 《Training:Supporting Different Screens》 - 一世豁然的专栏 - CSDN博客





2017年04月14日 20:05:13[一世豁然](https://me.csdn.net/Explorer_day)阅读数：210








注意：本人非专业翻译人员，写此博客只是为了自学使用，如有疑问，请参考官方文档




Android使用两种常规属性对设备屏幕进行分类：大小和密度。您应该期望您的app将安装在具有尺寸和密度范围的设备屏幕上。因此，对适应不同的屏幕尺寸和密度，您应该添加一些可以优化您app的外观的可选择资源。


I、有四种广义尺寸：小，正常，大，大。

II、和四个广义密度：低（ldpi），中（mdpi），高（hdpi），超高（xhdpi）。




要声明用于不同屏幕的不同布局和位图，您必须将这些替代资源放在不同的目录中，与对不同语言字符串的操作类似。





还要注意，屏幕方向（横向或纵向）被认为是屏幕尺寸的变化，所以许多app应该修改布局以优化每个方向的用户体验。








Create Different Layouts


要优化不同屏幕尺寸的用户体验，您应该为要支持的每个屏幕尺寸创建一个独特的布局XML文件。 每个布局应该保存到适当的资源目录中，命名为 - <screen_size>后缀。 例如，大屏幕的独特布局应该保存在res / layout-large /下。


注意：Android会自动缩放您的布局，以正确适合屏幕。 因此，不同屏幕尺寸的布局不需要担心UI元素的绝对大小，而是重点关注影响用户体验的布局结构（例如相对于兄弟视图的重要视图的大小或位置）。




例如，该项目包括大屏幕的默认布局和替代布局：




```
MyProject/
    res/
        layout/
            main.xml
        layout-large/
            main.xml
```






文件名必须完全相同，但内容不同，以便为相应的屏幕尺寸提供优化的UI。



只需像往常一样在应用中引用布局文件：



```java
@Override
 protected void onCreate(Bundle savedInstanceState) {
     super.onCreate(savedInstanceState);
     setContentView(R.layout.main);
}
```






系统根据您应用程序运行的设备屏幕尺寸，从相应的布局目录加载布局文件。 有关Android选择适当资源的更多信息，请参阅Providing Resources指南。




另一个例子，这是一个具有横向导向的替代布局的项目：




```
MyProject/
    res/
        layout/
            main.xml
        layout-land/
            main.xml
```






默认情况下，layout / main.xml文件用于纵向。




如果你想为景观提供一个特殊的布局，包括在大屏幕上，那么你需要同时使用大型和地面限定词：



```
MyProject/
    res/
        layout/              # default (portrait)
            main.xml
        layout-land/         # landscape
            main.xml
        layout-large/        # large (portrait)
            main.xml
        layout-large-land/   # large landscape
            main.xml
```






注意：Android 3.2及以上版本支持一种高级方法来定义屏幕尺寸，允许您根据密度无关像素的最小宽度和高度来指定屏幕尺寸的资源。 本课不涵盖这一新技术。 有关详细信息，请参阅Designing for Multiple Screens







 Create Different Bitmaps

您应该始终提供适当缩放到每个广义密度桶的位图资源：低，中，高和超高密度。 这有助于您在所有屏幕密度上实现良好的图形质量和性能。




要生成这些图像，您应该以矢量格式从原始资源开始，并使用以下尺寸比例为每个浓度生成图像：


I、xhdpi: 2.0

II、hdpi: 1.5

III、mdpi：1.0（基线）

IV、ldpi: 0.75




这意味着如果您为xhdpi设备生成200x200映像，则应为150x150的hdpi，100x100的mdpi，75x75的ldpi设备生成相同的资源。





然后，将文件放在相应的drawable资源目录中：




```
MyProject/
    res/
        drawable-xhdpi/
            awesomeimage.png
        drawable-hdpi/
            awesomeimage.png
        drawable-mdpi/
            awesomeimage.png
        drawable-ldpi/
            awesomeimage.png
```



每当您引用@ drawable / awesomeimage时，系统将根据屏幕的密度选择适当的位图。



注意：低密度（ldpi）资源并不总是必需的。 当您提供hdpi资源时，系统将其缩小一半以适应ldpi屏幕。




有关为您的应用创建图标资源的更多提示和指南，请参阅Iconography design指南。





