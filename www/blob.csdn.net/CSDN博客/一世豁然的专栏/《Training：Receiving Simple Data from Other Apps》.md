# 《Training：Receiving Simple Data from Other Apps》 - 一世豁然的专栏 - CSDN博客





2017年04月21日 16:52:01[一世豁然](https://me.csdn.net/Explorer_day)阅读数：264标签：[android](https://so.csdn.net/so/search/s.do?q=android&t=blog)
个人分类：[Android](https://blog.csdn.net/Explorer_day/article/category/6289318)









注：本人非专业翻译人员，编写此博客只是为了自学使用，如有疑问，请参考官方文档。







正如您的应用程序可以将数据发送到其他应用程序一样，也可以轻松地从应用程序接收数据。 考虑用户如何与应用程序进行交互，以及要从其他应用程序接收的数据类型。 例如，社交网络应用程序可能有兴趣从另一个应用程序接收文本内容，如有趣的网址。 Google+ Android应用程式同时接受文字和单张或多张图片。 使用此应用，用户可以轻松地从Android Gallery应用程序的照片开始新的Google+信息。








一、更新你的清单文件

意图过滤器通知系统应用程序组件愿意接受哪些意图。 类似于在“将简单数据发送到其他应用程序”课程中构建动作ACTION_SEND的意图，您可以创建意图过滤器，以便能够接收到此操作的意图。 您可以使用<intent-filter>元素在清单中定义一个意图过滤器。 例如，如果您的应用程序处理接收文本内容，任何类型的单个图像或任何类型的多个图像，则清单将如下所示：




```
<activity android:name=".ui.MyActivity" >
    <intent-filter>
        <action android:name="android.intent.action.SEND" />
        <category android:name="android.intent.category.DEFAULT" />
        <data android:mimeType="image/*" />
    </intent-filter>
    <intent-filter>
        <action android:name="android.intent.action.SEND" />
        <category android:name="android.intent.category.DEFAULT" />
        <data android:mimeType="text/plain" />
    </intent-filter>
    <intent-filter>
        <action android:name="android.intent.action.SEND_MULTIPLE" />
        <category android:name="android.intent.category.DEFAULT" />
        <data android:mimeType="image/*" />
    </intent-filter>
</activity>
```


注意：有关意图过滤器和意图决议的更多信息，请阅读意图和意图过滤器




当另一个应用程序尝试通过构建一个意图并将其传递给startActivity（）来分享这些东西时，您的应用程序将被列为intent选择器中的一个选项。 如果用户选择您的应用程序，则将启动相应的活动（上述示例中的.ui.MyActivity）。 在你的代码和UI中适当地处理内容是由你决定的。








二、处理传入的内容

要处理Intent提供的内容，首先调用getIntent（）获取Intent对象。 一旦你有了对象，你可以检查它的内容，以确定下一步做什么。 请记住，如果可以从系统的其他部分（例如启动器）启动此活动，则在检查意图时需要考虑这一点。




```java
void onCreate (Bundle savedInstanceState) {
    ...
    // Get intent, action and MIME type
    Intent intent = getIntent();
    String action = intent.getAction();
    String type = intent.getType();

    if (Intent.ACTION_SEND.equals(action) && type != null) {
        if ("text/plain".equals(type)) {
            handleSendText(intent); // Handle text being sent
        } else if (type.startsWith("image/")) {
            handleSendImage(intent); // Handle single image being sent
        }
    } else if (Intent.ACTION_SEND_MULTIPLE.equals(action) && type != null) {
        if (type.startsWith("image/")) {
            handleSendMultipleImages(intent); // Handle multiple images being sent
        }
    } else {
        // Handle other intents, such as being started from the home screen
    }
    ...
}

void handleSendText(Intent intent) {
    String sharedText = intent.getStringExtra(Intent.EXTRA_TEXT);
    if (sharedText != null) {
        // Update UI to reflect text being shared
    }
}

void handleSendImage(Intent intent) {
    Uri imageUri = (Uri) intent.getParcelableExtra(Intent.EXTRA_STREAM);
    if (imageUri != null) {
        // Update UI to reflect image being shared
    }
}

void handleSendMultipleImages(Intent intent) {
    ArrayList<Uri> imageUris = intent.getParcelableArrayListExtra(Intent.EXTRA_STREAM);
    if (imageUris != null) {
        // Update UI to reflect multiple images being shared
    }
}
```


注意：请注意检查传入的数据，您永远不知道其他应用程序可能发送给您什么。 例如，可能设置错误的MIME类型，或者正在发送的映像可能非常大。 另外，请记住在单独的线程而不是主（“UI”）线程中处理二进制数据。




更新UI可以像填充EditText一样简单，或者像图像中应用有趣的照片过滤器一样可以更复杂。 这对您的应用程序接下来会发生什么确实是特定的。




