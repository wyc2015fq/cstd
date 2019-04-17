# 《Training：Sending Simple Data to Other Apps》 - 一世豁然的专栏 - CSDN博客





2017年04月21日 16:38:22[一世豁然](https://me.csdn.net/Explorer_day)阅读数：288标签：[android](https://so.csdn.net/so/search/s.do?q=android&t=blog)
个人分类：[Android](https://blog.csdn.net/Explorer_day/article/category/6289318)









注：本人非专业翻译人员，编写次博客只是为了自学使用，如有疑问，请参考官方文档。







构建意图时，您必须指定想要“触发”的意图。 Android定义了几个操作，其中包括ACTION_SEND，您可以猜到，它表示意图将数据从一个活动发送到另一个活动，甚至跨过程边界。 要将数据发送到另一个活动，您需要做的是指定数据及其类型，系统将识别兼容的接收活动并将其显示给用户（如果有多个选项）或立即启动活动（如果只有 一个选项）。 同样，您可以通过在清单中指定它们来宣传活动支持从其他应用程序接收的数据类型。




通过意图发送和接收应用程序之间的数据通常用于内容的社会共享。 意图允许用户使用自己喜欢的应用程序快速，轻松地共享信息。





注意：向ActionBar添加共享操作项目的最佳方法是使用ShareActionProvider，该API可在API级别14中使用。ShareActionProvider在有关Adding an Easy Share Action的课程中讨论。








一、发送文本内容

ACTION_SEND操作最直接和常见的用途是将文本内容从一个活动发送到另一个活动。 例如，内置的浏览器应用程序可以将任何应用程序的当前显示页面的URL与文本共享。 这对于通过电子邮件或社交网络与朋友分享文章或网站非常有用。 以下是实现此类共享的代码：




```java
Intent sendIntent = new Intent();
sendIntent.setAction(Intent.ACTION_SEND);
sendIntent.putExtra(Intent.EXTRA_TEXT, "This is my text to send.");
sendIntent.setType("text/plain");
startActivity(sendIntent);
```



如果安装的应用程序具有与ACTION_SEND和MIME类型text / plain匹配的过滤器，则Android系统将运行它; 如果多个应用程序匹配，系统将显示一个消歧对话框（“选择器”），允许用户选择一个应用程序。





但是，如果调用Intent.createChooser（），传递Intent对象，它将返回一个始终显示选择器的意图版本。 这有一些优点：


1、即使用户先前为此意图选择了默认操作，仍将显示选择器。

2、如果没有应用程序匹配，Android会显示系统消息。

3、您可以为选择器对话框指定标题。




以下是更新的代码：




```java
Intent sendIntent = new Intent();
sendIntent.setAction(Intent.ACTION_SEND);
sendIntent.putExtra(Intent.EXTRA_TEXT, "This is my text to send.");
sendIntent.setType("text/plain");
startActivity(Intent.createChooser(sendIntent, getResources().getText(R.string.send_to)));
```


产生的对话框如下图所示：

![](https://img-blog.csdn.net/20170421162633843?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





或者，您可以为意图设置一些标准附加功能：EXTRA_EMAIL，EXTRA_CC，EXTRA_BCC，EXTRA_SUBJECT。 如果接收的应用程序不是设计为使用它们，则此意图将被忽略。





注意：某些电子邮件应用程序（如Gmail）期待一个String []类似EXTRA_EMAIL和EXTRA_CC的附件，使用putExtra（String，String []）将其添加到您的意图。








二、发送二进制内容

使用ACTION_SEND操作与设置相应的MIME类型并将URI放置到额外命名的EXTRA_STREAM中的数据来共享二进制数据。 这通常用于共享图像，但可用于共享任何类型的二进制内容：




```java
Intent shareIntent = new Intent();
shareIntent.setAction(Intent.ACTION_SEND);
shareIntent.putExtra(Intent.EXTRA_STREAM, uriToImage);
shareIntent.setType("image/jpeg");
startActivity(Intent.createChooser(shareIntent, getResources().getText(R.string.send_to)));
```


请注意以下事项：

1、您可以使用MIME类型的“* / *”，但这只会匹配能够处理通用数据流的活动。

2、接收应用程序需要访问Uri指向的数据的权限。 推荐的方法是：

 1）、将数据存储在您自己的ContentProvider中，确保其他应用程序具有访问您的提供商的正确权限。 提供访问的首选机制是使用临时性的per-URI权限，并仅授予对接收应用程序的访问权限。 创建ContentProvider的简单方法就是使用FileProvider帮助器类。

 2）、使用系统MediaStore。 MediaStore主要针对视频，音频和图像MIME类型，但是从Android 3.0（API 11级）开始，它也可以存储非媒体类型（有关更多信息，请参阅MediaStore.Files）。 可以使用scanFile（）将文件插入到MediaStore中，之后将适合共享的content：// style Uri传递给提供的onScanCompleted（）回调。 请注意，一旦添加到系统MediaStore，内容可以访问设备上的任何应用程序。







三、发送多个内容

要共享多个内容，请使用ACTION_SEND_MULTIPLE操作以及指向内容的URI列表。 MIME类型根据您分享的内容的组合而有所不同。 例如，如果您共享3张JPEG图像，该类型仍然是“image / jpeg”。 对于图像类型的混合，应该是“image / *”来匹配处理任何类型图像的活动。 如果您分享了各种各样的类型，您应该只使用“* / *”。 如前所述，由接收应用程序来解析和处理您的数据。 以下是一个例子：




```java
ArrayList<Uri> imageUris = new ArrayList<Uri>();
imageUris.add(imageUri1); // Add your image URIs here
imageUris.add(imageUri2);

Intent shareIntent = new Intent();
shareIntent.setAction(Intent.ACTION_SEND_MULTIPLE);
shareIntent.putParcelableArrayListExtra(Intent.EXTRA_STREAM, imageUris);
shareIntent.setType("image/*");
startActivity(Intent.createChooser(shareIntent, "Share images to.."));
```


如前所述，请确保提供的URI指向接收应用程序可以访问的数据。



